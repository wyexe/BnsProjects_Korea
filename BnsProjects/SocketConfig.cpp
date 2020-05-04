#include "stdafx.h"
#include "SocketConfig.h"

VOID CSocketConfig::SaveAccountLog_To_Server(_In_ em_ClientAccountLog_Type emType, _In_ CONST std::wstring& wsText) CONST
{
	ClientAccountLog Content;
	Content.emType = emType;
	Content.wsContentText = std::move(wsText);

	_SocketAPI.AddAccountLog(Content);
}

std::wstring CSocketConfig::GetConfigValue(_In_ CONST std::wstring& wsPlayerName, _In_ CONST std::wstring& wsConfigName) CONST
{
	ConfigContent Content;
	Content.wsConfigName = wsConfigName;
	_SocketAPI.ReadCloudConfig(wsPlayerName, Content);
	return Content.wsConfigValue;
}

VOID CSocketConfig::SetConfigValue(_In_ CONST ConfigContent& Content) CONST
{
	_SocketAPI.WriteCloudConfig(std::move(Content));
}
