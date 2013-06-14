#ifndef __BRAM_QUERIER_H__
#define __BRAM_QUERIER_H__

#include "CfgQuerier.h"
#include "utils.h"
#include <string>
#include <locale>
#include <sstream>

using std::string;

struct BramItem : public SpecItem
{
	string tile;
	string site;
	string bram;
	string initBits;
	void print( std::ostream & os /* = std::cout */ ) const;
};

class BramQuerier :
	public CfgQuerier
{
public:
	BramQuerier(void);
	BramQuerier(Cil* cil)
		: _curTileInst(0),
		_curTileType(0),
		_curSiteDist(0),
		_curBram(0),
		CfgQuerier(cil)
	{
	}
	~BramQuerier(void);

	void loadSpec( Netlist& ckt );
	const char* type() const { return "bram"; }
	void query();

	std::vector<int> hex2bin( const std::string& init );
private:
	TileInst*       _curTileInst;
	Tile*           _curTileType;
	SiteDist*       _curSiteDist;
	Bram*           _curBram;
	//Site*           _curSiteType;

};

#endif