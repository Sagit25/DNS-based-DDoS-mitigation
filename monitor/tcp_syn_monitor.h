#pragma once
#ifndef TCP_SYN_MONITOR_H
#define TCP_SYN_MONITOR_H

#include <pthread.h>
#include <pcap.h>
#include <stdio.h>
#include "circular_buffer.h"
#include "packet_handler.h"

typedef struct {
	pthread_t tid;
	pcap_t *handle;
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_if_t *alldevs;
	char *device;
	volatile bool keep_running;
	cb_ptr buffer;
} pcap_thread_data;

int init_pcap_thread(pcap_thread_data *data, cb_ptr buffer);
void start_pcap_thread(pcap_thread_data *data);
void stop_pcap_thread(pcap_thread_data *data);

#endif
