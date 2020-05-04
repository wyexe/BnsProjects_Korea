#include "stdafx.h"
#include "NpcExtend.h"
#include <MyTools/Log.h>
#include "ObjectFunction.h"
#include "BnsFilter.h"
#include "PersonFunction.h"
#include "SomeFun.h"
#include "BagFunction.h"
#include "Npc.h"
#include "Person.h"
#include "Cat.h"
#include "CatExpend.h"

#define _SELF L"NpcExtend.cpp"

BOOL CNpcExtend::FindNpc_By_Name(_In_ CONST std::wstring& wsNpcName, _In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CNpc* pNpc) CONST
{
	return FindNpc_By_Condiction(FixPtInfo, pNpc, [&wsNpcName](CONST CNpc& Npc){return Npc.GetName() == wsNpcName; });
}

BOOL CNpcExtend::FindNpc_By_Name(_In_ CONST std::wstring& wsNpcName, _In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ std::vector<CNpc>& vlst) CONST
{
	std::vector<CNpc> vNpc;
	CObjectFunction::GetInstance().GetSortGameObjectList<CNpc>(vNpc, CObjectFunction::em_GameObject_Type_Npc, FixPtInfo);

	for (CONST auto& itm : vNpc)
	{
		if (itm.GetName() == wsNpcName)
			vlst.push_back(itm);
	}
	return vlst.size() != NULL;
}

BOOL CNpcExtend::FindNpc_By_Id(_In_ DWORD dwNpcId, _In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CNpc* pNpc) CONST
{
	return FindNpc_By_Condiction(FixPtInfo, pNpc, [&dwNpcId](CONST CNpc& Npc){return Npc.GetId() == dwNpcId; });
}
BOOL CNpcExtend::FindNpc_By_Condiction(_In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CNpc* pNpc, std::function<BOOL(CONST CNpc&)> fnFindNpc) CONST
{
	return CObjectFunction::GetInstance().FindElement_By_ObjectTraverse<CNpc>(CObjectFunction::em_GameObject_Type_Npc, FixPtInfo, pNpc, fnFindNpc);
}

BOOL CNpcExtend::DoAction_By_ExistNpc(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<BOOL(CONST CNpc&)> fnFind, _In_ std::function<VOID(CONST CNpc&)> fnAction) CONST
{
	CNpc Npc;
	BOOL bExist = FALSE;

	if (FindNpc_By_Condiction(FixPtInfo, &Npc, fnFind))
	{
		fnAction(Npc);
		bExist = TRUE;
	}
	return bExist;
}

BOOL CNpcExtend::DoAction_By_ExistNpc_For_Name(_In_ CONST std::wstring wsName, _In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<VOID(CONST CNpc&)> fnAction) CONST
{
	return DoAction_By_ExistNpc(FixPtInfo, [&wsName](CONST CNpc& Npc){return Npc.GetName() == wsName; }, fnAction);
}

UINT CNpcExtend::GetAroundNpcList(_In_ CONST FixPointInfo& FixPtInfo, _Out_ std::vector<CNpc>& vlst) CONST
{
	CObjectFunction::GetInstance().GetSortGameObjectList<CNpc>(vlst, CObjectFunction::em_GameObject_Type_Npc, FixPtInfo);
	return CFilter::GetInstance().FilterList_By_MonsterFilter(vlst);
}


BOOL CNpcExtend::GetTopNpc_By_Dis(_In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CNpc* pNpc) CONST
{
	//return FindNpc_By_Condiction(FixPtInfo, pNpc, [](CONST CNpc& Npc){return TRUE; });
	std::vector<CNpc> NpcVec;
	GetAroundNpcList(FixPtInfo, NpcVec);
	return MyTools::CLPublic::Vec_find_if(NpcVec, pNpc, [](CONST CNpc&){ return TRUE; });
}

UINT CNpcExtend::GetAroundNpcCount(_In_ CONST FixPointInfo& FixPtInfo) CONST
{
	std::vector<CNpc> vlst;
	return GetAroundNpcList(FixPtInfo, vlst);
}

BOOL CNpcExtend::SellItem_By_Npc(_In_ CONST std::wstring& wsNpcName) CONST
{
	Point FixPoint = CBaseObject::GetPersonPoint();

	CNpc Npc;
	if (!FindNpc_By_Name(wsNpcName, FixPointInfo(CBaseObject::GetPersonPoint(), 30), &Npc))
	{
		LOG_CF_D( L"附近30m找不到出售物品的Npc:%s", wsNpcName.c_str());
		return FALSE;
	}

	// Move To Npc
	BnsMove(Npc.GetFrontPoint(2.0f), 1.0f);
	if (Npc.GetDis() > 3.0f)
	{
		LOG_CF_D( L"离Npc太远了, 就不卖了! Dis=%.2f", Npc.GetDis());
		return FALSE;
	}

	std::vector<CBag> vlst;
	CObjectFunction::GetInstance().GetBagList(vlst, CObjectFunction::em_Bag_Type_Bag);
	for (CONST auto& itm : vlst)
	{
		if (CFilter::GetInstance().ExistItemFilter(itm.GetName()) == CFilter::ItemFilterType::Filter_Item_Sell)
		{
			Npc.Sell(itm);
			LOG_CF_D(L"卖:%s", itm.GetName().c_str());
			BnsSleep(500);
		}
	}
	return TRUE;
}

UINT CNpcExtend::GetAroundTargetNpcList(_In_ CONST FixPointInfo& FixPtInfo, _Out_ std::vector<CNpc>& vlst) CONST
{
	GetAroundNpcList(FixPtInfo, vlst);

	CCat Cat;
	CCatExpend::GetInstance().FindSelfCat(&Cat);

	DWORD dwSelfId = CPerson::GetInstance().GetId();
	return MyTools::CLPublic::Vec_erase_if(vlst, [&dwSelfId, &Cat](CONST CNpc& Npc){
		if (Npc.GetTargetId() == dwSelfId)
			return FALSE;
		else if (Cat.GetBase() != NULL && Cat.GetId() == Npc.GetTargetId())
			return FALSE;
		return TRUE;
	});
}
