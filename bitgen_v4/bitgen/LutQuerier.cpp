#include "LutQuerier.h"

void LutItem::print( std::ostream & os ) const
{
	os << "LutExp";
	os << "|Tile:" << tile;
	os << "|Site:" << site;
	os << "|LUT:"  << lut;
	os << "|Mode:" << mode;
	os << "|Expr:" << expr;
}

void LutQuerier::loadSpec( Netlist & ckt )
{
	_curArch = _cil->getArchLib()->getArch(ckt.design.device.c_str());

	using std::vector;
	for ( vector<NetlistInst>::iterator instIt = ckt.insts.begin(); instIt != ckt.insts.end(); ++instIt ) {
		NetlistInst &curInst = *instIt;
		for ( vector<CktInstCfg>::iterator cfgIt = curInst.cfgs.begin(); cfgIt != curInst.cfgs.end(); ) {
			// 删去所有无用的cfg
			CktInstCfg &curCfg = *cfgIt;
			if ( curCfg.option.empty() || curCfg.option == "#OFF" ) {
				cfgIt = curInst.cfgs.erase(cfgIt);
				continue;
			}

			// 符合条件的存入数据结构
			if ( curCfg.attribute == "F" || curCfg.attribute == "G") {
				LutItem *aItem = new LutItem();
				aItem->tile = curInst.placedTile;
				aItem->site = curInst.placedSite;
				aItem->lut  = curCfg.attribute;
				aItem->mode = curCfg.option.substr(0, 4);
				aItem->expr  = curCfg.option.substr(7);
				addSpec(aItem);
				// 处理完之后就将该cfg删除
				cfgIt = curInst.cfgs.erase(cfgIt);
				continue;
			}
			cfgIt++;
		}
	}
}



void LutQuerier::query()
{
	foreach( SpecItem* specItem, _specs)
	{
		LutItem* boolItem = static_cast<LutItem*>(specItem);
		_curTileInst = _curArch->getTileInstByName(boolItem->tile);
		BOOST_ASSERT(_curTileInst && "Cannot find tile inst.");
		

		// 获取tileInst的tile类型
		string tileTypeName = _curTileInst->getRef();
		_curTileType = _cil->getTileLib()->getTile(tileTypeName);


		// 找到分配表
		string distTableName = _curTileInst->getDistTable(boolItem->site);
		_curSiteDist = _curTileType->getSiteDist(distTableName);


		string siteTypeName = _curSiteDist->getRef();
		_curSiteType = _cil->getSiteLib()->getSite(siteTypeName);

		

		//std::cout << _curSiteType->getName() << std::endl;
		Attribute* _curAttr = _curSiteType->getAttribute(boolItem->lut);
		BOOST_ASSERT( _curAttr && "Cannot find the attribute." );

		Option* _curOption = _curAttr->getOption(boolItem->mode);
		BOOST_ASSERT( _curOption && "Cannot find the option." );
		vector<int> truthTable = Exp2LUT(boolItem->expr, 16).toVec();

		// 控制模式选择的sram们
		for( xml_node<>* sramNode = _curOption->first_node("sram");
			sramNode; sramNode = sramNode->next_sibling("sram") )
		{
			SramInSite* sram = static_cast<SramInSite*>(sramNode);

			SramBit* bit = new SramBit;
			bit->tileName = boolItem->tile;
			bit->tilePos  = _curTileInst->getPos();

			bit->siteName = boolItem->site;
			bit->sramName = sram->getName();


			if ( sramNode->first_attribute("value") ) {

				bit->sramValue = sram->getValue();

			}

			if ( xml_attribute<>* sramAttr = sramNode->first_attribute("address") ) {
				int sramAddress = sram->getAddress();
				assert(sramAddress >= 0);
				bit->sramValue = truthTable[sramAddress];
			}

			// 找到bl和wl
			SramDist* curDis = _curSiteDist->getSramDist(bit->sramName);
			bit->localPlace = Point(curDis->getBitLine(), curDis->getWordLine());
			bit->tileOffset = curDis->getOffset();
			_srams.push_back(bit);
		}
	}
}




