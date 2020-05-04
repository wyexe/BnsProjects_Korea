#ifndef __GITBNSPROJECTS_BNSPROJECTS_OBJECTS_OTHEROBJECTS_H__
#define __GITBNSPROJECTS_BNSPROJECTS_OBJECTS_OTHEROBJECTS_H__

#include "GameVariable.h"

class CGameUi
{
public:
	explicit CGameUi(_In_ DWORD dwUiBase_) throw();
	explicit CGameUi();
	~CGameUi();

	// UiBase
	DWORD GetBase() CONST;

	// UiObj
	DWORD GetObj() CONST;

	// UiId
	DWORD GetId() CONST;

	// UiName
	BOOL GetName_SEH(std::wstring& wsName) CONST;
	CONST std::wstring GetName() CONST;

	// UiShow
	BOOL IsShow() CONST;

	DWORD GetNameLen() CONST;

	bool operator < (CONST CGameUi& GameUi_) CONST;

	bool operator == (CONST CGameUi& GameUi_) CONST;

	BOOL AcceptTeamInvite() CONST;
	// 关闭窗口
	BOOL CloseGameWindow() CONST;
	BOOL CloseConfirmWindow() CONST;
	BOOL ClickYes() CONST;

	VOID  SetHash();
	DWORD GetHash() CONST;

	BOOL SetUiShow(_In_ BOOL bShow) CONST;

	// 是否可控BOSS (必须是在'Target2_StatusPanel' 下才可用)
	BOOL IsEnableControl() CONST;
private:
	DWORD dwUiBase;
	DWORD dwHash;
};



#endif