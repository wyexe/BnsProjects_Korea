#ifndef __GITBNSPROJECTS_CONSOLEDLL_FEATURE_NCLANUCHER_NCLANUCHERLOGIN_H__
#define __GITBNSPROJECTS_CONSOLEDLL_FEATURE_NCLANUCHER_NCLANUCHERLOGIN_H__

#include <set>
#include <type_traits>
#include "ConsoleVariable.h"

class CNcLanucherLogin
{
private:
	enum
	{
		em_LoginUi_Control_Edit_AccountName = 0x65,
		em_LoginUi_Control_Edit_Password	= 0x66,
		em_LoginUi_Control_Button_Login		= 0x12D,
		em_GameLauncher_Control_Progress	= 0x41B,
		em_GameLauncher_Control_StartGame	= 0x407,
		em_GameLauncher_Control_PromptBtn	= 0x41C,
	};

	struct EnumControlContent
	{
		std::wstring wsClassName;
		DWORD dwCtrlId;
		HWND* hWnd;
	};
public:
	enum class em_Login_Progress
	{
		em_None,

		// 等待 账号登录器初始化
		em_LoginInitialize,

		// 输入账号密码中
		em_Typing,

		// 等待 游戏登陆初始化
		em_LaucherInitialize,

		// 更新中
		em_Updating,

		// 开始游戏中
		em_Starting,
	};

	using CreateProcessWPtr = BOOL (WINAPI*)(_In_opt_ LPCWSTR lpApplicationName,
		_Inout_opt_ LPWSTR lpCommandLine,
		_In_opt_ LPSECURITY_ATTRIBUTES lpProcessAttributes,
		_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
		_In_ BOOL bInheritHandles,
		_In_ DWORD dwCreationFlags,
		_In_opt_ LPVOID lpEnvironment,
		_In_opt_ LPCWSTR lpCurrentDirectory,
		_In_ LPSTARTUPINFOW lpStartupInfo,
		_Out_ LPPROCESS_INFORMATION lpProcessInformation);
public:
	CNcLanucherLogin();
	~CNcLanucherLogin() = default;

	DWORD	RunNcLauncher(_In_ CONST LoginLanucherConfig& LoginLanucherCfg);

	std::wstring GetProgressText() CONST;

	BOOL	Start(_In_ CONST LoginLanucherConfig& LoginLanucherCfg);

	DWORD	GetClientPid() CONST;

	VOID	Stop();
private:
	BOOL 	ClickNcLanucherPrompt();

	VOID	SetProgressText(_In_ LPCWSTR pwszFormat, ...);

#pragma region LoginLanucher
	static BOOL CALLBACK EnumLanucherControl(_In_ HWND hControl, _In_ LPARAM lpParam);

	BOOL FindControlHwnd(_In_ DWORD dwCtrlId, _In_ CONST std::wstring& wsClassName, _In_ DWORD dwTimeout, _Out_ HWND& hWnd);

	// 获取登陆控件句柄
	BOOL	GetLoginControlHwnd(_In_ CONST LoginLanucherConfig& LoginLanucherCfg);

	// 输入账号密码并且登陆
	VOID	AccountLogin(_In_ CONST LoginLanucherConfig& LoginLanucherCfg);

	// 登陆
	BOOL	Login(_In_ CONST LoginLanucherConfig& LoginLanucherCfg);
#pragma endregion


#pragma region GameLanucher
	static BOOL CALLBACK EnumPromptWindows(_In_ HWND hControl, _In_ LPARAM lpParam);

	// 开始游戏
	BOOL	RunGame(_In_ CONST LoginLanucherConfig& LoginLanucherCfg);

	// 拍一个Client.exe的进程快照
	VOID	SnapshotClient();

	DWORD	GetNewClientPid();
#pragma endregion
private:
	BOOL	_IsRuning;
	std::wstring _wsProgressText;
	HWND	_hLanucherHwnd;
	HWND	_hAccountNameHwnd;
	HWND	_hPasswordHwnd;
	HWND	_hLoginButtonHwnd;

	DWORD	_dwClientPid;
	std::set<DWORD> _VecSnapshotPid;
};



#endif // !__GITBNSPROJECTS_CONSOLEDLL_FEATURE_NCLANUCHER_NCLANUCHERLOGIN_H__
