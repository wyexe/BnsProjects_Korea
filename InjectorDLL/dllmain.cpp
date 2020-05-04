// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <MyTools/Character.h>
#include <MyTools/CLResManager.h>
#include <MyTools/CLPublic.h>
#include <MyTools/CLFile.h>
#include "Shlwapi.h"
#include <MyTools/CLNoModuleDLL.h>

BOOL SetDLLPath(HMODULE hModule)
{
	WCHAR wszPath[MAX_PATH] = { 0 };
	::GetModuleFileNameW(hModule, wszPath, MAX_PATH);

	::PathRemoveFileSpecW(wszPath);
	std::wstring wsDLLPath = MyTools::CCharacter::MakeFormatText(L"%s\\BnsProjects.dll", wszPath);
	if (!MyTools::CLPublic::FileExist(wsDLLPath))
	{
		::MessageBoxW(NULL, L"BnsProjects.dll 不存在!", L"Error", NULL);
		return FALSE;
	}
	
	LPVOID pvFileContent = nullptr;
	UINT uFileSize = 0;

	if (!MyTools::CLFile::ReadFileContent(wsDLLPath, pvFileContent, uFileSize))
	{
		::MessageBoxW(NULL, L"读取BnsProjects.dll 失败!", L"Error", NULL);
		return FALSE;
	}

	MyTools::CLNoModuleDLL NoModuleDLL;
	if (!NoModuleDLL.LoadMemoryDLL(pvFileContent, uFileSize))
	{
		::TerminateProcess(::GetCurrentProcess(), 0);
		MessageBoxW(NULL, L"测试模式: 加载BnsProjects.dll 失败 等待自动关闭即可!", L"Error", NULL);
		return FALSE;
	}

	NoModuleDLL.ErasePeMark(pvFileContent);
	return TRUE;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		SetDLLPath(hModule);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

