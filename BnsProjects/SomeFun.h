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
	// 初始化DLL
	BOOL		InitDLL() CONST;
	
	// 初始化内存共享
	BOOL		InitMapView() CONST;
	// 初始化AccountGame
	BOOL		InitAccountGame() CONST;
	
	// 模拟按键
	BOOL		SimlationKey(_In_ DWORD dwAscii, _In_ DWORD dwSleepTime = 200, _In_ DWORD dwAfterSleepTime = NULL) CONST;
	VOID        SimlationKey_Immediately(_In_ DWORD dwAscii) CONST;

	VOID		PushSimlationKey(_In_ DWORD dwAscii) CONST;

	// 加速
	VOID		SetSpeed(__in float fSpeed) CONST;
	float		GetSpeed() CONST;
	// 优化
	VOID		SetOptimization(__in DWORD dwSleepTime) CONST;
	// 自定义休眠
	BOOL		Sleep(__in DWORD dwSleepTime) CONST;
	// 切换视角
	VOID		SwitchViewOfAngel() CONST;
	// 设置窗口标题
	BOOL		SetGameTitle() CONST;
	// 等待超时
	BOOL		WaitToDo(DWORD dwSleepTime, DWORD dwMaxSleepTime, std::function<BOOL(void)> f) CONST;
	// 条件判断(超时返回TRUE)
	BOOL		TimeOut_Condiction(DWORD dwMaxTimeOut, std::function<BOOL(void)> fnCondiction) CONST;
	BOOL		TimeOut_Condiction_GameRun(DWORD dwMaxTimeOut, std::function<BOOL(void)> fnCondiction) CONST;
	// Text Convert to Point
	Point		GetPoint_By_ScriptParm(CONST std::wstring& cwstr);

	//
	VOID		CreateKeepServerAliveThread() CONST;

	//
	VOID		SendKeepALive(_In_ TaskInfoParm* pTaskParam, _In_ CONST std::wstring& wsContent) CONST;
private:
	// 检查掉线线程
	static unsigned	KeepALiveThread(LPVOID lpParm);

	DSIABLE_COPY_AND_ASSIGN(CSomeFun)
};



#endif