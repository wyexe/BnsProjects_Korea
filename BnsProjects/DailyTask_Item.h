#ifndef __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_COMMON_DAILYTASK_ITEM_H__
#define __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_COMMON_DAILYTASK_ITEM_H__

#include "GameVariable.h"

class CDailyTask_Item
{
public:
	CDailyTask_Item() = default;
	~CDailyTask_Item() = default;

	// �ǲ��ǰ�����
	BOOL IsDealWithDungeonItem() CONST;

	// ��������Ʒ(��)
	BOOL DealWithDungeonItem_Sell(_In_ CONST std::wstring& wsNpcName) CONST;

	// ��������Ʒ(������, ��, �ֽ�, ��....)
	VOID DealWithDungeonItem() CONST;

	// Repair
	BOOL IsRepairWeaspon(_In_ DWORD dwValue) CONST;

	VOID RepairWeaspon(_In_ DWORD dwValue) CONST;

	// Check DailyTask Initialize Item
	BOOL CheckDailyTaskInitializeItem() CONST;

private:
	// ��������Ʒ(������)
	VOID DealWithDungeonBox() CONST;

	// ��������Ʒ(��)
	VOID DealWithDungeon_Drop() CONST;

	// �����·�
	VOID DealWithCloth() CONST;

	// ����ʯ
	VOID DealWithGem() CONST;
};



#endif // !__GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_COMMON_DAILYTASK_ITEM_H__
