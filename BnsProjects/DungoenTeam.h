#ifndef __GITBNSPROJECTS_BNSPROJECTS_GAME_SOMEFUN_DUNGOENTEAM_DUNGOENTEAM_H__
#define __GITBNSPROJECTS_BNSPROJECTS_GAME_SOMEFUN_DUNGOENTEAM_DUNGOENTEAM_H__

#include "GameVariable.h"

class CDungoenTeam : public MyTools::CClassInstance<CDungoenTeam>
{
public:
	CDungoenTeam() = default;
	~CDungoenTeam() = default;

private:
	// 进入跨服组队
	BOOL GoToRoom() CONST;

	// 是否进入完毕
	BOOL IsExistRoom() CONST _NOEXCEPT;

	// 创建房间密码
	VOID CreatePass() CONST;

	//查找并且进入房间,false进入失败
	BOOL FindRoomById(_In_ DWORD dwId) CONST;

	//获取房间基址
	DWORD GetRoomBase() CONST;

	//获取当前队伍人数
	DWORD GetRoomMemberCount() CONST;

	//获取当前房间ID
	DWORD GetCurrentRoomId() CONST;

	// 队员准备call
	BOOL ReadyFormRoom(_In_ BOOL bCaptain = FALSE) CONST;

	// 队员是否准备完毕
	BOOL IsReadyForRoom() CONST;

	// 队长使用(是否所有队员都准备完毕)
	BOOL IsReadyForTeamMember() CONST;

	// 
	BOOL ChoiceMap(_In_ DWORD dwMapId) CONST;

	//
	BOOL ChoiceDifficulty(_In_ DWORD dwValue) CONST;
};



#endif // !__GITBNSPROJECTS_BNSPROJECTS_GAME_SOMEFUN_DUNGOENTEAM_DUNGOENTEAM_H__
