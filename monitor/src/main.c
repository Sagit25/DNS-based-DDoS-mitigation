#include <unistd.h>
#include "tcp_syn_monitor.h"

int main(){
	struct circular_buffer buffer;
	init_circular_buffer(&buffer);

	pcap_thread_data data;
	init_pcap_thread(&data, &buffer);

	start_pcap_thread(&data);

	printf("pcap thread started\n");
	printf("press any key to stop\n");
	printf("ISP number:\t");
	for(int i = 0; i < ISP_NUMBER; i++){
		printf("ISP%d\t", i);
	}
	printf("\n");
	while(1){
		sleep(1);
		printf("ISP count:\t");
		for(int i = 0; i < ISP_NUMBER; i++){
			printf("%d\t", buffer.isp_count[i]);
		}
		printf("\n");
	}

	stop_pcap_thread(&data);

	return 0;
}
