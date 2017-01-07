#include "ssp-master.hpp"
#include "../smart-sensor/lasertag-ssp/ssp/Inc/ssp-driver.h"
#include "../smart-sensor/lasertag-ssp/ssp/Inc/ssp-master-part.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/shared_array.hpp>

#include <iostream>
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
		m_pushAnimTask(io),
		m_printAddrList(io)
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
	m_pushAnimTask.run([this]{ doPushAnimTasks(); }, 3000);
	startAsyncRead();
	//doRunDiscovery();
}

void SSPMaster::startAsyncRead()
{
	//m_serial.asyncReadPerByte(ssp_receive_byte);

	m_serial.asyncRead([this](
		const std::vector<uint8_t>& buffer) {	messageCallback(buffer); },
		20
	);
}

void SSPMaster::messageCallback(const std::vector<uint8_t>& buffer)
{
	if (!buffer.empty())
	{
		/*
		cout << "==========" << endl;
		cout << "Bus message: " << arrayToHexStr(buffer.data(), buffer.size()) << endl;
		cout << "==========" << endl;
*/

		for (auto it : buffer)
		{
			ssp_receive_byte(it);
		}
	}

	startAsyncRead();
}

void SSPMaster::doReqIR()
{
	if (ssp_is_busy())
	{
		cout << "Busy" << endl;
		return;
	}
	ssp_push_ir_request(123);
	//startAsyncRead();
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
		cout << "Bits: " << ir->buffer.bits_count << endl;
		cout << "Data: ";
		for (int i=0; i < SSP_IR_BUFFER_MAX_SIZE; i++)
		{
			cout << (int) (ir->buffer.data[i]) << " ";
		}
		cout << endl;
	}
}

void SSPMaster::doPushAnimTasks()
{
	if (ssp_is_busy())
	{
		cout << "Im busy";
		return;
	}
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

void SSPMaster::doRunDiscovery()
{
	if (ssp_is_busy())
		return;
	cout << "Running address discovering..." << endl;
	ssp_start_address_discovering();
	//startAsyncRead();
}

void SSPMaster::doPrintAddrsList()
{
	cout << "Address list size: " << ssp_registered_addrs.size << endl;
	for (int i=0; i<ssp_registered_addrs.size; i++)
	{
		cout << "Known: " << ssp_registered_addrs.address[i] << endl;
	}
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

std::string SSPMaster::arrayToHexStr(const uint8_t* array, size_t size)
{
	std::stringstream ss;
	ss << std::hex << std::setfill('0');
	for (int i = 0; i < size; ++i)
	{
		ss << std::setw(2) << static_cast<unsigned>(array[i]) << " ";
	}
	return ss.str();
}

///////////////////////
// C driver
void ssp_drivers_init(void)
{

}

uint32_t ssp_get_time_ms()
{
	static boost::posix_time::ptime startTime = boost::posix_time::microsec_clock::local_time();

	boost::posix_time::ptime nowTime = boost::posix_time::microsec_clock::local_time();
	boost::posix_time::time_duration diff = nowTime - startTime;
	return diff.total_milliseconds();
}


void ssp_send_data(uint8_t* data, uint16_t size)
{
	SSPMaster::sspMasterActive->serial().stop();
	SSPMaster::sspMasterActive->serial().write(data, size);
	SSPMaster::sspMasterActive->startAsyncRead();
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
