#ifndef __GITBNSPROJECTS_BNSPROJECTS_FILTER_FILTER_H__
#define __GITBNSPROJECTS_BNSPROJECTS_FILTER_FILTER_H__
#include "GameVariable.h"
class CFilter : public MyTools::CClassInstance<CFilter>
{
public:
	CFilter() = default;
	~CFilter() = default;
public:
	enum class  MonsterFilterType
	{
		Filter_Monster_High,
		Filter_Monster_Default,
		Filter_Monster_Low
	};

	struct MonsterFilterContent
	{
		MonsterFilterType	FilterType = MonsterFilterType::Filter_Monster_Default;
		ULONGLONG			ulTick		= NULL;
	};

	enum class ItemFilterType
	{
		Filter_Item_Drop, 
		Filter_Item_Sell, 
		Filter_Item_Save, 
		Filter_Item_Explain, 
		Filter_Item_Keep, 
		Filter_Item_Default 
	};
public:
	// 是否存在物品过滤
	ItemFilterType ExistItemFilter(_In_ CONST std::wstring& wsName);

	CONST std::wstring ReplaceItemName(_In_ CONST std::wstring& wsItemName) CONST;

	///////物品过滤///////////////////////////////////////////////////////////////////
	VOID AddItemFilter(_In_ CONST std::wstring& wsName, _In_ ItemFilterType FilterType);
	BOOL AddItemFilter(_In_ CONST std::wstring& wsName, _In_ CONST std::wstring& wsText);

	///////捡物超时黑名单///////////////////////////////////////////////////////////////////
	// 添加到捡物黑名单
	VOID AddDropItemToBlackList(_In_ DWORD dwId);

	// 判断是否存在捡物黑名单
	BOOL ExistIdInDropItemBlackList(_In_ DWORD dwId);

	// 清除超时的捡物黑名单
	VOID ClearTimeOutDropItemBlackList();

	///////怪物过滤///////////////////////////////////////////////////////////////////
	// 添加怪物过滤
	VOID AddMonsterFilter(_In_ CONST std::wstring& wsName, _In_ MonsterFilterType emMonsterFilterType);

	// 根据Text来添加
	BOOL AddMonsterFilter(_In_ CONST std::wstring& wsName, _In_ CONST std::wstring& wsFilterText);

	// 临时黑名单
	VOID AddTemporaryMonsterFilter(_In_ CONST std::wstring& wsName);

	//
	VOID RemoveMonsterFilter(_In_ CONST std::wstring& wsName);

	//
	MonsterFilterType ExistMonsterFilter(_In_ CONST std::wstring& wsName);

	//
	VOID ClearTemporaryMonster();

	//
	VOID ClearMonsterFilter();

	// 根据怪物过滤列表去排序
	template<typename T>
	UINT FilterList_By_MonsterFilter(_In_ _Out_ std::vector<T>& Vec)
	{
		BOOL bFilterHigh = FALSE;
		ClearTemporaryMonster();
		for (int i = 0; i < static_cast<int>(Vec.size()); ++i)
		{
			// 过滤列表
			auto FilterType = ExistMonsterFilter(Vec.at(i).GetName());
			switch (FilterType)
			{
			case  MonsterFilterType::Filter_Monster_High:
				// 优先杀的怪, 直接排序到头部! 直接放弃其他遍历
				if (!bFilterHigh)
				{
					std::swap(Vec.at(i), Vec.at(0));
					bFilterHigh = TRUE;
				}
			case MonsterFilterType::Filter_Monster_Default:
				// 默认, 不管
				break;
			case MonsterFilterType::Filter_Monster_Low:
				// 不杀, 从列表中删除
				Vec.erase(Vec.begin() + i--);
				break;
			default:
				break;
			}
		}
		return Vec.size();
	}
private:
		std::mutex _MutexDropItem;
		std::map<DWORD, ULONGLONG> _MapBlackListDropItem;

		std::mutex _MtxMonsterFilter;
		std::map<std::wstring, MonsterFilterContent> _MapTemporaryMonsterFilterContent;
		std::map<std::wstring, MonsterFilterType> _MapMonsterFilterContent;

		std::mutex _MtxItemFilter;
		std::map<std::wstring, ItemFilterType> _MapItemFilterContent;
private:
	DSIABLE_COPY_AND_ASSIGN(CFilter)
};




#endif