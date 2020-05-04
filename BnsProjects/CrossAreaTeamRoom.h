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

	// �������ճ� // RefreshDailyTaskPtr() => �򵥵ĺ����̵��¼���  || ���ѵĺ����̵��¼���
	BOOL Run(_In_ std::function<std::vector<std::wstring>()> RefreshDailyTaskPtr);
private:
	// ѭ��ˢ
	BOOL LoopFarmDungeon(_In_ std::function<std::vector<std::wstring>()> RefreshDailyTaskPtr);

	// ѡ���ͼ׼���׶�
	BOOL MainFormReadyStep();

	// ���븱��ǰ�Ĺ��������׶�
	BOOL CommonDungeonStep();

	// ˢ�����׶�
	BOOL FarmDungeonStep();

	// ����׼���׶�
	BOOL BackToMainFormReadyStep();

	// ��������
	BOOL CreateTeam_In_Server();

	// ����������
	BOOL EnterToMainForm(_In_ DWORD dwTimeout) CONST;

	// �Ƿ���������
	BOOL IsInMainForm() CONST;

	// �Ƿ����ж�Ա��׼�����(���˶ӳ�)
	BOOL IsALLPlayerReady() CONST;

	// �Ƿ��Լ�׼�������(���˶ӳ�)
	BOOL PlayerAttribute_IsMemberReady() CONST;

	// �ǲ����ڹ�������
	BOOL PlayerAttribute_IsInCommonDungeon() CONST;

	// ׼��(���˶ӳ�)
	VOID PlayerAction_MemberReady() CONST;

	// �ӳ�׼��
	VOID PlayerAction_Captain_MoveToDungeon() CONST;

	// ѡ�񸱱�
	VOID PlayerAction_ChoiceMap() CONST;

	// ѡ���Ѷ�
	VOID PlayerAction_ChoiceDifficulty() CONST;

	// ��ȡ�����ڵĶ�������
	DWORD GetRoomMemberCount() CONST;

	// ��ȡ����ID
	DWORD GetCurrentRoomId() CONST;

	// ѡ��ĵ�ͼID
	DWORD GetCurrentMapId() CONST;

	// ѡ����Ѷ�
	em_CrossAreaTeam_Difficulty GetCurrentDifficulty() CONST;

	// 
	DWORD GetRoomBase() CONST;

	// ����ӳ��ķ���
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
