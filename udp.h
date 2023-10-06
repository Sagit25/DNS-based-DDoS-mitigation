//
// Created by junjinyong on 23. 9. 14.
//

#ifndef DDOS_MITIGATION_UDP_H
#define DDOS_MITIGATION_UDP_H

#include <string.h>

#define BUF_SIZE 1024
#define CHAIN_LENGTH 64

void error_handling(char* message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

const char* server_ip = "127.0.0.1";
const char* server_port = "10000";

const char* dns_ip = "127.0.0.1";
const char* dns_port = "20000";

const char* auth_ip = "127.0.0.1";
const char* auth_port = "30000";

struct sockaddr_in server_address, dns_address, auth_address;

void initialize_address() {
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(server_ip);
    server_address.sin_port = htons(atoi(server_port));

    memset(&dns_address, 0, sizeof(dns_address));
    dns_address.sin_family = AF_INET;
    dns_address.sin_addr.s_addr = inet_addr(dns_ip);
    dns_address.sin_port = htons(atoi(dns_port));

    memset(&auth_address, 0, sizeof(auth_address));
    auth_address.sin_family = AF_INET;
    auth_address.sin_addr.s_addr = inet_addr(auth_ip);
    auth_address.sin_port = htons(atoi(auth_port));
}

int compare(struct sockaddr_in *a, struct sockaddr_in *b) {
    return (a -> sin_addr).s_addr == (b -> sin_addr).s_addr && (a -> sin_port) == (b -> sin_port);
}

#endif //DDOS_MITIGATION_UDP_H
