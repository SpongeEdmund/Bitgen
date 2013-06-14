#ifndef _CFG_SPEC_H_
#define _CFG_SPEC_H_

#include <string>
#include <vector>
#include <algorithm>
#include <exception>
#include <cassert>
#include <iostream>
#include "Circuit.h"

//#include "CktInst.h"
//#include "CktNet.h"

using std::string;
using std::vector;
using std::ostream;

// 顶层逻辑配置数据结构
struct LogicSpec
{
	enum Type
	{
		KEY_VALUE,
		BOOLEAN_LOGIC,
		SRAM_BIT
	};
	string tileName;
	string siteName;
	string siteType;
	string attribute;
	string option;
	Type   type;
	LogicSpec(LogicSpec::Type t = KEY_VALUE) : type(t) {}
	void specifyType();
	//bool isKeyValue();
	bool isBooleanLogic();
	bool isSramBit();


	bool belongsToSlice();
	bool belongsToBram();
	friend ostream& operator<< ( ostream &os, const LogicSpec &logicCfg );
};

// 顶层互联配置数据结构
struct RouteSpec
{
	string tileName;
	string srcNet;
	string snkNet;
	friend ostream & operator<< ( ostream &os, const RouteSpec &routeCfg );
};

class CfgSpec
{
	typedef std::vector<LogicSpec> LogicCfgs;
	typedef std::vector<RouteSpec> RouteCfgs;

public:
	CfgSpec() {}
	CfgSpec(const Circuit &);
	void constructFromCkt( const Circuit & );
	void clear() { _logicCfgs.clear(); _routeCfgs.clear(); }
	void showLogicSpec( std::ostream & os ) { 
		for(LogicCfgs::iterator lit = logicCfgBegin(); lit != logicCfgEnd(); ++lit) 
			os << *lit << std::endl;
	}
	void showRouteSpec( std::ostream & os ) { 
		for( RouteCfgs::iterator rit = routeCfgBegin(); rit != routeCfgEnd(); ++rit )
			os << *rit << std::endl;
	}
	LogicCfgs::iterator logicCfgBegin() { return _logicCfgs.begin(); }
	LogicCfgs::const_iterator logicCfgBegin() const { return _logicCfgs.begin(); }
	LogicCfgs::iterator logicCfgEnd() { return _logicCfgs.end(); }
	LogicCfgs::const_iterator logicCfgEnd() const { return _logicCfgs.end(); }

	RouteCfgs::iterator routeCfgBegin() { return _routeCfgs.begin(); }
	RouteCfgs::const_iterator routeCfgBegin() const { return _routeCfgs.begin(); }
	RouteCfgs::iterator routeCfgEnd() { return _routeCfgs.end(); }
	RouteCfgs::const_iterator routeCfgEnd() const { return _routeCfgs.end(); }

private:
	LogicCfgs _logicCfgs;
	RouteCfgs _routeCfgs;

	//bool _fDefault;
	//bool _fUsed;

	void getLogicCfgFromCkt( const Circuit & );
	void getRouteCfgFromCkt( const Circuit & );
};
#endif