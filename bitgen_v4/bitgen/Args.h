#ifndef ARGS_H
#define ARGS_H

#include <string>

namespace bitgen {

	struct Args {
		//! Parse the command line of bitgen
		Args() : ISENCRYPTED(false) {}
		void parse( int argc, char* argv[] );

		//! Arguments of bitgen app
		std::string DEVICE;    //!< device name like fdp5p15
		std::string PACKAGE;   //!< pad package
		std::string CIL;       //!< configuration info library
		std::string NETLIST;   //!< netlist file *.xml or *.xdl
		std::string BITSTREAM; //!< bitstream file *.bit
		bool        ISENCRYPTED; //!< use encrypted cil file or not
	};


}

#endif