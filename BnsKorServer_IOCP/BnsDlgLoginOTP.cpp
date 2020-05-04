#include "BnsDlgLoginOTP.h"
#include <iostream>
#include <MyTools/CLFile.h>
#include <MyTools/Character.h>
#include <MyTools/Log.h>
#include "ServerCommon.h"
#include "BnsOTP.h"

#define _SELF L"BnsDlgLoginOTP.cpp"
VOID CBnsDlgLoginOTP::Run()
{
	SetShareMemory();

	cbBEGINTHREADEX(NULL, NULL, _WorkThread, this, NULL, NULL);
}

VOID CBnsDlgLoginOTP::ReadAccountFile()
{
	WCHAR wszPath[MAX_PATH] = { 0 };
	::GetCurrentDirectoryW(MAX_PATH, wszPath);
	lstrcatW(wszPath, L"\\Account.txt");

	std::wstring wsFileContent;
	MyTools::CLFile::ReadUnicodeFile(wszPath, wsFileContent);

	std::vector<std::wstring> VecFileContent;
	MyTools::CCharacter::Split(wsFileContent, L"\r\n", VecFileContent, Split_Option_RemoveEmptyEntries);

	_VecLoginAccountContent.clear();
	for (CONST auto& itm : VecFileContent)
	{
		std::vector<std::wstring> VecAccountText;
		if (MyTools::CCharacter::Split(itm, L",", VecAccountText, Split_Option_RemoveEmptyEntries) != 4)
		{
			LOG_MSG_CF(L"Invalid Account Format Text:%s", itm.c_str());
			return;
		}

		LoginAccountContent AccountContent_;
		AccountContent_.wsPlayerName = VecAccountText.at(0);
		AccountContent_.wsAccountName = VecAccountText.at(1);
		AccountContent_.wsAccountPass = VecAccountText.at(2);
		AccountContent_.wsKey = VecAccountText.at(3);
		_VecLoginAccountContent.push_back(std::move(AccountContent_));
	}
}

VOID CBnsDlgLoginOTP::InitShareMemory()
{
	// 创建文件映射
	HANDLE hFileMap = ::CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, NULL, sizeof(Login_Share_Info), SZFILE_NAME_LOGIN_SHAREINFO);
	if (hFileMap == NULL)
	{
		LOG_MSG_CF(L"hFileMap = NULL");
		return;
	}


	// 映射文件到内存
	_pLoginShareInfo = (Login_Share_Info *)MapViewOfFile(hFileMap, FILE_MAP_READ | FILE_SHARE_WRITE, NULL, NULL, sizeof(Login_Share_Info));
	if (_pLoginShareInfo == NULL)
	{
		LOG_MSG_CF(L"g_pSharedInfo = NULL");
		return;
	}

	// Init ShareInfo
	ZeroMemory(_pLoginShareInfo, sizeof(Login_Share_Info));
}

VOID CBnsDlgLoginOTP::SetShareMemory()
{
	ReadAccountFile();

	InitShareMemory();

	for (CONST auto& itm : _VecLoginAccountContent)
	{
		std::wcout << itm.wsAccountName.c_str() << std::endl;
		MyTools::CCharacter::wstrcpy_my(_pLoginShareInfo->arrLoginAccount[_pLoginShareInfo->uCount].wszPlayerName, itm.wsPlayerName.c_str());
		MyTools::CCharacter::wstrcpy_my(_pLoginShareInfo->arrLoginAccount[_pLoginShareInfo->uCount].wszAccountName, itm.wsAccountName.c_str());
		MyTools::CCharacter::wstrcpy_my(_pLoginShareInfo->arrLoginAccount[_pLoginShareInfo->uCount].wszAccountPass, itm.wsAccountPass.c_str());
		MyTools::CCharacter::wstrcpy_my(_pLoginShareInfo->arrLoginAccount[_pLoginShareInfo->uCount].wszAccountKey, itm.wsKey.c_str());
		_pLoginShareInfo->uCount += 1;
	}
}

DWORD WINAPI CBnsDlgLoginOTP::_WorkThread(_In_ LPVOID lpParm)
{
	CBnsDlgLoginOTP* pCLoginOTP = (CBnsDlgLoginOTP *)lpParm;

	auto& pShareInfo = pCLoginOTP->_pLoginShareInfo;

	for (;;)
	{
		for (UINT i = 0; i < pShareInfo->uCount; ++i)
		{
			if (wcslen(pShareInfo->arrLoginAccount[i].wszAccountKey) == 16)
			{
				DWORD dwValue = 0;
				pShareInfo->arrLoginAccount[i].dwValue = CBnsOTP::GetInstance().CalculateOTP(pShareInfo->arrLoginAccount[i].wszAccountKey, dwValue) ? dwValue : 0;
			}
		}
		Sleep(3000);
	}
}
