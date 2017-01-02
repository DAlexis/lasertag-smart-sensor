/*
 * smart-sensor.c
 *
 *  Created on: 25 дек. 2016 г.
 *      Author: dalexies
 */

#include "smart-sensor.h"
#include "ir-receiver.h"
#include "stm32f0xx_hal.h"
#include "ssp-sensor-part.h"

#include <math.h>

void smart_sensor_init(void)
{
	ssp_sensor_init();
}

void smart_sensor_main_loop(void)
{
	printf("main loop\n");
	for (;;)
	{
		ir_task_tick();
		//transiver_task_tick();
		ssp_sensor_task_tick();
		/*
		if (ir_is_data_ready())
		{
			IR_Data_Buffer* data = ir_get_data();
			for (int i=0; i<ceil(data->size / 8); i++)
			{
				printf("%x ", (unsigned int) data->buffer[i]);
			}
			printf("\n");
		}*/
	}
}
