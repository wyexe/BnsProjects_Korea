#ifndef __GITBNSPROJECTS_BNSKORSERVER_CONFIG_CLIENTCONFIG_H__
#define __GITBNSPROJECTS_BNSKORSERVER_CONFIG_CLIENTCONFIG_H__
#include "VerManage.h"

class ByteBuffer;
class CClientConfig : public CClassInstance<CClientConfig>
{
public:
	CClientConfig() = default;
	~CClientConfig() = default;

public:

	//
	BOOL GetConfigValueByName(_In_ CONST ClientConfigContent& ClientConfigContent_, _Out_ ByteBuffer& ByteBuffer_) CONST;

	//
	BOOL SetConfigValueByName(_In_ CONST ClientConfigContent& ClientConfigContent_) CONST;

	//
	BOOL Initialize() CONST;


	// 
	VOID ClearDBConfig(_In_ UINT uDay) CONST;
private:
	BOOL CreateMapText(_In_ CONST ClientConfigContent& ClientConfigContent_, _Out_ std::wstring& wsText) CONST;

	VOID ActionConfigMap(_In_ std::function<VOID(std::map<std::wstring, std::wstring>&)> Worker) CONST;
private:
};


#endif