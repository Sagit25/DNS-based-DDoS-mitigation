#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

#include "error.h"

int main(int argc, char* argv[]) {
    int host_sock;
    char msg[BUF_SIZE];
    char puzzle_data[BUF_SIZE];
    int str_len;
    socklen_t dns_adr_sz;
    struct sockaddr_in dns_adr, host_adr;

    if (argc != 2) {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    host_sock = socket(PF_INET, SOCK_DGRAM, 0); // Host server's UDP socket
    if (host_sock == -1) err("UDP socket creation error");
    memset(&host_adr, 0, sizeof(host_adr));
    host_adr.sin_family = AF_INET;
    host_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    host_adr.sin_port = htons(atoi(argv[1]));
    if (bind(host_sock, (struct sockaddr*)&host_adr, sizeof(host_adr)) == -1) err("bind() error");

    while (1) {
        dns_adr_sz = sizeof(dns_adr);
        str_len = recvfrom(host_sock, msg, BUF_SIZE, 0, (struct sockaddr*)&dns_adr, &dns_adr_sz);
        sendto(host_sock, puzzle_data, str_len, 0, (struct sockaddr*)&dns_adr, dns_adr_sz);
    }

    close(host_sock);
    return 0;
}