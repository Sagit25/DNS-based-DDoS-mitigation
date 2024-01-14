#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "udp.h"

int monitor() {

}

int main(int argc, char *argv[]) {
    int auth_sock;
    char message[BUF_SIZE];
    int str_len;
    socklen_t address_size;

    struct sockaddr_in incoming_address;
    initialize_address();

    auth_sock = socket(PF_INET, SOCK_DGRAM, 0); // Authoritative name server's UDP socket
    if (auth_sock == -1) {
        printf("Socket creation error\n");
        exit(1);
    }
    printf("Creat auth. DNS socket\n");

    const int option = 1;
    setsockopt(auth_sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    if (bind(auth_sock, (struct sockaddr *) &auth_address, sizeof(auth_address)) == -1) {
        printf("Socket bind error\n");
        exit(1);
    }
    puts("Bind auth. DNS socket\n");

    while (1) {
        address_size = sizeof(incoming_address);
        str_len = (int) recvfrom(auth_sock, message, BUF_SIZE, 0,
                                 (struct sockaddr *) &incoming_address, &address_size);
        message[str_len] = '\0';

        // TODO: change_difficulty(); 

        const int seed = rand();
        const int length = CHAIN_LENGTH;
        sprintf(message, "%d %d %d", seed, length, difficulty);

        sendto(auth_sock, message, strlen(message), 0,
               (struct sockaddr *) &dns_address, address_size);
        printf("%d %d %d", seed, length, difficulty);
    }

    close(auth_sock);
    return 0;
}