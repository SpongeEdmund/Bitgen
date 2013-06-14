#ifndef _XDL_CKT_HANDLER_H_
#define _XDL_CKT_HANDLER_H_

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <exception>
#include <cassert>

#include <boost/xpressive/xpressive.hpp> //������ʽ

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

	// ��xdl����load��_contents��
	void loadXdl( const string &file);
};


#endif