#include <unistd.h>
#include "tcp_syn_monitor.h"
#include "monitor_log.h"
//#include "puzzle.h"

// Normal traffic of each ISP
int ISP_NORMAL_TRAFFIC[ISP_NUMBER] = { 10, 10, 5, 10, 10, 10, 10, 10, 10, 10 };
unsigned int DIFFICULTY[16] = {3000000, 1500000, 1000000, 500000, 210000, 110000, 55000, 30000, 20000, 6500, 3500, 2500, 2000, 1500, 1000, 500};
int DDOS_DETECTED = 0;

void
set_difficulty(int isp_id, int current)
{
	// TODO: implement this function - set the difficulty of the ISP
	//         according to the ISP_NORMAL_TRAFFIC and the current traffic
	
	int tmp = ISP_NORMAL_TRAFFIC[isp_id];
	int rise = 0;
	while (tmp <= current) {
		tmp *= 2;
		rise++;
		if (rise == 15) {
			break;
		}
	}

	unsigned int puzzle_threshold = DIFFICULTY[rise];
	char *isp_dns_ip_str;

	switch (isp_id) {
		case 0:
			isp_dns_ip_str = "192.168.0.12";
			break;
		case 1:
			isp_dns_ip_str = "192.168.0.13";
			break;
		case 2:
			isp_dns_ip_str = "192.168.0.14";
			break;
		case 3:
			isp_dns_ip_str = "192.168.0.15";
			break;
		case 4:
			isp_dns_ip_str = "192.168.0.16";
			break;
		case 5:
			isp_dns_ip_str = "192.168.0.17";
			break;
		case 6:
			isp_dns_ip_str = "192.168.0.18";
			break;
		case 7:
			isp_dns_ip_str = "192.168.0.19";
			break;
		case 8:
			isp_dns_ip_str = "192.168.0.10";
			break;
		case 9:
			isp_dns_ip_str = "192.168.0.11";
			break;
		default:
			isp_dns_ip_str = "";
	}

        unsigned int current_threshold = syscall(458, inet_addr(isp_dns_ip_str));
        printf("ISP%d current threshold: %d\n", isp_id, current_threshold);
        if (current_threshold == 0 || current_threshold > puzzle_threshold) {
                printf("\tISP%d (%s) threshold changed: %d -> %d\n", isp_id, isp_dns_ip_str, current_threshold, puzzle_threshold);
		int ret = syscall(459, inet_addr(isp_dns_ip_str), puzzle_threshold);
                if (ret != puzzle_threshold) {
                        printf("Set threshold error!\n");
                } else {
                        printf("Set threshold success!\n");
                }
        }
}

void
handle_ddos(cb_ptr buffer)
{
	for (int i = 0; i < ISP_NUMBER; i++) {
		int current_traffic = get_circular_buffer_isp_count(buffer, i);
		set_difficulty(i, current_traffic);
	}
}

int
detect_ddos(void)
{
	const int DDOS_THRESHOLD = 30;
	struct circular_buffer buffer;
	init_circular_buffer(&buffer);

	pcap_thread_data pcap_data;
	init_pcap_thread(&pcap_data, &buffer);

	start_pcap_thread(&pcap_data);

	printf("pcap thread started\n");
	printf("press any key to stop\n");
	printf("ISP number:\t");

	for (int i = 0; i < ISP_NUMBER; i++) {
		printf(",ISP%d\t", i);
	}

	printf("\n");

	log_thread_data log_data;
	log_data.buffer = &buffer;
	start_log_thread(&log_data);

	while (1) {
		if (get_circular_buffer_size(&buffer) > DDOS_THRESHOLD) {
			//printf("DDOS detected\n");
                        if (DDOS_DETECTED == 0) {
                                DDOS_DETECTED = 1;
                                //int pret = syscall(461, PZLTYPE_EXT);
                                int pret = syscall(461, 1);
                                //if (pret != PZLTYPE_EXT) {
                                if (pret != 1) {
                                        fprintf(stderr, "Set puzzle type error!\n");
                                }
                        }
			handle_ddos(&buffer);
		}

	}

	stop_log_thread(&log_data);

	stop_pcap_thread(&pcap_data);

	return 0;
}

void
start_detect_ddos_thread(pthread_t *tid)
{
	int ret = pthread_create(tid, NULL, (void*)detect_ddos, NULL);
	if (ret) {
		fprintf(stderr, "Error: detect ddos pthread_create() failed: (%d)\n", ret);
	}
}

void
stop_detect_ddos_thread(pthread_t *tid)
{
	int ret = pthread_join(*tid, NULL);
	if (ret) {
		fprintf(stderr, "Error: detect ddos pthread_join() failed: (%d)\n", ret);
	}
}
