#ifndef __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_ѩ�˶�_DAILYTASK_SNOWHOLE_H__
#define __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_ѩ�˶�_DAILYTASK_SNOWHOLE_H__

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
		em_BladeMaster_����BUFF = 0x9A2E08,
		em_Skill_����ѩ��_�ӻ� = 0x5A0FAA,
		em_Skill_����ѩ��_�ڿ� = 0x5A0FAC,
		em_Skill_����ѩ��_���� = 0x5A0FAB,
		em_Skill_����ѩ��_��ˤ = 0x5A0FAD,
		em_Skill_����ѩ��_ǰ�� = 0x5A0FB0,
		em_Skill_����ѩ��_��ȡ���� = 0x5A0FB3,
		em_Skill_����ѩ��_����Ծ = 0x5A0FB1,
		em_Skill_����ѩ��_�������� = 0x5A0FB9,
		em_Skill_����ѩ��_���� = 0x5A0FB2,
		em_Skill_����ѩ��_����ע�� = 0x5A0FBA,
		em_Skill_����ѩ��_���� = 0x5A0FB5,
		em_Skill_����ѩ��_�����緢 = 0x5A0FB6,
		em_Skill_����ѩ��_����ѩ���� = 0x5A0FB7,
		em_Skill_����ѩ��_����ѩ���� = 0x5A0FB8,
	};
public:
	CDailyTask_SnowHole();
	~CDailyTask_SnowHole() = default;

	virtual BOOL Run() override;

	// ����ѩ��
	BOOL KillBoss();
private:
	//
	BOOL Initialize();

	// ��㵽Ģ����ͼ��
	BOOL Step1(_In_ CONST std::vector<Point>& VecPoint);

	// ���������
	BOOL Step2(_In_ CONST std::vector<Point>& VecPoint);

	// �ڶ���Ģ����
	BOOL Step3(_In_ CONST std::vector<Point>& VecPoint);

	// ѩ�˲���ӳ�֮ǰ��һ��С��
	BOOL Step4(_In_ CONST std::vector<Point>& VecPoint);

	// ѩ�˲���ӳ�
	BOOL Step5(_In_ CONST std::vector<Point>& VecPoint);

	// �ܵ�̨����(������Ģ����ǰ)
	BOOL Step6(_In_ CONST std::vector<Point>& VecPoint);

	// ������Ģ����
	BOOL Step7(_In_ CONST std::vector<Point>& VecPoint);

	// ȥ����BOSS
	BOOL Step8(_In_ CONST std::vector<Point>& VecPoint);

	// ����ѩ��
	BOOL Step9(_In_ CONST std::vector<Point>& VecPoint);

	// ��³��
	BOOL Step10(_In_ CONST std::vector<Point>& VecPoint);

	// ս�������� ������
	BOOL End(_In_ CONST std::vector<Point>&);

	// �߹���
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
	// ---------��ʿ------------------------------------------------------------------
	VOID BossSkill_�ӻ�_BladeMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_�ڿ�_BladeMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);
	
	VOID BossSkill_����_BladeMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_��ˤ_BladeMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_ǰ��_BladeMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_��ȡ����_BladeMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_����_BladeMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_����Ծ_BladeMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_��������_BladeMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID Skill_BladeMaster(_In_ CONST CMonster& Boss, _In_ CONST CDailyTask_Skill& DailyTaskSkill);

	// -------����-----------------------------------------------------------------------
	VOID BossSkill_ǰ��_ForceMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_��ȡ����_ForceMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_����_ForceMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_����Ծ_ForceMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_��������_ForceMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID Skill_ForceMaster(_In_ CONST CMonster& Boss, _In_ CONST CDailyTask_Skill& DailyTaskSkill);

	// ---------ȭʦ------------------------------------------------------------------
	VOID BossSkill_�ӻ�_KongFuMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_�ڿ�_KongFuMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_����_KongFuMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_��ˤ_KongFuMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_ǰ��_KongFuMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_��ȡ����_KongFuMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_����_KongFuMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_����Ծ_KongFuMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_��������_KongFuMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID Skill_KongFuMaster(_In_ CONST CMonster& Boss, _In_ CONST CDailyTask_Skill& DailyTaskSkill);

	// ---------�̿�------------------------------------------------------------------
	VOID BossSkill_�ӻ�_AssassinMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_�ڿ�_AssassinMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_����_AssassinMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_��ˤ_AssassinMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_ǰ��_AssassinMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_��ȡ����_AssassinMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_����_AssassinMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_����Ծ_AssassinMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_��������_AssassinMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID Skill_AssassinMaster(_In_ CONST CMonster& Boss, _In_ CONST CDailyTask_Skill& DailyTaskSkill);

private:
	/// ----Action--------------------------------------
	virtual BOOL AccpetDailyTask() CONST override;

	virtual VOID StudyDailySkill() CONST override;

	// �ߵ������ſ�
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


#endif // !__GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_ѩ�˶�_DAILYTASK_SNOWHOLE_H__
