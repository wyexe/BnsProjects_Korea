#include "stdafx.h"
#include "FarmBoss.h"
#include <map>
#include <MyTools/CLFile.h>
#include <MyTools/Log.h>
#include <MyTools/TimeTick.h>
#include "PersonFunction.h"
#include "FarmSkill.h"
#include "SomeFun.h"
#include "MonsterExtend.h"
#include "GameFunction.h"
#include "Monster.h"
#include "FarmTask.h"
#include "Person.h"
#include "NpcExtend.h"
#include "Npc.h"
#include "BnsFilter.h"
#include "TeamMemberExtend.h"
#include "Skill.h"
#include "ObjectFunction.h"
#include "Farm.h"
#include "PlayerExtend.h"
#include "GameUI.h"
#include "FarmBossSkill.h"
#include <set>

#define _SELF L"FarmBoss.cpp"

CFarmBoss::CFarmBoss() : vBossSkillLock(L"CFarmBoss::vBossSkillLock"), uLoopCount(NULL)
{
	
}

BOOL CFarmBoss::InitBossSkill()
{
	return TRUE;
}


DWORD CFarmBoss::GetHookMonsterBase()
{
	return dwHookMonsterBase;
}

VOID CFarmBoss::SetHookMonsterBase(_In_ DWORD dwHookMonsterBase_)
{
	dwHookMonsterBase = dwHookMonsterBase_;
}

BOOL CFarmBoss::KillBoss_By_Name(_In_ CONST std::wstring& wsBossName, _In_ CONST FixPointInfo& FixPtInfo)
{
	Xueren_flag = 1;
	Xueren_2263 = 1;
	Xueren_2265 = 1;
	Xueren_2254 = 1;
	Xueyugong_One = 1;
	Strike_Zhongzi_Count = 1;
	Yiluka_Sum = 1;
	Yiluka_Control = 1;
	Xiuluowang_Control = 1;
	Xiuluowang_Control_2 = 1;
	Xiuluowang_Control_3 = 1;
	Heilongjiao_Control_1 = 1;
	Heilongjiao_Control_2_1 = 1;
	Heilongjiao_Control_2_2 = 1;
	Heilongjiao_Buff = 1;
	Heilongjiao_Huafen = FALSE;
	InitBossSkill();
	CMonster Monster;
	if (!GetBossMonster(wsBossName, Monster))
	{
		LOG_C_D( L"30m范围都找不到BOSS:%s", wsBossName.c_str());
		return FALSE;
	}
	//先刷新队伍位置
	CTeamMemberExtend::GetInstance().SetCurrentTeamIndexByClasses();
	CGameFunction::GetInstance().HookMonsterSkill(TRUE, Monster.GetBase());
	SetBossEnviroment(Monster);
	
	CSomeFun::GetInstance().SetSpeed(3);
	DWORD dwOldValue = CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_KILLMONSTERR_MOVE_DIS, 8);
	DWORD dwOldSet = CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_DISABLE_BECKONCAT, TRUE);


	auto emPlayerClasses = CPerson::GetInstance().GetClasses();
	if (emPlayerClasses == em_Player_Classes_Destroyer)
		KillBoss_Lux(Monster, 60 * 1000);
	else if (emPlayerClasses == em_Player_Classes_KongFuForceMaster)
		KillBoss_Kffm(Monster, FixPtInfo);
	else if (emPlayerClasses == em_Player_Classes_Summoner)
		KillBoss_Sum(Monster, FixPtInfo);
	else if (emPlayerClasses == em_Player_Classes_Warlock)
		KillBoss_Warlock(Monster, FixPtInfo);

	CFarmBossSkill::GetInstance().ReleaseBossObject();
	CSomeFun::GetInstance().SetSpeed(1);
	CPersonFunction::GetInstance().RestoreMoveFlag();

	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_DISABLE_BECKONCAT, dwOldSet);
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_KILLMONSTERR_MOVE_DIS, dwOldValue);
	return CGameFunction::GetInstance().HookMonsterSkill(FALSE, GetHookMonsterBase());
}

BOOL CFarmBoss::GetBossMonster(_In_ CONST std::wstring& wsBossName, _Out_opt_ CMonster& Monster) CONST
{
	if (wsBossName == L"교도관")
	{
		std::vector<CMonster> VecMonster;
		if (CMonsterExtend::GetInstance().GetAroundMonsterListByName(wsBossName, CBaseObject::CreateFixPointInfo(30), VecMonster) == 0)
			return FALSE;

		if (VecMonster.size() == 1)
		{
			Monster = *VecMonster.begin();
			return TRUE;
		}

		if (VecMonster.at(0).GetHp() > VecMonster.at(1).GetHp())
			Monster = VecMonster.at(0);
		else
			Monster = VecMonster.at(1);
		return TRUE;
	}
	else if(CMonsterExtend::GetInstance().FindMonster_By_Name(wsBossName, CBaseObject::CreateFixPointInfo(30.0f), &Monster))
		return TRUE;

	CNpc Npc;
	if (CNpcExtend::GetInstance().FindNpc_By_Name(wsBossName, CBaseObject::CreateFixPointInfo(30.0f), &Npc))
	{
		CMonster Monster_(Npc);
		Monster_.GetDis();
		Monster = std::move(Monster_);
		return TRUE;
	}
	return FALSE;
}

DWORD& CFarmBoss::GetMonsterSkillId() CONST
{
	static DWORD dwSkillId = 0;
	return dwSkillId;
}

BOOL CFarmBoss::UseSkill_By_BossSkillId_Lux(_In_ CMonster& Monster, _In_ DWORD dwBossSkillId)
{
	VMProtectBegin("Lux-boss");
	auto& FarmSkill = CFarmSkill::GetInstance();
	auto  NowPoint = Monster.GetPoint();

	CONST static Point BossPoint(-49415, 24578, 14462);
	CONST static Point TarPoint(-49872, 24648, 14448);//雪人王BOSS要中间要躲避的坐标

	CONST static Point xueren_point1(-49048, 25028, 14462);
	CONST static Point xueren_point2(-48892, 24180, 14462);
	CONST static Point xueren_point3(-49403, 23627, 14462);
	CONST static Point xueren_point4(-50025, 23611, 14462);
	CONST static Point xueren_point5(-50652, 23955, 14462);
	CONST static Point xueren_point6(-50888, 24552, 14462);
	CONST static Point xueren_point7(-50970, 25337, 14462);
	CONST static Point xueren_point8(-50312, 25547, 14462);
	CONST static Point xueren_point9(-49596, 25568, 14462);

	CONST static Point xueren_going1(-49304, 24638, 14462);
	CONST static Point xueren_going2(-49460, 25180, 14462);
	CONST static Point xueren_going3(-50286, 25343, 14462);
	CONST static Point xueren_going4(-50488, 24817, 14462);
	CONST static Point xueren_going5(-50160, 24235, 14462);
	CONST static Point xueren_going6(-49422, 24267, 14462);



	BOOL bRetCode = TRUE;
	switch (dwBossSkillId)
	{
	case 5190190:
		CSkill::UseSkill_By_Id(Lux_SkillId_V_R, TRUE);
		Sleep(300);
		CSkill::UseSkill_By_Id(Lux_SkillId_V_R, TRUE);
		Sleep(300);
		CSkill::UseSkill_By_Id(Lux_SkillId_Tab, TRUE);
		BnsSleep(1200);
		break;
	case 5190192:
		CSkill::UseSkill_By_Id(Lux_SkillId_Tab, TRUE);
		BnsSleep(1200);
		break;
	case 5190195:
		if (CPerson::GetInstance().GetPercentHp() <= 70 && CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_V))
		{
			LOG_C_D( L"使用V!");
			BOOL bUseSkill = FALSE;
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(4000, [&Monster, &bUseSkill]{
				if (!bUseSkill)
				{
					CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2 * 1000, []{
						CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V);
						return !CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr);
					});
					bUseSkill = TRUE;
				}

				if (CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr) && GameRun)
				{
					Monster.SetAngle();
					CSkill::UseSkill_By_Id(Lux_SkillId_V_R, TRUE);
				}
				return BnsSleep(100);
			});

			CPersonFunction::GetInstance().Move(Monster.GetBehindPoint(2.0f));
		}
		else
		{
			if (CPerson::GetInstance().GetMp() >= 30 && CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_C))
			{
				CSkill::UseSkill_By_Id(Lux_SkillId_R, TRUE);
				Sleep(300);
				CSkill::UseSkill_By_Id(Lux_SkillId_R, TRUE);
				Sleep(300);
				CSkill::UseSkill_By_Id(Lux_SkillId_C, TRUE);
				BnsSleep(1500);
			}
			else
			{
				CSkill::UseSkill_By_Id(Lux_SkillId_V_R, TRUE);
				Sleep(300);
				CSkill::UseSkill_By_Id(Lux_SkillId_V_R, TRUE);
				Sleep(300);
				CSkill::UseSkill_By_Id(Lux_SkillId_Tab, TRUE);
				BnsSleep(1200);
			}
		}
		break;
	case 5513136:
		LOG_C_D( L"5513136");
		if (CPerson::GetInstance().GetPercentHp() <= 70 && CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_V))
		{
			LOG_C_D( L"使用V!");
			BOOL bUseSkill = FALSE;
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(4000, [&Monster, &bUseSkill]{
				if (!bUseSkill)
				{
					Monster.SetAngle();
					CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2 * 1000, []{
						CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V);
						return !CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr);
					});
					bUseSkill = TRUE;
				}

				if (CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr) && GameRun)
				{
					Monster.SetAngle();
					CSkill::UseSkill_By_Id(Lux_SkillId_V_R, TRUE);
				}
				return BnsSleep(100);
			});

			CPersonFunction::GetInstance().Move(Monster.GetBehindPoint(2.0f));
		}
		else if (FarmSkill.IsCool_By_SkillId_In_HideUi(Lux_SkillId_SS))
		{
			LOG_C_D( L"使用SS");
			BOOL bUseSkill = FALSE;
			Point FrontPoint = Monster.GetFrontPoint(8.0f);
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2000, [&bUseSkill, &FrontPoint, &Monster]{
				if (!bUseSkill)
				{
					CFarmSkill::GetInstance().UseSkill_By_SS();
					BnsSleep(500);
					bUseSkill = TRUE;
				}
				if (Monster.GetDis() < 5.0f)
					CPersonFunction::GetInstance().Move(FrontPoint);
				return BnsSleep(100);
			});
			Monster.SetAngle();
			BnsSleep(500);
			if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_2))
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_2);
			else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_1))
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_1);
		}
		else
		{
			LOG_C_D( L"转圈躲转圈!");
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(3500, []{
				CSkill::UseSkill_By_Id(Lux_SkillId_Tab, TRUE);
				return BnsSleep(100);
			});
		}

		break;
	case 5513124:case 5513114:case 5513120:case 5513121:case 5513135:
		LOG_C_D( L"统一走后");
		CPersonFunction::GetInstance().Move(Monster.GetBehindPoint(3.0f));
		BnsMove(Monster.GetBehindPoint(3.0f), 1.0f);
		break;
	case 5513133:
		LOG_C_D( L"转圈!");
		CSkill::UseSkill_By_Id(Lux_SkillId_Tab, TRUE);
		CPersonFunction::GetInstance().Move(Monster.GetBehindPoint(3.0f));
		BnsMove(Monster.GetBehindPoint(3.0f), 1.0f);
		break;
	case 5513112:
		LOG_C_D( L"用C");
		if (FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_C))
		{
			CSkill::UseSkill_By_Id(Lux_SkillId_V_R);
			BnsSleep(300);
			CSkill::UseSkill_By_Id(Lux_SkillId_C, TRUE);
			BnsSleep(1500);
		}
		else
		{
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2500, []{
				CSkill::UseSkill_By_Id(Lux_SkillId_Tab, TRUE);
				return BnsSleep(100);
			});
		}

		break;
	case 5100344:
		LOG_C_D( L"跳起!");
		Sleep(500);
		FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
		Sleep(800);
		break;
	case 5100345:
		LOG_C_D( L"5100345-----------走tab!");
		FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
		Sleep(200);
		FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
		Sleep(600);
		break;
	case 5100366:
		LOG_C_D( L"走后爆菊....");
		CPersonFunction::GetInstance().Move(Monster.GetBehindPoint(2.0f));
		BnsMove(Monster.GetBehindPoint(2.0f), 1.0f);
		Monster.SetAngle();
		Sleep(100);
		FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
		Sleep(450);
		Monster.SetAngle();
		FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
		break;
	case 5100341:
		LOG_C_D( L"走sss....");
		CSomeFun::GetInstance().SimlationKey('S', 400);
		Sleep(500);
		CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100));

		/*FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
		Sleep(800);*/
		//LOG_C_D( L"走背后!");
		//CPersonFunction::GetInstance().Move(Monster.GetBehindPoint(2.0f));
		//BnsMove(Monster.GetBehindPoint(2.0f), 1.0f);
		break;
	case 5100347:
		if (CPerson::GetInstance().GetPercentHp() < 70 && FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_V))
		{
			LOG_C_D( L"准备放V!");
			CSkill::UseSkill_By_Id(Lux_SkillId_V_R, TRUE);
			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_V);
			Sleep(300);
			do
			{
				Monster.SetAngle();
				CSkill::UseSkill_By_Id(Lux_SkillId_V_R, TRUE);
			} while (CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr));
		}
		else
		{
			LOG_C_D( L"5100347--往sss....");
			CSomeFun::GetInstance().SimlationKey('S', 400);
			Sleep(3500);
			LOG_C_D( L"5100347--往www....");
			CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100));
		}
		break;
	case 5510101:case 5510922: case 5510926: case 5565098: case 5565109: case 6200356: case 6200363: case 5534009: case 6200502: case 5565074: case 5565075: case 5565076:
	case 5534015:case 5565077: case 5565033: case 5534008:
		if (CMonsterExtend::GetInstance().FindMonster_By_Name(L"건향", CBaseObject::CreateFixPointInfo(5.0f), nullptr))
		{
			LOG_C_D( L"要放3把剑了!");
			CONST static std::vector<Point> vPoint = {
				Point(-18708, -32368, -5840), Point(-18604, -33112, -5840), Point(-19304, -33300, -5840),
				Point(-19880, -32760, -5848), Point(-19428, -32120, -5860),
			};

			for (CONST auto& itm : vPoint)
			{
				// 判断目标坐标范围5m之内是否有剑的存在
				if (!CMonsterExtend::GetInstance().FindMonster_By_Name(L"건향", FixPointInfo(itm, 5), nullptr))
				{
					CSkill::UseSkill_By_Id(Lux_SkillId_Tab, TRUE);
					BnsSleep(1000);
					BnsMove(itm, 3);
					CSomeFun::GetInstance().TimeOut_Condiction_GameRun(5 * 1000, [&Monster]{
						return Monster.GetDis() > 3.0f;
					});
					break;
				}
			}
		}
		else
		{
			static ULONGLONG ulTick = 0;
			if (static_cast<DWORD>(::GetTickCount64() - ulTick) >= 1500)
			{
				if (CPerson::GetInstance().GetPercentHp() < 70 && FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_V))
				{
					CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2 * 1000, []{
						CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V);
						return !CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr);
					});
					BnsSleep(500);
					CSomeFun::GetInstance().TimeOut_Condiction_GameRun(3500, [&Monster, &FarmSkill]{
						Monster.SetAngle();
						CSkill::UseSkill_By_Id(Lux_SkillId_V_R, TRUE);
						return BnsSleep(100) && CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr);
					});
				}
				else if (FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_1))
				{
					FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_1);
					ulTick = ::GetTickCount64();
				}
				else if (FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_2))
				{
					FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_2);
					ulTick = ::GetTickCount64();
				}
				else if (CPerson::GetInstance().GetMp() > 50 && FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_Z_Attract))
				{
					FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Z_Attract);
					ulTick = ::GetTickCount64();
				}
				else if (CPerson::GetInstance().GetMp() > 50 && FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_4))
				{
					FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_4);
					ulTick = ::GetTickCount64();
				}
				else
					bRetCode = FALSE;
			}
		}
		break;
	case 5190367:case 5190366:  case 5190121: case 5190368: case 5190123: case 5190360:
		fKillDis = 6.0f;
		if (CPerson::GetInstance().GetPercentHp() < 40 && CNpcExtend::GetInstance().FindNpc_By_Name(L"내무반장 막소보", CBaseObject::CreateFixPointInfo(10.0f), nullptr))
		{
			CONST static std::vector<Point> vPoint = {
				Point(47245, 45959, 5289), Point(47202, 45810, 5293), Point(47098, 45459, 5305), Point(46981, 45106, 5311),
				Point(46849, 44719, 5311), Point(46730, 44366, 5311), Point(46611, 44014, 5311), Point(46491, 43662, 5311),
			};

			for (UINT i = 0; i < vPoint.size() && GameRun && !EMPTY_PERSONHP; ++i)
				BnsMove(vPoint.at(i), 3);

			while (GameRun && !EMPTY_PERSONHP && CPerson::GetInstance().GetPercentHp() != 100)
			{
				CFarm::GetInstance().EatFood(90);
				BnsSleep(1000);
			}

			for (int i = static_cast<int>(vPoint.size() - 1); i >= 0 && GameRun && !EMPTY_PERSONHP; --i)
				BnsMove(vPoint.at(i), 3);
		}
		else if (CPerson::GetInstance().GetPercentHp() < 70 && FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_V))
		{
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2 * 1000, []{
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V);
				return !CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr);
			});
			BnsSleep(500);
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(3500, [&Monster, &FarmSkill]{
				Monster.SetAngle();
				CSkill::UseSkill_By_Id(Lux_SkillId_V_R, TRUE);
				return BnsSleep(100) && CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr);
			});
		}
		else
		{
			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
			BnsSleep(300);
			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
			CPersonFunction::GetInstance().Move(Monster.GetBehindPoint(5.0f));
			BnsMove(Monster.GetBehindPoint(2.0f), 1.5f);
			BnsSleep(1200);
		}
		break;
	case 5190126:
		if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_C))
		{
			LOG_C_D( L"用C躲转圈!");
			BnsSleep(1000);
			CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_C);
		}
		else if (FarmSkill.IsCool_By_SkillId_In_HideUi(Lux_SkillId_SS))
		{
			LOG_C_D( L"使用SS");
			BOOL bUseSkill = FALSE;
			Point FrontPoint = Monster.GetFrontPoint(8.0f);
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2000, [&bUseSkill, &FrontPoint]{
				if (!bUseSkill)
				{
					CFarmSkill::GetInstance().UseSkill_By_SS();
					BnsSleep(500);
					bUseSkill = TRUE;
				}
				if (CBaseObject::GetPersonDis(FrontPoint) > 2.0f)
					CPersonFunction::GetInstance().Move(FrontPoint);
				return BnsSleep(100);
			});
			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
			Monster.SetAngle();
			BnsSleep(500);
			BnsMove(NowPoint, 5);
		}
		else
		{
			LOG_C_D( L"转圈躲转圈!");
			BnsMove(Monster.GetFrontPoint(8.0f), 2);
		}
		break;
		//破天左边任务BOSS2--开始
	case 5190070:case 5190071:
		LOG_C_D( L"Tab------------");
		FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
		Sleep(800);
		break;
	case 5190074:
		LOG_C_D( L"统一走后");
		//CPersonFunction::GetInstance().Move(Monster.GetBehindPoint(2.5f));
		BnsMove(Monster.GetBehindPoint(2.5f), 3.0f);
		Monster.SetAngle();
		break;
	case 5190079:
		if (CPerson::GetInstance().GetPercentHp() < 70 && FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_V))
		{
			LOG_C_D( L"准备放V!");
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2 * 1000, []{
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V);
				return !CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr);
			});
			BnsSleep(500);
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(3500, [&Monster, &FarmSkill]{
				Monster.SetAngle();
				CSkill::UseSkill_By_Id(Lux_SkillId_V_R, TRUE);
				return BnsSleep(100) && CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr);
			});
		}
		else
		{
			LOG_C_D( L"5190079走后");
			BnsMove(Monster.GetBehindPoint(2.5f), 3.0f);
			Monster.SetAngle();
		}
		break;
	case 5190073:
		if (CPerson::GetInstance().GetPercentHp() < 70 && FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_V))
		{
			LOG_C_D( L"准备放V!");
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2 * 1000, []{
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V);
				return !CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr);
			});
			BnsSleep(500);
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(3500, [&Monster, &FarmSkill]{
				Monster.SetAngle();
				CSkill::UseSkill_By_Id(Lux_SkillId_V_R, TRUE);
				return BnsSleep(100) && CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr);
			});
		}
		else
		{
			if (Monster.GetName() == L"교도관")
			{
				CSomeFun::GetInstance().SimlationKey('S', 1000);
				Sleep(3500);
				CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100));
			}
			else
			{
				Monster.SetAngle();
				if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_2))
				{
					LOG_C_D( L"准备放2!");
					FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_2);
				}
				else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_4))
				{
					LOG_C_D( L"准备放4!");
					FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_4);
				}
				else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_Z_Attract))
				{
					LOG_C_D( L"准备放Z!");
					FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Z_Attract);
				}
				else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_1))
				{
					LOG_C_D( L"准备放1!");
					FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_1);
				}
				else //if (FarmSkill.IsCool_By_SkillId_In_HideUi(Lux_SkillId_SS))
				{
					/*LOG_C_D( L"准备SS!");
					CFarmSkill::GetInstance().UseSkill_By_SS();
					Sleep(3500);*/
					LOG_C_D( L"5190073--往sss....");
					CSomeFun::GetInstance().SimlationKey('S', 1000);
					Sleep(3500);
					LOG_C_D( L"5190073--往www....");
					CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100));
				}
			}
		}
		break;
	case 6200503:
		LOG_C_D( L"召唤怪物了,准备清怪..");
		CSkill::UseSkill_By_Id(Lux_SkillId_V_R,FALSE);
		Sleep(500);
		CSkill::UseSkill_By_Id(Lux_SkillId_V_R, FALSE);
		Sleep(500);
		CSkill::UseSkill_By_Id(Lux_SkillId_V_R, FALSE);
		Sleep(500);
		CSkill::UseSkill_By_Id(Lux_SkillId_T, FALSE);
		Sleep(500);
		CSkill::UseSkill_By_Id(Lux_SkillId_V_R, FALSE);
		Sleep(500);
		CSkill::UseSkill_By_Id(Lux_SkillId_T, FALSE);
		Sleep(1000);

		LOG_C_D( L"准备执行清怪.....");
		ClearAroundMonster(Monster, L"흑랑", CBaseObject::CreateFixPointInfo(30.0f));
		CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100));
		break;
		//破天左边任务BOSS2--结束
		//破天左边任务BOSS1--开始 力士
	case 5900400:case 5900404:
		LOG_C_D( L"统一走后");
		BnsMove(Monster.GetBehindPoint(2.0f), 3.0f);
		Monster.SetAngle();
		break;
	case 5900403:
		Monster.SetAngle();
		if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_C))
		{
			LOG_C_D( L"用C躲转圈!");
			CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
			Sleep(500);
			CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
			Sleep(500);
			CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_C);
			Sleep(1700);
			LOG_C_D( L"用C后走背后。");
			BnsMove(Monster.GetBehindPoint(2.0f), 3.0f);
			Monster.SetAngle();
		}
		else if (FarmSkill.IsCool_By_SkillId_In_HideUi(Lux_SkillId_SS))
		{
			LOG_C_D( L"用SS躲转圈!");
			CFarmSkill::GetInstance().UseSkill_By_SS();
			Sleep(3000);
		}
		else
		{
			LOG_C_D( L"准备放tab!");
			CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
			Sleep(400);
			CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
			Sleep(400);
			CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
			Sleep(300);
			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
			Sleep(800);
		}
		break;
	case 5900410:
		Monster.SetAngle();
		if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_2))
		{
			LOG_C_D( L"准备放2!");
			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_2);
		}
		else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_4))
		{
			LOG_C_D( L"准备放4!");
			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_4);
		}
		else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_Z_Attract))
		{
			LOG_C_D( L"准备放Z!");
			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Z_Attract);
		}
		else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_1))
		{
			LOG_C_D( L"准备放1!");
			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_1);
		}
		else
		{
			LOG_C_D( L"准备放tab!");
			CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
			Sleep(400);
			CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
			Sleep(400);
			CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
			Sleep(400);
			CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
			Sleep(400);
			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
			Sleep(800);
		}
		break;
	case 5900401:
		if (CPerson::GetInstance().GetPercentHp() < 70 && FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_V))
		{
			LOG_C_D( L"准备放V!");
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2 * 1000, []{
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V);
				return !CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr);
			});
			BnsSleep(500);
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(3500, [&Monster, &FarmSkill]{
				Monster.SetAngle();
				CSkill::UseSkill_By_Id(Lux_SkillId_V_R, TRUE);
				return BnsSleep(100) && CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr);
			});
		}
		else
		{
			Monster.SetAngle();
			if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_1))
			{
				LOG_C_D( L"准备放1!");
				FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_1);
			}
			else
			{
				LOG_C_D( L"准备放tab!");
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
				Sleep(400);
				//CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
				//Sleep(400);
				FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
				Sleep(800);
			}
		}
		break;
		//破天左边任务BOSS1--结束

		//破天左边任务BOSS3-开始
	case 5101100:case 5101101:case 5101102:case 5101103:case 5101104:case 5101106:case 5101131:
		if (CPerson::GetInstance().GetPercentHp() < 70 && FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_V))
		{
			LOG_C_D( L"准备放V!");
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2 * 1000, []{
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V);
				return !CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr);
			});
			BnsSleep(500);
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(3500, [&Monster, &FarmSkill]{
				Monster.SetAngle();
				CSkill::UseSkill_By_Id(Lux_SkillId_V_R, TRUE);
				return BnsSleep(100) && CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr);
			});
		}
		else
		{
			LOG_C_D( L"统一走后");
			BnsMove(Monster.GetBehindPoint(2.0f), 3.0f);
			Monster.SetAngle();
		}

		break;
	case 5101105:
		if (CPerson::GetInstance().GetPercentHp() < 70 && FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_V))
		{
			LOG_C_D( L"准备放V!");
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2 * 1000, []{
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V);
				return !CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr);
			});
			BnsSleep(500);
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(3500, [&Monster, &FarmSkill]{
				Monster.SetAngle();
				CSkill::UseSkill_By_Id(Lux_SkillId_V_R, TRUE);
				return BnsSleep(100) && CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr);
			});
		}
		else
		{
			Monster.SetAngle();
			if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_2))
			{
				LOG_C_D( L"准备放2!");
				FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_2);
			}
			else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_4))
			{
				LOG_C_D( L"准备放4!");
				FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_4);
			}
			else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_Z_Attract))
			{
				LOG_C_D( L"准备放Z!");
				FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Z_Attract);
			}
			else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_1))
			{
				LOG_C_D( L"准备放1!");
				FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_1);
			}
			else
			{
				LOG_C_D( L"准备放tab!");
				FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
				Sleep(400);
				FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
				Sleep(800);
				FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
				Sleep(800);
			}
		}

		break;
		//破天左边任务BOSS3-结束

		//破天右边任务BOSS1-开始
	case 5190600:
		if (CPerson::GetInstance().GetPercentHp() < 70 && FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_V))
		{
			LOG_C_D( L"准备放V!");
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2 * 1000, []{
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V);
				return !CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr);
			});
			while (CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr))
			{
				Monster.SetAngle();
				CSkill::UseSkill_By_Id(Lux_SkillId_V_R, TRUE);
				Sleep(100);
			}
		}
		else
		{
			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
			Sleep(500);
		}
		break;
	case 5190604:
		LOG_C_D( L"走到后面爆菊花");
		CPersonFunction::GetInstance().Move(Monster.GetBehindPoint(2.0f));
		BnsMove(Monster.GetBehindPoint(2.0f), 1.0f);
		Monster.SetAngle();
		CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
		Sleep(400);
		Monster.SetAngle();
		CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
		Sleep(400);
		Monster.SetAngle();
		CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
		Sleep(400);
		break;
	case 5190601:
		LOG_C_D( L"统一走后");
		CPersonFunction::GetInstance().Move(Monster.GetBehindPoint(2.0f));
		BnsMove(Monster.GetBehindPoint(2.0f), 1.0f);
		Monster.SetAngle();
		break;
	case 5190606:case 5190602:
		Monster.SetAngle();

		if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_2))
		{
			LOG_C_D( L"准备放2!");
			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_2);
		}
		else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_4))
		{
			LOG_C_D( L"准备放4!");
			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_4);
		}
		else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_Z_Attract))
		{
			LOG_C_D( L"准备放Z!");
			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Z_Attract);
		}
		else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_1))
		{
			LOG_C_D( L"准备放1!");
			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_1);
		}
		else
		{
			LOG_C_D( L"准备放tab!");
			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
			Sleep(1000);
		}
		break;
	case 5190603:
		Monster.SetAngle();
		if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_2))
		{
			LOG_C_D( L"准备放2!");
			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_2);
		}
		else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_4))
		{
			LOG_C_D( L"准备放4!");
			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_4);
		}
		else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_Z_Attract))
		{
			LOG_C_D( L"准备放Z!");
			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Z_Attract);
		}
		else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_1))
		{
			LOG_C_D( L"准备放1!");
			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_1);
		}
		else
		{
			LOG_C_D( L"准备放tab!");
			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
			Sleep(800);
			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
			Sleep(800);
		}
		break;
	case 5190607://case 5190608:这个是远距离把人抓过来的技能。
		if (CPerson::GetInstance().GetPercentHp() < 70 && FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_V))
		{
			LOG_C_D( L"准备放V!");
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2 * 1000, []{
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V);
				return !CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr);
			});
			while (CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr))
			{
				Monster.SetAngle();
				CSkill::UseSkill_By_Id(Lux_SkillId_V_R, TRUE);
			}
		}
		else
		{
			Monster.SetAngle();
			if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_C))
			{
				LOG_C_D( L"使用C技能。!");
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
				Sleep(300);
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
				Sleep(300);
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_C);
				Sleep(1500);
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_C_c);
				Sleep(500);
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(0x5e13);
			}
			else if (FarmSkill.IsCool_By_SkillId_In_HideUi(Lux_SkillId_SS))
			{
				LOG_C_D( L"使用SS!");
				CFarmSkill::GetInstance().UseSkill_By_SS();
				Sleep(3000);
			}
			else
			{
				LOG_C_D( L"准备放tab!");
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
				Sleep(300);
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
				Sleep(300);
				FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
				Sleep(1000);
			}
		}
		break;
		//破天右边任务BOSS1-结束

		//破天右边任务BOSS2-开始
	case 5090680:
		if (CPerson::GetInstance().GetPercentHp() < 70 && FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_V))
		{
			LOG_C_D( L"准备放V!");
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2 * 1000, []{
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V);
				return !CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr);
			});
			while (CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr))
			{
				Monster.SetAngle();
				CSkill::UseSkill_By_Id(Lux_SkillId_V_R, TRUE);
				Sleep(100);
			}
		}
		else
		{
			CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
			CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
			Sleep(800);
		}
		break;
	case 5090690:case 5090681:
		LOG_C_D( L"统一走后");
		CPersonFunction::GetInstance().Move(Monster.GetBehindPoint(2.0f));
		BnsMove(Monster.GetBehindPoint(2.0f), 1.0f);
		Monster.SetAngle();
		break;

	case 5090687:case 5090682:
		if (CPerson::GetInstance().GetPercentHp() < 70 && FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_V))
		{
			LOG_C_D( L"准备放V!");
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2 * 1000, []{
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V);
				return !CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr);
			});
			while (CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr))
			{
				Monster.SetAngle();
				CSkill::UseSkill_By_Id(Lux_SkillId_V_R, TRUE);
				Sleep(100);
			}
		}
		else
		{
			Monster.SetAngle();

			if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_2))
			{
				LOG_C_D( L"准备放2!");
				FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_2);
			}
			else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_4))
			{
				LOG_C_D( L"准备放4!");
				FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_4);
			}
			else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_Z_Attract))
			{
				LOG_C_D( L"准备放Z!");
				FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Z_Attract);
			}
			else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_1))
			{
				LOG_C_D( L"准备放1!");
				FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_1);
			}
			else
			{
				LOG_C_D( L"准备放tab!");
				FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
				Sleep(400);
				FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
				Sleep(400);
				FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
				Sleep(1000);
			}
		}
		break;
	case 5090683:
		Monster.SetAngle();
		if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_C))
		{
			LOG_C_D( L"使用C技能。!");
			CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
			Sleep(300);
			CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
			Sleep(300);
			CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_C);
			Sleep(1500);
			CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_C_c);
			Sleep(500);
			CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(0x5e13);
		}
		else
		{
			LOG_C_D( L"准备放tab!");
			CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
			Sleep(300);
			CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
			Sleep(300);
			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
			Sleep(1000);
		}
		break;
		//破天右边任务BOSS2-结束

		//破天右边每日任务BOSS2-开始

	case 5101236: 
		if (CPerson::GetInstance().GetPercentHp() < 70 && FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_V))
		{
			LOG_C_D( L"准备放V!");
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2 * 1000, []{
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V);
				return !CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr);
			});
			while (CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr))
			{
				Monster.SetAngle();
				CSkill::UseSkill_By_Id(Lux_SkillId_V_R, TRUE);
				Sleep(100);
			}
		}
		else
		{
			Monster.SetAngle();

			if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_2))
			{
				LOG_C_D( L"准备放2!");
				FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_2);
			}
			else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_4))
			{
				LOG_C_D( L"准备放4!");
				FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_4);
			}
			else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_Z_Attract))
			{
				LOG_C_D( L"准备放Z!");
				FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Z_Attract);
			}
			else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_1))
			{
				LOG_C_D( L"准备放1!");
				FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_1);
			}
			else
			{
				LOG_C_D( L"准备放tab!");
				FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
				Sleep(400);
				FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
				Sleep(400);
				FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
				Sleep(1000);
			}
		}
		break;
	case 5101239:case 5101235:case 5101234:case 5101240:case 5101242:
		//5101239直

		LOG_C_D( L"统一走后");
		CPersonFunction::GetInstance().Move(Monster.GetBehindPoint(2.0f));
		BnsMove(Monster.GetBehindPoint(2.0f), 3.0f);
		Monster.SetAngle();
		break;
		/*
	case 5101238:
	Monster.SetAngle();
	if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_C))
	{
	LOG_C_D( L"使用C技能。!");
	CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
	Sleep(300);
	CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_C);
	Sleep(1000);
	CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_C_c);
	Sleep(500);
	CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(0x5e13);
	}
	else
	{
	LOG_C_D( L"准备放tab!");
	CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
	Sleep(300);
	FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
	Sleep(1000);
	}
	break;
	*/
		//破天右边每日任务BOSS2-结束

		//2B7最后一个BOSS-开始
	case 5190196:case 5190197:
		LOG_C_D( L"2B7统一走后");
		CPersonFunction::GetInstance().Move(Monster.GetBehindPoint(2.0f));
		BnsMove(Monster.GetBehindPoint(2.0f), 1.0f);
		Monster.SetAngle();
		break;
	case 5190193:case 6200291:
		if (CPerson::GetInstance().GetPercentHp() < 70 && FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_V))
		{
			LOG_C_D( L"准备放V!");
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2 * 1000, []{
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V);
				return !CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr);
			});
			while (CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr))
			{
				Monster.SetAngle();
				CSkill::UseSkill_By_Id(Lux_SkillId_V_R, TRUE);
				Sleep(100);
			}
		}
		else
		{
			Monster.SetAngle();
			LOG_C_D( L"2B7准备放tab!");
			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
			Sleep(400);
			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
			Sleep(800);
		}
		break;
	case 5190194:
		if (CPerson::GetInstance().GetPercentHp() < 70 && FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_V))
		{
			LOG_C_D( L"准备放V!");
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2 * 1000, []{
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V);
				return !CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr);
			});
			while (CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr))
			{
				Monster.SetAngle();
				CSkill::UseSkill_By_Id(Lux_SkillId_V_R, TRUE);
			}
		}
		else
		{
			Monster.SetAngle();
			if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_C))
			{
				LOG_C_D( L"2B7使用C技能。!");
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
				Sleep(300);
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
				Sleep(300);
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
				Sleep(300);
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_C);
				Sleep(2500);
			}
			else
			{
				LOG_C_D( L"2B7准备放tab!");
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
				Sleep(300);
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
				Sleep(300);
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
				Sleep(300);
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
				Sleep(300);
				FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
				Sleep(1000);
			}
		}
		break;
	case 5190245:
		LOG_C_D( L"2B7,不管继续打。!");
		break;
		//2B7最后一个BOSS-结束
		//3e4-6-boss-开始-----------
	case 5190170:
		LOG_C_D( L"3e4,不管继续打。!");
		break;
	case 5190171:case 5190177:case 6200156:case 5190172:
		if (CPerson::GetInstance().GetPercentHp() < 70 && FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_V))
		{
			LOG_C_D( L"准备放V!");
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2 * 1000, []{
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V);
				return !CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr);
			});
			while (CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr))
			{
				Monster.SetAngle();
				CSkill::UseSkill_By_Id(Lux_SkillId_V_R, TRUE);
			}
		}
		else
		{
			LOG_C_D( L"3e4统一走后");
			CPersonFunction::GetInstance().Move(Monster.GetBehindPoint(2.0f));
			BnsMove(Monster.GetBehindPoint(2.0f), 1.0f);
			Monster.SetAngle();
		}
		break;
	case 5190310:
		if (CPerson::GetInstance().GetPercentHp() < 70 && FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_V))
		{
			LOG_C_D( L"准备放V!");
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2 * 1000, []{
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V);
				return !CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr);
			});
			while (CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr))
			{
				Monster.SetAngle();
				CSkill::UseSkill_By_Id(Lux_SkillId_V_R, TRUE);
			}
		}
		else
		{
			LOG_C_D( L"3e4准备放tab!");
			CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
			Sleep(300);
			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
			Sleep(1000);
		}
		break;
		//3e4-6-boss-结束-----------

		//三天王洞开始-----------------------
	case 5190160:
		if (CPerson::GetInstance().GetPercentHp() < 70 && FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_V))
		{
			LOG_C_D( L"三天王洞-----准备放V!");
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2 * 1000, []{
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V);
				return !CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr);
			});
			while (CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr))
			{
				Monster.SetAngle();
				CSkill::UseSkill_By_Id(Lux_SkillId_V_R, TRUE);
			}
		}
		else
		{
			LOG_C_D( L"三天王洞-----准备放tab!");
			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
			Sleep(500);
		}
		break;
	case 5190161:case 5190162:case 5190163:
		LOG_C_D( L"三天王洞-----统一走后");
		CPersonFunction::GetInstance().Move(Monster.GetBehindPoint(2.0f));
		BnsMove(Monster.GetBehindPoint(2.0f), 1.0f);
		Monster.SetAngle();
		break;
	case 5190168:case 5190179:
		
			if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_C))
			{
				LOG_C_D( L"三天王洞-----准备放c!");
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_C);
				Sleep(1000);
			}
			else
			{
				LOG_C_D( L"三天王洞-----准备放tab!");
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V_R);
				Sleep(300);
				FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
				Sleep(1000);
			}
		break;


		//三天王洞结束-----------------------


		//雪人洞老三开始------------------
	case 5902251:
		//if (Monster.GetTargetId() == CPerson::GetInstance().GetId())
		//{
			CSkill::UseSkill_By_Id(Lux_SkillId_V_R, FALSE);
			Sleep(300);
			CSkill::UseSkill_By_Id(Lux_SkillId_R, FALSE);
			Sleep(400);
			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
			Sleep(800);
		//}
		//else
		//{
		//	LOG_C_D( L"雪人洞--目标不是我---统一走后");
		//	CPersonFunction::GetInstance().Move(Monster.GetBehindPoint(2.0f));
		//	BnsMove(Monster.GetBehindPoint(2.0f), 1.0f);
		//	Monster.SetAngle();
		//}
		//
		break;
	case 5902250:
		CSkill::UseSkill_By_Id(Lux_SkillId_V_R, FALSE);
		Sleep(300);
		FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
		Sleep(1000);
		break;
	case 5902252:
		//if (Monster.GetTargetId() == CPerson::GetInstance().GetId())
		//{
			LOG_C_D( L"释放tab 500");
			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
			Sleep(600);
			/*}
			else
			{
			LOG_C_D( L"雪人洞--目标不是我---统一走后");
			CPersonFunction::GetInstance().Move(Monster.GetBehindPoint(2.0f));
			BnsMove(Monster.GetBehindPoint(2.0f), 1.0f);
			Monster.SetAngle();
			}*/
		break;
	case 5902253:
		LOG_C_D( L"S,600 跳目标");
		CSomeFun::GetInstance().SetSpeed(3.0f);
		CSomeFun::GetInstance().SimlationKey('S', 700);
		if (CPerson::GetInstance().GetPercentHp() < 85 && FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_V))
		{
			LOG_C_D( L"雪人洞-----准备放V!");
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2 * 1000, []{
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V);
				return !CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr);
			});
			CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100));
			CSkill::UseSkill_By_Id(Lux_SkillId_V_R);
			Sleep(500);
			CSkill::UseSkill_By_Id(Lux_SkillId_V_R);
			Sleep(500);
			CSkill::UseSkill_By_Id(Lux_SkillId_V_R);
		}
		else
		{
			Sleep(1000);
			CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100));
			CSkill::UseSkill_By_Id(Lux_SkillId_V_R);
			Sleep(500);
			CSkill::UseSkill_By_Id(Lux_SkillId_V_R);
		}
		CSomeFun::GetInstance().SetSpeed(3.0f);
		break;
	case 5902256:
		LOG_C_D( L"5902256-----");
		FarmSkill.UseSkill_By_SS();
		//CSomeFun::GetInstance().SetSpeed(3.0f);
		//CSomeFun::GetInstance().SimlationKey('S', 1200);
		Sleep(1000);
		//CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100));
		//CSomeFun::GetInstance().SetSpeed(3.0f);
		break;
	case 5902254:
		//LOG_C_D( L"Xueren_5902254=%d", Xueren_5902254);
		//if (Xueren_5902254 % 2 == 0)
		//{

			BnsMove(TarPoint, 1.0f);
			Monster.SetAngle();
			GetMonsterSkillId() = 0;
			LOG_C_D( L"等它后退1.6秒");
			Sleep(1600);
			LOG_C_D( L"当前技能---%d", GetMonsterSkillId());
			if (GetMonsterSkillId() == 5902252 || GetMonsterSkillId() == 0)
			{
				Sleep(700);
				FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
				Sleep(800);
				CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100));
			}
			else if (GetMonsterSkillId() == 5902256)
			{
				CSomeFun::GetInstance().SetSpeed(3.0f);
				CSomeFun::GetInstance().SimlationKey('S', 1200);
				FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
				Sleep(1000);
				CSomeFun::GetInstance().SetSpeed(3.0f);
			}
			else if (GetMonsterSkillId() == 5902257)
			{
				LOG_C_D( L"5902254中的5902257------%d", Xueren_flag);

				if (Xueren_flag % 2 == 0)
				{
					CSomeFun::GetInstance().SetSpeed(3.0f);
					LOG_C_D( L"准备控制....");
					if (CPersonFunction::GetInstance().ExistBuff_Person(0x3849D45, (GAMEOBJ_BUFF*)nullptr))
					{
						BnsMove(BossPoint, 1.0f);
						Sleep(1500);
						LOG_C_D( L"对准放2....");
						Monster.SetAngle();
						Sleep(1100);
						FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_2);
					}
					else
					{
						BnsMove(TarPoint, 1.0f);
						Sleep(500);
						BnsMove(BossPoint, 1.0f);
						Sleep(1700);
						LOG_C_D( L"对准放2....");
						Monster.SetAngle();
						Sleep(1100);
						FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_2);
					}
				}
				else
				{
					LOG_C_D( L"S---等2.5秒-----------------第1次");
					if (CPerson::GetInstance().GetPercentHp() < 75 && FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_V))
					{
						LOG_C_D( L"雪人洞-----准备放V!");
						BnsMove(BossPoint, 1.0f);
						Monster.SetAngle();
						Sleep(1000);
						CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2 * 1000, []{
							CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V);
							return !CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr);
						});
						Sleep(1300);
					}
					else
					{
						BnsMove(BossPoint, 1.0f);
						Monster.SetAngle();
						Sleep(2800);
					}
				}
				CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100));
				Xueren_flag++;
				CSomeFun::GetInstance().SetSpeed(3.0f);
			}

		break;
	case 5902257:
		//LOG_C_D( L"走到(-49884,24636,14462);");
		//LOG_C_D( L"走到(%d,%d,%d),等2秒", TarPoint.GetX(), TarPoint.GetY(), TarPoint.GetZ());
		LOG_C_D( L"5902257------%d", Xueren_flag);
		
		if (Xueren_flag % 2 == 0)
		{
			CSomeFun::GetInstance().SetSpeed(3.0f);
			LOG_C_D( L"准备控制....");
			if (CPersonFunction::GetInstance().ExistBuff_Person(0x3849D45, (GAMEOBJ_BUFF*)nullptr))
			{
				BnsMove(BossPoint, 1.0f);
				Sleep(1500);
				LOG_C_D( L"对准放2....");
				Monster.SetAngle();
				Sleep(1100);
				FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_2);
			}
			else
			{
				BnsMove(TarPoint, 1.0f);
				Sleep(600);
				BnsMove(BossPoint, 1.0f);
				Sleep(1600);
				LOG_C_D( L"对准放2....");
				Monster.SetAngle();
				Sleep(1100);
				FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_2);
			}
		}
		else
		{
			LOG_C_D( L"S---等2.5秒-----------------第1次");
			if (CPerson::GetInstance().GetPercentHp() < 75 && FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_V))
			{
				LOG_C_D( L"雪人洞-----准备放V!");
				BnsMove(BossPoint, 1.0f);
				Monster.SetAngle();
				Sleep(1000);
				CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2 * 1000, []{
					CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V);
					return !CPersonFunction::GetInstance().ExistBuff_Person(0x16F206D, nullptr);
				});
				Sleep(1300);
			}
			else
			{
				BnsMove(BossPoint, 1.0f);
				Monster.SetAngle();
				Sleep(2800);
			}
		}
		CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100));
		Xueren_flag++;
		CSomeFun::GetInstance().SetSpeed(3.0f);
		
		break;
	case 5902265:
		LOG_C_D( L"雪人王-不能控制,放无敌");
		//CSomeFun::GetInstance().SetSpeed(3.0f);
		//if (Xueren_2265 == 3)
		//{
			CSkill::UseSkill_By_Id(Lux_SkillId_V_R, FALSE);
			Sleep(400);
			CSkill::UseSkill_By_Id(Lux_SkillId_R, FALSE);
			Sleep(400);
			CSkill::UseSkill_By_Id(Lux_SkillId_V_R, FALSE);
			Sleep(400);
			CSkill::UseSkill_By_Id(Lux_SkillId_R, FALSE);
			Sleep(400);
			CSkill::UseSkill_By_Id(Lux_SkillId_V_R, FALSE);
			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_1);
		//}
		//else
		//{
		//	CSkill::UseSkill_By_Id(Lux_SkillId_V_R, FALSE);
		//	Sleep(400);
		//	CSkill::UseSkill_By_Id(Lux_SkillId_R, FALSE);
		//	Sleep(400);
		//	CSkill::UseSkill_By_Id(Lux_SkillId_V_R, FALSE);
		//	Sleep(400);
		//	CSkill::UseSkill_By_Id(Lux_SkillId_R, FALSE);
		//	Sleep(400);
		//	CSkill::UseSkill_By_Id(Lux_SkillId_V_R, FALSE);
		//	FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_C);
		//	Sleep(500);
		//}
		Xueren_2265++;
		break;
	case 5902259:
		LOG_C_D( L"吸-------tab");
		CSkill::UseSkill_By_Id(Lux_SkillId_V_R, FALSE);
		Sleep(300);
		CSkill::UseSkill_By_Id(Lux_SkillId_R, FALSE);
		Sleep(400);
		FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
		Sleep(800);
		break;
	case 5902258:
		LOG_C_D( L"吸热----");
		//FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_C);
		if (Monster.GetTargetId() == CPerson::GetInstance().GetTargetId())
		{
			LOG_C_D( L"等5.5秒-------");
			Sleep(5500);
		}
		break;
	case 5902260:
		LOG_C_D( L"摆姿势---");
		break;
	case 5902261:
		LOG_C_D( L"检查周围冰块的位置----准备执行清怪");
		CSkill::UseSkill_By_Id(Lux_SkillId_V_R, FALSE);
		Sleep(300);
		CSkill::UseSkill_By_Id(Lux_SkillId_R, FALSE);
		Sleep(400);
		CSkill::UseSkill_By_Id(Lux_SkillId_T, FALSE);
		Sleep(300);
		CSkill::UseSkill_By_Id(Lux_SkillId_V_R, FALSE);
		Sleep(300);
		CSkill::UseSkill_By_Id(Lux_SkillId_R, FALSE);
		Sleep(400);
		CSkill::UseSkill_By_Id(Lux_SkillId_T, FALSE);
		Sleep(300);
		CSkill::UseSkill_By_Id(Lux_SkillId_V_R, FALSE);
		Sleep(300);
		CSkill::UseSkill_By_Id(Lux_SkillId_R, FALSE);
		Sleep(400);
		CSkill::UseSkill_By_Id(Lux_SkillId_T, FALSE);
		Sleep(300);
		CSkill::UseSkill_By_Id(Lux_SkillId_V_R, FALSE);
		Sleep(300);
		CSkill::UseSkill_By_Id(Lux_SkillId_R, FALSE);
		Sleep(400);
		CSkill::UseSkill_By_Id(Lux_SkillId_T, FALSE);
		Sleep(300);
		CSkill::UseSkill_By_Id(Lux_SkillId_V_R, FALSE);
		Sleep(300);
		CSkill::UseSkill_By_Id(Lux_SkillId_R, FALSE);
		Sleep(400);
		CSkill::UseSkill_By_Id(Lux_SkillId_T, FALSE);
		Sleep(300);
		CFarmBoss::GetInstance().MoveToLightBySnowBoss(Monster);
		//Sleep(2000);
		//CSomeFun::GetInstance().SetSpeed(3.0f);
		//TeamIndex = CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex();
		//if (TeamIndex == 0)
		//{
		//	BnsMove(xueren_going1, 1.0f);
		//	CSomeFun::GetInstance().SimlationKey('A',600);
		//	/*BnsMove(xueren_point1, 1.0f);
		//	ClearAroundMonster_ts(Monster, L"얼음 뱀버섯", CBaseObject::CreateFixPointInfo(5.0f));
		//	Sleep(1000);
		//	CSomeFun::GetInstance().SimlationKey('F');
		//	CSomeFun::GetInstance().SimlationKey('F');

		//	Monster.SetAngle();
		//	Sleep(200);
		//	CSomeFun::GetInstance().SimlationKey('R', 100);*/
		//}
		//else if (TeamIndex == 1)
		//{
		//	BnsMove(xueren_going2, 1.0f);
		//	CSomeFun::GetInstance().SimlationKey('A', 600);
		//	/*BnsMove(xueren_point2, 1.0f);
		//	ClearAroundMonster_ts(Monster, L"얼음 뱀버섯", CBaseObject::CreateFixPointInfo(5.0f));
		//	Sleep(1000);
		//	CSomeFun::GetInstance().SimlationKey('F');
		//	CSomeFun::GetInstance().SimlationKey('F');

		//	Monster.SetAngle();
		//	Sleep(200);
		//	CSomeFun::GetInstance().SimlationKey('R', 100);*/
		//}
		//else if (TeamIndex == 2)
		//{
		//	BnsMove(xueren_going3, 1.0f);
		//	CSomeFun::GetInstance().SimlationKey('A', 600);
		//	/*BnsMove(xueren_point4, 1.0f);
		//	ClearAroundMonster_ts(Monster, L"얼음 뱀버섯", CBaseObject::CreateFixPointInfo(5.0f));
		//	Sleep(1000);
		//	CSomeFun::GetInstance().SimlationKey('F');
		//	CSomeFun::GetInstance().SimlationKey('F');

		//	Monster.SetAngle();
		//	Sleep(200);
		//	CSomeFun::GetInstance().SimlationKey('R', 100);*/
		//}
		//else if (TeamIndex == 3)
		//{
		//	BnsMove(xueren_going4, 1.0f);
		//	CSomeFun::GetInstance().SimlationKey('A', 600);
		//	/*BnsMove(xueren_point6, 1.0f);
		//	ClearAroundMonster_ts(Monster, L"얼음 뱀버섯", CBaseObject::CreateFixPointInfo(5.0f));
		//	Sleep(1000);
		//	CSomeFun::GetInstance().SimlationKey('F');
		//	CSomeFun::GetInstance().SimlationKey('F');
		//	Monster.SetAngle();
		//	Sleep(200);
		//	CSomeFun::GetInstance().SimlationKey('R', 100);*/
		//}
		//else if (TeamIndex == 4)
		//{
		//	BnsMove(xueren_going5, 1.0f);
		//	CSomeFun::GetInstance().SimlationKey('A', 600);
		//	/*BnsMove(xueren_point7, 1.0f);
		//	ClearAroundMonster_ts(Monster, L"얼음 뱀버섯", CBaseObject::CreateFixPointInfo(5.0f));
		//	Sleep(1000);
		//	CSomeFun::GetInstance().SimlationKey('F');
		//	CSomeFun::GetInstance().SimlationKey('F');

		//	Monster.SetAngle();
		//	Sleep(200);
		//	CSomeFun::GetInstance().SimlationKey('R', 100);*/
		//}
		//else if (TeamIndex == 5)
		//{
		//	BnsMove(xueren_going6, 1.0f);
		//	CSomeFun::GetInstance().SimlationKey('A', 600);
		//	/*BnsMove(xueren_point8, 1.0f);
		//	ClearAroundMonster_ts(Monster, L"얼음 뱀버섯", CBaseObject::CreateFixPointInfo(5.0f));
		//	Sleep(1000);
		//	CSomeFun::GetInstance().SimlationKey('F');
		//	CSomeFun::GetInstance().SimlationKey('F');

		//	Monster.SetAngle();
		//	Sleep(200);
		//	CSomeFun::GetInstance().SimlationKey('R', 100);*/
		//}
		//CSomeFun::GetInstance().SetSpeed(3.0f);
		break;
	case 5902262:
		LOG_C_D( L"CCCC----");
		CSkill::UseSkill_By_Id(Lux_SkillId_V_R, FALSE);
		Sleep(300);
		CSkill::UseSkill_By_Id(Lux_SkillId_R, FALSE);
		Sleep(400);
		FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_C);
		Xueren_2263++;
		break;
	case 5902263:case 5902264:
		LOG_C_D( L"8连开始----");
		CSomeFun::GetInstance().SimlationKey('E',120);
		CSomeFun::GetInstance().SimlationKey('E',120);
		//CSkill::UseSkill_By_Id(Lux_SkillId_E, FALSE);
		/*if (Xueren_2263 > 1)
		{
		if (FarmSkill.IsCool_By_SkillId_In_HideUi(Lux_SkillId_E))
		{
		FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_E);
		}
		else
		{
		Sleep(500);
		FarmSkill.UseSkill_By_SS();
		if (FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_1))
		{
		FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_1);
		}
		else if (FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_2))
		{
		FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_2);
		}
		else if (FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_3))
		{
		FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_3);
		}
		}
		}
		else
		{
		Sleep(500);
		FarmSkill.UseSkill_By_SS();
		if (FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_1))
		{
		FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_1);
		}
		else if (FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_2))
		{
		FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_2);
		}
		else if (FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_3))
		{
		FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_3);
		}
		}
		*/
		
		break;
		//雪人洞老三结束------------------
	case 0:
		//LOG_C_D( L"当前技能=0");
		bRetCode = FALSE;
		break;
	default:
		//LOG_C_D( L"default，释放TAB,延迟1.2,当前怪物技能=%d", dwBossSkillId);
		//CSkill::UseSkill_By_Id(Lux_SkillId_Tab, TRUE);
		///BnsSleep(1200);
		bRetCode = FALSE;
		break;
	}
	VMProtectEnd();
	return bRetCode;
}

BOOL CFarmBoss::KillBoss_Lux(_In_ CMonster& Monster, _In_ ULONGLONG dwMaxout_LoseHp)
{
	auto dwHp = Monster.GetHp();
	auto ulLastHpTick = ::GetTickCount64();
	auto& FarmSkill = CFarmSkill::GetInstance();


	while (GameRun && !EMPTY_PERSONHP && !Monster.IsDead() && CFarm::GetInstance().IsLoseHp(Monster, dwHp, ulLastHpTick, dwMaxout_LoseHp))
	{
		if (GetMonsterSkillId() == 0)
		{
			//LOG_C_D( L"当前BOSS-id=%d!", GetMonsterSkillId());
				if (!CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(200)))
				{
					LOG_C(MyTools::CLog::em_Log_Type::em_Log_Type_Exception, L"距离BOSS超过100米了, 不追了!");
					return FALSE;
				}
		}
		
		if (CPerson::GetInstance().GetTargetId() != Monster.GetId())
			Monster.SetAngle();

		// bRetCode = FALSE 表示并没有施放对应的BOSS技能
		if (UseSkill_By_BossSkillId_Lux(Monster, GetMonsterSkillId()))
		{
			LOG_C_D( L"处理完毕0000!");
			GetMonsterSkillId() = 0;
			continue;
		}

		if (Monster.GetName() != L"초롱설인")
		{
			if (CPerson::GetInstance().GetMp() >= 50 && FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_3))
			{
				LOG_C_D( L"力士使用3!");
				FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_3, FALSE);
				//continue;
			}
			// 使用了E后, 只用RT!, 不用其他技能(除了V)
			if (FarmSkill.UseSkill_By_Destoer_E())
			{
				LOG_C_D( L"力士使用E愤怒!");
				continue;
			}
		}
		

		if (CPerson::GetInstance().GetPercentHp() <= 90 && CPerson::GetInstance().GetMp() >= 60 && FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_X))
		{
			LOG_C_D( L"力士使用X!");
			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_X);
		}

		if (CPerson::GetInstance().GetMp() >= 60)
		{
			CSkill::UseSkill_By_Id(Lux_SkillId_T);
			BnsSleep(100);
			if (CPersonFunction::GetInstance().ExistBuff_Person(0x16F9595, (GAMEOBJ_BUFF*)nullptr))
				CSkill::UseSkill_By_Id(Lux_SkillId_E_R);
			else
				CSkill::UseSkill_By_Id(Lux_SkillId_R);
		}
		else
		{
			CSkill::UseSkill_By_Id(Lux_SkillId_R);
			BnsSleep(100);
		}
	}

	return TRUE;
}

BOOL CFarmBoss::KillBoss_2B7_B(_In_ CONST CMonster& Monster)
{
	CONST static std::vector<Point> vPoint = 
	{
		Point(-30948, 94956, -384), Point(-31492, 94772, -384), Point(-31832, 94184, -384), Point(-31456, 93608, -384),
		Point(-30992, 93316, -384), Point(-30316, 93500, -384), Point(-30128, 94252, -384), Point(-30436, 94696, -384)
	};

	UINT nIndex = 0;
	while (!Monster.IsDead() && GameRun && !EMPTY_PERSONHP)
	{
		BnsSleep(100);
		if (CPerson::GetInstance().GetFightPose() == em_Player_Pose_KongFuMaster)
		{
			/*if (FarmSkill.IsCool_By_SkillId(Kffm_Fm_SkillId_SS))
			{
			FarmSkill.UseSkill_By_SkillId(Kffm_Fm_SkillId_SS);
			continue;
			}*/
			CSomeFun::GetInstance().SimlationKey(VK_TAB, 200);
			continue;
		}

		while (CBaseObject::GetPersonDis(vPoint.at(nIndex)) > 3.0f && GameRun && !EMPTY_PERSONHP)
		{
			CPersonFunction::GetInstance().Move(vPoint.at(nIndex));
			BnsSleep(500);
			CPersonFunction::GetInstance().Move(CBaseObject::GetPersonPoint());
			BnsSleep(300);

			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2 * 1000, [&Monster]{
				Monster.SetAngle();
				/*	if (CFarm::GetInstance().kffm_UseForeMasterSkill(Monster))
						return TRUE;*/

				if (CPerson::GetInstance().GetMp() > 2)
					CSkill::UseSkill_By_Id(Kffm_Fm_SkillId_T, TRUE);
				else
					CSkill::UseSkill_By_Id(Kffm_Fm_SkillId_R, TRUE);
				return TRUE;
			});


		}
		if (++nIndex >= vPoint.size())
			nIndex = 0;
	}
	return TRUE;
}

BOOL CFarmBoss::ExistSpecialMonsterName(_In_ CONST CMonster& Monster)
{
	struct _SpecialBossPtr
	{
		std::wstring wsBossName;
		std::function<BOOL(CONST CMonster&)> fn;
		DWORD dwHash;
	};

	CONST static std::vector<_SpecialBossPtr> vlst = {
		{ L"탁기에 잠식된 왕수", std::bind(&CFarmBoss::KillBoss_2B7_B, this, std::placeholders::_1), MyTools::CLPublic::GetHash(L"탁기에 잠식된 왕수") }
	};

	DWORD dwHash = MyTools::CLPublic::GetHash(Monster.GetName());

	auto Ptr = MyTools::CLPublic::Vec_find_if_Const(vlst, [&dwHash](CONST _SpecialBossPtr& SpecialBossPtr){return dwHash == SpecialBossPtr.dwHash; });
	if (Ptr != nullptr)
	{
		Ptr->fn(Monster);
		return TRUE;
	}
	return FALSE;
}

VOID CFarmBoss::SetBossEnviroment(_In_ CONST CMonster& Boss)
{
	struct BossEnviroment
	{
		std::wstring wsBossName;
		FixPointInfo RangePoint;
		float fMinKeepAttackDis;
		float fKeepAttackDis;
	};

	CONST static std::vector<BossEnviroment> VecBossEnviroment = 
	{
		{ L"부카", FixPointInfo(Point(-30584, -37060, 10496), 30), 4, 12 },
		{ L"마골카", FixPointInfo(Point(-35088, -40436, 10193), 35), 3, 12 },
		{ L"경비대장", FixPointInfo(Point(-8151, -67996, 1571), 15), 0, 3 },
		{ L"간수장", FixPointInfo(Point(-3481, -71798, 152), 15), 0, 3 },
		{ L"추격대장", FixPointInfo(Point(1443, -68000, 159), 30), 0, 3 },
		{ L"우완", FixPointInfo(Point(-9067, 9961, 1844), 30), 0, 3 },
		{ L"마후가라", FixPointInfo(Point(-6222, 15587, 2650), 30), 0, 3 },
		{ L"교도관", FixPointInfo(Point(-2093, -20695, -7879), 15), 0, 3 },
		{ L"자칸", FixPointInfo(Point(-33252, -50836, 5080), 35), 0, 11 },
	};

	auto p = MyTools::CLPublic::Vec_find_if_Const(VecBossEnviroment, [Boss](CONST BossEnviroment& BossEnviroment_){ return BossEnviroment_.wsBossName == Boss.GetName(); });
	if (p != nullptr)
	{
		LOG_C_D(L"[%s]的BOSS范围设置~", p->wsBossName.c_str());
		_BossRange = p->RangePoint;
		_MinKeepAttackDis = p->fMinKeepAttackDis;
		_KeepAttackDis = p->fKeepAttackDis;
		CFarmBossSkill::GetInstance().SetBossObject(Boss, _BossRange);
	}
	else
	{
		LOG_C_D(L"默认的BOSS范围设置~");
		_BossRange = FixPointInfo(Boss.GetPoint(), 30);
		_MinKeepAttackDis = 0;
		_KeepAttackDis = 10;
		CFarmBossSkill::GetInstance().SetBossObject(Boss, _BossRange);
	}

	CFarmBossSkill::GetInstance().SetClassesNormalAttack();
	CPersonFunction::GetInstance().SetMoveFlag(MOVE_FLAG_DISABLE_JUMP | MOVE_FLAG_DISABLE_SPEED | MOVE_FLAG_DISABLE_SS);
}

BOOL CFarmBoss::KillBoss_Singe_Skill(_In_ CMonster& Monster, _In_ DWORD dwTimeOut) CONST
{
	return CSomeFun::GetInstance().TimeOut_Condiction_GameRun(dwTimeOut, [&Monster]
	{
		Monster.SetAngle();
		if (CPerson::GetInstance().GetMp() >= 50)
			CSkill::UseSkill_By_Id(Lux_SkillId_T, TRUE);
		else
			CSkill::UseSkill_By_Id(Lux_SkillId_R, TRUE);

		return BnsSleep(200) && !Monster.IsDead();
	});
}

BOOL CFarmBoss::KillBoss_Kffm(_In_ CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo)
{
	auto dwHp = Monster.GetHp();
	auto ulLastHpTick = ::GetTickCount64();
	auto& FarmSkill = CFarmSkill::GetInstance();
	auto dwMode = CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_KFFM_BOSS_MODE);
	fKillDis = 4.0f;

	while (GameRun && !EMPTY_PERSONHP && !Monster.IsDead() && CFarm::GetInstance().IsLoseHp(Monster, dwHp, ulLastHpTick, 60 * 1000))
	{
		em_Player_Pose emPersonPose = CPerson::GetInstance().GetFightPose();
		if (dwMode == 0) // 远程模式
		{
			if (emPersonPose != em_Player_Pose_ForceMaster) // 气功姿势下, 判断技能是否全部都在CD中。 是的话， 那就要用Tab转换成拳师姿势!
			{
				LOG_C_D( L"切换成气功姿势!");
				CSomeFun::GetInstance().SimlationKey(VK_TAB, 200, 600);
				continue;
			}

			if (Monster.GetDis() > 16.0f)
			{
				if (!CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100)))
				{
					LOG_C(MyTools::CLog::em_Log_Type::em_Log_Type_Exception, L"距离BOSS超过100米了, 不追了!");
					return FALSE;
				}
			}
		}
		else // 近战模式
		{
			if (emPersonPose != em_Player_Pose_KongFuMaster) // 气功姿势下, 判断技能是否全部都在CD中。 是的话， 那就要用Tab转换成拳师姿势!
			{
				LOG_C_D( L"切换成拳师姿势!");
				CSomeFun::GetInstance().SimlationKey(VK_TAB, 200, 600);
				continue;
			}

			if (Monster.GetDis() > fKillDis && !CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100)))
			{
				LOG_C(MyTools::CLog::em_Log_Type::em_Log_Type_Exception, L"距离BOSS超过100米了, 不追了!");
				return FALSE;
			}
		}


		if (CPerson::GetInstance().GetTargetId() != Monster.GetTargetId())
			Monster.SetAngle();


		// bRetCode = FALSE 表示并没有施放对应的BOSS技能
		if (UseSkill_By_BossSkillId_Kffm(Monster, GetMonsterSkillId(), FixPtInfo))
		{
			LOG_C_D( L"处理完毕0000!");
			GetMonsterSkillId() = 0;
			continue;
		}


		if (dwMode == 0)
		{
			CONST static std::vector<DWORD> vFmSkill = {
				Kffm_Fm_SkillId_Z, Kffm_Fm_SkillId_V, Kffm_Fm_SkillId_4, Kffm_Fm_SkillId_3, Kffm_Fm_SkillId_2
			};

			Monster.SetAngle();
			if (FarmSkill.IsExist_By_SkillId_In_Ui(Kffm_Fm_SkillId_V_F) || FarmSkill.IsExist_By_SkillId_In_Ui(0x7684))
			{
				//LOG_C_D( L"Kffm_Km_SkillId_V_F");
				CONST static std::vector<DWORD> vSkill_V_F = {
					Kffm_Fm_SkillId_V_F, Kffm_Fm_SkillId_V_F + 0x1, Kffm_Fm_SkillId_V_F + 0x2, Kffm_Fm_SkillId_V_F + 0x3,
					0x7684, 0x7684 + 0x1, 0x7684 + 0x2, 0x7684 + 0x3,
				};

				if (std::find_if(vSkill_V_F.begin(), vSkill_V_F.end(), [&FarmSkill](DWORD dwId){
					return FarmSkill.IsExist_By_SkillId_In_Ui(dwId);
				}) != vSkill_V_F.end())
				{
					CSomeFun::GetInstance().SimlationKey('F', 100);
					continue;
				}
			}
			for (auto itr = vFmSkill.begin(); itr != vFmSkill.end() && GameRun && !EMPTY_PERSONHP && !Monster.IsDead(); ++itr)
			{
				if (FarmSkill.IsCool_By_SkillId_In_Ui(*itr))
				{
					FarmSkill.UseSkill_By_SkillId(*itr);
					break;
				}
			}

			if (CPerson::GetInstance().GetMp() >= 5)
				CSkill::UseSkill_By_Id(Kffm_Fm_SkillId_T, TRUE);
			else
				CSkill::UseSkill_By_Id(Kffm_Fm_SkillId_R, TRUE);
		}
		else
		{
			// 判断怪物是否面对咱们!
			if (FarmSkill.IsExist_By_SkillId_In_Ui(Kffm_Kfm_SkillId_C_F))
				FarmSkill.UseSkill_By_SkillId(Kffm_Kfm_SkillId_C_F);
			else if (FarmSkill.IsExist_By_SkillId_In_Ui(Kffm_Kfm_SkillId_V_F))
			{
				CONST static std::vector<DWORD> vSkill_V_F = {
					Kffm_Kfm_SkillId_V_F, Kffm_Kfm_SkillId_V_F + 0x1, Kffm_Kfm_SkillId_V_F + 0x2,
					Kffm_Kfm_SkillId_V_F + 0x3, Kffm_Kfm_SkillId_V_F + 0x4
				};

				while (GameRun && !EMPTY_PERSONHP)
				{
					CSomeFun::GetInstance().SimlationKey('F', 100);
					if (std::find_if(vSkill_V_F.begin(), vSkill_V_F.end(), [&FarmSkill](DWORD dwId){
						return FarmSkill.IsExist_By_SkillId_In_Ui(dwId);
					}) == vSkill_V_F.end())
					{
						break;
					}
				}
			}
			else if (CPersonFunction::GetInstance().ExistBuff_Person(0x1C9C416, nullptr))
				FarmSkill.UseSkill_By_SkillId(Kffm_Kfm_SkillId_V);
			else if (FarmSkill.IsCool_By_SkillId_In_Ui(Kffm_Kfm_SkillId_Z))
				FarmSkill.UseSkill_By_SkillId(Kffm_Kfm_SkillId_Z);
			else if (FarmSkill.IsCool_By_SkillId_In_Ui(Kffm_Kfm_SkillId_2))
				FarmSkill.UseSkill_By_SkillId(Kffm_Kfm_SkillId_2);
			else if (FarmSkill.IsExist_By_SkillId_In_Ui(Kffm_Kfm_SkillId_T_F))
				FarmSkill.UseSkill_By_SkillId(Kffm_Kfm_SkillId_T_F); // 贴山靠
			else if (FarmSkill.IsExist_By_SkillId_In_Ui(Kffm_Kfm_SkillId_2_F))
				FarmSkill.UseSkill_By_SkillId(Kffm_Kfm_SkillId_2_F);
			else
			{
				if (CPerson::GetInstance().GetMp() > 2)
				{
					if (FarmSkill.IsExist_By_SkillId_In_Ui(Kffm_Kfm_SkillId_T))
						CSkill::UseSkill_By_Id(Kffm_Kfm_SkillId_T, TRUE);
					else if (FarmSkill.IsExist_By_SkillId_In_Ui(Kffm_Kfm_SkillId_T + 0x1))
						CSkill::UseSkill_By_Id(Kffm_Kfm_SkillId_T + 0x1, TRUE);
				}
				else if (FarmSkill.IsExist_By_SkillId_In_Ui(Kffm_Kfm_SkillId_R + 0x2))
					CSkill::UseSkill_By_Id(Kffm_Kfm_SkillId_R + 0x2, TRUE);
				else if (FarmSkill.IsExist_By_SkillId_In_Ui(Kffm_Kfm_SkillId_R + 0x1))
					CSkill::UseSkill_By_Id(Kffm_Kfm_SkillId_R + 0x1, TRUE);
				else
					CSkill::UseSkill_By_Id(Kffm_Kfm_SkillId_R, TRUE); // 0x7530
			}
		}

	}
	return TRUE;
}

BOOL CFarmBoss::KillBoss_Sum(_In_ CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo)
{
	Step_Flag = TRUE;
	Skill_Flag = TRUE;
	auto dwHp = Monster.GetHp();
	auto ulLastHpTick = ::GetTickCount64();
	auto Sum_Q_L1 = ::GetTickCount64();
	auto Sum_Q_L2 = ::GetTickCount64();
	auto Sum_Q_L3 = ::GetTickCount64();
	auto Sum_Q_L4 = ::GetTickCount64();
	DWORD TmpBoss_id = 0;
	auto& FarmSkill = CFarmSkill::GetInstance();
	Sum_Q = ::GetTickCount64();
	Guimenguan_boss_Count = 1;
	Guimenguan_jiaxue = 0;
	CCat newcat;
	CCatExpend::GetInstance().FindSelfCat(&newcat);
	CGameUi Target2_StatusPanel;


	CFarmBossSkill::GetInstance().Run();
	CFarmBossSkill::GetInstance().Move(Monster.GetFacePoint(12.0f), 12.0f);
	while (GameRun && !Monster.IsDead() && (Monster.GetHp() == 1 ||  CFarm::GetInstance().IsLoseHp(Monster, dwHp, ulLastHpTick, 60 * 1000)))
	{
		//LOG_C(CLog::em_Log_Type::em_Log_Type_Exception, L"外循环KillBoss_Sum测试时间1"); 
		if (EMPTY_PERSONHP)
		{
			if (!CTeamMemberExtend::GetInstance().IsTryRelive() || !CPersonFunction::GetInstance().TryRelive())
				break;

			continue;
		}

		
		if (GetMonsterSkillId() == 0)
		{
			if (Step_Flag)
			{
				//LOG_C_D( L"当前BOSS-id=%d!", GetMonsterSkillId());
				//MoveToBoss
				//if (!CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100)))
					//if (!MoveToBoss(Monster))
				//if (CPerson::GetInstance().GetDis(Monster.GetPoint()) > 14.0f)
				if (Monster.GetDis() > 14.0f)
					//CFarmBossSkill::GetInstance().MoveToSixAngle(10.0f,1*1000);
					MoveToPoint(Monster.GetFacePoint(10.0f));
			}
		}

		//LOG_C(CLog::em_Log_Type::em_Log_Type_Exception, L"外循环KillBoss_Sum测试时间2");
		Monster.SetAngle();
		RefushGameUi(L"Target2_StatusPanel", Target2_StatusPanel);


		
		// ---特殊处理, 雪玉宫伊鲁卡需要在获取技能ID之前处理大招!-------------------------------------
		if (Monster.GetName() == L"전격의 이루가")//雪域宫老2
		{
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab_Suppress))
			{
				CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
				//CSkill::UseSkill_By_Id(Sum_SkillId_Tab_Suppress, FALSE, em_Skill_Type_Cat);
			}
			else if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab_Suppress_1))
			{
				CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
				//CSkill::UseSkill_By_Id(Sum_SkillId_Tab_Suppress_1, FALSE, em_Skill_Type_Cat);
			}
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_3) && CPerson::GetInstance().GetMp() >= 3)
			{
				//CSomeFun::GetInstance().SimlationKey(VK_3, 10);
				CSomeFun::GetInstance().SimlationKey(VK_3, 10);
			}
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_X) && CPerson::GetInstance().GetPercentHp() < 80)
			{
				LOG_C_D( L"使用X加血");
				//CSomeFun::GetInstance().SimlationKey('X', 10);
				CSomeFun::GetInstance().SimlationKey('X', 10);
			}

			if (InterruptYLKShadowSkill(Monster))
			{
				CSomeFun::GetInstance().TimeOut_Condiction_GameRun(30 * 1000, [this, Monster]
				{
					ReboundYLKSkill(Monster, FALSE);
					return GetMonsterSkillId() != 5902160 && !EMPTY_PERSONHP;
				});
				LOG_C_D( L"BOSS放元气弹了,丢盾牌!");
				ReboundYLKSkill(Monster, TRUE);
				CFarmSkill::GetInstance().UseULT();
				MoveToPoint(Monster.GetFrontPoint(3));
				GetMonsterSkillId() = NULL;
			}
		}

		if (UseSkill_By_BossSkillId_Sum(Monster, GetMonsterSkillId(), FixPtInfo, Target2_StatusPanel, newcat, TmpBoss_id))
		{
			LOG_C_D( L"处理完毕0000!");
			TmpBoss_id = GetMonsterSkillId();
			GetMonsterSkillId() = 0;
			continue;
		}
		if (!Skill_Flag)
		{
			//设置当前是否攻击
			Sleep(100);
			continue;
		}
		ForceBeckonCat(newcat);
		//갈마왕 == 鬼门关BOSS
		//귀문 만령강시 = 鬼门关万魂
		//흑신시 = 黑天照
		//雪域宫-은동-银童
		//雪域宫-금동-金童
		//雪域宫-전격의 이루가-伊鲁卡 老2
		//雪域宫-수라왕-修罗王
		//黑龙教-부카-布卡
		//黑龙教-마골카-
		//黑龙教-자칸
		//Log(LOG_LEVEL_EXCEPTION, L"外循环KillBoss_Sum测试时间4");
		//7-11的黑龙教主
		//바우 = 黑森林BOSS
		Monster.SetAngle();

		
		if (static_cast<DWORD>(::GetTickCount64() - Sum_Q_L1) >= 2000)
		{
			if (CTeamMemberExtend::GetInstance().GetCurrentTeamIndexByClasses(em_Player_Classes_Summoner) == 0)
			{
				if (!FarmSkill.IsCool_By_SkillId_In_HideUi(Sum_SkillId_Q))
				{
					Sum_Q_L1 = ::GetTickCount64();
				}
				else
				{
					if (FarmSkill.IsActive_By_SkillId_In_Ui(Sum_SkillId_Q))
					{
						CSomeFun::GetInstance().SimlationKey('Q', 10);
						//CSomeFun::GetInstance().SimlationKey('Q', 10);
						//CSkill::UseSkill_By_Id(Sum_SkillId_Q, FALSE, em_Skill_Type_Cat);
						continue;
					}
				}
				if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_1))
				{
					CSomeFun::GetInstance().SimlationKey(VK_1, 10);
					//CSomeFun::GetInstance().SimlationKey(VK_1, 10);
					//CSomeFun::GetInstance().SimlationKey(VK_1, 10);
					continue;
				}
				if (Monster.GetName() == L"자칸" && CPerson::GetInstance().GetDis(Monster.GetPoint()) <= 11)
				{
					if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_2))
					{
						CSomeFun::GetInstance().SimlationKey(VK_2, 10);
						//CSomeFun::GetInstance().SimlationKey(VK_2, 10);
						//CSkill::UseSkill_By_Id(Sum_SkillId_2, FALSE);
						continue;
					}
				}
				else
				{
					if (Monster.GetName() != L"부카" && Monster.GetName() != L"전격의 이루가")
					{
						if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_2))
						{
							CSomeFun::GetInstance().SimlationKey(VK_2, 10);
							//CSomeFun::GetInstance().SimlationKey(VK_2, 10);
							//CSkill::UseSkill_By_Id(Sum_SkillId_2, FALSE);
							continue;
						}
					}
				}
			}
		}
		if (static_cast<DWORD>(::GetTickCount64() - Sum_Q_L2) >= 5000)
		{
			if (CTeamMemberExtend::GetInstance().GetCurrentTeamIndexByClasses(em_Player_Classes_Summoner) == 1)
			{
				if (!FarmSkill.IsCool_By_SkillId_In_HideUi(Sum_SkillId_Q))
				{
					Sum_Q_L2 = ::GetTickCount64();
				}
				else
				{
					if (FarmSkill.IsActive_By_SkillId_In_Ui(Sum_SkillId_Q))
					{
						CSomeFun::GetInstance().SimlationKey('Q' , 10);
						//CSomeFun::GetInstance().SimlationKey('Q', 10);
						//CSkill::UseSkill_By_Id(Sum_SkillId_Q, FALSE,em_Skill_Type_Cat);
						continue;
					}
				}
				if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_1))
				{
					CSomeFun::GetInstance().SimlationKey(VK_1, 10);
					//CSomeFun::GetInstance().SimlationKey(VK_1, 10);
					//CSomeFun::GetInstance().SimlationKey(VK_1, 10);
					continue;
				}
				if (Monster.GetName() == L"자칸" && CPerson::GetInstance().GetDis(Monster.GetPoint()) <= 11)
				{
					if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_2))
					{
						CSomeFun::GetInstance().SimlationKey(VK_2, 10);
						//CSomeFun::GetInstance().SimlationKey(VK_2, 10);
						//CSkill::UseSkill_By_Id(Sum_SkillId_2, FALSE);
						continue;
					}
				}
				else
				{
					if (Monster.GetName() != L"부카" && Monster.GetName() != L"전격의 이루가")
					{
						if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_2))
						{
							CSomeFun::GetInstance().SimlationKey(VK_2, 10);
							//CSomeFun::GetInstance().SimlationKey(VK_2, 10);
							//CSkill::UseSkill_By_Id(Sum_SkillId_2, FALSE);
							continue;
						}
					}
				}
			}
		}

		if (static_cast<DWORD>(::GetTickCount64() - Sum_Q_L3) >= 8000)
		{
			if (CTeamMemberExtend::GetInstance().GetCurrentTeamIndexByClasses(em_Player_Classes_Summoner) == 2)
			{
				if (!FarmSkill.IsCool_By_SkillId_In_HideUi(Sum_SkillId_Q))
				{
					Sum_Q_L3 = ::GetTickCount64();
				}
				else
				{
					if (FarmSkill.IsActive_By_SkillId_In_Ui(Sum_SkillId_Q))
					{
						CSomeFun::GetInstance().SimlationKey('Q', 10);
						//CSomeFun::GetInstance().SimlationKey('Q', 10);
						//CSkill::UseSkill_By_Id(Sum_SkillId_Q, FALSE, em_Skill_Type_Cat);
						continue;
					}
				}
				if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_1))
				{
					CSomeFun::GetInstance().SimlationKey(VK_1, 10);
					//CSomeFun::GetInstance().SimlationKey(VK_1, 10);
					//CSomeFun::GetInstance().SimlationKey(VK_1, 10);
					continue;
				}
				if (Monster.GetName() == L"자칸" && CPerson::GetInstance().GetDis(Monster.GetPoint()) <= 11)
				{
					if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_2))
					{
						CSomeFun::GetInstance().SimlationKey(VK_2, 10);
						//CSomeFun::GetInstance().SimlationKey(VK_2, 10);
						//CSkill::UseSkill_By_Id(Sum_SkillId_2, FALSE);
						continue;
					}
				}
				else
				{
					if (Monster.GetName() != L"부카" && Monster.GetName() != L"전격의 이루가")
					{
						if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_2))
						{
							CSomeFun::GetInstance().SimlationKey(VK_2, 10);
							//CSomeFun::GetInstance().SimlationKey(VK_2, 10);
							//CSkill::UseSkill_By_Id(Sum_SkillId_2, FALSE);
							continue;
						}
					}
				}
			}
		}
		if (static_cast<DWORD>(::GetTickCount64() - Sum_Q_L4) >= 10000)
		{
			if (CTeamMemberExtend::GetInstance().GetCurrentTeamIndexByClasses(em_Player_Classes_Summoner) == 3)
			{
				if (!FarmSkill.IsCool_By_SkillId_In_HideUi(Sum_SkillId_Q))
				{
					Sum_Q_L4 = ::GetTickCount64();
				}
				else
				{
					if (FarmSkill.IsActive_By_SkillId_In_Ui(Sum_SkillId_Q))
					{
						
						CSomeFun::GetInstance().SimlationKey('Q', 10);
						//CSkill::UseSkill_By_Id(Sum_SkillId_Q, FALSE, em_Skill_Type_Cat);
						continue;
					}
				}
				if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_1))
				{
					CSomeFun::GetInstance().SimlationKey(VK_1, 10);
					//CSomeFun::GetInstance().SimlationKey(VK_1, 10);
					continue;
				}
				if (Monster.GetName() == L"자칸" && CPerson::GetInstance().GetDis(Monster.GetPoint()) <= 11)
				{
					if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_2))
					{
						CSomeFun::GetInstance().SimlationKey(VK_2, 10);
						//CSkill::UseSkill_By_Id(Sum_SkillId_2, FALSE);
						continue;
					}
				}
				else
				{
					if (Monster.GetName() != L"부카" && Monster.GetName() != L"전격의 이루가")
					{
						if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_2))
						{
							CSomeFun::GetInstance().SimlationKey(VK_2, 10);
							//CSkill::UseSkill_By_Id(Sum_SkillId_2, FALSE);
							continue;
						}
					}
				}
			}
		}


		if (Monster.GetName() == L"흑룡교주")
		{
			if (newcat.GetPercentHp() < 90 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_C))
			{
				//CSkill::UseSkill_By_Id(Sum_SkillId_C, FALSE, em_Skill_Type_Cat);
				CSomeFun::GetInstance().SimlationKey('C', 10);
			}

			if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab_Suppress))
			{
				CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
				//CSkill::UseSkill_By_Id(Sum_SkillId_Tab_Suppress, FALSE, em_Skill_Type_Cat);
			}
		}
		else if (Monster.GetName() == L"수라왕")//雪域宫老4 修罗王
		{
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab_Suppress))
			{
				CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
				//CSkill::UseSkill_By_Id(Sum_SkillId_Tab_Suppress, FALSE, em_Skill_Type_Cat);
			}
			else if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab_Suppress_1))
			{
				CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
				//CSkill::UseSkill_By_Id(Sum_SkillId_Tab_Suppress_1, FALSE, em_Skill_Type_Cat);
			}
			if (CTeamMemberExtend::GetInstance().GetCurrentTeamIndexByClasses(em_Player_Classes_Summoner) == 1 ||
				CTeamMemberExtend::GetInstance().GetCurrentTeamIndexByClasses(em_Player_Classes_Summoner) == 2 ||
				CTeamMemberExtend::GetInstance().GetCurrentTeamIndexByClasses(em_Player_Classes_Summoner) == 3)
			{
				if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_3))
				{
					CSomeFun::GetInstance().SimlationKey(VK_3, 10);
				}
			}
			//18FDF3F 压猫的BUFF
			if (CObjectFunction::GetInstance().ExistMonsterBuff(Monster.GetId(), 0x18FDF3F))
			{
				if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_G))
				{
					CSomeFun::GetInstance().SimlationKey('G', 10);
					BnsSleep(4 * 1000);
				}
			}
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_1) && CPerson::GetInstance().GetMp() >= 3)
			{
				CSomeFun::GetInstance().SimlationKey(VK_1, 10);
			}
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_C))
			{
				CSomeFun::GetInstance().SimlationKey('C', 10);
			}
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_V))
			{
				CSomeFun::GetInstance().SimlationKey('V', 10);
			}
			StepOnFlowerForXiuLuoWang();
			XiuLuoWang_BlockBall();
		}
		else if (Monster.GetName() == L"자칸")//地下监狱老3
		{
			/*	if (Monster.GetPercentHp() > 89)
				{
				if (IsEnableControlBoss(Target2_StatusPanel))
				{
				if (Guimenguan_boss_Count == 1)
				{
				if (CTeamMemberExtend::GetInstance().GetCurrentTeamIndexByClasses(em_Player_Classes_Summoner) == 0 ||
				CTeamMemberExtend::GetInstance().GetCurrentTeamIndexByClasses(em_Player_Classes_Summoner) == 1)
				{
				CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
				Sleep(500);
				}
				}
				else if (Guimenguan_boss_Count == 2)
				{
				if (CTeamMemberExtend::GetInstance().GetCurrentTeamIndexByClasses(em_Player_Classes_Summoner) == 2 ||
				CTeamMemberExtend::GetInstance().GetCurrentTeamIndexByClasses(em_Player_Classes_Summoner) == 3)
				{
				CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
				Sleep(500);
				}
				}
				else if (Guimenguan_boss_Count == 3)
				{
				CSomeFun::GetInstance().SimlationKey('V', 10);
				Sleep(500);
				}
				else if (Guimenguan_boss_Count == 4)
				{
				CSomeFun::GetInstance().SimlationKey('C', 10);
				Sleep(500);
				}
				}
				}*/
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab_Suppress))
			{
				CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
				//CSkill::UseSkill_By_Id(Sum_SkillId_Tab_Suppress, FALSE, em_Skill_Type_Cat);
				continue;
			}
			else if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab_Suppress_1))
			{
				CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
				//CSkill::UseSkill_By_Id(Sum_SkillId_Tab_Suppress_1, FALSE, em_Skill_Type_Cat);
				continue;
			}
			//18FDF3F 压猫的BUFF
			if (CObjectFunction::GetInstance().ExistMonsterBuff(Monster.GetId(), 0x18FDF3F))
			{
				if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_G))
				{
					CSomeFun::GetInstance().SimlationKey('G', 10);
					BnsSleep(4 * 1000);
					continue;
				}
			}
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_3) && CPerson::GetInstance().GetMp() >= 3)
			{
				CSomeFun::GetInstance().SimlationKey(VK_3, 10);
				//CSomeFun::GetInstance().SimlationKey(VK_3, 10);
				continue;
			}
			//if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_C))
			//{
			//	CSomeFun::GetInstance().SimlationKey('C', 10);
			//}
			//if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_V))
			//{
			//	CSomeFun::GetInstance().SimlationKey('V', 10);
			//}
			Guimenguan_boss_Count++;

			if (Guimenguan_boss_Count > 4)
			{
				Guimenguan_boss_Count = 1;
			}
		}
		else if (Monster.GetName() == L"마골카")//地下监狱老2
		{
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_1) && CPerson::GetInstance().GetMp() >= 3)
			{
				CSomeFun::GetInstance().SimlationKey(VK_1, 10);
			}
			/*if (Monster.GetMaxHp() == 13000000)
			{
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_3) && CPerson::GetInstance().GetMp() >= 3)
			{
			CSomeFun::GetInstance().SimlationKey(VK_3, 10);
			}
			}*/
			if (Heilongjiao_Huafen)
			{
				if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_3) && CPerson::GetInstance().GetMp() >= 3)
				{
					CSomeFun::GetInstance().SimlationKey(VK_3, 10);
				}
			}
			//18FDF3F 压猫的BUFF
			if (CObjectFunction::GetInstance().ExistMonsterBuff(Monster.GetId(), 0x18FDF3F))
			{
				if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_G))
				{
					CSomeFun::GetInstance().SimlationKey('G', 10);
					BnsSleep(4 * 1000);
				}
			}
		}
		else if (Monster.GetName() == L"부카")//地下监狱老1
		{
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab_Suppress))
			{
				CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
				//CSkill::UseSkill_By_Id(Sum_SkillId_Tab_Suppress, FALSE, em_Skill_Type_Cat);
			}
			else if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab_Suppress_1))
			{
				CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
				//CSkill::UseSkill_By_Id(Sum_SkillId_Tab_Suppress_1, FALSE, em_Skill_Type_Cat);
			}
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_3) && CPerson::GetInstance().GetMp() >= 3)
			{
				CSomeFun::GetInstance().SimlationKey(VK_3, 10);
			}
			//18FDF3F 压猫的BUFF
			if (CObjectFunction::GetInstance().ExistMonsterBuff(Monster.GetId(), 0x18FDF3F))
			{
				if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_G))
				{
					CSomeFun::GetInstance().SimlationKey('G', 10);
					BnsSleep(4 * 1000);
				}
			}
			AvoidBuKaZhuoQiZhen(Monster, FixPointInfo(Point(-30584, -37060, 10496), 15));
		}
		else if (Monster.GetName() == L"수라왕")
		{
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab_Suppress))
			{
				CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
				//CSkill::UseSkill_By_Id(Sum_SkillId_Tab_Suppress, FALSE, em_Skill_Type_Cat);
			}
			else if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab_Suppress_1))
			{
				CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
				//CSkill::UseSkill_By_Id(Sum_SkillId_Tab_Suppress_1, FALSE, em_Skill_Type_Cat);
			}
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_X) && CPerson::GetInstance().GetPercentHp() < 80)
			{
				LOG_C_D( L"使用X加血");
				CSomeFun::GetInstance().SimlationKey('X', 10);
			}
		}
		else if (Monster.GetName() == L"은동" || Monster.GetName() == L"금동")
		{
			//金童银童
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab_Suppress))
			{
				CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
				//CSkill::UseSkill_By_Id(Sum_SkillId_Tab_Suppress, FALSE, em_Skill_Type_Cat);
			}
			else if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab_Suppress_1))
			{
				CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
				//CSkill::UseSkill_By_Id(Sum_SkillId_Tab_Suppress_1, FALSE, em_Skill_Type_Cat);
			}
			if (newcat.GetPercentHp() < 90 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_C) && static_cast<DWORD>(::GetTickCount64() - Sum_Q) >= 8 * 1000)
			{
				//CSkill::UseSkill_By_Id(Sum_SkillId_C, FALSE, em_Skill_Type_Cat);
				CSomeFun::GetInstance().SimlationKey('C', 10);
			}
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_X) && CPerson::GetInstance().GetPercentHp() < 80)
			{
				LOG_C_D( L"使用X加血");
				CSomeFun::GetInstance().SimlationKey('X', 10);
			}

			if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_3) && CPerson::GetInstance().GetMp() >= 3)
			{
				CSomeFun::GetInstance().SimlationKey(VK_3, 10);
			}
		}
		else if (Monster.GetName() == L"바우")
		{
			//黑森林BOSS
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_3) && CPerson::GetInstance().GetMp() >= 3)
			{
				CSomeFun::GetInstance().SimlationKey(VK_3, 10);
			}


			if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab_Suppress))
			{
				LOG_C_D( L"执行压猫。。。");
				//CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
				CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
				//CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
			}
		}
		else if (Monster.GetName() == L"갈마왕" || Monster.GetName() == L"귀문 만령강시" || Monster.GetName() == L"흑신시" )
		{
			if (Monster.GetName() == L"갈마왕")
			{
				if (CPerson::GetInstance().GetAbNormalStatus() == em_WarStatus::em_WarStatus_Hide)
				{
					LOG_C_D( L"当前是隐身状态!");
					if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_F_4))
					{
						CSomeFun::GetInstance().SimlationKey(VK_4, 10);
					}
					continue;
				}
			}
			
			//如果是鬼门关BOSS，判断是否是可以压猫的状态
			if (Monster.GetName() == L"갈마왕")
			{
				if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab_Suppress))
				{
					CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
				}
			}
			//鬼门关BOSS放大招时硬顶过去。
			if (Guimenguan_boss)
			{
				LOG_C_D( L"进入BOSS大招状态");
				if (CPerson::GetInstance().GetAbNormalStatus() != em_WarStatus::em_WarStatus_Hide)
				{
					LOG_C_D( L"没有隐身,继续干,判断队员血量.....................");
					if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Z) && CTeamMemberExtend::GetInstance().GetTeamMember_By_LessPercentHp(70, nullptr))
					{
						LOG_C_D( L"有队员血量低于70");
						if (Guimenguan_boss_Count % 2 == 0)
						{
							if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 0 || CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 1 || CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 2)
							{
								LOG_C_D( L"第一阶梯加血");
								CSomeFun::GetInstance().SimlationKey('Z', 10);
								CSomeFun::GetInstance().SimlationKey('Z', 10);
							}
						}
						else
						{
							if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 3 || CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 4 || CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 5)
							{
								LOG_C_D( L"第二阶梯加血");
								CSomeFun::GetInstance().SimlationKey('Z', 10);
								CSomeFun::GetInstance().SimlationKey('Z', 10);
							}
						}
						
					}
					else if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_X) && CPerson::GetInstance().GetPercentHp() < 70)
					{
						LOG_C_D( L"使用X加血");
						CSomeFun::GetInstance().SimlationKey('X', 10);
					}
					else
					{
						LOG_C_D( L"使用4准备隐身");
						CSomeFun::GetInstance().SimlationKey(VK_4, 10);
						Sleep(1800);
					}
				}
			}

			if (CPerson::GetInstance().GetMp() >= 5 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_3))
			{
				//LOG_C_D( L"执行牵牛花");
				CSomeFun::GetInstance().SimlationKey(VK_3, 10);
			}


			if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab_Suppress))
			{
				CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
				//CSkill::UseSkill_By_Id(Sum_SkillId_Tab_Suppress, FALSE, em_Skill_Type_Cat);
			}
		}
		


		if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_F))
		{
			//LOG_C_D( L"执行牵牛花");
			CSomeFun::GetInstance().SimlationKey('F', 10);
		}
		else if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_F_F))
		{
			//LOG_C_D( L"执行板栗球。。。");
			CSomeFun::GetInstance().SimlationKey('F', 10);
		}
		else if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_F_4))
		{
			CSomeFun::GetInstance().SimlationKey(VK_4, 10);
		}
		else if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_C_C))
		{
			CSomeFun::GetInstance().SimlationKey('C', 10);
		}
		else if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Floor_C1))
		{
			CSomeFun::GetInstance().SimlationKey('C', 10);
		}


		if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_V_2) && static_cast<DWORD>(::GetTickCount64() - Sum_Q) >= 8 * 1000)
		{
			CSomeFun::GetInstance().SimlationKey('V', 10);
		}

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_V_3) && static_cast<DWORD>(::GetTickCount64() - Sum_Q) >= 8 * 1000)
		{
			CSomeFun::GetInstance().SimlationKey('V', 10);
			//FarmSkill.UseSkill_By_SkillId(Sum_SkillId_V_3, em_Skill_Type_Cat);
		}

		
		
		//갈마왕 == 鬼门关BOSS
		if (Monster.GetName() != L"대전사" && Monster.GetName() != L"갈마왕")
		{
			if (!CObjectFunction::GetInstance().ExistMonsterBuff(Monster.GetId(), 0xF5BE37))
			{
				if (CPerson::GetInstance().GetMp() >= 5 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_1))
				{
					//LOG_C_D( L"执行常春藤");
					CSomeFun::GetInstance().SimlationKey(VK_1, 10);
				}
			}
		}
		
		//LOG_C(CLog::em_Log_Type::em_Log_Type_Exception, L"外循环KillBoss_Sum测试时间6");
		/*if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_T2_Ui_D))
		{
			CSomeFun::GetInstance().SimlationKey('T', 10);
		}
		else
		{
			if (CPerson::GetInstance().GetMp() > 5)
			{
				//LOG_C_D( L"执行右键----");
				CSomeFun::GetInstance().SimlationKey('T', 10);
			}
			else
			{
				//LOG_C_D( L"执行玫瑰----");
				CSomeFun::GetInstance().SimlationKey('R', 10);
			}
		}*/
		Sleep(50);
	}

	return TRUE;
}

BOOL CFarmBoss::KillBoss_Warlock(_In_ CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo)
{
	Step_Flag = TRUE;
	Skill_Flag = TRUE;
	auto dwHp = Monster.GetHp();
	auto ulLastHpTick = ::GetTickCount64();
	DWORD TmpBoss_id = 0;
	auto& FarmSkill = CFarmSkill::GetInstance();
	Sum_Q = ::GetTickCount64();
	Guimenguan_boss_Count = 1;
	Guimenguan_jiaxue = 0;
	CGameUi Target2_StatusPanel;

	if (Monster.GetName() != L"마골카")
	{
		Heilongjiao_Huafen = TRUE;
	}


	CFarmBossSkill::GetInstance().Run();
	if (CTeamMemberExtend::GetInstance().GetCurrentTeamIndexByClasses(em_Player_Classes_Warlock) == 0)
	{
		if (Heilongjiao_Huafen)
		{
			//1B77402 超神BUFF公共CD
			if (!CPersonFunction::GetInstance().ExistBuff_Person(0x1B77402, nullptr) && GameRun)
			{
				if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_Tab))
				{
					CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
					Sleep(1500);
				}
				if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_Tab_God))
				{
					CFarmBossSkill::GetInstance().SetPower(TRUE);
					//FarmSkill.UseSkill_By_SkillId(Warlock_SkillId_Tab_God, em_Skill_Type_Cat);
					CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
					Sleep(1500);
					CFarmBossSkill::GetInstance().SetPower(FALSE);
				}
			}
		}
		
	}
	CFarmBossSkill::GetInstance().Move(Monster.GetFacePoint(12.0f), 12.0f);

	while (GameRun && !Monster.IsDead() && (Monster.GetHp() == 1 || CFarm::GetInstance().IsLoseHp(Monster, dwHp, ulLastHpTick, 60 * 1000)))
	{
		//LOG_C(CLog::em_Log_Type::em_Log_Type_Exception, L"外循环KillBoss_Sum测试时间1"); 
		if (EMPTY_PERSONHP)
		{
			if (!CTeamMemberExtend::GetInstance().IsTryRelive() || !CPersonFunction::GetInstance().TryRelive())
				break;

			continue;
		}


		if (GetMonsterSkillId() == 0)
		{
			if (Step_Flag)
			{
				//LOG_C_D( L"当前BOSS-id=%d!", GetMonsterSkillId());
				//MoveToBoss
				//if (!CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100)))
				//if (!MoveToBoss(Monster))
				//if (CPerson::GetInstance().GetDis(Monster.GetPoint()) > 14.0f)
				if (Monster.GetDis() > 14.0f)
					//CFarmBossSkill::GetInstance().MoveToSixAngle(10.0f,1*1000);
					MoveToPoint(Monster.GetFacePoint(10.0f));
			}
		}

		//LOG_C(CLog::em_Log_Type::em_Log_Type_Exception, L"外循环KillBoss_Sum测试时间2");
		Monster.SetAngle();
		RefushGameUi(L"Target2_StatusPanel", Target2_StatusPanel);




		if (UseSkill_By_BossSkillId_Warlock(Monster, GetMonsterSkillId(), FixPtInfo, Target2_StatusPanel, TmpBoss_id))
		{
			LOG_C_D( L"处理完毕0000!");
			TmpBoss_id = GetMonsterSkillId();
			GetMonsterSkillId() = 0;
			continue;
		}

		Monster.SetAngle();

		if (!Skill_Flag)
		{
			//设置当前是否攻击
			Sleep(100);
			continue;
		}

		//갈마왕 == 鬼门关BOSS
		//귀문 만령강시 = 鬼门关万魂
		//흑신시 = 黑天照
		//雪域宫-은동-银童
		//雪域宫-금동-金童
		//雪域宫-전격의 이루가-伊鲁卡 老2
		//雪域宫-수라왕-修罗王
		//黑龙教-부카-布卡
		//黑龙教-마골카-
		//黑龙教-자칸
		//Log(LOG_LEVEL_EXCEPTION, L"外循环KillBoss_Sum测试时间4");
		//7-11的黑龙教主
		//바우 = 黑森林BOSS

		if (Monster.GetName() == L"부카")//地下监狱老1
		{
			AvoidBuKaZhuoQiZhen(Monster, FixPointInfo(Point(-30584, -37060, 10496), 15));
		}

		////1B77403 超神BUFF
		if (CPersonFunction::GetInstance().ExistBuff_Person(0x1B77403, nullptr) && GameRun)
			continue;
		//if (CPersonFunction::GetInstance().ExistBuff_Person(0x1B77403, nullptr) && GameRun)
		//{
		//	if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_God_4))
		//	{
		//		if (FarmSkill.IsActive_By_SkillId_In_Ui(Warlock_SkillId_God_4))
		//		{
		//			FarmSkill.UseSkill_BySkillId_In_Sleep(Warlock_SkillId_God_4);
		//		}
		//	}
		//	if (CPerson::GetInstance().GetMp() > 6)
		//		CSomeFun::GetInstance().SimlationKey('T', 10);
		//	else
		//		CSomeFun::GetInstance().SimlationKey('R', 10);
		//	continue;
		//}

		

		

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_G))
		{
			CFarmBossSkill::GetInstance().SetPower(TRUE);
			CSomeFun::GetInstance().SimlationKey('G', 10);
			Sleep(2000);
			CFarmBossSkill::GetInstance().SetPower(FALSE);
			continue;
		}
		
		//1AB3F71 3个可以释放4
		//LOG_CF_D( L"4_1");
		if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_4_1))
		{
			if (FarmSkill.IsActive_By_SkillId_In_Ui(Warlock_SkillId_4_1))
			{
				CSomeFun::GetInstance().SimlationKey(VK_4, 10);
			}
		}
		//LOG_CF_D( L"F");
	

		//LOG_CF_D( L"2");
		 if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_2))
		{
			if (FarmSkill.IsActive_By_SkillId_In_Ui(Warlock_SkillId_2))
			{
				CSomeFun::GetInstance().SimlationKey(VK_2, 10);
			}
		}

		//LOG_CF_D( L"3");
		if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_3))
		{
			if (FarmSkill.IsActive_By_SkillId_In_Ui(Warlock_SkillId_3))
			{
				CFarmBossSkill::GetInstance().SetPower(TRUE);
				CSomeFun::GetInstance().SimlationKey(VK_3, 10);
				Sleep(1500);
				CFarmBossSkill::GetInstance().SetPower(FALSE);
			}
		}

		//LOG_CF_D( L"V1");
		

		//LOG_CF_D( L"E");
		//if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_E))
		//{
		//	if (FarmSkill.IsActive_By_SkillId_In_Ui(Warlock_SkillId_E))
		//	{
		//		FarmSkill.UseSkill_BySkillId_In_Sleep(Warlock_SkillId_E);
		//	}
		//}
		if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_Q))
		{
			if (FarmSkill.IsActive_By_SkillId_In_Ui(Warlock_SkillId_Q))
			{
				CSomeFun::GetInstance().SimlationKey('Q', 10);
			}
		}

		if (Monster.GetName() == L"마골카")
		{
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_X))
			{
				if (FarmSkill.IsActive_By_SkillId_In_Ui(Warlock_SkillId_X))
				{
					CSomeFun::GetInstance().SimlationKey('X', 10);
				}
			}
		}
		

		if (Monster.GetName() != L"자칸")
		{
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_C))
			{
				CSomeFun::GetInstance().SimlationKey('C', 10);
			}
		}

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_Tab))
		{
			CFarmBossSkill::GetInstance().SetPower(TRUE);
			CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
			Sleep(1000);
			CFarmBossSkill::GetInstance().SetPower(FALSE);
		}

		//1B77402 超神BUFF公共CD
		if (!CPersonFunction::GetInstance().ExistBuff_Person(0x1B77402, nullptr) && GameRun)
		{
			LOG_C_D( L"我身上没BUFF,准备放超神%d", CTeamMemberExtend::GetInstance().GetCurrentTeamIndexByClasses(em_Player_Classes_Warlock));
			if (Heilongjiao_Huafen)
			{
				if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_Tab_God))
				{
					CFarmBossSkill::GetInstance().SetPower(TRUE);
					//FarmSkill.UseSkill_By_SkillId(Warlock_SkillId_Tab_God, em_Skill_Type_Cat);
					CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
					Sleep(1000);
					CFarmBossSkill::GetInstance().SetPower(FALSE);
				}
			}
		}

		//if (CPerson::GetInstance().GetMp() > 6)
		//	CSomeFun::GetInstance().SimlationKey('T', 10);
		//else
		//	CSomeFun::GetInstance().SimlationKey('R', 10);

		Sleep(50);
	}

	return TRUE;
}

BOOL CFarmBoss::UseSkill_By_BossSkillId_Kffm(_In_ CMonster& Monster, _In_ DWORD dwBossSkillId, _In_ CONST FixPointInfo&)
{
	auto& FarmSkill = CFarmSkill::GetInstance();
	auto  NowPoint = Monster.GetPoint();

	auto fnUseSkill_1 = [&FarmSkill](DWORD dwSkillId1)
	{
		if (FarmSkill.IsExist_By_SkillId_In_Ui(dwSkillId1))
		{
			CSomeFun::GetInstance().SimlationKey(VK_1, 200, 600);
			CSomeFun::GetInstance().SimlationKey(VK_1, 200, 600);
		}
	};

	BOOL bRetCode = TRUE;
	switch (dwBossSkillId)
	{
	case 5100344:
		LOG_C_D( L"BOSS跳起来了!");
		if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Kffm_Fm_SkillId_C))
			CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Kffm_Fm_SkillId_C);

		BnsMove(Monster.GetBehindPoint(2.0f), 1.0f);
		break;
	case 5100341:

		LOG_C_D( L"用Q!");
		CSkill::UseSkill_By_Id(Kffm_Fm_SkillId_Q, TRUE);
		Sleep(300);
		Monster.SetAngle();
		break;
	case 5100366:
		LOG_C_D( L"走后面!");
		CPersonFunction::GetInstance().Move(Monster.GetBehindPoint(2.0f));
		BnsMove(Monster.GetBehindPoint(2.0f), 1.0f);
		Sleep(300);
		Monster.SetAngle();
		break;
	case 5100345:
		if (FarmSkill.IsCool_By_SkillId_In_Ui(Kffm_Fm_SkillId_C))
		{
			LOG_C_D( L"C可以用，用C档..!");
			CSkill::UseSkill_By_Id(Kffm_Fm_SkillId_C, TRUE);
			Sleep(300);
		}
		else if (FarmSkill.IsCool_By_SkillId_In_HideUi(Kffm_Fm_SkillId_E))
		{
			LOG_C_D( L"用e..!");
			//CSkill::UseSkill_By_Id(Kffm_Fm_SkillId_C, FALSE);
			CSkill::UseSkill_By_Id(Kffm_Fm_SkillId_R, FALSE);
			Sleep(300);
			CSkill::UseSkill_By_Id(Kffm_Fm_SkillId_R, FALSE);
			Sleep(200);
			CSkill::UseSkill_By_Id(Kffm_Fm_SkillId_E, TRUE);
			Sleep(300);
			Monster.SetAngle();
		}
		else if (FarmSkill.IsCool_By_SkillId_In_Ui(0x75F8))
		{
			LOG_C_D( L"4可以用，用4眩晕..!");
			CSkill::UseSkill_By_Id(0x75F8, TRUE);
			CPersonFunction::GetInstance().Move(Monster.GetBehindPoint(2.0f));
			BnsMove(Monster.GetBehindPoint(2.0f), 1.0f);
			Sleep(300);
			Monster.SetAngle();
		}
		else
		{
			LOG_C_D( L"用1..!");
			CSkill::UseSkill_By_Id(Kffm_Fm_SkillId_1, TRUE);
			Sleep(900);
		}

		break;
	case 5100347:
		LOG_C_D( L"sssssss!");
		//CSkill::UseSkill_By_Id(Kffm_Fm_SkillId_SS, TRUE);
		CSomeFun::GetInstance().SimlationKey('S', 600);
		CSkill::UseSkill_By_Id(Kffm_Fm_SkillId_R, TRUE);
		CSkill::UseSkill_By_Id(Kffm_Fm_SkillId_R, TRUE);
		CSkill::UseSkill_By_Id(Kffm_Fm_SkillId_R, TRUE);
		CSkill::UseSkill_By_Id(Kffm_Fm_SkillId_T, TRUE);
		CSkill::UseSkill_By_Id(Kffm_Fm_SkillId_R, TRUE);

		if (CPerson::GetInstance().GetPercentHp() < 80 && FarmSkill.IsCool_By_SkillId_In_Ui(Kffm_Fm_SkillId_X))
		{
			LOG_C_D( L"5100347用X回血!");
			CSkill::UseSkill_By_Id(Kffm_Fm_SkillId_X, TRUE);
		}
		else
		{
			CSkill::UseSkill_By_Id(Kffm_Fm_SkillId_R, TRUE);
			CSkill::UseSkill_By_Id(Kffm_Fm_SkillId_R, TRUE);
		}
		CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100));
		break;
	case 5190121:
		fKillDis = 6.0f;
		if (CPerson::GetInstance().GetPercentHp() < 50 && CNpcExtend::GetInstance().FindNpc_By_Name(L"내무반장 막소보", CBaseObject::CreateFixPointInfo(20.0f), nullptr))
		{
			CONST static std::vector<Point> vPoint = 
			{
				Point(47245, 45959, 5289), Point(47202, 45810, 5293), Point(47098, 45459, 5305), Point(46981, 45106, 5311),
				Point(46849, 44719, 5311), Point(46730, 44366, 5311), Point(46611, 44014, 5311), Point(46491, 43662, 5311),
			};

			for (UINT i = 0; i < vPoint.size() && GameRun && !EMPTY_PERSONHP; ++i)
				BnsMove(vPoint.at(i), 3);

			while (GameRun && !EMPTY_PERSONHP && CPerson::GetInstance().GetPercentHp() != 100)
			{
				CFarm::GetInstance().EatFood(90);
				BnsSleep(1000);
			}

			for (int i = static_cast<int>(vPoint.size() - 1); i >= 0 && GameRun && !EMPTY_PERSONHP; --i)
				BnsMove(vPoint.at(i), 3);
		}
		else if (FarmSkill.IsCool_By_SkillId_In_HideUi(Kffm_Kfm_SkillId_Q))
		{
			FarmSkill.UseSkill_BySkillId_In_Sleep(Kffm_Kfm_SkillId_Q);
		}
		else if (FarmSkill.IsCool_By_SkillId_In_HideUi(Kffm_Kfm_SkillId_E))
		{
			FarmSkill.UseSkill_BySkillId_In_Sleep(Kffm_Kfm_SkillId_E);
		}
		else
		{
			CPersonFunction::GetInstance().Move(Monster.GetBehindPoint(5.0f));
			BnsMove(Monster.GetBehindPoint(2.0f), 1.5f);
		}
		break;
	case 5190123:
		LOG_C_D( L"用1格挡!");
		BnsSleep(dwBossSkillId == 5190123 ? 2000 : 1000);
		fnUseSkill_1(Kffm_Kfm_SkillId_1);
		fnUseSkill_1(Kffm_Kfm_SkillId_1 + 0x1);
		break;
	case 5190126:
		if (CPerson::GetInstance().GetPercentHp() < 50 && CNpcExtend::GetInstance().FindNpc_By_Name(L"내무반장 막소보", CBaseObject::CreateFixPointInfo(20.0f), nullptr))
		{
			CONST static std::vector<Point> vPoint = 
			{
				Point(47245, 45959, 5289), Point(47202, 45810, 5293), Point(47098, 45459, 5305), Point(46981, 45106, 5311),
				Point(46849, 44719, 5311), Point(46730, 44366, 5311), Point(46611, 44014, 5311), Point(46491, 43662, 5311),
			};

			for (UINT i = 0; i < vPoint.size() && GameRun && !EMPTY_PERSONHP; ++i)
				BnsMove(vPoint.at(i), 3);

			while (GameRun && !EMPTY_PERSONHP && CPerson::GetInstance().GetPercentHp() != 100)
			{
				CFarm::GetInstance().EatFood(90);
				BnsSleep(1000);
			}

			for (int i = static_cast<int>(vPoint.size() - 1); i >= 0 && GameRun && !EMPTY_PERSONHP; --i)
				BnsMove(vPoint.at(i), 3);
		}
		else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Kffm_Kfm_SkillId_C))
		{
			LOG_C_D( L"用C躲转圈!");
			CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Kffm_Kfm_SkillId_C);
		}
		else if (FarmSkill.IsCool_By_SkillId_In_HideUi(Kffm_Fm_SkillId_SS))
		{
			LOG_C_D( L"使用SS");
			BOOL bUseSkill = FALSE;
			Point FrontPoint = Monster.GetFrontPoint(8.0f);
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2000, [&bUseSkill, &FrontPoint]{
				if (!bUseSkill)
				{
					CFarmSkill::GetInstance().UseSkill_By_SS();
					BnsSleep(500);
					bUseSkill = TRUE;
				}
				if (CBaseObject::GetPersonDis(FrontPoint) > 2.0f)
					CPersonFunction::GetInstance().Move(FrontPoint);
				return BnsSleep(100);
			});
			Monster.SetAngle();
			BnsSleep(500);
			BnsMove(NowPoint, 5);
		}
		else
		{
			LOG_C_D( L"转圈躲转圈!");
			BnsMove(Monster.GetFrontPoint(8.0f), 2);
		}
		break;
	case 5190360:case 5190366:
		if (CPerson::GetInstance().GetPercentHp() < 50 && CNpcExtend::GetInstance().FindNpc_By_Name(L"내무반장 막소보", CBaseObject::CreateFixPointInfo(20.0f), nullptr))
		{
			CONST static std::vector<Point> vPoint = {
				Point(47245, 45959, 5289), Point(47202, 45810, 5293), Point(47098, 45459, 5305), Point(46981, 45106, 5311),
				Point(46849, 44719, 5311), Point(46730, 44366, 5311), Point(46611, 44014, 5311), Point(46491, 43662, 5311),
			};

			for (UINT i = 0; i < vPoint.size() && GameRun && !EMPTY_PERSONHP; ++i)
				BnsMove(vPoint.at(i), 3);

			while (GameRun && !EMPTY_PERSONHP && CPerson::GetInstance().GetPercentHp() != 100)
			{
				CFarm::GetInstance().EatFood(90);
				BnsSleep(1000);
			}

			for (int i = static_cast<int>(vPoint.size() - 1); i >= 0 && GameRun && !EMPTY_PERSONHP; --i)
				BnsMove(vPoint.at(i), 3);
		}
		break;
	case 0:
		//CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100.0f));
		bRetCode = FALSE;
		break;
	default:
		LOG_C_D( L"default,当前怪物技能=%d", dwBossSkillId);
		break;
	}
	return bRetCode;
}

BOOL CFarmBoss::UseSkill_By_BossSkillId_Sum(_In_ CMonster& Monster, _In_ DWORD dwBossSkillId, _In_ CONST FixPointInfo& FixPtInfo, _In_ CONST CGameUi& Target2_StatusPanel, CCat CatInfo, _In_ DWORD TmpBossSkillId)
{
	VMProtectBegin("Sum-boss");
	auto& FarmSkill = CFarmSkill::GetInstance();
	auto  NowPoint = Monster.GetPoint();
	CMonster Monster_q;//7-11 黑龙教主
	CCat newcat;
	CCatExpend::GetInstance().FindSelfCat(&newcat);
	std::vector<CMonster> VecList;
	CONST static Point Boss_1_point(-3481, -71798, 152);
	CONST static Point Boss_7_11_point(34324, -86092, 7282);
	CONST static Point Xuerendong_BossPoint(-49415, 24578, 14462);
	CONST static Point Xuerendong_TarPoint(-49872, 24648, 14448);//雪人王BOSS要中间要躲避的坐标
	CONST static Point Xuerendong_TarPoint2(-49752, 24648, 14448);//雪人王BOSS要中间要躲避的坐标
	CONST static Point SnowBossPoint(-49868, 24644, 14448);

	CONST static Point HeiBossPoint1(-3979, -6046, 552);//黑森林BOSS走位1
	CONST static Point HeiBossPoint2(-1446, -5668, 544);//黑森林BOSS走位2
	CONST static Point HeiBossPoint3(-3380, -3587, 576);//黑森林BOSS走位3
	CONST static Point HeiBossPoint4(-3959, -6176, 552);//黑森林BOSS走位4
	CONST static Point HeiBossPoint5(-1443, -5564, 541);//黑森林BOSS走位5
	CONST static Point HeiBossPoint6(-3313, -3586, 575);//黑森林BOSS走位6

	ULONGLONG ulTick = ::GetTickCount64();

	BOOL bRetCode = TRUE;
	switch (dwBossSkillId)
	{
		//6-7BOSS	开始 3B6
	case 5100344:
		LOG_C_D( L"BOSS跳起来了!");
		if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_X))
			CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Sum_SkillId_X);

		BnsMove(Monster.GetBehindPoint(2.0f), 3.0f);
		break;
	case 5100341:
		if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_4))
		{
			LOG_C_D( L"5100341用蒲公英..!");
			if (FarmSkill.UseSkill_By_SkillId(Sum_SkillId_4))
			{
				BnsSleep(500);
				CPersonFunction::GetInstance().WaitForProgressBarBySkill(1000);
			}
		}
		else
		{
			if (Monster.GetDis() < 5)
			{
				LOG_C_D( L"5100341用SSSSSSS..!");
				CSomeFun::GetInstance().SetSpeed(3.0f);
				CSomeFun::GetInstance().SimlationKey('S', 600);
				Sleep(1000);
				CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100), FALSE);
				CSomeFun::GetInstance().SetSpeed(3.0f);
			}
		}
		break;
	case 5100366:case 5100365:
		LOG_C_D( L"走后面!");
		CPersonFunction::GetInstance().Move(Monster.GetBehindPoint(2.0f));
		BnsMove(Monster.GetBehindPoint(2.0f), 3.0f);
		Sleep(300);
		Monster.SetAngle();
		break;
	case 5100345:
		if (!CPersonFunction::GetInstance().ExistBuff_Person(0x1915E34, nullptr))
		{
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_X))
			{
				LOG_C_D( L"X可以用，用X档..!");
				FarmSkill.UseSkill_By_SkillId(Sum_SkillId_X);
				Sleep(300);
			}
			else if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_4))
			{
				LOG_C_D( L"用蒲公英..!");
				if (FarmSkill.UseSkill_By_SkillId(Sum_SkillId_4))
				{
					BnsSleep(500);
					CPersonFunction::GetInstance().WaitForProgressBarBySkill(1000);
				}
			}
			else
			{
				if (Monster.GetDis() < 5)
				{
					LOG_C_D( L"5100345用SSSSSSS..!");
					FarmSkill.UseSkill_By_SkillId(Sum_SkillId_1);

					CSomeFun::GetInstance().SetSpeed(3.0f);
					CSomeFun::GetInstance().SimlationKey('S', 600);
					CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100), FALSE);
					CSomeFun::GetInstance().SetSpeed(3.0f);
				}
			}
		}
		break;
	case 5100347:
		LOG_C_D( L"5100347………………");
		if (Monster.GetDis() < 5)
		{
			LOG_C_D( L"距离BOSS小于5，SSSSSS!");
			CSomeFun::GetInstance().SetSpeed(3.0f);
			CSomeFun::GetInstance().SimlationKey('S', 600);
			CSomeFun::GetInstance().SetSpeed(3.0f);
		}
		
		LOG_C_D( L"猫的血量当前血%d,最大血%d", newcat.GetHp(), newcat.GetMaxHp());
		if (newcat.GetPercentHp() < 50 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Z) && CPerson::GetInstance().GetLevel() > 5)
		{
			if (CPerson::GetInstance().GetPercentHp() > 50)
			{
				LOG_C_D( L"猫的血量低于50了，执行鼓励,当前血%d,最大血%d", newcat.GetHp(), newcat.GetMaxHp());
				CSomeFun::GetInstance().SimlationKey('Z', 10);
				Sleep(500);
				CPersonFunction::GetInstance().WaitForProgressBarBySkill(4 * 1000);
			}
			else
			{
				LOG_C_D( L"猫的血量低于50了，但是人物血量低于%%50,不执行鼓励");
			}
		}
		else
		{
			CSkill::UseSkill_By_Id(Sum_SkillId_R, TRUE);
			CSkill::UseSkill_By_Id(Sum_SkillId_R, TRUE);
			CSkill::UseSkill_By_Id(Sum_SkillId_R, TRUE);
			CSkill::UseSkill_By_Id(Sum_SkillId_T, TRUE);
			CSkill::UseSkill_By_Id(Sum_SkillId_R, TRUE);
			CSkill::UseSkill_By_Id(Sum_SkillId_R, TRUE);
			CSkill::UseSkill_By_Id(Sum_SkillId_T, TRUE);
			CSkill::UseSkill_By_Id(Sum_SkillId_R, TRUE);
			CSkill::UseSkill_By_Id(Sum_SkillId_R, TRUE);
		}
		CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100), FALSE);

		
		break;
		//6-7BOSS	

		//破天左边任务BOSS1--开始 召唤师
	case 5900400:case 5900404:
		LOG_C_D( L"统一走后");
		//CPersonFunction::GetInstance().Move(Monster.GetBehindPoint(2.0f));
		BnsMove(Monster.GetBehindPoint(2.0f), 3.0f);
		Monster.SetAngle();
		break;
	case 5900403:
		Monster.SetAngle();
		if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_2))
		{
			LOG_C_D( L"5900410准备放2击倒!");
			CSomeFun::GetInstance().SimlationKey(VK_2, 10);
		}
		else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_C))
		{
			LOG_C_D( L"5900410准备放C虚弱!");
			CSkill::UseSkill_By_Id(Sum_SkillId_C, TRUE, em_Skill_Type_Cat);
			//FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_Tab);
		}
		else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_V))
		{
			LOG_C_D( L"5900410准备放V击倒!");
			CSkill::UseSkill_By_Id(Sum_SkillId_V, TRUE, em_Skill_Type_Cat);
			//FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_Tab);
		}
		else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab))
		{
			LOG_C_D( L"5900410准备放Tab击晕!");
			CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
			//FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_Tab);
		}
		else
		{
			LOG_C_D( L"5900403用蒲公英啦..!");
			Sleep(1800);
			if (FarmSkill.UseSkill_By_SkillId(Sum_SkillId_4))
			{
				Sleep(2000);
			}
			else
			{
				FarmSkill.UseSkill_By_SS();
				Sleep(2000);
			}
		}
		break;
	case 5900410:
		Monster.SetAngle();
		if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_2))
		{
			LOG_C_D( L"5900410准备放2击倒!");
			CSomeFun::GetInstance().SimlationKey(VK_2, 10);
		}
		else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_C))
		{
			LOG_C_D( L"5900410准备放C虚弱!");
			CSkill::UseSkill_By_Id(Sum_SkillId_C, TRUE, em_Skill_Type_Cat);
			//FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_Tab);
		}
		else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_V))
		{
			LOG_C_D( L"5900410准备放V击倒!");
			CSkill::UseSkill_By_Id(Sum_SkillId_V, TRUE, em_Skill_Type_Cat);
			//FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_Tab);
		}
		else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab))
		{
			LOG_C_D( L"5900410准备放Tab击晕!");
			CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
			//FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_Tab);
		}
		else
		{
			LOG_C_D( L"5900410用蒲公英..!");
			CSkill::UseSkill_By_Id(Sum_SkillId_R, FALSE);
			Sleep(300);
			CSkill::UseSkill_By_Id(Sum_SkillId_R, FALSE);
			Sleep(500);
			CSkill::UseSkill_By_Id(Sum_SkillId_T, FALSE);
			Sleep(300);
			CSkill::UseSkill_By_Id(Sum_SkillId_R, FALSE);
			Sleep(300);
			CSkill::UseSkill_By_Id(Sum_SkillId_R, FALSE);
			Sleep(300);
			CSkill::UseSkill_By_Id(Sum_SkillId_T, FALSE);
			Sleep(500);
			CSkill::UseSkill_By_Id(Sum_SkillId_R, FALSE);
			Sleep(300);
			CSkill::UseSkill_By_Id(Sum_SkillId_R, FALSE);
			Sleep(300);
			CSkill::UseSkill_By_Id(Sum_SkillId_T, FALSE);
			if (FarmSkill.UseSkill_By_SkillId(Sum_SkillId_4))
			{
				Sleep(2000);
			}
			else
			{
				FarmSkill.UseSkill_By_SS();
				Sleep(2000);
			}
		}
		break;
	case 5900401:
		if (Monster.GetDis() < 5)
		{
			LOG_C_D( L"5900401距离BOSS小于5，SSSSSS!");
			CSomeFun::GetInstance().SimlationKey('S', 600);
		}
		if (CPerson::GetInstance().GetPercentHp() < 80 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_X))
		{
			LOG_C_D( L"准备放X回血!");
			CSomeFun::GetInstance().SimlationKey('X', 10);
			Sleep(1000);
		}
		else
		{
			Sleep(1500);
		}
		CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100), FALSE);
		
		break;
		//破天左边任务BOSS1--结束 召唤师


		//破天左边任务BOSS2--开始  召唤师
	case 5190071:case 5190070:
		LOG_C_D( L"5190071和5190070");
		if (FarmSkill.UseSkill_By_SkillId(Sum_SkillId_4))
		{
			LOG_C_D( L"5190071和5190070蒲公英");
			Sleep(1500);
		}
		BnsMove(Boss_1_point, 1.0f);
		break;
	case 5190074:case 5190079:
		LOG_C_D( L"统一走后");
		BnsMove(Monster.GetBehindPoint(2.5f), 3.0f);
		if (CPerson::GetInstance().GetPercentHp() < 80 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_X))
		{
			LOG_C_D( L"血不满80，加血X");
			CSomeFun::GetInstance().SimlationKey('X', 10);
		}
		Monster.SetAngle();
		break;
	case 5190073:
			if (Monster.GetName() == L"교도관")
			{
				LOG_C_D( L"5190073--SSSSSSS");
				CSomeFun::GetInstance().SimlationKey('S', 1000);
				Sleep(3500);
				CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100));
			}
			else
			{
				Monster.SetAngle();
				if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_2))
				{
					LOG_C_D( L"5190073准备放2击倒!");
					CSomeFun::GetInstance().SimlationKey(VK_2, 10);
				}
				else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_C))
				{
					LOG_C_D( L"5190073准备放C虚弱!");
					CSkill::UseSkill_By_Id(Sum_SkillId_C, TRUE, em_Skill_Type_Cat);
				}
				else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_V))
				{
					LOG_C_D( L"5190073准备放V击倒!");
					CSkill::UseSkill_By_Id(Sum_SkillId_V, TRUE, em_Skill_Type_Cat);
				}
				else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab))
				{
					LOG_C_D( L"5190073准备放Tab击晕!");
					CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
				}
				else
				{
					LOG_C_D( L"5190073--往sss....");
					CSomeFun::GetInstance().SimlationKey('S', 1000);
					Sleep(3500);
					CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100));
				}
			}
		break;
	case 6200503:
		LOG_C_D( L"召唤怪物了,准备清怪..");
		CSkill::UseSkill_By_Id(Sum_SkillId_R, FALSE);
		Sleep(500);
		CSkill::UseSkill_By_Id(Sum_SkillId_R, FALSE);
		Sleep(500);
		CSkill::UseSkill_By_Id(Sum_SkillId_R, FALSE);
		Sleep(500);
		CSkill::UseSkill_By_Id(Sum_SkillId_T, FALSE);
		Sleep(500);
		CSkill::UseSkill_By_Id(Sum_SkillId_R, FALSE);
		Sleep(500);
		CSkill::UseSkill_By_Id(Sum_SkillId_T, FALSE);
		Sleep(1000);

		LOG_C_D( L"准备执行清怪.....");
		ClearAroundMonster(Monster, L"흑랑", CBaseObject::CreateFixPointInfo(30.0f));
		CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100));
		break;
		//破天左边任务BOSS2--结束  召唤师

		//破天左边任务BOSS3--开始  召唤师
	case 5101100:case 5101101:case 5101102:case 5101103:case 5101104:case 5101106:case 5101131:
		LOG_C_D( L"统一走后");
		BnsMove(Monster.GetBehindPoint(2.0f), 1.0f);
		if (CPerson::GetInstance().GetPercentHp() < 80 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_X))
		{
			LOG_C_D( L"血不满80，加血X");
			CSomeFun::GetInstance().SimlationKey('X', 10);
		}
		Monster.SetAngle();
		break;
	case 5101105:
		Monster.SetAngle();
		if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_2))
		{
			LOG_C_D( L"5101105准备放2击倒!");
			CSomeFun::GetInstance().SimlationKey(VK_2, 10);
		}
		else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_C))
		{
			LOG_C_D( L"5101105准备放C虚弱!");
			CSkill::UseSkill_By_Id(Sum_SkillId_C, TRUE, em_Skill_Type_Cat);
		}
		else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_V))
		{
			LOG_C_D( L"5101105准备放V击倒!");
			CSkill::UseSkill_By_Id(Sum_SkillId_V, TRUE, em_Skill_Type_Cat);
		}
		else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab))
		{
			LOG_C_D( L"5101105准备放Tab击晕!");
			CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
		}
		else
		{
			FarmSkill.UseSkill_By_SS();
			Sleep(1000);
		}
		break;
		//破天左边任务BOSS3-结束  召唤师


		//破天右边任务BOSS1-开始  召唤师
	case 5190601:case 5190600:case 5190604:
		LOG_C_D( L"统一走后");
		CPersonFunction::GetInstance().Move(Monster.GetBehindPoint(2.0f));
		BnsMove(Monster.GetBehindPoint(2.0f), 1.0f);
		if (CPerson::GetInstance().GetPercentHp() < 80 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_X))
		{
			LOG_C_D( L"血不满80，加血X");
			CSomeFun::GetInstance().SimlationKey('X', 10);
		}
		Monster.SetAngle();
		break;
	case 5190606:case 5190602:case 5190603:case 5190607:
		Monster.SetAngle();
		if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_2))
		{
			LOG_C_D( L"5101105准备放2击倒!");
			CSomeFun::GetInstance().SimlationKey(VK_2, 10);
		}
		else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_C))
		{
			LOG_C_D( L"5101105准备放C虚弱!");
			CSkill::UseSkill_By_Id(Sum_SkillId_C, TRUE, em_Skill_Type_Cat);
		}
		else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_V))
		{
			LOG_C_D( L"5101105准备放V击倒!");
			CSkill::UseSkill_By_Id(Sum_SkillId_V, TRUE, em_Skill_Type_Cat);
		}
		else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab))
		{
			LOG_C_D( L"5101105准备放Tab击晕!");
			CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
		}
		else
		{
			FarmSkill.UseSkill_By_SS();
			Sleep(1000);
			CSomeFun::GetInstance().SimlationKey(VK_4, 10);
			Sleep(2000);
		}
		break;
		//破天右边任务BOSS1-结束  召唤师

		//破天右边任务BOSS2-开始  召唤师
	case 5090690:
		if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_4))
		{
			CSomeFun::GetInstance().SimlationKey(VK_4, 10);
			Sleep(2000);
		}
		break;
	case 5090681:case 5090680:
		LOG_C_D( L"统一走后");
		BnsMove(Monster.GetBehindPoint(2.0f), 1.0f);
		if (CPerson::GetInstance().GetPercentHp() < 80 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_X))
		{
			LOG_C_D( L"血不满80，加血X");
			CSomeFun::GetInstance().SimlationKey('X', 10);
		}
		Monster.SetAngle();
		break;

	case 5090687:case 5090682:case 5090683:
		Monster.SetAngle();
		if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_2))
		{
			LOG_C_D( L"5101105准备放2击倒!");
			CSomeFun::GetInstance().SimlationKey(VK_2, 10);
		}
		else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_C))
		{
			LOG_C_D( L"5101105准备放C虚弱!");
			CSkill::UseSkill_By_Id(Sum_SkillId_C, TRUE, em_Skill_Type_Cat);
		}
		else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_V))
		{
			LOG_C_D( L"5101105准备放V击倒!");
			CSkill::UseSkill_By_Id(Sum_SkillId_V, TRUE, em_Skill_Type_Cat);
		}
		else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab))
		{
			LOG_C_D( L"5101105准备放Tab击晕!");
			CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
		}
		else
		{
			FarmSkill.UseSkill_By_SS();
			Sleep(1000);
			CSomeFun::GetInstance().SimlationKey(VK_4, 10);
			Sleep(2000);
		}
		break;
		//破天右边任务BOSS2-结束   召唤师

		//破天右边每日任务BOSS2-开始  召唤师

	case 5101236:case 5101238:
		Monster.SetAngle();
		if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_2))
		{
			LOG_C_D( L"5101105准备放2击倒!");
			CSomeFun::GetInstance().SimlationKey(VK_2, 10);
		}
		else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_C))
		{
			LOG_C_D( L"5101105准备放C虚弱!");
			CSkill::UseSkill_By_Id(Sum_SkillId_C, TRUE, em_Skill_Type_Cat);
		}
		else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_V))
		{
			LOG_C_D( L"5101105准备放V击倒!");
			CSkill::UseSkill_By_Id(Sum_SkillId_V, TRUE, em_Skill_Type_Cat);
		}
		else if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab))
		{
			LOG_C_D( L"5101105准备放Tab击晕!");
			CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
		}
		else
		{
			FarmSkill.UseSkill_By_SS();
			Sleep(1000);
			CSomeFun::GetInstance().SimlationKey(VK_4, 10);
			Sleep(2000);
		}
		break;
	case 5101239:case 5101235:case 5101234:case 5101240:case 5101242:case 5101675:
		LOG_C_D( L"统一走后");
		BnsMove(Monster.GetBehindPoint(2.0f), 3.0f);
		if (CPerson::GetInstance().GetPercentHp() < 80 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_X))
		{
			LOG_C_D( L"血不满80，加血X");
			CSomeFun::GetInstance().SimlationKey('X', 10);
		}
		Monster.SetAngle();
		break;
		//破天右边每日任务BOSS2-结束  召唤师

		//7-11 黑龙教主 开始--召唤师
	case 5101681:case 5101692:case 5101688:
		LOG_C_D( L"统一走后");
		BnsMove(Monster.GetBehindPoint(2.0f), 2.0f);
		Monster.SetAngle();
		if (CPerson::GetInstance().GetPercentHp() < 90 && CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_2))
		{
			LOG_C_D( L"准备放2++++!");
			CSomeFun::GetInstance().SimlationKey(VK_2, 10);
		}
		else if (CPerson::GetInstance().GetPercentHp() < 81 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_X))
		{
			LOG_C_D( L"准备XXXX+++++++!");
			CSomeFun::GetInstance().SimlationKey('X', 10);
		}
		break;
	case 5101676:case 5101682:
		if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab))
		{
			LOG_C_D( L"可以放TAB双控++++!");
			CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
		}
		else
		{
			LOG_C_D( L"case 5101676:case 5101682:case 5101675: 统一走后");
			BnsMove(Monster.GetBehindPoint(2.0f), 2.0f);
			Monster.SetAngle();
		}
		if (CPerson::GetInstance().GetPercentHp() < 81 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_X))
		{
			LOG_C_D( L"准备XXXX+++++++!");
			CSomeFun::GetInstance().SimlationKey('X', 10);
		}
		if (newcat.GetPercentHp() < 90 && CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_C))
		{
			LOG_C_D( L"准备放C++++!");
			CSkill::UseSkill_By_Id(Sum_SkillId_C, TRUE, em_Skill_Type_Cat);
		}
		break;
	case 5101697:
		LOG_C_D( L"准备找球-----------");
		CSomeFun::GetInstance().SimlationKey(VK_4, 10);
		if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_4_4))
		{
			FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_4_4);
		}
		while (GameRun && !EMPTY_PERSONHP && CMonsterExtend::GetInstance().FindMonster_By_Name(L"마기탄", CBaseObject::CreateFixPointInfo(30.0f), &Monster_q))
		{
			LOG_C_D( L"找到球了,过去踩死。-----------");
			CFarm::GetInstance().Move_To_Monster(Monster_q, CBaseObject::CreateFixPointInfo(30.0f));
			BnsSleep(500);
		}
		BnsMove(Boss_7_11_point, 3.0f);
		FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_E);
		if (CPerson::GetInstance().GetPercentHp() < 81 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_X))
		{
			LOG_C_D( L"准备XXXX+++++++!");
			CSomeFun::GetInstance().SimlationKey('X', 10);
		}
		break;
	case 5101685:case 5101684:case 5101678:case 5101689:case 5101698:
		LOG_C_D( L"追杀我呢，放Q挡一下。");
		CSomeFun::GetInstance().SimlationKey('Q', 10);
		FarmSkill.UseSkill_By_SS();
		if (CPerson::GetInstance().GetPercentHp() < 81 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_X))
		{
			LOG_C_D( L"准备XXXX+++++++!");
			CSomeFun::GetInstance().SimlationKey('X', 10);
		}
		break;
	case 5101674:
		LOG_C_D( L"看看算不算该放无敌了");
		Sleep(1200);
		FarmSkill.UseSkill_By_SS();
		Sleep(1500);
		break;
	case 5101691:
		Sleep(1000);
		FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_E);
		Sleep(1000);
		FarmSkill.UseSkill_By_SS();
		Sleep(1000);
		CSomeFun::GetInstance().SimlationKey(VK_4, 10);
		FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_4_4);
		break;
			//7-11 黑龙教主 结束--召唤师

		//鬼门关-万魂 召唤师
	case 5902311:case 5902312:case 5902313:case 5902314:case 5902319:
		LOG_C_D( L"走后");
		BnsMove(Monster.GetBehindPoint(2.5f), 3.0f);
		if (CObjectFunction::GetInstance().ExistMonsterBuff(Monster.GetId(), 0xF5BE37))
		{
			if (newcat.GetPercentHp() < 45)
			{
				if (CPerson::GetInstance().GetPercentHp() > 70)
				{
					CSomeFun::GetInstance().SimlationKey('Z', 10);
					Sleep(500);
				}
				else
				{
					LOG_C_D( L"要给猫加血，但是人物的血量不足70");
				}
			}
		}
		if (CPerson::GetInstance().GetPercentHp() < 80 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_X))
		{
			LOG_C_D( L"血不满80，加血X");
			CSomeFun::GetInstance().SimlationKey('X', 10);
		}
		break;
	case 5902315:
		LOG_C_D( L"5902315,飞过来。");
		CSomeFun::GetInstance().SetSpeed(3.0f);
		CSomeFun::GetInstance().SimlationKey('S', 600);
		CSomeFun::GetInstance().SetSpeed(3.0f);
		CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100));
		break;
	case 5902320:
		LOG_C_D( L"召唤小怪,继续干。");
		//召唤小怪,不管他.
		break;
	case 5902316:
		LOG_C_D( L"吸人，不管他。继续干。");
		break;
	case 5902318:
		if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_4))
		{
			CSomeFun::GetInstance().SimlationKey(VK_4, 10);
			Sleep(2000);
		}
		else
		{
			CSomeFun::GetInstance().SetSpeed(3.0f);
			CSomeFun::GetInstance().SimlationKey('S', 600);
			CSomeFun::GetInstance().SetSpeed(3.0f);
			CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100));
		}
		break;
		//鬼门关-万魂 召唤师-结束

		//鬼门关-黑天照 召唤师-开始
	case 5902376:case 5902377:case 5902380:
		LOG_C_D( L"走后");
		BnsMove(Monster.GetBehindPoint(2.5f), 3.0f);
		if (CObjectFunction::GetInstance().ExistMonsterBuff(Monster.GetId(), 0xF5BE37))
		{
			if (newcat.GetPercentHp() < 45)
			{
				if (CPerson::GetInstance().GetPercentHp() > 70)
				{
					CSomeFun::GetInstance().SimlationKey('Z', 10);
					Sleep(500);
				}
				else
				{
					LOG_C_D( L"要给猫加血，但是人物的血量不足70");
				}
			}
		}
		if (CPerson::GetInstance().GetPercentHp() < 80 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_X))
		{
			LOG_C_D( L"血不满80，加血X");
			CSomeFun::GetInstance().SimlationKey('X', 10);
		}
		break;
	case 5902379://要控制
		if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_2))
		{
			LOG_C_D( L"控制----，2");
			CSomeFun::GetInstance().SimlationKey(VK_2, 10);
		}
		else if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_V))
		{
			LOG_C_D( L"控制----，V");
			FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_V);
		}
		else if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_C))
		{
			LOG_C_D( L"控制----，C");
			FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_C);
		}
		break;
	case 5902382:
		CSkill::UseSkill_By_Id(Sum_SkillId_R);
		Sleep(300);
		CSkill::UseSkill_By_Id(Sum_SkillId_R);
		Sleep(300);
		CSkill::UseSkill_By_Id(Sum_SkillId_R);
		Sleep(300);
		CSkill::UseSkill_By_Id(Sum_SkillId_T);
		CSomeFun::GetInstance().SimlationKey(VK_4, 10);
		Sleep(2000);
		break;
	case 5902381:
		LOG_C_D( L"不管。继续干。");
		break;
		//鬼门关-黑天照 召唤师-结束
		
		//鬼门关-爆魔人 召唤师-开始
	case 5902397:case 5902304:case 5902301:
		LOG_C_D( L"走后");
		BnsMove(Monster.GetBehindPoint(3.0f), 3.0f);
		if (CObjectFunction::GetInstance().ExistMonsterBuff(Monster.GetId(), 0xF5BE37))
		{
			if (newcat.GetPercentHp() < 45)
			{
				if (CPerson::GetInstance().GetPercentHp() > 70)
				{
					CSomeFun::GetInstance().SimlationKey('Z', 10);
					Sleep(500);
				}
				else
				{
					LOG_C_D( L"要给猫加血，但是人物的血量不足70");
				}
			}
		}
		if (CPerson::GetInstance().GetPercentHp() < 80 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_X))
		{
			LOG_C_D( L"血不满80，加血X");
			CSomeFun::GetInstance().SimlationKey('X', 10);
		}
		break;
	case 5902303:
		LOG_C_D( L"不管。继续干。");
		break;
	case 502300:
		CSkill::UseSkill_By_Id(Sum_SkillId_R);
		Sleep(300);
		CSkill::UseSkill_By_Id(Sum_SkillId_R);
		Sleep(300);
		CSkill::UseSkill_By_Id(Sum_SkillId_R);
		Sleep(300);
		CSkill::UseSkill_By_Id(Sum_SkillId_T);
		CSomeFun::GetInstance().SimlationKey(VK_4, 10);
		Sleep(2000);
		break;
		//鬼门关-爆魔人 召唤师-结束


		//鬼门关-BOSS 召唤师-开始
	case 5902383:
		//走45度角
		LOG_C_D( L"走45");
		BnsMove(Monster.GetLeftPoint(3.0f),3.0f);
		Monster.SetAngle();
		if (CTeamMemberExtend::GetInstance().GetTeamMember_By_LessPercentHp(70, nullptr))
		{
			if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == Guimenguan_jiaxue)
			{
				LOG_C_D( L"队员%d执行加血", Guimenguan_jiaxue);
				//FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_Z, TRUE, em_Skill_Type_Cat);
				CSomeFun::GetInstance().SimlationKey('Z', 10);
				CSomeFun::GetInstance().SimlationKey('Z', 10);
			}
		}

		if (Guimenguan_jiaxue == 0 || Guimenguan_jiaxue == 1)
		{
			if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 0 || CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 1)
			{
				Monster.SetAngle();
				Sleep(200);
				LOG_C_D( L"队员1,2执行技能1");
				CSomeFun::GetInstance().SimlationKey(VK_1, 10);
			}
		}
		else if (Guimenguan_jiaxue == 2 || Guimenguan_jiaxue == 3)
		{
			if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 2 || CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 3)
			{
				Monster.SetAngle();
				Sleep(200);
				LOG_C_D( L"队员3,4执行技能1");
				CSomeFun::GetInstance().SimlationKey(VK_1, 10);
			}
		}
		else
		{
			if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 4 || CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 5)
			{
				Monster.SetAngle();
				Sleep(200);
				LOG_C_D( L"队员5,6执行技能1");
				CSomeFun::GetInstance().SimlationKey(VK_1, 10);
			}
		}
		Guimenguan_jiaxue++;
		if (Guimenguan_jiaxue > 5)
		{
			Guimenguan_jiaxue = 0;
		}
		break;
	case 5902384:case 5902385:
		LOG_C_D( L"走后");
		BnsMove(Monster.GetBehindPoint(3.0f), 3.0f);
		Monster.SetAngle();
		if (CTeamMemberExtend::GetInstance().GetTeamMember_By_LessPercentHp(70, nullptr))
		{
			if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == Guimenguan_jiaxue)
			{
				LOG_C_D( L"队员%d执行加血", Guimenguan_jiaxue);
				//FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_Z, TRUE, em_Skill_Type_Cat);
				CSomeFun::GetInstance().SimlationKey('Z', 10);
				CSomeFun::GetInstance().SimlationKey('Z', 10);
			}
		}

		if (Guimenguan_jiaxue == 0 || Guimenguan_jiaxue == 1)
		{
			if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 0 || CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 1)
			{
				Monster.SetAngle();
				Sleep(200);
				LOG_C_D( L"队员1,2执行技能1");
				CSomeFun::GetInstance().SimlationKey(VK_1, 10);
			}
		}
		else if (Guimenguan_jiaxue == 2 || Guimenguan_jiaxue == 3)
		{
			if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 2 || CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 3)
			{
				Monster.SetAngle();
				Sleep(200);
				LOG_C_D( L"队员3,4执行技能1");
				CSomeFun::GetInstance().SimlationKey(VK_1, 10);
			}
		}
		else
		{
			if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 4 || CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 5)
			{
				Monster.SetAngle();
				Sleep(200);
				LOG_C_D( L"队员5,6执行技能1");
				CSomeFun::GetInstance().SimlationKey(VK_1, 10);
			}
		}
		Guimenguan_jiaxue++;
		if (Guimenguan_jiaxue > 5)
		{
			Guimenguan_jiaxue = 0;
		}
		break;
	case 5902386:case 5902388:
		//控制
		if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab))
		{
			CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
		}
		else if(FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_2))
		{
			CSomeFun::GetInstance().SimlationKey(VK_2, 10);
		}
		else if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_V))
		{
			FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_V);
		}
		break;
	case 5902389:
		//召唤猫
		//LOG_C_D( L"召唤猫来帮我挡");
		//FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_E);
		LOG_C_D( L"SSSSSSSSSSSSSSSSS");
		CSomeFun::GetInstance().SetSpeed(3.0f);
		CSomeFun::GetInstance().SimlationKey('S', 900);
		CSomeFun::GetInstance().SetSpeed(3.0f);
		CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100));
		break;
	case 5902391:
		LOG_C_D( L"进入大招策略.....................");

		Guimenguan_boss = TRUE;
		Guimenguan_boss_Count++;

		if (FarmSkill.IsCool_By_SkillId_In_HideUi(Sum_SkillId_Q))
		{
			FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_Q);
		}

		break;
	case 5902400:
		LOG_C_D( L"狂暴了，但是继续使用大招策略....................");
		Guimenguan_boss = TRUE;
		Guimenguan_boss_Count++;
		if (FarmSkill.IsCool_By_SkillId_In_HideUi(Sum_SkillId_Q))
		{
			FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_Q);
		}
		break;
		//鬼门关-BOSS 召唤师-结束


		//雪人洞-Boss 召唤师-开始
	case 5902250:case 5902251:
		LOG_C_D( L"走后");
		BnsMove(Monster.GetBehindPoint(3.0f), 3.0f);
		break;
	case 5902252:
		LOG_C_D( L"SSSSSSSSSSSSSS");
		CSomeFun::GetInstance().SetSpeed(10.0f);
		CSomeFun::GetInstance().SimlationKey('S', 800);
		CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100));
		CSomeFun::GetInstance().SetSpeed(3.0f);
		break;
	case 5902253:
		LOG_C_D( L"S,600 跳目标");
		CSomeFun::GetInstance().SetSpeed(10.0f);
		CSomeFun::GetInstance().SimlationKey('S', 700);
		if (CTeamMemberExtend::GetInstance().GetTeamMember_By_LessPercentHp(70, nullptr))
		{
			LOG_C_D( L"有队员血不满50，加血Z");
			CSomeFun::GetInstance().SimlationKey('Z', 10);
			Sleep(1000);
		}
		else
		{
			Sleep(1000);
			CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100));
		}
		CSomeFun::GetInstance().SetSpeed(3.0f);
		break;
	case 5902256:
		LOG_C_D( L"5902256-----");
		Sleep(1000);
		FarmSkill.UseSkill_By_SS();
		Sleep(1000);
		break;
	case 5902254:
		LOG_C_D( L"5902254-----");
		//BnsMove(Xuerendong_TarPoint, 3.0f);
		//Monster.SetAngle();
		//GetMonsterSkillId() = 0;
		//LOG_C_D( L"等它后退1.6秒");
		//Sleep(1600);
		//LOG_C_D( L"当前技能---%d", GetMonsterSkillId());
		//if (GetMonsterSkillId() == 5902252 || GetMonsterSkillId() == 0)
		//{
		//	Sleep(700);
		//	CSomeFun::GetInstance().SimlationKey(VK_4, 10);
		//	Sleep(1000);
		//	CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100));
		//	Sleep(1000);
		//}
		//else if (GetMonsterSkillId() == 5902256)
		//{
		//	/*CSomeFun::GetInstance().SetSpeed(3.0f);
		//	CSomeFun::GetInstance().SimlationKey('S', 1200);
		//	CSomeFun::GetInstance().SimlationKey(VK_4, 10);*/
		//	Sleep(1000);
		//	FarmSkill.UseSkill_By_SS();
		//	Sleep(1000);
		//	//CSomeFun::GetInstance().SetSpeed(3.0f);
		//}
		//else if (GetMonsterSkillId() == 5902257)
		//{
		//	LOG_C_D( L"5902254中的5902257------%d", Xueren_flag);

		//	if (Xueren_flag % 2 == 0)
		//	{
		//		CSomeFun::GetInstance().SetSpeed(3.0f);
		//		LOG_C_D( L"准备控制....");
		//		if (CPersonFunction::GetInstance().ExistBuff_Person(0x3849D45, (GAMEOBJ_BUFF*)nullptr))
		//		{
		//			BnsMove(Xuerendong_BossPoint, 3.0f);
		//			FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_E);
		//			Sleep(1500);
		//			LOG_C_D( L"对准放tab...");
		//			Monster.SetAngle();
		//			Sleep(1000);
		//			CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
		//		}
		//		else
		//		{
		//			BnsMove(Xuerendong_TarPoint, 3.0f);
		//			Sleep(500);
		//			BnsMove(Xuerendong_BossPoint, 3.0f);
		//			FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_E);
		//			Sleep(1700);
		//			LOG_C_D( L"对准放tab....");
		//			Monster.SetAngle();
		//			Sleep(1000);
		//			CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
		//		}
		//	}
		//	else
		//	{
		//		LOG_C_D( L"S---等2.5秒-----------------第1次");
		//		BnsMove(Xuerendong_BossPoint, 3.0f);

		//		if (CTeamMemberExtend::GetInstance().GetTeamMember_By_LessPercentHp(70, nullptr))
		//		{
		//			CSomeFun::GetInstance().SimlationKey('Z', 10);
		//			FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_E);
		//			Monster.SetAngle();
		//			Sleep(2500);
		//		}
		//		else
		//		{
		//			FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_E);
		//			Monster.SetAngle();
		//			Sleep(2800);
		//		}
		//	}
		//	CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100));
		//	Xueren_flag++;
		//	CSomeFun::GetInstance().SetSpeed(3.0f);
		//}

		if (Xueren_2254 == 1)
		{
			if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 0)
			{
				LOG_C_D( L"队长执行1111");
				CSomeFun::GetInstance().SimlationKey(VK_1, 10);
			}
		}
		else if (Xueren_2254 == 2)
		{
			if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 1)
			{
				LOG_C_D( L"队员1执行1111");
				CSomeFun::GetInstance().SimlationKey(VK_1, 10);
			}
		}
		else if (Xueren_2254 == 3)
		{
			if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 2)
			{
				LOG_C_D( L"队员2执行1111");
				CSomeFun::GetInstance().SimlationKey(VK_1, 10);
			}
		}
		else if (Xueren_2254 == 4)
		{
			if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 3)
			{
				LOG_C_D( L"队员3执行1111");
				CSomeFun::GetInstance().SimlationKey(VK_1, 10);
			}
		}
		else if (Xueren_2254 == 5)
		{
			if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 4)
			{
				LOG_C_D( L"队员4执行1111");
				CSomeFun::GetInstance().SimlationKey(VK_1, 10);
			}
		}
		else if (Xueren_2254 == 6)
		{
			if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 5)
			{
				LOG_C_D( L"队员5执行1111");
				CSomeFun::GetInstance().SimlationKey(VK_1, 10);
			}
			Xueren_2254 = 1;
		}
		Xueren_2254++;
		break;
	case 5902257:
		LOG_C_D( L"5902257------%d", Xueren_flag);
		CSomeFun::GetInstance().SetSpeed(10.0f);
		if (Xueren_flag % 2 == 0)
		{
			LOG_C_D( L"准备用2打断!");
			/*CONST static Point SnowBossPoint(-49868, 24644, 14448);
			std::vector<Point> VecBossAround;
			CFarm::GetInstance().GetRoundPointList(FixPointInfo(SnowBossPoint, 8), 6, VecBossAround);
			// 先分散站开! 防止一起被压
			BnsMove(VecBossAround.at(CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex()), 3.0f);
			Sleep(2000);
			
			// 靠近BOSS
			LOG_C_D( L"靠近BOSS!");
			BnsMove(Monster.GetFrontPoint(8), 1);
			Monster.SetAngle();
			Sleep(800);
			CSomeFun::GetInstance().SimlationKey(VK_2, 10);*/
			CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NMAE_ISPUASEHELPTHREAD, FALSE);
			CSomeFun::GetInstance().SetSpeed(3.0f);
			LOG_C_D( L"准备控制....");
			if (CPersonFunction::GetInstance().ExistBuff_Person(0x3849D45, (GAMEOBJ_BUFF*)nullptr))
			{
				BnsMove(Xuerendong_BossPoint, 3.0f);
				FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_E);
				Sleep(1000);
				CSomeFun::GetInstance().SimlationKey('V', 10);
				if (Monster.GetDis() >= 8)
					BnsMove(Monster.GetFrontPoint(8), 1);

				Sleep(500);
				LOG_C_D( L"对准放2222....");
				Monster.SetAngle();
				Sleep(800);
				//CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
				if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_2))
					CSomeFun::GetInstance().SimlationKey(VK_2, 10);
				else
					FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_Tab, TRUE, em_Skill_Type_Cat);
			}
			else
			{
				BnsMove(Xuerendong_TarPoint, 3.0f);
				Sleep(600);
				BnsMove(Xuerendong_BossPoint, 3.0f);
				Sleep(2000);
				CSomeFun::GetInstance().SimlationKey('V', 10);
				if (Monster.GetDis() >= 8)
					BnsMove(Monster.GetFrontPoint(8), 1);

				Sleep(600);
				LOG_C_D( L"对准放222....");
				Monster.SetAngle();
				Sleep(800);
				if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_2))
					CSomeFun::GetInstance().SimlationKey(VK_2, 10);
				else
					FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_Tab, TRUE, em_Skill_Type_Cat);
			}
			CSomeFun::GetInstance().SimlationKey('X', 10);
		}
		else
		{
			std::vector<Point> VecBossAround;
			CFarm::GetInstance().GetRoundPointList(FixPointInfo(SnowBossPoint, 8), 6, VecBossAround);
			LOG_C_D( L"S---等2.5秒-----------------第1次");
			BnsMove(VecBossAround.at(CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex()), 3.0f);
			Sleep(1800);
			if (CTeamMemberExtend::GetInstance().GetTeamMember_By_LessPercentHp(70, nullptr))
			{
				CSomeFun::GetInstance().SimlationKey('Z', 10);
				FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_E);
				Monster.SetAngle();
			}
			else
			{
				FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_E);
				Monster.SetAngle();
			}
			Sleep(2500);
		}
		CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100));
		Xueren_flag++;
		CSomeFun::GetInstance().SetSpeed(3.0f);

		break;
	case 5902265:
//	lb_5902265 : ;
		LOG_C_D( L"雪人王-不能控制,放无敌");
		//BnsMove(Monster.GetBehindPoint(1.0f), 3.0f);
		//BnsMove(Xuerendong_TarPoint2, 3.0f);
		// 喵用Q来防止被冻住然后被秒!
		CSkill::UseSkill_By_Id(Sum_SkillId_Q, FALSE, em_Skill_Type_Cat);

		Monster.SetAngle();
		CSkill::UseSkill_By_Id(Sum_SkillId_R, FALSE);
		Sleep(400);
		CSkill::UseSkill_By_Id(Sum_SkillId_R, FALSE);
		Sleep(400);
		CSkill::UseSkill_By_Id(Sum_SkillId_T, FALSE);

		Monster.SetAngle();
		Sleep(400);
		CSkill::UseSkill_By_Id(Sum_SkillId_R, FALSE);
		Sleep(200);
		CSkill::UseSkill_By_Id(Sum_SkillId_4, FALSE);
		Sleep(500);
		if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_4_4))
			CSkill::UseSkill_By_Id(Sum_SkillId_4_4, FALSE);
		Sleep(1500);
		Xueren_2265++;
		break;
	case 5902259:
		LOG_C_D( L"吸-------"); 
		CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NMAE_ISPUASEHELPTHREAD, TRUE);
		//Sleep(1150);
		//CSkill::UseSkill_By_Id(Sum_SkillId_4, FALSE);
		//Sleep(2000);
		//Sleep(1150);
		//FarmSkill.UseSkill_By_SS();
		//Sleep(1000);
		break;
	case 5902258:
		LOG_C_D( L"吸热----");
		CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_SNOWBOSS_FINSKILL, FALSE);
		CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NMAE_ISPUASEHELPTHREAD, FALSE);
		/*{
			CPlayer Player;
			if (CPlayerExtend::GetInstance().FindPlayer_By_Id(Monster.GetTargetId(), CBaseObject::CreateFixPointInfo(-1), &Player))
			{
				LOG_C_D( L"%s 身上带有热BUFF!", Player.GetName().c_str());
				if (Player.GetId() == CPerson::GetInstance().GetId())
					BnsSleep(3000);
				else
				{
					while (GameRun && !EMPTY_PERSONHP && !CPersonFunction::GetInstance().ExistBuff_Person(0x3849D45))
					{
						BnsMove(Player.GetPoint(), 1);
						BnsSleep(500);
					}
					CSomeFun::GetInstance().SimlationKey('X', 10);
				}
				
			}
		}*/
		
		//CSomeFun::GetInstance().SimlationKey('Q', 10);
		break;
	case 5902260:
		LOG_C_D( L"摆姿势---");
		break;
	case 5902261:
		LOG_C_D( L"检查周围冰块的位置----准备执行清怪");
		MoveToLightBySnowBoss(Monster);
		CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_SNOWBOSS_FINSKILL, TRUE);
		break;
	case 5902262:
		LOG_C_D( L"双SS----");
		// 猫要用Q防止被冻住, 然后判断使用Q用去了多少时间， 保证是在1250之内完成.
		CSkill::UseSkill_By_Id(Sum_SkillId_Q, FALSE, em_Skill_Type_Cat);
		Sleep(1250 - static_cast<DWORD>(::GetTickCount64() - ulTick));
		FarmSkill.UseSkill_By_SS();
		CGameVariable::GetInstance().Action_Variable_By_Name(VARIABLE_NAME_SNOWBOSS_FINSKILL, TRUE, [&FarmSkill]
		{
			Sleep(500);
			CSomeFun::GetInstance().SimlationKey('X', 10);
		});;
		//
		//CSomeFun::GetInstance().TimeOut_Condiction_GameRun(10 * 1000, [this]{ return GetMonsterSkillId() != 5902265; });
		//LOG_C_D( L"好了,BOSS开始捶地了!");
		//goto lb_5902265;
		break;
	case 5902263:case 5902264:
		LOG_C_D( L"8连开始----");
		CSomeFun::GetInstance().SimlationKey('X', 10);
		Xueren_2263++;

		break;
		//雪人洞老三结束------------------

		//雪人洞-Boss 召唤师-结束

		//黑森林-Boss 召唤师-开始
	case 5902706:case 5902708:case 5902716:case 5902719:
		LOG_C_D( L"黑森林走后");
		MoveToPoint(Monster.GetBehindPoint(3.0f), 3.0f);
		Monster.SetAngle();
		if (CTeamMemberExtend::GetInstance().GetTeamMember_By_LessPercentHp(70, nullptr))
		{
			LOG_C_D( L"走45,有队员70以下血了，加血。");
			CSomeFun::GetInstance().SimlationKey('Z', 10);
			CSomeFun::GetInstance().SimlationKey('Z', 10);
		}
		break;
	case 5902703:
		if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_2))
		{
			CSomeFun::GetInstance().SimlationKey(VK_2, 10);
		}
		else
		{
			LOG_C_D( L"黑森林走后");
			MoveToPoint(Monster.GetBehindPoint(3.0f), 3.0f);
			Monster.SetAngle();
			if (CTeamMemberExtend::GetInstance().GetTeamMember_By_LessPercentHp(70, nullptr))
			{
				LOG_C_D( L"走45,有队员70以下血了，加血。");
				CSomeFun::GetInstance().SimlationKey('Z', 10);
				CSomeFun::GetInstance().SimlationKey('Z', 10);
			}
		}
		
		break;
	case 5902704:
		if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_C))
		{
			CSomeFun::GetInstance().SimlationKey('C', 10);
		}
		else
		{
			LOG_C_D( L"黑森林走后");
			MoveToPoint(Monster.GetBehindPoint(3.0f), 3.0f);
			Monster.SetAngle();
			if (CTeamMemberExtend::GetInstance().GetTeamMember_By_LessPercentHp(70, nullptr))
			{
				LOG_C_D( L"走45,有队员70以下血了，加血。");
				CSomeFun::GetInstance().SimlationKey('Z', 10);
				CSomeFun::GetInstance().SimlationKey('Z', 10);
			}
		}
		break;
	case 5902705:
		FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_R);
		Sleep(500);
		CSomeFun::GetInstance().SimlationKey(VK_4, 10);
		Sleep(2000);
		break;
	case 5902709:
		LOG_C_D( L"黑森林秒人技能走后");
		MoveToPoint(Monster.GetBehindPoint(3.0f), 3.0f);
		Monster.SetAngle();
		if (CTeamMemberExtend::GetInstance().GetTeamMember_By_LessPercentHp(70, nullptr))
		{
			LOG_C_D( L"走45,有队员70以下血了，加血。");
			CSomeFun::GetInstance().SimlationKey('Z', 10);
			CSomeFun::GetInstance().SimlationKey('Z', 10);
		}
		/*LOG_C_D( L"等待秒人技能");
		if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 3)
		{
			CSomeFun::GetInstance().SetSpeed(6.0f);
			if (Strike_Zhongzi_Count == 1 ||
				Strike_Zhongzi_Count == 4)
			{
				LOG_C_D( L"走1点");
				CPersonFunction::GetInstance().MoveToPoint(HeiBossPoint1, 3.0f, 150.0f);
				
				Sleep(8000);
				LOG_C_D( L"5902709等待8秒");
				LOG_C_D( L"往点3走。");
				CPersonFunction::GetInstance().MoveToPoint(HeiBossPoint3, 3.0f, 150.0f);
				Sleep(9000);
				LOG_C_D( L"5902709等待9秒");
			}
			else if (Strike_Zhongzi_Count == 2 ||
				Strike_Zhongzi_Count == 5)
			{
				LOG_C_D( L"走2点");
				CPersonFunction::GetInstance().MoveToPoint(HeiBossPoint2, 3.0f, 150.0f);
				Sleep(8000);
				LOG_C_D( L"5902709等待8秒");
				LOG_C_D( L"往点1走。");
				CPersonFunction::GetInstance().MoveToPoint(HeiBossPoint1, 3.0f, 150.0f);
				Sleep(9000);
				LOG_C_D( L"5902709等待9秒2此等待");
			}
			else if (Strike_Zhongzi_Count == 3 ||
				Strike_Zhongzi_Count == 6)
			{
				LOG_C_D( L"走3点");
				CPersonFunction::GetInstance().MoveToPoint(HeiBossPoint3, 3.0f, 150.0f);
				Sleep(8000);
				LOG_C_D( L"5902709等待8秒");
				LOG_C_D( L"走2点");
				CPersonFunction::GetInstance().MoveToPoint(HeiBossPoint2, 3.0f, 150.0f);
				LOG_C_D( L"5902709等待9秒");
			}
		}
		CSomeFun::GetInstance().SetSpeed(3.0f);*/

		break;
	case 5902712:
		if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_V))
		{
			CSomeFun::GetInstance().SimlationKey('V', 10);
		}
		else
		{
			CSomeFun::GetInstance().SimlationKey('X', 10);
		}
		/*if (Strike_Zhongzi_Count > 6)
		{
			Strike_Zhongzi_Count = 1;
		}
		LOG_C_D( L"跳种子控 - 使用技能E------ - 当前种子第%d此释放", Strike_Zhongzi_Count);
		
		CFarm::GetInstance().CallBackCat(TRUE);
		CSomeFun::GetInstance().SetSpeed(5.0f);
		if (Strike_Zhongzi_Count == 1 ||
			Strike_Zhongzi_Count == 4)
		{
			CPersonFunction::GetInstance().MoveToPoint(HeiBossPoint4, 3.0f, 150.0f);
		}
		else if (Strike_Zhongzi_Count == 2 ||
			Strike_Zhongzi_Count == 5)
		{
			CPersonFunction::GetInstance().MoveToPoint(HeiBossPoint5, 3.0f, 150.0f);
		}
		else if (Strike_Zhongzi_Count == 3 ||
			Strike_Zhongzi_Count == 6)
		{
			CPersonFunction::GetInstance().MoveToPoint(HeiBossPoint6, 3.0f, 150.0f);
		}
		CSomeFun::GetInstance().SetSpeed(3.0f);
		CFarm::GetInstance().CallBackCat(FALSE);
		CSomeFun::GetInstance().SimlationKey('X', 10);
		if (Strike_Zhongzi_Count == 2)
		{
			Sleep(5000);
			CSomeFun::GetInstance().SimlationKey(VK_4, 10);
			Sleep(4000);
		}
		else if (Strike_Zhongzi_Count == 4)
		{
			Sleep(5000);
			CSomeFun::GetInstance().SimlationKey(VK_4, 10);
			Sleep(4000);
		}
		else if (Strike_Zhongzi_Count == 6)
		{
			Sleep(5000);
			CSomeFun::GetInstance().SimlationKey(VK_4, 10);
			Sleep(4000);
		}
		else
		{
			Sleep(5000);
			CSomeFun::GetInstance().SimlationKey(VK_4, 10);
			Sleep(4000);
		}*/
		Strike_Zhongzi_Count++;

		break;
	case 5902723:
		Sleep(2000);
		CSomeFun::GetInstance().SimlationKey(VK_4, 10);
		Sleep(2000);
		break;
	case 5902715:
		LOG_C_D( L"跳中间，不管继续打BOSS");
		break;
	case 5902707:
		FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_R);
		Sleep(500);
		CSomeFun::GetInstance().SimlationKey(VK_4, 10);
		Sleep(2000);
		break;
	case 5902710:
		LOG_C_D( L"黑森林5902710,不管");
		break;
	case 5902718:
		LOG_C_D( L"黑森林5902718,不管");
		break;
	case 5902722:
		LOG_C_D( L"黑森林5902722,不管");
		break;
	case 5902724:
		LOG_C_D( L"狂暴了，肯定要死了。");
		break;
	case 5902725:
		LOG_C_D( L"黑森林5902725,不管");
		break;
	case 5902768:
		LOG_C_D( L"黑森林5902768,不管");
		break;
	case 5902770:
		LOG_C_D( L"黑森林5902770,不管");
		break;
		//黑森林-Boss 召唤师-结束

		//雪域宫-老一-金童-开始
	case 5902102:
		if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_C))
		{
			FarmSkill.UseSkill_By_SkillId(Sum_SkillId_C,em_Skill_Type_Cat);
		}
		else
		{
			LOG_C_D( L"金童走后");
			MoveToPoint(Monster.GetBehindPoint(3.0f), 3.0f);
			Monster.SetAngle();
			if (CTeamMemberExtend::GetInstance().GetTeamMember_By_LessPercentHp(80, nullptr))
			{
				LOG_C_D( L"走45,有队员70以下血了，加血。");
				if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Z))
				{
					CSomeFun::GetInstance().SimlationKey('Z', 10);
				}
			}
		}
		break;

	case 5902114: case 5902103:case 5902104:case 5902110:case 5902111:case 5902116:case 5902119:
		LOG_C_D( L"金童走后");
		MoveToPoint(Monster.GetBehindPoint(3.0f), 3.0f);
		Monster.SetAngle();
		if (CTeamMemberExtend::GetInstance().GetTeamMember_By_LessPercentHp(80, nullptr))
		{
			LOG_C_D( L"走45,有队员70以下血了，加血。");
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Z))
			{
				CSomeFun::GetInstance().SimlationKey('Z', 10);
			}
		}
		break;
	case 	5902106:
		if (Guimenguan_jiaxue % 1 == 0)
		{
			LOG_CF_D( L"金童走后");
			MoveToPoint(Monster.GetBehindPoint(3.0f), 3.0f);
			Monster.SetAngle();
		}
		else if (Guimenguan_jiaxue % 2 == 0)
		{
			LOG_CF_D( L"金童走左");
			MoveToPoint(Monster.GetLeftPoint(3.0f), 3.0f);
			Monster.SetAngle();
		}
		else if (Guimenguan_jiaxue % 3 == 0)
		{
			LOG_CF_D( L"金童走后");
			MoveToPoint(Monster.GetBehindPoint(3.0f), 3.0f);
			Monster.SetAngle();
		}
		else
		{
			LOG_CF_D( L"金童走右");
			MoveToPoint(Monster.GetRightPoint(3.0f), 3.0f);
			Monster.SetAngle();
		}

		if (CTeamMemberExtend::GetInstance().GetTeamMember_By_LessPercentHp(80, nullptr))
		{
			LOG_CF_D( L"金童走45,有队员80以下血了，加血。");
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Z))
			{
				CSomeFun::GetInstance().SimlationKey('Z', 10);
			}
		}
		Guimenguan_jiaxue++;
		break;
		//雪域宫-老一-金童-结束

		//雪域宫-老一-银童-开始
	case 	5902112:
		if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_C))
		{
			CSomeFun::GetInstance().SimlationKey('C', 10);
		}
		else
		{
			LOG_C_D( L"银童走后");
			MoveToPoint(Monster.GetBehindPoint(3.0f), 3.0f);
			Monster.SetAngle();
			if (CTeamMemberExtend::GetInstance().GetTeamMember_By_LessPercentHp(80, nullptr))
			{
				LOG_C_D( L"银童走45,有队员70以下血了，加血。");
				if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Z))
				{
					CSomeFun::GetInstance().SimlationKey('Z', 10);
				}
			}
		}
		
		break;
	case 5902107:
		if (Guimenguan_jiaxue % 1 == 0)
		{
			LOG_CF_D( L"银童走后");
			MoveToPoint(Monster.GetBehindPoint(3.0f), 3.0f);
			Monster.SetAngle();
		}
		else if (Guimenguan_jiaxue % 2 == 0)
		{
			LOG_CF_D( L"银童走左");
			MoveToPoint(Monster.GetLeftPoint(3.0f), 3.0f);
			Monster.SetAngle();
		}
		else if (Guimenguan_jiaxue % 3 == 0)
		{
			LOG_CF_D( L"银童走后");
			MoveToPoint(Monster.GetBehindPoint(3.0f), 3.0f);
			Monster.SetAngle();
		}
		else
		{
			LOG_CF_D( L"银童走右");
			MoveToPoint(Monster.GetRightPoint(3.0f), 3.0f);
			Monster.SetAngle();
		}

		if (CTeamMemberExtend::GetInstance().GetTeamMember_By_LessPercentHp(80, nullptr))
		{
			LOG_CF_D( L"银童走45,有队员70以下血了，加血。");
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Z))
			{
				CSomeFun::GetInstance().SimlationKey('Z', 10);
			}
		}
		Xueyugong_One++;
		break;
	case 5902113:case 5902117:case 5902120:case 5902121:case 5902122:case 5902123:
		LOG_C_D( L"银童走后");
		MoveToPoint(Monster.GetBehindPoint(3.0f), 3.0f);
		Monster.SetAngle();
		if (CTeamMemberExtend::GetInstance().GetTeamMember_By_LessPercentHp(80, nullptr))
		{
			LOG_C_D( L"银童走45,有队员70以下血了，加血。");
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Z))
			{
				CSomeFun::GetInstance().SimlationKey('Z', 10);
			}
		}
		break;
		//雪域宫-老一-银童-结束

		//雪域宫-老二-伊鲁卡-开始
	case 5902153:case 5902155:
		LOG_C_D( L"伊鲁卡走后");
		MoveToPoint(Monster.GetBehindPoint(3.0f), 3.0f);
		Monster.SetAngle();
		if (CTeamMemberExtend::GetInstance().GetTeamMember_By_LessPercentHp(70, nullptr))
		{
			LOG_C_D( L"伊鲁卡走后,有队员70以下血了，加血。");
			CSomeFun::GetInstance().SimlationKey('Z', 10);
		}
		break;
	case 5902150:case 5902151:
		if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_C))
		{
			if (Yiluka_Control == 1)
			{
				if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 0 || CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 1)
				{
					CFarmSkill::GetInstance().UseSkill_By_SkillId(Sum_SkillId_C, em_Skill_Type::em_Skill_Type_Cat);
				}
			}
			else if (Yiluka_Control == 2)
			{
				if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 2 || CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 3)
				{
					CFarmSkill::GetInstance().UseSkill_By_SkillId(Sum_SkillId_C, em_Skill_Type::em_Skill_Type_Cat);
				}
			}
			else
			{
				if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 4 || CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 5)
				{
					CFarmSkill::GetInstance().UseSkill_By_SkillId(Sum_SkillId_C, em_Skill_Type::em_Skill_Type_Cat);
				}
			}

		}
		Yiluka_Control++;
		if (Yiluka_Control > 3)
		{
			Yiluka_Control = 1;
		}
		break;
	case 5902152:case 5902156:
		LOG_C_D( L"伊鲁卡SS");
		Sleep(500);
		CSomeFun::GetInstance().SimlationKey(VK_4, 10);
		Sleep(2000);
		break;
		/*
		if (Yiluka_Sum % 3 ==0)
		{
			LOG_C_D( L"召唤小怪了，去找小怪。");
			FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_R);
			Sleep(500);
			FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_R);
			Sleep(500);
			FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_R);
			Sleep(500);
			FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_R);
			Sleep(500);
			InterruptYLKShadowSkill(Monster);
		}
		else
		{
			LOG_C_D( L"还没召唤小怪走后。");
			BnsMove(Monster.GetBehindPoint(3.0f), 3.0f);
			Monster.SetAngle();
			if (CTeamMemberExtend::GetInstance().GetTeamMember_By_LessPercentHp(70, nullptr))
			{
				LOG_C_D( L"伊鲁卡走后,有队员70以下血了，加血。");
				CSomeFun::GetInstance().SimlationKey('Z', 10);
				CSomeFun::GetInstance().SimlationKey('Z', 10);
			}
		}
		Yiluka_Sum++;
		if (Yiluka_Sum > 3)
		{
			Yiluka_Sum = 1;
		}
		break;*/
		//KillShieldSolider(); //打盾兵
		//ReboundYLKSkill();//按1
	case 5902157:case 5902158:case 5902159:case 5902161: case 5902162:
		//ReboundYLKSkill(Monster, FALSE);
		LOG_C_D( L"准备放大招了.....队长去捡盾牌");
		KillShieldSolider(Monster);
		CSomeFun::GetInstance().TimeOut_Condiction_GameRun(30 * 1000, [this, Monster]
		{
			ReboundYLKSkill(Monster, FALSE);
			return GetMonsterSkillId() != 5902160 && !EMPTY_PERSONHP;
		});
		LOG_C_D( L"BOSS放元气弹了,丢盾牌!");
		ReboundYLKSkill(Monster, TRUE);
		CFarmSkill::GetInstance().UseULT();
		MoveToPoint(Monster.GetFrontPoint(3));
		break;
	case 5902160:
		LOG_C_D( L"该丢盾牌了");
		//ReboundYLKSkill(Monster, TRUE);
		Monster.SetAngle();
		CSomeFun::GetInstance().SimlationKey('R');
		CSomeFun::GetInstance().SimlationKey('R');
		if (newcat.IsDead())
		{
			CSomeFun::GetInstance().SimlationKey(VK_TAB);
			Sleep(5500);
		}
		break;
		//雪域宫-老二-伊鲁卡-结束

		//雪域宫-老四-修罗王-开始
	case 5902201:case 5902202:case 5902203:case 5902210:
		LOG_CF_D( L"准备控制...");
		MoveSixHorn(Monster, 12);
		if (IsEnableControlBoss(Target2_StatusPanel))
		{
			if (Guimenguan_boss_Count == 1)
			{
				if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 0)
				{
					if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab))
					{
						CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
					}
				}
			}
			else if (Guimenguan_boss_Count == 2)
			{
				if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 1)
				{
					if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab))
					{
						CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
					}
				}
			}
			else if (Guimenguan_boss_Count == 3)
			{
				if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 2)
				{
					if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab))
					{
						CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
					}
				}
			}
			else if (Guimenguan_boss_Count == 4)
			{
				if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 3)
				{
					if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab))
					{
						CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
					}
				}
			}
			else if (Guimenguan_boss_Count == 5)
			{
				if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 4)
				{
					if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab))
					{
						CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
					}
				}
			}
			else if (Guimenguan_boss_Count == 6)
			{
				if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 5)
				{
					if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab))
					{
						CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
					}
				}
			}
			Guimenguan_boss_Count++;
			if (Guimenguan_boss_Count > 6)
			{
				Guimenguan_boss_Count = 1;
			}
		}
		break;
	case 5902204:case 5902206:case 5902207:
	case 5902223:case 5902229:case 5902205:case 5902234:case 5902214:
		LOG_CF_D( L"分6角走开");
		MoveSixHorn(Monster, 12);
		Monster.SetAngle();
		if (CTeamMemberExtend::GetInstance().GetTeamMember_By_LessPercentHp(80, nullptr))
		{
			LOG_CF_D( L"修罗王走后,有队员70以下血了，加血。");
			CSomeFun::GetInstance().SimlationKey('Z', 10);
		}
		break;
	case 5902208:
		LOG_CF_D( L"修罗王跳中间，不管");
		CPersonFunction::GetInstance().MoveToPoint(Point(-54020, 36372, 13042), 15.0f, 50);
		Skill_Flag = TRUE;
		break;
	case 5902232:case 5902213:
		Step_Flag = FALSE;
		Skill_Flag = FALSE;
		CPersonFunction::GetInstance().MoveToPoint(Point(-54020, 36372, 13042), 1.0f, 50);
		CSomeFun::GetInstance().SimlationKey('E', 10);
		CSomeFun::GetInstance().SimlationKey('E', 10);
		Sleep(1000);

		if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 0 ||
			CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 1)
		{
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(3500, [Monster, this]
			{
				Monster.SetAngle();
				CSomeFun::GetInstance().SimlationKey('W');
				Sleep(100);
				CSomeFun::GetInstance().SimlationKey('S');
				Sleep(100);
				CSomeFun::GetInstance().SimlationKey(VK_SPACE);
				Sleep(100);
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Sum_SkillId_3, TRUE);
				CSomeFun::GetInstance().SimlationKey(VK_3);
				return CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_3);
			});
		}
		else
			Sleep(3500);

		break;
	case 5902235:case 5902236:case 5902237:
		LOG_CF_D( L"地上的爆炸技能");
		MoveSixHorn(Monster, 12);
		if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_X))
		{
			CSomeFun::GetInstance().SimlationKey('X', 10);
		}
		break;
	case 5902221:
		LOG_CF_D( L"开隐身去挡球");
		Step_Flag = FALSE;
		Skill_Flag = FALSE;
		
		if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 0 ||
			CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 1)
		{
			if (TmpBossSkillId == 5902232)
			{
				//CFarmSkill::GetInstance().UseSkill_By_SkillId(Sum_SkillId_X);
				CPersonFunction::GetInstance().MoveToPoint(Monster.GetLeftPoint(6.0f), 1.0f, 50);
				Monster.SetAngle();
				Sleep(100);
				CSomeFun::GetInstance().TimeOut_Condiction_GameRun(1000, [Monster, this]
				{
					Monster.SetAngle();
					Sleep(100);
					CSomeFun::GetInstance().SimlationKey('R');
					CSomeFun::GetInstance().SimlationKey('T');
					return CSomeFun::GetInstance().SimlationKey('R');
				});
				
				CPersonFunction::GetInstance().MoveToPoint(Point(-55115, 35592, 13044), 3.0f, 50);
				CSomeFun::GetInstance().SimlationKey('E', 10);
				CSomeFun::GetInstance().SimlationKey('E', 10);
				if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Z))
				{
					CSomeFun::GetInstance().SimlationKey('Z', 10);
				}
				Sleep(1500);
				CPersonFunction::GetInstance().MoveToPoint(Point(-55116, 35086, 13044), 3.0f, 50);
				Sleep(1500);
				CPersonFunction::GetInstance().MoveToPoint(Point(-55116, 34465, 13044), 3.0f, 50);
				Sleep(1500);
				CPersonFunction::GetInstance().MoveToPoint(Point(-54671, 34427, 13044), 3.0f, 50);
				Sleep(1000);
				CPersonFunction::GetInstance().MoveToPoint(Monster.GetLeftPoint(6.0f), 3.0f, 50);
			}
			else
			{
				CPersonFunction::GetInstance().MoveToPoint(Point(-54020, 36372, 13042), 1.0f, 50);
				Monster.SetAngle();
				Sleep(100);
				CSomeFun::GetInstance().SimlationKey('E', 10);
				CSomeFun::GetInstance().SimlationKey('E', 10);
				Monster.SetAngle();
				Sleep(1000);
				CSomeFun::GetInstance().SimlationKey(VK_4, 10);
				CSomeFun::GetInstance().SimlationKey('W', 10);
				Sleep(2000);
				CSomeFun::GetInstance().SimlationKey('S', 10);
				CSomeFun::GetInstance().TimeOut_Condiction_GameRun(3500, [Monster, this]
				{

					Monster.SetAngle();
					Sleep(100);
					//CSomeFun::GetInstance().SimlationKey('W', 10);
					//Sleep(100);
					//CSomeFun::GetInstance().SimlationKey('S', 10);
					//Sleep(100);
					CSomeFun::GetInstance().SimlationKey(VK_SPACE);
					Sleep(100);
					CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Sum_SkillId_3, TRUE);
					CSomeFun::GetInstance().SimlationKey(VK_3);
					return CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_3);
				});
			}

		}
		else if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 2 ||
			CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 3)
		{
			Monster.SetAngle();
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Z))
			{
				CSomeFun::GetInstance().SimlationKey('Z', 10);
			}
			if (TmpBossSkillId == 5902237)
			{
				CPersonFunction::GetInstance().MoveToPoint(Point(-54020, 36372, 13042), 3.0f, 50);
				Monster.SetAngle();
				Sleep(100);
				CSomeFun::GetInstance().SimlationKey('E', 10);
				CSomeFun::GetInstance().SimlationKey('E', 10);
				Sleep(3000);
				CSomeFun::GetInstance().SimlationKey(VK_4, 10);
				CSomeFun::GetInstance().SimlationKey('W', 10);
				Sleep(2000);
				CSomeFun::GetInstance().SimlationKey('S', 10);
				
			}
			else
			{
				Sleep(4000);
				CSomeFun::GetInstance().SimlationKey(VK_4, 10);
				CSomeFun::GetInstance().SimlationKey('W', 10);
				Sleep(2000);
				CSomeFun::GetInstance().SimlationKey('S', 10);
				if (CMonsterExtend::GetInstance().GetAroundMonsterListByName(Monster_生命之花, CBaseObject::CreateFixPointInfo(100), VecList) != 0)
				{
					LOG_CF_D( L"221有花先去采花");
					StepOnFlowerForXiuLuoWang();
				}
				else
				{
					CPersonFunction::GetInstance().MoveToPoint(Point(-52907, 36624, 13044), 3.0f, 50);
					CSomeFun::GetInstance().SimlationKey('E', 10);
					CSomeFun::GetInstance().SimlationKey('E', 10);
					if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Z))
					{
						CSomeFun::GetInstance().SimlationKey('Z', 10);
					}
					Sleep(1000);
					CPersonFunction::GetInstance().MoveToPoint(Point(-52907, 36180, 13044), 3.0f, 50);
					Sleep(1500);
					/*CPersonFunction::GetInstance().MoveToPoint(Point(-52907, 35378, 13045), 3.0f, 50);
					Sleep(1500);*/
					CPersonFunction::GetInstance().MoveToPoint(Point(-52907, 35621, 13044), 3.0f, 50);
					Sleep(1500);
					CPersonFunction::GetInstance().MoveToPoint(Point(-52907, 35099, 13044), 3.0f, 50);
					Sleep(1000);
					CPersonFunction::GetInstance().MoveToPoint(Monster.GetBehindPoint(6.0f), 3.0f, 50);
				}
			}
		}
		else if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 4 ||
			CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 5)
		{
			Monster.SetAngle();
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Z))
			{
				CSomeFun::GetInstance().SimlationKey('Z', 10);
			}
			if (TmpBossSkillId == 5902237)
			{
				CPersonFunction::GetInstance().MoveToPoint(Point(-54020, 36372, 13042), 3.0f, 50);
				Monster.SetAngle();
				Sleep(100);
				CSomeFun::GetInstance().SimlationKey('E', 10);
				CSomeFun::GetInstance().SimlationKey('E', 10);
				Sleep(4000);
				XiuLuoWang_BlockBall();
				/*CSomeFun::GetInstance().SimlationKey(VK_4, 10);
				CSomeFun::GetInstance().SimlationKey('W', 10);
				Sleep(2000);
				CSomeFun::GetInstance().SimlationKey('S', 10);*/
			}
			else
			{
				Sleep(5000);
				XiuLuoWang_BlockBall();
				/*CSomeFun::GetInstance().SimlationKey(VK_4, 10);
				CSomeFun::GetInstance().SimlationKey('W', 10);
				Sleep(2000);
				CSomeFun::GetInstance().SimlationKey('S', 10);*/
				if (CMonsterExtend::GetInstance().GetAroundMonsterListByName(Monster_生命之花, CBaseObject::CreateFixPointInfo(100), VecList) != 0)
				{
					LOG_CF_D( L"221有花先去采花");
					StepOnFlowerForXiuLuoWang();
				}
				else
				{
					CPersonFunction::GetInstance().MoveToPoint(Point(-52907, 36624, 13044), 3.0f, 50);
					CSomeFun::GetInstance().SimlationKey('E', 10);
					CSomeFun::GetInstance().SimlationKey('E', 10);
					if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Z))
					{
						CSomeFun::GetInstance().SimlationKey('Z', 10);
					}
					Sleep(1000);
					CPersonFunction::GetInstance().MoveToPoint(Point(-52907, 36180, 13044), 3.0f, 50);
					Sleep(1500);
					/*CPersonFunction::GetInstance().MoveToPoint(Point(-52907, 35378, 13045), 3.0f, 50);
					Sleep(1500);*/
					CPersonFunction::GetInstance().MoveToPoint(Point(-52907, 35621, 13044), 3.0f, 50);
					Sleep(1500);
					CPersonFunction::GetInstance().MoveToPoint(Point(-52907, 35099, 13044), 3.0f, 50);
					Sleep(1000);
					CPersonFunction::GetInstance().MoveToPoint(Monster.GetBehindPoint(6.0f), 3.0f, 50);
				}
			}
		}
		//LOG_CF_D( L"放大招了，快跑,当前仇恨是%d", CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex());
		Skill_Flag = TRUE;
		break;
	case 5902212:case 5902215:case 5902216:case 5902217:case 5902218:case 5902219:case 5902211:
		LOG_CF_D( L"BOSS持续性技能");
		break;
	case 5902209:
		//3849BC3 花BUFF
		LOG_CF_D( L"修罗王放花，去踩花");
		break;
	case 5902230:
		CSomeFun::GetInstance().TimeOut_Condiction_GameRun(3500, [Monster, this]
		{
			Monster.SetAngle();
			Sleep(100);
			CSomeFun::GetInstance().SimlationKey('T');
			CSomeFun::GetInstance().SimlationKey('R');
			return CSomeFun::GetInstance().SimlationKey('R');
		});
		FarmSkill.UseSkill_By_SS();
		Sleep(1000);
		break;
	case 5902225:
		LOG_CF_D( L"冰吸--------------");
		Sleep(1200);
		FarmSkill.UseSkill_By_SS();
		Sleep(1000);
		Step_Flag = TRUE;
		Skill_Flag = TRUE;
		break;
	case 5902224:
		LOG_CF_D( L"火吸--------------");
		Sleep(1000);
		FarmSkill.UseSkill_By_SS();
		Sleep(1000);
		Step_Flag = TRUE;
		Skill_Flag = TRUE;
		break;
	case 5902226:
		LOG_CF_D( L"什么技能看看--------------");
		break;
		//雪域宫-老四-修罗王-结束


		//地下监狱-老一布卡-开始
	case 5903116:case 5903117:case 5903120:case 5903122:case 5903123:case 5903124:case 5903125:
		/*LOG_C_D( L"老一布卡走后");
		BnsMove(Monster.GetBehindPoint(3.0f), 3.0f);
		Monster.SetAngle();*/
		LOG_C_D( L"老一布卡普通技能.");
		if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Z))
		{
			CSomeFun::GetInstance().SimlationKey('Z', 10);
		}
		break;
	case 5903259:
		if (FarmSkill.IsCool_By_SkillId_In_HideUi(Sum_SkillId_SS))
		{
			FarmSkill.UseSkill_By_SS();
			MoveToPoint(Monster.GetLeftPoint(5.0f), 15.0f);
			/*CSomeFun::GetInstance().TimeOut_Condiction_GameRun(3000, [Monster,this]
			{
			Monster.SetAngle();
			Sleep(100);
			CSomeFun::GetInstance().SimlationKey('T');
			CSomeFun::GetInstance().SimlationKey('R');
			if (GetMonsterSkillId() != 5903259)
			{
			return FALSE;
			}
			return CSomeFun::GetInstance().SimlationKey('R');
			});*/
		}
		break;
	case 5903119:case 5903258:
		if (FarmSkill.IsCool_By_SkillId_In_HideUi(Sum_SkillId_SS))
		{
			FarmSkill.UseSkill_By_SS();
			Sleep(500);

		}
		else if(FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_X))
		{
			CSomeFun::GetInstance().SimlationKey('X', 10);
		}
		break;
	case 5903118:
		LOG_C_D( L"上一个技能是%d",TmpBossSkillId);
		if (TmpBossSkillId == 5903258)
		{
			LOG_C_D( L"执行虚弱");

			CFarmSkill::GetInstance().UseSkill_By_SkillId(Sum_SkillId_C, em_Skill_Type::em_Skill_Type_Cat);
		}
		else if (TmpBossSkillId == 5903259)
		{
			LOG_C_D( L"执行击倒");
			CFarmSkill::GetInstance().UseSkill_By_SkillId(Sum_SkillId_V, em_Skill_Type::em_Skill_Type_Cat);
			CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100.0f));
			Monster.SetAngle();
			Sleep(100);
			CSomeFun::GetInstance().SimlationKey(VK_2, 10);
		}
		else 
		{
			LOG_C_D( L"执行眩晕");
			CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
		}
		break;
		//地下监狱-老一布卡-结束

		//地下监狱-老二-开始
	case 5903131:
		LOG_C_D( L"去杀小怪");
		//Heilongjiao_Huafen = TRUE;
		if (!Heilongjiao_Huafen)
		{
			if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 0 ||
				CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 1 ||
				CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 2)
			{
				MoveBeindAndKillMonster(Monster, L"마충", _BossRange);
			}
			else if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 3 ||
				CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 4 ||
				CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 5)
			{
				MoveBeindAndKillMonster(Monster, L"골성", _BossRange);
			}
		}
		else
		{
			if (Monster.GetMaxHp() != 13000000)
			{
				if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 0 ||
					CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 1 ||
					CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 2)
				{
					MoveBeindAndKillMonster(Monster, L"마충", _BossRange);
				}
				else if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 3 ||
					CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 4 ||
					CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 5)
				{
					MoveBeindAndKillMonster(Monster, L"골성", _BossRange);
				}
			}
			else
			{
				MoveBeindAndKillMonster(Monster, L"마충", _BossRange);
			}
		}
		
		break;
	case 5902654:case 5903109:
		//向前冲的
		LOG_C_D( L"走进，放4");
		CFarmBossSkill::GetInstance().SetPower(TRUE);
		CSomeFun::GetInstance().SetSpeed(5.0f);

		if (Monster.GetTargetId() == CPerson::GetInstance().GetId())
		{
			if (Heilongjiao_Buff == 1)
			{
				Sleep(300);
				CSomeFun::GetInstance().SimlationKey(VK_4, 10);
				Sleep(2000);
			}
			else
			{
				CSomeFun::GetInstance().SimlationKey(VK_4, 10);
				Sleep(2000);
			}
		}
		else
		{
			if (Heilongjiao_Buff == 1)
			{
				Sleep(300);
				CSomeFun::GetInstance().SimlationKey(VK_4, 10);
				Sleep(200);
				//MoveToPoint(Monster.GetFacePoint(4.0f), 15.0f);
				Sleep(1800);
			}
			else
			{
				CSomeFun::GetInstance().SimlationKey(VK_4, 10);
				Sleep(200);
				//MoveToPoint(Monster.GetFacePoint(4.0f), 15.0f);
				Sleep(1800);
			}
		}
		CSomeFun::GetInstance().SetSpeed(3.0f);
		Heilongjiao_Buff++;
		if (Heilongjiao_Buff > 3)
		{
			Heilongjiao_Buff = 1;
		}
		CFarmBossSkill::GetInstance().SetPower(FALSE);
		break;
		//650喷
	case 5903101:case 5902650:case 5902652:case 5903103:case 5903268:case 5903267:
		LOG_C_D( L"普通技能,不管.");
		MoveSixHorn(Monster, 10);
		break;
	case 5902653:
		CSomeFun::GetInstance().SimlationKey('X', 10);
		CSomeFun::GetInstance().SimlationKey('Z', 10);
		break;
	case 5903104:
		
		LOG_C_D( L"当前104里%d", Heilongjiao_Control_2_2);
		if (TmpBossSkillId == 5902654 ||
			TmpBossSkillId == 5903109)
		{
			//Heilongjiao_Huafen = FALSE;
			if (Heilongjiao_Control_2_2 == 3)
			{
				LOG_C_D( L"上个技能是654或109,去档浊气珠");
				AvoidMoGuKeZhuoQiZhu(Monster);
			}
			else
			{
				Sleep(600);
				FarmSkill.UseSkill_By_SS();
			}
			Heilongjiao_Control_2_2++;
		}
		else
		{
			LOG_C_D( L"只SS");
			Sleep(1000);
			FarmSkill.UseSkill_By_SS();
		}
		if (Heilongjiao_Control_2_2 > 3)
		{
			Heilongjiao_Control_2_2 = 1;
		}
		break;
	case 5903108:
		LOG_C_D( L"老二108控制技能%d", Heilongjiao_Control_2_1);
		if (Heilongjiao_Control_2_1 == 1)
		{
			if (CTeamMemberExtend::GetInstance().GetCurrentTeamIndexByClasses(em_Player_Classes_Summoner) == 0)
			{
				CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
			}
			CFarmBossSkill::GetInstance().Move(Monster.GetFacePoint(7.0f), 15.0f);
		}
		else if (Heilongjiao_Control_2_1 == 2)
		{
			if (CTeamMemberExtend::GetInstance().GetCurrentTeamIndexByClasses(em_Player_Classes_Summoner) == 1)
			{
				CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
			}
			CFarmBossSkill::GetInstance().Move(Monster.GetFacePoint(7.0f), 15.0f);
		}
		else if (Heilongjiao_Control_2_1 == 3)
		{
			if (CTeamMemberExtend::GetInstance().GetCurrentTeamIndexByClasses(em_Player_Classes_Summoner) == 2)
			{
				CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
			}
			CFarmBossSkill::GetInstance().Move(Monster.GetFacePoint(7.0f), 15.0f);
		}

		Heilongjiao_Control_2_1++;
		if (Heilongjiao_Control_2_1 > 3)
		{
			Heilongjiao_Control_2_1 = 1;
		}
		break;
	case 5903130:case 5903100:
		LOG_C_D( L"老二控制技能%d", Heilongjiao_Control_1);
		if (Heilongjiao_Control_1 == 1)
		{
			if (CTeamMemberExtend::GetInstance().GetCurrentTeamIndexByClasses(em_Player_Classes_Summoner) == 0 ||
				CTeamMemberExtend::GetInstance().GetCurrentTeamIndexByClasses(em_Player_Classes_Summoner) == 1)
			{
				CSomeFun::GetInstance().SimlationKey('C', 10);
			}
		}
		else if (Heilongjiao_Control_1 == 2)
		{
			if (CTeamMemberExtend::GetInstance().GetCurrentTeamIndexByClasses(em_Player_Classes_Summoner) == 2 ||
				CTeamMemberExtend::GetInstance().GetCurrentTeamIndexByClasses(em_Player_Classes_Summoner) == 3)
			{
				CSomeFun::GetInstance().SimlationKey('C', 10);
			}
		}
		else if (Heilongjiao_Control_1 == 3)
		{
			if (CTeamMemberExtend::GetInstance().GetCurrentTeamIndexByClasses(em_Player_Classes_Summoner) == 0 ||
				CTeamMemberExtend::GetInstance().GetCurrentTeamIndexByClasses(em_Player_Classes_Summoner) == 1)
			{
				CSomeFun::GetInstance().SimlationKey('V', 10);
			}
		}
		else if (Heilongjiao_Control_1 == 4)
		{
			if (CTeamMemberExtend::GetInstance().GetCurrentTeamIndexByClasses(em_Player_Classes_Summoner) == 2 ||
				CTeamMemberExtend::GetInstance().GetCurrentTeamIndexByClasses(em_Player_Classes_Summoner) == 3)
			{
				CSomeFun::GetInstance().SimlationKey('V', 10);
			}
		}

		Heilongjiao_Control_1++;

		if (Heilongjiao_Control_1 > 4)
		{
			Heilongjiao_Control_1 = 1;
		}
		break;
		//地下监狱-老二-结束

		//地下监狱-老三-开始
	case 5903139:
		BnsMove(Monster.GetFacePoint(5.0f), 3.0f);
		break;
	case 5902666:case 5902663:
		if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_X))
		{
			CSomeFun::GetInstance().SimlationKey('X', 10);
		}
		
		BnsMove(Monster.GetFacePoint(5.0f), 3.0f);
		Monster.SetAngle();
		break;
	case 5903273:case 5902662:
		LOG_C_D( L"凝视前置技能");
		CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NMAE_ISPUASEHELPTHREAD, 1);

		break;
	case 5903129:
		//384BDF0 凝视BUFF
		//384BDEF 凝视后给的多加攻击的BUFF
		LOG_C_D( L"准备释放4躲避凝视%d", Heilongjiao_Buff);
		/*BnsMove(Monster.GetFacePoint(3.0f), 3.0f);
		Monster.SetAngle();
		BnsMove(Monster.GetFacePoint(3.0f), 3.0f);
		Monster.SetAngle();*/
		BnsMove(Monster.GetFacePoint(3.0f), 3.0f);

		//if (CPersonFunction::GetInstance().ExistBuff_Person(0x384BDF0, nullptr))
		//{
		//	if (Heilongjiao_Buff == 1)
		//	{
		//		//BnsSleep(3500);
		//		Sleep(2500);
		//		CFarmBossSkill::GetInstance().Move(Monster.GetFacePoint(3.0f), 3.0f);
		//		CFarmBossSkill::GetInstance().SetPower(TRUE);
		//		Sleep(1500);
		//	}
		//	else if (Heilongjiao_Buff == 2)
		//	{
		//		//BnsSleep(3200);
		//		Sleep(2500);
		//		CFarmBossSkill::GetInstance().Move(Monster.GetFacePoint(3.0f), 3.0f);
		//		CFarmBossSkill::GetInstance().SetPower(TRUE);
		//		Sleep(1200);
		//		
		//	}
		//	else
		//	{
		//		//BnsSleep(3400);
		//		Sleep(2500);
		//		CFarmBossSkill::GetInstance().Move(Monster.GetFacePoint(3.0f), 3.0f);
		//		CFarmBossSkill::GetInstance().SetPower(TRUE);
		//		Sleep(1350);
		//	}
		//	LOG_C_D( L"凝视BUFF，继续打%d", CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex());
		//	CSomeFun::GetInstance().SimlationKey(VK_4, 10);
		//	Sleep(200);
		//	FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_4_4);
		//	Sleep(1500);
		//}
		//else
		//{
		//	if (Heilongjiao_Buff == 1)
		//	{
		//		//BnsSleep(3500);
		//		Sleep(2500);
		//		CFarmBossSkill::GetInstance().Move(Monster.GetFacePoint(3.0f), 3.0f);
		//		CFarmBossSkill::GetInstance().SetPower(TRUE);
		//		Sleep(1500);
		//	}
		//	else if (Heilongjiao_Buff == 2)
		//	{
		//		//BnsSleep(3200);
		//		Sleep(2500);
		//		CFarmBossSkill::GetInstance().Move(Monster.GetFacePoint(3.0f), 3.0f);
		//		CFarmBossSkill::GetInstance().SetPower(TRUE);
		//		Sleep(1200);

		//	}
		//	else
		//	{
		//		//BnsSleep(3400);
		//		Sleep(2500);
		//		CFarmBossSkill::GetInstance().Move(Monster.GetFacePoint(3.0f), 3.0f);
		//		CFarmBossSkill::GetInstance().SetPower(TRUE);
		//		Sleep(1350);
		//	}
		//	CSomeFun::GetInstance().SimlationKey(VK_4, 10);
		//	Sleep(200);
		//	FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_4_4);
		//	Sleep(4000);
		//}


			if (Heilongjiao_Buff == 1)
			{
				//BnsSleep(3500);
				Sleep(1000);
				CFarmBossSkill::GetInstance().Move(Monster.GetFacePoint(3.0f), 3.0f);
				CFarmBossSkill::GetInstance().SetPower(TRUE);
				Sleep(3000);
			}
			else if (Heilongjiao_Buff == 2)
			{
				//BnsSleep(3200);
				Sleep(1000);
				CFarmBossSkill::GetInstance().Move(Monster.GetFacePoint(3.0f), 3.0f);
				CFarmBossSkill::GetInstance().SetPower(TRUE);
				Sleep(3000);
				
			}
			else
			{
				//BnsSleep(3400);
				Sleep(1000);
				CFarmBossSkill::GetInstance().Move(Monster.GetFacePoint(3.0f), 3.0f);
				CFarmBossSkill::GetInstance().SetPower(TRUE);
				Sleep(3000);
			}
			//LOG_C_D( L"凝视BUFF，继续打%d", CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex());
			CSomeFun::GetInstance().SimlationKey(VK_4, 10);
			Sleep(200);
			CSomeFun::GetInstance().SimlationKey('F', 10);
			//FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_4_4);
			Sleep(3000);
		if (CPerson::GetInstance().GetAbNormalStatus() == em_WarStatus::em_WarStatus_Hide)
		{
			LOG_C_D( L"凝视BUFF，不打%d", CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex());
			Sleep(3000);
		}
		else
		{
			LOG_C_D( L"凝视BUFF，继续打%d", CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex());
		}

		Heilongjiao_Buff++;
		if (Heilongjiao_Buff > 2)
		{
			Heilongjiao_Buff = 1;
		}
		CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NMAE_ISPUASEHELPTHREAD, 0);
		
		CFarmBossSkill::GetInstance().SetPower(FALSE);
		break;
	case 5903128:case 5903238:case 5902664:
		LOG_C_D( L"不动继续打");
		break;
	case 5903274:
		CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NMAE_ISPUASEHELPTHREAD, 1);
		Sleep(6000);
		CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NMAE_ISPUASEHELPTHREAD, 0);
		break;
	case 5902665:case 5903277:
		
		if (!CPersonFunction::GetInstance().ExistBuff_Person(0x384BDEF, nullptr))
		{
			CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100));
		}
		else
		{
			CFarmBossSkill::GetInstance().MoveToSixAngle(8,1*1000);
			//MoveSixHorn(Monster, 12);
			Monster.SetAngle();
		}
		break;
	case 5903232:case 5903239:case 5903227:
		if (IsEnableControlBoss(Target2_StatusPanel))
		{
			if (Heilongjiao_Control_2_2 == 1)
			{
				if (CTeamMemberExtend::GetInstance().GetCurrentTeamIndexByClasses(em_Player_Classes_Summoner) == 0 ||
					CTeamMemberExtend::GetInstance().GetCurrentTeamIndexByClasses(em_Player_Classes_Summoner) == 1)
				{
					/*if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab))
					{
					CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
					}*/
					LOG_C_D( L"tab-%d", Heilongjiao_Control_2_2);
					CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
				}
			}
			else if (Heilongjiao_Control_2_2 == 2)
			{
				if (CTeamMemberExtend::GetInstance().GetCurrentTeamIndexByClasses(em_Player_Classes_Summoner) == 2 ||
					CTeamMemberExtend::GetInstance().GetCurrentTeamIndexByClasses(em_Player_Classes_Summoner) == 3)
				{
					/*if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab))
					{
					CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
					}*/
					LOG_C_D( L"tab-%d", Heilongjiao_Control_2_2);
					CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
				}

			}
			else if (Heilongjiao_Control_2_2 == 3)
			{
				LOG_C_D( L"v-%d", Heilongjiao_Control_2_2);
				if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_V))
				{
					CSomeFun::GetInstance().SimlationKey('V', 10);
				}
			}
			else if (Heilongjiao_Control_2_2 == 4)
			{
				LOG_C_D( L"c-%d", Heilongjiao_Control_2_2);
				if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_C))
				{
					CSomeFun::GetInstance().SimlationKey('C', 10);
				}
			}
			Heilongjiao_Control_2_2++;

			if (Heilongjiao_Control_2_2 > 4)
			{
				Heilongjiao_Control_2_2 = 1;
			}
		}
		break;
	case 5903261:
	case 5902658:case 5902660:case 5902661:case 5903132:case 5903257:
	case 5903229:case 5903231:case 5903233:case 5903234:case 5903256:
	case 5903263:case 5903264:case 5903228:case 5902659:case 5903262:
		
		CFarmBossSkill::GetInstance().MoveToSixAngle(11,1*1000);
		Monster.SetAngle();
		break;
	case 5903249:

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_X))
		{
			CSomeFun::GetInstance().SimlationKey('X', 10);
		}
		else
		{
		CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2700, [Monster]
		{
			Monster.SetAngle();
			Sleep(100);
			CSomeFun::GetInstance().SimlationKey('T');
			CSomeFun::GetInstance().SimlationKey('R');
			return CSomeFun::GetInstance().SimlationKey('R');
		});
		FarmSkill.UseSkill_By_SS();
		Sleep(1000);
		}
		break;
		//地下监狱-老三-结束
	case 0:
		//CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100.0f));
		bRetCode = FALSE;
		break;
	default:
		LOG_C_D( L"default,当前怪物技能=%d", dwBossSkillId);
		break;
	}
	VMProtectEnd();
	return bRetCode;
}

BOOL CFarmBoss::UseSkill_By_BossSkillId_Warlock(_In_ CMonster& Monster, _In_ DWORD dwBossSkillId, _In_ CONST FixPointInfo& FixPtInfo, _In_ CONST CGameUi& Target2_StatusPanel, _In_ DWORD TmpBossSkillId)
{
	VMProtectBegin("Sum-boss");
	auto& FarmSkill = CFarmSkill::GetInstance();
	auto  NowPoint = Monster.GetPoint();
	CMonster Monster_q;//7-11 黑龙教主
	CCat newcat;
	CCatExpend::GetInstance().FindSelfCat(&newcat);
	std::vector<CMonster> VecList;

	CONST static Point Boss_1_point(-3481, -71798, 152);
	CONST static Point Boss_7_11_point(34324, -86092, 7282);
	CONST static Point Xuerendong_BossPoint(-49415, 24578, 14462);
	CONST static Point Xuerendong_TarPoint(-49872, 24648, 14448);//雪人王BOSS要中间要躲避的坐标
	CONST static Point Xuerendong_TarPoint2(-49752, 24648, 14448);//雪人王BOSS要中间要躲避的坐标
	CONST static Point SnowBossPoint(-49868, 24644, 14448);

	CONST static Point HeiBossPoint1(-3979, -6046, 552);//黑森林BOSS走位1
	CONST static Point HeiBossPoint2(-1446, -5668, 544);//黑森林BOSS走位2
	CONST static Point HeiBossPoint3(-3380, -3587, 576);//黑森林BOSS走位3
	CONST static Point HeiBossPoint4(-3959, -6176, 552);//黑森林BOSS走位4
	CONST static Point HeiBossPoint5(-1443, -5564, 541);//黑森林BOSS走位5
	CONST static Point HeiBossPoint6(-3313, -3586, 575);//黑森林BOSS走位6


	BOOL bRetCode = TRUE;
	switch (dwBossSkillId)
	{

		//地下监狱-老一布卡-咒术师开始
	case 5903116:case 5903117:case 5903120:case 5903122:case 5903123:case 5903124:case 5903125:
		/*LOG_C_D( L"老一布卡走后");
		BnsMove(Monster.GetBehindPoint(3.0f), 3.0f);
		Monster.SetAngle();*/
		LOG_C_D( L"老一布卡普通技能.");
		break;
	case 5903259:
		if (FarmSkill.IsCool_By_SkillId_In_HideUi(Warlock_SkillId_SS))
		{
			FarmSkill.UseSkill_By_SS();
			Sleep(1000);
			BnsMove(Monster.GetLeftPoint(5.0f), 15.0f);
		}
		break;
	case 5903119:case 5903258:
		if (FarmSkill.IsCool_By_SkillId_In_HideUi(Warlock_SkillId_SS))
		{
			FarmSkill.UseSkill_By_SS();
			Sleep(500);

		}
		
		break;
	case 5903118:
		LOG_C_D( L"上一个技能是%d", TmpBossSkillId);
		if (TmpBossSkillId == 5903259)
		{
			LOG_C_D( L"执行击倒");
			//BnsMove(Monster.GetPoint(), 15.0f);
			MoveToPoint(Monster.GetFacePoint(10.0f), 15.0f);
			CSomeFun::GetInstance().SimlationKey('X', 10);
		}
		break;
		//地下监狱-老一布卡-咒术师结束

		//地下监狱-老二-咒术师开始
	case 5903131:
		LOG_C_D( L"咒术师去杀小怪");
		if (!Heilongjiao_Huafen)
		{
			if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 0 ||
				CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 1 ||
				CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 2)
			{
				MoveBeindAndKillMonster(Monster, L"마충", _BossRange);
			}
			else if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 3 ||
				CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 4 ||
				CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 5)
			{
				MoveBeindAndKillMonster(Monster, L"골성", _BossRange);
			}
		}
		else
		{
			if (Monster.GetMaxHp() != 13000000)
			{
				if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 0 ||
					CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 1 ||
					CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 2)
				{
					MoveBeindAndKillMonster(Monster, L"마충", _BossRange);
				}
				else if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 3 ||
					CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 4 ||
					CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 5)
				{
					MoveBeindAndKillMonster(Monster, L"골성", _BossRange);
				}
			}
			else
			{
				MoveBeindAndKillMonster(Monster, L"마충", _BossRange);
			}

		}
		//Heilongjiao_Huafen = TRUE;
		break;
	case 5902654:case 5903109:
		//向前冲的
		LOG_C_D( L"走进，放4");
		CFarmBossSkill::GetInstance().SetPower(TRUE);
		CSomeFun::GetInstance().SetSpeed(5.0f);
		//BnsMove(Monster.GetFacePoint(6.0f), 1.0f);
		Monster.SetAngle();
		Sleep(100);
		CSomeFun::GetInstance().SimlationKey(VK_1, 10);
		Sleep(1800);
		CSomeFun::GetInstance().SetSpeed(3.0f);
		CFarmBossSkill::GetInstance().SetPower(FALSE);
		break;
		//650喷
	case 5903101:case 5902650:case 5902652:case 5903103:case 5903268:case 5903267:
		LOG_C_D( L"普通技能,不管.");
		CFarmBossSkill::GetInstance().MoveToSixAngle(10,1*1000);
		//MoveSixHorn(Monster, 12);
		break;
	case 5902653:
		CSomeFun::GetInstance().SimlationKey('Z', 10);
		break;
	case 5903104:

		LOG_C_D( L"当前104里%d", Heilongjiao_Control_2_2);
		if (TmpBossSkillId == 5902654 ||
			TmpBossSkillId == 5903109)
		{
			Heilongjiao_Huafen = FALSE;
			if (Heilongjiao_Control_2_2 == 3)
			{
				LOG_C_D( L"上个技能是654或109,去档浊气珠");
				AvoidMoGuKeZhuoQiZhu(Monster);
			}
			else
			{
				Sleep(600);
				FarmSkill.UseSkill_By_SS();
			}
			Heilongjiao_Control_2_2++;
		}
		else
		{
			LOG_C_D( L"只SS");
			Sleep(1200);
			FarmSkill.UseSkill_By_SS();
		}
		if (Heilongjiao_Control_2_2 > 3)
		{
			Heilongjiao_Control_2_2 = 1;
		}
		break;
	case 5903108:
		LOG_C_D( L"控制技能，还没想清楚咒术怎么抵挡");
		break;
	case 5903130:case 5903100:
		LOG_C_D( L"控制技能，还没想清楚咒术怎么抵挡");
		break;
		//地下监狱-老二-咒术师结束

		//地下监狱-老三-咒术师开始
	case 5903139:
		BnsMove(Monster.GetFacePoint(2.0f), 2.0f);
		break;
	case 5902666:case 5902663:
		CSomeFun::GetInstance().SimlationKey(VK_SPACE, 10);
		
		BnsMove(Monster.GetFacePoint(2.0f), 2.0f);
		Monster.SetAngle();
		break;
	case 5903273:case 5902662:
		LOG_C_D( L"咒术师凝视前置技能");
		CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NMAE_ISPUASEHELPTHREAD, 1);
		break;
	case 5903129:
		//384BDF0 凝视BUFF
		//384BDEF 凝视后给的多加攻击的BUFF
		LOG_C_D( L"咒术师凝视");
		BnsMove(Monster.GetFacePoint(2.0f),2.0f);
		if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_C))
		{
			CSomeFun::GetInstance().SimlationKey('C', 10);
			Sleep(300);
		}
		Sleep(1000);
		CFarmBossSkill::GetInstance().SetPower(TRUE);
		Sleep(6000);
		if (CPerson::GetInstance().GetAbNormalStatus() == em_WarStatus::em_WarStatus_Hide)
		{
			LOG_C_D( L"咒术师凝视隐身,不打%d", CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex());
			Sleep(3000);
		}
		else
		{
			LOG_C_D( L"咒术师凝视隐身，继续打打%d", CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex());
		}
		CFarmBossSkill::GetInstance().SetPower(FALSE);
		CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NMAE_ISPUASEHELPTHREAD, 0);
		
		break;
	case 5903128:case 5903238:case 5902664:
		LOG_C_D( L"不动继续打");
		break;
	case 5903274:
		CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NMAE_ISPUASEHELPTHREAD, 1);
		Sleep(6000);
		CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NMAE_ISPUASEHELPTHREAD, 0);
		break;
	case 5903277:case 5902665:
		if (!CPersonFunction::GetInstance().ExistBuff_Person(0x384BDEF, nullptr))
		{
			CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100));
		}
		else
		{
			//CFarmBossSkill::GetInstance().MoveToSixAngle(10,1*1000);
			MoveSixHorn(Monster, 8);
			Monster.SetAngle();
		}
		break;
	case 5903232:case 5903227:case 5903239:case 5903261:
	case 5902658:case 5902660:case 5902661:case 5903132:case 5903257:
	case 5903229:case 5903231:case 5903233:case 5903234:case 5903256:
	case 5903263:case 5903264:case 5903228:case 5902659:case 5903262:
			//CFarmBossSkill::GetInstance().MoveToSixAngle(8,1*1000);
			MoveSixHorn(Monster, 8);
			Monster.SetAngle();
		break;
	case 5903249:

	
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2700, [Monster]
			{
				Monster.SetAngle();
				Sleep(100);
				CSomeFun::GetInstance().SimlationKey('T');
				CSomeFun::GetInstance().SimlationKey('R');
				return CSomeFun::GetInstance().SimlationKey('R');
			});
			FarmSkill.UseSkill_By_SS();
			Sleep(1000);
		break;
		//地下监狱-老三-咒术师结束
	case 0:
		//CFarm::GetInstance().Move_To_Monster(Monster, CBaseObject::CreateFixPointInfo(100.0f));
		bRetCode = FALSE;
		break;
	default:
		LOG_C_D( L"default,当前怪物技能=%d", dwBossSkillId);
		break;
	}
	VMProtectEnd();
	return bRetCode;
}

CONST std::map<DWORD, DWORD>& CFarmBoss::GetBossSkillVec() CONST
{
	static CONST std::map<DWORD, DWORD> vlst = {
		// 第一个是SkillId, 第二个是次数, {1,2} 表示技能ID=1, 2表示2次
		{ 5100345, 5 }, { 5902391, 3 }
	};

	return vlst;
}

CONST std::set<DWORD>& CFarmBoss::GetDisableBossSkillSet() CONST
{
	CONST static std::set<DWORD> SetDisableSkillId = 
	{
		5902233, 5902220
	};
	return SetDisableSkillId;
}

BOOL CFarmBoss::GetBossSkillByQue(_Out_ MonsterSkillContent& MonsterSkillContent_)
{
	BOOL bExist = FALSE;
	vBossSkillLock.Access([this, &bExist, &MonsterSkillContent_]
	{
		if (!queBossSkill.empty())
		{
			MonsterSkillContent_ = std::move(queBossSkill.front());
			queBossSkill.pop();
			bExist = TRUE;
		}
	});
	return bExist;
}

VOID CFarmBoss::AddBossSkillByQue(_In_ CONST MonsterSkillContent& MonsterSkillContent_)
{
	vBossSkillLock.Access([MonsterSkillContent_, this]{queBossSkill.push(MonsterSkillContent_); });
}

BOOL& CFarmBoss::IsHookByDlg() CONST
{
	static BOOL bDlg;
	return bDlg;
}

//monster 怪物名 范围
BOOL CFarmBoss::ClearAroundMonster(_In_ CONST CMonster& Boss, _In_ CONST std::wstring& wsMonsterName, _In_ CONST FixPointInfo& FixPtInfo) CONST
{
	CONST std::wstring wsBossName = Boss.GetName();
	CFilter::GetInstance().AddMonsterFilter(wsBossName, CFilter::MonsterFilterType::Filter_Monster_Low);
	CFarm::GetInstance().ClearAround_TarName(wsMonsterName, FixPtInfo);
	CFilter::GetInstance().RemoveMonsterFilter(wsBossName);

	return TRUE;
}

BOOL CFarmBoss::ClearAroundMonster_ts(_In_ CONST CMonster& Boss, _In_ CONST std::wstring& wsMonsterName, _In_ CONST FixPointInfo& FixPtInfo) CONST
{
	CONST std::wstring wsBossName = Boss.GetName();
	CFilter::GetInstance().AddMonsterFilter(wsBossName, CFilter::MonsterFilterType::Filter_Monster_Low);
	CFarm::GetInstance().ClearAround_TarName_ts(wsMonsterName, FixPtInfo);
	CFilter::GetInstance().RemoveMonsterFilter(wsBossName);
	return TRUE;
}

VOID CFarmBoss::RefushGameUi(_In_ CONST std::wstring& wsUiName, _Out_opt_ CGameUi& GameUi) throw()
{
	if (++uLoopCount % 60 == 0)
	{
		CObjectFunction::GetInstance().Action_By_GameUiName(wsUiName, [&GameUi](CONST CGameUi& tmpGameUi){GameUi = tmpGameUi; });
	}
}

BOOL CFarmBoss::IsEnableControlBoss(_In_ CONST CGameUi& Target2_StatusPanel) CONST throw()
{
	//== 1 ? 不可控 : 可控    + 4 == FF ? 不可控 : 可控
	return ReadBYTE(Target2_StatusPanel.GetBase() + BOSS是否可控偏移) == 0x1 ? FALSE : TRUE;
}

DWORD CFarmBoss::GetColtrolBossCount(_In_ CONST CGameUi& Target2_StatusPanel) CONST throw()
{
	return ReadBYTE(Target2_StatusPanel.GetBase() + BOSS被控制BUFF数量偏移);
}

BOOL CFarmBoss::MoveToBoss(_In_ CONST CMonster& Monster) CONST
{
	float fDis = Monster.GetDis();
	if (fDis >= 100)
		return FALSE;
	else if (fDis >= _MinKeepAttackDis && fDis < 15.0f)
		return TRUE;

	//LOG_C_D(L"跟BOSS的距离低于设定的%.2fm", _MinKeepAttackDis);

	std::vector<Point> VecPoint;
	CFarm::GetInstance().GetRoundPointList(FixPointInfo(_BossRange.FixPoint, _KeepAttackDis), 36, VecPoint);

	// 先去掉超出范围的坐标
	MyTools::CLPublic::Vec_erase_if(VecPoint, [Monster](CONST Point& Point_){ return Monster.GetDis(Point_) > 15; });

	// 再去掉离BOSS太近的坐标
	MyTools::CLPublic::Vec_erase_if(VecPoint, [Monster, this](CONST Point& Point_){ return Monster.GetDis(Point_) < _MinKeepAttackDis; });

	for (float i = 1; i <= 15 - _KeepAttackDis; ++i)
	{
		auto p = MyTools::CLPublic::Vec_find_if_Const(VecPoint, [Monster, this, i](CONST Point& Point_){ return fabs(Monster.GetDis(Point_) - _KeepAttackDis) <= i; });
		if (p == nullptr)
			continue;

		LOG_C_D(L"正在跟BOSS保持距离走路! KeepDis=%.2f, MinDis=%.2f", _KeepAttackDis, _MinKeepAttackDis);
		CPersonFunction::GetInstance().Move(*p);
		Monster.SetAngle();
		return TRUE;
	}

	if (VecPoint.size() > 0)
	{
		LOG_C_D(L"只能走第一个点了……");
		CPersonFunction::GetInstance().Move(VecPoint.at(0));
	}

	return TRUE;
}

BOOL CFarmBoss::MoveToPoint(_In_ CONST Point& TargetPoint, _In_ float fDis /*= 3.0f*/) CONST
{
	if (CBaseObject::GetPersonDis(TargetPoint) > 35.0f)
	{
		LOG_C_D( L"终点与当前坐标距离=%.2f, 超过%.2f, 不走!:%d,%d,%d", CBaseObject::GetPersonDis(TargetPoint), 35.0f, TargetPoint.GetX(), TargetPoint.GetY(), TargetPoint.GetZ());
		return TRUE;
	}

	//进入MoveToPoint，先执行一次走路。
	CPersonFunction::GetInstance().Move(TargetPoint);
	return TRUE;
}

VOID CFarmBoss::MoveSixHorn(_In_ CONST CMonster& Boss, _In_ float fDis) CONST
{
	if (CPersonFunction::GetInstance().ExistBuff_Person(0x384BDEF, nullptr))
		fDis += 1.f;

	if (fabs(Boss.GetDis() - fDis) <= 2.0f)
		return;

	std::vector<Point> VecPoint;
	CFarm::GetInstance().GetRoundPointList(FixPointInfo(Boss.GetPoint(), fDis), 6, VecPoint);

	auto Index = CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex();
	if (Index < 0)
		return;

	CONST Point& HornPoint = VecPoint.at(Index);
	if (CPerson::GetInstance().GetDis(HornPoint) > 1)
	{
		CPersonFunction::GetInstance().Move(HornPoint);
	}
}

VOID CFarmBoss::ForceBeckonCat(_In_ CONST CCat& Cat) CONST
{
	while (GameRun && !EMPTY_PERSONHP && Cat.GetHp() == NULL)
	{
		LOG_C_D( L"开始召唤猫!");
		CSomeFun::GetInstance().SimlationKey(VK_TAB);
		CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2 * 1000, [&Cat]
		{
			return Cat.GetHp() == NULL;
		});
	}
}

