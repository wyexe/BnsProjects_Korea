#include "stdafx.h"
#include "BagFunction.h"
#include <MyTools/Log.h>
#include "ObjectFunction.h"
#include "SomeFun.h"
#include "Person.h"
#include "GameSend.h"

#define _SELF L"BagFunction.cpp"

DWORD CBagFunction::GetEndurance() CONST
{
	return ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(CPerson::GetInstance().GetBase() + 背包遍历偏移) + 0x1C) + 0x0) + 0x48) & 0xFF;
}

VOID CBagFunction::CleanBag() CONST
{
	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([]{CGameSend::CleanBag(); });
}

BOOL CBagFunction::CreateMoreBag() CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([]{CGameSend::CreateMoreBag(); });
}

BOOL CBagFunction::IsBagFull(_In_ int nBagType, DWORD dwRemainCount) CONST
{
	CObjectFunction::em_Bag_Type emBagType = static_cast<CObjectFunction::em_Bag_Type>(nBagType);
	DWORD dwMaxCount = CObjectFunction::GetInstance().GetMaxBagCount(emBagType);

	std::vector<CBag> vlst;
	if (CObjectFunction::GetInstance().GetBagList(vlst, emBagType) >= dwMaxCount - dwRemainCount)
	{
		LOG_CF_E( L"背包size=%d, Now=%d, 背包已经满了!", dwMaxCount, vlst.size());
		return TRUE;
	}
	return FALSE;
}

BOOL CBagFunction::GetEmptyItemObj(_In_ int nBagType , _Out_opt_ DWORD& dwItemObj) CONST
{
	std::vector<CBag> vlst;
	CObjectFunction::em_Bag_Type emBagType = static_cast<CObjectFunction::em_Bag_Type>(nBagType);
	CObjectFunction::GetInstance().GetBagList(vlst, emBagType);
	if (vlst.size() == CObjectFunction::GetInstance().GetMaxBagCount(emBagType))
	{
		LOG_CF_E( L"构建ItemObj失败,格子已满!, emBagType=%X", emBagType);
		return FALSE;
	}

	// 是否存在该序号
	auto fnExistIndex = [&vlst](DWORD dwIndex)
	{
		for (auto& Bag : vlst)
		{
			if (Bag.GetIndex() + 1 == dwIndex)
				return TRUE;
		}
		return FALSE;
	};

	BOOL bExist = FALSE;
	DWORD dwMaxCount = CObjectFunction::GetInstance().GetMaxBagCount(emBagType);
	for (DWORD i = 1; i < dwMaxCount; ++i)
	{
		if (!fnExistIndex(i))
		{
			// 构造Id
			switch (emBagType)
			{
			case CObjectTraverse::em_Bag_Type_Bag:
				dwItemObj = (i << 0x10) + 0x2;
				break;
			case CObjectTraverse::em_Bag_Type_Equi:
				break;
			case CObjectTraverse::em_Bag_Type_Warehouse:
				dwItemObj = (i << 0x10) + 0x3;
				break;
			case CObjectTraverse::em_Bag_Type_Wardrobe:
				dwItemObj = (i << 0x10) + 0x6;
				break;
			default:
				break;
			}
			
			bExist = TRUE;
			break;
		}
	}

	return bExist;
}


BOOL CBagFunction::ExistHammer(_Out_opt_ CBag* Hammer) CONST
{
	return ExistItem_By_Name(Item_修理工具, Hammer);
}

DWORD CBagFunction::GetHammerCount() CONST
{
	return GetItemCount_By_Name(Item_修理工具);
}

BOOL CBagFunction::ExistItem_By_Condiction(_In_ std::function<BOOL(CONST CBag&)> fn, _Out_opt_ CBag* pBag /*= nullptr*/) CONST
{
	std::vector<CBag> vlst;
	CObjectFunction::GetInstance().GetBagList(vlst, CObjectFunction::em_Bag_Type_Bag);

	return MyTools::CLPublic::Vec_find_if(vlst, pBag, fn);
}

BOOL CBagFunction::DoItem_By_EixstItem(_In_ std::function<BOOL(CONST CBag&)> fnFind, _In_ std::function<VOID(CONST CBag&)> fnDo) CONST
{
	std::vector<CBag> vlst;
	CObjectFunction::GetInstance().GetBagList(vlst, CObjectFunction::em_Bag_Type_Bag);
	
	CBag Itm;
	if (MyTools::CLPublic::Vec_find_if(vlst,	&Itm, fnFind))
	{
		fnDo(Itm);
		return TRUE;
	}
	return FALSE;
}

BOOL CBagFunction::DoItem_By_ExistItem_To_Name(_In_ CONST std::wstring& wsName, _In_ std::function<VOID(CONST CBag&)> fnDo) CONST
{
	return DoItem_By_EixstItem([&wsName](CONST CBag& Bag){ return Bag.GetName() == wsName; }, fnDo);
}

BOOL CBagFunction::ExistItem_By_Name(_In_ CONST std::wstring& wsItemName, _Out_opt_ CBag* pBag /* = nullptr OPTIONAL */) CONST
{
	return ExistItem_By_Condiction([&wsItemName](CONST CBag& Bag){return wsItemName == Bag.GetName() || (wsItemName + L" " == Bag.GetName()); }, pBag);
}

BOOL CBagFunction::ExistItem_By_Id(_In_ DWORD dwItemId, _Out_opt_ CBag* pBag /* = nullptr OPTIONAL */) CONST
{
	return ExistItem_By_Condiction([&dwItemId](CONST CBag& Bag){return dwItemId == Bag.GetGId();}, pBag);
}


BOOL CBagFunction::GetEqui_By_Type(_In_ CBag::em_Equi_Type emType, _Out_opt_ CBag* pEqui /* = nullptr OPTIONAL */) CONST
{
	return ExistEqui_By_Condiction([&emType](CONST CBag& Equi){return Equi.GetItemType() == static_cast<DWORD>(emType); }, pEqui);
}

BOOL CBagFunction::ExistEqui_By_Condiction(_In_ std::function<BOOL(CONST CBag&)> fn, _Out_opt_ CBag* pBag /*= nullptr*/) CONST
{
	std::vector<CBag> vlst;
	CObjectFunction::GetInstance().GetBagList(vlst, CObjectFunction::em_Bag_Type_Equi);

	return MyTools::CLPublic::Vec_find_if(vlst, pBag, fn);
}

BOOL CBagFunction::ExistEqui_By_Name(_In_ CONST std::wstring& wsName, _Out_opt_ CBag* pBag /*= nullptr*/) CONST
{
	return ExistEqui_By_Condiction([&wsName](CONST CBag& Equi){return Equi.GetName() == wsName; }, pBag);
}

UINT CBagFunction::GetItemCount_By_Name(_In_ CONST std::wstring& wsItemName) CONST
{
	std::vector<CBag> vlst;
	CObjectFunction::GetInstance().GetBagList(vlst, CObjectFunction::em_Bag_Type_Bag);

	DWORD dwCount = 0;
	for (auto& Bag : vlst)
	{
		if (wsItemName == Bag.GetName())
			dwCount += Bag.GetCount();
	}

	return dwCount;
}

BOOL CBagFunction::GetEquiType_By_Text(_In_ CONST std::wstring& wsText, _Out_opt_ Equi_Type& EquiType)
{
	CONST static std::vector<Equi_Type> vlst = 
	{
		{ L"武器", CBag::em_Equi_Type::em_Equi_Type_Weapon },
		{ L"项链", CBag::em_Equi_Type::em_Equi_Type_Necklace },
		{ L"戒指", CBag::em_Equi_Type::em_Equi_Type_Ring },
		{ L"耳环", CBag::em_Equi_Type::em_Equi_Type_Earring },
		{ L"手镯", CBag::em_Equi_Type::em_Equi_Type_Bracelet },
		{ L"腰带", CBag::em_Equi_Type::em_Equi_Type_Belt },
		{ L"魂", CBag::em_Equi_Type::em_Equi_Type_Soul }
	};

	return MyTools::CLPublic::Vec_find_if(vlst, &EquiType, [&wsText](CONST Equi_Type& EquiType_){
		return EquiType_.wsEquiName == wsText;
	});
}

BOOL CBagFunction::ExistGem_By_Weaspon(DWORD dwItemId) CONST
{
	std::vector<DWORD> vlst;
	CObjectFunction::GetInstance().GetWeasponGemList(vlst);

	return MyTools::CLPublic::Vec_find_if(vlst, static_cast<DWORD*>(nullptr), [&dwItemId](_In_ DWORD dwItemId_){
		return dwItemId_ == dwItemId;
	});
}

UINT CBagFunction::TotalOccupyItem(int emBagType) CONST
{
	std::vector<CBag> VecBag;
	CObjectFunction::GetInstance().GetBagList(VecBag, static_cast<CObjectFunction::em_Bag_Type>(emBagType));

	UINT uCount = 0;
	for (CONST auto& itm : VecBag)
		uCount += itm.GetCount();

	return uCount;
}

UINT CBagFunction::GetItemList_By_Name(_In_ CONST std::wstring& wsItemName, _Out_ std::vector<CBag>& VecItem) CONST
{
	std::vector<CBag> VecBag;
	CObjectFunction::GetInstance().GetBagList(VecBag, CObjectFunction::em_Bag_Type::em_Bag_Type_Bag);

	for (CONST auto& itm : VecBag)
	{
		if (itm.GetName() == wsItemName)
			VecItem.push_back(std::move(itm));
	}
	return VecItem.size();
}
