#include "CfgQuerier.h"

namespace bitgen {



	void CfgQuerier::specifyArch()
	{
		_cfgHir._curChip = 
			_cil.root()->get_chip_lib()->find_chip_by_name(ARGS.DEVICE);
		assert( _cfgHir._curChip != 0 );

		_cfgHir._curArch = _cfgHir._curChip->get_arch();
		assert( _cfgHir._curArch != 0 );
		tile_inst* t = _cfgHir._curArch->first_tile_inst(); 
		for ( ; t ; t = t->next_tile_inst() ) {
			//_posTileMap.insert( make_pair<string, tile_inst*>(pos, t));
			_nameTileMap.insert( um::value_type(t->get_name(), t) );
		}
		_cfgHir._curPackage = _cfgHir._curChip->find_package_by_name(ARGS.PACKAGE);
		assert( _cfgHir._curPackage != 0 );
	}

}
