/*
   <cluster_lib>
     <cluster name="SLICE" scale="2X2">
       <site_inst name="XI0" ref="SLICEM" pos="0"/>
	   <site_inst name="XI1" ref="SLICEM" pos="1"/>
	   <site_inst name="XI2" ref="SLICEL" pos="2"/>
	   <site_inst name="XI3" ref="SLICEL" pos="3"/>
     </cluster>
   </cluster>
*/

#ifndef __CLUSTER_LIB_H__
#define __CLUSTER_LIB_H__



#include "XmlModUtils.h"

DEFINE_NODE( site_inst )
{
public:
	HAS_ATTRIBUTE( name )
	HAS_ATTRIBUTE( ref )
	HAS_ATTRIBUTE( pos )
};

DEFINE_NODE( cluster )
{
public:
	HAS_ATTRIBUTE( name )
	HAS_ATTRIBUTE( scale )
	HAS_CHILDREN_FOUND_BY1( site_inst, pos )
};

DEFINE_NODE( cluster_lib )
{
public:
	HAS_CHILDREN_FOUND_BY1( cluster, name )
	

};
#endif