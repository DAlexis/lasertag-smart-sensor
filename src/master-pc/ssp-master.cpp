#include "ssp-master.hpp"
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

SSPMaster::SSPMaster(boost::asio::io_service& io, SerialPort& serial) :
	m_timer(io),
	m_serial(serial)
{

}


void SSPMaster::startAsyncReading()
{
	m_serial.asyncRead([this](
			const std::vector<uint8_t>& buffer) {	messageCallback(buffer); },
			10
	);

}

void SSPMaster::requestIRData()
{
	cout << "==========" << endl;
	cout << "Requesting IR data" << endl;
	cout << "==========" << endl;

	sendCommand(SSP_M2S_GET_IR_BUFFER);
}

void SSPMaster::requestIRDataCycle(unsigned int ms)
{
	m_irReqPeriod = ms;
	m_timer.expires_from_now(boost::posix_time::milliseconds(ms));
	m_timer.async_wait([this](const boost::system::error_code& err) { timerReqIRCallback(err); });
}

void SSPMaster::sendCommand(Sensor_Command command)
{
	SSP_M2S_Header package;
	package.start_byte = SSP_START_BYTE_M2S;
	package.target = 123;
	package.command = command;

	m_serial.write(reinterpret_cast<uint8_t*> (&package), sizeof(package));
}

void SSPMaster::messageCallback(const std::vector<uint8_t>& buffer)
{
	if (!buffer.empty())
	{
		string str(buffer.begin(), buffer.end());
		cout << "==========" << endl;
		cout << "Bus message: " << str << endl;
		switch (buffer[0])
		{
		case SSP_START_BYTE_M2S:
			cout << "Message type: master to slave" << endl;
			break;
		case SSP_START_BYTE_DEBUG:
			cout << "Message type: debug" << endl;
			break;
		case SSP_START_BYTE_S2M:
			cout << "Message type: slave to master" << endl;
			parseSlaveToMaster(buffer);
			break;
		}
		cout << "==========" << endl;
	}

	startAsyncReading();
}

void SSPMaster::parseSlaveToMaster(const std::vector<uint8_t>& buffer)
{
	SSP_S2M_Header *header;
	if (buffer.size() < sizeof(*header))
	{
		cout << "Error: buffer.size() < size of msg header" << endl;
		return;
	}

	cout << "Package size: " << header->package_size << endl;
	if (buffer.size() < sizeof(*header) + header->package_size)
	{
		cout << "Error: buffer.size() < sizeof(*header) + header->package_size + sizeof(SSP_Footer)" << endl;
		return;
	}

	if (header->package_type == SSP_S2M_PACKAGE_TYPE_IR_DATA)
	{
		cout << "Type: IR data" << endl;
		const uint8_t *data = buffer.data() + sizeof(header);
		cout << "Data: " << ios::hex;
		for (int i = 0; i<header->package_size; i++)
		{
			cout << data[i] << " " << endl;
		}
		cout << ios::dec << endl;
	}
}

void SSPMaster::timerReqIRCallback(const boost::system::error_code& err)
{
	requestIRData();
	requestIRDataCycle(m_irReqPeriod);
}
