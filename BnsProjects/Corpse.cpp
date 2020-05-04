#include "stdafx.h"
#include "Corpse.h"
#include "SomeFun.h"
#include "PersonFunction.h"
#include "GameSend.h"

#define _SELF L"Corpse.cpp"

CCorpse::CCorpse(DWORD dwCorpseBase) : CPlayer(dwCorpseBase)
{
	
}

CCorpse::CCorpse()
{

}

CCorpse::~CCorpse()
{
}

DWORD CCorpse::GetIdType() CONST
{
	return ReadDWORD(GetBase() + 0xE) & 0xFF;
}

CONST Point CCorpse::GetPoint() CONST
{
	DWORD dwIdType = GetIdType();
	if (dwIdType == 0x90)
	{
		auto fn_GetDropItemPoint = [this](int n)
		{
			return (float)(4 * (int)(short)ReadDWORD(GetBase() + 0x28 + 0x2 + n));
		};

		return Point(fn_GetDropItemPoint(0), fn_GetDropItemPoint(2), fn_GetDropItemPoint(4));
	}
	else if (dwIdType == 0x20)
	{
		auto fn_GetCorpsePoint = [this](int n)
		{
			return ReadFloat(GetBase() + n) * 4;
		};
#ifdef BnsProject_TaiWan
		return Point(fn_GetCorpsePoint(0x2C), fn_GetCorpsePoint(0x30), fn_GetCorpsePoint(0x34));

#endif // BnsProject_TaiWan

#ifdef BnsProject_Korea
		return Point(fn_GetCorpsePoint(0x30), fn_GetCorpsePoint(0x34), fn_GetCorpsePoint(0x38));

#endif // BnsProject_Korea
	}
	
	return Point();
}

float CCorpse::GetDis() CONST
{
	return GetPersonDis(GetPoint());
}

float CCorpse::GetDis(_In_ CONST Point& Tar) CONST
{
	return Get3DDis(Tar, GetPoint());
}

BOOL CCorpse::TakeItem() CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([this]{
		CGameSend::TakeItem_By_Task(GetId(), GetIdParm());
	});
}

bool CCorpse::operator<(const CCorpse& Corpse) CONST
{
	return this->SortDis() < Corpse.SortDis();
}

BOOL CCorpse::IsExist() CONST
{
	//if (GetPoint().IsZero())
		//return FALSE;
	return TRUE;
}

BOOL CCorpse::TakeBall() CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([this]{
		CGameSend::TakeBall(GetId(), GetIdParm());
	});
}

CONST std::wstring& CCorpse::GetName() CONST
{
	return Name;
}

BOOL CCorpse::IsCollect() CONST
{
	//0x20 2->3		 0x7D ? ->9		0x83 ? ? ->6D		0x87 ? ? ->FF
	DWORD dwValue = ReadDWORD(GetBase() + 任务尸体是否可捡偏移);


	return (dwValue == 0x2 || dwValue == 0x1/*箱子打开后还没按F的状态!*/) ? TRUE : FALSE;
}

BOOL CCorpse::IsTaskCollect() CONST
{
#ifdef BnsProject_TaiWan
	return (ReadDWORD(GetBase() + 0x28) >> 0x18) == 0x1 ? TRUE : FALSE;

#endif // BnsProject_TaiWan
#ifdef BnsProject_Korea
	return (ReadDWORD(GetBase() + 0x2C) >> 0x18) == 0x1 ? TRUE : FALSE;
#endif // BnsProject_Korea

}

void CCorpse::SetAngle() CONST
{
	CBaseObject::SetAngle(GetPoint());
}

VOID CCorpse::SetName() CONST
{
	WCHAR wszText[MAX_OBJECT_NAME_LEN] = { 0 };
	DWORD dwAddr = ReadDWORD(GetBase() + 尸体名字返回偏移);
	DWORD dwValue = ReadDWORD(GetObjectNameAddr(ReadDWORD(dwAddr + 0x1C), ReadDWORD(dwAddr + 0x20)) + 0x18);
	if (dwValue != NULL && ReadDWORD(dwValue) != NULL)
		MyTools::CCharacter::wstrcpy_my(wszText, (LPCWSTR)dwValue, _countof(wszText) - 1);
	Name = wszText;
}

