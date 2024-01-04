#pragma once
#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <sys/time.h>
#include <time.h>
#include <stdbool.h>

#define MAX_BUFFER_SIZE 10000
#define ISP_NUMBER 10

//#define circular_buffer_ptr struct circular_buffer*
typedef struct circular_buffer* cb_ptr;

struct packet_info {
	struct timeval ts;
	int isp_id;
};

struct circular_buffer {
	struct packet_info _data[MAX_BUFFER_SIZE];
	int isp_count[ISP_NUMBER];
	int _start;
	int _end;
	int _size;
};

void init_circular_buffer(cb_ptr);
bool is_circular_buffer_full(cb_ptr);
bool is_circular_buffer_empty(cb_ptr);
void push_circular_buffer(cb_ptr, struct packet_info pi);
void pop_circular_buffer(cb_ptr);
int get_circular_buffer_size(cb_ptr);
struct packet_info get_circular_buffer_front(cb_ptr);
int get_circular_buffer_isp_count(cb_ptr, int isp_id);

#endif
