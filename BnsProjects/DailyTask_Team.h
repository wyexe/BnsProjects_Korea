#ifndef __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_COMMON_DAILYTASK_TEAM_H__
#define __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_COMMON_DAILYTASK_TEAM_H__

#include "GameVariable.h"

class CDailyTask_Team
{
private:
	struct TeamMemberClumpContent
	{
		WCHAR wszAccountName[64];
		WCHAR wszPlayerName[64];
		DWORD dwPlayerId;
		WCHAR wszScheduleName[64];
		UINT  uScheduleIndex;
		UINT  uIndex;

		TeamMemberClumpContent()
		{
			dwPlayerId = NULL;
			uScheduleIndex = NULL;
			uIndex = NULL;

			ZeroMemory(wszAccountName, sizeof(wszAccountName));
			ZeroMemory(wszPlayerName, sizeof(wszPlayerName));
			ZeroMemory(wszScheduleName, sizeof(wszScheduleName));
		}
	};

	struct TeamClumpContent
	{
		TeamMemberClumpContent ArrMember[10];
		UINT uCount;
	};
public:
	CDailyTask_Team();
	~CDailyTask_Team() = default;

	// 创建日常队伍
	VOID CreateTeam();

	// 集合
	VOID ClumpTeamMember(_In_ UINT uScheduleIndex, _In_ CONST std::wstring& wsScheduleName, _In_ CONST Point& ClumpPoint);

	// 复活队友
	VOID ReviveTeamMember();

	// 设置进度
	VOID SetSchedule(_In_ UINT uScheduleIndex, _In_ CONST std::wstring& wsScheduleName);
private:
	//
	VOID InitLocalTeam();

	//
	VOID CreateTeam_Captain();

	//
	VOID CreateTeam_Member();

	//
	TeamMemberClumpContent* FindMaxScheduleMember();
private:
	TeamClumpContent* _pTeamClumpContent;
	TeamMemberClumpContent* _pCurrentMemberClumpContent;
};



#endif // !__GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_COMMON_DAILYTASK_TEAM_H__
