/*
 * rgb.c
 *
 *  Created on: 4 янв. 2017 г.
 *      Author: dalexies
 */

#include "rgb.h"
#include "tim.h"

#include "main.h"

void init_rgb()
{
//	MX_TIM3_Init();
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);

	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 128); //sets the PWM duty cycle (Capture Compare Value)
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 128); //sets the PWM duty cycle (Capture Compare Value)
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 128); //sets the PWM duty cycle (Capture Compare Value)
}

void set_rgb_intensity(uint8_t r, uint8_t g, uint8_t b)
{
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, r);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, g);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, b);
}

void set_vibro(uint8_t v)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, v != 0 ? RESET : SET);
}
