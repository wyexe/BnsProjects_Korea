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

	// 角色名
	virtual CONST std::wstring	GetName() CONST;

	// 等级
	virtual DWORD		GetLevel() CONST;

	// 角色是否删除状态
	virtual BOOL		IsDeleteStatus() CONST;

	// 获取服务器ID
	virtual DWORD		GetServerId() CONST;

	// 获取职业
	virtual em_Player_Classes GetLoginPlayerClasses() CONST;

	// 获取索引
	virtual DWORD		GetIndex() CONST;

	BOOL GetFinishMainTask() CONST;

	VOID SetFinishMainTask(_In_ BOOL bFinishMainTask_);
private:
	DWORD dwBase;
	UINT  uIndex;
	BOOL  bFinishMainTask;
};



#endif