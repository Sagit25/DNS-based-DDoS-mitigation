/* Created by shyang */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "puzzle.h"

#define WO_MONITOR 0
#define W_MONITOR 1

int main(int argc, char* argv[]) {
    int auth_sock;
    int monitor_mode = WO_MONITOR;
    char msg[BUF_SIZE];
    socklen_t local_dns_adr_sz, monitor_adr_sz;
    struct sockaddr_in local_dns_adr, auth_adr, monitor_adr;

    if (argc != 5 && argc != 7) 
    {
        printf("Usage : %s <auth ip> <auth port> <host ip> <host port> (<monitor ip> <monitor port>)\n", argv[0]);
        exit(1);
    }
    if (argc == 7) monitor_mode = W_MONITOR;

    // Set host ip and port
    struct ip_msg ipmsg;
    ipmsg.ip_str = argv[3];
    ipmsg.ip_num = inet_addr(argv[3]);
    ipmsg.port_num = atoi(argv[4]);
    
    // Create and bind authoritative name server UDP socket
    auth_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (auth_sock == -1) 
    {
        printf("UDP socket creation error");
        exit(1);
    }
    printf("Create authoritative name server UDP socket\n");
    memset(&auth_adr, 0, sizeof(auth_adr));
    auth_adr.sin_family = AF_INET;
    auth_adr.sin_addr.s_addr = inet_addr(argv[1]);
    auth_adr.sin_port = htons(atoi(argv[2]));
    if (bind(auth_sock, (struct sockaddr*)&auth_adr, sizeof(auth_adr)) == -1) 
    {
        printf("bind() error");
        exit(1);
    }
    printf("Bind authoritative name server UDP socket\n");

    // Define monitor server address
    if (monitor_mode == W_MONITOR) {
        memset(&monitor_adr, 0, sizeof(monitor_adr));
        monitor_adr.sin_family = AF_INET;
        monitor_adr.sin_addr.s_addr = inet_addr(argv[5]);
        monitor_adr.sin_port = htons(atoi(argv[6]));
    }

    while (1) {
        local_dns_adr_sz = sizeof(local_dns_adr);
        recvfrom(auth_sock, msg, BUF_SIZE, 0, (struct sockaddr*)&local_dns_adr, &local_dns_adr_sz);
        printf("Receive message from local dns server\n");
        if (msg[0] == 'i') {
            sendto(auth_sock, (void*)&ipmsg, sizeof(ipmsg), 0, (struct sockaddr*)&local_dns_adr, local_dns_adr_sz);
            printf("Send host ip record to local dns server\n");
        }
        else {
            struct chain_msg cmsg;
            if (monitor_mode == WO_MONITOR) {
                cmsg.seed = rand();
                cmsg.length = MAX_CHAIN_LENGTH; // TODO
                cmsg.type = syscall(455);
                cmsg.threshold = syscall(453, ipmsg.ip_num);
            }
            else {
                // Get hash chain message from monitor
                monitor_adr_sz = sizeof(monitor_adr);
                sendto(auth_sock, (void*)&ipmsg, sizeof(ipmsg), 0, (struct sockaddr*)&monitor_adr, monitor_adr_sz);
                printf("Receive message from auth server\n");
                recvfrom(auth_sock, (void*)&cmsg, sizeof(cmsg), 0, (struct sockaddr*)&monitor_adr, &monitor_adr_sz);
                printf("Send puzzle record to monitor server\n");
            }
            printf("Puzzle record %u, %u, %u, %u\n", cmsg.seed, cmsg.length, cmsg.type, cmsg.threshold);
            sendto(auth_sock, (void*)&cmsg, sizeof(cmsg), 0, (struct sockaddr*)&local_dns_adr, local_dns_adr_sz);
            printf("Send puzzle record to local dns server\n");
        }
    }

    // Close socket
    close(auth_sock);

    return 0;
}