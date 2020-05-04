#ifndef __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_黑龙教地下监狱_DAILYTASK_HUKRYONGPRISON_H__
#define __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_黑龙教地下监狱_DAILYTASK_HUKRYONGPRISON_H__

#include "DailyTask_Common.h"
#include "DailyTask_Team.h"
#include "DailyTask_BossSkill.h"

class CMonster;
class CDailyTask_Skill;

class CDailyTask_HukryongPrison : public CDailyTask_Common
{
private:
	
public:
	CDailyTask_HukryongPrison();
	~CDailyTask_HukryongPrison() = default;

	virtual BOOL Run() override;
private:

	
private:
	/// ----Action--------------------------------------
	virtual BOOL AccpetDailyTask() CONST override;

	virtual VOID StudyDailySkill() CONST override;

	// 走到副本门口
	virtual BOOL MoveToDungeon() override;
private:
	///------Attribute-------------------------------------
	virtual BOOL IsInDungeon() CONST override;
private:
	
private:
	CDailyTask_HukryongPrison operator=(CONST CDailyTask_HukryongPrison&) = delete;
};



#endif // !__GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_黑龙教地下监狱_DAILYTASK_HUKRYONGPRISON_H__
