#ifndef __GITBNSPRJECTS_BNSPROJECT_SOMEFUN_BNSGAMEFUN_H__
#define __GITBNSPRJECTS_BNSPROJECT_SOMEFUN_BNSGAMEFUN_H__

#include "GameVariable.h"

// ��δ����Ϸ�Ĵ�����
class CBnsGameFun : public MyTools::CClassInstance<CBnsGameFun>
{
public:
	explicit CBnsGameFun() = default;
	~CBnsGameFun() = default;

	// 
	static HHOOK& GetHHOK_HotKey();
public:
	// ���ڻ�û����->��¼��Ϸ�ʺ�����Ľ׶�
	BOOL InitGame() CONST;
	// ͬ��Э��->ѡ���ɫ�׶�
	BOOL IntoSwtichPlayer() CONST;

	// ��ʼ��ִ�нű���ȫ�ֲ���
	BOOL InitScriptParmeter() CONST;

	// �ȴ��ű��߳̽���!
	enum _em_Excute_Type{ _em_Excute_Type_None, em_Excute_Type_PK_TaskUpgrade, em_Excute_Type_PK_PK, em_Excute_Type_Warehouse, em_Excute_Type_DeletePlayer, em_Excute_Type_Mail };
	BOOL  WaitForThread(_In_ _em_Excute_Type emType) CONST;

	BOOL  GameLogin(CONST std::wstring& wsAccountName, CONST std::wstring& wsAccountPass) CONST;
	
	BOOL ToSeletedPlayer() CONST;

	BOOL ExcuteScript_Type() CONST;
	
	// ��¼
	BOOL		Login(_In_ CONST std::wstring& wsAccount, _In_ CONST std::wstring& wsPass) CONST;

	// ���õ�¼ʧ��
	enum _em_LoginFaild { em_LoginFaild_DeleteAccount, em_LoginFaild_UnDeleteAccount };
	VOID		SetLoginFaild(_In_ CONST std::wstring& wsLogText, _In_ _em_LoginFaild em) CONST;
	VOID		SetCloseGame(_In_ CONST std::wstring& wsLogText, _In_ _em_Close_Result emResult) CONST;
	

	// �Զ������߼�
	BOOL SartGame_CloseOther() CONST;
	BOOL BackToSwitchAccount() CONST;
	BOOL StartGame_By_Index(_In_ DWORD dwIndex) CONST;
	BOOL DeleteAccount_By_Wps(_Out_ int& nRetIndex, _Out_ std::wstring& wsRetPlayerName) CONST;

	// �ȼ�������ɫ
	BOOL CreatePlayer_By_HotKey() CONST;
	// �ȼ�ɾ����ɫ
	BOOL DeletePlayer_By_HotKey() CONST;

	SYSTEMTIME AddDay(SYSTEMTIME SysTime, int nDay) CONST;

	std::function<BOOL(VOID)>& GetScriptExceptionPtr() CONST;
private:
	DSIABLE_COPY_AND_ASSIGN(CBnsGameFun)
private:
	// 5���ӳ�ʱ�����߳�
	static unsigned _TimeoutThread(LPVOID lpParm);
	
	// ִ�нű��߳�
	static unsigned ExcuteScriptThread(LPVOID lpParm);

	//
	static BOOL CALLBACK EnumSelfWnd(_In_ HWND hWnd, _In_ LPARAM lParam);
};



#endif