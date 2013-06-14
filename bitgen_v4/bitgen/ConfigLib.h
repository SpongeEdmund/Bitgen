//! cil is the root of cil
/*!
    <cil family="fdp5">
	  <element_lib>
	    ...
	  </element_lib>
	  <grm_lib>
	    ...
	  </grm_lib>
	  <site_lib>
	    ...
	  </site_lib>
	  <tile_lib>
	    ...
	  </tile_lib>
	  <bram_lib>
	    ...
	  </bram_lib>
	  <chip_lib>
	    ...
	  </chip_lib>
	</config_info>
*/

#ifndef __CFG_LIB_H__
#define __CFG_LIB_H__

#include "XmlModUtils.h"

#include "ElementLib.h"
#include "GrmLib.h"
#include "SiteLib.h"
#include "BramLib.h"
#include "TileLib.h"
#include "ChipLib.h"
#include "ClusterLib.h"

DEFINE_NODE(cil)
{
	HAS_ATTRIBUTE(family)
	HAS_ATTRIBUTE(version)
	HAS_ATTRIBUTE(author)
	HAS_ONE_CHILD(element_lib)
	HAS_ONE_CHILD(grm_lib)
	HAS_ONE_CHILD(site_lib)
	HAS_ONE_CHILD(cluster_lib)
	HAS_ONE_CHILD(tile_lib)
	HAS_ONE_CHILD(bram_lib)
	HAS_ONE_CHILD(chip_lib)
};


#endif