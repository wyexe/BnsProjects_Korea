#include "stdafx.h"
#include "DailyTask_Bitan.h"
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
#include "Npc.h"
#include "NpcExtend.h"

#define _SELF L"DailyTask_Bitan.cpp"
CDailyTask_Bitan::CDailyTask_Bitan()
{
}

CDailyTask_Bitan::~CDailyTask_Bitan()
{
}

BOOL CDailyTask_Bitan::Run()
{
	auto& VecPoint = GetVecBiTanPoint();

	CONST std::vector<Dungeon_BiTan_Content> VecMethodPtr = 
	{
		{ em_Dungeon_Bitan_Step1 ,std::bind(&CDailyTask_Bitan::Step1, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step2_Left ,std::bind(&CDailyTask_Bitan::Step2_Left, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step2_Right ,std::bind(&CDailyTask_Bitan::Step2_Right, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step3_Left ,std::bind(&CDailyTask_Bitan::Step3_Left, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step3_Right ,std::bind(&CDailyTask_Bitan::Step3_Right, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step4_Left ,std::bind(&CDailyTask_Bitan::Step4_Left, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step4_Right ,std::bind(&CDailyTask_Bitan::Step4_Right, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step5_Left ,std::bind(&CDailyTask_Bitan::Step5_Left, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step5_Right ,std::bind(&CDailyTask_Bitan::Step5_Right, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step6_Left ,std::bind(&CDailyTask_Bitan::Step6_Left, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step6_Right ,std::bind(&CDailyTask_Bitan::Step6_Right, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step7_Left ,std::bind(&CDailyTask_Bitan::Step7_Left, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step7_Right ,std::bind(&CDailyTask_Bitan::Step7_Right, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step8_Left ,std::bind(&CDailyTask_Bitan::Step8_Left, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step8_Right ,std::bind(&CDailyTask_Bitan::Step8_Right, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step9_Left ,std::bind(&CDailyTask_Bitan::Step9_Left, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step9_Right ,std::bind(&CDailyTask_Bitan::Step9_Right, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step10_Left ,std::bind(&CDailyTask_Bitan::Step10_Left, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step10_Right ,std::bind(&CDailyTask_Bitan::Step10_Right, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step11 ,std::bind(&CDailyTask_Bitan::Step11, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step12 ,std::bind(&CDailyTask_Bitan::Step12, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step13 ,std::bind(&CDailyTask_Bitan::Step13, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step14 ,std::bind(&CDailyTask_Bitan::Step14, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step15 ,std::bind(&CDailyTask_Bitan::Step15, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step16 ,std::bind(&CDailyTask_Bitan::Step16, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step17 ,std::bind(&CDailyTask_Bitan::Step17, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step18 ,std::bind(&CDailyTask_Bitan::Step18, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step19 ,std::bind(&CDailyTask_Bitan::Step19, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step20 ,std::bind(&CDailyTask_Bitan::Step20, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step21 ,std::bind(&CDailyTask_Bitan::Step21, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step22 ,std::bind(&CDailyTask_Bitan::Step22, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step23 ,std::bind(&CDailyTask_Bitan::Step23, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step24 ,std::bind(&CDailyTask_Bitan::Step24, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step25 ,std::bind(&CDailyTask_Bitan::Step25, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step26 ,std::bind(&CDailyTask_Bitan::Step26, this, std::placeholders::_1) },
		{ em_Dungeon_Bitan_Step_End ,std::bind(&CDailyTask_Bitan::End, this, std::placeholders::_1) },
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

BOOL CDailyTask_Bitan::Initialize()
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
	if (DailyTask_Item.CheckDailyTaskInitializeItem())
	{
		return FALSE;
	}
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

CONST std::vector<CDailyTask_Bitan::BiTanPoint>& CDailyTask_Bitan::GetVecBiTanPoint() CONST _NOEXCEPT
{
	CONST static std::vector<BiTanPoint> VecPoint =
	{
		{ Point(-51050,-1880,14093), em_Dungeon_Bitan_Step1 },
		{ Point(-51055,-1577,14093), em_Dungeon_Bitan_Step1 },
		{ Point(-51063,-1082,14093), em_Dungeon_Bitan_Step1 },
		{ Point(-51069,-818,14093), em_Dungeon_Bitan_Step1 },
		{ Point(-51070,-665,14093), em_Dungeon_Bitan_Step1 },
		{ Point(-51071,-513,14093), em_Dungeon_Bitan_Step1 },
		{ Point(-51056,200,14064), em_Dungeon_Bitan_Step1 }, // 准备分组
		{ Point(-51178,-2217,14079), em_Dungeon_Bitan_Step2_Left },
		{ Point(-51175,-2062,14093), em_Dungeon_Bitan_Step2_Left },
		{ Point(-51168,-1697,14093), em_Dungeon_Bitan_Step2_Left },
		{ Point(-51138,-1301,14093), em_Dungeon_Bitan_Step2_Left },
		{ Point(-51107,-930,14093), em_Dungeon_Bitan_Step2_Left },
		{ Point(-51079,-578,14093), em_Dungeon_Bitan_Step2_Left },
		{ Point(-51049,-204,14084), em_Dungeon_Bitan_Step2_Left },
		{ Point(-51020,164,14067), em_Dungeon_Bitan_Step2_Left },
		{ Point(-51000,405,14067), em_Dungeon_Bitan_Step2_Left },
		{ Point(-51002,-234,14090), em_Dungeon_Bitan_Step2_Left },
		{ Point(-50939,-97,14071), em_Dungeon_Bitan_Step2_Left },
		{ Point(-50875,41,14071), em_Dungeon_Bitan_Step2_Left },
		{ Point(-50784,162,14067), em_Dungeon_Bitan_Step2_Left },
		{ Point(-50705,291,14067), em_Dungeon_Bitan_Step2_Left },
		{ Point(-50617,413,14067), em_Dungeon_Bitan_Step2_Left },
		{ Point(-50507,520,14067), em_Dungeon_Bitan_Step2_Left },
		{ Point(-50378,598,14067), em_Dungeon_Bitan_Step2_Left },
		{ Point(-50246,669,14067), em_Dungeon_Bitan_Step2_Left },
		{ Point(-50349,558,14067), em_Dungeon_Bitan_Step3_Left },
		{ Point(-50466,463,14067), em_Dungeon_Bitan_Step3_Left },
		{ Point(-50585,367,14067), em_Dungeon_Bitan_Step3_Left },
		{ Point(-50625,262,14067), em_Dungeon_Bitan_Step3_Left },
		{ Point(-50771,220,14067), em_Dungeon_Bitan_Step3_Left },
		{ Point(-50920,203,14067), em_Dungeon_Bitan_Step3_Left },
		{ Point(-51182,169,14067), em_Dungeon_Bitan_Step4_Left },
		{ Point(-51049,242,14067), em_Dungeon_Bitan_Step4_Left },
		{ Point(-50920,327,14067), em_Dungeon_Bitan_Step4_Left },
		{ Point(-50798,418,14067), em_Dungeon_Bitan_Step4_Left },
		{ Point(-50677,509,14067), em_Dungeon_Bitan_Step4_Left },
		{ Point(-50549,597,14067), em_Dungeon_Bitan_Step4_Left },
		{ Point(-50419,676,14067), em_Dungeon_Bitan_Step4_Left },
		{ Point(-50295,762,14067), em_Dungeon_Bitan_Step4_Left },
		{ Point(-50190,875,14067), em_Dungeon_Bitan_Step4_Left },
		{ Point(-50081,981,14067), em_Dungeon_Bitan_Step4_Left },
		{ Point(-49978,1092,14067), em_Dungeon_Bitan_Step4_Left },
		{ Point(-49875,1209,14076), em_Dungeon_Bitan_Step4_Left },
		{ Point(-49832,1353,14073), em_Dungeon_Bitan_Step4_Left },
		{ Point(-49881,1497,14072), em_Dungeon_Bitan_Step4_Left },
		{ Point(-49930,1644,14071), em_Dungeon_Bitan_Step4_Left },
		{ Point(-49972,1789,14076), em_Dungeon_Bitan_Step4_Left },
		{ Point(-50049,1923,14081), em_Dungeon_Bitan_Step4_Left },
		{ Point(-50108,2063,14077), em_Dungeon_Bitan_Step4_Left },
		{ Point(-50124,2214,14077), em_Dungeon_Bitan_Step4_Left },
		{ Point(-50126,2362,14107), em_Dungeon_Bitan_Step4_Left },
		{ Point(-50125,2511,14077), em_Dungeon_Bitan_Step4_Left },
		{ Point(-50114,2665,14078), em_Dungeon_Bitan_Step4_Left },
		{ Point(-50080,2813,14079), em_Dungeon_Bitan_Step4_Left },
		{ Point(-49990,2934,14079), em_Dungeon_Bitan_Step4_Left },
		{ Point(-49919,3070,14072), em_Dungeon_Bitan_Step4_Left },
		{ Point(-49891,3219,14085), em_Dungeon_Bitan_Step4_Left },
		{ Point(-49875,3370,14098), em_Dungeon_Bitan_Step4_Left },
		{ Point(-49864,3512,14150), em_Dungeon_Bitan_Step4_Left },
		{ Point(-49855,3648,14215), em_Dungeon_Bitan_Step4_Left },
		{ Point(-49847,3785,14282), em_Dungeon_Bitan_Step4_Left },
		{ Point(-49809,4588,14484), em_Dungeon_Bitan_Step5_Left },
		{ Point(-49790,4512,14476), em_Dungeon_Bitan_Step5_Left },
		{ Point(-49773,4663,14484), em_Dungeon_Bitan_Step5_Left },
		{ Point(-49749,4868,14466), em_Dungeon_Bitan_Step5_Left },
		{ Point(-49704,5013,14466), em_Dungeon_Bitan_Step5_Left },
		{ Point(-49511,5037,14466), em_Dungeon_Bitan_Step5_Left },
		{ Point(-49359,5039,14466), em_Dungeon_Bitan_Step5_Left },
		{ Point(-49102,5042,14466), em_Dungeon_Bitan_Step5_Left },
		{ Point(-49325,5085,14466), em_Dungeon_Bitan_Step6_Left },
		{ Point(-49171,5064,14466), em_Dungeon_Bitan_Step6_Left },
		{ Point(-48984,5041,14466), em_Dungeon_Bitan_Step6_Left },
		{ Point(-48711,5040,14466), em_Dungeon_Bitan_Step6_Left },
		{ Point(-48559,5044,14466), em_Dungeon_Bitan_Step6_Left },
		{ Point(-48373,5073,14466), em_Dungeon_Bitan_Step6_Left },
		{ Point(-48352,5226,14466), em_Dungeon_Bitan_Step6_Left },
		{ Point(-48304,5588,14474), em_Dungeon_Bitan_Step6_Left },
		{ Point(-48318,5820,14472), em_Dungeon_Bitan_Step6_Left },
		{ Point(-48315,6006,14476), em_Dungeon_Bitan_Step6_Left },
		{ Point(-48316,6378,14477), em_Dungeon_Bitan_Step6_Left },
		{ Point(-48314,6601,14472), em_Dungeon_Bitan_Step6_Left },
		{ Point(-48312,6842,14470), em_Dungeon_Bitan_Step6_Left },
		{ Point(-48312,7139,14475), em_Dungeon_Bitan_Step6_Left },
		{ Point(-48313,7294,14472), em_Dungeon_Bitan_Step6_Left },
		{ Point(-48315,7592,14497), em_Dungeon_Bitan_Step6_Left },
		{ Point(-48317,7890,14587), em_Dungeon_Bitan_Step6_Left },
		{ Point(-48318,8012,14676), em_Dungeon_Bitan_Step6_Left },
		{ Point(-48320,8259,14775), em_Dungeon_Bitan_Step6_Left },
		{ Point(-48266,8680,14874),em_Dungeon_Bitan_Step7_Left },
		{ Point(-48294,8837,14885),em_Dungeon_Bitan_Step7_Left },
		{ Point(-48309,8990,14870),em_Dungeon_Bitan_Step7_Left },
		{ Point(-48321,9142,14867),em_Dungeon_Bitan_Step7_Left },
		{ Point(-48334,9297,14867),em_Dungeon_Bitan_Step7_Left },
		{ Point(-48705,9287,14867),em_Dungeon_Bitan_Step8_Left },
		{ Point(-48859,9304,14867),em_Dungeon_Bitan_Step8_Left },
		{ Point(-49010,9322,14867),em_Dungeon_Bitan_Step8_Left },
		{ Point(-49379,9364,14867),em_Dungeon_Bitan_Step8_Left },
		{ Point(-49748,9412,14867),em_Dungeon_Bitan_Step8_Left },
		{ Point(-49782,9561,14867),em_Dungeon_Bitan_Step8_Left },
		{ Point(-49824,9855,14875),em_Dungeon_Bitan_Step8_Left },
		{ Point(-49843,10147,14875),em_Dungeon_Bitan_Step8_Left },
		{ Point(-49855,10420,14886),em_Dungeon_Bitan_Step8_Left },
		{ Point(-49856,10717,14875),em_Dungeon_Bitan_Step8_Left },
		{ Point(-49853,11015,14872),em_Dungeon_Bitan_Step8_Left },
		{ Point(-49851,11165,14874),em_Dungeon_Bitan_Step8_Left },
		{ Point(-49849,11465,14874),em_Dungeon_Bitan_Step8_Left },
		{ Point(-49847,11765,14896),em_Dungeon_Bitan_Step8_Left },
		{ Point(-49840,12063,14974),em_Dungeon_Bitan_Step8_Left },
		{ Point(-49841,12360,15123),em_Dungeon_Bitan_Step8_Left },
		{ Point(-49841,13091,15283),em_Dungeon_Bitan_Step9_Left },
		{ Point(-49806,13235,15263),em_Dungeon_Bitan_Step9_Left },
		{ Point(-49752,13462,15262),em_Dungeon_Bitan_Step9_Left },
		{ Point(-49608,13519,15262),em_Dungeon_Bitan_Step9_Left },
		{ Point(-49219,13621,15262),em_Dungeon_Bitan_Step9_Left },
		{ Point(-48857,13709,15262),em_Dungeon_Bitan_Step9_Left },
		{ Point(-48709,13760,15262),em_Dungeon_Bitan_Step9_Left },
		{ Point(-48666,13941,15262),em_Dungeon_Bitan_Step9_Left },
		{ Point(-48583,14304,15263),em_Dungeon_Bitan_Step9_Left },
		{ Point(-48507,14917,15263),em_Dungeon_Bitan_Step10_Left },
		{ Point(-48524,15101,15268),em_Dungeon_Bitan_Step10_Left },
		{ Point(-48536,15254,15263),em_Dungeon_Bitan_Step10_Left },
		{ Point(-48549,15416,15268),em_Dungeon_Bitan_Step10_Left },
		{ Point(-48421,14567,15263),em_Dungeon_Bitan_Step10_Left },
		{ Point(-48451,14719,15263),em_Dungeon_Bitan_Step10_Left },
		{ Point(-48522,15077,15270),em_Dungeon_Bitan_Step10_Left },
		{ Point(-48552,15226,15263),em_Dungeon_Bitan_Step10_Left },
		{ Point(-48599,15460,15268),em_Dungeon_Bitan_Step10_Left },
		{ Point(-48750,15493,15268),em_Dungeon_Bitan_Step10_Left },
		{ Point(-48935,15534,15268),em_Dungeon_Bitan_Step10_Left },
		{ Point(-49265,15590,15268),em_Dungeon_Bitan_Step10_Left },
		{ Point(-49707,15078,14276),em_Dungeon_Bitan_Step10_Left },
		{ Point(-49750,14825,14276),em_Dungeon_Bitan_Step10_Left },
		{ Point(-51178,-2217,14079),em_Dungeon_Bitan_Step2_Right },
		{ Point(-51175,-2062,14093),em_Dungeon_Bitan_Step2_Right },
		{ Point(-51168,-1697,14093),em_Dungeon_Bitan_Step2_Right },
		{ Point(-51138,-1301,14093),em_Dungeon_Bitan_Step2_Right },
		{ Point(-51107,-930,14093),em_Dungeon_Bitan_Step2_Right },
		{ Point(-51079,-578,14093),em_Dungeon_Bitan_Step2_Right },
		{ Point(-51049,-204,14084),em_Dungeon_Bitan_Step2_Right },
		{ Point(-51020,164,14067),em_Dungeon_Bitan_Step2_Right },
		{ Point(-51000,405,14067),em_Dungeon_Bitan_Step2_Right },
		{ Point(-51090,170,14067),em_Dungeon_Bitan_Step2_Right },
		{ Point(-51223,277,14067),em_Dungeon_Bitan_Step2_Right },
		{ Point(-51350,383,14067),em_Dungeon_Bitan_Step2_Right },
		{ Point(-51490,458,14067),em_Dungeon_Bitan_Step2_Right },
		{ Point(-51690,559,14067),em_Dungeon_Bitan_Step2_Right },
		{ Point(-51827,628,14067),em_Dungeon_Bitan_Step2_Right },
		{ Point(-52083,756,14067),em_Dungeon_Bitan_Step2_Right },
		{ Point(-51941,698,14067),em_Dungeon_Bitan_Step3_Right },
		{ Point(-51771,618,14067),em_Dungeon_Bitan_Step3_Right },
		{ Point(-51639,540,14067),em_Dungeon_Bitan_Step3_Right },
		{ Point(-51509,462,14067),em_Dungeon_Bitan_Step3_Right },
		{ Point(-51201,253,14067),em_Dungeon_Bitan_Step3_Right },
		{ Point(-51345,347,14067),em_Dungeon_Bitan_Step4_Right },
		{ Point(-51461,446,14067),em_Dungeon_Bitan_Step4_Right },
		{ Point(-51587,536,14067),em_Dungeon_Bitan_Step4_Right },
		{ Point(-51724,601,14067),em_Dungeon_Bitan_Step4_Right },
		{ Point(-51856,676,14067),em_Dungeon_Bitan_Step4_Right },
		{ Point(-51971,770,14100),em_Dungeon_Bitan_Step4_Right },
		{ Point(-52085,866,14067),em_Dungeon_Bitan_Step4_Right },
		{ Point(-52187,982,14067),em_Dungeon_Bitan_Step4_Right },
		{ Point(-52236,1125,14067),em_Dungeon_Bitan_Step4_Right },
		{ Point(-52264,1274,14076),em_Dungeon_Bitan_Step4_Right },
		{ Point(-52266,1429,14072),em_Dungeon_Bitan_Step4_Right },
		{ Point(-52240,1578,14071),em_Dungeon_Bitan_Step4_Right },
		{ Point(-52175,1715,14073),em_Dungeon_Bitan_Step4_Right },
		{ Point(-52089,1844,14078),em_Dungeon_Bitan_Step4_Right },
		{ Point(-52022,1979,14083),em_Dungeon_Bitan_Step4_Right },
		{ Point(-52001,2129,14078),em_Dungeon_Bitan_Step4_Right },
		{ Point(-51988,2284,14078),em_Dungeon_Bitan_Step4_Right },
		{ Point(-51983,2436,14076),em_Dungeon_Bitan_Step4_Right },
		{ Point(-51995,2587,14077),em_Dungeon_Bitan_Step4_Right },
		{ Point(-52023,2739,14078),em_Dungeon_Bitan_Step4_Right },
		{ Point(-52106,2864,14078),em_Dungeon_Bitan_Step4_Right },
		{ Point(-52192,2988,14071),em_Dungeon_Bitan_Step4_Right },
		{ Point(-52229,3138,14078),em_Dungeon_Bitan_Step4_Right },
		{ Point(-52248,3289,14091),em_Dungeon_Bitan_Step4_Right },
		{ Point(-52260,3441,14104),em_Dungeon_Bitan_Step4_Right },
		{ Point(-52268,3570,14179),em_Dungeon_Bitan_Step4_Right },
		{ Point(-52277,3707,14244),em_Dungeon_Bitan_Step4_Right },
		{ Point(-52285,3843,14312),em_Dungeon_Bitan_Step4_Right },
		{ Point(-52291,4509,14476),em_Dungeon_Bitan_Step5_Right },
		{ Point(-52303,4663,14484),em_Dungeon_Bitan_Step5_Right },
		{ Point(-52352,4837,14464),em_Dungeon_Bitan_Step5_Right },
		{ Point(-52492,4897,14464),em_Dungeon_Bitan_Step5_Right },
		{ Point(-52740,5030,14464),em_Dungeon_Bitan_Step5_Right },
		{ Point(-53085,5050,14464),em_Dungeon_Bitan_Step6_Right },
		{ Point(-53240,5055,14464),em_Dungeon_Bitan_Step6_Right },
		{ Point(-53389,5083,14464),em_Dungeon_Bitan_Step6_Right },
		{ Point(-53521,5165,14464),em_Dungeon_Bitan_Step6_Right },
		{ Point(-53610,5287,14464),em_Dungeon_Bitan_Step6_Right },
		{ Point(-53657,5431,14468),em_Dungeon_Bitan_Step6_Right },
		{ Point(-53665,5586,14474),em_Dungeon_Bitan_Step6_Right },
		{ Point(-53661,5738,14473),em_Dungeon_Bitan_Step6_Right },
		{ Point(-53662,5889,14472),em_Dungeon_Bitan_Step6_Right },
		{ Point(-53681,6042,14477),em_Dungeon_Bitan_Step6_Right },
		{ Point(-53696,6193,14483),em_Dungeon_Bitan_Step6_Right },
		{ Point(-53712,6344,14478),em_Dungeon_Bitan_Step6_Right },
		{ Point(-53711,6497,14474),em_Dungeon_Bitan_Step6_Right },
		{ Point(-53695,6650,14472),em_Dungeon_Bitan_Step6_Right },
		{ Point(-53695,6801,14470),em_Dungeon_Bitan_Step6_Right },
		{ Point(-53702,6952,14472),em_Dungeon_Bitan_Step6_Right },
		{ Point(-53710,7106,14474),em_Dungeon_Bitan_Step6_Right },
		{ Point(-53719,7257,14472),em_Dungeon_Bitan_Step6_Right },
		{ Point(-53728,7408,14481),em_Dungeon_Bitan_Step6_Right },
		{ Point(-53736,7561,14494),em_Dungeon_Bitan_Step6_Right },
		{ Point(-53741,7713,14507),em_Dungeon_Bitan_Step6_Right },
		{ Point(-53748,7839,14589),em_Dungeon_Bitan_Step6_Right },
		{ Point(-53754,7973,14658),em_Dungeon_Bitan_Step6_Right },
		{ Point(-53760,8109,14725),em_Dungeon_Bitan_Step6_Right },
		{ Point(-53765,8245,14796),em_Dungeon_Bitan_Step6_Right },
		{ Point(-53792,8876,14885),em_Dungeon_Bitan_Step7_Right },
		{ Point(-53747,8524,14869),em_Dungeon_Bitan_Step7_Right },
		{ Point(-53747,8676,14874),em_Dungeon_Bitan_Step7_Right },
		{ Point(-53739,8922,14885),em_Dungeon_Bitan_Step7_Right },
		{ Point(-53733,9105,14867),em_Dungeon_Bitan_Step7_Right },
		{ Point(-53728,9261,14867),em_Dungeon_Bitan_Step7_Right },
		{ Point(-53599,9342,14867),em_Dungeon_Bitan_Step7_Right },
		{ Point(-53281,9309,14867),em_Dungeon_Bitan_Step8_Right },
		{ Point(-53131,9345,14867),em_Dungeon_Bitan_Step8_Right },
		{ Point(-52779,9446,14867),em_Dungeon_Bitan_Step8_Right },
		{ Point(-52422,9632,14867),em_Dungeon_Bitan_Step8_Right },
		{ Point(-52290,9813,14876),em_Dungeon_Bitan_Step8_Right },
		{ Point(-52277,10073,14874),em_Dungeon_Bitan_Step8_Right },
		{ Point(-52277,10445,14887),em_Dungeon_Bitan_Step8_Right },
		{ Point(-52286,10817,14874),em_Dungeon_Bitan_Step8_Right },
		{ Point(-52293,11152,14874),em_Dungeon_Bitan_Step8_Right },
		{ Point(-52297,11305,14876),em_Dungeon_Bitan_Step8_Right },
		{ Point(-52304,11603,14882),em_Dungeon_Bitan_Step8_Right },
		{ Point(-52310,11920,14909),em_Dungeon_Bitan_Step8_Right },
		{ Point(-52314,12223,15052),em_Dungeon_Bitan_Step8_Right },
		{ Point(-52317,12437,15164),em_Dungeon_Bitan_Step8_Right },
		{ Point(-52319,13030,15281),em_Dungeon_Bitan_Step9_Right },
		{ Point(-52318,13185,15274),em_Dungeon_Bitan_Step9_Right },
		{ Point(-52319,13337,15262),em_Dungeon_Bitan_Step9_Right },
		{ Point(-52465,13373,15262),em_Dungeon_Bitan_Step9_Right },
		{ Point(-52733,13405,15262),em_Dungeon_Bitan_Step9_Right },
		{ Point(-52989,13436,15262),em_Dungeon_Bitan_Step9_Right },
		{ Point(-53350,13507,15262),em_Dungeon_Bitan_Step9_Right },
		{ Point(-53500,13572,15262),em_Dungeon_Bitan_Step9_Right },
		{ Point(-53531,13724,15262),em_Dungeon_Bitan_Step9_Right },
		{ Point(-53549,14123,15265),em_Dungeon_Bitan_Step9_Right },
		{ Point(-53507,14803,15265),em_Dungeon_Bitan_Step10_Right },
		{ Point(-53514,14946,15312),em_Dungeon_Bitan_Step10_Right },
		{ Point(-53528,15097,15286),em_Dungeon_Bitan_Step10_Right },
		{ Point(-53548,15304,15265),em_Dungeon_Bitan_Step10_Right },
		{ Point(-53557,15534,15268),em_Dungeon_Bitan_Step10_Right },
		{ Point(-53408,15577,15268),em_Dungeon_Bitan_Step10_Right },
		{ Point(-53071,15578,15268),em_Dungeon_Bitan_Step10_Right },
		{ Point(-52917,15594,15268),em_Dungeon_Bitan_Step10_Right },
		{ Point(-52644,15088,14276),em_Dungeon_Bitan_Step10_Right },
		{ Point(-52415,14824,14276),em_Dungeon_Bitan_Step10_Right },
		{ Point(-51153,15663,14263),em_Dungeon_Bitan_Step11 },
		{ Point(-51073,16084,14263),em_Dungeon_Bitan_Step11 },
		{ Point(-50919,16089,14263),em_Dungeon_Bitan_Step11 },
		{ Point(-50546,16101,14280),em_Dungeon_Bitan_Step11 },
		{ Point(-50177,16144,14276),em_Dungeon_Bitan_Step11 },
		{ Point(-49973,16205,14276),em_Dungeon_Bitan_Step11 },
		{ Point(-50041,16343,14276),em_Dungeon_Bitan_Step11 },
		{ Point(-50069,16495,14276),em_Dungeon_Bitan_Step11 },
		{ Point(-50075,16868,14313),em_Dungeon_Bitan_Step11 },
		{ Point(-50066,17239,14275),em_Dungeon_Bitan_Step11 },
		{ Point(-50039,17610,14275),em_Dungeon_Bitan_Step11 },
		{ Point(-50031,17957,14275),em_Dungeon_Bitan_Step11 },
		{ Point(-50175,18009,14275),em_Dungeon_Bitan_Step11 },
		{ Point(-50522,18138,14275),em_Dungeon_Bitan_Step11 },
		{ Point(-50864,18285,14275),em_Dungeon_Bitan_Step11 },
		{ Point(-51080,18378,14275),em_Dungeon_Bitan_Step11 },
		{ Point(-51070,18531,14275),em_Dungeon_Bitan_Step11 },
		{ Point(-51035,18907,14275),em_Dungeon_Bitan_Step11 },
		{ Point(-51020,19074,14281),em_Dungeon_Bitan_Step11 },
		{ Point(-51008,19229,14284),em_Dungeon_Bitan_Step11 },
		{ Point(-51015,19380,14284),em_Dungeon_Bitan_Step11 },
		{ Point(-51040,19530,14283),em_Dungeon_Bitan_Step11 },
		{ Point(-51105,19896,14258),em_Dungeon_Bitan_Step11 },
		{ Point(-51136,20045,14235),em_Dungeon_Bitan_Step11 },
		{ Point(-51222,20401,14147),em_Dungeon_Bitan_Step11 },
		{ Point(-51328,20768,14021),em_Dungeon_Bitan_Step11 },
		{ Point(-51496,21098,13934),em_Dungeon_Bitan_Step11 },
		{ Point(-51764,21333,13886),em_Dungeon_Bitan_Step11 },
		{ Point(-52122,21453,13892),em_Dungeon_Bitan_Step11 },
		{ Point(-52492,21500,13890),em_Dungeon_Bitan_Step11 },
		{ Point(-52860,21546,13858),em_Dungeon_Bitan_Step11 },
		{ Point(-53229,21590,13856),em_Dungeon_Bitan_Step11 },
		{ Point(-53599,21629,13852),em_Dungeon_Bitan_Step11 },
		{ Point(-53949,21657,13851),em_Dungeon_Bitan_Step11 },
		{ Point(-54334,21687,13856),em_Dungeon_Bitan_Step11 },
		{ Point(-54704,21717,13884),em_Dungeon_Bitan_Step11 },
		{ Point(-54856,21721,13889),em_Dungeon_Bitan_Step11 },
		{ Point(-55222,21739,13893),em_Dungeon_Bitan_Step11 },
		{ Point(-55340,21612,13893),em_Dungeon_Bitan_Step12 },
		{ Point(-55852,21513,13891),em_Dungeon_Bitan_Step12 },
		{ Point(-56244,21707,13130),em_Dungeon_Bitan_Step12 },
		{ Point(-55802,21217,12180),em_Dungeon_Bitan_Step12 },
		{ Point(-56246,20477,12181),em_Dungeon_Bitan_Step12 },
		{ Point(-56453,19980,11180),em_Dungeon_Bitan_Step12 },
		{ Point(-55905,20020,11181),em_Dungeon_Bitan_Step13 },
		{ Point(-55751,20019,11181),em_Dungeon_Bitan_Step13 },
		{ Point(-55522,20017,11180),em_Dungeon_Bitan_Step13 },
		{ Point(-55368,20015,11180),em_Dungeon_Bitan_Step13 },
		{ Point(-54995,20017,11180),em_Dungeon_Bitan_Step13 },
		{ Point(-54701,20017,11180),em_Dungeon_Bitan_Step13 },
		{ Point(-54329,20013,11179),em_Dungeon_Bitan_Step13 },
		{ Point(-53957,20013,11179),em_Dungeon_Bitan_Step13 },
		{ Point(-53805,20014,11179),em_Dungeon_Bitan_Step13 },
		{ Point(-53463,20014,11179),em_Dungeon_Bitan_Step13 },
		{ Point(-53309,20014,11179),em_Dungeon_Bitan_Step13 },
		{ Point(-52939,20013,11179),em_Dungeon_Bitan_Step13 },
		{ Point(-52794,20064,11179),em_Dungeon_Bitan_Step13 },
		{ Point(-52642,20061,11179),em_Dungeon_Bitan_Step13 },
		{ Point(-52270,20051,11179),em_Dungeon_Bitan_Step13 },
		{ Point(-51898,20041,11179),em_Dungeon_Bitan_Step13 },
		{ Point(-51746,20036,11179),em_Dungeon_Bitan_Step14 },
		{ Point(-51593,20029,11179),em_Dungeon_Bitan_Step14 },
		{ Point(-51202,20014,11179),em_Dungeon_Bitan_Step14 },
		{ Point(-51052,20008,11179),em_Dungeon_Bitan_Step14 },
		{ Point(-50898,20005,11179),em_Dungeon_Bitan_Step15 },
		{ Point(-50652,20007,11179),em_Dungeon_Bitan_Step15 },
		{ Point(-50498,20005,11179),em_Dungeon_Bitan_Step15 },
		{ Point(-50121,20003,11179),em_Dungeon_Bitan_Step15 },
		{ Point(-49749,20010,11179),em_Dungeon_Bitan_Step15 },
		{ Point(-49377,20014,11179),em_Dungeon_Bitan_Step15 },
		{ Point(-49157,19976,11179),em_Dungeon_Bitan_Step15 },
		{ Point(-49083,19841,11179),em_Dungeon_Bitan_Step15 },
		{ Point(-48905,19520,11179),em_Dungeon_Bitan_Step15 },
		{ Point(-48747,19184,11180),em_Dungeon_Bitan_Step15 },
		{ Point(-48549,18871,11180),em_Dungeon_Bitan_Step15 },
		{ Point(-48312,18723,11180),em_Dungeon_Bitan_Step15 },
		{ Point(-48159,18722,11203),em_Dungeon_Bitan_Step15 },
		{ Point(-47793,18710,11184),em_Dungeon_Bitan_Step15 },
		{ Point(-47420,18698,11183),em_Dungeon_Bitan_Step15 },
		{ Point(-47270,18722,11183),em_Dungeon_Bitan_Step15 },
		{ Point(-46851,18724,11179),em_Dungeon_Bitan_Step15 },
		{ Point(-46528,18732,11179),em_Dungeon_Bitan_Step15 },
		{ Point(-46393,18664,11179),em_Dungeon_Bitan_Step16 },
		{ Point(-46027,18678,11179),em_Dungeon_Bitan_Step16 },
		{ Point(-45874,18683,11179),em_Dungeon_Bitan_Step16 },
		{ Point(-45723,18688,11179),em_Dungeon_Bitan_Step16 },
		{ Point(-45572,18698,11179),em_Dungeon_Bitan_Step16 },
		{ Point(-45511,18702,11179),em_Dungeon_Bitan_Step16 },
		{ Point(-45419,18705,11179),em_Dungeon_Bitan_Step17 },
		{ Point(-45106,18714,11179),em_Dungeon_Bitan_Step17 },
		{ Point(-44901,18721,11185),em_Dungeon_Bitan_Step17 },
		{ Point(-44530,18734,11186),em_Dungeon_Bitan_Step17 },
		{ Point(-44489,18879,11186),em_Dungeon_Bitan_Step17 },
		{ Point(-44456,19180,11185),em_Dungeon_Bitan_Step17 },
		{ Point(-44454,19333,11185),em_Dungeon_Bitan_Step17 },
		{ Point(-44448,19711,11181),em_Dungeon_Bitan_Step17 },
		{ Point(-44441,20083,11181),em_Dungeon_Bitan_Step17 },
		{ Point(-44437,20363,11181),em_Dungeon_Bitan_Step17 },
		{ Point(-44433,20567,11181),em_Dungeon_Bitan_Step17 },
		{ Point(-44427,20939,11184),em_Dungeon_Bitan_Step17 },
		{ Point(-44869,21543,11184),em_Dungeon_Bitan_Step18 },
		{ Point(-45023,21543,11184),em_Dungeon_Bitan_Step18 },
		{ Point(-45173,21552,11184),em_Dungeon_Bitan_Step18 },
		{ Point(-45325,21558,11184),em_Dungeon_Bitan_Step18 },
		{ Point(-45476,21572,11183),em_Dungeon_Bitan_Step18 },
		{ Point(-45851,21601,11183),em_Dungeon_Bitan_Step19 },
		{ Point(-46004,21609,11183),em_Dungeon_Bitan_Step19 },
		{ Point(-46371,21603,11183),em_Dungeon_Bitan_Step19 },
		{ Point(-46757,21599,11191),em_Dungeon_Bitan_Step19 },
		{ Point(-47129,21601,11186),em_Dungeon_Bitan_Step19 },
		{ Point(-47265,21602,11186),em_Dungeon_Bitan_Step19 },
		{ Point(-47029,23577,13132),em_Dungeon_Bitan_Step20 },
		{ Point(-47018,23958,13132),em_Dungeon_Bitan_Step20 },
		{ Point(-46990,25065,13134),em_Dungeon_Bitan_Step20 },
		{ Point(-47143,26115,12119),em_Dungeon_Bitan_Step21 },
		{ Point(-47187,26260,12119),em_Dungeon_Bitan_Step21 },
		{ Point(-47136,26401,12119),em_Dungeon_Bitan_Step21 },
		{ Point(-47084,26609,12119),em_Dungeon_Bitan_Step21 },
		{ Point(-47043,26761,12119),em_Dungeon_Bitan_Step21 },
		{ Point(-47025,26826,12119),em_Dungeon_Bitan_Step21 },
		{ Point(-46970,28091,13133),em_Dungeon_Bitan_Step22 },
		{ Point(-46982,28243,13133),em_Dungeon_Bitan_Step22 },
		{ Point(-47020,28602,13133),em_Dungeon_Bitan_Step22 },
		{ Point(-47129,28980,13133),em_Dungeon_Bitan_Step22 },
		{ Point(-47382,29248,13133),em_Dungeon_Bitan_Step22 },
		{ Point(-47739,29276,13134),em_Dungeon_Bitan_Step22 },
		{ Point(-48111,29290,13134),em_Dungeon_Bitan_Step22 },
		{ Point(-48264,29298,13134),em_Dungeon_Bitan_Step22 },
		{ Point(-48631,29317,13260),em_Dungeon_Bitan_Step22 },
		{ Point(-49003,29335,13444),em_Dungeon_Bitan_Step22 },
		{ Point(-49375,29356,13628),em_Dungeon_Bitan_Step22 },
		{ Point(-49747,29380,13812),em_Dungeon_Bitan_Step22 },
		{ Point(-50102,29396,13885),em_Dungeon_Bitan_Step22 },
		{ Point(-50369,29401,13885),em_Dungeon_Bitan_Step22 },
		{ Point(-47003,27383,13130),em_Dungeon_Bitan_Step23 },
		{ Point(-46987,27536,13134),em_Dungeon_Bitan_Step23 },
		{ Point(-46987,27688,13141),em_Dungeon_Bitan_Step23 },
		{ Point(-47000,28048,13133),em_Dungeon_Bitan_Step23 },
		{ Point(-47006,28201,13133),em_Dungeon_Bitan_Step23 },
		{ Point(-47056,28609,13133),em_Dungeon_Bitan_Step23 },
		{ Point(-47085,28775,13133),em_Dungeon_Bitan_Step23 },
		{ Point(-47238,29111,13133),em_Dungeon_Bitan_Step23 },
		{ Point(-47401,29173,13133),em_Dungeon_Bitan_Step23 },
		{ Point(-47772,29202,13134),em_Dungeon_Bitan_Step23 },
		{ Point(-48108,29210,13134),em_Dungeon_Bitan_Step23 },
		{ Point(-48262,29224,13134),em_Dungeon_Bitan_Step23 },
		{ Point(-48627,29258,13258),em_Dungeon_Bitan_Step23 },
		{ Point(-48998,29238,13442),em_Dungeon_Bitan_Step23 },
		{ Point(-49369,29224,13625),em_Dungeon_Bitan_Step23 },
		{ Point(-49741,29216,13809),em_Dungeon_Bitan_Step23 },
		{ Point(-50098,29235,13885),em_Dungeon_Bitan_Step23 },
		{ Point(-50248,29265,13885),em_Dungeon_Bitan_Step23 },
		{ Point(-50398,29285,13885),em_Dungeon_Bitan_Step23 },
		{ Point(-50773,29299,13895),em_Dungeon_Bitan_Step23 },
		{ Point(-51145,29300,13901),em_Dungeon_Bitan_Step23 },
		{ Point(-51517,29301,13902),em_Dungeon_Bitan_Step23 },
		{ Point(-51594,29167,13903),em_Dungeon_Bitan_Step24 },
		{ Point(-51700,29057,13903),em_Dungeon_Bitan_Step24 },
		{ Point(-51774,28806,13903),em_Dungeon_Bitan_Step24 },
		{ Point(-51817,28658,13904),em_Dungeon_Bitan_Step24 },
		{ Point(-51899,28291,13905),em_Dungeon_Bitan_Step24 },
		{ Point(-51925,28046,13902),em_Dungeon_Bitan_Step24 },
		{ Point(-52061,27982,13900),em_Dungeon_Bitan_Step24 },
		{ Point(-52295,27967,13852),em_Dungeon_Bitan_Step24 },
		{ Point(-52422,27961,13773),em_Dungeon_Bitan_Step24 },
		{ Point(-52794,27944,13541),em_Dungeon_Bitan_Step24 },
		{ Point(-53108,27931,13533),em_Dungeon_Bitan_Step24 },
		{ Point(-53261,27938,13534),em_Dungeon_Bitan_Step24 },
		{ Point(-53630,27953,13534),em_Dungeon_Bitan_Step24 },
		{ Point(-53782,27960,13534),em_Dungeon_Bitan_Step24 },
		{ Point(-53698,28085,13534),em_Dungeon_Bitan_Step25 },
		{ Point(-53759,28259,13534),em_Dungeon_Bitan_Step25 },
		{ Point(-53849,28480,13534),em_Dungeon_Bitan_Step25 },
		{ Point(-53906,28621,13534),em_Dungeon_Bitan_Step25 },
		{ Point(-53961,28763,13534),em_Dungeon_Bitan_Step25 },
		{ Point(-54023,29021,13527),em_Dungeon_Bitan_Step25 },
		{ Point(-54052,29245,13564),em_Dungeon_Bitan_Step25 },
		{ Point(-54093,29614,13795),em_Dungeon_Bitan_Step25 },
		{ Point(-54082,29961,13900),em_Dungeon_Bitan_Step25 },
		{ Point(-54076,30155,13901),em_Dungeon_Bitan_Step25 },
		{ Point(-54064,30527,13901),em_Dungeon_Bitan_Step25 },
		{ Point(-54051,30898,13922),em_Dungeon_Bitan_Step25 },
		{ Point(-54040,31234,14031),em_Dungeon_Bitan_Step25 },
		{ Point(-54031,31600,14031),em_Dungeon_Bitan_Step25 },
		{ Point(-54023,31972,14031),em_Dungeon_Bitan_Step25 },
		{ Point(-54016,32353,14031),em_Dungeon_Bitan_Step25 },
		{ Point(-54010,32719,13979),em_Dungeon_Bitan_Step25 },
		{ Point(-54007,32872,13979),em_Dungeon_Bitan_Step25 },
		{ Point(-54007,33185,13979),em_Dungeon_Bitan_Step25 },
		{ Point(-53990,33736,13996),em_Dungeon_Bitan_Step25 },
		{ Point(-54026,34301,14022),em_Dungeon_Bitan_Step25 },
		{ Point(-54030,34596,14012),em_Dungeon_Bitan_Step25 },
		{ Point(-54093,34921,14012),em_Dungeon_Bitan_Step25 },
		{ Point(-54512,34868,13045),em_Dungeon_Bitan_Step25 },
		{ Point(-54377,35182,13045),em_Dungeon_Bitan_Step26 },
		{ Point(-54293,35442,13045),em_Dungeon_Bitan_Step26 },
		{ Point(-54206,35715,13045),em_Dungeon_Bitan_Step26 },
	};

	return VecPoint;
}

BOOL CDailyTask_Bitan::Step1(_In_ CONST std::vector<Point>& VecPoint)
{
	// 
	_DailyTask_Team.ClumpTeamMember(1, L"第一波怪", VecPoint.front());

	CNpcExtend::GetInstance().DoAction_By_ExistNpc_For_Name(Npc_洛猴头, CBaseObject::CreateFixPointInfo(10), [](CONST CNpc& Npc) 
	{
		BnsMove(Npc.GetPoint(), 3);
		BnsSleep(2 * 1000);

		CTaskExtend::GetInstance().PickBLTask_By_Npc(SumitBLTask_Src(0x7C, 0x45B7D, 0x12C, 0, 1), Npc_洛猴头);
		CTaskExtend::GetInstance().PickBLTask_By_Npc(SumitBLTask_Src(0x7C, 0x45B7D, 0x12C, 0, 0), Npc_洛猴头);
	});

	_DailyTask_Move.FarmAndMove(VecPoint, 16);
	BnsSleep(3 * 1000);
	CFarm::GetInstance().ClearAroundMonster(FixPointInfo(VecPoint.back(), 16), [] { return FALSE; });
	BnsMove(VecPoint.back(), 3);
	return TRUE;
}

BOOL CDailyTask_Bitan::Step2_Left(_In_ CONST std::vector<Point>& VecPoint)
{
	if (!IsCorrectDirection(em_Bitan_Team_Direction::Direction_Left))
		return TRUE;

	MoveThenKill(VecPoint);
	return TRUE;
}

BOOL CDailyTask_Bitan::Step2_Right(_In_ CONST std::vector<Point>& VecPoint)
{
	if (!IsCorrectDirection(em_Bitan_Team_Direction::Direction_Right))
		return TRUE;

	MoveThenKill(VecPoint);
	return TRUE;
}

BOOL CDailyTask_Bitan::Step3_Left(_In_ CONST std::vector<Point>& VecPoint)
{
	if (!IsCorrectDirection(em_Bitan_Team_Direction::Direction_Left))
		return TRUE;


	MoveThenKill(VecPoint);
	BnsMove(VecPoint.back(), 3);
	CFarm::GetInstance().TakeCorpse(CBaseObject::CreateFixPointInfo(16), Corpse_雪玉宫通路装置);
	_DailyTask_Team.ClumpTeamMember(2, L"第一扇门", VecPoint.back());
	return TRUE;
}

BOOL CDailyTask_Bitan::Step3_Right(_In_ CONST std::vector<Point>& VecPoint)
{
	if (!IsCorrectDirection(em_Bitan_Team_Direction::Direction_Right))
		return TRUE;


	MoveThenKill(VecPoint);
	BnsMove(VecPoint.back(), 3);
	CFarm::GetInstance().TakeCorpse(CBaseObject::CreateFixPointInfo(16), Corpse_雪玉宫通路装置);
	_DailyTask_Team.ClumpTeamMember(2, L"第一扇门", VecPoint.back());
	return TRUE;
}

BOOL CDailyTask_Bitan::Step4_Left(_In_ CONST std::vector<Point>& VecPoint)
{
	if (!IsCorrectDirection(em_Bitan_Team_Direction::Direction_Left))
		return TRUE;


	_DailyTask_Move.MoveToVecPoint(VecPoint, [](CONST Point&) { return FALSE; });
	CFarm::GetInstance().ClearAroundMonster(CBaseObject::CreateFixPointInfo(20), [] {return FALSE; });


	BnsMove(VecPoint.back(), 3);
	BnsSleep(3 * 1000);
	CFarm::GetInstance().ClearAroundMonster(CBaseObject::CreateFixPointInfo(20), [] {return FALSE; });
	CResAction::GetInstance().TakeDropItem(CBaseObject::CreateFixPointInfo(16));

	BnsMove(VecPoint.back(), 3);
	CFarm::GetInstance().TakeCorpse(CBaseObject::CreateFixPointInfo(16), Corpse_雪玉宫通路装置);


	_DailyTask_Team.ClumpTeamMember(3, L"第二扇门", VecPoint.back());
	return TRUE;
}

BOOL CDailyTask_Bitan::Step4_Right(_In_ CONST std::vector<Point>& VecPoint)
{
	if (!IsCorrectDirection(em_Bitan_Team_Direction::Direction_Right))
		return TRUE;


	_DailyTask_Move.MoveToVecPoint(VecPoint, [](CONST Point&) { return FALSE; });
	CFarm::GetInstance().ClearAroundMonster(CBaseObject::CreateFixPointInfo(20), [] {return FALSE; });

	CONST Point GatherPoint(-52294, 3921, 14326);
	BnsMove(GatherPoint, 3);
	BnsSleep(3 * 1000);
	CFarm::GetInstance().ClearAroundMonster(CBaseObject::CreateFixPointInfo(20), [] {return FALSE; });
	CResAction::GetInstance().TakeDropItem(CBaseObject::CreateFixPointInfo(16));

	BnsMove(GatherPoint, 3);
	CFarm::GetInstance().TakeCorpse(CBaseObject::CreateFixPointInfo(16), Corpse_雪玉宫通路装置);


	_DailyTask_Team.ClumpTeamMember(3, L"第二扇门", GatherPoint);
	return TRUE;
}

BOOL CDailyTask_Bitan::Step5_Left(_In_ CONST std::vector<Point>& VecPoint)
{
	if (!IsCorrectDirection(em_Bitan_Team_Direction::Direction_Left))
		return TRUE;


	MoveThenKill(VecPoint);
	return TRUE;
}

BOOL CDailyTask_Bitan::Step5_Right(_In_ CONST std::vector<Point>& VecPoint)
{
	if (!IsCorrectDirection(em_Bitan_Team_Direction::Direction_Right))
		return TRUE;


	MoveThenKill(VecPoint);
	return TRUE;
}

BOOL CDailyTask_Bitan::Step6_Left(_In_ CONST std::vector<Point>& VecPoint)
{
	if (!IsCorrectDirection(em_Bitan_Team_Direction::Direction_Left))
		return TRUE;

	_DailyTask_Move.MoveToVecPoint(VecPoint, [](CONST Point&) { return FALSE; });
	CFarm::GetInstance().ClearAroundMonster(CBaseObject::CreateFixPointInfo(16), [] {return FALSE; });


	BnsMove(Point(-48295, 8188, 14739), 3);
	BnsSleep(3 * 1000);
	CFarm::GetInstance().ClearAroundMonster(CBaseObject::CreateFixPointInfo(16), [] {return FALSE; });
	CResAction::GetInstance().TakeDropItem(CBaseObject::CreateFixPointInfo(16));

	BnsMove(Point(-48157, 8629, 14873), 3);
	CFarm::GetInstance().TakeCorpse(CBaseObject::CreateFixPointInfo(16), Corpse_雪玉宫通路装置);
	_DailyTask_Team.ClumpTeamMember(4, L"第三扇门", Point(-48157, 8629, 14873));
	return TRUE;
}

BOOL CDailyTask_Bitan::Step6_Right(_In_ CONST std::vector<Point>& VecPoint)
{
	if (!IsCorrectDirection(em_Bitan_Team_Direction::Direction_Right))
		return TRUE;

	_DailyTask_Move.MoveToVecPoint(VecPoint, [](CONST Point&) { return FALSE; });
	CFarm::GetInstance().ClearAroundMonster(CBaseObject::CreateFixPointInfo(16), [] {return FALSE; });


	BnsMove(VecPoint.back(), 3);
	BnsSleep(3 * 1000);
	CFarm::GetInstance().ClearAroundMonster(CBaseObject::CreateFixPointInfo(16), [] {return FALSE; });
	CResAction::GetInstance().TakeDropItem(CBaseObject::CreateFixPointInfo(16));

	BnsMove(Point(-53868, 8643, 14873), 3);
	CFarm::GetInstance().TakeCorpse(CBaseObject::CreateFixPointInfo(16), Corpse_雪玉宫通路装置);
	_DailyTask_Team.ClumpTeamMember(4, L"第三扇门", Point(-53868, 8643, 14873));
	return TRUE;
}

BOOL CDailyTask_Bitan::Step7_Left(_In_ CONST std::vector<Point>& VecPoint)
{
	if (!IsCorrectDirection(em_Bitan_Team_Direction::Direction_Left))
		return TRUE;

	MoveThenKill(VecPoint);
	return TRUE;
}

BOOL CDailyTask_Bitan::Step7_Right(_In_ CONST std::vector<Point>& VecPoint)
{
	if (!IsCorrectDirection(em_Bitan_Team_Direction::Direction_Right))
		return TRUE;

	MoveThenKill(VecPoint);
	return TRUE;
}

BOOL CDailyTask_Bitan::Step8_Left(_In_ CONST std::vector<Point>& VecPoint)
{
	if (!IsCorrectDirection(em_Bitan_Team_Direction::Direction_Left))
		return TRUE;

	_DailyTask_Move.MoveToVecPoint(VecPoint, [](CONST Point&) { return FALSE; });
	CFarm::GetInstance().ClearAroundMonster(CBaseObject::CreateFixPointInfo(16), [] {return FALSE; });


	BnsMove(Point(-49839, 12634, 15252), 3);
	BnsSleep(3 * 1000);
	CFarm::GetInstance().ClearAroundMonster(CBaseObject::CreateFixPointInfo(16), [] {return FALSE; });
	CResAction::GetInstance().TakeDropItem(CBaseObject::CreateFixPointInfo(16));

	BnsMove(Point(-49839, 12634, 15252), 3);
	CFarm::GetInstance().TakeCorpse(CBaseObject::CreateFixPointInfo(16), Corpse_雪玉宫通路装置);
	_DailyTask_Team.ClumpTeamMember(5, L"第四扇门", Point(-49839, 12634, 15252));
	return TRUE;
}

BOOL CDailyTask_Bitan::Step8_Right(_In_ CONST std::vector<Point>& VecPoint)
{
	if (!IsCorrectDirection(em_Bitan_Team_Direction::Direction_Right))
		return TRUE;

	_DailyTask_Move.MoveToVecPoint(VecPoint, [](CONST Point&) { return FALSE; });
	CFarm::GetInstance().ClearAroundMonster(CBaseObject::CreateFixPointInfo(16), [] {return FALSE; });


	BnsMove(VecPoint.back(), 3);
	BnsSleep(3 * 1000);
	CFarm::GetInstance().ClearAroundMonster(CBaseObject::CreateFixPointInfo(16), [] {return FALSE; });
	CResAction::GetInstance().TakeDropItem(CBaseObject::CreateFixPointInfo(16));

	BnsMove(Point(-52383, 12896, 15277), 3);
	CFarm::GetInstance().TakeCorpse(CBaseObject::CreateFixPointInfo(16), Corpse_雪玉宫通路装置);
	_DailyTask_Team.ClumpTeamMember(5, L"第四扇门", Point(-52383, 12896, 15277));
	return TRUE;
}

BOOL CDailyTask_Bitan::Step9_Left(_In_ CONST std::vector<Point>& VecPoint)
{
	if (!IsCorrectDirection(em_Bitan_Team_Direction::Direction_Left))
		return TRUE;

	MoveThenKill(VecPoint);
	BnsMove(Point(-48581, 14313, 15263), 3);
	_DailyTask_Team.ClumpTeamMember(6, L"第五扇门", Point(-48581, 14313, 15263));
	return TRUE;
}

BOOL CDailyTask_Bitan::Step9_Right(_In_ CONST std::vector<Point>& VecPoint)
{
	if (!IsCorrectDirection(em_Bitan_Team_Direction::Direction_Right))
		return TRUE;

	MoveThenKill(VecPoint);
	BnsMove(Point(-53541, 14935, 15272), 3);
	_DailyTask_Team.ClumpTeamMember(6, L"第五扇门", Point(-53541, 14935, 15272));
	return TRUE;
}

BOOL CDailyTask_Bitan::Step10_Left(_In_ CONST std::vector<Point>& VecPoint)
{
	if (!IsCorrectDirection(em_Bitan_Team_Direction::Direction_Left))
		return TRUE;

	_DailyTask_Move.MoveToVecPoint(VecPoint, [](CONST Point&) { return FALSE; });
	_DailyTask_Team.ClumpTeamMember(7, L"金银童BOSS", VecPoint.back());

	CMonster Boss;
	if (CMonsterExtend::GetInstance().FindMonster_By_Name(Monster_金童, CBaseObject::CreateFixPointInfo(50), &Boss))
	{
		SetBossEnviroment(Boss);
		_DailyTask_Move.MoveToTarget(Monster_金童, 50);


		CONST std::vector<KillBossMethod> Vec =
		{
			//{ em_Player_Classes_Summoner, std::bind(&CDailyTask_SnowHole::KillBoss_Summoner, this, std::placeholders::_1) },
			{ em_Player_Classes_BladeMaster, std::bind(&CDailyTask_Bitan::Kill_金童_By_BladeMaster, this, std::placeholders::_1) },
		};

		auto emPlayerClasses = CPerson::GetInstance().GetClasses();
		auto Element = MyTools::CLPublic::Vec_find_if_Const(Vec, [emPlayerClasses](CONST KillBossMethod& itm) { return itm.emPlayerClasses == emPlayerClasses; });
		if (Element != nullptr)
		{
			Element->MethodPtr(Boss);
		}
		RestoreBossEnviroment();
		WaitToMemberDead();
	}
	return TRUE;
}

BOOL CDailyTask_Bitan::Step10_Right(_In_ CONST std::vector<Point>& VecPoint)
{
	if (!IsCorrectDirection(em_Bitan_Team_Direction::Direction_Right))
		return TRUE;

	_DailyTask_Move.MoveToVecPoint(VecPoint, [](CONST Point&) { return FALSE; });
	_DailyTask_Team.ClumpTeamMember(7, L"金银童BOSS", VecPoint.back());
	
	CMonster Boss;
	if (CMonsterExtend::GetInstance().FindMonster_By_Name(Monster_银童, CBaseObject::CreateFixPointInfo(50), &Boss))
	{
		SetBossEnviroment(Boss);
		_DailyTask_Move.MoveToTarget(Monster_银童, 50);

		struct KillBossMethod
		{
			em_Player_Classes emPlayerClasses;
			std::function<VOID(CONST CMonster&)> MethodPtr;
		};

		CONST std::vector<KillBossMethod> Vec =
		{
			//{ em_Player_Classes_Summoner, std::bind(&CDailyTask_SnowHole::KillBoss_Summoner, this, std::placeholders::_1) },
			{ em_Player_Classes_BladeMaster, std::bind(&CDailyTask_Bitan::Kill_银童_By_BladeMaster, this, std::placeholders::_1) },
		};

		auto emPlayerClasses = CPerson::GetInstance().GetClasses();
		auto Element = MyTools::CLPublic::Vec_find_if_Const(Vec, [emPlayerClasses](CONST KillBossMethod& itm) { return itm.emPlayerClasses == emPlayerClasses; });
		if (Element != nullptr)
		{
			Element->MethodPtr(Boss);
		}
		RestoreBossEnviroment();
		WaitToMemberDead();
	}
	return TRUE;
}

BOOL CDailyTask_Bitan::Step11(_In_ CONST std::vector<Point>& VecPoint)
{
	_DailyTask_Move.MoveToVecPoint(VecPoint, [](CONST Point&) { return FALSE; });
	return TRUE;
}

BOOL CDailyTask_Bitan::Step12(_In_ CONST std::vector<Point>& VecPoint)
{
	CNpcExtend::GetInstance().DoAction_By_ExistNpc_For_Name(Npc_老邱, CBaseObject::CreateFixPointInfo(16), [](CONST CNpc& Npc) 
	{
		CTaskExtend::GetInstance().PickBLTask_By_Npc(SumitBLTask_Src(0x16, 0x45D3A, 0x1C6, 0, 1), Npc_老邱);
		CTaskExtend::GetInstance().PickBLTask_By_Npc(SumitBLTask_Src(0x16, 0x45D3A, 0x1C6, 0, 0), Npc_老邱);
	});

	_DailyTask_Move.MoveToVecPoint(VecPoint, [](CONST Point&) { return FALSE; });
	return TRUE;
}

BOOL CDailyTask_Bitan::Step13(_In_ CONST std::vector<Point>& VecPoint)
{
	_DailyTask_Move.FarmAndMove(VecPoint, 10);
	CResAction::GetInstance().TakeDropItem(CBaseObject::CreateFixPointInfo(16));

	_DailyTask_Team.ClumpTeamMember(8, L"第一个伊鲁卡", VecPoint.back());
	return TRUE;
}

BOOL CDailyTask_Bitan::Step14(_In_ CONST std::vector<Point>& VecPoint)
{
	return KillYLKShadow(VecPoint);
}

BOOL CDailyTask_Bitan::Step15(_In_ CONST std::vector<Point>& VecPoint)
{
	_DailyTask_Move.FarmAndMove(VecPoint, 10);
	CResAction::GetInstance().TakeDropItem(CBaseObject::CreateFixPointInfo(16));

	_DailyTask_Team.ClumpTeamMember(9, L"第二个伊鲁卡", VecPoint.back());
	return TRUE;
}

BOOL CDailyTask_Bitan::Step16(_In_ CONST std::vector<Point>& VecPoint)
{
	return KillYLKShadow(VecPoint);
}

BOOL CDailyTask_Bitan::Step17(_In_ CONST std::vector<Point>& VecPoint)
{
	_DailyTask_Move.MoveToVecPoint(VecPoint, [](CONST Point&) { return FALSE; });
	BnsSleep(3 * 1000);
	CFarm::GetInstance().ClearAroundMonster(CBaseObject::CreateFixPointInfo(20));
	return TRUE;
}

BOOL CDailyTask_Bitan::Step18(_In_ CONST std::vector<Point>& VecPoint)
{
	return KillYLKShadow(VecPoint);
}

BOOL CDailyTask_Bitan::Step19(_In_ CONST std::vector<Point>& VecPoint)
{
	_DailyTask_Move.MoveToVecPoint(VecPoint, [](CONST Point&) { return FALSE; });
	BnsSleep(2000);

	while (GameRun && !EMPTY_PERSONHP && CPerson::GetInstance().GetDis(Point(-47265, 21602, 11186)) < 5)
	{
		BnsMove(Point(-47416, 21576, 11184), 3);
		BnsSleep(1000);
		CFarm::GetInstance().Pickup_By_LongMai(Corpse_地下通道龙脉);
		BnsSleep(8 * 1000);
	}

	return TRUE;
}

BOOL CDailyTask_Bitan::Step20(_In_ CONST std::vector<Point>& VecPoint)
{
	_DailyTask_Move.MoveToVecPoint(VecPoint, [](CONST Point&) { return FALSE; });
	_DailyTask_Team.ClumpTeamMember(10, L"伊鲁卡BOSS", VecPoint.back());
	CFilter::GetInstance().AddMonsterFilter(Monster_茱莉叶, CFilter::MonsterFilterType::Filter_Monster_Low);

	CMonster Boss;
	if (CMonsterExtend::GetInstance().FindMonster_By_Name(Monster_进击的伊鲁卡, CBaseObject::CreateFixPointInfo(50), &Boss))
	{
		SetBossEnviroment(Boss);
		_DailyTask_Move.MoveToTarget(Monster_进击的伊鲁卡, 50);

		CONST std::vector<KillBossMethod> Vec =
		{
			//{ em_Player_Classes_Summoner, std::bind(&CDailyTask_SnowHole::KillBoss_Summoner, this, std::placeholders::_1) },
			{ em_Player_Classes_BladeMaster, std::bind(&CDailyTask_Bitan::Kill_进击的伊鲁卡_BladeMaster, this, std::placeholders::_1) },
		};

		auto emPlayerClasses = CPerson::GetInstance().GetClasses();
		auto Element = MyTools::CLPublic::Vec_find_if_Const(Vec, [emPlayerClasses](CONST KillBossMethod& itm) { return itm.emPlayerClasses == emPlayerClasses; });
		if (Element != nullptr)
		{
			Element->MethodPtr(Boss);
		}
		RestoreBossEnviroment();
		WaitToMemberDead();
	}

	CFilter::GetInstance().RemoveMonsterFilter(Monster_茱莉叶);
	return TRUE;
}

BOOL CDailyTask_Bitan::Step21(_In_ CONST std::vector<Point>& VecPoint)
{
	_DailyTask_Move.MoveToVecPoint(VecPoint, [](CONST Point&) { return FALSE; });
	BnsSleep(2000);

	while (GameRun && !EMPTY_PERSONHP && CPerson::GetInstance().GetDis(Point(-47025, 26826, 12119)) < 5)
	{
		BnsMove(VecPoint.back(), 2);
		BnsSleep(1000);
		CFarm::GetInstance().Pickup_By_LongMai(Corpse_地下通道龙脉);
		BnsSleep(8 * 1000);
	}

	return TRUE;
}

BOOL CDailyTask_Bitan::Step22(_In_ CONST std::vector<Point>& VecPoint)
{
	_DailyTask_Move.MoveToVecPoint(VecPoint, [](CONST Point&) { return FALSE; });
	CTaskExtend::GetInstance().PickBLTask_By_Npc(SumitBLTask_Src(0x4, 0x45EFE, 0x1D7, 0, 1), Npc_杜宏昌的幽灵);
	CTaskExtend::GetInstance().PickBLTask_By_Npc(SumitBLTask_Src(0x4, 0x45EFE, 0x1D7, 0, 0), Npc_杜宏昌的幽灵);
	return TRUE;
}

BOOL CDailyTask_Bitan::Step23(_In_ CONST std::vector<Point>& VecPoint)
{
	CFilter::GetInstance().AddMonsterFilter(Monster_火山, CFilter::MonsterFilterType::Filter_Monster_Low);
	CFilter::GetInstance().AddMonsterFilter(Monster_冰河, CFilter::MonsterFilterType::Filter_Monster_Low);

	MoveThenKill(VecPoint);
	_DailyTask_Team.ClumpTeamMember(11, L"一群小BOSS", VecPoint.back());
	return TRUE;
}

BOOL CDailyTask_Bitan::Step24(_In_ CONST std::vector<Point>& VecPoint)
{
	MoveThenKill(VecPoint);

	CFilter::GetInstance().AddMonsterFilter(Monster_雪玉之核, CFilter::MonsterFilterType::Filter_Monster_Low);
	for (int i = 0;i < 3; ++i)
	{
		BnsMove(Point(-53782, 27960, 13534), 3);
		BnsSleep(20 * 1000);
		CFarm::GetInstance().ClearAroundMonster(CBaseObject::CreateFixPointInfo(16));
		CResAction::GetInstance().TakeDropItem(CBaseObject::CreateFixPointInfo(16));
	}

	BnsSleep(15 * 1000);
	// Kill Bird!


	BnsMove(VecPoint.back(), 3);
	BnsSleep(1000);
	CFarm::GetInstance().ClearAroundMonster(CBaseObject::CreateFixPointInfo(16));
	CResAction::GetInstance().TakeDropItem(CBaseObject::CreateFixPointInfo(16));
	return TRUE;
}

BOOL CDailyTask_Bitan::Step25(_In_ CONST std::vector<Point>& VecPoint)
{
	_DailyTask_Move.MoveToVecPoint(VecPoint, [](CONST Point&) { return FALSE; });
	CFarm::GetInstance().EatFood(50);
	_DailyTask_Team.ClumpTeamMember(12, L"修罗王", VecPoint.back());
	return TRUE;
}

BOOL CDailyTask_Bitan::Step26(_In_ CONST std::vector<Point>& VecPoint)
{
	_DailyTask_Move.MoveToVecPoint(VecPoint, [](CONST Point&) { return FALSE; });

	CMonster Boss;
	if (CMonsterExtend::GetInstance().FindMonster_By_Name(Monster_修罗王, CBaseObject::CreateFixPointInfo(50), &Boss))
	{
		SetBossEnviroment(Boss);
		_DailyTask_Move.MoveToTarget(Monster_修罗王, 50);


		CONST std::vector<KillBossMethod> Vec =
		{
			//{ em_Player_Classes_Summoner, std::bind(&CDailyTask_SnowHole::KillBoss_Summoner, this, std::placeholders::_1) },
			{ em_Player_Classes_BladeMaster, std::bind(&CDailyTask_Bitan::Kill_金童_By_BladeMaster, this, std::placeholders::_1) },
		};

		auto emPlayerClasses = CPerson::GetInstance().GetClasses();
		auto Element = MyTools::CLPublic::Vec_find_if_Const(Vec, [emPlayerClasses](CONST KillBossMethod& itm) { return itm.emPlayerClasses == emPlayerClasses; });
		if (Element != nullptr)
		{
			Element->MethodPtr(Boss);
		}
		RestoreBossEnviroment();
		WaitToMemberDead();
	}
	return TRUE;
}

BOOL CDailyTask_Bitan::End(_In_ CONST std::vector<Point>&)
{
	BnsSleep(5 * 1000);
	BnsMove(Point(-54100, 35580, 13044), 3);
	
	CFarm::GetInstance().ClearAroundMonster(CBaseObject::CreateFixPointInfo(16), [] { return FALSE; });
	CResAction::GetInstance().TakeDropItem(CBaseObject::CreateFixPointInfo(16));

	// 斩首任务-奇特的灵兽
	CTaskExtend::GetInstance().SumitTask_By_Parm(SumitTask_Src(0x2, 0x61C0, 0, 0, 0xFF, 0x261C0, 0, 0), L"", L"");
	
	// 斩首任务-悲叹的雪域宫宫主
	CTaskExtend::GetInstance().SumitTask_By_Parm(SumitTask_Src(0x2, 0x61EB, 0, 0, 0xFF, 0x261EB, 0, 0), L"", L"");

	CTaskExtend::GetInstance().SumitTask_By_Parm(SumitTask_Src(0x2, 0x12B, 0, 1, 0xFF, 0x2012B, 0, 0), L"", L"");
	CTaskExtend::GetInstance().SumitTask_By_Parm(SumitTask_Src(0x2, 0x12C, 0, 0, 0xFF, 0x2012C, 0, 0), L"", L"");
	CTaskExtend::GetInstance().SumitTask_By_Parm(SumitTask_Src(0x2, 0x1C6, 0, 0, 0xFF, 0x201C6, 0, 0), L"", L"");
	CTaskExtend::GetInstance().SumitTask_By_Parm(SumitTask_Src(0x2, 0x1D7, 0, 0, 0xFF, 0x201D7, 0, 0), L"", L"");
	CTaskExtend::GetInstance().SumitTask_By_Parm(SumitTask_Src(0x2, 0x4E4, 0, 1, 0xFF, 0x204E4, 0, 0), L"", L"");

	
	// 离开副本
	CResExtend::GetInstance().DoAction_By_ExistCorpse_For_Name(Monster_雪玉宫龙脉, CBaseObject::CreateFixPointInfo(100), [](CONST CCorpse& Door)
	{
		BnsMove(Door.GetPoint(), 1);
		BnsSleep(3 * 1000);
		Door.Take();
	});

	BnsSleep(5 * 1000);
	CPersonFunction::GetInstance().WaitGameLoad();

	CONST static std::vector<Point> VecSumitTaskPoint = 
	{
		Point(-40568,8360,-1740),Point(-40676,8996,-1740),Point(-40792,9692,-1740),Point(-40904,10504,-1740),
	};
	_DailyTask_Move.MoveToVecPoint(VecSumitTaskPoint, [](CONST Point&) { return FALSE; });

	CTaskExtend::GetInstance().SumitTask_By_Parm(SumitTask_Src(0x2, 0x254, 0, 0, 0xFF, 0x84, 0x41E33, 0), Npc_尹喜丘, L"");
	CTaskExtend::GetInstance().SumitTask_By_Parm(SumitTask_Src(0x2, 0x12A, 0, 0, 0xFF, 0x84, 0x41E33, 0), Npc_尹喜丘, L"");
	CTaskExtend::GetInstance().SumitTask_By_Parm(SumitTask_Src(0x3, 0x129, 0, 0, 0xFF, 0x83, 0x41E33, 0), Npc_傅丁中, L"");
	BnsSleep(1000);
	return TRUE;
}

BOOL CDailyTask_Bitan::MoveToDungeon()
{
	// 副本门口
	BnsMove(Point(-40624, 10428, -1740), 3);
	BnsMove(Point(-40604, 10704, -1700), 3);
	while (GameRun && !IsInDungeon())
	{
		BnsMove(Point(-40604, 10804, -1700), 1);
		BnsSleep(3 * 1000);
		CPersonFunction::GetInstance().WaitGameLoad();
	}
	return TRUE;
}

BOOL CDailyTask_Bitan::AccpetDailyTask() CONST
{
	CONST static std::vector<Point> VecPoint =
	{
		Point(-40376,9412,-1740),Point(-40320,9964,-1740),Point(-40624,10428,-1740)
	};

	_DailyTask_Move.MoveToVecPoint(VecPoint, 0);

	CTaskExtend::GetInstance().PickBLTask_By_Npc(SumitBLTask_Src(0x86, 0x401B8, 0x12B, 0, 0x1), Npc_杜宏昌的幽灵);
	CTaskExtend::GetInstance().PickBLTask_By_Parm(SumitBLTask_Src(0x128, 0, 0x128, 0, 0), [](SumitBLTask_Src&) { return TRUE; });
	
	BnsMove(Point(-40396, 10524, -1740), 1);
	BnsSleep(2000);

	CTaskExtend::GetInstance().SumitTask_By_Parm(SumitTask_Src(0x1, 0x128, 0x0, 0x0, 0xFF, 0x85, 0x479B6, 0x0), Npc_杜宏昌的幽灵, L"");
	CTaskExtend::GetInstance().PickBLTask_By_Npc(SumitBLTask_Src(0x85, 0x479B6, 0x12B, 0, 0), Npc_杜宏昌的幽灵);

	BnsMove(Point(-40904, 10516, -1740), 1);
	BnsSleep(2000);

	CTaskExtend::GetInstance().PickBLTask_By_Npc(SumitBLTask_Src(0x84, 0x479B6, 0x254, 0, 0), Npc_尹喜丘);
	CTaskExtend::GetInstance().PickBLTask_By_Npc(SumitBLTask_Src(0x86, 0x401B8, 0x12A, 0, 0), Npc_尹喜丘);
	CTaskExtend::GetInstance().PickBLTask_By_Npc(SumitBLTask_Src(0x83, 0x401B8, 0x129, 0, 0), Npc_傅丁中);

	BnsSleep(3 * 1000);
	return CTask::GetInstance().ExistTask(0x12B, nullptr);
}

BOOL CDailyTask_Bitan::IsInDungeon() CONST
{
	static CONST std::set<DWORD> SetBiTanMap = { 0x170F };
	return SetBiTanMap.find(CPerson::GetInstance().GetMapId()) != SetBiTanMap.end();
}

VOID CDailyTask_Bitan::StudyDailySkill() CONST
{
	CONST static std::vector<LearnSkillContent> VecBladeMasterSkill =
	{
		{ BladeMaster_SkillId_R1,0 },{ BladeMaster_SkillId_T1,1 },{ BladeMaster_SkillId_2,2 },{ BladeMaster_SkillId_Tab,1 },{ BladeMaster_SkillId_X,0 },
		{ BladeMaster_SkillId_R2, 0 },{ BladeMaster_SkillId_Z,0 },{ BladeMaster_SkillId_C,0 },{ BladeMaster_SkillId_4,2 },{ BladeMaster_SkillId_Gesture_3,0 },
		{ BladeMaster_SkillId_V,0 }
	};

	switch (CPerson::GetInstance().GetClasses())
	{
	case em_Player_Classes_Destroyer:
		break;
	case em_Player_Classes_Assassin:
		break;
	case em_Player_Classes_BladeMaster:
		CDailyTask_Skill().LearningSkill(L"剑士火焰系", VecBladeMasterSkill);
		CFarmSkill::GetInstance().AddDisableSkillId(BladeMaster_SkillId_V);
		break;
	case em_Player_Classes_Summoner:
		break;
	case em_Player_Classes_ForceMaster:
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

VOID CDailyTask_Bitan::Kill_金童_By_BladeMaster(_In_ CONST CMonster& Gold)
{

}

VOID CDailyTask_Bitan::Kill_银童_By_BladeMaster(_In_ CONST CMonster& Boss)
{
	
}

VOID CDailyTask_Bitan::Kill_进击的伊鲁卡_BladeMaster(_In_ CONST CMonster& Boss)
{

}

VOID CDailyTask_Bitan::Kill_修罗王_BladeMaster(_In_ CONST CMonster& Boss)
{

}

BOOL CDailyTask_Bitan::TakeShield(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<BOOL(VOID)> fnExit) CONST
{
	std::vector<CCorpse> ShieldVec;
	CResExtend::GetInstance().GetAroundBallList(FixPtInfo, ShieldVec);

	// 降序
	std::sort(ShieldVec.begin(), ShieldVec.end(), [](CONST CCorpse& Corpse1, CONST CCorpse& Corpse2) 
	{
		return Corpse1.GetId() < Corpse2.GetId();
	});


	DWORD dwIndex = CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex();
	if (dwIndex == -1)
	{
		LOG_CF_E(L"当前队伍索引竟然是-1!");
		return FALSE;
	}
	else if (dwIndex >= ShieldVec.size())
	{
		LOG_CF_E(L"dwindex=%d, Monster.size()=%d", dwIndex, ShieldVec.size());
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

BOOL CDailyTask_Bitan::KillYLKShadow(_In_ CONST std::vector<Point>& VecPoint) CONST
{
	CFilter::GetInstance().AddMonsterFilter(Monster_伊鲁卡的影子, CFilter::MonsterFilterType::Filter_Monster_Low);
	_DailyTask_Move.MoveToVecPoint(VecPoint, [](CONST Point&) { return FALSE; });

	// 等待盾兵刷新出来
	BnsSleep(2000);
	CFarm::GetInstance().ClearAroundMonster(CBaseObject::CreateFixPointInfo(16));
	CFilter::GetInstance().RemoveMonsterFilter(Monster_伊鲁卡的影子);

	CMonster Monster;
	if (CMonsterExtend::GetInstance().FindMonster_By_Name(Monster_伊鲁卡的影子, CBaseObject::CreateFixPointInfo(30), &Monster))
	{
		TakeShield(CBaseObject::CreateFixPointInfo(30), [Monster]
		{
			Monster.SetAngle();
			return Monster.IsDead();
		});
	}

	CPersonFunction::GetInstance().CancelPutDownStatus();
	return TRUE;
}

BOOL CDailyTask_Bitan::IsCorrectDirection(_In_ em_Bitan_Team_Direction emDirection) CONST
{
	switch (CPerson::GetInstance().GetClasses())
	{
	case em_Player_Classes_Destroyer:
		break;
	case em_Player_Classes_Assassin:	// 刺客走左边
		return emDirection == em_Bitan_Team_Direction::Direction_Left;
	case em_Player_Classes_BladeMaster: // 剑士走左边
		return emDirection == em_Bitan_Team_Direction::Direction_Left;
	case em_Player_Classes_Summoner:
		break;
	case em_Player_Classes_ForceMaster: // 气功走左边
		return emDirection == em_Bitan_Team_Direction::Direction_Left;
	case em_Player_Classes_KongFuMaster: // 拳师走右边
		return emDirection == em_Bitan_Team_Direction::Direction_Right;
	case em_Player_Classes_Danncer:
		break;
	case em_Player_Classes_Warlock:		// 咒术师走右边
		return emDirection == em_Bitan_Team_Direction::Direction_Right;
	case em_Player_Classes_KongFuForceMaster:
		break;
	case em_Player_Classes_Gunner:		// 炮手走右边
		return emDirection == em_Bitan_Team_Direction::Direction_Right;
	default:
		break;
	}
	return FALSE;
}
