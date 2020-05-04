#ifndef __GITBNSPROJECTS_BNSPROJECTS_GAME_TEAM_CROSSAREATEAM_ROOM_CROSSAREATEAMROOM_H__
#define __GITBNSPROJECTS_BNSPROJECTS_GAME_TEAM_CROSSAREATEAM_ROOM_CROSSAREATEAMROOM_H__

#include "GameVariable.h"
#include "../BnsKorServer_IOCP/ServerCommon.h"


class CCrossAreaTeamRoom
{
public:
	struct CrossAreaTeamRoomContent
	{
		DWORD dwRoomMapIndex;
		em_CrossAreaTeam_Difficulty emDifficulty;
	};

	struct CrossAreaTeamContent
	{
		BOOL bCaptain;
		ClientTeamMember Captain;
		std::vector<ClientTeamMember> VecMember;
		std::wstring wsCurrentDungeonName;
	};

	enum { em_MapId_CommonDungeon = 0x327 };
public:
	CCrossAreaTeamRoom() = default;
	~CCrossAreaTeamRoom() = default;

	// 跨服组队日常 // RefreshDailyTaskPtr() => 简单的黑龙教地下监狱  || 困难的黑龙教地下监狱
	BOOL Run(_In_ std::function<std::vector<std::wstring>()> RefreshDailyTaskPtr);
private:
	// 循环刷
	BOOL LoopFarmDungeon(_In_ std::function<std::vector<std::wstring>()> RefreshDailyTaskPtr);

	// 选择地图准备阶段
	BOOL MainFormReadyStep();

	// 进入副本前的公共副本阶段
	BOOL CommonDungeonStep();

	// 刷副本阶段
	BOOL FarmDungeonStep();

	// 返回准备阶段
	BOOL BackToMainFormReadyStep();

	// 创建队伍
	BOOL CreateTeam_In_Server();

	// 进入跨服房间
	BOOL EnterToMainForm(_In_ DWORD dwTimeout) CONST;

	// 是否在主界面
	BOOL IsInMainForm() CONST;

	// 是否所有队员都准备完毕(除了队长)
	BOOL IsALLPlayerReady() CONST;

	// 是否自己准备完毕了(除了队长)
	BOOL PlayerAttribute_IsMemberReady() CONST;

	// 是不是在公共副本
	BOOL PlayerAttribute_IsInCommonDungeon() CONST;

	// 准备(除了队长)
	VOID PlayerAction_MemberReady() CONST;

	// 队长准备
	VOID PlayerAction_Captain_MoveToDungeon() CONST;

	// 选择副本
	VOID PlayerAction_ChoiceMap() CONST;

	// 选择难度
	VOID PlayerAction_ChoiceDifficulty() CONST;

	// 获取房间内的队友数量
	DWORD GetRoomMemberCount() CONST;

	// 获取房间ID
	DWORD GetCurrentRoomId() CONST;

	// 选择的地图ID
	DWORD GetCurrentMapId() CONST;

	// 选择的难度
	em_CrossAreaTeam_Difficulty GetCurrentDifficulty() CONST;

	// 
	DWORD GetRoomBase() CONST;

	// 进入队长的房间
	VOID EnterToCaptainRoom() CONST;

	//
	static std::vector<ClientTeamMember> VecCrossAreaTeamToClientTeamMember(_In_ CONST std::vector<CrossAreaTeamMember>& Vec);

	//
	VOID LeaveRoom() CONST;

	//
	VOID ExitMainForm() CONST;
private:
	DWORD _dwRoomId;
	CrossAreaTeamRoomContent _RoomContent;
	CrossAreaTeamContent _TeamContent;
	std::vector<std::wstring> _VecDungeonName;
};



#endif // !__GITBNSPROJECTS_BNSPROJECTS_GAME_TEAM_CROSSAREATEAM_ROOM_CROSSAREATEAMROOM_H__
