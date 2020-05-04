#include "stdafx.h"
#include "ConsoleLog.h"
#include "SomeFun.h"
#include <MyTools/Log.h>
#include "BnsConfig.h"

#define _SELF L"ConsoleLog.cpp"
CConsoleLog::CConsoleLog(_In_ em_ConsoleLog emType, _In_ CONST std::wstring& wsLogContent)
{
	WriteToConsoleLog(emType, wsLogContent);
}

CConsoleLog::CConsoleLog(_In_ CONST std::wstring& wsLogContent)
{
	WriteToConsoleLog(em_ConsoleLog::em_ConsoleLog_Content, wsLogContent);
}

VOID CConsoleLog::WriteToConsoleLog(_In_ em_ConsoleLog emType, _In_ CONST std::wstring& wsLogContent) CONST _NOEXCEPT
{
	static std::mutex _Mtx;
	std::lock_guard<std::mutex> _Lock(_Mtx);
	auto& Hwnd = CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.hConsoleWnd;
	if (Hwnd != NULL)
	{
		auto wsText = MyTools::CCharacter::MakeFormatText(L"%s:%s", CGameVariable::GetInstance().GetAccShareInfo(), wsLogContent.c_str());
		auto wsTextPtr = MyTools::CLPublic::AllocArraySharePtr<WCHAR>(wsText.length() + 1);
		MyTools::CCharacter::wstrcpy_my(wsTextPtr.get(), wsText.c_str());
		std::wstring wsPicPath;

		COPYDATASTRUCT cd;
		switch (emType)
		{
		case em_ConsoleLog::em_ConsoleLog_Status:
			cd.dwData = 0x1;
			break;
		case em_ConsoleLog::em_ConsoleLog_Content:
			cd.dwData = 0x2;
			break;
		case em_ConsoleLog::em_ConsoleLog_Result:
			cd.dwData = 0x4;
			break;
		case em_ConsoleLog::em_ConsoleLog_BugReport:
			CBnsConfig::GetInstance().SaveScreenshot(CGameVariable::GetInstance().GetAccShareInfo(), wsPicPath);
			cd.dwData = 0x5;
			break;
		default:
			cd.dwData = 0x0;
			break;
		}

		LOG_CF_D(L"WriteToConsoleLog[%s], Type=%d", wsText.c_str(), cd.dwData);
		cd.lpData = wsTextPtr.get();
		cd.cbData = (wsText.length() + 1) * 2;

		::SendMessageW(Hwnd, WM_COPYDATA, (WPARAM)&cd, (LPARAM)&cd);
	}
}
