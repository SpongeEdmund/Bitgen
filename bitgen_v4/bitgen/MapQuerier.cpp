#include "MapQuerier.h"

void MapItem::print( std::ostream & os ) const
{
	os << "Map";
	os << "|Tile:" << tile;
	os << "|Site:" << site;
	os << "|Attribute:" << attribute;
	os << "|Option:" << option;
}

void MapQuerier::loadSpec( Netlist& ckt )
{
	_curArch = _cil->getArchLib()->getArch(ckt.design.device.c_str());
	using std::vector;
	for ( vector<NetlistInst>::iterator instIt = ckt.insts.begin(); instIt != ckt.insts.end(); ++instIt ) {
		NetlistInst &curInst = *instIt;
		for ( vector<CktInstCfg>::iterator cfgIt = curInst.cfgs.begin(); cfgIt != curInst.cfgs.end(); ) {
			// 删去所有无用的cfg
			CktInstCfg &curCfg = *cfgIt;
			//if ( curCfg.option.empty() || curCfg.option == "#OFF" ) {
			//	cfgIt = curInst.cfgs.erase(cfgIt);
			//	continue;
			//}
			MapItem *aItem = new MapItem();
			aItem->tile = curInst.placedTile;
			aItem->site = curInst.placedSite;
			aItem->attribute  = curCfg.attribute;
			aItem->option = curCfg.option;
			addSpec(aItem);
			// 处理完之后就将该cfg删除
			cfgIt++;
		}
	}
}

void MapQuerier::query()
{
	foreach( SpecItem* specItem, _specs)
	{
		MapItem* mapItem = static_cast<MapItem*>(specItem);
		_curTileInst = _curArch->getTileInstByName(mapItem->tile);
		BOOST_ASSERT(_curTileInst && "Cannot find tile inst.");
		//std::cout << _curTileInst->getName() << std::endl;

		// 获取tileInst的tile类型
		string tileTypeName = _curTileInst->getRef();
		_curTileType = _cil->getTileLib()->getTile(tileTypeName);


		// 找到分配表
		string distTableName = _curTileInst->getDistTable(mapItem->site);
		_curSiteDist = _curTileType->getSiteDist(distTableName);
		
		
		string siteTypeName = _curSiteDist->getRef();

		_curSiteType = _cil->getSiteLib()->getSite(siteTypeName);

		//std::cout << _curSiteType->getName() << std::endl;
		Attribute* _curAttr = _curSiteType->getAttribute(mapItem->attribute);
		BOOST_ASSERT( _curAttr && "Cannot find the attribute." );

		Option* _curOption = _curAttr->getOption(mapItem->option);
		BOOST_ASSERT( _curOption && "Cannot find the option." );

		// 将查到的Option下所有的sram配置情况填入_srams中

		for( xml_node<>* sramNode = _curOption->first_node("sram");
			sramNode; sramNode = sramNode->next_sibling("sram") )
		{
			SramInSite* sram = static_cast<SramInSite*>(sramNode);

			SramBit* bit = new SramBit;
			bit->tileName = mapItem->tile;
			bit->tilePos  = _curTileInst->getPos();

			bit->siteName = mapItem->site;
			bit->sramName = sram->getName();
			bit->sramValue = sram->getValue();

			// 找到bl和wl
			SramDist* curDis = _curSiteDist->getSramDist(bit->sramName);
			bit->localPlace = Point(curDis->getBitLine(), curDis->getWordLine());
			bit->tileOffset = curDis->getOffset();
			_srams.push_back(bit);
		}
	}
	
}

