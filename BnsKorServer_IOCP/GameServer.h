#ifndef __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_COMMON_SOCKET_SERVER_GAMESERVER_H__
#define __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_COMMON_SOCKET_SERVER_GAMESERVER_H__

#include <set>
#include <map>
#include <MyTools/CLServer.h>
#include "ServerCommon.h"


class CGameClient;
class CGameServer : public MyTools::CLServer
{
public:
	CGameServer() = default;
	~CGameServer() = default;

	virtual MyTools::CLSocketClient* CreateNewSocketClient(_In_ SOCKET ClientSock) override;

	virtual VOID					 ReleaseSocketClient(_In_ MyTools::CLSocketClient* pSocketClient) override;

	virtual BOOL					EchoRecive(_In_ MyTools::CLSocketClient* pSocketClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) override;

	VOID PrintOnLinePlayerThread();
public:
	VOID InitializeBnsService();
private:
	std::set<em_Sock_Msg> _VecCheckCard;
	std::map<em_Sock_Msg, std::function<BOOL(CGameClient&, MyTools::CLSocketBuffer*)>> _VecEchoPacketPtr;
};

#endif // !__GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_COMMON_SOCKET_SERVER_GAMESERVER_H__
