#include "stdafx.h"
#include "ConsoleLog.h"
#include <MyTools/CLPublic.h>
#include <MyTools/Log.h>
#include <MyTools/Character.h>

#define _SELF L"ConsoleLog.cpp"
VOID CConsoleLog::Add(_In_ em_ConsoleLog emType, _In_ CONST std::wstring& wsAccountName, _In_ CONST std::wstring& wsLogContent)
{
	static std::mutex _Mtx;
	std::lock_guard<std::mutex> _Lock(_Mtx);
	auto& Hwnd = g_pSharedInfo->ConsoleContent.hConsoleWnd;
	if (Hwnd != NULL)
	{
		auto wsText = MyTools::CCharacter::MakeFormatText(L"%s:%s", wsAccountName.c_str(), wsLogContent.c_str());
		auto wsTextPtr = MyTools::CLPublic::AllocArraySharePtr<WCHAR>(wsText.length() + 1);
		MyTools::CCharacter::wstrcpy_my(wsTextPtr.get(), wsText.c_str());

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
		default:
			cd.dwData = 0x0;
			break;
		}

		cd.lpData = wsTextPtr.get();
		cd.cbData = (wsText.length() + 1) * 2;

		::SendMessageW(Hwnd, WM_COPYDATA, (WPARAM)&cd, (LPARAM)&cd);
	}
}

std::function<VOID(em_ConsoleLog, CONST std::wstring&, CONST std::wstring&)> CConsoleLog::GetAddPtr()
{
	return std::bind(&CConsoleLog::Add, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}
