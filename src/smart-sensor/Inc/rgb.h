/*
 * rgb.h
 *
 *  Created on: 4 янв. 2017 г.
 *      Author: dalexies
 */

#ifndef SMART_SENSOR_SRC_RGB_H_
#define SMART_SENSOR_SRC_RGB_H_

#include <stdint.h>

void init_rgb();
void set_rgb_intensity(uint8_t r, uint8_t g, uint8_t b);
void set_vibro(uint8_t v);

#endif /* SMART_SENSOR_SRC_RGB_H_ */
