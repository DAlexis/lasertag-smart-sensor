/*
 * ssp-driver.cpp
 *
 *  Created on: 1 янв. 2017 г.
 *      Author: dalexies
 */

#include "../smart-sensor/lasertag-ssp/ssp/Inc/ssp-driver.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <string>

void ssp_drivers_init(void)
{
}

uint32_t ssp_get_time_ms()
{
	static boost::posix_time::ptime startTime = boost::posix_time::second_clock::local_time();

	boost::posix_time::ptime nowTime = boost::posix_time::second_clock::local_time();
	boost::posix_time::time_duration diff = nowTime - startTime;
	return diff.total_milliseconds();
}


void ssp_send_data(uint8_t* data, uint16_t size)
{
}

uint8_t ssp_is_ir_data_ready(void)
{
	return 0;
}

void ssp_get_ir_data(uint8_t** data, uint16_t* size)
{
}

void ssp_write_debug(uint8_t* data, uint16_t size)
{
	std::string s(data, data+size-1);
	std::cout << "Debug channel: " << s << std::endl;
}
