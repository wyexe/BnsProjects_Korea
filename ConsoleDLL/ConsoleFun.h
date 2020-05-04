#ifndef __GITBNSPROJECTS_CONSOLEDLL_CONSOLEFUN_H__
#define __GITBNSPROJECTS_CONSOLEDLL_CONSOLEFUN_H__

#include <atomic>
#include "ConsoleVariable.h"
#include "../BnsProjects/BnsConfig.h"

class CConsoleFun : public MyTools::CClassInstance<CConsoleFun>
{
public:
	CConsoleFun();
	~CConsoleFun() = default;

	// 初始化
	BOOL Init();

	// 初始化内存共享
	BOOL InitMapView();

	// 关闭游戏
	BOOL KillGame_By_AccountName(_In_ PACCOUNT_INFO_GAME pAccountGame);
	BOOL DeleteBnsCefSubProcess();

	// 启动游戏
	BOOL RunGame(_In_ PACCOUNT_INFO_GAME pAccountGame);

	//
	BOOL RunNcLanucher(_In_ PACCOUNT_INFO_GAME pAccountGame);

	// 获取在线帐号数量
	UINT GetRunAccountCount();

	// 读取IP
	BOOL SetIpAddress() CONST;

	// 是否CPU使用率=0
	BOOL IsEmptyCPUUsage(_In_ PACCOUNT_INFO_GAME pAccountGame) CONST;

	// 是否窗口未响应
	BOOL ISWindowStopRespond(_In_ PACCOUNT_INFO_GAME pAccGame) CONST;

	//
	VOID SetConsoleLogPtr(_In_ std::function<VOID(em_ConsoleLog, CONST std::wstring&, CONST std::wstring&)> ConsoleLogPtr);

	//
	BOOL LoadNpDLL();

	//
	BOOL LoadCommonDLL() CONST;
private:
	VOID DestoryOtherClient() CONST;

	VOID PrintLogToConsole(_In_ CONST std::wstring& wsAccountName, _In_ CONST std::wstring& wsLogText) CONST;

	static BOOL CALLBACK EnumGameWnd(_In_ HWND hWnd, _In_ LPARAM lpParam);
public:
	DSIABLE_COPY_AND_ASSIGN(CConsoleFun);
private:
	HANDLE	hFileMap = NULL;
	MyTools::CLLock Lock_ConsoleFun;
	std::function<VOID(em_ConsoleLog, CONST std::wstring&, CONST std::wstring&)> _ConsoleLogPtr = nullptr;
};


#endif