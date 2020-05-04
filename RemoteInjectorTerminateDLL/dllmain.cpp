// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <shellapi.h>
#include <string>

#ifndef _WIN64
#error "Must Comipler in x64"
#endif // !_WIN64

HANDLE hThread = NULL;
DWORD WINAPI _WorkThread(LPVOID)
{
	int nArgsCount = 0;
	LPWSTR* pwszArrayText = CommandLineToArgvW(GetCommandLineW(), &nArgsCount);
	if (nArgsCount > 1)
	{
		// [0] = CurrentPath
		// [1] = dwPid
		DWORD dwPid = static_cast<DWORD>(_wtoi(pwszArrayText[1]));

		HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, dwPid);
		if (hProcess != NULL)
		{
			::TerminateProcess(hProcess, 0);
			::WaitForSingleObject(hProcess, 3000);
			::CloseHandle(hProcess);
		}
	}


	return 0;
}


BOOL APIENTRY DllMain( HMODULE ,
                       DWORD  ul_reason_for_call,
                       LPVOID 
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		if (hThread == NULL)
		{
			hThread = ::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)_WorkThread, NULL, NULL, NULL);
		}
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

