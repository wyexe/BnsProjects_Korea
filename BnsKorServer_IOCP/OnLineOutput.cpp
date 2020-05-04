#include "OnLineOutput.h"
#include <MyTools/Log.h>
#include <MyTools/CLPublic.h>
#include "PlayerOnLine.h"
#include "ScriptFile.h"
#include "CardExtend.h"
#include "BnsTeam.h"

#define _SELF L"OnLineOutput.cpp"
VOID COnLineOutput::PrintPlayerKeepALiveContent(_In_ CONST std::wstring& wsPlayerName) CONST
{
	KeepALiveContent Content;
	if (!CPlayerOnLine::GetInstance().GetKeepALiveContent(wsPlayerName, Content))
	{
		LOG_CF_E(L"玩家[%s] 还不存在心跳信息", wsPlayerName.c_str());
		return;
	}

	LOG_CF_D(L"玩家[%s]的金币=[%d], 等级[%d], 内容[%s]", wsPlayerName.c_str(), Content.Gold, static_cast<DWORD>(Content.Level), Content.wsContent.c_str());
	if (Content.dwTaskId != NULL)
	{
		std::wstring wsCapter;
		if (!CScriptFile::GetInstance().FindCapterByTaskId(Content.dwTaskId, wsCapter))
			LOG_CF_D(L"任务[%X,%X]是属于[%s]", Content.dwTaskId, Content.dwScheduleId, wsCapter.c_str());
		else
			LOG_CF_D(L"任务[%X,%X]不属于服务器上的任务列表!", Content.dwTaskId, Content.dwScheduleId);
	}
}

VOID COnLineOutput::PrintCardByPlayerName(_In_ CONST std::wstring& wsPlayerName) CONST
{
	CardInfo Card;
	if (!CCardExtend::GetInstance().FindCard_By_Condition(Card, [wsPlayerName](CONST CardInfo& Card) { return Card.wsPlayerName == wsPlayerName; }))
	{
		LOG_CF_E(L"玩家[%s] 还不存在卡号信息", wsPlayerName.c_str());
		return;
	}

	PrintCardContent(Card);
}

VOID COnLineOutput::PrintCardByAccountName(_In_ CONST std::wstring& wsAccountName, _Out_ std::wstring* pwsPlayerName) CONST
{
	CardInfo Card;
	if (!CCardExtend::GetInstance().FindCard_By_Condition(Card, [wsAccountName](CONST CardInfo& Card) { return Card.wsAccountName == wsAccountName; }))
	{
		LOG_CF_E(L"玩家[%s] 还不存在卡号信息",wsAccountName.c_str());
		return;
	}

	MyTools::CLPublic::SetPtr(pwsPlayerName, Card.wsPlayerName);
	PrintCardContent(Card);
}

VOID COnLineOutput::PrintCardContent(_In_ CONST CardInfo& Card) CONST
{
	LOG_CF_D(L"玩家[%s]的卡号[%s], 剩余时间[%d]小时, 帐号[%s], 登录IP[%s]",
		Card.wsPlayerName.c_str(),
		Card.GetSurplusHour(),
		Card.wsAccountName.c_str(),
		Card.wsIp.c_str());
}

VOID COnLineOutput::PrintTeamByAccount(_In_ CONST std::wstring& wsAccountName) CONST
{
	TeamMemberGroup Group_;
	if (!CBnsTaskTeam::GetInstance().FindTeamGroupByAccountName(wsAccountName, Group_))
	{
		LOG_CF_E(L"不存在队员 wsAccountName = %s", wsAccountName.c_str());
		return;
	}


	LOG_CF_D(L"频道[%d], 任务ID[%X,%X],服务器ID[%d],队伍ID[%X],只读[%d],最大队友上限[%d],当前队友数量[%d],副本名[%s]", 
		Group_.dwChannelId,
		Group_.dwTaskId, 
		Group_.dwScheduleId, 
		Group_.dwServerId, 
		Group_.dwTeamId, 
		Group_.IsReadOnly, 
		Group_.uMaxMemberCount, 
		Group_.MemberVec.size(), 
		Group_.wsDungoenName.c_str());

	for (CONST auto& itm : Group_.MemberVec)
	{
		LOG_CF_D(L"身份[%s], 帐号[%s], 角色名[%s], ID[%X], 进度[%s], 进度[%d]", 
			itm.bCaptain ? L"队长" : L"队员", 
			itm.Member.wsAccountName.c_str(), 
			itm.Member.wsPlayerName.c_str(), 
			itm.Member.dwPlayerId, 
			itm.Dungoen.wsDungoenSchedule.c_str(), 
			itm.Dungoen.uGatherIndex);
	}
}
