#include "stdafx.h"
#include "FarmMonster.h"
#include <MyTools/Log.h>
#include <MyTools/TimeTick.h>
#include <MyTools/CLEchoException.h>
#include <thread>
#include "PersonFunction.h"
#include "FarmSkill.h"
#include "SomeFun.h"
#include "BnsFilter.h"
#include "MonsterExtend.h"
#include "ResAction.h"
#include "Monster.h"
#include "Person.h"
#include "ObjectFunction.h"
#include "GameUI.h"
#include "Cat.h"
#include "CatExpend.h"
#include "Skill.h"
#include "TeamMemberExtend.h"
#include "DailyTask_Skill.h"
#define _SELF L"FarmMonster.cpp"

CFarmMonster::CFarmMonster() : _bMove(TRUE)
{

}

BOOL CFarmMonster::Move_To_Monster(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo ,_In_ BOOL Summoner_flag) CONST
{
	if (!_bMove)
	{
		return TRUE;
	}
	else if (Monster.GetDis(FixPtInfo.FixPoint) > FixPtInfo.fDis)
	{
		LOG_CF_D( L"怪物:%s 超出起始点攻击范围:%.2f, Dis:%.2f", Monster.GetName().c_str(), FixPtInfo.fDis, Monster.GetDis(FixPtInfo.FixPoint));
		return FALSE;
	}

	int nCount = 1;
	float fLastDis = Monster.GetDis();
	DWORD dwClasses = CGameVariable::GetInstance().GetAccShareInfo()->MyAcount_Info.dwClasses;
	float fFaceDis = static_cast<float>(CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_KILLMONSTERR_MOVE_DIS));
	auto TarPoint = Monster.GetFrontPoint(fFaceDis);
	while (GameRun && !EMPTY_PERSONHP && CPerson::GetInstance().GetDis(TarPoint) > fFaceDis && !Monster.IsDead())
	{
		if (CPersonFunction::GetInstance().IsGameLoad())
		{
			CPersonFunction::GetInstance().WaitGameLoad();
			LOG_CF_D( L"走路中出现过图…… 放弃走路!");
			return FALSE;
		}

		Monster.SetAngle();

		if (Monster.GetDis(FixPtInfo.FixPoint) > FixPtInfo.fDis)
		{
			LOG_CF_D( L"怪物:%s 超出起始点攻击范围:%.2f, Dis:%.2f", Monster.GetName().c_str(), FixPtInfo.fDis, Monster.GetDis(FixPtInfo.FixPoint));
			return FALSE;
		}
		
		if (CPerson::GetInstance().GetDis(TarPoint) >= 50.0f)
		{
			LOG_CF_D( L"终点与当前坐标距离=%.2f, 超过%.2f, 不走!", CPerson::GetInstance().GetDis(TarPoint), 50.0f);
			return TRUE;
		}
		//if (nCount == 1)
		//{
			//LOG_CF_D( L"移动到怪物旁, Dis=%.2f", CPerson::GetInstance().GetDis(TarPoint));
		//}

		if (Summoner_flag)
		{
			if (dwClasses == static_cast<DWORD>(em_Player_Classes_Summoner))
			{
				if (Monster.GetId() == CPerson::GetInstance().GetTargetId())
				{
					//LOG_CF_D( L"当前是召唤师,目标ID与怪物ID是一样的，不走。");
					return TRUE;
				}
				else
				{
					if (CPerson::GetInstance().GetDis(TarPoint) < 7.0f)
					{
						//LOG_CF_D( L"当前是召唤师,目标ID与怪物ID不一样,但是距离小于7了不走, Dis=%.2f", CPerson::GetInstance().GetDis(TarPoint));
						return TRUE;
					}
				}
			}
			else if (dwClasses == static_cast<DWORD>(em_Player_Classes_Warlock))
			{
				if (CPerson::GetInstance().GetDis(TarPoint) < 10.0f)
				{
					Monster.SetAngle();
					CSkill::UseSkill_By_Id(Warlock_SkillId_R1_1, FALSE);
					CSomeFun::GetInstance().SimlationKey('S', 20);
					LOG_CF_D( L"当前是咒术师,10以下就可以跳出了。");
					return TRUE;
				}
			}
			else if (dwClasses == static_cast<DWORD>(em_Player_Classes_ForceMaster))
			{
				if (CPerson::GetInstance().GetDis(TarPoint) < 10.0f)
				{
					Monster.SetAngle();
					CSomeFun::GetInstance().SimlationKey('R', 10);
					CSomeFun::GetInstance().SimlationKey('S', 20);
					LOG_CF_D( L"当前是气功,10以下就可以跳出了。");
					return TRUE;
				}
			}
			else if (dwClasses == static_cast<DWORD>(em_Player_Classes_Gunner))
			{
				if (CPerson::GetInstance().GetDis(TarPoint) < 10.0f)
				{
					Monster.SetAngle();
					CSomeFun::GetInstance().SimlationKey('T', 10);
					CSomeFun::GetInstance().SimlationKey('S', 20);
					LOG_CF_D( L"当前是枪手,10以下就可以跳出了。");
					return TRUE;
				}
			}
		}
		
		


		if (nCount ==0 || !CPerson::GetInstance().IsMoving())
		{
			//LOG_CF_D( L"没有在走路状态，执行走路。距离终点:%.2f, fFaceDis=%.2f", CPerson::GetInstance().GetDis(TarPoint), fFaceDis);
			CPersonFunction::GetInstance().Move(TarPoint);
		}

		if (Monster.GetPercentHp() == 100 && dwClasses == static_cast<DWORD>(em_Player_Classes_Destroyer))
		{
			if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_2))
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_2);
			else if (CPerson::GetInstance().GetDis(TarPoint) < 5.0f)
				CSkill::UseSkill_By_Id(Lux_SkillId_R, FALSE);
		}
		else if (Monster.GetPercentHp() == 100 && dwClasses == static_cast<DWORD>(em_Player_Classes_KongFuForceMaster) && CPerson::GetInstance().GetFightPose() == em_Player_Pose_KongFuMaster)
		{
			if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Kffm_Kfm_SkillId_2))
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Kffm_Kfm_SkillId_2, TRUE);
			else if (CPerson::GetInstance().GetDis(TarPoint) < 4.0f)
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Kffm_Kfm_SkillId_R, FALSE);
		}
		else if (Monster.GetPercentHp() == 100 && dwClasses == static_cast<DWORD>(em_Player_Classes_Summoner))
		{
			//if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab))
			//	//CFarmSkill::GetInstance().UseSkill_By_SkillId(Sum_SkillId_Tab, em_Skill_Type_Cat);
			//	CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
			//else if (CPerson::GetInstance().GetDis(TarPoint) < 15.0f)
			CSkill::UseSkill_By_Id(Sum_SkillId_R, FALSE);
		}
		else if (Monster.GetPercentHp() == 100 && dwClasses == static_cast<DWORD>(em_Player_Classes_Warlock))
		{
			//if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab))
			//	//CFarmSkill::GetInstance().UseSkill_By_SkillId(Sum_SkillId_Tab, em_Skill_Type_Cat);
			//	CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
			//else if (CPerson::GetInstance().GetDis(TarPoint) < 15.0f)
			CSkill::UseSkill_By_Id(Sum_SkillId_R, FALSE);
		}
		else if (Monster.GetPercentHp() == 100 && dwClasses == static_cast<DWORD>(em_Player_Classes_Danncer))
		{
			CSomeFun::GetInstance().SimlationKey(VK_1, 0);
		}
		else if (Monster.GetPercentHp() == 100 && dwClasses == static_cast<DWORD>(em_Player_Classes_KongFuMaster))
		{
			CSkill::UseSkill_By_Id(KongFuMaster_SkillId_Tab, FALSE);
		}
		else if (Monster.GetPercentHp() == 100 && dwClasses == static_cast<DWORD>(em_Player_Classes_Gunner))
		{
			CSkill::UseSkill_By_Id(Gunner_SkillId_T, FALSE);
		}
		if (++nCount % 40 == 0)
		{
			float fNowDis = CPerson::GetInstance().GetDis(TarPoint);
			if (fLastDis - fNowDis <= 2.0f) // 6s之内, 走路只走了2m, 就表示不正常
				CFarmSkill::GetInstance().UseSkill_By_SS();
			else
				fLastDis = fNowDis;
		}

		if (nCount >= 5 * 10) // 10s
		{
			LOG_CF_D( L"10s之内都没走到目的地, 还有%.2fm", CPerson::GetInstance().GetDis(TarPoint));
			//Move_To_Monster_By_Nav(Monster);
			//return CPerson::GetInstance().GetDis(TarPoint) >= fFaceDis ? FALSE : TRUE;
			return TRUE;
		}

		BnsSleep(100);
	}


	/*if (CFarmSkill::GetInstance().IsUnEnableSkill3())
	{
		//LOG_CF_D( L"打怪走路的时候,3技能变灰了,按一下W");
		CSomeFun::GetInstance().SimlationKey('W', 0, 0);
	}*/

	return TRUE;
}

VOID CFarmMonster::SetMoveToMonster(_In_ BOOL bMove)
{
	_bMove = bMove;
}

BOOL CFarmMonster::Lux_Single(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo, _In_ BOOL bIsForce, _In_ std::function<UINT(VOID)> fnGetAroundCount) CONST
{
	LOG_CF_D( L"Lux_Single, Name=%s, Dis=%.2f", Monster.GetName().c_str(), Monster.GetDis());
	auto dwTick = ::GetTickCount64();
	auto dwHp = Monster.GetHp();

	CPersonFunction::GetInstance().CancelPutDownStatus();
	while (GameRun && !EMPTY_PERSONHP && !Monster.IsDead() && IsLoseHp(Monster, dwHp, dwTick))
	{
		if (CPersonFunction::GetInstance().IsGameLoad())
		{
			LOG_CF_D( L"打怪的时候出现读图!, 等待过图完毕后break!");
			CPersonFunction::GetInstance().IsGameLoad();
			break;
		}
		if (!Move_To_Monster(Monster, FixPtInfo))
		{
			LOG_CF_D( L"卡刀时,无法移动到怪物旁边! ID=%X, Name=%s, HP=%d,Dis=%.2f", Monster.GetId(), Monster.GetName().c_str(), Monster.GetHp(), Monster.GetDis());
			return FALSE;
		}

		Monster.SetAngle(); // 对准
		if (Monster.GetHp() != Monster.GetMaxHp() && !bIsForce && fnGetAroundCount() > 1)
		{
			LOG_CF_D( L"周围怪物数量>1, 不用卡刀!");
			break;
		}

		// 用V加血(内置有判断条件)
		if (UseLux_V(Monster))
			continue;

		// 使用技能
		if (!UseLuxSkill(Monster, TRUE))
			continue;

		Monster.SetAngle(); // 对准
		if (CPerson::GetInstance().GetMp() >= 50)
		{
			CSkill::UseSkill_By_Id(Lux_SkillId_R, TRUE);
			BnsSleep(100);
			CSkill::UseSkill_By_Id(Lux_SkillId_T, TRUE);
			continue;
		}

		CSkill::UseSkill_By_Id(Lux_SkillId_R, TRUE);
		BnsSleep(100);
	}

	return CGameVariable::GetInstance().Action_Variable_By_Name(VARIABLE_NAME_AUTOTAKEITEM, TRUE, [&FixPtInfo]{
		CResAction::GetInstance().TakeDropItem(FixPtInfo);
	});
}

BOOL CFarmMonster::UseLux_V(_In_ CONST CMonster&) CONST
{
	if (CPerson::GetInstance().GetPercentHp() <= 70 && CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_V))
	{
		LOG_CF_D( L"使用V!");
		CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_V);
		return TRUE;
	}
	if (CPersonFunction::GetInstance().Exist_Lux_V_Buff())
	{
		CSkill::UseSkill_By_Id(Lux_SkillId_V_R);
		return TRUE;
	}

	return FALSE;
}

BOOL CFarmMonster::Lux_AOE(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo, _In_ BOOL bIsForce, _In_ std::function<UINT(VOID)> fnGetAroundCount) CONST
{
	LOG_CF_D( L"Lux_AOE, Name=%s, Dis=%.2f", Monster.GetName().c_str(), Monster.GetDis());
	auto dwTick = ::GetTickCount64();
	auto dwHp = Monster.GetHp();
	auto dwLevel = CPerson::GetInstance().GetLevel();

	CPersonFunction::GetInstance().CancelPutDownStatus();
	while (GameRun && !EMPTY_PERSONHP && !Monster.IsDead() && IsLoseHp(Monster, dwHp, dwTick))
	{
		if (CPersonFunction::GetInstance().IsGameLoad())
		{
			LOG_CF_D( L"打怪的时候出现读图!, 等待过图完毕后break!");
			CPersonFunction::GetInstance().IsGameLoad();
			break;
		}
		// 距离不足的跑近
		if (!Move_To_Monster(Monster, FixPtInfo))
		{
			LOG_CF_D( L"杀怪时,无法移动到怪物旁边! ID=%X, Name=%s, HP=%d,Dis=%.2f", Monster.GetId(), Monster.GetName().c_str(), Monster.GetHp(), Monster.GetDis());
			return FALSE;
		}

		if (CPerson::GetInstance().GetTargetId() != Monster.GetId())
			Monster.SetAngle(); // 对准

		// 判断是否需要切换卡刀还是AOE
		if (Monster.GetHp() != Monster.GetMaxHp() && !bIsForce && fnGetAroundCount() == 1)
		{
			LOG_CF_D( L"周围怪物数量==1, 用卡刀!");
			break;
		}

		// 用V加血(内置有判断条件)
		if (UseLux_V(Monster))
			continue;

		// 使用技能
		if (!UseLuxSkill(Monster, FALSE))
			continue;


		// MP > 50 使用Tab
		if (dwLevel >= 20)
		{
			if (CPerson::GetInstance().GetMp() > 50)
			{
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
				continue;
			}
			else if (CPersonFunction::GetInstance().Exist_Lux_Tab_Buff()) // 使用R
			{
				BnsSleep(300);
				continue;
			}
		}
		else
		{
			if (CPerson::GetInstance().GetMp() > 50)
			{
				CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
				BnsSleep(800);
				continue;
			}
		}

		CSkill::UseSkill_By_Id(Lux_SkillId_R);
		BnsSleep(200);
	}

	// 根据设置的变量是否需要打完一个怪就捡物
	return CGameVariable::GetInstance().Action_Variable_By_Name(VARIABLE_NAME_AUTOTAKEITEM, TRUE, [&FixPtInfo]{
		CResAction::GetInstance().TakeDropItem(FixPtInfo);
	});
}

BOOL CFarmMonster::IsLoseHp(_In_ CONST CMonster& Monster, _In_ DWORD& dwLastHp, _In_ ULONGLONG& ulTick, _In_ ULONGLONG ulMaxTimeOut /* = 10 * 1000 */) CONST
{
	if (Monster.GetHp() == dwLastHp)
	{
		if (static_cast<DWORD>(::GetTickCount64() - ulTick) >= ulMaxTimeOut)
		{
			// 设置不打!
			LOG_CF_D( L"怪物:%s 10s内不掉血, 拉入黑名单", Monster.GetName().c_str());
			CFilter::GetInstance().AddTemporaryMonsterFilter(Monster.GetName());
			if (CPerson::GetInstance().GetClasses() == em_Player_Classes::em_Player_Classes_Summoner)
			{
				LOG_CF_D( L"当前是召唤职业, 把喵给召回!");
				CallBackCat();
			}

			return FALSE;
		}
	}
	else
	{
		dwLastHp = Monster.GetHp();
		ulTick = ::GetTickCount64();
	}
	return TRUE;
}


BOOL CFarmMonster::Kffm_Fm_Round(_In_ CONST CMonster& Monster) CONST
{
	// 从拳师姿势转换成气功师姿势!
	if (CPerson::GetInstance().GetFightPose() == em_Player_Pose_KongFuMaster)
	{
		if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_HideUi(Kffm_Fm_SkillId_SS))
		{
			CFarmSkill::GetInstance().UseSkill_By_SkillId(Kffm_Fm_SkillId_SS);
			return TRUE;
		}
		CSomeFun::GetInstance().SimlationKey(VK_TAB, 200);
		return TRUE;
	}



	if (CPerson::GetInstance().GetMp() >= 3)
		CSkill::UseSkill_By_Id(Kffm_Fm_SkillId_T, TRUE);
	else
		CSkill::UseSkill_By_Id(Kffm_Fm_SkillId_R, TRUE);
	return TRUE;
}

BOOL CFarmMonster::UseLuxSkill(_In_ CONST CMonster& Monster, _In_ BOOL bKaDaoMode) CONST
{
	auto& FarmSkill = CFarmSkill::GetInstance();

	// 使用了E后, 只用RT!, 不用其他技能(除了V)
	if (bKaDaoMode && CPersonFunction::GetInstance().Exist_Lux_E_Buff())
	{
		CSkill::UseSkill_By_Id(Lux_SkillId_E_R, TRUE);
		BnsSleep(100);
		CSkill::UseSkill_By_Id(Lux_SkillId_T, TRUE);
		return FALSE;
	}

	if (bKaDaoMode && FarmSkill.UseSkill_By_Destoer_E()) // 只在卡刀下立即释放E
	{
		LOG_CF_D( L"使用E");
		return FALSE;
	}

	if (CPerson::GetInstance().GetPercentHp() < 90 && CPerson::GetInstance().GetMp() >= 20 && FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_X))
	{
		LOG_CF_D( L"使用X!");
		FarmSkill.UseSkill_By_SkillId(Lux_SkillId_X);
		return TRUE;
	}


	if (FarmSkill.ExistQTE())
	{
		CSomeFun::GetInstance().SimlationKey(VK_SPACE, 200, 300);
		return TRUE;
	}

	if (bKaDaoMode)
	{
		static CONST std::vector<DWORD> vFarmAroundSkill = {
			Lux_SkillId_1, Lux_SkillId_2, Lux_SkillId_3, Lux_SkillId_4, Lux_SkillId_Z_Attract
		};
		// 异常状态下不用技能
		if (Monster.GetFightStatus() == em_FightStatus_Other && CPerson::GetInstance().GetMp() >= 20)
		{
			UINT uIndex = 0;
			for (CONST auto& itm : vFarmAroundSkill)
			{
				if (uIndex++ >= 2 && CPerson::GetInstance().GetMp() < 50)
					return TRUE;

				if (FarmSkill.IsCool_By_SkillId_In_Ui(itm))
				{
					FarmSkill.UseSkill_By_SkillId(itm);
					return TRUE;
				}
			}
		}
		return TRUE;
	}

	if (CPerson::GetInstance().GetMp() >= 50)
	{
		if (FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_Z_Attract) && FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_3))
		{
			LOG_CF_D( L"使用连招Z + 3 + Q");
			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Z_Attract);
			if (FarmSkill.UseSkill_By_Destoer_E())
				BnsSleep(300);

			//CSomeFun::GetInstance().SimlationKey('W', 100, 200);
			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_3);
			FarmSkill.UseSkill_By_Destoer_Q();
		}

		return TRUE;
	}

	static CONST std::vector<DWORD> vFarmAroundSkill = {
		Lux_SkillId_1, Lux_SkillId_3, Lux_SkillId_4, Lux_SkillId_Z_Attract, Lux_SkillId_Q
	};

	// 异常状态下不用技能
	if (CPerson::GetInstance().GetFightStatus() == em_FightStatus_Other  && CPerson::GetInstance().GetMp() >= 20)
	{
		UINT uIndex = 0;
		for (CONST auto& itm : vFarmAroundSkill)
		{
			if (uIndex++ >= 2 && CPerson::GetInstance().GetMp() < 20)
				return TRUE;

			if (FarmSkill.IsCool_By_SkillId_In_Ui(itm))
			{
				FarmSkill.UseSkill_By_SkillId(itm);
				return TRUE;
			}
		}
	}
	return TRUE;
}

BOOL CFarmMonster::PK_Lux(_In_ CONST CPlayer& Player, _In_ BOOL, _In_ std::function<BOOL(VOID)> fnTerminate) CONST
{
	static CONST std::vector<DWORD> vControlSkill = {
		Lux_SkillId_Z_Kick, Lux_SkillId_F_Kick, Lux_SkillId_2, Lux_SkillId_4, Lux_SkillId_1,
	};

	CMonster Monster(Player);
	auto& FarmSkill = CFarmSkill::GetInstance();
	CSomeFun::GetInstance().SetSpeed(3.0f);
	BOOL bRunThread = TRUE;

	std::thread t([&FarmSkill, &bRunThread]{
		CONST static  std::vector<DWORD> vlst = {
			Lux_SkillId_Floor_2, Lux_SkillId_Floor_F, Lux_SkillId_Floor_Tab,
		};
		while (bRunThread && GameRun)
		{
			for (CONST auto& itm : vlst)
			{
				if (FarmSkill.IsCool_By_SkillId_In_Ui(itm))
				{
					//LOG_CF_D(L"使用技能:%X", itm);
					CSkill::UseSkill_By_Id(itm, FALSE);
					break;
				}
			}

			BnsSleep(100);
		}
	});

	while (GameRun && !EMPTY_PERSONHP && !Monster.IsDead() && !fnTerminate() && Monster.GetDis() < 100)
	{
		Monster.SetAngle();
		if (Monster.GetDis() >= 4.0f)
		{
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_1))
			{
				FarmSkill.UseSkill_By_SkillId(Lux_SkillId_1);
				continue;
			}
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_2))
			{
				FarmSkill.UseSkill_By_SkillId(Lux_SkillId_2);
				continue;
			}

			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab, FALSE);
			CPersonFunction::GetInstance().Move(Monster.GetPoint());
			continue;
		}

		if (CPersonFunction::GetInstance().Exist_Lux_E_Buff())
		{
			CSkill::UseSkill_By_Id(Lux_SkillId_E_R, TRUE);
			CSkill::UseSkill_By_Id(Lux_SkillId_T, TRUE);
			continue;
		}

		// 异常状态下不用技能
		if (Monster.GetFightStatus() == em_FightStatus_Other && CPerson::GetInstance().GetMp() >= 20)
		{
			for (CONST auto& itm : vControlSkill)
			{
				if (FarmSkill.IsCool_By_SkillId_In_Ui(itm) && Monster.GetDis() <= 4.0f)
				{
					FarmSkill.UseSkill_By_SkillId(itm);
					Monster.SetAngle();
					break;
				}
			}
		}

		if (CPerson::GetInstance().GetMp() >= 20)
		{
			CONST static std::vector<DWORD> vSkill = { Lux_SkillId_F_Throat, Lux_SkillId_F_Fly, 0x5DDA, 0x5E8B, 0x5E11 };
			for (CONST auto& itm : vSkill)
			{
				if (FarmSkill.IsCool_By_SkillId_In_Ui(itm))
				{
					FarmSkill.UseSkill_By_SkillId(itm);
					break;
				}
			}

			if (FarmSkill.UseSkill_By_Destoer_E() && FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_X + 4))
				FarmSkill.UseSkill_By_SkillId(Lux_SkillId_X + 4);

			if (FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_X))
				FarmSkill.UseSkill_By_SkillId(Lux_SkillId_X);
		}

		


		// 用V加血(内置有判断条件)
		if (!CPersonFunction::GetInstance().Exist_Lux_E_Buff() && CPerson::GetInstance().GetPercentHp() <= 80 && FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_V))
		{
			FarmSkill.UseSkill_By_SkillId(Lux_SkillId_V);
			while (GameRun && !EMPTY_PERSONHP && CPersonFunction::GetInstance().Exist_Lux_V_Buff())
			{
				Monster.SetAngle();
				FarmSkill.UseSkill_By_SkillId(Lux_SkillId_V_R);
			}
			continue;
		}

		Monster.SetAngle();
		if (CPerson::GetInstance().GetMp() >= 30)
		{
			CSkill::UseSkill_By_Id(Lux_SkillId_R, TRUE);
			CSkill::UseSkill_By_Id(Lux_SkillId_T, TRUE);
			continue;
		}
		CSkill::UseSkill_By_Id(Lux_SkillId_V_R, TRUE);
	}
	bRunThread = FALSE;
	t.join();
	LOG_CF_D( L"PK完毕!");
	return TRUE;
}

BOOL CFarmMonster::PK_Lux_Pk(_In_ CONST CPlayer& Player, _In_ BOOL, _In_ std::function<BOOL(VOID)> fnTerminate) CONST
{
	static CONST std::vector<DWORD> vControlSkill = {
		Lux_SkillId_Z_Kick, Lux_SkillId_F_Kick, Lux_SkillId_2, Lux_SkillId_4, Lux_SkillId_1,
	};

	CMonster Monster(Player);
	auto& FarmSkill = CFarmSkill::GetInstance();
	CSomeFun::GetInstance().SetSpeed(3.0f);
	BOOL bRunThread = TRUE;

	std::thread t([&FarmSkill, &bRunThread]{
		CONST static  std::vector<DWORD> vlst = {
			Lux_SkillId_Floor_2, Lux_SkillId_Floor_F, Lux_SkillId_Floor_Tab, Lux_SkillId_F_Throat, Lux_SkillId_F_Z
		};
		while (bRunThread && GameRun)
		{
			for (CONST auto& itm : vlst)
			{
				if (FarmSkill.IsCool_By_SkillId_In_Ui(itm))
				{
					//LOG_CF_D(L"使用技能:%X", itm);
					FarmSkill.UseSkill_By_SkillId(itm);
					break;
				}
			}

			BnsSleep(100);
		}
	});

	while (GameRun && !EMPTY_PERSONHP && !Monster.IsDead() && !fnTerminate() && Monster.GetDis() < 100)
	{
		Monster.SetAngle();
		if (Monster.GetDis() >= 4.0f)
		{
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_1))
			{
				FarmSkill.UseSkill_By_SkillId(Lux_SkillId_1);
				continue;
			}
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_2))
			{
				FarmSkill.UseSkill_By_SkillId(Lux_SkillId_2);
				continue;
			}

			FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
			Move_To_Monster(Monster, FixPointInfo(CBaseObject::GetPersonPoint(), 100));
			continue;
		}

		if (CPersonFunction::GetInstance().Exist_Lux_E_Buff())
		{
			CSkill::UseSkill_By_Id(Lux_SkillId_E_R, TRUE);
			CSkill::UseSkill_By_Id(Lux_SkillId_T, TRUE);
			continue;
		}

		// 异常状态下不用技能
		if (Monster.GetFightStatus() == em_FightStatus_Other && CPerson::GetInstance().GetMp() >= 20)
		{
			for (CONST auto& itm : vControlSkill)
			{
				if (FarmSkill.IsCool_By_SkillId_In_Ui(itm) && Monster.GetDis() <= 4.0f)
				{
					FarmSkill.UseSkill_By_SkillId(itm);
					Monster.SetAngle();
					break;
				}
			}
		}

		if (CPerson::GetInstance().GetPercentHp() <= 90 && FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_C))
		{
			FarmSkill.UseSkill_By_SkillId(Lux_SkillId_C);
			FarmSkill.UseSkill_By_SkillId(Lux_SkillId_C_c);
		}

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_C_c))
			FarmSkill.UseSkill_By_SkillId(Lux_SkillId_C_c);

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_3))
			FarmSkill.UseSkill_By_SkillId(Lux_SkillId_3);

		if (CPerson::GetInstance().GetPercentHp() <= 95 && FarmSkill.UseSkill_By_Destoer_E() && FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_X + 4))
			FarmSkill.UseSkill_By_SkillId(Lux_SkillId_X + 4);

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_X))
			FarmSkill.UseSkill_By_SkillId(Lux_SkillId_X);

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_X + 0x2))
			FarmSkill.UseSkill_By_SkillId(Lux_SkillId_X + 0x2);


		// 用V加血(内置有判断条件)
		if (!CPersonFunction::GetInstance().Exist_Lux_E_Buff() && CPerson::GetInstance().GetPercentHp() <= 80 && FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_V))
		{
			FarmSkill.UseSkill_By_SkillId(Lux_SkillId_V);
			while (GameRun && !EMPTY_PERSONHP && CPersonFunction::GetInstance().Exist_Lux_V_Buff())
			{
				Monster.SetAngle();
				FarmSkill.UseSkill_By_SkillId(Lux_SkillId_V_R);
			}
			continue;
		}

		Monster.SetAngle();
		if (CPerson::GetInstance().GetMp() >= 30)
		{
			CSkill::UseSkill_By_Id(Lux_SkillId_R, TRUE);
			CSkill::UseSkill_By_Id(Lux_SkillId_T, TRUE);
			continue;
		}
		CSkill::UseSkill_By_Id(Lux_SkillId_R, TRUE);
	}
	bRunThread = FALSE;
	t.join();
	LOG_CF_D( L"PK完毕!");
	return TRUE;
}

BOOL CFarmMonster::Kffm_AOE(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo, _In_ BOOL bIsForce, _In_ std::function<UINT(VOID)> fnGetAroundCount) CONST
{
	LOG_CF_D( L"Lux_AOE, Name=%s, Dis=%.2f", Monster.GetName().c_str(), Monster.GetDis());
	auto dwTick = ::GetTickCount64();
	auto dwHp = Monster.GetHp();
	auto& FarmSkill = CFarmSkill::GetInstance();

	CPersonFunction::GetInstance().CancelPutDownStatus();
	while (GameRun && !EMPTY_PERSONHP && !Monster.IsDead() && IsLoseHp(Monster, dwHp, dwTick))
	{
		if (CPersonFunction::GetInstance().IsGameLoad())
		{
			LOG_CF_D( L"打怪的时候出现读图!, 等待过图完毕后break!");
			CPersonFunction::GetInstance().IsGameLoad();
			break;
		}

		// 距离不足的跑近
		em_Player_Pose emPersonPose = CPerson::GetInstance().GetFightPose();
		if (emPersonPose != em_Player_Pose_ForceMaster)
		{
			CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
			continue;
		}
		 if (emPersonPose == em_Player_Pose_ForceMaster && Monster.GetDis() > 10.0f) // 气功姿势
		{
			if (!Move_To_Monster(Monster, FixPtInfo))
			{
				LOG_CF_D( L"杀怪时,无法移动到怪物旁边! ID=%X, Name=%s, HP=%d,Dis=%.2f", Monster.GetId(), Monster.GetName().c_str(), Monster.GetHp(), Monster.GetDis());
				return FALSE;
			}
		}


		Monster.SetAngle(); // 对准

		// 判断是否需要切换卡刀还是AOE
		if (Monster.GetHp() != Monster.GetMaxHp() && !bIsForce && fnGetAroundCount() == 1)
		{
			LOG_CF_D( L"周围怪物数量==1, 切换拳状态!");
			break;
		}

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Kffm_Fm_SkillId_2))
		{
			CSomeFun::GetInstance().SimlationKey(VK_2, 10);
		}

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Kffm_Fm_SkillId_4))
		{
			CSomeFun::GetInstance().SimlationKey(VK_4, 10);
		}

		if (CPerson::GetInstance().GetMp() > 5)
			CSomeFun::GetInstance().SimlationKey('T', 10);
		else
			CSomeFun::GetInstance().SimlationKey('R', 10);
		Sleep(100);
	}

	// 根据设置的变量是否需要打完一个怪就捡物
	return CGameVariable::GetInstance().Action_Variable_By_Name(VARIABLE_NAME_AUTOTAKEITEM, TRUE, [&FixPtInfo]{
		CResAction::GetInstance().TakeDropItem(FixPtInfo);
	});
}

BOOL CFarmMonster::Pk_Kffm(_In_ CPlayer& Player, _In_ CONST FixPointInfo& RoundPoint, _In_ BOOL, _In_ std::function<BOOL(VOID)> fnTerminte) CONST
{
	LOG_CF_D( L"Pk_Kffm没写");
	return TRUE;
}

BOOL CFarmMonster::KillMonster_By_AOE(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo, _In_ BOOL bIsForce, _In_ std::function<UINT(VOID)> fnGetAroundCount) CONST
{
	em_Player_Classes emPlayerClasses = CPerson::GetInstance().GetClasses();
	LOG_CF_D( L"emPlayerClasses=%X", emPlayerClasses);
	CSomeFun::GetInstance().SetSpeed(3.0f);
	switch (emPlayerClasses)
	{
	case em_Player_Classes_Destroyer:
		return Lux_AOE(Monster, FixPtInfo, bIsForce, fnGetAroundCount);
	case em_Player_Classes_Assassin:
		return Assassin_Singe(Monster, FixPtInfo);
	case em_Player_Classes_BladeMaster:
		return BladeMaster_Singe(Monster, FixPtInfo);
	case em_Player_Classes_Summoner:
		return Summoner_Singe(Monster, FixPtInfo);
	case em_Player_Classes_ForceMaster:
		return ForceMaster_Singe(Monster, FixPtInfo);
	case em_Player_Classes_KongFuMaster:
		return KongFuMaster_Singe(Monster, FixPtInfo);
	case em_Player_Classes_Danncer:
		return Danncer_Singe(Monster, FixPtInfo);
	case em_Player_Classes_Warlock:
		return Warlock_Aoe(Monster, FixPtInfo);
	case em_Player_Classes_KongFuForceMaster:
		return Kffm_AOE(Monster, FixPtInfo, bIsForce, fnGetAroundCount);
	case em_Player_Classes_Gunner:
		return Gunner_Aoe(Monster, FixPtInfo);
	default:
		LOG_MSG_CF( L"未知的职业:%X", emPlayerClasses);
		break;
	}
	return FALSE;
}

BOOL CFarmMonster::KillMonster_By_Single(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo, _In_ BOOL bIsForce, _In_ std::function<UINT(VOID)> fnGetAroundCount) CONST
{
	em_Player_Classes emPlayerClasses = CPerson::GetInstance().GetClasses();
	switch (emPlayerClasses)
	{
	case em_Player_Classes_Destroyer:
		return Lux_Single(Monster, FixPtInfo, bIsForce, fnGetAroundCount);
	case em_Player_Classes_Assassin:
		return Assassin_Singe(Monster, FixPtInfo);
	case em_Player_Classes_BladeMaster:
		return BladeMaster_Singe(Monster, FixPtInfo);
	case em_Player_Classes_Summoner:
		return Summoner_Singe(Monster, FixPtInfo);
	case em_Player_Classes_ForceMaster:
		return ForceMaster_Singe(Monster, FixPtInfo);
	case em_Player_Classes_KongFuMaster:
		return KongFuMaster_Singe(Monster, FixPtInfo);
	case em_Player_Classes_Danncer:
		return Danncer_Singe(Monster, FixPtInfo);
	case em_Player_Classes_Warlock:
		return Warlock_Singe(Monster, FixPtInfo);
	case em_Player_Classes_KongFuForceMaster:
		return Kffm_Singe(Monster, FixPtInfo);
	case em_Player_Classes_Gunner:
		return Gunner_Singe(Monster, FixPtInfo);
	default:
		break;
	}
	
	return FALSE;
}

BOOL CFarmMonster::KillMonster_By_Dog(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__,[&]
	{
		em_Player_Classes emPlayerClasses = (em_Player_Classes)CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->dwClasses;
		//LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"emPlayerClasses=%X", emPlayerClasses);
		//CSomeFun::GetInstance().SetSpeed(3.0f);
		switch (emPlayerClasses)
		{
		case em_Player_Classes_Destroyer:
			return Lux_Single(Monster, FixPtInfo, TRUE, nullptr);
		case em_Player_Classes_Assassin:
			return Assassin_Singe(Monster, FixPtInfo);
		case em_Player_Classes_BladeMaster:
			return BladeMaster_Singe(Monster, FixPtInfo);
		case em_Player_Classes_Summoner:
			return Summoner_KillDog(Monster, FixPtInfo);
		case em_Player_Classes_ForceMaster:
			return ForceMaster_Singe(Monster, FixPtInfo);
		case em_Player_Classes_KongFuMaster:
			return KongFuMaster_Singe(Monster, FixPtInfo);
		case em_Player_Classes_Danncer:
			return Danncer_Singe(Monster, FixPtInfo);
		case em_Player_Classes_Warlock:
			return Warlock_Aoe(Monster, FixPtInfo);
		case em_Player_Classes_KongFuForceMaster:
			return Kffm_KillDog(Monster, FixPtInfo);
		case em_Player_Classes_Gunner:
			return Gunner_Singe(Monster, FixPtInfo);
		default:
			break;
		}
		return FALSE;
	});
}


BOOL CFarmMonster::BackwardWhenClosing(_In_ CONST CMonster& Monster_, _In_ BOOL bRun, _In_ float fClosingDis_, _In_ float fBackwardDis_) CONST
{
	static std::thread BackwardThread;
	static bool bRunThread = false;

	struct BackwardContent
	{
		CMonster Monster;
		float    fClosingDis;
		float	 fBackwardDis;
	};
	static BackwardContent BackwardContent_;


	if (!bRun)
	{
		if (BackwardThread.joinable())
		{
			LOG_CF_D( L"后退线程正在结束……");
			bRunThread = false;
			BackwardThread.join();
			BackwardThread = std::thread();
			LOG_CF_D( L"后退线程完毕……");
		}
		else
			LOG_CF_D( L"后退线程已经被结束了!");
		return TRUE;
	}
	

	if (BackwardThread.joinable())
	{
		LOG_CF_D( L"后退线程已经被创建了, 必须先结束才能再开启新的线程!");
		return FALSE;
	}


	bRunThread = true;
	BackwardContent_.Monster = Monster_;
	BackwardContent_.fBackwardDis = fBackwardDis_;
	BackwardContent_.fClosingDis = fClosingDis_;


	BackwardThread = std::thread([]
	{
		while (GameRun && !EMPTY_PERSONHP && bRunThread && !BackwardContent_.Monster.IsDead() && BackwardContent_.Monster.GetDis() < 36.0f)
		{
			CPersonFunction::GetInstance().WaitGameLoad();
			if (BackwardContent_.Monster.GetDis() <= BackwardContent_.fClosingDis)
			{
				LOG_CF_D( L"怪物:%s 靠近了%.2fm了,后退!", BackwardContent_.Monster.GetName().c_str(), BackwardContent_.Monster.GetDis());
				BnsMove(BackwardContent_.Monster.GetFacePoint(16), 3);
			}
			else if (BackwardContent_.Monster.GetDis() >= 16.0f)
			{
				BnsMove(BackwardContent_.Monster.GetFacePoint(16), 3);
			}
			BnsSleep(500);
		}
		LOG_CF_D( L"已经退出了后退线程!");
		CSomeFun::GetInstance().SimlationKey('S');
	});
	return TRUE;
}

BOOL CFarmMonster::Kffm_Singe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST
{
	auto dwTick = ::GetTickCount64();
	auto dwHp = Monster.GetHp();

	CPersonFunction::GetInstance().CancelPutDownStatus();
	auto& FarmSkill = CFarmSkill::GetInstance();
	while (GameRun && !EMPTY_PERSONHP && !Monster.IsDead() && IsLoseHp(Monster, dwHp, dwTick))
	{
		// 距离不足的跑近
		em_Player_Pose emPersonPose = CPerson::GetInstance().GetFightPose();
		if (emPersonPose == em_Player_Pose_KongFuMaster)
		{
			if (!Move_To_Monster(Monster, FixPtInfo))
			{
				LOG_CF_D( L"杀怪时,无法移动到怪物旁边! ID=%X, Name=%s, HP=%d,Dis=%.2f", Monster.GetId(), Monster.GetName().c_str(), Monster.GetHp(), Monster.GetDis());
				return FALSE;
			}
		}
		else if (emPersonPose == em_Player_Pose_ForceMaster && Monster.GetDis() > 15.0f) // 气功姿势
		{
			if (!Move_To_Monster(Monster, FixPtInfo))
			{
				LOG_CF_D( L"杀怪时,无法移动到怪物旁边! ID=%X, Name=%s, HP=%d,Dis=%.2f", Monster.GetId(), Monster.GetName().c_str(), Monster.GetHp(), Monster.GetDis());
				return FALSE;
			}
		}

		Monster.SetAngle(); // 对准

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Kffm_Kfm_SkillId_2))
		{
			CSomeFun::GetInstance().SimlationKey(VK_2, 10);
		}
		

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Kffm_Kfm_SkillId_3))
		{
			CSomeFun::GetInstance().SimlationKey(VK_3, 10);
		}


		if (FarmSkill.IsCool_By_SkillId_In_Ui(Kffm_Kfm_SkillId_4))
		{
			CSomeFun::GetInstance().SimlationKey(VK_4, 10);
		}


		if (CPerson::GetInstance().GetMp() >= 5)
			CSomeFun::GetInstance().SimlationKey('T');
		else
			CSomeFun::GetInstance().SimlationKey('R');
		Sleep(50);
		CSomeFun::GetInstance().SimlationKey('F');
		Sleep(100);
	}

	// 根据设置的变量是否需要打完一个怪就捡物
	return CGameVariable::GetInstance().Action_Variable_By_Name(VARIABLE_NAME_AUTOTAKEITEM, TRUE, [&FixPtInfo]{
		CResAction::GetInstance().TakeDropItem(FixPtInfo);
	});
}

VOID CFarmMonster::SetPlayerClassesRoundMode() CONST
{
	DWORD dwValue = 0;
	switch (CPerson::GetInstance().GetClasses())
	{
	case em_Player_Classes::em_Player_Classes_KongFuForceMaster:
		dwValue = static_cast<DWORD>(em_KillMonster_Round_Mode::em_KillMonster_Round_Mode_KFFM);
		break;
	case em_Player_Classes::em_Player_Classes_Summoner:
		dwValue = static_cast<DWORD>(em_KillMonster_Round_Mode::em_KillMonster_Round_Mode_SUMMONER);
		break;
	case em_Player_Classes::em_Player_Classes_Warlock:
		dwValue = static_cast<DWORD>(em_KillMonster_Round_Mode::em_KillMonster_Round_Mode_WarLock);
		break;
	default:
		break;
	}
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_KILLMONSTER_ROUND_MODE, dwValue);
}

BOOL CFarmMonster::KillMonster_By_Round(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo, _In_ CONST FixPointInfo& RoundPoint) CONST
{
	std::vector<Point> vRound;
	GetRoundPointList(RoundPoint, 12, vRound);
	if (vRound.size() == 0)
	{
		LOG_MSG_CF( L"vRound.size()=%d", vRound.size());
		return FALSE;
	}
	
	// 运行对准角度和跑路线程!
	BOOL bRunThread = TRUE;
	std::thread t([&bRunThread, &Monster, &vRound, RoundPoint]
	{
		UINT nIndex = 0;
		while (bRunThread && GameRun)
		{
			auto& NextPoint = vRound.at(nIndex);
			CPersonFunction::GetInstance().Move(NextPoint);

			nIndex = (nIndex + 1 >= vRound.size()) ? 0 : (nIndex + 1);

			// 这里是循环走路, 没走到下一个坐标点就会一直走
			while (CBaseObject::GetPersonDis(NextPoint) > 1.0f && GameRun && !EMPTY_PERSONHP && bRunThread)
			{
				Monster.SetRoundAngle();
				BnsSleep(100);

				if (CBaseObject::GetPersonDis(NextPoint) > 100.0f)
				{
					bRunThread = FALSE;
					break;
				}

				if (!CPerson::GetInstance().IsMoving())
				{
					//LOG_CF_D( L"正在走第%d个坐标, 距离:%.2f", nIndex, CBaseObject::GetPersonDis(NextPoint));
					CPersonFunction::GetInstance().Move(NextPoint);
				}
			}

			// 走完之后停顿多久
			BnsSleep(1500);
		}
		LOG_CF_D(L"Exit SetAngle Thread!");
	});

	auto dwHp = Monster.GetHp();
	auto dwTick = ::GetTickCount64();

	CCat Cat;
	CCatExpend::GetInstance().FindSelfCat(&Cat);

	em_KillMonster_Round_Mode emMode = static_cast<em_KillMonster_Round_Mode>(CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_KILLMONSTER_ROUND_MODE));
	while (!Monster.IsDead() && GameRun && !EMPTY_PERSONHP && bRunThread && IsLoseHp(Monster, dwHp, dwTick))
	{
		if (Monster.GetDis(FixPtInfo.FixPoint) > FixPtInfo.fDis)
		{
			LOG_CF_D( L"怪物超出攻击范围! 现在的距离是:%.2f, 限定范围是:%.2f", Monster.GetDis(FixPtInfo.FixPoint), FixPtInfo.fDis);
			break;
		}

		BnsSleep(200);
		switch (emMode)
		{
		case CFarmMonster::em_KillMonster_Round_Mode_KFFM:
			Kffm_Fm_Round(Monster);
			break;
		case CFarmMonster::em_KillMonster_Round_Mode_SUMMONER:
			Summoner_Round(Monster, Cat);
			break;
		case CFarmMonster::em_KillMonster_Round_Mode_XYG_IceFireBird:
			XueYuGong_IceFireBird(Monster, Cat);
			break;
		case CFarmMonster::em_KillMonster_Round_Mode_WarLock:
			Warlock_Round(Monster, Cat);
			break;
		default:
			LOG_MSG_CF( L"暂时还不支持该模式!:emMode=%X", emMode);
			break;
		}
	}
	bRunThread = FALSE;
	LOG_CF_D(L"等待线程结束……");
	t.join();
	LOG_CF_D(L"线程成功结束");
	return TRUE;
}


BOOL CFarmMonster::Kffm_KillDog(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST
{

	return TRUE;
}


BOOL CFarmMonster::GetRoundPointList(_In_ CONST FixPointInfo& RoundPoint, _In_ UINT uCount, _Out_ std::vector<Point>& vlst) CONST
{
	// 已知圆心和半径, 把圆分成8份, 那就是每个坐标是45°
	// 根据公式(x－a)²+(y－b)²=r²

	// 还是用三角函数好
	// X = 圆心.X + 半径* cos(角度 * 3.14 / 180)
	// Y = 圆心.Y + 半径* cin(角度 * 3.14 / 180)
	auto fn = [&RoundPoint](_In_ float fAngel)
	{
		Point FixPoint;
		FixPoint.X = RoundPoint.FixPoint.X + RoundPoint.fDis * 50.0f * cos(fAngel * 3.14f / 180.0f);
		FixPoint.Y = RoundPoint.FixPoint.Y + RoundPoint.fDis * 50.0f * sin(fAngel * 3.14f / 180.0f);
		FixPoint.Z = RoundPoint.FixPoint.Z;
		return FixPoint;
	};
	float fSub = 360.0f / (float)uCount;
	for (UINT i = 0; i < uCount; ++i)
		vlst.push_back(fn((float)i * fSub));

	return TRUE;
}

BOOL CFarmMonster::ExistSpecialMonster(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST
{
	auto bExist = FALSE;
	FixPointInfo RoundInfo;
	CGameVariable::GetInstance().Action_Map_Kffm_Fm_SpecialMonster([&Monster, &bExist, &RoundInfo](std::map<std::wstring, FixPointInfo>& map_){
		auto itr = map_.find(Monster.GetName());
		if (itr != map_.end())
		{
			bExist = TRUE;
			RoundInfo = itr->second;
		}
	});

	return bExist ? KillMonster_By_Round(Monster, FixPtInfo, RoundInfo) : FALSE;
}

BOOL CFarmMonster::Summoner_Singe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST
{
	LOG_CF_D( L"Summoner_Singe, Name=%s, Dis=%.2f", Monster.GetName().c_str(), Monster.GetDis());
	auto dwTick = ::GetTickCount64();
	auto dwTick_Q = ::GetTickCount64();
	auto dwHp = Monster.GetHp();
	auto& FarmSkill = CFarmSkill::GetInstance();
	BOOL bEnableZ = CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_ENABLE_Z) == 1 ? TRUE : FALSE;
	CCat newcat;
	CCatExpend::GetInstance().FindSelfCat(&newcat);

	CPersonFunction::GetInstance().CancelPutDownStatus();
	Monster.SetAngle(); // 对准
	
	while (GameRun && !EMPTY_PERSONHP && !Monster.IsDead() && IsLoseHp(Monster, dwHp, dwTick))
	{
		if (CPersonFunction::GetInstance().IsGameLoad())
		{
			LOG_CF_D( L"打怪的时候出现读图!, 等待过图完毕后break!");
			CPersonFunction::GetInstance().WaitGameLoad();
			break;
		}
		if (!Move_To_Monster(Monster, FixPtInfo))
		{
			LOG_CF_D( L"卡刀时,无法移动到怪物旁边! ID=%X, Name=%s, HP=%d,Dis=%.2f", Monster.GetId(), Monster.GetName().c_str(), Monster.GetHp(), Monster.GetDis());
			return FALSE;
		}

		Monster.SetAngle(); // 对准

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_T2_Ui_D))
			CSomeFun::GetInstance().SimlationKey('T', 10);

		if (!bEnableZ)
		{
			if (CPerson::GetInstance().GetAbNormalStatus() == em_WarStatus::em_WarStatus_Hide)
				continue;
		}
		
		
		if (CPerson::GetInstance().GetMp() > 2 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_T2_Ui))
			//FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_T2_Ui);
			CSomeFun::GetInstance().SimlationKey('T', 10);

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab) && static_cast<DWORD>(::GetTickCount64() - dwTick_Q) >= 7 * 1000)
			//CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
			CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab_Suppress) && static_cast<DWORD>(::GetTickCount64() - dwTick_Q) >= 7 * 1000)
			//CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
			CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_V_2) && static_cast<DWORD>(::GetTickCount64() - dwTick_Q) >= 7 * 1000)
			//CSomeFun::GetInstance().SimlationKey('V', 10);
			CSomeFun::GetInstance().SimlationKey('V', 10);

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_V_3) && static_cast<DWORD>(::GetTickCount64() - dwTick_Q) >= 7 * 1000)
			//FarmSkill.UseSkill_By_SkillId(Sum_SkillId_V_3, em_Skill_Type_Cat);
			CSomeFun::GetInstance().SimlationKey('V', 10);

		if (!bEnableZ)
		{
			if (newcat.GetPercentHp() < 40 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Z) && CPerson::GetInstance().GetLevel() > 5 && CPerson::GetInstance().GetPercentHp() > 50)
			{
				//CSomeFun::GetInstance().SimlationKey('Z', 10);
				CSomeFun::GetInstance().SimlationKey('Z', 10);
				Sleep(500);
			}
		}
		else
		{
			if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() != -1 && CTeamMemberExtend::GetInstance().ExistTeamMemberWhenHpLessValue(80))
			{
				if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Z))
				{
					//CSomeFun::GetInstance().SimlationKey('Z', 10);
					CSomeFun::GetInstance().SimlationKey('Z', 10);
					Sleep(500);
				}
			}
		}

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_3))
			//CSomeFun::GetInstance().SimlationKey(VK_3, 10);
			CSomeFun::GetInstance().SimlationKey(VK_3, 10);

		if (CPerson::GetInstance().GetPercentHp() <= 80 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_X) && CPerson::GetInstance().GetLevel() > 21)
			CSomeFun::GetInstance().SimlationKey('X', 10);

		if (CPerson::GetInstance().GetPercentHp() <= 80 && FarmSkill.IsCool_By_SkillId_In_HideUi(Sum_SkillId_Q) && CPerson::GetInstance().GetLevel() > 15 && static_cast<DWORD>(::GetTickCount64() - dwTick_Q) >= 8 * 1000)
		{
				if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_4))
				{
					//FarmSkill.UseSkill_By_SkillId(Sum_SkillId_4);
					CSomeFun::GetInstance().SimlationKey(VK_4, 10);
					Sleep(2000);
				}
				if (CPerson::GetInstance().GetAbNormalStatus() == em_WarStatus::em_WarStatus_Hide)
				{
					CSomeFun::GetInstance().SimlationKey('Q', 10);
					dwTick_Q = ::GetTickCount64();
					if (Monster.GetDis() <= 5)
					{
						if (FarmSkill.IsCool_By_SkillId_In_HideUi(Sum_SkillId_SS))
						{
							CSomeFun::GetInstance().SetSpeed(3.0f);
							CSomeFun::GetInstance().SimlationKey('S', 500);
							CSomeFun::GetInstance().SetSpeed(3.0f);
						}
					}
					Sleep(4000);
				}
				else
				{
					CSomeFun::GetInstance().SimlationKey('Q', 10);
					//CSomeFun::GetInstance().SimlationKey('Q', 10);
					dwTick_Q = ::GetTickCount64();
					if (Monster.GetDis() <= 10)
					{
						Sleep(1000);
						if (FarmSkill.IsCool_By_SkillId_In_HideUi(Sum_SkillId_SS))
						{
							FarmSkill.UseSkill_By_SS();
							Sleep(3000);
						}
					}
				}
		}

		

		if (FarmSkill.IsCool_By_SkillId_In_HideUi(Sum_SkillId_Q) && CPerson::GetInstance().GetLevel() > 15)
		{
			CSomeFun::GetInstance().SimlationKey('Q', 10);
			dwTick_Q = ::GetTickCount64();
		}

		if (CPerson::GetInstance().GetPercentHp() <= 90 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_F))
			//FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_F);
			CSomeFun::GetInstance().SimlationKey('F', 10);

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_V) && static_cast<DWORD>(::GetTickCount64() - dwTick_Q) >= 6 * 1000)
			//CSkill::UseSkill_By_Id(Sum_SkillId_V, FALSE, em_Skill_Type_Cat);
			CSomeFun::GetInstance().SimlationKey('V', 10);

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_F_F) && CPerson::GetInstance().GetLevel() > 11)
			//CSomeFun::GetInstance().SimlationKey('F', 10);
			CSomeFun::GetInstance().SimlationKey('F', 10);

		if (CPerson::GetInstance().GetMp() >= 5 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_1) && !CObjectFunction::GetInstance().ExistMonsterBuff(Monster.GetId(), Sumoner_BuffId_1))
			//CSomeFun::GetInstance().SimlationKey(VK_1, 10);
			CSomeFun::GetInstance().SimlationKey(VK_1, 10);

		if (CPerson::GetInstance().GetMp() >= 5 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_2))
			//CSkill::UseSkill_By_Id(Sum_SkillId_2, FALSE);
			CSomeFun::GetInstance().SimlationKey(VK_2, 10);

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_C) && static_cast<DWORD>(::GetTickCount64() - dwTick_Q) >= 7 * 1000)
			//CSkill::UseSkill_By_Id(Sum_SkillId_C, FALSE, em_Skill_Type_Cat);
			CSomeFun::GetInstance().SimlationKey('C', 10);

		if (CPerson::GetInstance().GetMp() > 5)
			CSomeFun::GetInstance().SimlationKey('T', 10);
		else
		{
			CSomeFun::GetInstance().SimlationKey('R', 10);
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_T_Floor))
				//FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_T_Floor);
				CSomeFun::GetInstance().SimlationKey('F', 10);
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_T_Floor2))
			{
				//FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_T_Floor2);
				CSomeFun::GetInstance().SimlationKey('T', 10);
				CSomeFun::GetInstance().SimlationKey('F', 10);
			}
				

		}
		Sleep(100);
	}

	return CGameVariable::GetInstance().Action_Variable_By_Name(VARIABLE_NAME_AUTOTAKEITEM, TRUE, [&FixPtInfo]{CResAction::GetInstance().TakeDropItem(FixPtInfo); });
}

BOOL CFarmMonster::Summoner_UseSkill(_In_ CONST CMonster&) CONST
{
	// return TRUE就表示使用技能了, 不会去执行卡刀处理
	// return FALSE表示没使用技能, 会去执行卡刀处理
	return TRUE;
}


BOOL CFarmMonster::Summoner_Round(_In_ CONST CMonster& Monster, _In_ CONST CCat& newcat) CONST
{
	static ULONGLONG dwTick = NULL;
	static ULONGLONG dwTick_Q = NULL;
	static ULONGLONG dwTick_1 = NULL;
	auto& FarmSkill = CFarmSkill::GetInstance();

	LOG_CF_D(L"Check1");
	if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_T2_Ui_D))
	{
		LOG_CF_D( L"执行大向日葵");
		CSomeFun::GetInstance().SimlationKey('T', 10);
		return TRUE;
	}

	LOG_CF_D(L"Check2");
	if (CPerson::GetInstance().GetMp() > 2 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_T2_Ui))
	{
		LOG_CF_D( L"执行大向日葵");
		//FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_T2_Ui, FALSE);
		CSomeFun::GetInstance().SimlationKey('T', 10);
		return TRUE;
	}
	else if (static_cast<DWORD>(::GetTickCount64() - dwTick_Q) >= 7 * 1000 && !newcat.IsDead() && (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab) || FarmSkill.IsCool_By_SkillId_In_HideUi(Sum_SkillId_Tab_Suppress)))
	{
		CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
	}

	LOG_CF_D(L"Check3");
	if (CPerson::GetInstance().GetPercentHp() <= 88 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_3) && !CPersonFunction::GetInstance().Exist_Sumoner_3_Buff())
	{
		LOG_CF_D( L"执行花粉...............");
		//FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_3, FALSE);
		CSomeFun::GetInstance().SimlationKey(VK_3, 10);
		return TRUE;
	}
	else if (CPerson::GetInstance().GetPercentHp() <= 80 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_X) && CPerson::GetInstance().GetLevel() > 21 && !newcat.IsDead())
	{
		LOG_CF_D( L"执行友情...............");
		//FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_X, FALSE);
		CSomeFun::GetInstance().SimlationKey('X', 10);
		return TRUE;
	}
	else if ((CPerson::GetInstance().GetPercentHp() <= 80 || newcat.GetPercentHp() < 50) && FarmSkill.IsCool_By_SkillId_In_HideUi(Sum_SkillId_Q) && CPerson::GetInstance().GetLevel() > 15 && static_cast<DWORD>(::GetTickCount64() - dwTick_Q) >= 8 * 1000 && !newcat.IsDead())
	{
		if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_4))
		{
			LOG_CF_D( L"执行蒲公英....");
			//FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_4, FALSE);
			CSomeFun::GetInstance().SimlationKey(VK_4, 10);
			BnsSleep(2 * 1000);
			return TRUE;
		}
		LOG_CF_D( L"执行猥琐....");
		CSomeFun::GetInstance().SimlationKey('Q', 10);
		//CSomeFun::GetInstance().SimlationKey('Q', 10);
		dwTick_Q = ::GetTickCount64();
	}

	LOG_CF_D(L"Check4");
	if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_T_Floor))
	{
		//FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_T_Floor, FALSE);
		CSomeFun::GetInstance().SimlationKey('T', 10);
		CSomeFun::GetInstance().SimlationKey('F', 10);
		return TRUE;
	}
	if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_T_Floor2))
	{
		//FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_T_Floor2, FALSE);
		CSomeFun::GetInstance().SimlationKey('T', 10);
		CSomeFun::GetInstance().SimlationKey('F', 10);
		return TRUE;
	}
	else if (CPerson::GetInstance().GetPercentHp() <= 80 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_F))
	{
		LOG_CF_D( L"执行牵牛花-血量50");
		//FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_F, FALSE);
		CSomeFun::GetInstance().SimlationKey('F', 10);
		return TRUE;
	}

	LOG_CF_D(L"Check5");
	if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_V) && static_cast<DWORD>(::GetTickCount64() - dwTick_Q) >= 6 * 1000)
	{
		LOG_CF_D( L"执行下段斩");
		//CSomeFun::GetInstance().SimlationKey('V', 10);
		CSomeFun::GetInstance().SimlationKey('V', 10);
	}
	if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_C) && static_cast<DWORD>(::GetTickCount64() - dwTick_Q) >= 7 * 1000)
	{
		LOG_CF_D( L"执行大榔头");
		//CSomeFun::GetInstance().SimlationKey('C', 10);
		CSomeFun::GetInstance().SimlationKey('C', 10);
	}

	if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_F_F) && CPerson::GetInstance().GetLevel() > 11)
	{
		LOG_CF_D( L"执行板栗球。。。");
		//FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_F_F, FALSE);
		CSomeFun::GetInstance().SimlationKey('F', 10);
		return TRUE;
	}
	if (CPerson::GetInstance().GetMp() >= 5 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_1) && !CObjectFunction::GetInstance().ExistMonsterBuff(Monster.GetId(), Sumoner_BuffId_1))
	{
		LOG_CF_D( L"执行常春藤");
		//FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_1, FALSE);
		CSomeFun::GetInstance().SimlationKey(VK_1, 10);
		return TRUE;
	}

	if (CPerson::GetInstance().GetMp() >= 5 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_2))
	{
		LOG_CF_D( L"执行荆刺藤");
		//FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_2, FALSE);
		CSomeFun::GetInstance().SimlationKey(VK_2, 10);
		return TRUE;
	}

	
	LOG_CF_D(L"Check6");
	if (CPerson::GetInstance().GetMp() > 5)
	{
		LOG_CF_D( L"执行马蜂");
		//FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_T, FALSE);
		CSomeFun::GetInstance().SimlationKey('T', 10);
	}
	else
	{
		LOG_CF_D( L"执行玫瑰");
		//CSkill::UseSkill_By_Id(Sum_SkillId_R2, FALSE);
		//Sleep(50);
		//CSkill::UseSkill_By_Id(Sum_SkillId_R, FALSE);
		CSomeFun::GetInstance().SimlationKey('R', 10);
	}

	return TRUE;
}

BOOL CFarmMonster::Summoner_KillDog(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST
{
	auto dwTick = ::GetTickCount64();
	auto dwHp = Monster.GetHp();
	auto& FarmSkill = CFarmSkill::GetInstance();

	CCat newcat;
	CCatExpend::GetInstance().FindSelfCat(&newcat);

	CPersonFunction::GetInstance().CancelPutDownStatus();

	Monster.SetAngle(); // 对准
	CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);

	CONST static std::vector<DWORD> vSkill = {
		Sum_SkillId_F, Sum_SkillId_T2_Ui, Sum_SkillId_F_F, Sum_SkillId_1, Sum_SkillId_2,
		Sum_SkillId_C, Sum_SkillId_V
	};

	while (GameRun && !EMPTY_PERSONHP && !Monster.IsDead() && IsLoseHp(Monster, dwHp, dwTick))
	{
		// 距离不足的跑近
		if (Monster.GetDis(FixPtInfo.FixPoint) > FixPtInfo.fDis)
		{
			LOG_CF_D( L"杀怪时,无法移动到怪物旁边! ID=%X, Name=%s, HP=%d,Dis=%.2f", Monster.GetId(), Monster.GetName().c_str(), Monster.GetHp(), Monster.GetDis());
			return FALSE;
		}
		else if (Monster.GetDis() > 15.0f) // 气功姿势
		{
			CPersonFunction::GetInstance().Move(Monster.GetPoint());
			BnsSleep(300);
			continue;
		}
		else if (Monster.GetDis() < 15.0f && CPerson::GetInstance().IsMoving())
		{
			CPersonFunction::GetInstance().Move(CBaseObject::GetPersonPoint());
			BnsSleep(300);
			continue;
		}

		auto pSkill = MyTools::CLPublic::Vec_find_if_Const(vSkill, [&FarmSkill](CONST DWORD& dwSkillId){
			return FarmSkill.IsCool_By_SkillId_In_Ui(dwSkillId);
		});

		Monster.SetAngle();

		if (pSkill != nullptr)
			FarmSkill.UseSkill_By_SkillId(*pSkill);

		

		if (CPerson::GetInstance().GetMp() > 2)
		{
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_T2_Ui))
			{
				LOG_CF_D( L"执行大向日葵");
				//FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_T2_Ui);
				CSomeFun::GetInstance().SimlationKey('T', 10);
			}
		}

		if (CPerson::GetInstance().GetMp() > 5)
		{
			LOG_CF_D( L"执行马蜂");
			CSomeFun::GetInstance().SimlationKey('T', 10);
		}
		else
		{
			LOG_CF_D( L"执行玫瑰");
			CSomeFun::GetInstance().SimlationKey('R', 10);
			CSomeFun::GetInstance().SimlationKey('T', 10);
			CSomeFun::GetInstance().SimlationKey('F', 10);

		}
	}

	return TRUE;
}

BOOL CFarmMonster::Pk_Summoner(_In_ CONST CPlayer& Player, _In_ CONST FixPointInfo& Round, _In_ std::function<BOOL(VOID)> fnTerminate) CONST
{
	CMonster Monster(Player);
	auto& FarmSkill = CFarmSkill::GetInstance();
	CSomeFun::GetInstance().SetSpeed(3.0f);
	BOOL bRunThread = TRUE;

	std::vector<Point> vRound;
	GetRoundPointList(Round, 6, vRound);

	std::thread t([&FarmSkill, &bRunThread, &vRound, &Monster]
	{
		CONST static  std::vector<DWORD> vlst = {
			Sum_SkillId_Floor_2, Sum_SkillId_Floor_1, Sum_SkillId_Floor_F
		};

		UINT nIndex = 0;
		while (bRunThread && GameRun)
		{
			nIndex = nIndex + 1 >= vRound.size() ? 0 : nIndex + 1;
			auto& NextPoint = vRound.at(nIndex);
			if (Monster.GetDis(NextPoint) < 8 || Monster.GetDis(NextPoint) > 16)
			{
				BnsSleep(100);
				continue;
			}

			UINT uCount = 0;
			while (CBaseObject::GetPersonDis(NextPoint) > 3.0f && GameRun && !EMPTY_PERSONHP && bRunThread && !Monster.IsDead())
			{
				BnsSleep(100);
				Monster.SetAngle();

				if (uCount++ % 3 == 0)
				{
					if (CBaseObject::GetPersonDis(NextPoint) > 100.0f)
					{
						bRunThread = FALSE;
						break;
					}
					CPersonFunction::GetInstance().Move(NextPoint);
				}

				for (CONST auto& itm : vlst)
				{
					if (FarmSkill.IsCool_By_SkillId_In_Ui(itm))
					{
						FarmSkill.UseSkill_BySkillId_In_Sleep(itm);
						break;
					}
				}
			}
		}
	});

	// 敌人猫或者人 靠近的时候用4和3

	CCat Cat;
	if (!CCatExpend::GetInstance().FindSelfCat(&Cat))
	{
		LOG_CF_D( L"找不到猫！ 等死吧!");
		while (GameRun && !EMPTY_PERSONHP && !Monster.IsDead() && !fnTerminate() && Monster.GetDis() < 100 && BnsSleep(1000));
	}

	ULONGLONG ulTick = NULL;
	while (GameRun && !EMPTY_PERSONHP && !Monster.IsDead() && !fnTerminate() && Monster.GetDis() < 100)
	{
		CONST static std::vector<DWORD> SkillVec =
		{
			Sum_SkillId_T_Floor, Sum_SkillId_T_Floor2, Sum_SkillId_F, Sum_SkillId_F_F,
			Sum_SkillId_1, Sum_SkillId_T2_Ui
		};

		Monster.SetAngle();
		if (!Cat.IsDead())
		{
			if (CPerson::GetInstance().GetPercentHp() <= 80 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_X))
			{
				LOG_CF_D( L"友情!");
				FarmSkill.UseSkill_By_SkillId(Sum_SkillId_X);
			}

			if (static_cast<DWORD>(::GetTickCount64() - ulTick) >= 8 * 1000 && (FarmSkill.IsCool_By_SkillId_In_HideUi(Sum_SkillId_Tab_Suppress) || FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab)))
				CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);

			if (Monster.GetDis(Cat.GetPoint()) < 3.0f && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_V))
				CSomeFun::GetInstance().SimlationKey('V', 10);
			else if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_C))
				CSomeFun::GetInstance().SimlationKey('C', 10);
			//else if (FarmSkill.IsCool_By_SkillId_In_HideUi(Sum_SkillId_E))t
			//	FarmSkill.UseSkill_By_SkillId(Sum_SkillId_E);

		}

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_3) && !CPersonFunction::GetInstance().Exist_Sumoner_3_Buff())
		{
			LOG_CF_D( L"放花粉!!!!");
			FarmSkill.UseSkill_By_SkillId(Sum_SkillId_3);
		}

		if (CPerson::GetInstance().GetPercentHp() <= 70 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_4))
		{
			LOG_CF_D( L"放蒲公英!!!!");
			FarmSkill.UseSkill_By_SkillId(Sum_SkillId_4);
		}

		CONST auto p = MyTools::CLPublic::Vec_find_if_Const(SkillVec, [&FarmSkill](CONST DWORD itm){ return FarmSkill.IsCool_By_SkillId_In_Ui(itm); });
		if (p != nullptr && CPerson::GetInstance().GetMp() >= 2)
			FarmSkill.UseSkill_By_SkillId(*p);

		if (CPerson::GetInstance().GetMp() >= 4)
		{
			CSkill::UseSkill_By_Id(Sum_SkillId_T, FALSE);
			CSomeFun::GetInstance().SimlationKey('T', 10);
		}
		else
		{
			CSomeFun::GetInstance().SimlationKey('R', 10);
		}
			//CSomeFun::GetInstance().SimlationKey('R', 10);
	}
	bRunThread = FALSE;
	t.join();
	LOG_CF_D( L"PK完毕!");
	return TRUE;
}

BOOL CFarmMonster::BaiXianPratice_Summoner(_In_ CONST CPlayer& Player, _In_ CONST FixPointInfo& Round, _In_ std::function<BOOL(VOID)> fnTerminate) CONST
{
	CMonster Monster(Player);
	auto& FarmSkill = CFarmSkill::GetInstance();
	BOOL bRunThread = TRUE;
	ULONGLONG ulTick = NULL;

	std::vector<Point> vRound;
	GetRoundPointList(Round, 6, vRound);

	CCat Cat;
	if (!CCatExpend::GetInstance().FindSelfCat(&Cat))
	{
		LOG_CF_D( L"找不到猫！ 等死吧!");
		while (GameRun && !EMPTY_PERSONHP && !Monster.IsDead() && !fnTerminate() && Monster.GetDis() < 100 && BnsSleep(1000));
	}

	std::thread t([&FarmSkill, &bRunThread, &vRound, &Monster, &Cat, &ulTick]
	{
		CONST static  std::vector<DWORD> vlst = {
			Sum_SkillId_Floor_F, Sum_SkillId_Floor_2, Sum_SkillId_Floor_1, Sum_SkillId_X
		};
		CONST static std::vector<DWORD> CatVec = {
			Sum_SkillId_V_2, Sum_SkillId_V_3, Sum_SkillId_Floor_C, Sum_SkillId_X_2
		};

		while (bRunThread && GameRun)
		{
			for (CONST auto& itm : vlst)
			{
				if (FarmSkill.IsCool_By_SkillId_In_Ui(itm))
				{
					if (itm == Sum_SkillId_X && CPerson::GetInstance().GetPercentHp() >= 90)
						continue;

					FarmSkill.UseSkill_BySkillId_In_Sleep(itm, FALSE);
					break;
				}
			}

			for (CONST auto& itm : CatVec)
			{
				if (FarmSkill.IsCool_By_SkillId_In_Ui(itm) )
				{
					FarmSkill.UseSkill_BySkillId_In_Sleep(itm, FALSE, em_Skill_Type::em_Skill_Type_Cat);
					break;
				}
			}
			
			Monster.SetAngle();
			BnsSleep(100);
			Monster.SetAngle();
		}
	});

	// 敌人猫或者人 靠近的时候用4和3
	auto fnGetSafePoint = [&Monster, &vRound]
	{
		return MyTools::CLPublic::Vec_find_if(vRound, [&Monster](CONST Point& RoundPoint){
			return Monster.GetDis(RoundPoint) > 8 && Monster.GetDis(RoundPoint) < 16;
		});

	};

	auto fnUseT = [&FarmSkill,&Monster]
	{
		CONST static std::vector<DWORD> TVec = {
			Sum_SkillId_T, 0x66C3, 0x66C5
		};
		auto p = MyTools::CLPublic::Vec_find_if_Const(TVec, [&FarmSkill](DWORD dwSkillId){
			return FarmSkill.IsCool_By_SkillId_In_Ui(dwSkillId);
		});
		if (p != nullptr)
		{
			Monster.SetAngle();
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(1000, [&FarmSkill, &p]{return FarmSkill.UseSkill_By_SkillId(*p); });
			BnsSleep(500);
			CPersonFunction::GetInstance().WaitForProgressBarBySkill(1500);
		}
	};
	
	em_Player_Classes EnemyClasses = Monster.GetClasses();

	CSomeFun::GetInstance().SetSpeed(3.0f);
	while (GameRun && !EMPTY_PERSONHP && !Monster.IsDead() && !fnTerminate() && Monster.GetDis() < 100)
	{
		CONST static std::vector<DWORD> SkillVec =
		{
			Sum_SkillId_T_Floor, Sum_SkillId_T_Floor2, Sum_SkillId_F, Sum_SkillId_F_F,
			Sum_SkillId_1, Sum_SkillId_T2_Ui, Sum_SkillId_3,
		};

		Monster.SetAngle();
		if (EnemyClasses != em_Player_Classes::em_Player_Classes_Destroyer)
		{
			if(Monster.GetDis() > 15)
				BnsMove(Monster.GetFrontPoint(12), 3);
			else if (Monster.GetDis() <= 5.0f)
			{
				if (FarmSkill.IsCool_By_SkillId_In_HideUi(Sum_SkillId_SS))
					FarmSkill.UseSkill_By_SkillId(Sum_SkillId_SS);
				else if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_4) && Monster.GetAbNormalStatus() != em_WarStatus::em_WarStatus_Floor \
					&& !CPersonFunction::GetInstance().Exist_Sumoner_X_Buff() && !CObjectFunction::GetInstance().ExistMonsterBuff(Monster.GetId(), Sumoner_BuffId_1))
				{
					FarmSkill.UseSkill_By_SkillId(Sum_SkillId_4);
					CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2 * 1000, []{return CPerson::GetInstance().GetAbNormalStatus() != em_WarStatus::em_WarStatus_Hide; });
					if (CPerson::GetInstance().GetAbNormalStatus() == em_WarStatus::em_WarStatus_Hide)
					{
						auto p = fnGetSafePoint();
						if (p != nullptr)
						{
							LOG_CF_D( L"隐形状态下走到安全坐标,离人物距离%.2f,怪物距离%.2f!", CBaseObject::GetPersonDis(*p), Monster.GetDis(*p));
							while (GameRun && !EMPTY_PERSONHP && CPerson::GetInstance().GetAbNormalStatus() == em_WarStatus::em_WarStatus_Hide && CBaseObject::GetPersonDis(*p) > 3)
							{
								if (!CPerson::GetInstance().IsMoving())
								{
									CBaseObject::SetAngle(*p);
									CPersonFunction::GetInstance().Move(*p);
								}
								BnsSleep(300);
							}
						}

						if (CPerson::GetInstance().GetAbNormalStatus() == em_WarStatus::em_WarStatus_Hide)
						{
							fnUseT();
						}
						
					}
				}
			}
		}

		if (!Cat.IsDead())
			CSomeFun::GetInstance().SimlationKey(VK_TAB);

		if (Monster.GetAbNormalStatus() != em_WarStatus::em_WarStatus_Floor && !CObjectFunction::GetInstance().ExistMonsterBuff(Monster.GetId(), Sumoner_BuffId_1))
		{
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_1))
				FarmSkill.UseSkill_By_SkillId(Sum_SkillId_1);
			else if (Monster.GetDis(Cat.GetPoint()) < 10.0f && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_2))
				FarmSkill.UseSkill_By_SkillId(Sum_SkillId_2);
			else if (Monster.GetDis(Cat.GetPoint()) < 3.0f && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_V))
				CSomeFun::GetInstance().SimlationKey('V', 10);
			else if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_C))
				CSomeFun::GetInstance().SimlationKey('C', 10);
		}

		CONST auto p = MyTools::CLPublic::Vec_find_if_Const(SkillVec, [&FarmSkill](CONST DWORD itm){ return FarmSkill.IsCool_By_SkillId_In_Ui(itm); });
		if (p != nullptr && CPerson::GetInstance().GetMp() >= 2)
			FarmSkill.UseSkill_By_SkillId(*p);

		fnUseT();

		Monster.SetAngle();
		CSomeFun::GetInstance().SimlationKey('R', 10);
	}
	bRunThread = FALSE;
	t.join();
	LOG_CF_D( L"PK完毕!");
	return TRUE;
}

BOOL CFarmMonster::CallBackCat() CONST
{
	CCat Cat;
	if (!CCatExpend::GetInstance().FindSelfCat(&Cat) || Cat.IsDead())
		return FALSE;
	
	// 至少要执行一次E
	CSomeFun::GetInstance().TimeOut_Condiction_GameRun(4 * 1000, [Cat]
	{
		CSomeFun::GetInstance().SimlationKey('E');
		BnsSleep(500);
		return !EMPTY_PERSONHP && !Cat.IsDead() && CFarmSkill::GetInstance().IsCool_By_SkillId_In_HideUi(Sum_SkillId_E);
	});

	return TRUE;
}

BOOL CFarmMonster::CallBackCat(_In_ BOOL bRun) CONST
{
	static std::thread CallBackCatThread;
	static bool bRunThread = false;


	if (!bRun)
	{
		if (CallBackCatThread.joinable())
		{
			LOG_CF_D( L"召回召唤兽线程正在结束……");
			bRunThread = false;
			CallBackCatThread.join();
			CallBackCatThread = std::thread();
			LOG_CF_D( L"召回召唤兽线程完毕……");
		}
		else
			LOG_CF_D( L"召回召唤兽线程已经被结束了!");
		return TRUE;
	}


	if (CallBackCatThread.joinable())
	{
		LOG_CF_D( L"召回召唤兽线程已经被创建了, 必须先结束才能再开启新的线程!");
		return FALSE;
	}


	bRunThread = true;
	CallBackCatThread = std::thread([]
	{
		CCat Cat;
		while (GameRun && !EMPTY_PERSONHP && bRunThread && CCatExpend::GetInstance().FindSelfCat(&Cat) && !Cat.IsDead())
		{
			CPersonFunction::GetInstance().WaitGameLoad();


			if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_E))
				CSomeFun::GetInstance().SimlationKey('E');

			BnsSleep(300);
		}
		LOG_CF_D( L"退出召回召唤兽线程!");
	});
	return TRUE;
}

VOID CFarmMonster::CallBackCat_Time(_In_ DWORD dwTime) CONST
{
	CCat Cat;
	if (!CCatExpend::GetInstance().FindSelfCat(&Cat) || Cat.IsDead())
		return;

	CSomeFun::GetInstance().TimeOut_Condiction_GameRun(dwTime, []
	{
		if (!CPersonFunction::GetInstance().IsGameLoad() && CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_E))
			CSomeFun::GetInstance().SimlationKey('E');
		BnsSleep(300);
		return TRUE;
	});
}

BOOL CFarmMonster::XueYuGong_IceFireBird(_In_ CONST CMonster& Monster, _In_ CONST CCat& newcat) CONST
{
	static ULONGLONG dwTick = NULL;
	static ULONGLONG dwTick_Q = NULL;
	static ULONGLONG dwTick_1 = NULL;
	auto& FarmSkill = CFarmSkill::GetInstance();

	Monster.SetAngle(); // 对准
	if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_T2_Ui_D))
	{
		//LOG_CF_D( L"执行大向日葵");
		CSomeFun::GetInstance().SimlationKey('T', 10);
		return TRUE;
	}

	if (CPerson::GetInstance().GetMp() > 2 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_T2_Ui))
	{
		//LOG_CF_D( L"执行大向日葵");
		FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_T2_Ui, FALSE);
		return TRUE;
	}
	else if (static_cast<DWORD>(::GetTickCount64() - dwTick_Q) >= 7 * 1000 && !newcat.IsDead() && (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_Tab) || FarmSkill.IsCool_By_SkillId_In_HideUi(Sum_SkillId_Tab_Suppress)))
	{
		CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
	}

	if (CPerson::GetInstance().GetPercentHp() <= 88 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_3) && !CPersonFunction::GetInstance().Exist_Sumoner_3_Buff())
	{
		//LOG_CF_D( L"执行花粉...............");
		FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_3, FALSE);
		return TRUE;
	}
	else if (CPerson::GetInstance().GetPercentHp() <= 80 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_X) && CPerson::GetInstance().GetLevel() > 21 && !newcat.IsDead())
	{
		//LOG_CF_D( L"执行友情...............");
		FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_X, FALSE);
		return TRUE;
	}
	else if ((CPerson::GetInstance().GetPercentHp() <= 80 || newcat.GetPercentHp() < 50) && FarmSkill.IsCool_By_SkillId_In_HideUi(Sum_SkillId_Q) && CPerson::GetInstance().GetLevel() > 15 && static_cast<DWORD>(::GetTickCount64() - dwTick_Q) >= 8 * 1000 && !newcat.IsDead())
	{
		if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_4))
		{
			//LOG_CF_D( L"执行蒲公英....");
			FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_4, FALSE);
			BnsSleep(2 * 1000);
			return TRUE;
		}
		//LOG_CF_D( L"执行猥琐....");
		CSomeFun::GetInstance().SimlationKey('Q', 10);
		//CSomeFun::GetInstance().SimlationKey('Q', 10);
		dwTick_Q = ::GetTickCount64();
	}

	if (CTeamMemberExtend::GetInstance().GetTeamMember_By_LessPercentHp(70, nullptr))
	{
		LOG_CF_D( L"圆形走位有队员70以下血了，加血。");
		CSomeFun::GetInstance().SimlationKey('Z', 10);
	}

	if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_T_Floor))
	{
		FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_T_Floor, FALSE);
		return TRUE;
	}
	if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_T_Floor2))
	{
		FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_T_Floor2, FALSE);
		return TRUE;
	}

	else if (CPerson::GetInstance().GetPercentHp() <= 80 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_F))
	{
		//LOG_CF_D( L"执行牵牛花-血量50");
		FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_F, FALSE);
		return TRUE;
	}

	if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_V) && static_cast<DWORD>(::GetTickCount64() - dwTick_Q) >= 6 * 1000)
	{
		//LOG_CF_D( L"执行下段斩");
		CSomeFun::GetInstance().SimlationKey('V', 10);
	}
	if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_C) && static_cast<DWORD>(::GetTickCount64() - dwTick_Q) >= 7 * 1000)
	{
		//LOG_CF_D( L"执行大榔头");
		CSomeFun::GetInstance().SimlationKey('C', 10);
	}

	if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_F_F) && CPerson::GetInstance().GetLevel() > 11)
	{
		//LOG_CF_D( L"执行板栗球。。。");
		FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_F_F, FALSE);
		return TRUE;
	}
	if (CPerson::GetInstance().GetMp() >= 5 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_1) && !CObjectFunction::GetInstance().ExistMonsterBuff(Monster.GetId(), Sumoner_BuffId_1))
	{
		//LOG_CF_D( L"执行常春藤");
		FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_1, FALSE);
		return TRUE;
	}

	if (CPerson::GetInstance().GetMp() >= 5 && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_2))
	{
		//LOG_CF_D( L"执行荆刺藤");
		FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_2, FALSE);
		return TRUE;
	}



	if (CPerson::GetInstance().GetMp() > 5)
	{
		//LOG_CF_D( L"执行马蜂");
		//FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_T, FALSE);
		CSomeFun::GetInstance().SimlationKey('T', 10);
	}
	else
	{
		//LOG_CF_D( L"执行玫瑰");
		CSkill::UseSkill_By_Id(Sum_SkillId_R2, FALSE);
		Sleep(50);
		CSkill::UseSkill_By_Id(Sum_SkillId_R, FALSE);
		//CSomeFun::GetInstance().SimlationKey('R', 10);
	}

	return TRUE;
}

BOOL CFarmMonster::Warlock_Singe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST
{
	LOG_CF_D( L"Warlock_Singe, Name=%s, Dis=%.2f", Monster.GetName().c_str(), Monster.GetDis());
	auto dwTick = ::GetTickCount64();
	auto dwHp = Monster.GetHp();
	auto& FarmSkill = CFarmSkill::GetInstance();

	CPersonFunction::GetInstance().CancelPutDownStatus();
	Monster.SetAngle(); // 对准
	if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_1_F))
	{
		if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_1))
		{
			//CSomeFun::GetInstance().SimlationKey(VK_1, 10);
			CSomeFun::GetInstance().SimlationKey(VK_1, 10);
			Sleep(500);
			//FarmSkill.UseSkill_BySkillId_In_Sleep(Warlock_SkillId_1_F);
			CSomeFun::GetInstance().SimlationKey('F' , 10);
		}
	}
	while (GameRun && !EMPTY_PERSONHP && !Monster.IsDead() && IsLoseHp(Monster, dwHp, dwTick))
	{
		if (CPersonFunction::GetInstance().IsGameLoad())
		{
			LOG_CF_D( L"打怪的时候出现读图!, 等待过图完毕后break!");
			CPersonFunction::GetInstance().WaitGameLoad();
			break;
		}
		if (CPerson::GetInstance().GetDis(Monster.GetFacePoint()) > 10.0f)
		{
			if (!Move_To_Monster(Monster, FixPtInfo, TRUE))
			{
				LOG_CF_D( L"卡刀时,无法移动到怪物旁边! ID=%X, Name=%s, HP=%d,Dis=%.2f", Monster.GetId(), Monster.GetName().c_str(), Monster.GetHp(), Monster.GetDis());
				return FALSE;
			}
		}
		Monster.SetAngle(); // 对准

		//1B77403 超神BUFF
		if (CPersonFunction::GetInstance().ExistBuff_Person(0x1B77403, nullptr) && GameRun)
		{
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_God_4))
			{
				if (FarmSkill.IsActive_By_SkillId_In_Ui(Warlock_SkillId_God_4))
				{
					//FarmSkill.UseSkill_BySkillId_In_Sleep(Warlock_SkillId_God_4);
					CSomeFun::GetInstance().SimlationKey(VK_4, 10);
				}
			}
			if (CPerson::GetInstance().GetMp() > 6)
				CSomeFun::GetInstance().SimlationKey('T', 10);
			else
				CSomeFun::GetInstance().SimlationKey('R', 10);
			continue;
		}
		

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_Tab))
		{
			//CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
			CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
			Sleep(800);
		}

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_Tab_God))
		{
			//FarmSkill.UseSkill_By_SkillId(Warlock_SkillId_Tab_God, em_Skill_Type_Cat);
			CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
			Sleep(800);
		}
		if (CPerson::GetInstance().GetLevel() > 17)
		{
			//1AB3F71 3个可以释放4
			//LOG_CF_D( L"4_1");
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_4_1))
			{
				if (FarmSkill.IsActive_By_SkillId_In_Ui(Warlock_SkillId_4_1))
				{
					//CSomeFun::GetInstance().SimlationKey(VK_4, 10);
					CSomeFun::GetInstance().SimlationKey(VK_4, 10);
				}
			}
			//LOG_CF_D( L"F");
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_F1_1) ||
				FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_F1_2) ||
				FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_F2))
			{
				//CSomeFun::GetInstance().SimlationKey('F', 10);
				CSomeFun::GetInstance().SimlationKey('F', 10);
			}

			//LOG_CF_D( L"2");
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_2))
			{
				if (FarmSkill.IsActive_By_SkillId_In_Ui(Warlock_SkillId_2))
				{
					//CSomeFun::GetInstance().SimlationKey(VK_2, 10);
					CSomeFun::GetInstance().SimlationKey(VK_2, 10);
				}
			}

			//LOG_CF_D( L"3");
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_3))
			{
				if (FarmSkill.IsActive_By_SkillId_In_Ui(Warlock_SkillId_3))
				{
					//CSomeFun::GetInstance().SimlationKey(VK_3, 10);
					CSomeFun::GetInstance().SimlationKey(VK_3, 10);
					Sleep(1500);
				}
			}

			//LOG_CF_D( L"V1");
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_V_2))
			{
				//CSomeFun::GetInstance().SimlationKey('V', 10);
				CSomeFun::GetInstance().SimlationKey('V', 10);
			}
			else if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_V_1))
			{
				//CSomeFun::GetInstance().SimlationKey('V', 10);
				CSomeFun::GetInstance().SimlationKey('V', 10);
				Sleep(800);
			}

			//LOG_CF_D( L"E");
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_E))
			{
				if (FarmSkill.IsActive_By_SkillId_In_Ui(Warlock_SkillId_E))
				{
					//FarmSkill.UseSkill_BySkillId_In_Sleep(Warlock_SkillId_E);
					CSomeFun::GetInstance().SimlationKey('E', 10);
				}
			}
			else if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_Q))
			{
				if (FarmSkill.IsActive_By_SkillId_In_Ui(Warlock_SkillId_Q))
				{
					//CSomeFun::GetInstance().SimlationKey('Q', 10);
					CSomeFun::GetInstance().SimlationKey('Q', 10);
				}
			}

			//LOG_CF_D( L"X");
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_X))
			{
				if (FarmSkill.IsActive_By_SkillId_In_Ui(Warlock_SkillId_X))
				{
					//CSomeFun::GetInstance().SimlationKey('X', 10);
					CSomeFun::GetInstance().SimlationKey('X', 10);
				}
			}

			//LOG_CF_D( L"Z");
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_Z_1) ||
				FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_Z_2))
			{
				//CSomeFun::GetInstance().SimlationKey('Z', 10);
				CSomeFun::GetInstance().SimlationKey('Z', 10);
			}
			
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_C))
			{
				//CSomeFun::GetInstance().SimlationKey('C', 10);
				CSomeFun::GetInstance().SimlationKey('C', 10);
				Sleep(300);
			}


		}
		else
		{
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_F1_1) ||
				FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_F1_2) ||
				FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_F2))
			{
				//CSomeFun::GetInstance().SimlationKey('F', 10);
				CSomeFun::GetInstance().SimlationKey('F', 10);
			}

			if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_Tab))
			{
				//CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
				CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
				Sleep(800);
			}
			else if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_4))
			{
				//FarmSkill.UseSkill_BySkillId_In_Sleep(Warlock_SkillId_4);
				CSomeFun::GetInstance().SimlationKey(VK_4, 10);
				Sleep(2500);
			}
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_E))
			{
				//FarmSkill.UseSkill_BySkillId_In_Sleep(Warlock_SkillId_E);
				CSomeFun::GetInstance().SimlationKey('E', 10);
			}
			else if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_Q))
			{
				//CSomeFun::GetInstance().SimlationKey('Q', 10);
				CSomeFun::GetInstance().SimlationKey('Q', 10);
			}
		}
		

		if (CPerson::GetInstance().GetLevel() > 17)
		{
			if (CPerson::GetInstance().GetMp() > 6)
				CSomeFun::GetInstance().SimlationKey('T', 10);
			else
				CSomeFun::GetInstance().SimlationKey('R', 10);
		}
		else
		{
			if (CPersonFunction::GetInstance().ExistBuff_Person(0x1ABC7BB, nullptr))//真言BUFF
			{
				CSomeFun::GetInstance().SimlationKey('T', 10);
			}
			CSomeFun::GetInstance().SimlationKey('R', 10);
		}
		
		Sleep(200);
	}

	return CGameVariable::GetInstance().Action_Variable_By_Name(VARIABLE_NAME_AUTOTAKEITEM, TRUE, [&FixPtInfo]{CResAction::GetInstance().TakeDropItem(FixPtInfo); });
}

BOOL CFarmMonster::Warlock_Aoe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST
{
	LOG_CF_D( L"Warlock_Singe, Name=%s, Dis=%.2f", Monster.GetName().c_str(), Monster.GetDis());
	auto dwTick = ::GetTickCount64();
	auto dwHp = Monster.GetHp();
	auto& FarmSkill = CFarmSkill::GetInstance();

	CPersonFunction::GetInstance().CancelPutDownStatus();
	Monster.SetAngle(); // 对准
	CSomeFun::GetInstance().SimlationKey('R', 10);
	if (FarmSkill.IsCool_By_SkillId_In_HideUi(Warlock_SkillId_1_F))
	{
		if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_1))
		{
			//CSomeFun::GetInstance().SimlationKey(VK_1, 10);
			CSomeFun::GetInstance().SimlationKey(VK_1, 10);
			Sleep(500);
			//FarmSkill.UseSkill_BySkillId_In_Sleep(Warlock_SkillId_1_F);
			CSomeFun::GetInstance().SimlationKey('F', 10);
		}
	}


	while (GameRun && !EMPTY_PERSONHP && !Monster.IsDead() && IsLoseHp(Monster, dwHp, dwTick))
	{
		if (CPersonFunction::GetInstance().IsGameLoad())
		{
			LOG_CF_D( L"打怪的时候出现读图!, 等待过图完毕后break!");
			CPersonFunction::GetInstance().WaitGameLoad();
			break;
		}
		
		if (CPerson::GetInstance().GetDis(Monster.GetFacePoint()) > 10.0f)
		{
			if (!Move_To_Monster(Monster, FixPtInfo, TRUE))
			{
				LOG_CF_D( L"卡刀时,无法移动到怪物旁边! ID=%X, Name=%s, HP=%d,Dis=%.2f", Monster.GetId(), Monster.GetName().c_str(), Monster.GetHp(), Monster.GetDis());
				return FALSE;
			}
		}
		

		Monster.SetAngle(); // 对准

		//1B77403 超神BUFF
		if (CPersonFunction::GetInstance().ExistBuff_Person(0x1B77403, nullptr) && GameRun)
		{
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_God_4))
			{
				if (FarmSkill.IsActive_By_SkillId_In_Ui(Warlock_SkillId_God_4))
				{
					//FarmSkill.UseSkill_BySkillId_In_Sleep(Warlock_SkillId_God_4);
					CSomeFun::GetInstance().SimlationKey(VK_4, 10);
				}
			}
			if (CPerson::GetInstance().GetMp() > 6)
				CSomeFun::GetInstance().SimlationKey('T', 10);
			else
				CSomeFun::GetInstance().SimlationKey('R', 10);
			continue;
		}

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_Tab))
		{
			//CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
			CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
			Sleep(800);
		}

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_Tab_God))
		{
			//FarmSkill.UseSkill_By_SkillId(Warlock_SkillId_Tab_God, em_Skill_Type_Cat);
			CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
			Sleep(800);
		}

		if (CPerson::GetInstance().GetLevel() > 17)
		{
			
			//1AB3F71 3个可以释放4
			//LOG_CF_D( L"4_1");
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_4_1))
			{
				if (FarmSkill.IsActive_By_SkillId_In_Ui(Warlock_SkillId_4_1))
				{
					//CSomeFun::GetInstance().SimlationKey(VK_4, 10);
					CSomeFun::GetInstance().SimlationKey(VK_4, 10);
				}
			}
			//LOG_CF_D( L"F");
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_F1_1) ||
				FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_F1_2) ||
				FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_F2))
			{
				//CSomeFun::GetInstance().SimlationKey('F', 10);
				CSomeFun::GetInstance().SimlationKey('F', 10);
			}


			//LOG_CF_D( L"2");
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_2))
			{
				if (FarmSkill.IsActive_By_SkillId_In_Ui(Warlock_SkillId_2))
				{
					//CSomeFun::GetInstance().SimlationKey(VK_2, 10);
					CSomeFun::GetInstance().SimlationKey(VK_2, 10);
				}
			}

			//LOG_CF_D( L"3");
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_3))
			{
				if (FarmSkill.IsActive_By_SkillId_In_Ui(Warlock_SkillId_3))
				{
					//CSomeFun::GetInstance().SimlationKey(VK_3, 10);
					CSomeFun::GetInstance().SimlationKey(VK_3, 10);
					Sleep(1500);
				}
			}

			//LOG_CF_D( L"V1");
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_V_2))
			{
				//CSomeFun::GetInstance().SimlationKey('V', 10);
				CSomeFun::GetInstance().SimlationKey('V', 10);
			}
			else if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_V_1))
			{
				//CSomeFun::GetInstance().SimlationKey('V', 10);
				CSomeFun::GetInstance().SimlationKey('V', 10);
				Sleep(800);
			}

			//LOG_CF_D( L"E");
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_E))
			{
				if (FarmSkill.IsActive_By_SkillId_In_Ui(Warlock_SkillId_E))
				{
					//FarmSkill.UseSkill_BySkillId_In_Sleep(Warlock_SkillId_E);
					CSomeFun::GetInstance().SimlationKey('E', 10);
				}
			}
			else if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_Q))
			{
				if (FarmSkill.IsActive_By_SkillId_In_Ui(Warlock_SkillId_Q))
				{
					//CSomeFun::GetInstance().SimlationKey('Q', 10);
					CSomeFun::GetInstance().SimlationKey('Q', 10);
				}
			}

			//LOG_CF_D( L"X");
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_X))
			{
				if (FarmSkill.IsActive_By_SkillId_In_Ui(Warlock_SkillId_X))
				{
					//CSomeFun::GetInstance().SimlationKey('X', 10);
					CSomeFun::GetInstance().SimlationKey('X', 10);
				}
			}

			//LOG_CF_D( L"Z");
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_Z_1) ||
				FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_Z_2))
			{
				//CSomeFun::GetInstance().SimlationKey('Z', 10);
				CSomeFun::GetInstance().SimlationKey('Z', 10);
			}
			
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_C))
			{
				//CSomeFun::GetInstance().SimlationKey('C', 10);
				CSomeFun::GetInstance().SimlationKey('C', 10);
				Sleep(300);
			}
		}
		else
		{
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_F1_1) ||
				FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_F1_2) ||
				FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_F2))
			{
				//CSomeFun::GetInstance().SimlationKey('F', 10);
				CSomeFun::GetInstance().SimlationKey('F', 10);
			}
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_Tab))
			{
				//CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
				CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
				Sleep(800);
			}
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_X))
			{
				//CSomeFun::GetInstance().SimlationKey('X', 10);
				CSomeFun::GetInstance().SimlationKey('X', 10);
			}

			if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_3))
			{
				//CSomeFun::GetInstance().SimlationKey(VK_2, 10);
				//CSomeFun::GetInstance().SimlationKey(VK_3, 10);
				CSomeFun::GetInstance().SimlationKey(VK_3, 10);
				Sleep(1500);
			}

			if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_4_1))
			{
				//CSomeFun::GetInstance().SimlationKey(VK_4, 10);
				CSomeFun::GetInstance().SimlationKey(VK_4, 10);
			}


			if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_E))
			{
				//FarmSkill.UseSkill_BySkillId_In_Sleep(Warlock_SkillId_E);
				CSomeFun::GetInstance().SimlationKey('E', 10);
			}
			else if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_Q))
			{
				//FarmSkill.UseSkill_BySkillId_In_Sleep(Warlock_SkillId_Q);
				CSomeFun::GetInstance().SimlationKey('Q', 10);
			}
		}
		if (CPerson::GetInstance().GetLevel() > 17)
		{
			if (CPerson::GetInstance().GetMp() > 6)
				CSomeFun::GetInstance().SimlationKey('T', 10);
			else
				CSomeFun::GetInstance().SimlationKey('R', 10);
		}
		else
		{
			if (CPersonFunction::GetInstance().ExistBuff_Person(0x1ABC7BB, nullptr))//真言BUFF
			{
				CSomeFun::GetInstance().SimlationKey('T', 10);
			}
			CSomeFun::GetInstance().SimlationKey('R', 10);
		}

		Sleep(200);
	}

	return CGameVariable::GetInstance().Action_Variable_By_Name(VARIABLE_NAME_AUTOTAKEITEM, TRUE, [&FixPtInfo]{CResAction::GetInstance().TakeDropItem(FixPtInfo); });
}

BOOL CFarmMonster::Warlock_Round(_In_ CONST CMonster& Monster, _In_ CONST CCat& newcaat) CONST
{
	auto& FarmSkill = CFarmSkill::GetInstance();

	CPersonFunction::GetInstance().CancelPutDownStatus();
	Monster.SetAngle(); // 对准

	//1B77403 超神BUFF
	if (CPersonFunction::GetInstance().ExistBuff_Person(0x1B77403, nullptr) && GameRun)
	{
		if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_God_4))
		{
			if (FarmSkill.IsActive_By_SkillId_In_Ui(Warlock_SkillId_God_4))
			{
				//FarmSkill.UseSkill_BySkillId_In_Sleep(Warlock_SkillId_God_4);
				CSomeFun::GetInstance().SimlationKey(VK_4, 10);
			}
		}
		if (CPerson::GetInstance().GetMp() > 6)
			CSomeFun::GetInstance().SimlationKey('T', 10);
		else
			CSomeFun::GetInstance().SimlationKey('R', 10);
		return TRUE;
	}

	if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_Tab_God))
	{
		//FarmSkill.UseSkill_By_SkillId(Warlock_SkillId_Tab_God, em_Skill_Type_Cat);
		CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
		Sleep(800);
	}

	if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_4_1))
	{
		if (FarmSkill.IsActive_By_SkillId_In_Ui(Warlock_SkillId_4_1))
		{
			//CSomeFun::GetInstance().SimlationKey(VK_4, 10);
			CSomeFun::GetInstance().SimlationKey(VK_4, 10);
		}
	}
	//LOG_CF_D( L"F");
	if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_F1_1) ||
		FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_F1_2) ||
		FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_F2))
	{
		//CSomeFun::GetInstance().SimlationKey('F', 10);
		CSomeFun::GetInstance().SimlationKey('F', 10);
	}


	//LOG_CF_D( L"2");
	if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_2))
	{
		if (FarmSkill.IsActive_By_SkillId_In_Ui(Warlock_SkillId_2))
		{
			//CSomeFun::GetInstance().SimlationKey(VK_2, 10);
			CSomeFun::GetInstance().SimlationKey(VK_2, 10);
		}
	}

	//LOG_CF_D( L"3");
	if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_3))
	{
		if (FarmSkill.IsActive_By_SkillId_In_Ui(Warlock_SkillId_3))
		{
			//CSomeFun::GetInstance().SimlationKey(VK_3, 10);
			CSomeFun::GetInstance().SimlationKey(VK_3, 10);
			Sleep(1500);
		}
	}

	//LOG_CF_D( L"V1");
	if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_V_2))
	{
		//CSomeFun::GetInstance().SimlationKey('V', 10);
		CSomeFun::GetInstance().SimlationKey('V', 10);
	}
	else if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_V_1))
	{
		//CSomeFun::GetInstance().SimlationKey('V', 10);
		CSomeFun::GetInstance().SimlationKey('V', 10);
		Sleep(800);
	}

	//LOG_CF_D( L"E");
	if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_E))
	{
		if (FarmSkill.IsActive_By_SkillId_In_Ui(Warlock_SkillId_E))
		{
			//FarmSkill.UseSkill_BySkillId_In_Sleep(Warlock_SkillId_E);
			CSomeFun::GetInstance().SimlationKey('E', 10);
		}
	}
	else if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_Q))
	{
		if (FarmSkill.IsActive_By_SkillId_In_Ui(Warlock_SkillId_Q))
		{
			//CSomeFun::GetInstance().SimlationKey('Q', 10);
			CSomeFun::GetInstance().SimlationKey('Q', 10);
		}
	}

	//LOG_CF_D( L"X");
	if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_X))
	{
		if (FarmSkill.IsActive_By_SkillId_In_Ui(Warlock_SkillId_X))
		{
			//CSomeFun::GetInstance().SimlationKey('X', 10);
			CSomeFun::GetInstance().SimlationKey('X', 10);
		}
	}

	//LOG_CF_D( L"Z");
	if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_Z_1) ||
		FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_Z_2))
	{
		//CSomeFun::GetInstance().SimlationKey('Z', 10);
		CSomeFun::GetInstance().SimlationKey('Z', 10);
	}
	
	if (FarmSkill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_C))
	{
		//CSomeFun::GetInstance().SimlationKey('C', 10);
		CSomeFun::GetInstance().SimlationKey('C', 10);
		Sleep(300);
	}

	if (CPerson::GetInstance().GetMp() > 6)
		CSomeFun::GetInstance().SimlationKey('T', 10);
	else
		CSomeFun::GetInstance().SimlationKey('R', 10);

	Sleep(200);
	return TRUE;
}

BOOL CFarmMonster::BladeMaster_Singe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&] 
	{
		LOG_CF_D(L"BladeMaster_Singe, Name=%s, Dis=%.2f", Monster.GetName().c_str(), Monster.GetDis());
		auto dwTick = ::GetTickCount64();
		auto dwHp = Monster.GetHp();
		static MyTools::CTimeTick TimeTickZ;

		DWORD dwMp = 0;
		CDailyTask_Skill FarmSkill;
		CPersonFunction::GetInstance().CancelPutDownStatus();
		Monster.SetAngle(); // 对准

		MyTools::CTimeTick TimeTickQ;
		while (GameRun && !EMPTY_PERSONHP && !Monster.IsDead() && IsLoseHp(Monster, dwHp, dwTick))
		{
			if (CPersonFunction::GetInstance().IsGameLoad())
			{
				LOG_CF_D(L"打怪的时候出现读图!, 等待过图完毕后break!");
				CPersonFunction::GetInstance().WaitGameLoad();
				break;
			}
			if (Monster.GetDis() > 3.0f)
			{
				if (!Move_To_Monster(Monster, FixPtInfo, TRUE))
				{
					LOG_CF_D(L"卡刀时,无法移动到怪物旁边! ID=%X, Name=%s, HP=%d,Dis=%.2f", Monster.GetId(), Monster.GetName().c_str(), Monster.GetHp(), Monster.GetDis());
					return FALSE;
				}
			}
			//07是拔剑姿态   02是正常姿态
			//em_Player_Pose emPersonPose = CPerson::GetInstance().GetFightPose();

			Monster.SetAngle(); // 对准

			dwMp = CPerson::GetInstance().GetMp();

			FarmSkill.RefreshVecSkill();
			if (Monster.GetDis() >= 7)
			{
				if (FarmSkill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_2))
				{
					//LOG_CF_D(L"突进靠近BOSS");
					FarmSkill.UseSkill_By_SkillId(BladeMaster_SkillId_2, FALSE);
					//return;
				}
				else if (FarmSkill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_2))
				{
					//LOG_CF_D(L"突袭靠近BOSS");
					FarmSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_2, FALSE);
					//return;
				}
			}

			if (CPerson::GetInstance().GetPercentHp() < 30)
			{
				if (FarmSkill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_E))
				{
					FarmSkill.UseSkill_By_SkillId(BladeMaster_SkillId_E, TRUE);
				}
				else if (FarmSkill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_E))
				{
					FarmSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_E, TRUE);
				}
			}
			else if (CPerson::GetInstance().GetPercentHp() < 80)
			{
				if (FarmSkill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Q))
				{
					FarmSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Q, TRUE);
				}
				else if (FarmSkill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_Q))
				{
					FarmSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_Q, TRUE);
				}
			}

			if (dwMp < 7 && FarmSkill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Tab))
			{
				//LOG_CF_D(L"满月斩转变成拔剑姿态");
				FarmSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Tab, FALSE);
			}
			else if (dwMp < 7 && FarmSkill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_Tab))
			{
				//LOG_CF_D(L"满月斩转变成拔剑姿态");
				FarmSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_Tab, FALSE);
			}
			else if (dwMp < 5 && FarmSkill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_2))
			{
				//LOG_CF_D(L"突进!");
				FarmSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_2, FALSE);
				//CSomeFun::GetInstance().SimlationKey(VK_2, 0);
				//return;
			}
			else if (dwMp <= 5 && FarmSkill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_21))
			{
				//LOG_CF_D(L"突进!");
				FarmSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_21, FALSE);
				//CSomeFun::GetInstance().SimlationKey(VK_2, 0);
				//return;
			}
			//仇恨BUFF-0x9D41EC

			if (CPerson::GetInstance().GetFightPose() == em_Player_Pose::em_Player_Pose_BladeMaster)
			{
				//LOG_CF_D(L"拔剑姿态输出中1%d", static_cast<DWORD>(::GetTickCount64() - nTick));
				if (FarmSkill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_R5))
				{
					//LOG_CF_D(L"快剑");
					FarmSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_R5, FALSE);
					//return;
				}
				else if (dwMp < 7 && TimeTickZ.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Second) >= 5 && FarmSkill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_Z)) // Z 和 C都是回蓝技能, 所以不能一起放！ 放了一个, 另外一个至少要10s后才能放!
				{
					//LOG_CF_D(L"山崩地裂");
					FarmSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_Z, FALSE);
					TimeTickZ.Reset();
					//return;
				}
				else if (dwMp < 7 && TimeTickZ.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Second) >= 5 && FarmSkill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_C))
				{
					//LOG_CF_D(L"风月斩");
					FarmSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_C, FALSE);
					TimeTickZ.Reset();
					//return;
				}
				else if (FarmSkill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_V_X))
				{
					//LOG_CF_D(L"御剑连斩-释放火龙连斩BUFF");
					FarmSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_V_X, FALSE);
					//return;
				}
				else if (FarmSkill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_V_V))
				{
					//LOG_CF_D(L"回天令");
					FarmSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_V_V, FALSE);
					//return;
				}
				else if (FarmSkill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_V_Z))
				{
					//LOG_CF_D(L"爆剑令");
					FarmSkill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_V_Z, FALSE);
					//return;
				}
				static CONST std::vector<DWORD> VecBuff =
				{
					0x9A2E08,0x9A2E0A,0x9A2E0B,
					0x9A2E0C,0x9A2E0D,0x9A2E0E,
					0x9A2E0F
				};
				if (CPersonFunction::GetInstance().ExistBuff_Person(VecBuff))
				{
					CSomeFun::GetInstance().SimlationKey_Immediately('T');
				}
				else if (FarmSkill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_3_R))
				{
					CSomeFun::GetInstance().SimlationKey_Immediately('T');
				}
				else
				{
					CSomeFun::GetInstance().SimlationKey_Immediately('T');
					CSomeFun::GetInstance().SimlationKey_Immediately('R');
				}
			}
			else
			{
				if (dwMp >= 8 && FarmSkill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_4))
				{
					//LOG_CF_D(L"焦热斩转变成拔剑姿态");
					FarmSkill.UseSkill_By_SkillId(BladeMaster_SkillId_4, FALSE);
					//return;
				}
				else if (FarmSkill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_1_R))
				{
					//LOG_CF_D(L"风火轮");
					FarmSkill.UseSkill_By_SkillId(BladeMaster_SkillId_1_R, FALSE);
					//return;
				}
				else if (FarmSkill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_F2))
				{
					//LOG_CF_D(L"火龙闪？");
					FarmSkill.UseSkill_By_SkillId(BladeMaster_SkillId_F2, FALSE);
					//return;
				}
				else if (FarmSkill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_F))
				{
					//LOG_CF_D(L"?");
					FarmSkill.UseSkill_By_SkillId(BladeMaster_SkillId_F, FALSE);
					//return;
				}
				else if (FarmSkill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_F1))
				{
					//LOG_CF_D(L"?");
					FarmSkill.UseSkill_By_SkillId(BladeMaster_SkillId_F1, FALSE);
					//return;
				}
				else if (FarmSkill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_V))
				{
					//LOG_CF_D(L"凤翼");
					FarmSkill.UseSkill_By_SkillId(BladeMaster_SkillId_V, FALSE);
					//return;
				}
				else if (FarmSkill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_V_V))
				{
					//LOG_CF_D(L"回天令");
					FarmSkill.UseSkill_By_SkillId(BladeMaster_SkillId_V_V, FALSE);
					//return;
				}
				else if (dwMp > 5 && FarmSkill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_T1))
				{
					//LOG_CF_D(L"爆剑令");
					FarmSkill.UseSkill_By_SkillId(BladeMaster_SkillId_T1, FALSE);
					//return;
				}
				else if (dwMp > 5 && FarmSkill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_T2))
				{
					//LOG_CF_D(L"爆剑令");
					FarmSkill.UseSkill_By_SkillId(BladeMaster_SkillId_T2, FALSE);
					//return;
				}
				else if (dwMp > 5 && FarmSkill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_T3))
				{
					//LOG_CF_D(L"爆剑令");
					FarmSkill.UseSkill_By_SkillId(BladeMaster_SkillId_T3, FALSE);
					//return;
				}
				FarmSkill.UseSkill_By_SkillId(BladeMaster_SkillId_R7, FALSE);
				//LOG_CF_D(L"御剑姿态输出中2%d", static_cast<DWORD>(::GetTickCount64() - nTick));
			}
		}

		return CGameVariable::GetInstance().Action_Variable_By_Name(VARIABLE_NAME_AUTOTAKEITEM, TRUE, [&FixPtInfo] {CResAction::GetInstance().TakeDropItem(FixPtInfo); });
	});
}

BOOL CFarmMonster::ForceMaster_Singe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST
{
	LOG_CF_D( L"ForceMaster_Singe, Name=%s, Dis=%.2f", Monster.GetName().c_str(), Monster.GetDis());
	auto dwTick = ::GetTickCount64();
	auto dwHp = Monster.GetHp();
	auto& FarmSkill = CFarmSkill::GetInstance();

	CPersonFunction::GetInstance().CancelPutDownStatus();
	Monster.SetAngle(); // 对准

	while (GameRun && !EMPTY_PERSONHP && !Monster.IsDead() && IsLoseHp(Monster, dwHp, dwTick))
	{
		if (CPersonFunction::GetInstance().IsGameLoad())
		{
			LOG_CF_D( L"打怪的时候出现读图!, 等待过图完毕后break!");
			CPersonFunction::GetInstance().WaitGameLoad();
			break;
		}
		if (Monster.GetDis() > 10.0f)
		{
			if (!Move_To_Monster(Monster, FixPtInfo, TRUE))
			{
				LOG_CF_D( L"卡刀时,无法移动到怪物旁边! ID=%X, Name=%s, HP=%d,Dis=%.2f", Monster.GetId(), Monster.GetName().c_str(), Monster.GetHp(), Monster.GetDis());
				return FALSE;
			}
		}
		//08非战斗状态 09是火   0A是冰
		//em_Player_Pose emPersonPose = CPerson::GetInstance().GetFightPose();

		Monster.SetAngle(); // 对准

		if (CPerson::GetInstance().GetPercentHp() < 30)
		{
			if (FarmSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_Ice_Tab))
			{
				if (FarmSkill.IsActive_By_SkillId_In_Ui(ForceMaster_SkillId_Ice_Tab))
				{
					CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
					Sleep(5000);
				}
			}
		}

		if (CPerson::GetInstance().GetPercentHp() < 80)
		{
			if (FarmSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_Q) || FarmSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_Q1))
			{
				CSomeFun::GetInstance().SimlationKey('Q', 10);
				continue;
			}
		}

		if (CPerson::GetInstance().GetPercentHp() < 70)
		{
			if (FarmSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_E) || FarmSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_E1))
			{
				CSomeFun::GetInstance().SimlationKey('E', 10);
				continue;
			}
		}

		/*
		if (FarmSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_Fure_X) )
		{
			FarmSkill.UseSkill_BySkillId_In_Sleep(ForceMaster_SkillId_Fure_X);
		}
		else if (FarmSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_Fure_C))
		{
			FarmSkill.UseSkill_BySkillId_In_Sleep(ForceMaster_SkillId_Fure_C);
		}
		else if (FarmSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_Fure_V) )
		{
			FarmSkill.UseSkill_BySkillId_In_Sleep(ForceMaster_SkillId_Fure_V);
		}
		else if (FarmSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_G))
		{
			FarmSkill.UseSkill_BySkillId_In_Sleep(ForceMaster_SkillId_G);
		}
		
	*/
		if (FarmSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_Ice_Z))
		{
			FarmSkill.UseSkill_BySkillId_In_Sleep(ForceMaster_SkillId_Ice_Z);
		}
		else if (FarmSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_Fure_Z))
		{
			FarmSkill.UseSkill_BySkillId_In_Sleep(ForceMaster_SkillId_Fure_Z);
		}
		else if (FarmSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_Fure_Z_Z))
		{
			FarmSkill.UseSkill_BySkillId_In_Sleep(ForceMaster_SkillId_Fure_Z_Z);
		}
		else if (FarmSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_4_1))
		{
			FarmSkill.UseSkill_BySkillId_In_Sleep(ForceMaster_SkillId_4_1);
		}
		else if (FarmSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_4_2))
		{
			FarmSkill.UseSkill_BySkillId_In_Sleep(ForceMaster_SkillId_4_2);
		}
		else if (FarmSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_Ice_X))
		{
			FarmSkill.UseSkill_BySkillId_In_Sleep(ForceMaster_SkillId_Ice_X);
		}
		/*
		else if (FarmSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_1_1))
		{
			FarmSkill.UseSkill_BySkillId_In_Sleep(ForceMaster_SkillId_1_1);
		}
		else if (FarmSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_1_2))
		{
			FarmSkill.UseSkill_BySkillId_In_Sleep(ForceMaster_SkillId_1_2);
		}
		else if (FarmSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_1_3))
		{
			FarmSkill.UseSkill_BySkillId_In_Sleep(ForceMaster_SkillId_1_3);
		}
		*/
		else if (FarmSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_Fure_X))
		{
			FarmSkill.UseSkill_BySkillId_In_Sleep(ForceMaster_SkillId_Fure_X);
		}
		else if (FarmSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_Fure_C))
		{
			FarmSkill.UseSkill_BySkillId_In_Sleep(ForceMaster_SkillId_Fure_C);
		}
		else if (FarmSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_Fure_V))
		{
			FarmSkill.UseSkill_BySkillId_In_Sleep(ForceMaster_SkillId_Fure_V);
		}
		else if (FarmSkill.IsCool_By_SkillId_In_Ui(ForceMaster_SkillId_G))
		{
			FarmSkill.UseSkill_BySkillId_In_Sleep(ForceMaster_SkillId_G);
		}
		//LOG_CF_D( L"测试4");
		if (CPerson::GetInstance().GetMp() < 4)
		{
			CSomeFun::GetInstance().SimlationKey('F', 0);
			CSomeFun::GetInstance().SimlationKey('R', 0);
			CSomeFun::GetInstance().SimlationKey('T', 0);
		}
		else
		{
			CSomeFun::GetInstance().SimlationKey(VK_2, 0);
			CSomeFun::GetInstance().SimlationKey('F', 0);
			CSomeFun::GetInstance().SimlationKey('R', 0);
			CSomeFun::GetInstance().SimlationKey('T', 0);
		}
	}

	return CGameVariable::GetInstance().Action_Variable_By_Name(VARIABLE_NAME_AUTOTAKEITEM, TRUE, [&FixPtInfo]{CResAction::GetInstance().TakeDropItem(FixPtInfo); });
}

BOOL CFarmMonster::ForceMaster_Round(_In_ CONST CMonster& Monster, _In_ CONST CCat& newcaat) CONST
{
	return TRUE;
}

BOOL CFarmMonster::Assassin_Singe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST
{
	LOG_CF_D( L"Assassin_Singe, Name=%s, Dis=%.2f", Monster.GetName().c_str(), Monster.GetDis());
	auto dwTick = ::GetTickCount64();
	auto dwHp = Monster.GetHp();
	auto& FarmSkill = CFarmSkill::GetInstance();

	CPersonFunction::GetInstance().CancelPutDownStatus();
	Monster.SetAngle(); // 对准

	while (GameRun && !EMPTY_PERSONHP && !Monster.IsDead() && IsLoseHp(Monster, dwHp, dwTick,15*1000))
	{
		if (CPersonFunction::GetInstance().IsGameLoad())
		{
			LOG_CF_D( L"打怪的时候出现读图!, 等待过图完毕后break!");
			CPersonFunction::GetInstance().WaitGameLoad();
			break;
		}
		if (Monster.GetDis() > 1.0f)
		{
			if (!Move_To_Monster(Monster, FixPtInfo, TRUE))
			{
				LOG_CF_D( L"卡刀时,无法移动到怪物旁边! ID=%X, Name=%s, HP=%d,Dis=%.2f", Monster.GetId(), Monster.GetName().c_str(), Monster.GetHp(), Monster.GetDis());
				return FALSE;
			}
		}
		//08非战斗状态 09是火   0A是冰
		//em_Player_Pose emPersonPose = CPerson::GetInstance().GetFightPose();

		Monster.SetAngle(); // 对准

		if (CPerson::GetInstance().GetPercentHp() < 30)
		{
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Assassin_SkillId_2))
			{
				CSomeFun::GetInstance().SimlationKey(VK_2, 10);
				Sleep(1000);
			}

			if (FarmSkill.IsCool_By_SkillId_In_Ui(Assassin_SkillId_SS))
			{
				FarmSkill.UseSkill_By_SS();
				Sleep(1000);
			}
		}

		if (CPerson::GetInstance().GetPercentHp() < 75)
		{
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Assassin_SkillId_E))
			{
				CSomeFun::GetInstance().SimlationKey('E', 10);
				Sleep(100);
				continue;
			}
		}

		if (CPerson::GetInstance().GetPercentHp() < 85)
		{
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Assassin_SkillId_Q))
			{
				CSomeFun::GetInstance().SimlationKey('Q', 10);
				Sleep(100);
				continue;
			}
		}

		if (CPerson::GetInstance().GetPercentHp() < 90)
		{
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Assassin_SkillId_C))
			{
				CSomeFun::GetInstance().SimlationKey('C', 10);
				Sleep(1200);
			}
		}


		//LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"怪物最大%d!,我最大%d", Monster.GetMaxHp() , CPerson::GetInstance().GetMaxHp());
		if (Monster.GetMaxHp() > CPerson::GetInstance().GetMaxHp())
		{
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Assassin_SkillId_V))
			{
				CSomeFun::GetInstance().SimlationKey('V', 10);
			}
		}
		

		if (CPerson::GetInstance().GetFightPose() == em_Player_Pose_Assassin_Hide)
		{
			//隐身状态
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Assassin_SkillId_Hide_1))
			{
				CSomeFun::GetInstance().SimlationKey(VK_1, 10);
			}
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Assassin_SkillId_Hide_3))
			{
				CSomeFun::GetInstance().SimlationKey(VK_3, 10);
			}
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Assassin_SkillId_Hide_R))
			{
				CSomeFun::GetInstance().SimlationKey('R', 10);
			}
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Assassin_SkillId_Hide_T) ||
				FarmSkill.IsCool_By_SkillId_In_Ui(Assassin_SkillId_2_T))
			{
				CSomeFun::GetInstance().SimlationKey('T', 10);
			}
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Assassin_SkillId_Hide_F))
			{
				CSomeFun::GetInstance().SimlationKey('F', 10);
			}

			if (FarmSkill.IsCool_By_SkillId_In_Ui(Assassin_SkillId_Hide_4))
			{
				CSomeFun::GetInstance().SimlationKey(VK_4, 10);
			}
			//if (FarmSkill.IsCool_By_SkillId_In_Ui(Assassin_SkillId_Hide_Tab))
			//{
			//	CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
			//}
			continue;
		}

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Assassin_SkillId_SS_F))
		{
			CSomeFun::GetInstance().SimlationKey('F', 10);
		}


		if (FarmSkill.IsCool_By_SkillId_In_Ui(Assassin_SkillId_X) ||
			FarmSkill.IsCool_By_SkillId_In_Ui(Assassin_SkillId_Hide_X1) ||
			FarmSkill.IsCool_By_SkillId_In_Ui(Assassin_SkillId_Hide_X2) ||
			FarmSkill.IsCool_By_SkillId_In_Ui(Assassin_SkillId_Hide_X))
		{
			CSomeFun::GetInstance().SimlationKey('X', 10);
		}


		if (FarmSkill.IsCool_By_SkillId_In_Ui(Assassin_SkillId_2_T))
		{
			CSomeFun::GetInstance().SimlationKey('T', 10);
		}

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Assassin_SkillId_Tab_Tab) ||
			FarmSkill.IsCool_By_SkillId_In_Ui(Assassin_SkillId_Tab1) ||
			FarmSkill.IsCool_By_SkillId_In_Ui(Assassin_SkillId_Tab2) ||
			FarmSkill.IsCool_By_SkillId_In_Ui(Assassin_SkillId_Tab3) ||
			FarmSkill.IsCool_By_SkillId_In_Ui(Assassin_SkillId_Tab4) )
		{
			CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
		}


		if (FarmSkill.IsCool_By_SkillId_In_Ui(Assassin_SkillId_3) || FarmSkill.IsCool_By_SkillId_In_Ui(Assassin_SkillId_3_3))
		{
			CSomeFun::GetInstance().SimlationKey(VK_3, 10);
		}

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Assassin_SkillId_4))
		{
			CSomeFun::GetInstance().SimlationKey(VK_4, 10);
			Sleep(500);
		}

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Assassin_SkillId_Z) ||
			FarmSkill.IsCool_By_SkillId_In_Ui(Assassin_SkillId_Z_1))
		{
			CSomeFun::GetInstance().SimlationKey('Z', 10);
		}
		 

		//LOG_CF_D( L"测试4");
		if (CPerson::GetInstance().GetLevel() > 3)
		{
			if (CPerson::GetInstance().GetMp() > 3)
			{
				CSomeFun::GetInstance().SimlationKey('T', 10);
			}
			else
			{
				CSomeFun::GetInstance().SimlationKey('R', 10);
				Sleep(50);
				CSomeFun::GetInstance().SimlationKey('X', 10);
			}
		}
		else
		{
			CSomeFun::GetInstance().SimlationKey('R', 10);
		}
		Sleep(100);
	}

	return CGameVariable::GetInstance().Action_Variable_By_Name(VARIABLE_NAME_AUTOTAKEITEM, TRUE, [&FixPtInfo]{CResAction::GetInstance().TakeDropItem(FixPtInfo); });
}

BOOL CFarmMonster::Danncer_Singe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST
{
	LOG_CF_D( L"Danncer_Singe, Name=%s, Dis=%.2f", Monster.GetName().c_str(), Monster.GetDis());
	auto dwTick = ::GetTickCount64();
	//auto Skill_Q = ::GetTickCount64();
	auto dwHp = Monster.GetHp();
	auto& FarmSkill = CFarmSkill::GetInstance();

	CPersonFunction::GetInstance().CancelPutDownStatus();
	Monster.SetAngle(); // 对准

	while (GameRun && !EMPTY_PERSONHP && !Monster.IsDead() && IsLoseHp(Monster, dwHp, dwTick))
	{
		if (CPersonFunction::GetInstance().IsGameLoad())
		{
			LOG_CF_D( L"打怪的时候出现读图!, 等待过图完毕后break!");
			CPersonFunction::GetInstance().WaitGameLoad();
			break;
		}
		if (Monster.GetDis() > 2.0f)
		{
			if (!Move_To_Monster(Monster, FixPtInfo, TRUE))
			{
				LOG_CF_D( L"卡刀时,无法移动到怪物旁边! ID=%X, Name=%s, HP=%d,Dis=%.2f", Monster.GetId(), Monster.GetName().c_str(), Monster.GetHp(), Monster.GetDis());
				return FALSE;
			}
		}
		//16是拔剑姿态   14是正常姿态
		em_Player_Pose emPersonPose = CPerson::GetInstance().GetFightPose();

		Monster.SetAngle(); // 对准

		//LOG_CF_D( L"姿态%x", emPersonPose);
		


		
			if (emPersonPose == 0x16)
			{
				if (CPerson::GetInstance().GetPercentHp() < 70)
				{
					if (FarmSkill.IsCool_By_SkillId_In_Ui(Danncer_SkillId_Sword_E))
					{
						CSomeFun::GetInstance().SimlationKey('E', 10);
					}
				}

				if (CPerson::GetInstance().GetPercentHp() < 85)
				{
					if (FarmSkill.IsCool_By_SkillId_In_Ui(Danncer_SkillId_Sword_Q))
					{
						CSomeFun::GetInstance().SimlationKey('Q', 10);
					}
				}
				if (CPerson::GetInstance().GetMp() < 5)
				{
					if (FarmSkill.IsCool_By_SkillId_In_Ui(Danncer_SkillId_Sword_2))
					{
						CSomeFun::GetInstance().SimlationKey(VK_2, 10);
					}
				}

				if (FarmSkill.IsCool_By_SkillId_In_Ui(Danncer_SkillId_Z) ||
					FarmSkill.IsCool_By_SkillId_In_Ui(Danncer_SkillId_Sword_Z))
				{
					CSomeFun::GetInstance().SimlationKey('Z', 10);
				}

				/*if (FarmSkill.IsCool_By_SkillId_In_Ui(Danncer_SkillId_Sword_F))
				{
				CSomeFun::GetInstance().SimlationKey('F', 10);
				}
				// 19BFCD2

				if (CPersonFunction::GetInstance().ExistBuff_Person(0x19BFCCF, nullptr) ||
					CPersonFunction::GetInstance().ExistBuff_Person(0x19BFCD0, nullptr) ||
					CPersonFunction::GetInstance().ExistBuff_Person(0x19BFCD1, nullptr) ||
					CPersonFunction::GetInstance().ExistBuff_Person(0x19BFCD2, nullptr)) // 雷魂
					*/
					//if (CPersonFunction::GetInstance().ExistBuff_Person(0x19BFCED, nullptr))
					//{
					//	CSomeFun::GetInstance().SimlationKey('F', 10);
					//	CSomeFun::GetInstance().SimlationKey('R', 10);
					//}
				if (CPersonFunction::GetInstance().ExistBuff_Person(0x19BFCCF, nullptr) ||
					CPersonFunction::GetInstance().ExistBuff_Person(0x19BFCD0, nullptr) ||
					CPersonFunction::GetInstance().ExistBuff_Person(0x19BFCD1, nullptr) ||
					CPersonFunction::GetInstance().ExistBuff_Person(0x19BFCD2, nullptr) ||
					CPersonFunction::GetInstance().ExistBuff_Person(0x19BFCEB, nullptr) )
				{
					CSomeFun::GetInstance().SimlationKey('F', 0);
				}
				else
				{
					if (CPerson::GetInstance().GetMp() > 4)
					{
						if (FarmSkill.IsCool_By_SkillId_In_Ui(Danncer_SkillId_X))
						{
							//CSomeFun::GetInstance().SimlationKey('X', 10);
							FarmSkill.UseSkill_BySkillId_In_Sleep(Danncer_SkillId_X);
						}
						else if (FarmSkill.IsCool_By_SkillId_In_Ui(Danncer_SkillId_C))
						{
							//CSomeFun::GetInstance().SimlationKey('C', 10);
							FarmSkill.UseSkill_BySkillId_In_Sleep(Danncer_SkillId_C);
						}
						else if (FarmSkill.IsCool_By_SkillId_In_Ui(Danncer_SkillId_V))
						{
							//CSomeFun::GetInstance().SimlationKey('V', 10);
							FarmSkill.UseSkill_BySkillId_In_Sleep(Danncer_SkillId_V);
						}
					}
					CSomeFun::GetInstance().SimlationKey('F', 0);
				}
				CSomeFun::GetInstance().SimlationKey('R', 0);
				continue;
			}

			if (CPerson::GetInstance().GetPercentHp() < 30)
			{
				if (FarmSkill.IsCool_By_SkillId_In_HideUi(Danncer_SkillId_Tab_F))
				{
					CSomeFun::GetInstance().SimlationKey(VK_TAB, 0);
					
					CSomeFun::GetInstance().SimlationKey('F', 0);
					CSomeFun::GetInstance().SimlationKey('F', 0);
					//Sleep(800);
				}
				else
				{
					CSomeFun::GetInstance().SimlationKey(VK_TAB, 0);
					Sleep(800);
				}
			}

			if (CPerson::GetInstance().GetPercentHp() < 70)
			{
				if (FarmSkill.IsCool_By_SkillId_In_Ui(Danncer_SkillId_E) ||
					FarmSkill.IsCool_By_SkillId_In_Ui(Danncer_SkillId_Sword_E) ||
					FarmSkill.IsCool_By_SkillId_In_Ui(Danncer_SkillId_Grab_E))
				{
					CSomeFun::GetInstance().SimlationKey('E', 10);
				}
			}
			else if (CPerson::GetInstance().GetPercentHp() < 85)
			{
				if (FarmSkill.IsCool_By_SkillId_In_Ui(Danncer_SkillId_Q) ||
					FarmSkill.IsCool_By_SkillId_In_Ui(Danncer_SkillId_Sword_Q) ||
					FarmSkill.IsCool_By_SkillId_In_Ui(Danncer_SkillId_Grab_Q))
				{
					CSomeFun::GetInstance().SimlationKey('Q', 10);
				}

			}


			if (CPerson::GetInstance().GetMp() > 4)
			{
				if (FarmSkill.IsCool_By_SkillId_In_Ui(Danncer_SkillId_V))
				{
					//CSomeFun::GetInstance().SimlationKey('V', 10);
					FarmSkill.UseSkill_BySkillId_In_Sleep(Danncer_SkillId_V);
					continue;
				}

				if (FarmSkill.IsCool_By_SkillId_In_Ui(Danncer_SkillId_C))
				{
					//CSomeFun::GetInstance().SimlationKey('C', 10);
					FarmSkill.UseSkill_BySkillId_In_Sleep(Danncer_SkillId_C);
					continue;
				}

				if (FarmSkill.IsCool_By_SkillId_In_Ui(Danncer_SkillId_X))
				{
					//CSomeFun::GetInstance().SimlationKey('X', 10);
					FarmSkill.UseSkill_BySkillId_In_Sleep(Danncer_SkillId_X);
					continue;
				}
			}

			if (FarmSkill.IsExist_By_SkillId_In_Ui(Danncer_SkillId_Awaken_F1) ||
				FarmSkill.IsExist_By_SkillId_In_Ui(Danncer_SkillId_Awaken_F2) ||
				FarmSkill.IsExist_By_SkillId_In_Ui(Danncer_SkillId_Awaken_F3) ||
				FarmSkill.IsExist_By_SkillId_In_Ui(Danncer_SkillId_Awaken_F4) ||
				FarmSkill.IsExist_By_SkillId_In_Ui(Danncer_SkillId_Awaken_F5) ||
				FarmSkill.IsExist_By_SkillId_In_Ui(Danncer_SkillId_Fly_F1) ||
				FarmSkill.IsExist_By_SkillId_In_Ui(Danncer_SkillId_Fly_F2) ||
				FarmSkill.IsExist_By_SkillId_In_Ui(Danncer_SkillId_Grab_F) ||
				FarmSkill.IsExist_By_SkillId_In_Ui(Danncer_SkillId_Sword_Awaken_F1) ||
				FarmSkill.IsExist_By_SkillId_In_Ui(Danncer_SkillId_Sword_Awaken_F2) ||
				FarmSkill.IsExist_By_SkillId_In_Ui(Danncer_SkillId_Sword_Awaken_F3))
			{
				CSomeFun::GetInstance().SimlationKey('F', 10);
			}

			if (FarmSkill.IsCool_By_SkillId_In_Ui(Danncer_SkillId_1))
			{
				//CSomeFun::GetInstance().SimlationKey(VK_1, 10);
				FarmSkill.UseSkill_BySkillId_In_Sleep(Danncer_SkillId_1);
			}

			if (FarmSkill.IsCool_By_SkillId_In_Ui(Danncer_SkillId_2_1) || FarmSkill.IsCool_By_SkillId_In_Ui(Danncer_SkillId_2_2))
			{
				CSomeFun::GetInstance().SimlationKey(VK_2, 10);
			}

			if (FarmSkill.IsCool_By_SkillId_In_Ui(Danncer_SkillId_3))
			{
				//CSomeFun::GetInstance().SimlationKey(VK_3, 10);
				FarmSkill.UseSkill_BySkillId_In_Sleep(Danncer_SkillId_3);
			}




			//LOG_CF_D( L"测试4");
			if (CPerson::GetInstance().GetMp() > 5)
				CSomeFun::GetInstance().SimlationKey('T', 10);
			else
				CSomeFun::GetInstance().SimlationKey('R', 10);
	}

	return CGameVariable::GetInstance().Action_Variable_By_Name(VARIABLE_NAME_AUTOTAKEITEM, TRUE, [&FixPtInfo]{CResAction::GetInstance().TakeDropItem(FixPtInfo); });
}

BOOL CFarmMonster::KongFuMaster_Singe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST
{
	LOG_CF_D( L"KongFuMaster_Singe, Name=%s, Dis=%.2f", Monster.GetName().c_str(), Monster.GetDis());
	auto dwTick = ::GetTickCount64();
	auto dwHp = Monster.GetHp();
	auto& FarmSkill = CFarmSkill::GetInstance();

	CPersonFunction::GetInstance().CancelPutDownStatus();
	Monster.SetAngle(); // 对准

	if (FarmSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_T))
	{
		CSomeFun::GetInstance().SimlationKey('T', 10);
	}

	while (GameRun && !EMPTY_PERSONHP && !Monster.IsDead() && IsLoseHp(Monster, dwHp, dwTick, 8 * 1000))
	{
		if (CPersonFunction::GetInstance().IsGameLoad())
		{
			LOG_CF_D( L"打怪的时候出现读图!, 等待过图完毕后break!");
			CPersonFunction::GetInstance().WaitGameLoad();
			break;
		}
		if (Monster.GetDis() > 2.0f)
		{
			if (!Move_To_Monster(Monster, FixPtInfo, TRUE))
			{
				LOG_CF_D( L"卡刀时,无法移动到怪物旁边! ID=%X, Name=%s, HP=%d,Dis=%.2f", Monster.GetId(), Monster.GetName().c_str(), Monster.GetHp(), Monster.GetDis());
				return FALSE;
			}
		}
		//16是拔剑姿态   14是正常姿态
		//em_Player_Pose emPersonPose = CPerson::GetInstance().GetFightPose();

		Monster.SetAngle(); // 对准

		//LOG_CF_D( L"姿态%x", emPersonPose);
		if (Monster.GetMaxHp() > CPerson::GetInstance().GetMaxHp())
		{
			if (FarmSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_Tab1))
			{
				CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
			}
		}

		if (FarmSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_X_1) ||
			FarmSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_X_2) ||
			FarmSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_X_3) ||
			FarmSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_X_4) ||
			FarmSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_X1) ||
			FarmSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_X2) ||
			FarmSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_X3) || 
			FarmSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_X5)  )
		{
			CSomeFun::GetInstance().SimlationKey('X', 10);
			continue;
		}
		
		if (CPerson::GetInstance().GetPercentHp() < 65)
		{
			if (FarmSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_E))
			{
				CSomeFun::GetInstance().SimlationKey('E', 10);
			}
		}
		if (CPerson::GetInstance().GetPercentHp() < 85)
		{
			if (FarmSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_Q))
			{
				CSomeFun::GetInstance().SimlationKey('Q', 10);
			}
		}

		if (FarmSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_3) ||
			FarmSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_3_1))
		{
			CSomeFun::GetInstance().SimlationKey(VK_3, 10);
		}

		if (FarmSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_V))
		{
			CSomeFun::GetInstance().SimlationKey('V', 10);
		}



		if (FarmSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_T) ||
			FarmSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_T1) ||
			FarmSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_T2))
		{
			CSomeFun::GetInstance().SimlationKey('T', 10);
		}

		if (FarmSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_Z))
		{
			CSomeFun::GetInstance().SimlationKey('Z', 10);
		}

		if (FarmSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_4_1) ||
			FarmSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_41_2))
		{
			CSomeFun::GetInstance().SimlationKey(VK_4, 10);
		}


		if (FarmSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_F1_1) ||
			FarmSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_F1_2) ||
			FarmSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_F2_1) ||
			FarmSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_F2_2) ||
			FarmSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_F2_3) || 
			FarmSkill.IsCool_By_SkillId_In_Ui(KongFuMaster_SkillId_F3_1))
		{
			CSomeFun::GetInstance().SimlationKey('F', 10);
			Sleep(100);
			continue;
		}

	
		//LOG_CF_D( L"测试4");
		if (CPerson::GetInstance().GetMp() > 4)
		{
			CSomeFun::GetInstance().SimlationKey(VK_2, 10);
			Sleep(50); 
		}
		CSomeFun::GetInstance().SimlationKey('R', 10);
		Sleep(100);
	}

	return CGameVariable::GetInstance().Action_Variable_By_Name(VARIABLE_NAME_AUTOTAKEITEM, TRUE, [&FixPtInfo]{CResAction::GetInstance().TakeDropItem(FixPtInfo); });
}

BOOL CFarmMonster::Gunner_Singe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST
{
	//IsExist_By_SkillId_In_Ui
	LOG_CF_D( L"Gunner_Singe, Name=%s, Dis=%.2f", Monster.GetName().c_str(), Monster.GetDis());
	auto dwTick = ::GetTickCount64();
	auto dwHp = Monster.GetHp();
	auto& FarmSkill = CFarmSkill::GetInstance();

	CPersonFunction::GetInstance().CancelPutDownStatus();
	Monster.SetAngle(); // 对准

	while (GameRun && !EMPTY_PERSONHP && !Monster.IsDead() && IsLoseHp(Monster, dwHp, dwTick))
	{
		if (CPersonFunction::GetInstance().IsGameLoad())
		{
			LOG_CF_D( L"打怪的时候出现读图!, 等待过图完毕后break!");
			CPersonFunction::GetInstance().WaitGameLoad();
			break;
		}
		if (Monster.GetDis() > 10.0f)
		{
			if (!Move_To_Monster(Monster, FixPtInfo,TRUE))
			{
				LOG_CF_D( L"卡刀时,无法移动到怪物旁边! ID=%X, Name=%s, HP=%d,Dis=%.2f", Monster.GetId(), Monster.GetName().c_str(), Monster.GetHp(), Monster.GetDis());
				return FALSE;
			}
		}
		//08非战斗状态 09是火   0A是冰
		//em_Player_Pose emPersonPose = CPerson::GetInstance().GetFightPose();

		Monster.SetAngle(); // 对准

		if (CPersonFunction::GetInstance().ExistBuff_Person(0x166953B, nullptr)) // 小超神BUFF
		{
			if (FarmSkill.IsExist_By_SkillId_In_Ui(Gunner_SkillId_Tab_F))//只要出现就按F
			{
				CSomeFun::GetInstance().SimlationKey('F', 10);
			}
			else
			{
				CSomeFun::GetInstance().SimlationKey('R', 10);
				Sleep(50);
			}
			continue;
		}
		

		if (CPerson::GetInstance().GetPercentHp() < 50)
		{
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Gunner_SkillId_X))
			{
				CSomeFun::GetInstance().SimlationKey('X', 10);
			}
		}

		if (CPerson::GetInstance().GetPercentHp() < 70)
		{
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Gunner_SkillId_E))
			{
				CSomeFun::GetInstance().SimlationKey('E', 10);
			}
		}

		if (Monster.GetMaxHp() > CPerson::GetInstance().GetMaxHp())
		{
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Gunner_SkillId_Tab))
			{
				CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
				continue;
			}
		}

		if (FarmSkill.IsExist_By_SkillId_In_Ui(Gunner_SkillId_F) ||
			FarmSkill.IsExist_By_SkillId_In_Ui(Gunner_SkillId_3_F))
		{
			CSomeFun::GetInstance().SimlationKey('F', 10);
		}

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Gunner_SkillId_3))
		{
			CSomeFun::GetInstance().SimlationKey(VK_3, 10);
		}

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Gunner_SkillId_4) ||
			FarmSkill.IsCool_By_SkillId_In_Ui(Gunner_SkillId_4_1) ||
			FarmSkill.IsCool_By_SkillId_In_Ui(Gunner_SkillId_4_2))
		{
			CSomeFun::GetInstance().SimlationKey(VK_4, 10);
		}

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Gunner_SkillId_2))
		{
			CSomeFun::GetInstance().SimlationKey(VK_2, 10);
		}

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Gunner_SkillId_C))
		{
			CSomeFun::GetInstance().SimlationKey('C', 10);
		}

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Gunner_SkillId_V))
		{
			CSomeFun::GetInstance().SimlationKey('V', 10);
		}

		//LOG_CF_D( L"测试4");
		if (CPerson::GetInstance().GetMp() > 10)
		{
			CSomeFun::GetInstance().SimlationKey('T', 10);
		}
		else
		{
			CSomeFun::GetInstance().SimlationKey('R', 10);
		}
		Sleep(100);
	}

	return CGameVariable::GetInstance().Action_Variable_By_Name(VARIABLE_NAME_AUTOTAKEITEM, TRUE, [&FixPtInfo]{CResAction::GetInstance().TakeDropItem(FixPtInfo); });
}

BOOL CFarmMonster::Gunner_Aoe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST
{
	//IsExist_By_SkillId_In_Ui
	LOG_CF_D( L"Gunner_Aoe, Name=%s, Dis=%.2f", Monster.GetName().c_str(), Monster.GetDis());
	auto dwTick = ::GetTickCount64();
	auto dwHp = Monster.GetHp();
	auto& FarmSkill = CFarmSkill::GetInstance();

	CPersonFunction::GetInstance().CancelPutDownStatus();
	Monster.SetAngle(); // 对准

	while (GameRun && !EMPTY_PERSONHP && !Monster.IsDead() && IsLoseHp(Monster, dwHp, dwTick))
	{
		if (CPersonFunction::GetInstance().IsGameLoad())
		{
			LOG_CF_D( L"打怪的时候出现读图!, 等待过图完毕后break!");
			CPersonFunction::GetInstance().WaitGameLoad();
			break;
		}
		if (Monster.GetDis() > 10.0f)
		{
			if (!Move_To_Monster(Monster, FixPtInfo, TRUE))
			{
				LOG_CF_D( L"卡刀时,无法移动到怪物旁边! ID=%X, Name=%s, HP=%d,Dis=%.2f", Monster.GetId(), Monster.GetName().c_str(), Monster.GetHp(), Monster.GetDis());
				return FALSE;
			}
		}
		//08非战斗状态 09是火   0A是冰
		//em_Player_Pose emPersonPose = CPerson::GetInstance().GetFightPose();

		Monster.SetAngle(); // 对准

		if (CPersonFunction::GetInstance().ExistBuff_Person(0x166953B, nullptr)) // 小超神BUFF
		{
			if (FarmSkill.IsExist_By_SkillId_In_Ui(Gunner_SkillId_Tab_F))//只要出现就按F
			{
				CSomeFun::GetInstance().SimlationKey('F', 10);
			}
			else
			{
				CSomeFun::GetInstance().SimlationKey('R', 10);
				Sleep(50);
			}
			continue;
		}


		if (CPerson::GetInstance().GetPercentHp() < 50)
		{
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Gunner_SkillId_X))
			{
				CSomeFun::GetInstance().SimlationKey('X', 10);
			}
		}

		if (CPerson::GetInstance().GetPercentHp() < 70)
		{
			if (FarmSkill.IsCool_By_SkillId_In_Ui(Gunner_SkillId_E))
			{
				CSomeFun::GetInstance().SimlationKey('E', 10);
			}
		}

			if (FarmSkill.IsCool_By_SkillId_In_Ui(Gunner_SkillId_Tab))
			{
				CSomeFun::GetInstance().SimlationKey(VK_TAB, 10);
				continue;
			}

		if (FarmSkill.IsExist_By_SkillId_In_Ui(Gunner_SkillId_F) ||
			FarmSkill.IsExist_By_SkillId_In_Ui(Gunner_SkillId_3_F) ||
			FarmSkill.IsExist_By_SkillId_In_Ui(Gunner_SkillId_E_F))
		{
			CSomeFun::GetInstance().SimlationKey('F', 10);
		}

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Gunner_SkillId_3))
		{
			CSomeFun::GetInstance().SimlationKey(VK_3, 10);
		}

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Gunner_SkillId_4) ||
			FarmSkill.IsCool_By_SkillId_In_Ui(Gunner_SkillId_4_1) ||
			FarmSkill.IsCool_By_SkillId_In_Ui(Gunner_SkillId_4_2))
		{
			CSomeFun::GetInstance().SimlationKey(VK_4, 10);
		}

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Gunner_SkillId_2))
		{
			CSomeFun::GetInstance().SimlationKey(VK_2, 10);
		}

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Gunner_SkillId_C))
		{
			CSomeFun::GetInstance().SimlationKey('C', 10);
		}

		if (FarmSkill.IsCool_By_SkillId_In_Ui(Gunner_SkillId_V))
		{
			CSomeFun::GetInstance().SimlationKey('V', 10);
		}

		//LOG_CF_D( L"测试4");
		if (CPerson::GetInstance().GetMp() > 10)
		{
			CSomeFun::GetInstance().SimlationKey('T', 10);
		}
		else
		{
			CSomeFun::GetInstance().SimlationKey('R', 10);
		}
		Sleep(100);
	}

	return CGameVariable::GetInstance().Action_Variable_By_Name(VARIABLE_NAME_AUTOTAKEITEM, TRUE, [&FixPtInfo]{CResAction::GetInstance().TakeDropItem(FixPtInfo); });
}

/*
BOOL CFarmMonster::BaiXianPratice_Summoner(_In_ CONST CPlayer& Player, _In_ CONST FixPointInfo& Round, _In_ std::function<BOOL(VOID)> fnTerminate) CONST
{
	CMonster Monster(Player);
	auto& FarmSkill = CFarmSkill::GetInstance();
	BOOL bRunThread = TRUE;
	ULONGLONG ulTick = NULL;

	std::vector<Point> vRound;
	GetRoundPointList(Round, 6, vRound);

	CCat Cat;
	if (!CCatExpend::GetInstance().FindSelfCat(&Cat))
	{
		LOG_CF_D( L"找不到猫！ 等死吧!");
		while (GameRun && !EMPTY_PERSONHP && !Monster.IsDead() && !fnTerminate() && Monster.GetDis() < 100 && BnsSleep(1000));
	}

	std::thread t([&FarmSkill, &bRunThread, &vRound, &Monster, &Cat, &ulTick]
	{
		CONST static  std::vector<DWORD> vlst = {
			Sum_SkillId_Floor_F, Sum_SkillId_Floor_2, Sum_SkillId_Floor_1, Sum_SkillId_X
		};
		CONST static std::vector<DWORD> CatVec = {
			Sum_SkillId_V_2, Sum_SkillId_V_3, Sum_SkillId_Floor_C, Sum_SkillId_X_2
		};

		UINT nIndex = 0;
		while (bRunThread && GameRun)
		{
			for (CONST auto& itm : vlst)
			{
				if (FarmSkill.IsCool_By_SkillId_In_Ui(itm))
				{
					if (itm == Sum_SkillId_X && CPerson::GetInstance().GetPercentHp() >= 90)
						continue;

					FarmSkill.UseSkill_BySkillId_In_Sleep(itm, FALSE);
					break;
				}
			}

			for (CONST auto& itm : CatVec)
			{
				if (FarmSkill.IsCool_By_SkillId_In_Ui(itm))
				{
					FarmSkill.UseSkill_BySkillId_In_Sleep(itm, FALSE, em_Skill_Type::em_Skill_Type_Cat);
					break;
				}
			}

			Monster.SetAngle();
			BnsSleep(100);
			Monster.SetAngle();
		}
	});

	// 敌人猫或者人 靠近的时候用4和3
	auto fnGetSafePoint = [&Monster, &vRound]
	{
		return CLPublic::Vec_find_if(vRound, [&Monster](CONST Point& RoundPoint){
			return Monster.GetDis(RoundPoint) > 8 && Monster.GetDis(RoundPoint) < 16;
		});
	};

	auto fnUseT = [&FarmSkill, &Monster]
	{
		CONST static std::vector<DWORD> TVec = {
			Sum_SkillId_T2, Sum_SkillId_T2_Ui, Sum_SkillId_T, 0x66C3, 0x66C5
		};
		auto p = CLPublic::Vec_find_if_Const(TVec, [&FarmSkill](DWORD dwSkillId){
			return FarmSkill.IsCool_By_SkillId_In_Ui(dwSkillId);
		});
		if (p != nullptr && CPerson::GetInstance().GetMp() >= 4 && Monster.GetDis() < 16)
		{
			Monster.SetAngle();
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(1000, [&FarmSkill, &p]{return FarmSkill.UseSkill_By_SkillId(*p); });
			BnsSleep(500);
			CPersonFunction::GetInstance().WaitForProgressBarBySkill(1500);
		}
	};

	em_Player_Classes EnemyClasses = Monster.GetClasses();
	UINT nIndex = 0;

	CSomeFun::GetInstance().SetSpeed(3.0f);
	BnsMove(Round.FixPoint, 1);

	while (GameRun && !EMPTY_PERSONHP && !Monster.IsDead() && !fnTerminate() && Monster.GetDis() < 100)
	{
		CONST static std::vector<DWORD> SkillVec =
		{
			Sum_SkillId_T_Floor, Sum_SkillId_T_Floor2, Sum_SkillId_F, Sum_SkillId_F_F,
			Sum_SkillId_1, Sum_SkillId_T2_Ui, Sum_SkillId_3,
		};


		if (EnemyClasses != em_Player_Classes::em_Player_Classes_Destroyer)
		{
			if (Monster.GetDis() > 16)
			{
				if (EnemyClasses == em_Player_Classes::em_Player_Classes_BladeMaster)
					BnsSleep(300);
				else
					CPersonFunction::GetInstance().Move(Monster.GetFrontPoint(12.0f));
				continue;
			}
			else if (Monster.GetDis() <= 5.0f && FarmSkill.IsCool_By_SkillId_In_HideUi(Sum_SkillId_SS))
			{
				Monster.SetAngle();
				FarmSkill.UseSkill_By_SkillId(Sum_SkillId_SS);
			}
			else if (Monster.GetDis() <= 3.0f)
			{
				Monster.SetAngle();
				if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_4) && !CPersonFunction::GetInstance().Exist_Sumoner_X_Buff() && !CObjectFunction::GetInstance().ExistMonsterBuff(Monster.GetId(), Sumoner_BuffId_1))
				{
					FarmSkill.UseSkill_By_SkillId(Sum_SkillId_4);
					CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2 * 1000, []{return CPerson::GetInstance().GetAbNormalStatus() != em_WarStatus::em_WarStatus_Hide; });
					if (CPerson::GetInstance().GetAbNormalStatus() == em_WarStatus::em_WarStatus_Hide)
					{
						LOG_CF_D( L"成功隐形!");
						auto p = fnGetSafePoint();
						Point Pt = p != nullptr ? *p : Round.FixPoint;
						LOG_CF_D( L"隐形状态下走到安全坐标,离人物距离%.2f,怪物距离%.2f!", CBaseObject::GetPersonDis(Pt), Monster.GetDis(Pt));
						while (GameRun && !EMPTY_PERSONHP && CPerson::GetInstance().GetAbNormalStatus() == em_WarStatus::em_WarStatus_Hide && CBaseObject::GetPersonDis(Pt) > 3)
						{
							if (!CPerson::GetInstance().IsMoving())
							{
								CBaseObject::SetAngle(Pt);
								CPersonFunction::GetInstance().Move(Pt);
							}
							BnsSleep(300);
						}


						if (CPerson::GetInstance().GetAbNormalStatus() == em_WarStatus::em_WarStatus_Hide)
						{
							fnUseT();
						}

					}
				}
				else
				{
					auto p = fnGetSafePoint();
					Point Pt = p != nullptr ? *p : Round.FixPoint;
					if (!CPerson::GetInstance().IsMoving())
					{
						CBaseObject::SetAngle(Pt);
						CPersonFunction::GetInstance().Move(Pt);
					}
					BnsSleep(300);
					continue;
				}
			}
		}

		Monster.SetAngle();
		if (!Cat.IsDead())
			CSomeFun::GetInstance().SimlationKey(VK_TAB);

		if ((CPerson::GetInstance().GetAbNormalStatus() == em_WarStatus::em_WarStatus_Floor || Monster.GetAbNormalStatus() != em_WarStatus::em_WarStatus_Floor) && !CObjectFunction::GetInstance().ExistMonsterBuff(Monster.GetId(), Sumoner_BuffId_1))
		{
			if (Monster.GetDis(Cat.GetPoint()) < 5.0f && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_V))
				FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_V, em_Skill_Type_Cat);
			else if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_C, FALSE))
				FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_C, em_Skill_Type_Cat);
			else if (FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_1, FALSE))
				CSomeFun::GetInstance().SimlationKey(VK_1, 10);
			else if (Monster.GetDis(Cat.GetPoint()) < 5.0f && FarmSkill.IsCool_By_SkillId_In_Ui(Sum_SkillId_2))
				FarmSkill.UseSkill_BySkillId_In_Sleep(Sum_SkillId_2, FALSE);

		}

		CONST auto p = CLPublic::Vec_find_if_Const(SkillVec, [&FarmSkill](CONST DWORD itm){ return FarmSkill.IsCool_By_SkillId_In_Ui(itm); });
		if (p != nullptr && CPerson::GetInstance().GetMp() >= 2)
			FarmSkill.UseSkill_By_SkillId(*p);

		fnUseT();

		Monster.SetAngle();
		CSkill::UseSkill_By_Id(Sum_SkillId_R2, FALSE);
		Sleep(100);
		CSkill::UseSkill_By_Id(Sum_SkillId_R, FALSE);
	}
	bRunThread = FALSE;
	t.join();
	LOG_CF_D( L"PK完毕!");
	return TRUE;
}*/