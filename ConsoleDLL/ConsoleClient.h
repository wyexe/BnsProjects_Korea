#ifndef __MYTOOLS_SOCKET_CONSOLECLIENT_H__
#define __MYTOOLS_SOCKET_CONSOLECLIENT_H__

#include <MyTools/CLClient.h>
#include <MyTools/ClassInstance.h>
#include <MyTools/SocketBuffer.h>
#include "../BnsKorServer_IOCP/ServerCommon.h"

class CConsoleClient : public MyTools::CLClient, public MyTools::CClassInstance<CConsoleClient>
{
public:
	CConsoleClient();
	~CConsoleClient() = default;

	// ͬ������
	BOOL SyncSend(_In_ em_Sock_Msg emMsg, _In_ std::function<VOID(MyTools::CLSocketBuffer&)> FillBufferPtr);


	// ͬ������(�ȴ�������Ϣ)
	BOOL SyncSend(_In_ MyTools::CLSocketBuffer& SocketBuffer, std::function<VOID(MyTools::CLSocketBuffer& SocketBuffer)> ExtractPtr);

	//
	VOID SetEchoErrorPtr(_In_ std::function<VOID(CONST std::wstring&)> EchoErrorPtr);

	//
	VOID SetPrintToConsolePtr(_In_ std::function<VOID(CONST std::wstring&)> PrintToConsolePtr);
protected:

	// �����հ�
	virtual VOID EchoPacket(_In_ MyTools::CLSocketBuffer& SocketBuffer) override;


	// ��������Ͽ����ӻص�
	virtual VOID DisConnectServer() override;

private:
	VOID EchoError(_In_ CONST std::wstring& wsLog);

	VOID PrintToConsole(_In_ CONST std::wstring& wsLog);
private:
	DWORD _dwMsgHead;
	HANDLE _hEvent;


	std::function<VOID(MyTools::CLSocketBuffer& SocketBuffer)> _ExtractActionPtr;

	std::function<VOID(CONST std::wstring&)> _EchoErrorPtr;
	std::function<VOID(CONST std::wstring&)> _PrintToConsolePtr;

	std::mutex _Mtx;
};





#endif