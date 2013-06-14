//! Site library stores the configuration information of configurable sites
/*!
	The XML format of site library is like this

  <site_lib>
    <site name="SLICEL" type="SLICE">
      <attribute name="BXINV" method="direct">
        <option name="#OFF" default="true">
          <sram name="s1" value="0"/>
          <sram name="s2" value="0"/>
        </option>
        <option name="BX" default="false">
          <sram name="s1" value="1"/>
          <sram name="s2" value="0"/>
        </option>
        <option name="BX_B" default="false">
          <sram name="s1" value="0"/>
          <sram name="s2" value="1"/>
        </option>
      </attribute>
      <attribute name="F" method="lut">
        <option name="#LUT" default="no">
          <sram name="SFT_RAMF" value="1"/>
          <sram name="LUTF0" address="0"/>
          <sram name="LUTF1" address="1"/>
          <sram name="LUTF2" address="2"/>
          <sram name="LUTF3" address="3"/>
          <sram name="LUTF4" address="4"/>
          <sram name="LUTF5" address="5"/>
          <sram name="LUTF6" address="6"/>
          <sram name="LUTF7" address="7"/>
          <sram name="LUTF8" address="8"/>
          <sram name="LUTF9" address="9"/>
          <sram name="LUTF10" address="10"/>
          <sram name="LUTF11" address="11"/>
          <sram name="LUTF12" address="12"/>
          <sram name="LUTF13" address="13"/>
          <sram name="LUTF14" address="14"/>
          <sram name="LUTF15" address="15"/>
        </option>
	  </site>
	  <site...
	  </site>
	  ...
	</site_lib>
*/

#ifndef __SITE_LIB_H__
#define __SITE_LIB_H__

#include "XmlModUtils.h"
#include "ElementLib.h" //!< To use the sram node



DEFINE_NODE(option)
{
	HAS_CHILDREN( sram )
	HAS_ATTRIBUTE(name)
	HAS_ATTRIBUTE(default)
	HAS_CHILDREN_FOUND_BY1(sram, address)
};

DEFINE_NODE(attribute)
{
	HAS_ATTRIBUTE(name)
	HAS_ATTRIBUTE(method)
	HAS_CHILDREN_FOUND_BY1(option, name)

};

DEFINE_NODE(site)
{
	HAS_ATTRIBUTE(name)
	HAS_ATTRIBUTE(type)
	HAS_CHILDREN_FOUND_BY1(attribute, name)
};

DEFINE_NODE(site_lib)
{
	HAS_CHILDREN_FOUND_BY1( site, name )
};
#endif