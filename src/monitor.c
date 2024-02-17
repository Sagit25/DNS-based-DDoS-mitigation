/* Created by shyang */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "puzzle.h"

int main(int argc, char* argv[]) {
    int monitor_sock;
    char msg[BUF_SIZE];
    socklen_t auth_adr_sz;
    struct sockaddr_in auth_adr, monitor_adr;

    if (argc != 3) 
    {
        printf("Usage : %s <monitor ip> <monitor port>\n", argv[0]);
        exit(1);
    }
    
    // Create and bind monitor server UDP socket
    monitor_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (monitor_sock == -1) 
    {
        printf("UDP socket creation error");
        exit(1);
    }
    printf("Create monitor server UDP socket\n");
    memset(&monitor_adr, 0, sizeof(monitor_adr));
    monitor_adr.sin_family = AF_INET;
    monitor_adr.sin_addr.s_addr = inet_addr(argv[1]);
    monitor_adr.sin_port = htons(atoi(argv[2]));
    if (bind(monitor_sock, (struct sockaddr*)&monitor_adr, sizeof(monitor_adr)) == -1) 
    {
        printf("bind() error");
        exit(1);
    }
    printf("Bind monitor server UDP socket\n");

    while (1) {
        auth_adr_sz = sizeof(auth_adr);
        recvfrom(monitor_sock, (void*)&cmsg, sizeof(cmsg), 0, (struct sockaddr*)&auth_adr, &auth_adr_sz);
        printf("Receive message from auth server\n");
        struct chain_msg cmsg;
        cmsg.seed = rand();
        cmsg.length = MAX_CHAIN_LENGTH; // TODO
        cmsg.threshold = syscall(453, ipmsg.ip_num); // TODO
        sendto(monitor_sock, (void*)&cmsg, sizeof(cmsg), 0, (struct sockaddr*)&auth_adr, auth_adr_sz);
        printf("Send puzzle record to auth server\n");
    }

    // Close socket
    close(monitor_sock);

    return 0;
}