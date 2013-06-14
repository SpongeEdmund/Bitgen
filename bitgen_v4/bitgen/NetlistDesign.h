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

//CktDesign�а���design�����ƣ�����оƬ��Ϣ���Լ�ncd�汾��
struct NetlistDesign
{
	string designName;
	//оƬ��Ϣ
	//���� xc4vlx15ff668-12
	string device;
	string package;
	string speed;
	
	string ncdVersion;
	
};

std::ostream& operator<< ( std::ostream& , const NetlistDesign& );

#endif