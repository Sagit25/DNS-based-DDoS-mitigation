#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "error.h"

#define BUF_SIZE 1024

int main(int argc, char* argv[]) {
    int dns_sock;
    char msg[BUF_SIZE]; // get data from bot, bot UDP
    char msg_puzzle[BUF_SIZE]; // get data from target, this is changed by target UDP
    int str_len;
    socklen_t bot_adr_sz, target_adr_sz;
    struct sockaddr_in dns_adr, bot_adr, target_adr;

    if (argc > 3 || argc < 2) {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    dns_sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP); // Local DNS resolver's UDP socket
    if (dns_sock == -1) err("UDP socket creation error");
    memset(&dns_adr, 0, sizeof(dns_adr));
    dns_adr.sin_family = AF_INET;
    dns_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    dns_adr.sin_port = htons(atoi(argv[1]));
    if (bind(dns_sock, (struct sockaddr*)&dns_adr, sizeof(dns_adr)) == -1) err("bind() error");

    while (1) {
        bot_adr_sz = sizeof(bot_adr);
        target_adr_sz = sizeof(target_adr);
        str_len = recvfrom(dns_sock, msg, BUF_SIZE, 0, (struct sockaddr*)&bot_adr, &bot_adr_sz);
        if (msg != msg_puzzle) {
            sendto(dns_sock, msg_puzzle, str_len, 0, (struct sockaddr*)&target_adr, &target_adr_sz);
            str_len = recvfrom(dns_sock, msg_puzzle, BUF_SIZE, 0, (struct sockaddr*)&target_adr, &target_adr_sz);
        }
        sendto(dns_sock, msg, str_len, 0, (struct sockaddr*)&bot_adr, bot_adr_sz);
    }

    close(dns_sock);
    return 0;
}