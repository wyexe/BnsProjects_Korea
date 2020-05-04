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
	// ���ҹ���(������!)
	BOOL FindMonster_By_Name(_In_ CONST std::wstring& wsMonsterName, _In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CMonster* pMonster) CONST;

	//
	BOOL FindMonster_By_Id(_In_ DWORD dwMonsterId, _In_ CONST FixPointInfo& FixPtInfo,  _Out_opt_ CMonster* pMonster) CONST;

	// 
	BOOL FindMonster_By_Condiction(_In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CMonster* pMonster, _In_ std::function<BOOL(CONST CMonster&)> fnFindMonster) CONST;

	// ��ָ���Ĺ����� �� Action
	BOOL DoAction_By_MonsterName(_In_ CONST std::wstring& wsName, _In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<VOID(CONST CMonster&)> fn) CONST;

	// ��ȡ��Χ�Ĺ���(���˺�)
	UINT GetAroundMonsterList(_In_ CONST FixPointInfo& FixPtInfo, _Out_ std::vector<CMonster>& vlst) CONST;

	// ���������һ������(��������)
	BOOL GetTopMonster_By_Dis( _In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CMonster* pMonster) CONST;

	// ������Χ��������(��������)
	UINT GetAroundMonsterCount(_In_ CONST FixPointInfo& FixPtInfo) CONST;

	// ������ΧTargetId=Self�Ĺ����б�(��������)
	UINT GetAroundTargetMonsterList(_In_ CONST FixPointInfo& FixPtInfo, _Out_ std::vector<CMonster>& vlst) CONST;

	// ������ΧMAXHP�Ĺ���
	UINT GetAroundMonsterByMaxHp(_In_ DWORD dwMaxHp, _In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ std::vector<CMonster>& vlst) CONST;

	// ������Χ��ͬ���ֵĹ���
	UINT GetAroundMonsterListByName(_In_ CONST std::wstring& wsName, _In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ std::vector<CMonster>& vlst) CONST;

	// 
	BOOL ExistMonsterBuff(_In_ DWORD dwMonsterId, _In_ DWORD dwBufferId, _In_ DWORD dwBuffCount) CONST;
private:
	DSIABLE_COPY_AND_ASSIGN(CMonsterExtend)
};



#endif