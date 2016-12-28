/*
 * signal-handler.hpp
 *
 *  Created on: 28 дек. 2016 г.
 *      Author: dalexies
 */

#ifndef MASTER_PC_SIGNAL_HANDLER_HPP_
#define MASTER_PC_SIGNAL_HANDLER_HPP_

#include <boost/asio.hpp>
#include <list>

class IStoppableObject
{
public:
	virtual ~IStoppableObject() {}
	virtual void stop() = 0;
};

class SignalHandler
{
public:
	static SignalHandler& instance();
	void init(boost::asio::io_service& io);
	void addStoppable(IStoppableObject* stoppable);

private:
	SignalHandler();

	void handle(const boost::system::error_code& error, int signal_number);
	void stopAll();

	std::unique_ptr<boost::asio::signal_set> m_signals;//(io, SIGINT, SIGTERM);
	boost::asio::io_service* m_io = nullptr;

	std::list<IStoppableObject*> m_stoppable;
};


#endif /* MASTER_PC_SIGNAL_HANDLER_HPP_ */
