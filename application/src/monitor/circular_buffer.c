#include "circular_buffer.h"

void
init_circular_buffer(cb_ptr buffer)
{
	for (int i = 0; i < ISP_NUMBER; i++) {
		buffer->isp_count[i] = 0;
	}
	buffer->_start = 0;
	buffer->_end = 0;
	buffer->_size = 0;
}

bool
is_circular_buffer_empty(cb_ptr buffer)
{
	return buffer->_size == 0;
}

bool
is_circular_buffer_full(cb_ptr buffer)
{
	return buffer->_size == MAX_BUFFER_SIZE;
}

void
push_circular_buffer(cb_ptr buffer, struct packet_info pi)
{
	if (is_circular_buffer_full(buffer)) {
		buffer->_start = (buffer->_start + 1) % MAX_BUFFER_SIZE;
	}

	buffer->_data[buffer->_end] = pi;
	buffer->isp_count[pi.isp_id]++;
	buffer->_end = (buffer->_end + 1) % MAX_BUFFER_SIZE;
	buffer->_size++;
}

void
pop_circular_buffer(cb_ptr buffer)
{
	if (!is_circular_buffer_empty(buffer)) {
		buffer->isp_count[buffer->_data[buffer->_start].isp_id]--;
		buffer->_start = (buffer->_start + 1) % MAX_BUFFER_SIZE;
		buffer->_size--;
	}
}

int
get_circular_buffer_size(cb_ptr buffer)
{
	return buffer->_size;
}

struct packet_info
get_circular_buffer_front(cb_ptr buffer)
{
	return buffer->_data[buffer->_start];
}

int
get_circular_buffer_isp_count(cb_ptr buffer, int isp_id)
{
	return buffer->isp_count[isp_id];
}
