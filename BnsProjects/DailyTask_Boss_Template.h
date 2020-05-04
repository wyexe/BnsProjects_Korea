#ifndef __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_COMMON_DAILYTASK_BOSS_TEMPLATE_H__
#define __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_COMMON_DAILYTASK_BOSS_TEMPLATE_H__

#include "DailyTask_Skill.h"
#include "DailyTask_Move.h"
#include "Monster.h"

class CDailyTask_Boss_Template
{
protected:
	struct KillBossMethod
	{
		em_Player_Classes			emPlayerClasses;
		std::function<VOID(VOID)>	MethodPtr;
	};
public:
	CDailyTask_Boss_Template(_In_ CONST CMonster& Boss, _In_ CONST CDailyTask_Move& DailyTask_Move, CONST FixPointInfo& BossRange);
	~CDailyTask_Boss_Template() = default;

	virtual BOOL Kill() = NULL;

	BOOL IsFaceToBoss(_In_ float fRange) CONST;
protected:
	CMonster				_Boss;
	DWORD						_dwLastSkillId;
	CDailyTask_Move		_DailyTask_Move;
	FixPointInfo			_BossRange;
	CDailyTask_Skill			_DailyTask_Skill;
};



#endif // !__GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_COMMON_DAILYTASK_BOSS_TEMPLATE_H__
