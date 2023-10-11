#include <pcap.h>
#include <stdio.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include "circular_buffer.h"
#include "packet_handler.h"

int main(int argc, char *argv[]){
	int ret;
	char *device;
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_if_t *alldevs;
	pcap_if_t *d;
	pcap_t *handle;
	init_circular_buffer();

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
