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
    char msg[BUF_SIZE], msg_rcv[BUF_SIZE];
    int msg_len;
    socklen_t local_dns_adr_sz, server_adr_sz;
    struct sockaddr_in local_dns_adr, server_adr, client_adr;
    struct ip_msg ipmsg;
    struct puzzle_msg pmsg;

    if (argc != 5) 
    {
        printf("Usage : %s <local dns ip> <local dns port> <client ip> <client port>\n", argv[0]);
        exit(1);
    }

    // Create client's UDP socket 
    client_udp_sock = socket(PF_INET, SOCK_DGRAM, 0); 
    if (client_udp_sock == -1) 
    {
        printf("UDP socket creation error\n");
        exit(1);   
    }
    printf("Create client UDP socket\n");
    memset(&local_dns_adr, 0, sizeof(local_dns_adr));
    local_dns_adr.sin_family = AF_INET;
    local_dns_adr.sin_addr.s_addr = inet_addr(argv[1]);
    local_dns_adr.sin_port = htons(atoi(argv[2]));
    printf("Bind address of local DNS server\n");

    // Set local DNS in kernel
    int lret = syscall(458, inet_addr(argv[1]), atoi(argv[2]));
    if (lret != 0) 
    {
        printf("Set local DNS error\n");
        exit(1);   
    }

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
    printf("Get address of target server [%d:%d]\n", ipmsg.ip_num, ipmsg.port_num);

    // Set address of target server
    memset(&server_adr, 0, sizeof(server_adr));
    server_adr.sin_family = AF_INET;
    server_adr.sin_addr.s_addr = ipmsg.ip_num;
    server_adr.sin_port = htons(ipmsg.port_num);
    printf("Set address of target server\n");
    
    // Loop for sending TCP packets to host server
    while (1) {
        // Create client TCP socket
        client_tcp_sock = socket(PF_INET, SOCK_STREAM, 0); 
        if (client_tcp_sock == -1) 
        {
            printf("TCP socket creation error");
            exit(1);   
        }
        printf("Create client TCP socket\n");
        memset(&client_adr, 0, sizeof(client_adr));
        client_adr.sin_family = AF_INET;
        client_adr.sin_addr.s_addr = inet_addr(argv[3]);
        client_adr.sin_port = htons(atoi(argv[4]));
        if (bind(client_tcp_sock, (struct sockaddr*)&client_adr, sizeof(client_adr)) == -1) {
            printf("bind() error\n");
            exit(1);
        }
        printf("Bind client server TCP socket\n");

        int puzzle_type = syscall(455); // get puzzle type()

        if (puzzle_type == PZLTYPE_EXT) {
            // Get puzzle record from local dns resolver
            local_dns_adr_sz = sizeof(local_dns_adr);
            msg[0] = 'p';
            msg[1] = '\0';
            sendto(client_udp_sock, msg, strlen(msg), 0, (struct sockaddr*)&local_dns_adr, local_dns_adr_sz);
            msg_len = recvfrom(client_udp_sock, (void*)&pmsg, sizeof(pmsg), 0, (struct sockaddr *)&local_dns_adr, &local_dns_adr_sz);
            if (msg_len < 0) {
                printf("UDP recvfrom() error - puzzle record\n");
                exit(1);   
            }
            msg[msg_len] = '\0';
            printf("Get puzzle token:%u, threshold:%u\n", pmsg.token, pmsg.threshold);
            printf("ip1: %u, 1p2: %u\n", atoi(argv[3]), inet_addr(argv[3]));
            int ret = syscall(461, atoi(argv[3]), atoi(argv[1]), puzzle_type, pmsg.token, pmsg.threshold); // set_puzzle_cache()
            if (ret < 0) {
                printf("Set puzzle cache error");
                exit(1);   
            }
        }
                
        // Send TCP SYN packets
        server_adr_sz = sizeof(server_adr);
        int connect_result = connect(client_tcp_sock, (struct sockaddr*)&server_adr, server_adr_sz);
        if (connect_result == -1) {
            printf("Connect failed\n");
            puzzle_type = syscall(456, PZLTYPE_EXT);
            close(client_tcp_sock);
            continue;
        }
        printf("Connect to client TCP socket [%d:%d]\n", ipmsg.ip_num, ipmsg.port_num);

        // Send TCP packet
        int send_result = send(client_tcp_sock, msg, BUF_SIZE-1, 0);
        printf("Send TCP packets to target server (%d)\n", send_result);
        msg_len = recv(client_tcp_sock, msg_rcv, BUF_SIZE-1, 0);
        msg_rcv[msg_len] = '\0';
        printf("Receive TCP packets from target server (%d)\n", msg_len);

        // Close client TCP socket
        close(client_tcp_sock);
    }
    
    // Close client UDP socket
    close(client_udp_sock);

    return 0;
}