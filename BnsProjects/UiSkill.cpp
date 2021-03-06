#include "stdafx.h"
#include "UiSkill.h"
#include "Player.h"
#include "GameSend.h"
#include "Skill.h"
#include "Person.h"
#include "Cat.h"
#include "CatExpend.h"
#define _SELF L"UiSkill.cpp"

CUiSkill::CUiSkill()
{
}

CUiSkill::CUiSkill(_In_ DWORD dwBase_, _In_ DWORD dwIndex_) : dwIndex(dwIndex_), dwBase(dwBase_)
{
	
}

CUiSkill::~CUiSkill()
{
}

DWORD CUiSkill::GetId() CONST throw()
{
	return ReadDWORD(GetBase() + 0x10);
}

DWORD CUiSkill::GetBase() CONST throw()
{
	return dwBase;
}

BOOL CUiSkill::IsCool() CONST throw()
{
	// != 2
	return ReadBYTE(GetBase() + 0xC8) == 0 ? TRUE : FALSE;
}

BOOL CUiSkill::IsActive() CONST throw()
{
	// != 2 
	return ReadBYTE(GetBase() + 0x30) == 0 ? TRUE : FALSE;
}

BOOL CUiSkill::IsExist() CONST throw()
{
	return ReadDWORD(GetBase() + 0x8) == 0x1 && (ReadDWORD(GetBase() + 0x64) == 0x2E || ReadDWORD(GetBase() + 0x64) == 0x1F);
}

bool CUiSkill::operator<(CONST CUiSkill& CUiSkill_) CONST
{
	return this->GetId() < CUiSkill_.GetId();
}

CONST std::wstring& CUiSkill::GetTextKeyByIndex() CONST throw()
{
	struct KeyText
	{
	 std::wstring wsKeyText;
		UINT	uIndex;
	};

	CONST static std::vector<KeyText> KeyTextVec = {
		//////////////////////////////////////////////////////////////////////////
		{ L"1", 0 }, { L"2", 1 }, { L"3", 2 }, { L"4", 3 },
		{ L"Z", 4 }, { L"X", 5 }, { L"C", 6 }, { L"V", 7 },
		{ L"SS", 8 }, { L"Q", 9 }, { L"E", 10 }, {L"Tab", 11},
		//////////////////////////////////////////////////////////////////////////
		{ L"R", 35 }, { L"T", 38 }, { L"G", 44 }, { L"B", 47 }, { L"F", 41 },
	};
	static CONST std::wstring wsUnKnow = L"未知";

	auto p = MyTools::CLPublic::Vec_find_if_Const(KeyTextVec, [this](CONST KeyText& itm){ return itm.uIndex == dwIndex; });
	return p != nullptr ? p->wsKeyText : wsUnKnow;
}

DWORD CUiSkill::GetIndex() CONST throw()
{
	return dwIndex;
}

std::wstring CUiSkill::GetName() CONST
{
	DWORD dwParam1 = ReadDWORD(ReadDWORD(GetBase() + UI技能名字偏移) + 技能名字偏移1);
	DWORD dwParam2 = ReadDWORD(ReadDWORD(GetBase() + UI技能名字偏移) + 技能名字偏移2);

	DWORD dwPtrName = ReadDWORD(CBaseObject::GetObjectNameAddr(dwParam1 != 0 ? dwParam1 : dwParam2, dwParam1 == 0 ? dwParam1 : dwParam2) + 0x18);
	if (dwPtrName != NULL && ReadDWORD(dwPtrName) != NULL)
	{
		WCHAR wszText[MAX_OBJECT_NAME_LEN] = { 0 }; 
		MyTools::CCharacter::wstrcpy_my(wszText, (LPCWSTR)dwPtrName, _countof(wszText) - 1);
		return std::wstring(wszText);
	}

	return L"Empty";
}

DWORD CUiSkill::GetSendPacketParmater() CONST
{
	return ReadDWORD(ReadDWORD(GetBase() + UI技能资源偏移) + 0xC) & 0xFF;
}
