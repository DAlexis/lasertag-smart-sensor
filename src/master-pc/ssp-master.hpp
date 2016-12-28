/*
 * ssp-master.hpp
 *
 *  Created on: 28 дек. 2016 г.
 *      Author: dalexies
 */

#ifndef MASTER_PC_SSP_MASTER_HPP_
#define MASTER_PC_SSP_MASTER_HPP_

#include "serial-port.hpp"

class SSPMaster
{
public:
	SSPMaster(SerialPort& serial);

	void startAsyncReading();

private:
	void messageCallback(const std::vector<uint8_t>& buffer);
	SerialPort& m_serial;

};

#endif /* MASTER_PC_SSP_MASTER_HPP_ */
