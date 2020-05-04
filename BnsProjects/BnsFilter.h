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
	// �Ƿ������Ʒ����
	ItemFilterType ExistItemFilter(_In_ CONST std::wstring& wsName);

	CONST std::wstring ReplaceItemName(_In_ CONST std::wstring& wsItemName) CONST;

	///////��Ʒ����///////////////////////////////////////////////////////////////////
	VOID AddItemFilter(_In_ CONST std::wstring& wsName, _In_ ItemFilterType FilterType);
	BOOL AddItemFilter(_In_ CONST std::wstring& wsName, _In_ CONST std::wstring& wsText);

	///////���ﳬʱ������///////////////////////////////////////////////////////////////////
	// ��ӵ����������
	VOID AddDropItemToBlackList(_In_ DWORD dwId);

	// �ж��Ƿ���ڼ��������
	BOOL ExistIdInDropItemBlackList(_In_ DWORD dwId);

	// �����ʱ�ļ��������
	VOID ClearTimeOutDropItemBlackList();

	///////�������///////////////////////////////////////////////////////////////////
	// ��ӹ������
	VOID AddMonsterFilter(_In_ CONST std::wstring& wsName, _In_ MonsterFilterType emMonsterFilterType);

	// ����Text�����
	BOOL AddMonsterFilter(_In_ CONST std::wstring& wsName, _In_ CONST std::wstring& wsFilterText);

	// ��ʱ������
	VOID AddTemporaryMonsterFilter(_In_ CONST std::wstring& wsName);

	//
	VOID RemoveMonsterFilter(_In_ CONST std::wstring& wsName);

	//
	MonsterFilterType ExistMonsterFilter(_In_ CONST std::wstring& wsName);

	//
	VOID ClearTemporaryMonster();

	//
	VOID ClearMonsterFilter();

	// ���ݹ�������б�ȥ����
	template<typename T>
	UINT FilterList_By_MonsterFilter(_In_ _Out_ std::vector<T>& Vec)
	{
		BOOL bFilterHigh = FALSE;
		ClearTemporaryMonster();
		for (int i = 0; i < static_cast<int>(Vec.size()); ++i)
		{
			// �����б�
			auto FilterType = ExistMonsterFilter(Vec.at(i).GetName());
			switch (FilterType)
			{
			case  MonsterFilterType::Filter_Monster_High:
				// ����ɱ�Ĺ�, ֱ������ͷ��! ֱ�ӷ�����������
				if (!bFilterHigh)
				{
					std::swap(Vec.at(i), Vec.at(0));
					bFilterHigh = TRUE;
				}
			case MonsterFilterType::Filter_Monster_Default:
				// Ĭ��, ����
				break;
			case MonsterFilterType::Filter_Monster_Low:
				// ��ɱ, ���б���ɾ��
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