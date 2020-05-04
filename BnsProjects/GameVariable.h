#ifndef __BNS_GAMEVARIABLE_H__
#define __BNS_GAMEVARIABLE_H__
#include <map>
#include <set>
#include <queue>
#include <VMProtectSDK.h>
#include <MyTools/CLHook.h>
#include <MyTools/ClassInstance.h>
#include "GameStruct.h"
#include "GameMutilLanguageText.h"
///////声明类, 不然各种头文件引用重复///////////////////////////////////////////////////////////////////
//class CGameObject;
//class CSomeFun;
//class CObjectFunction;
//class CMonster;
//class CNpc;
//class CBag;
//class CDropItem;
//class CCorpse;
//class CSkill;
//class CTask;
//class CGameSend;
//class CGameFunction;

#define MAX_MOVE_DIS			35.0f
#define BnsMove(a,b)			CPersonFunction::GetInstance().MoveToPoint(a,b)

extern DWORD					g_dwMoveBase;								// 坐标基址
extern DWORD					g_dwMoveStatusOffset;						// 走路状态偏移
extern em_GameCmd				g_GameStatus;								// 游戏状态
extern DWORD					g_dwTimerId;								// 时钟Id
extern DWORD					g_dwSimulationBase;							// 按键基址
extern DWORD					g_dwSimulationCALL;							// 按键CALL
extern DWORD					g_dwOptimizationTime;						// 优化的时间
extern DWORD					g_dwOldD3dCall;								// 旧的D3d CALL
extern DWORD					g_dwHookD3dAddr;							// Hook D3d地址
extern DWORD					g_dwHookSpeedAddr;							// Hook加速的地址
extern DWORD					g_dwSpeedCALL;								// Hook加速的CALL
extern float					g_fSpeed;									// 加速倍数
extern BOOL						g_bExitGame;								// 是否需要强制退出游戏
extern BOOL						g_bUninstall;								// 是否要卸载DLL


#define GameRun	(g_GameStatus == em_GameCmd_Start)
#define StopGame g_GameStatus = em_GameCmd_Stop
#define StartGame g_GameStatus = em_GameCmd_Start

class CLLock;
class CGameVariable : public MyTools::CClassInstance<CGameVariable>
{
public:
	typedef struct _CityInfo
	{
		_CityInfo() = default;
		std::wstring	wsMapName;
		DWORD		dwCityId;
	}CityInfo;

	typedef struct _ProvinceInfo
	{
		_ProvinceInfo() = default;
		std::wstring wsProvinceName;
		std::wstring	wsMapName;
		DWORD		dwCityId;
	}ProvinceInfo;

	typedef struct _MainThreadFunPtrInfo
	{
		MainThreadFunPtr MainThreadFunPtr_;
		HANDLE			 hEvent;
	}MainThreadFunPtrInfo;

public:
	CGameVariable();
	~CGameVariable() = default;

public:
	BOOL DoList_MainThreadFunPtr(_In_ std::function<VOID(std::queue<MainThreadFunPtrInfo>&)> fn) CONST;
	BOOL PushMainThreadFunPtr_synch(_In_ MainThreadFunPtr MainThreadFunPtr_) CONST;
	BOOL ClearMainThreadFunPtr() CONST;

	// 气拳师圆形杀怪表
	BOOL Action_Map_Kffm_Fm_SpecialMonster(_In_ std::function<VOID(std::map<std::wstring, FixPointInfo>&)> fn) CONST;
	

	// 获取全局变量
	DWORD& GetVariable_By_Name(_In_ CONST std::wstring& wsName) CONST;
	DWORD  SetVariable_By_Name(_In_ CONST std::wstring& wsName, _In_ DWORD dwValue) CONST;
	BOOL   Action_Variable_By_Name(_In_ CONST std::wstring& wsName, _In_ DWORD dwValue, _In_ std::function<VOID(VOID)> fn) CONST;


	// 全局Tick
	ULONGLONG& GetTickVariable_By_Name(_In_ CONST std::wstring& wsName) CONST;

	// 共享内存指针
	PSHARED_INFO& GetShareInfo() CONST;
	PACCOUNT_INFO_GAME& GetAccShareInfo() CONST;

	BOOL DoList_HookList(_In_ std::function<VOID(std::vector<MyTools::MYHOOK_CONTENT>&)> fn) CONST;

	CONST std::wstring GetCurMapInfo(_In_ DWORD dwCurrentCityId, _In_ DWORD dwCurrentMapId) CONST;
	
	// 是否特殊意义的名字
	enum em_SpicalName_Type
	{
		em_SpicalName_Type_None,
		em_SpicalName_Type_Mail,
		em_SpicalName_Type_Warehouse
	};
	em_SpicalName_Type ExistSpicalName(_In_ CONST std::wstring& wsPlayerName) CONST;

	DWORD GetGameChartValue() CONST;

	CONST CityInfo* FindInstanceMapIdList(_In_ DWORD dwInstanceMapId) CONST;
	CONST ProvinceInfo* FindProvinceInfoList(IN DWORD dwCityId) CONST;

	// 收礼箱
	BOOL Action_Set_GiftFilterName(_In_ std::function<VOID(std::set<std::wstring>&)> fn) CONST;

	BOOL IsMainTask_By_Id(_In_ DWORD dwTaskId) CONST;
	BOOL IsSpecialNpcTask(_In_ CONST TaskInfoParm& Ti) CONST;

	MyTools::CLLock& GetVarLock() CONST;

	// 换线Tick
	ULONGLONG& GetSetSwitchChannelTick() CONST;

	DWORD GetEnvironmentValue() CONST;

	static time_t SystemTimeToTime_t(const SYSTEMTIME& st);

	static SYSTEMTIME ConvertTextToSysTime(_In_ CONST std::wstring& wsText);

	static CONST std::wstring GetSpendHourForText(_In_ CONST SYSTEMTIME& SysTime1, _In_ CONST SYSTEMTIME& SysTime2);

	// Hook 禁止刷新当前的坐标
	DWORD GetHookHeightAddr() CONST;

	// Hook 一直跳
	DWORD GetHookJmpAddr() CONST;

	// 设置强制小退
	VOID SetForceBackToSwitchPlayer() CONST;

	// 设置停止挂机
	VOID SetStopGame() CONST;

	// 当前正在挂机的脚本名称
	std::wstring& CurrentScriptName() CONST;

	// 获取当前仓库的配置
	CONST Share_Global_Warehouse* GetWarehouseConfig() CONST;
	CONST Share_Global_Warehouse* GetWarehouseConfig(_In_ CONST std::wstring& wsPlayerName) CONST;

	// 召唤的BOSS的 默认技能列表
	struct ClassesBossSkill
	{
		em_Skill_Type	emSkillType;
		DWORD			dwSkillId;
	};
	CONST std::vector<ClassesBossSkill>& GetSummonerBossSkillVec() CONST;

	// 咒术师的BOSS的默认技能列表
	CONST std::vector<ClassesBossSkill>& GetWarlockBossSkillVec() CONST;

private:
	std::function<VOID(VOID)> _fnHelpThreadLoopMethodPtr;
private:
	DSIABLE_COPY_AND_ASSIGN(CGameVariable);
};



#endif