#include <pcap.h>
#include <stdio.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

void packet_handler(u_char *extra_user_data, const struct pcap_pkthdr *packet_header, const u_char *packet){
	// ip header starting point by adding 
	// 14 bytes(typical ethernet header length) to packet
	struct ip *ip_header = (struct ip *)(packet + 14);
	// tcp header starting point by adding offset of ip header length
	struct tcphdr *tcp_header = (struct tcphdr *)(packet + 14 + (ip_header->ip_hl << 2));

	// Only analyze TCP-SYN packets
	if((tcp_header->th_flags & TH_SYN) && !(tcp_header->th_flags & TH_ACK)){
		// TODO: check source ip address, check ISP, count number of packets
	}
}

int main(int argc, char *argv[]){
	char *device;
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t *handle;

	device = pcap_lookupdev(errbuf); // 'pcap_lookupdev' deprecated
	// device = pcap_findalldevs(pcap_if_t **, char *)
	if(device == NULL){
		fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
		return(2);
	}

	handle = pcap_open_live(device, BUFSIZ, 1, 1000, errbuf);
	if(handle == NULL){
		fprintf(stderr, "Couldn't open device %s: %s\n", device, errbuf);
		return(2);
	}

	pcap_loop(handle , 0, packet_handler, NULL);
	pcap_close(handle);

}
