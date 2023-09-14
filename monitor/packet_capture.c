#include <pcap.h>
#include <stdio.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <time.h>
#include <stdbool.h>

#define BUFSIZE 100000

struct CircularBuffer{
	struct timeval data[BUFSIZE];
	int size;
	int start;
	int end;
} buffer;

void initBuffer(){
	buffer.size = 0;
	buffer.start = 0;
	buffer.end = 0;
}

bool isEmpty(){
	return buffer.size == 0;
}

bool isFull(){
	return buffer.size == BUFSIZE;
}

void push(struct timeval timestamp){
	if(isFull()){
		buffer.start = (buffer.start + 1) % BUFSIZE;
	}

	buffer.data[buffer.end] = timestamp;
	buffer.end = (buffer.end + 1) % BUFSIZE;
	buffer.size++;
}

void pop(){
	if(!isEmpty()){
		buffer.start = (buffer.start + 1) % BUFSIZE;
		buffer.size--;
	}
}

void packet_handler(u_char *extra_user_data, const struct pcap_pkthdr *packet_header, const u_char *packet){
	// ip header starting point by adding 
	// 14 bytes(typical ethernet header length) to packet
	struct ip *ip_header = (struct ip *)(packet + 14);
	// tcp header starting point by adding offset of ip header length
	struct tcphdr *tcp_header = (struct tcphdr *)(packet + 14 + (ip_header->ip_hl << 2));

	// clear buffer by removing old packets
	while(!isEmpty() && packet_header->ts.tv_sec - buffer.data[buffer.start].tv_sec > 1){
		pop();
	}

	// Only analyze TCP-SYN packets
	if((tcp_header->th_flags & TH_SYN) && !(tcp_header->th_flags & TH_ACK)){
		// TODO: check source ip address, check ISP, count number of packets
		char source_ip[INET_ADDRSTRLEN];
		char destination_ip[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(ip_header->ip_src), source_ip, INET_ADDRSTRLEN);
		inet_ntop(AF_INET, &(ip_header->ip_dst), destination_ip, INET_ADDRSTRLEN);
		push(packet_header->ts);
		printf("\nSource IP: %s,\tDest IP: %s\n", source_ip, destination_ip);
	}

	printf("\nTCP-SYN packet in the last second : %d\n", buffer.size);
}

int main(int argc, char *argv[]){
	int ret;
	char *device;
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_if_t *alldevs;
	pcap_if_t *d;
	pcap_t *handle;
	initBuffer();

	ret = pcap_findalldevs(&alldevs, errbuf);
	if(ret == -1){
		fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
		return 2;
	} else if(alldevs == NULL){
		fprintf(stderr, "No device found.\n");
		return 2;
	}

	d = alldevs;
	device = d->name;
	handle = pcap_open_live(device, BUFSIZ, 1, 1000, errbuf);
	if(handle == NULL){
		fprintf(stderr, "Couldn't open device %s: %s\n", device, errbuf);
		return 2;
	}

	pcap_loop(handle , 0, packet_handler, NULL);
	pcap_close(handle);

}
