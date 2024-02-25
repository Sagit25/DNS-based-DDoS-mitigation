/* Created by shyang */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <openssl/sha.h>

#define BUF_SIZE 1024
#define MAX_CHAIN_LENGTH 10001
#define SHA256_LENGTH 32
#define CBF_INPUT_LENGTH 8

#define PZLTYPE_NONE 0
#define PZLTYPE_EXT 1
#define PZLTYPE_BOT 2

#define HASH_SALT 42

struct ip_msg {
    char* ip_str;
    int ip_num;
    int port_num;
};

struct puzzle_msg {
    unsigned int type;
    unsigned int token;
    unsigned int threshold;
};

struct chain_msg {
    int seed;
    int length;
    unsigned int type;
    unsigned int threshold;
};

unsigned int hash_ftn(unsigned int salt, unsigned int x) {
    unsigned char input[CBF_INPUT_LENGTH];
    unsigned char output[SHA256_LENGTH];

    for (int i = 0; i < 4; ++i) {
        input[i] = (salt >> (i * 8)) & 255;
        input[i + 4] = (x >> (i * 8)) & 255;
    }

    SHA256(input, CBF_INPUT_LENGTH, output);

    return *((unsigned int*) output);
}
