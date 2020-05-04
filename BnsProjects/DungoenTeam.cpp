#include "stdafx.h"
#include "DungoenTeam.h"
#include "SomeFun.h"
#include "ObjectFunction.h"
#include "GameUI.h"
#include "GameSend.h"

BOOL CDungoenTeam::GoToRoom() CONST
{
	if (!IsExistRoom())
	{
		CSomeFun::GetInstance().SimlationKey(VK_F8);
		BnsSleep(2000);
		CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"TotalGateConfirmPanel", [](CONST CGameUi& TotalGateConfirmPanel)
		{
			CGameVariable::GetInstance().PushMainThreadFunPtr_synch([TotalGateConfirmPanel]{ CGameSend::IntoRoom(TotalGateConfirmPanel.GetBase(), CGameSend::em_Room_Type::em_Room_Type_Dungeon); });
		});
		BnsSleep(5 * 1000);
	}

	return TRUE;
}

BOOL CDungoenTeam::IsExistRoom() CONST _NOEXCEPT
{
	return CGameVariable::GetInstance().GetEnvironmentValue() == 5;
}

VOID CDungoenTeam::CreatePass() CONST
{
	//CGameVariable::GetInstance().PushMainThreadFunPtr_synch([]{ CGameSend::CreatePassWorkRoom(); });
}

BOOL CDungoenTeam::FindRoomById(_In_ DWORD dwId) CONST
{
	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([dwId]{ CGameSend::FindDungoenRoom_By_Id(dwId,0); });
	::Sleep(3 * 1000);
	return GetRoomMemberCount() != 1;
}

DWORD CDungoenTeam::GetRoomBase() CONST
{
	return ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(人物基址) + 人物基址偏移1) + 副本房间偏移1) + 副本房间偏移2);
}

DWORD CDungoenTeam::GetRoomMemberCount() CONST
{
	return ReadDWORD(GetRoomBase() + 0x18) & 0xFF;
}

DWORD CDungoenTeam::GetCurrentRoomId() CONST
{
	return ReadDWORD(GetRoomBase() + 0x20);
}

BOOL CDungoenTeam::ReadyFormRoom(_In_ BOOL bCaptain) CONST
{
	if (bCaptain)
	{
		if (!IsReadyForTeamMember())
		{
			MyTools::CCharacter::WriteBYTE(GetRoomBase() + 队长房间准备偏移, 1);
			CGameVariable::GetInstance().PushMainThreadFunPtr_synch([this]{  CGameSend::ReadyForGungoenRoom(GetRoomBase()); });
			return TRUE;
		}
	}
	else
	{
		if (!IsReadyForRoom())
		{
			CGameVariable::GetInstance().PushMainThreadFunPtr_synch([this]{  CGameSend::ReadyForGungoenRoom(GetRoomBase()); });
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CDungoenTeam::IsReadyForRoom() CONST
{
	BOOL bValue = FALSE;
	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&]{ bValue = CGameSend::IsRoomReady(GetRoomBase()); });
	return bValue;
}

BOOL CDungoenTeam::IsReadyForTeamMember() CONST
{
	BOOL bValue = FALSE;
	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&]{ bValue = CGameSend::IsRoomReadyForCaptain(GetRoomBase()); });
	return bValue;
}

BOOL CDungoenTeam::ChoiceMap(_In_ DWORD dwMapId) CONST
{
	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&]{ CGameSend::ChoiceGungeonMap(dwMapId,GetRoomBase()); });
	BnsSleep(2000);
	return ReadDWORD(GetRoomBase() + 0x6C) == dwMapId;
}

BOOL CDungoenTeam::ChoiceDifficulty(_In_ DWORD dwValue) CONST
{
	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&]{ CGameSend::ChoiceGungeonDifficulty(dwValue, GetRoomBase()); });
	BnsSleep(5 * 1000);
	return ReadDWORD(GetRoomBase() + 0x90) == dwValue;
}
