#ifndef __GITBNSPROJECTS_BNSPROJECTS_OBJECTS_GAMEOBJECT_PERSON_PERSONFUNCTION_H__
#define __GITBNSPROJECTS_BNSPROJECTS_OBJECTS_GAMEOBJECT_PERSON_PERSONFUNCTION_H__

#include "GameVariable.h"

#define MOVE_FLAG_NORMAL		0x2
#define MOVE_FLAG_DISABLE_SPEED 0x4
#define MOVE_FLAG_DISABLE_JUMP  0x8
#define MOVE_FLAG_DISABLE_SS	0x10

class CPersonFunction : public MyTools::CClassInstance<CPersonFunction>
{
public:
	CPersonFunction();
	~CPersonFunction() = default;

public:
	// 等待读条完毕
	BOOL WaitForProgressBar(_In_ DWORD dwMaxTime = 10 * 1000) CONST;

	// 切换频道
	BOOL SwitchChannel(_In_ DWORD dwChannel) CONST;

	// 隐藏玩家
	BOOL HidePlayer() CONST;

	// 做花式动作
	BOOL DoAction(DWORD dwActionId) CONST;

	// 小退
	BOOL BackToSwitchPlayer() CONST;

	// 是否过图中
	BOOL IsGameLoad() CONST;

	// 是否出现动画
	BOOL IsGameChart() CONST;

	// 等待过图
	BOOL WaitGameLoad(_In_ BOOL bForceWait = FALSE, _In_ BOOL bCheckSS = TRUE) CONST;

	// 等待出现过图(参数=超时时间)
	BOOL IsWaitForGameLoad(_In_ DWORD dwMaxTime) CONST;

	// 备份走路Flag
	VOID SetMoveFlag(_In_ DWORD dwFlag);
	// 恢复走路Flag
	VOID RestoreMoveFlag();

	// 走路
	BOOL Move(_In_ CONST Point& TargetPoint) CONST;
	BOOL MoveToPoint(_In_ CONST Point& TargetPoint, _In_ float fDis, _In_ float fMaxDis = MAX_MOVE_DIS) CONST;
	BOOL MoveToPoint_For_List(_In_ std::vector<Point>& PointVec) CONST;
	BOOL Move_2D(_In_ CONST FixPointInfo& TarFixPoint) CONST;

	// 获取当前城市Id
	DWORD GetCityId() CONST;

	// 获取当前地图Id
	DWORD GetMapId() CONST;

	// 获取最大频道
	DWORD GetMaxChannel() CONST;

	// 遁术
	BOOL TransportCiry(_In_ DWORD dwTargetCityId) CONST;

	// 是否存在BUFFID
	BOOL ExistBuff_Person(_In_ DWORD dwBuffId, _Out_opt_ GAMEOBJ_BUFF* pGameObjBuff = nullptr) CONST;
	BOOL ExistBuff_Person(_In_ CONST std::vector<DWORD>& BuffVec) CONST;

	// 复活
	BOOL Resurrect() CONST;

	// 尝试复活
	BOOL TryRelive() CONST;

	// 等待读条
	BOOL WaitToLoadProgressBar(DWORD dwSleepTime, DWORD dwMaxSleepTime, std::function<void(void)> f) CONST;

	// 一次捡物
	BOOL Pickup(CONST struct _GameObjectID& GameObjectID_) CONST;

	// 转盘
	//BOOL Turntable(_In_ DWORD dwAttendancePanel) CONST;

	// 放下手上的东西, 取消捡物状态
	BOOL CancelPutDownStatus() CONST;

	// 逃脱
	BOOL DoEscape() CONST;

	// 是否Tab状态
	BOOL Exist_Lux_Tab_Buff() CONST;

	// 是否使用力士E的状态
	BOOL Exist_Lux_E_Buff() CONST;

	// Exist Lux V Buff
	BOOL Exist_Lux_V_Buff() CONST;

	// 是否存在召唤的友情状态
	BOOL Exist_Sumoner_X_Buff() CONST;

	// 是否存在召唤的花粉状态
	BOOL Exist_Sumoner_3_Buff() CONST;

	// 等待技能读条完毕
	BOOL WaitForProgressBarBySkill(_In_ DWORD dwMaxTimeOut) CONST;

	// 等待战斗状态取消!
	BOOL WaitForWarStatus() CONST;

	// 退出门派!
	BOOL ExitSchool() CONST;

	// 跑
	enum em_Run_Type{ em_Run_Type_OnFloor, em_Run_Type_FastFly };
	BOOL Run(_In_ em_Run_Type emRunType) CONST;
private:
	std::tuple<CONST std::wstring, CONST std::wstring, CONST std::wstring, CONST std::wstring, DWORD, DWORD, Point, Point> MakeBnsConfigParm(_In_ CONST std::wstring& wsMapName, _In_ CONST Point& TarPoint) CONST;
private:
	DSIABLE_COPY_AND_ASSIGN(CPersonFunction)
private:
	DWORD _dwMoveFlag;
	DWORD _dwMoveFlagBackup;
};


#endif