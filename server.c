#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

#include "udp.h"

int verify_puzzle(int puzzle);
int monitor();

int main(int argc, char *argv[]) {
    int sock;
    char message[BUF_SIZE];
    int str_len;
    socklen_t address_size;
    int puzzle;

    struct sockaddr_in incoming_address;
    initialize_address();

    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == -1) {
        error_handling("UDP socket creation error");
    }
    puts("Server socket creation successful");

    const int option = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));



    if (bind(sock, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
        error_handling("bind() error");
    }
    puts("Server socket bind successful");

    sleep(1);

    while (1) {
        address_size = sizeof(incoming_address);
        str_len = (int) recvfrom(sock, message, BUF_SIZE, 0,
                                 (struct sockaddr *) &incoming_address, &address_size);

        message[str_len] = '\0';
        puts("server received query from user");

        if(strcmp(message, "Q") == 0) {
            break;
        }

        const int verified = verify_puzzle(atoi(message));
        sprintf(message, "%d", verified);

        sendto(sock, message, strlen(message), 0,
               (struct sockaddr *) &incoming_address, address_size);
        puts("server sent response to user");

        const int signal = monitor();
        if(signal) {
            sprintf(message, "%d", signal);
            sendto(sock, message, strlen(message), 0,
                   (struct sockaddr *) &auth_address, address_size);
            puts("server sent signal to authoritative DNS");
        }
    }

    puts("All processes have completed");

    close(sock);
    getchar();
    return 0;
}

int verify_puzzle(int puzzle) {
    return 1;
    // return (puzzle & 1023) == 0;
}

int monitor() {
    static long long int traffic = 0;
    static time_t prev = 0;

    ++traffic;
    time_t curr = time(NULL);
    if(curr - prev >= 1) {
        int result = 0;
        if(traffic > 20) {
            result = 1;
        } else if(traffic < 5) {
            result = -1;
        }

        traffic = 0;
        prev = curr;
        return result;
    }
    return 0;
}
