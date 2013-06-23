//! The bram library gives the correspondance of the bram address and the bl 
/*! 
  <bram_lib>
    <bram type="INIT" wl="20" offset="0,0">
	  <cell address="0" bl="4" inv="no"/>
	  <cell address="1" bl="5" inv="no"/>
	  <cell address="2" bl="14" inv="no"/>
	  ...
	</bram>
	<bram type="INITP" wl="20" offset="0,0">
	  <cell address="0" bl="23" inv="no"/>
	  <cell address="1" bl="24" inv="no"/>
	  ...
	</bram>
  </bram_lib>
*/
#ifndef __BRAM_LIB_H__
#define __BRAM_LIB_H__

#include "XmlModUtils.h"

DEFINE_NODE(cell)
{
	HAS_ATTRIBUTE(address)
	HAS_ATTRIBUTE(bl)
	HAS_ATTRIBUTE(inv)
};

DEFINE_NODE(bram)
{
	HAS_ATTRIBUTE(type)
	HAS_ATTRIBUTE(wl)
	HAS_ATTRIBUTE(offset)
	HAS_CHILDREN_FOUND_BY1(cell, address)
};

DEFINE_NODE(bram_lib)
{
	HAS_CHILDREN_FOUND_BY1(bram, type)
};
#endif