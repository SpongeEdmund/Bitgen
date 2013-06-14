//! Tile library stores the distribution information of srams
/*!
    The format of tile library is :

	<tile_lib>
	  <tile name="INT" scale="80,20">
	    <grm_inst name="GRM_INT" ref="GRM_INT">
		  <dist sram="P6_" inst="XIIMUX_B25" bl="54" wl="14" inv="yes"/>
		  ...
		</grm_inst>
	  </tile>
	  <tile name="CLB" scale="80,2">
	    <cluster_inst name="SLICE" ref="SLICE">
		  <dist sram="AU_SF0" inst="SLICE0" bl="3" wl="18" inv="yes" offset="-1,0"/>
		  ...
		</cluster_inst>
	  </tile>
*/


#ifndef __TILE_LIB_H__
#define __TILE_LIB_H__

#include "XmlModUtils.h"


DEFINE_NODE(dist)
{
public:
	HAS_ATTRIBUTE(sram)
	HAS_ATTRIBUTE(inst)
	HAS_ATTRIBUTE(bl)
	HAS_ATTRIBUTE(wl)
	HAS_ATTRIBUTE(inv)
	HAS_ATTRIBUTE(offset)
};

DEFINE_NODE(grm_inst)
{
public:
	HAS_ATTRIBUTE(name)
	HAS_ATTRIBUTE(ref)
	HAS_CHILDREN_FOUND_BY2(dist, inst, sram)
};


DEFINE_NODE(cluster_inst)
{
public:
	HAS_ATTRIBUTE(name)
	HAS_ATTRIBUTE(ref)
	HAS_CHILDREN_FOUND_BY2(dist, inst, sram)
};

DEFINE_NODE(tile)
{
public:
	HAS_ATTRIBUTE(name)
	HAS_ATTRIBUTE(scale)
	HAS_ONE_CHILD(grm_inst)
	//HAS_ONE_CHILD(cluster_inst)
	HAS_CHILDREN_FOUND_BY1(cluster_inst, name)
	
};

DEFINE_NODE(tile_lib)
{
public:
	//HAS_ONE_CHILD(cluster_inst)

	HAS_CHILDREN_FOUND_BY1(tile, name)

};

#endif