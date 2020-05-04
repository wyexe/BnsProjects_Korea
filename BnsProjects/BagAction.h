#ifndef __GITBNSPROJECTS_BNSPROJECTS_OBJECT_OTHEROBJECT_BAG_BAGACTION_H__
#define __GITBNSPROJECTS_BNSPROJECTS_OBJECT_OTHEROBJECT_BAG_BAGACTION_H__

#include "BagBase.h"
class CBagAction : public MyTools::CClassInstance<CBagAction>
{
public:
	CBagAction() = default;
	~CBagAction() = default;
public:
	// 分解东西
	BOOL ExplainItem() CONST;

	// 丢东西
	BOOL DiscardItem() CONST;

	// 丢东西(例外物品的名字)
	BOOL DiscardItem_By_Type(_In_ CONST std::vector<CBag::em_Equi_Type>& VecType) CONST;

	// 是否需要啃药 -> 补充血量
	BOOL SupplementHp() CONST;

	// 强化武器
	BOOL EquiGrowth(_In_ CBag::em_Equi_Type emType, _In_ CONST std::wstring& wsSacrificeName) CONST;

	// 强化没强化过的八卦
	BOOL BaGuaGrowth(_In_ CONST std::wstring& wsSacrificeName) CONST;

	// 穿装备
	BOOL WearEqui_By_Name(_In_ CONST std::wstring& wsEquiName) CONST;

	// 大转盘
	BOOL RollWheel_By_ItemName(_In_ CONST std::wstring& wsItemName, _In_ DWORD dwWheelId) CONST;

	// 存仓
	BOOL SaveItem(_In_ int nBagType, _In_ CONST std::wstring& wsNpcName) CONST;

	// 取仓
	BOOL PullItem_By_WareHouse(_In_ CONST std::wstring& wsNpcName, _In_ CONST std::vector<std::wstring>& vItem) CONST;

	struct tagPullWardrobeItm{ std::wstring wsItmName; UINT uCount; };
	BOOL PullItemByWardrobeWithList(_In_ CONST std::wstring& wsNpcName, _In_ CONST CBagAction::tagPullWardrobeItm& PullItm) CONST;

	// 快速修理武器!
	BOOL QuickRepairWeaspon() CONST;

	// 从背包购买物品
	BOOL Buy(_In_ CONST std::wstring& wsItemName, _In_ DWORD dwPageIndex, _In_ DWORD dwIndex, _In_ DWORD dwCount) CONST;

	// 取宝石
	BOOL RemoveALLGem() CONST;

	// 是否存在物品Name
	BOOL ExistItemByNameWithType(_In_ CONST std::wstring& wsItemName, _In_ int nBagType, _Out_opt_ CBag* pBag) CONST;

	// 丢物品
	VOID DiscardItem_By_Condition(_In_ std::function<BOOL(CONST CBag&)> fnItemFilter) CONST;

	// 使用物品
	VOID UseItem_By_Condition(_In_ std::function<BOOL(CONST CBag&)> fnItemFilter) CONST;

	// 开箱子
	BOOL OpenBox_By_Condition(_In_ std::function<BOOL(CONST CBag&)> fnItemFilter) CONST;

	// 分解装备
	BOOL Explain_By_Condition(_In_ std::function<BOOL(CONST CBag&)> fnItemFilter) CONST;

	// 卖物品
	BOOL SellItem_By_Condition(_In_ CONST std::wstring& wsNpcName, _In_ std::function<BOOL(CONST CBag&)> fnItemFilter) CONST;
	
private:
	DSIABLE_COPY_AND_ASSIGN(CBagAction)
};



#endif