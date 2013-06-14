#ifndef __BOOL_SPEC_H__
#define __BOOL_SPEC_H__



#include "CfgQuerier.h"
#include "LutCalc.h"


using std::string;
using std::ostream;

struct LutItem : public SpecItem
{
	string tile;
	string site;
	string lut;      
	string mode;
	string expr; // 布尔表达式
	void print( std::ostream & os ) const ;
};


class LutQuerier : public CfgQuerier
{
public:

	LutQuerier(Cil* cil)
		: CfgQuerier(cil),
		  _curTileInst(0),
		  _curTileType(0),
		  _curSiteDist(0),
		  _curSiteType(0)
	{}

	void loadSpec( Netlist & ckt );
	const char* type() const { return "boolexpr"; }
	void query();
private:
	TileInst* _curTileInst;
	Tile*     _curTileType;
	SiteDist* _curSiteDist;
	Site*     _curSiteType;

};

#endif