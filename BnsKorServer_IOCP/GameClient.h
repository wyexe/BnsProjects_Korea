#ifndef __GITBNSPROJECTS_BNSKORSERVER_SOCKET_GAMECLIENT_H__
#define __GITBNSPROJECTS_BNSKORSERVER_SOCKET_GAMECLIENT_H__

#include <atomic>
#include <MyTools/SocketClient.h>
#include "ServerCommon.h"

class CGameClient : public MyTools::CLSocketClient
{
public:
	CGameClient(_In_ SOCKET Sock);
	virtual ~CGameClient() override;

	// ���˿ͻ��˶Ͽ����ӵ�ʱ��ᱻ����
	virtual VOID Remove() override;

	// ���ͻ������ӵ�ʱ������
	virtual VOID Add() override;

	// �Ƿ���Ч���ʺ�
	virtual BOOL IsOnLine() CONST override;

	virtual BOOL IsKeepALiveTimeout() CONST override;

	// �ʺŵ�һЩ����
	ClientAccountContent& GetAccountContent();

	//
	std::shared_ptr<ClientTeamInfo> GetClientTeamInfo();

	//
	std::shared_ptr<CrossAreaTeamMember> GetCrossAreaTeamMember();
public:
	static std::atomic<DWORD> _dwGameClientCount;
private:
	ClientAccountContent            _Account;
	DWORD                           _dwToken;
	std::shared_ptr<ClientTeamInfo> _TeamContent;
	std::shared_ptr<CrossAreaTeamMember> _CorssAreaTeamMemberContent;
	ULONGLONG						_OnLineTick;
};


#endif // !__GITBNSPROJECTS_BNSKORSERVER_SOCKET_GAMECLIENT_H__

