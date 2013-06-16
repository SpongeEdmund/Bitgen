#ifndef __FDP5_H__
#define __FDP5_H__

// This file defines some parameters of fdp5

#include <string>
#include <set>

using std::string;
using std::set;


	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	// Bram
	const unsigned BRAM_LENGTH_PER_BLOCK = 256;
	const unsigned BRAM_PARITY_LENGTH = 32;

	/************************************************************************/
	/*               Config Info                                            */
	/************************************************************************/

	const unsigned FRAME_WORDS = 41;

	
	const string cfgClusters[] = {
		"SLICE","DSP48","FIFO16","RAMB16"
	};
	set<string> CFG_CLUSTERS( cfgClusters, cfgClusters+4 );
	const string cfgTiles[] = {
		"CLB", "INT", "INT_SO", "INT_SO_DCM0", "DSP", "BRAM"
	};
	set<string> CFG_TILES( cfgTiles, cfgTiles+6 );


#endif