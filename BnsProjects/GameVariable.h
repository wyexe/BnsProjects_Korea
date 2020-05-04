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
///////������, ��Ȼ����ͷ�ļ������ظ�///////////////////////////////////////////////////////////////////
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

extern DWORD					g_dwMoveBase;								// �����ַ
extern DWORD					g_dwMoveStatusOffset;						// ��·״̬ƫ��
extern em_GameCmd				g_GameStatus;								// ��Ϸ״̬
extern DWORD					g_dwTimerId;								// ʱ��Id
extern DWORD					g_dwSimulationBase;							// ������ַ
extern DWORD					g_dwSimulationCALL;							// ����CALL
extern DWORD					g_dwOptimizationTime;						// �Ż���ʱ��
extern DWORD					g_dwOldD3dCall;								// �ɵ�D3d CALL
extern DWORD					g_dwHookD3dAddr;							// Hook D3d��ַ
extern DWORD					g_dwHookSpeedAddr;							// Hook���ٵĵ�ַ
extern DWORD					g_dwSpeedCALL;								// Hook���ٵ�CALL
extern float					g_fSpeed;									// ���ٱ���
extern BOOL						g_bExitGame;								// �Ƿ���Ҫǿ���˳���Ϸ
extern BOOL						g_bUninstall;								// �Ƿ�Ҫж��DLL


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

	// ��ȭʦԲ��ɱ�ֱ�
	BOOL Action_Map_Kffm_Fm_SpecialMonster(_In_ std::function<VOID(std::map<std::wstring, FixPointInfo>&)> fn) CONST;
	

	// ��ȡȫ�ֱ���
	DWORD& GetVariable_By_Name(_In_ CONST std::wstring& wsName) CONST;
	DWORD  SetVariable_By_Name(_In_ CONST std::wstring& wsName, _In_ DWORD dwValue) CONST;
	BOOL   Action_Variable_By_Name(_In_ CONST std::wstring& wsName, _In_ DWORD dwValue, _In_ std::function<VOID(VOID)> fn) CONST;


	// ȫ��Tick
	ULONGLONG& GetTickVariable_By_Name(_In_ CONST std::wstring& wsName) CONST;

	// �����ڴ�ָ��
	PSHARED_INFO& GetShareInfo() CONST;
	PACCOUNT_INFO_GAME& GetAccShareInfo() CONST;

	BOOL DoList_HookList(_In_ std::function<VOID(std::vector<MyTools::MYHOOK_CONTENT>&)> fn) CONST;

	CONST std::wstring GetCurMapInfo(_In_ DWORD dwCurrentCityId, _In_ DWORD dwCurrentMapId) CONST;
	
	// �Ƿ��������������
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

	// ������
	BOOL Action_Set_GiftFilterName(_In_ std::function<VOID(std::set<std::wstring>&)> fn) CONST;

	BOOL IsMainTask_By_Id(_In_ DWORD dwTaskId) CONST;
	BOOL IsSpecialNpcTask(_In_ CONST TaskInfoParm& Ti) CONST;

	MyTools::CLLock& GetVarLock() CONST;

	// ����Tick
	ULONGLONG& GetSetSwitchChannelTick() CONST;

	DWORD GetEnvironmentValue() CONST;

	static time_t SystemTimeToTime_t(const SYSTEMTIME& st);

	static SYSTEMTIME ConvertTextToSysTime(_In_ CONST std::wstring& wsText);

	static CONST std::wstring GetSpendHourForText(_In_ CONST SYSTEMTIME& SysTime1, _In_ CONST SYSTEMTIME& SysTime2);

	// Hook ��ֹˢ�µ�ǰ������
	DWORD GetHookHeightAddr() CONST;

	// Hook һֱ��
	DWORD GetHookJmpAddr() CONST;

	// ����ǿ��С��
	VOID SetForceBackToSwitchPlayer() CONST;

	// ����ֹͣ�һ�
	VOID SetStopGame() CONST;

	// ��ǰ���ڹһ��Ľű�����
	std::wstring& CurrentScriptName() CONST;

	// ��ȡ��ǰ�ֿ������
	CONST Share_Global_Warehouse* GetWarehouseConfig() CONST;
	CONST Share_Global_Warehouse* GetWarehouseConfig(_In_ CONST std::wstring& wsPlayerName) CONST;

	// �ٻ���BOSS�� Ĭ�ϼ����б�
	struct ClassesBossSkill
	{
		em_Skill_Type	emSkillType;
		DWORD			dwSkillId;
	};
	CONST std::vector<ClassesBossSkill>& GetSummonerBossSkillVec() CONST;

	// ����ʦ��BOSS��Ĭ�ϼ����б�
	CONST std::vector<ClassesBossSkill>& GetWarlockBossSkillVec() CONST;

private:
	std::function<VOID(VOID)> _fnHelpThreadLoopMethodPtr;
private:
	DSIABLE_COPY_AND_ASSIGN(CGameVariable);
};



#endif