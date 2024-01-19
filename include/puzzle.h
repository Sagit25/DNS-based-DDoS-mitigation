/* Created by shyang */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUF_SIZE 1024
#define MAX_CHAIN_LENGTH 10001

struct ip_msg {
    char* ip_str;
    int ip_num;
    int port_num;
};

struct puzzle_msg {
    unsigned int token;
    unsigned int threshold;
};

struct chain_msg {
    int seed;
    int length;
    unsigned int threshold;
};

int hash_ftn(int seed) {
    return seed+1;
}
