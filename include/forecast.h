#ifndef FORECAST_STL_H
#define FORECAST_STL_H

#include "numcpp.h"
#include <cmath>

enum SEVERITY {IGNORE, LOW, MEDIUM, HIGH, HIGHRISK};

double antilog(double value) {
    return static_cast<double>(exp(value));
}

SEVERITY eval_severity(double expected, double observed) {

    if(expected >= observed) return IGNORE;

    double diff = observed - expected;

    double perc_diff = diff / expected;

    if(perc_diff < 2) return LOW;
    else if(perc_diff < 4) return MEDIUM;
    else if(perc_diff < 10) return HIGH;
    else return HIGHRISK;
}

struct Anomaly {
    bool is_anomaly;
    SEVERITY severity;
    double expected_log;
    double observed_log;
    double expected_value;
    double observed_value;

    Anomaly() {
        is_anomaly = false;
        severity = IGNORE;
        expected_value = 0;
        expected_log = 0;
        observed_value = 0;
        observed_log = 0;
    }

    Anomaly(double expectedLog, double observed) {
        expected_log = expectedLog;
        observed_value = observed;

        observed_log = log(observed_value);
        expected_value = antilog(expected_log);

        severity = eval_severity(expected_value, observed_value);

        if(severity == LOW || severity == MEDIUM || severity == IGNORE)
            is_anomaly = false;
        else is_anomaly = true;
    }
};

struct Sample {

    long long max_size;
    long long forecast_size;

    nc::array inputs;
    nc::array season;
    nc::array trends;
    nc::array remainders;

    Sample(long long sz) {
        max_size = sz;
        forecast_size = sz;
    }

    Sample(long long sz, long long fsz) {
        max_size = sz;
        forecast_size = fsz;
    }

    Sample(nc::array inps, nc::array _season,
           nc::array _trends, nc::array _remainders) {
        inputs = inps;
        season = _season;
        trends = _trends;
        remainders = _remainders;

        // assert(inps.size() == season.size() && season.size() == remainders.size() && season.size() == trends.size());

        max_size = season.size();
    }
};

bool insert_to_sample(Sample *sample, double toInsert) {
    try {
        while(sample->inputs.size() > sample->max_size) {
            sample->inputs.erase(sample->inputs.begin());
        }

        sample->inputs.push_back(toInsert);

        return true;
    } catch(int error) {
        return false;
    }
}

struct Anomaly *build_anomaly(Sample *sample, double expected_log, double observed) {
    insert_to_sample(sample, observed);
    return new Anomaly(expected_log, observed);
}

struct Anomaly *predict(Sample *sample, double observed) {

    long long max_size = sample->max_size;
    long long forecast_size = sample->forecast_size;

    long long index_0 = max_size - forecast_size - 1;
    long long index_k = max_size - 1;
    double y1, yk;

    double predicted_seasonal = sample->season[index_0];
    y1 = sample->trends[index_0] + sample->remainders[index_0];
    yk = sample->trends[index_k] + sample->remainders[index_k];

    double expected_log = predicted_seasonal + yk + (yk - y1) / (forecast_size - 1);

    return build_anomaly(sample, expected_log, observed);
}

#endif