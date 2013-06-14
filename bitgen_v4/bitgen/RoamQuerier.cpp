#include "RoamQuerier.h"

void RoamItem::print( std::ostream & os ) const
{
	os << "Ram/Rom";
	os << "|Tile:" << tile;
	os << "|Site:" << site;
	os << "|LUT:"  << lut;
	os << "|Mode:" << mode;
	os << "|InitialBits:" << initBits;
}

RoamQuerier::RoamQuerier(void)
{
}


RoamQuerier::~RoamQuerier(void)
{
}


void RoamQuerier::loadSpec( Circuit& ckt )
{
	_curArch = _cil->getArchLib()->getArch(ckt.design.device.c_str());
	using std::vector;
	for ( vector<CktInst>::iterator instIt = ckt.insts.begin(); instIt != ckt.insts.end(); ++instIt ) {
		CktInst &curInst = *instIt;
		for ( vector<CktInstCfg>::iterator cfgIt = curInst.cfgs.begin(); cfgIt != curInst.cfgs.end(); ) {
			// ɾȥ�������õ�cfg
			CktInstCfg &curCfg = *cfgIt;
			if ( curCfg.option.empty() || curCfg.option == "#OFF" ) {
				cfgIt = curInst.cfgs.erase(cfgIt);
				continue;
			}

			// ���������Ĵ������ݽṹ
			if ( curCfg.option.substr(0, 4) == "#RAM" || curCfg.option.substr(0, 4) == "#ROM") {
				RoamItem *aItem = new RoamItem();
				aItem->tile = curInst.placedTile;
				aItem->site = curInst.placedSite;
				aItem->lut  = curCfg.attribute;
				aItem->mode = curCfg.option.substr(0, 4);
				aItem->initBits  = curCfg.option.substr(9);
				addSpec(aItem);
				// ������֮��ͽ���cfgɾ��
				cfgIt = curInst.cfgs.erase(cfgIt);
				continue;
			}
			cfgIt++;
		}
	}
	
}

void RoamQuerier::query( )
{
	//TODO
}