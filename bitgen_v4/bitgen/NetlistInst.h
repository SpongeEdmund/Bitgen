#ifndef _CKT_INST_H_
#define _CKT_INST_H_

#include <string>
#include <vector>

using std::string;

struct LogicSpec;

/************************************************************************/
/*���ñ���߱�������
/************************************************************************/

// �����ݽṹ������inst�ڲ����������ƺ�����ֵ���Լ��ڸ������µ�����
struct InstCfg
{
	//enum CfgType
	//{
	//	KEY_VALUE,      // ��ֵ�ͣ�����cil��Ϊkey��ÿһ��value��ָ����sram�������
	//	BOOLEAN_LOGIC,  // �����߼�����Ҫ���㲼�����ʽ�����ò��ұ��16��sram
	//	SRAM_BIT        // �洢���ͣ���2������ֱ��ģ�����еĴ洢���
	//};
	string  _attribute;
	string  _instName;
	string  _option;
	//bool isMap;
	//InstCfg(CfgType t = KEY_VALUE) : type(t) {}
};



struct NetlistInst
{
	typedef std::vector<InstCfg> InstCfgs;

	//void listLogicCfg( LogicCfg &logicCfg ){}

	string _instName;
	string _siteDef;    //��inst���ֺ�����site������
	string _placedTile; //��inst���ֺ�������tile
	string _placedSite; //��inst���ֺ�������site

	InstCfgs _cfgs;
};



#endif 