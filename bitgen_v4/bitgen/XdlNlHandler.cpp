#include "XdlNlHandler.h"
#include "AdjustBiPipDir.h"
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost::xpressive;
using boost::lexical_cast;
// Constructor
XdlNlHandler::XdlNlHandler()
	:_netlist(0), _usedSlices(0)
{
	
}

XdlNlHandler::~XdlNlHandler()
{
	// do nothing
}

// private methods
// Load in the xdl ascii file in. 

void XdlNlHandler::loadXdl( const string &file )
{
	ifstream infile(file.c_str());
	
	if (!infile) throw (exception("Cannot open xdl file."));

	ostringstream oss;
	// char buf
	char c;
	while (infile.get(c))
		oss.put(c);

	_contents = oss.str();
}

//public methods
Netlist* XdlNlHandler::parse( const string &file )
{
	assert( 0 == _netlist );
	_netlist = new Netlist;
	loadXdl(file);
	//std::cout << _contents << std::endl;

	// 如果contents匹配 
	// 1. design ...;
	// 2. inst ...;
	// 3. net ...;

	// xdl中object的正则表达式
	const string cktObjRe = 
		"^\\s*" // 可能存在的前置空格符
		"(design|inst(ance)*|net)\\s+(.*?)\\s*\\n*;" // [1] = objName, [3] = objInfo
	;
	sregex cktObjRegex = sregex::compile(cktObjRe);
	sregex_iterator pos(_contents.begin(), _contents.end(), cktObjRegex);
	sregex_iterator end;
	while ( pos != end ) {
		string objName = (*pos)[1], objInfo = (*pos)[3];

		if ( objName == "design" ) {
			//cout << "parsing design" << endl;
			const string designRe =
				// [1]=designName [2]=deviceName [3]=package [4]=speed [5]=ncdVersion
				"\"(.*?)\"\\s+([a-z]+\\d+[a-z]+\\d+)(\\w+)(-\\d+)\\s+([vV]\\d+\\.\\d+)\\s*.*"
			;

			sregex designRegex = sregex::compile(designRe);
			smatch what;

			bool mDesignRegex = regex_match(objInfo, what, designRegex);
			assert( mDesignRegex );

			_netlist->design.designName = what[1];
			_netlist->design.device     = what[2];
			_netlist->design.package    = what[3];
			_netlist->design.speed      = what[4];
			_netlist->design.ncdVersion = what[5];
			
			// 显示芯片信息
			// cout << _ckt->design << endl;


		} else if ( objName == "inst") {
			//cout << "parsing inst" << endl;
			NetlistInst aInst;

			const string instRe =
				// [1]=instName, [2]=instType, [3]=placedTile, [4]=placedSite
				"\"(.*?)\"\\s+\"(\\w+)\"\\s*,\\s*placed\\s+(\\w+)\\s+(\\w+)\\s*,.*?"
				// [5]=cfgInfo
				"cfg\\s+\"\\s*(.*?)\\s*\\n*\"\\s*"
			;

			sregex instRegex = sregex::compile(instRe);
			smatch what;

			bool mInstRegex = regex_match(objInfo, what, instRegex);
			assert(mInstRegex);
			
			//cout << "inst : " << what[1] << " type : " << what[2] << endl;
			//cout << "placed in tile " << what[3] << " site " << what[4] << " with cfg below :" << endl;
			//cout << what[5] << endl;

			aInst._instName   = what[1];
			aInst._siteDef    = what[2];
			
			if ( aInst._siteDef.substr(0,5) == "SLICE" ) 
				++_usedSlices;
			
			aInst._placedTile = what[3];
			aInst._placedSite = what[4];

			//将cfgInfo按空格进行分割，并对每一个单独的属性键值对进行分析
			const string cfgInfo = what[5];
			sregex splitRe = sregex::compile("\\s+");
			sregex_token_iterator pos(cfgInfo.begin(), cfgInfo.end(), splitRe, -1);
			sregex_token_iterator end;
			while ( pos != end ) {
				string aInstCfgInfo = *pos;
				// cout << "[" << *pos << "]" << endl;
				// 对属性配置值进行判断
				sregex instCfgRegex1 = sregex::compile("(.+?):(.*?):(.*?)");
				sregex instCfgRegex2 = sregex::compile("(.+?)=([01])");
				//对cfgInfo进行匹配
				smatch what;

				if ( regex_match(aInstCfgInfo, what, instCfgRegex1) ) 
				{
					//ignored_attribute;
					// modified on 2013 3 12
					if (what[1] == "_INST_PROP" || what[1] == "_BEL_PROP" ) {
						++pos;
						continue;
					}

					InstCfg aInstCfg;
					aInstCfg._attribute = what[1];
					// Routethrough is a pip implemented using site
					if ( what[1] == "_ROUTETHROUGH" ) {
						string from = what[2];
						string to = what[3];
						aInstCfg._option  = from.append(":").append(to);
					} else {
						aInstCfg._instName = what[2];
						aInstCfg._option = what[3];
					}

					aInst._cfgs.push_back(aInstCfg);
				}
				// cfg段中以sramName = sramValue的形式来补充用xilinx无法覆盖的配置描述
				else if ( regex_match( aInstCfgInfo, what, instCfgRegex2 ) )
				{
					InstCfg aInstCfg;
					aInstCfg._type = InstCfg::Sram;
					aInstCfg._sramName = what[1];
					aInstCfg._sramVal = lexical_cast<int>( what[2] );
					aInst._cfgs.push_back(aInstCfg);
				}

				//cout << "Cfg attribute " << aInstCfg.attribute;
				//cout << " inst name " << aInstCfg.instName;
				//cout << " option " << aInstCfg.option << endl;

				
				++pos;
			}
			_netlist->insts.push_back(aInst);
			
		} else if ( objName == "net") {
			
			NetlistNet aNet;
			// cout << "parsing net" << endl;
			// 
			// cout << objInfo << endl;
			const string netRe1 = 
				"\"(.*?)\"(\\s+(gnd|vcc))?\\s*,\\s*\\n*(.*?)\\s*\\n*"
			;
			sregex netRegex = sregex::compile(netRe1);
			
			smatch what;
			
			bool mNetRegex = regex_match(objInfo, what, netRegex);
			
			if ( !mNetRegex ) continue;

			aNet.netName = what[1];
			string netInfo = what[4];

			const string netChildRe = 
				"(outpin|inpin|pip)\\s+(.*?)\\s*,"
			;
			sregex netChildRegex = sregex::compile(netChildRe);
			sregex_iterator pos(netInfo.begin(), netInfo.end(), netChildRegex);
			sregex_iterator end;
			while ( pos != end ) {
				string netChildType = (*pos)[1], netChildInfo = (*pos)[2];
				if ( netChildType == "outpin" || netChildType == "inpin" ) {
					NetPin aPin;
					aPin.dir = netChildType == "outpin" ? NetPin::outpin : NetPin::inpin ;
					sregex pinRegex = sregex::compile("\"(.*?)\"\\s+(\\w+)\\s*");
					
					smatch what;
					bool mPinRegex = regex_match(netChildInfo, what, pinRegex);
					assert(mPinRegex);
					aPin.instRefName = what[1];
					aPin.pinName = what[2];

					//cout << "pin " << what[1] << " " << what[2] << endl;

					aNet.pins.push_back(aPin);
				} else {
					NetPip aPip;
					// modified on 2013 3 12
					sregex pipRegex = sregex::compile("(\\w+)\\s+(\\w+)\\s*([-=>]+)\\s*(\\w+)");
					smatch what;
					bool mPipRegex = regex_match(netChildInfo, what, pipRegex);
					assert( mPipRegex );
					aPip.tileName = what[1];
					aPip.srcNet   = what[2];
					aPip.dirType  = what[3];
					aPip.snkNet   = what[4];
					aNet.pips.push_back(aPip);

					//cout << "pip " << what[1] << " " << what[2] << "->" << what[3] << endl;
				}
				++pos;
			} // end of net info parsing
			AdjustNet(aNet);
			_netlist->nets.push_back(aNet);
		} else {
			// Throw unknown type of netlist
		}
		++pos;
	}
	return _netlist;
}