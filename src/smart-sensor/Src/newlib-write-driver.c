#include "ssp-slave.h"

void write_to_uart(char *ptr, int len)
{
	ssp_send_debug_msg(ptr, len);
}
