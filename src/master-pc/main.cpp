#include "serial-port.hpp"
#include "ssp-master.hpp"
#include "signal-handler.hpp"
#include "main-loop.hpp"

#include <boost/program_options.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <cstdlib>

using namespace std;

boost::asio::io_service io;

int main(int argc, char** argv)
{
	SignalHandler::instance().init(io);

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

    SSPMaster sspMaster(io, serial);
    sspMaster.startAsyncReading();

    sspMaster.requestIRDataCycle(1000);
    MainLoop ml(io);
    ml.mainLoop();

    return EXIT_SUCCESS;
}
