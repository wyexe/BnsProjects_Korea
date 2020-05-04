#include "CloudConfigExtend.h"
#include <iostream>
#include <MyTools/Character.h>
#include <MyTools/Log.h>
#include "DbManager.h"

#define _SELF L"CloudConfigExtend.cpp"
std::wstring ClientConfigContent::to_wstring() CONST
{
	return MyTools::CCharacter::MakeFormatText(L"%s-%s-%s", this->wsAccountName.c_str(), this->wsPlayerName.c_str(), this->ConfigContent_.wsConfigName.c_str());
}

BOOL CCloudConfigExtend::Initialize()
{
	BOOL bRetCode = FALSE;
	_Lock.Access([&] 
	{ 
		bRetCode = CBnsDbManager::GetInstance().GetCloudConfigList(_MapConfig); 
		std::wcout << L"Cloud Config =" << _MapConfig.size() << std::endl;
	});
	return bRetCode;
}

BOOL CCloudConfigExtend::FindConfigValue(_In_ CONST ClientConfigContent& ClientConfigContent_, _Out_ std::wstring& wsConfigValue) CONST
{
	BOOL bExist = FALSE;
	_Lock.Access([&]
	{
		auto itr = _MapConfig.find(ClientConfigContent_.to_wstring());
		if (itr != _MapConfig.end())
		{
			bExist = TRUE;
			wsConfigValue = itr->second;
		}
	});

	return bExist;
}

VOID CCloudConfigExtend::WriteConfigValue(_In_ CONST ClientConfigContent& ClientConfigContent_)
{
	_Lock.Access([&] 
	{
		auto itr = _MapConfig.find(ClientConfigContent_.to_wstring());
		if (itr == _MapConfig.end())
			_MapConfig.insert(std::make_pair(ClientConfigContent_.to_wstring(), ClientConfigContent_.ConfigContent_.wsConfigValue));
		else if (itr != _MapConfig.end() && itr->second != ClientConfigContent_.ConfigContent_.wsConfigValue)
			itr->second = ClientConfigContent_.ConfigContent_.wsConfigValue;
	});

	CBnsDbManager::GetInstance().AsyncExcuteSQL(MyTools::CCharacter::MakeFormatText(L"exec proc_SetConfig N'%s',N'%s',N'%s',N'%s'", \
		ClientConfigContent_.wsAccountName.c_str(), ClientConfigContent_.wsPlayerName.c_str(), ClientConfigContent_.ConfigContent_.wsConfigName.c_str(), ClientConfigContent_.ConfigContent_.wsConfigValue.c_str()));
}

VOID CCloudConfigExtend::Print()
{
	_Lock.Access([=] 
	{
		LOG_CF_D(L"CloudConfig.size=%d", _MapConfig.size());
	});
}

