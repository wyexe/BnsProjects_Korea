#include "stdafx.h"
#include "LoginPlayer.h"
#include "BnsConfig.h"

#define _SELF L"LoginPlayer.cpp"
CLoginPlayer::CLoginPlayer(_In_ DWORD dwBase_, _In_ UINT uIndex_) : bFinishMainTask(FALSE)
{
	dwBase = dwBase_;
	uIndex = uIndex_;
}

CLoginPlayer::CLoginPlayer() : bFinishMainTask(FALSE)
{
	dwBase = NULL;
	uIndex = NULL;
}

CLoginPlayer::~CLoginPlayer()
{
}

DWORD CLoginPlayer::GetBase() CONST
{
	return dwBase;
}

CONST std::wstring CLoginPlayer::GetName() CONST
{
	WCHAR wszName[MAX_OBJECT_NAME_LEN] = { 0 };
	if (ReadDWORD(GetBase() + 0x14 + 0x14) == 0xF)
	{
		if (ReadDWORD(ReadDWORD(GetBase() + 0x14)) != NULL)
			MyTools::CCharacter::wstrcpy_my(wszName, (LPCWSTR)ReadDWORD(GetBase() + 0x14), _countof(wszName) - 1);
	}
	else
	{
		if (ReadDWORD(GetBase() + 0x14) != NULL)
			MyTools::CCharacter::wstrcpy_my(wszName, (LPCWSTR)(GetBase() + 0x14), _countof(wszName) - 1);
	}

	return std::wstring(wszName);
}

DWORD CLoginPlayer::GetLevel() CONST
{
	return ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(人物基址) + 人物基址偏移1 - 0x4) + 0x10) + uIndex * 4) + 0x12E) & 0xFF;
}

BOOL CLoginPlayer::IsDeleteStatus() CONST
{
	return ReadBYTE(GetBase() + 0x2E) == 0 ? FALSE : TRUE;
}

DWORD CLoginPlayer::GetServerId() CONST
{
	return static_cast<DWORD>(ReadBYTE(GetBase() + 0x2C));
}

em_Player_Classes CLoginPlayer::GetLoginPlayerClasses() CONST
{
	return (em_Player_Classes)ReadBYTE(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(人物基址) + 人物基址偏移1 - 0x4) + 0x10) + uIndex * 4) + 0x2);
}

DWORD CLoginPlayer::GetIndex() CONST
{
	return uIndex;
}

BOOL CLoginPlayer::GetFinishMainTask() CONST
{
	return bFinishMainTask;
}

VOID CLoginPlayer::SetFinishMainTask(_In_ BOOL bFinishMainTask_)
{
	bFinishMainTask = bFinishMainTask_;
}
