/*
 * uart-transiver.h
 *
 *  Created on: 27 дек. 2016 г.
 *      Author: dalexies
 */

#ifndef INC_UART_TRANSIVER_H_
#define INC_UART_TRANSIVER_H_

#include <stdint.h>

#define UART_TRANSIVER_BUFFER_SIZE    100

typedef struct {
	uint8_t buffer[UART_TRANSIVER_BUFFER_SIZE];
	uint16_t size;
} Transiver_Buffer;

void transiver_init(void);
void transiver_send_data(uint8_t* data, uint16_t size);
void transiver_task_tick();

uint8_t transiver_is_input_data_ready(void);
Transiver_Buffer* transiver_get_input_data(void);

uint8_t transiver_is_ready_to_output(void);
Transiver_Buffer* transiver_get_input_data(void);

#endif /* INC_UART_TRANSIVER_H_ */
