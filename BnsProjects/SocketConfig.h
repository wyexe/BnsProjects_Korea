#ifndef __GITBNSPROJECTS_BNSPROJECTS_GAME_SOCKET_CONFIG_SOCKETCONFIG_H__
#define __GITBNSPROJECTS_BNSPROJECTS_GAME_SOCKET_CONFIG_SOCKETCONFIG_H__

#include "../ConsoleDLL/SocketAPI.h"

class CSocketConfig
{
public:
	CSocketConfig() = default;
	~CSocketConfig() = default;

	// 保存日志 到 服务器
	VOID SaveAccountLog_To_Server(_In_ em_ClientAccountLog_Type emType, _In_ CONST std::wstring& wsText) CONST;

	// 读取云配置
	std::wstring GetConfigValue(_In_ CONST std::wstring& wsPlayerName, _In_ CONST std::wstring& wsConfigName) CONST;

	// 写入云配置
	VOID SetConfigValue(_In_ CONST ConfigContent& Content) CONST;
private:
	CSocketAPI _SocketAPI;
};



#endif // !__GITBNSPROJECTS_BNSPROJECTS_GAME_SOCKET_CONFIG_SOCKETCONFIG_H__
