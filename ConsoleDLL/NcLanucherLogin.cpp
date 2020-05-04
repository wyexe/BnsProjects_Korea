#include "stdafx.h"
#include "NcLanucherLogin.h"
#include <future>
#include <MyTools/TimeTick.h>
#include <MyTools/Character.h>
#include <MyTools/LdrHeader.h>
#include <MyTools/CLProcess.h>
#include <MyTools/CLPublic.h>
#include <MyTools/Log.h>

#define _SELF L"NcLanucherLogin.cpp"

CNcLanucherLogin::CNcLanucherLogin()
{
	_IsRuning = FALSE;
	_hLanucherHwnd = _hPasswordHwnd = _hAccountNameHwnd = _hLoginButtonHwnd = NULL;
	_dwClientPid = NULL;
	SetProgressText(L"启动登录器......");
}

DWORD CNcLanucherLogin::RunNcLauncher(_In_ CONST LoginLanucherConfig& LoginLanucherCfg)
{
	// Terminate Proc
	MyTools::CLProcess::TerminateProc_For_ProcName(L"NCLauncherS.exe");


	PROCESS_INFORMATION pi = { 0 };


	std::wstring wsLanucherPath = LoginLanucherCfg.wszNcLauncherPath;
	wsLanucherPath += L"\\NCLauncherS.exe";
	//LOG_C_D(L"wsLanucherPath=%s", wsLanucherPath.c_str());
	//LOG_C_D(L"DLLPath=%s", MyTools::CLPublic::MakeCurrentPath(L"\\CopyFile\\JLnp_hf.dll").c_str());

	//STARTUPINFOW si = { 0 };
	//return ::CreateProcessW(wsLanucherPath.c_str(), NULL, NULL, NULL, NULL, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	return MyTools::CLProcess::CreateProcess_InjectorRemoteDLL(wsLanucherPath.c_str(), MyTools::CLPublic::MakeCurrentPath(L"\\CopyFile\\JLnp_hf.dll").c_str(), &pi) ? pi.dwProcessId : NULL;
}

std::wstring CNcLanucherLogin::GetProgressText() CONST
{
	return _wsProgressText;
}

BOOL CNcLanucherLogin::Start(_In_ CONST LoginLanucherConfig& LoginLanucherCfg)
{
	_IsRuning = TRUE;
	SetProgressText(L"等待获取登陆控件");
	if (!GetLoginControlHwnd(LoginLanucherCfg))
	{
		return FALSE;
	}


	SetProgressText(L"开始登陆......");
	if (!Login(LoginLanucherCfg))
	{
		return FALSE;
	}


	SetProgressText(L"开始启动游戏...");
	auto bRetCode = RunGame(LoginLanucherCfg);

	_IsRuning = FALSE;
	return bRetCode;
}

DWORD CNcLanucherLogin::GetClientPid() CONST
{
	return _dwClientPid;
}

VOID CNcLanucherLogin::Stop()
{
	_IsRuning = FALSE;
}

BOOL CNcLanucherLogin::ClickNcLanucherPrompt()
{
	HWND hWnd = ::FindWindowW(L"#32770 (对话框)", NULL);

	BOOL bExist = FALSE;
	::EnumChildWindows(hWnd, EnumPromptWindows, reinterpret_cast<LPARAM>(&bExist));
	return bExist;
}

VOID CNcLanucherLogin::SetProgressText(_In_ LPCWSTR pwszFormat, ...)
{
	va_list		args;
	wchar_t		szBuff[1024] = { 0 };

	va_start(args, pwszFormat);
	_vsnwprintf_s(szBuff, _countof(szBuff) - 1, _TRUNCATE, pwszFormat, args);
	va_end(args);

	LOG_C_D(L"SetProgressText:[%s]", szBuff);
	_wsProgressText = szBuff;
}


BOOL CALLBACK CNcLanucherLogin::EnumLanucherControl(_In_ HWND hControl, _In_ LPARAM lpParam)
{
	WCHAR wszWinClass[MAX_PATH] = { 0 };
	::GetClassNameW(hControl, wszWinClass, _countof(wszWinClass) - 1);

	auto* pEnumControlContent = reinterpret_cast<EnumControlContent *>(lpParam);
	if (pEnumControlContent->wsClassName == wszWinClass && ::GetDlgCtrlID(hControl) == static_cast<int>(pEnumControlContent->dwCtrlId))
	{
		*pEnumControlContent->hWnd = hControl;
		return FALSE;
	}
	return TRUE;
}

BOOL CNcLanucherLogin::FindControlHwnd(_In_ DWORD dwCtrlId, _In_ CONST std::wstring& wsClassName, _In_ DWORD dwTimeout, _Out_ HWND& hWnd)
{
	EnumControlContent Content = { wsClassName, dwCtrlId, &hWnd };

	hWnd = NULL;
	for (MyTools::CTimeTick TimeTick; static_cast<DWORD>(TimeTick.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Second)) < dwTimeout && hWnd == NULL && _IsRuning;)
	{
		_hLanucherHwnd = ::FindWindowW(L"LauncherDialog", L"NC Launcher");
		if (_hLanucherHwnd != NULL)
		{
			::EnumChildWindows(_hLanucherHwnd, EnumLanucherControl, reinterpret_cast<LPARAM>(&Content));
		}
		

		::Sleep(1000);
	}

	return hWnd != NULL;
}

BOOL CNcLanucherLogin::GetLoginControlHwnd(_In_ CONST LoginLanucherConfig& LoginLanucherCfg)
{
	for(MyTools::CTimeTick TimeTick; _hLoginButtonHwnd == NULL || _hAccountNameHwnd == NULL || _hPasswordHwnd == NULL;)
	{
		if (ClickNcLanucherPrompt())
		{
			SetProgressText(L"Update错误,重新启动登录器");
			return FALSE;
		}
		FindControlHwnd(em_LoginUi_Control_Button_Login, L"Button", 1, _hLoginButtonHwnd);
		FindControlHwnd(em_LoginUi_Control_Edit_AccountName, L"Edit", 1, _hAccountNameHwnd);
		FindControlHwnd(em_LoginUi_Control_Edit_Password, L"Edit", 1, _hPasswordHwnd);

		if (TimeTick.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Second) >= LoginLanucherCfg.dwLoginControlHwndTimeout)
		{
			SetProgressText(L"等待显示 [获取登陆控件] 超时[%d]秒", LoginLanucherCfg.dwLoginControlHwndTimeout);
			return FALSE;
		}
	}
	
	return TRUE;
}

VOID CNcLanucherLogin::AccountLogin(_In_ CONST LoginLanucherConfig& LoginLanucherCfg)
{
	// Is Enable Login
	//::SetForegroundWindow(_hLanucherHwnd);

	::SendMessageW(_hAccountNameHwnd, WM_SETTEXT, NULL, reinterpret_cast<LPARAM>(LoginLanucherCfg.wszAccountName));
	::SendMessageW(_hPasswordHwnd, WM_SETTEXT, NULL, reinterpret_cast<LPARAM>(LoginLanucherCfg.wszAccountPass));
	::Sleep(1000);


	if ((::GetWindowLongW(_hLoginButtonHwnd, GWL_STYLE) & WS_DISABLED) == 0)
	{
		::SendMessageW(_hLoginButtonHwnd, WM_LBUTTONDOWN, NULL, NULL);
		::SendMessageW(_hLoginButtonHwnd, WM_LBUTTONUP, NULL, NULL);
		::Sleep(2000);
	}
}

BOOL CNcLanucherLogin::Login(_In_ CONST LoginLanucherConfig& LoginLanucherCfg)
{
	MyTools::CTimeTick TimeTick;
	while (FindControlHwnd(em_LoginUi_Control_Button_Login, L"Button", 2, _hLoginButtonHwnd) && _IsRuning)
	{
		AccountLogin(LoginLanucherCfg);
		if (TimeTick.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Second) >= LoginLanucherCfg.dwLoginTimeout)
		{
			SetProgressText(L"账号登陆失败 超时[%d]秒", LoginLanucherCfg.dwLoginTimeout);
			return FALSE;
		}
	}

	::Sleep(3 * 1000);
	return TRUE;
}

BOOL CALLBACK CNcLanucherLogin::EnumPromptWindows(_In_ HWND hControl, _In_ LPARAM lpParam)
{
	__try
	{
		WCHAR wszClassName[MAX_PATH] = { 0 };
		::GetClassNameW(hControl, wszClassName, _countof(wszClassName) - 1);

		if (MyTools::CCharacter::wstrcmp_my(wszClassName, L"LauncherDialog"))
		{
			//LOG_C_D(L"wszClassName=%s", wszClassName);
			::EnumChildWindows(hControl, EnumPromptWindows, lpParam);
			//return FALSE;
		}
		if (MyTools::CCharacter::wstrcmp_my(wszClassName, L"Button") && ::GetDlgCtrlID(hControl) == em_GameLauncher_Control_PromptBtn)
		{
			//LOG_C_D(L"wszClassName=%s", wszClassName);
			::SendMessageW(hControl, WM_LBUTTONDOWN, NULL, NULL);
			::SendMessageW(hControl, WM_LBUTTONUP, NULL, NULL);
			*reinterpret_cast<BOOL *>(lpParam) = TRUE;
			return FALSE;
		}
	}
	__except (1)
	{
		LOG_MSG_CF(L"EnumPromptWindows.");
	}

	//LOG_C_D(L"ClassName = %s", wszClassName);
	return TRUE;
}

BOOL CNcLanucherLogin::RunGame(_In_ CONST LoginLanucherConfig& LoginLanucherCfg)
{
	HWND hProgress = NULL, hRunGameButtonHwnd = NULL;
	SnapshotClient();
	for (MyTools::CTimeTick SpentTimeTick, DisableButtonTimeTick; _IsRuning;)
	{
		ClickNcLanucherPrompt();
		if (FindControlHwnd(em_GameLauncher_Control_Progress, L"AfxWnd120su", 1, hProgress) && (::GetWindowLongW(hProgress, GWL_STYLE) & WS_VISIBLE) != 0)
		{
			SetProgressText(L"游戏更新中……用时[%d]分钟", SpentTimeTick.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Minute));
			DisableButtonTimeTick.Reset();
			continue;
		}
		if (hRunGameButtonHwnd == NULL)
		{
			FindControlHwnd(em_GameLauncher_Control_StartGame, L"Button", 1, hRunGameButtonHwnd);
			continue;
		}
		if (hRunGameButtonHwnd != NULL)
		{
			if (!::IsWindowEnabled(hRunGameButtonHwnd))
			{
				::EnableWindow(hRunGameButtonHwnd, TRUE);
				::Sleep(2 * 1000);
				//LOG_MSG_CF(L"EnableWindow");
				continue;
			}
			//LOG_MSG_CF(L"Start ...");
			SetProgressText(L"正在启动游戏...");
			::SendMessage(_hLanucherHwnd, WM_COMMAND, MAKEWPARAM(em_GameLauncher_Control_StartGame, BN_CLICKED), (LPARAM)hRunGameButtonHwnd);
			//::SendMessageW(hRunGameButtonHwnd, WM_LBUTTONDOWN, NULL, NULL);
			//::SendMessageW(hRunGameButtonHwnd, WM_LBUTTONUP, NULL, NULL);
			::Sleep(5 * 1000);
			break;
		}

		if (DisableButtonTimeTick.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Second) >= LoginLanucherCfg.dwDisableLoginButtonTimeout)
		{
			SetProgressText(L"等不到禁用的按钮恢复, 超时[%d]秒", LoginLanucherCfg.dwDisableLoginButtonTimeout);
			return FALSE;
		}
		SetProgressText(L"开始按钮被禁用,等待恢复...用时[%d]分钟", static_cast<int>(SpentTimeTick.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Minute)));
	}
	_dwClientPid = GetNewClientPid();
	if (_dwClientPid == NULL)
	{
		SetProgressText(L"无法找到游戏的进程ID");
		return FALSE;
	}
	SetProgressText(L"启动游戏完毕!");
	return TRUE;
}

VOID CNcLanucherLogin::SnapshotClient()
{
	std::vector<PROCESSENTRY32> Vec;
	MyTools::CLProcess::GetProcessSnapshot(Vec);

	for (auto& itm : Vec)
	{
		if (MyTools::CCharacter::MakeTextToUpper(std::wstring(itm.szExeFile)) == L"CLIENT.EXE")
		{
			_VecSnapshotPid.insert(itm.th32ProcessID);
		}
	}
}

DWORD CNcLanucherLogin::GetNewClientPid()
{
	auto Vec = _VecSnapshotPid;
	_VecSnapshotPid.clear();

	SnapshotClient();
	for (auto& itm : _VecSnapshotPid)
	{
		if (Vec.find(itm) == Vec.end())
		{
			return itm;
		}
	}

	return 0;
}

