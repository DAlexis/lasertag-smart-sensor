#include "signal-handler.hpp"
#include <iostream>
#include <cstdlib>

using namespace std;

SignalHandler& SignalHandler::instance()
{
	static SignalHandler inst;
	return inst;
}

void SignalHandler::init(boost::asio::io_service& io)
{
	m_io = &io;
	m_signals.reset(new boost::asio::signal_set(io, SIGINT, SIGTERM));
	m_signals->async_wait([this](const boost::system::error_code& error, int signal_number) {
		handle(error, signal_number);
	});
}

void SignalHandler::addStoppable(IStoppableObject* stoppable)
{
	m_stoppable.push_back(stoppable);
}

SignalHandler::SignalHandler()
{

}

void SignalHandler::handle(const boost::system::error_code& error, int signal_number)
{
	if (signal_number == SIGINT)
	{
		cout << " User interrupt by Ctrl+C" << endl;
		stopAll();
		return;
	}
	if (signal_number == SIGINT)
	{
		cout << " Program terminated" << endl;
		stopAll();
		return;
	}
}

void SignalHandler::stopAll()
{
	for (auto it: m_stoppable)
		it->stop();
}
