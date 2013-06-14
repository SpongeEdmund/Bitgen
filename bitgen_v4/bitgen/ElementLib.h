//! Element library stores the configurable switches such as muxes and buffers
/*!
	The xml format of element library is like this:

	<element_lib>
	  <element name="MUX2TO1">
	    <path from="IN0" to="OUT">
		  <sram name="P0_" value="1"/>
		  <sram name="P1_" value="0"/>
		</path>
		<path from="IN1" to="OUT">
		  <sram name="P0_" value="0"/>
		  <sram name="P1_" value="1"/>
		</path>
	  </element>
	  <element name=...
	  ...
	  </element>
      ...
	</element_lib>
*/

#ifndef __ELEMENT_LIB_H__
#define __ELEMENT_LIB_H__

#include "XmlModUtils.h"

DEFINE_NODE(sram)
{
	
	HAS_ATTRIBUTE(name)
	HAS_ATTRIBUTE(value)
	// this is a hidden attribute here
	HAS_HIDDEN_ATTRIBUTE(address, "")
	HAS_SIBLING(sram)
};

DEFINE_NODE(path)
{
	HAS_CHILDREN(sram)
	HAS_ATTRIBUTE(from)
	HAS_ATTRIBUTE(to)	
};

DEFINE_NODE(element)
{
	HAS_ATTRIBUTE(name)
	HAS_CHILDREN_FOUND_BY2(path, from, to)
};

DEFINE_NODE(element_lib)
{
	HAS_CHILDREN_FOUND_BY1(element, name)
};


#endif  