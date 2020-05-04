#include "stdafx.h"
#include "DailyTask_HukryongPrison_Boss_BuKa.h"
#include <MyTools/Log.h>
#include <MyTools/TimeTick.h>
#include <MyTools/CLEchoException.h>
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

#define _SELF L"DailyTask_HukryongPrison_Boss_BuKa.cpp"
CDailyTask_HukryongPrison_Boss_BuKa::CDailyTask_HukryongPrison_Boss_BuKa(_In_ CONST CMonster& Boss, _In_ CONST CDailyTask_Move& DailyTask_Move)
	: CDailyTask_Boss_Template(Boss, DailyTask_Move, FixPointInfo(Point(-30584, -37064, 10496), 18.0f))
{

}

BOOL CDailyTask_HukryongPrison_Boss_BuKa::Kill()
{
	
	CONST std::vector<KillBossMethod> Vec =
	{
		{ em_Player_Classes_ForceMaster, std::bind(&CDailyTask_HukryongPrison_Boss_BuKa::KillBoss_ForceMaster, this) },
		{ em_Player_Classes_BladeMaster, std::bind(&CDailyTask_HukryongPrison_Boss_BuKa::KillBoss_BladeMaster, this) },
		{ em_Player_Classes_Assassin, std::bind(&CDailyTask_HukryongPrison_Boss_BuKa::KillBoss_Assassin, this) },
		{ em_Player_Classes_KongFuMaster, std::bind(&CDailyTask_HukryongPrison_Boss_BuKa::KillBoss_KongFuMaster, this) },
		{ em_Player_Classes_Warlock, std::bind(&CDailyTask_HukryongPrison_Boss_BuKa::KillBoss_Warlock, this) },
	};

	auto emPlayerClasses = CPerson::GetInstance().GetClasses();
	auto Element = MyTools::CLPublic::Vec_find_if_Const(Vec, [emPlayerClasses](CONST KillBossMethod& itm) { return itm.emPlayerClasses == emPlayerClasses; });
	if (Element != nullptr)
	{
		Element->MethodPtr();
	}
	return TRUE;
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::KillBoss_BladeMaster()
{
	auto& DailyTask_BossSkill = CDailyTask_BossSkill::GetInstance();
	DailyTask_BossSkill.SetMaxSameSkillTimeout(5 * 1000);
	CDailyTask_BossSkill::TickBossSkill Lastpr(0);
	while (GameRun && !_Boss.IsDead())
	{
		if (EMPTY_PERSONHP)
		{
			break;
		}

		//auto Pr = DailyTask_BossSkill.GetBossSkillId();
		auto Pr = DailyTask_BossSkill.GetUnSameBossSkillId();

		if (!Pr.first)
		{
			NormalMove();
			if (_dwLastSkillId == em_Skill_布卡_挥击)
			{
				if (Lastpr.SpendTime() < 1900)
				{
					_DailyTask_Skill.RefreshVecSkill();
					Skill_BladeMaster();
				}
				else
				{
					//LOG_CF_D(L"上一个技能是挥击,距离现在已经过去%d,不调用普通技能", Lastpr.SpendTime());
				}
			}
			else if (_dwLastSkillId == em_Skill_布卡_喵火流星)
			{
				if (Lastpr.SpendTime() < 2800)
				{
					_DailyTask_Skill.RefreshVecSkill();
					Skill_BladeMaster();
				}
				else
				{
					//LOG_CF_D(L"上一个技能是喵火流星,距离现在已经过去%d,不调用普通技能", Lastpr.SpendTime());
				}
			}
			else if (_dwLastSkillId == em_Skill_布卡_奇袭攻击)
			{
				if (Lastpr.SpendTime() < 900)
				{
					_DailyTask_Skill.RefreshVecSkill();
					Skill_BladeMaster();
				}
				else
				{
					//LOG_CF_D(L"上一个技能是奇袭攻击,距离现在已经过去%d,不调用普通技能", Lastpr.SpendTime());
				}
			}
			else if (_dwLastSkillId == em_Skill_布卡_连续攻击)
			{
				if (Lastpr.SpendTime() < 2500)
				{
					_DailyTask_Skill.RefreshVecSkill();
					Skill_BladeMaster();
				}
				else
				{
					//LOG_CF_D(L"上一个技能是连续攻击,距离现在已经过去%d,不调用普通技能", Lastpr.SpendTime());
				}
			}
			else if (_dwLastSkillId == em_Skill_布卡_猛捶)
			{
				if (Lastpr.SpendTime() < 4200)
				{
					_DailyTask_Skill.RefreshVecSkill();
					Skill_BladeMaster();
				}
				else
				{
					//LOG_CF_D(L"上一个技能是猛捶,距离现在已经过去%d,不调用普通技能", Lastpr.SpendTime());
				}
			}
			else if (_dwLastSkillId == em_Skill_布卡_骨面旋击)
			{
				if (Lastpr.SpendTime() < 3400)
				{
					_DailyTask_Skill.RefreshVecSkill();
					Skill_BladeMaster();
				}
				else
				{
					//LOG_CF_D(L"上一个技能是骨面旋击,距离现在已经过去%d,不调用普通技能", Lastpr.SpendTime());
				}
			}
			else if (_dwLastSkillId == em_Skill_布卡_斧之风)
			{
				if (Lastpr.SpendTime() < 4300)
				{
					_DailyTask_Skill.RefreshVecSkill();
					Skill_BladeMaster();
				}
				else
				{
					//LOG_CF_D(L"上一个技能是斧之风,距离现在已经过去%d,不调用普通技能", Lastpr.SpendTime());
				}
			}
			else if (_dwLastSkillId == em_Skill_布卡_旋转攻击)
			{
				_DailyTask_Skill.RefreshVecSkill();
				Skill_BladeMaster();
			}
			else
			{
				_DailyTask_Skill.RefreshVecSkill();
				Skill_BladeMaster();
			}
			continue;
		}

		auto dwBossSkillId = Pr.second.dwSkillId;

		// 
		_Boss.SetAngle();
		_DailyTask_Skill.RefreshVecSkill();

		//auto ulTick = ::GetTickCount64();
		//LOG_CF_D(L"Echo Skill[%X] 准备处理的时候已经用时=%d,Tick=%d", dwBossSkillId, static_cast<DWORD>(ulTick - Pr.second.ulTick), static_cast<DWORD>(Pr.second.ulTick));
		switch (dwBossSkillId)
		{
		case em_Skill_布卡_挥击:
			BossSkill_挥击_BladeMaster(Pr.second);
			break;
		case em_Skill_布卡_喵火流星:
			BossSkill_喵火流星_BladeMaster(Pr.second);
			break;
		case em_Skill_布卡_奇袭攻击:
			BossSkill_奇袭攻击_BladeMaster(Pr.second);
			break;
		case em_Skill_布卡_连续攻击:
			BossSkill_连续攻击_BladeMaster(Pr.second);
			break;
		case em_Skill_布卡_猛捶:
			BossSkill_猛捶_BladeMaster(Pr.second);
			break;
		case em_Skill_布卡_骨面旋击:
			BossSkill_骨面旋击_BladeMaster(Pr.second);
			break;
		case em_Skill_布卡_斧之风:
			BossSkill_斧之风_BladeMaster(Pr.second);
			break;
		case em_Skill_布卡_骨面呐喊:
			BossSkill_骨面呐喊_BladeMaster(Pr.second);
			break;
		case em_Skill_布卡_旋转攻击:
			BossSkill_旋转攻击_BladeMaster(Pr.second);
			break;
		default:
			break;
		}
		//LOG_CF_D(L"处理技能[%X] 用时%d, 总时长%d", dwBossSkillId, static_cast<DWORD>(::GetTickCount64() - ulTick), static_cast<DWORD>(::GetTickCount64() - Pr.second.ulTick));
		_dwLastSkillId = dwBossSkillId;
		Lastpr = Pr.second;
	}
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::Skill_BladeMaster()
{
	//auto nTick = ::GetTickCount64();
	//LOG_CF_D(L"开始Skill_BladeMaster=%d",static_cast<DWORD>(nTick));
	_Boss.SetAngle();
	
	DWORD dwMp = CPerson::GetInstance().GetMp();
	static MyTools::CTimeTick TimeTickZ;
	_DailyTask_Skill.RefreshVecSkill();
	if (_Boss.GetDis() >= 7)
	{
		if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_2))
		{
			//LOG_CF_D(L"突进靠近BOSS");
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_2, FALSE);
			//return;
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_2))
		{
			//LOG_CF_D(L"突袭靠近BOSS");
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_2, FALSE);
			//return;
		}
	}
	
	if (dwMp < 7 && _DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Tab))
	{
		//LOG_CF_D(L"满月斩转变成拔剑姿态");
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Tab, FALSE);
	}
	else if (dwMp < 7 && _DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_Tab))
	{
		//LOG_CF_D(L"满月斩转变成拔剑姿态");
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_Tab, FALSE);
	}
	else if (dwMp < 5 && _DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_2))
	{
		//LOG_CF_D(L"突进!");
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_2, FALSE);
		//CSomeFun::GetInstance().SimlationKey(VK_2, 0);
		//return;
	}
	else if (dwMp <= 5 && _DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_21))
	{
		//LOG_CF_D(L"突进!");
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_21, FALSE);
		//CSomeFun::GetInstance().SimlationKey(VK_2, 0);
		//return;
	}
	//仇恨BUFF-0x9D41EC

	if (CPerson::GetInstance().GetFightPose() == em_Player_Pose::em_Player_Pose_BladeMaster)
	{
		//LOG_CF_D(L"拔剑姿态输出中1%d", static_cast<DWORD>(::GetTickCount64() - nTick));
		if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_R5))
		{
			//LOG_CF_D(L"快剑");
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_R5, FALSE);
			//return;
		}
		else if (dwMp < 7 && TimeTickZ.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Second) >= 5 && _DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_Z)) // Z 和 C都是回蓝技能, 所以不能一起放！ 放了一个, 另外一个至少要10s后才能放!
		{
			//LOG_CF_D(L"山崩地裂");
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_Z, FALSE);
			TimeTickZ.Reset();
			//return;
		}
		else if (dwMp < 7 && TimeTickZ.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Second) >= 5 && _DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_C))
		{
			//LOG_CF_D(L"风月斩");
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_C, FALSE);
			TimeTickZ.Reset();
			//return;
		}
		//else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_T14))
		//{
		//	//LOG_CF_D(L"火龙连斩");
		//	_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_T14, FALSE);
		//	//return;
		//}
		//else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_T11))
		//{
		//	//LOG_CF_D(L"火龙连斩");
		//	_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_T11, FALSE);
		//	//return;
		//}
		//else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_T12))
		//{
		//	//LOG_CF_D(L"火龙连斩");
		//	_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_T12, FALSE);
		//	//return;
		//}
		//else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_T13))
		//{
		//	//LOG_CF_D(L"火龙连斩");
		//	_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_T13, FALSE);
		//	//return;
		//}
		//else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_T10))
		//{
		//	//LOG_CF_D(L"火龙连斩");
		//	_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_T10, FALSE);
		//	//return;
		//}
		//else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_T9))
		//{
		//	//LOG_CF_D(L"火龙连斩");
		//	_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_T9, FALSE);
		//	//return;
		//}
		//else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_T8))
		//{
		//	//LOG_CF_D(L"火龙连斩");
		//	_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_T8, FALSE);
		//	//return;
		//}
		//else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_T7))
		//{
		//	//LOG_CF_D(L"火龙连斩");
		//	_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_T7, FALSE);
		//	//return;
		//}
		//else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_T6))
		//{
		//	//LOG_CF_D(L"火龙连斩");
		//	_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_T6, FALSE);
		//	//return;
		//}
		//else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_T5))
		//{
		//	//LOG_CF_D(L"火龙连斩");
		//	_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_T5, FALSE);
		//	//return;
		//}
		//else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_T4))
		//{
		//	//LOG_CF_D(L"火龙连斩");
		//	_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_T4, FALSE);
		//	//return;
		//}
		//else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_T3))
		//{
		//	//LOG_CF_D(L"火龙连斩");
		//	_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_T3, FALSE);
		//	//return;
		//}
		//else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_T2))
		//{
		//	//LOG_CF_D(L"火龙连斩");
		//	_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_T2, FALSE);
		//	//return;
		//}
		//else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_T1))
		//{
		//	//LOG_CF_D(L"火龙连斩");
		//	_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_T1, FALSE);
		//	//return;
		//}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_V_X))
		{
			//LOG_CF_D(L"御剑连斩-释放火龙连斩BUFF");
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_V_X, FALSE);
			//return;
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_V_V))
		{
			//LOG_CF_D(L"回天令");
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_V_V, FALSE);
			//return;
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_V_Z))
		{
			//LOG_CF_D(L"爆剑令");
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_V_Z, FALSE);
			//return;
		}
		//if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_R1))
		//{
		//	//LOG_CF_D(L"拔剑");
		//	_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_R1, FALSE);
		//	//return;
		//}
		//else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_R2))
		//{
		//	//LOG_CF_D(L"拔剑");
		//	_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_R2, FALSE);
		//	//return;
		//}
		//else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_R3))
		//{
		//	//LOG_CF_D(L"拔剑");
		//	_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_R3, FALSE);
		//	//return;
		//}
		//else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_R4))
		//{
		//	//LOG_CF_D(L"拔剑");
		//	_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_R4, FALSE);
		//	//return;
		//}
		//auto nTick1 = ::GetTickCount64();
		//LOG_CF_D(L"拔剑姿态开始中");
		static CONST std::vector<DWORD> VecBuff = 
		{
			em_BladeMaster_火龙BUFF,em_BladeMaster_火龙BUFF1,em_BladeMaster_火龙BUFF2,
			em_BladeMaster_火龙BUFF3,em_BladeMaster_火龙BUFF4,em_BladeMaster_火龙BUFF5,
			em_BladeMaster_火龙BUFF6
		};
		if (CPersonFunction::GetInstance().ExistBuff_Person(VecBuff))
		{
			//LOG_CF_D(L"拔剑状态，不放R, SpendTime=%d", static_cast<DWORD>(::GetTickCount64() - nTick1));
			CSomeFun::GetInstance().SimlationKey_Immediately('T');
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_3_R))
		{
			//LOG_CF_D(L"拔剑状态，不放R SpendTime=%d", static_cast<DWORD>(::GetTickCount64() - nTick1));
			CSomeFun::GetInstance().SimlationKey_Immediately('T');
		}
		else
		{
			//LOG_CF_D(L"火龙连斩+T SpendTime=%d", static_cast<DWORD>(::GetTickCount64() - nTick1));
			CSomeFun::GetInstance().SimlationKey_Immediately('T');
			CSomeFun::GetInstance().SimlationKey_Immediately('R');
		}
		//LOG_CF_D(L"拔剑姿态输出中%d", static_cast<DWORD>(::GetTickCount64() - nTick1));
	}
	else
	{
		if (dwMp >= 8 && _DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_4))
		{
			//LOG_CF_D(L"焦热斩转变成拔剑姿态");
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_4, FALSE);
			//return;
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_1_R))
		{
			//LOG_CF_D(L"风火轮");
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_1_R, FALSE);
			//return;
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_F2))
		{
			//LOG_CF_D(L"火龙闪？");
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_F2, FALSE);
			//return;
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_F))
		{
			//LOG_CF_D(L"?");
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_F, FALSE);
			//return;
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_F1))
		{
			//LOG_CF_D(L"?");
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_F1, FALSE);
			//return;
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_V))
		{
			//LOG_CF_D(L"凤翼");
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_V, FALSE);
			//return;
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_V_V))
		{
			//LOG_CF_D(L"回天令");
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_V_V, FALSE);
			//return;
		}
		//else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_V_Z))
		//{
		//	//LOG_CF_D(L"爆剑令");
		//	_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_V_Z, FALSE);
		//	//return;
		//}
		else if (dwMp >5  && _DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_T1))
		{
			//LOG_CF_D(L"爆剑令");
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_T1, FALSE);
			//return;
		}
		else if (dwMp > 5 && _DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_T2))
		{
			//LOG_CF_D(L"爆剑令");
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_T2, FALSE);
			//return;
		}
		else if (dwMp > 5 && _DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_T3))
		{
			//LOG_CF_D(L"爆剑令");
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_T3, FALSE);
			//return;
		}
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_R7, FALSE);
		//LOG_CF_D(L"御剑姿态输出中2%d", static_cast<DWORD>(::GetTickCount64() - nTick));
	}
	
	//LOG_CF_D(L"输出结束%d", static_cast<DWORD>(::GetTickCount64() - nTick));
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_挥击_BladeMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	//LOG_CF_D(L"挥击");
	if (_Boss.GetDis() < 7)
	{
		if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_F))
		{
			//LOG_CF_D(L"击剑格挡");
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_F, TRUE);
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(500, [&]
			{
				_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_F, FALSE);
				return !_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_T7);
			});
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_1))
		{
			//LOG_CF_D(L"用1格挡");
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_1, TRUE);
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(500, [&]
			{
				_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_1, FALSE);
				return !_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_1_T);
			});
		}
		else
		{
			//LOG_CF_D(L"击剑理论不应该出现这种情况");
		}
	}
	//LOG_CF_D(L"挥击结束");
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_喵火流星_BladeMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	//LOG_CF_D(L"喵火流星");
	if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_Q))
	{
		//LOG_CF_D(L"Gesture_Q无敌去躲");
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_Q, TRUE);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Q))
	{
		//LOG_CF_D(L"Q无敌去躲");
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Q, TRUE);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_3))
	{
		//LOG_CF_D(L"3无敌去躲");
		TickBossSkill_.BeforeSleep(600);
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_3, TRUE);
		BnsSleep(1000);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_E))
	{
		//LOG_CF_D(L"Gesture_E无敌去躲");
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_E, TRUE);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_E))
	{
		//LOG_CF_D(L"E无敌去躲");
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_E, TRUE);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_SS))
	{
		//LOG_CF_D(L"Gesture_SS无敌去躲");
		CBaseObject::SetAngle(_Boss.GetFrontPoint(10));
		BnsSleep(100);
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_SS, TRUE);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_SS))
	{
		//LOG_CF_D(L"SS无敌去躲");
		CBaseObject::SetAngle(_Boss.GetFrontPoint(10));
		BnsSleep(100);
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_SS, TRUE);
	}
	else
	{
		//LOG_CF_D(L"喵火流星理论不应该出现这种情况,走路");
		CPersonFunction::GetInstance().Move(_Boss.GetBehindPoint(3.5));
	}
	//LOG_CF_D(L"喵火流星结束");
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_奇袭攻击_BladeMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	//LOG_CF_D(L"奇袭攻击");
	if (_Boss.GetDis() < 7)
	{
		if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_F))
		{
			//LOG_CF_D(L"击剑格挡");
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_F, TRUE);
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_1))
		{
			//LOG_CF_D(L"用1格挡");
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_1, TRUE);
		}
		else
		{
			//LOG_CF_D(L"奇袭攻击理论不应该出现这种情况");
		}
	}
	//LOG_CF_D(L"奇袭攻击结束");
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_连续攻击_BladeMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	//LOG_CF_D(L"连续攻击");
	//LOG_CF_D(L"用1格挡");
	_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_1, TRUE);
	BnsSleep(1000);
	/*if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_Q))
	{
		//LOG_CF_D(L"Gesture_Q无敌去躲");
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_Q, TRUE);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Q))
	{
		//LOG_CF_D(L"Q无敌去躲");
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Q, TRUE);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_3))
	{
		//LOG_CF_D(L"3无敌去躲");
		TickBossSkill_.BeforeSleep(500);
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_3, TRUE);
		BnsSleep(1000);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_E))
	{
		//LOG_CF_D(L"Gesture_E无敌去躲");
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_E, TRUE);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_E))
	{
		//LOG_CF_D(L"E无敌去躲");
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_E, TRUE);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_SS))
	{
		//LOG_CF_D(L"Gesture_SS无敌去躲");
		CBaseObject::SetAngle(_Boss.GetFrontPoint(10));
		BnsSleep(100);
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_SS, TRUE);
		BnsSleep(1000);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_SS))
	{
		//LOG_CF_D(L"SS无敌去躲");
		CBaseObject::SetAngle(_Boss.GetFrontPoint(10));
		BnsSleep(100);
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_SS, TRUE);
		BnsSleep(1000);
	}
	else
	{
		//LOG_CF_D(L"连续攻击-理论不应该出现这种情况");
	}*/
	//LOG_CF_D(L"连续攻击结束");
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_猛捶_BladeMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	//LOG_CF_D(L"猛捶开始");
	if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_Q))
	{
		//LOG_CF_D(L"Gesture_Q无敌去躲");
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_Q, TRUE);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Q))
	{
		//LOG_CF_D(L"Q无敌去躲");
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Q, TRUE);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_3))
	{
		//LOG_CF_D(L"3无敌去躲");
		TickBossSkill_.BeforeSleep(500);
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_3, TRUE);
		BnsSleep(1000);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_E))
	{
		//LOG_CF_D(L"Gesture_E无敌去躲");
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_E, TRUE);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_E))
	{
		//LOG_CF_D(L"E无敌去躲");
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_E, TRUE);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_SS))
	{
		//LOG_CF_D(L"Gesture_SS无敌去躲");
		CBaseObject::SetAngle(_Boss.GetFrontPoint(10));
		BnsSleep(100);
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_SS, TRUE);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_SS))
	{
		//LOG_CF_D(L"SS无敌去躲");
		CBaseObject::SetAngle(_Boss.GetFrontPoint(10));
		BnsSleep(100);
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_SS, TRUE);
	}
	else
	{
		//LOG_CF_D(L"猛捶-理论不应该出现这种情况");
	}
	//LOG_CF_D(L"猛捶结束");
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_骨面旋击_BladeMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	//LOG_CF_D(L"骨面旋击");
	if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_Q))
	{
		//LOG_CF_D(L"Gesture_Q无敌去躲");
		TickBossSkill_.BeforeSleep(500);
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_Q, TRUE);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Q))
	{
		//LOG_CF_D(L"Q无敌去躲");
		TickBossSkill_.BeforeSleep(500);
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Q, TRUE);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_3))
	{
		//LOG_CF_D(L"3无敌去躲");
		TickBossSkill_.BeforeSleep(200);
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_3, TRUE);
		BnsSleep(1000);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_E))
	{
		//LOG_CF_D(L"Gesture_E无敌去躲");
		TickBossSkill_.BeforeSleep(500);
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_E, TRUE);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_E))
	{
		//LOG_CF_D(L"E无敌去躲");
		TickBossSkill_.BeforeSleep(500);
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_E, TRUE);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_SS))
	{
		//LOG_CF_D(L"Gesture_SS无敌去躲");
		CBaseObject::SetAngle(_Boss.GetFrontPoint(10));
		BnsSleep(100);
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_SS, TRUE);
		BnsSleep(1000);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_SS))
	{
		//LOG_CF_D(L"SS无敌去躲");
		CBaseObject::SetAngle(_Boss.GetFrontPoint(10));
		BnsSleep(100);
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_SS, TRUE);
		BnsSleep(1000);
	}
	else
	{
		//LOG_CF_D(L"骨面旋击-理论不应该出现这种情况");
	}
	//LOG_CF_D(L"骨面旋击结束");
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_骨面猛击_BladeMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	//LOG_CF_D(L"骨面猛击");
	//TickBossSkill_.BeforeSleep(500);
	_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_1, TRUE);
	_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_1_F, TRUE);
	//if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_Q))
	//{
	//	//LOG_CF_D(L"Gesture_Q无敌去躲");
	//	TickBossSkill_.BeforeSleep(500);
	//	_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_Q, TRUE);
	//}
	//else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Q))
	//{
	//	//LOG_CF_D(L"Q无敌去躲");
	//	TickBossSkill_.BeforeSleep(500);
	//	_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Q, TRUE);
	//}
	//else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_3))
	//{
	//	//LOG_CF_D(L"3无敌去躲");
	//	TickBossSkill_.BeforeSleep(200);
	//	_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_3, TRUE);
	//	BnsSleep(1000);
	//}
	//else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_E))
	//{
	//	//LOG_CF_D(L"Gesture_E无敌去躲");
	//	TickBossSkill_.BeforeSleep(500);
	//	_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_E, TRUE);
	//}
	//else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_E))
	//{
	//	//LOG_CF_D(L"E无敌去躲");
	//	TickBossSkill_.BeforeSleep(500);
	//	_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_E, TRUE);
	//}
	//else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_SS))
	//{
	//	//LOG_CF_D(L"Gesture_SS无敌去躲");
	//	CBaseObject::SetAngle(_Boss.GetFrontPoint(10));
	//	BnsSleep(100);
	//	_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_SS, TRUE);
	//	BnsSleep(1000);
	//}
	//else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_SS))
	//{
	//	//LOG_CF_D(L"SS无敌去躲");
	//	CBaseObject::SetAngle(_Boss.GetFrontPoint(10));
	//	BnsSleep(100);
	//	_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_SS, TRUE);
	//	BnsSleep(1000);
	//}
	//else
	//{
	//	//LOG_CF_D(L"骨面猛击-理论不应该出现这种情况");
	//}
	//LOG_CF_D(L"骨面猛击结束");
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_斧之风_BladeMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	//LOG_CF_D(L"斧之风");
	if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_Q))
	{
		//LOG_CF_D(L"Gesture_Q无敌去躲");
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_Q, TRUE);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Q))
	{
		//LOG_CF_D(L"Q无敌去躲");
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Q, TRUE);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_E))
	{
		//LOG_CF_D(L"Gesture_E无敌去躲");
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_E, TRUE);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_E))
	{
		//LOG_CF_D(L"E无敌去躲");
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_E, TRUE);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_SS))
	{
		//LOG_CF_D(L"Gesture_SS无敌去躲");
		CBaseObject::SetAngle(_Boss.GetFrontPoint(10));
		BnsSleep(100);
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Gesture_SS, TRUE);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_SS))
	{
		//LOG_CF_D(L"SS无敌去躲");
		CBaseObject::SetAngle(_Boss.GetFrontPoint(10));
		BnsSleep(100);
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_SS, TRUE);
	}
	else
	{
		//LOG_CF_D(L"斧之风-理论不应该出现这种情况");
	}
	//LOG_CF_D(L"斧之风结束");
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_骨面呐喊_BladeMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	//LOG_CF_D(L"骨面呐喊");
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_旋转攻击_BladeMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	//LOG_CF_D(L"旋转攻击上个技能是%d", _dwLastSkillId);
	//上个技能是5903258执行虚弱
	if (_dwLastSkillId == 5903258)
	{
		//LOG_CF_D(L"执行虚弱");
		//_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Tab, TRUE);
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_C, TRUE);
	}
	//上个技能是5903259执行击倒
	else if (_dwLastSkillId == 5903259)
	{
		//LOG_CF_D(L"执行击倒");
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Z, TRUE);
		if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_3))
		{
			//LOG_CF_D(L"执行下段斩");
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Z, TRUE);
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_3, TRUE);
		}
		if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_V_Z))
		{
			//LOG_CF_D(L"当前是御剑状态，能遍历到爆剑令");
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Z, TRUE);
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_3, TRUE);
		}
		if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_V_Z))
		{
			//LOG_CF_D(L"当前是拔剑状态，能遍历到爆剑令");
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Z, TRUE);
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_3, TRUE);
		}
		if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_Gesture_3))
		{
			//LOG_CF_D(L"当前可以遍历到五连斩");
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_1, TRUE);
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Z, TRUE);
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_3, TRUE);
		}
		else
		{
			//LOG_CF_D(L"没有技能可以使用");
		}
	}
	//上个技能是??执行眩晕
	else
	{
		//LOG_CF_D(L"执行眩晕");
		
		if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(BladeMaster_SkillId_X))
		{
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_X, TRUE);
		}
		else
		{
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_1, TRUE);
			_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_X, TRUE);
		}
	}
	//LOG_CF_D(L"旋转攻击结束");
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::KillBoss_ForceMaster()
{
	auto& DailyTask_BossSkill = CDailyTask_BossSkill::GetInstance();
	while (GameRun && !_Boss.IsDead())
	{
		if (EMPTY_PERSONHP)
		{
			break;
		}

		auto Pr = DailyTask_BossSkill.GetUnSameBossSkillId();
		if (!Pr.first)
		{
			NormalMove();
			_DailyTask_Skill.RefreshVecSkill();

			_Boss.SetAngle();
			Skill_ForceMaster();
			if (_dwLastSkillId == em_Skill_布卡_旋转攻击)
			{
				BnsSleep(3000);
				//LOG_CF_D(L"上个技能是旋转攻击，我是气功，过去干小怪。");
				KillFish();
			}
			continue;
		}

		auto dwBossSkillId = Pr.second.dwSkillId;

		// 
		_Boss.SetAngle();
		_DailyTask_Skill.RefreshVecSkill();

		////LOG_CF_D(L"Echo Skill[%X] Tick=%d,Tick=%d", dwBossSkillId, static_cast<DWORD>(ulTick - Pr.second.ulTick), static_cast<DWORD>(Pr.second.ulTick));
		switch (dwBossSkillId)
		{
		case em_Skill_布卡_挥击:
			BossSkill_挥击_ForceMaster(Pr.second);
			break;
		case em_Skill_布卡_骨面猛击:
			BossSkill_骨面猛击_ForceMaster(Pr.second);
			break;
		case em_Skill_布卡_旋转攻击:
			BossSkill_旋转攻击_ForceMaster(Pr.second);
			break;
		default:
			break;
		}
		_dwLastSkillId = dwBossSkillId;
	}
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::Skill_ForceMaster()
{
	DWORD dwMp = CPerson::GetInstance().GetMp();

	static MyTools::CTimeTick TimeTickIce(0);
	static MyTools::CTimeTick TimeTick4(0);
	MyTools::CLEchoException::GetInstance().InvokeAction(__FUNCTIONW__, [&]
	{
		
		if (TimeTick4.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Second) < 1)
		{
			return;
		}
		if (TimeTick4.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Second) < 7 && CPerson::GetInstance().IsSustainabilityByUseSkill())
		{
			return;
		}


		if (dwMp >= 2 && _DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(ForceMaster_SkillId_4_4))
		{
		//LOG_CF_D(L"重力掌!");
			_DailyTask_Skill.UseSkill_By_SkillId(ForceMaster_SkillId_4_4, FALSE);
			TimeTick4.Reset();
			return;
		}
		if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(ForceMaster_SkillId_Ice_F))
		{
		//LOG_CF_D(L"冰龙破!");
			_DailyTask_Skill.UseSkill_By_SkillId(ForceMaster_SkillId_Ice_F, FALSE);
			return;
		}
		if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(ForceMaster_SkillId_Fure_F))
		{
		//LOG_CF_D(L"炎龙破!");
			_DailyTask_Skill.UseSkill_By_SkillId(ForceMaster_SkillId_Fure_F, FALSE);
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
			if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(ForceMaster_SkillId_Ice_F))
			{
			//LOG_CF_D(L"冰龙破!");
				_DailyTask_Skill.UseSkill_By_SkillId(ForceMaster_SkillId_Ice_F, FALSE);
				return;
			}
			if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(ForceMaster_SkillId_Ice_Z))
			{
			//LOG_CF_D(L"破冰!");
				_DailyTask_Skill.UseSkill_By_SkillId(ForceMaster_SkillId_Ice_Z, FALSE);
				return;
			}
			if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(ForceMaster_SkillId_Ice_X))
			{
			//LOG_CF_D(L"冰河掌!");
				_DailyTask_Skill.UseSkill_By_SkillId(ForceMaster_SkillId_Ice_X, FALSE);
				return;
			}

		//LOG_CF_D(L"冰系技能已经用完了!");
			TimeTickIce.Reset();
		}

		// 判断BOSS身上的火种叠够5层就用1引爆
		if (CMonsterExtend::GetInstance().ExistMonsterBuff(_Boss.GetId(), ForceMaster_Buff_火种, 5))
		{
		//LOG_CF_D(L"火种叠够5层就用1引爆");
			CSomeFun::GetInstance().SimlationKey(VK_1, 0);
			return;
		}

		if (dwMp >= 2 && _DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(ForceMaster_SkillId_Fure_X))
		{
		//LOG_CF_D(L"火莲掌!");
			_DailyTask_Skill.UseSkill_By_SkillId(ForceMaster_SkillId_Fure_X, FALSE);
			return;
		}
		if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(ForceMaster_SkillId_Fure_C))
		{
		//LOG_CF_D(L"莲花指!");
			_DailyTask_Skill.UseSkill_By_SkillId(ForceMaster_SkillId_Fure_C, FALSE);
			return;
		}
		if (dwMp >= 2 && _DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(ForceMaster_SkillId_Fure_V))
		{
		//LOG_CF_D(L"流星指!");
			_DailyTask_Skill.UseSkill_By_SkillId(ForceMaster_SkillId_Fure_V, FALSE);
			return;
		}
		if (dwMp >= 2 && _DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(ForceMaster_SkillId_2))
		{
		//LOG_CF_D(L"爆裂炎炮!");
			_DailyTask_Skill.UseSkill_By_SkillId(ForceMaster_SkillId_2, FALSE);
			return;
		}

		if (_DailyTask_Skill.IsCool_By_SkillId_In_HideUi(ForceMaster_SkillId_Fure_F))
		{
		//LOG_CF_D(L"用R引出炎龙破");
			CSomeFun::GetInstance().SimlationKey('R', 0);
			return;
		}
		if (dwMp >= 1 && _DailyTask_Skill.IsCool_By_SkillId_In_HideUi(ForceMaster_SkillId_Ice_F))
		{
		//LOG_CF_D(L"用T引出冰龙破");
			CSomeFun::GetInstance().SimlationKey('T', 0);
			return;
		}
		

		CSomeFun::GetInstance().SimlationKey('R', 0);
	});
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_挥击_ForceMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{

}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_骨面猛击_ForceMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	LOG_CF_D(L"气功骨面猛击");
	TickBossSkill_.BeforeSleep(500);
	if (_DailyTask_Skill.IsCool_By_SkillId_In_HideUi(ForceMaster_SkillId_Q))
	{
		LOG_CF_D(L"气功骨面猛击ForceMaster_SkillId_Q");
		_DailyTask_Skill.UseSkill_By_SkillId(ForceMaster_SkillId_Q, TRUE);
	}
	
	else if (_DailyTask_Skill.IsCool_By_SkillId_In_HideUi(ForceMaster_SkillId_SS))
	{
		LOG_CF_D(L"气功骨面猛击ForceMaster_SkillId_SS");
		_DailyTask_Skill.UseSkill_By_SkillId(ForceMaster_SkillId_SS, TRUE);
	}
	else
	{
		LOG_CF_D(L"没有可用技能");
	}
	BnsSleep(1000);
	LOG_CF_D(L"气功骨面猛击结束");
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_旋转攻击_ForceMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	//LOG_CF_D(L"旋转攻击上个技能是%d", _dwLastSkillId);
	//上个技能是5903258执行虚弱
	if (_dwLastSkillId == 5903258)
	{
		//LOG_CF_D(L"执行虚弱，我是气功不管");
		//_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Tab, TRUE);
		//_DailyTask_Skill.UseSkill_By_SkillId(ForceMaster_SkillId_Fure_Z, TRUE);
	}
	//上个技能是5903259执行击倒
	else if (_dwLastSkillId == 5903259)
	{
		//LOG_CF_D(L"执行击倒,我是气功执行Z");
		_DailyTask_Skill.UseSkill_By_SkillId(ForceMaster_SkillId_Fure_Z, TRUE);
		_DailyTask_Skill.UseSkill_By_SkillId(ForceMaster_SkillId_Fure_Z_Z, TRUE);
	}
	//上个技能是??执行眩晕
	else
	{
		LOG_CF_D(L"执行眩晕,我是气功执行3");
		_DailyTask_Skill.UseSkill_By_SkillId(ForceMaster_SkillId_3, TRUE);
	}
	
	//LOG_CF_D(L"旋转攻击结束");
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::KillBoss_Warlock()
{
	auto& DailyTask_BossSkill = CDailyTask_BossSkill::GetInstance();
	while (GameRun && !_Boss.IsDead())
	{
		if (EMPTY_PERSONHP)
		{
			break;
		}

		auto Pr = DailyTask_BossSkill.GetUnSameBossSkillId();
		if (!Pr.first)
		{
			NormalMove();
			_DailyTask_Skill.RefreshVecSkill();

			_Boss.SetAngle();
			Skill_Warlock();
			if (_dwLastSkillId == em_Skill_布卡_旋转攻击)
			{
				BnsSleep(3000);
				//LOG_CF_D(L"上个技能是旋转攻击，我是气功，过去干小怪。");
				KillFish();
			}
			continue;
		}

		auto dwBossSkillId = Pr.second.dwSkillId;

		// 
		_Boss.SetAngle();
		_DailyTask_Skill.RefreshVecSkill();

		////LOG_CF_D(L"Echo Skill[%X] Tick=%d,Tick=%d", dwBossSkillId, static_cast<DWORD>(ulTick - Pr.second.ulTick), static_cast<DWORD>(Pr.second.ulTick));
		switch (dwBossSkillId)
		{
		case em_Skill_布卡_挥击:
			BossSkill_挥击_Warlock(Pr.second);
			break;
		case em_Skill_布卡_骨面猛击:
			BossSkill_骨面猛击_Warlock(Pr.second);
			break;
		case em_Skill_布卡_旋转攻击:
			BossSkill_旋转攻击_Warlock(Pr.second);
			break;
		default:
			break;
		}
		_dwLastSkillId = dwBossSkillId;
	}
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::Skill_Warlock()
{
	auto nTick = ::GetTickCount64();
	LOG_CF_D(L"开始Skill_Warlock=%d", nTick);
	_Boss.SetAngle();

	DWORD dwMp = CPerson::GetInstance().GetMp();
	_DailyTask_Skill.RefreshVecSkill();
	//1B77402 超神BUFF公共CD
	
	if (!CPersonFunction::GetInstance().ExistBuff_Person(0x1B77402, nullptr) && GameRun)
	{
		if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Warlock_SkillId_Tab_God))
		{
			LOG_CF_D(L"Warlock_SkillId_Tab_God可以释放");
			CSomeFun::GetInstance().SimlationKey(VK_TAB, 0);
			Sleep(500);
		}
	}
	if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Warlock_SkillId_Tab))
	{
		LOG_CF_D(L"Warlock_SkillId_Tab可以释放");
		_DailyTask_Skill.UseSkill_By_SkillId(Warlock_SkillId_Tab, FALSE);
		Sleep(1500);
	}
	else if (_DailyTask_Skill.IsCool_By_SkillId_In_Ui(Warlock_SkillId_G))
	{
		LOG_CF_D(L"G");
		_DailyTask_Skill.UseSkill_By_SkillId(Warlock_SkillId_G, FALSE);
		Sleep(2000);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_KeyText_In_Ui(L"4"))
	{
		//1AB3F71 3个可以释放4
		LOG_CF_D( L"4");
		CSomeFun::GetInstance().SimlationKey(VK_4, 0);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Warlock_SkillId_2))
	{
		LOG_CF_D( L"2");
		_DailyTask_Skill.UseSkill_By_SkillId(Warlock_SkillId_2, FALSE);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Warlock_SkillId_3))
	{
		LOG_CF_D( L"3");
		_DailyTask_Skill.UseSkill_By_SkillId(Warlock_SkillId_3, FALSE);
		Sleep(1500);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_KeyText_In_Ui(L"V"))
	{
		LOG_CF_D( L"V");
		CSomeFun::GetInstance().SimlationKey('V', 0);
	}
	//else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Warlock_SkillId_Q))
	//{
	//	//LOG_CF_D( L"召唤兽Q");
	//	CSomeFun::GetInstance().SimlationKey('Q', 0);
	//}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Warlock_SkillId_C))
	{
		LOG_CF_D( L"降魔阵");
		_DailyTask_Skill.UseSkill_By_SkillId(Warlock_SkillId_C, FALSE);
	}
	else if (dwMp > 3)
	{
		LOG_CF_D(L"RTF");
		CSomeFun::GetInstance().SimlationKey('T', 0);
		CSomeFun::GetInstance().SimlationKey('R', 0);
		CSomeFun::GetInstance().SimlationKey('F', 0);
	}
	else
	{
		LOG_CF_D(L"R");
		CSomeFun::GetInstance().SimlationKey('R', 0);
	}
	LOG_CF_D(L"结束Skill_Warlock=%d", static_cast<DWORD>(::GetTickCount64() - nTick));
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_挥击_Warlock(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{

}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_骨面猛击_Warlock(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{

}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_旋转攻击_Warlock(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	//LOG_CF_D(L"旋转攻击上个技能是%d", _dwLastSkillId);
	//上个技能是5903258执行虚弱
	if (_dwLastSkillId == 5903258)
	{
		LOG_CF_D(L"执行虚弱，我是咒术师执行X");
		_DailyTask_Skill.UseSkill_By_SkillId(Warlock_SkillId_X, TRUE);
	}
	//上个技能是5903259执行击倒
	else if (_dwLastSkillId == 5903259)
	{
		//LOG_CF_D(L"执行击倒,我是咒术师执行Z");
	}
	//上个技能是??执行眩晕
	else
	{
		//LOG_CF_D(L"执行眩晕,我是咒术师执行");
	}

	//LOG_CF_D(L"旋转攻击结束");
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::KillBoss_Assassin()
{
	auto& DailyTask_BossSkill = CDailyTask_BossSkill::GetInstance();
	DailyTask_BossSkill.SetMaxSameSkillTimeout(5 * 1000);
	CDailyTask_BossSkill::TickBossSkill Lastpr(0);
	while (GameRun && !_Boss.IsDead())
	{
		if (EMPTY_PERSONHP)
		{
			break;
		}

		//auto Pr = DailyTask_BossSkill.GetBossSkillId();
		auto Pr = DailyTask_BossSkill.GetUnSameBossSkillId();

		if (!Pr.first)
		{
			NormalMove();
			if (_dwLastSkillId == em_Skill_布卡_挥击)
			{
				if (Lastpr.SpendTime() < 1900)
				{
					_DailyTask_Skill.RefreshVecSkill();
					Skill_Assassin();
				}
				else
				{
					LOG_CF_D(L"上一个技能是挥击,距离现在已经过去%d,不调用普通技能", Lastpr.SpendTime());
				}
			}
			else if (_dwLastSkillId == em_Skill_布卡_喵火流星)
			{
				if (Lastpr.SpendTime() < 2500)
				{
					_DailyTask_Skill.RefreshVecSkill();
					Skill_Assassin();
				}
				else
				{
					LOG_CF_D(L"上一个技能是喵火流星,距离现在已经过去%d,不调用普通技能", Lastpr.SpendTime());
				}
			}
			else if (_dwLastSkillId == em_Skill_布卡_奇袭攻击)
			{
				if (Lastpr.SpendTime() < 1000)
				{
					_DailyTask_Skill.RefreshVecSkill();
					Skill_Assassin();
				}
				else
				{
					LOG_CF_D(L"上一个技能是奇袭攻击,距离现在已经过去%d,不调用普通技能", Lastpr.SpendTime());
				}
			}
			else if (_dwLastSkillId == em_Skill_布卡_连续攻击)
			{
				if (Lastpr.SpendTime() < 2500)
				{
					_DailyTask_Skill.RefreshVecSkill();
					Skill_Assassin();
				}
				else
				{
					LOG_CF_D(L"上一个技能是连续攻击,距离现在已经过去%d,不调用普通技能", Lastpr.SpendTime());
				}
			}
			else if (_dwLastSkillId == em_Skill_布卡_猛捶)
			{
				if (Lastpr.SpendTime() < 4000)
				{
					_DailyTask_Skill.RefreshVecSkill();
					Skill_Assassin();
				}
				else
				{
					LOG_CF_D(L"上一个技能是猛捶,距离现在已经过去%d,不调用普通技能", Lastpr.SpendTime());
				}
			}
			else if (_dwLastSkillId == em_Skill_布卡_骨面旋击)
			{
				if (Lastpr.SpendTime() < 3400)
				{
					_DailyTask_Skill.RefreshVecSkill();
					Skill_Assassin();
				}
				else
				{
					LOG_CF_D(L"上一个技能是骨面旋击,距离现在已经过去%d,不调用普通技能", Lastpr.SpendTime());
				}
			}
			else if (_dwLastSkillId == em_Skill_布卡_斧之风)
			{
				if (Lastpr.SpendTime() < 5000)
				{
					_DailyTask_Skill.RefreshVecSkill();
					Skill_Assassin();
				}
				else
				{
					LOG_CF_D(L"上一个技能是斧之风,距离现在已经过去%d,不调用普通技能", Lastpr.SpendTime());
				}
			}
			else if (_dwLastSkillId == em_Skill_布卡_旋转攻击)
			{
				_DailyTask_Skill.RefreshVecSkill();
				Skill_Assassin();
			}
			else
			{
				_DailyTask_Skill.RefreshVecSkill();
				Skill_Assassin();
			}
			continue;
		}

		auto dwBossSkillId = Pr.second.dwSkillId;

		// 
		_Boss.SetAngle();
		_DailyTask_Skill.RefreshVecSkill();

		auto ulTick = ::GetTickCount64();
		LOG_CF_D(L"Echo Skill[%X] 准备处理的时候已经用时=%d,Tick=%d", dwBossSkillId, static_cast<DWORD>(ulTick - Pr.second.ulTick), static_cast<DWORD>(Pr.second.ulTick));
		switch (dwBossSkillId)
		{
		case em_Skill_布卡_挥击:
			BossSkill_挥击_Assassin(Pr.second);
			break;
		case em_Skill_布卡_喵火流星:
			BossSkill_喵火流星_Assassin(Pr.second);
			break;
		case em_Skill_布卡_奇袭攻击:
			BossSkill_奇袭攻击_Assassin(Pr.second);
			break;
		case em_Skill_布卡_连续攻击:
			BossSkill_连续攻击_Assassin(Pr.second);
			break;
		case em_Skill_布卡_猛捶:
			BossSkill_猛捶_Assassin(Pr.second);
			break;
		case em_Skill_布卡_骨面旋击:
			BossSkill_骨面旋击_Assassin(Pr.second);
			break;
		case em_Skill_布卡_斧之风:
			BossSkill_斧之风_Assassin(Pr.second);
			break;
		case em_Skill_布卡_骨面呐喊:
			BossSkill_骨面呐喊_Assassin(Pr.second);
			break;
		case em_Skill_布卡_旋转攻击:
			BossSkill_旋转攻击_Assassin(Pr.second);
			break;
		default:
			break;
		}
		LOG_CF_D(L"处理技能[%X] 用时%d, 总时长%d", dwBossSkillId, static_cast<DWORD>(::GetTickCount64() - ulTick), static_cast<DWORD>(::GetTickCount64() - Pr.second.ulTick));
		_dwLastSkillId = dwBossSkillId;
		Lastpr = Pr.second;
	}
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::Skill_Assassin()
{
	auto nTick = ::GetTickCount64();
	LOG_CF_D(L"开始Skill_Assassin=%d", nTick);
	_Boss.SetAngle();

	DWORD dwMp = CPerson::GetInstance().GetMp();
	static MyTools::CTimeTick TimeTickZ;
	_DailyTask_Skill.RefreshVecSkill();
	if (_Boss.GetDis() >= 7)
	{
		if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_Tab2))
		{
			LOG_CF_D(L"木叶移行");
			_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_Tab2, FALSE);
			//return;
		}
		//else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_Hide_3))
		//{
		//	LOG_CF_D(L"无影步");
		//	_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_Hide_3, FALSE);
		//}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_1_1))
		{
			LOG_CF_D(L"潜行靠近BOSS");
			_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_1_1, FALSE);
			//return;
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_1_2))
		{
			LOG_CF_D(L"潜行靠近BOSS");
			_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_1_2, FALSE);
			//return;
		}
	}

	if (CPerson::GetInstance().GetFightPose() == em_Player_Pose_Assassin_Hide)
	{
		if (dwMp < 5)
		{
			if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_Hide_11))
			{
				LOG_CF_D(L"mp少于3吸影恢复蓝2");
				_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_Hide_11, FALSE);
			}
			else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_Hide_1))
			{
				LOG_CF_D(L"mp少于3吸影恢复蓝1");
				_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_Hide_1, FALSE);
			}
			else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_Hide_X))
			{
				LOG_CF_D(L"影匕");
				_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_Hide_X, FALSE);
			}
			else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_Hide_X1))
			{
				LOG_CF_D(L"mp少于3 踏影术");
				_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_Hide_X1, FALSE);
			}
			else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_Hide_4))
			{
				LOG_CF_D(L"mp少于3雷决恢复蓝");
				_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_Hide_4, FALSE);
			}
			
			else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_Hide_X2))
			{
				LOG_CF_D(L"mp少于3淬毒匕首|毒匕首");
				_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_Hide_X2, FALSE);
			}
			else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_Hide_2))
			{
				LOG_CF_D(L"mp少于3长线火雷");
				_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_Hide_2, FALSE);
			}
			else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_Hide_Tab))
			{
				LOG_CF_D(L"mp少于3无影脚");
				_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_Hide_Tab, FALSE);
			}
			else
			{
				LOG_CF_D(L"刺客没有技能可以使用");
			}
		}
		else
		{
			LOG_CF_D(L"5层毒,隐身中可以直接放刺心");
			CSomeFun::GetInstance().SimlationKey('F', 0);
			CSomeFun::GetInstance().SimlationKey('T', 0);
		}
	}
	else
	{
		if (CMonsterExtend::GetInstance().ExistMonsterBuff(_Boss.GetId(), Assassin_Buff_毒, 5))
		{
			LOG_CF_D(L"5层毒");
			if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_V))
			{
				LOG_CF_D(L"斗志昂扬");
				_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_V, FALSE);
			}
			if (dwMp < 4)
			{
				LOG_CF_D(L"5层毒时MP低,R1");
				_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_R1_1, FALSE);
			}
			else
			{
				LOG_CF_D(L"5层毒时F-T-R");
				CSomeFun::GetInstance().SimlationKey('F', 0);
				CSomeFun::GetInstance().SimlationKey('T', 0);
				CSomeFun::GetInstance().SimlationKey('R', 0);
			}
		}
		else
		{
			if (CMonsterExtend::GetInstance().ExistMonsterBuff(_Boss.GetId(), Assassin_Buff_毒, 4) ||
				CMonsterExtend::GetInstance().ExistMonsterBuff(_Boss.GetId(), Assassin_Buff_毒, 3) ||
				CMonsterExtend::GetInstance().ExistMonsterBuff(_Boss.GetId(), Assassin_Buff_毒, 2) ||
				CMonsterExtend::GetInstance().ExistMonsterBuff(_Boss.GetId(), Assassin_Buff_毒, 1))
			{
				if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_Tab))
				{
					LOG_CF_D(L"回身踢1");
					_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_Tab, FALSE);
				}
				else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_Tab1))
				{
					LOG_CF_D(L"回身踢2");
					_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_Tab1, FALSE);
				}
			}
			if (dwMp > 4)
			{
				if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_T5))
				{
					LOG_CF_D(L"毒刺");
					_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_T5, FALSE);
				}
				else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_1_1))
				{
					LOG_CF_D(L"潜行叠毒1");
					_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_1_1, FALSE);
					//return;
				}
				else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_1_2))
				{
					LOG_CF_D(L"潜行叠毒2");
					_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_1_2, FALSE);
					//return;
				}
				else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_4))
				{
					LOG_CF_D(L"投掷毒物|吹毒");
					_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_4, FALSE);
				}
				else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_3))
				{
					LOG_CF_D(L"剧毒斩");
					_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_3, FALSE);
				}
				else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_Hide_X2))
				{
					LOG_CF_D(L"mp少于3淬毒匕首|毒匕首");
					_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_Hide_X2, FALSE);
				}
				/*else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_Z))
				{
				LOG_CF_D(L"冲天雷");
				_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_Z, FALSE);
				}
				else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_Z_1))
				{
				LOG_CF_D(L"冲天雷-爆炸");
				_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_Z_1, FALSE);
				}*/
				else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_Z))
				{
					LOG_CF_D(L"掷雷");
					_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_Z, FALSE);
				}
				//CSomeFun::GetInstance().SimlationKey('R', 0);
			}
			_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_R1_1, FALSE);
		}
	}
	LOG_CF_D(L"结束Skill_Assassin=%d", static_cast<DWORD>(::GetTickCount64() - nTick));;
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_挥击_Assassin(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	LOG_CF_D(L"刺客-挥击");
	if (IsFaceToBoss(8))
	{
		if (_Boss.GetTargetId() == CPerson::GetInstance().GetId())
		{
			if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_SS_F))
			{
				LOG_CF_D(L"木叶疾斩");
				_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_SS_F, TRUE);
			}
			else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_2))
			{
				LOG_CF_D(L"替身术");
				_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_2, TRUE);
				CSomeFun::GetInstance().TimeOut_Condiction_GameRun(500, [&]
				{
					LOG_CF_D(L"挥击-替身术-%d", CPerson::GetInstance().GetFightPose());
					return CPerson::GetInstance().GetFightPose() != em_Player_Pose_Assassin_Hide;
				});
			}
			else
			{
				LOG_CF_D(L"刺客-没有技能可以使用");
			}
		}
		else
		{
			LOG_CF_D(L"刺客-目标不是我,不用管.");
		}
	}
	else
	{
		LOG_CF_D(L"刺客-挥击背面");
	}
	LOG_CF_D(L"刺客-挥击结束");
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_喵火流星_Assassin(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	LOG_CF_D(L"刺客-喵火流星");
	if (IsFaceToBoss(8))
	{
		if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_SS_F))
		{
			LOG_CF_D(L"木叶疾斩");
			_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_SS_F, TRUE);
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_Q))
		{
			LOG_CF_D(L"Q");
			_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_Q, TRUE);
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_E))
		{
			LOG_CF_D(L"E");
			_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_E, TRUE);
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_SS))
		{
			LOG_CF_D(L"SS");
			CBaseObject::SetAngle(_Boss.GetFrontPoint(10));
			BnsSleep(100);
			_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_SS, TRUE);
		}
		else
		{
			LOG_CF_D(L"刺客-走到背后");
			CPersonFunction::GetInstance().Move(_Boss.GetBehindPoint(4));
		}
	}
	LOG_CF_D(L"刺客-喵火流星结束");
	
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_奇袭攻击_Assassin(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	LOG_CF_D(L"刺客-奇袭攻击");
	//if (IsFaceToBoss(8))
	//{
	//	if (_Boss.GetTargetId() == CPerson::GetInstance().GetId())
	//	{
	//		if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_SS_F))
	//		{
	//			LOG_CF_D(L"木叶疾斩");
	//			_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_SS_F, TRUE);
	//		}
	//		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_Q))
	//		{
	//			LOG_CF_D(L"Q");
	//			_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_Q, TRUE);
	//		}
	//		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_E))
	//		{
	//			LOG_CF_D(L"E");
	//			_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_E, TRUE);
	//		}
	//		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_G))
	//		{
	//			LOG_CF_D(L"夜行");
	//			_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_G, TRUE);
	//			_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_G1, TRUE);
	//		}
	//		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_B))
	//		{
	//			LOG_CF_D(L"禁影步");
	//			_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_B, TRUE);
	//		}
	//		else
	//		{
	//			LOG_CF_D(L"刺客-没有技能可以使用");
	//			//CPersonFunction::GetInstance().Move(_Boss.GetBehindPoint(4));
	//		}
	//	}
	//	else
	//	{
	//		LOG_CF_D(L"目标不是我-刺客");
	//	}
	//}
	LOG_CF_D(L"刺客-奇袭攻击结束");
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_连续攻击_Assassin(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	LOG_CF_D(L"刺客-连续攻击");
	if (IsFaceToBoss(8))
	{
		if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_SS_F))
		{
			LOG_CF_D(L"木叶疾斩");
			_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_SS_F, TRUE);
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_2))
		{
			LOG_CF_D(L"替身术");
			_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_2, TRUE);
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(500, [&]
			{
				LOG_CF_D(L"连续攻击-替身术-%d", CPerson::GetInstance().GetFightPose());
				return CPerson::GetInstance().GetFightPose() != em_Player_Pose_Assassin_Hide;
			});
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_Q))
		{
			LOG_CF_D(L"刺客-Q");
			_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_Q, TRUE);
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_E))
		{
			LOG_CF_D(L"刺客-E");
			_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_E, TRUE);
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_SS))
		{
			LOG_CF_D(L"刺客-SS");
			CBaseObject::SetAngle(_Boss.GetFrontPoint(10));
			BnsSleep(150);
			_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_SS, TRUE);
			BnsSleep(500);
		}
		else
		{
			LOG_CF_D(L"刺客-没有技能可以使用");
			//CPersonFunction::GetInstance().Move(_Boss.GetBehindPoint(4));
		}
	}
	LOG_CF_D(L"刺客-连续攻击结束");
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_猛捶_Assassin(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	LOG_CF_D(L"刺客-猛捶");
	if (IsFaceToBoss(8))
	{
		if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_SS_F))
		{
			LOG_CF_D(L"木叶疾斩");
			_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_SS_F, TRUE);
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_Q))
		{
			LOG_CF_D(L"刺客-Q");
			_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_Q, TRUE);
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_E))
		{
			LOG_CF_D(L"刺客-E");
			_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_E, TRUE);
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_SS))
		{
			LOG_CF_D(L"刺客-SS");
			CBaseObject::SetAngle(_Boss.GetFrontPoint(10));
			BnsSleep(150);
			_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_SS, TRUE);
			BnsSleep(500);
		}
		else
		{
			LOG_CF_D(L"刺客-走到背后");
			CPersonFunction::GetInstance().Move(_Boss.GetBehindPoint(4));
		}
	}
	LOG_CF_D(L"刺客-猛捶结束");
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_骨面旋击_Assassin(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	LOG_CF_D(L"刺客-骨面旋击");
	if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_C))
	{
		//TickBossSkill_.BeforeSleep(500);
		LOG_CF_D(L"莲华乱舞");
		_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_C, TRUE);
		BnsSleep(1000);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_X))
	{
		LOG_CF_D(L"烟幕");
		TickBossSkill_.BeforeSleep(500);
		_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_X, TRUE);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_G))
	{
		LOG_CF_D(L"夜行");
		_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_G, TRUE);
		_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_G1, TRUE);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_B))
	{
		LOG_CF_D(L"禁影步");
		_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_B, TRUE);
	}
	LOG_CF_D(L"刺客-骨面旋击结束");
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_骨面猛击_Assassin(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	LOG_CF_D(L"刺客-骨面猛击");
	if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_C))
	{
		//TickBossSkill_.BeforeSleep(500);
		LOG_CF_D(L"莲华乱舞");
		_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_C, TRUE);
		BnsSleep(1000);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_X))
	{
		LOG_CF_D(L"烟幕");
		TickBossSkill_.BeforeSleep(500);
		_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_X, TRUE);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_G))
	{
		LOG_CF_D(L"夜行");
		_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_G, TRUE);
		_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_G1, TRUE);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_B))
	{
		LOG_CF_D(L"禁影步");
		_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_B, TRUE);
	}
	LOG_CF_D(L"刺客-骨面猛击结束");
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_斧之风_Assassin(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	LOG_CF_D(L"斧之风");
	if (IsFaceToBoss(8))
	{
		if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_SS_F))
		{
			LOG_CF_D(L"木叶疾斩");
			_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_SS_F, TRUE);
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_2))
		{
			LOG_CF_D(L"替身术");
			_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_2, TRUE);
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(500, [&]
			{
				LOG_CF_D(L"斧之风-替身术-%d", CPerson::GetInstance().GetFightPose());
				return CPerson::GetInstance().GetFightPose() != em_Player_Pose_Assassin_Hide;
			});
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_Q))
		{
			LOG_CF_D(L"刺客Q");
			_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_Q, TRUE);
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_E))
		{
			LOG_CF_D(L"刺客E");
			_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_E, TRUE);
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_SS))
		{
			LOG_CF_D(L"刺客SS");
			CBaseObject::SetAngle(_Boss.GetFrontPoint(10));
			BnsSleep(150);
			_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_SS, TRUE);
			BnsSleep(500);
		}
		else
		{
			LOG_CF_D(L"没有技能可以使用");
			//CPersonFunction::GetInstance().Move(_Boss.GetBehindPoint(4));
		}
	}
	LOG_CF_D(L"斧之风结束");
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_骨面呐喊_Assassin(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	LOG_CF_D(L"骨面呐喊");
	LOG_CF_D(L"骨面呐喊结束");
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_旋转攻击_Assassin(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	LOG_CF_D(L"刺客旋转攻击上个技能是%d", _dwLastSkillId);
	//上个技能是5903258执行虚弱
	if (_dwLastSkillId == 5903258)
	{
		LOG_CF_D(L"执行虚弱,我是刺客");
		if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(Assassin_SkillId_2_F))
		{
			LOG_CF_D(L"木叶闪现");
			_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_2_F, TRUE);
		}
		else
		{
			LOG_CF_D(L"执行虚弱,Assassin_SkillId_2_F");
			_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_2, TRUE);
			_DailyTask_Skill.UseSkill_By_SkillId(Assassin_SkillId_2_F, TRUE);
		}
	}
	//上个技能是5903259执行击倒
	else if (_dwLastSkillId == 5903259)
	{
		LOG_CF_D(L"执行击倒,我是刺客");
	}
	//上个技能是??执行眩晕
	else
	{
		LOG_CF_D(L"执行眩晕,我是刺客");
	}
	LOG_CF_D(L"旋转攻击结束");
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::KillBoss_KongFuMaster()
{
	auto& DailyTask_BossSkill = CDailyTask_BossSkill::GetInstance();
	DailyTask_BossSkill.SetMaxSameSkillTimeout(5 * 1000);
	CDailyTask_BossSkill::TickBossSkill Lastpr(0);

	while (GameRun && !_Boss.IsDead())
	{
		if (EMPTY_PERSONHP)
		{
			break;
		}

		auto Pr = DailyTask_BossSkill.GetBossSkillId();
		if (!Pr.first)
		{
			NormalMove();
			if (_dwLastSkillId == em_Skill_布卡_挥击)
			{
				if (Lastpr.SpendTime() < 1500)
				{
					_DailyTask_Skill.RefreshVecSkill();
					Skill_KongFuMaster();
				}
				else
				{
					//LOG_CF_D(L"上一个技能是挥击,距离现在已经过去%d,不调用普通技能", Lastpr.SpendTime());
				}
			}
			else if (_dwLastSkillId == em_Skill_布卡_喵火流星)
			{
				if (Lastpr.SpendTime() < 1900)
				{
					_DailyTask_Skill.RefreshVecSkill();
					Skill_KongFuMaster();
				}
				else
				{
					//LOG_CF_D(L"上一个技能是喵火流星,距离现在已经过去%d,不调用普通技能", Lastpr.SpendTime());
				}
			}
			else if (_dwLastSkillId == em_Skill_布卡_奇袭攻击)
			{
				if (Lastpr.SpendTime() < 900)
				{
					_DailyTask_Skill.RefreshVecSkill();
					Skill_KongFuMaster();
				}
				else
				{
					//LOG_CF_D(L"上一个技能是奇袭攻击,距离现在已经过去%d,不调用普通技能", Lastpr.SpendTime());
				}
			}
			else if (_dwLastSkillId == em_Skill_布卡_连续攻击)
			{
				if (Lastpr.SpendTime() < 2500)
				{
					_DailyTask_Skill.RefreshVecSkill();
					Skill_KongFuMaster();
				}
				else
				{
					//LOG_CF_D(L"上一个技能是连续攻击,距离现在已经过去%d,不调用普通技能", Lastpr.SpendTime());
				}
			}
			else if (_dwLastSkillId == em_Skill_布卡_猛捶)
			{
				if (Lastpr.SpendTime() < 2300)
				{
					_DailyTask_Skill.RefreshVecSkill();
					Skill_KongFuMaster();
				}
				else
				{
					//LOG_CF_D(L"上一个技能是猛捶,距离现在已经过去%d,不调用普通技能", Lastpr.SpendTime());
				}
			}
			else if (_dwLastSkillId == em_Skill_布卡_骨面旋击)
			{
				if (Lastpr.SpendTime() < 3400)
				{
					_DailyTask_Skill.RefreshVecSkill();
					Skill_KongFuMaster();
				}
				else
				{
					//LOG_CF_D(L"上一个技能是骨面旋击,距离现在已经过去%d,不调用普通技能", Lastpr.SpendTime());
				}
			}
			else if (_dwLastSkillId == em_Skill_布卡_斧之风)
			{
				if (Lastpr.SpendTime() < 3400)
				{
					_DailyTask_Skill.RefreshVecSkill();
					Skill_KongFuMaster();
				}
				else
				{
					//LOG_CF_D(L"上一个技能是斧之风,距离现在已经过去%d,不调用普通技能", Lastpr.SpendTime());
				}
			}
			else if (_dwLastSkillId == em_Skill_布卡_旋转攻击)
			{
				_DailyTask_Skill.RefreshVecSkill();
				Skill_KongFuMaster();
			}
			else
			{
				_DailyTask_Skill.RefreshVecSkill();
				Skill_KongFuMaster();
			}
			
			continue;
		}

		auto dwBossSkillId = Pr.second.dwSkillId;

		// 
		_Boss.SetAngle();
		_DailyTask_Skill.RefreshVecSkill();

		//auto ulTick = ::GetTickCount64();
		//LOG_CF_D(L"Echo Skill[%X] 准备处理的时候已经用时=%d,Tick=%d", dwBossSkillId, static_cast<DWORD>(ulTick - Pr.second.ulTick), static_cast<DWORD>(Pr.second.ulTick));
		switch (dwBossSkillId)
		{
		case em_Skill_布卡_挥击:
			BossSkill_挥击_KongFuMaster(Pr.second);
			break;
		case em_Skill_布卡_喵火流星:
			BossSkill_喵火流星_KongFuMaster(Pr.second);
			break;
		case em_Skill_布卡_奇袭攻击:
			BossSkill_奇袭攻击_KongFuMaster(Pr.second);
			break;
		case em_Skill_布卡_连续攻击:
			BossSkill_连续攻击_KongFuMaster(Pr.second);
			break;
		case em_Skill_布卡_猛捶:
			BossSkill_猛捶_KongFuMaster(Pr.second);
			break;
		case em_Skill_布卡_骨面旋击:
			BossSkill_骨面旋击_KongFuMaster(Pr.second);
			break;
		case em_Skill_布卡_斧之风:
			BossSkill_斧之风_KongFuMaster(Pr.second);
			break;
		case em_Skill_布卡_骨面呐喊:
			BossSkill_骨面呐喊_KongFuMaster(Pr.second);
			break;
		case em_Skill_布卡_旋转攻击:
			BossSkill_旋转攻击_KongFuMaster(Pr.second);
			break;
		default:
			break;
		}
		//LOG_CF_D(L"处理技能[%X] 用时%d, 总时长%d", dwBossSkillId, static_cast<DWORD>(::GetTickCount64() - ulTick), static_cast<DWORD>(::GetTickCount64() - Pr.second.ulTick));
		_dwLastSkillId = dwBossSkillId;
		Lastpr = Pr.second;
	}
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::Skill_KongFuMaster()
{
	//auto nTick = ::GetTickCount64();
	//LOG_CF_D(L"开始Skill_BladeMaster=%d", nTick);
	_Boss.SetAngle();
	DWORD dwMp = CPerson::GetInstance().GetMp();
	if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_X_1))
	{
		//LOG_CF_D(L"点穴!");
		_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_X_1, FALSE);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_X_2))
	{
		//LOG_CF_D(L"点穴2!");
		_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_X_2, FALSE);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_X_3))
	{
		//LOG_CF_D(L"点穴3!");
		_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_X_3, FALSE);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_X_4))
	{
		//LOG_CF_D(L"点穴4!");
		_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_X_4, FALSE);
	}

		if (_Boss.GetDis() >= 7)
		{
			if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_Tab))
			{
				//LOG_CF_D(L"突进靠近_Boss");
				_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_Tab, FALSE);
				//return;
			}
			else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_T))
			{
				//LOG_CF_D(L"突袭靠近_Boss");
				_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_T, FALSE);
				//return;
			}
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_Tab1))
		{
			_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_Tab1, FALSE);
			//return;
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_F1_1))
		{
			//LOG_CF_D(L"爆裂脚!");
			_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_F1_1, FALSE);
			//return;
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_F1_2))
		{
			//LOG_CF_D(L"爆裂脚!");
			_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_F1_2, FALSE);
			//return;
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_F2_1))
		{
			//LOG_CF_D(L"升龙拳!");
			_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_F2_1, FALSE);
			//return;
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_F2_2))
		{
			//LOG_CF_D(L"升龙拳!");
			_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_F2_2, FALSE);
			//return;
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_F2_3))
		{
			//LOG_CF_D(L"升龙拳!");
			_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_F2_3, FALSE);
			//return;
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_F3_1))
		{
			//LOG_CF_D(L"铁山靠!");
			_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_F3_1, FALSE);
			//return;
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_4_1))
		{
			//LOG_CF_D(L"4_1!");
			_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_4_1, FALSE);
			//return;
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_41_2))
		{
			//LOG_CF_D(L"41!");
			_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_41_2, FALSE);
			//return;
		}

		if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_X1))
		{
			//LOG_CF_D(L"通背拳!1");
			_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_X1, FALSE);
			//return;
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_X2))
		{
			//LOG_CF_D(L"通背拳2!");
			_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_X2, FALSE);
			//return;
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_X3))
		{
			//LOG_CF_D(L"通背拳3!");
			_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_X3, FALSE);
			//return;
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_X4))
		{
			//LOG_CF_D(L"通背拳4!");
			_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_X4, FALSE);
			//return;
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_X5))
		{
			//LOG_CF_D(L"通背拳5!");
			_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_X5, FALSE);
			//return;
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_T4))
		{
			//LOG_CF_D(L"烈焰脚!");
			_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_T4, FALSE);
			//return;
		}
		else if (dwMp >= 5 && _DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_V))
		{
			_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_V, FALSE);
			//return;
		}
		else if (dwMp >= 5 && _DailyTask_Skill.IsCool_By_KeyText_In_Ui(L"2"))
		{
			CSomeFun::GetInstance().SimlationKey(VK_2,10);
		}
		else
		{
			_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_R1_1, FALSE);
		}
	//LOG_CF_D(L"输出结束2%d", static_cast<DWORD>(::GetTickCount64() - nTick));
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_挥击_KongFuMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	//LOG_CF_D(L"挥击");
	
	if (IsFaceToBoss(8))
	{
		_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_1, TRUE);

		CSomeFun::GetInstance().TimeOut_Condiction_GameRun(500, [&]
		{
			//LOG_CF_D(L"奇袭攻击击剑");
			_DailyTask_Skill.RefreshVecSkill();
			auto boolflag = _DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_F3_1);
			auto boolflag1 = _DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_T1);
			auto boolflag2 = _DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_T2);
			//LOG_CF_D(L"奇袭攻击击剑-%d", boolflag);
			return !boolflag || !boolflag1 || !boolflag2;
		});
	}
	else
	{
		//LOG_CF_D(L"挥击背面");
	}
	//LOG_CF_D(L"挥击结束");
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_喵火流星_KongFuMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	//LOG_CF_D(L"喵火流星");
	
	if (IsFaceToBoss(8))
	{
		if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_E))
		{
			//LOG_CF_D(L"E无敌去躲");
			_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_E, TRUE);
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_Q))
		{
			//LOG_CF_D(L"Q无敌去躲");
			_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_Q, TRUE);
		}
		else
		{
			//LOG_CF_D(L"没有技能躲避,执行走路");
			CPersonFunction::GetInstance().Move(_Boss.GetBehindPoint(3.5));
		}
	}

	//LOG_CF_D(L"喵火流星结束");
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_奇袭攻击_KongFuMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	//LOG_CF_D(L"奇袭攻击");
	if (IsFaceToBoss(8))
	{
		_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_1, TRUE);

		CSomeFun::GetInstance().TimeOut_Condiction_GameRun(500, [&]
		{
			//LOG_CF_D(L"奇袭攻击反击");
			_DailyTask_Skill.RefreshVecSkill();
			auto boolflag = _DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_F3_1);
			auto boolflag1 = _DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_T1);
			auto boolflag2 = _DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_T2);
			//LOG_CF_D(L"奇袭攻击击剑-%d", boolflag);
			return !boolflag || !boolflag1 || !boolflag2;
		});
	}
	else
	{
		//LOG_CF_D(L"奇袭攻击背面");
	}

	//LOG_CF_D(L"奇袭攻击结束");
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_连续攻击_KongFuMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	//LOG_CF_D(L"_连续攻击!");
	
	if (IsFaceToBoss(8))
	{
			//LOG_CF_D(L"连续攻击没有技能可以用,使用反击");
			//CPersonFunction::GetInstance().Move(_Boss.GetBehindPoint(3.5));
			_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_1, TRUE);
			BnsSleep(500);
	}
	else
	{
		//LOG_CF_D(L"连续攻击背面结束");
	}

	//LOG_CF_D(L"_连续攻击结束!");
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_猛捶_KongFuMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	
	//LOG_CF_D(L"猛捶");
	
	if (IsFaceToBoss(8))
	{
		if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_SS))
		{
			//LOG_CF_D(L"SS");
			CBaseObject::SetAngle(_Boss.GetFrontPoint(10));
			BnsSleep(100);
			_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_SS, TRUE);
			BnsSleep(1000);
		}
		else
		{
			//LOG_CF_D(L"没有SS,使用走路");
			CPersonFunction::GetInstance().Move(_Boss.GetBehindPoint(3.5));
		}
	}
	//LOG_CF_D(L"猛捶结束");
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_骨面旋击_KongFuMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	//LOG_CF_D(L"骨面旋击");
	if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_C))
	{
		//LOG_CF_D(L"C无敌去躲");
		_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_C, TRUE);
		BnsSleep(1500);
	}
	else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_SS))
	{
		//LOG_CF_D(L"SS");
		CBaseObject::SetAngle(_Boss.GetFrontPoint(10));
		BnsSleep(100);
		_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_SS, TRUE);
		BnsSleep(1000);
	}
	else
	{
		//LOG_CF_D(L"没有技能可以用");
	}

	//LOG_CF_D(L"骨面旋击结束");
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_斧之风_KongFuMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	//LOG_CF_D(L"斧之风");
	if (IsFaceToBoss(8))
	{
		if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_E))
		{
			//LOG_CF_D(L"E无敌去躲");
			_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_E, TRUE);
		}
		else if (_DailyTask_Skill.IsActiveCool_By_SkillId_In_Ui(KongFuMaster_SkillId_Q))
		{
			//LOG_CF_D(L"Q无敌去躲");
			_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_Q, TRUE);
		}
		else
		{
			//LOG_CF_D(L"没有技能可以用,走路");
			CPersonFunction::GetInstance().Move(_Boss.GetBehindPoint(3.5));
		}
	}
	else
	{
		//LOG_CF_D(L"斧之风背面结束");
	}
	//LOG_CF_D(L"斧之风结束");
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_骨面呐喊_KongFuMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	
	//LOG_CF_D(L"骨面呐喊-地上出现浊气阵...");

	//LOG_CF_D(L"骨面呐喊结束");
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::BossSkill_旋转攻击_KongFuMaster(_In_ CONST CDailyTask_BossSkill::TickBossSkill& TickBossSkill_)
{
	
	//LOG_CF_D(L"旋转攻击上个技能是%d", _dwLastSkillId);
	//上个技能是5903258执行虚弱
	if (_dwLastSkillId == 5903258)
	{
		//LOG_CF_D(L"执行虚弱,我是拳师Z");
		_DailyTask_Skill.UseSkill_By_SkillId(BladeMaster_SkillId_Z, TRUE);
	}
	//上个技能是5903259执行击倒
	else if (_dwLastSkillId == 5903259)
	{
		//LOG_CF_D(L"执行击倒,我是拳师3");
		_DailyTask_Skill.UseSkill_By_SkillId(KongFuMaster_SkillId_3,TRUE);
	}
	//上个技能是??执行眩晕
	else
	{
		//LOG_CF_D(L"执行眩晕,我是拳师");
		
	}
	//LOG_CF_D(L"旋转攻击结束");
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::NormalMove()
{
	_Boss.SetAngle();
	switch (CPerson::GetInstance().GetClasses())
	{
		// 
	case em_Player_Classes::em_Player_Classes_Assassin:
		MeleeMove1();
		break;
	case em_Player_Classes::em_Player_Classes_Destroyer:
	case em_Player_Classes::em_Player_Classes_BladeMaster:
	case em_Player_Classes::em_Player_Classes_KongFuMaster:
	case em_Player_Classes::em_Player_Classes_Danncer:
		MeleeMove();
		break;
	default:
		FarRangeMove();
		break;
	}
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::MeleeMove()
{
	std::vector<Point> VecTurbiGround = GetTurbidGround();

	Point TurbiGroundPoint;
	if (ContainTrubidGroud(VecTurbiGround, CPerson::GetInstance().GetPoint(), 4.0f, &TurbiGroundPoint) || ContainTrubidGroud(VecTurbiGround, _Boss.GetPoint(), 4.0f, &TurbiGroundPoint))
	{
		// 踩在浊气阵上
		LOG_CF_E(L"踩在浊气阵上");
		Point SafePoint;
		if (FindSafePoint(VecTurbiGround, TurbiGroundPoint, SafePoint))
		{
			//LOG_CF_D(L"离开浊气阵, NextStep.dis=%.2f", CPerson::GetInstance().GetDis(SafePoint));
			std::vector<Point> VecSafePath;
			FindSafePath(VecTurbiGround, SafePoint, VecSafePath);
			MoveToSafePoint(VecSafePath);
			//BnsSleep(1000);
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2 * 1000, [=]
			{
				_Boss.SetAngle();
				//CSomeFun::GetInstance().SimlationKey(VK_1, 0);
				BnsSleep(50);
				return _Boss.GetDis() >= 5;
			});
			return;
		}
		LOG_CF_E(L"UnExist TurbiGround Point,size=%d", VecTurbiGround.size());
	}

	if (_Boss.GetDis() > 5 + em_Boss_BodySize)
	{
		CPersonFunction::GetInstance().Move(_Boss.GetFrontPoint(4));
	}
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::MeleeMove1()
{
	std::vector<Point> VecTurbiGround = GetTurbidGround();

	Point TurbiGroundPoint;
	if (ContainTrubidGroud(VecTurbiGround, CPerson::GetInstance().GetPoint(), 4.0f, &TurbiGroundPoint) || ContainTrubidGroud(VecTurbiGround, _Boss.GetPoint(), 4.0f, &TurbiGroundPoint))
	{
		// 踩在浊气阵上
		LOG_CF_E(L"踩在浊气阵上");
		Point SafePoint;
		if (FindSafePoint(VecTurbiGround, TurbiGroundPoint, SafePoint))
		{
			LOG_CF_D(L"离开浊气阵, NextStep.dis=%.2f", CPerson::GetInstance().GetDis(SafePoint));
			std::vector<Point> VecSafePath;
			FindSafePath(VecTurbiGround, SafePoint, VecSafePath);
			MoveToSafePoint(VecSafePath);
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(2 * 1000, [=]
			{
				_Boss.SetAngle();
				//CSomeFun::GetInstance().SimlationKey(VK_1, 0);
				BnsSleep(50);
				return _Boss.GetDis() >= 5;
			});
			return;
		}
		LOG_CF_E(L"UnExist TurbiGround Point");
	}

	if (_Boss.GetDis() > 3 + em_Boss_BodySize)
	{
		std::vector<Point> VecSafePath;
		FindSafePath(VecTurbiGround, _Boss.GetFrontPoint(3), VecSafePath);
		MoveToSafePoint(VecSafePath);
	}
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::FarRangeMove()
{
	auto fnMoveToSafePoint = [=](_In_ CONST std::vector<Point>& VecTurbiGround)
	{
		for (int i = 12 + em_Boss_BodySize; i < 15 + em_Boss_BodySize; ++i)
		{
			std::vector<Point> VecPoint;
			CFarm::GetInstance().GetRoundPointList(FixPointInfo(_Boss.GetPoint(), static_cast<float>(i)), 24, VecPoint);

			// 根据人物距离排序, 离人物越近, 越优先判断
			std::sort(VecPoint.begin(), VecPoint.end(), [](CONST Point& p1, CONST Point& p2) { return CPerson::GetInstance().GetDis(p1) < CPerson::GetInstance().GetDis(p2); });

			// 判断是否走在浊气阵上
			for (auto& itm : VecPoint)
			{
				if (CBaseObject::Get3DDis(_BossRange.FixPoint, itm) < 15 + em_Boss_BodySize && !ContainTrubidGroud(VecTurbiGround, itm, 4, nullptr))
				{
					std::vector<Point> VecSafePath;
					FindSafePath(VecTurbiGround, itm, VecSafePath);
					MoveToSafePoint(VecSafePath);
					return;
				}
			}

			LOG_C_D(L"没有适合的浊气阵坐标!");
		}
	};

	MyTools::CLEchoException::GetInstance().InvokeAction(__FUNCTIONW__, [=]
	{
		std::vector<Point> VecTurbiGround = GetTurbidGround();

		Point TurbiGroundPoint;
		if (ContainTrubidGroud(VecTurbiGround, CPerson::GetInstance().GetPoint(), 4.0f, &TurbiGroundPoint))
		{
			// 踩在浊气阵上
			LOG_C_D(L"踩在浊气阵上");
			fnMoveToSafePoint(VecTurbiGround);
		}

		float fDis = CPerson::GetInstance().Get2DDis(_Boss.GetPoint());
		if (fDis >= 10 + em_Boss_BodySize && fDis < 16 + em_Boss_BodySize)
			return;


		fnMoveToSafePoint(VecTurbiGround);
	});
}

std::vector<Point> CDailyTask_HukryongPrison_Boss_BuKa::GetTurbidGround() CONST
{
	std::vector<Point> VecPoint;
	MyTools::CLEchoException::GetInstance().InvokeAction(__FUNCTIONW__, [&VecPoint]
	{
		std::vector<CMonster> VecMonster;
		CMonsterExtend::GetInstance().GetAroundMonsterByMaxHp(109560, CBaseObject::CreateFixPointInfo(50), VecMonster);
		for (auto& itm : VecMonster)
			VecPoint.push_back(itm.GetPoint());

		VecMonster.clear();
		CMonsterExtend::GetInstance().GetAroundMonsterByMaxHp(166000, CBaseObject::CreateFixPointInfo(50), VecMonster);
		for (auto& itm : VecMonster)
			VecPoint.push_back(itm.GetPoint());
	});
	return VecPoint;
}

BOOL CDailyTask_HukryongPrison_Boss_BuKa::ContainTrubidGroud(_In_ CONST std::vector<Point>& VecPoint, _In_ CONST Point& ComprPoint, _In_ float fMaxDis, _Out_opt_ Point* pTurbidGroundPoint /* = nullptr */) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&] 
	{
		return MyTools::CLPublic::Vec_find_if_Const(VecPoint, pTurbidGroundPoint, [ComprPoint, fMaxDis](CONST Point& Pt) { return CBaseObject::Get3DDis(ComprPoint, Pt) < fMaxDis; });
	});
}

BOOL CDailyTask_HukryongPrison_Boss_BuKa::FindSafePoint(_In_ CONST std::vector<Point>& VecTurbiGround, _In_ CONST Point& TurbiGround, _Out_ Point& RetPoint) CONST
{
	auto fnFindSafePoint = [&](float fMinDis)
	{
		for (int i = 12; i <= 30 && GameRun; ++i)
		{
			std::vector<Point> VecRound;
			CFarm::GetInstance().GetRoundPointList(FixPointInfo(TurbiGround, static_cast<float>(i)), i * 2, VecRound);

			for (auto& itm : VecRound)
			{
				if (CBaseObject::Get3DDis(_BossRange.FixPoint, itm) >= 15)
					continue;

				BOOL bContain = FALSE;
				for (auto& TarPoint : VecTurbiGround)
				{
					if (CBaseObject::Get3DDis(itm, TarPoint) < fMinDis)
					{
						bContain = TRUE;
						break;
					}
				}

				if (!bContain)
				{
					RetPoint = itm;
					return TRUE;
				}
			}
		}

		return FALSE;
	};

	for (float i = 14.0f; i >= 10.0f; --i)
	{
		if (fnFindSafePoint(i))
			return TRUE;
	}
	
	return FALSE;
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::KillFish()
{
	std::vector<CMonster> VecMonster;
	auto fnFindMonster = [&VecMonster](_In_ CONST std::wstring& wsMonsterName)
	{
		std::vector<CMonster> Vec;
		CMonsterExtend::GetInstance().GetAroundMonsterListByName(wsMonsterName, CBaseObject::CreateFixPointInfo(50), Vec);
		std::for_each(Vec.begin(), Vec.end(), [&VecMonster](CONST CMonster& itm) { VecMonster.push_back(itm); });
	};

	auto fnExistMonster = [fnFindMonster, &VecMonster]
	{
		VecMonster.clear();
		fnFindMonster(L"돌격병");
		fnFindMonster(L"전투병");
		return !VecMonster.empty();
	};

	MyTools::CLEchoException::GetInstance().InvokeAction(__FUNCTIONW__, [=]
	{
		auto emPlayerClasses = CPerson::GetInstance().GetClasses();
		while (GameRun && !EMPTY_PERSONHP && fnExistMonster())
		{
			std::vector<Point> VecTurbiGround = GetTurbidGround();

			Point TurbiGroundPoint;
			if (ContainTrubidGroud(VecTurbiGround, CPerson::GetInstance().GetPoint(), 4.0f, &TurbiGroundPoint))
			{
				// 踩在浊气阵上
				LOG_C_D(L"踩在浊气阵上");
				FarRangeMove();
				continue;
			}

			CONST CMonster& Monster = VecMonster.at(0);
			switch (emPlayerClasses)
			{
			case em_Player_Classes_ForceMaster:
				Monster.SetAngle();
				Skill_ForceMaster();
				break;
			case em_Player_Classes_Warlock:
				Monster.SetAngle();
				Skill_Warlock();
				break;
			case em_Player_Classes_Gunner:
				break;
			default:
				return;
			}
		}
	});
}

BOOL CDailyTask_HukryongPrison_Boss_BuKa::FindSafePath(_In_ CONST std::vector<Point>& VecTurbiGround_, _In_ CONST Point& TarPoint, _Out_ std::vector<Point>& VecRetPoint) CONST
{
	auto VecTurbiGround = VecTurbiGround_;
	auto fnRemovePointPtr = [&VecTurbiGround](_In_ CONST Point& FixPoint)
	{
		MyTools::CLPublic::Vec_erase_if(VecTurbiGround, [FixPoint](CONST Point& TurbiGroudPoint) { return CBaseObject::Get3DDis(FixPoint, TurbiGroudPoint) <= 4; });
	};

	// 去掉自身坐标的浊气阵(不然就无法寻路)
	fnRemovePointPtr(CPerson::GetInstance().GetPoint());


	// 去掉BOSS坐标的浊气阵
	fnRemovePointPtr(TarPoint);

	enum { em_Max_FindSafePath_Range = 30, em_TurbiGroud_Dis = 4 };
	// 建立圆, 圆心=BOSS, 半径=30。 寻路不可超出这个范围 , 防止触碰到浊气阵, 将浊气阵的范围从4 -> 5
	Point CurPoint = CPerson::GetInstance().GetPoint();
	while (GameRun && !EMPTY_PERSONHP && CBaseObject::Get3DDis(TarPoint, CurPoint) > 2)
	{
		// 不用A* 也不用dijkstra, 直接用一个坐标搜索即可!
		std::vector<Point> VecDir;
		VecDir.push_back(Point(CurPoint.GetX() + 50, CurPoint.GetY(), CurPoint.GetZ()));
		VecDir.push_back(Point(CurPoint.GetX() - 50, CurPoint.GetY(), CurPoint.GetZ()));
		VecDir.push_back(Point(CurPoint.GetX(), CurPoint.GetY() + 50, CurPoint.GetZ()));
		VecDir.push_back(Point(CurPoint.GetX(), CurPoint.GetY() - 50, CurPoint.GetZ()));

		// Remove 
		MyTools::CLPublic::Vec_erase_if(VecDir, [TarPoint](CONST Point& Pt) { return CBaseObject::Get3DDis(Pt, TarPoint) >= em_Max_FindSafePath_Range; });
		MyTools::CLPublic::Vec_erase_if(VecDir, [=](CONST Point& Pt)
		{
			return ContainTrubidGroud(VecTurbiGround, Pt, em_TurbiGroud_Dis);
		});
		MyTools::CLPublic::Vec_erase_if(VecDir, [VecRetPoint](CONST Point& Pt)
		{
			return MyTools::CLPublic::Vec_find_if_Const(VecRetPoint, [Pt](CONST Point& p1) { return p1.GetX() == Pt.GetX() && p1.GetY() == Pt.GetY(); });
		});
		std::sort(VecDir.begin(), VecDir.end(), [TarPoint](CONST Point& p1, CONST Point& p2) { return CBaseObject::Get3DDis(p1, TarPoint) < CBaseObject::Get3DDis(p2, TarPoint); });
		if (VecDir.size() == 0)
		{
			LOG_CF_E(L"VecDir.size = 0, 寻路失败! em");
			VecRetPoint.clear();
			return TRUE;
		}

		CurPoint = VecDir.at(0);
		VecRetPoint.push_back(CurPoint);
		//LOG_CF_D(L"CurPoint.dis=%.2f", CBaseObject::Get3DDis(CurPoint, TarPoint));
	}
	VecRetPoint.push_back(TarPoint);
	return TRUE;
}

VOID CDailyTask_HukryongPrison_Boss_BuKa::MoveToSafePoint(_In_ CONST std::vector<Point>& VecSafePath) CONST
{
	for (auto& SafePoint : VecSafePath)
	{
		CPersonFunction::GetInstance().Move(SafePoint);
		while (GameRun && !EMPTY_PERSONHP && CPerson::GetInstance().GetDis(SafePoint) > 2)
		{
			if (!CPerson::GetInstance().IsMoving())
			{
				CPersonFunction::GetInstance().Move(SafePoint);
			}
			BnsSleep(100);
		}
	}
}
