#ifndef __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_CONFIG_CLOUDCONFIGEXTEND_H__
#define __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_CONFIG_CLOUDCONFIGEXTEND_H__

#include <map>
#include <MyTools/CLLock.h>
#include <MyTools/ClassInstance.h>
#include "ServerCommon.h"

class CCloudConfigExtend : public MyTools::CClassInstance<CCloudConfigExtend>
{
public:
	CCloudConfigExtend() : _Lock(L"CCloudConfigExtend.Lock") {};
	~CCloudConfigExtend() = default;

	// ��ʼ��, ��ȡ���ݿ���������Ч�������õ��ڴ�, �������ݿ����Ӵ���
	BOOL Initialize();

	// ���������õ�ֵ
	BOOL FindConfigValue(_In_ CONST ClientConfigContent& ClientConfigContent_, _Out_ std::wstring& wsConfigValue) CONST;

	// д��������
	VOID WriteConfigValue(_In_ CONST ClientConfigContent& ClientConfigContent_);

	//
	VOID Print();
private:
	MyTools::CLLock _Lock;
	std::map<std::wstring, std::wstring> _MapConfig;
};

#endif // !__GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_CONFIG_CLOUDCONFIGEXTEND_H__
