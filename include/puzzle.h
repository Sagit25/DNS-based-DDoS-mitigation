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

int stohi(char *ip){
	char c = *ip;
	unsigned int res;
	int val;
	for (int j = 0; j < 4; j++) {
		if (!isdigit(c)) return 0;
		val = 0;
		for (int i = 0; i < 3; i++) {
			if (isdigit(c)) {
				val = (val * 10) + (c - '0');
				c = *(++ip);
			} 
            else break;
		}
		if (val < 0 || val > 255) return 0;		
		if (c == '.') {
			res = (res << 8) | val;
			c = *(++ip);
		} 
		else if (j == 3 && c == '\0'){
			res = (res << 8) | val;
			break;
		}
			
	}
	if (c != '\0') return 0;
	return res;
}
