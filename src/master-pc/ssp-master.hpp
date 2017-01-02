/*
 * ssp-master.hpp
 *
 *  Created on: 28 дек. 2016 г.
 *      Author: dalexies
 */

#ifndef MASTER_PC_SSP_MASTER_HPP_
#define MASTER_PC_SSP_MASTER_HPP_

#include "serial-port.hpp"
#include "../smart-sensor/lasertag-ssp/ssp/Inc/ssp.h"

class SSPMaster
{
public:
	SSPMaster(boost::asio::io_service& io, SerialPort& serial);

	void connectToCDriver();
	void start();
	SerialPort& serial();

	static SSPMaster* sspMasterActive;
private:
	void sendCommand(SSP_Command command);
	void messageCallback(const std::vector<uint8_t>& buffer);
	void parseSlaveToMaster(const std::vector<uint8_t>& buffer);
	void timerReqIRCallback(const boost::system::error_code& err);
	void doTick();
	void setupTickTimer();

	SerialPort& m_serial;
	boost::asio::deadline_timer m_timer;
	boost::asio::deadline_timer m_timerTick;
	unsigned int m_irReqPeriod = 0; // ms

};

#endif /* MASTER_PC_SSP_MASTER_HPP_ */
