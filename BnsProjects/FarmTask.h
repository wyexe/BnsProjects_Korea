#ifndef __GITBNSPROJECTS_BNSPROJECTS_SOMEFUN_FARM_FARMTASK_H__
#define __GITBNSPROJECTS_BNSPROJECTS_SOMEFUN_FARM_FARMTASK_H__

#include "FarmMonster.h"

class CFarmTask : public CFarmMonster
{
public:
	explicit CFarmTask() = default;
	~CFarmTask() = default;

public:
	// ��� in ����
	BOOL Farm_By_Condiction(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<UINT(VOID)> fnGetAroundCount, _In_ std::function<BOOL(std::vector<CMonster>&, CONST FixPointInfo&)> fnBreak) CONST;

	//������֣�ֻ�ÿ����������κ��������ܡ�
	BOOL Farm_By_Condiction_ts(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<UINT(VOID)> fnGetAroundCount, _In_ std::function<BOOL(std::vector<CMonster>&, CONST FixPointInfo&)> fnBreak) CONST;

	// �����Χ��Χ�� �Ĺ���
	BOOL ClearAroundMonster(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<BOOL(VOID)> fnBreak) CONST;
	BOOL ClearAroundMonster(_In_ CONST FixPointInfo& FixPtInfo) CONST;

	//
	BOOL ClearAroundNpc(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<BOOL(VOID)> fnBreak) CONST;

	// �����Χ�Ĺ��� in List
	//BOOL ClearMonster_In_List_By_Condiction(_In_ CONST FixPointInfo& FixPtInfo, _In_ CONST std::vector<Point>& vPoint, _In_ std::function<BOOL(void)> fnComplete) CONST;

	// �����Χ��Ŀ������Լ�
	BOOL ClearAround_TarIdSelf(_In_ CONST FixPointInfo& FixPtInfo) CONST;

	// �����Χ���ر��Ŀ���
	BOOL ClearAround_TarName(_In_ CONST std::wstring& wsMonsterName, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	//�����Χ���ر��Ŀ���-ֻ����
	BOOL ClearAround_TarName_ts(_In_ CONST std::wstring& wsMonsterName, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	// ��ҩ
	BOOL EatFood(_In_ DWORD dwPercentHp) CONST;

	// ��ʬ��
	//BOOL TakePlayer_By_Condiction(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<BOOL(CONST CPlayer&)> fnFind, _In_ std::function<BOOL(std::vector<CPlayer>&)> fnComplete) CONST;

	//BOOL TakePlayer_By_List(_In_ CONST FixPointInfo& FixPtInfo, _In_ CONST std::vector<Point>& vPoint, _In_ std::function<BOOL(CONST CPlayer&)> fnFind, _In_ std::function<BOOL(std::vector<CPlayer>&)> fnComplete) CONST;

	
	// ����Npc
	BOOL FollowPlayer(_In_ CONST CPlayer& Player, _In_ CONST Point& EndPoint, _In_ float fKillDis) CONST;

	// 'վ�ż�'
	BOOL Pickup_By_Condition(_In_ CONST std::wstring& wsName) CONST;

	// ����ӳ����
	BOOL FollowCaptain() CONST;
private:
	DSIABLE_COPY_AND_ASSIGN(CFarmTask)
	//BOOL DoAround_By_List(_In_ CONST FixPointInfo& FixPtInfo, _In_ CONST std::vector<Point>& vPoint, _In_ std::function<BOOL(CONST FixPointInfo&)> fnBreak) CONST;
};



#endif