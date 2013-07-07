#ifndef _CKT_INST_H_
#define _CKT_INST_H_

#include <string>
#include <vector>

using std::string;

struct LogicSpec;

/************************************************************************/
/*配置本身具备的属性
/************************************************************************/

// 该数据结构是描述inst内部各属性名称和配置值，以及在该配置下的属性
struct InstCfg
{
	//enum CfgType
	//{
	//	KEY_VALUE,      // 键值型，即在cil中为key的每一个value都指定了sram配置情况
	//	BOOLEAN_LOGIC,  // 布尔逻辑，需要计算布尔表达式以配置查找表的16个sram
	//	SRAM_BIT        // 存储器型，以2进制数直接模拟阵列的存储情况
	//};
	enum Type {
		Attribute,
		Sram
	};
	InstCfg() : _type(Attribute) {}
	// Attribute type
	string  _attribute;
	string  _instName;
	string  _option;
	// Sram type
	string _sramName;
	int    _sramVal;
	
	//bool isMap;

	Type _type;
};



struct NetlistInst
{
	typedef std::vector<InstCfg> InstCfgs;

	//void listLogicCfg( LogicCfg &logicCfg ){}

	string _instName;
	string _siteDef;    //该inst布局后所处site的类型
	string _placedTile; //该inst布局后所处的tile
	string _placedSite; //该inst布局后所处的site

	InstCfgs _cfgs;
};



#endif 