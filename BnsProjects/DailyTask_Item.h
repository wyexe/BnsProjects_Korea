#ifndef __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_COMMON_DAILYTASK_ITEM_H__
#define __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_COMMON_DAILYTASK_ITEM_H__

#include "GameVariable.h"

class CDailyTask_Item
{
public:
	CDailyTask_Item() = default;
	~CDailyTask_Item() = default;

	// 是不是包满了
	BOOL IsDealWithDungeonItem() CONST;

	// 处理副本物品(卖)
	BOOL DealWithDungeonItem_Sell(_In_ CONST std::wstring& wsNpcName) CONST;

	// 处理副本物品(开箱子, 卖, 分解, 丢....)
	VOID DealWithDungeonItem() CONST;

	// Repair
	BOOL IsRepairWeaspon(_In_ DWORD dwValue) CONST;

	VOID RepairWeaspon(_In_ DWORD dwValue) CONST;

	// Check DailyTask Initialize Item
	BOOL CheckDailyTaskInitializeItem() CONST;

private:
	// 处理副本物品(开箱子)
	VOID DealWithDungeonBox() CONST;

	// 处理副本物品(丢)
	VOID DealWithDungeon_Drop() CONST;

	// 处理衣服
	VOID DealWithCloth() CONST;

	// 处理宝石
	VOID DealWithGem() CONST;
};



#endif // !__GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_COMMON_DAILYTASK_ITEM_H__
