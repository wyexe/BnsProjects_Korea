#include "stdafx.h"
#include "PlayerExtend.h"
#include "ObjectFunction.h"
#include "Player.h"
#include "Person.h"

#define _SELF L"PlayerExtend.cpp"
BOOL CPlayerExtend::FindPlayer_By_Id(_In_ DWORD dwPlayerId, _In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CPlayer* pPlayer) CONST
{
	return FindPlayer_By_Condition(FixPtInfo, pPlayer, [&dwPlayerId](CONST CPlayer& Player){
		return dwPlayerId == Player.GetId();
	});
}

BOOL CPlayerExtend::FindPlayer_By_Name(_In_ CONST std::wstring& wsPlayerName, _In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CPlayer* pPlayer) CONST
{
	return FindPlayer_By_Condition(FixPtInfo, pPlayer, [&wsPlayerName](CONST CPlayer& Player){
		return wsPlayerName == Player.GetName();
	});
}

BOOL CPlayerExtend::FindPlayer_By_Condition(_In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CPlayer* pPlayer, std::function<BOOL(CONST CPlayer&)> f) CONST
{
	return CObjectFunction::GetInstance().FindElement_By_ObjectTraverse<CPlayer>(CObjectFunction::em_GameObject_Type_Player, FixPtInfo, pPlayer, f);
}


BOOL CPlayerExtend::GetTopPlayer_By_Dis(_In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CPlayer* pPlayer) CONST
{
	return FindPlayer_By_Condition(FixPtInfo, pPlayer, [](CONST CPlayer& Player){ return Player.GetId() != CPerson::GetInstance().GetId(); });
}

BOOL CPlayerExtend::GetTopPraticeNpc_By_Dis(_In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CPlayer* pPlayer) CONST
{
	return CObjectFunction::GetInstance().FindElement_By_ObjectTraverse<CPlayer>(CObjectFunction::em_GameObject_Type_PracticePlayer, FixPtInfo, pPlayer, [](CONST CPlayer& Player){
		return Player.GetId() <= 0x10000;
	});
}
