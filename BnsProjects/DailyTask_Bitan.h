#ifndef __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_雪玉宫_DAILYTASK_BITAN_H__
#define __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_雪玉宫_DAILYTASK_BITAN_H__

#include "DailyTask_Common.h"
#include "DailyTask_BossSkill.h"

class CMonster;
class CDailyTask_Skill;
class CDailyTask_Bitan : public CDailyTask_Common
{
private:
	enum em_Dungeon_Bitan_Step
	{
		em_Dungeon_Bitan_Step_None,
		em_Dungeon_Bitan_Step1,
		em_Dungeon_Bitan_Step2_Left,
		em_Dungeon_Bitan_Step2_Right,
		em_Dungeon_Bitan_Step3_Left,
		em_Dungeon_Bitan_Step3_Right,
		em_Dungeon_Bitan_Step4_Left,
		em_Dungeon_Bitan_Step4_Right,
		em_Dungeon_Bitan_Step5_Left,
		em_Dungeon_Bitan_Step5_Right,
		em_Dungeon_Bitan_Step6_Left,
		em_Dungeon_Bitan_Step6_Right,
		em_Dungeon_Bitan_Step7_Left,
		em_Dungeon_Bitan_Step7_Right,
		em_Dungeon_Bitan_Step8_Left,
		em_Dungeon_Bitan_Step8_Right,
		em_Dungeon_Bitan_Step9_Left,
		em_Dungeon_Bitan_Step9_Right,
		em_Dungeon_Bitan_Step10_Left,
		em_Dungeon_Bitan_Step10_Right,
		em_Dungeon_Bitan_Step11,
		em_Dungeon_Bitan_Step12,
		em_Dungeon_Bitan_Step13,
		em_Dungeon_Bitan_Step14,
		em_Dungeon_Bitan_Step15,
		em_Dungeon_Bitan_Step16,
		em_Dungeon_Bitan_Step17,
		em_Dungeon_Bitan_Step18,
		em_Dungeon_Bitan_Step19,
		em_Dungeon_Bitan_Step20,
		em_Dungeon_Bitan_Step21,
		em_Dungeon_Bitan_Step22,
		em_Dungeon_Bitan_Step23,
		em_Dungeon_Bitan_Step24,
		em_Dungeon_Bitan_Step25,
		em_Dungeon_Bitan_Step26,
		em_Dungeon_Bitan_Step27,
		em_Dungeon_Bitan_Step28,
		em_Dungeon_Bitan_Step29,
		em_Dungeon_Bitan_Step30,
		em_Dungeon_Bitan_Step_End,
	};

	struct BiTanPoint
	{
		Point Pt;
		em_Dungeon_Bitan_Step emStep;
	};

	struct Dungeon_BiTan_Content
	{
		em_Dungeon_Bitan_Step	emStep;
		StepMethodPtr			StepPtr;
	};
public:
	CDailyTask_Bitan();
	~CDailyTask_Bitan();

	virtual BOOL Run() override;
private:
	//
	BOOL Initialize();

	//
	CONST std::vector<BiTanPoint>& GetVecBiTanPoint() CONST _NOEXCEPT;

	// 门口一波小怪
	BOOL Step1(_In_ CONST std::vector<Point>& VecPoint);

	// 到第一个门口
	BOOL Step2_Left(_In_ CONST std::vector<Point>& VecPoint);

	//
	BOOL Step2_Right(_In_ CONST std::vector<Point>& VecPoint);

	// 第一个门口的机关
	BOOL Step3_Left(_In_ CONST std::vector<Point>& VecPoint);

	//
	BOOL Step3_Right(_In_ CONST std::vector<Point>& VecPoint);

	//
	BOOL Step4_Left(_In_ CONST std::vector<Point>& VecPoint);

	//
	BOOL Step4_Right(_In_ CONST std::vector<Point>& VecPoint);

	//
	BOOL Step5_Left(_In_ CONST std::vector<Point>& VecPoint);

	//
	BOOL Step5_Right(_In_ CONST std::vector<Point>& VecPoint);

	//
	BOOL Step6_Left(_In_ CONST std::vector<Point>& VecPoint);

	//
	BOOL Step6_Right(_In_ CONST std::vector<Point>& VecPoint);

	//
	BOOL Step7_Left(_In_ CONST std::vector<Point>& VecPoint);

	//
	BOOL Step7_Right(_In_ CONST std::vector<Point>& VecPoint);

	//
	BOOL Step8_Left(_In_ CONST std::vector<Point>& VecPoint);

	//
	BOOL Step8_Right(_In_ CONST std::vector<Point>& VecPoint);

	//
	BOOL Step9_Left(_In_ CONST std::vector<Point>& VecPoint);

	//
	BOOL Step9_Right(_In_ CONST std::vector<Point>& VecPoint);

	//
	BOOL Step10_Left(_In_ CONST std::vector<Point>& VecPoint);

	//
	BOOL Step10_Right(_In_ CONST std::vector<Point>& VecPoint);

	//
	BOOL Step11(_In_ CONST std::vector<Point>& VecPoint);

	//
	BOOL Step12(_In_ CONST std::vector<Point>& VecPoint);

	//
	BOOL Step13(_In_ CONST std::vector<Point>& VecPoint);

	//
	BOOL Step14(_In_ CONST std::vector<Point>& VecPoint);

	//
	BOOL Step15(_In_ CONST std::vector<Point>& VecPoint);

	//
	BOOL Step16(_In_ CONST std::vector<Point>& VecPoint);

	//
	BOOL Step17(_In_ CONST std::vector<Point>& VecPoint);

	//
	BOOL Step18(_In_ CONST std::vector<Point>& VecPoint);

	//
	BOOL Step19(_In_ CONST std::vector<Point>& VecPoint);

	//
	BOOL Step20(_In_ CONST std::vector<Point>& VecPoint);

	//
	BOOL Step21(_In_ CONST std::vector<Point>& VecPoint);

	//
	BOOL Step22(_In_ CONST std::vector<Point>& VecPoint);

	//
	BOOL Step23(_In_ CONST std::vector<Point>& VecPoint);

	//
	BOOL Step24(_In_ CONST std::vector<Point>& VecPoint);

	//
	BOOL Step25(_In_ CONST std::vector<Point>& VecPoint);

	//
	BOOL Step26(_In_ CONST std::vector<Point>& VecPoint);

	//
	BOOL End(_In_ CONST std::vector<Point>& VecPoint);

private:
	// 
	virtual BOOL MoveToDungeon() override;

	//
	virtual BOOL AccpetDailyTask() CONST override;

	//
	virtual BOOL IsInDungeon() CONST override;

	//
	virtual VOID StudyDailySkill() CONST override;
private:
	// 
	VOID Kill_金童_By_BladeMaster(_In_ CONST CMonster& Boss);

	VOID Kill_银童_By_BladeMaster(_In_ CONST CMonster& Boss);

	VOID Kill_进击的伊鲁卡_BladeMaster(_In_ CONST CMonster& Boss);

	VOID Kill_修罗王_BladeMaster(_In_ CONST CMonster& Boss);
private:
	//
	BOOL TakeShield(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<BOOL(VOID)> fnExit) CONST;

	//
	BOOL KillYLKShadow(_In_ CONST std::vector<Point>& VecPoint) CONST;
private:
	enum class em_Bitan_Team_Direction
	{
		Direction_Left, Direction_Right
	};

	BOOL IsCorrectDirection(_In_ em_Bitan_Team_Direction emDirection) CONST;
};



#endif // !__GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_雪玉宫_DAILYTASK_BITAN_H__
