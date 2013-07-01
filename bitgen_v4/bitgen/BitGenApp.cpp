#include "BitGenApp.h"
#include "Args.h"
#include "XdlNlHandler.h"
#include <iomanip>
#include <fstream>
#include <boost/lexical_cast.hpp>
namespace bitgen {

	using std::string;
	using std::cout;
	using std::endl;
	using std::ofstream;
	using boost::lexical_cast;
	void BitGenApp::loadCil(const string & cilFile) 
	{
		std::cout << ">>| Loading cil file " << cilFile << std::endl;
		if( _cil ) delete _cil;
		_cil = new Cil( cilFile );
	}

	void BitGenApp::loadNetlist(const string & netlistFile )
	{
		std::cout << ">>| Loading netlist file " << netlistFile << std::endl;
		if ( _netlist ) delete _netlist;
		
		XdlNlHandler* xdlNlHandler = new XdlNlHandler;
		_netlist = xdlNlHandler->parse(netlistFile);
		_usedSlices = xdlNlHandler->getUsedSliceNum();

		delete xdlNlHandler;
	}

	void BitGenApp::genBitStream(  ) 
	{
		std::cout << ">>| Generating bitstream ..." << std::endl;
		if (_bitstream) delete _bitstream;
		_bitstream = new BitStream( *_cil );
		_bitstream->init();
		
		double time1 = static_cast<double>( clock() );

		//assert( !_instQuerier );
		_instQuerier = new InstQuerier( *_cil, *_netlist );
		std::cout << "  >> Generating SRAM map for instances ..." << std::endl;
		_instQuerier->runQuery( _sramVec );

		double time2 = static_cast<double>( clock() );
		std::cout << "    >> Time elapsed " << (time2 - time1) * 1e-3 << "s." << std::endl;
		

		_netQuerier = new NetQuerier( *_cil, *_netlist );
		std::cout << "  >> Generating SRAM map for nets ..." << std::endl;
		_netQuerier->runQuery( _sramVec );
		double time3 = static_cast<double>( clock() );
		std::cout << "    >> Time elapsed " << (time3 - time2) * 1e-3 << "s." << std::endl;
		//SramBit s;
		//s.bitValue = 0;
		//s.localPos = Point(79,63);
		//s.tilePos = Point(71,54);
		//s.offset = Point(0,0);
		//_sramVec.push_back(s);

		int sramAmount = _sramVec.size();
		float per = static_cast<float>(sramAmount) / static_cast<float>(_bitstream->getSize()*32) * 100;

		std::cout << "    >> Value changed SRAM amount : " << _sramVec.size() 
			<< "(" << per << "%)"<< std::endl;
		std::cout << "  >> Mapping to bitstream ..." << std::endl;
		_bitstream->loadSrams( _sramVec );
		double time4 = static_cast<double>( clock() );
		std::cout << "    >> Time elapsed " << (time4 - time3) * 1e-3 << "s." << std::endl;
	}

	void BitGenApp::run()
	{


		loadCil( ARGS.CIL );
		
//#ifdef _TEST
		double _loadCilTime = static_cast<double>( clock() );
		std::cout << "  >> Time elapsed " << (_loadCilTime-_startTime) * 1e-3 << "s." << std::endl;
//#endif


		loadNetlist( ARGS.NETLIST );
//#ifdef _TEST
		double _loadNlTime = static_cast<double>( clock() );
		std::cout << "  >> Time elapsed " << (_loadNlTime-_loadCilTime) * 1e-3 << "s." << std::endl;
//#endif

		genBitStream();
		
		// outputBitstream( std::cout );
		//#ifdef _TEST
		double _genBsTime = static_cast<double>( clock() );
		std::cout << "  >> Time elapsed " << (_genBsTime - _loadNlTime) * 1e-3 << "s." << std::endl;
		//#endif
		ofstream bitstrFile( ARGS.BITSTREAM );

		std::cout << ">>| Output bitstream to " << ARGS.BITSTREAM << std::endl;
		outputBitstream( bitstrFile );
//#ifdef _TEST
		double _outBsTime = static_cast<double>( clock() );
		std::cout << "  >> Time elapsed " << (_outBsTime-_genBsTime) * 1e-3 << "s." << std::endl;
//#endif
		int sliceAmount = lexical_cast<int>(_cil->root()->get_chip_lib()->find_chip_by_name(ARGS.DEVICE)->get_slice_amount());
		
		std::cout << ">>| Slice utilization: " << _usedSlices << "(" <<
			std::setprecision(5) << static_cast<double>(_usedSlices) / static_cast<double>(sliceAmount)*100 << "%)" << std::endl;
	}

}