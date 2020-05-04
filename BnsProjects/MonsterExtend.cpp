#include "stdafx.h"
#include "MonsterExtend.h"
#include "ObjectFunction.h"
#include "BnsFilter.h"
#include "Monster.h"
#include "Person.h"
#include "CatExpend.h"

#define _SELF L"MonsterExtend.cpp"
BOOL CMonsterExtend::FindMonster_By_Name(_In_ CONST std::wstring& wsMonsterName, _In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CMonster* pMonster) CONST
{
	return FindMonster_By_Condiction(FixPtInfo, pMonster, [&wsMonsterName](CONST CMonster& Monster){return wsMonsterName == Monster.GetName(); });
}

BOOL CMonsterExtend::FindMonster_By_Id(_In_ DWORD dwMonsterId, _In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CMonster* pMonster) CONST
{
	return FindMonster_By_Condiction(FixPtInfo, pMonster, [&dwMonsterId](CONST CMonster& Monster){return dwMonsterId == Monster.GetId(); });
}

BOOL CMonsterExtend::FindMonster_By_Condiction(_In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CMonster* pMonster, _In_ std::function<BOOL(CONST CMonster&)> fnFindMonster) CONST
{
	return CObjectFunction::GetInstance().FindElement_By_ObjectTraverse<CMonster>(CObjectFunction::em_GameObject_Type_Monster, FixPtInfo, pMonster, fnFindMonster);
}

UINT CMonsterExtend::GetAroundMonsterList(_In_ CONST FixPointInfo& FixPtInfo, _Out_ std::vector<CMonster>& vlst) CONST
{
	CObjectFunction::GetInstance().GetSortGameObjectList<CMonster>(vlst, CObjectFunction::em_GameObject_Type_Monster, FixPtInfo);
	return CFilter::GetInstance().FilterList_By_MonsterFilter(vlst);
}

BOOL CMonsterExtend::GetTopMonster_By_Dis(_In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CMonster* pMonster) CONST
{
	std::vector<CMonster> MonsterVec;
	GetAroundMonsterList(FixPtInfo, MonsterVec);
	return MyTools::CLPublic::Vec_find_if(MonsterVec, pMonster, [](CONST CMonster&) {return TRUE; });
}

UINT CMonsterExtend::GetAroundMonsterCount(_In_ CONST FixPointInfo& FixPtInfo) CONST
{
	std::vector<CMonster> vlst;
	return GetAroundMonsterList(FixPtInfo, vlst);
}

UINT CMonsterExtend::GetAroundTargetMonsterList(_In_ CONST FixPointInfo& FixPtInfo, _Out_ std::vector<CMonster>& vlst) CONST
{
	GetAroundMonsterList(FixPtInfo, vlst);

	CCat Cat;
	CCatExpend::GetInstance().FindSelfCat(&Cat);

	// 将目标ID != 自己的怪物全部删掉!
	DWORD dwSelfId = CPerson::GetInstance().GetId();
	return MyTools::CLPublic::Vec_erase_if(vlst, [&dwSelfId, &Cat](CONST CMonster& Monster)
	{
		if (Monster.GetTargetId() == dwSelfId) // Monster的TarId= 自己的时候不删除
			return FALSE;
		else if (Cat.GetBase() != NULL && Cat.GetId() == Monster.GetTargetId()) // TarId = 猫
			return FALSE;
		return TRUE;
	});
}

BOOL CMonsterExtend::DoAction_By_MonsterName(_In_ CONST std::wstring& wsName, _In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<VOID(CONST CMonster&)> fn) CONST
{
	CMonster Monster;
	if (FindMonster_By_Name(wsName, FixPtInfo, &Monster))
	{
		fn(Monster);
		return TRUE;
	}
	return FALSE;
}

UINT CMonsterExtend::GetAroundMonsterByMaxHp(_In_ DWORD dwMaxHp, _In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ std::vector<CMonster>& vlst) CONST
{
	GetAroundMonsterList(CBaseObject::CreateFixPointInfo(-1), vlst);
	return MyTools::CLPublic::Vec_erase_if(vlst, [&dwMaxHp, &FixPtInfo](CONST CMonster& Monster){return Monster.GetMaxHp() != dwMaxHp || Monster.GetDis() > FixPtInfo.fDis; });
}

UINT CMonsterExtend::GetAroundMonsterListByName(_In_ CONST std::wstring& wsName, _In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ std::vector<CMonster>& vlst) CONST
{
	GetAroundMonsterList(FixPtInfo, vlst);
	return MyTools::CLPublic::Vec_erase_if(vlst, [&wsName](CONST CMonster& itm){return itm.GetName() != wsName; });
}

BOOL CMonsterExtend::ExistMonsterBuff(_In_ DWORD dwMonsterId, _In_ DWORD dwBufferId, _In_ DWORD dwBuffCount) CONST
{
	std::vector<GAMEOBJ_BUFF> VecBuff;
	CObjectFunction::GetInstance().GetMonsterBuffList(VecBuff);

	return MyTools::CLPublic::Vec_find_if_Const(VecBuff, static_cast<GAMEOBJ_BUFF*>(nullptr), [=](CONST GAMEOBJ_BUFF& itm) { return itm.dwBuffId == dwBufferId && itm.dwTargetId == dwMonsterId && itm.dwBuffCount >= dwBuffCount; });
}
