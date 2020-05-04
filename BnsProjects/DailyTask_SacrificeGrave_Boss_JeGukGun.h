#ifndef __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_遗忘墓地_DAILYTASK_SACRIFICEGRAVE_BOSS_JEGUKGUN_H__
#define __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_遗忘墓地_DAILYTASK_SACRIFICEGRAVE_BOSS_JEGUKGUN_H__

#include "DailyTask_Boss_Template.h"
#include "DailyTask_BossSkill.h"

class CMonster;
class CDailyTask_Move;
class CDailyTask_SacrificeGrave_Boss_JeGuGun : public CDailyTask_Boss_Template
{
private:

public:
	CDailyTask_SacrificeGrave_Boss_JeGuGun(_In_ CONST CMonster& Boss, _In_ CONST CDailyTask_Move& DailyTask_Move);
	~CDailyTask_SacrificeGrave_Boss_JeGuGun() = default;

	virtual BOOL Kill() override;
private:
	/// ---------剑士------------------------------------------------------------------
	VOID KillBoss_BladeMaster();

	VOID Skill_BladeMaster();

private:
	/// -----气功---------------------------------------------
	VOID KillBoss_ForceMaster();

	VOID Skill_ForceMaster();
private:
	// ---Common---------------------------------------------------------
	// 
	VOID NormalMove();

	//
	VOID MeleeMove();

	//
	VOID FarRangeMove();

};



#endif // !__GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_遗忘墓地_DAILYTASK_SACRIFICEGRAVE_BOSS_JEGUKGUN_H__
