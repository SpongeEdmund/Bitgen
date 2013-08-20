// This is the argument module of bitgen app

#include "Args.h"
#include <boost/program_options.hpp>
#include <iostream>

namespace bitgen {

	using std::string;
	using std::cout;
	using std::endl;

	Args ARGS;

	void dispHelp()
	{
		cout << "Usage: \n"
			<< "bitgen [--arch|-a] <archFile> [--cil|-c] <cilFile> [--netlist|-n] <netlistFile> [--bitstream|-b] <bitstreamFile>"
			<< endl;
	}

	void dispVer()
	{
		cout << "Bistream Generator For FDP FPGA, Version 1.1\n"
			<< "Copyright 2008-2013, Fudan University, State Key Lab"
			<< endl;
	}


	void Args::parse( int argc, char* argv[] )
	{
		namespace po = boost::program_options;
		po::options_description visible("Allowed options");
		visible.add_options()
			("device,d",    po::value<string>(), "device name")
			("package,p",    po::value<string>(), "pad package")
			("cil,c",       po::value<string>(), "cil file")
			("netlist,n",   po::value<string>(), "netlist file")
			("bitstream,b", po::value<string>(), "bitstream file")
			("help,h",                           "show help information")
			("version,v",                        "show version information")
			("encrypted,e",                        "use encrypted cil file")
			;
		po::variables_map vm;
		po::options_description cmdline;
		cmdline.add(visible);
		po::store(po::command_line_parser(argc,argv).options(cmdline).run(), vm);
		po::notify(vm);


		if ( vm.count("help") ) {
			cout << visible << endl;
			dispVer();
			dispHelp();
		}
		
		if ( vm.count("version") ) {
			dispVer();
		}

		if( vm.count("device") ) {
			DEVICE = vm["device"].as<string>();
		}
	
		if( vm.count("package") ) {
			PACKAGE = vm["package"].as<string>();
		}
		
		if (vm.count("cil")) {
			CIL = vm["cil"].as<string>();
		}
	
		if (vm.count("netlist")) {
			NETLIST = vm["netlist"].as<string>();
		}

		if (vm.count("bitstream")) {
			BITSTREAM = vm["bitstream"].as<string>();
		}

		if (vm.count("encrypted")) {
			ISENCRYPTED = true;
		}
	}

}
