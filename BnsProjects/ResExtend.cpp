#include "stdafx.h"
#include "ResExtend.h"
#include "DropItem.h"
#include "Corpse.h"
#include <MyTools/Log.h>
#include "ObjectFunction.h"
#include "BnsFilter.h"

#define _SELF L"ResExtend.cpp"
BOOL CResExtend::FindCorpse_By_Id(_In_ DWORD dwCorpseId, _In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CCorpse* pCorpse) CONST
{
	return FindCorpse_By_Condition(FixPtInfo, pCorpse, [&dwCorpseId](CONST CCorpse& Corpse){return Corpse.GetId() == dwCorpseId; });
}

BOOL CResExtend::FindCorpse_By_Name(_In_ CONST std::wstring& wsCorpseName, _In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CCorpse* pCorpse) CONST
{
	return FindCorpse_By_Condition(FixPtInfo, pCorpse, [&wsCorpseName](CONST CCorpse& Corpse){return Corpse.GetName() == wsCorpseName; });
}

BOOL CResExtend::FindCorpse_By_Condition(_In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CCorpse* pCorpse, std::function<BOOL(CONST CCorpse&)> fnFindCorpse) CONST
{
	return CObjectFunction::GetInstance().FindElement_By_ObjectTraverse<CCorpse>(CObjectFunction::em_GameObject_Type_Corpse, FixPtInfo, pCorpse, fnFindCorpse);
}

UINT CResExtend::GetAroundCorpseList(_In_ CONST FixPointInfo& FixPtInfo, _Out_ std::vector<CCorpse>& vlst) CONST
{
	CObjectFunction::GetInstance().GetSortGameObjectList<CCorpse>(vlst, CObjectFunction::em_GameObject_Type_Corpse, FixPtInfo);
	return MyTools::CLPublic::Vec_erase_if(vlst, [](CONST CCorpse& itm){return !itm.IsCollect(); });
}

UINT CResExtend::GetAroundCorpseCount(_In_ CONST FixPointInfo& FixPtInfo) CONST
{
	std::vector<CCorpse> vlst;
	return GetAroundCorpseList(FixPtInfo, vlst);
}

UINT CResExtend::GetCorpseList_By_Condiction(_In_ CONST FixPointInfo& FixPtInfo, _Out_ std::vector<CCorpse>& vlst, _In_ std::function<BOOL(CONST CCorpse&)> fnFind) CONST
{
	GetAroundCorpseList(FixPtInfo, vlst);
	return MyTools::CLPublic::Vec_erase_if(vlst, fnFind);
}

BOOL CResExtend::Action_Corpse_By_MinDis(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<VOID(CONST CCorpse&)> Worker) CONST
{
	CCorpse Corpse;
	if (FindCorpse_By_Condition(FixPtInfo, &Corpse, [](CONST CCorpse&){ return TRUE; }))
	{
		Worker(Corpse);
		return TRUE;
	}
	return FALSE;
}


UINT CResExtend::GetAroundDropItem(_In_ CONST FixPointInfo& FixPtInfo, _Out_ std::vector<CDropItem>& vlst) CONST
{
	CObjectFunction::GetInstance().GetSortGameObjectList<CDropItem>(vlst, CObjectFunction::em_GameObject_Type_DropItem, FixPtInfo);
	return MyTools::CLPublic::Vec_erase_if(vlst, [](CONST CDropItem& itm){return !itm.IsDropItem(); });
}

BOOL CResExtend::GetCurrentDropItem(_In_ CONST FixPointInfo& FixPtInfo, _Out_ CDropItem& DropItem) CONST
{
	std::vector<CDropItem> VecDropItem;
	if (GetAroundDropItem(FixPtInfo, VecDropItem) == 0)
		return FALSE;

	for (CONST auto& itm : VecDropItem)
	{
		if (CFilter::GetInstance().ExistIdInDropItemBlackList(itm.GetId()))
		{
			LOG_CF_D(L"周围地上物品ID=%d,存在黑名单",itm.GetId());
			continue;
		}

		DropItem = itm;
		return TRUE;
	}

	return FALSE;
}

UINT CResExtend::GetAroundCorpseDropItem(_In_ CONST FixPointInfo& FixPtInfo, _Out_ std::vector<CDropItem>& vlst) CONST
{
	CObjectFunction::GetInstance().GetSortGameObjectList<CDropItem>(vlst, CObjectFunction::em_GameObject_Type_DropItem, FixPtInfo);

	return MyTools::CLPublic::Vec_erase_if(vlst, [](CONST CDropItem& itm){return !itm.IsCorpse(); });
}

BOOL CResExtend::FindDropItemCorpse_By_Id(_In_ DWORD dwId, _In_ CONST FixPointInfo& FixPtInfo, _In_ CDropItem* pDropItem /* = nullptr */) CONST
{
	return FindDropItem_By_Condition(FixPtInfo, pDropItem, [&dwId](CONST CDropItem& DropItem_){return DropItem_.GetMonsterId() == dwId; });
}

BOOL CResExtend::FindDropItem_By_Condition(_In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CDropItem* pDropItem, std::function<BOOL(CONST CDropItem&)> fnFind) CONST
{
	return CObjectFunction::GetInstance().FindElement_By_ObjectTraverse<CDropItem>(CObjectFunction::em_GameObject_Type_DropItem, FixPtInfo, pDropItem, fnFind);
}

BOOL CResExtend::FindTopBall(_In_ CONST FixPointInfo& FixPtInfo, _In_ CONST std::vector<DWORD>& FilterBallIdVec, _Out_ CCorpse* pBall) CONST
{
	return FindBall_By_Condition(FixPtInfo, pBall, [&FilterBallIdVec](CONST CCorpse& Ball){return MyTools::CLPublic::Vec_find_if_Const(FilterBallIdVec, [&Ball](DWORD dwBallId){ return dwBallId == Ball.GetId(); }) == nullptr ? TRUE : FALSE;});
}

BOOL CResExtend::FindBall_By_Id(_In_ DWORD dwId, _In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CCorpse* pBall) CONST
{
	return FindBall_By_Condition(FixPtInfo, pBall, [&dwId](CONST CCorpse& Corpse){return dwId == Corpse.GetId();});
}

BOOL CResExtend::FindBall_By_Condition(_In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CCorpse* pBall, std::function<BOOL(CONST CCorpse&)> fnFind) CONST
{
	return CObjectFunction::GetInstance().FindElement_By_ObjectTraverse<CCorpse>(CObjectFunction::em_GameObject_Type_MonsterCorpse, FixPtInfo, pBall, fnFind);
}

UINT CResExtend::GetAroundBallList(_In_ CONST FixPointInfo& FixPtInfo, _Out_ std::vector<CCorpse>& vlst) CONST
{
	return CObjectFunction::GetInstance().GetSortGameObjectList<CCorpse>(vlst, CObjectFunction::em_GameObject_Type_MonsterCorpse, FixPtInfo);
}

BOOL CResExtend::DoAction_By_ExistCorpse_For_Name(_In_ CONST std::wstring& wsCorpseName, _In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<VOID(CONST CCorpse&)> fn) CONST
{
	CCorpse Corpse;
	if (FindCorpse_By_Name(wsCorpseName, FixPtInfo, &Corpse))
	{
		fn(Corpse);
		return TRUE;
	}
	return FALSE;
}

