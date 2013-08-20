#include "Cil.h"
#include "zfstream.h"

Cil::Cil( const std::string & cilFile, bool isEncrypted )
{
	if( isEncrypted )
	{
		zifstream zifs( cilFile.c_str() );
		_file = new file<>( zifs );
	} else {
		std::ifstream ifs( cilFile.c_str() );
		_file = new file<>( ifs );
	}
	_doc = new xml_document<>( );
	_doc->parse<0>( _file->data() );

}

Cil::~Cil()
{
	delete _file;
	delete _doc;
}