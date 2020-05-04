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
		LOG_CF_E(L"���[%s] ��������������Ϣ", wsPlayerName.c_str());
		return;
	}

	LOG_CF_D(L"���[%s]�Ľ��=[%d], �ȼ�[%d], ����[%s]", wsPlayerName.c_str(), Content.Gold, static_cast<DWORD>(Content.Level), Content.wsContent.c_str());
	if (Content.dwTaskId != NULL)
	{
		std::wstring wsCapter;
		if (!CScriptFile::GetInstance().FindCapterByTaskId(Content.dwTaskId, wsCapter))
			LOG_CF_D(L"����[%X,%X]������[%s]", Content.dwTaskId, Content.dwScheduleId, wsCapter.c_str());
		else
			LOG_CF_D(L"����[%X,%X]�����ڷ������ϵ������б�!", Content.dwTaskId, Content.dwScheduleId);
	}
}

VOID COnLineOutput::PrintCardByPlayerName(_In_ CONST std::wstring& wsPlayerName) CONST
{
	CardInfo Card;
	if (!CCardExtend::GetInstance().FindCard_By_Condition(Card, [wsPlayerName](CONST CardInfo& Card) { return Card.wsPlayerName == wsPlayerName; }))
	{
		LOG_CF_E(L"���[%s] �������ڿ�����Ϣ", wsPlayerName.c_str());
		return;
	}

	PrintCardContent(Card);
}

VOID COnLineOutput::PrintCardByAccountName(_In_ CONST std::wstring& wsAccountName, _Out_ std::wstring* pwsPlayerName) CONST
{
	CardInfo Card;
	if (!CCardExtend::GetInstance().FindCard_By_Condition(Card, [wsAccountName](CONST CardInfo& Card) { return Card.wsAccountName == wsAccountName; }))
	{
		LOG_CF_E(L"���[%s] �������ڿ�����Ϣ",wsAccountName.c_str());
		return;
	}

	MyTools::CLPublic::SetPtr(pwsPlayerName, Card.wsPlayerName);
	PrintCardContent(Card);
}

VOID COnLineOutput::PrintCardContent(_In_ CONST CardInfo& Card) CONST
{
	LOG_CF_D(L"���[%s]�Ŀ���[%s], ʣ��ʱ��[%d]Сʱ, �ʺ�[%s], ��¼IP[%s]",
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
		LOG_CF_E(L"�����ڶ�Ա wsAccountName = %s", wsAccountName.c_str());
		return;
	}


	LOG_CF_D(L"Ƶ��[%d], ����ID[%X,%X],������ID[%d],����ID[%X],ֻ��[%d],����������[%d],��ǰ��������[%d],������[%s]", 
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
		LOG_CF_D(L"���[%s], �ʺ�[%s], ��ɫ��[%s], ID[%X], ����[%s], ����[%d]", 
			itm.bCaptain ? L"�ӳ�" : L"��Ա", 
			itm.Member.wsAccountName.c_str(), 
			itm.Member.wsPlayerName.c_str(), 
			itm.Member.dwPlayerId, 
			itm.Dungoen.wsDungoenSchedule.c_str(), 
			itm.Dungoen.uGatherIndex);
	}
}
