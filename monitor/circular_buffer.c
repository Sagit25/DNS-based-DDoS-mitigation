#include <time.h>
#include <stdbool.h>
#include "circular_buffer.h"

#define BUFFER_SIZE 10000

struct circular_buffer {
	struct timeval data[BUFFER_SIZE];
	int start;
	int end;
	int size;
};

struct circular_buffer buffer;

void init_circular_buffer() {
	buffer.start = 0;
	buffer.end = 0;
	buffer.size = 0;
}

bool is_circular_buffer_empty() {
	return buffer.size == 0;
}

bool is_circular_buffer_full() {
	return buffer.size == BUFFER_SIZE;
}

void push_circular_buffer(struct timeval ts) {
	if(is_circular_buffer_full()) {
		buffer.start = (buffer.start + 1) % BUFFER_SIZE;
	}

	buffer.data[buffer.end] = ts;
	buffer.end = (buffer.end + 1) % BUFFER_SIZE;
	buffer.size++;
}

void pop_circular_buffer() {
	if(!is_circular_buffer_empty()){
		buffer.start = (buffer.start + 1) % BUFFER_SIZE;
		buffer.size--;
	}
}

int get_circular_buffer_size() {
	return buffer.size;
}

struct timeval get_circular_buffer_front() {
	return buffer.data[buffer.start];
}
