/* Created by shyang */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "error.h"

#define BUF_SIZE 1024

int main(int argc, char* argv[]) {
    int client_tcp_sock, client_udp_sock;
    char msg[BUF_SIZE];
    int str_len;
    socklen_t dns_adr_sz, host_adr_sz;
    struct sockaddr_in dns_adr, client_udp_adr, client_tcp_adr;

    if (argc != 5) err("Usage : <client name> <tcp ip> <tcp port> <udp ip> <udp port> \n");

    // Create client's TCP socket
    client_udp_sock = socket(PF_INET, SOCK_STREAM, 0); 
    if (client_udp_sock == -1) err("UDP socket creation error");
    memset(&client_tcp_adr, 0, sizeof(client_tcp_adr));
    client_tcp_adr.sin_family = AF_INET;
    client_tcp_adr.sin_addr.s_addr = inet_addr(argv[1]);
    client_tcp_adr.sin_port = htons(atoi(argv[2]));
    if (bind(client_udp_sock, (struct sockaddr*)&client_tcp_adr, sizeof(client_tcp_adr)) == -1) err("bind() error");

    // Create and bind client's UDP socket 
    client_udp_sock = socket(PF_INET, SOCK_DGRAM, 0); 
    if (client_udp_sock != 0) err("UDP socket creation error");
    memset(&client_udp_adr, 0, sizeof(client_udp_adr));
    client_udp_adr.sin_family = AF_INET;
    client_udp_adr.sin_addr.s_addr = inet_addr(argv[3]);
    client_udp_adr.sin_port = htons(atoi(argv[4]));
    if (bind(client_udp_sock, (struct sockaddr*)&client_udp_adr, sizeof(client_udp_adr)) == -1) err("bind() error");


    while (1) {
        dns_adr_sz = sizeof(dns_adr);
        sendto(client_udp_sock, msg, str_len, 0, (struct sockaddr*)&dns_adr, dns_adr_sz);
        str_len = recvfrom(client_udp_sock, msg, BUF_SIZE, 0, (struct sockaddr*)&dns_adr, &dns_adr_sz);
    }

    close(client_udp_sock);
    return 0;
}