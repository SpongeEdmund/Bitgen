#ifndef __ARCH_LIB_H__
#define __ARCH_LIB_H__

#include "CilBase.h"
#include "TileLib.h"
#include "utils.h"
#include <boost/lexical_cast.hpp>

class NominationMap : public CilBase
{
public:
	string getNominatedName() const { return first_attribute("name")->value(); }
	string getSiteInstName() const { return first_attribute("inst")->value(); }
};


class TileInst : public CilBase
{
public:
	
	string getName() const { return first_attribute("name")->value(); }
	string getRef() const { return first_attribute("ref")->value(); }

	Point getPos() const { return boost::lexical_cast<Point>(first_attribute("pos")->value()); }
	//int getLabel() const { return boost::lexical_cast<int>(first_attribute("label")->value()); }

	int getCol() const
	{
		if ( xml_attribute<>* colAttr = first_attribute("col") )
			return boost::lexical_cast<int>(colAttr->value());
		else
			return -1;
	}
	// 根据分配的site inst的名称查询原有site inst的名称
	string getDistTable( const string& nominatedName ) const;

};

class Arch : public CilBase
{
public:
	string getName() const { return first_attribute("name")->value(); }
	TileInst* getTileInstByName( const string& tileName ); 
	TileInst* getTileInstByPos( const Point& pos );
};


class ArchLib : public CilBase
{
public:
	Arch* getArch( const string& archName );

	void accept( CilVisitor& visitor ) { visitor.visitArchLib(this); }

};

#endif