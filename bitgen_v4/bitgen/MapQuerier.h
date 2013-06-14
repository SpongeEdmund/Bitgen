#ifndef __MAP_QUERIER_H__
#define __MAP_QUERIER_H__


#include <string>

#include "CfgQuerier.h"

using std::string;

struct MapItem : public SpecItem
{
	string tile; // tile inst
	string site; // site inst
	string attribute; // attribute name
	string option;    // option name
	void print( std::ostream & os /* = std::cout */ ) const;
};


// 最常见的是通过键值对直接查找sram

class MapQuerier : public CfgQuerier
{
public:
	MapQuerier(Cil* cil)
		: _curTileInst(0),
		  _curTileType(0),
		  _curSiteDist(0),
		  _curSiteType(0),
		  CfgQuerier(cil)
	{
	}


	void loadSpec( Netlist& ckt );

	const char* type() const { return "map"; }

	void query();
private:
	TileInst*       _curTileInst;
	Tile*           _curTileType;
	SiteDist*       _curSiteDist;
	Site*           _curSiteType;
};


#endif