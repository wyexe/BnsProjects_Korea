#ifndef __GITBNSPROJECTS_BNSPROJECTS_GAME_TEAM_CROSSAREATEAM_CROSSAREATEAMSERVICE_H__
#define __GITBNSPROJECTS_BNSPROJECTS_GAME_TEAM_CROSSAREATEAM_CROSSAREATEAMSERVICE_H__

#include "GameVariable.h"
#include "../ConsoleDLL/SocketAPI.h"

class CCrossAreaTeamService
{
public:
	struct CrossAreaTeamParam
	{
		std::vector<std::wstring> VecDungeonName;
		DWORD dwRoomId;
		ClientTeamMember Member;
	};
public:
	CCrossAreaTeamService() = default;
	~CCrossAreaTeamService() = default;

	BOOL Run(_In_ CONST CrossAreaTeamParam& Param, _Out_ CrossAreaTeamGroup& Result);

	VOID LeaveTeam(_In_ DWORD dwTeamId) CONST;
private:
	VOID FillCrossAreaTeamMember(_In_ CONST std::vector<std::wstring>& VecDungeonName, _Out_ CrossAreaTeamMember& Member) CONST;

	
private:
	CSocketAPI _SocketAPI;
};



#endif //!__GITBNSPROJECTS_BNSPROJECTS_GAME_TEAM_CROSSAREATEAM_CROSSAREATEAMSERVICE_H__
