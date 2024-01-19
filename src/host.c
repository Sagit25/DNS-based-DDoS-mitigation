/* Created by shyang */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "puzzle.h"

int main(int argc, char* argv[]) {
    int host_sock, client_sock;
    char msg[BUF_SIZE];
    socklen_t client_adr_sz;
    struct sockaddr_in client_adr, host_adr;

    if (argc != 3) {
        printf("Usage : %s <host ip> <host port>\n", argv[0]);
        exit(1);
    }

    // Create and bind host's TCP socket
    host_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (host_sock == -1) {
        printf("UDP socket creation error\n");
        exit(1);
    }
    memset(&host_adr, 0, sizeof(host_adr));
    host_adr.sin_family = AF_INET;
    host_adr.sin_addr.s_addr = inet_addr(argv[1]);
    host_adr.sin_port = htons(atoi(argv[2]));
    if (bind(host_sock, (struct sockaddr*)&host_adr, sizeof(host_adr)) == -1) {
        printf("bind() error\n");
        exit(1);
    }

    // Listen TCP socket for incoming connections
    if (listen(host_sock, 5) == -1) {
        printf("listen() error\n");
        exit(1);
    }

    // Loop for response TCP packets
    while (1) {
        // Accept client TCP socket
        client_adr_sz = sizeof(client_adr);
        client_sock = accept(host_sock, (struct sockaddr*)&client_adr, &client_adr_sz);
        if (client_sock == -1) {
            printf("accept() error\n");
            exit(1);
        }

        // Receive and send message between client and host server
        if (recv(host_sock, msg, strlen(msg), 0) == -1) {
            printf("recv() error\n");
            exit(1);
        }
        if (send(host_sock, msg, strlen(msg), 0)) {
            printf("send() error\n");
            exit(1);
        }

        // Close client TCP socket
        close(client_sock);
    }

    // Close host server socket
    close(host_sock);

    return 0;
}
