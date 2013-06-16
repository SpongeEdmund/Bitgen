#include "BitStream.h"
#include <iostream>
#include <sstream>
#include <iomanip>
//#include <cstdlib>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
namespace bitgen {

#define foreach BOOST_FOREACH

	const WORD ALL_ONE  = 0xffffffff;
	const WORD ALL_ZERO = 0x00000000;


	using std::string;
	using std::stringstream;
	using std::endl;
	using boost::lexical_cast;

	string _COR_REG     = "00000012";
	string _ID_CODE_REG = "0167c093";
	string _MASK_REG    = "00000008";
	string _CTL_REG     = "00000008";
	string _FAR_REG     = "00280000";
	string _CRC_REG     = "17af7ea6";

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

		int totalFrames = lexical_cast<int>( f * w * s );
		int wordsPerFrame = lexical_cast<int>( _curPlan->get_words_per_frame() );
		WORD initWord = initValue ? ALL_ONE : ALL_ZERO;
#ifdef _TEST
		std::cout << "The word amount is " << totalFrames * wordsPerFrame << std::endl;

#endif
		// read in the total words amount from cil
		_body.resize( totalFrames * wordsPerFrame, initWord );


	}


	void BitStream::loadSrams( const SramVec & sramVec )
	{
		foreach( const SramBit & s, sramVec ) {
			// We ignore the srams with value 1
			if ( s.bitValue == 1 ) continue;
			
			resetBit( s );
		}
	}
	

	void BitStream::resetBit( const size_t idx )
	{
		int prevWord = idx / 32;
		int offsetBit = idx % 32;
		ResetBitOfWordBigEdian( _body[prevWord], offsetBit-1 );
	}

	void BitStream::resetBit( const SramBit & tilePos )
	{
		size_t idx = mapSramToIdx( tilePos );
		resetBit( idx );
	}

	//! map the point pos to sequence index of the vector
	size_t BitStream::mapSramToIdx( const SramBit & s )
	{
		size_t idx = 0;
		Point actualPos = s.tilePos + s.offset;
		// We need to calculate the Segment address and Packet address of the tile
		string colStr = lexical_cast<string>( actualPos.getY() );
		size_t packetAddr = lexical_cast<size_t>( _curPlan->find_packet_by_col(colStr)->get_address() );
		

		segment* curSegment = _curPlan->first_segment();
		while( curSegment ) {
			size_t curSegmentRow = lexical_cast<size_t>( curSegment->get_row() );
			size_t rowsPerSegment = lexical_cast<size_t>( _curPlan->get_rows_per_segment() );
			if ( actualPos.getX() >= curSegmentRow && actualPos.getX() - curSegmentRow < rowsPerSegment ) { 
				
				break;
			}
			curSegment = curSegment->next_segment();
		}

		// Frames per segment
		size_t fps = lexical_cast<size_t>( _curPlan->get_frames_per_segment() );
		// Words per frame
		size_t wpf = lexical_cast<size_t>( _curPlan->get_words_per_frame() );
		
		
		size_t segmentAddr = lexical_cast<size_t>(curSegment->get_address());
		idx += segmentAddr * fps * wpf * 32 ;

		packet* curPacket;
		for ( size_t i = 0; i < packetAddr; ++i )
		{
			string curPacketAddrStr = lexical_cast<string>(i);
			curPacket = _curPlan->find_packet_by_address(curPacketAddrStr);
			size_t frameAmount = lexical_cast<size_t>(curPacket->get_frame_amount());
			idx += frameAmount * wpf * 32 ;
			
		}
		
		// Add front frames
		idx += wpf * s.localPos.getY();
		
		// Add front bits in current frame
		size_t segmentRow = lexical_cast<size_t>(curSegment->get_row());
		for ( int r = segmentRow; r < actualPos.getX() ; ++r ) {

			stringstream curPosStr;
			curPosStr << r << "," << actualPos.getY();

			
			tile_inst* curTileInst = _curChip->get_arch()->find_tile_inst_by_pos(curPosStr.str());
			string tileType = curTileInst->get_ref();
			
			tile* curTile = _cil.root()->get_tile_lib()->find_tile_by_name(tileType);
			assert( curTile );

			Point localSize = lexical_cast<Point>(curTile->get_scale());
			int localRowSize = localSize.getX();
			
			idx += localRowSize;
		}
		// Add local bit line number
		idx += s.localPos.getX();
		return idx;
	}

	void BitStream::printHead( std::ostream & os ) const
	{
		os << "00000000" << "\n"; 
		os << "00000000" << "\n"; 
		os << "00000000" << "\n"; 
		os << "00000000" << "\n"; 
		os << "aa995566" << "\n"; 
		os << "30008001" << "\n"; 
		os << "00000007" << "\n"; 
		os << "20000000" << "\n"; 
		os << "20000000" << "\n"; 
		os << "30012001" << "\n"; 
		os << _COR_REG << "\n";
		os << "30018001" << "\n";
		os << _ID_CODE_REG << "\n";
		os << "30008001" << "\n";
		os << "00000009" << "\n";
		os << "20000000" << "\n";
		os << "3000c001" << "\n";
		os << _MASK_REG << "\n";
		os << "3000a001" << "\n";
		os << _CTL_REG  << "\n";
		os << "30002001" << "\n";
		os << "00280000" << "\n";
		os << "30008001" << "\n";
		os << "00000001" << "\n";
		os << "20000000" << "\n";
		os << "20000000" << "\n";
		os << "20000000" << "\n";
		os << "20000000" << "\n";
		os << "20000000" << "\n";
		os << "30004000" << "\n";
		os << "5003b568" << "\n";
	}
	
	void BitStream::printBody( std::ostream & os ) const
	{
		std::vector<WORD>::const_iterator it = _body.begin();
		while ( it != _body.end() ) {
			os << std::hex << std::setw(8) << std::setfill('0') << *it << "\n"; 
			++it;
		}
	}
	
	
	void BitStream::printTail( std::ostream & os ) const
	{
		os << "30000001" << "\n";
		os << "e1831749" << "\n";
		os << "30008001" << "\n";
		os << "0000000a" << "\n";
		os << "20000000" << "\n";
		os << "30008001" << "\n";
		os << "00000003" << "\n";
		os << "30008001" << "\n";
		os << "0000000a" << "\n";
		os << "30002001" << "\n";
		os << _FAR_REG << "\n";
		os << "30008001" << "\n";
		os << "00000005" << "\n";
		os << "3000c001" << "\n";
		os << _MASK_REG << "\n";
		os << "3000a001" << "\n";
		os << _CTL_REG << "\n";
		os << "30000001" << "\n";
		os << _CRC_REG << "\n";
		os << "30008001" << "\n";
		os << "0000000d" << "\n";
		os << "20000000" << "\n"; 
		os << "20000000" << "\n"; 
		os << "20000000" << "\n"; 
		os << "20000000" << "\n"; 
		os << "20000000" << "\n"; 
		os << "20000000" << "\n"; 
		os << "20000000" << "\n"; 
		os << "20000000" << "\n"; 
		os << "20000000" << "\n"; 
		os << "20000000" << "\n"; 
		os << "20000000" << "\n"; 
		os << "20000000" << endl;
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

}