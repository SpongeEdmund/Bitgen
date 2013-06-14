#include "CfgSpec.h"

CfgQuerier::CfgQuerier(void)
{

}

CfgQuerier::~CfgQuerier(void)
{
	for ( vector<SpecItem*>::iterator it = begin(); it != end(); ++it ) {
		delete *it;
	}
}

//
////////////////////////////////////////////////////////////////////////////
//// LogicCfg
//bool LogicSpec::belongsToSlice()
//{
//	return (
//		siteType.find("SLICE") != string::npos
//	);
//}
//
//
//bool LogicSpec::belongsToBram()
//{
//	return (
//		siteType.find("RAMB") != string::npos
//	);
//}
//
//bool LogicSpec::isSramBit()
//{
//	bool isRamOrRom = (
//		belongsToSlice()
//		&& ( option.find("#RAM") != string::npos || option.find("#ROM") != string::npos )
//	);
//
//	bool isBram = (
//		belongsToBram()
//		&& ( attribute.find("INIT") != string::npos || attribute.find("SRVAL") != string::npos)
//	);
//
//	return (isRamOrRom || isBram);
//
//}
//
//bool LogicSpec::isBooleanLogic()
//{
//	return ( 
//		!siteType.empty() && !option.empty()
//		&& siteType.find("SLICE") != string::npos
//		&& option.find("#LUT") != string::npos
//	);
//
//}
//
//void LogicSpec::specifyType()
//{
//	assert(!tileName.empty());
//	if ( isBooleanLogic() ) {
//		// 删去 #LUT:D=
//		option.erase(0, 7);
//		// std::cout << "logic" << std::endl;
//		type = BOOLEAN_LOGIC;
//	}
//	else if ( isSramBit() ) {
//		type = SRAM_BIT;
//		if ( option.find("#R") != string::npos )
//			option.erase(0, 7);
//	} else {/* do nothing*/}
//}
//
//
//ostream& operator<< ( ostream &os, const LogicSpec &logicCfg )
//{
//	os << logicCfg.tileName << "." << logicCfg.siteName << "." << logicCfg.attribute <<  " : " << logicCfg.option << " type : " << logicCfg.type;
//	return os;
//}
//
//ostream & operator << ( ostream &os, const RouteSpec &routeCfg )
//{
//	os << routeCfg.tileName << " : " << routeCfg.srcNet << " -> " << routeCfg.snkNet;
//	return os;
//}
///////////////////////////////////////////////////////////////////////////
//// CfgSpec
//CfgSpec::CfgSpec( const Circuit &ckt )
//{
//	// 从Circuit数据结构中获取逻辑配置信息表
//	constructFromCkt(ckt);
//}
//
//void CfgSpec::constructFromCkt( const Circuit & ckt )
//{
//	getLogicCfgFromCkt(ckt);
//	getRouteCfgFromCkt(ckt);
//}
//
//void CfgSpec::getLogicCfgFromCkt( const Circuit &ckt )
//{
//	for ( vector<CktInst>::const_iterator instIt = ckt.insts.begin(); instIt != ckt.insts.end(); ++instIt ) {
//		for ( vector<CktInstCfg>::const_iterator cfgIt = (*instIt).cfgs.begin(); cfgIt != (*instIt).cfgs.end(); ++cfgIt ) {
//			// 忽略没有option的配置
//			// 忽略option为#OFF的配置
//			if ( (*cfgIt).option.empty() || (*cfgIt).option == "#OFF" ) continue;
//			
//			LogicSpec aLogicCfg;
//			aLogicCfg.tileName  = (*instIt).placedTile;
//			aLogicCfg.siteName  = (*instIt).placedSite;
//			aLogicCfg.siteType  = (*instIt).siteDef;
//			aLogicCfg.attribute = (*cfgIt).attribute;
//			aLogicCfg.option    = (*cfgIt).option;
//			aLogicCfg.specifyType();
//			// 
//			std::cout << aLogicCfg << std::endl;
//
//			_logicCfgs.push_back(aLogicCfg);
//		}
//	}
//}
//
//
//void CfgSpec::getRouteCfgFromCkt( const Circuit &ckt )
//{
//	for ( vector<CktNet>::const_iterator netIt = ckt.nets.begin(); netIt != ckt.nets.end(); ++netIt ) {
//		for ( vector<CktNetPip>::const_iterator pipIt = (*netIt).pips.begin(); pipIt != (*netIt).pips.end(); ++pipIt ) {
//			
//			RouteSpec aRouteCfg;
//			aRouteCfg.tileName = (*pipIt).tileName;
//			aRouteCfg.srcNet   = (*pipIt).srcNet;
//			aRouteCfg.snkNet   = (*pipIt).snkNet;
//			//
//			std::cout << aRouteCfg << std::endl;
//			_routeCfgs.push_back(aRouteCfg);
//		}
//	}
//}
