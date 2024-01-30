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

unsigned int iptou(char* cp) {
	unsigned int val;
	int base;
	char c;
	unsigned int parts[4];
	unsigned int *pp = parts;

	for (;;) {
		val = 0; base = 10;
		if (*cp == '0') {
			if (*++cp == 'x' || *cp == 'X')
				base = 16, cp++;
			else
				base = 8;
		}
		while ((c = *cp) != '\0') {
			if (isascii(c) && isdigit(c)) {
				val = (val * base) + (c - '0');
				cp++;
				continue;
			}
			if (base == 16 && isascii(c) && isxdigit(c)) {
				val = (val << 4) + 
					(c + 10 - (islower(c) ? 'a' : 'A'));
				cp++;
				continue;
			}
			break;
		}
		if (*cp == '.') {
			if (pp >= parts + 3 || val > 0xff)
				return (0);
			*pp++ = val, cp++;
		} else
			break;
	}
	if (*cp && (!isascii(*cp) || !isspace(*cp))) return (0);
	if (val > 0xff) return (0);
	val |= (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8);
	return val;
}


