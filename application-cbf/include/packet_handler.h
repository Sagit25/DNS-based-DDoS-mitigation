#pragma once
#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H
#include <pcap.h>
#include <stdio.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include "circular_buffer.h"

int determine_isp(char *ip_address);
void packet_handler(u_char *extra_user_data, const struct pcap_pkthdr *packet_header, const u_char *packet);
#endif
