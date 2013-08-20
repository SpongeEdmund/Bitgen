/* File    : BitGenApp.h
 * Brief   : BitGen顶层模块
 * Author  : Edmund Wong
 * Date    : 2013-1-11
 * Version : 1.0
 */

//! Top module of bitstream generating process

#ifndef BITGENAPP_H
#define BITGENAPP_H

#include <boost/program_options.hpp>
#include <vector>
#include <string>
#include <ctime>
#include "Utils.h"
#include "Args.h"
#include "SramBit.h"
#include "InstQuerier.h"
#include "NetQuerier.h"
#include "Cil.h"


#include "Netlist.h"
#include "BitStream.h"

namespace bitgen
{

	extern Args ARGS;
	class BitGenApp
	{
	public:
		typedef std::vector<SramBit> SramVec;

	public:
		//! As there can only be one bitgen top module, here we use singleton design pattern.
		static BitGenApp& instance() {
			static BitGenApp inst;
			return inst;
		}

	protected:
		//! Constuctor, note the start running time
		BitGenApp() 
			: _cil(0),
			  _netlist(0),
			  _instQuerier(0),
			  _bitstream(0),
			  _usedSlices(0)
		{
			std::cout << ">> ------Bitgen started at " << GetCurrentTime() << "------" << std::endl;
			_startTime = static_cast<double> ( clock() );
		}

	public:
		~BitGenApp()
		{
			if (_cil) delete _cil;
			if (_netlist) delete _netlist;
			if (_bitstream) delete _bitstream;
			_endTime = static_cast<double>( clock() );
			std::cout << ">>| Bitgen finished in " << ( _endTime - _startTime ) / 1e3 << "s." << std::endl;
		}
	
	public:
		//! Run the whole query process.
		void run( );
		//! Load in cil
		void loadCil(const std::string &cilFile , bool isEncrypted);
		//! Load in circuit 
		void loadNetlist(const std::string & );		
		//! Generate bitstream
		void genBitStream();
		//! Output bit file in hex format
		void outputBitstream( std::ostream & os ) { os << *_bitstream << std::endl; }
	private:
		//! Config info lib, which is read from xml
		Cil*         _cil;
		//! Netlist to implement
		Netlist*     _netlist;

		//! Queriers
		InstQuerier* _instQuerier; 
		NetQuerier*  _netQuerier;

		//!
		SramVec _sramVec;

		
		BitStream*   _bitstream;

		//! Start time of the process
		double _startTime;
		//! End time of the process
		double _endTime;

		//! Slices used
		int _usedSlices;

	};


	

}
#endif