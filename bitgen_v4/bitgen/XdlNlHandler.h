#ifndef _XDL_CKT_HANDLER_H_
#define _XDL_CKT_HANDLER_H_

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <exception>
#include <cassert>

#include <boost/xpressive/xpressive.hpp> //正则表达式

#include "Netlist.h"
#include "Args.h"


using std::string;

extern bitgen::Args ARGS;

//! XdlNlHandler has the reponsibility to parse xdl netlist into our specified data structure.

//! @return A handle to a Netlist

class XdlNlHandler
{

public:
	// explicit cktHandler(Circuit* curCkt) : _curCkt(curCkt) {}
	XdlNlHandler();
	~XdlNlHandler();
	Netlist* parse(const string &file);
private:
	Netlist *_netlist;
	string _contents;

	// 将xdl网标load进_contents里
	void loadXdl( const string &file);
};


#endif