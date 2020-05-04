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
		// 比如说X秒之内必须发送超过Ykb
		DWORD	dwMaxSendSizeTime;
		UINT		uMaxSendSize;

		// max send time for everytime 
		DWORD	dwSingleMaxSendTime;

		// max recv time for every time
		DWORD	dwSingleMaxRecvTime;

		// max alloc memory size for recv buffer
		ULONGLONG	dwMaxAllocMemorySize;

		// 心跳超时时间(/s)
		DWORD	dwMaxKeepAliveTimeOut;

		// 锁定超时
		ULONGLONG	ulMaxLockTimeOut;
	};

public:
	explicit CSocketServer();
	~CSocketServer();

	CSocketServer(CONST CSocketServer&) = delete;
	VOID operator=(CONST CSocketServer&) = delete;

	// 设置掉线
	VOID SetBreakConnect(_In_ std::function<VOID(std::shared_ptr<SocketClientInfo>)> Worker);

	BOOL Run(_In_ DWORD dwPort, _In_ UINT uThreadCount);
	BOOL Stop();

	// Send线程
	static unsigned WINAPI _SendThread(LPVOID lpParm);
	
	// 获取Client是否发送了数据
	static unsigned WINAPI _TryRecvThread(LPVOID lpParm);

	// Recv数据线程
	static unsigned WINAPI _RecvThread(LPVOID lpParm);

	// Accept Client线程
	static unsigned WINAPI _AcceptThread(LPVOID lpParm);

	// 断开连接线程
	static unsigned WINAPI _BreakConnectThread(LPVOID lpParm);

	VOID SetSocketVariable(_In_ std::function<VOID(tagSocketServerVariable&)> Worker);

	// 发送
	VOID Send(_In_ std::shared_ptr<SocketClientBuffer>& SocketClientBufferPtr, _In_ CONST ByteBuffer& ByteBuffer_);

	// 获取待处理的数据
	BOOL GetWaitToProcessData(_In_ _Out_ BOOL& bLastExist, _Out_ std::shared_ptr<SocketClientBuffer>& SocketClientBufferPtr);
private:
	VOID PrintThreadCalcText(_In_ _Out_ ULONGLONG& ulTick, _In_ _Out_ UINT& uCount, _In_ LPCWSTR pwszFormat,...);

	BOOL DoAction_By_EmptyIndex(_In_ std::function<VOID(std::shared_ptr<SocketClientInfo>)> Worker);

	BOOL Lock_DoAction_By_ClientBuffer(_In_ std::function<VOID(std::shared_ptr<SocketClientInfo>)> Worker);

	int CustomeRecv(_In_ CONST std::shared_ptr<SocketClientInfo>& SocketClientInfoPtr, _In_ DWORD dwMsg, _In_ CHAR* buf, _In_ INT uLen);

	BOOL DoAction_By_SocketClientBufferDeque(_In_ std::function<VOID(std::deque<std::shared_ptr<SocketClientInfo>>&)> Worker);

	// 判断SOCKET是否有数据
	int GetClientSelect(_In_ SOCKET sk) CONST;

	////////Send//////////////////////////////////////////////////////////////////

	// 自定义发送
	int CustomeSend(_In_ CONST std::shared_ptr<SocketClientInfo>& SocketClientInfoPtr, _In_ DWORD dwMsg, _In_ CHAR* buf, _In_ INT uLen);

	// 回发客户端 告诉它 让他重发数据
	VOID Send_To_Client_ReSend(_In_ std::shared_ptr<SocketClientBuffer>& SocketClientBufferPtr);

	// 添加到 发送队列
	VOID AddToSendQueue(_In_ std::shared_ptr<SocketClientBuffer>& SocketClientBufferPtr);

	// 获取 准备发送到客户端的数据
	BOOL GetSendListClientBuffer(_In_ _Out_ BOOL& bLastExist, _Out_ std::shared_ptr<SocketClientBuffer>& SocketClientBufferPtr);

	std::queue<std::shared_ptr<SocketClientBuffer>> SendQueue;
	CLLock SendLock;
	HANDLE hSendEvent;

	/////////Recv/////////////////////////////////////////////////////////////////

	// 添加到接收数据队列
	VOID AddToTryRecvQueue(_In_ std::shared_ptr<SocketClientBuffer>& SocketClientBufferPtr);

	// 获取 准备接收客户端的数据
	BOOL GetTryRecvListClientBuffer(_In_ _Out_ BOOL& bLastExist, _In_ std::shared_ptr<SocketClientBuffer>& SocketClientBufferPtr);

	// 获取 客户端发送来的数据 大小
	BOOL GetClientSendSize(_In_ std::shared_ptr<SocketClientBuffer>& SocketClientBufferPtr, _Out_opt_ UINT& uSize, _Out_opt_ DWORD& dwMsg);

	std::queue<std::shared_ptr<SocketClientBuffer>> TryRecvQueue;
	CLLock TryRecvLock;
	HANDLE hTryRecvEvent;

	/////断开连接操作/////////////////////////////////////////////////////////////////////

	// 添加到断开连接的队列
	VOID AddToBreakQueue(_In_ std::shared_ptr<SocketClientInfo>& SocketClientInfoPtr);

	// 获取断开连接队伍的数据
	BOOL GetBreakQueueData(_In_ _Out_ BOOL& bLastExists, _Out_ std::shared_ptr<SocketClientInfo>& SocketClientInfoPtr);

	std::queue<std::shared_ptr<SocketClientInfo>> BreakConnectQueue;
	CLLock BreakConnectLock;
	HANDLE hBreakConnectEvent;

	/////处理线程 获取Recv数据操作/////////////////////////////////////////////////////////////////////

	// 添加到待处理队列
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