#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "udp.h"

int main(int argc, char *argv[]) {
    int sock;
    char message[BUF_SIZE];
    int str_len;
    int difficulty = 0;
    socklen_t address_size;

    struct sockaddr_in incoming_address;
    initialize_address();

    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == -1) {
        error_handling("UDP socket creation error");
    }
    puts("Authoritative DNS socket creation successful");

    const int option = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));



    if (bind(sock, (struct sockaddr *) &auth_address, sizeof(auth_address)) == -1) {
        error_handling("bind() error");
    }
    puts("Authoritative DNS socket bind successful");

    sleep(1);

    while (1) {
        address_size = sizeof(incoming_address);
        str_len = (int) recvfrom(sock, message, BUF_SIZE, 0,
                                 (struct sockaddr *) &incoming_address, &address_size);
        message[str_len] = '\0';

        if(compare(&incoming_address, &server_address)) {
            puts("Authoritative DNS detected signal from server");

            const int signal = atoi(message);
            if(signal == 1 && difficulty < 31) {
                ++difficulty;
            } else if(signal == -1 && difficulty > 0) {
                --difficulty;
            }
            printf("difficulty: %d\n", difficulty);
            continue;
        } else if(compare(&incoming_address, &dns_address)) {
            puts("Authoritative DNS detected signal from local DNS");
        } else {
            break;
        }

        const int seed = rand();
        const int length = CHAIN_LENGTH;
        sprintf(message, "%d %d %d", seed, length, difficulty);

        sendto(sock, message, strlen(message), 0,
               (struct sockaddr *) &dns_address, address_size);
        puts("Authoritative DNS sent signal to local DNS");
    }

    puts("All processes have completed");

    close(sock);
    getchar();
    return 0;
}
