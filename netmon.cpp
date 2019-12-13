#include <iostream>
#include <tins/tins.h>
#include <ctime>
#include "include/aggr_utils.h"
#include "include/kafka/raw_producer.h"

using namespace aggr_utils;
using namespace Tins;
const std::string interface = "wlo1";

bool handler(const PDU &pdu) {
	DNS dns = pdu.rfind_pdu<RawPDU>().to<DNS>();
	const IP &ip = pdu.rfind_pdu<IP>();
	std::string deviceIP = ip.src_addr().to_string();
	// std::cout << deviceIP << std::endl;
	produce(
		toString(PRODUCER_BROKERS),
		toString(SNIFFER_FEED),
		deviceIP
		);

	return true;
}

void netmon() {
	SnifferConfiguration config;

	config.set_promisc_mode(true);
	config.set_filter("udp and dst port 53");

	Sniffer sniffer(interface, config);
	sniffer.sniff_loop(handler);
}

int main(void) {
	netmon();
	return 0;
}