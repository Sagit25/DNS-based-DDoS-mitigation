#ifndef MONITOR_LOG_H
#define MONITOR_LOG_H

#include <pthread.h>

typedef struct {
	pthread_t tid;
	cb_ptr buffer;
} log_thread_data;

void *log_thread(void *data);
void start_log_thread(log_thread_data *data);
void stop_log_thread(log_thread_data *data);

#endif
