#include "stdafx.h"
#include "BnsProcExtend.h"
#include <future>
#include <MyTools/CLProcess.h>
#include <MyTools/Character.h>
#include <MyTools/Log.h>
#define _SELF L"BnsProcExtend.cpp"
VOID CBnsProcExtend::DestoryBnsClient() CONST
{
	auto AsyncPtr = std::async(launch::async, [this] 
	{
		std::vector<DWORD> VecPid;
		GetVecBnsProcId(VecPid);

		while (VecPid.size() != 0)
		{
			for (auto& dwPid : VecPid)
				KillGameClientByPId(dwPid);

			VecPid.clear();
			GetVecBnsProcId(VecPid);
		}
	});
	

	MyTools::CLProcess::TerminateProc_For_ProcName(L"NCLauncherS.exe");
	AsyncPtr.wait();
}

BOOL CBnsProcExtend::KillGameClientByPId(_In_ DWORD dwPid) CONST
{
	HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, dwPid);
	if (hProcess == NULL)
	{
		LOG_CF_E(L"打开进程ID[%X] 失败", dwPid);
		return FALSE;
	}

	::TerminateProcess(hProcess, 0);
	::Sleep(500);


	if (MyTools::CLProcess::Is_Exist_Process_For_ProcId(dwPid))
	{
		LOG_CF_E(L"第一次结束进程失败!尝试第二次结束进程!");
		MyTools::CLProcess::TerminateProc_By_UnLoad_NtDLL(dwPid);
		::Sleep(3000);
	}

	if (MyTools::CLProcess::Is_Exist_Process_For_ProcId(dwPid))
	{
		std::vector<PROCESSENTRY32> VecProc;
		MyTools::CLProcess::GetProcessSnapshot(VecProc);
		for (auto& itm : VecProc)
		{
			LOG_CF_D(L"ProcID=%X, Name:%s", itm.th32ProcessID, itm.szExeFile);
		}
		//临时关闭下,不然会卡着其他号都不登录了
		//LOG_MSG_CF(L"游戏进程:%d 可能卡死了，任务管理器里手动结束一下!", dwPid);
		return FALSE;
	}

	return TRUE;
}

VOID CBnsProcExtend::GetVecBnsProcId(_Out_opt_ std::vector<DWORD>& VecPid) CONST
{
	HANDLE hThSnap32 = NULL;
	PROCESSENTRY32W pe32;

	hThSnap32 = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hThSnap32 == INVALID_HANDLE_VALUE)
		return;

	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32FirstW(hThSnap32, &pe32))
	{
		::CloseHandle(hThSnap32);
		return;
	}

	do
	{
		if (MyTools::CCharacter::MakeTextToLower(std::wstring(pe32.szExeFile)) == L"client.exe")
		{
			VecPid.push_back(pe32.th32ProcessID);
			continue;
		}

	} while (Process32NextW(hThSnap32, &pe32));
	::CloseHandle(hThSnap32);
	
	return;
}
