#ifndef __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_ºÚÁú½ÌµØÏÂ¼àÓü_DAILYTASK_HUKRYONGPRISON_BOSS_BUKA_H__
#define __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_ºÚÁú½ÌµØÏÂ¼àÓü_DAILYTASK_HUKRYONGPRISON_BOSS_BUKA_H__

#include "DailyTask_Boss_Template.h"
#include "DailyTask_BossSkill.h"

class CMonster;
class CDailyTask_Move;
class CDailyTask_HukryongPrison_Boss_BuKa : public CDailyTask_Boss_Template
{
private:
	enum
	{
		em_BladeMaster_»ðÁúBUFF = 0x9A2E08,
		em_BladeMaster_»ðÁúBUFF1 = 0x9A2E0A,
		em_BladeMaster_»ðÁúBUFF2 = 0x9A2E0B, 
		em_BladeMaster_»ðÁúBUFF3 = 0x9A2E0C,
		em_BladeMaster_»ðÁúBUFF4 = 0x9A2E0D,
		em_BladeMaster_»ðÁúBUFF5 = 0x9A2E0E,
		em_BladeMaster_»ðÁúBUFF6 = 0x9A2E0F,

		em_Skill_²¼¿¨_»Ó»÷		= 0x5A130C,	// 5903116-
		em_Skill_²¼¿¨_ß÷»ðÁ÷ÐÇ	= 0x5A1312,	// 5903122-
		em_Skill_²¼¿¨_ÆæÏ®¹¥»÷	= 0x5A1313, // 5903123
		em_Skill_²¼¿¨_Á¬Ðø¹¥»÷   = 0x5A1315, // 5903125
		em_Skill_²¼¿¨_ÃÍ´·       = 0x5A130D, // 5903117-
		em_Skill_²¼¿¨_¹ÇÃæÐý»÷   = 0x5A130F, // 5903119
		em_Skill_²¼¿¨_¹ÇÃæÃÍ»÷ = 0x5A139A,//5903258
		em_Skill_²¼¿¨_¹ÇÃæ³å·æ = 0x5A139B,//5903259
		em_Skill_²¼¿¨_¸«Ö®·ç     = 0x5A1314, // 5903124-
		em_Skill_²¼¿¨_¹ÇÃæÄÅº°   = 0x5A1310,	// 5903120
		em_Skill_²¼¿¨_Ðý×ª¹¥»÷   = 0x5A130E, // 5903118
	};

	enum { em_Boss_BodySize = 0x2 };
public:
	CDailyTask_HukryongPrison_Boss_BuKa(_In_ CONST CMonster& Boss, _In_ CONST CDailyTask_Move& DailyTask_Move);
	~CDailyTask_HukryongPrison_Boss_BuKa() = default;

	virtual BOOL Kill() override;
private:
	// ---------½£Ê¿------------------------------------------------------------------
	VOID KillBoss_BladeMaster();

	VOID Skill_BladeMaster();

	VOID BossSkill_»Ó»÷_BladeMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_ß÷»ðÁ÷ÐÇ_BladeMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_ÆæÏ®¹¥»÷_BladeMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_Á¬Ðø¹¥»÷_BladeMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_ÃÍ´·_BladeMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_¹ÇÃæÐý»÷_BladeMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_¹ÇÃæÃÍ»÷_BladeMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_¸«Ö®·ç_BladeMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_¹ÇÃæÄÅº°_BladeMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_Ðý×ª¹¥»÷_BladeMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	// -------Æø¹¦-----------------------------------------------------------------------
	VOID KillBoss_ForceMaster();

	VOID Skill_ForceMaster();

	VOID BossSkill_»Ó»÷_ForceMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_¹ÇÃæÃÍ»÷_ForceMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_Ðý×ª¹¥»÷_ForceMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	// -------ÖäÊõÊ¦-----------------------------------------------------------------------
	VOID KillBoss_Warlock();

	VOID Skill_Warlock();

	VOID BossSkill_»Ó»÷_Warlock(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_¹ÇÃæÃÍ»÷_Warlock(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_Ðý×ª¹¥»÷_Warlock(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);
	// -------´Ì¿Í---------------------------------------------------------------------------------
	VOID KillBoss_Assassin();

	VOID Skill_Assassin();

	VOID BossSkill_»Ó»÷_Assassin(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_ß÷»ðÁ÷ÐÇ_Assassin(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_ÆæÏ®¹¥»÷_Assassin(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_Á¬Ðø¹¥»÷_Assassin(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_ÃÍ´·_Assassin(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_¹ÇÃæÐý»÷_Assassin(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_¹ÇÃæÃÍ»÷_Assassin(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_¸«Ö®·ç_Assassin(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_¹ÇÃæÄÅº°_Assassin(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_Ðý×ª¹¥»÷_Assassin(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	// -------È­Ê¦---------------------------------------------------------------------------------
	VOID KillBoss_KongFuMaster();

	VOID Skill_KongFuMaster();

	VOID BossSkill_»Ó»÷_KongFuMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_ß÷»ðÁ÷ÐÇ_KongFuMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_ÆæÏ®¹¥»÷_KongFuMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_Á¬Ðø¹¥»÷_KongFuMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_ÃÍ´·_KongFuMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_¹ÇÃæÐý»÷_KongFuMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_¸«Ö®·ç_KongFuMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_¹ÇÃæÄÅº°_KongFuMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);

	VOID BossSkill_Ðý×ª¹¥»÷_KongFuMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_);
private:
	// ---Common---------------------------------------------------------
	// 
	VOID NormalMove();

	//
	VOID MeleeMove();

	//´Ì¿Í
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

#endif // !__GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_ºÚÁú½ÌµØÏÂ¼àÓü_DAILYTASK_HUKRYONGPRISON_BOSS_BUKA_H__
