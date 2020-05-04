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
	// �����µ�BOSS
	VOID SetBossObject(_In_ CONST CMonster& Boss, _In_ CONST FixPointInfo& Range_);

	// let's go!
	VOID Run();

	// �ͷ�BOSS����
	VOID ReleaseBossObject();

	// ���ý�ֹʹ�õļ���ID
	VOID SetDisableSkillId(_In_ DWORD dwSkillId);

	// ���ûָ�ʹ�õļ���ID
	VOID SetEnableSkillId(_In_ em_Skill_Type emSkillType, _In_ DWORD dwSkillId);

	// ������ͨ����ʩ��
	VOID SetNormalAttack(_In_ std::function<VOID(VOID)> fnNormalAttackPtr);
	VOID SetClassesNormalAttack();

	// ���ñ���ʩ��
	VOID SetRetain(_In_ std::function<VOID(VOID)> fnRetainPtr);

	// ��·��Ŀ������
	virtual VOID Move(_In_ CONST Point& TarPoint, _In_ float fDis = 3.0f) CONST;

	virtual VOID MoveToPoint(_In_ CONST Point& TarPoint, _In_ DWORD dwMaxTimeOut = 7 * 1000,  _In_ float fDis = 3.0f) CONST;

	// �ߵ�BOSS
	enum em_MoveType
	{
		em_MoveType_Front,
		em_MoveType_Left,
		em_MoveType_Right,
		em_MoveType_Behind
	};
	virtual VOID MoveToBoss(_In_ em_MoveType emMoveType, _In_ DWORD dwTimeOut = 7 * 1000, _In_ float fDis = 3.0f) CONST;

	// �ֿ�6����ȥ�ƶ�
	virtual VOID MoveToSixAngle(_In_ float fDis, _In_ DWORD dwTimeOut = 7 * 1000) CONST;

	// ���ÿ���Ȩ
	VOID SetPower(_In_ BOOL bPower);
private:
	static DWORD WINAPI _FarmBossSkillThread(LPVOID lpParm);

private:
	CONST CMonster* pBoss;

	// �Ƿ������С���
	BOOL _Run;

	// �߳̾��
	HANDLE hThread;

	// ����Ȩ
	BOOL _Power;

	// ʩ���ļ���
	std::vector<CGameVariable::ClassesBossSkill> _VecDefaultSkill;

	// ��ͨ����ʩ��
	std::function<VOID(VOID)> _fnNormalAttackPtr;

	// ����ʩ��
	std::function<VOID(VOID)> _fnRetainPtr;

	// ��·��Χ
	FixPointInfo _Range;

	MyTools::CLLock LockVecDefaultSkill;
};



#endif // !__GITBNSPROJECTS_BNSPROJECTS_GAME_SOMEFUN_FARM_BOSS_FARMBOSSSKILL_H__
