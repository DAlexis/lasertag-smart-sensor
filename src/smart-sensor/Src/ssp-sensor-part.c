#include "ssp-sensor-part.h"
#include "uart-transiver.h"
#include "ir-receiver.h"

// private functions prototypes
void parse_package(SSP_M2S_Package *incoming);
//uint8_t is_bus_idle(void);
void send_ir_data(void);

// private variables
//static uint32_t last_msg_time = 0;

void SSP_S2M_Header_Struct_Init(SSP_S2M_Header* header)
{
	header->start_byte = SSP_START_BYTE_S2M;
	header->package_size = 0;
	header->package_type = SSP_S2M_PACKAGE_TYPE_NOPE;
}

void SSP_Debug_Header_Init(SSP_Debug_Header* header)
{
	header->start_byte = SSP_START_BYTE_DEBUG;
}

void SSP_Footer_Init(SSP_Footer* footer)
{
	footer->stop_byte = SSP_STOP_BYTE;
}

void SSP_Debug_Footer_Init(SSP_Debug_Footer* footer)
{
	footer->stop_byte = SSP_STOP_BYTE;
}

void ssp_init(void)
{
	ir_receiver_init();
	transiver_init();
}

void ssp_receive(uint8_t* data, uint16_t size)
{
	if (size != sizeof(SSP_M2S_Package))
		return; // Package size is invalid

	SSP_M2S_Package *incoming = (SSP_M2S_Package *) data;
	if (incoming->stop_byte != SSP_STOP_BYTE)
		return;  // Package stop byte is invalid

	if (incoming->start_byte != SSP_START_BYTE_M2S)
		return; // Package is not for sensor

	if (incoming->target != SSP_BROADCAST_ADDRESS && incoming->target != SSP_SELF_ADDRESS)
		return; // Package not for me

	// Now we have valid package! We can parse it
	parse_package(incoming);
}

void ssp_send_debug_msg(char *ptr, int len)
{
	SSP_Debug_Header header;
	SSP_Debug_Footer footer;
	SSP_Debug_Header_Init(&header);
	SSP_Debug_Footer_Init(&footer);

	transiver_send_data((uint8_t*) &header, sizeof(header));
	transiver_send_data((uint8_t*) ptr, len);
	transiver_send_data((uint8_t*) &footer, sizeof(footer));
}

void parse_package(SSP_M2S_Package *incoming)
{
	switch(incoming->command)
	{
	case SSP_M2S_GET_IR_BUFFER: send_ir_data(); return;
	}
}

void send_ir_data(void)
{
	SSP_S2M_Header header;
	SSP_Footer footer;
	SSP_S2M_Header_Struct_Init(&header);
	SSP_Footer_Init(&footer);
	header.package_type = SSP_S2M_PACKAGE_TYPE_IR_DATA;

	if (ir_is_data_ready())
	{
		// We have IR data, so sending it via SSP
		IR_Data_Buffer* ird = ir_get_data();
		header.package_size = ird->size;
		transiver_send_data((uint8_t*) &header, sizeof(header));
		transiver_send_data(ird->buffer, ird->size);
	} else {
		header.package_size = 0;
		transiver_send_data((uint8_t*) &header, sizeof(header));
	}
	transiver_send_data((uint8_t*) &footer, sizeof(footer));
}

void write_to_uart(char *ptr, int len)
{
	ssp_send_debug_msg(ptr, len);
}
