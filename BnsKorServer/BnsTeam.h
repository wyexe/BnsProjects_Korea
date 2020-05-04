#ifndef __GITBNSPROJECTS_VERBNSKORSERVER_TEAM_BNSTEAM_H__
#define __GITBNSPROJECTS_VERBNSKORSERVER_TEAM_BNSTEAM_H__
#include "VerManage.h"
#include <MyTools/CLLock.h>
#include <map>

struct DungoenTeamMember
{
	std::wstring			wsDungoenSchedule;	// 副本进度
	UINT					uGatherIndex;		// 进度
	

	DungoenTeamMember()
	{
		wsDungoenSchedule	= L"";
		uGatherIndex		= 0;
	}
};

// 队伍成员信息(包含了是否队长,帐号,角色名,ID)
typedef struct _TeamMemberInfo
{
	BOOL bCaptain;
	ClientTeamMember Member;
	DungoenTeamMember Dungoen;
}TeamMemberInfo;

// 一个队伍的所有人的信息
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
		wsDungoenName = L"主线";
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
	
	// 获取最少人(自己人)的频道
	BOOL GetLessMemberChannel(_In_ CONST TeamChannelMsg& TeamChannelMsg_, _Out_ ByteBuffer& ByteBuffer_);

	// 设置队伍只读
	BOOL SetReadOnlyTeam(_In_ DWORD dwTeamId, _Out_ ByteBuffer& ByteBuffer_);

private:
	// 判断是否合法的结构
	BOOL IsWork(_In_ CONST ClientTeamInfo& CtiMember) CONST;

	// 
	BOOL JoinTeam(_In_ DWORD dwMsg, _In_ CONST ClientTeamInfo& CtiMember, _In_ std::map<std::wstring, TeamGroup>& map_TeamGroup, _Out_ ByteBuffer& ServerBuffer);

	// 是否存在 在队伍列表中!
	BOOL ExistTeamMember(_In_ std::map<std::wstring, TeamGroup>& map_TeamGroup, _In_ CONST ClientTeamInfo& CtiMember, _In_ std::function<VOID(TeamMemberGroup&,TeamMemberInfo&)> Worker);
	BOOL ExistTeamMember(_In_ std::map<std::wstring, TeamGroup>& map_TeamGroup, _In_ DWORD dwTeamId, _In_ std::function<VOID(TeamMemberGroup&)> Worker);

	// 创建map的查询Text
	VOID CreateMapText(_In_ CONST ClientTeamInfo& CtiMember, _Out_ std::wstring& wsText);

	// 创建队伍区域
	VOID CreateTeamGroup(_In_ CONST std::wstring& wsMapText, _In_ CONST ClientTeamInfo& CtiMember, _In_ std::map<std::wstring, TeamGroup>& map_TeamGroup);

	// 是否存在 在 区域队伍里
	BOOL ExistMember_By_AccountName_In_TeamMemberGroup(_In_ CONST std::wstring& wsAccountName, _In_ TeamGroup& TeamGroup_, _In_ std::function<VOID(TeamMemberGroup&, TeamMemberInfo&)> Worker) CONST;
	BOOL ExistMember_By_TeamId_In_TeamMemberGroup(_In_ DWORD dwTeamId, _In_ TeamGroup& TeamGroup_, _In_ std::function<VOID(TeamMemberGroup&)> Worker) CONST;

	// 返回队长的个人信息
	BOOL FindCaptain_By_AccountName_In_TeamMemberGroup(_In_ TeamMemberGroup& TeamMemberGroup_, _In_ std::function<VOID(TeamMemberGroup&,TeamMemberInfo&)> Worker);

	// 是否有足够空余的队员位置,返回队长
	BOOL FindVacantPosition_In_TeamMemberGroup(_In_ CONST ClientTeamInfo& CtiMember, _In_ std::map<std::wstring, TeamGroup>& map_TeamGroup, _In_ std::function<VOID(TeamMemberGroup&, TeamMemberInfo&)> Worker);

	// 创建队伍
	BOOL CreateTeamMemberGroup(_In_ TeamGroup& TeamGroup_, _In_ CONST ClientTeamInfo& CtiMember, _In_ std::function<VOID(TeamMemberGroup&, TeamMemberInfo&)> Worker);

	// 离开队伍
	BOOL DeleteTeamMember(_In_ CONST std::wstring& wsAccountName, _In_ std::map<std::wstring, TeamGroup>& map_TeamGroup);

	// 解散队伍
	BOOL DismissTeamMemberGroup(_In_ TeamGroup& TeamGroup_, _In_ TeamMemberGroup& TeamMemberGroup_);

	// 获取所有相同频道的数量
	BOOL GetSameChannelMap(_In_ CONST TeamChannelMsg& TeamChannelMsg_, _Out_opt_ std::map<DWORD, DWORD>& ChannelMap);

	// 
	BOOL SearchGroupByAccountName(_In_ std::map<std::wstring, TeamGroup>& TeamGroupMap, _In_ CONST std::wstring& wsAccountName, _In_ std::function<VOID(CONST TeamMemberGroup&)> Worker) CONST;

	//
	BOOL SearchGroupByTask(_In_ CONST std::map<std::wstring, TeamGroup>& TeamGroupMap, _In_ CONST TaskInfoParm& Tip, _In_ std::function<VOID(CONST TeamMemberGroup&)> Worker) CONST;

	// 对TeamMemberGroup里面的成员进行操作
	BOOL ActionInTeamGroupMemberByAccountName(_In_ CONST std::wstring& wsAccountName, _In_ TeamMemberGroup& TeamMemberGroup_, _In_ std::function<VOID(TeamMemberInfo&)> Worker) CONST;

private:
	// 任务队伍
	CLLock Lock_BnsTeam;
	std::map<std::wstring, TeamGroup> TaskTeamMap;
	DWORD m_dwTeamId;

	// 副本队伍
	CLLock Lock_DungoenTeam;
	std::map<std::wstring, TeamGroup> DungoenTeamMap;
	DWORD m_dwDungoenTeamId;
	
private:
	DSIABLE_COPY_AND_ASSIGN(CBnsTaskTeam);
};





#endif // __GITBNSPROJECTS_VERBNSKORSERVER_TEAM_BNSTEAM_H__
