#include "stdafx.h"
#include "CrossAreaTeamService.h"
#include <MyTools/Log.h>
#include <MyTools/Character.h>
#include "SomeFun.h"
#include "Person.h"
#include "BnsGameFun.h"
#include "TeamMemberExtend.h"
#include "ConsoleLog.h"

#define _SELF L"CrossAreaTeamService.cpp"
BOOL CCrossAreaTeamService::Run(_In_ CONST CrossAreaTeamParam& Param, _Out_ CrossAreaTeamGroup& Result)
{
	auto ulTick = ::GetTickCount64();
	DWORD dwTeamId = 0;

	// 
	LeaveTeam(NULL);

	// Fill
	CrossAreaTeamMember MemberContent;
	FillCrossAreaTeamMember(Param.VecDungeonName, MemberContent);


	BOOL bSetTeamContent = FALSE;
	while (GameRun)
	{
		CConsoleLog(MyTools::CCharacter::MakeFormatText(L"跨服组队耗费%d秒", static_cast<DWORD>(::GetTickCount64() - ulTick) / 1000));
		if (!bSetTeamContent)
		{
			LOG_CF_D(L"设置跨服组队Detail");
			bSetTeamContent = _SocketAPI.SetCrossAreaTeamContent(MemberContent);
			BnsSleep(5 * 1000);
			continue;
		}
		if (dwTeamId != NULL)
		{
			auto ResultType = _SocketAPI.GetTeamGroupContent(dwTeamId, Result);
			switch (ResultType)
			{
			case CSocketAPI::em_CrossAreaTeam_ResultType::Succ:
				CConsoleLog(L"从服务器上跨服组队成功~ 准备进入跨服副本!");
				return TRUE;
			case CSocketAPI::em_CrossAreaTeam_ResultType::Undefine:
				LOG_CF_E(L"队伍ID[%X]的队伍已经被解散了, 重新组队~");
				dwTeamId = NULL;
				break;
			case CSocketAPI::em_CrossAreaTeam_ResultType::Timeout:
				CConsoleLog(L"请求获取跨服队伍内容超时");
				break;
			default:
				break;
			}
			continue;
		}

		dwTeamId = _SocketAPI.JoinCrossAreaTeam();
		if (dwTeamId != NULL)
			continue;

		BnsSleep(5 * 1000);
	}

	return FALSE;
}

VOID CCrossAreaTeamService::LeaveTeam(_In_ DWORD dwTeamId) CONST
{
	_SocketAPI.LeaveCrossAreaTeam(dwTeamId);
}

VOID CCrossAreaTeamService::FillCrossAreaTeamMember(_In_ CONST std::vector<std::wstring>& VecDungeonName, _Out_ CrossAreaTeamMember& Member) CONST
{
	Member.emClasses = static_cast<em_TeamMeber_Classes>(CPerson::GetInstance().GetClasses());
	Member.Info.dwPlayerId = CPerson::GetInstance().GetId();
	Member.wsGroupTitle = std::wstring(CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.wszGroupTitle);
	
	for (CONST auto& itm : VecDungeonName)
		Member.SetDungoenName.insert(itm);
}
