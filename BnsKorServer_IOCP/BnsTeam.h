#ifndef __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_TEAM_BNSTEAM_H__
#define __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_TEAM_BNSTEAM_H__

#include <map>
#include <deque>
#include <MyTools/CLLock.h>
#include <MyTools/ClassInstance.h>
#include "ServerCommon.h"

struct DungoenTeamMember
{
	std::wstring			wsDungoenSchedule;	// 副本进度
	UINT					uGatherIndex;		// 进度


	DungoenTeamMember()
	{
		wsDungoenSchedule = L"";
		uGatherIndex = 0;
	}
};

// 队伍成员信息(包含了是否队长,帐号,角色名,ID)
struct TeamMemberInfo
{
	BOOL bCaptain;
	ClientTeamMember Member;
	DungoenTeamMember Dungoen;
};

// 一个队伍的所有人的信息
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
		wsDungoenName = L"主线";
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

	// 主线组队
	BOOL JoinTaskTeam(_In_ CONST ClientTeamInfo& CtiMember, _Out_ BnsTeamResult& Result);

	// 副本组队
	BOOL JoinDungoenTeam(_In_ CONST ClientTeamInfo& CtiMember, _Out_ BnsTeamResult& Result);

	// 删除队员(一般是掉线调用)
	BOOL DeleteTaskMember(_In_ CONST std::wstring& wsAccountName);

	// 删除副本队员(一般是掉线调用)
	BOOL DeleteDungoenTeamMember(_In_ CONST std::wstring& wsPlayerName);

	// 修改副本进度
	BOOL UpdateDungoenSchedule(_In_ CONST TeamScheduleMsg& TeamScheduleMsg_);

	// 获取副本进度
	BOOL GetDungoenSchedule(_In_ CONST TeamScheduleMsg& TeamScheduleMsg_, _Out_ std::vector<BnsDungoenScheduleMember>& Result);

	// 获取最少人(自己人)的频道
	BOOL GetLessMemberChannel(_In_ CONST TeamChannelMsg& TeamChannelMsg_, _Out_ DWORD& dwChannel);

	// 设置队伍只读
	BOOL SetReadOnlyTeam(_In_ DWORD dwTeamId);

	// 
	BOOL FindTeamGroupByAccountName(_In_ CONST std::wstring& wsAccountName, _Out_ TeamMemberGroup& TeamContent) CONST;

	//
	VOID PrintOnLineTeam() CONST;
private:
	// 判断是否合法的结构
	BOOL IsWork(_In_ CONST ClientTeamInfo& CtiMember) CONST;

	// 
	BOOL JoinTeam(_In_ CONST ClientTeamInfo& CtiMember, _In_ std::map<std::wstring, TeamGroup>& map_TeamGroup, _Out_ BnsTeamResult& Result);

	// 是否存在 在队伍列表中!
	BOOL ExistTeamMember(_In_ std::map<std::wstring, TeamGroup>& map_TeamGroup, _In_ CONST ClientTeamInfo& CtiMember, _In_ std::function<VOID(TeamMemberGroup&, TeamMemberInfo&)> Worker);
	BOOL ExistTeamMember(_In_ std::map<std::wstring, TeamGroup>& map_TeamGroup, _In_ DWORD dwTeamId, _In_ std::function<VOID(TeamMemberGroup&)> Worker);

	// 创建map的查询Text
	VOID CreateMapText(_In_ CONST ClientTeamInfo& CtiMember, _Out_ std::wstring& wsText);

	// 创建队伍区域
	VOID CreateTeamGroup(_In_ CONST std::wstring& wsMapText, _In_ CONST ClientTeamInfo& CtiMember, _In_ std::map<std::wstring, TeamGroup>& map_TeamGroup);

	// 是否存在 在 区域队伍里
	BOOL ExistMember_By_AccountName_In_TeamMemberGroup(_In_ CONST std::wstring& wsAccountName, _In_ TeamGroup& TeamGroup_, _In_ std::function<VOID(TeamMemberGroup&, TeamMemberInfo&)> Worker) CONST;
	BOOL ExistMember_By_TeamId_In_TeamMemberGroup(_In_ DWORD dwTeamId, _In_ TeamGroup& TeamGroup_, _In_ std::function<VOID(TeamMemberGroup&)> Worker) CONST;

	// 返回队长的个人信息
	BOOL FindCaptain_By_AccountName_In_TeamMemberGroup(_In_ TeamMemberGroup& TeamMemberGroup_, _In_ std::function<VOID(TeamMemberGroup&, TeamMemberInfo&)> Worker);

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

	// 对TeamMemberGroup里面的成员进行操作
	BOOL ActionInTeamGroupMemberByAccountName(_In_ CONST std::wstring& wsAccountName, _In_ TeamMemberGroup& TeamMemberGroup_, _In_ std::function<VOID(TeamMemberInfo&)> Worker) CONST;

	// Find Team Group
	BOOL FindTeamGroupByAccountName(_In_ CONST std::wstring& wsAccountName, _In_ CONST std::map<std::wstring, TeamGroup>& MapTeam, _Out_ TeamMemberGroup& TeamContent) CONST;
private:
	// 任务队伍
	MyTools::CLLock Lock_BnsTeam;
	std::map<std::wstring, TeamGroup> TaskTeamMap;
	DWORD m_dwTeamId;

	// 副本队伍
	MyTools::CLLock Lock_DungoenTeam;
	std::map<std::wstring, TeamGroup> DungoenTeamMap;
	DWORD m_dwDungoenTeamId;

};





#endif // __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_TEAM_BNSTEAM_H__
