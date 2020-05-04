#ifndef __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_雪人洞_DAILYTASK_SNOWHOLE_H__
#define __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_雪人洞_DAILYTASK_SNOWHOLE_H__

#include "DailyTask_Common.h"
#include "DailyTask_Team.h"
#include "DailyTask_BossSkill.h"

class CMonster;
class CDailyTask_Skill;
class CCat;
class CDailyTask_SnowHole : public CDailyTask_Common
{
private:
	enum em_Dungeon_SnowHole_Step
	{
		em_Dungeon_SnowHole_Step_None,
		em_Dungeon_SnowHole_Step1,
		em_Dungeon_SnowHole_Step2,
		em_Dungeon_SnowHole_Step3,
		em_Dungeon_SnowHole_Step4,
		em_Dungeon_SnowHole_Step5,
		em_Dungeon_SnowHole_Step6,
		em_Dungeon_SnowHole_Step7,
		em_Dungeon_SnowHole_Step8,
		em_Dungeon_SnowHole_Step9,
		em_Dungeon_SnowHole_Step10,
		em_Dungeon_SnowHole_End,
	};
	struct SnowHolePoint
	{
		Point Pt;
		em_Dungeon_SnowHole_Step emStep;
	};
	struct Dungeon_SnowHole_Content
	{
		em_Dungeon_SnowHole_Step	emStep;
		StepMethodPtr				StepPtr;
	};

	enum
	{
		em_BladeMaster_火龙BUFF = 0x9A2E08,
		em_Skill_灯笼雪人_挥击 = 0x5A0FAA,
		em_Skill_灯笼雪人_腾空 = 0x5A0FAC,
		em_Skill_灯笼雪人_击掌 = 0x5A0FAB,
		em_Skill_灯笼雪人_墩摔 = 0x5A0FAD,
		em_Skill_灯笼雪人_前扑 = 0x5A0FB0,
		em_Skill_灯笼雪人_吸取热气 = 0x5A0FB3,
		em_Skill_灯笼雪人_天来跃 = 0x5A0FB1,
		em_Skill_灯笼雪人_冻波撼地 = 0x5A0FB9,
		em_Skill_灯笼雪人_抢夺 = 0x5A0FB2,
		em_Skill_灯笼雪人_热气注入 = 0x5A0FBA,
		em_Skill_灯笼雪人_咆哮 = 0x5A0FB5,
		em_Skill_灯笼雪人_寒气喷发 = 0x5A0FB6,
		em_Skill_灯笼雪人_寒气雪舞左 = 0x5A0FB7,
		em_Skill_灯笼雪人_寒气雪舞右 = 0x5A0FB8,
	};
public:
	CDailyTask_SnowHole();
	~CDailyTask_SnowHole() = default;

	virtual BOOL Run() override;

	// 灯笼雪人
	BOOL KillBoss();
private:
	//
	BOOL Initialize();

	// 起点到蘑菇阵图腾
	BOOL Step1(_In_ CONST std::vector<Point>& VecPoint);

	// 大力丘八佬
	BOOL Step2(_In_ CONST std::vector<Point>& VecPoint);

	// 第二个蘑菇阵
	BOOL Step3(_In_ CONST std::vector<Point>& VecPoint);

	// 雪人捕获队长之前的一波小怪
	BOOL Step4(_In_ CONST std::vector<Point>& VecPoint);

	// 雪人捕获队长
	BOOL Step5(_In_ CONST std::vector<Point>& VecPoint);

	// 跑到台阶上(第三个蘑菇阵前)
	BOOL Step6(_In_ CONST std::vector<Point>& VecPoint);

	// 第三个蘑菇阵
	BOOL Step7(_In_ CONST std::vector<Point>& VecPoint);

	// 去最终BOSS
	BOOL Step8(_In_ CONST std::vector<Point>& VecPoint);

	// 灯笼雪人
	BOOL Step9(_In_ CONST std::vector<Point>& VecPoint);

	// 卡鲁拉
	BOOL Step10(_In_ CONST std::vector<Point>& VecPoint);

	// 战斗结束后 整理背包
	BOOL End(_In_ CONST std::vector<Point>&);

	// 蛇菇阵
	BOOL SnakeMushRoom(_In_ CONST std::vector<Point>& VecPoint);

	//
	VOID KillBoss_Warlock(_In_ CONST CMonster& Boss);

	//
	VOID KillBoss_BladeMaster(_In_ CONST CMonster& Boss);

	//
	VOID KillBoss_ForceMaster(_In_ CONST CMonster& Boss);

	//
	VOID KillBoss_KongFuMaster(_In_ CONST CMonster& Boss);

	//
	VOID KillBoss__Gunner(_In_ CONST CMonster& Boss);

	//
	VOID KillHideBoss(_In_ CONST CMonster& Boss);
private:
	// ---------剑士------------------------------------------------------------------
	VOID BossSkill_挥击_BladeMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_腾空_BladeMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);
	
	VOID BossSkill_击掌_BladeMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_墩摔_BladeMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_前扑_BladeMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_吸取热气_BladeMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_抢夺_BladeMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_天来跃_BladeMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_冻波撼地_BladeMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID Skill_BladeMaster(_In_ CONST CMonster& Boss, _In_ CONST CDailyTask_Skill& DailyTaskSkill);

	// -------气功-----------------------------------------------------------------------
	VOID BossSkill_前扑_ForceMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_吸取热气_ForceMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_抢夺_ForceMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_天来跃_ForceMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_冻波撼地_ForceMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID Skill_ForceMaster(_In_ CONST CMonster& Boss, _In_ CONST CDailyTask_Skill& DailyTaskSkill);

	// ---------拳师------------------------------------------------------------------
	VOID BossSkill_挥击_KongFuMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_腾空_KongFuMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_击掌_KongFuMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_墩摔_KongFuMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_前扑_KongFuMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_吸取热气_KongFuMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_抢夺_KongFuMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_天来跃_KongFuMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_冻波撼地_KongFuMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID Skill_KongFuMaster(_In_ CONST CMonster& Boss, _In_ CONST CDailyTask_Skill& DailyTaskSkill);

	// ---------刺客------------------------------------------------------------------
	VOID BossSkill_挥击_AssassinMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_腾空_AssassinMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_击掌_AssassinMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_墩摔_AssassinMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_前扑_AssassinMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_吸取热气_AssassinMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_抢夺_AssassinMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_天来跃_AssassinMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_冻波撼地_AssassinMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID Skill_AssassinMaster(_In_ CONST CMonster& Boss, _In_ CONST CDailyTask_Skill& DailyTaskSkill);

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
	// 
	VOID NormalMove(_In_ CONST CMonster& Boss);

	//
	VOID MeleeMove(_In_ CONST CMonster& Boss);

	//
	VOID FarRangeMove(_In_ CONST CMonster& Boss);

private:
	CDailyTask_SnowHole operator=(CONST CDailyTask_SnowHole&) = delete;
private:
	CONST FixPointInfo	_BossRange;
	DWORD				_dwLastSkillId;
};


#endif // !__GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_雪人洞_DAILYTASK_SNOWHOLE_H__
