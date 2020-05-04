#include "stdafx.h"
#include "SomeFun.h"
#include <psapi.h>
#include <thread>
#include <MyTools/CLSearchBase.h>
#include <MyTools/CLProcess.h>
#include <MyTools/CLResManager.h>
#include <MyTools/Log.h>
#include "FarmSkill.h"
#include "ObjectFunction.h"
#include "PersonFunction.h"
#include "BnsFilter.h"
#include "Person.h"
#include "BnsConfig.h"
#include "GameUI.h"
#include "BnsGameFun.h"
#include "GameSend.h"
#include "ConsoleLog.h"
#pragma comment(lib,"psapi.lib")
#if BnsProject_DLL
#include "../ConsoleDLL/SocketAPI.h"
#include "MiniDump.h"
#endif

#define _SELF L"SomeFun.cpp"

BOOL CSomeFun::InitDLL() CONST
{
	///////////坐标///////////////////////////////////////////////////////////////
	g_dwMoveBase = MyTools::CLSearchBase::FindBase("8b0885c97433", 0x102C4B18 - 0x102C4B00, 0x1, 0x0, L"bsengine_Shipping.dll", 0xFFFFFFFF);
	if (g_dwMoveBase == NULL)
	{
		LOG_MSG_CF( L"InitDLL 失败 g_dwMoveBase = NULL!");
		return FALSE;
	}

	////////模拟按键//////////////////////////////////////////////////////////////////
	g_dwSimulationBase = MyTools::CLSearchBase::FindBase("d944240c518b0cb0", 0x073DBB58 - 0x073DBB53, 0x1, 0x0, L"bsengine_Shipping.dll", 0xFFFFFFFF);
	if (g_dwSimulationBase == NULL)
	{
		LOG_MSG_CF( L"InitDLL 失败 g_dwSimulationBase = NULL!");
		return FALSE;
	}

	DWORD dwDLLModule = (DWORD)::GetModuleHandleW(L"bsengine_Shipping.dll");
	g_dwSimulationCALL = MyTools::CLSearchBase::FindCALL("bba00000008b4e", 0x073DA57A - 0x073DA586, dwDLLModule, 0x1, 0x0, L"bsengine_Shipping.dll");
	if (g_dwSimulationCALL == NULL)
	{
		LOG_MSG_CF( L"InitDLL 失败 g_dwSimulationCALL = NULL!");
		return FALSE;
	}

	///////优化///////////////////////////////////////////////////////////////////
	/*DWORD dwOptimizationBase = CLSearchBase::FindBase("83CDFF896C243C8B", 0xA506A32 - 0x0A506A39, 0x2, 0x0, L"bsengine_Shipping.dll", 0xFFFFFFFF);
	if (dwOptimizationBase == NULL)
	{
		LOG_MSG_CF( L"InitDLL 失败 dwOptimizationBase = NULL!");
		return FALSE;
	}*/

	g_dwMoveStatusOffset = MyTools::CLSearchBase::FindBase("8A40183C020F84????????3C030F84????????8B", 0x09E19F75 - 0x09E19F67, 0x2, 0, L"bsengine_Shipping.dll", 0xFFFFFFFF);
	if (g_dwMoveStatusOffset == NULL)
	{
		LOG_MSG_CF( L"InitDLL 失败 g_dwMoveStatusOffset = NULL!");
		return FALSE;
	}

	//g_dwHookD3dAddr = ReadDWORD(ReadDWORD(dwOptimizationBase)) + 0x118;
	//g_dwOldD3dCall = ReadDWORD(g_dwHookD3dAddr);

	////////加速//////////////////////////////////////////////////////////////////
	g_dwHookSpeedAddr = ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(g_dwMoveBase) + 人物坐标偏移1) + 0x0) + 人物坐标偏移2) + 人物坐标偏移3) + 0x0) + 0x178;
	g_dwSpeedCALL = ReadDWORD(g_dwHookSpeedAddr);

	MyTools::CCharacter::s_ReadMemoryType = MyTools::ReadMemoryType::Read_API;
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_FOODDIS, 5);
	return TRUE;
}



BOOL CSomeFun::InitMapView() CONST
{
	HANDLE hFileMap = ::OpenFileMappingW(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, SZFILE_NAME_SHAREDINFO);
	if (hFileMap == NULL)
		return FALSE;

	CGameVariable::GetInstance().GetShareInfo() = (PSHARED_INFO)MapViewOfFile(hFileMap, FILE_MAP_READ | FILE_MAP_WRITE, NULL, NULL, sizeof(SHARED_INFO));
	if (CGameVariable::GetInstance().GetShareInfo() == nullptr)
	{
		::CloseHandle(hFileMap);
		hFileMap = NULL;
		return FALSE;
	}

	return InitAccountGame();
}

BOOL CSomeFun::InitAccountGame() CONST
{
	// 初始化全局变量
	if (CGameVariable::GetInstance().GetShareInfo() != nullptr)
	{
		CGameVariable::GetInstance().GetAccShareInfo() = CGameVariable::GetInstance().GetShareInfo()->GetCurrentAccountGame(::GetCurrentProcessId());
	}

	if (CGameVariable::GetInstance().GetAccShareInfo() == nullptr)
	{
		LOG_MSG_CF( L"CGameVariable::GetInstance().GetAccShareInfo() = NULL");
		return FALSE;
	}

	while (CGameVariable::GetInstance().GetAccShareInfo()->hGameWnd == NULL)
		::Sleep(3 * 1000);

	return TRUE;
}

DWORD WINAPI KeepServerAliveThread(LPVOID lpParm)
{
#if BnsProject_DLL
	for (;;)
	{
		::Sleep(10 * 1000);
		CSomeFun::GetInstance().SendKeepALive(nullptr, L"");
	}
#else
	return 0;
#endif
}

VOID CSomeFun::CreateKeepServerAliveThread() CONST
{
	static HANDLE hKeepServerALiveThread = NULL;
	if (hKeepServerALiveThread == NULL)
	{
		LOG_CF_D( L"创建KeepServerAliveThread线程!");
		HANDLE hKeepALiveThread = cbBEGINTHREADEX(NULL, NULL, KeepALiveThread, NULL, NULL, NULL);
		hKeepServerALiveThread = cbBEGINTHREADEX(NULL, NULL, KeepServerAliveThread, NULL, NULL, NULL);

		SetResDeleter(hKeepServerALiveThread, [](HANDLE& hThread) { ::CloseHandle(hThread); });
		SetResDeleter(hKeepALiveThread, [](HANDLE& hThread) { ::CloseHandle(hThread); });
	}
}

VOID CSomeFun::SendKeepALive(_In_ TaskInfoParm* pTaskParam, _In_ CONST std::wstring& wsContent) CONST
{
#if BnsProject_DLL
	KeepALiveContent Content;
	Content.Level = static_cast<BYTE>(CPerson::GetInstance().GetLevel());
	Content.Gold = CPerson::GetInstance().GetMoney();
	Content.wsContent = wsContent;

	if (pTaskParam != nullptr)
	{
		Content.dwTaskId = pTaskParam->dwTaskId;
		Content.dwScheduleId = pTaskParam->dwScheduleId;
	}

	CSocketAPI().KeepALive(&Content);
#endif
}

unsigned CSomeFun::KeepALiveThread(LPVOID lpParm)
{
	for (;;)
	{
		::Sleep(1000);
		if (CGameVariable::GetInstance().GetAccShareInfo() != nullptr)
		{
			CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->bAlive = !g_bExitGame;
		}

		auto emLoseConnect = CPerson::GetInstance().IsLoseConnect();
		if (emLoseConnect != CPerson::em_LoseConnect_Reason::em_LoseConnect_Reason_Normal)
		{
			CConsoleLog(CPerson::GetText_By_LoseConnectReason(emLoseConnect));

			// Sleep 2s 等待中间那个断开连接弹窗完全出来
			::Sleep(2 * 1000);
			std::wstring wsPicPath;
			CBnsConfig::GetInstance().SaveScreenshot(CGameVariable::GetInstance().GetAccShareInfo(), wsPicPath);
			CBnsGameFun::GetInstance().SetCloseGame(MyTools::CCharacter::MakeFormatText(L"%s, 截图路径:%s", CPerson::GetText_By_LoseConnectReason(emLoseConnect).c_str(), wsPicPath.c_str()), em_Close_Result::em_Close_Result_ReStart);
		}

		PROCESS_MEMORY_COUNTERS pmc;
		GetProcessMemoryInfo(::GetCurrentProcess(), &pmc, sizeof(pmc));
		// WorkSet more then 500M
		if (pmc.WorkingSetSize / 1024 / 1024 >= 1024)
		{
			LOG_CF_D( L"开始清理内存!");
			MyTools::CLProcess::ClearWorkingMemory();
		}

		/*if (GameRun && !CPersonFunction::GetInstance().IsGameLoad() && uCount++ >= 60 && CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_GUARD_TIMEOUTTHREAD) != FALSE)
		{
			// 检测超时线程是否在正常运行!
			DWORD dwTimeOutCount = CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_GUARD_TIMEOUTTHREAD);
			::Sleep(5 * 1000);
			DWORD dwNewTimeOutCount = CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_GUARD_TIMEOUTTHREAD);
			if (GameRun && dwNewTimeOutCount >= 0x1 && dwTimeOutCount >= dwNewTimeOutCount && !CPersonFunction::GetInstance().IsGameLoad())
			{
				CBnsGameFun::GetInstance().WriteConsoleLog(L"检测超时线程异常原因被关闭了!");
				CBnsConfig::GetInstance().WriteAccountLog( L"检测超时线程异常原因被关闭了!");
				CBnsGameFun::GetInstance().SetCloseGame(em_Close_Result::em_Close_Result_ReStart);
			}
		}*/

		if (CPersonFunction::GetInstance().IsGameChart())
		{
			LOG_CF_D( L"出现动画!, 按ESC!");
			//CSomeFun::GetInstance().SimlationKey(VK_ESCAPE);
			MyTools::CLPublic::SimulationKey(VK_ESCAPE, CGameVariable::GetInstance().GetAccShareInfo()->hGameWnd);
			continue;
		}
	}
}


BOOL CSomeFun::SimlationKey(_In_ DWORD dwAscii, _In_ DWORD dwSleepTime /* = 200 */, _In_ DWORD dwAfterSleepTime /* = NULL */) CONST
{
	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&dwAscii]{CGameSend::SimulationKey(dwAscii, TRUE); });
	BnsSleep(dwSleepTime);
	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&dwAscii]{CGameSend::SimulationKey(dwAscii, FALSE); });
	BnsSleep(dwAfterSleepTime);
	return TRUE;
}

VOID CSomeFun::SimlationKey_Immediately(_In_ DWORD dwAscii) CONST
{
	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&dwAscii]
	{
		CGameSend::SimulationKey(dwAscii, TRUE);
		CGameSend::SimulationKey(dwAscii, FALSE);
	});
}

VOID CSomeFun::PushSimlationKey(_In_ DWORD dwAscii) CONST
{
	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([=]{CGameSend::SimulationKey(dwAscii, TRUE); });
}

VOID CSomeFun::SetSpeed(__in float fSpeed) CONST
{
#if BnsProject_DLL
	
	/*if (MyTools::CCharacter::wstrcmp_my(CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.wszTypeName, L"保号召唤模式") ||
		MyTools::CCharacter::wstrcmp_my(CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.wszTypeName, L"召唤50级加速模式") ||
		MyTools::CCharacter::wstrcmp_my(CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.wszTypeName, L"剑士模式"))
	{
		//LOG_CF_D( L"当前是保号召唤模式，加速还原。");
		if (g_fSpeed == fSpeed)
		{
			//LOG_CF_D( L"其他");
			return;
		}
		else
		{
			g_fSpeed = fSpeed;
			return;
		}
	}
	else
	{
		//LOG_CF_D( L"当前输入加速=%f",fSpeed);
		if (fSpeed > 3)
		{
			if (g_fSpeed == fSpeed)
			{
				//LOG_CF_D( L"其他");
				return;
			}
			else
			{
				g_fSpeed = fSpeed;
				return;
			}
		}
		else
		{
			//LOG_CF_D( L"1");
			g_fSpeed = 1.0f;
			return;
		}
	}*/
#else
	//LOG_C_D(L"当前设置加速=%.2f", fSpeed);
	g_fSpeed = fSpeed;
#endif
}

float CSomeFun::GetSpeed() CONST
{
	return g_fSpeed;
}


VOID CSomeFun::SetOptimization(__in DWORD dwSleepTime) CONST
{
	g_dwOptimizationTime = dwSleepTime;
}

BOOL CSomeFun::Sleep(__in DWORD dwSleepTime) CONST
{
	for (DWORD i = 0; i <= dwSleepTime && GameRun; i += 100)
		::Sleep(100);

	return TRUE;
}


VOID CSomeFun::SwitchViewOfAngel() CONST
{
	if (ReadDWORD(视角基址) == 0x1)
		return;

	if (CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_DISABLE_SWITCHVIEW) == 0x1)
		return;

	CGameUi TalkControlPanel;
	if (CObjectFunction::GetInstance().IsShow_In_GameUi(L"TalkControlPanel"))
	{
		LOG_CF_D(L"切换视角的时候被Npc对话锁定了!");
		CSomeFun::GetInstance().SimlationKey(VK_ESCAPE, 200, 800);
	}

	HANDLE hMutex = ::CreateMutexW(NULL, FALSE, L"CL_BNS_KOREA_SWITCHVIEWOFANGEL");
	if (hMutex == INVALID_HANDLE_VALUE || hMutex == NULL)
	{
		LOG_MSG_CF( L"CreateMutexW Faild!");
		return;
	}

	LOG_CF_D(L"等待排队切换视角!");
	WaitForSingleObject(hMutex, 10 * 1000);
	SetResDeleter(hMutex, [](HANDLE& hThread){::CloseHandle(hThread); });

	//LOG_CF_D( L"现在视角=%s", ReadDWORD(dwAddr) == NULL ? L"剑灵视角" : L"正常视角");
	::ShowWindow(CGameVariable::GetInstance().GetAccShareInfo()->hGameWnd, SW_MINIMIZE);
	::SendMessageW(CGameVariable::GetInstance().GetAccShareInfo()->hGameWnd, WM_SYSCOMMAND, SC_MINIMIZE, NULL);
	BnsSleep(1000);

	::ShowWindow(CGameVariable::GetInstance().GetAccShareInfo()->hGameWnd, SW_RESTORE);
	::SetWindowPos(CGameVariable::GetInstance().GetAccShareInfo()->hGameWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	SwitchToThisWindow(CGameVariable::GetInstance().GetAccShareInfo()->hGameWnd, TRUE);
	BnsSleep(1000);

	// 发送按键!
	MyTools::CLPublic::SendAscii(VK_F2, TRUE);
	BnsSleep(1000);

	::ShowWindow(CGameVariable::GetInstance().GetAccShareInfo()->hGameWnd, SW_NORMAL);
	::SetWindowPos(CGameVariable::GetInstance().GetAccShareInfo()->hGameWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	LOG_CF_D(L"切换视角完毕!");
}

BOOL CSomeFun::SetGameTitle() CONST
{
	WCHAR wszText[128];
	swprintf_s(wszText, _countof(wszText) - 1, L"第%d个角色 %s", CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->dwOrderIndex, CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->szUserName);
	return ::SetWindowTextW(CGameVariable::GetInstance().GetAccShareInfo()->hGameWnd, wszText);
}

BOOL CSomeFun::WaitToDo(DWORD dwSleepTime, DWORD dwMaxSleepTime, std::function<BOOL(void)> f) CONST
{
	BnsSleep(dwSleepTime);

	DWORD dwTick = ::GetTickCount();
	while (GameRun && !EMPTY_PERSONHP && ::GetTickCount() - dwTick <= dwMaxSleepTime && f())
		BnsSleep(100);

	return BnsSleep(dwSleepTime);
}

BOOL CSomeFun::TimeOut_Condiction(DWORD dwMaxTimeOut, std::function<BOOL(void)> fnCondiction) CONST
{
	DWORD dwTick = ::GetTickCount();
	while (fnCondiction())
	{
		if (::GetTickCount() - dwTick >= dwMaxTimeOut)
			return TRUE;

		::Sleep(100);
	}
	return FALSE;
}

BOOL CSomeFun::TimeOut_Condiction_GameRun(DWORD dwMaxTimeOut, std::function<BOOL(void)> fnCondiction) CONST
{
	DWORD dwTick = ::GetTickCount();
	while (fnCondiction() && GameRun)
	{
		if (::GetTickCount() - dwTick >= dwMaxTimeOut)
			return TRUE;

		::Sleep(100);
	}
	return FALSE;
}

Point CSomeFun::GetPoint_By_ScriptParm(CONST std::wstring& cwstr)
{
	std::vector<std::wstring> vlst;
	Point Pt;

	if (MyTools::CCharacter::Split(cwstr, L",", vlst, Split_Option_RemoveEmptyEntries | Split_Option_KeepOnly) == 3)
	{
		Pt.X = static_cast<float>(_wtof(vlst.at(0).c_str()));
		Pt.Y = static_cast<float>(_wtof(vlst.at(1).c_str()));
		Pt.Z = static_cast<float>(_wtof(vlst.at(2).c_str()));
	}
	else
		LOG_MSG_CF( L"脚本错误! 当前参数是:%s, 无法转换成Point", cwstr.c_str());

	return Pt;
}