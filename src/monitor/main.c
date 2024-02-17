#include <unistd.h>
#include "tcp_syn_monitor.h"

// Normal traffic of each ISP
int ISP_NORMAL_TRAFFIC[ISP_NUMBER] = { 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 };

void set_difficulty(int isp_id, int current, int normal)
{
    unsigned puzzle_threshold = ??? // TODO: determine puzzle threshold value using both traffic datas
    unsigned isp_dns_ip = ??? // TODO: mapping local dns ip using isp_id
    syscall(454, atoi(isp_dns_ip), puzzle_threshold);
	// TODO: implement this function - set the difficulty of the ISP
	//         according to the ISP_NORMAL_TRAFFIC and the current traffic
}

void handle_ddos(cb_ptr buffer)
{
	for (int i = 0; i < ISP_NUMBER; i++) {
		int current_traffic = get_circular_buffer_isp_count(buffer, i);
		set_difficulty(i, current_traffic, ISP_NORMAL_TRAFFIC[i]);
	}
}

int main()
{
	const int DDOS_THRESHOLD = 1000;
	struct circular_buffer buffer;
	init_circular_buffer(&buffer);

	pcap_thread_data data;
	init_pcap_thread(&data, &buffer);

	start_pcap_thread(&data);

	printf("pcap thread started\n");
	printf("press any key to stop\n");
	printf("ISP number:\t");

	for (int i = 0; i < ISP_NUMBER; i++) {
		printf("ISP%d\t", i);
	}

	printf("\n");

	while (1) {
		sleep(1);
		printf("ISP count:\t");

		if (get_circular_buffer_size(&buffer) > DDOS_THRESHOLD) {
			printf("DDOS detected\n");
            syscall(456, 1); // set puzzle type (): plz use this function unique when ddos detected
			handle_ddos(&buffer);
		}

		for (int i = 0; i < ISP_NUMBER; i++) {
			printf("%d\t", get_circular_buffer_isp_count(&buffer, i));
		}

		printf("\n");
	}

	stop_pcap_thread(&data);

	return 0;
}