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
    socklen_t address_size;
    int seed, length, difficulty, hash_chain[CHAIN_LENGTH];

    struct sockaddr_in incoming_address;
    initialize_address();

    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == -1) {
        error_handling("UDP socket creation error");
    }
    puts("DNS socket creation successful");

    const int option = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    if (bind(sock, (struct sockaddr *) &dns_address, sizeof(dns_address)) == -1) {
        error_handling("bind() error");
    }
    puts("DNS socket bind successful");

    sleep(1);

    int index = 0;
    while (1) {
        address_size = sizeof(incoming_address);
        str_len = (int) recvfrom(sock, message, BUF_SIZE, 0,
                                 (struct sockaddr *) &incoming_address, &address_size);
        message[str_len] = '\0';

        if(compare(&incoming_address, &auth_address)) {
            puts("Local DNS received signal from authoritative DNS");

            sscanf(message, "%d %d %d", &seed, &length, &difficulty);

            hash_chain[0] = seed + rand();
            for(int i = 1; i < length; ++i) {
                hash_chain[i] = (hash_chain[i - 1] + 233333) * 2027;
                if(hash_chain[i] == -1) {
                    hash_chain[i] = 20020517;
                }
            }

            index = length - 1;

            continue;
        }

        puts("DNS received query from user");

        if(strcmp(message, "Q") == 0) {
            break;
        }

        printf("index: %d\n", index);
        if(index >= 0) {
            sprintf(message, "%d %d", hash_chain[index], difficulty);
            --index;
        } else {
            sprintf(message, "%d", 1);
            sendto(sock, message, strlen(message), 0,
                   (struct sockaddr *) &auth_address, address_size);

            sleep(1);

            sprintf(message, "%d %d", -1, 32);
        }

        sendto(sock, message, strlen(message), 0,
               (struct sockaddr *) &incoming_address, address_size);
        puts("DNS sent response to user");
    }

    puts("All processes have completed");

    close(sock);
    getchar();
    return 0;
}
