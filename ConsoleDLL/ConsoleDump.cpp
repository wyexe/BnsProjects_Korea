/*
Author:	Vladimir Sedach.

Purpose: demo of Call Stack creation by our own means,
and with MiniDumpWriteDump() function of DbgHelp.dll.
*/
#include "stdafx.h"
#include "ConsoleDump.h"
#include <STDIO.H>
#include <TIME.H>
#include <MyTools/Log.h>
#include <MyTools/Character.h>

#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

BOOL g_bStopDumpFile = FALSE;
WCHAR g_wszDumpPath[MAX_PATH] = { 0 };
WCHAR g_wszDumpAccountName[64] = { 0 };

HMODULE	hDbgHelp;
MINIDUMP_WRITE_DUMP	MiniDumpWriteDump_;

CREATE_TOOL_HELP32_SNAPSHOT	CreateToolhelp32Snapshot_;
MODULE32_FIRST	Module32First_;
MODULE32_NEST	Module32Next_;

#define	DUMP_SIZE_MAX	8000	//max size of our dump
#define	CALL_TRACE_MAX	((DUMP_SIZE_MAX - 2000) / (MAX_PATH + 40))	//max number of traced calls

#define _SELF L"Dump.cpp"
LONG WINAPI CrashReportEx(LPEXCEPTION_POINTERS ExceptionInfo)
{
	//char	szFileName[MAX_PATH] = {0x00};
	//UINT	nRet = 0;

	// 重启程序
	// 	::GetModuleFileName(NULL, szFileName, MAX_PATH);
	// 	nRet = WinExec(szFileName, SW_SHOW);

	// 创建DUMP文件
	MessageBoxW(NULL, L"程序崩溃", L"", NULL);
	Create_Dump(ExceptionInfo);
	LOG_CF_D(L"Crash Client...");
	MyTools::CLog::GetInstance().SaveLog_Immediately(TRUE);
	MessageBoxW(NULL, L"程序崩溃,创建转存文件成功!", L"", NULL);
	return EXCEPTION_EXECUTE_HANDLER;
}
void RegDumpFunction()
{
	SetUnhandledExceptionFilter(CrashReportEx);
	HMODULE	hKernel32;

	// Try to get MiniDumpWriteDump() address.
	hDbgHelp = LoadLibraryA("DBGHELP.DLL");
	MiniDumpWriteDump_ = (MINIDUMP_WRITE_DUMP)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");
	//	d("hDbgHelp=%X, MiniDumpWriteDump_=%X", hDbgHelp, MiniDumpWriteDump_);

	// Try to get Tool Help library functions.
	hKernel32 = GetModuleHandleA("KERNEL32");
	CreateToolhelp32Snapshot_ = (CREATE_TOOL_HELP32_SNAPSHOT)GetProcAddress(hKernel32, "CreateToolhelp32Snapshot");
	Module32First_ = (MODULE32_FIRST)GetProcAddress(hKernel32, "Module32First");
	Module32Next_ = (MODULE32_NEST)GetProcAddress(hKernel32, "Module32Next");
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void WINAPI Create_Dump(PEXCEPTION_POINTERS pException)
{
	auto wsDumpFilePath = MyTools::CCharacter::MakeFormatText(L"%s\\DumpLog\\%s.dmp", g_wszDumpPath, g_wszDumpAccountName);
	MINIDUMP_EXCEPTION_INFORMATION	M;

	M.ThreadId = GetCurrentThreadId();
	M.ExceptionPointers = pException;
	M.ClientPointers = 0;

	HANDLE hDump_File = CreateFile(wsDumpFilePath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	MiniDumpWriteDump_(GetCurrentProcess(), GetCurrentProcessId(), hDump_File, MiniDumpNormal, (pException) ? &M : NULL, NULL, NULL);
	CloseHandle(hDump_File);

}
