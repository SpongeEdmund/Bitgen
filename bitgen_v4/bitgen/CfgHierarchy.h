// File Name: CfgHierarchy.h
// Description:
//   This file defines the configuration hierarchy of a SRAM. Generally,
//   There are 3 types of SRAM classified by the slave circuit they controll.
//   1. Instance SRAM 2. BRAM SRAM 3. Net SRAM
//   
#include "Cil.h"
#include <string>
namespace bitgen {
	struct CfgHierarchy
	{
		chip*         _curChip;    
		arch*         _curArch;    
		package*      _curPackage; 
		tile_inst*    _curTileInst;
		tile*         _curTile;

		// for instance
		cluster_inst* _curClusterInst;
		cluster*      _curCluster;
		site_inst*    _curSiteInst;
		site*         _curSite;
		dist*         _curDist;
		attribute*    _curAttr;
		option*       _curOption;
		// for bram
		bram*         _curBram;
		cell*         _curCell;
		// for net
		grm_inst*     _curGrmInst;
		grm*          _curGrm;
		route*        _curRoute;
		step*         _curStep;
		element_inst* _curElementInst;
		element*      _curElement;
		path*         _curPath;
		sram*         _curSram;

		CfgHierarchy();

		void clear();
		std::string getInstHir() const;
		std::string getBramHir() const;
		std::string getNetHir() const;

	};

}