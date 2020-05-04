#include "stdafx.h"
#include "BnsFilter.h"
#include <MyTools/CLFile.h>
#include <MyTools/CLLock.h>
#include <MyTools/Log.h>
#define _SELF L"Filter.cpp"

CFilter::ItemFilterType CFilter::ExistItemFilter(_In_ CONST std::wstring& wsName)
{
	std::lock_guard<std::mutex> _Lock(_MtxItemFilter);

	auto itr = _MapItemFilterContent.find(ReplaceItemName(wsName));
	return itr == _MapItemFilterContent.end() ? CFilter::ItemFilterType::Filter_Item_Default : itr->second;
}

CONST std::wstring CFilter::ReplaceItemName(_In_ CONST std::wstring& wsItemName) CONST
{
	CONST static std::map<std::wstring, std::wstring> MapText =
	{
		{ L"백월팔찌 ", L"백월팔찌" }
	};

	auto itr = MapText.find(wsItemName);
	return itr == MapText.end() ? wsItemName : itr->second;
}

VOID CFilter::AddItemFilter(_In_ CONST std::wstring& wsName, _In_ ItemFilterType FilterType)
{
	std::lock_guard<std::mutex> _Lock(_MtxItemFilter);

	auto wsItemName = ReplaceItemName(wsName);
	auto itr = _MapItemFilterContent.find(wsItemName);
	if (itr == _MapItemFilterContent.end())
	{
		_MapItemFilterContent.insert(std::make_pair(wsItemName, FilterType));
	}
	else if (itr->second != FilterType)
	{
		itr->second = FilterType;
	}
}

BOOL CFilter::AddItemFilter(_In_ CONST std::wstring& wsName, _In_ CONST std::wstring& wsText)
{
	CONST static std::map<std::wstring, ItemFilterType> MapText = 
	{
		{ L"丢", ItemFilterType::Filter_Item_Drop },
		{ L"卖", ItemFilterType::Filter_Item_Sell },
		{ L"存", ItemFilterType::Filter_Item_Save },
		{ L"分解", ItemFilterType::Filter_Item_Explain },
		{ L"保留", ItemFilterType::Filter_Item_Keep },
		{ L"默认", ItemFilterType::Filter_Item_Default }
	};

	auto itr = MapText.find(wsText);
	if (itr == MapText.end())
		return FALSE;

	AddItemFilter(wsName, itr->second);
	return TRUE;
}

VOID CFilter::AddDropItemToBlackList(_In_ DWORD dwId)
{
	std::lock_guard<std::mutex> DropItemLock(_MutexDropItem);
	_MapBlackListDropItem.insert(std::make_pair(dwId, ::GetTickCount64()));
}

BOOL CFilter::ExistIdInDropItemBlackList(_In_ DWORD dwId)
{
	std::lock_guard<std::mutex> DropItemLock(_MutexDropItem);
	return _MapBlackListDropItem.find(dwId) != _MapBlackListDropItem.end();
}

VOID CFilter::ClearTimeOutDropItemBlackList()
{
	std::lock_guard<std::mutex> DropItemLock(_MutexDropItem);
	for (auto itr = _MapBlackListDropItem.begin(); itr != _MapBlackListDropItem.end();)
	{
		if (::GetTickCount64() - itr->second >= 60 * 1000)
		{
			LOG_CF_D(L"地上物品黑名单ID=%d,删除捡物黑名单", itr->first);
			itr = _MapBlackListDropItem.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}

VOID CFilter::AddMonsterFilter(_In_ CONST std::wstring& wsName, _In_ MonsterFilterType emMonsterFilterType)
{
	std::lock_guard<std::mutex> _Lock(_MtxMonsterFilter);

	auto itr = _MapMonsterFilterContent.find(wsName);
	if (itr == _MapMonsterFilterContent.end())
	{
		_MapMonsterFilterContent.insert(std::make_pair(wsName, emMonsterFilterType));
	}
	else if (itr->second != emMonsterFilterType)
	{
		itr->second = emMonsterFilterType;
	}
}

CFilter::MonsterFilterType CFilter::ExistMonsterFilter(_In_ CONST std::wstring& wsName)
{
	std::lock_guard<std::mutex> _Lock(_MtxMonsterFilter);

	// 临时黑名单
	if (_MapTemporaryMonsterFilterContent.find(wsName) != _MapTemporaryMonsterFilterContent.end())
		return MonsterFilterType::Filter_Monster_Low;

	// 怪物过滤列表
	auto itr = _MapMonsterFilterContent.find(wsName);
	return itr == _MapMonsterFilterContent.end() ? CFilter::MonsterFilterType::Filter_Monster_Default : itr->second;
}

VOID CFilter::ClearTemporaryMonster()
{
	std::lock_guard<std::mutex> _Lock(_MtxMonsterFilter);

	for (auto itr = _MapTemporaryMonsterFilterContent.begin(); itr != _MapTemporaryMonsterFilterContent.end();)
	{
		if (itr->second.ulTick != NULL && ::GetTickCount64() - itr->second.ulTick >= 20 * 1000)
			itr = _MapTemporaryMonsterFilterContent.erase(itr);
		else
			itr++;
	}
}

VOID CFilter::ClearMonsterFilter()
{
	std::lock_guard<std::mutex> _Lock(_MtxMonsterFilter);
	_MapMonsterFilterContent.clear();
}

BOOL CFilter::AddMonsterFilter(_In_ CONST std::wstring& wsName, _In_ CONST std::wstring& wsFilterText)
{
	struct FilterTextContent
	{
		std::wstring		wsText;
		MonsterFilterType	FilterType;
	};

	CONST static std::vector<FilterTextContent> Vec =
	{
		{ L"优先", MonsterFilterType::Filter_Monster_High },
		{ L"必杀", MonsterFilterType::Filter_Monster_Default },
		{ L"不杀", MonsterFilterType::Filter_Monster_Low }
	};

	auto Element = MyTools::CLPublic::Vec_find_if_Const(Vec, [wsFilterText](CONST FilterTextContent& itm){ return itm.wsText == wsFilterText; });
	if (Element == nullptr)
		return FALSE;

	AddMonsterFilter(wsName, Element->FilterType);
	return TRUE;
}

VOID CFilter::AddTemporaryMonsterFilter(_In_ CONST std::wstring& wsName)
{
	std::lock_guard<std::mutex> _Lock(_MtxMonsterFilter);

	if (_MapTemporaryMonsterFilterContent.find(wsName) == _MapTemporaryMonsterFilterContent.end())
	{
		MonsterFilterContent Content;
		Content.FilterType = MonsterFilterType::Filter_Monster_Low;
		Content.ulTick = ::GetTickCount64();
		_MapTemporaryMonsterFilterContent.insert(std::make_pair(wsName, std::move(Content)));
	}
}

VOID CFilter::RemoveMonsterFilter(_In_ CONST std::wstring& wsName)
{
	std::lock_guard<std::mutex> _Lock(_MtxMonsterFilter);

	auto itr = _MapMonsterFilterContent.find(wsName);
	if (itr != _MapMonsterFilterContent.end())
	{
		_MapMonsterFilterContent.erase(itr);
	}
}

