#ifndef __GITBNSPROJECTS_BNSPROJECTS_OBJECT_GAMEOBJECT_MONSTER_MONSTEREXTEND_H__
#define __GITBNSPROJECTS_BNSPROJECTS_OBJECT_GAMEOBJECT_MONSTER_MONSTEREXTEND_H__

#include "GameVariable.h"

class CMonster;
class CMonsterExtend : public MyTools::CClassInstance<CMonsterExtend>
{
public:
	CMonsterExtend() = default;
	~CMonsterExtend() = default;

public:
	// 查找怪物(不过滤!)
	BOOL FindMonster_By_Name(_In_ CONST std::wstring& wsMonsterName, _In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CMonster* pMonster) CONST;

	//
	BOOL FindMonster_By_Id(_In_ DWORD dwMonsterId, _In_ CONST FixPointInfo& FixPtInfo,  _Out_opt_ CMonster* pMonster) CONST;

	// 
	BOOL FindMonster_By_Condiction(_In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CMonster* pMonster, _In_ std::function<BOOL(CONST CMonster&)> fnFindMonster) CONST;

	// 对指定的怪物名 做 Action
	BOOL DoAction_By_MonsterName(_In_ CONST std::wstring& wsName, _In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<VOID(CONST CMonster&)> fn) CONST;

	// 获取周围的怪物(过滤后)
	UINT GetAroundMonsterList(_In_ CONST FixPointInfo& FixPtInfo, _Out_ std::vector<CMonster>& vlst) CONST;

	// 返回最近的一个怪物(包括过滤)
	BOOL GetTopMonster_By_Dis( _In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CMonster* pMonster) CONST;

	// 返回周围怪物数量(包含过滤)
	UINT GetAroundMonsterCount(_In_ CONST FixPointInfo& FixPtInfo) CONST;

	// 返回周围TargetId=Self的怪物列表(包括过滤)
	UINT GetAroundTargetMonsterList(_In_ CONST FixPointInfo& FixPtInfo, _Out_ std::vector<CMonster>& vlst) CONST;

	// 搜索周围MAXHP的怪物
	UINT GetAroundMonsterByMaxHp(_In_ DWORD dwMaxHp, _In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ std::vector<CMonster>& vlst) CONST;

	// 搜索周围相同名字的怪物
	UINT GetAroundMonsterListByName(_In_ CONST std::wstring& wsName, _In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ std::vector<CMonster>& vlst) CONST;

	// 
	BOOL ExistMonsterBuff(_In_ DWORD dwMonsterId, _In_ DWORD dwBufferId, _In_ DWORD dwBuffCount) CONST;
private:
	DSIABLE_COPY_AND_ASSIGN(CMonsterExtend)
};



#endif