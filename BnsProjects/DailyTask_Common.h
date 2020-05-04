#ifndef __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_COMMON_DAILYTASK_COMMON_H__
#define __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_COMMON_DAILYTASK_COMMON_H__

#include "GameVariable.h"
#include "DailyTask_Team.h"
#include "DailyTask_Move.h"

class CMonster;
class CDailyTask_Common
{
protected:
#define PERSON_MOVE_W 0x2
#define PERSON_MOVE_S 0x4
#define PERSON_MOVE_A 0x8
#define PERSON_MOVE_D 0x16
	typedef std::function<BOOL(CONST std::vector<Point>&)> StepMethodPtr;
public:
	typedef std::vector<Point> DailyPath;
	typedef std::map<std::wstring, DailyPath>::iterator DailyPathItr;
	typedef std::pair<bool, DailyPathItr> DailyPathPair;

	struct KillBossMethod
	{
		em_Player_Classes emPlayerClasses;
		std::function<VOID(CONST CMonster&)> MethodPtr;
	};
public:
	CDailyTask_Common();
	~CDailyTask_Common() = default;

	virtual BOOL Run() = NULL;
protected:
	template<typename T, typename em>
	std::vector<Point> FindVecPointByStep(_In_ CONST std::vector<T>& Vec, em emStep) CONST _NOEXCEPT
	{
		std::vector<Point> VecPoint;
		for (auto& itm : Vec)
		{
			if (itm.emStep == emStep)
			{
				VecPoint.push_back(itm.Pt);
			}
			else if (VecPoint.size() != 0)
				break;
		}

		return VecPoint;
	}

	template<typename T>
	int FindNearestIndexByVecStep(_In_ CONST std::vector<T>& Vec, _In_ CONST Point& TarPoint, _In_ float fMinDis = 16) CONST _NOEXCEPT
	{
		auto Index = -1;
		for (UINT i = 0;i < Vec.size(); ++i)
		{
			float fDis = CBaseObject::Get3DDis(Vec.at(i).Pt, TarPoint);
			if (fDis < fMinDis)
			{
				fMinDis = fDis;
				Index = static_cast<decltype(Index)>(i);
			}
		}

		return Index;
	}
private:
	VOID MoveToPoint(_In_ CONST Point& Pt, _In_ CONST FixPointInfo& Range) CONST;
protected:
	//
	VOID SetBossEnviroment(_In_ CONST CMonster& Boss);

	//
	VOID RestoreBossEnviroment();

	//
	VOID Move(_In_ DWORD dwMoveFlag);

	//
	VOID StopMove_SingleFlag(_In_ DWORD dwMoveFlag);

	//
	VOID StopMove_MultiFlag(_In_ DWORD dwMoveFlag);

	//
	VOID Move_GameSend(_In_ DWORD dwMoveFlag, _In_ BOOL IsPush);

	//
	BOOL IsFaceToBoss(_In_ CONST CMonster& Boss, _In_ float fRange) CONST;

	// 等待队友全挂(里面有自救和救队友)
	BOOL WaitToMemberDead() CONST;

	VOID MoveThenKill(_In_ CONST std::vector<Point>& VecPoint) CONST;
protected:

	//
	virtual BOOL IsInDungeon() CONST = NULL;

	//
	virtual BOOL AccpetDailyTask() CONST = NULL;

	//
	virtual VOID StudyDailySkill() CONST = NULL;

	//
	virtual BOOL MoveToDungeon() = NULL;
protected:
	CDailyTask_Team _DailyTask_Team;
	CDailyTask_Move _DailyTask_Move;
private:
	DWORD _dwKillMonsterMovDis;
	DWORD _dwDisableBeckonCat;
	DWORD _dwMoveFlag;
};


#endif // !__GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_COMMON_DAILYTASK_COMMON_H__
