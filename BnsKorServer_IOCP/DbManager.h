#ifndef __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_DB_DBMANAGER_H__
#define __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_DB_DBMANAGER_H__

#include <map>
#include <MyTools/CLDbManager.h>
#include <MyTools/ClassInstance.h>
#include "ServerCommon.h"

class CBnsDbManager : public MyTools::CLDbManager, public MyTools::CClassInstance<CBnsDbManager>
{
public:
	
public:
	CBnsDbManager() = default;
	~CBnsDbManager() = default;

	// ��ѯ���ݿ�����Ч�Ŀ���(��û����,�����Ѿ�������)
	BOOL GetEffectiveCardList(_Out_ std::map<DWORD,CardInfo>& MapCard) CONST;

	// ��ѯ���ݿ������е�������
	BOOL GetCloudConfigList(_Out_ std::map<std::wstring,std::wstring>& MapConfig) CONST;

	// ����CRC���ҿ���
	BOOL FindCard_By_CRC(_In_ DWORD dwCardCrc, _Out_ CardInfo& Card) CONST;

	// ��ӵ�¼��־
	BOOL AddLoginLogToDb(_In_ CONST ClientAccountContent& ClientAccountContent_) CONST;

	// ��ѯ���ݿ��ڵ�OTP
	BOOL GetOTPContent(_Out_ std::map<std::wstring,std::wstring>& MapOTP) CONST;

	// �����Ϸ�Ͽ�������־
	BOOL AddBreakConnectLogToDb(_In_ CONST ClientAccountContent& ClientAccountContent_) CONST;

	// ��ӷ�ż�¼
	BOOL AddBannedRecordToDb(_In_ CONST ClientAccountContent& ClientAccountContent_) CONST;

	// ����ʺ���־
	BOOL AddAccountLogToDb(_In_ CONST ClientAccountContent& ClientAccountContent_, _In_ CONST ClientAccountLog& AccountLog) CONST;

	// ��ӽ��׼�¼
	BOOL AddTradeContentToDb(_In_ CONST ClientAccountContent& ClientAccountContent_, _In_ CONST TradeContent& TradeContent_) CONST;

	// ��ȡ����
	BOOL GetClientTitle(_In_ CONST std::wstring& wsIp, _Out_ std::wstring& wsTitle) CONST;

	// �����
	BOOL ActiveCard(_In_ CONST ClientAccountContent& ClientAccountContent_, _In_ DWORD dwCardCrc, _Out_ CardInfo& CardInfo_) CONST;

	// ���N��ǰ��������(�ǽ���N��û�޸Ĺ���������,����˵�����޸ĵ��������ǲ���ɾ��)
	BOOL CleanRecentlyCloudConfig(_In_ UINT uDay) CONST;

	// ������Ƿ��Ѿ�������(�·�ĺ�)
	BOOL IsNewBanned(_In_ CONST std::wstring& wsAccountName) CONST;

	// ��ѯ�˺ŵ�����Ӳ��
	BOOL QueryAccountSeriaNumber(_In_ std::wstring& wsAccountName, _Out_ SocketAccountVirtualSeriaNumber& Content) CONST;

	virtual VOID Initialize() override;
};

#endif // !__GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_DB_DBMANAGER_H__
