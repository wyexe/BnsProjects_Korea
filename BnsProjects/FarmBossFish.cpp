#include "stdafx.h"
#include "FarmBossFish.h"
#include <MyTools/Log.h>
#include <MyTools/TimeTick.h>
#include "Monster.h"
#include "MonsterExtend.h"
#include "Person.h"
#include "PersonFunction.h"
#include "Corpse.h"
#include "ResExtend.h"
#include "SomeFun.h"
#include "TeamMember.h"
#include "TeamMemberExtend.h"
#include "Farm.h"
#include "ObjectFunction.h"
#include "FarmSkill.h"
#include "PlayerExtend.h"
#include "Cat.h"
#include "CatExpend.h"
#include "FarmBossSkill.h"
#define _SELF L"FarmBossFish.cpp"


BOOL CFarmBossFish::MoveToLightBySnowBoss(CONST CMonster& Monster) CONST
{
	/*std::vector<CMonster> MushroomVec;
	CMonsterExtend::GetInstance().GetAroundMonsterListByName(L"얼음 뱀버섯", CBaseObject::CreateFixPointInfo(100), MushroomVec);
	if (MushroomVec.size() == 0)
	{
		LOG_CF_D( L"周围没蘑菇!");
		return FALSE;
	}
	LOG_CF_E( L"周围蘑菇数量:%d", MushroomVec.size());

	// 降序
	std::sort(MushroomVec.begin(), MushroomVec.end(), [](CONST CMonster& m1, CONST CMonster& m2){
		return m1.GetId() < m2.GetId();
	});

	UINT uTeamIndex = CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex();
	if (MushroomVec.size() > uTeamIndex)
	{
		// 杀蘑菇
		CMonster Mushroom = MushroomVec.at(uTeamIndex);
		LOG_CF_E( L"干蘑菇去!");
		Point MushroomPoint = Mushroom.GetPoint();
		CPersonFunction::GetInstance().MoveToPoint(Mushroom.GetBehindPoint(1.0f), 3, 100);
		if (CPersonFunction::GetInstance().ExistBuff_Person(0x3849D00))
		{
			LOG_CF_E( L"已经踩冰了!那就放弃去弄蘑菇了!");
			return FALSE;
		}

		CGameVariable::GetInstance().Action_Set_DisableSkill([](std::set<DWORD>& SetDisableSkill)
		{
			if (SetDisableSkill.find(Sum_SkillId_SS) == SetDisableSkill.end())
				SetDisableSkill.insert(Sum_SkillId_SS);
		});
		CFarm::GetInstance().KillMonster_By_Single(Mushroom, CBaseObject::CreateFixPointInfo(30), TRUE, nullptr);
		BnsSleep(1000);
		CGameVariable::GetInstance().Action_Set_DisableSkill([](std::set<DWORD>& SetDisableSkill)
		{
			auto& itr = SetDisableSkill.find(Sum_SkillId_SS);
			if (itr != SetDisableSkill.end())
				SetDisableSkill.erase(itr);
		});

		CCorpse Corpse;
		if (CResExtend::GetInstance().FindBall_By_Condition(CBaseObject::CreateFixPointInfo(30), &Corpse, [](CONST CCorpse&){ return TRUE; }))
		{
			BnsMove(Corpse.GetPoint(), 1);
			Corpse.Take(NULL);
		}

		BnsMove(MushroomPoint, 1);
		CSomeFun::GetInstance().SimlationKey('F');
		CSomeFun::GetInstance().SimlationKey('F');
	}*/

	/*if (CMonsterExtend::GetInstance().GetAroundMonsterByMaxHp(61900, CBaseObject::CreateFixPointInfo(35), MonsterVec) == 0)
	{
		LOG_CF_E( L"附近的光点没了");
		CPersonFunction::GetInstance().CancelPutDownStatus();
		return FALSE;
	}

	LOG_CF_E( L"准备丢球!");
	MonsterVec.begin()->SetAngle();
	BnsSleep(500);
	CSomeFun::GetInstance().SimlationKey('R');
	CSomeFun::GetInstance().SimlationKey('R');*/

	std::vector<CMonster> MonsterVec;
	while (GameRun && !EMPTY_PERSONHP && !Monster.IsDead() && !CPersonFunction::GetInstance().ExistBuff_Person(0x3849D00) && CMonsterExtend::GetInstance().GetAroundMonsterByMaxHp(61900, CBaseObject::CreateFixPointInfo(50), MonsterVec) > 1)
	{
		LOG_CF_E( L"周围有%d个光点", MonsterVec.size());
		CSomeFun::GetInstance().SetSpeed(3.0f);
		for (UINT i = 0; i < MonsterVec.size() && !Monster.IsDead() && !CPersonFunction::GetInstance().ExistBuff_Person(0x3849D00) && GameRun && !EMPTY_PERSONHP; ++i)
		{
			LOG_CF_D( L"走向雪人BOSS的光点, ID=%X, dis=%.2f", MonsterVec.begin()->GetId(), MonsterVec.begin()->GetDis());
			BnsMove(MonsterVec.at(i).GetPoint(), 1);
		}
	}
	LOG_CF_D( L"等待冰冻状态结束!");
	while (GameRun && !EMPTY_PERSONHP && CPersonFunction::GetInstance().ExistBuff_Person(0x3849D00) && BnsSleep(300));
	LOG_CF_D( L"冰冻状态结束!");
	return TRUE;
}

BOOL CFarmBossFish::TakeShield(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<BOOL(VOID)> fnExit) CONST
{
	std::vector<CCorpse> ShieldVec;
	CResExtend::GetInstance().GetAroundBallList(FixPtInfo, ShieldVec);

	// 降序
	std::sort(ShieldVec.begin(), ShieldVec.end(), [](CONST CCorpse& Corpse1, CONST CCorpse& Corpse2){
		return Corpse1.GetId() < Corpse2.GetId();
	});


	DWORD dwIndex = CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex();
	if (dwIndex == -1)
	{
		LOG_CF_E( L"当前队伍索引竟然是-1!");
		return FALSE;
	}
	else if (dwIndex >= ShieldVec.size())
	{
		LOG_CF_E( L"dwindex=%d, Monster.size()=%d", dwIndex, ShieldVec.size());
		return FALSE;
	}

	for (int i = 0; i < 3 && GameRun && !EMPTY_PERSONHP && !CPerson::GetInstance().IsPickStatus(); ++i)
		ShieldVec.at(dwIndex).Take(500);
	
	BnsMove(FixPtInfo.FixPoint, 3);
	CSomeFun::GetInstance().TimeOut_Condiction_GameRun(30 * 1000, [fnExit]
	{
		CSomeFun::GetInstance().SimlationKey(VK_1);
		BnsSleep(500);
		return !fnExit() && !EMPTY_PERSONHP;
	});

	return CPersonFunction::GetInstance().CancelPutDownStatus();
}

BOOL CFarmBossFish::MoveBeindAndKillMonster(_In_ CONST CMonster& Boss, _In_ CONST std::wstring& wsMonsterName, _In_ CONST FixPointInfo& Range) CONST
{
	CCat newcat;
	CCatExpend::GetInstance().FindSelfCat(&newcat);
	while (GameRun && !EMPTY_PERSONHP)
	{
		std::vector<CMonster> VecMonster;
		if(CMonsterExtend::GetInstance().GetAroundMonsterListByName(wsMonsterName, Range, VecMonster) == 0)
			break;

		CONST CMonster& Monster = VecMonster.at(0);
		Point TarPoint = _GetBehindByTwoPoint(Boss.GetPoint(), Monster.GetPoint(), 4);
		LOG_CF_D(L"跟BOSS连成一条线的坐标距离当前%.2fm", CPerson::GetInstance().GetDis(TarPoint));
		CPersonFunction::GetInstance().MoveToPoint(TarPoint, 1, 50);
		CFarm::GetInstance().KillMonster_By_Single(Monster, Range, TRUE, nullptr);
	}
	return TRUE;
}

BOOL CFarmBossFish::InterruptYLKShadowSkill(_In_ CONST CMonster& BOSS) CONST
{
	// 获取当前角色在队伍的索引
	int nTeamMemberIndex = CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex();
	if (nTeamMemberIndex == -1)
	{
		LOG_CF_E( L"当前人物的队伍Index=-1,表示没队伍!!!");
		return FALSE;
	}
	if (BOSS.GetPercentHp() < 10)
		return FALSE;

	CCat newcat;
	CCatExpend::GetInstance().FindSelfCat(&newcat);
	
	// 获取所有伊鲁卡的影子
	std::vector<CMonster> VecYLK;
	GetVecYIKShadowByName(Monster_扭曲的影子, VecYLK);
	GetVecYIKShadowByName(Monster_倒下的影子, VecYLK);
	sort(VecYLK.begin(), VecYLK.end(), [](CONST CMonster& Monster1, CONST CMonster& Monster2){ return Monster1.GetId() < Monster2.GetId(); });
	if (VecYLK.size() < 3)
	{
		//LOG_CF_E( L"伊鲁卡的影子只有%d个? 不太对啊!", VecYLK.size());
		return FALSE;
	}
	if (BOSS.GetTargetId() == CPerson::GetInstance().GetId() || BOSS.GetTargetId() == newcat.GetId() )
	{
		LOG_CF_D( L"我是仇恨,直接Q猫!");
		CFarmSkill::GetInstance().UseSkill_By_SkillId(Sum_SkillId_Q, em_Skill_Type::em_Skill_Type_Cat);
		return FALSE;
	}


	LOG_CF_E( L"伊鲁卡的影子出现3个了!");
	

	// 分配好当前角色去刷哪个怪!
	CMonster TarMonster;
	em_Monster_YLK_Color emMonsterYLKColor;
	switch (nTeamMemberIndex)
	{
	case 0: case 1:
		TarMonster = VecYLK.at(0);
		break;
	case 2: case 3:
		TarMonster = VecYLK.at(1);
		break;
	case 4: case 5:
		TarMonster = VecYLK.at(2);
		break;
	default:
		LOG_CF_E( L"nTeamMemberIndex=%d", nTeamMemberIndex);
		return FALSE;
	}


	emMonsterYLKColor = TarMonster.GetName() == Monster_扭曲的影子 ? em_Monster_YLK_Color_Blue : em_Monster_YLK_Color_Green;
	LOG_CF_D( L"当前人物角色索引=%d, 要去控制的怪是:%s", nTeamMemberIndex, TarMonster.GetName().c_str());

	// 停止打BOSS
	CFarmBossSkill::GetInstance().SetPower(TRUE);

	// 去杀第一个影子
	KillYLKShadow(TarMonster, emMonsterYLKColor, TRUE);


	// 去帮忙杀其他的影子!
	HelpToSkillYLKShadow(emMonsterYLKColor);
	

	// 等待刷新盾牌兵!
	return KillShieldSolider(BOSS);
}

BOOL CFarmBossFish::KillShieldSolider(_In_ CONST CMonster& BOSS) CONST
{
	CONST static Point CenterPoint(-47008, 26147, 12119);
	CONST FixPointInfo ShieldSoliderRange = FixPointInfo(CenterPoint, 30);
	CCat newcat;
	CCatExpend::GetInstance().FindSelfCat(&newcat);

	auto fnExistShildSoilder = [ShieldSoliderRange](_Out_ CMonster* pMonster)
	{
		return CMonsterExtend::GetInstance().FindMonster_By_Name(Monster_绝缘盾牌兵, ShieldSoliderRange, pMonster);
	};


	CFarmBossSkill::GetInstance().SetPower(TRUE);

	// 跑到刷新点
	BnsMove(CenterPoint, 3);

	CCat Cat;
	if (!CCatExpend::GetInstance().FindSelfCat(&Cat) || Cat.IsDead())
		return FALSE;

	// 把猫召唤回来不然要被电!
	if (Cat.GetDis() > 5)
	{
		LOG_CF_D( L"发现猫的距离超过5m, 召唤回来!");
		CFarm::GetInstance().CallBackCat();
	}


	// 等待盾兵刷新
	while (GameRun && !EMPTY_PERSONHP && !fnExistShildSoilder(nullptr))
	{
		std::vector<CCorpse> VecShield;
		if (CResExtend::GetInstance().GetAroundBallList(CBaseObject::CreateFixPointInfo(30), VecShield) != 0)
		{
			LOG_CF_D( L"地上已经有盾牌了,但是没遍历到盾兵, 直接去捡盾牌!");
			break;
		}
		LOG_CF_D( L"等待刷新盾兵!!!");
		BnsSleep(1000);
	}

	// 把猫召唤回来不然要被电!
	if (Cat.GetDis() > 5)
	{
		LOG_CF_D( L"发现猫的距离超过5m, 召唤回来!");
		CFarm::GetInstance().CallBackCat();
	}

	// 开杀!
	CMonster ShiledSolider;
	while (GameRun && !EMPTY_PERSONHP && fnExistShildSoilder(&ShiledSolider))
	{
		LOG_CF_D( L"正在杀盾兵!!!");
		CFarm::GetInstance().KillMonster_By_Single(ShiledSolider, ShieldSoliderRange, TRUE, nullptr);
	}

	if (Cat.GetDis() > 5)
	{
		LOG_CF_D( L"发现猫的距离超过5m, 召唤回来!");
		CFarm::GetInstance().CallBackCat();
	}

	return ReboundYLKSkill(BOSS,FALSE);
}

BOOL CFarmBossFish::ReboundYLKSkill(_In_ CONST CMonster& BOSS, _In_ BOOL bDiscardShield) CONST
{
	CONST static Point CenterPoint(-47008, 26147, 12119);

	int nTeamMemberIndex = CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex();
	if (nTeamMemberIndex == -1)
	{
		LOG_CF_E( L"当前人物的队伍Index=-1,表示没队伍!!!");
		return FALSE;
	}

	if (CPerson::GetInstance().IsDead())
	{
		CFarmBossSkill::GetInstance().SetPower(FALSE);
		return FALSE;
	}

	if (bDiscardShield)
	{
		// 往怪物身上丢盾牌
		LOG_CF_D( L"向怪物身上砸盾牌!");
		BOSS.SetAngle();
		BnsSleep(500);
		CSomeFun::GetInstance().SimlationKey('R');
		BnsSleep(500);
		CSomeFun::GetInstance().SimlationKey('R');
		BnsSleep(500);
		CPersonFunction::GetInstance().CancelPutDownStatus();
		CFarm::GetInstance().BeckonCat(TRUE);
		CFarmBossSkill::GetInstance().SetPower(FALSE);
		return TRUE;
	}
	
	while (GameRun && !EMPTY_PERSONHP && !CPerson::GetInstance().IsPickStatus())
	{
		// 遍历地上的盾牌!
		std::vector<CCorpse> VecShield;
		if (CResExtend::GetInstance().GetAroundBallList(CBaseObject::CreateFixPointInfo(30), VecShield) == 0)
		{
			LOG_CF_E( L"已经没盾牌可以用了!");
			return FALSE;
		}
		sort(VecShield.begin(), VecShield.end(), [](CONST CCorpse& Corpse1, CONST CCorpse& Corpse2){ return Corpse1.GetId() < Corpse2.GetId(); });
		if (VecShield.size() < 6)
		{
			LOG_CF_E( L"盾牌数量=%d, 不够分!", VecShield.size());
			return FALSE;
		}
		
		VecShield.at(nTeamMemberIndex).Take(0);
		BnsMove(CenterPoint, 2);
		LOG_CF_D( L"捡起属于自己的盾牌了!");
	}

	BOSS.SetAngle();
	LOG_CF_D( L"按1反弹技能!");
	return CSomeFun::GetInstance().SimlationKey(VK_1);
}

BOOL CFarmBossFish::StepOnFlowerForXiuLuoWang() CONST
{
	/*CGameVariable::GetInstance().SetHelpThreadLoopMethodPtr([]
	{
		if (CPersonFunction::GetInstance().ExistBuff_Person(em_BUFF_XiuLuoWang_Flower))
		{
			LOG_C_D(L"人物[%X,%s]走路过程中发现身上有修罗王花的BUFF,使用SS!", CPerson::GetInstance().GetId(), CPerson::GetInstance().GetName().c_str());
			std::vector<GAMEOBJ_BUFF> vlst;
			CObjectFunction::GetInstance().GetPersonBuffList(vlst);
			for (CONST auto& itm : vlst)
				LOG_C_D(L"人物:[%X] 拥有BUFF:[%X]", CPerson::GetInstance().GetId(), itm.dwBuffId);

			if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_SS))
			{
				CFarmSkill::GetInstance().UseSkill_By_SkillId(Sum_SkillId_SS);
			}

		}
	});*/

	std::vector<CMonster> VecList;
	while (GameRun && !EMPTY_PERSONHP && CMonsterExtend::GetInstance().GetAroundMonsterListByName(Monster_生命之花, CBaseObject::CreateFixPointInfo(100), VecList) != 0)
	{
		auto Index = CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex();
		if (Index == 0xFFFFFFFF)
			return TRUE;

		CFarmBossSkill::GetInstance().SetPower(TRUE);
		// 少于6朵花的时候, 队长不去踩!
		if (VecList.size() < 6)
		{
			if (Index == 0)
				break;

			// 其他人的序号往前挪一步
			Index -= 1;
		}

		if (Index >= VecList.size())
			break;

		sort(VecList.begin(), VecList.end(), [](CONST CMonster& m1, CONST CMonster& m2){ return m1.GetId() < m2.GetId(); });
		CMonster Flower = VecList.at(Index);
		LOG_CF_D(L"我是[%s:%X], 去踩花[%X]!", CPerson::GetInstance().GetName().c_str(), CPerson::GetInstance().GetId(), Flower.GetId());
		BnsMove(Flower.GetPoint(), 1);
		VecList.clear();
	}
	CFarmBossSkill::GetInstance().SetPower(FALSE);
	return TRUE;
}

VOID CFarmBossFish::XiuLuoWang_BlockBall() CONST
{
	// 5号和6号队员去挡冰球
	auto Index = CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex();
	if (Index != 4 && Index != 5)
		return;

	// CD没好就别去, 争取时间输出!
	if (!CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_4))
		return;

	std::vector<CMonster> VecBall;
	if (CMonsterExtend::GetInstance().GetAroundMonsterListByName(Monster_冰炎弹, CBaseObject::CreateFixPointInfo(50), VecBall) == 0)
		return;

	CMonster Monster;
	if (VecBall.size() == 1)
	{
		// 1个球的情况下, 6号队员去就行了!
		if (Index != 5)
			return;

		Monster = VecBall.at(0);
	}
	else if (VecBall.size() > 1)
	{
		// 2个球的情况下, 5号和6号 去就行了
		if (Index == 4)
			Monster = VecBall.at(0);
		else if (Index == 5)
			Monster = VecBall.at(1);
		else
			return;
	}

	CFarmBossSkill::GetInstance().SetPower(TRUE);
	LOG_C_D(L"赶紧追上球!");
	CSomeFun::GetInstance().TimeOut_Condiction_GameRun(5 * 1000, [Monster,this]
	{
		if (Monster.GetDis() < 8.0f && CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_4))
		{
			LOG_C_D(L"使用4去挡球!");
			CFarmSkill::GetInstance().UseSkill_By_SkillId(Sum_SkillId_4);
		}
		Monster.SetAngle();
		CPersonFunction::GetInstance().Move(_GetBehindByTwoPoint(CPerson::GetInstance().GetPoint(), Monster.GetPoint(), 5));
		BnsSleep(300);
		return !EMPTY_PERSONHP && !Monster.IsDead();
	});
	CFarmBossSkill::GetInstance().SetPower(FALSE);
}

BOOL CFarmBossFish::AvoidBuKaZhuoQiZhen(_In_ CONST CMonster& BOSS, _In_ CONST FixPointInfo& RangePoint) CONST
{
	std::vector<CMonster> VecMonster1;
	CMonsterExtend::GetInstance().GetAroundMonsterByMaxHp(109560, CBaseObject::CreateFixPointInfo(50), VecMonster1);

	std::vector<CMonster> VecMonster2;
	CMonsterExtend::GetInstance().GetAroundMonsterByMaxHp(166000, CBaseObject::CreateFixPointInfo(50), VecMonster2);
	
	std::vector<CMonster> VecMonster;
	VecMonster.insert(VecMonster.end(), VecMonster1.begin(), VecMonster1.end());
	VecMonster.insert(VecMonster.end(), VecMonster2.begin(), VecMonster2.end());
	if (VecMonster.size() == 0)
		return TRUE;

	auto fnExistZhuoQiZhen = [VecMonster](_In_ CONST Point& TarPoint)
	{
		return MyTools::CLPublic::Vec_find_if_Const(VecMonster, [TarPoint](CONST CMonster& Monster){ return Monster.GetDis(TarPoint) <= 5.0f; }) != nullptr;
	};

	Point TarPoint = CPerson::GetInstance().GetPoint();
	if (!fnExistZhuoQiZhen(TarPoint))
		return TRUE;

	//LOG_CF_D(L"已经踩在浊气阵上了");
	auto fnMoveOutPtr = [&TarPoint, BOSS, RangePoint, fnExistZhuoQiZhen](_In_ std::function<Point()> fnGetTarPoint)
	{
		// 向BOSS的方向退后一步~
		TarPoint = fnGetTarPoint();
		if (CBaseObject::Get3DDis(TarPoint, RangePoint.FixPoint) < RangePoint.fDis && !fnExistZhuoQiZhen(TarPoint))
		{
			LOG_CF_E(L"走出浊气阵范围!");
			BnsMove(TarPoint, 1);
			//CPersonFunction::GetInstance().Move(TarPoint);
			return TRUE;
		}
		//LOG_CF_D(L"BOSS面前距离:%.2f 依然是浊气阵范围", BOSS.GetDis(TarPoint));
		return FALSE;
	};

	std::vector<Point> VecPoint;
	CFarm::GetInstance().GetRoundPointList(FixPointInfo(BOSS.GetPoint(), BOSS.GetDis()), 36, VecPoint);
	for (CONST auto& itm : VecPoint)
	{
		if (fnMoveOutPtr([itm]{ return itm; }))
			return TRUE;
	}

	VecPoint.clear();
	CFarm::GetInstance().GetRoundPointList(FixPointInfo(BOSS.GetPoint(), 8), 36, VecPoint);
	for (CONST auto& itm : VecPoint)
	{
		if (fnMoveOutPtr([itm]{ return itm; }))
			return TRUE;
	}

	VecPoint.clear();
	CFarm::GetInstance().GetRoundPointList(FixPointInfo(BOSS.GetPoint(), 12), 36, VecPoint);
	for (CONST auto& itm : VecPoint)
	{
		if (fnMoveOutPtr([itm]{ return itm; }))
			return TRUE;
	}

	LOG_CF_E(L"BOSS的一条直线坐标都被浊气阵覆盖了, 应该不会有这种情况吧?");
	return TRUE;
}

BOOL CFarmBossFish::AvoidMoGuKeZhuoQiZhu(_In_ CONST CMonster& BOSS) CONST
{
	// 获取所有的浊气球
	std::vector<CMonster> VecMonster;
	if (CMonsterExtend::GetInstance().GetAroundMonsterListByName(Monster_浊气珠, CBaseObject::CreateFixPointInfo(50), VecMonster) == 0)
		return TRUE;

	// 添加每个球的仇恨对象(有可能只有一个)
	/*std::set<DWORD> SetTeamMember;
	for (CONST auto& SingleBall : VecMonster)
	{
		if (SetTeamMember.size() < 4 && CTeamMemberExtend::GetInstance().FindTeamMember_By_Condiction(nullptr, [SingleBall](CONST CTeamMember& Member){ return Member.GetId() == SingleBall.GetTargetId(); }))
		{
			LOG_CF_D(L"球的仇恨ID=%X", SingleBall.GetTargetId());
			SetTeamMember.insert(SingleBall.GetTargetId());
		}
	}

	// 一直添加到球数量的成员为止, std::set默认去掉重复的ID
	for (UINT i = SetTeamMember.size(); i < VecMonster.size(); ++i)
	{
		CTeamMemberExtend::GetInstance().ForEach([&SetTeamMember](CONST CTeamMember& Member)
		{
			LOG_CF_D(L"不止一个球, 添加队友:%X", Member.GetId());
			SetTeamMember.insert(Member.GetId());
		});
	}
	
	// 不在这4个队友里面
	if (SetTeamMember.find(CPerson::GetInstance().GetId()) == SetTeamMember.end())
		return TRUE;

	// 排序, 得到的结果在不同的电脑也是一样的
	std::sort(VecMonster.begin(), VecMonster.end(), [](CONST CMonster& m1, CONST CMonster& m2){ return m1.GetId() > m1.GetId(); });
	
	// 找到自己在容器里面的索引
	UINT Index = 0;
	for (CONST auto& TeamMeberId : SetTeamMember)
	{
		if (TeamMeberId == CPerson::GetInstance().GetId())
			break;
		Index += 1;
	}
	LOG_CF_D(L"一共有%d个球! 当前自己的索引是:%d", VecMonster.size(), Index);

	CMonster Monster;
	switch (Index)
	{
	case 0:
		Monster = std::move(VecMonster.at(0));
		break;
	case 1:
		if (VecMonster.size() > 1)
			Monster = std::move(VecMonster.at(1));
		break;
	case 2:
		if (VecMonster.size() > 2)
			Monster = std::move(VecMonster.at(2));
		break;
	case 3:
		if (VecMonster.size() > 3)
			Monster = std::move(VecMonster.at(3));
		break;
	default:
		return TRUE;
	}

	if (Monster.GetBase() == NULL)
		return TRUE;

	static CONST Point CenterPoint(-34386, -40447, 10195);
	BnsMove(CenterPoint, 1);
	//LOG_CF_D(L"跑到挡球的地点, 球的仇恨是:%X", Monster.GetTargetId());
	BOOL bTimeOut = CSomeFun::GetInstance().TimeOut_Condiction_GameRun(30 * 1000, [Monster]
	{
		Monster.SetAngle();
		return !EMPTY_PERSONHP && !Monster.IsDead() && Monster.GetDis() > 8;
	});
	LOG_CF_D(L"球快来了, 仇恨ID=%X", Monster.GetTargetId());
	if (!bTimeOut)
	{
		Monster.SetAngle();
		BnsSleep(500);
		CFarmSkill::GetInstance().UseSkill_By_SkillId(Sum_SkillId_4);
		BnsSleep(2 * 1000);
	}*/
	
	//static CONST Point CenterPoint(-34386, -40447, 10195);
	
	CMonster Monster = std::move(VecMonster.at(0));
	LOG_CF_D(L"当前人物, 跑去集合点挡球!");
	CSomeFun::GetInstance().SetSpeed(6.0f);
	BnsMove(Monster.GetFacePoint(3.0f), 3.0f);
	Sleep(2500);
	CFarmSkill::GetInstance().UseSkill_By_SkillId(Sum_SkillId_4);
	BnsSleep(2 * 1000);

	//CMonster Monster;
	//BOOL bTimeOut = CSomeFun::GetInstance().TimeOut_Condiction_GameRun(30 * 1000, [VecMonster, &Monster]
	//{
	//	BnsMove(CenterPoint,3.0f);
	//	for (CONST auto& itm : VecMonster)
	//	{
	//		if (!EMPTY_PERSONHP && !itm.IsDead() && itm.GetDis() < 10)
	//		{
	//			Monster = itm;
	//			return FALSE;
	//		}
	//	}
	//	return TRUE;
	//});

	//if (bTimeOut)
	//	return TRUE;

	//CSomeFun::GetInstance().SetSpeed(1.0f);
	//LOG_CF_D(L"集合点挡球!");
	//Monster.SetAngle();
	//BnsSleep(500);
	//CFarmSkill::GetInstance().UseSkill_By_SkillId(Sum_SkillId_4);
	//BnsSleep(2 * 1000);
	//CFarmSkill::GetInstance().WaitForCool_By_SkillId(Sum_SkillId_4);

	//VecMonster.clear();
	//if (GameRun && !EMPTY_PERSONHP && CMonsterExtend::GetInstance().GetAroundMonsterListByName(gwsHeiLongJiao_MGK_ZhuoQiZhuName, CBaseObject::CreateFixPointInfo(50), VecMonster) != 0)
	//{
	//	CSomeFun::GetInstance().SetSpeed(6.0f);
	//	Monster.SetAngle();
	//	CFarmSkill::GetInstance().UseSkill_By_SkillId(Sum_SkillId_4);

	//	// 获取当前人物和球之间, 球屁股后面的3m的坐标(走过去要时间)
	//	LOG_CF_D(L"追着去格挡球! 当前人物ID=%X, 怪物仇恨ID=%X", VecMonster.at(0).GetTargetId());
	//	Point BehindPoint = _GetBehindByTwoPoint(CPerson::GetInstance().GetPoint(), VecMonster.at(0).GetPoint(), 3);
	//	BnsMove(BehindPoint, 1);
	//	VecMonster.clear();
	//}
	//LOG_CF_D(L"格挡球完毕!");
	CSomeFun::GetInstance().SetSpeed(3.0f);
	return TRUE;
}

int CFarmBossFish::GetTeamMemberIndex_By_BossDis(_In_ CONST CMonster& Boss) CONST
{
	std::vector<CTeamMember> VecTeamMember;
	CObjectFunction::GetInstance().GetTeamList(VecTeamMember);

	std::sort(VecTeamMember.begin(), VecTeamMember.end(), [Boss](CONST CTeamMember& Member1, CONST CTeamMember& Member2){ return Member1.GetDis(Boss.GetPoint()) < Member2.GetDis(Boss.GetPoint()); });
	return VecTeamMember.size() > 0 ? VecTeamMember.at(0).GetIndex() : -1;
}

VOID CFarmBossFish::GetVecYIKShadowByName(_In_ CONST std::wstring& wsMonsterName, _Out_ std::vector<CMonster>& VecMonster) CONST
{
	std::vector<CMonster> VecList;
	CMonsterExtend::GetInstance().GetAroundMonsterListByName(wsMonsterName, FixPointInfo(Point(-47008, 26147, 12119), 30), VecList);
	for (CONST auto& itm : VecList)
		VecMonster.push_back(std::move(itm));
}

VOID CFarmBossFish::HelpToSkillYLKShadow(_In_ em_Monster_YLK_Color emMonsterYLKColor) CONST
{
	while (GameRun && !EMPTY_PERSONHP)
	{
		// 获取相反颜色的伊鲁卡的影子
		std::vector<CMonster> VecYLK;
		GetVecYIKShadowByName(Monster_扭曲的影子, VecYLK);
		GetVecYIKShadowByName(Monster_倒下的影子, VecYLK);
		if (VecYLK.size() == 0)
		{
			LOG_CF_D( L"伊鲁卡的影子已经被全部消灭!");
			return;
		}
		LOG_CF_D( L"还有的伊鲁卡影子还没被消灭, 去帮忙!");

		// 杀影子!
		CMonster& Monster = VecYLK.at(0);
		KillYLKShadow(Monster, Monster.GetName() == Monster_扭曲的影子 ? em_Monster_YLK_Color_Blue : em_Monster_YLK_Color_Green, FALSE);
	}
	
}

VOID CFarmBossFish::KillYLKShadow(_In_ CONST CMonster& TarMonster, _In_ em_Monster_YLK_Color emMonsterYLKColor, _In_ BOOL bFirst) CONST
{
	CSomeFun::GetInstance().SimlationKey('E', 10);
	CSomeFun::GetInstance().SimlationKey('E', 10);
	// 走到影子旁边
	//BnsMove(TarMonster.GetBehindPoint(3.0f), 1);

	while (GameRun && !EMPTY_PERSONHP && !TarMonster.IsDead())
	{
		CPersonFunction::GetInstance().MoveToPoint(TarMonster.GetFacePoint(3.0f), 1);
		if (TarMonster.IsDead())
			break;


		// 第一次去的时候需要等待1.5s等待怪物完全刷新出来
		if (bFirst)
		{
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(1500, [TarMonster]
			{
				TarMonster.SetAngle();
				Sleep(100);
				CSomeFun::GetInstance().SimlationKey('R');
				return CSomeFun::GetInstance().SimlationKey('R');
			});

			auto Index = CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex();
			if (Index == 1 || Index == 3 || Index == 5)
				BnsSleep(3 * 1000);
			else
				BnsSleep(1000);
			bFirst = FALSE;
		}

		if (TarMonster.IsDead())
			break;
		if (TarMonster.GetDis() > 5)
			BnsMove(TarMonster.GetFacePoint(3.0f), 1);


		if (TarMonster.IsDead())
			break;
		else if (emMonsterYLKColor == em_Monster_YLK_Color::em_Monster_YLK_Color_Blue)
		{
			TarMonster.SetAngle();
			BnsSleep(100);
			CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
		}
		else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_V) && CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_2))
		{
			TarMonster.SetAngle();
			BnsSleep(100);
			CFarmSkill::GetInstance().UseSkill_By_SkillId(Sum_SkillId_V, em_Skill_Type::em_Skill_Type_Cat);
			TarMonster.SetAngle();
			BnsSleep(100);
			CFarmSkill::GetInstance().UseSkill_By_SkillId(Sum_SkillId_2);
		}
		BnsSleep(1000);
	}
}

CONST Point CFarmBossFish::_GetBehindByTwoPoint(_In_ CONST Point& p1, _In_ CONST Point& p2, _In_ float fDis) CONST
{
	float k = MyTools::CLPublic::GetDisBy2D(p1, p2);

	Point Point_;
	Point_.X = p1.X + (p2.X - p1.X) / k * (k + fDis * 50.0f);
	Point_.Y = p1.Y + (p2.Y - p1.Y) / k * (k + fDis * 50.0f);
	Point_.Z = p2.Z;
	return Point_;
}

