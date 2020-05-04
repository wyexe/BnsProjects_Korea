#ifndef __GITBNSPROJECTS_BNSPROJECTS_TEAM_CLIENTTEAM_H__
#define __GITBNSPROJECTS_BNSPROJECTS_TEAM_CLIENTTEAM_H__

#include "GameVariable.h"
#if BnsProject_DLL
#include "../ConsoleDLL/SocketAPI.h"
#endif

class CGameUi;
class CClientTeam : public MyTools::CClassInstance<CClientTeam>
{
public:
	CClientTeam() ; 
	~CClientTeam() = default;
public:
	// 开始组队
	BOOL BnsScript_JoinTeam(_In_ DWORD dwMaxMember, _In_ BOOL bWait);

	// 离队
	BOOL BnsScript_LeaveTeam();

	// 副本组队
	BOOL BnsScript_JoinGungoenTeam(_In_ CONST TaskInfoParm& TaskInfoParm_, _In_ DWORD dwMaxMember, _In_ BOOL bWait);

	// 副本离队
	BOOL BnsScript_LeaveGungoenTeam();

	// 获取副本队伍进度
	VOID RefreshGunoneTeamSchedule();

	// 停止组队线程
	VOID StopJoinTeamThread();

	// 获取副本队伍进度
	TeamDungeonSchedule& GetTeamDungoenSchedule();

	// 队伍ID
	DWORD GetTeamId() CONST;

	// 修改副本进度
	VOID SetDungeonSchedule(_In_ CONST std::wstring& wsScheduleText, _In_ UINT uScheduleIndex) CONST;

	// 获取人数最少的频道
	DWORD GetChannelForServer(_In_ CONST TaskInfoParm& TaskParam) CONST;
private:

#if BnsProject_DLL
	// 组队
	CSocketAPI::SocketTeamResult JoinTeam(_In_ CSocketAPI::em_JoinTeam_Type emType, _In_ BOOL bFirst);

	// 离队
	VOID LeaveTeam(_In_ CSocketAPI::em_JoinTeam_Type emType);
#endif

	// 同上
	BOOL SetClientTeamInfo(_In_ DWORD dwMaxMemberCount);

	BOOL SetClientGungoenTeamInfo(_In_ CONST TaskInfoParm& TaskInfoParm_, _In_ DWORD dwMaxMemberCount);
#if BnsProject_DLL
	// 队长邀请List的队员(里面自带过滤, 在队伍中了就不邀请了)
	BOOL InvitePlayer_By_List(_In_ CONST CSocketAPI::SocketTeamResult& TeamContent) CONST;

	// 组队（队长）
	BOOL JoinTeam_Captain(_In_ CSocketAPI::em_JoinTeam_Type emType, _In_ BOOL bWait);

	// 组队（队员）
	BOOL JoinTeam_Member(_In_ CSocketAPI::em_JoinTeam_Type emType, CONST ClientTeamMember& CtiCaptain, _In_ BOOL bWait);
#endif
	// 组队线程
	static unsigned WINAPI _JoinTeamThread(LPVOID lpParm);
#if BnsProject_DLL
	// 队长的操作(邀请队员)
	BOOL DoCaptain(_In_ CONST CSocketAPI::SocketTeamResult& TeamContent) CONST;
	// 队员的操作(接受队长的邀请)
	BOOL DoMember(_In_ CONST ClientTeamMember& CtiCaptain) CONST;
#endif
private:
#if BnsProject_DLL
	// 当前自身的信息（如果是队长, 还包含了队员的信息）
	ClientTeamInfo _ClientTeamInfo;
#endif
	//             组队线程句柄
	HANDLE         _hJoinTeamThread;

	//             是否运行线程
	BOOL           _bRunThread;

#if BnsProject_DLL
	CSocketAPI	   _SocketAPI;
#endif

	// 副本队伍进度表
	TeamMemberSchedule _TeamSchedulel;

	TeamDungeonSchedule _TeamDungeonSchedule;

	DWORD			_dwTeamId = NULL;

	// 禁止拷贝对象
	DSIABLE_COPY_AND_ASSIGN(CClientTeam)
};



#endif 