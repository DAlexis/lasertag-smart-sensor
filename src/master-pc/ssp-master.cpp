#include "ssp-master.hpp"
#include "../smart-sensor/lasertag-ssp/ssp/Inc/ssp-driver.h"
#include "../smart-sensor/lasertag-ssp/ssp/Inc/ssp-master-part.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/shared_array.hpp>

#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

SSPMaster* SSPMaster::sspMasterActive = nullptr;


FunctionRepeater::FunctionRepeater(boost::asio::io_service& io) :
		m_timer(io)
{

}

void FunctionRepeater::run(Callback callback, unsigned int ms)
{
	m_callback = callback;
	m_period = ms;
	startTimer();
}

void FunctionRepeater::stop()
{
	m_timer.cancel();
}

void FunctionRepeater::startTimer()
{
	m_timer.expires_from_now(boost::posix_time::milliseconds(m_period));
	m_timer.async_wait([this](const boost::system::error_code& err)
		{
			if (err != boost::asio::error::operation_aborted)
			{
				startTimer();
				m_callback();
			}
		}
	);
}


SSPMaster::SSPMaster(boost::asio::io_service& io, SerialPort& serial) :
		m_serial(serial),
		m_scanRepeater(io),
		m_tickRepeater(io),
		m_reqIRRepeater(io),
		m_pushAnimTask(io)
{

}

void SSPMaster::connectToCDriver()
{
	sspMasterActive = this;
}

void SSPMaster::start()
{
	//m_serial.asyncReadPerByte([](uint8_t byte){ ssp_receive_byte(byte); });
	m_tickRepeater.run([this]{ doTick(); }, 10);
	m_scanRepeater.run([this]{ doScanIR(); }, 10);
	m_pushAnimTask.run([this]{ doPushAnimTasks(); }, 5000);
	startAsyncRead();
}

void SSPMaster::startAsyncRead()
{
	//m_serial.asyncReadPerByte(ssp_receive_byte);

	m_serial.asyncRead([this](
		const std::vector<uint8_t>& buffer) {	messageCallback(buffer); },
		10
	);
}

void SSPMaster::messageCallback(const std::vector<uint8_t>& buffer)
{
	if (!buffer.empty())
	{
		string str(buffer.begin(), buffer.end());
		cout << "==========" << endl;
		cout << "Bus message: " << str << endl;
		cout << "==========" << endl;
		for (auto it : buffer)
		{
			ssp_receive_byte(it);
		}
	}

	startAsyncRead();
}

void SSPMaster::doReqIR()
{
	ssp_push_ir_request(123);
	startAsyncRead();
}

void SSPMaster::doTick()
{
	ssp_master_task_tick();
}

void SSPMaster::doScanIR()
{
	SSP_IR_Buffer* ir = ssp_get_next_ir_buffer();
	if (ir != nullptr)
	{
		cout << "IR buffer received" << endl;
		cout << "Sender: " << ir->sensor_address << endl;
		cout << "Data: " << ios::hex;
		for (int i=0; i<ir->size; i++)
		{
			cout << ir->data[i];
		}
		cout << ios::dec << endl;
	}
}

void SSPMaster::doPushAnimTasks()
{
	cout << "Pushing tasks" << endl;
	SSP_Sensor_Animation_Task task;
	task.state.red = 0;
	task.state.green = 0;
	task.state.blue = 0;
	task.state.vibro = 0;
	task.ms_from_last_state = 500;
	ssp_push_animation_task(123, &task);

	task.state.red = 255;
	task.state.green = 0;
	task.state.blue = 0;
	task.state.vibro = 0;
	task.ms_from_last_state = 500;
	ssp_push_animation_task(123, &task);

	task.state.red = 0;
	task.state.green = 255;
	task.state.blue = 0;
	task.state.vibro = 0;
	task.ms_from_last_state = 500;
	ssp_push_animation_task(123, &task);

	task.state.red = 0;
	task.state.green = 0;
	task.state.blue = 255;
	task.state.vibro = 0;
	task.ms_from_last_state = 500;
	ssp_push_animation_task(123, &task);

	task.state.red = 0;
	task.state.green = 0;
	task.state.blue = 0;
	task.state.vibro = 0;
	task.ms_from_last_state = 500;
	ssp_push_animation_task(123, &task);
}

void SSPMaster::requestIRDataCycle(unsigned int ms)
{
	m_irReqPeriod = ms;
	m_reqIRRepeater.run([this] { doReqIR(); } , ms);
}

SerialPort& SSPMaster::serial()
{
	return m_serial;
}
/*
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
*/

///////////////////////
// C driver
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
	SSPMaster::sspMasterActive->serial().stop();
	SSPMaster::sspMasterActive->serial().write(data, size);
}

uint8_t ssp_is_ir_data_ready(void) { return 0; }

void ssp_get_ir_data(uint8_t** data, uint16_t* size) { }

void ssp_write_debug(uint8_t* data, uint16_t size)
{
	std::cout << "Debug channel: ";
	if (size == 0)
	{
		cout << "<empty>";
	} else {
		std::string s(data, data+size-1);
		cout << s;
	}
	cout << endl;
}
