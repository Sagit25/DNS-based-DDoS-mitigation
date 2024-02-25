/* Created by shyang */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "puzzle.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage : %s <local dns ip> <threshold>\n", argv[0]);
        exit(1);
    }
    int pret = syscall(456, PZLTYPE_EXT); // set puzzle type()
    if (pret != PZLTYPE_EXT) {
        printf("Set threshold error!\n");
        return 0;
    }
    int ret = syscall(454, inet_addr(argv[1]), atoi(argv[2])); // set threshold()
    if (ret != atoi(argv[2])) {
        printf("Set threshold error!\n");
        return 0;
    }
    printf("Change ptype: %u, threshold ip:%u, threshold:%u\n", syscall(455), inet_addr(argv[1]), syscall(453, inet_addr(argv[1])));

    return 0;
}
