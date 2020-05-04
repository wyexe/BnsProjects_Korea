#ifndef __GITBNSPROJECTS_BNSPROJECTS_GAME_CONSOLE_LOG_CONSOLELOG_H__
#define __GITBNSPROJECTS_BNSPROJECTS_GAME_CONSOLE_LOG_CONSOLELOG_H__

#include "GameVariable.h"

class CConsoleLog
{
public:
	CConsoleLog(_In_ em_ConsoleLog emType, _In_ CONST std::wstring& wsLogContent);
	CConsoleLog(_In_ CONST std::wstring& wsLogContent);
	~CConsoleLog() = default;

private:
	VOID WriteToConsoleLog(_In_ em_ConsoleLog emType, _In_ CONST std::wstring& wsLogContent) CONST _NOEXCEPT;
};



#endif// !__GITBNSPROJECTS_BNSPROJECTS_GAME_CONSOLE_LOG_CONSOLELOG_H__
