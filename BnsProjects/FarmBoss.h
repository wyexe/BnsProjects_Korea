#ifndef __GITBNSPROJECTS_BNSPROJECTS_BOSS_FARMBOSS_H__
#define __GITBNSPROJECTS_BNSPROJECTS_BOSS_FARMBOSS_H__

#include "FarmBossFish.h"
#include "Cat.h"
#include "CatExpend.h"
#include <MyTools/CLLock.h>

class CMonster;
class CGameUi;
class CFarmBoss : public MyTools::CClassInstance<CFarmBoss>, public CFarmBossFish
{
public:
	struct MonsterSkillContent
	{
		DWORD dwSkillId;
		DWORD dwResAddr;
	};
public:
	CFarmBoss();
	~CFarmBoss() = default;

	BOOL	KillBoss_By_Name(_In_ CONST std::wstring& wsBossName, _In_ CONST FixPointInfo& FixPtInfo);


	BOOL	GetBossMonster(_In_ CONST std::wstring& wsBossName, _Out_opt_ CMonster& Monster) CONST;

public:
	//////特殊BOSS:탁기에 잠식된 왕수////////////////////////////////////////////////////////////////////
	BOOL	KillBoss_2B7_B(_In_ CONST CMonster& Monster);

	BOOL	ExistSpecialMonsterName(_In_ CONST CMonster& Monster);

	VOID	SetBossEnviroment(_In_ CONST CMonster& Boss);

public:

	VOID	SetHookMonsterBase(_In_ DWORD dwHookMonsterBase_);


	DWORD	GetHookMonsterBase();


	BOOL	GetBossSkillByQue(_Out_opt_ MonsterSkillContent& MonsterSkillContent_);


	VOID	AddBossSkillByQue(_In_ CONST MonsterSkillContent& MonsterSkillContent_);


	DWORD& GetMonsterSkillId() CONST;


	CONST std::map<DWORD, DWORD>& GetBossSkillVec() CONST;

	CONST std::set<DWORD>& GetDisableBossSkillSet() CONST;

	BOOL&	IsHookByDlg() CONST;

	VOID	ForceBeckonCat(_In_ CONST CCat& Cat) CONST;
private:

	// 通用打BOSS函数
	BOOL	KillBoss_Lux(_In_ CMonster& Monster, _In_ ULONGLONG dwMaxout_LoseHp);
	BOOL	KillBoss_Kffm(_In_ CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo);
	BOOL	KillBoss_Sum(_In_ CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo);
	BOOL	KillBoss_Warlock(_In_ CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo);
	BOOL	UseSkill_By_BossSkillId_Lux(_In_ CMonster& Monster, _In_ DWORD dwBossSkillId);
	BOOL	UseSkill_By_BossSkillId_Kffm(_In_ CMonster& Monster, _In_ DWORD dwBossSkillId, _In_ CONST FixPointInfo& FixPtInfo);
	BOOL	UseSkill_By_BossSkillId_Sum(_In_ CMonster& Monster, _In_ DWORD dwBossSkillId, _In_ CONST FixPointInfo& FixPtInfo, _In_ CONST CGameUi& Target2_StatusPanel, _In_ CONST CCat CatInfo, _In_ DWORD TmpBossSkillId);
	BOOL	UseSkill_By_BossSkillId_Warlock(_In_ CMonster& Monster, _In_ DWORD dwBossSkillId, _In_ CONST FixPointInfo& FixPtInfo, _In_ CONST CGameUi& Target2_StatusPanel,_In_ DWORD TmpBossSkillId);
private:
	// 读取BOSS技能ID的过滤条件
	BOOL	InitBossSkill();
	BOOL	KillBoss_Singe_Skill(_In_ CMonster& Monster, _In_ DWORD dwTimeOut) CONST;

	BOOL	ClearAroundMonster(_In_ CONST CMonster& Boss, _In_ CONST std::wstring& wsMonsterName, _In_ CONST FixPointInfo& FixPtInfo) CONST;
	BOOL	ClearAroundMonster_ts(_In_ CONST CMonster& Boss, _In_ CONST std::wstring& wsMonsterName, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	VOID	RefushGameUi(_In_ CONST std::wstring& wsUiName, _Out_opt_ CGameUi& GameUi) throw();

	BOOL	IsEnableControlBoss(_In_ CONST CGameUi& Target2_StatusPanel) CONST throw();

	DWORD	GetColtrolBossCount(_In_ CONST CGameUi& Target2_StatusPanel) CONST throw();

	BOOL    MoveToBoss(_In_ CONST CMonster& Monster) CONST;

	BOOL	MoveToPoint(_In_ CONST Point& TarPoint, _In_ float fDis = 3.0f) CONST;

	VOID	MoveSixHorn(_In_ CONST CMonster& Boss, _In_ float fDis) CONST;

	DSIABLE_COPY_AND_ASSIGN(CFarmBoss)
private:
	DWORD dwHookMonsterBase;
	MyTools::CLLock vBossSkillLock;
	float fKillDis;
	std::queue<MonsterSkillContent> queBossSkill;
	DWORD Xueren_flag;
	DWORD Xueren_2263;
	DWORD Xueren_2254;
	DWORD Xueren_2265;
	DWORD Xueyugong_One;
	DWORD Guimenguan_jiaxue;
	DWORD Yiluka_Sum;
	DWORD Yiluka_Control;
	DWORD Xiuluowang_Control;
	DWORD Xiuluowang_Control_2;
	DWORD Xiuluowang_Control_3;
	DWORD Heilongjiao_Control_1;
	DWORD Heilongjiao_Control_2_1;
	BOOL Heilongjiao_Huafen;
	DWORD Heilongjiao_Control_2_2;
	DWORD Heilongjiao_Buff;
	BOOL Guimenguan_boss;
	BOOL Step_Flag;
	BOOL Skill_Flag;
	DWORD Strike_Zhongzi_Count;
	DWORD Guimenguan_boss_Count;
	ULONGLONG Sum_Q;
	UINT  uLoopCount;

	// 限制在打BOSS的范围
	FixPointInfo _BossRange;

	// 最小的攻击距离(小于这个工具距离就必须要走到最大的保持攻击范围)
	float _MinKeepAttackDis;

	// 最大的保持攻击范围
	float _KeepAttackDis;

};



#endif