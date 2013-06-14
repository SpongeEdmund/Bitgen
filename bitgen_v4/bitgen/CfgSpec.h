#ifndef __CFG_SPEC_H__
#define __CFG_SPEC_H__

#include <iostream>
#include <vector>

#include "Circuit.h"
#include "Cil.h"

using std::vector;

struct CfgItem
{
	virtual ~CfgItem() {}
};

class CfgQuerier
{
public:
	CfgQuerier(void);
	virtual ~CfgQuerier(void);
	virtual void loadSpec( Circuit& ckt ) = 0; 
	virtual void show( std::ostream & os = std::cout ) const = 0;
	virtual const char * type() const = 0;

	virtual void query( const Cil & cil ) = 0;

	void addItem( CfgItem* newItem ) { _items.push_back(newItem); }
	vector<CfgItem*>::iterator begin() { return _items.begin(); }
	vector<CfgItem*>::iterator end()   { return _items.end();   }
	vector<CfgItem*>::const_iterator begin() const { return _items.begin(); }
	vector<CfgItem*>::const_iterator end() const   { return _items.end(); }
private:
	vector<CfgItem*> _items;

};

#endif