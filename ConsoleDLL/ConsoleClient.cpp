#include "stdafx.h"
#include "ConsoleClient.h"
#include <algorithm>
#include <memory>
#include <errno.h>
#include <MyTools/Log.h>
#include <MyTools/Character.h>
#include <MyTools/CLPublic.h>
#include <MyTools/CLEchoException.h>
#include <thread>
#include <mutex>

#define _SELF L"SocketClient.cpp"
CConsoleClient::CConsoleClient() : _ExtractActionPtr(nullptr), _EchoErrorPtr(nullptr), _dwMsgHead(NULL), _PrintToConsolePtr(nullptr)
{
	_hEvent = INVALID_HANDLE_VALUE;
}


BOOL CConsoleClient::SyncSend(_In_ em_Sock_Msg emMsg, _In_ std::function<VOID(MyTools::CLSocketBuffer&)> FillBufferPtr)
{
	MyTools::CLSocketBuffer SocketBuffer;
	SocketBuffer.InitializeHead(emMsg);

	if (FillBufferPtr != nullptr)
		FillBufferPtr(SocketBuffer);


	return SyncSend(SocketBuffer, [](MyTools::CLSocketBuffer&) {});
}

BOOL CConsoleClient::SyncSend(_In_ MyTools::CLSocketBuffer& SocketBuffer, std::function<VOID(MyTools::CLSocketBuffer& SocketBuffer)> ExtractPtr)
{
	std::lock_guard<std::mutex> Lck(_Mtx);

	_dwMsgHead = SocketBuffer.GetMsgHead<DWORD>();
	_ExtractActionPtr = ExtractPtr;
	_hEvent = ::CreateEventW(NULL, FALSE, FALSE, NULL);
	if (_hEvent == NULL)
	{
		LOG_MSG_CF(L"_hEvent = NULL");
		return FALSE;
	}

	for (int i = 0; i < 3; ++i)
	{
		if (!PostSend(SocketBuffer))
		{
			LOG_CF_E(L"AsyncSend = FALSE");
			return FALSE;
		}
		if (::WaitForSingleObject(_hEvent, 10 * 1000) == WAIT_TIMEOUT)
		{
			LOG_CF_E(L"SyncSend Timeout 10s! Msg=%X, ReSend...", _dwMsgHead);
			continue;
		}

		// Correct Echo Packet
		::CloseHandle(_hEvent);
		_hEvent = INVALID_HANDLE_VALUE;
		_ExtractActionPtr = nullptr;
		_dwMsgHead = NULL;
		return TRUE;
	}

	DisConnectServer();
	return FALSE;
}

VOID CConsoleClient::SetEchoErrorPtr(_In_ std::function<VOID(CONST std::wstring&)> EchoErrorPtr)
{
	_EchoErrorPtr = EchoErrorPtr;
}

VOID CConsoleClient::SetPrintToConsolePtr(_In_ std::function<VOID(CONST std::wstring&)> PrintToConsolePtr)
{
	_PrintToConsolePtr = PrintToConsolePtr;
}

VOID CConsoleClient::EchoPacket(_In_ MyTools::CLSocketBuffer& SocketBuffer)
{
	em_Sock_Msg emSockMsg;
	auto Error = SocketBuffer.VerDataPtr<em_Sock_Msg>(emSockMsg);
	if (Error != MyTools::CLSocketBuffer::em_SocketBuffer_Error::em_SocketBuffer_Error_None)
	{
		DWORD dwSize, dwValue1, dwValue2 = 0;
		SocketBuffer >> dwSize >> dwValue1 >> dwValue2;
		LOG_CF_E(L"dwSize=%d != [%d], dwValue1=%X, dwValue2=%X", dwSize, SocketBuffer.size(), dwValue1, dwValue2);
		LOG_CF_E(L"解包错误, Error=%d", Error);
		return;
	}

	if (emSockMsg == em_Sock_Msg::em_Sock_Msg_ErrorText)
	{
		std::wstring wsText;
		SocketBuffer >> wsText;

		LOG_CF_E(L"服务器返回消息:%s", wsText.c_str());
		PrintToConsole(L"服务器返回消息:" + wsText);
		EchoError(wsText);
		return;
	}

	if (_hEvent == INVALID_HANDLE_VALUE || SocketBuffer.GetMsgHead<DWORD>() != _dwMsgHead)
	{
		LOG_CF_E(L"不处理的包,_hEvent=%X, Head=%X,_dwMsgHead=%X", _hEvent, emSockMsg, _dwMsgHead);
		return;
	}


	// Exctrcat Packet
	if (_ExtractActionPtr != nullptr)
		_ExtractActionPtr(SocketBuffer);

	::SetEvent(_hEvent);
}

VOID CConsoleClient::DisConnectServer()
{
	EchoError(L"与服务器失去连接……");
}

VOID CConsoleClient::EchoError(_In_ CONST std::wstring& wsLog)
{
	if (_EchoErrorPtr != nullptr)
	{
		_EchoErrorPtr(wsLog);
	}
}

VOID CConsoleClient::PrintToConsole(_In_ CONST std::wstring& wsLog)
{
	if (_PrintToConsolePtr != nullptr)
	{
		_PrintToConsolePtr(wsLog);
	}
}