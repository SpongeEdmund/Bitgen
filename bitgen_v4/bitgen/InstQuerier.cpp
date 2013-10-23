#include "InstQuerier.h"
#include "utils.h"
#include "LutCalc.h"
#include "Exceptions.h"
#include <algorithm>

#include <cstdlib>
#include <cctype>
#include <vector>

#include <boost/lexical_cast.hpp>
//#include "fdp5params.cpp"
#include "fdp5.h"
using std::set;
using std::vector;
using std::string;
using std::istringstream;
using boost::lexical_cast;
using std::transform;
using std::abs;

//using namespace FDP5;
//extern const unsigned BRAM_LENGTH_PER_BLOCK;
//extern const unsigned BRAM_PARITY_LENGTH;
//extern set<string> CFG_CLUSTERS;
//extern set<string> CFG_TILES;

namespace bitgen {




	void InstQuerier::runQuery( SramVec & sramVec )
	{
		//std::cout << "specify arch" << std::endl;
		specifyArch();

		// main query process of inst
		foreach( const NetlistInst & inst, _netlist.insts ) {
			
#ifdef _TEST	
			std::cout << "<DebugInfo> Querying configuration for instance: "
				<< inst._instName << " which is placed in site: "
				<< inst._placedSite << " tile: " << inst._placedTile
				<< std::endl;
#endif
			string tileInstName       = inst._placedTile;

			// find current tile inst
			_cfgHir._curTileInst = _nameTileMap[tileInstName];

#ifdef _TEST
			stringstream tileInstMissInfo;
			tileInstMissInfo << "[!Error!] Cannot find tile instance: " << tileInstName;
			CONDITIONAL_THROW (
				_cfgHir._curTileInst != 0,
				CilInfoMissException, 
				tileInstMissInfo.str()
				)
#endif
			

			// find current tile type
			string tileName = _cfgHir._curTileInst->get_ref();

			// If the current tile is not listed in the confugrable tile list, just ignore it.
			if ( !CFG_TILES.count(tileName) ) {
#ifdef _TEST
				std::cout << "  <Warning>  There is no configuration for tile: " << tileName << std::endl;
#endif
				continue;
			}

			_cfgHir._curTile = _cil.root()->get_tile_lib()->find_tile_by_name( tileName );
			//assert( _cfgHir._curTile != 0 );
#ifdef _TEST
			stringstream tileTypeMissInfo;
			tileTypeMissInfo << "[!Error!] Cannot find tile type: " << tileName;
			CONDITIONAL_THROW (
				_cfgHir._curTile != 0,
				CilInfoMissException, 
				tileTypeMissInfo.str()
				)
#endif
			

				
			// Global name is like this: clusterName_XmYn (for site) or pad name
			// Firstly we should judge the type of the global name
			// We can tell by finding if there is a '_' in the name
			string siteInstGlobalName = inst._placedSite;
			
			string::size_type sit = siteInstGlobalName.find_first_of('_');
			
			int siteInstPos;

			// =======================================================================
			// There are two expressions of site instances.
			// 1. Non-IOB, expressed by SiteName_XmYn
			// 2. IOB, expressed by Ax, Bx...
			// =======================================================================
			// For non-IOB instances
			if ( sit != string::npos ) {

				// We should get the siteInstLocalName according to given the global name.
				// The relationship between global and local name is given by cluster_lib


				string clusterName = siteInstGlobalName.substr( 0, sit );

				if ( !CFG_CLUSTERS.count(clusterName) ) {
#ifdef _TEST
					std::cout << "  <Warning>  There is no configuration for cluster: " << clusterName << std::endl;
#endif
					continue;
				}

				string xyCoordination = siteInstGlobalName.substr( sit + 1 );
				// Find current cluster inst
				_cfgHir._curClusterInst = _cfgHir._curTile->find_cluster_inst_by_name( clusterName );
				
				//assert( _cfgHir._curClusterInst != 0 );
#ifdef _TEST
				stringstream clusterInstMissInfo;
				clusterInstMissInfo << "[!Error!] Cannot find cluster instance: " << clusterName;
				CONDITIONAL_THROW (
					_cfgHir._curClusterInst != 0,
					CilInfoMissException, 
					clusterInstMissInfo.str()
					)
#endif
				

				// Find current cluster type
				_cfgHir._curCluster = _cil.root()->get_cluster_lib()->find_cluster_by_name( clusterName );
				//assert( _cfgHir._curCluster != 0 );


#ifdef _TEST
				stringstream clusterTypeMissInfo;
				clusterTypeMissInfo << "[!Error!] Cannot find cluster type: " << clusterName;
				CONDITIONAL_THROW (
					_cfgHir._curCluster != 0,
					CilInfoMissException, 
					clusterTypeMissInfo.str()
					)
#endif
				Point globalXY = lexical_cast<Point>( xyCoordination );

				string clusterScaleStr = _cfgHir._curCluster->get_scale();

			
				Point clusterScale = lexical_cast<Point>( clusterScaleStr );
				// We can use this equation to get the site inst position
				siteInstPos = 
					globalXY.getX() % clusterScale.getY() * clusterScale.getY() + globalXY.getY() % clusterScale.getX();
			
			// For IOB instances
			} else {

				// Find the iob inst pos from the package
				pad* p = _cfgHir._curPackage->find_pad_by_name( siteInstGlobalName );
#ifdef _TEST
				stringstream padMissInfo;
				padMissInfo << "[!Error!] Cannot find pad: " << siteInstGlobalName;
				CONDITIONAL_THROW (
					p != 0,
					CilInfoMissException, 
					padMissInfo.str()
					)
#endif				
				string posStr =  p->get_pos(); //posStr is like this x,y,z
				
				string::size_type comma = posStr.find_last_of(',');
				string z = posStr.substr(comma + 1);
				siteInstPos = lexical_cast<int>(z);

			}

			// Find current site instance
			_cfgHir._curSiteInst = 
				_cfgHir._curCluster->find_site_inst_by_pos( lexical_cast<string>(siteInstPos) );
			//assert( _cfgHir._curSiteInst != 0 );
#ifdef _TEST
			stringstream siteInstMissInfo;
			siteInstMissInfo << "[!Error!] Cannot find site instance whose pos is " << siteInstPos 
				<< " in cluster " << _cfgHir._curCluster->get_name() ;
			CONDITIONAL_THROW (
				_cfgHir._curSiteInst != 0,
				CilInfoMissException, 
				siteInstMissInfo.str()
				)
#endif	
			// Find current site
			string siteName = _cfgHir._curSiteInst->get_ref();
			_cfgHir._curSite = _cil.root()->get_site_lib()->find_site_by_name(siteName);
			//assert( _cfgHir._curSite != 0 );
#ifdef _TEST
			stringstream siteTypeMissInfo;
			siteTypeMissInfo << "[!Error!] Cannot find site type: " << siteName;
			CONDITIONAL_THROW (
				_cfgHir._curSite != 0,
				CilInfoMissException, 
				siteTypeMissInfo.str()
				)
#endif	
			string siteInstName = _cfgHir._curSiteInst->get_name();

			foreach( const InstCfg & instCfg, inst._cfgs ) {

				if ( instCfg._type == InstCfg::Attribute ) {
					string attrName = instCfg._attribute;
					string optionName = instCfg._option;
					
					if ( "" == optionName ) continue;
	#ifdef _TEST
					std::cout << "<DebugInfo>  Querying for " << attrName << "::" << optionName << "..." << std::endl;
	#endif
					_cfgHir._curAttr = _cfgHir._curSite->find_attribute_by_name(attrName);
					//assert( _cfgHir._curAttr != 0 );
	#ifdef _TEST
					stringstream attrMissInfo;
					attrMissInfo << "[!Error!] Cannot find attribute: " << attrName << " in site: "
						<< _cfgHir._curSite->get_name();
					CONDITIONAL_THROW (
						_cfgHir._curAttr != 0,
						CilInfoMissException, 
						attrMissInfo.str()
						)
	#endif	
					// It is very important here
					// We have 4 main kinds of searching methods
					// 1. map
					// 2. equation
					// 3. bitstr
					// 4. bram
					// So I wrote 4 functions to implement.
					string method = _cfgHir._curAttr->get_method();

					// To avoid upper or lower case problem, I unify the string to lower case firstly
					transform( method.begin(), method.end(), method.begin() , ::tolower );

					if ( "map" == method ) {
						recordMapSrams( sramVec, optionName );
					} else if ( "equation" == method ) {
						recordEqnSrams( sramVec, optionName );
					} else if ( "bitstr" == method ) {
						recordBitSrams( sramVec, optionName );
					} else if ( "bram" == method ) {
						recordBramSrams( sramVec, attrName, optionName );
					} else {
						MY_THROW( UnknownAttrTypeException, "[!Error!] Unknown attribute type!");
					}
				} else {
					string sramName = instCfg._sramName;
					_cfgHir._curDist =
						_cfgHir._curClusterInst->find_dist_by_inst_and_sram( siteInstName, sramName );
					//assert( _cfgHir._curDist );
#ifdef _TEST
					stringstream distMissInfo;
					distMissInfo << "[!Error!] Cannot find distribution info for sram: " 
						<< siteInstName << "(" << _cfgHir._curSiteInst->get_ref() << ")" << "." << sramName 
						<< " in tile: " << _cfgHir._curTile->get_name()
						<< " in cluster: " << _cfgHir._curCluster->get_name();
					CONDITIONAL_THROW (
						_cfgHir._curDist != 0,
						CilInfoMissException, 
						distMissInfo.str()
						)
#endif


					Point tilePos = lexical_cast<Point>( _cfgHir._curTileInst->get_pos() );
					Point offset = lexical_cast<Point>(_cfgHir._curDist->get_offset());
					int bl = lexical_cast<int>( _cfgHir._curDist->get_bl() );
					int wl = lexical_cast<int>( _cfgHir._curDist->get_wl() );

					Point localPos( bl, wl );
					int bitValue = instCfg._sramVal;


					SramBit newSram;
					newSram.tilePos = tilePos;
					newSram.offset = offset;
					newSram.localPos = localPos;
					newSram.bitValue = 
						( _cfgHir._curDist->get_inv() == "yes" ) ? abs(bitValue - 1) : bitValue;
					sramVec.push_back(newSram);
				}
			}
		}
	}

	
	void InstQuerier::recordMapSrams( SramVec & sramVec, string option )
	{
		_cfgHir._curOption = _cfgHir._curAttr->find_option_by_name( option );
#ifdef _TEST
		stringstream optionMissInfo;
		optionMissInfo << "Cannot find option: " << option
			<< " in attribute: " << _cfgHir._curAttr->get_name()
			<< " in site: " << _cfgHir._curSite->get_name();
		CONDITIONAL_THROW (
			_cfgHir._curOption != 0,
			CilInfoMissException, 
			optionMissInfo.str()
			)
#endif	
		Point tilePos = lexical_cast<Point>( _cfgHir._curTileInst->get_pos() );
		
		sram* s = _cfgHir._curOption->first_sram();
		while ( s ) {
			// Get all the srams with no attribute address. 
			if ( ! s->first_attribute("address") ) {
				string sramName = s->get_name();
				string siteInstName = _cfgHir._curSiteInst->get_name();

				_cfgHir._curDist =
					_cfgHir._curClusterInst->find_dist_by_inst_and_sram( siteInstName, sramName );
				//assert( _cfgHir._curDist );
#ifdef _TEST
				stringstream distMissInfo;
				distMissInfo << "[!Error!] Cannot find distribution info for sram: " 
					<< siteInstName << "(" << _cfgHir._curSiteInst->get_ref() << ")" << "." << sramName 
					<< " in tile: " << _cfgHir._curTile->get_name()
					<< " in cluster: " << _cfgHir._curCluster->get_name();
				CONDITIONAL_THROW (
					_cfgHir._curDist != 0,
					CilInfoMissException, 
					distMissInfo.str()
					)
#endif
				// Find the offset from the tile
				Point offset = lexical_cast<Point>(_cfgHir._curDist->get_offset());
				int bl = lexical_cast<int>( _cfgHir._curDist->get_bl() );
				int wl = lexical_cast<int>( _cfgHir._curDist->get_wl() );

				Point localPos( bl, wl );
				int bitValue = lexical_cast<int>( s->get_value() );


				SramBit newSram;
				newSram.tilePos = tilePos;
				newSram.offset = offset;
				newSram.localPos = localPos;
				newSram.bitValue = 
					( _cfgHir._curDist->get_inv() == "yes" ) ? abs(bitValue - 1) : bitValue;
#ifdef _TEST
				std::cout << newSram << std::endl;
#endif
				sramVec.push_back(newSram);
			}
			s = s->next_sram();
		}

	}

	void InstQuerier::recordEqnSrams( SramVec & sramVec, string equation )
	{
		// equation is like this #LUT:D=A1+A2+A3+A4
		string mode  = equation.substr(0, 4);
		string right = equation.substr(7);
		Point tilePos = lexical_cast<Point>( _cfgHir._curTileInst->get_pos() );
		// Firstly we should config the sram to determine the work mode
		recordMapSrams(sramVec, mode);

		// Then we config the sram related to the equation
		vector<int> truthTable = Exp2LUT( right, 16 ).toVec();

		for ( unsigned i = 0; i < truthTable.size(); ++i ) {

			int bitValue = truthTable[i];

			string addr = lexical_cast<string>(i);
			
			sram* s = _cfgHir._curOption->find_sram_by_address(addr);
			assert(s!=0);
			string siteInstName = _cfgHir._curSiteInst->get_name();
			string sramName = s->get_name();
			_cfgHir._curDist = 
				_cfgHir._curClusterInst->find_dist_by_inst_and_sram(siteInstName, sramName);
			//assert(_cfgHir._curDist);
#ifdef _TEST
			stringstream distMissInfo;
			distMissInfo << "[!Error!] Cannot find distribution info for sram: " 
				<< siteInstName << "(" << _cfgHir._curSiteInst->get_ref() << ")" << "." << sramName 
				<< " in tile: " << _cfgHir._curTile->get_name()
				<< " in cluster: " << _cfgHir._curCluster->get_name();
			CONDITIONAL_THROW (
				_cfgHir._curDist != 0,
				CilInfoMissException, 
				distMissInfo.str()
				)
#endif	
			int bl = lexical_cast<int>( _cfgHir._curDist->get_bl() );
			int wl = lexical_cast<int>( _cfgHir._curDist->get_wl() );
			Point offset =  lexical_cast<Point>( _cfgHir._curDist->get_offset() );
			// push the new sram found into the sramVec
			SramBit newSram;
			newSram.tilePos = tilePos;
			newSram.offset = offset;
			newSram.localPos = Point(bl, wl);
			newSram.bitValue = 
				( _cfgHir._curDist->get_inv() == "yes" ) ? abs(bitValue - 1) : bitValue; 
#ifdef _TEST
			std::cout << newSram << std::endl;
#endif
			sramVec.push_back(newSram);
		}
	}

	void InstQuerier::recordBitSrams( SramVec & sramVec, string bitstr )
	{
		Point tilePos = lexical_cast<Point>( _cfgHir._curTileInst->get_pos() );

		string base = "hex";
		_cfgHir._curOption = _cfgHir._curAttr->find_option_by_name("hex");
		
		if ( ! _cfgHir._curOption ) {
			_cfgHir._curOption = _cfgHir._curAttr->find_option_by_name("bin");
			base = "bin";
		}

		assert( _cfgHir._curOption != NULL );
		


		// declare a vector to store the bits
		vector<int> bits;
		if ( "hex" == base ) {
			recordMapSrams( sramVec, "hex" );
			Hex2BitVec( bits, bitstr );
		} else {
			recordMapSrams( sramVec, "bin" );
			Bin2BitVec( bits, bitstr );
		}

		for ( unsigned i = 0; i < bits.size(); ++i ) {

			int bitValue = bits[i];

			string addr = lexical_cast<string>(i);
			sram* s = _cfgHir._curOption->find_sram_by_address(addr);
			if (s == NULL) 
			{
				std::cout << "Warning: No SRAM configuration." << std::endl;
				break;
			}
			string siteInstName = _cfgHir._curSiteInst->get_name();
			string sramName = s->get_name();
			_cfgHir._curDist = 
				_cfgHir._curClusterInst->find_dist_by_inst_and_sram(siteInstName, sramName);
			//assert(_cfgHir._curDist);
#ifdef _TEST
			stringstream distMissInfo;
			distMissInfo << "[!Error!] Cannot find distribution info for sram: " 
				<< siteInstName << "(" << _cfgHir._curSiteInst->get_ref() << ")" << "." << sramName 
				<< " in tile: " << _cfgHir._curTile->get_name()
				<< " in cluster: " << _cfgHir._curCluster->get_name();
			CONDITIONAL_THROW (
				_cfgHir._curDist != 0,
				CilInfoMissException, 
				distMissInfo.str()
				)
#endif
			int bl = lexical_cast<int>( _cfgHir._curDist->get_bl() );
			int wl = lexical_cast<int>( _cfgHir._curDist->get_wl() );
			Point offset =  lexical_cast<Point>( _cfgHir._curDist->get_offset() );
			// push the new sram found into the sramVec
			SramBit newSram;
			newSram.tilePos = tilePos;
			newSram.offset = offset;
			newSram.localPos = Point(bl, wl);
			newSram.bitValue = 
				( _cfgHir._curDist->get_inv() == "yes" ) ? abs(bitValue - 1) : bitValue; 
#ifdef _TEST
			std::cout << newSram << std::endl;
#endif
			sramVec.push_back(newSram);
		}

	}

	void InstQuerier::recordBramSrams( SramVec & sramVec, string bramCellName, string bramInitBitStr )
	{
		Point tilePos = lexical_cast<Point>( _cfgHir._curTileInst->get_pos() );

		string::size_type _pos = bramCellName.find_first_of('_');
		string bramCellType = bramCellName.substr(0, _pos);
		string bramCellAddrHex = bramCellName.substr( _pos + 1 );
		toLower(bramCellAddrHex);
		
		// Decimal bram cell address
		int bramCellAddrDec = 0;
		vector<int> addrBin;
		Hex2BitVec(addrBin, bramCellAddrHex);
		// Transform the hex part of the BRAM attribute to decimal integer
		bramCellAddrDec = BitVec2U(addrBin);

		vector<int> bramBin;
		toLower(bramInitBitStr);
		Hex2BitVec( bramBin, bramInitBitStr );
		// std::reverse( bramBin.begin(), bramBin.end() );
		assert( bramBin.size() == BRAM_LENGTH_PER_BLOCK );

		// Find the current ram cell
		_cfgHir._curBram = _cil.root()->get_bram_lib()->find_bram_by_type(bramCellType);
		
		for( int i = 0 ; i < bramBin.size() ; ++i ) {
			int cellAddr;
			int wlOffset = 0;
			if ( "INIT" == bramCellType ) {
				cellAddr = i;
				wlOffset = bramCellAddrDec;
			}
			else if ( "INITP" == bramCellType ) {
				cellAddr = i % BRAM_PARITY_LENGTH;
				// Change bram address of INITP
				wlOffset = (bramCellAddrDec*BRAM_LENGTH_PER_BLOCK + i)/BRAM_PARITY_LENGTH;
				//std::cout << bramCellAddrDec << std::endl;
			} else {
				// Throw unexpected bram type error
			}

			string cellAddrStr = lexical_cast<string>(cellAddr);
			// Find the current ram cell
			_cfgHir._curCell = _cfgHir._curBram->find_cell_by_address(cellAddrStr);
			
			int bitValue = bramBin[ bramBin.size()-1-i ];

			int wl = lexical_cast<int>( _cfgHir._curBram->get_wl() ) + wlOffset;
			//std::cout << wl << std::endl;
			int bl = lexical_cast<int>( _cfgHir._curCell->get_bl() );

				
			SramBit newSram;
			newSram.bitValue = 
					"yes" ==_cfgHir._curCell->get_inv() ? abs(bitValue - 1) : bitValue;
			newSram.tilePos = tilePos;
			newSram.localPos = Point( bl, wl );
			newSram.offset = 
				lexical_cast<Point>( _cfgHir._curBram->get_offset() );
#ifdef _TEST
			std::cout << newSram << std::endl;
#endif
			sramVec.push_back(newSram);
		}

	}
}