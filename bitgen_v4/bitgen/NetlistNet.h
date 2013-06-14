#ifndef _CKT_NET_H_
#define _CKT_NET_H_

#include <string>
#include <vector>
struct RouteSpec;
using std::string;

struct NetPin
{
	enum Dir { inpin, outpin } dir;
	string pinName;
	string instRefName;
};

struct NetPip
{
	string tileName;
	string srcNet;
	string snkNet;
	string dirType;
};

struct NetlistNet
{
	typedef std::vector<NetPin> NetPins;
	typedef std::vector<NetPip> NetPips;
	
	string netName;
	NetPins pins;
	NetPips pips;

	//void listRouteCfg( RouteCfg &cfgBits ) {};

};

#endif