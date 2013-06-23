#ifndef __BITSTREAM_H__
#define __BITSTREAM_H__
#include <vector>
#include <string>

#include "Args.h"
#include "Cil.h"
#include "SramBit.h"
#include "utils.h"

class Point;


namespace bitgen {

	typedef unsigned __int32 WORD;


	extern Args ARGS;

	extern std::string _COR_REG;
	extern std::string _ID_CODE_REG;
	extern std::string _MASK_REG;
	extern std::string _CTL_REG;
	extern std::string _FAR_REG;
	extern std::string _CRC_REG; 

	class BitStream
	{
	public:
		typedef std::vector<SramBit> SramVec;

		//! Constructor
		BitStream( const Cil& c ) : _cil(c), _size(0), _curChip(0), _curPlan(0) {}

		void init( bool initVal = true );

		// We load srams from sram vector
		void loadSrams( const SramVec & sramVec );
	
		void printHead( std::ostream & ) const;
		void printBody( std::ostream & ) const;
		void printTail( std::ostream & ) const;
		
		void setBit( const int idx );
		void setBit( const Point & tilePos );
		void resetBit( const int idx );
		void resetBit( const SramBit & tilePos );

		int getSize() const { return _size; }
	
		friend std::ostream & operator<< ( std::ostream &, const BitStream & );
	private:
		int mapSramToIdx( const SramBit & pt );
	private:

		//! stream is composed by 32-bit words
		std::vector<WORD> _body;

		//! cil reference
		/*!
			Here it should be stressed that a bitstream cannot exist without a cil.
			We use reference here to notify once a cil is appointed, we can not change
			the reference to other cil.
		*/
		const Cil& _cil;
		int _size;
		chip* _curChip;
		plan* _curPlan;
	};



	
	void SetBitOfWordBigEdian( WORD & word, int pos );
	void ResetBitOfWordBigEdian( WORD & word, int pos );
}

#endif