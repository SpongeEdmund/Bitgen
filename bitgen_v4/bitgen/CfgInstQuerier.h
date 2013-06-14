// File name: CfgInstQuerier.h
// Description: 
//   CfgInstQuerier is the base class of configuration queriers such as SLICEQuerier,
//   BRAMQuerier, IOBQuerier, DSPQuerier, DCMQuerier etc.
//   If you want to creat a querier of a new kind of CfgInst, you should inherit from
//   this class and rewrite methods:
//     - loadFromCkt
//     - getPosInTile

// Author : Chi Wang
// Date: 2013-3-23


#ifndef __CFG_INST_QUERIER_H__
#define __CFG_INST_QUERIER_H__

#include <string>
#include <vector>
#include "Circuit.h"
#include "SramBit.h"
#include "Cil.h"

using std::string;
using std::vector;
//! class CfgInstQuerier
/*! 
	A CfgInstQuerier can load corresponding instance configuration information from class Circuit,
    according to the type of the instance.
*/

class CfgInstQuerier
{
public:
	////! Constructor
	//CfgInstQuerier();
	//! Construct from Circuit
	explicit CfgInstQuerier( const Netlist & circuit );
	
	//! Query the sram bit configuration from the cil
	void queryCil( const Cil & cil, vector<SramBit> & bits );
protected:

	vector<NetlistInst> _insts;

	
	//Arch*     _curArch;     //!< The current architecture, got according to the chip info given by the CktDesign
	//TileInst* _curTileInst; //!< The current tile instance
	//Tile*     _curTileType; //!< The current tile type
	//SiteInst* _curSiteInst; //!< The current site instance
	//Site*     _curSiteType; //!< The current site type



	//! Get the z position of the instance inside the landlord tile.
	/*! 
		\param  placedSite placedTile is named after the rule TYPE_X?Y?, such as SLICE_X0Y0
	*/
	virtual size_t getPosInTile( const string& placedSite ) = 0;

	//! Load configuration requests from the circuit 
	
	virtual void loadCfgs( const Netlist & circuit ) = 0;
	
	//! Do one cil query according to one configuration request.
	/*!
		There are 3 main kinds of requests classified by the expression of the request.
		 - Attribute + Mode. The most common kind. You can find from the cil the attribute 
		   node in the site, then the option node, the srams appending to the option node are what
		   to be searched.
		 - Attribute + Mode + Addictive Expression. This can be seen when configuring LUT.
		   An LUT's configuration request can be described as F + #LUT + D=A1+A2+A3+A4
		 - Attribute + Expression. This is often seen in the bram initial value configuration.
		 Here we should deal with the three kinds of requests separately.
	*/
	virtual void doOneQuery( const Cil& cil, const CktInstCfg& cfg, vector<SramBit> & bits ) = 0;

	void doMapQuery( const Cil & cil, const CktInstCfg & cfg, vector<SramBit> & bits);
};

#endif