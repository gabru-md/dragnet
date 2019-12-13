#ifndef AGGR_UTILS_H
#define AAGR_UTILS_H

#include <iostream>
#include <string>
#include <fstream>
#include <chrono>

using namespace std::chrono;

namespace aggr_utils {
    long long DELTA = 200; // in milliseconds
    long long MAX_SIZE = 30;
    long long FORECAST_SIZE = 6;

    enum FEEDS {
        SNIFFER_FEED,
        ANOMALY_FEED,
        SNIFFER_FEED_GROUP_ID,
        ANOMALY_FEED_GROUP_ID,
        PRODUCER_BROKERS
    };

    inline const std::string toString(FEEDS feed) {
        switch(feed) {
        case SNIFFER_FEED :
            return "sniffer_ingestion_feed";
        case ANOMALY_FEED :
            return "anomaly_packet_feed";
        case SNIFFER_FEED_GROUP_ID :
            return "sniffer_feed_group_id";
        case ANOMALY_FEED_GROUP_ID:
            return "anomaly_packet_feed_group_id";
        case PRODUCER_BROKERS :
            return "localhost:9092";
        default :
            return "sniffer_ingestion_feed";
        }
    }

    long long get_timestamp() {
        milliseconds ms = duration_cast< milliseconds > (
                              system_clock::now().time_since_epoch());
        return ms.count();
    }
}

#endif