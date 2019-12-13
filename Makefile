all:
	make aggregator anomaly benchmark netmon

aggregator:
	g++ -I"/usr/local/include/librdkafka" aggregator.cpp -o aggregator_component -lrdkafka++ --std=c++11

anomaly:
	g++ -I"/usr/include/opencv" -I"/usr/include/python2.7" anomaly.cpp -o anomaly_component -ldl -lpthread -lrt -lopencv_core -lpython2.7 -lrdkafka++ --std=c++11

benchmark:
	g++ -I"/usr/include/opencv" -I"/usr/include/python2.7" benchmark.cpp -o benchmark_component -ldl -lpthread -lrt -lopencv_core -lpython2.7 --std=c++11

netmon:
	g++ netmon.cpp -o netmon_component -lpthread -ltins -lrdkafka++ --std=c++11

clean:
	rm anomaly_component
	rm aggregator_component
	rm benchmark_component
	rm netmon_component

log:
	rm datapoints.log
	rm expected.log
	rm observed.log	

# CXX = g++
# LINKFLAG = -ldl -lpthread -lrt -lopencv_core -lpython2.7
# INCFLAGS = -std=c++11 -I"/usr/include/opencv" -I"/usr/include/python2.7"
# CXXFLAGS = -O3 --fast-math -msse2 

# stl:
# 	$(CXX) $(INCFLAGS) stl.cpp -o stl $(LINKFLAG)
# clean:
# 	rm stl
# again:
# 	make clean
# 	make stl


# bin/kafka-topics.sh --create --bootstrap-server localhost:9092 --topic sniffer_ingestion_feed --partitions 1 --replication-factor 1
# bin/kafka-console-producer.sh --broker-list localhost:9092 --topic sniffer_ingestion_feed
# g++ -I"/usr/local/include/librdkafka" aggregator.cpp -o aggregator -lrdkafka++ --std=c++11


# .c.o:
# 	$(CXX) $(INCFLAGS) $< -o $@ -c $(CXXFLAGS)

# .cpp.o:
# 	$(CXX) $(INCFLAGS) $< -o $@ -c $(CXXFLAGS)

# .cc.o:
# 	$(CXX) $(INCFLAGS) $< -o $@ -c $(CXXFLAGS)

# g++ -o l1qc_test l1qc_test.o -ldl -lpthread -lrt -lopencv_core
# g++ -o stl stl.o -ldl -lpthread -lrt -lopencv_core
