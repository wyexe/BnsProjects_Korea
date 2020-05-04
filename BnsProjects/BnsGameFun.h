#ifndef __GITBNSPRJECTS_BNSPROJECT_SOMEFUN_BNSGAMEFUN_H__
#define __GITBNSPRJECTS_BNSPROJECT_SOMEFUN_BNSGAMEFUN_H__

#include "GameVariable.h"

// 尚未进游戏的处理函数
class CBnsGameFun : public MyTools::CClassInstance<CBnsGameFun>
{
public:
	explicit CBnsGameFun() = default;
	~CBnsGameFun() = default;

	// 
	static HHOOK& GetHHOK_HotKey();
public:
	// 窗口还没出来->登录游戏帐号密码的阶段
	BOOL InitGame() CONST;
	// 同意协议->选择角色阶段
	BOOL IntoSwtichPlayer() CONST;

	// 初始化执行脚本的全局参数
	BOOL InitScriptParmeter() CONST;

	// 等待脚本线程结束!
	enum _em_Excute_Type{ _em_Excute_Type_None, em_Excute_Type_PK_TaskUpgrade, em_Excute_Type_PK_PK, em_Excute_Type_Warehouse, em_Excute_Type_DeletePlayer, em_Excute_Type_Mail };
	BOOL  WaitForThread(_In_ _em_Excute_Type emType) CONST;

	BOOL  GameLogin(CONST std::wstring& wsAccountName, CONST std::wstring& wsAccountPass) CONST;
	
	BOOL ToSeletedPlayer() CONST;

	BOOL ExcuteScript_Type() CONST;
	
	// 登录
	BOOL		Login(_In_ CONST std::wstring& wsAccount, _In_ CONST std::wstring& wsPass) CONST;

	// 设置登录失败
	enum _em_LoginFaild { em_LoginFaild_DeleteAccount, em_LoginFaild_UnDeleteAccount };
	VOID		SetLoginFaild(_In_ CONST std::wstring& wsLogText, _In_ _em_LoginFaild em) CONST;
	VOID		SetCloseGame(_In_ CONST std::wstring& wsLogText, _In_ _em_Close_Result emResult) CONST;
	

	// 自动换号逻辑
	BOOL SartGame_CloseOther() CONST;
	BOOL BackToSwitchAccount() CONST;
	BOOL StartGame_By_Index(_In_ DWORD dwIndex) CONST;
	BOOL DeleteAccount_By_Wps(_Out_ int& nRetIndex, _Out_ std::wstring& wsRetPlayerName) CONST;

	// 热键创建角色
	BOOL CreatePlayer_By_HotKey() CONST;
	// 热键删除角色
	BOOL DeletePlayer_By_HotKey() CONST;

	SYSTEMTIME AddDay(SYSTEMTIME SysTime, int nDay) CONST;

	std::function<BOOL(VOID)>& GetScriptExceptionPtr() CONST;
private:
	DSIABLE_COPY_AND_ASSIGN(CBnsGameFun)
private:
	// 5分钟超时不动线程
	static unsigned _TimeoutThread(LPVOID lpParm);
	
	// 执行脚本线程
	static unsigned ExcuteScriptThread(LPVOID lpParm);

	//
	static BOOL CALLBACK EnumSelfWnd(_In_ HWND hWnd, _In_ LPARAM lParam);
};



#endif