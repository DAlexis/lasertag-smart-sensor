#include "serial-port.hpp"
#include <iostream>
using namespace std;

SerialPort::SerialPort(boost::asio::io_service& io, std::string port, unsigned int baud_rate) :
	m_io(io), m_serial(io,port)
{
	m_serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
	SignalHandler::instance().addStoppable(this);
}


void SerialPort::writeString(std::string s)
{
	boost::asio::write(m_serial,boost::asio::buffer(s.c_str(),s.size()));
}

void SerialPort::asyncReadLine(char stopChar, ReadDoneCallback callback)
{
	m_rxCallback = callback;
	m_stopChar = stopChar;
	m_readBuffer.clear();
	asyncReadNextByte();
}

std::vector<uint8_t> SerialPort::readLine(char stopChar)
{
	//Reading data char by char, code is optimized for simplicity, not speed
	using namespace boost;
	std::vector<uint8_t> result;
	uint8_t c;
	do
	{
		asio::read(m_serial,asio::buffer(&c,1));
		result.push_back(c);
	}
	while(c != stopChar);
	return result;
}

std::string SerialPort::readLineStr(char stopChar)
{
	std::vector<uint8_t> v = readLine(stopChar);
	std::string str(v.begin(), v.end());
	return str;
}

void SerialPort::stop()
{
	m_serial.cancel();
}

void SerialPort::asyncReadNextByte()
{
	m_serial.async_read_some(
		boost::asio::buffer(&m_nextChar, 1),
		[this](
				const boost::system::error_code& error, // Result of operation.
				std::size_t bytes_transferred
		)
		{
			byteReadedCallback(error, bytes_transferred);
		}
	);
}

void SerialPort::byteReadedCallback(
		const boost::system::error_code& error,
		std::size_t bytes_transferred
)
{
	m_readBuffer.push_back(m_nextChar);
	if (m_nextChar == m_stopChar)
	{
		m_rxCallback(m_readBuffer);
	} else {
		asyncReadNextByte();
	}
}
