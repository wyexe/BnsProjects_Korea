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
	// ��ʼ���
	BOOL BnsScript_JoinTeam(_In_ DWORD dwMaxMember, _In_ BOOL bWait);

	// ���
	BOOL BnsScript_LeaveTeam();

	// �������
	BOOL BnsScript_JoinGungoenTeam(_In_ CONST TaskInfoParm& TaskInfoParm_, _In_ DWORD dwMaxMember, _In_ BOOL bWait);

	// �������
	BOOL BnsScript_LeaveGungoenTeam();

	// ��ȡ�����������
	VOID RefreshGunoneTeamSchedule();

	// ֹͣ����߳�
	VOID StopJoinTeamThread();

	// ��ȡ�����������
	TeamDungeonSchedule& GetTeamDungoenSchedule();

	// ����ID
	DWORD GetTeamId() CONST;

	// �޸ĸ�������
	VOID SetDungeonSchedule(_In_ CONST std::wstring& wsScheduleText, _In_ UINT uScheduleIndex) CONST;

	// ��ȡ�������ٵ�Ƶ��
	DWORD GetChannelForServer(_In_ CONST TaskInfoParm& TaskParam) CONST;
private:

#if BnsProject_DLL
	// ���
	CSocketAPI::SocketTeamResult JoinTeam(_In_ CSocketAPI::em_JoinTeam_Type emType, _In_ BOOL bFirst);

	// ���
	VOID LeaveTeam(_In_ CSocketAPI::em_JoinTeam_Type emType);
#endif

	// ͬ��
	BOOL SetClientTeamInfo(_In_ DWORD dwMaxMemberCount);

	BOOL SetClientGungoenTeamInfo(_In_ CONST TaskInfoParm& TaskInfoParm_, _In_ DWORD dwMaxMemberCount);
#if BnsProject_DLL
	// �ӳ�����List�Ķ�Ա(�����Դ�����, �ڶ������˾Ͳ�������)
	BOOL InvitePlayer_By_List(_In_ CONST CSocketAPI::SocketTeamResult& TeamContent) CONST;

	// ��ӣ��ӳ���
	BOOL JoinTeam_Captain(_In_ CSocketAPI::em_JoinTeam_Type emType, _In_ BOOL bWait);

	// ��ӣ���Ա��
	BOOL JoinTeam_Member(_In_ CSocketAPI::em_JoinTeam_Type emType, CONST ClientTeamMember& CtiCaptain, _In_ BOOL bWait);
#endif
	// ����߳�
	static unsigned WINAPI _JoinTeamThread(LPVOID lpParm);
#if BnsProject_DLL
	// �ӳ��Ĳ���(�����Ա)
	BOOL DoCaptain(_In_ CONST CSocketAPI::SocketTeamResult& TeamContent) CONST;
	// ��Ա�Ĳ���(���ܶӳ�������)
	BOOL DoMember(_In_ CONST ClientTeamMember& CtiCaptain) CONST;
#endif
private:
#if BnsProject_DLL
	// ��ǰ�������Ϣ������Ƕӳ�, �������˶�Ա����Ϣ��
	ClientTeamInfo _ClientTeamInfo;
#endif
	//             ����߳̾��
	HANDLE         _hJoinTeamThread;

	//             �Ƿ������߳�
	BOOL           _bRunThread;

#if BnsProject_DLL
	CSocketAPI	   _SocketAPI;
#endif

	// ����������ȱ�
	TeamMemberSchedule _TeamSchedulel;

	TeamDungeonSchedule _TeamDungeonSchedule;

	DWORD			_dwTeamId = NULL;

	// ��ֹ��������
	DSIABLE_COPY_AND_ASSIGN(CClientTeam)
};



#endif 