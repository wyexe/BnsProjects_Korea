#ifndef __GITBNSPROJECTS_BNSPROJECTS_SOMEFUN_FARM_FARMTASK_H__
#define __GITBNSPROJECTS_BNSPROJECTS_SOMEFUN_FARM_FARMTASK_H__

#include "FarmMonster.h"

class CFarmTask : public CFarmMonster
{
public:
	explicit CFarmTask() = default;
	~CFarmTask() = default;

public:
	// 清怪 in 条件
	BOOL Farm_By_Condiction(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<UINT(VOID)> fnGetAroundCount, _In_ std::function<BOOL(std::vector<CMonster>&, CONST FixPointInfo&)> fnBreak) CONST;

	//特殊清怪，只用卡刀。不用任何其他技能。
	BOOL Farm_By_Condiction_ts(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<UINT(VOID)> fnGetAroundCount, _In_ std::function<BOOL(std::vector<CMonster>&, CONST FixPointInfo&)> fnBreak) CONST;

	// 清除周围范围内 的怪物
	BOOL ClearAroundMonster(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<BOOL(VOID)> fnBreak) CONST;
	BOOL ClearAroundMonster(_In_ CONST FixPointInfo& FixPtInfo) CONST;

	//
	BOOL ClearAroundNpc(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<BOOL(VOID)> fnBreak) CONST;

	// 清除周围的怪物 in List
	//BOOL ClearMonster_In_List_By_Condiction(_In_ CONST FixPointInfo& FixPtInfo, _In_ CONST std::vector<Point>& vPoint, _In_ std::function<BOOL(void)> fnComplete) CONST;

	// 清除范围内目标怪是自己
	BOOL ClearAround_TarIdSelf(_In_ CONST FixPointInfo& FixPtInfo) CONST;

	// 清除范围内特别的目标怪
	BOOL ClearAround_TarName(_In_ CONST std::wstring& wsMonsterName, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	//清除范围内特别的目标怪-只卡刀
	BOOL ClearAround_TarName_ts(_In_ CONST std::wstring& wsMonsterName, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	// 吃药
	BOOL EatFood(_In_ DWORD dwPercentHp) CONST;

	// 捡尸体
	//BOOL TakePlayer_By_Condiction(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<BOOL(CONST CPlayer&)> fnFind, _In_ std::function<BOOL(std::vector<CPlayer>&)> fnComplete) CONST;

	//BOOL TakePlayer_By_List(_In_ CONST FixPointInfo& FixPtInfo, _In_ CONST std::vector<Point>& vPoint, _In_ std::function<BOOL(CONST CPlayer&)> fnFind, _In_ std::function<BOOL(std::vector<CPlayer>&)> fnComplete) CONST;

	
	// 跟随Npc
	BOOL FollowPlayer(_In_ CONST CPlayer& Player, _In_ CONST Point& EndPoint, _In_ float fKillDis) CONST;

	// '站着捡'
	BOOL Pickup_By_Condition(_In_ CONST std::wstring& wsName) CONST;

	// 跟随队长打怪
	BOOL FollowCaptain() CONST;
private:
	DSIABLE_COPY_AND_ASSIGN(CFarmTask)
	//BOOL DoAround_By_List(_In_ CONST FixPointInfo& FixPtInfo, _In_ CONST std::vector<Point>& vPoint, _In_ std::function<BOOL(CONST FixPointInfo&)> fnBreak) CONST;
};



#endif