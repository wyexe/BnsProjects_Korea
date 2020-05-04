#include "stdafx.h"
#include "GameUI.h"
#include "GameSend.h"

#define _SELF L"GameUI.cpp"

CGameUi::CGameUi()
{
}

CGameUi::CGameUi(_In_ DWORD dwUiBase_) throw() : dwUiBase(dwUiBase_)
{
	//dwHash = CLPublic::GetHash(GetName());
}

CGameUi::~CGameUi()
{
}

DWORD CGameUi::GetBase() CONST
{
	return dwUiBase;
}

DWORD CGameUi::GetObj() CONST
{
	return dwUiBase;
}

DWORD CGameUi::GetId() CONST
{
	return ReadDWORD(GetObj() + 0x4);
}

BOOL CGameUi::GetName_SEH(std::wstring& wsName) CONST
{
	__try
	{
		wsName = (LPCWSTR)ReadDWORD(GetObj() + 0xC);
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		//CPrintLog::MsgBoxLog_W(_SELF,__LINE__,L"GetName_SEH发生了异常");
	}
	return FALSE;
}

CONST std::wstring CGameUi::GetName() CONST
{
	std::wstring wsName;
	return GetName_SEH(wsName) ? wsName : std::wstring(L"");
}

BOOL CGameUi::IsShow() CONST
{
	return ReadDWORD(GetObj() + 0x38) == 0x1;
}

DWORD CGameUi::GetNameLen() CONST
{
	return ReadDWORD(GetObj() + 0xC + 0x10 + 0x4);
}

bool CGameUi::operator<(CONST CGameUi& GameUi_) CONST
{
	return this->GetObj() < GameUi_.GetObj();
}

bool CGameUi::operator==(CONST CGameUi& GameUi_) CONST
{
	return this->GetObj() == GameUi_.GetObj();
}

BOOL CGameUi::AcceptTeamInvite() CONST
{
	if (!IsShow())
		return FALSE;

	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([this]{CGameSend::AcceptTeamInvite(GetObj()); });
}

BOOL CGameUi::CloseGameWindow() CONST
{
	if (!IsShow())
		return TRUE;

	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([this]{CGameSend::CloseGameWindow(GetObj()); });
}

BOOL CGameUi::CloseConfirmWindow() CONST
{
	return CloseGameWindow();
}

DWORD CGameUi::GetHash() CONST
{
	return dwHash;
}

VOID CGameUi::SetHash()
{
	dwHash = MyTools::CLPublic::GetHash(GetName());
}

BOOL CGameUi::ClickYes() CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([this]{CGameSend::ClickYes(GetObj()); });
}

BOOL CGameUi::SetUiShow(_In_ BOOL bShow) CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([this, &bShow]{ CGameSend::SetShowUi(GetObj(), bShow); });
}

BOOL CGameUi::IsEnableControl() CONST
{
	return ReadBYTE(GetBase() + BOSS是否可控偏移) == 0x1 ? FALSE : TRUE;
}
