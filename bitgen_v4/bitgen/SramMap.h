

//! SRAM map stores the results queried from the CIL 
/*!
	A SRAM map item is composed of the following information.
	1. A tile instance, represented by its position.
	2. The srams belonging to the tile instance, represented by its local position, 
	   thus bl and wl, and its value and tile offset.
*/


#ifndef __SRAM_MAP_H__
#define __SRAM_MAP_H__

#include <map>

#include "utils.h"
#include "Cil.h"

using std::map;
using std::pair;



class SramMap
{
public:
	//! To make it clear, we rename the Point.
	typedef Point TilePos;
	typedef Point BlWl;
	typedef Point Offset;

	//! Constructor
	/*! 
		To output the sram map as a xml format log file, we should also get information from 
		cil. That determines there must be a pointer refering to the corresponding cil & arch.
	*/


	//! Add a new queried result.
	void add( TilePos tilePos, BlWl blwl, Offset offset, bool val );


private:
	Cil*  _cil;
	//Arch* _arch;
	map< TilePos, map< BlWl, pair< Offset, bool > > > _map;
};

#endif