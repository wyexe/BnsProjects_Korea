#ifndef __GITBNSPROJECTS_BNSPROJECTS_GAME_SOCKET_SCRIPT_TRANSFERSCRIPT_H__
#define __GITBNSPROJECTS_BNSPROJECTS_GAME_SOCKET_SCRIPT_TRANSFERSCRIPT_H__

#include "../ConsoleDLL/SocketAPI.h"

class CTransferScript
{
public:
	CTransferScript() = default;
	~CTransferScript() = default;

	// 下载脚本
	std::wstring DownLoadScript(_In_ CONST std::wstring& wsScriptName) CONST;

	// 从服务器上查询今天还有多少个日常可以做
	VOID QueryDailyTask(_Out_ std::vector<std::wstring>& VecDailyTask) CONST;
private:
	// 解密脚本
	BOOL DecodeScript(_In_ _Out_ CSocketAPI::CiphertextScriptInfo& CiphertextContent, _Out_ std::wstring& wsScriptContent) CONST;
private:
	CSocketAPI _SocketAPI;
};



#endif // !__GITBNSPROJECTS_BNSPROJECTS_GAME_SOCKET_SCRIPT_EXTRACTSCRIPT_H__
