#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <sys/time.h>
#include <time.h>
#include <stdbool.h>

void init_circular_buffer();
bool is_circular_buffer_full();
bool is_circular_buffer_empty();
void push_circular_buffer(struct timeval timestamp);
void pop_circular_buffer();
int get_circular_buffer_size();
struct timeval get_circular_buffer_front();

#endif
