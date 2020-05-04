#include "stdafx.h"
#include "DailyTask_SnowHole.h"
#include <MyTools/Log.h>
#include <MyTools/TimeTick.h>
#include "DailyTask_Move.h"
#include "DailyTask_Item.h"
#include "DailyTask_Skill.h"
#include "DailyTask_BossSkill.h"
#include "TeamAuction.h"
#include "GameUI.h"
#include "ObjectFunction.h"
#include "Person.h"
#include "SomeFun.h"
#include "TaskExtend.h"
#include "Task.h"
#include "PersonFunction.h"
#include "TeamMemberExtend.h"
#include "BnsFilter.h"
#include "Monster.h"
#include "MonsterExtend.h"
#include "Farm.h"
#include "ResAction.h"
#include "FarmSkill.h"
#include "ResExtend.h"
#include "Corpse.h"
#include "Cat.h"
#include "CatExpend.h"

#define _SELF L"DailyTask_SnowHole.cpp"


CDailyTask_SnowHole::CDailyTask_SnowHole() : _BossRange(Point(-49872,24644,14448), 10), _dwLastSkillId(NULL)
{

}

BOOL CDailyTask_SnowHole::Run()
{
	CONST static std::vector<SnowHolePoint> VecPoint = 
	{
		{ Point(-52288,18008,13864),em_Dungeon_SnowHole_Step1 },
		{ Point(-52304,18488,13864),em_Dungeon_SnowHole_Step1 },
		{ Point(-52388,19716,13864),em_Dungeon_SnowHole_Step1 },
		{ Point(-52796,20000,13864),em_Dungeon_SnowHole_Step1 },
		{ Point(-53548,20084,13864),em_Dungeon_SnowHole_Step1 },
		{ Point(-54480,20840,13872),em_Dungeon_SnowHole_Step1 },
		{ Point(-54900,21264,13916),em_Dungeon_SnowHole_Step1 },
		{ Point(-55292,21684,14064),em_Dungeon_SnowHole_Step1 },
		{ Point(-55548,21944,14200),em_Dungeon_SnowHole_Step1 },
		{ Point(-55816,22240,14260),em_Dungeon_SnowHole_Step1 },
		{ Point(-56132,22604,14264),em_Dungeon_SnowHole_Step1 }, // 图腾 
		{ Point(-56132,22604,14264),em_Dungeon_SnowHole_Step2 }, // Clump
		{ Point(-56068,23048,14280),em_Dungeon_SnowHole_Step2 },
		{ Point(-55988,23564,14244),em_Dungeon_SnowHole_Step2 },
		{ Point(-55772,24228,14124),em_Dungeon_SnowHole_Step2 }, // -- BOSS1
		{ Point(-56252,25236,14076),em_Dungeon_SnowHole_Step3 },
		{ Point(-57136,25176,14060),em_Dungeon_SnowHole_Step3 },
		{ Point(-57136,25176,14060),em_Dungeon_SnowHole_Step4 }, // -Clump
		{ Point(-57088,25680,14032),em_Dungeon_SnowHole_Step4 },
		{ Point(-56980,26600,14044),em_Dungeon_SnowHole_Step4 },
		{ Point(-56416,27252,14044),em_Dungeon_SnowHole_Step4 },
		{ Point(-55924,27708,14076),em_Dungeon_SnowHole_Step4 },
		{ Point(-55476,28164,14196),em_Dungeon_SnowHole_Step4 },
		{ Point(-55200,28460,14340),em_Dungeon_SnowHole_Step4 },
		{ Point(-54892,28796,14436),em_Dungeon_SnowHole_Step4 },
		{ Point(-54496,29132,14440),em_Dungeon_SnowHole_Step4 }, // -- 图腾
		{ Point(-54496,29132,14440),em_Dungeon_SnowHole_Step5 }, //  Clump
		{ Point(-54216,29000,14436),em_Dungeon_SnowHole_Step5 },
		{ Point(-53844,29136,14432),em_Dungeon_SnowHole_Step5 },
		{ Point(-53400,29404,14436),em_Dungeon_SnowHole_Step5 }, // -- Clear 10m
		{ Point(-53532,29328,14436),em_Dungeon_SnowHole_Step5 },
		{ Point(-53068,29620,14440),em_Dungeon_SnowHole_Step5 }, // -- Clear 10m
		{ Point(-52684,30032,14512),em_Dungeon_SnowHole_Step5 },// -- Clear 10m
		{ Point(-52204,30644,14564),em_Dungeon_SnowHole_Step5 },// -- Clear 10m
		{ Point(-51696,30468,14556),em_Dungeon_SnowHole_Step5 },// -- Clear 10m
		{ Point(-50992,30884,14556),em_Dungeon_SnowHole_Step5 },// -- Clear 10m
		{ Point(-50460,30504,14504),em_Dungeon_SnowHole_Step5 },// -- Clear 10m
		{ Point(-50508,29672,14496),em_Dungeon_SnowHole_Step5 },// -- Clear 10m
		{ Point(-51316,29556,14520),em_Dungeon_SnowHole_Step5 },// -- Clear 10m
		{ Point(-52232,30132,14520),em_Dungeon_SnowHole_Step5 },// -- Clear 10m & Wait BOSS Die
		{ Point(-52004,30684,14564),em_Dungeon_SnowHole_Step6 },
		{ Point(-51600,31276,14568),em_Dungeon_SnowHole_Step6 },
		{ Point(-51200,31496,14632),em_Dungeon_SnowHole_Step6 },
		{ Point(-50716,31504,14748),em_Dungeon_SnowHole_Step6 },
		{ Point(-50232,31468,14876),em_Dungeon_SnowHole_Step6 },
		{ Point(-49640,31436,15008),em_Dungeon_SnowHole_Step6 },
		{ Point(-49356,31120,15056),em_Dungeon_SnowHole_Step6 },
		{ Point(-49028,30688,15076),em_Dungeon_SnowHole_Step6 },
		{ Point(-49176,30240,15124),em_Dungeon_SnowHole_Step6 },
		{ Point(-49584,29464,15176),em_Dungeon_SnowHole_Step6 },  
		{ Point(-49584,29464,15176),em_Dungeon_SnowHole_Step7 }, //--Clump
		{ Point(-49132,29320,15184),em_Dungeon_SnowHole_Step7 },
		{ Point(-48500,29232,15200),em_Dungeon_SnowHole_Step7 },
		{ Point(-48036,28888,15196),em_Dungeon_SnowHole_Step7 },
		{ Point(-47556,28456,15212),em_Dungeon_SnowHole_Step7 },
		{ Point(-47124,27928,15288),em_Dungeon_SnowHole_Step7 },
		{ Point(-46640,27504,15500),em_Dungeon_SnowHole_Step7 },
		{ Point(-46200,27024,15600),em_Dungeon_SnowHole_Step7 },
		{ Point(-45988,26752,15596),em_Dungeon_SnowHole_Step7 }, // -- 图腾
		{ Point(-46052,26372,15596),em_Dungeon_SnowHole_Step8 },
		{ Point(-46012,25544,15592),em_Dungeon_SnowHole_Step8 },
		{ Point(-46040,24928,15584),em_Dungeon_SnowHole_Step8 },
		{ Point(-46360,24572,15584),em_Dungeon_SnowHole_Step8 },
		{ Point(-47192,24580,15576),em_Dungeon_SnowHole_Step8 },
		{ Point(-48072,24616,15580),em_Dungeon_SnowHole_Step8 },
		{ Point(-48468,24636,15572),em_Dungeon_SnowHole_Step8 }, // -- Clump
	};



	CONST std::vector<Dungeon_SnowHole_Content> VecMethodPtr = 
	{
		{ em_Dungeon_SnowHole_Step1 ,std::bind(&CDailyTask_SnowHole::Step1, this, std::placeholders::_1)},
		{ em_Dungeon_SnowHole_Step2 ,std::bind(&CDailyTask_SnowHole::Step2, this, std::placeholders::_1)},
		{ em_Dungeon_SnowHole_Step3 ,std::bind(&CDailyTask_SnowHole::Step3, this, std::placeholders::_1)},
		{ em_Dungeon_SnowHole_Step4 ,std::bind(&CDailyTask_SnowHole::Step4, this, std::placeholders::_1)},
		{ em_Dungeon_SnowHole_Step5 ,std::bind(&CDailyTask_SnowHole::Step5, this, std::placeholders::_1)},
		{ em_Dungeon_SnowHole_Step6 ,std::bind(&CDailyTask_SnowHole::Step6, this, std::placeholders::_1)},
		{ em_Dungeon_SnowHole_Step7 ,std::bind(&CDailyTask_SnowHole::Step7, this, std::placeholders::_1)},
		{ em_Dungeon_SnowHole_Step8 ,std::bind(&CDailyTask_SnowHole::Step8, this, std::placeholders::_1)},
		{ em_Dungeon_SnowHole_Step9 ,std::bind(&CDailyTask_SnowHole::Step9, this, std::placeholders::_1) },
		{ em_Dungeon_SnowHole_Step10 ,std::bind(&CDailyTask_SnowHole::Step10, this, std::placeholders::_1) },
		{ em_Dungeon_SnowHole_End ,std::bind(&CDailyTask_SnowHole::End, this, std::placeholders::_1) },
	};

	if (!Initialize())
	{
		LOG_CF_E(L"Initialize Faild!");
		return FALSE;
	}

	while (GameRun && IsInDungeon())
	{
		auto Index = FindNearestIndexByVecStep(VecPoint, CPerson::GetInstance().GetPoint(), 16);
		if (Index == -1)
		{
			LOG_CF_D(L"Index = -1");
			return FALSE;
		}

		LOG_CF_D(L"Index=%d", Index);
		for (UINT i = static_cast<UINT>(Index); i < VecMethodPtr.size() && GameRun && !EMPTY_PERSONHP; ++i)
		{
			auto& itm = VecMethodPtr.at(i);
			if (!itm.StepPtr(FindVecPointByStep(VecPoint, itm.emStep)))
				return FALSE;

			CFarm::GetInstance().EatFood(50);
		}

		if (EMPTY_PERSONHP)
		{
			LOG_CF_D(L"挂了! 开始复活!");
			CPersonFunction::GetInstance().Resurrect();
		}
	}

	return TRUE;
}

BOOL CDailyTask_SnowHole::KillBoss()
{
	std::vector<Point> Vec;
	return Step9(Vec);
}

BOOL CDailyTask_SnowHole::Initialize()
{
	// 不需要竞拍, 得到的东西全部给队长
	CTeamAuction::GetInstance().SetAuction_Null();
	if (IsInDungeon())
	{
		_DailyTask_Team.CreateTeam();
		_DailyTask_Team.SetSchedule(0, L"Initialize");
		return TRUE;
	}

	// 处理物品
	CDailyTask_Move BeforeDoor;
	CDailyTask_Item DailyTask_Item;
	if (!DailyTask_Item.CheckDailyTaskInitializeItem())
		return FALSE;
	else if (DailyTask_Item.IsDealWithDungeonItem())
	{
		BeforeDoor.MoveToFengYueGuanSellNpc();
		DailyTask_Item.DealWithDungeonItem_Sell(Npc_吴步尚);
	}
	if (DailyTask_Item.IsRepairWeaspon(50))
	{
		BeforeDoor.MoveToFengYueGuanCenter();
		BeforeDoor.MoveToFengYueGuanRepair();
		BnsSleep(3 * 1000);
		DailyTask_Item.RepairWeaspon(50);
	}

	BeforeDoor.MoveToFengYueGuanCenter();

	if (!AccpetDailyTask())
	{
		LOG_CF_D(L"该日常已经被完成了!");
		return FALSE;
	}

	StudyDailySkill();

	_DailyTask_Team.CreateTeam();
	_DailyTask_Team.SetSchedule(0, L"Initialize");

	return MoveToDungeon();
}

BOOL CDailyTask_SnowHole::MoveToDungeon()
{
	// 副本门口
	BnsMove(Point(-39004, 6720, -1728), 3);
	BnsMove(Point(-38924, 6644, -1700), 3);
	while (GameRun && !IsInDungeon())
	{
		BnsMove(Point(-37924, 6544, -1700), 1);
		BnsSleep(3 * 1000);
		CPersonFunction::GetInstance().WaitGameLoad();
	}
	return TRUE;
}

BOOL CDailyTask_SnowHole::Step1(_In_ CONST std::vector<Point>& VecPoint)
{
	// 先集合
	_DailyTask_Team.ClumpTeamMember(1, L"第一波蛇蘑", VecPoint.front());
	return SnakeMushRoom(VecPoint);
}

BOOL CDailyTask_SnowHole::Step2(_In_ CONST std::vector<Point>& VecPoint)
{
	_DailyTask_Team.ClumpTeamMember(2, L"大力丘八佬", VecPoint.front());

	// Move to Target
	_DailyTask_Move.MoveToVecPoint(VecPoint, [](CONST Point&) { return FALSE; });

	struct tagHitSnowPoint
	{
		Point Pt;
		DWORD dwHeight;
		CONST Point& GetPoint() CONST
		{
			return this->Pt;
		}
	};

	CONST static std::vector<tagHitSnowPoint> vlst = {
		{ Point(-55648, 24840, 14068), 0xFC0 },{ Point(-56324, 25452, 14076), 0x11A0 },
		{ Point(-56980, 25136, 14068), 0x14D0 },{ Point(-56876, 24472, 14056), 0x1820 },
		{ Point(-56332, 24252, 14136), 0x1230 },{ Point(-55897, 24305, 14118), 0xF30 },
	};

	std::vector<DWORD> FilterBallVec;

	// 要绕过柱子去走向BOSS
	auto fnMoveToBoss = [](CONST CMonster& Monster)
	{
		// 判断BOSS离哪个坐标近
		auto nRecentlyBossIndex = MyTools::CLPublic::GetRecentlyIndexByPointVec(vlst, Monster.GetPoint(), 30.0f * 50.0f);
		// 判断自己离哪个坐标近
		auto nRecentlyPlayerIndex = MyTools::CLPublic::GetRecentlyIndexByPointVec(vlst, CPerson::GetInstance().GetPoint(), 30.0f * 50.0f);

		if (nRecentlyBossIndex == -1 || nRecentlyPlayerIndex == -1)
		{
			LOG_CF_E(L"没法找到哪个坐标离BOSS最近, 不可能啊?nRecentlyIndex=%d,nRecentlyPlayerIndex=%d", nRecentlyBossIndex, nRecentlyPlayerIndex);
			// 直接走向BOSS
			BnsMove(Monster.GetPoint(), 3);
			return;
		}

		if (nRecentlyBossIndex > nRecentlyPlayerIndex)
		{
			// BOSS在后面, 顺着坐标走
			for (int i = static_cast<decltype(nRecentlyPlayerIndex)>(nRecentlyPlayerIndex); i <= nRecentlyBossIndex; ++i)
				BnsMove(vlst.at(i).Pt, 3);

			return;
		}

		// BOSS在前面, 逆着走
		for (int i = static_cast<decltype(nRecentlyPlayerIndex)>(nRecentlyPlayerIndex); i >= nRecentlyBossIndex; --i)
			BnsMove(vlst.at(i).Pt, 3);

		return;
	};

	// 绕过柱子走到第三个坐标(第三个坐标离门口最近)
	auto fnMoveToDoor = []
	{
		CONST int nIndex = 2;
		// 判断自己离哪个坐标近
		auto nRecentlyPlayerIndex = MyTools::CLPublic::GetRecentlyIndexByPointVec(vlst, CPerson::GetInstance().GetPoint(), 30.0f * 50.0f);
		if (nRecentlyPlayerIndex == -1)
		{
			BnsMove(vlst.at(nIndex).Pt, 3);
			return TRUE;
		}

		if (nIndex > nRecentlyPlayerIndex)
		{
			for (int i = nRecentlyPlayerIndex; i <= nIndex; ++i)
				BnsMove(vlst.at(i).Pt, 3);
			return TRUE;
		}

		for (int i = nRecentlyPlayerIndex; i >= nIndex; --i)
			BnsMove(vlst.at(i).Pt, 3);
		return TRUE;
	};

	CMonster Boss;
	if (!CMonsterExtend::GetInstance().FindMonster_By_Name(Monster_大力丘八佬, CBaseObject::CreateFixPointInfo(100.0f), &Boss))
	{
		LOG_CF_D(L"找不到高台雪人!");
		goto lb_Exit;
	}
	CSomeFun::GetInstance().SetSpeed(3.0f);


	auto dwTeamMemberIndex = CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex();
	if (dwTeamMemberIndex != NULL && dwTeamMemberIndex < 6)
	{
		for (auto i = 0; i < static_cast<int>(dwTeamMemberIndex); ++i)
			CPersonFunction::GetInstance().MoveToPoint(vlst.at(i).GetPoint(), 3, 100);
	}

	// 添加脚本黑名单, 不要打BOSS
	CFilter::GetInstance().AddMonsterFilter(Monster_大力丘八佬, CFilter::MonsterFilterType::Filter_Monster_Low);

	auto nIndex = 0;
	while (GameRun && !EMPTY_PERSONHP && !Boss.IsDead())
	{
		nIndex = nIndex >= 5 ? 0 : nIndex + 1;
		if (fabs(Boss.GetPoint().Z - CPerson::GetInstance().GetPoint().Z) <= 100)
		{
			LOG_CF_D(L"雪人掉下来拉!");
			fnMoveToBoss(Boss);
			CFarm::GetInstance().KillMonster_By_Single(Boss, CBaseObject::CreateFixPointInfo(50.0f), TRUE, nullptr);
			BnsSleep(1000);
			CResAction::GetInstance().TakeDropItem(CBaseObject::CreateFixPointInfo(16));
			break;
		}

		if (Boss.GetTargetId() == CPerson::GetInstance().GetId())
		{
			LOG_CF_D(L"躲BOSS的大雪球!");
			BnsMove(vlst.at(nIndex).GetPoint(), 3);
			if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_3))
				CFarmSkill::GetInstance().UseSkill_By_SkillId(Sum_SkillId_3);
			else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_4))
				CFarmSkill::GetInstance().UseSkill_By_SkillId(Sum_SkillId_4);
			else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_HideUi(Sum_SkillId_E))
				CSomeFun::GetInstance().SimlationKey('E');
			continue;
		}

		CCorpse Corpse;
		if (CResExtend::GetInstance().FindTopBall(CBaseObject::CreateFixPointInfo(16.0f), FilterBallVec, &Corpse)) // 找最近的球
		{
			LOG_CF_D(L"捡起雪球!");
			if (Corpse.GetDis() > 2.0f)
			{
				BnsMove(Corpse.GetPoint(), 1);
				continue;
			}

			FilterBallVec.push_back(Corpse.GetId());
			CPersonFunction::GetInstance().Pickup(Corpse.GetObjectId());
			if (!CPerson::GetInstance().IsPickStatus())
			{
				LOG_CF_D(L"并没有检测到捡起雪球的状态!");
				continue;
			}

			auto nRecentlyIndex = MyTools::CLPublic::GetRecentlyIndexByPointVec(vlst, CPerson::GetInstance().GetPoint(), 30.0f * 50.0f);
			if (nRecentlyIndex == -1)
			{
				LOG_CF_E(L"找不到最近的坐标……");
				continue;
			}

			BnsMove(vlst.at(nRecentlyIndex).GetPoint(), 3);
			LOG_CF_D(L"开始扔雪球!");
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2 * 1000, [&Boss, &nRecentlyIndex] {
				CBaseObject::SetAngle(Boss.GetPoint(), vlst.at(nRecentlyIndex).dwHeight);
				CSomeFun::GetInstance().SimlationKey('R', 200, 300);
				return CPerson::GetInstance().IsPickStatus();
			});
		}

		// 召回喵!
		if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_HideUi(Sum_SkillId_E))
			CSomeFun::GetInstance().SimlationKey('E');

		// 找小雪人!
		CMonster SmallSnowman;
		if (CMonsterExtend::GetInstance().FindMonster_By_Name(Monster_美风雪人, CBaseObject::CreateFixPointInfo(10.0f), &SmallSnowman))
		{
			LOG_CF_D(L"干小雪人!");
			// 要用Tab, 防止喵上去干雪人
			SmallSnowman.SetAngle();
			CSomeFun::GetInstance().SimlationKey(VK_TAB);
			CFarm::GetInstance().KillMonster_By_Single(SmallSnowman, CBaseObject::CreateFixPointInfo(10.0f), TRUE, nullptr);
			continue;
		}
		else if (CMonsterExtend::GetInstance().GetAroundMonsterCount(CBaseObject::CreateFixPointInfo(16.0f)) != 0)
		{
			std::vector<CMonster> TarMonsterVec;
			if (CMonsterExtend::GetInstance().GetAroundTargetMonsterList(CBaseObject::CreateFixPointInfo(16.0f), TarMonsterVec) > 0)
			{
				SmallSnowman = TarMonsterVec.at(0);
				LOG_CF_D(L"干小雪人!");
				// 要用Tab, 防止喵上去干雪人
				SmallSnowman.SetAngle();
				CSomeFun::GetInstance().SimlationKey(VK_TAB);
				CFarm::GetInstance().KillMonster_By_Single(SmallSnowman, CBaseObject::CreateFixPointInfo(16.0f), TRUE, nullptr);
				continue;
			}
		}

		// 没雪人也没球的时候站着发呆等刷新就好了!
		BnsSleep(500);

	}

lb_Exit:;
	CFilter::GetInstance().RemoveMonsterFilter(Monster_大力丘八佬);
	return fnMoveToDoor();

}

BOOL CDailyTask_SnowHole::Step3(_In_ CONST std::vector<Point>& VecPoint)
{
	CFarm::GetInstance().EatFood(50);
	_DailyTask_Move.MoveToVecPoint(VecPoint, [](CONST Point&) { return FALSE; });
	return TRUE;
}

BOOL CDailyTask_SnowHole::Step4(_In_ CONST std::vector<Point>& VecPoint)
{
	_DailyTask_Team.ClumpTeamMember(4, L"第二波蛇蘑", VecPoint.front());
	return SnakeMushRoom(VecPoint);
}

BOOL CDailyTask_SnowHole::Step5(_In_ CONST std::vector<Point>& VecPoint)
{
	_DailyTask_Team.ClumpTeamMember(5, L"雪人捕获队长", VecPoint.back());
	// disable Take item
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_STOPTAKEITEM, TRUE);
	_DailyTask_Move.FarmAndMove(VecPoint, 10);

	CMonster Boss;
	if (!CMonsterExtend::GetInstance().FindMonster_By_Name(Monster_雪人捕获队长, CBaseObject::CreateFixPointInfo(50), &Boss))
		return TRUE;

	while (GameRun && !EMPTY_PERSONHP && !Boss.IsDead())
	{
		BnsMove(VecPoint.back(), 3);
		CFarm::GetInstance().ClearAroundMonster(CBaseObject::CreateFixPointInfo(16), [] { return FALSE; });
	}
	BnsMove(VecPoint.back(), 3);

	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_STOPTAKEITEM, FALSE);
	return TRUE;
}

BOOL CDailyTask_SnowHole::Step6(_In_ CONST std::vector<Point>& VecPoint)
{
	CFilter::GetInstance().AddMonsterFilter(Monster_恶童雪人, CFilter::MonsterFilterType::Filter_Monster_Low);
	_DailyTask_Move.MoveToVecPoint(VecPoint, 0);
	CFilter::GetInstance().RemoveMonsterFilter(Monster_恶童雪人);
	return TRUE;
}

BOOL CDailyTask_SnowHole::Step7(_In_ CONST std::vector<Point>& VecPoint)
{
	_DailyTask_Team.ClumpTeamMember(7, L"第三波蛇菇", VecPoint.front());
	return SnakeMushRoom(VecPoint);
}

BOOL CDailyTask_SnowHole::Step8(_In_ CONST std::vector<Point>& VecPoint)
{
	return _DailyTask_Move.MoveToVecPoint(VecPoint, 0);
}

BOOL CDailyTask_SnowHole::Step9(_In_ CONST std::vector<Point>&)
{
	if (GameRun && !EMPTY_PERSONHP)
	{
		CMonster Boss;
		if (CMonsterExtend::GetInstance().FindMonster_By_Name(Monster_灯笼雪人, CBaseObject::CreateFixPointInfo(100), &Boss))
		{
			CONST std::vector<KillBossMethod> Vec =
			{
				{ em_Player_Classes_ForceMaster, std::bind(&CDailyTask_SnowHole::KillBoss_ForceMaster, this, std::placeholders::_1) },
				{ em_Player_Classes_BladeMaster, std::bind(&CDailyTask_SnowHole::KillBoss_BladeMaster, this, std::placeholders::_1) },
				{ em_Player_Classes_KongFuMaster, std::bind(&CDailyTask_SnowHole::KillBoss_KongFuMaster, this, std::placeholders::_1) },
			};

			auto emPlayerClasses = CPerson::GetInstance().GetClasses();
			auto Element = MyTools::CLPublic::Vec_find_if_Const(Vec, [emPlayerClasses](CONST KillBossMethod& itm) { return itm.emPlayerClasses == emPlayerClasses; });
			if (Element != nullptr)
			{
				SetBossEnviroment(Boss);
				_DailyTask_Move.MoveToTarget(Monster_灯笼雪人, 100);

				Element->MethodPtr(Boss);

				RestoreBossEnviroment();
				WaitToMemberDead();
			}
		}
	}
	LOG_MSG_CF(L"Done!");
	return FALSE;
}

BOOL CDailyTask_SnowHole::Step10(_In_ CONST std::vector<Point>&)
{
	if (GameRun && !EMPTY_PERSONHP)
	{
		if (!CMonsterExtend::GetInstance().FindMonster_By_Name(Monster_卡鲁拉, CBaseObject::CreateFixPointInfo(30), nullptr))
			return TRUE;

		_DailyTask_Team.ClumpTeamMember(10, L"卡鲁拉", Point(-48468, 24636, 15572));

		CMonster Boss;
		if (CMonsterExtend::GetInstance().FindMonster_By_Name(Monster_卡鲁拉, CBaseObject::CreateFixPointInfo(50), &Boss))
		{
			SetBossEnviroment(Boss);
			KillHideBoss(Boss);
			RestoreBossEnviroment();
			WaitToMemberDead(); 
		}
	}
	return TRUE;
}



BOOL CDailyTask_SnowHole::End(_In_ CONST std::vector<Point>&)
{
	// 交斩首任务
	if (CTask::GetInstance().ExistTask(0x61EC, nullptr))
	{
		CTaskExtend::GetInstance().SumitTask_By_Parm(SumitTask_Src(0x2, 0x61EC, 0x0, 0x0, 0xFF, 0x261EC, 0x0, 0x0), L"", L"");
	}

	// 日常任务
	if (CTask::GetInstance().ExistTask(0x1E3, nullptr))
	{
		CTaskExtend::GetInstance().SumitTask_By_Parm(SumitTask_Src(0x2, 0x1E3, 0x0, 0x1, 0xFF, 0x201E3, 0x0, 0x0), L"", L"");
	}

	// 离开副本
	CResExtend::GetInstance().DoAction_By_ExistCorpse_For_Name(Monster_逃脱龙脉, CBaseObject::CreateFixPointInfo(100), [](CONST CCorpse& Door)
	{
		BnsMove(Door.GetPoint(), 1);
		BnsSleep(3 * 1000);
		Door.Take();
	});

	BnsSleep(5 * 1000);
	CPersonFunction::GetInstance().WaitGameLoad();
	return TRUE;
}

BOOL CDailyTask_SnowHole::SnakeMushRoom(_In_ CONST std::vector<Point>& VecPoint)
{
	CFilter::GetInstance().AddMonsterFilter(Monster_有裂痕的雪球, CFilter::MonsterFilterType::Filter_Monster_Low);	// 有裂痕的雪球
	CFilter::GetInstance().AddMonsterFilter(Monster_恶童雪人, CFilter::MonsterFilterType::Filter_Monster_Low);		// 恶童雪人
	CFilter::GetInstance().AddMonsterFilter(Monster_雪球控制图腾, CFilter::MonsterFilterType::Filter_Monster_High); // 
	_DailyTask_Move.FarmAndMove(VecPoint, 16);
	CFilter::GetInstance().RemoveMonsterFilter(Monster_恶童雪人);
	return TRUE;
}

BOOL CDailyTask_SnowHole::AccpetDailyTask() CONST
{
	CONST static std::vector<Point> VecPoint = 
	{
		Point(-39760,7768,-1738),Point(-39197,7243,-1738),Point(-39205,7092,-1738),Point(-39211,6940,-1738),Point(-39205,6785,-1738),Point(-39197,6634,-1738),
	};

	_DailyTask_Move.MoveToVecPoint(VecPoint, 0);

	SumitBLTask_Src Content = { 0,0,0x1E3,0,0 };
	CTaskExtend::GetInstance().PickBLTask_By_Npc(Content, Npc_于学尚);
	BnsSleep(3 * 1000);
	return CTask::GetInstance().ExistTask(0x1E3, nullptr);
}

VOID CDailyTask_SnowHole::StudyDailySkill() CONST
{
	CONST static std::vector<LearnSkillContent> VecForceMasterSkill =
	{
		{ ForceMaster_SkillId_1_1, 2 }, { ForceMaster_SkillId_2, 2 }, { ForceMaster_SkillId_3, 2 }, { ForceMaster_SkillId_Fure_Z, 0 },
		{ ForceMaster_SkillId_Fure_X, 1 },{ ForceMaster_SkillId_Ice_Z,2 },{ ForceMaster_SkillId_Ice_X,0 },
		{ ForceMaster_SkillId_Ice_V,0 },{ ForceMaster_SkillId_R,0 },{ ForceMaster_SkillId_T,0 },{ ForceMaster_SkillId_SS,0 }
	};

	CONST static std::vector<LearnSkillContent> VecBladeMasterSkill =
	{
		{ BladeMaster_SkillId_R1,0 },{ BladeMaster_SkillId_T1,1 },{ BladeMaster_SkillId_2,2 },{ BladeMaster_SkillId_Tab,1 },{ BladeMaster_SkillId_X,0 },
		{ BladeMaster_SkillId_R2, 0 },{ BladeMaster_SkillId_Z,0 },{ BladeMaster_SkillId_C,0 },{ BladeMaster_SkillId_4,2 },{ BladeMaster_SkillId_Gesture_3,0 },
		{ BladeMaster_SkillId_V,0 }
	};


	auto emPlayerClasses = CPerson::GetInstance().GetClasses();
	switch (emPlayerClasses)
	{
	case em_Player_Classes_Destroyer:
		break;
	case em_Player_Classes_Assassin:
		break;
	case em_Player_Classes_BladeMaster:
		CDailyTask_Skill().LearningSkill(L"剑士火焰系", VecBladeMasterSkill);
		break;
	case em_Player_Classes_ForceMaster:
		CDailyTask_Skill().LearningSkill(L"气功火系", VecForceMasterSkill);
		break;
	case em_Player_Classes_KongFuMaster:
		break;
	case em_Player_Classes_Danncer:
		break;
	case em_Player_Classes_Warlock:
		break;
	case em_Player_Classes_KongFuForceMaster:
		break;
	case em_Player_Classes_Gunner:
		break;
	default:
		break;
	}
}

BOOL CDailyTask_SnowHole::IsInDungeon() CONST
{
	return CPerson::GetInstance().GetMapId() == 0x1717;
}

VOID CDailyTask_SnowHole::KillBoss_BladeMaster(_In_ CONST CMonster& Boss)
{
	auto& DailyTask_BossSkill = CDailyTask_BossSkill::GetInstance();
	CDailyTask_Skill DailyTaskSkill;

	while (GameRun && !Boss.IsDead())
	{
		if (EMPTY_PERSONHP)
		{
			break;
		}

		auto Pr = DailyTask_BossSkill.GetBossSkillId();
		if (!Pr.first)
		{
			NormalMove(Boss);
			DailyTaskSkill.RefreshVecSkill();
			Skill_BladeMaster(Boss, DailyTaskSkill);
			continue;
		}

		auto dwBossSkillId = Pr.second.dwSkillId;

		// 
		Boss.SetAngle();
		DailyTaskSkill.RefreshVecSkill();


		//LOG_CF_D(L"Echo Skill[%X] Tick=%d,Tick=%d", dwBossSkillId, static_cast<DWORD>(ulTick - Pr.second.ulTick), static_cast<DWORD>(Pr.second.ulTick));
		switch (dwBossSkillId)
		{
		case em_Skill_灯笼雪人_挥击: // 挥击 
			BossSkill_挥击_BladeMaster(Boss, DailyTaskSkill, Pr.second);
			break;
		case em_Skill_灯笼雪人_腾空: // 腾空
			BossSkill_腾空_BladeMaster(Boss, DailyTaskSkill, Pr.second);
			break;
		case em_Skill_灯笼雪人_击掌: // 击掌
			BossSkill_击掌_BladeMaster(Boss, DailyTaskSkill, Pr.second);
			break;
		case em_Skill_灯笼雪人_墩摔: // 墩摔
			BossSkill_墩摔_BladeMaster(Boss, DailyTaskSkill, Pr.second);
			break;
		case em_Skill_灯笼雪人_前扑: // 前扑
			BossSkill_前扑_BladeMaster(Boss, DailyTaskSkill, Pr.second);
			break;
		case em_Skill_灯笼雪人_吸取热气: // 吸取热气
			BossSkill_吸取热气_BladeMaster(Boss, DailyTaskSkill, Pr.second);
			break;
		case em_Skill_灯笼雪人_天来跃: // 天来跃
			BossSkill_天来跃_BladeMaster(Boss, DailyTaskSkill, Pr.second);
			break;
		case em_Skill_灯笼雪人_冻波撼地: // 冻波撼地
			BossSkill_冻波撼地_BladeMaster(Boss, DailyTaskSkill, Pr.second);
			break;
		case em_Skill_灯笼雪人_抢夺: // 抢夺
			BossSkill_抢夺_BladeMaster(Boss, DailyTaskSkill, Pr.second);
			break;
		case em_Skill_灯笼雪人_热气注入: // 热气注入
			//LOG_CF_D(L"热气注入 不处理");
			break;
		case em_Skill_灯笼雪人_咆哮:
			break;
		case em_Skill_灯笼雪人_寒气喷发:
			break;
		case em_Skill_灯笼雪人_寒气雪舞左:
			break;
		default:
			break;
		}
		_dwLastSkillId = dwBossSkillId;
	}
}

VOID CDailyTask_SnowHole::KillBoss_ForceMaster(_In_ CONST CMonster& Boss)
{
	auto& DailyTask_BossSkill = CDailyTask_BossSkill::GetInstance();
	CDailyTask_Skill DailyTaskSkill;

	while (GameRun && !Boss.IsDead())
	{
		if (EMPTY_PERSONHP)
		{
			break;
		}

		auto Pr = DailyTask_BossSkill.GetBossSkillId();
		if (!Pr.first)
		{
			NormalMove(Boss);
			DailyTaskSkill.RefreshVecSkill();
			Skill_ForceMaster(Boss, DailyTaskSkill);
			continue;
		}

		auto dwBossSkillId = Pr.second.dwSkillId;

		// 
		Boss.SetAngle();
		DailyTaskSkill.RefreshVecSkill();

		auto ulTick = ::GetTickCount64();
		LOG_CF_D(L"Echo Skill[%X] Tick=%d,Tick=%d", dwBossSkillId, static_cast<DWORD>(ulTick - Pr.second.ulTick), static_cast<DWORD>(Pr.second.ulTick));
		switch (dwBossSkillId)
		{
		case em_Skill_灯笼雪人_挥击: // 挥击 
			LOG_CF_D(L"挥击");
			break;
		case em_Skill_灯笼雪人_腾空: // 腾空
			LOG_CF_D(L"腾空");
			break;
		case em_Skill_灯笼雪人_击掌: // 击掌
			LOG_CF_D(L"击掌");
			break;
		case em_Skill_灯笼雪人_墩摔: // 墩摔
			LOG_CF_D(L"墩摔");
			break;
		case em_Skill_灯笼雪人_前扑: // 前扑
			BossSkill_前扑_ForceMaster(Boss, DailyTaskSkill, Pr.second);
			break;
		case em_Skill_灯笼雪人_吸取热气: // 吸取热气
			BossSkill_吸取热气_ForceMaster(Boss, DailyTaskSkill, Pr.second);
			break;
		case em_Skill_灯笼雪人_天来跃: // 天来跃
			BossSkill_天来跃_ForceMaster(Boss, DailyTaskSkill, Pr.second);
			break;
		case em_Skill_灯笼雪人_冻波撼地: // 冻波撼地
			BossSkill_冻波撼地_ForceMaster(Boss, DailyTaskSkill, Pr.second);
			break;
		case em_Skill_灯笼雪人_抢夺: // 抢夺
			BossSkill_抢夺_ForceMaster(Boss, DailyTaskSkill, Pr.second);
			break;
		case em_Skill_灯笼雪人_热气注入: // 热气注入
			LOG_CF_D(L"热气注入 不处理");
			break;
		case em_Skill_灯笼雪人_咆哮:
			LOG_CF_D(L"咆哮 不处理");
			break;
		case em_Skill_灯笼雪人_寒气喷发:
			LOG_CF_D(L"寒气喷发 不处理");
			break;
		case em_Skill_灯笼雪人_寒气雪舞左:
			LOG_CF_D(L"寒气雪舞左 不处理");
			break;
		default:
			break;
		}
		_dwLastSkillId = dwBossSkillId;
	}
}

VOID CDailyTask_SnowHole::KillBoss_KongFuMaster(_In_ CONST CMonster& Boss)
{
	auto& DailyTask_BossSkill = CDailyTask_BossSkill::GetInstance();
	CDailyTask_Skill DailyTaskSkill;

	while (GameRun && !Boss.IsDead())
	{
		if (EMPTY_PERSONHP)
		{
			break;
		}

		auto Pr = DailyTask_BossSkill.GetBossSkillId();
		if (!Pr.first)
		{
			NormalMove(Boss);
			DailyTaskSkill.RefreshVecSkill();
			Skill_KongFuMaster(Boss, DailyTaskSkill);
			continue;
		}

		auto dwBossSkillId = Pr.second.dwSkillId;

		// 
		Boss.SetAngle();
		DailyTaskSkill.RefreshVecSkill();


		//LOG_CF_D(L"Echo Skill[%X] Tick=%d,Tick=%d", dwBossSkillId, static_cast<DWORD>(ulTick - Pr.second.ulTick), static_cast<DWORD>(Pr.second.ulTick));
		switch (dwBossSkillId)
		{
		case em_Skill_灯笼雪人_挥击: // 挥击 
			BossSkill_挥击_KongFuMaster(Boss, DailyTaskSkill, Pr.second);
			break;
		case em_Skill_灯笼雪人_腾空: // 腾空
			BossSkill_腾空_KongFuMaster(Boss, DailyTaskSkill, Pr.second);
			break;
		case em_Skill_灯笼雪人_击掌: // 击掌
			BossSkill_击掌_KongFuMaster(Boss, DailyTaskSkill, Pr.second);
			break;
		case em_Skill_灯笼雪人_墩摔: // 墩摔
			BossSkill_墩摔_KongFuMaster(Boss, DailyTaskSkill, Pr.second);
			break;
		case em_Skill_灯笼雪人_前扑: // 前扑
			BossSkill_前扑_KongFuMaster(Boss, DailyTaskSkill, Pr.second);
			break;
		case em_Skill_灯笼雪人_吸取热气: // 吸取热气
			BossSkill_吸取热气_KongFuMaster(Boss, DailyTaskSkill, Pr.second);
			break;
		case em_Skill_灯笼雪人_天来跃: // 天来跃
			BossSkill_天来跃_KongFuMaster(Boss, DailyTaskSkill, Pr.second);
			break;
		case em_Skill_灯笼雪人_冻波撼地: // 冻波撼地
			BossSkill_冻波撼地_KongFuMaster(Boss, DailyTaskSkill, Pr.second);
			break;
		case em_Skill_灯笼雪人_抢夺: // 抢夺
			BossSkill_抢夺_KongFuMaster(Boss, DailyTaskSkill, Pr.second);
			break;
		case em_Skill_灯笼雪人_热气注入: // 热气注入
			//LOG_CF_D(L"热气注入 不处理");
			break;
		case em_Skill_灯笼雪人_咆哮:
			break;
		case em_Skill_灯笼雪人_寒气喷发:
			break;
		case em_Skill_灯笼雪人_寒气雪舞左:
			break;
		default:
			break;
		}
		_dwLastSkillId = dwBossSkillId;
	}
}

VOID CDailyTask_SnowHole::BossSkill_挥击_BladeMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	//LOG_CF_D(L"挥击");
	if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_F))
	{
		TickBossSkill_.BeforeSleep(500);
		//LOG_CF_D(L"击剑");
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_F, TRUE);
		//DWORD dwTimeOut = TickBossSkill_.GetBeforeSleepTime(2500);
		////LOG_CF_D(L"dwTimeOut=%d, Tick=%d", dwTimeOut, static_cast<DWORD>(::GetTickCount64() - TickBossSkill_.ulTick));

		LOG_CF_D(L"Tick=%d", static_cast<DWORD>(::GetTickCount64()));
		CSomeFun::GetInstance().TimeOut_Condiction_GameRun(1000, [&DailyTaskSkill]
		{
			LOG_CF_D(L"挥击击剑");
			DailyTaskSkill.RefreshVecSkill();
			auto boolflag = DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_T7);
			LOG_CF_D(L"挥击击剑-%d", boolflag);
			return !boolflag;
		});
		LOG_CF_D(L"挥击击剑结束,Tick=%d", static_cast<DWORD>(::GetTickCount64()));

	}
	else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_V))
	{
		//LOG_CF_D(L"一闪躲技能");
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_V, TRUE);
	}
	else
	{
		//LOG_CF_D(L"用1格挡");
		TickBossSkill_.BeforeSleep(500);
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_1, TRUE);
		//DWORD dwTimeOut = TickBossSkill_.GetBeforeSleepTime(3000);
		////LOG_CF_D(L"dwTimeOut=%d, Tick=%d", dwTimeOut, static_cast<DWORD>(::GetTickCount64() - TickBossSkill_.ulTick));
		//CSomeFun::GetInstance().TimeOut_Condiction_GameRun(dwTimeOut <= 1000 ? 1000 : dwTimeOut, [DailyTaskSkill]
		//{
		//	CSomeFun::GetInstance().SimlationKey(VK_1, 0);
		//	return !CFarmSkill::GetInstance().UseSkill_By_SkillId_WhichCool(BladeMaster_SkillId_F3);
		//});

		LOG_CF_D(L"Tick=%d", static_cast<DWORD>(::GetTickCount64()));
		CSomeFun::GetInstance().TimeOut_Condiction_GameRun(1000, [&DailyTaskSkill]
		{
			LOG_CF_D(L"挥击破剑");
			DailyTaskSkill.RefreshVecSkill();
			auto boolflag = DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_T1);
			LOG_CF_D(L"挥击破剑-%d", boolflag);
			return !boolflag;
		});
		LOG_CF_D(L"挥击破剑结束,Tick=%d", static_cast<DWORD>(::GetTickCount64()));

	}
}

VOID CDailyTask_SnowHole::BossSkill_腾空_BladeMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	//LOG_CF_D(L"腾空");
	/*if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_F4))
	{
		TickBossSkill_.BeforeSleep(200);
		//LOG_CF_D(L"击剑");
		DWORD dwTimeOut = TickBossSkill_.GetBeforeSleepTime(2500);
		//LOG_CF_D(L"dwTimeOut=%d, Tick=%d", dwTimeOut, static_cast<DWORD>(::GetTickCount64() - TickBossSkill_.ulTick));
		CSomeFun::GetInstance().SimlationKey('F', 0);
		CSomeFun::GetInstance().TimeOut_Condiction_GameRun(dwTimeOut <= 1200 ? 1200 : dwTimeOut, []
		{
			CSomeFun::GetInstance().SimlationKey('F', 0);
			return !CPersonFunction::GetInstance().ExistBuff_Person(em_BladeMaster_火龙BUFF);
		});
	}
	else
	{  
		//LOG_CF_D(L"用1格挡");
		CSomeFun::GetInstance().SimlationKey(VK_1, 0);
		DWORD dwTimeOut = TickBossSkill_.GetBeforeSleepTime(3000);
		//LOG_CF_D(L"dwTimeOut=%d, Tick=%d", dwTimeOut, static_cast<DWORD>(::GetTickCount64() - TickBossSkill_.ulTick));
		CSomeFun::GetInstance().TimeOut_Condiction_GameRun(dwTimeOut <= 1000 ? 1000 : dwTimeOut, [DailyTaskSkill]
		{
			CSomeFun::GetInstance().SimlationKey(VK_1, 0);
			return !CFarmSkill::GetInstance().UseSkill_By_SkillId_WhichCool(BladeMaster_SkillId_F3);
		});
	}*/

	if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_F))
	{
		TickBossSkill_.BeforeSleep(300);
		//LOG_CF_D(L"击剑");
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_F, TRUE);
		//DWORD dwTimeOut = TickBossSkill_.GetBeforeSleepTime(2500);
		////LOG_CF_D(L"dwTimeOut=%d, Tick=%d", dwTimeOut, static_cast<DWORD>(::GetTickCount64() - TickBossSkill_.ulTick));
		LOG_CF_D(L"Tick=%d", static_cast<DWORD>(::GetTickCount64()));
		CSomeFun::GetInstance().TimeOut_Condiction_GameRun(1000, [&DailyTaskSkill]
		{
			LOG_CF_D(L"腾空击剑");
			DailyTaskSkill.RefreshVecSkill();
			auto boolflag = DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_T7);
			LOG_CF_D(L"腾空击剑-%d", boolflag);
			return !boolflag;
		});
		LOG_CF_D(L"腾空击剑结束,Tick=%d", static_cast<DWORD>(::GetTickCount64()));
	}
	else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_V))
	{
		//LOG_CF_D(L"一闪躲技能");
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_V, TRUE);
	}
	else
	{
		//LOG_CF_D(L"用1格挡");
		TickBossSkill_.BeforeSleep(500);
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_1, TRUE);
		//DWORD dwTimeOut = TickBossSkill_.GetBeforeSleepTime(3000);
		////LOG_CF_D(L"dwTimeOut=%d, Tick=%d", dwTimeOut, static_cast<DWORD>(::GetTickCount64() - TickBossSkill_.ulTick));
		//CSomeFun::GetInstance().TimeOut_Condiction_GameRun(dwTimeOut <= 1000 ? 1000 : dwTimeOut, [DailyTaskSkill]
		//{
		//	CSomeFun::GetInstance().SimlationKey(VK_1, 0);
		//	return !CFarmSkill::GetInstance().UseSkill_By_SkillId_WhichCool(BladeMaster_SkillId_F3);
		//});
		LOG_CF_D(L"Tick=%d", static_cast<DWORD>(::GetTickCount64()));
		CSomeFun::GetInstance().TimeOut_Condiction_GameRun(1000, [&DailyTaskSkill]
		{
			LOG_CF_D(L"腾空破剑");
			DailyTaskSkill.RefreshVecSkill();
			auto boolflag = DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_T1);
			LOG_CF_D(L"腾空破剑-%d", boolflag);
			return !boolflag;
		});
		LOG_CF_D(L"腾空破剑结束,Tick=%d", static_cast<DWORD>(::GetTickCount64()));
	}
}

VOID CDailyTask_SnowHole::BossSkill_击掌_BladeMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	//LOG_CF_D(L"击掌");
	/*if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_F4))
	{
		TickBossSkill_.BeforeSleep(500);

		//LOG_CF_D(L"击剑");
		DWORD dwTimeOut = TickBossSkill_.GetBeforeSleepTime(2500);
		//LOG_CF_D(L"dwTimeOut=%d, Tick=%d", dwTimeOut, static_cast<DWORD>(::GetTickCount64() - TickBossSkill_.ulTick));
		CSomeFun::GetInstance().TimeOut_Condiction_GameRun(dwTimeOut <= 1200 ? 1200 : dwTimeOut, []
		{
			CSomeFun::GetInstance().SimlationKey('F', 0);
			return !CPersonFunction::GetInstance().ExistBuff_Person(em_BladeMaster_火龙BUFF);
		});
	}
	else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_Q))
	{
		//LOG_CF_D(L"剑术姿态 Q无敌去躲");
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Q, TRUE);
	}
	else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_Tab_V))
	{
		//LOG_CF_D(L"一闪躲技能");
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Tab_V, TRUE);
	}
	else
	{
		//LOG_CF_D(L"用1格挡");
		CSomeFun::GetInstance().SimlationKey(VK_1, 0);
		DWORD dwTimeOut = TickBossSkill_.GetBeforeSleepTime(3000);
		//LOG_CF_D(L"dwTimeOut=%d, Tick=%d", dwTimeOut, static_cast<DWORD>(::GetTickCount64() - TickBossSkill_.ulTick));
		CSomeFun::GetInstance().TimeOut_Condiction_GameRun(dwTimeOut <= 1000 ? 1000 : dwTimeOut, [DailyTaskSkill]
		{
			CSomeFun::GetInstance().SimlationKey(VK_1, 0);
			return !CFarmSkill::GetInstance().UseSkill_By_SkillId_WhichCool(BladeMaster_SkillId_F3);
		});
	}*/

	if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_F))
	{
		//LOG_CF_D(L"击剑");
		TickBossSkill_.BeforeSleep(700);
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_F, TRUE);
		//DWORD dwTimeOut = TickBossSkill_.GetBeforeSleepTime(2500);
		////LOG_CF_D(L"dwTimeOut=%d, Tick=%d", dwTimeOut, static_cast<DWORD>(::GetTickCount64() - TickBossSkill_.ulTick));
		LOG_CF_D(L"Tick=%d", static_cast<DWORD>(::GetTickCount64()));
		CSomeFun::GetInstance().TimeOut_Condiction_GameRun(1000, [&DailyTaskSkill]
		{
			LOG_CF_D(L"击掌击剑");
			DailyTaskSkill.RefreshVecSkill();
			auto boolflag = DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_T7);
			LOG_CF_D(L"击掌击剑-%d", boolflag);
			return !boolflag;
		});
		LOG_CF_D(L"击掌击剑结束,Tick=%d", static_cast<DWORD>(::GetTickCount64()));
	}
	else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_V))
	{
		//LOG_CF_D(L"一闪躲技能");
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_V, TRUE);
	}
	else
	{
		//LOG_CF_D(L"用1格挡");
		TickBossSkill_.BeforeSleep(500);
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_1, TRUE);
		//DWORD dwTimeOut = TickBossSkill_.GetBeforeSleepTime(3000);
		////LOG_CF_D(L"dwTimeOut=%d, Tick=%d", dwTimeOut, static_cast<DWORD>(::GetTickCount64() - TickBossSkill_.ulTick));
		//CSomeFun::GetInstance().TimeOut_Condiction_GameRun(dwTimeOut <= 1000 ? 1000 : dwTimeOut, [DailyTaskSkill]
		//{
		//	CSomeFun::GetInstance().SimlationKey(VK_1, 0);
		//	return !CFarmSkill::GetInstance().UseSkill_By_SkillId_WhichCool(BladeMaster_SkillId_F3);
		//});
		LOG_CF_D(L"Tick=%d", static_cast<DWORD>(::GetTickCount64()));
		CSomeFun::GetInstance().TimeOut_Condiction_GameRun(1000, [&DailyTaskSkill]
		{
			LOG_CF_D(L"击掌破剑");
			DailyTaskSkill.RefreshVecSkill();
			auto boolflag = DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_T7);
			LOG_CF_D(L"击掌破剑-%d", boolflag);
			return !boolflag;
		});
		LOG_CF_D(L"击掌破剑结束,Tick=%d", static_cast<DWORD>(::GetTickCount64()));
	}
}

VOID CDailyTask_SnowHole::BossSkill_墩摔_BladeMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	//LOG_CF_D(L"墩摔!");
	TickBossSkill_.BeforeSleep(1500);
	DailyTaskSkill.RefreshVecSkill();
	if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_Q))
	{
		//LOG_CF_D(L"拔剑姿态 Q无敌去躲");
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_Q, TRUE);
	}
	else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_Q))
	{
		//LOG_CF_D(L"剑术姿态 Q无敌去躲");
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Q, TRUE);
	}
	else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_3))
	{
		//LOG_CF_D(L"五连斩无敌");
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_3, TRUE);
		BnsSleep(1000);
	}
	else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_E))
	{
		//LOG_CF_D(L"拔剑姿态 E无敌去躲");
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_E, TRUE);
	}
	else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_E))
	{
		//LOG_CF_D(L"剑术姿态 E无敌去躲");
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_E, TRUE);
	}
	else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_SS))
	{
		//LOG_CF_D(L"SS");
		CFarmSkill::GetInstance().UseSkill_By_SS();
	}
	//LOG_CF_D(L"躲避完毕!");
}

VOID CDailyTask_SnowHole::BossSkill_前扑_BladeMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	//LOG_CF_D(L"前扑!");
	TickBossSkill_.BeforeSleep(1500);
	if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_SS))
	{
		//LOG_CF_D(L"前扑之SS");
		CFarmSkill::GetInstance().UseSkill_By_SS(); 
		TickBossSkill_.BeforeSleep(4000);
	}
	//LOG_CF_D(L"躲避完毕!");
}

VOID CDailyTask_SnowHole::BossSkill_吸取热气_BladeMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	//LOG_CF_D(L"吸取热气!");
	CPersonFunction::GetInstance().Move(Boss.GetFrontPoint(2.0f));
	TickBossSkill_.BeforeSleep(1750);

	DailyTaskSkill.RefreshVecSkill();
	if (CPerson::GetInstance().GetFightPose() != em_Player_Pose::em_Player_Pose_BladeMaster)
	{
		//LOG_CF_D(L"剑术姿态 Q无敌去躲");
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Q, TRUE);
	}
	else
	{
		//LOG_CF_D(L"拔剑姿态 Q无敌去躲");
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_Q, TRUE);
	}
	//LOG_CF_D(L"躲避完毕!");
}

VOID CDailyTask_SnowHole::BossSkill_抢夺_BladeMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	//LOG_CF_D(L"抢夺!");
	CFarmSkill::GetInstance().UseSkill_By_SkillId(BladeMaster_SkillId_SS);

	// 必须靠近BOSS, 否则E就用不出来。。。。。
	//CPersonFunction::GetInstance().Move(Boss.GetBehindPoint(2.0f));
}

VOID CDailyTask_SnowHole::BossSkill_天来跃_BladeMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	LOG_CF_D(L"天来跃");
	//                     吸取热气                        天来跃                         抢夺                         热气注入
	if (_dwLastSkillId != em_Skill_灯笼雪人_吸取热气 && _dwLastSkillId != em_Skill_灯笼雪人_天来跃 && _dwLastSkillId != em_Skill_灯笼雪人_抢夺 && _dwLastSkillId != em_Skill_灯笼雪人_热气注入)
	{
		// 跳到中间的位置, 这个时候貌似可以不管, 自己会用2去靠近
		float fDis = CPerson::GetInstance().GetDis(_BossRange.FixPoint);
		if (fDis <= 5)
		{
			//LOG_CF_D(L"离中心太近了, 向后退!");
			CSomeFun::GetInstance().PushSimlationKey('S');
			TickBossSkill_.BeforeSleep(3000);
			CSomeFun::GetInstance().SimlationKey('S');
		}
		else if (fDis >= 16)
		{
			//LOG_CF_D(L"普通的天来跃 距离过远, 不等, 直接走过去!");
			TickBossSkill_.BeforeSleep(4000);
			return;
		}
		//LOG_CF_D(L"普通的天来跃 等一下再2过去!");
		TickBossSkill_.BeforeSleep(5000);
		return;
	}

	CSomeFun::GetInstance().PushSimlationKey('S');
	TickBossSkill_.BeforeSleep(2000);
	CSomeFun::GetInstance().SimlationKey('S');
	LOG_CF_D(L"3无敌去躲");
	TickBossSkill_.BeforeSleep(3000);
	if (CPerson::GetInstance().GetFightPose() != em_Player_Pose::em_Player_Pose_BladeMaster)
	{
		LOG_CF_D(L"剑术姿态 E无敌去躲");
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_E, TRUE);
	}
	else
	{
		LOG_CF_D(L"拔剑姿态 E无敌去躲");
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_E, TRUE);
	}
	

	TickBossSkill_.BeforeSleep(4000);
	if (CPerson::GetInstance().GetFightPose() == em_Player_Pose_BladeMaster)
	{
		//LOG_CF_D(L"按1切换成剑术姿态");
		CSomeFun::GetInstance().SimlationKey(VK_1);
	}
	if (Boss.GetDis() > 6.0f)
	{
		//LOG_CF_D(L"BOSS过远, 先靠近一点!");
		CFarmSkill::GetInstance().UseSkill_By_SkillId_WhichCool(BladeMaster_SkillId_2);
		CFarmSkill::GetInstance().UseSkill_By_SkillId_WhichCool(BladeMaster_SkillId_Gesture_2);
	}

	//LOG_CF_D(L"已经花费了%dms, 并且已经切换回剑术姿态", static_cast<DWORD>(::GetTickCount64() - TickBossSkill_.ulTick));
	TickBossSkill_.BeforeSleep(5000);
	//LOG_CF_D(L"已经花费了%dms, 准备打断!", static_cast<DWORD>(::GetTickCount64() - TickBossSkill_.ulTick));

	DailyTaskSkill.RefreshVecSkill();
	if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_C) && DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_Tab))
	{
		//LOG_CF_D(L"用虚弱打断天来跃");
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_C, TRUE);
		BnsSleep(500);
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Tab, TRUE);
	}
	else //if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_3) && DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_X))
	{
		// 最好别用倒地, 因为倒地会浪费时间, 而且回反击!
		//LOG_CF_D(L"用倒地打断天来跃");
		//DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_3, TRUE);
		//DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_X, TRUE);

		//LOG_CF_D(L"打断后用无敌, 不然要被误伤!");
		CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2 * 1000, []
		{
			CSomeFun::GetInstance().SimlationKey('R', 0);
			return TRUE;
		});
		BnsSleep(1000);
		DailyTaskSkill.RefreshVecSkill();
		if (DailyTaskSkill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Q))
		{
			//LOG_CF_D(L"剑术姿态 Q无敌去躲");
			DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Q, TRUE);
		}
		else if (DailyTaskSkill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_Q))
		{
			//LOG_CF_D(L"拔剑姿态 Q无敌去躲");
			DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_Q, TRUE);
		}
		else if (DailyTaskSkill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_E))
		{
			//LOG_CF_D(L"拔剑姿态 E无敌去躲");
			DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_E, TRUE);
		}
		else if (DailyTaskSkill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_E))
		{
			//LOG_CF_D(L"剑术姿态 E无敌去躲");
			DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_E, TRUE);
		}
		else if (DailyTaskSkill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_3))
		{
			//LOG_CF_D(L"五连斩无敌");
			DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_3, TRUE);
			BnsSleep(1000);
		}
		else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_SS))
		{
			//LOG_CF_D(L"SS");
			CFarmSkill::GetInstance().UseSkill_By_SS();
		}
	}
	//else
	//{
	//	LOG_CF_D(L"没有打断技能?");
	//	return;
	//}
	LOG_CF_D(L"躲避天来跃完毕!");
}

VOID CDailyTask_SnowHole::BossSkill_冻波撼地_BladeMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	//LOG_CF_D(L"冻波撼地");
	TickBossSkill_.BeforeSleep(2000);
	if (DailyTaskSkill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_3))
	{
		//LOG_CF_D(L"五连斩无敌");
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_3, TRUE);
		BnsSleep(1000);
	}
	else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_SS))
	{
		//LOG_CF_D(L"SS");
		CFarmSkill::GetInstance().UseSkill_By_SkillId(BladeMaster_SkillId_SS);
	}
	else
	{
		//LOG_CF_D(L"没技能打断 冻波撼地..... 不太可能啊!");
	}

	// 判断身上有没有热BUFF, 没有热BUFF的必须去BOSS的中心坐标去吸热!
	//LOG_CF_D(L"走去BOSS那边吸热BUFF");
	BnsMove(Boss.GetPoint(), 1);
}

VOID CDailyTask_SnowHole::Skill_BladeMaster(_In_ CONST CMonster& Boss, _In_ CONST CDailyTask_Skill& DailyTaskSkill)
{
	Boss.SetAngle();
	if (Boss.GetDis() > 7)
	{
		if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_2))
		{
			//LOG_CF_D(L"突进靠近BOSS");
			DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_2, FALSE);
			return;
		}
		else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_2))
		{
			//LOG_CF_D(L"突袭靠近BOSS");
			DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_2, FALSE);
			return;
		}
	}

	if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_1_R))
	{
		//LOG_CF_D(L"风火轮!");
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_1_R, FALSE);
		return;
	}

	DWORD dwMp = CPerson::GetInstance().GetMp();
	if (CPerson::GetInstance().GetFightPose() != em_Player_Pose::em_Player_Pose_BladeMaster && dwMp >= 8 && DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_4))
	{
		//LOG_CF_D(L"焦热斩转变成拔剑姿态");
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_4, FALSE);
		return;
	}


	static MyTools::CTimeTick TimeTickZ;
	if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_G))
	{
		//LOG_CF_D(L"以气御剑!");
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_G, FALSE);
		return;
	}
	else if (dwMp <= 5 && DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_2))
	{
		//LOG_CF_D(L"突进!");
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_2, FALSE);
		return;
	}
	else if (dwMp < 7 && TimeTickZ.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Second) >= 5 && DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_Z)) // Z 和 C都是回蓝技能, 所以不能一起放！ 放了一个, 另外一个至少要10s后才能放!
	{
		//LOG_CF_D(L"山崩地裂");
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_Z, FALSE);
		TimeTickZ.Reset();
		return;
	}
	else if (dwMp < 7 && TimeTickZ.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Second) >= 5 && DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_C))
	{
		//LOG_CF_D(L"风月斩");
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_C, FALSE);
		TimeTickZ.Reset();
		return;
	}
	else if (DailyTaskSkill.IsCool_By_KeyText_In_Ui(L"T"))
	{
		LOG_CF_D(L"无CD T 火龙连斩!");
		CSomeFun::GetInstance().SimlationKey('T', 10);
		return;
	}
	/*
	else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_T_T))
	{
		//LOG_CF_D(L"无CD T 火龙连斩!");
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_T_T, FALSE);
		return;
	}
	else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_T_T1))
	{
		//LOG_CF_D(L"无CD T 火龙连斩!");
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_T_T1, FALSE);
		return;
	}
	else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_T_TT))
	{
		//LOG_CF_D(L"T 火龙连斩!");
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_T_TT, FALSE);
		return;
	}
	else if(DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_T_TTT))
	{
		//LOG_CF_D(L"T 火龙连斩!");
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_T_TTT, FALSE);
		return;
	}
	else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_T_TTTT))
	{
		//LOG_CF_D(L"T 觉醒火龙连斩!");
		DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_T_TTT, FALSE);
		return;
	}*/


	// 必须准备好蓝去打断, 不能瞎用蓝!
	if (_dwLastSkillId == em_Skill_灯笼雪人_抢夺 || _dwLastSkillId == em_Skill_灯笼雪人_吸取热气)
	{
		DailyTaskSkill.UseSkill_By_SkillId_WhichCool(BladeMaster_SkillId_2);
		DailyTaskSkill.UseSkill_By_SkillId_WhichCool(BladeMaster_SkillId_Gesture_2);
		return;
	}

	CSomeFun::GetInstance().SimlationKey(CPerson::GetInstance().GetFightPose() != em_Player_Pose::em_Player_Pose_BladeMaster && dwMp == 10 ? 'T' : 'R', 0);
}

VOID CDailyTask_SnowHole::BossSkill_前扑_ForceMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	LOG_CF_D(L"前扑");
	if (Boss.GetTargetId() != CPerson::GetInstance().GetId())
		return;

	LOG_CF_D(L"BOSS 前扑的目标是气功……准备用躲技能!");
	TickBossSkill_.BeforeSleep(1500);
	if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_SS))
	{
		LOG_CF_D(L"前扑之SS");
		CFarmSkill::GetInstance().UseSkill_By_SS();
		TickBossSkill_.BeforeSleep(4000);
	}
	LOG_CF_D(L"躲避完毕!");
}

VOID CDailyTask_SnowHole::BossSkill_吸取热气_ForceMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	LOG_CF_D(L"吸取热气");
	TickBossSkill_.BeforeSleep(1750);

	DailyTaskSkill.RefreshVecSkill();
	
}

VOID CDailyTask_SnowHole::BossSkill_抢夺_ForceMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	LOG_CF_D(L"抢夺");
	if (Boss.GetTargetId() == CPerson::GetInstance().GetId())
	{
		LOG_CF_D(L"SS去躲抢夺");
		CFarmSkill::GetInstance().UseSkill_By_SS();
	}
}

VOID CDailyTask_SnowHole::BossSkill_天来跃_ForceMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	LOG_CF_D(L"天来跃.....不能放技能拉仇恨!");
	if (_dwLastSkillId != em_Skill_灯笼雪人_吸取热气 && _dwLastSkillId != em_Skill_灯笼雪人_天来跃 && _dwLastSkillId != em_Skill_灯笼雪人_抢夺 && _dwLastSkillId != em_Skill_灯笼雪人_热气注入)
	{
		TickBossSkill_.BeforeSleep(5 * 1000);
		LOG_CF_D(L"躲避完毕");
	}
	
}

VOID CDailyTask_SnowHole::BossSkill_冻波撼地_ForceMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	LOG_CF_D(L"冻波撼地");
	TickBossSkill_.BeforeSleep(2500);
	

	// 判断身上有没有热BUFF, 没有热BUFF的必须去BOSS的中心坐标去吸热!
	LOG_CF_D(L"走去BOSS那边吸热BUFF");
	BnsMove(Boss.GetPoint(), 1);
}

VOID CDailyTask_SnowHole::Skill_ForceMaster(_In_ CONST CMonster& Boss, _In_ CONST CDailyTask_Skill& DailyTaskSkill)
{
	DWORD dwMp = CPerson::GetInstance().GetMp();
	Boss.SetAngle();
	
	static MyTools::CTimeTick TimeTickIce(0);
	static MyTools::CTimeTick TimeTick4(0);
	
	
	if (TimeTick4.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Second) < 1)
	{
		return;
	}
	if (TimeTick4.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Second) < 7 && CPerson::GetInstance().IsSustainabilityByUseSkill())
	{
		return;
	}

	
	if (dwMp >= 2 && DailyTaskSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_4_4))
	{
		//LOG_CF_D(L"重力掌!");
		DailyTaskSkill.UseSkill_By_SkillId(ForceMaster_SkillId_4_4, FALSE);
		TimeTick4.Reset();
		return;
	}
	if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_Ice_F))
	{
		//LOG_CF_D(L"冰龙破!");
		DailyTaskSkill.UseSkill_By_SkillId(ForceMaster_SkillId_Ice_F, FALSE);
		return;
	}
	if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_Fure_F))
	{
		//LOG_CF_D(L"炎龙破!");
		DailyTaskSkill.UseSkill_By_SkillId(ForceMaster_SkillId_Fure_F, FALSE);
		return;
	}
	

	// Ice
	if (TimeTickIce.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Second) >= 36)
	{
		if (CPerson::GetInstance().GetFightPose() != em_Player_Pose_ForceMaster_Ice)
		{
			CSomeFun::GetInstance().SimlationKey(dwMp >= 1 ? 'T' : 'R', 0);
			return;
		}
		if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_Ice_F))
		{
			//LOG_CF_D(L"冰龙破!");
			DailyTaskSkill.UseSkill_By_SkillId(ForceMaster_SkillId_Ice_F, FALSE);
			return;
		}
		if (dwMp >= 2 && DailyTaskSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_Ice_C))
		{
			//LOG_CF_D(L"混元罩!");
			DailyTaskSkill.UseSkill_By_SkillId(ForceMaster_SkillId_Ice_C, FALSE);
			return;
		}
		if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_Ice_Z))
		{
			//LOG_CF_D(L"破冰!");
			DailyTaskSkill.UseSkill_By_SkillId(ForceMaster_SkillId_Ice_Z, FALSE);
			return;
		}
		if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_Ice_X))
		{
			//LOG_CF_D(L"冰河掌!");
			DailyTaskSkill.UseSkill_By_SkillId(ForceMaster_SkillId_Ice_X, FALSE);
			return;
		}

		LOG_CF_D(L"冰系技能已经用完了!");
		TimeTickIce.Reset();
	}

	// 判断BOSS身上的火种叠够5层就用1引爆

	if (dwMp >= 2 && DailyTaskSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_Fure_X))
	{
		//LOG_CF_D(L"火莲掌!");
		DailyTaskSkill.UseSkill_By_SkillId(ForceMaster_SkillId_Fure_X, FALSE);
		return;
	}
	if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_Fure_C))
	{
		//LOG_CF_D(L"莲花指!");
		DailyTaskSkill.UseSkill_By_SkillId(ForceMaster_SkillId_Fure_C, FALSE);
		return;
	}
	if (dwMp >= 2 && DailyTaskSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_Fure_V))
	{
		//LOG_CF_D(L"流星指!");
		DailyTaskSkill.UseSkill_By_SkillId(ForceMaster_SkillId_Fure_V, FALSE);
		return;
	}
	if (dwMp >= 2 && DailyTaskSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_2))
	{
		//LOG_CF_D(L"爆裂炎炮!");
		DailyTaskSkill.UseSkill_By_SkillId(ForceMaster_SkillId_2, FALSE);
		return;
	}

	if (DailyTaskSkill.IsCool_By_SkillId_In_HideUi(ForceMaster_SkillId_Fure_F))
	{
		//LOG_CF_D(L"用R引出炎龙破");
		CSomeFun::GetInstance().SimlationKey('R', 0);
		return;
	}
	if (dwMp >= 1 && DailyTaskSkill.IsCool_By_SkillId_In_HideUi(ForceMaster_SkillId_Ice_F))
	{
		//LOG_CF_D(L"用T引出冰龙破");
		CSomeFun::GetInstance().SimlationKey('T', 0);
		return;
	}

	CSomeFun::GetInstance().SimlationKey('R', 0);
}

VOID CDailyTask_SnowHole::BossSkill_挥击_KongFuMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	LOG_CF_D(L"挥击");
	if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_1))
	{
		TickBossSkill_.BeforeSleep(500);
		//LOG_CF_D(L"击剑");
		DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_1, TRUE);
		if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_T3))
		{
			DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_T3, TRUE);
		}
		//DWORD dwTimeOut = TickBossSkill_.GetBeforeSleepTime(2500);
		////LOG_CF_D(L"dwTimeOut=%d, Tick=%d", dwTimeOut, static_cast<DWORD>(::GetTickCount64() - TickBossSkill_.ulTick));
		//CSomeFun::GetInstance().TimeOut_Condiction_GameRun(dwTimeOut <= 1200 ? 1200 : dwTimeOut, []
		//{
		//	return CSomeFun::GetInstance().SimlationKey('F', 0);
		//});
	}
	LOG_CF_D(L"挥击结束");
}

VOID CDailyTask_SnowHole::BossSkill_腾空_KongFuMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	LOG_CF_D(L"腾空");
	if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_1))
	{
		TickBossSkill_.BeforeSleep(500);
		//LOG_CF_D(L"击剑");
		DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_1, TRUE);
		BnsSleep(500);
		if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_T3))
		{
			DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_T3, TRUE);
		}
		BnsSleep(500);
		if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_T3))
		{
			DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_T3, TRUE);
		}
	}
	LOG_CF_D(L"腾空结束");
}

VOID CDailyTask_SnowHole::BossSkill_击掌_KongFuMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	LOG_CF_D(L"击掌");
	if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_1))
	{
		TickBossSkill_.BeforeSleep(1000);
		//LOG_CF_D(L"击剑");
		DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_1, TRUE);
		BnsSleep(500);
		if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_T3))
		{
			DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_T3, TRUE);
		}
		BnsSleep(500);
		if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_T3))
		{
			DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_T3, TRUE);
		}
	}
	LOG_CF_D(L"击掌结束");
}

VOID CDailyTask_SnowHole::BossSkill_墩摔_KongFuMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	LOG_CF_D(L"墩摔!");
	TickBossSkill_.BeforeSleep(1500);
	DailyTaskSkill.RefreshVecSkill();
	if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_E))
	{
		//LOG_CF_D(L"E无敌去躲");
		DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_E, TRUE);
	}
	else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_Q))
	{
		//LOG_CF_D(L"Q无敌去躲");
		DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_Q, TRUE);
	}
	else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_C))
	{
		//LOG_CF_D(L"C无敌去躲");
		DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_C, TRUE);
		BnsSleep(1000);
	}
	else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_SS))
	{
		//LOG_CF_D(L"SS");
		CFarmSkill::GetInstance().UseSkill_By_SkillId(KongFuMaster_SkillId_SS);
	}
	LOG_CF_D(L"墩摔结束!");
}

VOID CDailyTask_SnowHole::BossSkill_前扑_KongFuMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	LOG_CF_D(L"前扑!");
	TickBossSkill_.BeforeSleep(1500);
	if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_SS))
	{
		LOG_CF_D(L"前扑之SS");
		CFarmSkill::GetInstance().UseSkill_By_SkillId(KongFuMaster_SkillId_SS);
		TickBossSkill_.BeforeSleep(4000);
	}
	LOG_CF_D(L"前扑结束!");
}

VOID CDailyTask_SnowHole::BossSkill_吸取热气_KongFuMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	LOG_CF_D(L"吸取热气!");
	CPersonFunction::GetInstance().Move(Boss.GetFrontPoint(2.0f));

	if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_C))
	{
		TickBossSkill_.BeforeSleep(1000);

		//LOG_CF_D(L"C无敌去躲");
		DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_C, TRUE);
		BnsSleep(1000);
	}
	else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_SS))
	{
		TickBossSkill_.BeforeSleep(1750);

		//LOG_CF_D(L"前扑之SS");
		CFarmSkill::GetInstance().UseSkill_By_SkillId(KongFuMaster_SkillId_SS);
	}
	TickBossSkill_.BeforeSleep(4000);
	LOG_CF_D(L"吸取热气结束!");
}

VOID CDailyTask_SnowHole::BossSkill_抢夺_KongFuMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	LOG_CF_D(L"抢夺!");
	CFarmSkill::GetInstance().UseSkill_By_SkillId(KongFuMaster_SkillId_SS);
	LOG_CF_D(L"抢夺结束!");
}

VOID CDailyTask_SnowHole::BossSkill_天来跃_KongFuMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	LOG_CF_D(L"天来跃!");
	//                     吸取热气                        天来跃                         抢夺                         热气注入
	if (_dwLastSkillId != em_Skill_灯笼雪人_吸取热气 && _dwLastSkillId != em_Skill_灯笼雪人_天来跃 && _dwLastSkillId != em_Skill_灯笼雪人_抢夺 && _dwLastSkillId != em_Skill_灯笼雪人_热气注入)
	{
		// 跳到中间的位置, 这个时候貌似可以不管, 自己会用2去靠近
		float fDis = CPerson::GetInstance().GetDis(_BossRange.FixPoint);
		if (fDis <= 5)
		{
			//LOG_CF_D(L"离中心太近了, 向后退!");
			CSomeFun::GetInstance().PushSimlationKey('S');
			TickBossSkill_.BeforeSleep(3000);
			CSomeFun::GetInstance().SimlationKey('S');
		}
		else if (fDis >= 16)
		{
			//LOG_CF_D(L"普通的天来跃 距离过远, 不等, 直接走过去!");
			TickBossSkill_.BeforeSleep(4000);
			return;
		}
		//LOG_CF_D(L"普通的天来跃 等一下再2过去!");
		TickBossSkill_.BeforeSleep(5000);
		return;
	}

	CSomeFun::GetInstance().PushSimlationKey('S');
	TickBossSkill_.BeforeSleep(2000);
	CSomeFun::GetInstance().SimlationKey('S');
	//LOG_CF_D(L"3无敌去躲");
	TickBossSkill_.BeforeSleep(3000);
	if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_E))
	{
		//LOG_CF_D(L"用虚弱打断天来跃");
		DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_E, TRUE);
	}


	TickBossSkill_.BeforeSleep(4000);

	if (Boss.GetDis() > 6.0f)
	{
		//LOG_CF_D(L"BOSS过远, 先靠近一点!");
		if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_Tab))
		{
			DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_Tab, TRUE);
		}
		else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_T))
		{
			DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_T, TRUE);
		}
	}

	//LOG_CF_D(L"已经花费了%dms, 并且已经切换回剑术姿态", static_cast<DWORD>(::GetTickCount64() - TickBossSkill_.ulTick));
	TickBossSkill_.BeforeSleep(5000);
	//LOG_CF_D(L"已经花费了%dms, 准备打断!", static_cast<DWORD>(::GetTickCount64() - TickBossSkill_.ulTick));

	DailyTaskSkill.RefreshVecSkill();
	if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_Z))
	{
		// 最好别用倒地, 因为倒地会浪费时间, 而且回反击!
		LOG_CF_D(L"打断天来跃");
		DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_Z, TRUE);
		//DailyTaskSkill.UseSkill_By_SkillId(, TRUE);

		//LOG_CF_D(L"打断后用无敌, 不然要被误伤!");
		CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2 * 1000, []
		{
			CSomeFun::GetInstance().SimlationKey('R', 0);
			return TRUE;
		});
		BnsSleep(1000);
		DailyTaskSkill.RefreshVecSkill();
		 if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_E))
		{
			//LOG_CF_D(L"E无敌去躲");
			DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_E, TRUE);
		}
		else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_Q))
		{
			//LOG_CF_D(L"Q无敌去躲");
			DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_Q, TRUE);
		}
		else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_C))
		{
			//LOG_CF_D(L"C无敌去躲");
			DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_C, TRUE);
			BnsSleep(1000);
		}
		else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_SS))
		{
			//LOG_CF_D(L"SS");
			CFarmSkill::GetInstance().UseSkill_By_SkillId(KongFuMaster_SkillId_SS);
		}
	}
	else
	{
		LOG_CF_D(L"没有打断技能?");
		return;
	}
	LOG_CF_D(L"天来跃结束!");
}

VOID CDailyTask_SnowHole::BossSkill_冻波撼地_KongFuMaster(_In_ CONST CMonster& Boss, _In_ CDailyTask_Skill& DailyTaskSkill, _In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	LOG_CF_D(L"冻波撼地!");
	TickBossSkill_.BeforeSleep(2000);
	if (DailyTaskSkill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_C))
	{
		//LOG_CF_D(L"拳舞无敌");
		DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_C, TRUE);
		BnsSleep(1000);
	}
	else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_SS))
	{
		//LOG_CF_D(L"SS");
		CFarmSkill::GetInstance().UseSkill_By_SkillId(KongFuMaster_SkillId_SS);
	}
	else
	{
		LOG_CF_D(L"没技能打断 冻波撼地..... 不太可能啊!");
	}

	// 判断身上有没有热BUFF, 没有热BUFF的必须去BOSS的中心坐标去吸热!
	//LOG_CF_D(L"走去BOSS那边吸热BUFF");
	BnsMove(Boss.GetPoint(), 1);
	LOG_CF_D(L"冻波撼地结束!");
}

VOID CDailyTask_SnowHole::Skill_KongFuMaster(_In_ CONST CMonster& Boss, _In_ CONST CDailyTask_Skill& DailyTaskSkill)
{
	Boss.SetAngle();
	if (Boss.GetDis() > 7)
	{
		if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_Tab))
		{
			//LOG_CF_D(L"突进靠近BOSS");
			DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_Tab, FALSE);
			return;
		}
		else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_T))
		{
			//LOG_CF_D(L"突袭靠近BOSS");
			DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_T, FALSE);
			return;
		}
	}

	if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_Tab1))
	{
		DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_Tab1, FALSE);
		return;
	}

	if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_X_1))
	{
		//LOG_CF_D(L"点穴!");
		DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_X_1, FALSE);
		return;
	}
	else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_X_2))
	{
		//LOG_CF_D(L"点穴2!");
		DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_X_2, FALSE);
		return;
	}
	else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_X_3))
	{
		//LOG_CF_D(L"点穴3!");
		DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_X_3, FALSE);
		return;
	}
	else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_X_4))
	{
		//LOG_CF_D(L"点穴4!");
		DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_X_4, FALSE);
		return;
	}



	
	if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_4_1))
	{
		//LOG_CF_D(L"风火轮!");
		DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_4_1, FALSE);
		return;
	}

	if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_X1))
	{
		//LOG_CF_D(L"通背拳!");
		DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_X1, FALSE);
		return;
	}
	else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_X2))
	{
		//LOG_CF_D(L"通背拳!");
		DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_X2, FALSE);
		return;
	}
	else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_X3))
	{
		//LOG_CF_D(L"通背拳!");
		DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_X3, FALSE);
		return;
	}
	else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_X4))
	{
		//LOG_CF_D(L"通背拳!");
		DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_X4, FALSE);
		return;
	}
	else if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_X5))
	{
		//LOG_CF_D(L"通背拳!");
		DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_X5, FALSE);
		return;
	}

	if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_T4))
	{
		//LOG_CF_D(L"烈焰脚!");
		DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_T4, FALSE);
		return;
	}

	DWORD dwMp = CPerson::GetInstance().GetMp();
	if (dwMp >= 5 && DailyTaskSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_V))
	{
		DailyTaskSkill.UseSkill_By_SkillId(KongFuMaster_SkillId_V, FALSE);
		return;
	}
	

	//if (CPerson::GetInstance().GetFightPose() != em_Player_Pose::em_Player_Pose_BladeMaster && dwMp >= 8 && DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_4))
	//{
	//	//LOG_CF_D(L"焦热斩转变成拔剑姿态");
	//	DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_4, FALSE);
	//	return;
	//}


	//static MyTools::CTimeTick TimeTickZ;
	//if (DailyTaskSkill.IsCool_By_SkillId_In_Ui(_SkillId_G))
	//{
	//	//LOG_CF_D(L"以气御剑!");
	//	DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_G, FALSE);
	//	return;
	//}
	
	//else if (dwMp < 7 && TimeTickZ.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Second) >= 5 && DailyTaskSkill.IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_Tab_Z)) // Z 和 C都是回蓝技能, 所以不能一起放！ 放了一个, 另外一个至少要10s后才能放!
	//{
	//	//LOG_CF_D(L"山崩地裂");
	//	DailyTaskSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Tab_Z, FALSE);
	//	TimeTickZ.Reset();
	//	return;
	//}
	
}

VOID CDailyTask_SnowHole::KillHideBoss(_In_ CONST CMonster& Boss)
{

}

VOID CDailyTask_SnowHole::NormalMove(_In_ CONST CMonster& Boss)
{
	Boss.SetAngle();
	switch (CPerson::GetInstance().GetClasses())
	{
		// 
	case em_Player_Classes::em_Player_Classes_Destroyer:
	case em_Player_Classes::em_Player_Classes_Assassin:
	case em_Player_Classes::em_Player_Classes_BladeMaster:
	case em_Player_Classes::em_Player_Classes_KongFuMaster:
	case em_Player_Classes::em_Player_Classes_Danncer:
		MeleeMove(Boss);
		break;
	default:
		FarRangeMove(Boss);
		break;
	}
}

VOID CDailyTask_SnowHole::MeleeMove(_In_ CONST CMonster& Boss)
{
	if (Boss.GetDis() > 7)
	{
		CPersonFunction::GetInstance().Move(Boss.GetFrontPoint(5));
	}
}

VOID CDailyTask_SnowHole::FarRangeMove(_In_ CONST CMonster& Boss)
{
	float fDis = CPerson::GetInstance().Get2DDis(Boss.GetPoint());
	if (fDis >= 10 + 4 && fDis < 16 + 4)
		return;
	
	std::vector<Point> VecPoint;
	CFarm::GetInstance().GetRoundPointList(FixPointInfo(Boss.GetPoint(), 12 + 4), 36, VecPoint);

	// 根据人物距离排序, 离人物越近, 越优先判断
	std::sort(VecPoint.begin(), VecPoint.end(), [](CONST Point& p1, CONST Point& p2) { return CPerson::GetInstance().GetDis(p1) < CPerson::GetInstance().GetDis(p2); });

	for (auto& itm : VecPoint)
	{
		if (CBaseObject::Get3DDis(_BossRange.FixPoint, itm) < 16 + 4)
		{
			CPersonFunction::GetInstance().Move(itm);
			return;
		}
	}

	LOG_CF_E(L"to far...");
}