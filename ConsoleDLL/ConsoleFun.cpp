#include "stdafx.h"
#include "ConsoleFun.h"
#include <future>
#include <MyTools/CLProcess.h>
#include <MyTools/CLResManager.h>
#include <MyTools/Log.h>
#include <MyTools/CLNoModuleDLL.h>
#include <MyTools/CLFile.h>
#include "ConsoleCard.h"
#include "BnsProcExtend.h"
#include "ConsoleLog.h"
#include "NcLanucherLogin.h"

#define _SELF L"ConsoleFun.cpp"


CConsoleFun::CConsoleFun() : Lock_ConsoleFun(L"CConsoleFun::Lock_ConsoleFun")
{
	
}

BOOL CConsoleFun::Init()
{
	//LOG_CF_D(L"CConsoleFun::Init");
	WCHAR wszPath[MAX_PATH] = { 0 };
	::GetCurrentDirectoryW(MAX_PATH, wszPath);
	::lstrcatW(wszPath, L"\\Log\\");

	MyTools::CLog::GetInstance().SetClientName(L"ConsoleDLL", wszPath);
	MyTools::CLog::GetInstance().ClearLogFile();

	if (!InitMapView())
		return FALSE;

	return TRUE;
}

BOOL CConsoleFun::InitMapView()
{
	//LOG_CF_D(L"CConsoleFun::InitMapView");
	if (hFileMap != NULL)
		return TRUE;

	// �����ļ�
	wchar_t szFilePath[MAX_PATH] = { 0 };
	wsprintf(szFilePath, L"%s.SharedMem", SZFILE_NAME_SHAREDINFO);
	HANDLE hFileSharedInfo = CreateFile(szFilePath, GENERIC_READ | GENERIC_WRITE,          // open for reading
		FILE_SHARE_READ | FILE_SHARE_WRITE,       // share for reading
		NULL,                  // default security
		CREATE_ALWAYS,         // existing file only
		FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM, // normal file
		NULL);
	if (hFileSharedInfo == NULL)
	{
		LOG_CF_E(L"hFileSharedInfo = NULL");
		return FALSE;
	}

	// �����ļ�ӳ��
	hFileMap = ::CreateFileMappingW(hFileSharedInfo, NULL, PAGE_READWRITE, NULL, sizeof(SHARED_INFO), SZFILE_NAME_SHAREDINFO);
	if (hFileMap == NULL)
	{
		LOG_MSG_CF(L"hFileMap = NULL");
		::CloseHandle(hFileSharedInfo);
		return FALSE;
	}

	::CloseHandle(hFileSharedInfo);

	// ӳ���ļ����ڴ�
	g_pSharedInfo = (PSHARED_INFO)MapViewOfFile(hFileMap, FILE_MAP_READ | FILE_SHARE_WRITE, NULL, NULL, sizeof(SHARED_INFO));
	if (g_pSharedInfo == NULL)
	{
		LOG_CF_E(L"g_pSharedInfo = NULL");
		return FALSE;
	}

	// Init ShareInfo
	ZeroMemory(g_pSharedInfo, sizeof(SHARED_INFO));
	::GetCurrentDirectoryW(MAX_PATH, g_pSharedInfo->ConsoleContent.szConsolePath);
	CBnsConfig::GetInstance().SetConfigPath(g_pSharedInfo, nullptr, g_pSharedInfo->ConsoleContent.szConsolePath);
	return TRUE;
}

BOOL CConsoleFun::KillGame_By_AccountName(_In_ PACCOUNT_INFO_GAME pAccountGame) // �ر���Ϸ
{
	if (pAccountGame == nullptr || pAccountGame->dwPid == NULL || !pAccountGame->AccountStatus.bExist)
		return FALSE;

	if (pAccountGame->dwPid == NULL || !MyTools::CLProcess::Is_Exist_Process_For_ProcId(pAccountGame->dwPid))
		return TRUE;

	LOG_CF_D( L"���ڽ�������,AccountName:%s, ����ID:%d", pAccountGame->MyAcount_Info.szUserName, pAccountGame->dwPid);
	CBnsProcExtend().KillGameClientByPId(pAccountGame->dwPid);

	DestoryOtherClient();
	pAccountGame->AccountStatus.ulOnLineTick = NULL;
	pAccountGame->dwPid = NULL;
	return TRUE;
}

BOOL CConsoleFun::DeleteBnsCefSubProcess()
{
	WCHAR wszBnsCefSubProcessPath[MAX_PATH] = { 0 };

	// ɾ��BnsCefSubProcess.exe
	swprintf_s(wszBnsCefSubProcessPath, _countof(wszBnsCefSubProcessPath), L"%s\\bin\\BnsCefSubProcess.exe", g_pSharedInfo->wszGamePath);
	if (MyTools::CLPublic::FileExit(wszBnsCefSubProcessPath))
		::DeleteFileW(wszBnsCefSubProcessPath);
	return TRUE;
}


BOOL CALLBACK CConsoleFun::EnumGameWnd(_In_ HWND hWnd, _In_ LPARAM lpParam)
{
	if (IsWindow(hWnd) && (GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW) != WS_EX_TOOLWINDOW)
	{
		WCHAR wszClassName[MAX_PATH] = { 0 };
		GetClassNameW(hWnd, wszClassName, _countof(wszClassName) - 1);
		if (MyTools::CCharacter::wstrcmp_my(wszClassName, L"LaunchUnrealUWindowsClient"))
		{
			DWORD PID;
			::GetWindowThreadProcessId(hWnd, &PID);

			if (PID == reinterpret_cast<PACCOUNT_INFO_GAME>(lpParam)->dwPid && IsWindowVisible(hWnd) && IsWindow(hWnd))
			{
				::Sleep(3 * 1000);
				reinterpret_cast<PACCOUNT_INFO_GAME>(lpParam)->hGameWnd = hWnd;
				return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL CConsoleFun::RunGame(_In_ PACCOUNT_INFO_GAME pAccountGame)
{
	BOOL bRetCode = FALSE;
	// ��ʼ��¼֮ǰ�ȳ��Թر�!
	KillGame_By_AccountName(pAccountGame);

	// ��ʼ��
	pAccountGame->MyAcount_Info.bLogin = TRUE;
	pAccountGame->AccountStatus.bLogining = TRUE;
	pAccountGame->AccountStatus.bClose = FALSE;
	pAccountGame->dwPid = NULL;
	pAccountGame->hGameWnd = NULL;
	pAccountGame->AccountStatus.bDone = FALSE;
	pAccountGame->AccountStatus.ulOnLineTick = NULL;
	pAccountGame->AccountStatus.CpuUsage.uCountForCPUUsage = 0;
	pAccountGame->AccountStatus.bStopRespond = FALSE;
	pAccountGame->AccountStatus.dwStopRespondCount = NULL;
	pAccountGame->AccountStatus.bRunNcLanucher = FALSE;
	// ������Ϸ����
	//fnCreateProcess(pAccountGame);
	if (!RunNcLanucher(pAccountGame))
	{
		LOG_CF_D(L"CreateGameProc = FALSE");
		pAccountGame->AccountStatus.uLoginFaildCount += 1;
		pAccountGame->MyAcount_Info.bLogin = FALSE;
		pAccountGame->AccountStatus.bLogining = FALSE;
		pAccountGame->AccountStatus.ulOnLineTick = ::GetTickCount64();
		return FALSE;
	}
	LOG_CF_D(L"Logining...");
	// ��¼����
	//UINT uSpendCount = 0;
	auto dwTick = ::GetTickCount64();
	for (; !bRetCode;)
	{
		if (::GetTickCount64() - dwTick >= 5 * 60 * 1000) // 5���ӳ�ʱ!
		{
			LOG_CF_D( L"����%s ��ʱ", pAccountGame->MyAcount_Info.szUserName);
			pAccountGame->AccountStatus.bLogining = FALSE;
			pAccountGame->MyAcount_Info.bLogin = FALSE;
			bRetCode = FALSE;
			break;
		}
		if (!pAccountGame->MyAcount_Info.bLogin) // bLogin = FALSE ��ʾ��¼���
		{
			LOG_CF_D( L"�ʺ�:%s ��¼���!", pAccountGame->MyAcount_Info.szUserName);
			pAccountGame->AccountStatus.bLogining = FALSE;
			pAccountGame->AccountStatus.ulOnLineTick = ::GetTickCount64();
			bRetCode = TRUE;
			break;
		}
		if (pAccountGame->AccountStatus.bClose)
		{
			LOG_CF_D( L"�ʺ�:%s ��¼ʧ��, bClose=TRUE!", pAccountGame->MyAcount_Info.szUserName);
			pAccountGame->MyAcount_Info.bLogin = FALSE;
			pAccountGame->AccountStatus.bLogining = FALSE;
			bRetCode = FALSE;
			break;
		}
		if (pAccountGame->dwPid != NULL && !MyTools::CLProcess::Is_Exist_Process_For_ProcId(pAccountGame->dwPid)) // �жϽ����Ƿ����
		{
			LOG_CF_D( L"�ʺ�:%s ��¼ʧ��! �Ҳ�������ID:%d!", pAccountGame->MyAcount_Info.szUserName, pAccountGame->dwPid);
			pAccountGame->MyAcount_Info.bLogin = FALSE;
			pAccountGame->AccountStatus.bLogining = FALSE;
			bRetCode = FALSE;
			break;
		}
		//if (++uSpendCount % 60 == 0) // ���
		//	LOG_CF_D( L"����̨�ȴ���¼����Ѿ��ķ�%d������", uSpendCount / 60);;

		if (pAccountGame->hGameWnd == NULL) // ö�ٴ��ڽ�������̨��ֵ!
		{
			for (int i = 0; i < 100 && EnumWindows((WNDENUMPROC)EnumGameWnd,  reinterpret_cast<LPARAM>(pAccountGame)); ++i);
			if (pAccountGame->hGameWnd != NULL) // �ҵ������Լ��Ĵ�����! �����ǳ�����֮��
			{
				LOG_CF_D(L"ע��DLL! �ʺ�:%s, hWnd=%X,PID=%X", pAccountGame->MyAcount_Info.szUserName, pAccountGame->hGameWnd, pAccountGame->dwPid);
				WCHAR wszInjectorPath[MAX_PATH] = { 0 };
				MyTools::CCharacter::wstrcpy_my(wszInjectorPath, g_pSharedInfo->ConsoleContent.szConsolePath, _countof(wszInjectorPath) - 1);
				lstrcatW(wszInjectorPath, g_pSharedInfo->ConsoleContent.GlobalAccount.IsNoModule ? L"\\CopyFile\\InjectorDLL.dll" : L"\\CopyFile\\BnsProjects.dll");
				MyTools::CLProcess::LoadRemoteDLL(pAccountGame->dwPid, wszInjectorPath);
			}
		}

		if (pAccountGame->hGameWnd == NULL && ::GetTickCount64() - dwTick >= 2 * 60 * 1000)
		{
			LOG_CF_E( L"�ʺ�:%s, �����˺�2���Ӷ�û������! ��ʧ�ܴ���!", pAccountGame->MyAcount_Info.szUserName);
			pAccountGame->MyAcount_Info.bLogin = FALSE;
			pAccountGame->AccountStatus.bLogining = FALSE;
			bRetCode = FALSE;
			break;
		}

		Sleep(1000);
	}

	if (!bRetCode)
	{
		LOG_CF_D(L"Kill Game!");
		KillGame_By_AccountName(pAccountGame);
	}


	DestoryOtherClient();
	return bRetCode;
}

BOOL CConsoleFun::RunNcLanucher(_In_ PACCOUNT_INFO_GAME pAccountGame)
{
	BOOL bRetCode = FALSE;
	CNcLanucherLogin NcLanucher;
	auto pAsyncPtr = std::async(std::launch::async, [&]
	{
		auto& LoginLanucher = g_pSharedInfo->ConsoleContent.GlobalAccount.LoginLanucher;
		MyTools::CCharacter::wstrcpy_my(LoginLanucher.wszAccountName, pAccountGame->GetAccName());
		MyTools::CCharacter::wstrcpy_my(LoginLanucher.wszAccountPass, pAccountGame->MyAcount_Info.szPassWord);

		if (NcLanucher.RunNcLauncher(LoginLanucher) == NULL)
		{
			PrintLogToConsole(pAccountGame->GetAccName(), NcLanucher.GetProgressText());
			bRetCode = FALSE;
			return;
		}

		bRetCode = NcLanucher.Start(LoginLanucher);
	});

	while (pAsyncPtr.wait_for(std::chrono::seconds(1)) != future_status::ready)
	{
		PrintLogToConsole(pAccountGame->GetAccName(), NcLanucher.GetProgressText());
	}

	pAccountGame->dwPid = NcLanucher.GetClientPid();
	pAccountGame->AccountStatus.bRunNcLanucher = TRUE;
	LOG_C_D(L"��½���...");
	return bRetCode;
}

UINT CConsoleFun::GetRunAccountCount()
{
	// ��ȡ��ǰ�����е��ʺ�����! bExist && bLogined ��ʾ��������(���ϵ��ߵ�)
	UINT uCount = 0;
	for (auto itr = begin(g_pSharedInfo->arrGameInfo); itr != end(g_pSharedInfo->arrGameInfo); ++itr)
	{
		if (itr->AccountStatus.bExist && itr->AccountStatus.bLogined)
			uCount++;
	}
	return uCount;
}

BOOL CConsoleFun::SetIpAddress() CONST
{
	wchar_t wszPath[MAX_PATH];
	::GetCurrentDirectoryW(MAX_PATH, wszPath);
	lstrcatW(wszPath, L"\\Config.ini");

	WCHAR wszIpType[32];
	::GetPrivateProfileStringW(L"Config", L"Server", L"Error", wszIpType, sizeof(wszIpType) / sizeof(WCHAR), wszPath);
	if (MyTools::CCharacter::wstrcmp_my(wszIpType, L"Error"))
	{
		LOG_MSG_CF(L"��ȡConfig.ini��Server����ʧ��!");
		return FALSE;
	}

	if (MyTools::CCharacter::wstrcmp_my(wszIpType, L"Server"))
		MyTools::CCharacter::strcpy_my(g_pSharedInfo->ConsoleContent.GlobalAccount.szServerIp, SERVER_IP.c_str());
	else if (MyTools::CCharacter::wstrcmp_my(wszIpType, L"Local"))
		MyTools::CCharacter::strcpy_my(g_pSharedInfo->ConsoleContent.GlobalAccount.szServerIp, LOCAL_IP.c_str());
	else if (MyTools::CCharacter::wstrcmp_my(wszIpType, L"Test"))
		MyTools::CCharacter::strcpy_my(g_pSharedInfo->ConsoleContent.GlobalAccount.szServerIp, TEST_IP.c_str());
	else if (MyTools::CCharacter::wstrcmp_my(wszIpType, L"Server2"))
		MyTools::CCharacter::strcpy_my(g_pSharedInfo->ConsoleContent.GlobalAccount.szServerIp, SERVER2_IP.c_str());
	else if (MyTools::CCharacter::wstrcmp_my(wszIpType, L"MY"))
		MyTools::CCharacter::strcpy_my(g_pSharedInfo->ConsoleContent.GlobalAccount.szServerIp, MY_IP.c_str());
	else
	{
		LOG_MSG_CF(L"Config.ini��Server������Ч!");
		return FALSE;
	}

	return TRUE;
}

VOID CConsoleFun::DestoryOtherClient() CONST
{
	auto fnExistPid = [](DWORD dwPid)
	{
		for (int i = 0; i < g_pSharedInfo->nAccountCount; ++i)
		{
			if (g_pSharedInfo->arrGameInfo[i].AccountStatus.bExist && g_pSharedInfo->arrGameInfo[i].dwPid == dwPid)
				return TRUE;
		}
		return FALSE;
	};

	std::vector<PROCESSENTRY32> VecProc;
	MyTools::CLProcess::GetProcessSnapshot(VecProc);

	CBnsProcExtend BnsProcExtend;
	for (CONST auto& itm : VecProc)
	{
		if (MyTools::CCharacter::MakeTextToLower(std::wstring(itm.szExeFile)) == L"client.exe")
		{
			if (!fnExistPid(itm.th32ProcessID))
			{
				BnsProcExtend.KillGameClientByPId(itm.th32ProcessID);
				LOG_CF_D( L"�رշ�ע�봰�ڵ���Ϸ����!, dwPid=%X", itm.th32ProcessID);
				Sleep(2000);
			}
		}
	}
}

VOID CConsoleFun::PrintLogToConsole(_In_ CONST std::wstring& wsAccountName, _In_ CONST std::wstring& wsLogText) CONST
{
	if (_ConsoleLogPtr != nullptr)
	{
		_ConsoleLogPtr(em_ConsoleLog::em_ConsoleLog_Content, wsAccountName, wsLogText);
	}
}

BOOL CConsoleFun::IsEmptyCPUUsage(_In_ PACCOUNT_INFO_GAME pAccountGame) CONST
{
	int nValue = MyTools::CLProcess::GetCpuUsageByPid(pAccountGame->dwPid, pAccountGame->AccountStatus.CpuUsage.llLastTime, pAccountGame->AccountStatus.CpuUsage.llLastSysTime);
	if (nValue == -1)
		return FALSE;
	else if (nValue == 0)
	{
		pAccountGame->AccountStatus.CpuUsage.uCountForCPUUsage += 1;
		LOG_CF_E( L"�ʺ�:%s CPUʹ����=0, ����=%d", pAccountGame->GetAccName(), pAccountGame->AccountStatus.CpuUsage.uCountForCPUUsage);
		return TRUE;
	}
	
	pAccountGame->AccountStatus.CpuUsage.uCountForCPUUsage = 0;
	return FALSE;
}

BOOL CConsoleFun::ISWindowStopRespond(_In_ PACCOUNT_INFO_GAME pAccGame) CONST
{
	if (pAccGame->hGameWnd != NULL && ::IsHungAppWindow(pAccGame->hGameWnd))
	{
		LOG_CF_E( L"��⵽����:[%s,%X] δ��Ӧ!, Count=%d", pAccGame->GetAccName(), pAccGame->dwPid, pAccGame->AccountStatus.dwStopRespondCount);
		if (++pAccGame->AccountStatus.dwStopRespondCount >= 5 * 6)
		{
			pAccGame->AccountStatus.bStopRespond = TRUE;
		}
	}
	else
	{
		pAccGame->AccountStatus.dwStopRespondCount = 0;
	}
	return TRUE;
}

VOID CConsoleFun::SetConsoleLogPtr(_In_ std::function<VOID(em_ConsoleLog, CONST std::wstring &, CONST std::wstring & )> ConsoleLogPtr)
{
	_ConsoleLogPtr = ConsoleLogPtr;
}

BOOL CConsoleFun::LoadNpDLL()
{
	WCHAR wszDLLPath[MAX_PATH] = { 0 };
	::GetCurrentDirectoryW(MAX_PATH, wszDLLPath);
	::lstrcatW(wszDLLPath, L"\\CopyFile\\JLnp_hf.dll");
	return ::LoadLibraryW(wszDLLPath) != NULL;
}

BOOL CConsoleFun::LoadCommonDLL() CONST
{
	auto fnLoadDLLOnCopyFile = [](_In_ CONST std::wstring& wsFileName)
	{
		WCHAR wszPath[MAX_PATH] = { 0 };
		::GetCurrentDirectoryW(MAX_PATH, wszPath);
		::lstrcatW(wszPath, L"\\CopyFile\\");
		::lstrcatW(wszPath, wsFileName.c_str());

		if (!MyTools::CLPublic::FileExist(wszPath))
		{
			LOG_MSG_CF(L"ȱ���ļ�:%s", wszPath);
			return FALSE;
		}

		HMODULE hmDLL = ::LoadLibraryW(wszPath);
		if (hmDLL == NULL)
		{
			LOG_MSG_CF(L"����DLLʧ��:%s", wszPath);
			return FALSE;
		}

		return TRUE;
	};


	static CONST std::vector<std::wstring> VecLoadDLLName = 
	{
		L"VMProtectSDK32.dll"
	};
	
	for (CONST auto& itm : VecLoadDLLName)
	{
		if (!fnLoadDLLOnCopyFile(itm))
			return FALSE;
	}

	return TRUE;
}
