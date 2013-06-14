#include <iostream>
#include <ctime>

#include "BitGenApp.h"
//#include "Args.h"

using namespace std;
using namespace bitgen;

int main( int argc, char* argv[] )
{
	extern Args ARGS;
	try {
		ARGS.parse( argc, argv );
		BitGenApp& app = BitGenApp::instance();
		app.run();
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






/************************************************************************/
/*  test LutCalc                                                        */
/************************************************************************/

//#include <iostream>
//
//#include "LutCalc.h"
//#include <boost/foreach.hpp>
//
//
//
//int main()
//{
//	std::string exp = "1011111111110111";
//	std::vector<int> truthTable = Exp2LUT(exp, 16).toVec();
//
//	BOOST_FOREACH( int truth, truthTable ) {
//		std::cout << truth;
//	}
//	std::cout << std::endl;
//	return 0;
//
//
//}

////#include "utils.h"
//#include <vector>
//#include <string>
//
//int main()
//{
//	extern std::vector<int> Hex2Bin( const std::string& hexExp );
//	std::string h = "1301";
//	std::vector<int> b = Hex2Bin(h);
//
//	return 0;
//
//
//
//}