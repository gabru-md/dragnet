#include "include/matplotlibcpp.h"
#include <string>
#include <cassert>
#include <fstream>
#include <cmath>

namespace plt = matplotlibcpp;

std::string datafile = "datapoints.log";
std::string expected_fn = "expected.log";
std::string observed_fn = "observed.log";


bool do_benchmark_anomaly() {
	std::ifstream datapoints(datafile);
	std::vector<double> data;
	double max_data = -1.0;

	if(datapoints.is_open()) {
		std::string line;
		while(getline(datapoints, line)) {
			data.push_back(stod(line));
			max_data = std::max(max_data, stod(line));
		}
		datapoints.close();

		plt::plot(data);
		plt::named_plot("requests", data);
		plt::legend();
		plt::ylim(0.0, floor(1.25*max_data));
		plt::show();

		return true;
	}

	return false;
} 

bool do_comparision_benchmarking() {
	std::ifstream expected_file(expected_fn);
	std::ifstream observed_file(observed_fn);

	std::vector<double> expected_points, observed_points;

	double max_data = -1.0;

	if(expected_file.is_open() && observed_file.is_open()) {
		std::string line;
		while(getline(expected_file, line)) {
			expected_points.push_back(stod(line));
			max_data = std::max(max_data, stod(line));
		}

		while(getline(observed_file, line)) {
			observed_points.push_back(stod(line));
			max_data = std::max(max_data, stod(line));
		}

		plt::plot(expected_points);
		plt::plot(observed_points);
		plt::named_plot("expected", expected_points);
		plt::named_plot("observed", observed_points);
		plt::legend();
		plt::ylim(0.0, floor(1.25*max_data));
		plt::show();

		return true;
	}

	return false;
}

void benchmark() {
	assert(do_benchmark_anomaly() == true);
	assert(do_comparision_benchmarking() == true);
}

int main(void) {
	benchmark();
	return 0;
}