/*
 * main-looop.cpp
 *
 *  Created on: 28 дек. 2016 г.
 *      Author: dalexies
 */

#include "main-loop.hpp"

#include <iostream>

using namespace std;

MainLoop::MainLoop(boost::asio::io_service& io) :
	m_io(io)
{
	SignalHandler::instance().addStoppable(this);
}

void MainLoop::mainLoop()
{
	while(!m_needStop)
	{
		m_io.run();
	}
}

void MainLoop::stop()
{
	m_needStop = true;
	m_io.stop();
}
