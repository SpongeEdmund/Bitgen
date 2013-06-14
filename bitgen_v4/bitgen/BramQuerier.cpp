#include "BramQuerier.h"

void BramItem::print( std::ostream & os ) const
{
	os << "BRAM" ;
	os << "|Tile:" << tile;
	os << "|Site:" << site;
	os << "|Bram:"  << bram;
	os << "|InitialBits:" << initBits;
}

BramQuerier::BramQuerier(void)
{
}


BramQuerier::~BramQuerier(void)
{
}

void BramQuerier::loadSpec( Netlist& ckt )
{
	_curArch = _cil->getArchLib()->getArch(ckt.design.device.c_str());
	using std::vector;
	for ( vector<NetlistInst>::iterator instIt = ckt.insts.begin(); instIt != ckt.insts.end(); ++instIt ) {
		NetlistInst &curInst = *instIt;
		for ( vector<CktInstCfg>::iterator cfgIt = curInst.cfgs.begin(); cfgIt != curInst.cfgs.end(); ) {
			// 删去所有无用的cfg
			CktInstCfg &curCfg = *cfgIt;
			if ( curCfg.option.empty() 
				|| curCfg.option == "#OFF" 
				|| curCfg.attribute == "INIT_A"
				|| curCfg.attribute == "SRVAL_A"
			)
			{
				cfgIt = curInst.cfgs.erase(cfgIt);
				continue;
			}

			// 符合条件的存入数据结构
			if ( 
				(curCfg.attribute.substr(0,5) == "INIT_" && isdigit(curCfg.attribute[5]))
				|| (curCfg.attribute.substr(0,6) == "INITP_" && isdigit(curCfg.attribute[6]))
			) 
			{
				BramItem* aItem = new BramItem();
				aItem->tile = curInst.placedTile;
				aItem->site = curInst.placedSite;
				aItem->bram = curCfg.attribute;
				aItem->initBits = curCfg.option;
				addSpec(aItem);
				// 处理完之后就将该cfg删除
				cfgIt = curInst.cfgs.erase(cfgIt);
				continue;
			}
			cfgIt++;
		}
	}
}


std::vector<int> BramQuerier::hex2bin( const std::string& init )
{
	using std::vector;
	using std::string;

	size_t length = init.size();
	assert( !(length % 4) );
	//size_t bram_words = init.size() / 8;
	vector<int> result/*( length * 4, 0 )*/;
	//用来存放十六进制数
	//std::istringstream iss(init);
	std::ostringstream buf;
	//char buf;
	//assert(length % 4);
	for ( int hb = 0; hb != length; ++hb ) {

		buf << init[hb];

		if ( !((hb + 1) % 4) ) {
			std::cout << buf.str() << std::endl;
			string rword = buf.str();
			string word(rword.rbegin(),rword.rend());
			vector<int> temp = Hex2Bin(word);
			result.insert(result.end(), temp.begin(), temp.end());
			buf.flush();
		}


	}
	return result;

}

void BramQuerier::query()
{
	foreach( SpecItem* specItem, _specs)
	{
		BramItem* bramItem = static_cast<BramItem*>(specItem);
		_curTileInst = _curArch->getTileInstByName(bramItem->tile);
		BOOST_ASSERT(_curTileInst && "Cannot find tile inst.");
		//std::cout << _curTileInst->getName() << std::endl;

		// 获取tileInst的tile类型
		string tileTypeName = _curTileInst->getRef();
		_curTileType = _cil->getTileLib()->getTile(tileTypeName);
		BOOST_ASSERT(_curTileType);

		// 找到分配表
		string distTableName = _curTileInst->getDistTable(bramItem->site);
		_curSiteDist = _curTileType->getSiteDist(distTableName);
		BOOST_ASSERT(_curSiteDist);
		_curBram = _cil->getBramLib()->getBram(bramItem->bram);
		/*string siteTypeName = _curSiteDist->getRef();

		_curSiteType = _cil->getSiteLib()->getSite(siteTypeName);*/

		//std::cout << _curSiteType->getName() << std::endl;
		//Attribute* _curAttr = _curSiteType->getAttribute(bramItem->attribute);
		//BOOST_ASSERT( _curAttr && "Cannot find the attribute." );

		//Option* _curOption = _curAttr->getOption(bramItem->option);
		//BOOST_ASSERT( _curOption && "Cannot find the option." );

		// 将查到的Option下所有的sram配置情况填入_srams中
		std::vector<int> init_bits = hex2bin(bramItem->initBits);

		for ( int i = 0; i != init_bits.size(); ++i ) {
			SramBit* bit = new SramBit;
			bit->tileName = bramItem->tile;
			bit->tilePos  = _curTileInst->getPos();

			bit->siteName = bramItem->site;
			//bit->sramName = sram->getName();
			//bit->sramValue = sram->getValue();

			bit->sramValue = init_bits[i];

			// 找到bl和wl
			BramDist* curDis = _curSiteDist->getBramDist(bramItem->bram, i);
			BOOST_ASSERT(curDis);
			//?
			// 需要确认一下~！！
			bit->localPlace = Point(curDis->getBitLine(), curDis->getWordLine() + _curBram->getRow());
			bit->tileOffset = curDis->getOffset();
			_srams.push_back(bit);
		}
	}

}