#include <iostream>
#include <thread>
#include <MyTools/CLProcess.h>
#include <MyTools/CLFile.h>
#include <MyTools/Log.h>
#include "../BnsKorServer_IOCP/CLHmacSha1.h"

#define BnsProject_Korea
#include "../BnsProjects/GameStruct.h"

#define _SELF L"main.cpp"
DWORD CalcOTP(_In_ CONST std::wstring& wsKey)
{
	DWORD dwValue = 0;
	CLHmacSha1::GetInstance().CalcOTPValue(wsKey, dwValue);

	return dwValue;
}

struct LoginAccountContent
{
	std::wstring wsPlayerName;
	std::wstring wsAccountName;
	std::wstring wsAccountPass;
	std::wstring wsKey;
};

std::vector<LoginAccountContent> _VecLoginAccountContent;
VOID ReadAccountFile()
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
		_VecLoginAccountContent.push_back(AccountContent_);
	}
}

std::wstring GetGamePath()
{
	WCHAR wszDataKey[1024] = { LR"(SOFTWARE\Wow6432Node\plaync\BNS_KOR\)" };

	HKEY hKey;
	if (::RegOpenKeyExW(HKEY_LOCAL_MACHINE, wszDataKey, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		std::wcout << L"Reg GamePath Faild1!" << std::endl;
		::Sleep(INFINITE);
	}

	WCHAR wszGamePath[1024] = { 0 };
	DWORD dwType = REG_SZ;
	DWORD dwSize = 1024;

	DWORD dwError = ::RegQueryValueExW(hKey, L"BaseDir", NULL, &dwType, reinterpret_cast<BYTE*>(wszGamePath), &dwSize);
	if (dwError != ERROR_SUCCESS)
	{
		std::wcout << L"Reg GamePath Faild! Err=" << dwError << std::endl;
		::Sleep(INFINITE);
	}

	return std::wstring(wszGamePath);
}

HMODULE LoadLoginDLL()
{
	WCHAR wszPath[MAX_PATH] = { 0 };
	::GetCurrentDirectoryW(MAX_PATH, wszPath);
	::lstrcatW(wszPath, L"\\CopyFile\\libeay32.dll");
	HMODULE hm = ::LoadLibraryW(wszPath);
	if (hm == NULL)
	{
		std::wcout << L"UnExist libeay32.dll" << std::endl;
		::Sleep(INFINITE);
		return 0;
	}

	::GetCurrentDirectoryW(MAX_PATH, wszPath);
	::lstrcatW(wszPath, L"\\CopyFile\\LoginLib.dll");
	hm = ::LoadLibraryW(wszPath);
	if (hm == NULL)
	{
		std::wcout << L"UnExist LoginLib.dll" << std::endl;
		::Sleep(INFINITE);
		return 0;
	}

	return hm;
}

LoginAccountContent ChoiceAccount()
{
	ReadAccountFile();
	if (_VecLoginAccountContent.size() == 0)
	{
		std::wcout << L"UnExist 'Account.txt' or UnExist Content in 'Account.txt'" << std::endl;
		::Sleep(INFINITE);
	}

	
	UINT nIndex = 0;
	for (CONST auto& itm : _VecLoginAccountContent)
	{
		std::wcout << L"Index:" << nIndex++ << L", Account:" << itm.wsAccountName.c_str() << std::endl;
	}

	std::wcout << L"Typing Your Choice Index:";

	std::wcin >> nIndex;
	if (nIndex >= _VecLoginAccountContent.size())
	{
		std::wcout << L"What?" << std::endl;
		::Sleep(INFINITE);
	}

	std::wcout << std::endl;
	std::wcout << L"Logining Account:" << _VecLoginAccountContent.at(nIndex).wsAccountName.c_str() << std::endl;
	return _VecLoginAccountContent.at(nIndex);
}

VOID WINAPI CreateGameProc(LPVOID pAccGame, LPCWSTR pwszToken)
{
	CONST WCHAR* lpToken = reinterpret_cast<CONST WCHAR*>(pwszToken);
	std::shared_ptr<WCHAR> Buffer(new WCHAR[wcslen(lpToken) * 2], [](WCHAR* p){ delete[] p; });

	WCHAR wszDLLPath[MAX_PATH] = { 0 };

	swprintf_s(Buffer.get(), wcslen(lpToken) * 2, L"%s\\bin\\Client.exe %s", GetGamePath().c_str(), lpToken);

	::GetCurrentDirectoryW(MAX_PATH, wszDLLPath);
	::lstrcatW(wszDLLPath, L"\\CopyFile\\JLnp_hf.dll");

	PROCESS_INFORMATION pi;
	MyTools::CLProcess::CreateProcess_InjectorRemoteDLL(Buffer.get(), wszDLLPath, &pi);
	if (pi.hProcess == NULL)
	{
		LOG_MSG_CF(L"创建进程失败!, Err=%d", ::GetLastError());
		return;
	}
	reinterpret_cast<PACCOUNT_INFO_GAME>(pAccGame)->dwPid = pi.dwProcessId;
	::CloseHandle(pi.hProcess);
}

VOID InputOTP(PACCOUNT_INFO_GAME pAccGame, _In_ CONST LoginAccountContent& LoginAccount_)
{
	std::wcout << L"dwPid=" << pAccGame->dwPid << std::endl;
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pAccGame->dwPid);
	if (hProcess == NULL)
	{
		std::wcout << L"hProcess Faild!!!" << std::endl;
		::Sleep(INFINITE);
		return;
	}

	WCHAR wszOTPValue[32] = { 0 };
	wsprintfW(wszOTPValue, L"%d", CalcOTP(LoginAccount_.wsKey));

	// 
	LPVOID lpOTPValue = ::VirtualAllocEx(hProcess, NULL, 32, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (lpOTPValue == nullptr)
	{
		std::wcout << L"VirtualAllocEx Faild!!!" << std::endl;
		::Sleep(INFINITE);
		return;
	}


	// Copy OTP Value Text to Target Process Memory
	::WriteProcessMemory(hProcess, lpOTPValue, wszOTPValue, _countof(wszOTPValue), NULL);

	// alloc Memory to Write Machine Code
	LPVOID lpAddr = ::VirtualAllocEx(hProcess, NULL, 1024, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (lpAddr == nullptr)
	{
		std::wcout << L"VirtualAllocEx Faild!!!" << std::endl;
		::Sleep(INFINITE);
		return;
	}

	/*
	MOV ECX, wszText;      B9 Addr
	MOV EAX, 输入OTPCALL;  E8 6682B1F8
	CALL EAX;
	RETN 4                 C2 0400 // to Remote Thread Param
	*/

	// Write 'MOV ECX,'
	BYTE MovCmd = 0xB9;
	::WriteProcessMemory(hProcess, lpAddr, &MovCmd, sizeof(MovCmd), NULL);

	// Write Addr
	::WriteProcessMemory(hProcess, reinterpret_cast<CHAR*>(lpAddr) + 1, &lpOTPValue, 4, NULL);

	// Write 'CALL'
	BYTE CALLCmd = 0xE8;
	::WriteProcessMemory(hProcess, reinterpret_cast<CHAR*>(lpAddr) + 5, &CALLCmd, sizeof(CALLCmd), NULL);

	// MEM RVA
	DWORD dwCALLAddr = reinterpret_cast<DWORD>(lpAddr) + 6;
	dwCALLAddr += 4;

	// RVA to VA
	dwCALLAddr = 输入OTPCALL - dwCALLAddr;
	::WriteProcessMemory(hProcess, reinterpret_cast<CHAR*>(lpAddr) + 6, &dwCALLAddr, sizeof(dwCALLAddr), NULL);

	// Write Retn 4
	CONST DWORD dwRetnCode = 0x900004C2;
	::WriteProcessMemory(hProcess, reinterpret_cast<CHAR*>(lpAddr) + 0xA, &dwRetnCode, sizeof(dwRetnCode), NULL);

	std::wcout << std::hex << lpAddr << std::endl;
	::CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)lpAddr, nullptr, NULL, NULL);
}

int main()
{
	CreateMutexW(NULL, FALSE, L"CL_BNS_KOR");

	ReadAccountFile();

	UINT nIndex = 0;
	for (CONST auto& itm : _VecLoginAccountContent)
	{
		std::wcout << L"Index:" << nIndex++ << L", Account:" << itm.wsAccountName.c_str() << std::endl;
	}

	if (_VecLoginAccountContent.size() == 0)
	{
		std::wcout << L"UnExist 'Account.txt' or UnExist Content in 'Account.txt'" << std::endl;
		::Sleep(INFINITE);
	}

	int Index = 0;
	std::wcout << L"Input Index:";
	std::wcin >> Index;
	if (Index == -1)
	{
		for (auto& itm : _VecLoginAccountContent)
		{
			std::thread t([itm]
			{
				HMODULE hm = LoadLoginDLL();

				typedef VOID(WINAPI* CreateGameProcPtr)(LPVOID, LPCWSTR);
				typedef CHAR*(WINAPIV* LoginStart)(LPCWSTR, LPCWSTR, PACCOUNT_INFO_GAME, CreateGameProcPtr);
				LoginStart MethodPtr = (LoginStart)::GetProcAddress(hm, "LoginStart");

				PACCOUNT_INFO_GAME pAccGame = new ACCOUNT_INFO_GAME;
				ZeroMemory(pAccGame, sizeof(ACCOUNT_INFO_GAME));
				MyTools::CCharacter::wstrcpy_my(pAccGame->MyAcount_Info.szUserName, itm.wsAccountName.c_str());
				MyTools::CCharacter::wstrcpy_my(pAccGame->MyAcount_Info.szPassWord, itm.wsAccountPass.c_str());

				CHAR* p = MethodPtr(itm.wsAccountName.c_str(), itm.wsAccountPass.c_str(), pAccGame, CreateGameProc);
				std::cout << p << std::endl;

				int i = 0;
				std::cin >> i;
				InputOTP(pAccGame, itm);
			});
			t.detach();

			::Sleep(5 * 1000);
		}
	}
	else
	{
		auto& itm = _VecLoginAccountContent.at(Index);
		HMODULE hm = LoadLoginDLL();

		typedef VOID(WINAPI* CreateGameProcPtr)(LPVOID, LPCWSTR);
		typedef CHAR*(WINAPIV* LoginStart)(LPCWSTR, LPCWSTR, PACCOUNT_INFO_GAME, CreateGameProcPtr);
		LoginStart MethodPtr = (LoginStart)::GetProcAddress(hm, "LoginStart");

		PACCOUNT_INFO_GAME pAccGame = new ACCOUNT_INFO_GAME;
		ZeroMemory(pAccGame, sizeof(ACCOUNT_INFO_GAME));
		MyTools::CCharacter::wstrcpy_my(pAccGame->MyAcount_Info.szUserName, itm.wsAccountName.c_str());
		MyTools::CCharacter::wstrcpy_my(pAccGame->MyAcount_Info.szPassWord, itm.wsAccountPass.c_str());

		CHAR* p = MethodPtr(itm.wsAccountName.c_str(), itm.wsAccountPass.c_str(), pAccGame, CreateGameProc);
		std::cout << p << std::endl;

		int i = 0;
		std::cin >> i;
		InputOTP(pAccGame, itm);
	}
	

	std::wcout << L"Done!" << std::endl;
	::Sleep(INFINITE);
	return 0;
}