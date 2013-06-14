#include "Cil.h"


Cil::Cil( const std::string & cilFile )
{
	_file = new file<>( cilFile.c_str() );
	_doc = new xml_document<>( );
	_doc->parse<0>( _file->data() );

}

Cil::~Cil()
{
	delete _file;
	delete _doc;
}