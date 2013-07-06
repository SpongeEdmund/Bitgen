#ifndef __CIL_H__
#define __CIL_H__

#include <string>
#include "ConfigLib.h"

#define TRY_FIND_OBJ_IN_CIL( obj, get ) \
	if ( !(obj = get) ) \
		throw std::exception("Cannot find "#obj" through "#get );

class Cil
{

public:
	explicit Cil( const std::string & cilFile );;
	~Cil();

	cil* root() const { return static_cast<cil*>( _doc->first_node() ); }

private:
	xml_document<>* _doc;
	file<>*     _file;

};


#endif