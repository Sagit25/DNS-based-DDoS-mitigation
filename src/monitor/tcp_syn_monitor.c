#include "tcp_syn_monitor.h"

void*
pcap_thread(void* arg)
{
	pcap_thread_data* data = (pcap_thread_data*)arg;
	while (data->keep_running) {
		pcap_dispatch(data->handle, -1, packet_handler, (u_char*)data->buffer);
		/*if(pcap_get_selectable_fd(data->handle) != -1 && !data->keep_running){
			pcap_breakloop(data->handle);
		}*/
	}
	pcap_close(data->handle);
	return NULL;
}

int
init_pcap_thread(pcap_thread_data *data, cb_ptr buffer)
{
	int ret = pcap_findalldevs(&data->alldevs, data->errbuf);
	if (ret == -1) {
		fprintf(stderr, "Error in pcap_findalldevs: %s\n", data->errbuf);
		return -1;
	} else if (data->alldevs == NULL) {
		fprintf(stderr, "No device found.\n");
		return -1;
	}
	pcap_if_t *d = data->alldevs;
	data->device = d->name;
	data->buffer = buffer;
	data->keep_running = true;
	data->handle = pcap_open_live(data->device, BUFSIZ, 1, 1000, data->errbuf);
	if (data->handle == NULL) {
		fprintf(stderr, "Error in pcap_open_live, device=%s: %s\n",data->device, data->errbuf);
		return -1;
	}
	return 0;
}

void
start_pcap_thread(pcap_thread_data *data)
{
	pthread_create(&data->tid, NULL, pcap_thread, (void*)data);
}

void
stop_pcap_thread(pcap_thread_data *data)
{
	data->keep_running = false;
	pthread_join(data->tid, NULL);
}
