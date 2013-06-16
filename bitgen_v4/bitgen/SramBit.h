#ifndef __SRAM_BIT_H__
#define __SRAM_BIT_H__

#include <string>
#include <vector>
#include <iostream>
#include "utils.h"

struct SramBit
{
	Point localPos;
	Point offset;
	Point tilePos;
	int   bitValue;
};

inline std::ostream & operator << ( std::ostream & os, const SramBit & bit ) 
{
	os << "[SRAM]Tile position: " << bit.tilePos + bit.offset
		<< "\tLocal position: B"  << bit.localPos.getX() << "W" << bit.localPos.getY()
		<< "\tValue: " << bit.bitValue;
	return os;
}


#endif