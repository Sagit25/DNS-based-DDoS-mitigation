/* Created by shyang */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "puzzle.h"

int main(int argc, char* argv[]) {
    int local_dns_sock;
    char msg[BUF_SIZE];
    socklen_t client_adr_sz, auth_adr_sz;
    struct sockaddr_in local_dns_adr, client_adr, auth_adr;

    if (argc != 5) 
    {
        printf("Usage : %s <local dns ip> <local dns port> <auth ip> <auth port> \n", argv[0]);
        exit(1);
    }

    // Set host ip and port
    struct ip_msg ipmsg;
    ipmsg.ip_num = 0;
    
    // Create and bind local DNS UDP socket
    local_dns_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (local_dns_sock == -1) 
    {
        printf("UDP socket creation error");
        exit(1);
    }
    printf("Create local DNS server UDP socket\n");
    memset(&local_dns_adr, 0, sizeof(local_dns_adr));
    local_dns_adr.sin_family = AF_INET;
    local_dns_adr.sin_addr.s_addr = inet_addr(argv[1]);
    local_dns_adr.sin_port = htons(atoi(argv[2]));
    if (bind(local_dns_sock, (struct sockaddr*)&local_dns_adr, sizeof(local_dns_adr)) == -1) 
    {
        printf("bind() error");
        exit(1);
    }
    printf("Bind local DNS server UDP socket\n");

    // Define authoritative name server address
    memset(&auth_adr, 0, sizeof(auth_adr));
    auth_adr.sin_family = AF_INET;
    auth_adr.sin_addr.s_addr = inet_addr(argv[3]);
    auth_adr.sin_port = htons(atoi(argv[4]));

    // Hash chain variables
    struct chain_msg cmsg;
    int seed, length = 1, threshold, type;
    int hash_chain[MAX_CHAIN_LENGTH];
    memset(&hash_chain, 0, sizeof(hash_chain));

    while (1) {
        client_adr_sz = sizeof(client_adr);
        auth_adr_sz = sizeof(auth_adr);
        recvfrom(local_dns_sock, msg, BUF_SIZE, 0, (struct sockaddr*)&client_adr, &client_adr_sz);
        printf("Receive message from client\n");
        if (msg[0] == 'i') {
            if (ipmsg.ip_num == 0) {
                sendto(local_dns_sock, msg, strlen(msg), 0, (struct sockaddr*)&auth_adr, auth_adr_sz);
                recvfrom(local_dns_sock, (void*)&ipmsg, sizeof(ipmsg), 0, (struct sockaddr*)&auth_adr, &auth_adr_sz);
                printf("Get host ip record from authoritative name server\n");
            }
            sendto(local_dns_sock, (void*)&ipmsg, sizeof(ipmsg), 0, (struct sockaddr*)&client_adr, client_adr_sz);
            printf("Send host ip record to client\n");
        }
        else {
            if (length <= 1 || msg[0] == 'u') {
                length = 1;
                sendto(local_dns_sock, msg, strlen(msg), 0, (struct sockaddr*)&auth_adr, auth_adr_sz);
                recvfrom(local_dns_sock, (void*)&cmsg, sizeof(cmsg), 0, (struct sockaddr*)&auth_adr, &auth_adr_sz);
                printf("Get puzzle record from authoritative name server\n");
                seed = cmsg.seed;
                threshold = cmsg.threshold;
                type = cmsg.type;
                hash_chain[0] = seed;
                while (length <= cmsg.length) {
                    hash_chain[length] = hash_ftn(HASH_SALT, hash_chain[length-1]);
                    length++;
                }
            }
            struct puzzle_msg pmsg = {type, hash_chain[length--], threshold};
            sendto(local_dns_sock, (void*)&pmsg, sizeof(pmsg), 0, (struct sockaddr*)&client_adr, client_adr_sz);
            printf("Send puzzle record to client\n");
        }
    }

    // Close socket
    close(local_dns_sock);

    return 0;
}