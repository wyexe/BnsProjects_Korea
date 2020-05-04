#ifndef __GITBNSPROJECTS_VERBNSKORSERVER_TEAM_BNSTEAM_H__
#define __GITBNSPROJECTS_VERBNSKORSERVER_TEAM_BNSTEAM_H__
#include "VerManage.h"
#include <MyTools/CLLock.h>
#include <map>

struct DungoenTeamMember
{
	std::wstring			wsDungoenSchedule;	// ��������
	UINT					uGatherIndex;		// ����
	

	DungoenTeamMember()
	{
		wsDungoenSchedule	= L"";
		uGatherIndex		= 0;
	}
};

// �����Ա��Ϣ(�������Ƿ�ӳ�,�ʺ�,��ɫ��,ID)
typedef struct _TeamMemberInfo
{
	BOOL bCaptain;
	ClientTeamMember Member;
	DungoenTeamMember Dungoen;
}TeamMemberInfo;

// һ������������˵���Ϣ
typedef struct _TeamMemberGroup
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
	_TeamMemberGroup()
	{
		uMaxMemberCount = 0;
		dwTaskId = dwChannelId = dwScheduleId = dwServerId = dwTeamId = 0;
		wsDungoenName = L"����";
		IsReadOnly = FALSE;

	}
}TeamMemberGroup;

typedef std::deque<TeamMemberGroup> TeamGroup;

class ByteBuffer;
class CBnsTaskTeam : public CClassInstance<CBnsTaskTeam>
{
public:
	CBnsTaskTeam();
	~CBnsTaskTeam() = default;

	BOOL JoinTaskTeam(_In_ CONST ClientTeamInfo& CtiMember, _Out_ ByteBuffer& ServerBuffer);

	BOOL JoinDungoenTeam(_In_ CONST ClientTeamInfo& CtiMember, _Out_ ByteBuffer& ServerBuffer);

	BOOL DeleteTaskMember(_In_ CONST std::wstring& wsAccountName);

	BOOL DeleteDungoenTeamMember(_In_ CONST std::wstring& wsPlayerName);

	BOOL SearchGroupByAccountName(_In_ CONST std::wstring& wsAccountName, _In_ std::function<VOID(CONST TeamMemberGroup&)> Worker);

	BOOL SearchGroupByTask(_In_ CONST TaskInfoParm& Tip, _In_ std::function<VOID(CONST TeamMemberGroup&)> Worker) CONST;

	BOOL SearchGroupByDungoneName(_In_ CONST std::wstring& wsDungoneName, _In_ std::function<VOID(CONST TeamMemberGroup&)> Worker) CONST;

	BOOL UpdateDungoenSchedule(_In_ CONST TeamScheduleMsg& TeamScheduleMsg_, _Out_ ByteBuffer& ByteBuffer_);

	BOOL GetDungoenSchedule(_In_ CONST TeamScheduleMsg& TeamScheduleMsg_, _Out_ ByteBuffer& ByteBuffer_);

	BOOL PrintTaskTeam() CONST;	
	
	// ��ȡ������(�Լ���)��Ƶ��
	BOOL GetLessMemberChannel(_In_ CONST TeamChannelMsg& TeamChannelMsg_, _Out_ ByteBuffer& ByteBuffer_);

	// ���ö���ֻ��
	BOOL SetReadOnlyTeam(_In_ DWORD dwTeamId, _Out_ ByteBuffer& ByteBuffer_);

private:
	// �ж��Ƿ�Ϸ��Ľṹ
	BOOL IsWork(_In_ CONST ClientTeamInfo& CtiMember) CONST;

	// 
	BOOL JoinTeam(_In_ DWORD dwMsg, _In_ CONST ClientTeamInfo& CtiMember, _In_ std::map<std::wstring, TeamGroup>& map_TeamGroup, _Out_ ByteBuffer& ServerBuffer);

	// �Ƿ���� �ڶ����б���!
	BOOL ExistTeamMember(_In_ std::map<std::wstring, TeamGroup>& map_TeamGroup, _In_ CONST ClientTeamInfo& CtiMember, _In_ std::function<VOID(TeamMemberGroup&,TeamMemberInfo&)> Worker);
	BOOL ExistTeamMember(_In_ std::map<std::wstring, TeamGroup>& map_TeamGroup, _In_ DWORD dwTeamId, _In_ std::function<VOID(TeamMemberGroup&)> Worker);

	// ����map�Ĳ�ѯText
	VOID CreateMapText(_In_ CONST ClientTeamInfo& CtiMember, _Out_ std::wstring& wsText);

	// ������������
	VOID CreateTeamGroup(_In_ CONST std::wstring& wsMapText, _In_ CONST ClientTeamInfo& CtiMember, _In_ std::map<std::wstring, TeamGroup>& map_TeamGroup);

	// �Ƿ���� �� ���������
	BOOL ExistMember_By_AccountName_In_TeamMemberGroup(_In_ CONST std::wstring& wsAccountName, _In_ TeamGroup& TeamGroup_, _In_ std::function<VOID(TeamMemberGroup&, TeamMemberInfo&)> Worker) CONST;
	BOOL ExistMember_By_TeamId_In_TeamMemberGroup(_In_ DWORD dwTeamId, _In_ TeamGroup& TeamGroup_, _In_ std::function<VOID(TeamMemberGroup&)> Worker) CONST;

	// ���ضӳ��ĸ�����Ϣ
	BOOL FindCaptain_By_AccountName_In_TeamMemberGroup(_In_ TeamMemberGroup& TeamMemberGroup_, _In_ std::function<VOID(TeamMemberGroup&,TeamMemberInfo&)> Worker);

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

	// 
	BOOL SearchGroupByAccountName(_In_ std::map<std::wstring, TeamGroup>& TeamGroupMap, _In_ CONST std::wstring& wsAccountName, _In_ std::function<VOID(CONST TeamMemberGroup&)> Worker) CONST;

	//
	BOOL SearchGroupByTask(_In_ CONST std::map<std::wstring, TeamGroup>& TeamGroupMap, _In_ CONST TaskInfoParm& Tip, _In_ std::function<VOID(CONST TeamMemberGroup&)> Worker) CONST;

	// ��TeamMemberGroup����ĳ�Ա���в���
	BOOL ActionInTeamGroupMemberByAccountName(_In_ CONST std::wstring& wsAccountName, _In_ TeamMemberGroup& TeamMemberGroup_, _In_ std::function<VOID(TeamMemberInfo&)> Worker) CONST;

private:
	// �������
	CLLock Lock_BnsTeam;
	std::map<std::wstring, TeamGroup> TaskTeamMap;
	DWORD m_dwTeamId;

	// ��������
	CLLock Lock_DungoenTeam;
	std::map<std::wstring, TeamGroup> DungoenTeamMap;
	DWORD m_dwDungoenTeamId;
	
private:
	DSIABLE_COPY_AND_ASSIGN(CBnsTaskTeam);
};





#endif // __GITBNSPROJECTS_VERBNSKORSERVER_TEAM_BNSTEAM_H__
