#ifndef __GITBNSPROJECTS_BNSPROJECTS_GAME_SOMEFUN_FARM_BOSS_FARMBOSSSKILL_H__
#define __GITBNSPROJECTS_BNSPROJECTS_GAME_SOMEFUN_FARM_BOSS_FARMBOSSSKILL_H__

#include "GameVariable.h"
#include <MyTools/CLLock.h>

class CMonster;
class CFarmBossSkill : public MyTools::CClassInstance<CFarmBossSkill>
{
public:
	CFarmBossSkill();
	~CFarmBossSkill();

public:
	// 设置新的BOSS
	VOID SetBossObject(_In_ CONST CMonster& Boss, _In_ CONST FixPointInfo& Range_);

	// let's go!
	VOID Run();

	// 释放BOSS对象
	VOID ReleaseBossObject();

	// 设置禁止使用的技能ID
	VOID SetDisableSkillId(_In_ DWORD dwSkillId);

	// 设置恢复使用的技能ID
	VOID SetEnableSkillId(_In_ em_Skill_Type emSkillType, _In_ DWORD dwSkillId);

	// 设置普通攻击施法
	VOID SetNormalAttack(_In_ std::function<VOID(VOID)> fnNormalAttackPtr);
	VOID SetClassesNormalAttack();

	// 设置保留施法
	VOID SetRetain(_In_ std::function<VOID(VOID)> fnRetainPtr);

	// 走路到目的坐标
	virtual VOID Move(_In_ CONST Point& TarPoint, _In_ float fDis = 3.0f) CONST;

	virtual VOID MoveToPoint(_In_ CONST Point& TarPoint, _In_ DWORD dwMaxTimeOut = 7 * 1000,  _In_ float fDis = 3.0f) CONST;

	// 走到BOSS
	enum em_MoveType
	{
		em_MoveType_Front,
		em_MoveType_Left,
		em_MoveType_Right,
		em_MoveType_Behind
	};
	virtual VOID MoveToBoss(_In_ em_MoveType emMoveType, _In_ DWORD dwTimeOut = 7 * 1000, _In_ float fDis = 3.0f) CONST;

	// 分开6个角去移动
	virtual VOID MoveToSixAngle(_In_ float fDis, _In_ DWORD dwTimeOut = 7 * 1000) CONST;

	// 设置控制权
	VOID SetPower(_In_ BOOL bPower);
private:
	static DWORD WINAPI _FarmBossSkillThread(LPVOID lpParm);

private:
	CONST CMonster* pBoss;

	// 是否运行中……
	BOOL _Run;

	// 线程句柄
	HANDLE hThread;

	// 控制权
	BOOL _Power;

	// 施法的技能
	std::vector<CGameVariable::ClassesBossSkill> _VecDefaultSkill;

	// 普通攻击施法
	std::function<VOID(VOID)> _fnNormalAttackPtr;

	// 保留施法
	std::function<VOID(VOID)> _fnRetainPtr;

	// 走路范围
	FixPointInfo _Range;

	MyTools::CLLock LockVecDefaultSkill;
};



#endif // !__GITBNSPROJECTS_BNSPROJECTS_GAME_SOMEFUN_FARM_BOSS_FARMBOSSSKILL_H__
