/* Created by shyang */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char* argv[]) {
    if (argc != 1) {
        printf("Usage : %s");
        exit(1);
    }
    syscall(456, 0);
    printf("Set puzzle type to PUZZLE_NONE\n");
    return 0;
}
