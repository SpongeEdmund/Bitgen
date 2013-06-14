#include "NetlistDesign.h"

std::ostream & operator<< ( std::ostream &os, const NetlistDesign &design )
{
	//using std::endl;

	os << "Target chip information : \n" ;
	os << "design      : " << design.designName << "\n";
	os << "device      : " << design.device << "\n";
	os << "package     : " << design.package << "\n";
	os << "speed       : " << design.speed << "\n";
	os << "ncd version : " << design.ncdVersion << "\n";
	return os;
}