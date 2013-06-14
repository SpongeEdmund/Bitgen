#ifndef __NET_QUERIER_H__
#define __NET_QUERIER_H__

#include "CfgQuerier.h"

namespace bitgen {

	class NetQuerier : public CfgQuerier
	{
	public:
		NetQuerier( const Cil & cil, Netlist & netlist )
			: CfgQuerier( cil, netlist )
		{

		}

		void runQuery( SramVec & sramVec );
	};
}


#endif