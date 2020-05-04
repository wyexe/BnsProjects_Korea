#include "stdafx.h"
#include "CrossAreaTeamRoom.h"
#include <MyTools/Log.h>
#include <MyTools/TimeTick.h>
#include "SomeFun.h"
#include "ObjectFunction.h"
#include "GameSend.h"
#include "GameUI.h"
#include "PersonFunction.h"
#include "CrossAreaTeamService.h"
#include "BnsGameFun.h"
#include "Person.h"
#include "TeamMemberExtend.h"
#include "ConsoleLog.h"

#define _SELF L"CrossAreaTeamRoom.cpp"
BOOL CCrossAreaTeamRoom::Run(_In_ std::function<std::vector<std::wstring>()> RefreshDailyTaskPtr)
{
	return LoopFarmDungeon(RefreshDailyTaskPtr);
}

BOOL CCrossAreaTeamRoom::LoopFarmDungeon(_In_ std::function<std::vector<std::wstring>()> RefreshDailyTaskPtr)
{
	while (GameRun && (_VecDungeonName = RefreshDailyTaskPtr()).size() != 0)
	{
		// War Status
		if (EnterToMainForm(5 * 60 * 1000))
		{
			LOG_CF_E(L"进入跨服副本房间超时!");
			continue;
		}

		if(!MainFormReadyStep())
			continue;

		if(!CommonDungeonStep())
			continue;

		if(!FarmDungeonStep())
			continue;

		BackToMainFormReadyStep();
	}

	LOG_CF_D(L"离开跨服组队界面!");
	ExitMainForm();
	return TRUE;
}

BOOL CCrossAreaTeamRoom::MainFormReadyStep()
{
	MyTools::CTimeTick TimeTick;
	while (GameRun && (_dwRoomId == NULL || GetRoomMemberCount() != _TeamContent.VecMember.size()))
	{
		if (_dwRoomId == NULL && !CreateTeam_In_Server())
			return FALSE;

		if (!_TeamContent.bCaptain && GetRoomMemberCount() == 1)
		{
			LOG_CF_D(L"进入房间[%d], 队长是[%s:%X]", _RoomContent, _TeamContent.Captain.wsPlayerName.c_str(), _TeamContent.Captain.dwPlayerId);
			EnterToCaptainRoom();
			BnsSleep(5 * 1000);
		}

		// 如果这里假设是1分钟超时的话, 那么应该直接离开房间 还是 离开跨服组队好呢?
		if (TimeTick.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Second) >= 60)
		{
			LOG_CF_D(L"1分钟之内队伍都没凑齐%d个人, 直接离开房间重新排!", _TeamContent.VecMember.size());
			LeaveRoom();
			TimeTick.Reset();
			_dwRoomId = NULL;
		}

		BnsSleep(1000);
	}
	LOG_CF_D(L"房间已经凑齐了%d个人了!", _TeamContent.VecMember.size());

	if (_TeamContent.bCaptain)
	{
		while (GameRun && GetCurrentMapId() != _RoomContent.dwRoomMapIndex)
		{
			LOG_CF_D(L"选择地图[%X]", _RoomContent.dwRoomMapIndex);
			PlayerAction_ChoiceMap();
			BnsSleep(5 * 1000);
		}
		if (GameRun && GetCurrentDifficulty() != _RoomContent.emDifficulty)
		{
			LOG_CF_D(L"选择难度[%X]", static_cast<DWORD>(_RoomContent.emDifficulty));
			PlayerAction_ChoiceDifficulty();
			BnsSleep(5 * 1000);
		}
	}

	CConsoleLog(L"等待所有队员准备完毕!");
	TimeTick.Reset();
	while (GameRun && IsALLPlayerReady())
	{
		// 如果这里假设是1分钟超时的话, 那么应该直接离开房间 还是 离开跨服组队好呢?
		if (GetRoomMemberCount() < _TeamContent.VecMember.size() || TimeTick.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Second) >= 60)
		{
			LOG_CF_D(L"1分钟之内都没准备完毕 或者 人数[%d]少于队伍数量[%d], 直接离开房间重新排!", GetRoomMemberCount(), _TeamContent.VecMember.size());
			LeaveRoom();
			_dwRoomId = NULL;
			return FALSE;
		}
		if (!_TeamContent.bCaptain && !PlayerAttribute_IsMemberReady())
		{
			LOG_CF_D(L"按准备.......");
			PlayerAction_MemberReady();
		}
		::Sleep(5 * 1000);
	}


	TimeTick.Reset();
	CConsoleLog(MyTools::CCharacter::MakeFormatText(L"开始! 移动到跨服副本[%s]", _TeamContent.wsCurrentDungeonName.c_str()));
	while (GameRun && !PlayerAttribute_IsInCommonDungeon())
	{
		if(GetRoomMemberCount() < _TeamContent.VecMember.size() || TimeTick.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Second) >= 3 * 60)
		{
			LOG_CF_D(L"3分钟之内都没移动到公共副本 或者 人数[%d]少于队伍数量[%d], 直接离开房间重新排!", GetRoomMemberCount(), _TeamContent.VecMember.size());
			LeaveRoom();
			_dwRoomId = NULL;
			return FALSE;
		}

		CPersonFunction::GetInstance().WaitGameLoad();
		if (_TeamContent.bCaptain)
		{
			LOG_CF_D(L"队长点开始!");
			PlayerAction_Captain_MoveToDungeon();
			BnsSleep(10 * 1000);
		}

		BnsSleep(5 * 1000);
	}

	return TRUE;
}

BOOL CCrossAreaTeamRoom::CommonDungeonStep()
{
	// 执行脚本函数的CommonDungeonStep()
	return TRUE;
}

BOOL CCrossAreaTeamRoom::FarmDungeonStep()
{
	// 调用脚本函数的FarmDungeonStep
	return TRUE;
}

BOOL CCrossAreaTeamRoom::BackToMainFormReadyStep()
{
	while (GameRun && IsInMainForm())
	{
		// 调用脚本函数的BackToMainFormReadyStep

		if (CTeamMemberExtend::GetInstance().GetTeamMemberCount() == 0)
		{
			CGameVariable::GetInstance().PushMainThreadFunPtr_synch([] {CGameSend::BackToCrossAreaTeamMainForm(); });
			BnsSleep(10 * 1000);
			CPersonFunction::GetInstance().WaitGameLoad();
		}
		else
		{
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(60 * 1000, [=] 
			{
				CPersonFunction::GetInstance().WaitGameLoad();
				BnsSleep(1000);
				return CPersonFunction::GetInstance().IsGameLoad() || !IsInMainForm(); 
			});
		}
	}
	
	LOG_CF_D(L"成功返回跨服组队准备界面!");
	return TRUE;
}

BOOL CCrossAreaTeamRoom::CreateTeam_In_Server()
{
	CCrossAreaTeamService Service;
	CCrossAreaTeamService::CrossAreaTeamParam Param;
	CrossAreaTeamGroup Result;

	Param.VecDungeonName = _VecDungeonName;
	Param.dwRoomId = GetCurrentRoomId();
	if (!Service.Run(std::move(Param), Result))
		return FALSE;

	_dwRoomId = Result.Captain.dwRoomId;
	_RoomContent.emDifficulty = Result.emDifficulty;
	_RoomContent.dwRoomMapIndex = Result.dwDungeonMapIndex;

	_TeamContent.bCaptain = Result.Captain.Info.dwPlayerId == CPerson::GetInstance().GetId();
	_TeamContent.Captain = Result.Captain.Info;
	_TeamContent.VecMember = VecCrossAreaTeamToClientTeamMember(Result.VecMember);
	_TeamContent.wsCurrentDungeonName = Result.wsDungeonName;
	return TRUE;
}

BOOL CCrossAreaTeamRoom::EnterToMainForm(_In_ DWORD dwTimeout) CONST
{
	return IsInMainForm() ? TRUE : CSomeFun::GetInstance().TimeOut_Condiction_GameRun(dwTimeout, [=]
	{
		CSomeFun::GetInstance().SimlationKey(VK_F8);
		BnsSleep(2000);
		CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"TotalGateConfirmPanel", [](CONST CGameUi& TotalGateConfirmPanel)
		{
			CGameVariable::GetInstance().PushMainThreadFunPtr_synch([TotalGateConfirmPanel] { CGameSend::IntoRoom(TotalGateConfirmPanel.GetBase(), CGameSend::em_Room_Type::em_Room_Type_Dungeon); });
		});
		BnsSleep(5 * 1000);
		CPersonFunction::GetInstance().WaitGameLoad();
		return !IsInMainForm();
	});
}

BOOL CCrossAreaTeamRoom::IsInMainForm() CONST
{
	return CGameVariable::GetInstance().GetEnvironmentValue() == 4;
}

BOOL CCrossAreaTeamRoom::IsALLPlayerReady() CONST
{
	BOOL bValue = FALSE;
	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&] { bValue = CGameSend::IsRoomReadyForCaptain(GetRoomBase()); });
	return bValue;
}

BOOL CCrossAreaTeamRoom::PlayerAttribute_IsMemberReady() CONST
{
	BOOL bValue = FALSE;
	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&] { bValue = CGameSend::IsRoomReadyForCaptain(GetRoomBase()); });
	return bValue;
}

BOOL CCrossAreaTeamRoom::PlayerAttribute_IsInCommonDungeon() CONST
{
	return CGameVariable::GetInstance().GetEnvironmentValue() == 0x3 && CPerson::GetInstance().GetMapId() == em_MapId_CommonDungeon;
}

VOID CCrossAreaTeamRoom::PlayerAction_MemberReady() CONST
{
	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([this] {  CGameSend::ReadyForGungoenRoom(GetRoomBase()); });
}

VOID CCrossAreaTeamRoom::PlayerAction_Captain_MoveToDungeon() CONST
{
	MyTools::CCharacter::WriteBYTE(GetRoomBase() + 队长房间准备偏移, 1);
	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([=] {CGameSend::ReadyForGungoenRoom(GetRoomBase()); });
}

VOID CCrossAreaTeamRoom::PlayerAction_ChoiceMap() CONST
{
	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([=] { CGameSend::ChoiceGungeonMap(_RoomContent.dwRoomMapIndex, GetRoomBase()); });
}

VOID CCrossAreaTeamRoom::PlayerAction_ChoiceDifficulty() CONST
{
	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([=] { CGameSend::ChoiceGungeonDifficulty(static_cast<DWORD>(_RoomContent.emDifficulty), GetRoomBase()); });
}

DWORD CCrossAreaTeamRoom::GetRoomMemberCount() CONST
{
	return ReadDWORD(GetRoomBase() + 0x18) & 0xFF;
}

DWORD CCrossAreaTeamRoom::GetCurrentRoomId() CONST
{
	return ReadDWORD(GetRoomBase() + 0x20);
}

DWORD CCrossAreaTeamRoom::GetCurrentMapId() CONST
{
	return ReadDWORD(GetRoomBase() + 0x6C);
}

em_CrossAreaTeam_Difficulty CCrossAreaTeamRoom::GetCurrentDifficulty() CONST
{
	return static_cast<em_CrossAreaTeam_Difficulty>(ReadDWORD(GetRoomBase() + 0x90));
}

DWORD CCrossAreaTeamRoom::GetRoomBase() CONST
{
	return ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(人物基址) + 人物基址偏移1) + 副本房间偏移1) + 副本房间偏移2);
}

VOID CCrossAreaTeamRoom::EnterToCaptainRoom() CONST
{
	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&] { CGameSend::FindDungoenRoom_By_Id(_dwRoomId, 0); });
}

std::vector<ClientTeamMember> CCrossAreaTeamRoom::VecCrossAreaTeamToClientTeamMember(_In_ CONST std::vector<CrossAreaTeamMember>& Vec)
{
	std::vector<ClientTeamMember> VecMember;
	for (auto& itm : Vec)
		VecMember.push_back(itm.Info);

	return VecMember;
}

VOID CCrossAreaTeamRoom::LeaveRoom() CONST
{
	while (GameRun && GetRoomMemberCount() > 1)
	{
		CGameVariable::GetInstance().PushMainThreadFunPtr_synch([] {CGameSend::ExitCrossAreaTeamRoom(); });
		BnsSleep(5 * 1000);
	}
}

VOID CCrossAreaTeamRoom::ExitMainForm() CONST
{
	while (GameRun && IsInMainForm())
	{
		CGameVariable::GetInstance().PushMainThreadFunPtr_synch([] {CGameSend::ExitCrossAreaTeamMainForm(); });
		BnsSleep(10 * 1000);
		CPersonFunction::GetInstance().WaitGameLoad();
	}
}
