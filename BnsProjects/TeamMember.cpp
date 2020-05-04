#include "stdafx.h"
#include "TeamMember.h"
#include "SomeFun.h"
#include "GameSend.h"


CTeamMember::CTeamMember(DWORD dwNodeBase, DWORD dwIndex_) : CBaseObject(dwNodeBase), dwIndex(dwIndex_)
{
	
}

CTeamMember::CTeamMember()
{

}

CTeamMember::~CTeamMember()
{
}

CONST std::wstring& CTeamMember::GetName() CONST
{
	if (Name_.empty())
	{
		SetName();
	}
	return Name_;
}

VOID CTeamMember::SetName() CONST
{
	WCHAR wszName[MAX_OBJECT_NAME_LEN] = { 0 };
	if (ReadDWORD(GetBase() + 0x94 + 0x14) >= 0xF)
	{
		if (ReadDWORD(GetBase() + 0x94) != NULL && ReadDWORD(ReadDWORD(GetBase() + 0x94)) != NULL)
		{
			MyTools::CCharacter::wstrcpy_my(wszName, reinterpret_cast<LPCWSTR>(ReadDWORD(GetBase() + 0x94)), _countof(wszName) - 1);
			Name_ = wszName;
		}
	}
	else
	{
		if (ReadDWORD(GetBase() + 0x94) != NULL)
		{
			MyTools::CCharacter::wstrcpy_my(wszName, reinterpret_cast<LPCWSTR>(GetBase() + 0x94), _countof(wszName) - 1);
			Name_ = wszName;
		}
	}
}


CONST Point CTeamMember::GetPoint() CONST
{
	auto fnGetPoint = [&](_In_ DWORD dwOffset)
	{
		return (float)(4 * (int)(short)ReadDWORD(GetBase() + dwOffset));
	};

	
	return Point(fnGetPoint(0x24), fnGetPoint(0x26), fnGetPoint(0x28));
}

BOOL CTeamMember::IsCaptain() CONST
{
	return dwIndex == 0;
}

BOOL CTeamMember::SetCaptain()
{
	dwIndex = 0;
	return TRUE;
}

BOOL CTeamMember::IsConnect() CONST
{
	return ReadDWORD(GetBase() + 0x6C) == 0 ? FALSE : TRUE;
}

BOOL CTeamMember::DeleteMember() CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([this]{
		CGameSend::RemoveTeamMember(dwIndex);
	});
}

DWORD CTeamMember::GetChannel() CONST
{
	return ReadDWORD(GetBase() + 0x60);
}

DWORD CTeamMember::GetPercentHp() CONST
{
	float fHp = static_cast<float>(ReadDWORD(GetBase() + 0x18));
	float fMaxHp = static_cast<float>(ReadDWORD(GetBase() + 0x30) + ReadDWORD(GetBase() + 0x38));
	if (fMaxHp == 0)
		return 0;

	return static_cast<DWORD>(fHp / fMaxHp * 100);
}

float CTeamMember::GetDis() CONST
{
	return CBaseObject::GetPersonDis(GetPoint());
}

float CTeamMember::GetDis(_In_ CONST Point& Tar) CONST
{
	return Get3DDis(Tar, GetPoint());
}

BOOL CTeamMember::IsExist() CONST
{
	return TRUE;
}

void CTeamMember::SetAngle() CONST
{
	CBaseObject::SetAngle(GetPoint());
}

DWORD CTeamMember::GetIndex() CONST
{
	return dwIndex;
}

DWORD CTeamMember::GetMapId() CONST
{
	return ReadDWORD(GetBase() + 0x2C);
}
