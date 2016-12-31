/*
 * ssp-driver.c
 *
 *  Created on: 30 дек. 2016 г.
 *      Author: dalexies
 */

#include "ssp-driver.h"
#include "usart.h"
#include "ir-receiver.h"
#include "ssp-sensor-part.h"

#include <stdint.h>

static uint8_t incoming = 0;

static void receive_next_byte_it(void)
{
	HAL_UART_Receive_IT(&huart1, &incoming, 1);
}

void ssp_drivers_init(void)
{
	MX_USART1_UART_Init();
	receive_next_byte_it();
}

void ssp_send_data(uint8_t* data, uint16_t size)
{
	HAL_UART_AbortReceive_IT(&huart1);
	HAL_UART_Transmit(&huart1, data, size, HAL_MAX_DELAY);
	receive_next_byte_it();
}

uint8_t ssp_is_ir_data_ready(void)
{
	return ir_is_data_ready();
}

void ssp_get_ir_data(uint8_t** data, uint16_t* size)
{
	*data = ir_get_data()->buffer;
	*size = ir_get_data()->size;
}

uint32_t ssp_get_time_ms()
{
	return HAL_GetTick();
}

void write_to_uart(char *ptr, int len)
{
	ssp_send_debug_msg(ptr, len);
}

// Interrupt callbacks

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	UNUSED(huart);
	ssp_receive_byte(incoming);
	receive_next_byte_it();
}

