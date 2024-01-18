#include <unistd.h>
#include "tcp_syn_monitor.h"

void set_puzzle_difficulty(int i, **) {
	syscall() // call set_threshold ()
}

int main(){
	struct circular_buffer buffer;
	init_circular_buffer(&buffer);

	pcap_thread_data data;
	init_pcap_thread(&data, &buffer);

	start_pcap_thread(&data);

	while(1) {
		int total_tcp_syn_packets = buffer->get_circular_buffer_size();
		for(int i = 0; i < ISP_NUM; i++){
			set_puzzle_difficulty(i, buffer->get_isp_tcp_syn_packets(i));
		}
	}

	stop_pcap_thread(&data);

	return 0;
}
