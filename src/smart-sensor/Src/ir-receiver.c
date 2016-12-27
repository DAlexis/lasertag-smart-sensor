/*
 * ir-receiver.c
 *
 *  Created on: 25 дек. 2016 г.
 *      Author: dalexies
 */

#include "ir-receiver.h"
#include "ir-timings.h"
#include "tim.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef enum {
	RS_WAITING_HEADER = 0,
	RS_WAITING_HEADER_END,
	RS_WAITING_BIT,
	RS_WAITING_SPACE,
} Receiver_State;

// private functions prototypes
static uint32_t ticks_to_us(uint16_t ticks_interval);
static void reset_receiver(void);
static void add_bit(uint8_t bit);

// private variables
static IR_Data_Buffer temporary_buffer;
static IR_Data_Buffer user_buffer;

static uint8_t data_ready = 0;

static uint16_t last_edge_ticks = 0;
static uint32_t last_interval_ms = 0;

static Receiver_State receiver_state = RS_WAITING_HEADER;



void ir_receiver_init(void)
{
	MX_TIM14_Init();
	HAL_TIM_IC_Start_IT(&htim14, TIM_CHANNEL_1);
}

uint8_t ir_is_data_ready(void)
{
	return data_ready;
}

IR_Data_Buffer* ir_get_data(void)
{
	data_ready = 0;
	return &user_buffer;
}

void ir_task_tick()
{
	uint16_t ticks_count = htim14.Instance->CNT;
	uint16_t ticks_diff = ticks_count - last_edge_ticks;
	if (receiver_state == RS_WAITING_HEADER)
	{
		last_edge_ticks = ticks_count;
	} else if (ticks_to_us(ticks_diff) > IR_MSG_END_DELAY)
	{
		memcpy(&user_buffer, &temporary_buffer, sizeof(temporary_buffer));
		data_ready = 1;
		reset_receiver();
	}
}

uint32_t ticks_to_us(uint16_t ticks_interval)
{
	return ticks_interval * 7.6295;
}

void reset_receiver(void)
{
	temporary_buffer.size = 0;
	receiver_state = RS_WAITING_HEADER;
}

void add_bit(uint8_t bit)
{
	uint8_t bit_mask = 1 << (7 - temporary_buffer.size%8);

	if (bit)
		temporary_buffer.buffer[temporary_buffer.size/8] |= bit_mask;
	else
		temporary_buffer.buffer[temporary_buffer.size/8] &= ~bit_mask;

	temporary_buffer.size++;
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Channel != HAL_TIM_ACTIVE_CHANNEL_1)
		return; // We should not reach this return

	uint16_t capture_time = htim->Instance->CCR1;
	uint8_t level = ! HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4);
	uint32_t interval_ms = ticks_to_us(capture_time - last_edge_ticks);

	switch(receiver_state)
	{
	case RS_WAITING_HEADER:
		if (level == 1) {
			receiver_state = RS_WAITING_HEADER_END;
		} else {
			reset_receiver();
		}
		break;
	case RS_WAITING_HEADER_END:
		if (level == 0) {
			receiver_state = RS_WAITING_BIT;
		} else {
			reset_receiver();
		}
		break;
	case RS_WAITING_BIT:
		if (level == 1) {
			receiver_state = RS_WAITING_SPACE;
		}  else {
			reset_receiver();
		}
		break;
	case RS_WAITING_SPACE:
		if (level == 0) {
			// Now 1 bit received. We can parse
			uint32_t time_with_prev_space = interval_ms + last_interval_ms;
			if (time_with_prev_space > IR_GAP_MIN + IR_ZERO_MIN
					&& time_with_prev_space < IR_GAP_MAX + IR_ZERO_MAX)
			{
				add_bit(0);
				receiver_state = RS_WAITING_BIT;
			} else if (time_with_prev_space > IR_GAP_MIN + IR_ONE_MIN
					&& time_with_prev_space < IR_GAP_MAX + IR_ONE_MAX)
			{
				add_bit(1);
				receiver_state = RS_WAITING_BIT;
			}  else {
				reset_receiver();
			}
		} else {
			reset_receiver();
		}
		break;
	}

	last_edge_ticks = capture_time;
	last_interval_ms = interval_ms;
}
