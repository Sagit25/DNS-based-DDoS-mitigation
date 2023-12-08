#include "tcp_syn_monitor.h"

void* pcap_thread(void* arg){
	pcap_thread_data* data = (pcap_thread_data*)arg;
	while(data->keep_running){
		pcap_dispatch(data->handle, -1, packet_handler, (u_char*)data->buffer);
		/*if(pcap_get_selectable_fd(data->handle) != -1 && !data->keep_running){
			pcap_breakloop(data->handle);
		}*/
	}
	pcap_close(data->handle);
	return NULL;
}

void init_pcap_thread_data(pcap_thread_data *data, cb_ptr buffer, const char* device){
	char errbuf[PCAP_ERRBUF_SIZE];
	data->buffer = buffer;
	data->keep_running = true;
	data->handle = pcap_open_live(device, BUFSIZ, 1, 1000, errbuf);
}

void start_pcap_thread(pcap_thread_data *data){
	pthread_create(&data->tid, NULL, pcap_thread, (void*)data);
}

void stop_pcap_thread(pcap_thread_data *data){
	data->keep_running = false;
	pthread_join(data->tid, NULL);
}
