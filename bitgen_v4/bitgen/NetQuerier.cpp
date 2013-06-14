#include "NetQuerier.h"
#include <boost/lexical_cast.hpp>

namespace bitgen {

	using boost::lexical_cast;

	void NetQuerier::runQuery( SramVec & sramVec )
	{
		specifyArch();

		foreach ( const NetlistNet & net, _netlist.nets ) {
			foreach ( const NetPip & pip, net.pips ) {
#ifdef _DEBUG
				std::cout << "Querying for " << pip.tileName << " "
					<< pip.srcNet << " -> " << pip.snkNet << std::endl;
#endif
				// Find the tile inst according to the tile inst name of the pip
				_cfgHir._curTileInst = _cfgHir._curArch->find_tile_inst_by_name( pip.tileName );
				assert( _cfgHir._curTileInst != NULL );
				
				// Find the tile type of the tile inst
				string tileTypeName = _cfgHir._curTileInst->get_ref();

				
				_cfgHir._curTile = _cil.root()->get_tile_lib()->find_tile_by_name( tileTypeName );
				//assert( _cfgHir._curTile != NULL );
				if ( _cfgHir._curTile == NULL ) {
#ifdef _DEBUG
					std::cout << "<Warning>: Tile: " << tileTypeName << " doesn't exist in cil."
						<< std::endl;
#endif
					continue;
				}

				// According to the naming rule, interconnection of a tile is called as GRM_Type
				string grmInstName = string("GRM_").append(tileTypeName);
				_cfgHir._curGrmInst = _cfgHir._curTile->get_grm_inst();
				
				// Before getting the GRM of the tile, firstly we should judge if the tile has
				// interconnection need to be configured. We just ignore the buffer-like pips.
				if ( _cfgHir._curGrmInst == NULL ) {
#ifdef _DEBUG
					std::cout << "<Warning>: Tile: " << _cfgHir._curTile->get_name() << " has no configurable interconnection."
						<< std::endl;
#endif
					continue;
				}

				string grmName = _cfgHir._curGrmInst->get_ref();
				_cfgHir._curGrm = _cil.root()->get_grm_lib()->find_grm_by_name( grmName );
				assert( _cfgHir._curGrm );


				_cfgHir._curRoute = _cfgHir._curGrm->find_route_by_src_and_snk( pip.srcNet, pip.snkNet );
				//assert( _cfgHir._curRoute );
				if ( NULL == _cfgHir._curRoute ) {
#ifdef _DEBUG
					std::cout << "<Warning>: PIP: " << pip.srcNet << " -> " << pip.snkNet
						<< " has no sram control." << std::endl;
#endif
					continue;
				}
				_cfgHir._curStep = _cfgHir._curRoute->first_step();
				while( _cfgHir._curStep ) {
					string from = _cfgHir._curStep->get_from();
					string to   = _cfgHir._curStep->get_to();
					string elemInstName = _cfgHir._curStep->get_through();

					_cfgHir._curElementInst = 
						_cfgHir._curGrm->find_element_inst_by_name( elemInstName );
					assert( _cfgHir._curElementInst );

					_cfgHir._curElement = 
						_cil.root()->get_element_lib()->find_element_by_name(_cfgHir._curElementInst->get_ref());
					assert( _cfgHir._curElement );

					_cfgHir._curPath = _cfgHir._curElement->find_path_by_from_and_to( from, to );
					
					// Find all the srams controlling the path and get the info of them
					_cfgHir._curSram = _cfgHir._curPath->first_sram();
					while ( _cfgHir._curSram ) {
						string sramName = _cfgHir._curSram->get_name();

						// Find the distribution of the sram
						_cfgHir._curDist = 
							_cfgHir._curGrmInst->find_dist_by_inst_and_sram( elemInstName, sramName );
						assert( _cfgHir._curDist );

						

						SramBit newSram;
						newSram.bitValue = lexical_cast<int>( _cfgHir._curSram->get_value() );
						newSram.tilePos  = lexical_cast<Point>( _cfgHir._curTileInst->get_pos() );
						newSram.offset   = lexical_cast<Point>( _cfgHir._curDist->get_offset() );
						
						int bl = lexical_cast<int>( _cfgHir._curDist->get_bl() );
						int wl = lexical_cast<int>( _cfgHir._curDist->get_wl() );
						newSram.localPos = Point( bl, wl );

						sramVec.push_back(newSram);
						
						_cfgHir._curSram = _cfgHir._curSram->next_sram();
					}

					_cfgHir._curStep = _cfgHir._curStep->next_step();
					
				}
			}
		}
	}
}