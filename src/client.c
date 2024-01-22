/* Created by shyang */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "puzzle.h"

int main(int argc, char* argv[]) {
    int client_tcp_sock, client_udp_sock;
    char msg[BUF_SIZE];
    int msg_len;
    socklen_t local_dns_adr_sz;
    struct sockaddr_in local_dns_adr, server_adr;
    struct ip_msg ipmsg;
    struct puzzle_msg pmsg;

    if (argc != 3) 
    {
        printf("Usage : %s <local dns ip> <local dns port>\n", argv[0]);
        exit(1);
    }

    // Create client's UDP socket 
    client_udp_sock = socket(PF_INET, SOCK_DGRAM, 0); 
    if (client_udp_sock == -1) 
    {
        printf("UDP socket creation error\n");
        exit(1);   
    }
    memset(&local_dns_adr, 0, sizeof(local_dns_adr));
    local_dns_adr.sin_family = AF_INET;
    local_dns_adr.sin_addr.s_addr = inet_addr(argv[1]);
    local_dns_adr.sin_port = htons(atoi(argv[2]));

    // Set local DNS in kernel
    /*
    int ret = syscall(458, inet_addr(argv[1]), atoi(argv[2]));
    if (ret != 0) 
    {
        printf("Set local DNS error\n");
        exit(1);   
    }
    */

    // Send UDP packet to get target server address
    local_dns_adr_sz = sizeof(local_dns_adr);
    msg[0] = 'i';
    msg[1] = '\0';
    sendto(client_udp_sock, msg, strlen(msg), 0, (struct sockaddr*)&local_dns_adr, local_dns_adr_sz);
    msg_len = recvfrom(client_udp_sock, (void*)&ipmsg, sizeof(ipmsg), 0, (struct sockaddr *)&local_dns_adr, &local_dns_adr_sz);
    if (msg_len < 0) 
    {
        printf("UDP recvfrom() error\n");    
        exit(1);   
    }
    msg[msg_len] = '\0';
    printf("%d:%d\n", ipmsg.ip_num, ipmsg.port_num);

    // Create and connect client's TCP socket
    client_tcp_sock = socket(PF_INET, SOCK_STREAM, 0); 
    if (client_tcp_sock == -1) 
    {
        printf("TCP socket creation error");
        exit(1);   
    }
    memset(&server_adr, 0, sizeof(server_adr));
    server_adr.sin_family = AF_INET;
    server_adr.sin_addr.s_addr = ipmsg.ip_num;
    server_adr.sin_port = htons(ipmsg.port_num);
    if (connect(client_tcp_sock, (struct sockaddr*)&server_adr, sizeof(server_adr)) == -1) 
    {
        printf("connect() error");
        exit(1);   
    }
    printf("Client TCP socket connect [%s:%d]", ipmsg.ip_str, ipmsg.port_num);

    // Loop for sending TCP packets to host server
    while (1) {
        int send_result = send(client_tcp_sock, msg, strlen(msg), 0);
        while (send_result == -1) { // TCP SYN reset
            // Send UDP packet to get puzzle information
            local_dns_adr_sz = sizeof(local_dns_adr);
            msg[0] = 'p';
            msg[1] = '\0';
            sendto(client_udp_sock, msg, strlen(msg), 0, (struct sockaddr*)&local_dns_adr, local_dns_adr_sz);
            msg_len = recvfrom(client_udp_sock, (void*)&pmsg, sizeof(pmsg), 0, (struct sockaddr *)&local_dns_adr, &local_dns_adr_sz);
            if (msg_len < 0) 
            {
                printf("UDP recvfrom() error\n");
                exit(1);   
            }
            msg[msg_len] = '\0';
            printf("token:%u, threshold:%u\n", pmsg.token, pmsg.threshold);
            syscall(461, inet_addr(INADDR_ANY), pmsg.token, pmsg.threshold); // set_puzzle_cache()
            
            // Re-send TCP packets
            send_result = send(client_tcp_sock, msg, strlen(msg), 0);
        }
        msg_len = recv(client_tcp_sock, msg, strlen(msg), 0);
        printf("Send TCP packets\n");
        
        sleep(1);
    }

    // Close sockets
    close(client_tcp_sock);
    close(client_udp_sock);

    return 0;
}