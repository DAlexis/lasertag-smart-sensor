/*
 * smart-sensor.c
 *
 *  Created on: 25 дек. 2016 г.
 *      Author: dalexies
 */

#include "smart-sensor.h"
#include "ir-receiver.h"
#include "stm32f0xx_hal.h"
#include "ssp-slave.h"
#include "rgb.h"

#include <math.h>

void smart_sensor_init(void)
{
	ssp_sensor_init();
	init_rgb();
}

void smart_sensor_main_loop(void)
{
	printf("main loop\n");
	uint8_t r, g, b, v;
	for (;;)
	{
		ir_task_tick();
		//transiver_task_tick();
		ssp_sensor_task_tick();
		ssp_get_leds_vibro_state(&r, &g, &b, &v);
		set_rgb_intensity(r, g, b);
		set_vibro(v);
	}
}
