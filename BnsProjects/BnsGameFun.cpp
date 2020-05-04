#include "stdafx.h"
#include "BnsGameFun.h"
#include <process.h>
#include <MyTools/CLSearchBase.h>
#include <MyTools/CLThread.h>
#include <MyTools/CLFile.h>
#include <MyTools/Log.h>
#include <MyTools/CLEchoException.h>
#include "PersonFunction.h"
#include "MiniDump.h"
#include "GameFunction.h"
#include "SomeFun.h"
#include "LoginPlayerExtend.h"
#include "BnsScript.h"
#include "Person.h"
#include "GameUI.h"
#include "ObjectFunction.h"
#include "BnsConfig.h"
#include "LoginPlayer.h"
#include "Task.h"
#include "Cat.h"
#include "CatExpend.h"
#include "GameSend.h"
#include "Farm.h"
#include "ConsoleLog.h"
#include "TimeoutCheck.h"
#if BnsProject_DLL
#include "TransferScript.h"
#endif

#define _SELF L"BnsGameFun.cpp"

unsigned CBnsGameFun::ExcuteScriptThread(LPVOID lpParm)
{
	std::shared_ptr<CBnsScript> pBnsScript = std::make_shared<CBnsScript>();
#if BnsProject_DLL
	CTransferScript TransferScript;
	std::wstring wsScriptContent = TransferScript.DownLoadScript(CGameVariable::GetInstance().CurrentScriptName());
	if (!pBnsScript->Read(CBnsScript::em_ReadScript_Type_Text, wsScriptContent))
	{
		LOG_MSG_CF( L"脚本解析失败:%s!", CGameVariable::GetInstance().CurrentScriptName().c_str());
		CBnsScript::ReleaseBnsScript();
		return 0;
	}

#else
	WCHAR wszScriptPath[MAX_PATH] = { 0 };

	MyTools::CCharacter::wstrcpy_my(wszScriptPath, CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.szConsolePath, _countof(wszScriptPath) - 1);
	wcscat_s(wszScriptPath, _countof(wszScriptPath) - 1, L"\\Script\\");
	wcscat_s(wszScriptPath, _countof(wszScriptPath) - 1, CGameVariable::GetInstance().CurrentScriptName().c_str());

	if (!pBnsScript->Read(CBnsScript::em_ReadScript_Type_FilePath, wszScriptPath))
	{
		LOG_MSG_CF( L"脚本:%s 不存在!", wszScriptPath);
		CBnsScript::ReleaseBnsScript();
		return 0;
	}
	LOG_CF_D( L"执行脚本:%s", wszScriptPath);
#endif
	
	BOOL bRetCode = FALSE;
	do 
	{
		bRetCode = pBnsScript->Excute(L"main");
		if (EMPTY_PERSONHP)
		{
			LOG_CF_D( L"人物挂了,复活!");
			CPersonFunction::GetInstance().Resurrect();
			continue;
		}
		LOG_CF_D( L"BnsScript.Excute=%X", bRetCode);
	} while (!bRetCode);
	return CBnsScript::ReleaseBnsScript();
}

BOOL CALLBACK CBnsGameFun::EnumSelfWnd(_In_ HWND hWnd, _In_ LPARAM lParam)
{
	DWORD dwPid = 0;
	::GetWindowThreadProcessId(hWnd, &dwPid);
	if (dwPid == reinterpret_cast<PACCOUNT_INFO_GAME>(lParam)->dwPid)
	{
		reinterpret_cast<PACCOUNT_INFO_GAME>(lParam)->hGameWnd = hWnd;
		return FALSE;
	}

	return TRUE;
}

static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)  // do not process message 
		return CallNextHookEx(CBnsGameFun::GetHHOK_HotKey(), nCode, wParam, lParam);

	if (nCode >= HC_ACTION && lParam & 0x80000000)
	{
		if (wParam == VK_HOME)
		{
			LOG_CF_D( L"热键Hook 开始挂机!");
			CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->GameCmd = em_GameCmd_Start;
		}
		else if (wParam == VK_END)
		{
			LOG_CF_D(L"热键Hook 停止挂机!");
			CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->GameCmd = em_GameCmd_Stop;
		}
		else if (wParam == VK_INSERT)
		{
			LOG_CF_D( L"热键Hook 创建角色!");
			CBnsGameFun::GetInstance().CreatePlayer_By_HotKey();
		}
		else if (wParam == VK_DELETE)
		{
			LOG_CF_D( L"热键Hook 删除角色!");
			CBnsGameFun::GetInstance().DeletePlayer_By_HotKey();
		}
		else if (wParam == VK_NEXT) // PgDn
		{
			LOG_CF_D(L"更新角色名!");
			CConsoleLog(MyTools::CCharacter::MakeFormatText(L"角色名:%s,加速:%.2f", CPerson::GetInstance().GetName().c_str(), CSomeFun::GetInstance().GetSpeed()));
		}
		else if (wParam == VK_PRIOR) // PgUp
		{

		}
	}

	return CallNextHookEx(CBnsGameFun::GetHHOK_HotKey(), nCode, wParam, lParam);
}

HHOOK& CBnsGameFun::GetHHOK_HotKey()
{
	static HHOOK hHook = NULL;
	return hHook;
}

BOOL CBnsGameFun::WaitForThread(_In_ _em_Excute_Type emType) CONST
{
	// 安装钩子
	if (GetHHOK_HotKey() == NULL)
		GetHHOK_HotKey() = ::SetWindowsHookExW(WH_KEYBOARD, KeyboardProc, NULL, MyTools::CLThread::GetMainThreadId());

	// 开始做任务
	BOOL bRunTimeout = TRUE;
	CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->GameCmd = em_GameCmd_Start;
	InitScriptParmeter();
	
	switch (emType)
	{
	case CBnsGameFun::em_Excute_Type_Warehouse:
		CGameVariable::GetInstance().CurrentScriptName() = CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.Warehouse.wszScriptName;
		break;
	case CBnsGameFun::em_Excute_Type_DeletePlayer:
		return TRUE;
	case CBnsGameFun::em_Excute_Type_Mail:
		CGameVariable::GetInstance().CurrentScriptName() = CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.Mail.wszScriptName;
		break;
	default:
		CGameVariable::GetInstance().CurrentScriptName() = CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.wszScriptName;
		break;
	}

	CConsoleLog(MyTools::CCharacter::MakeFormatText(L"执行脚本:%s", CGameVariable::GetInstance().CurrentScriptName().c_str()));
	HANDLE hThread = cbBEGINTHREADEX(NULL, NULL, ExcuteScriptThread, NULL, NULL, NULL);
	HANDLE hTimeOutThread = cbBEGINTHREADEX(NULL, NULL, _TimeoutThread, &bRunTimeout, NULL, NULL);

	auto fnTermiate = [this, &hThread, &bRunTimeout, &hTimeOutThread]
	{
		LOG_CF_D( L"停止挂机! Value=%d", g_GameStatus);
		CConsoleLog(L"停止挂机中……");
		bRunTimeout = FALSE;
		WaitForSingleObject(hThread, INFINITE);
		WaitForSingleObject(hTimeOutThread, INFINITE);
		::CloseHandle(hThread);
		::CloseHandle(hTimeOutThread);
		CConsoleLog(L"成功停止挂机!");
		hThread = NULL;
		hTimeOutThread = NULL;
	};

	auto fnRun = [&hThread, &bRunTimeout, &emType, this, &hTimeOutThread]
	{
		if (hThread == NULL)
		{
			bRunTimeout = TRUE;
			LOG_CF_D( L"正在尝试重新挂机!");
			switch (emType)
			{
			case CBnsGameFun::_em_Excute_Type_None:
				break;
			case CBnsGameFun::em_Excute_Type_PK_TaskUpgrade:
				CConsoleLog(MyTools::CCharacter::MakeFormatText(L"练级阶段: %d个角色", CGameVariable::GetInstance().GetAccShareInfo()->GetOrderIndex() + 1));
				break;
			case CBnsGameFun::em_Excute_Type_PK_PK:
				CConsoleLog(MyTools::CCharacter::MakeFormatText(L"PK阶段: %d个角色", CGameVariable::GetInstance().GetAccShareInfo()->GetOrderIndex() + 1));
				break;
			default:
				break;
			}
			StartGame;
			hThread = cbBEGINTHREADEX(NULL, NULL, ExcuteScriptThread, NULL, NULL, NULL);
			hTimeOutThread = cbBEGINTHREADEX(NULL, NULL, _TimeoutThread, &bRunTimeout, NULL, NULL);
			LOG_CF_D( L"重新挂机完毕!");
		}
	};

	LOG_CF_D( L"emForceMail=%d", CGameVariable::GetInstance().GetAccShareInfo()->AccountStatus.emForceMail);
	for (;;)
	{
		if (CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->GameCmd != em_GameCmd::em_GameCmd_Stop && hThread != NULL && WaitForSingleObject(hThread, 1000) == WAIT_OBJECT_0)
		{
			bRunTimeout = FALSE;
			::CloseHandle(hThread);
			hThread = NULL;

			WaitForSingleObject(hTimeOutThread, INFINITE);
			::CloseHandle(hTimeOutThread);
			break;
		}
		else if (CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->GameCmd != g_GameStatus)
		{
			g_GameStatus = CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->GameCmd;
			if (g_GameStatus == em_GameCmd_Stop)
				fnTermiate();
			else if (g_GameStatus == em_GameCmd_Start)
				fnRun();
		}

		// 一键收邮件!
		if (CGameVariable::GetInstance().GetAccShareInfo()->AccountStatus.emForceMail != em_ForceMail_Type::em_ForceMail_Type_None)
		{
			// 邮寄号和仓库号都不执行邮件操作
			CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_MAIL_MODE, static_cast<DWORD>(CGameVariable::GetInstance().GetAccShareInfo()->AccountStatus.emForceMail));
			if (CGameVariable::GetInstance().ExistSpicalName(CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->szPlayerName) == CGameVariable::em_SpicalName_Type_None)
			{
				LOG_CF_D( L"开始强制一键邮件模式!");
				CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->GameCmd = g_GameStatus = em_GameCmd::em_GameCmd_Stop;
				fnTermiate();

				CGameVariable::GetInstance().CurrentScriptName() = L"SendMail.inf";
				CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->GameCmd = g_GameStatus = em_GameCmd::em_GameCmd_Start;
				fnRun();
			}
			CGameVariable::GetInstance().GetAccShareInfo()->AccountStatus.emForceMail = em_ForceMail_Type::em_ForceMail_Type_None;
		}

		::Sleep(1000);
	}

	if (CGameVariable::GetInstance().GetAccShareInfo()->AccountStatus.bClose)
	{
		LOG_CF_D( L"bClose=TRUE; 等待客户端结束!");
		::Sleep(INFINITE);
	}

	LOG_CF_D( L"Exit WaitForThread! bClose=%X", CGameVariable::GetInstance().GetAccShareInfo()->AccountStatus.bClose);
	return TRUE;
}

unsigned CBnsGameFun::_TimeoutThread(LPVOID lpParm)
{
	LOG_CF_D( L"启动_TimeoutThread");
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NMAE_MIANTASK_TIMEOUT, 10 * 60);

	CTimeoutCheck TimeoutCheck;
	while (GameRun && *(PBOOL)lpParm)
	{
		BnsSleep(1000);
		if (CPersonFunction::GetInstance().IsGameLoad())
			continue;

		TimeoutCheck.Check();
	}
	

	LOG_CF_D( L"Exit TimeoutThread!");
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_GUARD_TIMEOUTTHREAD, FALSE);
	return 0;
}

BOOL CBnsGameFun::GameLogin(CONST std::wstring& wsAccountName, CONST std::wstring& wsAccountPass) CONST
{
	CGameUi LoginBannedConfirmPanel;
	LOG_CF_D( L"等待登录……");

	CConsoleLog(L"准备登录……");
	::Sleep(5 * 1000);
	CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->bLogin = FALSE;
	CGameVariable::GetInstance().GetAccShareInfo()->AccountStatus.emLoginStep = em_Login_Step::em_Login_Step_InputAccount;
	auto dwTick = ::GetTickCount64();
	while (static_cast<DWORD>(::GetTickCount64() - dwTick) <= 30 * 1000)
	{
		CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"LoginPanel", [this, &wsAccountName, &wsAccountPass](CONST CGameUi& LoginPanel)
		{
			// Login Over! Run Next Step
			CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->bLogin = FALSE;
			Login(wsAccountName, wsAccountPass);
			LOG_CF_D( L"登陆中……");
			BnsSleep(4 * 1000);
		});

		if (CObjectFunction::GetInstance().IsShow_In_GameUi(L"ClausePanel"))
			return BnsSleep(2 * 1000);

		if (CObjectFunction::GetInstance().IsShow_In_GameUi(L"LoginBannedConfirmPanel"))
		{
#if BnsProject_DLL
			CSocketAPI().AddBannedContentLog();
#endif
			::Sleep(5 * 1000);
			SetLoginFaild(L"被封号", em_LoginFaild_DeleteAccount);
		}

		if (CObjectFunction::GetInstance().IsShow_In_GameUi(L"LoginOtpPanel"))
		{
			
			/*if (!CCharacter::wstrcmp_my(CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.wszTypeName,L"保号召唤模式"))
			{
				WriteConsoleLog(L"出现OTP,但是当前模式不是'保号召唤模式',删号!");
				SetLoginFaild(em_LoginFaild_DeleteAccount);
			}*/
			CConsoleLog(L"出现OTP! 正在计算……");
#if BnsProject_DLL
			DWORD dwValue = CSocketAPI().CalcualteOTP();
			if (dwValue == 0)
			{
				if (CGameVariable::GetInstance().GetAccShareInfo()->GetLoginFaildCount() >= 3)
					SetLoginFaild(L"当前帐号的密钥不存在!, 把密钥报给5401!", em_LoginFaild_DeleteAccount);
				else
					SetLoginFaild(L"当前帐号的密钥不存在! 关闭游戏重新尝试", em_LoginFaild_UnDeleteAccount);
			}
			LOG_CF_D( L"Key=%d", dwValue);
			CGameVariable::GetInstance().PushMainThreadFunPtr_synch([dwValue]{ CGameSend::InputOTP(MyTools::CCharacter::MakeFormatText(L"%d", dwValue).c_str()); });
			::Sleep(5 * 1000);
			if (CObjectFunction::GetInstance().IsShow_In_GameUi(L"LoginOtpPanel"))
			{
				LOG_CF_D( L"OTP还在, 等待15s后再去服务器查询新的OTP!");
				::Sleep(10 * 1000);
			}
#endif
		}

		::Sleep(1000);
	}
	
	CGameVariable::GetInstance().GetAccShareInfo()->GetLoginFaildCount() += 1;
	SetCloseGame(MyTools::CCharacter::MakeFormatText(L"帐号密码这里 登录失败!次数=%d", 
		CGameVariable::GetInstance().GetAccShareInfo()->GetLoginFaildCount()), 
		MyTools::CCharacter::wstrcmp_my(CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.wszTypeName, L"交易模式2") ? em_Close_Result_Trading : em_Close_Result_ReStart);
	return FALSE;
}

BOOL CBnsGameFun::InitGame() CONST
{
	//枚举本窗口
	/*CGameVariable::GetInstance().GetAccShareInfo()->hGameWnd = NULL;
	CSomeFun::GetInstance().TimeOut_Condiction(10 * 1000, [] 
	{
		::EnumWindows(EnumSelfWnd, reinterpret_cast<LPARAM>(CGameVariable::GetInstance().GetAccShareInfo()));
		return CGameVariable::GetInstance().GetAccShareInfo()->hGameWnd == NULL; 
	});


	if (CGameVariable::GetInstance().GetAccShareInfo()->hGameWnd == NULL)
	{
		CGameVariable::GetInstance().GetAccShareInfo()->GetLoginFaildCount() += 1;
		SetCloseGame(L"10秒内无法找到属于自己的游戏窗口!", em_Close_Result_ReStart);
	}*/
	//枚举本窗口
	if (CSomeFun::GetInstance().TimeOut_Condiction(10 * 1000, [] { return CGameVariable::GetInstance().GetAccShareInfo()->hGameWnd == NULL; }))
	{
		CGameVariable::GetInstance().GetAccShareInfo()->GetLoginFaildCount() += 1;
		SetCloseGame(L"10秒内无法找到属于自己的游戏窗口!", em_Close_Result_ReStart);
	}

	// Set Bns Window Title
	CConsoleLog(L"正在设置窗口标题!");
	::SetWindowTextW(CGameVariable::GetInstance().GetAccShareInfo()->hGameWnd, CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->szUserName);
	::Sleep(3 * 1000);


	CSomeFun::GetInstance().InitDLL();
	CGameFunction::GetInstance().InitTimer();
	return CBnsGameFun::GameLogin(CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->szUserName, CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->szPassWord);
}

BOOL CBnsGameFun::IntoSwtichPlayer() CONST
{
	// Set Bns Windows Minimize
	::ShowWindow(CGameVariable::GetInstance().GetAccShareInfo()->hGameWnd, SW_MINIMIZE);
	::SendMessageW(CGameVariable::GetInstance().GetAccShareInfo()->hGameWnd, WM_SYSCOMMAND, SC_MINIMIZE, NULL);
	::Sleep(3 * 1000);

	auto dwLoginTick = ::GetTickCount64();
	while (CObjectFunction::GetInstance().IsShow_In_GameUi(L"ClausePanel"))
	{
		CConsoleLog(L"接受协议!");
		CGameVariable::GetInstance().GetAccShareInfo()->AccountStatus.emLoginStep = em_Login_Step::em_Login_Step_Agreement;
		CSomeFun::GetInstance().SimlationKey(VK_RETURN, 200, 5 * 1000);
		if (static_cast<DWORD>(::GetTickCount64() - dwLoginTick) > 20 * 1000)
		{
			SetCloseGame(L"登录界面那个接受协议失败", em_Close_Result_ReStart);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CBnsGameFun::ExcuteScript_Type() CONST
{
	ToSeletedPlayer();

	StartGame_By_Index(CLoginPlayerExtend::GetInstance().GetLoginPlayerIndex());
	CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->GameCmd = em_GameCmd_Start;
	for (;;)
	{
		::Sleep(1000);
		if (CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->GameCmd != em_GameCmd_Start)
			continue;

		// 设置不检查坐标超时!
		CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CHECKPAUSEPOINT, NULL);
		WaitForThread(CBnsGameFun::_em_Excute_Type_None);
	}
}

BOOL CBnsGameFun::Login(_In_ CONST std::wstring& wsAccount, _In_ CONST std::wstring& wsPass) CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&wsAccount, &wsPass]
	{
		static WCHAR wszAccountText[64];
		static WCHAR wszPassText[64];

		MyTools::CCharacter::wstrcpy_my(wszAccountText, wsAccount.c_str(), wsAccount.length());
		MyTools::CCharacter::wstrcpy_my(wszPassText, wsPass.c_str(), wsPass.length());
		CGameSend::Login((DWORD)wszAccountText, (DWORD)wszPassText);
	});
}

VOID CBnsGameFun::SetLoginFaild(_In_ CONST std::wstring& wsLogText, _em_LoginFaild em) CONST
{
	LOG_CF_E(L"SetLoginFaild! wsLogText=%s", wsLogText.c_str());
	CBnsConfig::GetInstance().WriteAccountLog(wsLogText);
	CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->bLogin = FALSE;
	StopGame;
	if (em == em_LoginFaild_DeleteAccount)
	{
		g_bExitGame = TRUE;
		CGameVariable::GetInstance().GetAccShareInfo()->AccountStatus.bDelete = TRUE;
		CGameVariable::GetInstance().GetAccShareInfo()->AccountStatus.bDone = TRUE;
	}
	else
	{
		CGameVariable::GetInstance().GetAccShareInfo()->AccountStatus.bClose = TRUE;
	}
#if BnsProject_DLL
	g_bStopDumpFile = TRUE;
	::Sleep(INFINITE);
#endif
}
VOID CBnsGameFun::SetCloseGame(_In_ CONST std::wstring& wsLogText, _In_ _em_Close_Result emResult) CONST
{
	LOG_CF_E(L"SetCloseGame! wsLogText=%s", wsLogText.c_str());
	CBnsConfig::GetInstance().WriteAccountLog(wsLogText);

	CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->bLogin = FALSE;
	CGameVariable::GetInstance().GetAccShareInfo()->AccountStatus.emCloseResult = emResult;
	CGameVariable::GetInstance().GetAccShareInfo()->AccountStatus.bClose = TRUE;
	StopGame;
#if BnsProject_DLL
	g_bStopDumpFile = TRUE;
	::Sleep(INFINITE);
#else
	LOG_MSG_CF(L"SetCloseGame");
#endif
	
}

BOOL CBnsGameFun::BackToSwitchAccount() CONST
{
	BOOL bTimeout = CSomeFun::GetInstance().TimeOut_Condiction(5 * 60 * 1000, []
	{
		LOG_CF_D( L"小退!");
		CPersonFunction::GetInstance().BackToSwitchPlayer();
		::Sleep(10 * 1000);
		CPersonFunction::GetInstance().WaitGameLoad(FALSE, FALSE);
		return !CObjectFunction::GetInstance().IsShow_In_GameUi(L"CreateCharacterInfomationHolder");
	});

	if (bTimeout)
	{
		SetCloseGame(L"小退5分钟超时!", em_Close_Result::em_Close_Result_ReStart);
	}
	LOG_CF_D( L"小退完毕!");
	return BnsSleep(5 * 1000);
}

BOOL CBnsGameFun::SartGame_CloseOther() CONST
{
	// 老号那种新任务
	while (CObjectFunction::GetInstance().IsShow_In_GameUi(L"QuestMappingPanel"))
	{
		LOG_CF_D(L"老号任务!");
		CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"QuestMappingPanel", [](CONST CGameUi& QuestMappingPanel)
		{
			CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&QuestMappingPanel]{CGameSend::SwitchNewMainTask(QuestMappingPanel.GetObj()); }); 
		});
		::Sleep(5 * 1000);
	}

	auto fnCloseGameWindow = [](_In_ CONST std::wstring& emGameUi)
	{
		return CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(emGameUi, [&emGameUi](CONST CGameUi& GameUi_)
		{
			LOG_CF_D( L"关闭UI:%s", emGameUi.c_str());
			GameUi_.CloseGameWindow();
			BnsSleep(1000);
		});
	};

	// 关闭新手Npc对话窗口
	if (fnCloseGameWindow(L"TalkControlPanel"))
	{
		CSomeFun::GetInstance().SimlationKey(VK_ESCAPE, 200, 800);
	}

	// 技能加点面板
	fnCloseGameWindow(L"SkillTrainPanel");

	// 还有一个什么物品窗口
	fnCloseGameWindow(L"ItemCleanerPanel");

	// 成就窗口
	fnCloseGameWindow(L"AchievementPanel");

	fnCloseGameWindow(L"Inventory2Panel");

	fnCloseGameWindow(L"EventGuidePanel");

	fnCloseGameWindow(L"GoodsStoreNotificationPanel");

	fnCloseGameWindow(L"AttendanceCalendarPanel");

	return CSomeFun::GetInstance().SimlationKey(VK_ESCAPE, 200, 800);
}

BOOL CBnsGameFun::StartGame_By_Index(_In_ DWORD dwIndex) CONST
{
	StartGame;
	CONST std::wstring wsTypeName = CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.wszTypeName;
	CLoginPlayerExtend::GetInstance().SetLoginPlayerIndex(dwIndex);
	CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->dwServerId = CLoginPlayerExtend::GetInstance().GetLoginPlayerServerId_By_Index(dwIndex);
	CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->dwOrderIndex = dwIndex;
	LOG_CF_D( L"当前角色ServerId=%X", CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->dwServerId);

	// 最多设置60s的登录时间(除非出现排队)
	CConsoleLog(L"进入游戏中……");
	CSomeFun::GetInstance().SimlationKey(VK_RETURN, 200, 3 * 1000);
	for (int i = 0; i < 10 && (CPerson::GetInstance().GetBase() == NULL || CGameVariable::GetInstance().GetEnvironmentValue() == 0x2) ; ++i)
	{
		CSomeFun::GetInstance().SimlationKey(VK_RETURN, 200, 5 * 1000);
		while (CObjectFunction::GetInstance().IsShow_In_GameUi(L"WaitingEnterWorldConfirmPanel"))
		{
			CGameVariable::GetInstance().GetAccShareInfo()->AccountStatus.emLoginStep = em_Login_Step::em_Login_Step_Queue;
			CConsoleLog(L"排队ing");
			Sleep(5000);
		}
	}

	if (CSomeFun::GetInstance().TimeOut_Condiction(5 * 60 * 1000, []
	{
		CSomeFun::GetInstance().SimlationKey(VK_RETURN, 200, 3 * 1000);
		CPersonFunction::GetInstance().WaitGameLoad(TRUE,FALSE);
		::Sleep(1000);
		return !CObjectFunction::GetInstance().IsShow_In_GameUi(L"SystemMenuPanel");
	}))
	{
		SetCloseGame(L"5分钟内读图超时!", em_Close_Result::em_Close_Result_ReStart);
	}

	// 是否回车按多了, 出现了聊天输入框
	while (CObjectFunction::GetInstance().IsShow_In_GameUi(L"ChatInputPanel"))
	{
		LOG_CF_D( L"出现聊天框!");
		CSomeFun::GetInstance().SimlationKey(VK_RETURN, 200, 5 * 1000);
	}

	if (EMPTY_PERSONHP)
		CPersonFunction::GetInstance().Resurrect();

	CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->dwClasses = static_cast<DWORD>(CPerson::GetInstance().GetClasses());
	CGameVariable::GetInstance().GetAccShareInfo()->AccountStatus.emLoginStep = em_Login_Step::em_Login_Step_Finish;
	if (CPerson::GetInstance().GetLevel() > 2)
		CFarm::GetInstance().BeckonCat();

	SartGame_CloseOther();
	CConsoleLog(L"登录完毕!");
	LOG_CF_D( L"正式进入游戏完毕!");
	CGameVariable::GetInstance().GetAccShareInfo()->GetLoginFaildCount() = 0;
	MyTools::CCharacter::wstrcpy_my(CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->szPlayerName, CPerson::GetInstance().GetName().c_str(), _countof(CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->szPlayerName) - 1);
#if BnsProject_DLL
	CSocketAPI().AddLoginContentLog(CPerson::GetInstance().GetName());
#endif
	return CSomeFun::GetInstance().SetGameTitle();
}

BOOL CBnsGameFun::DeleteAccount_By_Wps(_Out_ int& nRetIndex, _Out_ std::wstring& wsRetPlayerName) CONST
{
	std::vector<CLoginPlayer> vlst;
	CObjectFunction::GetInstance().GetLoginPlayerList(vlst);

	for (UINT i = 0; i < vlst.size(); ++i)
	{
		CONST auto& itm = vlst.at(i);
		CONST std::wstring wsName = itm.GetName();
		if (wsName[0] != 'W' || wsName[1] != 's' || wsName[2] != 'p')
		{
			nRetIndex = i;
			wsRetPlayerName = wsName;
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CBnsGameFun::ToSeletedPlayer() CONST
{
	if (!CBnsGameFun::GetInstance().InitGame())
	{
		LOG_CF_D( L"InitGame Faild!");
		return FALSE;
	}

	if (!CBnsGameFun::GetInstance().IntoSwtichPlayer())
	{
		LOG_CF_D( L"IntoSwtichPlayer Faild!");
		return FALSE;
	}
	return TRUE;
}

DWORD WINAPI _CreatePlayer_By_HotKey_Thread(LPVOID lpParm)
{
	std::vector<CLoginPlayer> vlst;
	CObjectFunction::GetInstance().GetLoginPlayerList(vlst);

	em_Player_Classes emPlayerClasses = em_Player_Classes_Destroyer;
	DWORD dwIndex = CObjectFunction::GetInstance().GetLoginPlayerCount();

	for (CONST auto& itm : vlst)
	{
		if (itm.GetLevel() >= 45)
		{
			LOG_CF_D( L"有角色在45级以上, 创建气宗!");
			emPlayerClasses = em_Player_Classes_KongFuForceMaster;
			break;
		}
	}

	LOG_CF_D( L"dwAreaId=%X, dwIndex=%X", CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.dwAreaId, dwIndex);
	return CLoginPlayerExtend::GetInstance().CreatePlayer(CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.dwAreaId, dwIndex, emPlayerClasses);
}

BOOL CBnsGameFun::CreatePlayer_By_HotKey() CONST
{
	auto hThread = cbBEGINTHREADEX(NULL, NULL, _CreatePlayer_By_HotKey_Thread, NULL, NULL, NULL);
	::CloseHandle(hThread);
	hThread = NULL;
	return TRUE;
}

BOOL CBnsGameFun::DeletePlayer_By_HotKey() CONST
{
	DWORD dwIndex = CLoginPlayerExtend::GetInstance().GetLoginPlayerIndex();
	if (CLoginPlayerExtend::GetInstance().GetLoginPlayerStatus_By_Index(dwIndex) == em_LoginPlayer_Deleted)
	{
		LOG_MSG_CF( L"兄台, 当前被选择的角色已经是删除状态了!");
		return TRUE;
	}

	LOG_CF_D( L"dwIndex=%X", dwIndex);
	return CLoginPlayerExtend::GetInstance().DeletePlayer(dwIndex, 1000);
}

SYSTEMTIME CBnsGameFun::AddDay(SYSTEMTIME SysTime, int nDay) CONST
{
	// // 每个月开始时是这年的第几天,0月不用
	const static unsigned daysInEveryMonth[13] = { 0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };

	auto fnGetDay = [](SYSTEMTIME& SysTime)
	{
		unsigned days(0);

		// 每100年有24个闰年
		days += SysTime.wYear / 100 * 24;
		// 每400年是闰年
		days += SysTime.wYear / 400;
		// 年数小于100时,每4年是闰年
		days += (SysTime.wYear % 100) / 4;

		days += SysTime.wYear * 365;
		days += daysInEveryMonth[SysTime.wMonth];
		days += SysTime.wDay;

		return days;
	};

	auto fnGetTime = [&SysTime](UINT uDay)
	{
		SYSTEMTIME tmpTime;
		memcpy(&tmpTime, &SysTime, sizeof(tmpTime));
		tmpTime.wYear = 0;
		while (uDay > 365)
		{
			tmpTime.wYear += 1;
			uDay -= 365;

			// 判断是否闰年
			if ((tmpTime.wYear % 400 == 0 || (tmpTime.wYear % 100 != 0 && tmpTime.wYear % 4 == 0)))
				--uDay;

		}

		// 计算月份
		tmpTime.wMonth = 12;
		while (uDay < daysInEveryMonth[tmpTime.wMonth])
			--tmpTime.wMonth;

		uDay -= daysInEveryMonth[tmpTime.wMonth];
		tmpTime.wDay = static_cast<WORD>(uDay);
		return tmpTime;
	};

	return fnGetTime(fnGetDay(SysTime) + nDay);
}

BOOL CBnsGameFun::InitScriptParmeter() CONST
{
	// 
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_FINISH_ALL_MAINTASK, FALSE);
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_DELETEACCOUNT, FALSE);
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_JOINTEAM_TIMEOUT, FALSE);
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NMAE_MIANTASK_TIMEOUT, FALSE);
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_ESCAPE_TIMEOUT, FALSE);
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NMAE_ISPUASEHELPTHREAD, FALSE);
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_TASKTIMEOUTID, FALSE);
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_BAIXIAN_STAGE_STEP, FALSE);
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_BAIXIAN_DONE, FALSE);
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_BAIXIAN_TIMEOUT, FALSE);
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_FORCESTOPPOINT, FALSE);
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_DISABLE_SWITCHVIEW, FALSE);
	return TRUE;
}

std::function<BOOL(VOID)>& CBnsGameFun::GetScriptExceptionPtr() CONST
{
	static std::function<BOOL(VOID)> fnScriptExceptionPtr = []
	{
		CBnsGameFun::GetInstance().SetCloseGame(L"执行脚本出现异常!", em_Close_Result::em_Close_Result_ReStart);
		return FALSE;
	};
	return fnScriptExceptionPtr;
}
