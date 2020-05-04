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

	// 查询数据库内有效的卡号(还没过期,但是已经被激活)
	BOOL GetEffectiveCardList(_Out_ std::map<DWORD,CardInfo>& MapCard) CONST;

	// 查询数据库内所有的云配置
	BOOL GetCloudConfigList(_Out_ std::map<std::wstring,std::wstring>& MapConfig) CONST;

	// 根据CRC查找卡号
	BOOL FindCard_By_CRC(_In_ DWORD dwCardCrc, _Out_ CardInfo& Card) CONST;

	// 添加登录日志
	BOOL AddLoginLogToDb(_In_ CONST ClientAccountContent& ClientAccountContent_) CONST;

	// 查询数据库内的OTP
	BOOL GetOTPContent(_Out_ std::map<std::wstring,std::wstring>& MapOTP) CONST;

	// 添加游戏断开连接日志
	BOOL AddBreakConnectLogToDb(_In_ CONST ClientAccountContent& ClientAccountContent_) CONST;

	// 添加封号记录
	BOOL AddBannedRecordToDb(_In_ CONST ClientAccountContent& ClientAccountContent_) CONST;

	// 添加帐号日志
	BOOL AddAccountLogToDb(_In_ CONST ClientAccountContent& ClientAccountContent_, _In_ CONST ClientAccountLog& AccountLog) CONST;

	// 添加交易记录
	BOOL AddTradeContentToDb(_In_ CONST ClientAccountContent& ClientAccountContent_, _In_ CONST TradeContent& TradeContent_) CONST;

	// 获取公告
	BOOL GetClientTitle(_In_ CONST std::wstring& wsIp, _Out_ std::wstring& wsTitle) CONST;

	// 激活卡号
	BOOL ActiveCard(_In_ CONST ClientAccountContent& ClientAccountContent_, _In_ DWORD dwCardCrc, _Out_ CardInfo& CardInfo_) CONST;

	// 清空N天前的云配置(是近期N天没修改过的云配置,比如说经常修改的云配置是不会删的)
	BOOL CleanRecentlyCloudConfig(_In_ UINT uDay) CONST;

	// 这个号是否已经被封了(新封的号)
	BOOL IsNewBanned(_In_ CONST std::wstring& wsAccountName) CONST;

	// 查询账号的虚拟硬件
	BOOL QueryAccountSeriaNumber(_In_ std::wstring& wsAccountName, _Out_ SocketAccountVirtualSeriaNumber& Content) CONST;

	virtual VOID Initialize() override;
};

#endif // !__GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_DB_DBMANAGER_H__
