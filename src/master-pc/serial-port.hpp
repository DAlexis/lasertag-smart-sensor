/*
 * serial-port.hpp
 *
 *  Created on: 28 дек. 2016 г.
 *      Author: dalexies
 */

#ifndef MASTER_PC_SERIAL_PORT_HPP_
#define MASTER_PC_SERIAL_PORT_HPP_

#include "signal-handler.hpp"

#include <boost/asio.hpp>
#include <vector>
#include <string>

class SerialPort : public IStoppableObject
{
public:
	using ReadDoneCallback = std::function<void(const std::vector<uint8_t>& buffer)>;
    SerialPort(boost::asio::io_service& io, std::string port, unsigned int baud_rate);
    void writeString(std::string s);
    void write(uint8_t* data, size_t size);

    void asyncReadLine(char stopChar, ReadDoneCallback callback);

    std::vector<uint8_t> readLine(char stopChar);
    std::string readLineStr(char stopChar);

    void stop() override;
private:
    void asyncReadNextByte();
    void byteReadedCallback(
    		const boost::system::error_code& error,
    		std::size_t bytes_transferred
    );
    boost::asio::io_service& m_io;
    boost::asio::serial_port m_serial;
    std::vector<uint8_t> m_readBuffer;
    uint8_t m_nextChar = 0;
    uint8_t m_stopChar = 0;
    ReadDoneCallback m_rxCallback;
};







#endif /* MASTER_PC_SERIAL_PORT_HPP_ */
