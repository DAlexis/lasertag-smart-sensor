#include "serial-port.hpp"

#include <boost/program_options.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <cstdlib>

using namespace std;

boost::asio::io_service io;

bool needStopSystem = false;

void signalHandler(const boost::system::error_code& error, int signal_number)
{
	if (signal_number == SIGINT)
	{
		cout << " User interrupt by Ctrl+C" << endl;
		needStopSystem = true;
		return;
	}
	if (signal_number == SIGINT)
	{
		cout << " Program terminated" << endl;
		needStopSystem = true;
		return;
	}
}


void incomingSerialHandler(const boost::system::error_code& error, std::size_t bytes_transferred)
{
	cout << "Incoming data " << endl;
}

int main(int argc, char** argv)
{
	// Construct a signal set registered for process termination.
	boost::asio::signal_set signals(io, SIGINT, SIGTERM);

	// Start an asynchronous wait for one of the signals to occur.
	signals.async_wait(signalHandler);

    namespace po = boost::program_options;
    po::options_description generalOptions("Genral options");
    generalOptions.add_options()
        ("help,h", "Print help message")
        ("serial-port,p", po::value<std::string>()->default_value("/dev/ttyUSB0"), "Serial port, default /dev/ttyUSB0")
        ("baudrate,b", po::value<unsigned int>()->default_value(115200), "Serial port speed")
        ("sensor-address,a", po::value<unsigned int>()->default_value(123), "Smart sensor address")
        ("test-ir-scan", "Run IR scanning test");
    
    po::options_description allOptions("Allowed options");
    allOptions.add(generalOptions);
    
    po::variables_map vmOptions;
    try
    {
        po::store(po::parse_command_line(argc, argv, allOptions), vmOptions);
        po::notify(vmOptions);
    }
    catch (po::error& e)
    {
        cerr << "Command line parsing error: " << e.what() << endl;
        return EXIT_FAILURE;
    }
    
    if (vmOptions.count("help"))
    {
        cout << allOptions << endl;
        return EXIT_SUCCESS;
    }

    // Creating serial port
    SerialPort serial(
    		io,
			vmOptions["serial-port"].as<std::string>(),
			vmOptions["baudrate"].as<unsigned int>()
    );

    serial.asyncReadLine('<', [](const std::vector<uint8_t>& buffer) {
    	std::string str(buffer.begin(), buffer.end());
    	cout << str << endl;
    });
    while (!needStopSystem)
    {
    	io.run();
    }
    return 0;
}
