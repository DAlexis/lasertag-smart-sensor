#include "serial-port.hpp"
#include <iostream>
using namespace std;

SerialPort::SerialPort(boost::asio::io_service& io, std::string port, unsigned int baud_rate) :
	m_io(io), m_timeoutTimer(m_io), m_serial(io,port)
{
	m_serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
	SignalHandler::instance().addStoppable(this);
}

void SerialPort::write(uint8_t* data, size_t size)
{
	m_writtenBytes = size;
	boost::asio::write(m_serial,boost::asio::buffer(data, size));
}

void SerialPort::asyncRead(ReadDoneCallback callback, size_t timeout)
{
	m_timeout = timeout;
	m_rxCallback = callback;
	m_readBuffer.clear();
	asyncReadNextByte();
}

void SerialPort::asyncReadPerByte(ReadByteDoneCallback callback)
{
	uint8_t byte = 0;
	m_serial.cancel();
	m_serial.async_read_some(
		boost::asio::buffer(&byte, 1),
		[this, callback, &byte](
				const boost::system::error_code& error, // Result of operation.
				std::size_t bytes_transferred
		)
		{
			if (error == boost::asio::error::operation_aborted)
				return;

			cout << byte;
			callback(byte);
			asyncReadPerByte(callback);
		}
	);
}

void SerialPort::stop()
{
	m_serial.cancel();
	m_timeoutTimer.cancel();
}

void SerialPort::asyncReadNextByte(bool needTimeout)
{
	m_serial.async_read_some(
		boost::asio::buffer(&m_nextChar, 1),
		[this](
				const boost::system::error_code& error, // Result of operation.
				std::size_t bytes_transferred
		)
		{
			if (error == boost::asio::error::operation_aborted)
				return;
			byteReadedCallback(error, bytes_transferred);
		}
	);
	m_timeoutTimer.cancel();
	if (needTimeout)
	{
		m_timeoutTimer.expires_from_now(boost::posix_time::milliseconds(m_timeout));
		m_timeoutTimer.async_wait(
			[this](const boost::system::error_code& err)
			{
				if (err == boost::asio::error::operation_aborted)
					return;

				m_serial.cancel();

				std::vector<uint8_t> result(m_readBuffer.begin()+m_writtenBytes, m_readBuffer.end());
				m_writtenBytes = 0;
				m_rxCallback(result);
			}
		);
	}
}

void SerialPort::byteReadedCallback(
		const boost::system::error_code& error,
		std::size_t bytes_transferred
)
{
	if (bytes_transferred == 0)
		return;
	m_timeoutTimer.cancel();
	m_readBuffer.push_back(m_nextChar);
	asyncReadNextByte();
}
