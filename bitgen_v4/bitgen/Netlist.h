#ifndef __NETLIST_H__
#define __NETLIST_H__

#include "NetlistDesign.h"
#include "NetlistInst.h"
#include "NetlistNet.h"

#include <vector>

struct Netlist
{
	typedef std::vector<NetlistInst> NlInsts;
	typedef std::vector<NetlistNet>  NlNets;

	NetlistDesign design;
	NlInsts      insts;
	NlNets       nets;
	
};
#endif