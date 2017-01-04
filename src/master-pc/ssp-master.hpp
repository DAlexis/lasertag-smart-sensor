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

class FunctionRepeater
{
public:
	using Callback = std::function<void(void)>;
	FunctionRepeater(boost::asio::io_service& io);

	void run(Callback callback, unsigned int ms);
	void stop();
private:
	void startTimer();

	boost::asio::deadline_timer m_timer;
	unsigned int m_period = 0;
	Callback m_callback = nullptr;
};

class SSPMaster
{
public:
	SSPMaster(boost::asio::io_service& io, SerialPort& serial);

	void connectToCDriver();
	void start();
	SerialPort& serial();
	void requestIRDataCycle(unsigned int ms);

	static SSPMaster* sspMasterActive;
private:
	void startAsyncRead();
	void sendCommand(SSP_Command command);
	void messageCallback(const std::vector<uint8_t>& buffer);
	void parseSlaveToMaster(const std::vector<uint8_t>& buffer);
	void doReqIR();
	void doTick();
	void doScanIR();
	void doPushAnimTasks();

	SerialPort& m_serial;

	FunctionRepeater m_scanRepeater;
	FunctionRepeater m_tickRepeater;
	FunctionRepeater m_reqIRRepeater;
	FunctionRepeater m_pushAnimTask;

	unsigned int m_irReqPeriod = 0; // ms

};

#endif /* MASTER_PC_SSP_MASTER_HPP_ */
