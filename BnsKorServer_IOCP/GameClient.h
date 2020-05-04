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

	// 当此客户端断开连接的时候会被调用
	virtual VOID Remove() override;

	// 当客户端连接的时候会调用
	virtual VOID Add() override;

	// 是否有效的帐号
	virtual BOOL IsOnLine() CONST override;

	virtual BOOL IsKeepALiveTimeout() CONST override;

	// 帐号的一些数据
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

