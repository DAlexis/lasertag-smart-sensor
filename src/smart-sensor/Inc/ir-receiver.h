/*
 * ir-receiver.h
 *
 *  Created on: 25 дек. 2016 г.
 *      Author: dalexies
 */

#ifndef INC_IR_RECEIVER_H_
#define INC_IR_RECEIVER_H_

#include <stdint.h>

#define INCOMING_BUFFER_SIZE	100

typedef struct {
	uint8_t buffer[INCOMING_BUFFER_SIZE];
	uint8_t size;
} IR_Data_Buffer;

void ir_receiver_init(void);

uint8_t ir_is_data_ready(void);

IR_Data_Buffer* ir_get_data(void);

/// This function should be called in main loop frequently
void ir_task_tick();

#endif /* INC_IR_RECEIVER_H_ */
