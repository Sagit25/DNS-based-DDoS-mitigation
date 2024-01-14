#include <stdlib.h>
#include <string.h>

void err(char* str) {
    fputs(str, stderr);
    fputc('\n', stderr);
    exit(1);
}
