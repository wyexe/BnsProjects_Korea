#ifndef __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_����Ĺ��_DAILYTASK_SACRIFICEGRAVE_H__
#define __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_����Ĺ��_DAILYTASK_SACRIFICEGRAVE_H__

#include "DailyTask_Common.h"
#include "DailyTask_Team.h"
#include "DailyTask_BossSkill.h"

class CDailyTask_SacrificeGrave : public CDailyTask_Common
{
public:
	CDailyTask_SacrificeGrave();
	~CDailyTask_SacrificeGrave() = default;

	virtual BOOL Run() override;
private:

private:
	/// ---Action----------------------------
	virtual BOOL AccpetDailyTask() CONST override;
	virtual VOID StudyDailySkill() CONST override;

	// �ߵ������ſ�
	virtual BOOL MoveToDungeon() override;
private:
	///------Attribute-------------------------------------
	virtual BOOL IsInDungeon() CONST override;
private:

private:
	CDailyTask_SacrificeGrave operator=(CONST CDailyTask_SacrificeGrave&) = delete;
};



#endif // !__GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_����Ĺ��_DAILYTASK_SACRIFICEGRAVE_H__
