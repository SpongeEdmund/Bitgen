#include "utils.h"

using std::string;
using std::vector;


void Hex2BitVec( std::vector<int>& bin, const std::string& hexExp )
{
	string hex = hexExp;
	toUpper(hex);
	string::size_type px = hex.find_first_of('X');
	if ( px != string::npos )
		hex = hex.substr(px + 1); // delete the 0x prefix
	int binLength = hex.size() * 4;
	bin.resize( binLength, 0 );

	for( string::size_type i = 0; i != hex.size(); ++i )
	{
		int dec;
		if ( hex[i] >= '0' && hex[i] <= '9' )
			dec = hex[i] - '0';
		else if ( hex[i] >= 'a' && hex[i] <='f' )
			dec = hex[i] - 'a' + 10;
		else
			throw std::exception("Invalid hex expression");

		unsigned char mask = 0x08;
		for ( int j = 0; j < 4; ++j )
		{
			bin[i * 4 + j] = mask & dec ? 1 : 0;
			mask >>= 1;
		}
	}
}

void Bin2BitVec( std::vector<int>& bin, const std::string & binExp )
{
	int binLength = binExp.size();
	bin.resize( binLength, 0 );
	for( int i = 0; i < binLength; ++i ) {
		switch ( binExp[i] ) {
		case '0' : 
			bin[i] = 0; break;
		case '1' : 
			bin[i] = 1; break;
		default  :
			throw std::exception("Invalid binary expression");
		}
	}
}

const string GetCurrentTime()
{
	time_t t = time(0); 
	char tmp[64];
	struct tm newTime;
	localtime_s( &newTime, &t);
	strftime( tmp, sizeof(tmp), "%Y/%m/%d %X %A", &newTime );
	return string(tmp);
}

void ShowRunningInfo( std::ostream& os, std::string & info, int indent )
{
	std::string ind(indent * 2, ' ');
	os << ind << ">>> " << info;
}

unsigned BitVec2U( const std::vector<int>& bitVec )
{
	unsigned int u = 0;
	for( unsigned i = 0; i < bitVec.size(); ++i ) {
		u = (u << 1) + bitVec[i];
	}
	return u;
}