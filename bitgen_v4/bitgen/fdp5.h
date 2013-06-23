#ifndef __FDP5_H__
#define __FDP5_H__


// This file defines some parameters of fdp5
#include <string>
#include <set>
#include <iostream>
#include <boost/assign.hpp>



//using std::set;
using namespace boost::assign;



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

	std::set<std::string> CFG_CLUSTERS =
		(list_of("SLICE"),"DSP48","FIFO16","RAMB16"
		);

	std::set<std::string> CFG_TILES = 
		(list_of("CLB"), "INT", "INT_SO", "INT_SO_DCM0", "DSP", "BRAM",
		"CLK_HROW_B", "CLK_HROW_T",
		"CLK_BUFGCTRL_B", "CLK_BUFCTRL_T",
		"CLK_IOB_B", "CLK_IOB_T",
		"CLKV_DCM_B", "CLKV_DCM_T",
		"HCLK", "HCLK_CENTER", "HCLK_CENTER_ABOVE_CFG",
		"HCLK_DCI", "HCLK_DCMIOB", "HCLK_IOBDCM", "HCLK_LVDS"
		);
#endif