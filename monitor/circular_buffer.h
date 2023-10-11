#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <sys/time.h>
#include <time.h>
#include <stdbool.h>

#define BUFFER_SIZE 10000
#define ISP_NUMBER 10

struct packet_info {
	struct timeval ts;
	int isp_id;
};

struct circular_buffer {
	struct packet_info _data[BUFFER_SIZE];
	int isp_count[ISP_NUMBER];
	int _start;
	int _end;
	int _size;
};

void init_circular_buffer();
bool is_circular_buffer_full();
bool is_circular_buffer_empty();
void push_circular_buffer(struct packet_info pi);
void pop_circular_buffer();
int get_circular_buffer_size();
struct packet_info get_circular_buffer_front();
int get_circular_buffer_isp_count(int isp_id);

#endif
