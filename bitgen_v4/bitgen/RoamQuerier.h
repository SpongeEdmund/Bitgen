#ifndef __ROAM_QUERIER_H__
#define __ROAM_QUERIER_H__

#include "CfgQuerier.h"
#include <string>
#include <vector>

using std::string;
using std::vector;


struct RoamItem : public SpecItem
{
	string tile;
	string site;
	string lut;
	string mode;
	string initBits;
	void print( std::ostream & os ) const ;
};


class RoamQuerier :
	public CfgQuerier
{
public:
	RoamQuerier(void);
	RoamQuerier(Cil* cil) : CfgQuerier(cil) {}
	~RoamQuerier(void);

	void loadSpec( Circuit& ckt );
	const char* type() const { return "ram|rom"; }
	void query();

};

#endif