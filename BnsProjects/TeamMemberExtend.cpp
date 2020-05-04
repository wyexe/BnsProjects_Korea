#include "stdafx.h"
#include "TeamMemberExtend.h"
#include <MyTools/Log.h>
#include "ObjectFunction.h"
#include "TeamMember.h"
#include "Person.h"
#include "GameSend.h"
#include "PlayerExtend.h"

#define _SELF L"TeamMemberExtend.cpp"

BOOL CTeamMemberExtend::IsTeamCaptain(__in CONST std::wstring& wsPlayerName) CONST
{
	std::vector<CTeamMember> vlst;
	CObjectFunction::GetInstance().GetTeamList(vlst);

	return FindTeamMember_By_Condiction(nullptr, [&wsPlayerName](_In_ CONST CTeamMember& TeamMember){
		return TeamMember.IsCaptain() && TeamMember.GetName() == wsPlayerName;
	});
}

BOOL CTeamMemberExtend::IsTeamMember_LoseConnect(_In_ CTeamMember* pTeamMember) CONST
{
	return FindTeamMember_By_Condiction(pTeamMember, [](CONST CTeamMember& TeamMember){
		return !TeamMember.IsConnect();
	});
}

BOOL CTeamMemberExtend::ExistTeamMember(__in CONST std::wstring& wsMemberName, _Out_opt_ CTeamMember* pTeamMember) CONST
{
	return FindTeamMember_By_Condiction(pTeamMember, [&wsMemberName](CONST CTeamMember& TeamMember){
		return TeamMember.GetName() == wsMemberName;
	});
}

BOOL CTeamMemberExtend::FindTeamMember_By_Condiction(_In_ CTeamMember* pTeamMember, std::function<BOOL(CONST CTeamMember&)> fnFind) CONST
{
	std::vector<CTeamMember> vlst;
	CObjectFunction::GetInstance().GetTeamList(vlst);

	return MyTools::CLPublic::Vec_find_if(vlst, pTeamMember, fnFind);
}

BOOL CTeamMemberExtend::SetAuctionQuality(em_Auction_Quality emAuctionQuality /*= em_Auction_Quality_Extreme*/) CONST
{
	return GetTeamMemberCount() == 0 ? FALSE : CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&emAuctionQuality]{
		CGameSend::SetAuctionQuality(emAuctionQuality); 
	});
}

BOOL CTeamMemberExtend::LeaveTeam() CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([]{CGameSend::LeaveTeam(); });
}

DWORD CTeamMemberExtend::GetTeamMemberCount() CONST
{
	DWORD dwAddr = ReadDWORD(CPerson::GetInstance().GetBase() + 队伍遍历偏移) + 0x14;
	return (ReadDWORD(dwAddr + 0x4) - ReadDWORD(dwAddr + 0x0)) / 4;
}

BOOL CTeamMemberExtend::IsSameChannel_By_Team() CONST
{
	std::vector<CTeamMember> vlst;
	CObjectFunction::GetInstance().GetTeamList(vlst);

	CTeamMember Captain;
	if (!GetCaptain(&Captain))
	{
		LOG_MSG_CF(L"竟然找不到队长是哪位! 玩笑开大了!");
		return FALSE;
	}

	return !MyTools::CLPublic::Vec_find_if(vlst, static_cast<CTeamMember*>(nullptr), [&Captain](CONST CTeamMember& TeamMember){
		return TeamMember.GetChannel() != Captain.GetChannel();
	});
}

BOOL CTeamMemberExtend::GetCaptain(_Out_opt_ CTeamMember* pCaptain) CONST
{
	std::vector<CTeamMember> vlst;
	CObjectFunction::GetInstance().GetTeamList(vlst);

	return MyTools::CLPublic::Vec_find_if(vlst, pCaptain, [](CONST CTeamMember& TeamMember){
		return TeamMember.IsCaptain();
	});
}

DWORD CTeamMemberExtend::GetCurrentTeamMemberIndex() CONST
{
	CTeamMember TeamMember;
	return ExistTeamMember(CPerson::GetInstance().GetName(), &TeamMember) ? TeamMember.GetIndex() : -1;
}

BOOL CTeamMemberExtend::GetTeamMemberByIndex(_In_ DWORD dwIndex, _Out_opt_ CTeamMember* pTeamMeber) CONST
{
	return FindTeamMember_By_Condiction(pTeamMeber, [&dwIndex](CONST CTeamMember& TeamMember){
		return TeamMember.GetIndex() == dwIndex;
	});
}

BOOL CTeamMemberExtend::ExistTeamMemberWhenHpLessValue(_In_ DWORD dwPercentHp) CONST
{
	return FindTeamMember_By_Condiction(nullptr, [&dwPercentHp](CONST CTeamMember& TeamMember){
		return TeamMember.GetPercentHp() != 0 && TeamMember.GetPercentHp() <= dwPercentHp && TeamMember.GetDis(CPerson::GetInstance().GetPoint()) < 50.0f;;
	});
}

UINT CTeamMemberExtend::GetTeamMemberCountByPoint(_In_ CONST FixPointInfo& FixPoint) CONST
{
	std::vector<CTeamMember> TeamMemberVec;
	CObjectFunction::GetInstance().GetTeamList(TeamMemberVec);

	UINT uCount = 0;
	for (CONST auto& itm : TeamMemberVec)
		uCount = itm.GetDis(FixPoint.FixPoint) <= FixPoint.fDis ? uCount + 1 : uCount;

	return uCount;
}

BOOL CTeamMemberExtend::Action_By_TeamMemberName(_In_ CONST std::wstring& wsPlayerName, _In_ std::function<VOID(CTeamMember&)> Worker) CONST
{
	CTeamMember TeamMember;
	if (!FindTeamMember_By_Condiction(&TeamMember, [&wsPlayerName](CONST CTeamMember& Member){ return Member.GetName() == wsPlayerName; }))
		return FALSE;

	Worker(TeamMember);
	return TRUE;
}

BOOL CTeamMemberExtend::GetTeamMember_By_LessPercentHp(_In_ DWORD dwPercentHp, _Out_opt_ CTeamMember* pTeamMeber) CONST
{
	return FindTeamMember_By_Condiction(pTeamMeber, [&dwPercentHp](CONST CTeamMember& TeamMember){ return TeamMember.GetPercentHp() != 0 && TeamMember.GetPercentHp() <= dwPercentHp && TeamMember.GetDis(CPerson::GetInstance().GetPoint()) < 50.0f; });
}

VOID CTeamMemberExtend::ForEach(_In_ std::function<VOID(CONST CTeamMember& TeamMember)> Worker)
{
	std::vector<CTeamMember> VecTeamMember;
	CObjectFunction::GetInstance().GetTeamList(VecTeamMember);

	std::for_each(VecTeamMember.begin(), VecTeamMember.end(), Worker);
}

BOOL CTeamMemberExtend::IsTryRelive() CONST
{
	std::vector<CTeamMember> VecTeamMember;
	CObjectFunction::GetInstance().GetTeamList(VecTeamMember);

	// 最大队友死亡数量=MaxCount / 2
	UINT uMaxDeadCount = VecTeamMember.size() / 2;

	UINT uDeadCount = 0;
	for (CONST auto& Member : VecTeamMember)
	{
		CPlayer Player;
		if (CPlayerExtend::GetInstance().FindPlayer_By_Name(Member.GetName(), CBaseObject::CreateFixPointInfo(-1), &Player) && Player.IsDead())
		{
			LOG_CF_D(L"玩家遍历:队友[%s] 已经挂了!", Member.GetName().c_str());
			uDeadCount += 1;
		}
		else if (Member.GetPercentHp() == 0)
		{
			LOG_CF_D(L"队伍遍历:队友[%s] 已经挂了!", Member.GetName().c_str());
			uDeadCount += 1;
		}
	}

	LOG_CF_E(L"人物[%s]挂了,死亡队友数量=%d, 最大队友死亡数量=%d",CPerson::GetInstance().GetName().c_str(), uDeadCount, uMaxDeadCount);
	return uDeadCount < uMaxDeadCount;
}

int CTeamMemberExtend::GetCurrentTeamIndexByClasses(_In_ em_Player_Classes emPlayerClasses) CONST
{
	auto p = MyTools::CLPublic::Vec_find_if_Const(VecTeamIndexClasses, [](_In_ CONST TeamIndexClasses& TeamIndexClasses_){ return TeamIndexClasses_.wsPlayerName == CPerson::GetInstance().GetName(); });
	return p == nullptr ? -1 : p->nIndex;
}

VOID CTeamMemberExtend::SetCurrentTeamIndexByClasses()
{
	VecTeamIndexClasses.clear();

	std::vector<CTeamMember> VecTeamMember;
	CObjectFunction::GetInstance().GetTeamList(VecTeamMember);

	std::vector<CPlayer> VecPlayer;
	CObjectFunction::GetInstance().GetSortGameObjectList<CPlayer>(VecPlayer, CObjectFunction::em_GameObject_Type_Player, CBaseObject::CreateFixPointInfo(-1));

	MyTools::CLPublic::Vec_erase_if(VecTeamMember, [VecPlayer](CONST CTeamMember& TeamMember)
	{
		auto pPlayer = MyTools::CLPublic::Vec_find_if_Const(VecPlayer, [TeamMember](CONST CPlayer& Player){ return Player.GetName() == TeamMember.GetName(); });
		if (pPlayer == nullptr)
			return TRUE;

		return pPlayer->GetClasses() != em_Player_Classes::em_Player_Classes_Summoner ? TRUE : FALSE;
	});

	for (CONST auto& itm : VecTeamMember)
		VecTeamIndexClasses.push_back(TeamIndexClasses(itm.GetName(), VecTeamIndexClasses.size()));


	VecTeamMember.clear();
	CObjectFunction::GetInstance().GetTeamList(VecTeamMember);

	MyTools::CLPublic::Vec_erase_if(VecTeamMember, [VecPlayer](CONST CTeamMember& TeamMember)
	{
		auto pPlayer = MyTools::CLPublic::Vec_find_if_Const(VecPlayer, [TeamMember](CONST CPlayer& Player){ return Player.GetName() == TeamMember.GetName(); });
		if (pPlayer == nullptr)
			return TRUE;

		return pPlayer->GetClasses() == em_Player_Classes::em_Player_Classes_Summoner ? TRUE : FALSE;
	});

	auto Index = 0;
	for (CONST auto& itm : VecTeamMember)
		VecTeamIndexClasses.push_back(TeamIndexClasses(itm.GetName(), Index++));
}

UINT CTeamMemberExtend::GetTeamMemberCountByCondition(_In_ std::function<BOOL(CONST CTeamMember&)> Finder) CONST
{
	std::vector<CTeamMember> TeamMemberVec;
	CObjectFunction::GetInstance().GetTeamList(TeamMemberVec);

	UINT uCount = 0;
	for (CONST auto& itm : TeamMemberVec)
		uCount = Finder(itm) ? uCount + 1 : uCount;

	return uCount;
}
