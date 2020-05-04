#ifndef __BNS_SOMEFUN_H__
#define __BNS_SOMEFUN_H__

#include "GameVariable.h"

#define BnsSleep(x)				CSomeFun::GetInstance().Sleep(x)

class CSomeFun : public MyTools::CClassInstance<CSomeFun>
{
public:
	explicit CSomeFun() = default;
	~CSomeFun() = default;

public:
	// ��ʼ��DLL
	BOOL		InitDLL() CONST;
	
	// ��ʼ���ڴ湲��
	BOOL		InitMapView() CONST;
	// ��ʼ��AccountGame
	BOOL		InitAccountGame() CONST;
	
	// ģ�ⰴ��
	BOOL		SimlationKey(_In_ DWORD dwAscii, _In_ DWORD dwSleepTime = 200, _In_ DWORD dwAfterSleepTime = NULL) CONST;
	VOID        SimlationKey_Immediately(_In_ DWORD dwAscii) CONST;

	VOID		PushSimlationKey(_In_ DWORD dwAscii) CONST;

	// ����
	VOID		SetSpeed(__in float fSpeed) CONST;
	float		GetSpeed() CONST;
	// �Ż�
	VOID		SetOptimization(__in DWORD dwSleepTime) CONST;
	// �Զ�������
	BOOL		Sleep(__in DWORD dwSleepTime) CONST;
	// �л��ӽ�
	VOID		SwitchViewOfAngel() CONST;
	// ���ô��ڱ���
	BOOL		SetGameTitle() CONST;
	// �ȴ���ʱ
	BOOL		WaitToDo(DWORD dwSleepTime, DWORD dwMaxSleepTime, std::function<BOOL(void)> f) CONST;
	// �����ж�(��ʱ����TRUE)
	BOOL		TimeOut_Condiction(DWORD dwMaxTimeOut, std::function<BOOL(void)> fnCondiction) CONST;
	BOOL		TimeOut_Condiction_GameRun(DWORD dwMaxTimeOut, std::function<BOOL(void)> fnCondiction) CONST;
	// Text Convert to Point
	Point		GetPoint_By_ScriptParm(CONST std::wstring& cwstr);

	//
	VOID		CreateKeepServerAliveThread() CONST;

	//
	VOID		SendKeepALive(_In_ TaskInfoParm* pTaskParam, _In_ CONST std::wstring& wsContent) CONST;
private:
	// �������߳�
	static unsigned	KeepALiveThread(LPVOID lpParm);

	DSIABLE_COPY_AND_ASSIGN(CSomeFun)
};



#endif