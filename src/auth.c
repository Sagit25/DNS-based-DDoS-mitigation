/* Created by shyang */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "puzzle.h"

int main(int argc, char* argv[]) {
    int auth_sock;
    char msg[BUF_SIZE];
    socklen_t local_dns_adr_sz;
    struct sockaddr_in local_dns_adr, auth_adr;

    if (argc != 5) 
    {
        printf("Usage : %s <auth ip> <auth port> <host ip> <host port> \n", argv[0]);
        exit(1);
    }

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
            cmsg.seed = rand();
            cmsg.length = MAX_CHAIN_LENGTH; // TODO
            cmsg.threshold = syscall(453, ipmsg.ip_num);
            sendto(auth_sock, (void*)&cmsg, sizeof(cmsg), 0, (struct sockaddr*)&local_dns_adr, local_dns_adr_sz);
            printf("Send puzzle record to local dns server\n");
        }
        
    }

    // Close socket
    close(auth_sock);

    return 0;
}