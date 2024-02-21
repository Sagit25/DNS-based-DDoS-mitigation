#ifndef DETECT_DDOS_H
#define DETECT_DDOS_H

#include <unistd.h>
#include "tcp_syn_monitor.h"

void set_difficulty(int isp_id, int current);
void handle_ddos(cb_ptr buffer);
int detect_ddos(void);
void start_detect_ddos_thread(pthread_t *tid);
void stop_detect_ddos_thread(pthread_t *tid);

#endif