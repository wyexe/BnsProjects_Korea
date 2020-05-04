#ifndef __GITBNSPROJECTS_BNSPROJECTS_OBJECT_GAMEOBJECT_TEAM_TEAMMEMBEREXTEND_H__
#define __GITBNSPROJECTS_BNSPROJECTS_OBJECT_GAMEOBJECT_TEAM_TEAMMEMBEREXTEND_H__

#include "GameVariable.h"

class CTeamMember;
class CTeamMemberExtend : public MyTools::CClassInstance<CTeamMemberExtend>
{
public:
	CTeamMemberExtend() = default;
	~CTeamMemberExtend() = default;
public:
	struct TeamIndexClasses
	{
		std::wstring wsPlayerName;
		int			 nIndex;

		TeamIndexClasses(_In_ CONST std::wstring& wsPlayerName_, _In_ int nIndex_) : wsPlayerName(wsPlayerName_), nIndex(nIndex_) {}
	};
public:
	// �ж϶ӳ��Ƿ�ӳ�
	BOOL IsTeamCaptain(_In_ CONST std::wstring& wsPlayerName) CONST;

	// ���ص��ߵĳ�Ա
	BOOL IsTeamMember_LoseConnect(_In_ CTeamMember* pTeamMember) CONST;

	// ����Ҫ��ӵĳ�Ա
	BOOL IsTeamMember_LeaveTeam(_In_ CTeamMember* pTeamMember) CONST;

	// �Ƿ��ڶ�����
	BOOL ExistTeamMember(_In_ CONST std::wstring& wsMemberName, _Out_opt_ CTeamMember* pTeamMember = nullptr) CONST;

	// ���Ҷ�Ա
	BOOL FindTeamMember_By_Condiction(_In_ CTeamMember* pTeamMember, std::function<BOOL(CONST CTeamMember&)> fnFind) CONST;

	// ���ö�������Ʒ��
	BOOL SetAuctionQuality(em_Auction_Quality emAuctionQuality = em_Auction_Quality_Extreme) CONST;

	// �뿪����
	BOOL LeaveTeam() CONST;

	// ��ȡ�����Ա����
	DWORD GetTeamMemberCount() CONST;

	// ��ȡ�ӳ�
	BOOL GetCaptain(_Out_opt_ CTeamMember* pCaptain) CONST;

	// �ж϶����Ƿ���һ��Ƶ��
	BOOL IsSameChannel_By_Team() CONST;

	// ���ص�ǰ�������ڵ�λ��
	DWORD GetCurrentTeamMemberIndex() CONST;

	// ������������TeamMember
	BOOL GetTeamMemberByIndex(_In_ DWORD dwIndex, _Out_opt_ CTeamMember* pTeamMeber) CONST;

	// �ж��Ƿ��ж�ԱHP<%?
	BOOL ExistTeamMemberWhenHpLessValue(_In_ DWORD dwPercentHp) CONST;

	// ��ȡ�����귶Χ���ж��ٸ���Ա
	UINT GetTeamMemberCountByPoint(_In_ CONST FixPointInfo& FixPoint) CONST;
	UINT GetTeamMemberCountByCondition(_In_ std::function<BOOL(CONST CTeamMember&)> Finder) CONST;

	// ��ĳ����Ա���в���
	BOOL Action_By_TeamMemberName(_In_ CONST std::wstring& wsPlayerName, _In_ std::function<VOID(CTeamMember&)> Worker) CONST;

	// �ж϶�Ա�Ƿ����%? ��Ѫ��
	BOOL GetTeamMember_By_LessPercentHp(_In_ DWORD dwPercentHp, _Out_opt_ CTeamMember* pTeamMeber) CONST;

	// ѭ��
	VOID ForEach(_In_ std::function<VOID(CONST CTeamMember& TeamMember)> Worker);

	// ���״̬���Ƿ񸴻�(���Ķ��Ѷ��˾Ͳ�������)
	BOOL IsTryRelive() CONST;

	int GetCurrentTeamIndexByClasses(_In_ em_Player_Classes emPlayerClasses) CONST;

	VOID SetCurrentTeamIndexByClasses();
private:
	std::vector<TeamIndexClasses> VecTeamIndexClasses;
private:
	DSIABLE_COPY_AND_ASSIGN(CTeamMemberExtend)
};



#endif