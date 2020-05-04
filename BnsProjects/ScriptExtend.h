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
	// ����ɱ������ '<=' '>=' '=='
	BOOL TaskMonsterCount_Below(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL TaskMonsterCount_Above(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL TaskMonsterCount_Equal(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL TaskMonsterCount(_In_ CONST Script_Code_If& ScriptCode, _In_ std::function<BOOL(DWORD,DWORD)> CompPtr) CONST;


	// è��Ѫ�� '<=' '>='
	BOOL CatHP_BelowEqual(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CatHP_AboveEqual(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CatHP(_In_ CONST Script_Code_If& ScriptCode, _In_ std::function<BOOL(DWORD, DWORD)> CompPtr) CONST;


	// ���ܲ�����
	BOOL UnExistSkill(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// �ж���Ʒ���� '<' '>'
	BOOL ExistItemCount_Below(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL ExistItemCount_Above(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL ExistItemCount(_In_ CONST Script_Code_If& ScriptCode, _In_ std::function<BOOL(DWORD, DWORD)> CompPtr) CONST;


	// �ж�BUFF����,�ж�BUFF������
	BOOL PersonBuff_Exist(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL PersonBuff_UnExist(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// �������, ���ﲻ����
	BOOL CheckMonster_Exist(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CheckMonster_UnExist(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// �ֵ�������
	BOOL MonsterArrivePoint(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// NPC����,NPC������
	BOOL CheckNpc_Exist(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CheckNpc_UnExist(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// �ж���Ʒ����,�ж���Ʒ������
	BOOL ExistItem(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL UnExistItem(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// ��� '>','<'
	BOOL CheckGold_Below(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CheckGold_Above(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// ������������!=
	BOOL IsFinishMainLine(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// ������δ���
	BOOL UnFinishTask(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// ����������� '<','>'
	BOOL PersonPointDis_Below(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL PersonPointDis_Above(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// �ж�����ȼ�����, �ж�����ȼ�С��, �ж�����ȼ�����
	BOOL CheckPersonLevel_Above(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CheckPersonLevel_Below(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CheckPersonLevel_Equal(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// �ж��������
	BOOL ExistTask(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// �����־ö�<
	BOOL WeasponDurability_Below(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// �������ø�������<
	BOOL ItemUsefulCount_Below(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// ��ǰ����ID==, ��ǰ����ID!=
	BOOL CheckCurrentCityId_Equal(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CheckCurrentCityId_UnEqual(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// ��������==, ��������!=
	BOOL CheckPersonName_Equal(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CheckPersonName_UnEqual(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// HP< , HP>
	BOOL CheckPersonHp_Below(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CheckPersonHp_Above(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// �ж���������==, �ж���������!=
	BOOL CheckWeasponName_Equal(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CheckWeasponName_UnEqual(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// ģʽ
	BOOL CheckMode(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// ������Ӫ
	BOOL CheckCamp(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// ����
	BOOL CheckDead(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// ְҵ
	BOOL CheckClasses(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// ��������!=
	BOOL CheckTeamMemberCount(_In_ CONST Script_Code_If& ScriptCode) CONST;

	
	// ���鲻��һ��Ƶ��
	BOOL CheckSameChannelInTeam(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// �ж϶������
	BOOL CheckTeamDis(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// �ж϶����ͼ
	BOOL CheckTeamMap(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// ��ǰ��ͼ!=,==
	BOOL CheckCurrentMap_Equal(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CheckCurrentMap_UnEqual(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// ��ǰ�������λ��
	BOOL ChhekCurrentTeamIndex(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// ������ȶ���λ��
	BOOL CheckTeamScheduleIndex(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// ����ʣ������ >=, ==, <=
	BOOL CheckSurplusMonsterCount_Below(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CheckSurplusMonsterCount_Above(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CheckSurplusMonsterCount_Equal(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CheckSurplusMonsterCount(_In_ CONST Script_Code_If& ScriptCode, _In_ std::function<BOOL(DWORD, DWORD)> CompPtr) CONST;


	// ������==, ������!=
	BOOL CloudConfig_Equal(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CloudConfig_UnEqual(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// �Ƿ���
	BOOL CheckDoor(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// ����==
	BOOL CheckVariable(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// �ֿ�λ��==
	BOOL CheckWarehouseLocation(_In_ CONST Script_Code_If& ScriptCode) CONST;


	// �Ǳ����� >=,==,<=
	BOOL CheckStarCoin_Below(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CheckStarCoin_Above(_In_ CONST Script_Code_If& ScriptCode) CONST;
	BOOL CheckStarCoin_Equal(_In_ CONST Script_Code_If& ScriptCode) CONST;

	// װ������, װ��������
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
