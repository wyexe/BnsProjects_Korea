#ifndef __GITBNSPROJECTS_BNSKORSERVER_VERSOCKET_H__
#define __GITBNSPROJECTS_BNSKORSERVER_VERSOCKET_H__


#include "VerManage.h"
#include "UpdateFile.h"
#include "BnsTeam.h"
#include "SocketServer.h"

class CVerSocket
{
public:
	CVerSocket();
	~CVerSocket();

	static DWORD WINAPI _SendThread(LPVOID lpParm);
	static DWORD WINAPI _WorkThread(LPVOID lpParm);
	static DWORD WINAPI _KeepALiveThread(LPVOID lpParm);

	static BOOL _GetTaskScriptFileList(_In_ CONST ClientAccountContent& ClientAccount , _In_ ByteBuffer& ClientByteBuffer, _Out_ ByteBuffer& ServerByteBuffer);
	static BOOL GetTaskScriptFile(_In_ ByteBuffer& ClientByteBuffer, _Out_ ByteBuffer& ServerByteBuffer);
	static BOOL _GetTaskScriptFile(_In_ ByteBuffer& ClientByteBuffer, _Out_ ByteBuffer& ServerByteBuffer);
	static BOOL _DencryptScriptCode(_In_ std::string& strDesCode, _Out_ ByteBuffer& ServerByteBuffer);
};





//restore configuration
#undef SERVER_PORT
#undef REUSE_OBJECT
#undef ENHANCED_STABILITY
#endif