#include "ssp-master.hpp"
#include "../smart-sensor/Inc/ssp.h"
#include <iostream>
#include <string>

using namespace std;

SSPMaster::SSPMaster(SerialPort& serial) :
	m_serial(serial)
{

}


void SSPMaster::startAsyncReading()
{
	m_serial.asyncReadLine('<', [this](const std::vector<uint8_t>& buffer) {
		messageCallback(buffer);
    });
}

void SSPMaster::messageCallback(const std::vector<uint8_t>& buffer)
{
	string str(buffer.begin(), buffer.end());
	cout << "==========" << endl;
	cout << "Bus message: " << str << endl;

	startAsyncReading();
}
