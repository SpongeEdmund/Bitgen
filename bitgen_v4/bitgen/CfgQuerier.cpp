#include "CfgQuerier.h"

namespace bitgen {



	void CfgQuerier::specifyArch()
	{
		_cfgHir._curChip = 
			_cil.root()->get_chip_lib()->find_chip_by_name(ARGS.DEVICE);
		assert( _cfgHir._curChip != 0 );

		_cfgHir._curArch = _cfgHir._curChip->get_arch();
		assert( _cfgHir._curArch != 0 );
		
		_cfgHir._curPackage = _cfgHir._curChip->find_package_by_name(ARGS.PACKAGE);
		assert( _cfgHir._curPackage != 0 );
	}

}
