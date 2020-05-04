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

	///ѩ��///////////////////////////////////////////////////////////////////////

	// ����϶���³����Ӱ��
	BOOL InterruptYLKShadowSkill(_In_ CONST CMonster& BOSS) CONST;
	
	// ���϶���³�������Ķܱ�
	BOOL KillShieldSolider(_In_ CONST CMonster& BOSS) CONST;

	// ������Ʋ��ҷ�����³���ļ���
	BOOL ReboundYLKSkill(_In_ CONST CMonster& BOSS,_In_ BOOL bDiscardShield) CONST;

	// �������Ȼ�
	BOOL StepOnFlowerForXiuLuoWang() CONST;

	// ����������
	VOID XiuLuoWang_BlockBall() CONST;

	//////������////////////////////////////////////////////////////////////////////

	// ��һ��BOSS������������
	BOOL AvoidBuKaZhuoQiZhen(_In_ CONST CMonster& BOSS, _In_ CONST FixPointInfo& RangePoint) CONST;

	// �ڶ���BOSS����
	BOOL AvoidMoGuKeZhuoQiZhu(_In_ CONST CMonster& BOSS) CONST;

	// �������BOSS����Ķ���
	int GetTeamMemberIndex_By_BossDis(_In_ CONST CMonster& Boss) CONST;
private:

	///ѩ�򹬵���³��///////////////////////////////////////////////////////////////////////

	// ����������³����Ӱ��
	VOID GetVecYIKShadowByName(_In_ CONST std::wstring& wsMonsterName, _Out_ std::vector<CMonster>& VecMonster) CONST;

	VOID HelpToSkillYLKShadow(_In_ em_Monster_YLK_Color emMonsterYLKColor) CONST;

	VOID KillYLKShadow(_In_ CONST CMonster& TarMonster, _In_ em_Monster_YLK_Color emMonsterYLKColor, _In_ BOOL bFirst) CONST;

	////ѩ�񹬵�������//////////////////////////////////////////////////////////////////////
private:
	CONST Point		_GetBehindByTwoPoint(_In_ CONST Point& p1, _In_ CONST Point& p2, _In_ float fDis) CONST;
private:

	DSIABLE_COPY_AND_ASSIGN(CFarmBossFish);
};



#endif