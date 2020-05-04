#include "stdafx.h"
#include "ScriptExtend.h"
#include <MyTools/Log.h>
#include "Task.h"
#include "CatExpend.h"
#include "Person.h"
#include "PersonFunction.h"
#include "FarmSkill.h"
#include "BagFunction.h"
#include "MonsterExtend.h"
#include "Monster.h"
#include "NpcExtend.h"
#include "ObjectFunction.h"
#include "TeamMemberExtend.h"
#include "TeamMember.h"
#include "BnsConfig.h"
#include "ResExtend.h"
#include "Corpse.h"
#if BnsProject_DLL
#include "ClientTeam.h"
#endif

#define _SELF L"ScriptExtend.cpp"

#define REGISTER_SCRIPT_CODE_EXTEND(MethodText, MethodName) { MethodText, std::bind(&CScriptExtend::MethodName, this, std::placeholders::_1) }

#define CHECK_SCRIPT_CODE_PARAMETER(ParamCount, CorrectFormat)	\
	if (ScriptCode.VecValue.size() != ParamCount)						\
	{															\
		LOG_MSG_CF( L"参数不对! 应该是%s才对! 现在是:条件=[%s], in [%s]", CorrectFormat, ScriptCode.wsCondition.c_str(), ScriptCode.wsMethodName.c_str());	\
		return FALSE;											\
	}															\

BOOL CScriptExtend::Script_If(_In_ CONST Script_Code_If& ScriptCodeIf) CONST
{
	return Script_While(ScriptCodeIf);
}

BOOL CScriptExtend::Script_While(_In_ CONST Script_Code_If& ScriptCodeWhile) CONST
{
	CONST static std::vector<ScriptCodeExtend> VecScriptCodeExtend = 
	{
		REGISTER_SCRIPT_CODE_EXTEND(L"任务杀怪数量<", TaskMonsterCount_Below),
		REGISTER_SCRIPT_CODE_EXTEND(L"任务杀怪数量>", TaskMonsterCount_Above),
		REGISTER_SCRIPT_CODE_EXTEND(L"任务杀怪数量=", TaskMonsterCount_Equal),
		REGISTER_SCRIPT_CODE_EXTEND(L"猫的血量<=", CatHP_BelowEqual),
		REGISTER_SCRIPT_CODE_EXTEND(L"猫的血量>=", CatHP_AboveEqual),
		REGISTER_SCRIPT_CODE_EXTEND(L"技能不存在", UnExistSkill),
		REGISTER_SCRIPT_CODE_EXTEND(L"判断物品数量<", ExistItemCount_Below),
		REGISTER_SCRIPT_CODE_EXTEND(L"判断物品数量>", ExistItemCount_Above),
		REGISTER_SCRIPT_CODE_EXTEND(L"判断BUFF存在", PersonBuff_Exist),
		REGISTER_SCRIPT_CODE_EXTEND(L"判断BUFF不存在", PersonBuff_UnExist),
		REGISTER_SCRIPT_CODE_EXTEND(L"怪物存在", CheckMonster_Exist),
		REGISTER_SCRIPT_CODE_EXTEND(L"怪物不存在", CheckMonster_UnExist),
		REGISTER_SCRIPT_CODE_EXTEND(L"怪到达坐标", MonsterArrivePoint),
		REGISTER_SCRIPT_CODE_EXTEND(L"NPC存在", CheckNpc_Exist),
		REGISTER_SCRIPT_CODE_EXTEND(L"NPC不存在", CheckNpc_UnExist),
		REGISTER_SCRIPT_CODE_EXTEND(L"判断物品存在", ExistItem),
		REGISTER_SCRIPT_CODE_EXTEND(L"判断物品不存在", UnExistItem),
		REGISTER_SCRIPT_CODE_EXTEND(L"金币<", CheckGold_Below),
		REGISTER_SCRIPT_CODE_EXTEND(L"金币>", CheckGold_Above),
		REGISTER_SCRIPT_CODE_EXTEND(L"做完所有主线!=", IsFinishMainLine),
		REGISTER_SCRIPT_CODE_EXTEND(L"任务尚未完成", UnFinishTask),
		REGISTER_SCRIPT_CODE_EXTEND(L"人物坐标距离<", PersonPointDis_Below),
		REGISTER_SCRIPT_CODE_EXTEND(L"人物坐标距离>", PersonPointDis_Above),
		REGISTER_SCRIPT_CODE_EXTEND(L"判断人物等级大于", CheckPersonLevel_Above),
		REGISTER_SCRIPT_CODE_EXTEND(L"判断人物等级小于", CheckPersonLevel_Below),
		REGISTER_SCRIPT_CODE_EXTEND(L"判断人物等级等于", CheckPersonLevel_Equal),
		REGISTER_SCRIPT_CODE_EXTEND(L"判断任务存在", ExistTask),
		REGISTER_SCRIPT_CODE_EXTEND(L"武器持久度<", WeasponDurability_Below),
		REGISTER_SCRIPT_CODE_EXTEND(L"背包可用格子数量<", ItemUsefulCount_Below),
		REGISTER_SCRIPT_CODE_EXTEND(L"当前城市ID==", CheckCurrentCityId_Equal),
		REGISTER_SCRIPT_CODE_EXTEND(L"当前城市ID!=", CheckCurrentCityId_UnEqual),
		REGISTER_SCRIPT_CODE_EXTEND(L"人物名称==", CheckPersonName_Equal),
		REGISTER_SCRIPT_CODE_EXTEND(L"人物名称!=", CheckPersonName_UnEqual),
		REGISTER_SCRIPT_CODE_EXTEND(L"HP<", CheckPersonHp_Below),
		REGISTER_SCRIPT_CODE_EXTEND(L"HP>", CheckPersonHp_Above),
		REGISTER_SCRIPT_CODE_EXTEND(L"判断武器名称==", CheckWeasponName_Equal),
		REGISTER_SCRIPT_CODE_EXTEND(L"判断武器名称!=", CheckWeasponName_UnEqual),
		REGISTER_SCRIPT_CODE_EXTEND(L"模式", CheckMode),
		REGISTER_SCRIPT_CODE_EXTEND(L"怪物是否还在", CheckMonster_Exist),
		REGISTER_SCRIPT_CODE_EXTEND(L"人物阵营", CheckCamp),
		REGISTER_SCRIPT_CODE_EXTEND(L"死亡", CheckDead),
		REGISTER_SCRIPT_CODE_EXTEND(L"职业", CheckClasses),
		REGISTER_SCRIPT_CODE_EXTEND(L"队伍人数!=", CheckTeamMemberCount),
		REGISTER_SCRIPT_CODE_EXTEND(L"队伍不在一个频道", CheckSameChannelInTeam),
		REGISTER_SCRIPT_CODE_EXTEND(L"Npc距离<", CheckNpc_Exist),
		REGISTER_SCRIPT_CODE_EXTEND(L"判断队伍距离", CheckTeamDis),
		REGISTER_SCRIPT_CODE_EXTEND(L"判断队伍地图", CheckTeamMap),
		REGISTER_SCRIPT_CODE_EXTEND(L"当前地图!=", CheckCurrentMap_UnEqual),
		REGISTER_SCRIPT_CODE_EXTEND(L"当前地图==", CheckCurrentMap_Equal),
		REGISTER_SCRIPT_CODE_EXTEND(L"当前人物队伍位置", ChhekCurrentTeamIndex),
		REGISTER_SCRIPT_CODE_EXTEND(L"人物进度队伍位置", CheckTeamScheduleIndex),
		REGISTER_SCRIPT_CODE_EXTEND(L"怪物剩余数量>=", CheckSurplusMonsterCount_Above),
		REGISTER_SCRIPT_CODE_EXTEND(L"怪物剩余数量==", CheckSurplusMonsterCount_Equal),
		REGISTER_SCRIPT_CODE_EXTEND(L"怪物剩余数量<=", CheckSurplusMonsterCount_Below),
		REGISTER_SCRIPT_CODE_EXTEND(L"云配置==", CloudConfig_Equal),
		REGISTER_SCRIPT_CODE_EXTEND(L"云配置!=", CloudConfig_UnEqual),
		REGISTER_SCRIPT_CODE_EXTEND(L"是否开门", CheckDoor),
		REGISTER_SCRIPT_CODE_EXTEND(L"变量==", CheckVariable),
		REGISTER_SCRIPT_CODE_EXTEND(L"仓库位置==", CheckWarehouseLocation),
		REGISTER_SCRIPT_CODE_EXTEND(L"星币数量>=", CheckStarCoin_Above),
		REGISTER_SCRIPT_CODE_EXTEND(L"星币数量==", CheckStarCoin_Equal),
		REGISTER_SCRIPT_CODE_EXTEND(L"星币数量<=", CheckStarCoin_Below),
		REGISTER_SCRIPT_CODE_EXTEND(L"装备存在", EquiExist),
		REGISTER_SCRIPT_CODE_EXTEND(L"装备不存在", EquiUnExist),
	};

	CONST auto p = MyTools::CLPublic::Vec_find_if_Const(VecScriptCodeExtend, [ScriptCodeWhile](CONST ScriptCodeExtend& ScriptCodeExtend_){ return ScriptCodeExtend_.wsCondition == ScriptCodeWhile.wsCondition; });
	if (p == nullptr)
	{
		LOG_MSG_CF( L"while不支持该判断:%s", ScriptCodeWhile.wsCondition.c_str());
		return FALSE;
	}

	//LOG_CF_D( L"执行判断Condition=%s,DefMethodName=%s,Type=%X", ScriptCodeWhile.wsCondition.c_str(), ScriptCodeWhile.wsMethodName.c_str(), ScriptCodeWhile.emScriptCodeType);
	return GameRun && p->MethodPtr(ScriptCodeWhile);
}

#pragma region 任务杀怪数量
BOOL CScriptExtend::TaskMonsterCount_Below(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(3, L"[TaskID,ScheduleID,Count]")
	return TaskMonsterCount(ScriptCode, [](DWORD dwTaskCount, DWORD dwCount){ return dwTaskCount < dwCount; });
}

BOOL CScriptExtend::TaskMonsterCount_Above(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(3, L"[TaskID,ScheduleID,Count]")
	return TaskMonsterCount(ScriptCode, [](DWORD dwTaskCount, DWORD dwCount){ return dwTaskCount > dwCount; });
}

BOOL CScriptExtend::TaskMonsterCount_Equal(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(3, L"[TaskID,ScheduleID,Count]")
	return TaskMonsterCount(ScriptCode, [](DWORD dwTaskCount, DWORD dwCount){ return dwTaskCount == dwCount; });
}

BOOL CScriptExtend::TaskMonsterCount(_In_ CONST Script_Code_If& ScriptCode, _In_ std::function<BOOL(DWORD, DWORD)> CompPtr) CONST
{
	TaskInfoParm TaskInfoParm_(ScriptCode.VecValue.at(0).ConvertDWORD(), ScriptCode.VecValue.at(1).ConvertDWORD());
	if (!CTask::GetInstance().IsCompletionTask(TaskInfoParm_))
	{
		DWORD dwCount = ScriptCode.VecValue.at(2).ConvertDWORD_By_Dec();
		DWORD dwTaskCount = CTask::GetInstance().QueryTaskScheduleCount(TaskInfoParm_);
		LOG_CF_D( L"现在杀怪数量=%d, 参数杀怪数量=%d", dwTaskCount, dwCount);
		return CompPtr(dwTaskCount, dwCount);
	}
	LOG_CF_D( L"任务[%s,%s]已经完成了! 就不跳转了!", ScriptCode.VecValue.at(0).GetString().c_str(), ScriptCode.VecValue.at(1).GetString().c_str());
	return FALSE;
}
#pragma endregion

#pragma region 猫的血量
BOOL CScriptExtend::CatHP_BelowEqual(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"[PercentHP]")
	return CatHP(ScriptCode, [](DWORD dwCatHp, DWORD dwHp){ return dwCatHp <= dwHp; });
}

BOOL CScriptExtend::CatHP_AboveEqual(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"[PercentHP]")
	return CatHP(ScriptCode, [](DWORD dwCatHp, DWORD dwHp){ return dwCatHp >= dwHp; });
}

BOOL CScriptExtend::CatHP(_In_ CONST Script_Code_If& ScriptCode, _In_ std::function<BOOL(DWORD, DWORD)> CompPtr) CONST
{
	CCat Cat;
	if (!CCatExpend::GetInstance().FindSelfCat(&Cat))
		return FALSE;

	return CompPtr(Cat.GetPercentHp(), ScriptCode.VecValue.at(0).ConvertDWORD_By_Dec());
}
#pragma  endregion

#pragma region 技能不存在
BOOL CScriptExtend::UnExistSkill(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"[SkillName]")
	return !CFarmSkill::GetInstance().GetSkill_By_Name(ScriptCode.VecValue.at(0).GetString(), nullptr);
}
#pragma endregion

#pragma region 判断物品数量
BOOL CScriptExtend::ExistItemCount_Below(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(2, L"[ItemName,ItemCount]")
	return ExistItemCount(ScriptCode, [](DWORD dwExistCount, DWORD dwItemCount){ return  dwExistCount < dwItemCount; });
}

BOOL CScriptExtend::ExistItemCount_Above(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(2, L"[ItemName,ItemCount]")
	return ExistItemCount(ScriptCode, [](DWORD dwExistCount, DWORD dwItemCount){ return  dwExistCount > dwItemCount; });
}

BOOL CScriptExtend::ExistItemCount(_In_ CONST Script_Code_If& ScriptCode, _In_ std::function<BOOL(DWORD, DWORD)> CompPtr) CONST
{
	DWORD dwItemCount = CBagFunction::GetInstance().GetItemCount_By_Name(ScriptCode.VecValue.at(0).GetString());
	LOG_C_D(L"Item[%s] Count=%d, CompCount=%d", ScriptCode.VecValue.at(0).GetString().c_str(), dwItemCount, ScriptCode.VecValue.at(1).ConvertDWORD_By_Dec());
	return CompPtr(dwItemCount, ScriptCode.VecValue.at(1).ConvertDWORD_By_Dec());
}
#pragma endregion

#pragma region 判断BUFF存在

BOOL CScriptExtend::PersonBuff_Exist(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"[BuffId]")
	return CPersonFunction::GetInstance().ExistBuff_Person(ScriptCode.VecValue.at(0).ConvertDWORD(), nullptr);
}

BOOL CScriptExtend::PersonBuff_UnExist(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"[BuffId]")
	return !CPersonFunction::GetInstance().ExistBuff_Person(ScriptCode.VecValue.at(0).ConvertDWORD(), nullptr);
}

#pragma endregion

#pragma region 怪物存在

BOOL CScriptExtend::CheckMonster_Exist(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(2, L"[MonsterName,Dis]");
	return CMonsterExtend::GetInstance().FindMonster_By_Name(ScriptCode.VecValue.at(0).GetString(), CBaseObject::CreateFixPointInfo(ScriptCode.VecValue.at(1).ConvertFLOAT()), nullptr);
}

BOOL CScriptExtend::CheckMonster_UnExist(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(2, L"[MonsterName,Dis]");
	return !CMonsterExtend::GetInstance().FindMonster_By_Name(ScriptCode.VecValue.at(0).GetString(), CBaseObject::CreateFixPointInfo(ScriptCode.VecValue.at(1).ConvertFLOAT()), nullptr);
}
#pragma endregion

#pragma region 怪到达坐标
BOOL CScriptExtend::MonsterArrivePoint(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(5, L"[Name,X,Y,Z,dis]");

	FixPointInfo RangePoint;
	RangePoint.FixPoint.X = ScriptCode.VecValue.at(1).ConvertFLOAT();
	RangePoint.FixPoint.Y = ScriptCode.VecValue.at(2).ConvertFLOAT();
	RangePoint.FixPoint.Z = ScriptCode.VecValue.at(3).ConvertFLOAT();
	RangePoint.fDis = ScriptCode.VecValue.at(4).ConvertFLOAT();

	CMonster Monster;
	if (!CMonsterExtend::GetInstance().FindMonster_By_Name(ScriptCode.VecValue.at(0).GetString(), RangePoint, &Monster))
	{
		LOG_CF_D( L"附近%.2fm都找不到%s, 就此跳出!", RangePoint.fDis, ScriptCode.VecValue.at(0).GetString().c_str());
		return FALSE;
	}

	if (CBaseObject::Get3DDis(Monster.GetPoint(), RangePoint.FixPoint) < RangePoint.fDis)
	{
		LOG_CF_D( L"怪物:%s 已经进到指定的地点!", Monster.GetName().c_str());
		return FALSE;
	}
	return TRUE;
}
#pragma endregion

#pragma region NPC存在
BOOL CScriptExtend::CheckNpc_Exist(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(2, L"[NpcName,Dis]");
	return CNpcExtend::GetInstance().FindNpc_By_Name(ScriptCode.VecValue.at(0).GetString(), CBaseObject::CreateFixPointInfo(ScriptCode.VecValue.at(1).ConvertFLOAT()), nullptr);
}

BOOL CScriptExtend::CheckNpc_UnExist(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(2, L"[NpcName,Dis]");
	return !CNpcExtend::GetInstance().FindNpc_By_Name(ScriptCode.VecValue.at(0).GetString(), CBaseObject::CreateFixPointInfo(ScriptCode.VecValue.at(1).ConvertFLOAT()), nullptr);
}
#pragma endregion

#pragma region 判断物品存在
BOOL CScriptExtend::ExistItem(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"[ItemName]");
	return CBagFunction::GetInstance().ExistItem_By_Name(ScriptCode.VecValue.at(0).GetString(), nullptr);
}

BOOL CScriptExtend::UnExistItem(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"[ItemName]");
	return !CBagFunction::GetInstance().ExistItem_By_Name(ScriptCode.VecValue.at(0).GetString(), nullptr);
}
#pragma endregion

#pragma region 金币
BOOL CScriptExtend::CheckGold_Below(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"GoldNumber");
	return CPerson::GetInstance().GetMoney() < ScriptCode.VecValue.at(0).ConvertDWORD_By_Dec();
}

BOOL CScriptExtend::CheckGold_Above(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"GoldNumber");
	return CPerson::GetInstance().GetMoney() > ScriptCode.VecValue.at(0).ConvertDWORD_By_Dec();
}
#pragma endregion

#pragma region 做完所有主线!=
BOOL CScriptExtend::IsFinishMainLine(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"Value");
	return CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_FINISH_ALL_MAINTASK) != ScriptCode.VecValue.at(0).ConvertDWORD_By_Dec();
}
#pragma endregion

#pragma region 任务尚未完成
BOOL CScriptExtend::UnFinishTask(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(3, L"[TaskId,ScheduleId,KillCount]");
	TaskContentMsg TaskContentMsg_;
	TaskContentMsg_.Ti.dwTaskId = ScriptCode.VecValue.at(0).ConvertDWORD();
	TaskContentMsg_.Ti.dwScheduleId = ScriptCode.VecValue.at(1).ConvertDWORD();
	TaskContentMsg_.dwKillCount = ScriptCode.VecValue.at(2).ConvertDWORD_By_Dec();

	if (TaskContentMsg_.dwKillCount == 0)
		return !CTask::GetInstance().IsCompletionTask(TaskContentMsg_.Ti);

	return !CTask::GetInstance().IsCompletionTask(TaskContentMsg_.Ti) && CTask::GetInstance().QueryTaskScheduleCount(TaskContentMsg_.Ti) < TaskContentMsg_.dwKillCount;
}
#pragma endregion

#pragma region 人物坐标距离
BOOL CScriptExtend::PersonPointDis_Below(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(4, L"[X,Y,Z,dis]");
	Point Point_;
	Point_.X = ScriptCode.VecValue.at(0).ConvertFLOAT();
	Point_.Y = ScriptCode.VecValue.at(1).ConvertFLOAT();
	Point_.Z = ScriptCode.VecValue.at(2).ConvertFLOAT();

	return CBaseObject::GetPersonDis(Point_) < ScriptCode.VecValue.at(3).ConvertFLOAT();
}

BOOL CScriptExtend::PersonPointDis_Above(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(4, L"[X,Y,Z,dis]");
	Point Point_;
	Point_.X = ScriptCode.VecValue.at(0).ConvertFLOAT();
	Point_.Y = ScriptCode.VecValue.at(1).ConvertFLOAT();
	Point_.Z = ScriptCode.VecValue.at(2).ConvertFLOAT();

	return CBaseObject::GetPersonDis(Point_) > ScriptCode.VecValue.at(3).ConvertFLOAT();
}
#pragma endregion


BOOL CScriptExtend::CheckPersonLevel_Above(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"[Level]");
	return CPerson::GetInstance().GetLevel() > ScriptCode.VecValue.at(0).ConvertDWORD_By_Dec();
}

BOOL CScriptExtend::CheckPersonLevel_Below(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"[Level]");
	return CPerson::GetInstance().GetLevel() < ScriptCode.VecValue.at(0).ConvertDWORD_By_Dec();
}

BOOL CScriptExtend::CheckPersonLevel_Equal(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"[Level]");
	return CPerson::GetInstance().GetLevel() == ScriptCode.VecValue.at(0).ConvertDWORD_By_Dec();
}

BOOL CScriptExtend::ExistTask(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"[TaskID]");
	return CTask::GetInstance().ExistTask(ScriptCode.VecValue.at(0).ConvertDWORD(), nullptr);
}

BOOL CScriptExtend::WeasponDurability_Below(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"[WeasponDurability]");
	return CBagFunction::GetInstance().GetEndurance() < ScriptCode.VecValue.at(0).ConvertDWORD_By_Dec();
}

BOOL CScriptExtend::ItemUsefulCount_Below(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"[ItemUsefulCount]");
	return CBagFunction::GetInstance().IsBagFull(CObjectFunction::em_Bag_Type::em_Bag_Type_Bag, ScriptCode.VecValue.at(0).ConvertDWORD_By_Dec());
}

BOOL CScriptExtend::CheckCurrentCityId_Equal(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"[CityID]");
	return CPersonFunction::GetInstance().GetCityId() == ScriptCode.VecValue.at(0).ConvertDWORD();
}

BOOL CScriptExtend::CheckCurrentCityId_UnEqual(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	//CHECK_SCRIPT_CODE_PARAMETER(1, L"[CityID]");
	for (CONST auto& itm : ScriptCode.VecValue)
	{
		if (CPersonFunction::GetInstance().GetCityId() == itm.ConvertDWORD())
			return FALSE;
	}
	return TRUE;
}

BOOL CScriptExtend::CheckPersonName_Equal(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"[Name]");
	return CPerson::GetInstance().GetName() == ScriptCode.VecValue.at(0).GetString();
}

BOOL CScriptExtend::CheckPersonName_UnEqual(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"[Name]");
	return CPerson::GetInstance().GetName() != ScriptCode.VecValue.at(0).GetString();
}

BOOL CScriptExtend::CheckPersonHp_Below(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"[HP]");
	return CPerson::GetInstance().GetPercentHp() < ScriptCode.VecValue.at(0).ConvertDWORD_By_Dec();
}

BOOL CScriptExtend::CheckPersonHp_Above(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"[HP]");
	return CPerson::GetInstance().GetPercentHp() > ScriptCode.VecValue.at(0).ConvertDWORD_By_Dec();
}

BOOL CScriptExtend::CheckWeasponName_Equal(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"[WeasponName]");
	
	CBag Weaspon;
	return CBagFunction::GetInstance().GetEqui_By_Type(CBag::em_Equi_Type_Weapon, &Weaspon) && Weaspon.GetName() == ScriptCode.VecValue.at(0).GetString();
}

BOOL CScriptExtend::CheckWeasponName_UnEqual(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"[WeasponName]");

	CBag Weaspon;
	return CBagFunction::GetInstance().GetEqui_By_Type(CBag::em_Equi_Type_Weapon, &Weaspon) && Weaspon.GetName() != ScriptCode.VecValue.at(0).GetString();
}

BOOL CScriptExtend::CheckMode(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"ModeName");
	return std::wstring(CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.wszTypeName) == ScriptCode.VecValue.at(0).GetString();
}

BOOL CScriptExtend::CheckCamp(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"[CampText]");
	auto emCamp = CPerson::GetInstance().GetCamp();
	return (emCamp == em_Camp::em_Camp_武林盟 && ScriptCode.VecValue.at(0).GetString() == L"武林盟") || (emCamp == em_Camp::em_Camp_浑天教 && ScriptCode.VecValue.at(0).GetString() == L"浑天教");
}

BOOL CScriptExtend::CheckDead(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	return EMPTY_PERSONHP;
}

BOOL CScriptExtend::CheckClasses(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"[ClassesText]");
	return CPerson::GetInstance().GetClassesText() == ScriptCode.VecValue.at(0).GetString();
}

BOOL CScriptExtend::CheckTeamMemberCount(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"TeamMemberCount");
	return CTeamMemberExtend::GetInstance().GetTeamMemberCount() != ScriptCode.VecValue.at(0).ConvertDWORD_By_Dec();
}

BOOL CScriptExtend::CheckSameChannelInTeam(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	return !CTeamMemberExtend::GetInstance().IsSameChannel_By_Team();
}

BOOL CScriptExtend::CheckTeamDis(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(6, L"[Index,Symbol,X,Y,Z,dis]");
	Point Point_;
	Point_.X = ScriptCode.VecValue.at(2).ConvertFLOAT();
	Point_.Y = ScriptCode.VecValue.at(3).ConvertFLOAT();
	Point_.Z = ScriptCode.VecValue.at(4).ConvertFLOAT();

	CTeamMember TeamMember;
	if (!CTeamMemberExtend::GetInstance().GetTeamMemberByIndex(ScriptCode.VecValue.at(0).ConvertDWORD_By_Dec(), &TeamMember))
		return FALSE;

	if (ScriptCode.VecValue.at(1).GetString() == L"<")
		return CBaseObject::GetPersonDis(Point_) < ScriptCode.VecValue.at(5).ConvertFLOAT();
	else if (ScriptCode.VecValue.at(1).GetString() == L">")
		return CBaseObject::GetPersonDis(Point_) > ScriptCode.VecValue.at(5).ConvertFLOAT();
	
	LOG_MSG_CF( L"只能用< >! 你这个是什么鬼:%s", ScriptCode.VecValue.at(1).GetString().c_str());
	return FALSE;
}

BOOL CScriptExtend::CheckTeamMap(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(2, L"[Index,MapId]");

	CTeamMember TeamMember;
	if (!CTeamMemberExtend::GetInstance().GetTeamMemberByIndex(ScriptCode.VecValue.at(0).ConvertDWORD_By_Dec(), &TeamMember))
		return FALSE;

	return TeamMember.GetMapId() == ScriptCode.VecValue.at(1).ConvertDWORD();
}

BOOL CScriptExtend::CheckCurrentMap_Equal(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"MapId");
	return CPerson::GetInstance().GetMapId() == ScriptCode.VecValue.at(0).ConvertDWORD();
}

BOOL CScriptExtend::CheckCurrentMap_UnEqual(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"MapId");
	return CPerson::GetInstance().GetMapId() != ScriptCode.VecValue.at(0).ConvertDWORD();
}

BOOL CScriptExtend::ChhekCurrentTeamIndex(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"Index");
	return CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == ScriptCode.VecValue.at(0).ConvertDWORD_By_Dec();
}

BOOL CScriptExtend::CheckTeamScheduleIndex(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"Index");
#if BnsProject_DLL
	CClientTeam::GetInstance().RefreshGunoneTeamSchedule();
	auto& TeamDungoneSchedule_ = CClientTeam::GetInstance().GetTeamDungoenSchedule();

	// 搜索当前人物在进度的第几个位置, 然后判断是否=参数位置
	CONST std::wstring wsPlayerName = CPerson::GetInstance().GetName();
	for (UINT i = 0; i < TeamDungoneSchedule_.VecMember.size(); ++i)
	{
		if (TeamDungoneSchedule_.VecMember.at(i).wsPlayerName == wsPlayerName)
			return ScriptCode.VecValue.at(0).ConvertDWORD_By_Dec() == static_cast<DWORD>(i);
	}

	return FALSE;
#else
	return CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == ScriptCode.VecValue.at(0).ConvertDWORD_By_Dec();
#endif
}

BOOL CScriptExtend::CheckSurplusMonsterCount_Below(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(3, L"[MonsterName,Count,dis]");
	return CheckSurplusMonsterCount(ScriptCode, [](DWORD dwExistCount, DWORD dwCompCount){ return dwExistCount <= dwCompCount; });
}

BOOL CScriptExtend::CheckSurplusMonsterCount_Above(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(3, L"[MonsterName,Count,dis]");
	return CheckSurplusMonsterCount(ScriptCode, [](DWORD dwExistCount, DWORD dwCompCount){ return dwExistCount >= dwCompCount; });
}

BOOL CScriptExtend::CheckSurplusMonsterCount_Equal(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(3, L"[MonsterName,Count,dis]");
	return CheckSurplusMonsterCount(ScriptCode, [](DWORD dwExistCount, DWORD dwCompCount){ return dwExistCount == dwCompCount; });
}

BOOL CScriptExtend::CheckSurplusMonsterCount(_In_ CONST Script_Code_If& ScriptCode, _In_ std::function<BOOL(DWORD, DWORD)> CompPtr) CONST
{
	FixPointInfo RangePoint = CBaseObject::CreateFixPointInfo(ScriptCode.VecValue.at(2).ConvertFLOAT());

	std::vector<CMonster> VecMonster;
	CMonsterExtend::GetInstance().GetAroundMonsterListByName(ScriptCode.VecValue.at(0).GetString(), RangePoint, VecMonster);

	return CompPtr(VecMonster.size(), ScriptCode.VecValue.at(1).ConvertDWORD_By_Dec());
}

BOOL CScriptExtend::CloudConfig_Equal(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(2, L"[ConfigName,ConfigValue]");

	return CBnsConfig::GetInstance().GetSingleConfigValueByServer(CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->szPlayerName, ScriptCode.VecValue.at(0).GetString()) == ScriptCode.VecValue.at(1).GetString();
}

BOOL CScriptExtend::CloudConfig_UnEqual(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	return CBnsConfig::GetInstance().GetSingleConfigValueByServer(CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->szPlayerName, ScriptCode.VecValue.at(0).GetString()) != ScriptCode.VecValue.at(1).GetString();
}


BOOL CScriptExtend::CheckDoor(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(4, L"[DoorName,X,Y,Z]");

	Point Point_;
	Point_.X = ScriptCode.VecValue.at(0).ConvertFLOAT();
	Point_.Y = ScriptCode.VecValue.at(1).ConvertFLOAT();
	Point_.Z = ScriptCode.VecValue.at(2).ConvertFLOAT();

	BOOL bRetCode = FALSE;
	CResExtend::GetInstance().DoAction_By_ExistCorpse_For_Name(ScriptCode.VecValue.at(0).GetString(), FixPointInfo(Point_, 3), [&bRetCode](CONST CCorpse& Door){ bRetCode = !Door.IsCollect(); });
	return bRetCode;
}

BOOL CScriptExtend::CheckVariable(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(2, L"[VarName,VarValue]");
	return CGameVariable::GetInstance().GetVariable_By_Name(ScriptCode.VecValue.at(0).GetString()) == ScriptCode.VecValue.at(1).ConvertDWORD();
}

BOOL CScriptExtend::CheckWarehouseLocation(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"WarehouseName");
	CONST auto pWarehouseConfig = CGameVariable::GetInstance().GetWarehouseConfig();
	if (pWarehouseConfig == nullptr)
	{
		LOG_CF_D( L"当前角色不是仓库号!");
		return FALSE;
	}
	
	LOG_CF_D( L"当前仓库号的角色名:%s, Npc:%s, City:%s", pWarehouseConfig->wszPlayerName, pWarehouseConfig->wszNpcName, pWarehouseConfig->wszCityName);
	return std::wstring(pWarehouseConfig->wszCityName) == ScriptCode.VecValue.at(0).GetString();
}

BOOL CScriptExtend::CheckStarCoin_Below(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"Count");
	return CPerson::GetInstance().GetStarCoin() <= ScriptCode.VecValue.at(0).ConvertDWORD_By_Dec();
}

BOOL CScriptExtend::CheckStarCoin_Above(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"Count");
	return CPerson::GetInstance().GetStarCoin() >= ScriptCode.VecValue.at(0).ConvertDWORD_By_Dec();
}

BOOL CScriptExtend::CheckStarCoin_Equal(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(1, L"Count");
	return CPerson::GetInstance().GetStarCoin() == ScriptCode.VecValue.at(0).ConvertDWORD_By_Dec();
}

BOOL CScriptExtend::EquiExist(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(2, L"[Type|Name,Text]");
	return CheckEqui(ScriptCode);
}

BOOL CScriptExtend::EquiUnExist(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	CHECK_SCRIPT_CODE_PARAMETER(2, L"[Type|Name,Text]");
	return !CheckEqui(ScriptCode);
}

BOOL CScriptExtend::CheckEqui(_In_ CONST Script_Code_If& ScriptCode) CONST
{
	if (ScriptCode.VecValue.at(0).GetString() == L"Type")
	{
		CBagFunction::Equi_Type EquiType;
		if (!CBagFunction::GetInstance().GetEquiType_By_Text(ScriptCode.VecValue.at(1).GetString(), EquiType))
		{
			LOG_MSG_CF(L"UnExist Text:[%s]", ScriptCode.VecValue.at(1).GetString().c_str());
			return FALSE;
		}

		return CBagFunction::GetInstance().GetEqui_By_Type(EquiType.emEquiType, nullptr);
	}
	else if (ScriptCode.VecValue.at(0).GetString() == L"Name")
	{
		return CBagFunction::GetInstance().ExistEqui_By_Name(ScriptCode.VecValue.at(1).GetString());
	}

	LOG_MSG_CF(L"Invalid Text:[%s]", ScriptCode.VecValue.at(0).GetString().c_str());
	return FALSE;
}
