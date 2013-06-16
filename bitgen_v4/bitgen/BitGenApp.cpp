#include "BitGenApp.h"
#include "Args.h"
#include "XdlNlHandler.h"
#include <fstream>

namespace bitgen {

	using std::string;
	using std::cout;
	using std::endl;
	using std::ofstream;

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
		delete xdlNlHandler;
	}

	void BitGenApp::genBitStream(  ) 
	{
		std::cout << ">>| Generating bitstream ..." << std::endl;
		if (_bitstream) delete _bitstream;
		_bitstream = new BitStream( *_cil );
		_bitstream->init();
		

		//assert( !_instQuerier );
		_instQuerier = new InstQuerier( *_cil, *_netlist );
		std::cout << "  >> Generating bitstream for instances ..." << std::endl;
		_instQuerier->runQuery( _sramVec );
	
		_netQuerier = new NetQuerier( *_cil, *_netlist );
		std::cout << "  >> Generating bitstream for nets ..." << std::endl;
		_netQuerier->runQuery( _sramVec );

		_bitstream->loadSrams( _sramVec );

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
		std::cout << "  >> Time elasped " << (_loadNlTime-_loadCilTime) * 1e-3 << "s." << std::endl;
//#endif

		genBitStream();
		
		// outputBitstream( std::cout );
		//#ifdef _TEST
		double _genBsTime = static_cast<double>( clock() );
		std::cout << "  >> Time elasped " << (_genBsTime - _loadNlTime) * 1e-3 << "s." << std::endl;
		//#endif
		ofstream bitstrFile( ARGS.BITSTREAM );

		std::cout << ">>| Output bitstream to " << ARGS.BITSTREAM << std::endl;
		outputBitstream( bitstrFile );
//#ifdef _TEST
		double _outBsTime = static_cast<double>( clock() );
		std::cout << "  >> Time elasped " << (_outBsTime-_genBsTime) * 1e-3 << "s." << std::endl;
//#endif
	}

}