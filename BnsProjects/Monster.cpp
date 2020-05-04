#include "stdafx.h"
#include "Monster.h"
#include "SomeFun.h"
#include "GameSend.h"
#include "Npc.h"
#define _SELF L"Monster.cpp"

CMonster::CMonster(DWORD dwBase_) : CPlayer(dwBase_)
{
	
}

CMonster::CMonster()
{
	
}

CMonster::CMonster(CONST CPlayer& Player) : CPlayer(Player)
{
	
}

CMonster::~CMonster()
{

}

DWORD CMonster::GetMonsterType() CONST
{
	return ReadDWORD(GetBase() + 0xE) & 0xFFFF;
}

VOID CMonster::SetName() CONST
{
	WCHAR wszText[MAX_OBJECT_NAME_LEN] = { 0 };
	DWORD dwAddr = ReadDWORD(GetBase() + π÷ŒÔ Ù–‘∆´“∆);
	DWORD dwValue = ReadDWORD(GetObjectNameAddr(ReadDWORD(dwAddr + 0x1C), ReadDWORD(dwAddr + 0x20)) + 0x18);
	if (dwValue != NULL && ReadDWORD(dwValue) != NULL)
		MyTools::CCharacter::wstrcpy_my(wszText, (LPCWSTR)dwValue, _countof(wszText) - 1);

	Name = wszText;
}

CONST std::wstring& CMonster::GetName() CONST
{
	return Name;
}

Point CMonster::GetBehindPoint(float fDis) CONST
{
	return _GetBehindPoint(this, fDis);
}

bool CMonster::operator<(const CMonster& GameObj) CONST
{
	return this->SortDis() < GameObj.SortDis();
}

BOOL CMonster::IsExist() CONST
{
	if (GetMaxHp() == 0 || GetPoint().IsZero() || IsDead())
		return FALSE;

	return TRUE;
}

BOOL CMonster::TakeNpcItem()
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([this]{
		CGameSend::TakeItem_By_Npc(GetId(), GetIdParm());
	});
}

Point CMonster::GetFrontPoint(float fDis) CONST
{
	return _GetFrontPoint(this, fDis);
}


Point CMonster::GetFacePoint(float fDis /*= 2.0f*/) CONST
{
	return _GetFacePoint(this, fDis);
}

CMonster::em_Maze_Shield_Color CMonster::Maze_GetShieldColor() CONST
{
	DWORD dwValue = ReadDWORD(GetBase() + 0x2C);
	if (dwValue == 0xB2)
		return em_Maze_Shield_Color::em_Maze_Shield_Color_Ice;
	else if (dwValue == 0xB1)
		return em_Maze_Shield_Color::em_Maze_Shield_Color_Fire;

	return em_Maze_Shield_Color::em_Maze_Shield_Color_None;
}
