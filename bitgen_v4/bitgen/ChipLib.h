//! Chip library stores FPGA chip information of the same serial.
/*! A chip has 3 aspects of information
    - arch, tile array architecture
	- plan, Segment and Packet planning
	- package, chip IO pad package
	
	Chip library's xml format is like this:

	<chip_lib>
	  <chip name="xc4vlx15">
		<arch scale="73,66">
		  <tile_inst name="EMPTY_CORNER_X0Y72" ref="EMPTY_CORNER" pos="0,0"/>
		  <tile_inst name="T_TERM_INT_X0Y64" ref="T_TERM_INT" pos="0,1"/>
          ...
		</arch>
		<plan>
		  <segment address="0" row="1" packet_amount="35"/>
		  <segment address="1" row="19" packet_amount="35"/>
		  <segment address="2" row="37" packet_amount="35"/>
		  <segment address="3" row="55" packet_amount="35"/>
		  <packet address="0" col="31" frame_amount="3"/>
		  <packet address="1" col="3" frame_amount="22"/>
		  ...
		  <packet address="34" tile_col="53" frame_amount="64"/>
		</plan>
		<package name="ff668">
		  <pad name="C17" pos="1,2,0"/>
		  <pad name="D17" pos="1,2,1"/>
		  ...
		</package>
	  </chip>
	</chip_lib>
	*/

#ifndef __CHIP_LIB_H__
#define __CHIP_LIB_H__

#include "XmlModUtils.h"

/************************************************************************/
/*                            arch                                      */
/************************************************************************/

DEFINE_NODE(tile_inst)
{
public:
	HAS_ATTRIBUTE(name)
	HAS_ATTRIBUTE(ref)
	HAS_ATTRIBUTE(pos)
};

DEFINE_NODE(arch)
{
public:
	HAS_ATTRIBUTE(scale)
	HAS_CHILDREN_FOUND_BY1(tile_inst, name)
	HAS_CHILDREN_FOUND_BY1(tile_inst, pos)
};


/************************************************************************/
/*                            plan                                      */
/************************************************************************/
DEFINE_NODE(segment)
{
public:
	HAS_SIBLING(segment)
	HAS_ATTRIBUTE(address)
	HAS_ATTRIBUTE(row)
	HAS_ATTRIBUTE(packet_amount)
};

DEFINE_NODE(packet)
{
public:
	HAS_SIBLING(packet)
	HAS_ATTRIBUTE(address)
	HAS_ATTRIBUTE(col)
	HAS_ATTRIBUTE(frame_amount)

};

DEFINE_NODE(plan)
{
public:
	HAS_ATTRIBUTE(segment_amount)
	HAS_ATTRIBUTE(frames_per_segment)
	HAS_ATTRIBUTE(words_per_frame)
	HAS_ATTRIBUTE(rows_per_segment)
	HAS_CHILDREN(segment)
	HAS_CHILDREN(packet)
	HAS_CHILDREN_FOUND_BY1(segment, row)
	HAS_CHILDREN_FOUND_BY1(packet, col)
	HAS_CHILDREN_FOUND_BY1(packet, address)

};

/************************************************************************/
/*                         package                                      */
/************************************************************************/
DEFINE_NODE(pad)
{
public:
	HAS_ATTRIBUTE(name)
	HAS_ATTRIBUTE(pos)

};

DEFINE_NODE(package)
{
public:
	HAS_ATTRIBUTE(name)
	HAS_CHILDREN_FOUND_BY1(pad, name)

};

/************************************************************************/
/*                          chip                                        */
/************************************************************************/
DEFINE_NODE(chip)
{
public:
	HAS_ATTRIBUTE(name)
	HAS_ATTRIBUTE(slice_amount)
	HAS_ONE_CHILD(arch)
	HAS_ONE_CHILD(plan)
	HAS_CHILDREN_FOUND_BY1(package, name)

};

DEFINE_NODE(chip_lib)
{
public:
	HAS_CHILDREN_FOUND_BY1(chip, name)
};


#endif