#include "stdafx.h"
#include "FarmTask.h"
#include <MyTools/Log.h>
#include <MyTools/CLEchoException.h>
#include "MonsterExtend.h"
#include "PersonFunction.h"
#include "SomeFun.h"
#include "BagFunction.h"
#include "ResAction.h"
#include "NpcExtend.h"
#include "FarmBoss.h"
#include "Person.h"
#include "Npc.h"
#include "ResExtend.h"
#include "Monster.h"
#include "Corpse.h"
#include "TeamMemberExtend.h"
#include "TeamMember.h"
#include "PlayerExtend.h"
#include "Skill.h"
#include "Cat.h"
#include "CatExpend.h"
#include "Farm.h"
#define _SELF L"FarmTask.cpp"

BOOL CFarmTask::Farm_By_Condiction(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<UINT(VOID)> fnGetAroundCount, _In_ std::function<BOOL(std::vector<CMonster>&, CONST FixPointInfo&)> fnBreak) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		BOOL bRetCode = FALSE;
		auto emPlayerClass = CPerson::GetInstance().GetClasses();

		do
		{
			std::vector<CMonster> vlst;
			DWORD dwForceMode = CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_KADAOMODE);
			while (GameRun && !EMPTY_PERSONHP && fnBreak(vlst, FixPtInfo))
			{
				LOG_CF_D(L"附近还有%d个怪,范围:%.2f, dwForceMode=%X", vlst.size(), FixPtInfo.fDis, dwForceMode);
				if (vlst.size() == 0)
				{
					BnsSleep(50);
					continue;
				}

				CMonster Monster = vlst.at(0);
				if ((emPlayerClass == em_Player_Classes::em_Player_Classes_KongFuForceMaster || emPlayerClass == em_Player_Classes::em_Player_Classes_Summoner) && ExistSpecialMonster(Monster, FixPtInfo))
					continue;

				if (!Move_To_Monster(Monster, FixPtInfo) || Monster.IsDead())
				{
					LOG_CF_D(L"移动到怪物旁失败");
					continue;
				}
				else if (Monster.IsDead())
				{
					LOG_CF_D(L"怪物已经挂了! 就不弄它了!");
					continue;
				}

				// vlst.size() == 1  GetAroundCount() == 2
				LOG_CF_D(L"杀怪Name:%s", Monster.GetName().c_str());
				if (dwForceMode != 2 && (fnGetAroundCount() == 1 || dwForceMode == 0x1)) // 5米范围内其实没有怪的。可以用卡刀了
					KillMonster_By_Single(Monster, FixPtInfo, dwForceMode == 0x1, fnGetAroundCount);
				else
					KillMonster_By_AOE(Monster, FixPtInfo, dwForceMode == 0x2, fnGetAroundCount);

			}

			CGameVariable::GetInstance().Action_Variable_By_Name(VARIABLE_NAME_STOPTAKEITEM, FALSE, [&FixPtInfo] {CResAction::GetInstance().TakeDropItem(FixPtInfo); });

			if (CPerson::GetInstance().GetLevel() < 45 || CPerson::GetInstance().GetClasses() == em_Player_Classes_KongFuForceMaster)
				bRetCode = !EatFood(CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_FOODPERCENT));

			// bRetCode = FALSE = WarStatus!
			if (bRetCode)
				CFarm::GetInstance().ClearAround_TarIdSelf(CBaseObject::CreateFixPointInfo(16.0f));

		} while (bRetCode);
		return TRUE;
	});
}


BOOL CFarmTask::Farm_By_Condiction_ts(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<UINT(VOID)> fnGetAroundCount, _In_ std::function<BOOL(std::vector<CMonster>&, CONST FixPointInfo&)> fnBreak) CONST
{
	BOOL bRetCode = FALSE;

	do
	{
		std::vector<CMonster> vlst;
		DWORD dwForceMode = CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_KADAOMODE);
		while (GameRun && !EMPTY_PERSONHP && fnBreak(vlst, FixPtInfo))
		{
			LOG_CF_D(L"附近还有%d个怪,范围:%.2f, dwForceMode=%X", vlst.size(), FixPtInfo.fDis, dwForceMode);
			if (vlst.size() == 0)
			{
				BnsSleep(50);
				continue;
			}

			CMonster Monster = vlst.at(0);

			if (!Move_To_Monster(Monster, FixPtInfo))
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
			//if (dwForceMode == 0x3)
			//{
			//	Kffm_Fm_Singe(Monster, FixPtInfo);
			//	BnsSleep(50);
			//	continue;
			//}
			//else if (dwForceMode == 0x4)
			//{
			//	Kffm_Fm_RT(Monster, FixPtInfo);
			//	BnsSleep(50);
			//	continue;
			//}

			//if (CPerson::GetInstance().GetClasses() == em_Player_Classes_KongFuForceMaster && ExistSpecialMonster(Monster, FixPtInfo))
			//	continue;

			//

			//// vlst.size() == 1  GetAroundCount() == 2
			////LOG_CF_D(L"杀怪Name:%s", Monster.GetName().c_str());
			//if (dwForceMode != 2 && (fnGetAroundCount() == 1 || dwForceMode == 0x1)) // 5米范围内其实没有怪的。可以用卡刀了
			//	KillMonster_By_Single(Monster, FixPtInfo, dwForceMode == 0x1, fnGetAroundCount);
			//else
			//	KillMonster_By_AOE(Monster, FixPtInfo, dwForceMode == 0x2, fnGetAroundCount);

		}

		CGameVariable::GetInstance().Action_Variable_By_Name(VARIABLE_NAME_STOPTAKEITEM, FALSE, [&FixPtInfo]{
			CResAction::GetInstance().TakeDropItem(FixPtInfo);
		});

		if (CPerson::GetInstance().GetLevel() < 45 || CPerson::GetInstance().GetClasses() == em_Player_Classes_KongFuForceMaster)
			bRetCode = !EatFood(CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_FOODPERCENT));

	} while (bRetCode);
	return TRUE;
}

BOOL CFarmTask::ClearAroundMonster(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<BOOL(VOID)> fnBreak) CONST
{
	LOG_CF_D(L"CFarmTask::ClearAroundMonster");
	float fAoeDis = static_cast<float>(CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_GETAROUNDMONSTER_DIS)) / 10.0f;
	return Farm_By_Condiction(FixPtInfo, 
		[&fAoeDis]{ return CMonsterExtend::GetInstance().GetAroundMonsterCount(CBaseObject::CreateFixPointInfo(fAoeDis)); },
	[&fnBreak](std::vector<CMonster>& vlst, CONST FixPointInfo& FixPtInfo){
		vlst.clear();

		if (fnBreak())
			return false;

		return CMonsterExtend::GetInstance().GetAroundMonsterList(FixPtInfo, vlst) != NULL;
	});
}

BOOL CFarmTask::ClearAroundMonster(_In_ CONST FixPointInfo& FixPtInfo) CONST
{
	return ClearAroundMonster(FixPtInfo, [] {return FALSE; });
}

BOOL CFarmTask::ClearAroundNpc(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<BOOL(VOID)> fnBreak) CONST
{
	LOG_CF_D(L"CFarmTask::ClearAroundNpc");
	float fAoeDis = static_cast<float>(CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_GETAROUNDMONSTER_DIS)) / 10.0f;
	return Farm_By_Condiction(FixPtInfo, [&fAoeDis]
	{
		return CNpcExtend::GetInstance().GetAroundNpcCount(CBaseObject::CreateFixPointInfo(fAoeDis)); 
	},
	[&fnBreak](std::vector<CMonster>& vlst, CONST FixPointInfo& FixPtInfo)
	{
		vlst.clear();

		if (fnBreak())
			return false;

		std::vector<CNpc> vNpc;
		CNpcExtend::GetInstance().GetAroundNpcList(FixPtInfo, vNpc);
		
		CFarmMonster::TovMonster(vNpc, vlst);
		return vlst.size() != NULL;
	});
}

BOOL CFarmTask::ClearAround_TarIdSelf(_In_ CONST FixPointInfo& FixPtInfo) CONST
{
	/// 周围目标ID=Self的怪物
	Farm_By_Condiction(FixPtInfo, 
		[&FixPtInfo]{ return CMonsterExtend::GetInstance().GetAroundMonsterCount(FixPtInfo); },
		[](std::vector<CMonster>& vlst, CONST FixPointInfo& FixPtInfo){
		return CMonsterExtend::GetInstance().GetAroundTargetMonsterList(FixPtInfo, vlst) != NULL;
	});

	/// 周围目标ID=Self的Npc
	Farm_By_Condiction(FixPtInfo,
		[&FixPtInfo]{ return CNpcExtend::GetInstance().GetAroundNpcCount(FixPtInfo); },
		[](std::vector<CMonster>& vlst, CONST FixPointInfo& FixPtInfo){
		std::vector<CNpc> vNpc;
		CNpcExtend::GetInstance().GetAroundTargetNpcList(FixPtInfo, vNpc);
		vlst.clear();
		CFarmMonster::TovMonster(vNpc, vlst);
		return vlst.size() != NULL;
	});
	return TRUE;
}


BOOL CFarmTask::ClearAround_TarName(_In_ CONST std::wstring& wsMonsterName, _In_ CONST FixPointInfo& FixPtInfo) CONST
{
	float fAoeDis = static_cast<float>(CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_GETAROUNDMONSTER_DIS)) / 10.0f;
	Farm_By_Condiction(FixPtInfo,
		[&fAoeDis]{ return CMonsterExtend::GetInstance().GetAroundMonsterCount(CBaseObject::CreateFixPointInfo(fAoeDis)); },
		[&wsMonsterName](std::vector<CMonster>& vlst, CONST FixPointInfo& FixPtInfo){
		CMonsterExtend::GetInstance().GetAroundMonsterList(FixPtInfo, vlst);
		return MyTools::CLPublic::Vec_erase_if(vlst, [&wsMonsterName](CONST CMonster& Monster){ return Monster.GetName() != wsMonsterName; }) != NULL;
	});

	/// 周围目标ID=Self的Npc
	Farm_By_Condiction(FixPtInfo,
		[&fAoeDis]{ return CNpcExtend::GetInstance().GetAroundNpcCount(CBaseObject::CreateFixPointInfo(fAoeDis)); },
		[&wsMonsterName](std::vector<CMonster>& vlst, CONST FixPointInfo& FixPtInfo){
		std::vector<CNpc> vNpc;
		CNpcExtend::GetInstance().GetAroundNpcList(FixPtInfo, vNpc);
		MyTools::CLPublic::Vec_erase_if(vNpc, [&wsMonsterName](CONST CNpc& Npc){ return Npc.GetName() != wsMonsterName; });

		vlst.clear();
		CFarmMonster::TovMonster(vNpc, vlst);
		return vlst.size() != NULL;
	});
	return TRUE;
}


BOOL CFarmTask::ClearAround_TarName_ts(_In_ CONST std::wstring& wsMonsterName, _In_ CONST FixPointInfo& FixPtInfo) CONST
{
	float fAoeDis = static_cast<float>(CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_GETAROUNDMONSTER_DIS)) / 10.0f;
	Farm_By_Condiction_ts(FixPtInfo,
		[&fAoeDis]{ return CMonsterExtend::GetInstance().GetAroundMonsterCount(CBaseObject::CreateFixPointInfo(fAoeDis)); },
		[&wsMonsterName](std::vector<CMonster>& vlst, CONST FixPointInfo& FixPtInfo){
		CMonsterExtend::GetInstance().GetAroundMonsterList(FixPtInfo, vlst);
		return MyTools::CLPublic::Vec_erase_if(vlst, [&wsMonsterName](CONST CMonster& Monster){ return Monster.GetName() != wsMonsterName; }) != NULL;
	});

	/// 周围目标ID=Self的Npc
	Farm_By_Condiction_ts(FixPtInfo,
		[&fAoeDis]{ return CNpcExtend::GetInstance().GetAroundNpcCount(CBaseObject::CreateFixPointInfo(fAoeDis)); },
		[&wsMonsterName](std::vector<CMonster>& vlst, CONST FixPointInfo& FixPtInfo){
		std::vector<CNpc> vNpc;
		CNpcExtend::GetInstance().GetAroundNpcList(FixPtInfo, vNpc);
		MyTools::CLPublic::Vec_erase_if(vNpc, [&wsMonsterName](CONST CNpc& Npc){ return Npc.GetName() != wsMonsterName; });

		vlst.clear();
		CFarmMonster::TovMonster(vNpc, vlst);
		return vlst.size() != NULL;
	});
	return TRUE;
}

BOOL CFarmTask::EatFood(_In_ DWORD dwPercentHp) CONST
{
	// 食物 만두
	if (CPerson::GetInstance().GetPercentHp() >= dwPercentHp || EMPTY_PERSONHP)
		return TRUE;

	auto fnFull = []
	{
		if (CPerson::GetInstance().GetClasses() == em_Player_Classes::em_Player_Classes_Summoner)
		{
			CCat Cat;
			if (!CCatExpend::GetInstance().FindSelfCat(&Cat) && Cat.IsDead())
			{
				return CPerson::GetInstance().GetPercentHp() < 100 || Cat.GetPercentHp() < 100;
			}
		}
		return CPerson::GetInstance().GetPercentHp() < 100;
	};
 
	Point FixPoint = CBaseObject::GetPersonPoint();
	while (GameRun && !EMPTY_PERSONHP && fnFull())
	{
		BnsSleep(500);
		LOG_CF_D(L"当前血量=%d", CPerson::GetInstance().GetPercentHp());
		if (CPerson::GetInstance().GetPersonStatus() != em_WarStatus_None)
			return FALSE;

		if (CPersonFunction::GetInstance().ExistBuff_Person(0x46A954C, nullptr))
		{
			BnsSleep(2 * 1000);
			continue;
		}

		// 没包子就直接跳出
		if (!CBagFunction::GetInstance().DoItem_By_ExistItem_To_Name(包子, [](CONST CBag& itm){itm.UseItem(); }))
		{
			break;
		}
	}
	return BnsMove(FixPoint,2);
}

BOOL CFarmTask::FollowPlayer(_In_ CONST CPlayer& Player, _In_ CONST Point& EndPoint, _In_ float fKillDis) CONST
{
	DWORD dwPercent = CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_FOODPERCENT, NULL);
	while (GameRun && !EMPTY_PERSONHP && CBaseObject::GetPersonDis(EndPoint) > 3.0f)
	{
		if (Player.GetDis() >= 30.0f)
		{
			LOG_CF_D(L"跟随的Npc超出了30m的范围!");
			break;
		}

		CPersonFunction::GetInstance().Move(Player.GetPoint());
		BnsSleep(500);

		DWORD dwCount = CMonsterExtend::GetInstance().GetAroundMonsterCount(FixPointInfo(CBaseObject::GetPersonPoint(), fKillDis));
		LOG_CF_D(L"离终点的距离是=%.2f, 周围怪物数量:%d", CBaseObject::GetPersonDis(EndPoint), dwCount);
		if (dwCount > 0)
			ClearAroundMonster(FixPointInfo(CBaseObject::GetPersonPoint(), fKillDis), []{return FALSE; });

	}
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_FOODPERCENT, dwPercent);
	return ClearAround_TarIdSelf(FixPointInfo(CBaseObject::GetPersonPoint(), fKillDis));
}

BOOL CFarmTask::Pickup_By_Condition(_In_ CONST std::wstring& wsName) CONST
{
	FixPointInfo FixPtInfo(CBaseObject::GetPersonPoint(), -1);

	BOOL bExist = FALSE;
	auto fnPickup_By_Monster = [&FixPtInfo, &wsName, &bExist]
	{
		std::vector<CMonster> vMonster;
		CMonsterExtend::GetInstance().GetAroundMonsterList(FixPtInfo, vMonster);
		for (CONST auto& itm : vMonster)
		{
			if (itm.GetName() == wsName)
			{
				bExist = TRUE;
				CPersonFunction::GetInstance().Pickup(itm.GetObjectId());
				BnsSleep(1000);
			}
		}
		return bExist;
	};

	auto fnPickup_By_Npc = [&FixPtInfo, &wsName, &bExist]
	{
		std::vector<CNpc> vNpc;
		CNpcExtend::GetInstance().GetAroundNpcList(FixPtInfo, vNpc);
		for (CONST auto& itm : vNpc)
		{
			if (itm.GetName() == wsName)
			{
				bExist = TRUE;
				CPersonFunction::GetInstance().Pickup(itm.GetObjectId());
				BnsSleep(1000);
			}
		}
		return bExist;
	};

	auto fnPickup_By_Corpse = [&FixPtInfo, &wsName, &bExist]
	{
		std::vector<CCorpse> vCorpse;
		CResExtend::GetInstance().GetAroundCorpseList(FixPtInfo, vCorpse);
		for (CONST auto& itm : vCorpse)
		{
			bExist = TRUE;
			if (itm.GetName() == wsName)
			{
				CPersonFunction::GetInstance().Pickup(itm.GetObjectId());
				BnsSleep(1000);
			}
		}
		return bExist;
	};

	if (fnPickup_By_Monster() || fnPickup_By_Npc() || fnPickup_By_Corpse())
		bExist = TRUE;
	else
		LOG_CF_D(L"站着捡无法找到合适的!");
	
	return TRUE;
}

BOOL CFarmTask::FollowCaptain() CONST
{
	auto fnGetAroundMonsterCount = []
	{
		return CMonsterExtend::GetInstance().GetAroundMonsterCount(CBaseObject::CreateFixPointInfo(5.0f));
	};

	CSomeFun::GetInstance().SetSpeed(3.0f);
	while (GameRun && !EMPTY_PERSONHP)
	{
		BnsSleep(100);
		CTeamMember Captain;
		if (!CTeamMemberExtend::GetInstance().GetCaptain(&Captain))
		{
			LOG_CF_E(L"还没队长呢!");
			break;
		}
		if (Captain.GetId() == CPerson::GetInstance().GetId())
		{
			LOG_MSG_CF(L"自己就是队长好吧?");
			break;
		}

		CPlayer Player;
		if (!CPlayerExtend::GetInstance().FindPlayer_By_Id(Captain.GetId(), CBaseObject::CreateFixPointInfo(30.0f), &Player))
		{
			LOG_CF_E(L"附近找不到队长,ID:%X", Captain.GetId());
			BnsSleep(1000);
			continue;
		}
		else if (Player.GetHp() == NULL)
		{
			LOG_CF_D(L"队长挂了……等待复活!");
			BnsMove(Player.GetPoint(), 3);
			continue;
		}

		if (Player.GetDis() > 3.0f)
		{
			BnsMove(Player.GetPoint(), 3);
			continue;
		}

		if (!Player.IsUseSkilling())
			continue;

		DWORD dwTargetId = Player.GetTargetId();
		LOG_CF_D(L"队长使用技能了!,目标ID:%X", dwTargetId);
		if (dwTargetId == NULL)
			continue;

		CMonster Monster;
		if (!CMonsterExtend::GetInstance().FindMonster_By_Id(dwTargetId, CBaseObject::CreateFixPointInfo(30.0f), &Monster))
		{
			LOG_CF_D(L"无法找到队长的目标:%X", dwTargetId);
			continue;
		}

		if (fnGetAroundMonsterCount() == 1)
			KillMonster_By_Single(Monster, CBaseObject::CreateFixPointInfo(30.0f), FALSE, fnGetAroundMonsterCount);
		else
			KillMonster_By_AOE(Monster, CBaseObject::CreateFixPointInfo(30.0f), FALSE, fnGetAroundMonsterCount);

	}
	return TRUE;
}
