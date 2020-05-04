#ifndef __GITBNSPROJECTS_BNSPROJECTS_GAME_SOMEFUN_DUNGOENTEAM_DUNGOENTEAM_H__
#define __GITBNSPROJECTS_BNSPROJECTS_GAME_SOMEFUN_DUNGOENTEAM_DUNGOENTEAM_H__

#include "GameVariable.h"

class CDungoenTeam : public MyTools::CClassInstance<CDungoenTeam>
{
public:
	CDungoenTeam() = default;
	~CDungoenTeam() = default;

private:
	// ���������
	BOOL GoToRoom() CONST;

	// �Ƿ�������
	BOOL IsExistRoom() CONST _NOEXCEPT;

	// ������������
	VOID CreatePass() CONST;

	//���Ҳ��ҽ��뷿��,false����ʧ��
	BOOL FindRoomById(_In_ DWORD dwId) CONST;

	//��ȡ�����ַ
	DWORD GetRoomBase() CONST;

	//��ȡ��ǰ��������
	DWORD GetRoomMemberCount() CONST;

	//��ȡ��ǰ����ID
	DWORD GetCurrentRoomId() CONST;

	// ��Ա׼��call
	BOOL ReadyFormRoom(_In_ BOOL bCaptain = FALSE) CONST;

	// ��Ա�Ƿ�׼�����
	BOOL IsReadyForRoom() CONST;

	// �ӳ�ʹ��(�Ƿ����ж�Ա��׼�����)
	BOOL IsReadyForTeamMember() CONST;

	// 
	BOOL ChoiceMap(_In_ DWORD dwMapId) CONST;

	//
	BOOL ChoiceDifficulty(_In_ DWORD dwValue) CONST;
};



#endif // !__GITBNSPROJECTS_BNSPROJECTS_GAME_SOMEFUN_DUNGOENTEAM_DUNGOENTEAM_H__
