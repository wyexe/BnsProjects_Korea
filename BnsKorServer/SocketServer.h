#ifndef __MYTOOLS_SOCKET_SOCKETSERVER_H__
#define __MYTOOLS_SOCKET_SOCKETSERVER_H__

#include "SocketCommon.h"
#include "../BnsProjects/GameStruct.h"

struct SocketClientBuffer : public std::enable_shared_from_this<SocketClientBuffer>
{
	DWORD dwMsg;
	ByteBuffer ByteBuffer_;
	std::shared_ptr<SocketClientInfo> SocketClientInfoPtr;
	SocketClientBuffer() : dwMsg(NULL) {}
};

#define MAX_CLIENT_COUNT 5000

class CSocketServer : public CClassInstance<CSocketServer>
{
	struct tagSocketServerVariable
	{
		// ����˵X��֮�ڱ��뷢�ͳ���Ykb
		DWORD	dwMaxSendSizeTime;
		UINT		uMaxSendSize;

		// max send time for everytime 
		DWORD	dwSingleMaxSendTime;

		// max recv time for every time
		DWORD	dwSingleMaxRecvTime;

		// max alloc memory size for recv buffer
		ULONGLONG	dwMaxAllocMemorySize;

		// ������ʱʱ��(/s)
		DWORD	dwMaxKeepAliveTimeOut;

		// ������ʱ
		ULONGLONG	ulMaxLockTimeOut;
	};

public:
	explicit CSocketServer();
	~CSocketServer();

	CSocketServer(CONST CSocketServer&) = delete;
	VOID operator=(CONST CSocketServer&) = delete;

	// ���õ���
	VOID SetBreakConnect(_In_ std::function<VOID(std::shared_ptr<SocketClientInfo>)> Worker);

	BOOL Run(_In_ DWORD dwPort, _In_ UINT uThreadCount);
	BOOL Stop();

	// Send�߳�
	static unsigned WINAPI _SendThread(LPVOID lpParm);
	
	// ��ȡClient�Ƿ���������
	static unsigned WINAPI _TryRecvThread(LPVOID lpParm);

	// Recv�����߳�
	static unsigned WINAPI _RecvThread(LPVOID lpParm);

	// Accept Client�߳�
	static unsigned WINAPI _AcceptThread(LPVOID lpParm);

	// �Ͽ������߳�
	static unsigned WINAPI _BreakConnectThread(LPVOID lpParm);

	VOID SetSocketVariable(_In_ std::function<VOID(tagSocketServerVariable&)> Worker);

	// ����
	VOID Send(_In_ std::shared_ptr<SocketClientBuffer>& SocketClientBufferPtr, _In_ CONST ByteBuffer& ByteBuffer_);

	// ��ȡ�����������
	BOOL GetWaitToProcessData(_In_ _Out_ BOOL& bLastExist, _Out_ std::shared_ptr<SocketClientBuffer>& SocketClientBufferPtr);
private:
	VOID PrintThreadCalcText(_In_ _Out_ ULONGLONG& ulTick, _In_ _Out_ UINT& uCount, _In_ LPCWSTR pwszFormat,...);

	BOOL DoAction_By_EmptyIndex(_In_ std::function<VOID(std::shared_ptr<SocketClientInfo>)> Worker);

	BOOL Lock_DoAction_By_ClientBuffer(_In_ std::function<VOID(std::shared_ptr<SocketClientInfo>)> Worker);

	int CustomeRecv(_In_ CONST std::shared_ptr<SocketClientInfo>& SocketClientInfoPtr, _In_ DWORD dwMsg, _In_ CHAR* buf, _In_ INT uLen);

	BOOL DoAction_By_SocketClientBufferDeque(_In_ std::function<VOID(std::deque<std::shared_ptr<SocketClientInfo>>&)> Worker);

	// �ж�SOCKET�Ƿ�������
	int GetClientSelect(_In_ SOCKET sk) CONST;

	////////Send//////////////////////////////////////////////////////////////////

	// �Զ��巢��
	int CustomeSend(_In_ CONST std::shared_ptr<SocketClientInfo>& SocketClientInfoPtr, _In_ DWORD dwMsg, _In_ CHAR* buf, _In_ INT uLen);

	// �ط��ͻ��� ������ �����ط�����
	VOID Send_To_Client_ReSend(_In_ std::shared_ptr<SocketClientBuffer>& SocketClientBufferPtr);

	// ��ӵ� ���Ͷ���
	VOID AddToSendQueue(_In_ std::shared_ptr<SocketClientBuffer>& SocketClientBufferPtr);

	// ��ȡ ׼�����͵��ͻ��˵�����
	BOOL GetSendListClientBuffer(_In_ _Out_ BOOL& bLastExist, _Out_ std::shared_ptr<SocketClientBuffer>& SocketClientBufferPtr);

	std::queue<std::shared_ptr<SocketClientBuffer>> SendQueue;
	CLLock SendLock;
	HANDLE hSendEvent;

	/////////Recv/////////////////////////////////////////////////////////////////

	// ��ӵ��������ݶ���
	VOID AddToTryRecvQueue(_In_ std::shared_ptr<SocketClientBuffer>& SocketClientBufferPtr);

	// ��ȡ ׼�����տͻ��˵�����
	BOOL GetTryRecvListClientBuffer(_In_ _Out_ BOOL& bLastExist, _In_ std::shared_ptr<SocketClientBuffer>& SocketClientBufferPtr);

	// ��ȡ �ͻ��˷����������� ��С
	BOOL GetClientSendSize(_In_ std::shared_ptr<SocketClientBuffer>& SocketClientBufferPtr, _Out_opt_ UINT& uSize, _Out_opt_ DWORD& dwMsg);

	std::queue<std::shared_ptr<SocketClientBuffer>> TryRecvQueue;
	CLLock TryRecvLock;
	HANDLE hTryRecvEvent;

	/////�Ͽ����Ӳ���/////////////////////////////////////////////////////////////////////

	// ��ӵ��Ͽ����ӵĶ���
	VOID AddToBreakQueue(_In_ std::shared_ptr<SocketClientInfo>& SocketClientInfoPtr);

	// ��ȡ�Ͽ����Ӷ��������
	BOOL GetBreakQueueData(_In_ _Out_ BOOL& bLastExists, _Out_ std::shared_ptr<SocketClientInfo>& SocketClientInfoPtr);

	std::queue<std::shared_ptr<SocketClientInfo>> BreakConnectQueue;
	CLLock BreakConnectLock;
	HANDLE hBreakConnectEvent;

	/////�����߳� ��ȡRecv���ݲ���/////////////////////////////////////////////////////////////////////

	// ��ӵ����������
	VOID AddWaitToProcessQueue(_In_ std::shared_ptr<SocketClientBuffer>& SocketClientBufferPtr);

	std::queue<std::shared_ptr<SocketClientBuffer>> WaitToProcessQueue;
	CLLock WaitToProcessLock;
	HANDLE hWaitToProcessEvent;
private:

	std::deque<std::shared_ptr<SocketClientInfo>> SocketClientBufferDeque;
	std::function<VOID(std::shared_ptr<SocketClientInfo>)> fnBreakConnect;

	std::vector<HANDLE> vhThread;
	SOCKET servSocket;
	BOOL IsStop;
	tagSocketServerVariable m_SocketVariable;
};



#endif