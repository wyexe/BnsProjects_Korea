#ifndef __GITBNSPROJECTS_BNSPROJECTS_SOMEFUN_FARM_FARMBOSSFISH_H__
#define __GITBNSPROJECTS_BNSPROJECTS_SOMEFUN_FARM_FARMBOSSFISH_H__

#include "GameVariable.h"

class CMonster;
class CFarmBossFish
{
private:
	enum em_Monster_YLK_Color
	{
		em_Monster_YLK_Color_Green,
		em_Monster_YLK_Color_Blue
	};
public:
	CFarmBossFish() = default;
	~CFarmBossFish() = default;

	enum
	{
		em_BUFF_XiuLuoWang_Flower = 0x3849BC0
	};
public:
	BOOL MoveToLightBySnowBoss(_In_ CONST CMonster& Monster) CONST;

	BOOL TakeShield(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<BOOL(VOID)> fnExit) CONST;

	BOOL MoveBeindAndKillMonster(_In_ CONST CMonster& Boss, _In_ CONST std::wstring& wsMonsterName, _In_ CONST FixPointInfo& Range) CONST;

	///雪域宫///////////////////////////////////////////////////////////////////////

	// 打断老二伊鲁卡的影子
	BOOL InterruptYLKShadowSkill(_In_ CONST CMonster& BOSS) CONST;
	
	// 打老二伊鲁卡产生的盾兵
	BOOL KillShieldSolider(_In_ CONST CMonster& BOSS) CONST;

	// 捡起盾牌并且反弹伊鲁卡的技能
	BOOL ReboundYLKSkill(_In_ CONST CMonster& BOSS,_In_ BOOL bDiscardShield) CONST;

	// 修罗王踩花
	BOOL StepOnFlowerForXiuLuoWang() CONST;

	// 修罗王格挡球
	VOID XiuLuoWang_BlockBall() CONST;

	//////黑龙教////////////////////////////////////////////////////////////////////

	// 第一个BOSS布卡的浊气阵
	BOOL AvoidBuKaZhuoQiZhen(_In_ CONST CMonster& BOSS, _In_ CONST FixPointInfo& RangePoint) CONST;

	// 第二个BOSS的球
	BOOL AvoidMoGuKeZhuoQiZhu(_In_ CONST CMonster& BOSS) CONST;

	// 计算距离BOSS最近的队友
	int GetTeamMemberIndex_By_BossDis(_In_ CONST CMonster& Boss) CONST;
private:

	///雪域宫的伊鲁卡///////////////////////////////////////////////////////////////////////

	// 返回所有伊鲁卡的影子
	VOID GetVecYIKShadowByName(_In_ CONST std::wstring& wsMonsterName, _Out_ std::vector<CMonster>& VecMonster) CONST;

	VOID HelpToSkillYLKShadow(_In_ em_Monster_YLK_Color emMonsterYLKColor) CONST;

	VOID KillYLKShadow(_In_ CONST CMonster& TarMonster, _In_ em_Monster_YLK_Color emMonsterYLKColor, _In_ BOOL bFirst) CONST;

	////雪玉宫的修罗王//////////////////////////////////////////////////////////////////////
private:
	CONST Point		_GetBehindByTwoPoint(_In_ CONST Point& p1, _In_ CONST Point& p2, _In_ float fDis) CONST;
private:

	DSIABLE_COPY_AND_ASSIGN(CFarmBossFish);
};



#endif