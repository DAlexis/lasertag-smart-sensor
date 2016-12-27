/*
 * ssp-sensor-part.h
 *
 *  Created on: 27 дек. 2016 г.
 *      Author: dalexies
 */

#ifndef INC_SSP_SENSOR_PART_H_
#define INC_SSP_SENSOR_PART_H_

#include "ssp.h"
#include "ssp-config.h"

void SSP_S2M_Header_Struct_Init(SSP_S2M_Header* header);
void SSP_Debug_Header_Init(SSP_Debug_Header* header);
void SSP_Footer_Init(SSP_Footer* footer);
void SSP_Debug_Footer_Init(SSP_Debug_Footer* footer);

void ssp_init(void);

void ssp_receive(uint8_t* data, uint16_t size);

void ssp_send_debug_msg(char *ptr, int len);


#endif /* INC_SSP_SENSOR_PART_H_ */
