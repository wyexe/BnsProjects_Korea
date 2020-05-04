#include "SocketServer.h"
#include <memory>
#include <MyTools/Log.h>
#include <MyTools/Character.h>
#include <MyTools/CLPublic.h>

#define _SELF L"SocketServer.cpp"
CSocketServer::CSocketServer() : BreakConnectLock(L"BreakConnectLock"), TryRecvLock(L"TryRecvLock"), SendLock(L"SendLock"), WaitToProcessLock(L"WaitToProcessLock")
{
	IsStop = FALSE;
	SetSocketVariable([](tagSocketServerVariable& itm)
	{
		itm.dwMaxSendSizeTime = 5 * 1000;
		itm.uMaxSendSize = 100 * 1024;
		itm.dwSingleMaxSendTime = 6 * 1000;
		itm.dwSingleMaxRecvTime = 6 * 1000;
		itm.dwMaxAllocMemorySize = 2 * 1024;
		itm.dwMaxKeepAliveTimeOut = 1 * 60 * 1000;
		itm.ulMaxLockTimeOut = 5 * 60 * 1000;
	});

	// 创建事件
	hWaitToProcessEvent = ::CreateEventW(NULL, FALSE, FALSE, NULL);
	hSendEvent			= ::CreateEventW(NULL, FALSE, FALSE, NULL);
	hBreakConnectEvent  = ::CreateEventW(NULL, FALSE, FALSE, NULL);
	hTryRecvEvent		= ::CreateEventW(NULL, FALSE, FALSE, NULL);

}

CSocketServer::~CSocketServer()
{
	//Stop();
}

BOOL CSocketServer::Run(_In_ DWORD dwPort, _In_ UINT uThreadCount)
{
	// init var
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	servSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (servSocket == INVALID_SOCKET)
		return FALSE;

	// set io mode
	unsigned long ul = 1;
	int r = ioctlsocket(servSocket, FIONBIO, &ul);
	if (r == SOCKET_ERROR)
	{
		LOG_C(CLog::em_Log_Type::em_Log_Type_Exception, L"ioctlsocket Faild!");
		return FALSE;
	}

	// set sock variable
	sockaddr_in addr;
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(dwPort);

	// bind to port
	r = ::bind(servSocket, (sockaddr*)&addr, sizeof(addr));
	if (r == SOCKET_ERROR)
	{
		LOG_C(CLog::em_Log_Type::em_Log_Type_Exception, L"bind Faild!");
		return FALSE;
	}

	// start listen
	int ret = listen(servSocket, MAX_CLIENT_COUNT);
	if (ret == SOCKET_ERROR)
	{
		LOG_C(CLog::em_Log_Type::em_Log_Type_Exception, L"listen Faild!");
		return FALSE;
	}

	// client accept socket
	vhThread.push_back(cbBEGINTHREADEX(NULL, NULL, _AcceptThread, this, NULL, NULL));
	vhThread.push_back(cbBEGINTHREADEX(NULL, NULL, _TryRecvThread, this, NULL, NULL));
	vhThread.push_back(cbBEGINTHREADEX(NULL, NULL, _BreakConnectThread, this, NULL, NULL));

	for (UINT i = 0; i < uThreadCount; ++i)
	{
		HANDLE hThread = cbBEGINTHREADEX(NULL, NULL, _RecvThread, this, NULL, NULL);
		if (hThread == NULL)
		{
			LOG_MSG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"_RecvThread Err=%d", ::GetLastError());
		}
		vhThread.push_back(hThread);
	}
	for (UINT i = 0; i < uThreadCount; ++i)
	{
		HANDLE hThread = cbBEGINTHREADEX(NULL, NULL, (LPTHREAD_START_ROUTINE)_SendThread, this, NULL, NULL);
		if (hThread == NULL)
		{
			LOG_MSG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"_SendThread Err=%d", ::GetLastError());
		}
		vhThread.push_back(hThread);
	}
	
	return TRUE;
}

unsigned WINAPI CSocketServer::_AcceptThread(LPVOID lpParm)
{
	auto* pCSocketServer = (CSocketServer *)lpParm;

	//auto ulThreadCalcTick = ::GetTickCount64();
	UINT uThreadCalcCount = 0;
	while (!pCSocketServer->IsStop)
	{
		//pCSocketServer->PrintThreadCalcText(ulThreadCalcTick, uThreadCalcCount, L"Accept Thread 1分钟之内有%d个客户端连接", uThreadCalcCount);

		sockaddr_in addr;
		int len = sizeof(addr);
		SOCKET s = ::accept(pCSocketServer->servSocket, (sockaddr*)&addr, &len);
		if (s == SOCKET_ERROR || s == INVALID_SOCKET) // un exist client to server
		{
			Sleep(50);
			continue;
		}
		
		// has new client!
		uThreadCalcCount += 1;
		SocketSession SockSession(CCharacter::ASCIIToUnicode(inet_ntoa(addr.sin_addr)), s);
		//Log(L"普通", L"客户端:%s 连接上服务器了!", SockSession.wsClientIp.c_str());
		pCSocketServer->DoAction_By_EmptyIndex([&SockSession](std::shared_ptr<SocketClientInfo> pSocketClientInfoPtr)
		{
			pSocketClientInfoPtr->SocketSession_ = SockSession;
			pSocketClientInfoPtr->ulKeepALiveTick = ::GetTickCount64();
			pSocketClientInfoPtr->bRecving = FALSE;
			pSocketClientInfoPtr->ulRecvTick = ::GetTickCount64();
			pSocketClientInfoPtr->ulHash = ::GetTickCount64() & static_cast<ULONGLONG>(CCharacter::GetRand_For_DWORD());
			pSocketClientInfoPtr->bClose = FALSE;
		});
	}
	return TRUE;
}

BOOL CSocketServer::DoAction_By_EmptyIndex(_In_ std::function<VOID(std::shared_ptr<SocketClientInfo>)> Worker)
{
	// find empty array!
	DoAction_By_SocketClientBufferDeque([&Worker](std::deque<std::shared_ptr<SocketClientInfo>>& Dque)
	{
		// Add New Socket Buffer
		auto pSocketClientBuffer = std::make_shared<SocketClientInfo>();
		Worker(pSocketClientBuffer);

		/*static ULONGLONG ulTick = ::GetTickCount64();
		if (static_cast<DWORD>(::GetTickCount64() - ulTick) >= 1 * 60 * 1000)
		{
			LOG_CF(CLog::em_Log_Type::em_Log_Type_Custome, L"socket使用数量=%d", Dque.size());
			ulTick = ::GetTickCount64();
		}*/
		Dque.push_back(pSocketClientBuffer);
	});
	return TRUE;
}

VOID CSocketServer::SetBreakConnect(_In_ std::function<VOID(std::shared_ptr<SocketClientInfo>)> Worker)
{
	fnBreakConnect = Worker;
}

int CSocketServer::GetClientSelect(_In_ SOCKET sk) CONST
{
	fd_set fdrecv;
	struct timeval timeout;
	FD_ZERO(&fdrecv); /* clear the set */
	FD_SET(sk, &fdrecv);
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	int nRetCode = ::select(0, &fdrecv, NULL, NULL, &timeout);
	if (nRetCode < 0)
		return -1;
	else if (nRetCode == 0)
		return 0;

	return 1;
}

int CSocketServer::CustomeRecv(_In_ CONST std::shared_ptr<SocketClientInfo>& SocketClientInfoPtr, _In_ DWORD  dwMsg, _In_ CHAR* buf, _In_ INT uLen)
{
	auto uIndex = 0;
	auto ulTick = ::GetTickCount64();
	while (!IsStop && uIndex < uLen && !SocketClientInfoPtr->bClose)
	{
		if (static_cast<DWORD>(::GetTickCount64() - ulTick) >= m_SocketVariable.dwSingleMaxRecvTime)
		{
			LOG_C(CLog::em_Log_Type::em_Log_Type_Warning, L"recv超时! uLen=%d,uIndex=%d, dwMsg=%X, Err=%d", uLen, uIndex, dwMsg, WSAGetLastError());
			return SOCKET_ERROR;
		}

		int nRetCode = ::recv(SocketClientInfoPtr->SocketSession_.sClient, (CHAR*)(buf + uIndex), (uLen - uIndex), 0);
		if (nRetCode <= 0)
		{
			int nErr = WSAGetLastError();
			if (nErr == WSAEWOULDBLOCK || nErr == WSAEINVAL)
			{
				::Sleep(10);
				continue;
			}
			return SOCKET_ERROR;
		}
		uIndex += nRetCode;
	}
	return static_cast<int>(uIndex);
}

BOOL CSocketServer::Stop()
{
	IsStop = TRUE;
	for (auto& itm : vhThread)
	{
		::WaitForSingleObject(itm, INFINITE);
	}
	return TRUE;
}

VOID CSocketServer::SetSocketVariable(_In_ std::function<VOID(tagSocketServerVariable&)> Worker)
{
	Worker(m_SocketVariable);
}

BOOL CSocketServer::DoAction_By_SocketClientBufferDeque(_In_ std::function<VOID(std::deque<std::shared_ptr<SocketClientInfo>>& RefSocketClientBufferDeque)> Worker)
{
	static CLLock Lock(L"CSocketServer::DoAction_By_SocketClientBufferDeque");
	return Lock.Access([&Worker, this]{Worker(SocketClientBufferDeque); });
}

//////////////////////////////////////////////////////////////////////////

int CSocketServer::CustomeSend(_In_ CONST std::shared_ptr<SocketClientInfo>& SocketClientInfoPtr, _In_ DWORD dwMsg, _In_ CHAR* buf, _In_ INT uLen)
{
	int uSendCount = 0;
	auto ulTick = ::GetTickCount64();
	while (!IsStop && uSendCount < uLen  && !SocketClientInfoPtr->bClose)
	{
		if (static_cast<DWORD>(::GetTickCount64() - ulTick) >= m_SocketVariable.dwSingleMaxSendTime)
		{
			LOG_CF(CLog::em_Log_Type::em_Log_Type_Warning, L"CustomeSend超时! uLen=%d,dwMsg=%X, Err=%d", uLen, dwMsg, WSAGetLastError());
			return uSendCount;
		}

		int nRetCode = ::send(SocketClientInfoPtr->SocketSession_.sClient, (CHAR*)(buf + uSendCount), (uLen - uSendCount), 0);
		if (nRetCode <= 0)
		{
			int nErr = WSAGetLastError();
			if (nErr == WSAEWOULDBLOCK || nErr == WSAEINVAL)
			{
				::Sleep(10);
				continue;
			}
			return SOCKET_ERROR;
		}
		uSendCount += nRetCode;
	}
	return static_cast<int>(uSendCount);
}

BOOL CSocketServer::GetSendListClientBuffer(_In_ _Out_ BOOL& bLastExist, _Out_ std::shared_ptr<SocketClientBuffer>& SocketClientBufferPtr)
{
	auto fnGetSendListClientBuffer = [this, &SocketClientBufferPtr]
	{
		BOOL bRetCode = FALSE;
		SendLock.Access([&bRetCode, this, &SocketClientBufferPtr]
		{
			if (SendQueue.size() > 0)
			{
				SocketClientBufferPtr = SendQueue.front();
				SendQueue.pop();
				bRetCode = TRUE;
			}
		});
		
		return bRetCode;
	};

	// 如果上一次是能搜索到数据的话, 不需要经过事件句柄, 直接判断是否有内容
	if (bLastExist || ::WaitForSingleObject(hSendEvent, 100) == WAIT_OBJECT_0)
		bLastExist = fnGetSendListClientBuffer();

	return bLastExist;
}

unsigned WINAPI CSocketServer::_SendThread(LPVOID lpParm)
{
	auto pCSocketServer = reinterpret_cast<CSocketServer *>(lpParm);
	BOOL bExist = FALSE;
	//auto ulThreadCalcTick = ::GetTickCount64();
	UINT uThreadCalcCount = 0;

	while (!pCSocketServer->IsStop)
	{
		//pCSocketServer->PrintThreadCalcText(ulThreadCalcTick, uThreadCalcCount, L"Send Thread ID:%d 1分钟之内有%d个客户端连接", uThreadId, uThreadCalcCount);

		std::shared_ptr<SocketClientBuffer> SocketClientBufferPtr;
		if (!pCSocketServer->GetSendListClientBuffer(bExist, SocketClientBufferPtr))
			continue;

		uThreadCalcCount += 1;

		// Empyt Content!
		if (SocketClientBufferPtr->ByteBuffer_.size() == 0)
		{
			LOG_C(CLog::em_Log_Type::em_Log_Type_Warning, L"Empyt Content!");
			continue;
		}


		CHAR* buf = (CHAR*)SocketClientBufferPtr->ByteBuffer_.contents();
		auto uSendMaxSize = SocketClientBufferPtr->ByteBuffer_.size();
		UINT uSendIndex = NULL;
		UINT uSendCountInTick = NULL;
		auto ulTick = ::GetTickCount64();

		// 5s 之内达不到20K就算了
		while (!pCSocketServer->IsStop && uSendIndex < uSendMaxSize && !SocketClientBufferPtr->SocketClientInfoPtr->bClose)
		{
			UINT uSendSize = (uSendMaxSize - uSendIndex) >= MAX_SOCKET_PACKET_SIZE ? MAX_SOCKET_PACKET_SIZE : (uSendMaxSize - uSendIndex);
			int nRetCode = pCSocketServer->CustomeSend(SocketClientBufferPtr->SocketClientInfoPtr, SocketClientBufferPtr->dwMsg, buf + uSendIndex, uSendSize);
			if (nRetCode == -1)
			{
				// break connect!
				pCSocketServer->AddToBreakQueue(SocketClientBufferPtr->SocketClientInfoPtr);
				break;
			}
			uSendIndex += nRetCode;
			uSendCountInTick += nRetCode;
			if (static_cast<DWORD>(::GetTickCount64() - ulTick) >= pCSocketServer->m_SocketVariable.dwMaxSendSizeTime)
			{
				if (uSendCountInTick < pCSocketServer->m_SocketVariable.uMaxSendSize) // 5s之内没发送超过100KB
					break;

				ulTick = ::GetTickCount64();
				uSendCountInTick = NULL;
			}
		}
	}
	return TRUE;
}

VOID CSocketServer::Send(_In_ std::shared_ptr<SocketClientBuffer>& SocketClientBufferPtr, _In_ CONST ByteBuffer& ByteBuffer_)
{
	SocketClientBufferPtr->ByteBuffer_.clear();

	// 434C536F 636B6574 << Size << Msg
	SocketClientBufferPtr->ByteBuffer_ << 0x434C536F;
	SocketClientBufferPtr->ByteBuffer_ << 0x636B6574;
	SocketClientBufferPtr->ByteBuffer_ << ByteBuffer_.size();
	SocketClientBufferPtr->ByteBuffer_.append(ByteBuffer_.contents(), ByteBuffer_.size());
	AddToSendQueue(SocketClientBufferPtr);
}

VOID CSocketServer::AddToSendQueue(_In_ std::shared_ptr<SocketClientBuffer>& SocketClientBufferPtr)
{
	// 添加到 断开连接 队列去!
	SendLock.Access([&SocketClientBufferPtr, this]
	{
		/*static ULONGLONG ulTick = ::GetTickCount64();
		if (static_cast<DWORD>(::GetTickCount64() - ulTick) >= 1 * 60 * 1000)
		{
			LOG_CF(CLog::em_Log_Type::em_Log_Type_Custome, L"Send队列.size()=%d", SendQueue.size());
			ulTick = ::GetTickCount64();
		}*/
		SendQueue.push(SocketClientBufferPtr); 
	});

	// 通知断开连接线程开始干活!
	::SetEvent(hSendEvent);
}

VOID CSocketServer::Send_To_Client_ReSend(_In_ std::shared_ptr<SocketClientBuffer>& SocketClientBufferPtr)
{
	ByteBuffer ByteBuffer_;
	ByteBuffer_ << SOCKET_MESSAGE_ERROR;
	Send(SocketClientBufferPtr, ByteBuffer_);
}

//////////////////////////////////////////////////////////////////////////
BOOL CSocketServer::Lock_DoAction_By_ClientBuffer(_In_ std::function<VOID(std::shared_ptr<SocketClientInfo>)> Worker)
{
	DoAction_By_SocketClientBufferDeque([this, &Worker](std::deque<std::shared_ptr<SocketClientInfo>>& RefSocketClientBufferDeq)
	{
		for (auto& itm : RefSocketClientBufferDeq)
		{
			// 判断心跳是否超时!
			if (itm->ulKeepALiveTick != NULL && static_cast<DWORD>(::GetTickCount64() - itm->ulKeepALiveTick) >= m_SocketVariable.dwMaxKeepAliveTimeOut)
			{
				LOG_C(CLog::em_Log_Type::em_Log_Type_Debug, L"[%s,%s]心跳超时!, 断开连接!", itm->ClientAccountContent_.wsAccountName.c_str(), itm->SocketSession_.wsClientIp.c_str());
				itm->ulKeepALiveTick = NULL;
				AddToBreakQueue(itm);
				continue;
			}

			// 如果正在recv就 不重复!
			if (itm->bRecving)
				continue;

			Worker(itm);
		}
	});

	return TRUE;
}

unsigned WINAPI CSocketServer::_TryRecvThread(LPVOID lpParm)
{
	auto pCSocketServer = reinterpret_cast<CSocketServer *>(lpParm);
	while (!pCSocketServer->IsStop)
	{
		pCSocketServer->Lock_DoAction_By_ClientBuffer([pCSocketServer](std::shared_ptr<SocketClientInfo> SocketClientInfoPtr)
		{
			if (pCSocketServer->GetClientSelect(SocketClientInfoPtr->SocketSession_.sClient) != 0)
			{
				// 客户端那边来消息了

				// 更新心跳 并且设置 正在接收中
				SocketClientInfoPtr->ulKeepALiveTick = ::GetTickCount64();
				SocketClientInfoPtr->ulRecvTick = ::GetTickCount64();
				SocketClientInfoPtr->bRecving = TRUE;

				// 构造一个接收的缓冲区, 并且设置他的 客户端指针
				auto SocketClientBufferPtr = std::make_shared<SocketClientBuffer>();
				SocketClientBufferPtr->SocketClientInfoPtr = SocketClientInfoPtr;

				// 交给接收线程!
				pCSocketServer->AddToTryRecvQueue(SocketClientBufferPtr);
			}

		});

		::Sleep(1);
	}
	return TRUE;
}

VOID CSocketServer::AddToTryRecvQueue(_In_ std::shared_ptr<SocketClientBuffer>& SocketClientBufferPtr)
{
	// 添加到 接收 队列去!
	TryRecvLock.Access([&SocketClientBufferPtr, this]
	{
		/*static ULONGLONG ulTick = ::GetTickCount64();
		if (static_cast<DWORD>(::GetTickCount64() - ulTick) >= 1 * 60 * 1000)
		{
			LOG_CF(CLog::em_Log_Type::em_Log_Type_Custome, L"TryRecv队列.size()=%d", TryRecvQueue.size());
			ulTick = ::GetTickCount64();
		}*/
		TryRecvQueue.push(SocketClientBufferPtr); 
	});

	// 通知接收数据线程开始干活!
	::SetEvent(hTryRecvEvent);
}

BOOL CSocketServer::GetTryRecvListClientBuffer(_In_ _Out_ BOOL& bLastExist, _Out_ std::shared_ptr<SocketClientBuffer>& SocketClientBufferPtr)
{
	auto fnGetTryRecvListClientBuffer = [this, &SocketClientBufferPtr]
	{
		BOOL bRetCode = FALSE;
		TryRecvLock.Access([this, &SocketClientBufferPtr, &bRetCode]
		{
			if (TryRecvQueue.size() > 0)
			{
				SocketClientBufferPtr = TryRecvQueue.front();
				TryRecvQueue.pop();
				bRetCode = TRUE;
			}
		});
		
		return bRetCode;
	};

	// 如果上一次是能搜索到数据的话, 不需要经过事件句柄, 直接判断是否有内容
	if (bLastExist || ::WaitForSingleObject(hTryRecvEvent, 100) == WAIT_OBJECT_0)
		bLastExist = fnGetTryRecvListClientBuffer();

	return bLastExist;
}

BOOL CSocketServer::GetClientSendSize(_In_ std::shared_ptr<SocketClientBuffer>& SocketClientBufferPtr, _Out_opt_ UINT& uSize, _Out_opt_ DWORD& dwMsg)
{
	// 先接收2个Flag + 1个Size + MsgType
	DWORD dwArray[4] = { 0 };
	int nRetLen = CustomeRecv(SocketClientBufferPtr->SocketClientInfoPtr, NULL, (char*)&dwArray, sizeof(dwArray));
	if (nRetLen <= 0)
	{
		//LOG_C(CLog::em_Log_Type::em_Log_Type_Exception, L"AccName=%s, bRecving=%d RecvSize Faild!", SocketClientBufferPtr->SocketClientInfoPtr->wsAccountName.c_str(), SocketClientBufferPtr->SocketClientInfoPtr->bRecving );
		AddToBreakQueue(SocketClientBufferPtr->SocketClientInfoPtr);
		return FALSE;
	}

	// 不是Flag的话, 就表示可能其他程序连错了!. 
	if (dwArray[0] != 0x434C436C || dwArray[1] != 0x69656E74 || dwArray[2] == NULL)
	{
		// 直接断了, 没啥好说的!
		AddToBreakQueue(SocketClientBufferPtr->SocketClientInfoPtr);

		// 重新让Client发送一次消息
		//Send_To_Client_ReSend(pSocketClientBuffer);
		//pSocketClientBuffer->bLock = TRUE;
		return FALSE;
	}

	uSize = static_cast<UINT>(dwArray[2]);
	dwMsg = dwArray[3];
	return TRUE;
}

unsigned WINAPI CSocketServer::_RecvThread(LPVOID lpParm)
{
	auto pCSocketServer = reinterpret_cast<CSocketServer *>(lpParm);
	BOOL bExist = FALSE;
	//auto ulThreadCalcTick = ::GetTickCount64();
	UINT uThreadCalcCount = 0;
	while (!pCSocketServer->IsStop)
	{
		//pCSocketServer->PrintThreadCalcText(ulThreadCalcTick, uThreadCalcCount, L"Recv Thread:%d 1分钟之内有%d个客户端连接", uThreadId, uThreadCalcCount);

		std::shared_ptr<SocketClientBuffer> SocketClientBufferPtr;
		if (!pCSocketServer->GetTryRecvListClientBuffer(bExist, SocketClientBufferPtr))
			continue;

		uThreadCalcCount += 1;

		UINT uSize = 0;
		DWORD dwMsg = 0;
		if (!pCSocketServer->GetClientSendSize(SocketClientBufferPtr, uSize, dwMsg))
			continue;


		if (uSize >= pCSocketServer->m_SocketVariable.dwMaxAllocMemorySize)
		{
			LOG_C(CLog::em_Log_Type::em_Log_Type_Exception, L"申请内存貌似过大了吧?dwSize=%d,dwMsg=%X,Account:%s", uSize, dwMsg, SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_.wsAccountName.c_str());
			pCSocketServer->AddToBreakQueue(SocketClientBufferPtr->SocketClientInfoPtr);
			continue;
		}

		// 申请拿来放置Client内容缓冲区内存
		std::shared_ptr<CHAR> pbuff(new CHAR[uSize], [](CHAR* p){ delete[] p; });
		((DWORD*)pbuff.get())[0] = dwMsg;

		// 开始接收
		DWORD dwIndex = 4;
		while (!pCSocketServer->IsStop && dwIndex < uSize)
		{
			int nRetCode = pCSocketServer->CustomeRecv(SocketClientBufferPtr->SocketClientInfoPtr, NULL, (char*)(pbuff.get() + dwIndex), uSize - 4);
			if (nRetCode <= 0)
			{
				LOG_C(CLog::em_Log_Type::em_Log_Type_Warning, L"dwIndex=%d, uSize=%d, Msg=%X", dwIndex, uSize, dwIndex < 4 ? -1 : ((DWORD*)pbuff.get())[0]);
				break;
			}

			dwIndex += nRetCode;
		}

		// 发现接收的数据不一致, 可能是包不完全, 或者超时了!
		if (dwIndex != uSize)
		{
			LOG_C(CLog::em_Log_Type::em_Log_Type_Exception, L"Client发送的数据大小=%d != 接收的=%d", uSize, dwIndex);

			// 让客户端重新发送, 当前数据包被抛弃
			pCSocketServer->Send_To_Client_ReSend(SocketClientBufferPtr);
			continue;
		}

		SocketClientBufferPtr->ByteBuffer_.clear();

		// 添加内容
		SocketClientBufferPtr->ByteBuffer_.append(pbuff.get(), uSize);

		// 设置可以进行下一次的recv了
		SocketClientBufferPtr->SocketClientInfoPtr->bRecving = FALSE;

		// 添加到 待处理列表中!
		pCSocketServer->AddWaitToProcessQueue(SocketClientBufferPtr);
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

VOID CSocketServer::AddToBreakQueue(_In_ std::shared_ptr<SocketClientInfo>& SocketClientInfoPtr)
{
	// 添加到 断开连接 队列去!
	BreakConnectLock.Access([&SocketClientInfoPtr, this]
	{
		/*static ULONGLONG ulTick = ::GetTickCount64();
		if (static_cast<DWORD>(::GetTickCount64() - ulTick) >= 1 * 60 * 1000)
		{
			LOG_CF(CLog::em_Log_Type::em_Log_Type_Custome, L"BreakConn队列.size()=%d", BreakConnectQueue.size());
			ulTick = ::GetTickCount64();
		}*/
		BreakConnectQueue.push(SocketClientInfoPtr); 
	});

	// 通知断开连接线程开始干活!
	::SetEvent(hBreakConnectEvent);
}

BOOL CSocketServer::GetBreakQueueData(_In_ _Out_ BOOL& bLastExists, _Out_ std::shared_ptr<SocketClientInfo>& SocketClientInfoPtr)
{
	auto fnGetBreakQueueData = [this, &SocketClientInfoPtr]
	{
		BOOL bRetCode = FALSE;
		BreakConnectLock.Access([&SocketClientInfoPtr, this, &bRetCode]
		{
			if (BreakConnectQueue.size() > 0)
			{
				SocketClientInfoPtr = BreakConnectQueue.front();
				BreakConnectQueue.pop();
				bRetCode = TRUE;
			}
		});
		
		return bRetCode;
	};

	// 如果上一次是能搜索到数据的话, 不需要经过事件句柄, 直接判断是否有内容
	if (bLastExists || ::WaitForSingleObject(hBreakConnectEvent, 100) == WAIT_OBJECT_0)
		bLastExists = fnGetBreakQueueData();

	return bLastExists;
}

unsigned WINAPI CSocketServer::_BreakConnectThread(LPVOID lpParm)
{
	auto pCSocketServer = reinterpret_cast<CSocketServer *>(lpParm);
	BOOL bExist = FALSE;
	while (!pCSocketServer->IsStop)
	{
		std::shared_ptr<SocketClientInfo> SocketClientInfoPtr;
		if (!pCSocketServer->GetBreakQueueData(bExist, SocketClientInfoPtr))
		{
			::Sleep(100);
			continue;
		}

		pCSocketServer->fnBreakConnect(SocketClientInfoPtr);
		if (SocketClientInfoPtr->SocketSession_.sClient != NULL)
		{
			//::shutdown(SocketClientInfoPtr->SocketSession_.sClient, SD_BOTH);
			SocketClientInfoPtr->bClose = TRUE;
			::closesocket(SocketClientInfoPtr->SocketSession_.sClient);
			SocketClientInfoPtr->SocketSession_.sClient = INVALID_SOCKET;
		}
		
		// 从连接列表中删除!
		pCSocketServer->DoAction_By_SocketClientBufferDeque([&SocketClientInfoPtr](std::deque<std::shared_ptr<SocketClientInfo>>& Dque)
		{
			CLPublic::Dque_erase_if(Dque, [&SocketClientInfoPtr](std::shared_ptr<SocketClientInfo> Ptr){ return SocketClientInfoPtr->ulHash == Ptr->ulHash; });
		});
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////

BOOL CSocketServer::GetWaitToProcessData(_In_ _Out_ BOOL& bLastExist, _Out_ std::shared_ptr<SocketClientBuffer>& SocketClientBufferPtr)
{
	auto fnGetWaitToProcessData = [this, &SocketClientBufferPtr]
	{
		BOOL bRetCode = FALSE;
		WaitToProcessLock.Access([&SocketClientBufferPtr, this, &bRetCode]
		{
			if (WaitToProcessQueue.size() > 0)
			{
				SocketClientBufferPtr = WaitToProcessQueue.front();
				WaitToProcessQueue.pop();
				bRetCode = TRUE;
			}
		});
		
		return bRetCode;
	};

	// 如果上一次是能搜索到数据的话, 不需要经过事件句柄, 直接判断是否有内容
	if (bLastExist || ::WaitForSingleObject(hWaitToProcessEvent, 100) == WAIT_OBJECT_0)
		bLastExist = fnGetWaitToProcessData();
	
	return bLastExist;
}

VOID CSocketServer::AddWaitToProcessQueue(_In_ std::shared_ptr<SocketClientBuffer>& SocketClientBufferPtr)
{
	// 添加到 待处理 队列去!
	WaitToProcessLock.Access([&SocketClientBufferPtr, this]
	{
		/*static ULONGLONG ulTick = ::GetTickCount64();
		if (static_cast<DWORD>(::GetTickCount64() - ulTick) >= 1 * 60 * 1000)
		{
			LOG_CF(CLog::em_Log_Type::em_Log_Type_Custome, L"WaitToProcessQueue队列.size()=%d", WaitToProcessQueue.size());
			ulTick = ::GetTickCount64();
		}*/
		WaitToProcessQueue.push(SocketClientBufferPtr); 
	});

	// 通知 待处理 线程开始干活!
	::SetEvent(hWaitToProcessEvent);
}

//////////////////////////////////////////////////////////////////////////
VOID CSocketServer::PrintThreadCalcText(_In_ _Out_ ULONGLONG& ulTick , _In_ _Out_ UINT& uCount, _In_ LPCWSTR pwszFormat, ...)
{
	if (static_cast<DWORD>(::GetTickCount64() - ulTick) >= 60 * 1000)
	{
		va_list		args;
		WCHAR		szBuff[1024];
		va_start(args, pwszFormat);
		_vsnwprintf_s(szBuff, _countof(szBuff) - 1, _TRUNCATE, pwszFormat, args);
		va_end(args);

		LOG_CF(CLog::em_Log_Type::em_Log_Type_Custome, L"%s", szBuff);

		// 重新计算60s
		ulTick = ::GetTickCount64();

		// 重新计算
		uCount = NULL;
	}
}
