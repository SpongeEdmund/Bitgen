/************************************************************************
 *File Name : CktDesign.h
 *Author    : Edmund Wong
 *Date      : 2013-1-14
 ************************************************************************/


#ifndef _CKT_DESIGN_H_
#define _CKT_DESIGN_H_


#include <string>
#include <iostream>

using std::string;

//CktDesign中包含design的名称，所用芯片信息，以及ncd版本号
struct NetlistDesign
{
	string designName;
	//芯片信息
	//例如 xc4vlx15ff668-12
	string device;
	string package;
	string speed;
	
	string ncdVersion;
	
};

std::ostream& operator<< ( std::ostream& , const NetlistDesign& );

#endif