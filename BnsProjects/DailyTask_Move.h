#ifndef __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_COMMON_DAILYTASK_MOVE_H__
#define __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_COMMON_DAILYTASK_MOVE_H__

#include "GameVariable.h"

class CDailyTask_Move
{
public:
	CDailyTask_Move() = default;
	~CDailyTask_Move() = default;

	// ·çÔÂ¹Ý
	BOOL MoveToFengYueGuanCenter();

	//
	BOOL MoveToMap(_In_ DWORD dwMapId);

	//
	BOOL MoveToFengYueGuanSellNpc();

	//
	BOOL MoveToFengYueGuanRepair();

	//
	BOOL MoveToVecPoint(_In_ CONST std::vector<Point>& Vec, _In_ int Index) CONST;

	//
	VOID MoveToVecPoint(_In_ CONST std::vector<Point>& Vec, _In_ std::function<BOOL(CONST Point&)> ExitPtr) CONST;

	// 
	std::pair<int, float> FindNearestPoint(_In_ CONST std::vector<Point>& Vec, _In_ CONST Point& CompPoint, _In_ float fMinDis = 30) CONST;

	//
	BOOL MoveToTarget(_In_ CONST std::wstring& wsName, _In_ float fMaxDis) CONST;

	//
	BOOL FarmAndMove(_In_ CONST std::vector<Point>& VecPoint, _In_ float fKillDis) CONST;
private:
	
};



#endif // !__GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_COMMON_DAILYTASK_MOVE_H__
