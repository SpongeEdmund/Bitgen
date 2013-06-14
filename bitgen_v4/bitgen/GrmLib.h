//! GRM library stores the routing matrix's routing information
/*! 
	The XML format of GRM library is like this
	<grm_lib>
	  <grm name="GRM_INT">
	    <element_inst name="XIIMUX_B1" ref="INT_MUX_36TO1"/>
		<element_inst.../>
        ...
	    <route src="BOUNCE1" snk="IMUX_B1"/>
		  <step from="IN11" to="OUT" inst="XIIMUX_B1"/>
		  <step .../>
          ...
		</route>
		<route ...
		</route>
        ...
	  </grm>
	  <grm...
	  </grm>
      ...
	</grm_lib>

*/

#ifndef __GRM_LIB_H__
#define __GRM_LIB_H__

#include "XmlModUtils.h"

DEFINE_NODE(element_inst)
{
	HAS_ATTRIBUTE(name)
	HAS_ATTRIBUTE(ref)
};

DEFINE_NODE(step)
{
	HAS_SIBLING(step)
	HAS_ATTRIBUTE(through)
	HAS_ATTRIBUTE(from)
	HAS_ATTRIBUTE(to)
};

DEFINE_NODE(route)
{
	HAS_CHILDREN(step)
	HAS_ATTRIBUTE(src)
	HAS_ATTRIBUTE(snk)
	HAS_ATTRIBUTE(inst)
};


DEFINE_NODE(grm)
{
	HAS_ATTRIBUTE( name ) 
	HAS_CHILDREN_FOUND_BY1(element_inst, name)
	HAS_CHILDREN_FOUND_BY2(route, src, snk)
};

DEFINE_NODE(grm_lib)
{
	HAS_CHILDREN_FOUND_BY1(grm, name)
};

#endif