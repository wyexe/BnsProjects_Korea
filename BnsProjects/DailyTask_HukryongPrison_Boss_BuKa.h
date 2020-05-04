#ifndef __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_�����̵��¼���_DAILYTASK_HUKRYONGPRISON_BOSS_BUKA_H__
#define __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_�����̵��¼���_DAILYTASK_HUKRYONGPRISON_BOSS_BUKA_H__

#include "DailyTask_Boss_Template.h"
#include "DailyTask_BossSkill.h"

class CMonster;
class CDailyTask_Move;
class CDailyTask_HukryongPrison_Boss_BuKa : public CDailyTask_Boss_Template
{
private:
	enum
	{
		em_BladeMaster_����BUFF = 0x9A2E08,
		em_BladeMaster_����BUFF1 = 0x9A2E0A,
		em_BladeMaster_����BUFF2 = 0x9A2E0B, 
		em_BladeMaster_����BUFF3 = 0x9A2E0C,
		em_BladeMaster_����BUFF4 = 0x9A2E0D,
		em_BladeMaster_����BUFF5 = 0x9A2E0E,
		em_BladeMaster_����BUFF6 = 0x9A2E0F,

		em_Skill_����_�ӻ�		= 0x5A130C,	// 5903116-
		em_Skill_����_��������	= 0x5A1312,	// 5903122-
		em_Skill_����_��Ϯ����	= 0x5A1313, // 5903123
		em_Skill_����_��������   = 0x5A1315, // 5903125
		em_Skill_����_�ʹ�       = 0x5A130D, // 5903117-
		em_Skill_����_��������   = 0x5A130F, // 5903119
		em_Skill_����_�����ͻ� = 0x5A139A,//5903258
		em_Skill_����_������ = 0x5A139B,//5903259
		em_Skill_����_��֮��     = 0x5A1314, // 5903124-
		em_Skill_����_�����ź�   = 0x5A1310,	// 5903120
		em_Skill_����_��ת����   = 0x5A130E, // 5903118
	};

	enum { em_Boss_BodySize = 0x2 };
public:
	CDailyTask_HukryongPrison_Boss_BuKa(_In_ CONST CMonster& Boss, _In_ CONST CDailyTask_Move& DailyTask_Move);
	~CDailyTask_HukryongPrison_Boss_BuKa() = default;

	virtual BOOL Kill() override;
private:
	// ---------��ʿ------------------------------------------------------------------
	VOID KillBoss_BladeMaster();

	VOID Skill_BladeMaster();

	VOID BossSkill_�ӻ�_BladeMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_��������_BladeMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_��Ϯ����_BladeMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_��������_BladeMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_�ʹ�_BladeMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_��������_BladeMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_�����ͻ�_BladeMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_��֮��_BladeMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_�����ź�_BladeMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_��ת����_BladeMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	// -------����-----------------------------------------------------------------------
	VOID KillBoss_ForceMaster();

	VOID Skill_ForceMaster();

	VOID BossSkill_�ӻ�_ForceMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_�����ͻ�_ForceMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_��ת����_ForceMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	// -------����ʦ-----------------------------------------------------------------------
	VOID KillBoss_Warlock();

	VOID Skill_Warlock();

	VOID BossSkill_�ӻ�_Warlock(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_�����ͻ�_Warlock(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_��ת����_Warlock(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);
	// -------�̿�---------------------------------------------------------------------------------
	VOID KillBoss_Assassin();

	VOID Skill_Assassin();

	VOID BossSkill_�ӻ�_Assassin(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_��������_Assassin(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_��Ϯ����_Assassin(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_��������_Assassin(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_�ʹ�_Assassin(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_��������_Assassin(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_�����ͻ�_Assassin(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_��֮��_Assassin(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_�����ź�_Assassin(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_��ת����_Assassin(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	// -------ȭʦ---------------------------------------------------------------------------------
	VOID KillBoss_KongFuMaster();

	VOID Skill_KongFuMaster();

	VOID BossSkill_�ӻ�_KongFuMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_��������_KongFuMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_��Ϯ����_KongFuMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_��������_KongFuMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_�ʹ�_KongFuMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_��������_KongFuMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_��֮��_KongFuMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_�����ź�_KongFuMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_��ת����_KongFuMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);
private:
	// ---Common---------------------------------------------------------
	// 
	VOID NormalMove();

	//
	VOID MeleeMove();

	//�̿�
	VOID MeleeMove1();

	//
	VOID FarRangeMove();

	//
	std::vector<Point> GetTurbidGround() CONST;

	//
	BOOL ContainTrubidGroud(_In_ CONST std::vector<Point>& VecPoint, _In_ CONST Point& ComprPoint, _In_ float fMaxDis, _Out_opt_ Point* pTurbidGroundPoint = nullptr) CONST;

	//
	BOOL FindSafePoint(_In_ CONST std::vector<Point>& VecTurbiGround, _In_ CONST Point& TurbiGround, _Out_ Point& RetPoint) CONST;

	//
	VOID KillFish();

	//
	BOOL FindSafePath(_In_ CONST std::vector<Point>& VecTurbiGround, _In_ CONST Point& TarPoint,  _Out_ std::vector<Point>& VecRetPoint) CONST;

	//
	VOID MoveToSafePoint(_In_ CONST std::vector<Point>& VecSafePath) CONST;
};

#endif // !__GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_�����̵��¼���_DAILYTASK_HUKRYONGPRISON_BOSS_BUKA_H__
