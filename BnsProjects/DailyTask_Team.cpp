#include "stdafx.h"
#include "DailyTask_Team.h"
#include <MyTools/Log.h>
#include <MyTools/TimeTick.h>
#include <MyTools/CLFile.h>
#include "../BnsKorServer_IOCP/ServerCommon.h"
#include "Person.h"
#include "TeamMember.h"
#include "TeamMemberExtend.h"
#include "SomeFun.h"
#include "ObjectFunction.h"
#include "GameUI.h"
#include "PersonFunction.h"
#include "Farm.h"
#include "FarmSkill.h"

#define _SELF L"DailyTask_Team.cpp"
CDailyTask_Team::CDailyTask_Team() : _pTeamClumpContent(nullptr)
{

}

VOID CDailyTask_Team::CreateTeam()
{
	// local test
	InitLocalTeam();
	if (CPerson::GetInstance().GetId() == _pTeamClumpContent->ArrMember[0].dwPlayerId)
		CreateTeam_Captain();
	else
		CreateTeam_Member();

	for (UINT i = 0; i < _pTeamClumpContent->uCount; ++i)
	{
		_pTeamClumpContent->ArrMember[i].uScheduleIndex = 0;
		MyTools::CCharacter::wstrcpy_my(_pTeamClumpContent->ArrMember[i].wszScheduleName, L"组队完毕!");
	}
}

VOID CDailyTask_Team::ClumpTeamMember(_In_ UINT uScheduleIndex, _In_ CONST std::wstring& wsScheduleName, _In_ CONST Point& ClumpPoint)
{
	BnsMove(ClumpPoint, 1);

	// local test
	auto pMaxMemberClumpContent = FindMaxScheduleMember();
	if (pMaxMemberClumpContent->uScheduleIndex > uScheduleIndex)
	{
		LOG_CF_D(L"最大进度的已经到了[%s,%s,%d]去了,现在进度只是:%d, break;",
			pMaxMemberClumpContent->wszAccountName, 
			pMaxMemberClumpContent->wszScheduleName, 
			pMaxMemberClumpContent->uScheduleIndex, 
			uScheduleIndex);

		return;
	}

	MyTools::CCharacter::wstrcpy_my(_pCurrentMemberClumpContent->wszScheduleName, wsScheduleName.c_str());
	_pCurrentMemberClumpContent->uScheduleIndex = uScheduleIndex;

	LOG_CF_D(L"等待其他人的进度……");

	MyTools::CTimeTick TimeTick;
	while (GameRun && !EMPTY_PERSONHP)
	{
		CFarm::GetInstance().ClearAround_TarIdSelf(CBaseObject::CreateFixPointInfo(16));

		if (TimeTick.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Minute) >= 5)
		{
			LOG_MSG_CF(L"5分钟队伍集合超时!");
			continue;
		}

		// Refresh Data
		pMaxMemberClumpContent = FindMaxScheduleMember();

		BOOL bSame = TRUE;
		DWORD dwLoseTeamMemberCount = 0;
		for (UINT i = 0; i < _pTeamClumpContent->uCount; ++i)
		{
			CONST auto& itm = _pTeamClumpContent->ArrMember[i];
			BOOL bLose = FALSE;
			CTeamMemberExtend::GetInstance().Action_By_TeamMemberName(itm.wszPlayerName, [&bLose](CTeamMember& Member) { bLose = !Member.IsConnect(); });
			if (bLose)
			{
				LOG_CF_D(L"队友:[%s,%s] 已经掉线了, 忽略他!", itm.wszAccountName, itm.wszPlayerName);
				dwLoseTeamMemberCount += 1;
				continue;
			}

			if (itm.uScheduleIndex == NULL)
			{
				LOG_CF_D(L"队友:[%s,%s] 还没初始化副本进度……等!", itm.wszAccountName, itm.wszPlayerName);
				bSame = FALSE;
				break;
			}

			if (itm.uScheduleIndex < pMaxMemberClumpContent->uScheduleIndex)
			{
				LOG_CF_D(L"当前最新的进度是:[%s,%X], 队友:[%s,%s] 的进度是:[%s,%X], 等!", 
					pMaxMemberClumpContent->wszScheduleName,
					pMaxMemberClumpContent->uScheduleIndex, 
					itm.wszAccountName, 
					itm.wszPlayerName, 
					itm.wszScheduleName, 
					itm.uScheduleIndex);

				bSame = FALSE;
				break;
			}
		}

		if (CTeamMemberExtend::GetInstance().GetTeamMemberCount() - dwLoseTeamMemberCount < 6)
		{
			LOG_MSG_CF(L"当前队伍在线人数=%d人, 至少需要%d个人集合. 没法玩了. 退队包平安!", CTeamMemberExtend::GetInstance().GetTeamMemberCount() - dwLoseTeamMemberCount, 6);
			// return 0 让下面的代码不继续执行了!
			return;
		}


		if (bSame)
		{
			LOG_CF_D(L"所有人的进度都一致了,继续往下跑!");
			break;
		}

		// 所有人进度不一样之前, 先等着!
		BnsSleep(2000);
	}
}

VOID CDailyTask_Team::ReviveTeamMember()
{
	LOG_CF_D(L"当前人物HP=%d", CPerson::GetInstance().GetPercentHp());
	while (GameRun)
	{
		// 是不是所有人都死光了
		if (CTeamMemberExtend::GetInstance().GetTeamMemberCountByCondition([](CONST CTeamMember& TeamMember) { return TeamMember.GetPercentHp() == NULL; }) == CTeamMemberExtend::GetInstance().GetTeamMemberCount())
		{
			LOG_CF_D(L"所有队友都死光了!");
			break;
		}

		if (EMPTY_PERSONHP)
		{
			BnsSleep(1000);
			continue;
		}

		// 判断在线的队伍人数必须全部复活!
		CTeamMember TeamMeber;
		BOOL bExistDeadMember = CTeamMemberExtend::GetInstance().FindTeamMember_By_Condiction(&TeamMeber, [](CONST CTeamMember& TeamMember)
		{
			LOG_CF_D(L"队友[%s] HP=%d", TeamMember.GetName().c_str(), TeamMember.GetPercentHp());
			return TeamMember.IsConnect() && TeamMember.GetPercentHp() == NULL && TeamMember.GetDis(CPerson::GetInstance().GetPoint()) < 50.0f && TeamMember.GetName() != CPerson::GetInstance().GetName();
		});

		if (!bExistDeadMember)
		{
			LOG_CF_D(L"所有队友都是已经存活的状态! break;");
			break;
		}

		// 没死的去帮队友复活
		LOG_CF_D(L"跑去复活队友:%s, SelfHP=%d", TeamMeber.GetName().c_str(), CPerson::GetInstance().GetPercentHp());
		CPersonFunction::GetInstance().MoveToPoint(TeamMeber.GetPoint(), 2, 50);
		BnsSleep(2 * 1000);

		if(CPerson::GetInstance().GetClasses() == em_Player_Classes::em_Player_Classes_Summoner)
			CFarmSkill::GetInstance().UseSkill_By_SkillId(Sum_SkillId_Relive, em_Skill_Type::em_Skill_Type_Cat);

		CFarmSkill::GetInstance().UseSkill_By_SkillId(SKILLID_RELIVE);
		BnsSleep(3 * 1000);
		CPersonFunction::GetInstance().WaitForProgressBar(20 * 1000);
	}
}

VOID CDailyTask_Team::SetSchedule(_In_ UINT uScheduleIndex, _In_ CONST std::wstring& wsScheduleName)
{
	InitLocalTeam();
	_pCurrentMemberClumpContent->uScheduleIndex = uScheduleIndex;
	MyTools::CCharacter::wstrcpy_my(_pCurrentMemberClumpContent->wszScheduleName, wsScheduleName.c_str());
}

VOID CDailyTask_Team::InitLocalTeam()
{
	if (_pTeamClumpContent == nullptr)
	{
		HANDLE hMutex = ::OpenMutexW(MUTEX_ALL_ACCESS, FALSE, L"CL_LOCAL_BNS_TEAM");
		if (hMutex == NULL)
			hMutex = ::CreateMutexW(NULL, FALSE, L"CL_LOCAL_BNS_TEAM");
		else
			::WaitForSingleObject(hMutex, 10 * 1000);


		LOG_CF_D(L"InitLocalTeam");
		HANDLE hFileMap = ::OpenFileMappingW(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, SZFILE_NAME_TEAM_SHAREINFO);
		if (hFileMap == NULL)
		{
			hFileMap = ::CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, NULL, sizeof(Login_Share_Info), SZFILE_NAME_TEAM_SHAREINFO);
			_pTeamClumpContent = (TeamClumpContent *)MapViewOfFile(hFileMap, FILE_MAP_READ | FILE_SHARE_WRITE, NULL, NULL, sizeof(TeamClumpContent));
			ZeroMemory(_pTeamClumpContent, sizeof(TeamClumpContent));
		}
		else
		{
			_pTeamClumpContent = (TeamClumpContent *)MapViewOfFile(hFileMap, FILE_MAP_READ | FILE_SHARE_WRITE, NULL, NULL, sizeof(TeamClumpContent));
		}

		// 判断是否存在该角色了
		for (UINT i = 0; i < _pTeamClumpContent->uCount; ++i)
		{
			if (std::wstring(_pTeamClumpContent->ArrMember[i].wszPlayerName) == CPerson::GetInstance().GetName())
			{
				_pCurrentMemberClumpContent = &_pTeamClumpContent->ArrMember[i];
				_pCurrentMemberClumpContent->dwPlayerId = CPerson::GetInstance().GetId();
				::ReleaseMutex(hMutex);
				return;
			}
		}

		_pCurrentMemberClumpContent = &_pTeamClumpContent->ArrMember[_pTeamClumpContent->uCount];
		ZeroMemory(_pCurrentMemberClumpContent, sizeof(TeamMemberClumpContent));
		_pCurrentMemberClumpContent->uIndex = _pTeamClumpContent->uCount;
		_pTeamClumpContent->uCount += 1;

		_pCurrentMemberClumpContent->dwPlayerId = CPerson::GetInstance().GetId();
		MyTools::CCharacter::wstrcpy_my(_pCurrentMemberClumpContent->wszPlayerName, CPerson::GetInstance().GetName().c_str());
		MyTools::CCharacter::wstrcpy_my(_pCurrentMemberClumpContent->wszAccountName, CGameVariable::GetInstance().GetAccShareInfo()->GetAccName());
		::ReleaseMutex(hMutex);
	}
}

VOID CDailyTask_Team::CreateTeam_Captain()
{
	LOG_C_D(L"我是队长!");
	// 队长
	std::map<DWORD, ULONGLONG> MapDisableInvite;
	while (GameRun && !EMPTY_PERSONHP && CTeamMemberExtend::GetInstance().GetTeamMemberCount() != 6)
	{
		for (UINT i = 1; i < _pTeamClumpContent->uCount; ++i)
		{
			auto itr = MapDisableInvite.find(_pTeamClumpContent->ArrMember[i].dwPlayerId);
			if (itr != MapDisableInvite.end())
			{
				if (::GetTickCount64() - itr->second >= 15 * 1000)
					MapDisableInvite.erase(itr);
				else
					continue;
			}
			if (CTeamMemberExtend::GetInstance().FindTeamMember_By_Condiction(nullptr, [this, i](CONST CTeamMember& TeamMember) { return TeamMember.GetId() == _pTeamClumpContent->ArrMember[i].dwPlayerId; }))
			{
				continue;
			}

			LOG_C_D(L"邀请队员加入队伍:%X", _pTeamClumpContent->ArrMember[i].dwPlayerId);
			CPlayer::InviteTeamMember(_pTeamClumpContent->ArrMember[i].dwPlayerId);
			MapDisableInvite.insert(std::make_pair(_pTeamClumpContent->ArrMember[i].dwPlayerId, ::GetTickCount64()));
			BnsSleep(500);
		}
		BnsSleep(1000);
	}
}

VOID CDailyTask_Team::CreateTeam_Member()
{
	LOG_C_D(L"我队员!");
	// 队员
	while (GameRun && !EMPTY_PERSONHP && CTeamMemberExtend::GetInstance().GetTeamMemberCount() != 6)
	{
		if (CTeamMemberExtend::GetInstance().GetTeamMemberCount() != 0)
		{
			BnsSleep(500);
			continue;
		}

		// 是否有人邀请!
		CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"InvitedPartyConfirmPanel", [this](CONST CGameUi& InvitedPartyConfirmPanel)
		{
			if (ReadDWORD(InvitedPartyConfirmPanel.GetObj() + 队伍邀请队长ID偏移) != _pTeamClumpContent->ArrMember[0].dwPlayerId)
			{
				CSomeFun::GetInstance().SimlationKey('N', 200, 800);
				return;
			}

			LOG_CF_D(L"接受队长的邀请!");
			InvitedPartyConfirmPanel.AcceptTeamInvite();
			BnsSleep(2000);
		});
		BnsSleep(1000);
	}
}

CDailyTask_Team::TeamMemberClumpContent* CDailyTask_Team::FindMaxScheduleMember()
{
	TeamMemberClumpContent* pMemberClumpContent = &_pTeamClumpContent->ArrMember[0];

	for (UINT i = 1;i < _pTeamClumpContent->uCount; ++i)
		if (_pTeamClumpContent->ArrMember[i].uScheduleIndex > pMemberClumpContent->uScheduleIndex)
			pMemberClumpContent = &_pTeamClumpContent->ArrMember[i];

	return pMemberClumpContent;
}