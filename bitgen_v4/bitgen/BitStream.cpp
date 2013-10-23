#include "BitStream.h"
#include <iostream>
#include <sstream>
#include <iomanip>
//#include <cstdlib>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include "Exceptions.h"
#include <ctime>
namespace bitgen {

#define foreach BOOST_FOREACH

	const WORD ALL_ONE  = 0xffffffff;
	const WORD ALL_ZERO = 0x00000000;


	using std::string;
	using std::stringstream;
	using std::endl;
	using boost::lexical_cast;
	using std::make_pair;

	//string _COR_REG     = "0000_0012";
	string _COR_REG     = "1001_2012";
	
	//string _ID_CODE_REG = "0167_c093";
	string _ID_CODE_REG = "ffff_ffff";
	string _MASK_REG    = "0000_0008";
	string _CTL_REG     = "0000_0008";
	string _FAR_REG     = "0028_0000";
	string _CRC_REG     = "17af_7ea6";

	void BitStream::init( bool initValue )
	{
#ifdef _TEST
		std::cout << "<DebugInfo> Initial a bitstream. " << std::flush;
		// read in the total words amount from cil
#endif
		chip_lib* chipLib = _cil.root()->get_chip_lib();

		_curChip = chipLib->find_chip_by_name(ARGS.DEVICE);
		if( !_curChip ) 
			throw std::exception("[!error!] Cannot find the chip type in the lib");
		_curPlan = _curChip->get_plan();
		if( !_curPlan )
			throw std::exception("[!error!] Cannot find config plan of the chip");
		
		int f = lexical_cast<int>( _curPlan->get_frames_per_segment() );
		int w = lexical_cast<int>( _curPlan->get_words_per_frame() ); 
		int s = lexical_cast<int>( _curPlan->get_segment_amount() );

		_size = lexical_cast<int>( f * w * s );
		//int wordsPerFrame = lexical_cast<int>( _curPlan->get_words_per_frame() );
		WORD initWord = initValue ? ALL_ONE : ALL_ZERO;
#ifdef _TEST
		std::cout << "The word amount is " << _size << std::endl;

#endif
		// read in the total words amount from cil
		_body.resize( _size, initWord );


		// Initial the map of pos and tile
		arch* curArch = _curChip->get_arch();
		tile_inst* t = curArch->first_tile_inst(); 
		for ( ; t ; t = t->next_tile_inst() ) {
			//_posTileMap.insert( make_pair<string, tile_inst*>(pos, t));
			_posTileMap.push_back(t);
		}

	}


	void BitStream::loadSrams( const SramVec & sramVec )
	{
		foreach( const SramBit & s, sramVec ) {
			// We ignore the srams with value 1
			if ( s.bitValue == 1 ) continue;
			
			resetBit( s );
		}
	}
	

	void BitStream::resetBit( const int idx )
	{
		int prevWord = idx / 32;
		int offsetBit = idx % 32;
		//ResetBitOfWordLittleEdian( _body[prevWord], offsetBit-1 );
		ResetBitOfWordLittleEdian(_body[prevWord],offsetBit);
	}

	void BitStream::resetBit( const SramBit & tilePos )
	{
		int idx = mapSramToIdx( tilePos );
		resetBit( idx );
	}

	//! map the point pos to sequence index of the vector
	int BitStream::mapSramToIdx( const SramBit & s )
	{
#ifdef _TEST
		std::cout << s << std::flush;
#endif

#ifdef _MAP_STAT
		double t0 = static_cast<double>( clock() );
#endif
		int idx = 0;
		Point actualPos = s.tilePos + s.offset;
		// We need to calculate the Segment address and Packet address of the tile
		string colStr = lexical_cast<string>( actualPos.getY() );
		packet* p = _curPlan->find_packet_by_col(colStr);

#ifdef _TEST
		stringstream packetMissInfo;
		packetMissInfo << "<Error> Cannot find a packet in column: " << colStr;
		CONDITIONAL_THROW(
			p != NULL,
			CilInfoMissException,
			packetMissInfo.str()
			)
#endif
		int packetAddr = lexical_cast<int>( p->get_address() );
		
		//??
		//std::cout << "Current packet: " << packetAddr << std::endl;

		segment* curSegment = _curPlan->first_segment();
#ifdef _MAP_STAT
		double t1 = static_cast<double>( clock() );
		std::cout << "\nT1: " << t1-t0 << "ms" << std::endl;
#endif
		while( curSegment ) {
			int curSegmentRow = lexical_cast<int>( curSegment->get_row() );
			int rowsPerSegment = lexical_cast<int>( _curPlan->get_rows_per_segment() );
			if ( actualPos.getX() >= curSegmentRow && actualPos.getX() - curSegmentRow < rowsPerSegment ) { 
				
				break;
			}
			curSegment = curSegment->next_segment();
		}
#ifdef _MAP_STAT
		double t2 = static_cast<double>( clock() );
		std::cout << "\nT2: " << t2-t1 << "ms" << std::endl;
#endif


		// Frames per segment
		int fps = lexical_cast<int>( _curPlan->get_frames_per_segment() );
		// Words per frame
		int wpf = lexical_cast<int>( _curPlan->get_words_per_frame() );
		
		
		int segmentAddr = lexical_cast<int>(curSegment->get_address());
		//??
		//std::cout << "Current segment: " << segmentAddr << std::endl;


		idx += segmentAddr * fps * wpf * 32 ;
		
		//??
		//std::cout << "Now index is: " << idx << std::endl;

		packet* curPacket;
#ifdef _MAP_STAT
		double t3 = static_cast<double>( clock() );
		std::cout << "\nT3: " << t3-t2 << "ms" << std::endl;
#endif		
		//??
		//std::cout << "Add up all front packets." << std::endl;
		for ( int i = 0; i < packetAddr; ++i )
		{
			string curPacketAddrStr = lexical_cast<string>(i);
			curPacket = _curPlan->find_packet_by_address(curPacketAddrStr);
			int frameAmount = lexical_cast<int>(curPacket->get_frame_amount());
			idx += frameAmount * wpf * 32 ;
			
		}


		//??
		//std::cout << "Now index is: " << idx << std::endl;

		// Add front frames
		//??
		//std::cout << "Add up all front local frames." << std::endl;
		idx += wpf * s.localPos.getY() * 32;
#ifdef _MAP_STAT
		double t4 = static_cast<double>( clock() );
		std::cout << "\nT4: " << t4-t3 << "ms" << std::endl;
#endif
		//??
		//std::cout << "Now index is: " << idx << std::endl;
		// Add front bits in current frame

		//??
		//std::cout << "Add up all front local bits." << std::endl;

		int segmentRow = lexical_cast<int>(curSegment->get_row());
		arch* curArch = _curChip->get_arch();
		Point tileScale = lexical_cast<Point>(curArch->get_scale()) ;
		int tilePerRow = tileScale.getY();

		for ( int r = segmentRow; r < actualPos.getX() ; ++r ) {
#ifdef _MAP_STAT
			double t50 = static_cast<double>( clock() );
#endif
			//stringstream curPosStr;
			//curPosStr << r << "," << actualPos.getY();

			// Here is the bottle neck of query
			// tile_inst* curTileInst = _curChip->get_arch()->find_tile_inst_by_pos(curPosStr.str());
			// tile_inst* curTileInst = _posTileMap[curPosStr.str()];
			int ti = r * tilePerRow + actualPos.getY();
			tile_inst* curTileInst = _posTileMap[ti];
#ifdef _MAP_STAT
			double t51 = static_cast<double>( clock() );
			std::cout << "\nT51: " << t51-t50 << "ms" << std::endl;
#endif

			string tileType = curTileInst->get_ref();

			tile* curTile = _cil.root()->get_tile_lib()->find_tile_by_name(tileType);


#ifdef _TEST
			stringstream tileTypeMissInfo;
			tileTypeMissInfo << "[!Error!] Cannot find tile type: " << tileType;
			CONDITIONAL_THROW (
				curTile != 0,
				CilInfoMissException, 
				tileTypeMissInfo.str()
				)
#endif

			Point localSize = lexical_cast<Point>(curTile->get_scale());
			int localRowSize = localSize.getX();

			idx += localRowSize;
#ifdef _MAP_STAT
			double t52 = static_cast<double>( clock() );
			std::cout << "\nT52: " << t52-t51 << "ms" << std::endl;
#endif
		}
		//??
		//std::cout << "Now index is: " << idx << std::endl;
		// Add local bit line number
		idx += s.localPos.getX();
#ifdef _MAP_STAT
		double t5 = static_cast<double>( clock() );
		std::cout << "\nT5: " << t5-t4 << "ms" << std::endl;
#endif
#ifdef _TEST
		std::cout << " idx:" << idx  << std::endl;

		stringstream soinfo;
		soinfo << "SRAM bitstream position overflows: " << s << " idx: "<< idx << " max: " << _size * 32;
		CONDITIONAL_THROW(
			idx < _size * 32,
			SramRangeOverflow, 
			soinfo.str()
			)
#endif
		return idx;
	}

	void BitStream::printHead( std::ostream & os ) const
	{
		os << "0000_0000" << "\n"; 
		os << "0000_0000" << "\n"; 
		os << "0000_0000" << "\n"; 
		os << "0000_0000" << "\n"; 
		os << "aa99_5566" << "\n"; 
		os << "3000_8001" << "\n"; 
		os << "0000_0007" << "\n"; 
		os << "2000_0000" << "\n"; 
		os << "2000_0000" << "\n"; 
		os << "3001_2001" << "\n"; 
		os << _COR_REG << "\n";
		os << "3001_8001" << "\n";
		os << _ID_CODE_REG << "\n";
		os << "3000_8001" << "\n";
		os << "0000_0009" << "\n";
		os << "2000_0000" << "\n";
		os << "3000_c001" << "\n";
		os << _MASK_REG << "\n";
		os << "3000_a001" << "\n";
		os << _CTL_REG  << "\n";
		os << "3000_2001" << "\n";
		os << "0028_0000" << "\n";
		os << "3001_c001" << "\n";
		os << "00c0_0000" << "\n";
		os << "3000_8001" << "\n";
		os << "0000_0001" << "\n";
		os << "2000_0000" << "\n";
		os << "2000_0000" << "\n";
		os << "2000_0000" << "\n";
		os << "2000_0000" << "\n";
		os << "2000_0000" << "\n";
		os << "3000_4000" << "\n";
		//os << "5003b568" << "\n";
		os << "5002_4090" << "\n";
	}
	
	void BitStream::printBody( std::ostream & os ) const
	{
		std::vector<WORD>::const_iterator it = _body.begin();
		while ( it != _body.end() ) {
			stringstream hexStr;
			
			//os << std::hex << std::setw(8) << std::setfill('0') << *it << "\n"; 
			hexStr << std::hex << std::setw(8) << std::setfill('0') << *it;
			os << hexStr.str().substr(0,4) << "_" << hexStr.str().substr(4,8) << "\n";
			++it;
		}
	}
	
	
	void BitStream::printTail( std::ostream & os ) const
	{
		os << "3000_0001" << "\n";
		os << "e183_1749" << "\n";
		os << "3000_8001" << "\n";
		os << "0000_000a" << "\n";
		os << "2000_0000" << "\n";
		os << "3000_8001" << "\n";
		os << "0000_0003" << "\n";
		os << "3000_8001" << "\n";
		os << "0000_000a" << "\n";
		os << "3000_2001" << "\n";
		os << _FAR_REG << "\n";
		os << "3000_8001" << "\n";
		os << "0000_0005" << "\n";
		os << "3000_c001" << "\n";
		os << _MASK_REG << "\n";
		os << "3000_a001" << "\n";
		os << _CTL_REG << "\n";
		os << "3000_0001" << "\n";
		os << _CRC_REG << "\n";
		os << "3000_8001" << "\n";
		os << "0000_000d" << "\n";
		os << "2000_0000" << "\n"; 
		os << "2000_0000" << "\n"; 
		os << "2000_0000" << "\n"; 
		os << "2000_0000" << "\n"; 
		os << "2000_0000" << "\n"; 
		os << "2000_0000" << "\n"; 
		os << "2000_0000" << "\n"; 
		os << "2000_0000" << "\n"; 
		os << "2000_0000" << "\n"; 
		os << "2000_0000" << "\n"; 
		os << "2000_0000" << "\n"; 
		os << "2000_0000" << endl;
	}

	
	std::ostream & operator << ( std::ostream& os, const BitStream& bitStream )
	{
		bitStream.printHead(os);
		bitStream.printBody(os);
		bitStream.printTail(os);
		return os;
	}

	void SetBitOfWordBigEdian( WORD & word, int pos )
	{
		WORD mask = 0x80000000;
		mask >>= pos;
		word |= mask;
	}

	void ResetBitOfWordBigEdian( WORD & word, int pos )
	{
		WORD mask = 0x80000000;
		mask >>= pos;
		word &= ~mask;
	}

	void ResetBitOfWordLittleEdian(WORD& word, int pos)
	{
		WORD mask = 0x00000001;
		mask <<= pos;
		word &= ~mask;

	}

}