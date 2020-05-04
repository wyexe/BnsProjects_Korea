#ifndef __GITBNSPROJECTS_BNSPROJECTS_TEXT_RESTEXT_H__
#define __GITBNSPROJECTS_BNSPROJECTS_TEXT_RESTEXT_H__

#include "GameStruct.h"
#include <MyTools/ClassInstance.h>
#include <MyTools/RelfexStruct.h>

struct ItemFilterVarInfo;
class CResText : public CClassInstance<CResText>, public CRelfexStruct
{
public:
	CResText() = default;
	virtual ~CResText() = default;

public:
	
	

public:
	// 初始化反射
	VOID Initialize();

	
private:
	// 注册物品过滤
	VOID RegisterItemFilterText();

	// 注册怪物过滤
	VOID RegisterMonsterFilterText();

	// 注册物品出售分类
	VOID RegisterItemSellText();

	// 注册游戏断开连接的提示
	VOID RegisterDisConnectText();

	// 注册角色职业Text
	VOID RegisterPlayerClassesText();

	// 注册物品丢弃的名字
	VOID RegisterItemDiscardName();

	// 注册物品名字
	VOID ReigisterItemName();

private:
	
};




#endif // !__GITBNSPROJECTS_BNSPROJECTS_TEXT_RESTEXT_H__
