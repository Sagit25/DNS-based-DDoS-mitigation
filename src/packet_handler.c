#include "circular_buffer.h"
#include "packet_handler.h"

int determine_isp(char *ip_address){
	// TODO: implement mapping between ip address and ISP
	
	// Placeholder function
	int sum = 0;
	char* idx = ip_address;
	while(*idx != '\0'){
		if(*idx >= '0' && *idx <= '9')
			sum = sum + (sum * (*idx - '0')) + (*idx - '0');
		idx++;
	}
	return sum%10; // dummy return value
}

void packet_handler(u_char *extra_user_data, const struct pcap_pkthdr *packet_header, const u_char *packet){
	// ip header starting point by adding 
	// 14 bytes(typical ethernet header length) to packet
	cb_ptr buffer = (cb_ptr)extra_user_data;
	struct ip *ip_header = (struct ip *)(packet + 14);
	// tcp header starting point by adding offset of ip header length
	struct tcphdr *tcp_header = (struct tcphdr *)(packet + 14 + (ip_header->ip_hl << 2));

	// clear buffer by removing old packets
	while(!is_circular_buffer_empty(buffer)){
		long time_diff = packet_header->ts.tv_sec - get_circular_buffer_front(buffer).ts.tv_sec;
		if(time_diff > 1){
			pop_circular_buffer(buffer);
		} else {
			break;
		}
	}

	// Only analyze TCP-SYN packets
	if((tcp_header->th_flags & TH_SYN) && !(tcp_header->th_flags & TH_ACK)){
		char source_ip[INET_ADDRSTRLEN];
		char destination_ip[INET_ADDRSTRLEN];
		struct packet_info new_packet_info;

		inet_ntop(AF_INET, &(ip_header->ip_src), source_ip, INET_ADDRSTRLEN);
		inet_ntop(AF_INET, &(ip_header->ip_dst), destination_ip, INET_ADDRSTRLEN);
		int packet_isp_id = determine_isp(source_ip);

		new_packet_info.ts = packet_header->ts;
		new_packet_info.isp_id = packet_isp_id;

		push_circular_buffer(buffer, new_packet_info);
		// printf("Source IP: %s,\tISP ID: %d,\tDest IP: %s\n", source_ip, packet_isp_id, destination_ip);
	}

	// printf("TCP-SYN packet in the last second : %d\n", get_circular_buffer_size());
	/*
	for(int i = 0; i < ISP_NUMBER; i++){
		printf("%d ", get_circular_buffer_isp_count(buffer, i));
	}
	printf("sum=%d\n", get_circular_buffer_size(buffer));
	*/
}
