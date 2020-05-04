#ifndef __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_SCRIPTEXTEND_H__
#define __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_SCRIPTEXTEND_H__

#include <MyTools/Script.h>
#include <MyTools/ClassInstance.h>

class CScriptExtend : private MyTools::CScript, public MyTools::CClassInstance<CScriptExtend>
{
public:
	CScriptExtend() = default;
	~CScriptExtend() = default;

	BOOL Script_If(_In_ CONST Script_Code_If& ScriptCodeIf) CONST;

	BOOL Script_While(_In_ CONST Script_Code_If& ScriptCodeWhile) CONST;
private:
	// 任务杀怪数量 '<=' '>=' '=='
	BOOL TaskMonsterCount_Below(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL TaskMonsterCount_Above(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL TaskMonsterCount_Equal(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL TaskMonsterCount(_In_ CONST Script_Code_If& ScriptCode, _In_ std::function<BOOL(DWORD,DWORD)> CompPtr) CONST;


	// 猫的血量 '<=' '>='
	BOOL CatHP_BelowEqual(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CatHP_AboveEqual(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CatHP(_In_ CONST Script_Code_If& ScriptCode, _In_ std::function<BOOL(DWORD, DWORD)> CompPtr) CONST;


	// 技能不存在
	BOOL UnExistSkill(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// 判断物品数量 '<' '>'
	BOOL ExistItemCount_Below(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL ExistItemCount_Above(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL ExistItemCount(_In_ CONST Script_Code_If& ScriptCode, _In_ std::function<BOOL(DWORD, DWORD)> CompPtr) CONST;


	// 判断BUFF存在,判断BUFF不存在
	BOOL PersonBuff_Exist(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL PersonBuff_UnExist(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// 怪物存在, 怪物不存在
	BOOL CheckMonster_Exist(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CheckMonster_UnExist(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// 怪到达坐标
	BOOL MonsterArrivePoint(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// NPC存在,NPC不存在
	BOOL CheckNpc_Exist(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CheckNpc_UnExist(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// 判断物品存在,判断物品不存在
	BOOL ExistItem(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL UnExistItem(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// 金币 '>','<'
	BOOL CheckGold_Below(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CheckGold_Above(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// 做完所有主线!=
	BOOL IsFinishMainLine(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// 任务尚未完成
	BOOL UnFinishTask(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// 人物坐标距离 '<','>'
	BOOL PersonPointDis_Below(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL PersonPointDis_Above(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// 判断人物等级大于, 判断人物等级小于, 判断人物等级等于
	BOOL CheckPersonLevel_Above(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CheckPersonLevel_Below(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CheckPersonLevel_Equal(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// 判断任务存在
	BOOL ExistTask(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// 武器持久度<
	BOOL WeasponDurability_Below(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// 背包可用格子数量<
	BOOL ItemUsefulCount_Below(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// 当前城市ID==, 当前城市ID!=
	BOOL CheckCurrentCityId_Equal(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CheckCurrentCityId_UnEqual(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// 人物名称==, 人物名称!=
	BOOL CheckPersonName_Equal(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CheckPersonName_UnEqual(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// HP< , HP>
	BOOL CheckPersonHp_Below(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CheckPersonHp_Above(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// 判断武器名称==, 判断武器名称!=
	BOOL CheckWeasponName_Equal(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CheckWeasponName_UnEqual(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// 模式
	BOOL CheckMode(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// 人物阵营
	BOOL CheckCamp(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// 死亡
	BOOL CheckDead(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// 职业
	BOOL CheckClasses(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// 队伍人数!=
	BOOL CheckTeamMemberCount(_In_ CONST Script_Code_If& ScriptCode) CONST;

	
	// 队伍不在一个频道
	BOOL CheckSameChannelInTeam(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// 判断队伍距离
	BOOL CheckTeamDis(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// 判断队伍地图
	BOOL CheckTeamMap(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// 当前地图!=,==
	BOOL CheckCurrentMap_Equal(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CheckCurrentMap_UnEqual(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// 当前人物队伍位置
	BOOL ChhekCurrentTeamIndex(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// 人物进度队伍位置
	BOOL CheckTeamScheduleIndex(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// 怪物剩余数量 >=, ==, <=
	BOOL CheckSurplusMonsterCount_Below(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CheckSurplusMonsterCount_Above(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CheckSurplusMonsterCount_Equal(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CheckSurplusMonsterCount(_In_ CONST Script_Code_If& ScriptCode, _In_ std::function<BOOL(DWORD, DWORD)> CompPtr) CONST;


	// 云配置==, 云配置!=
	BOOL CloudConfig_Equal(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CloudConfig_UnEqual(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// 是否开门
	BOOL CheckDoor(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// 变量==
	BOOL CheckVariable(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// 仓库位置==
	BOOL CheckWarehouseLocation(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// 星币数量 >=,==,<=
	BOOL CheckStarCoin_Below(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CheckStarCoin_Above(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CheckStarCoin_Equal(_In_ CONST Script_Code_If& ScriptCode) CONST;

	// 装备存在, 装备不存在
	BOOL EquiExist(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL EquiUnExist(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CheckEqui(_In_ CONST Script_Code_If& ScriptCode) CONST;

private:
	struct ScriptCodeExtend
	{
		std::wstring wsCondition;
		std::function<BOOL(CONST Script_Code_If&)> MethodPtr;
	};
};


#endif // !__GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_SCRIPTEXTEND_H__
