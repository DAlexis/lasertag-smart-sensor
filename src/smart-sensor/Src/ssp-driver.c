/*
 * ssp-driver.c
 *
 *  Created on: 30 дек. 2016 г.
 *      Author: dalexies
 */

#include "ssp-slave-driver.h"
#include "usart.h"
#include "ir-receiver.h"
#include "precision-clock.h"

#include <stdint.h>

static uint8_t incoming = 0;
static uint8_t sending_now = 0;

static void receive_next_byte_it(void)
{
	HAL_HalfDuplex_EnableReceiver(&huart1);
	HAL_UART_Receive_IT(&huart1, &incoming, 1);
}

void ssp_drivers_init(void)
{
	MX_USART1_UART_Init_With_Speed(SSP_UART_SPEED);
	receive_next_byte_it();
	ir_receiver_init();
}

void ssp_send_data(uint8_t* data, uint16_t size)
{
	//HAL_UART_AbortReceive_IT(&huart1);
	sending_now = 1;
	HAL_HalfDuplex_EnableTransmitter(&huart1);
	HAL_UART_Transmit(&huart1, data, size, HAL_MAX_DELAY);
	sending_now = 0;
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

uint32_t ssp_get_ticks()
{
	//return prec_clock_ticks();
	return HAL_GetTick();
}

SSP_Address ssp_self_address()
{
	return (DBGMCU->IDCODE) & 0xFFFF;
}

// Interrupt callbacks

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	UNUSED(huart);
	if (sending_now == 0)
	{
		ssp_receive_byte(incoming);
		receive_next_byte_it();
	}
}


