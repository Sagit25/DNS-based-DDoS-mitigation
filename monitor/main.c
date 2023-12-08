#include "tcp_syn_monitor.h"

int main(){
	struct circular_buffer buffer;
	init_circular_buffer(&buffer);

	pcap_thread_data data;
	init_pcap_thread_data(&data, &buffer, "eth0");

	start_pcap_thread(&data);

	printf("pcap thread started\n");
	printf("press any key to stop\n");
	scanf("%c", NULL);

	stop_pcap_thread(&data);

	return 0;
}
