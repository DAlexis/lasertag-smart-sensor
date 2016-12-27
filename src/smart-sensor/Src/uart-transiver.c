/*
 * uart-transiver.c
 *
 *  Created on: 27 дек. 2016 г.
 *      Author: dalexies
 */

#include "uart-transiver.h"

#include "usart.h"

#include <string.h>

#define STOP_CHAR		((uint8_t) '<')

static Transiver_Buffer input_buffer;
static Transiver_Buffer user_buffer;
static uint8_t data_ready;

typedef enum {
	TS_IDLE = 0,
} Transiver_State;

void reset_buffer()
{
	input_buffer.size = 0;
}

void begin_data_listening()
{
	reset_buffer();
	HAL_UART_Receive_IT(&huart1, &(input_buffer.buffer[0]), 1);
}

void stop_data_listening()
{
	HAL_UART_AbortReceive_IT(&huart1);
}

void transiver_init(void)
{
	memset(&input_buffer, 0, sizeof(input_buffer));
	memset(&user_buffer, 0, sizeof(user_buffer));
	MX_USART1_UART_Init();
	begin_data_listening();
}

void transiver_send_data(uint8_t* data, uint16_t size)
{
	stop_data_listening();
	HAL_UART_Transmit(&huart1, data, size, HAL_MAX_DELAY);
	begin_data_listening();
}

void transiver_task_tick()
{
	if ((input_buffer.buffer[input_buffer.size-1] == STOP_CHAR))
	{
		memcpy(&user_buffer, &input_buffer, sizeof(user_buffer));
		data_ready = 1;
		stop_data_listening();
	} else {
		data_ready = 0;
	}
}

uint8_t transiver_is_input_data_ready(void)
{
	return data_ready;
}

Transiver_Buffer* transiver_get_input_data(void)
{
	return &user_buffer;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	UNUSED(huart);
	HAL_UART_Receive_IT(&huart1, &(input_buffer.buffer[input_buffer.size++]), 1);
}


