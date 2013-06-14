#ifndef __INST_QUERIER_H__
#define __INST_QUERIER_H__

#include "CfgQuerier.h"


namespace bitgen
{

	//! Querier of inst of config spec
	class InstQuerier : public CfgQuerier
	{
	public:
		InstQuerier( const Cil & cil, Netlist & netlist )
			: CfgQuerier( cil, netlist )
		{
		}



		virtual void runQuery( SramVec & sramVec );
		void recordSrams( SramVec & sramVec, std::string method );
		

		void recordMapSrams( SramVec & sramVec, std::string option );
		void recordEqnSrams( SramVec & sramVec, std::string equation );
		void recordBitSrams( SramVec & sramVec, std::string bitStr );
		void recordBramSrams( SramVec & sramVec, std::string bramName, std::string bramStr );
	};
}

#endif