#ifndef AGGREGATOR_H
#define AGGREGATOR_H

#include <string>
#include "json.h"
#include "aggr_utils.h"
#include "kafka/raw_consumer.h"
#include "kafka/raw_producer.h"

using json = nlohmann::json;
using namespace aggr_utils;

struct AggregatedPoint {
    long long startTime;
    long long endTime;
    std::string log;
    long long counter;

    AggregatedPoint() {
        startTime = endTime = counter = 0;
    }

    AggregatedPoint(long long st,
                    long long et,
                    long long ct) {
        startTime = st;
        endTime = et;
        counter = ct;
    }
};

struct AggregatorState {

    long long startTime, endTime;
    long long counter;
    struct AggregatedPoint *aggregated_point;

} aggregator_state;

struct AggregatedPoint *build_aggregated_point(long long st,
        long long et,
        long long ct) {
    return new AggregatedPoint(st, et, ct);
}

bool increment_counter(AggregatedPoint *aggregated_point) {
    try {
        aggregated_point->counter++;
        return true;
    } catch(int error) {
        return false;
    }
}

bool increment_counter() {
    try {
        aggregator_state.counter++;
        return true;
    } catch(int error) {
        return false;
    }
}

void reset_counter() {
    aggregator_state.counter = 0;
}

long long get_starttime() {
    return aggregator_state.startTime;
}

void set_starttime(long long st) {
    aggregator_state.startTime = st;
}

long long get_counter() {
    return aggregator_state.counter;
}

std::string toString(AggregatedPoint *aggregated_point) {
    json json_object;
    json_object["startTime"] = aggregated_point->startTime;
    json_object["endTime"] = aggregated_point->endTime;
    json_object["counter"] = aggregated_point->counter;
    return json_object.dump();
}

void run_aggregator() {

    aggregator_state.startTime = aggregator_state.endTime = get_timestamp();
    aggregator_state.counter = 0;

    struct inline_aggregator {

        static void run_aggregator_on(std::string message) {
            long long endTime = get_timestamp();
            
            increment_counter();
            
            if(endTime - get_starttime() > DELTA) {
                struct AggregatedPoint *aggregated_point = build_aggregated_point(
                            get_starttime(),
                            endTime,
                            get_counter());

                reset_counter();
                set_starttime(endTime);

                std::string messageToProduce = toString(aggregated_point);

                produce(
                    toString(PRODUCER_BROKERS),
                    toString(ANOMALY_FEED),
                    messageToProduce
                    );
            }
        }
    };

    consume(
        toString(SNIFFER_FEED),
        toString(SNIFFER_FEED_GROUP_ID),
        inline_aggregator::run_aggregator_on
    );
}


#endif