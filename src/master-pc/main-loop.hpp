/*
 * main-loop.hpp
 *
 *  Created on: 28 дек. 2016 г.
 *      Author: dalexies
 */

#ifndef MASTER_PC_MAIN_LOOP_HPP_
#define MASTER_PC_MAIN_LOOP_HPP_

#include "signal-handler.hpp"
#include <boost/asio.hpp>

class MainLoop : public IStoppableObject
{
public:
	MainLoop(boost::asio::io_service& io);
	void mainLoop();
	void stop() override;

private:
	boost::asio::io_service& m_io;
	bool m_needStop = false;
};







#endif /* MASTER_PC_MAIN_LOOP_HPP_ */
