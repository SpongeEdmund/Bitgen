#ifndef __CFG_QUERIER_H__
#define __CFG_QUERIER_H__

#include <iostream>
#include <vector>
#include <string>
#include <boost/foreach.hpp>
#include <boost/unordered_map.hpp>
#include <cassert>
#include "Args.h"
#include "Netlist.h"
#include "Cil.h"
#include "SramBit.h"
#include "CfgHierarchy.h"
#define foreach BOOST_FOREACH

namespace bitgen {

	extern Args ARGS;



	typedef std::vector< SramBit > SramVec;

	typedef boost::unordered_map<string, tile_inst*> um;
	
	//! CfgQuerier, base class of InstQuerier and NetQuerier 
	/*! 
		CfgQuerier queries cil for sram information according to the config description language
		such as xdl.
	*/
	class CfgQuerier
	{
	public:
		CfgQuerier( const Cil& cil, const Netlist & netlist ) : _cil(cil), _netlist(netlist) { }
		
		//! Determine the hierarchy above tile
		/*!
			Since the query processes of the hierarchy above tile for both inst and net are the
			same, we use a member function here to merge the processes to one. Here we can determine
			chip, arch, package, tile_inst and tile hierarchy.
		*/
		void specifyArch();

		//! Queries cil for sram infos and fills SramVec with these srams.
		virtual void runQuery( SramVec & ) = 0;

		
	protected:
		//! Hierarchy 
		CfgHierarchy _cfgHir;
		//! Cil
		/*!
			A querier has to have a cil to query.
		*/
		const Cil & _cil;
		//! netlist
		/*!
			A querier has to be bonded to a netlist. We use reference here to stress that
			a querier belongs to one netlist and the netlist cannot be changed since we
			do not complicate the query process.
		*/
		const Netlist & _netlist;

		//std::vector<tile_inst*> _posTileMap;
		um _nameTileMap;
		
	};
}

#endif