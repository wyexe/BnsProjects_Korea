#include "ClientConfig.h"
#include <MyTools/CLLock.h>
#include <MyTools/ByteBuffer.h>

#define _SELF L"ClientConfig.cpp"
VOID CClientConfig::ActionConfigMap(_In_ std::function<VOID(std::map<std::wstring, std::wstring>&)> Worker) CONST
{
	static std::map<std::wstring, std::wstring> ClientConfigMap;
	static CLLock Lock(L"ActionConfigMap");
	Lock.Access([&Worker]{ Worker(ClientConfigMap); });
}

BOOL CClientConfig::CreateMapText(_In_ CONST ClientConfigContent& ClientConfigContent_, _Out_ std::wstring& wsText) CONST
{
	WCHAR wszText[MAX_PATH] = { 0 };
	swprintf_s(wszText, _countof(wszText) - 1, L"%s-%s-%s", ClientConfigContent_.wsAccountName.c_str(), ClientConfigContent_.wsPlayerName.c_str(), ClientConfigContent_.ConfigContent_.wsConfigName.c_str());
	wsText = wszText;
	return TRUE;
}

BOOL CClientConfig::GetConfigValueByName(_In_ CONST ClientConfigContent& ClientConfigContent_, _Out_ ByteBuffer& ByteBuffer_) CONST
{
	// Is Exist 'ClientConfigMap'
	ByteBuffer_ << SOCK_MSG_GET_CONFIG_VALUE << ClientConfigContent_.wsAccountName;
	ActionConfigMap([&ByteBuffer_, ClientConfigContent_, this](std::map<std::wstring, std::wstring>& ClientConfigMap)
	{
		static UINT uCount = 0;
		if (++uCount % 100 == 0)
			LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"ClientConfigMap.size=%d", ClientConfigMap.size());

		std::wstring wsText;
		CreateMapText(ClientConfigContent_, wsText);
		auto itr = ClientConfigMap.find(wsText);
		//LOG_CF(CLog::em_Log_Type_Debug, L"Get ConfigName=%s, Value=%s", wsText.c_str(), (itr != ClientConfigMap.end() ? itr->second.c_str() : L"NULL"));
		ByteBuffer_ << (itr != ClientConfigMap.end() ? itr->second : L"");
	});

	return TRUE;
}

BOOL CClientConfig::SetConfigValueByName(_In_ CONST ClientConfigContent& ClientConfigContent_) CONST
{
	ActionConfigMap([ClientConfigContent_, this](std::map<std::wstring, std::wstring>& ClientConfigMap)
	{
		std::wstring wsText;
		CreateMapText(ClientConfigContent_, wsText);
		auto itr = ClientConfigMap.find(wsText);
		//LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"Set ConfigName=%s, Value=%s", wsText.c_str(), ClientConfigContent_.Config_.wsConfigValue.c_str());
		if (itr == ClientConfigMap.end())
			ClientConfigMap.insert(std::make_pair(wsText, ClientConfigContent_.ConfigContent_.wsConfigValue));
		else
			itr->second = ClientConfigContent_.ConfigContent_.wsConfigValue;
	});

	// Add to DataBase when UnExist in Map
	WCHAR wszSQL[MAX_PATH] = { 0 };
	WCHAR wszRetMsg[MAX_PATH] = { 0 };
	swprintf_s(wszSQL, _countof(wszSQL) - 1, L"exec proc_SetConfig N'%s',N'%s',N'%s',N'%s'", ClientConfigContent_.wsAccountName.c_str(), \
		ClientConfigContent_.wsPlayerName.c_str(), ClientConfigContent_.ConfigContent_.wsConfigName.c_str(), ClientConfigContent_.ConfigContent_.wsConfigValue.c_str());
	return CDataBaseMdel::ExcuteSQL(wszSQL, wszRetMsg);
}

BOOL CClientConfig::Initialize() CONST
{
	std::deque<CDataBaseMdel::tmpClientConfig> vlst;
	CDataBaseMdel::GetClientConfigList(vlst);

	ActionConfigMap([&vlst](std::map<std::wstring, std::wstring>& ClientConfigMap)
	{
		for (CONST auto& itm : vlst)
		{
			WCHAR wszText[MAX_PATH] = { 0 };
			swprintf_s(wszText,_countof(wszText), L"%s-%s-%s", itm.wsAccountName.c_str(), itm.wsPlayerName.c_str(), itm.wsConfigName.c_str());

			ClientConfigMap.insert(std::make_pair(std::wstring(wszText), itm.wsConfigValue));
		}
	});
	return TRUE;
}

VOID CClientConfig::ClearDBConfig(_In_ UINT uDay) CONST
{
	std::shared_ptr<WCHAR> wsTextPtr(new WCHAR[1024], [](WCHAR* p){ delete[] p; });
	swprintf_s(wsTextPtr.get(), 1024 - 1, L"exec ClearAccoutConfig %d", uDay);
	CDataBaseMdel::AsyncExcuteSQL(wsTextPtr.get());
}
