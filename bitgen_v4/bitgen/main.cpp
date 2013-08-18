#include <iostream>
#include <ctime>

#include "BitGenApp.h"

using namespace std;
using namespace bitgen;

int main( int argc, char* argv[] )
{
	extern Args ARGS;
	try {
		ARGS.parse( argc, argv );
		BitGenApp& app = BitGenApp::instance();
		app.run();
		std::cout << ">>| Run successfully!!!" << std::endl;
	} 
	catch( std::exception & e ) {
		std::cout << e.what() << std::endl;
	}
	catch( ... ) {
		std::cout << "Unexpected error occurs." << std::endl;
		exit(-1);
	}
	//system("pause");
	return 0;
}


//#include "AdjustBiPipDir.h"
//#include "XdlNlHandler.h"
////test the adjust algorithm
//int main()
//{
//	try{
//		XdlNlHandler* xdlNlHandler = new XdlNlHandler();
//
//		Netlist* netlist = xdlNlHandler->parse("F:\\Projects\\PerlProjects\\TestbenchBitgen\\netlists\\lut_or.xdl");
//
//		NetlistNet netToAdjust;
//		foreach( const NetlistNet& net, netlist->nets )
//		{
//			if ( net.netName == "XIL_ML_PMV_OUT_SIG" )
//				netToAdjust = net;
//		}
//		foreach( const NetPip& pip, netToAdjust.pips)
//		{
//			std::cout << pip.srcNet << "->" << pip.snkNet << std::endl;
//		}
//		//AdjustNet(netToAdjust);
//		delete xdlNlHandler;
//		delete netlist;
//	}
//	catch( std::exception& e )
//	{
//		std::cout << e.what() << std::endl;
//	}
//	catch(...)
//	{
//		std::cout << "unexcepted error" << std::endl;
//	}
//}