#include "stdafx.h"
#include "BnsConfig.h"
#include <MyTools/CLFile.h>
#include <MyTools/CLProcess.h>
#include <MyTools/Log.h>
#include <MyTools/CLPic.h>
#include <MyTools/CLEchoException.h>
#if BnsProject_DLL
#include "SocketConfig.h"
#endif
#define _SELF L"BnsConfig.cpp"

BOOL CBnsConfig::DivisionConfig(_Out_opt_ std::vector<std::wstring>& vlst, _In_ CONST std::wstring& wsPath) CONST
{
	std::wstring wsContent;
	if (!ReadConfig(wsPath, wsContent))
	{
		LOG_CF_E(L"ReadConfig Faild!");
		
		return FALSE;
	}

	vlst.clear();
	MyTools::CCharacter::Split(wsContent, L"\r\n", vlst, Split_Option_RemoveEmptyEntries | Split_Option_KeepOnly);
	return TRUE;
}

BOOL CBnsConfig::ReadConfig(_In_ CONST std::wstring& wsPath, _Out_ std::wstring& wsContent) CONST
{
	return MyTools::CLFile::ReadUnicodeFile(wsPath, wsContent);
}

BOOL CBnsConfig::WriteConfig(_In_ CONST std::wstring& wsPath, _In_ CONST std::wstring& wsContent) CONST
{
	return MyTools::CLFile::WriteUnicodeFile(wsPath, wsContent);
}

BOOL CBnsConfig::ClearAccountConfig(_In_ CONST std::wstring& wsAccountName) CONST
{
	std::vector<AccountConfig> vlst;
	GetAccountConfigList(vlst);
	if (vlst.size() == NULL)
		return TRUE;

	MyTools::CLPublic::Vec_erase_if(vlst, [wsAccountName](CONST AccountConfig& itm){ return itm.wsAccountName == wsAccountName; });
	return SaveAccountConfigList(vlst);
}

VOID CBnsConfig::WriteAccountLog(_In_ CONST std::wstring& wsContent)
{
	MyTools::CLEchoException::GetInstance().InvokeAction(__FUNCTIONW__, [=] 
	{
		WriteAccountLog(m_pAccGame, wsContent);
	});
}

VOID CBnsConfig::WriteAccountLog(_In_ PACCOUNT_INFO_GAME pAccGame, _In_ CONST std::wstring& wsContent)
{
	SYSTEMTIME SysTime;
	::GetLocalTime(&SysTime);

	if (m_pShare != nullptr && pAccGame != nullptr)
	{
		PrintToConsoleLog(em_ConsoleLog::em_ConsoleLog_Result, pAccGame->GetAccName(), wsContent);
		PrintToConsoleLog(em_ConsoleLog::em_ConsoleLog_Content, pAccGame->GetAccName(), wsContent);

		std::wstring wsBuffer = MyTools::CCharacter::MakeFormatText(L"%04d-%02d-%02d %02d:%02d:%02d %s,%s:%s",
			SysTime.wYear,
			SysTime.wMonth,
			SysTime.wDay,
			SysTime.wHour,
			SysTime.wMinute,
			SysTime.wSecond,
			pAccGame->GetAccountInfo()->szUserName,
			pAccGame->GetAccountInfo()->szPassWord,
			wsContent.c_str());

		std::wstring wsPath = m_pShare->ConsoleContent.szConsolePath;
		wsPath += L"\\Setting\\AccountLog.txt";
#if BnsProject_DLL
		MyTools::CLFile::AppendUnicodeFile(wsPath, wsBuffer);
		CSocketConfig().SaveAccountLog_To_Server(em_ClientAccountLog_Type::em_ClientAccountLog_Type_AccountLog, wsContent);
#elif BnsProject_Dlg
		MyTools::CLFile::AppendUnicodeFile(L"C:\\Bns\\AccountLog.txt", wsBuffer);;
#else
		MyTools::CLFile::AppendUnicodeFile(wsPath, wsBuffer);
#endif
	}
}

BOOL CBnsConfig::GetAccountConfigList(_Out_opt_ std::vector<AccountConfig>& vlst) CONST
{
	std::vector<std::wstring> vstr;
	DivisionConfig(vstr, wsConfigPath + L"\\Setting\\Account.txt");

	auto fnGetAccountConfig = [](_In_ std::wstring& ws, _Out_ AccountConfig& AccountConfig_)
	{
		std::vector<std::wstring> vlst;
		if (MyTools::CCharacter::Split(ws, L",", vlst, Split_Option_RemoveEmptyEntries | Split_Option_KeepOnly) == 2)
		{
			AccountConfig_.wsAccountName = vlst.at(0);
			AccountConfig_.wsAccountPass = vlst.at(1);
			return TRUE;
		}
		return FALSE;
	};

	for (auto& ws : vstr)
	{
		AccountConfig AccountConfig_;
		if (fnGetAccountConfig(ws, AccountConfig_))
			vlst.push_back(AccountConfig_);
	}
	return TRUE;
}

BOOL CBnsConfig::SaveAccountConfigList(_In_ CONST std::vector<AccountConfig>& vlst) CONST
{
	std::wstring wsContent;
	for_each(vlst.begin(), vlst.end(), [&wsContent](const AccountConfig& AccountConfig_)
	{
		wsContent += AccountConfig_.wsAccountName;
		wsContent += L",";
		wsContent += AccountConfig_.wsAccountPass;
		wsContent += L"\r\n";
	});

	return WriteConfig(wsConfigPath + L"\\Setting\\Account.txt", wsContent);
}

std::shared_ptr<CHAR> CBnsConfig::GetTelprotFile(_In_ CONST std::wstring& wsPath, _In_ CONST ULONG& ulFileLen) CONST
{
	FILE* pFile = nullptr;
	fopen_s(&pFile, MyTools::CCharacter::UnicodeToASCII(wsPath).c_str(), "rb");
	if (pFile == nullptr)
	{
		LOG_MSG_CF( L"ReadTelportFile Fiald! Path:%s", wsPath.c_str());
		return nullptr;
	}

	std::shared_ptr<CHAR> psContent(new CHAR[ulFileLen], [](CHAR* p){delete[] p; });
	fseek(pFile, 0, SEEK_SET);
	fread(psContent.get(), sizeof(CHAR), (size_t)ulFileLen, pFile);
	fclose(pFile);
	return psContent;
}

VOID CBnsConfig::SetConfigPath(_In_ PSHARED_INFO pShare, _In_ PACCOUNT_INFO_GAME pAccGame, _In_ CONST std::wstring& wsPath)
{
	wsConfigPath = wsPath;
	m_pShare = pShare;
	m_pAccGame = pAccGame;
}

BOOL CBnsConfig::PrintDumpInfo(_In_ CONST std::wstring& wsPath) CONST
{
	std::wstring wsText = L"\r\n";
	PrintProcInfo_By_Dump(wsText);
	PrintShareInfo(wsText);
	return MyTools::CLFile::WriteUnicodeFile(wsPath, wsText);
}

BOOL CALLBACK BnsConfig_EnumWindows(HWND h, LPARAM l)
{
	if (IsWindow(h) && IsWindowVisible(h) && (GetWindowLong(h, GWL_EXSTYLE)&WS_EX_TOOLWINDOW) != WS_EX_TOOLWINDOW && (GetWindowLong(h, GWL_HWNDPARENT) == 0))
	{
		static WCHAR wszWinText[64] = { 0 };
		static WCHAR wszWinClass[64] = { 0 };
		if (GetClassNameW(h, wszWinClass, _countof(wszWinClass) - 1) > 0 && GetWindowTextW(h, wszWinText, _countof(wszWinText) - 1) > 0)
		{
			DWORD PID;
			::GetWindowThreadProcessId(h, &PID);

			std::wstring wsProcName;
			if (MyTools::CLProcess::GetProcName_By_Pid(PID, wsProcName))
			{
				std::wstring* pwsText = (std::wstring*)l;
				static WCHAR wszText[1024];
				swprintf_s(wszText, _countof(wszText) - 1, L"Class:%s Text:%s Pid:%X ProcName:%s \r\n", wszWinClass, wszWinText, PID, wsProcName.c_str());
				pwsText->append(wszText);
			}
		}
	}
	return true;
}

BOOL CBnsConfig::PrintProcInfo_By_Dump(_Out_opt_ std::wstring& wsText) CONST
{
	return EnumWindows((WNDENUMPROC)BnsConfig_EnumWindows, (LPARAM)&wsText);
}

BOOL CBnsConfig::PrintShareInfo(_Out_opt_ std::wstring& wsText) CONST
{
	if (m_pShare == nullptr)
		return FALSE;

	
	return TRUE;
}

VOID CBnsConfig::PrintToConsoleLog(_In_ em_ConsoleLog emType, CONST std::wstring& wsAccountName, CONST std::wstring& wsContent)
{
	if (_ConsoleLogPtr != nullptr)
	{
		_ConsoleLogPtr(emType, wsAccountName, wsContent);
	}
}

BOOL CBnsConfig::SaveLocalTimeConfigToServer(_In_ CONST std::wstring& wsPlayerName, _In_ CONST std::wstring& wsConfigName) CONST
{
#if BnsProject_DLL
	SYSTEMTIME CurrentTime;
	WCHAR wszCurrentTime[MAX_PATH] = { 0 };

	::GetLocalTime(&CurrentTime);
	swprintf_s(wszCurrentTime, L"%d-%d-%d-%d-%d-%d", CurrentTime.wYear, CurrentTime.wMonth, CurrentTime.wDay, CurrentTime.wHour, CurrentTime.wMinute, CurrentTime.wSecond);

	ConfigContent ConfigContent_;
	ConfigContent_.wsConfigName = wsConfigName;
	ConfigContent_.wsConfigValue = wszCurrentTime;
	CSocketConfig().SetConfigValue(std::move(ConfigContent_));
#endif
	return TRUE;
}

CONST std::wstring CBnsConfig::GetSingleConfigValueByServer(_In_ CONST std::wstring& wsPlayerName, _In_ CONST std::wstring& wsConfigName) CONST
{
#if BnsProject_DLL
	return CSocketConfig().GetConfigValue(wsPlayerName, wsConfigName);
#else
	return std::wstring(L"");
#endif	
}

VOID CBnsConfig::SetSingleConfigValueByServer(_In_ CONST BnsConfigContent& ConfigContent_) CONST
{
#if BnsProject_DLL
	ConfigContent Content_;
	Content_.wsConfigName = ConfigContent_.wsConfigName;
	Content_.wsConfigValue = ConfigContent_.wsConfigValue;
	CSocketConfig().SetConfigValue(std::move(Content_));
#endif
}

CBnsConfig::CBnsConfig() : m_pShare(nullptr), m_pAccGame(nullptr), _ConsoleLogPtr(nullptr)
{

}

BOOL CBnsConfig::SaveScreenshot(_In_ PACCOUNT_INFO_GAME pAccGame, _Out_ std::wstring& wsPicPath)
{
	if (pAccGame == nullptr || m_pShare == nullptr)
		return FALSE;

	wsPicPath = MyTools::CCharacter::MakeFormatText(L"%s\\Setting\\Screenshot\\%s.bmp", m_pShare->ConsoleContent.szConsolePath, pAccGame->GetAccName());
	return MyTools::CLPic::GetInstance().ScreenShot(pAccGame->hGameWnd, wsPicPath);
}

VOID CBnsConfig::SetConsoleLogPtr(_In_ std::function<VOID(em_ConsoleLog, CONST std::wstring&, CONST std::wstring&)> ConsoleLogPtr)
{
	_ConsoleLogPtr = ConsoleLogPtr;
}
