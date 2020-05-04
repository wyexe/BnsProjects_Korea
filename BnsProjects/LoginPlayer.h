#ifndef __GITBNSPROJECTS_BNSPROJECTS_OBJECTS_GAMEOBJECTS_LOGINPLAYER_H__
#define __GITBNSPROJECTS_BNSPROJECTS_OBJECTS_GAMEOBJECTS_LOGINPLAYER_H__

#include "GameVariable.h"

class CLoginPlayer
{
public:
	explicit CLoginPlayer(_In_ DWORD dwBase_, _In_ UINT uIndex_);
	explicit CLoginPlayer();
	~CLoginPlayer();

	
public:
	virtual DWORD GetBase() CONST;

	// ��ɫ��
	virtual CONST std::wstring	GetName() CONST;

	// �ȼ�
	virtual DWORD		GetLevel() CONST;

	// ��ɫ�Ƿ�ɾ��״̬
	virtual BOOL		IsDeleteStatus() CONST;

	// ��ȡ������ID
	virtual DWORD		GetServerId() CONST;

	// ��ȡְҵ
	virtual em_Player_Classes GetLoginPlayerClasses() CONST;

	// ��ȡ����
	virtual DWORD		GetIndex() CONST;

	BOOL GetFinishMainTask() CONST;

	VOID SetFinishMainTask(_In_ BOOL bFinishMainTask_);
private:
	DWORD dwBase;
	UINT  uIndex;
	BOOL  bFinishMainTask;
};



#endif