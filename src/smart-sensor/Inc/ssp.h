/*
 * ssp.h
 *
 *  Created on: 27 дек. 2016 г.
 *      Author: dalexies
 */

#ifndef INC_SSP_H_
#define INC_SSP_H_

/** SSP - Smart Sensor Protocol - protocol for communication
 * with lasertag smart sensor, this device.
 * "M2S" means Master To Sensor, "S2M" vice versa
 */

#include <stdint.h>

#ifdef __cplusplus
	extern "C" {
#endif

typedef uint16_t Sensor_Address;
typedef uint16_t Sensor_Command;
typedef uint8_t S2M_Package_Type;
typedef uint16_t S2M_Package_Size;

#pragma pack(push, 1)
// Package from master to sensor
typedef struct {
	uint8_t start_byte;
	Sensor_Address target;
	Sensor_Command command;
	uint8_t stop_byte;
} SSP_M2S_Package;

// Header of packager from sensor to master
typedef struct {
	uint8_t start_byte; // should be SSP_START_BYTE_S2M
	S2M_Package_Type package_type;
	S2M_Package_Size package_size;
} SSP_S2M_Header;

// Debug info (no real data, simply text for human)
typedef struct {
	uint8_t start_byte;
} SSP_Debug_Header;

typedef struct {
	uint8_t stop_byte;
} SSP_Footer;

typedef SSP_Footer SSP_Debug_Footer;

#pragma pack(pop)

#define SSP_STOP_BYTE                   ((uint8_t) '<')

#define SSP_START_BYTE_M2S              ((uint8_t) 'M')
#define SSP_START_BYTE_S2M              ((uint8_t) 'S')
#define SSP_START_BYTE_DEBUG            ((uint8_t) 'D')

#define SSP_BROADCAST_ADDRESS           ((Sensor_Address) 0xFFFF)

#define SSP_M2S_GET_IR_BUFFER           ((Sensor_Command) 'I')
#define SSP_M2S_ENABLE_RED_LED          ((Sensor_Command) 'R')
#define SSP_M2S_ENABLE_GREEN_LED        ((Sensor_Command) 'G')
#define SSP_M2S_ENABLE_BLUE_LED         ((Sensor_Command) 'B')

#define SSP_S2M_PACKAGE_TYPE_NOPE       ((S2M_Package_Type) 'n')
#define SSP_S2M_PACKAGE_TYPE_IR_DATA    ((S2M_Package_Type) 'i')

#ifdef __cplusplus
	} // end of extern "C"
#endif

#endif /* INC_SSP_H_ */
