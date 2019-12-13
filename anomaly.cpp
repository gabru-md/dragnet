#include "include/anomaly.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace aggr_utils;
using namespace stl;
using json = nlohmann::json;

Sample * sample = new Sample(MAX_SIZE, FORECAST_SIZE);
bool once = true;

std::string datafile = "datapoints.log";
std::string expected_data = "expected.log";
std::string observed_data = "observed.log";

void write2file(std::string file, std::string data) {
	std::ofstream outfile;
	outfile.open(file, std::ios::app);
	outfile << data << std::endl;
}

void log2files(double expected, double observed) {
	write2file(expected_data, std::to_string(expected));
	write2file(observed_data, std::to_string(observed));
}

void run_anomaly_component() {

	struct inline_anomaly {
		static void run_anomaly_component_on(std::string message) {

			auto aggregated_point = json::parse(message);
			double observed = aggregated_point["counter"];
			write2file(datafile, std::to_string(observed));
			cout << observed << " :: " << sample->inputs.size() << endl;
			if(sample->inputs.size() < MAX_SIZE  && once == true) {
				sample->inputs.emplace_back(observed);
				return;
			}

			once = false;

			nc::tuple_quad result = RobustSTL(sample->inputs, FORECAST_SIZE);

			sample->inputs = result.first;
			sample->trends = result.second;
			sample->season = result.third;
			sample->remainders = result.fourth;

			Anomaly * computed_anomaly = predict(sample, observed);

			std::cout << std::endl << computed_anomaly->expected_value << " :-: " << computed_anomaly->observed_value << std::endl;

			log2files(computed_anomaly->expected_value, computed_anomaly->observed_value);

			if(computed_anomaly->is_anomaly) {
				if(computed_anomaly->severity==HIGHRISK || computed_anomaly->severity==HIGH) {
					cout << "Anomaly has occured system must suspend this node as it has potential of being a zombie in a botnet";
				}
			}
			
		}
	};

	consume(
		toString(ANOMALY_FEED),
		toString(ANOMALY_FEED_GROUP_ID),
		inline_anomaly::run_anomaly_component_on
		);
}

int main(void) {
	run_anomaly_component();
	return 0;
}