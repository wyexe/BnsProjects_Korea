#ifndef __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_TEAM_BNSTEAM_H__
#define __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_TEAM_BNSTEAM_H__

#include <map>
#include <deque>
#include <MyTools/CLLock.h>
#include <MyTools/ClassInstance.h>
#include "ServerCommon.h"

struct DungoenTeamMember
{
	std::wstring			wsDungoenSchedule;	// ��������
	UINT					uGatherIndex;		// ����


	DungoenTeamMember()
	{
		wsDungoenSchedule = L"";
		uGatherIndex = 0;
	}
};

// �����Ա��Ϣ(�������Ƿ�ӳ�,�ʺ�,��ɫ��,ID)
struct TeamMemberInfo
{
	BOOL bCaptain;
	ClientTeamMember Member;
	DungoenTeamMember Dungoen;
};

// һ������������˵���Ϣ
struct TeamMemberGroup
{
	std::deque<TeamMemberInfo>	MemberVec;
	UINT					uMaxMemberCount;
	DWORD					dwTaskId;
	DWORD					dwScheduleId;
	DWORD					dwChannelId;
	DWORD					dwServerId;
	DWORD					dwTeamId;
	std::wstring			wsDungoenName;
	BOOL					IsReadOnly;
	std::wstring			wsGroupTitle;

	TeamMemberGroup()
	{
		uMaxMemberCount = 0;
		dwTaskId = dwChannelId = dwScheduleId = dwServerId = dwTeamId = 0;
		wsDungoenName = L"����";
		IsReadOnly = FALSE;

	}
};



class CBnsTaskTeam : public MyTools::CClassInstance<CBnsTaskTeam>
{
public:
	typedef std::deque<TeamMemberGroup> TeamGroup;
public:
	struct TeamMemberResult
	{
		std::wstring    wsPlayerName;
		DWORD			dwPlayerId;

		TeamMemberResult() : dwPlayerId(NULL) {}
	};
	struct BnsTeamResult
	{
		std::vector<TeamMemberResult>	VecTeamMember;
		DWORD							dwTeamId;
		TeamMemberResult				Captain;

		BnsTeamResult() : dwTeamId(NULL) {}

		VOID AddMember(_In_ CONST std::wstring& wsPlayerName, _In_ DWORD dwPlayerId)
		{
			TeamMemberResult Member;
			Member.dwPlayerId = dwPlayerId;
			Member.wsPlayerName = wsPlayerName;

			VecTeamMember.push_back(std::move(Member));
		}

		VOID SetCaptain(_In_ CONST std::wstring& wsPlayerName, _In_ DWORD dwPlayerId)
		{
			Captain.dwPlayerId = dwPlayerId;
			Captain.wsPlayerName = wsPlayerName;
		}

		VOID SetTeamId(_In_ DWORD dwTeamId_)
		{
			dwTeamId = dwTeamId_;
		}

		BOOL Ver()
		{
			return dwTeamId != NULL && Captain.dwPlayerId != NULL;
		}
	};

public:
	CBnsTaskTeam();
	~CBnsTaskTeam() = default;

	// �������
	BOOL JoinTaskTeam(_In_ CONST ClientTeamInfo& CtiMember, _Out_ BnsTeamResult& Result);

	// �������
	BOOL JoinDungoenTeam(_In_ CONST ClientTeamInfo& CtiMember, _Out_ BnsTeamResult& Result);

	// ɾ����Ա(һ���ǵ��ߵ���)
	BOOL DeleteTaskMember(_In_ CONST std::wstring& wsAccountName);

	// ɾ��������Ա(һ���ǵ��ߵ���)
	BOOL DeleteDungoenTeamMember(_In_ CONST std::wstring& wsPlayerName);

	// �޸ĸ�������
	BOOL UpdateDungoenSchedule(_In_ CONST TeamScheduleMsg& TeamScheduleMsg_);

	// ��ȡ��������
	BOOL GetDungoenSchedule(_In_ CONST TeamScheduleMsg& TeamScheduleMsg_, _Out_ std::vector<BnsDungoenScheduleMember>& Result);

	// ��ȡ������(�Լ���)��Ƶ��
	BOOL GetLessMemberChannel(_In_ CONST TeamChannelMsg& TeamChannelMsg_, _Out_ DWORD& dwChannel);

	// ���ö���ֻ��
	BOOL SetReadOnlyTeam(_In_ DWORD dwTeamId);

	// 
	BOOL FindTeamGroupByAccountName(_In_ CONST std::wstring& wsAccountName, _Out_ TeamMemberGroup& TeamContent) CONST;

	//
	VOID PrintOnLineTeam() CONST;
private:
	// �ж��Ƿ�Ϸ��Ľṹ
	BOOL IsWork(_In_ CONST ClientTeamInfo& CtiMember) CONST;

	// 
	BOOL JoinTeam(_In_ CONST ClientTeamInfo& CtiMember, _In_ std::map<std::wstring, TeamGroup>& map_TeamGroup, _Out_ BnsTeamResult& Result);

	// �Ƿ���� �ڶ����б���!
	BOOL ExistTeamMember(_In_ std::map<std::wstring, TeamGroup>& map_TeamGroup, _In_ CONST ClientTeamInfo& CtiMember, _In_ std::function<VOID(TeamMemberGroup&, TeamMemberInfo&)> Worker);
	BOOL ExistTeamMember(_In_ std::map<std::wstring, TeamGroup>& map_TeamGroup, _In_ DWORD dwTeamId, _In_ std::function<VOID(TeamMemberGroup&)> Worker);

	// ����map�Ĳ�ѯText
	VOID CreateMapText(_In_ CONST ClientTeamInfo& CtiMember, _Out_ std::wstring& wsText);

	// ������������
	VOID CreateTeamGroup(_In_ CONST std::wstring& wsMapText, _In_ CONST ClientTeamInfo& CtiMember, _In_ std::map<std::wstring, TeamGroup>& map_TeamGroup);

	// �Ƿ���� �� ���������
	BOOL ExistMember_By_AccountName_In_TeamMemberGroup(_In_ CONST std::wstring& wsAccountName, _In_ TeamGroup& TeamGroup_, _In_ std::function<VOID(TeamMemberGroup&, TeamMemberInfo&)> Worker) CONST;
	BOOL ExistMember_By_TeamId_In_TeamMemberGroup(_In_ DWORD dwTeamId, _In_ TeamGroup& TeamGroup_, _In_ std::function<VOID(TeamMemberGroup&)> Worker) CONST;

	// ���ضӳ��ĸ�����Ϣ
	BOOL FindCaptain_By_AccountName_In_TeamMemberGroup(_In_ TeamMemberGroup& TeamMemberGroup_, _In_ std::function<VOID(TeamMemberGroup&, TeamMemberInfo&)> Worker);

	// �Ƿ����㹻����Ķ�Աλ��,���ضӳ�
	BOOL FindVacantPosition_In_TeamMemberGroup(_In_ CONST ClientTeamInfo& CtiMember, _In_ std::map<std::wstring, TeamGroup>& map_TeamGroup, _In_ std::function<VOID(TeamMemberGroup&, TeamMemberInfo&)> Worker);

	// ��������
	BOOL CreateTeamMemberGroup(_In_ TeamGroup& TeamGroup_, _In_ CONST ClientTeamInfo& CtiMember, _In_ std::function<VOID(TeamMemberGroup&, TeamMemberInfo&)> Worker);

	// �뿪����
	BOOL DeleteTeamMember(_In_ CONST std::wstring& wsAccountName, _In_ std::map<std::wstring, TeamGroup>& map_TeamGroup);

	// ��ɢ����
	BOOL DismissTeamMemberGroup(_In_ TeamGroup& TeamGroup_, _In_ TeamMemberGroup& TeamMemberGroup_);

	// ��ȡ������ͬƵ��������
	BOOL GetSameChannelMap(_In_ CONST TeamChannelMsg& TeamChannelMsg_, _Out_opt_ std::map<DWORD, DWORD>& ChannelMap);

	// ��TeamMemberGroup����ĳ�Ա���в���
	BOOL ActionInTeamGroupMemberByAccountName(_In_ CONST std::wstring& wsAccountName, _In_ TeamMemberGroup& TeamMemberGroup_, _In_ std::function<VOID(TeamMemberInfo&)> Worker) CONST;

	// Find Team Group
	BOOL FindTeamGroupByAccountName(_In_ CONST std::wstring& wsAccountName, _In_ CONST std::map<std::wstring, TeamGroup>& MapTeam, _Out_ TeamMemberGroup& TeamContent) CONST;
private:
	// �������
	MyTools::CLLock Lock_BnsTeam;
	std::map<std::wstring, TeamGroup> TaskTeamMap;
	DWORD m_dwTeamId;

	// ��������
	MyTools::CLLock Lock_DungoenTeam;
	std::map<std::wstring, TeamGroup> DungoenTeamMap;
	DWORD m_dwDungoenTeamId;

};





#endif // __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_TEAM_BNSTEAM_H__
