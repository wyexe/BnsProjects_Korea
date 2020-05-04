#ifndef __GITBNSPROJECTS_BNSKORSERVER_DATABASEMODEL_H__
#define __GITBNSPROJECTS_BNSKORSERVER_DATABASEMODEL_H__

#include <MyTools/Log.h>
#include "../BnsProjects/GameStruct.h"
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <odbcss.h>

class CDataBaseMdel
{
public:
	typedef struct _tmpCard
	{
		WCHAR wszCardNo[64];
		WCHAR wszSurplusHour[64];
		WCHAR wszActiveDate[64];
		WCHAR wszMaxHour[64];
	}tmpCard;

	struct tmpClientConfig
	{
		std::wstring wsAccountName;
		std::wstring wsPlayerName;
		std::wstring wsConfigName;
		std::wstring wsConfigValue;
	};

	struct OTPAccountContent
	{
		std::wstring wsAccountName;
		std::wstring wsAccountKey;
	};
public:
	CDataBaseMdel();
	~CDataBaseMdel();

	static VOID PrintSQLErrText(_In_ SQLSMALLINT fHandleType, _In_ SQLHANDLE handle, _In_ int ConnInd);
	static BOOL	OpenSQLCon();

	static VOID	Lock();
	static VOID	UnLock();
	static VOID Clear();

	static BOOL GetCardList(_Out_ std::deque<CardInfo>& vlst);
	static BOOL GetClientConfigList(_In_ std::deque<tmpClientConfig>& vlst);

	static BOOL ExcuteSQL(LPCWSTR pwszSQL, _Out_ LPWSTR pwszMsg);
	static BOOL GetCardInfo(_Out_ CardInfo& Card);

	static BOOL InsertLoginLog(_In_ CONST std::wstring& wsCardNo, _In_ CONST std::wstring& wsIp, _In_ CONST std::wstring& wsAccountName, _In_ CONST std::wstring& wsAccountPass);

	static DWORD WINAPI _ExcuteSQLThread(LPVOID lpParm);

	static BOOL GetOTPAccountList(_Out_ std::deque<OTPAccountContent>& vlst);

	static BOOL AsyncExcuteSQL(_In_ CONST std::wstring& wsSQLText);
private:
	static BOOL DoSQL(_In_ std::function<VOID(std::queue<std::wstring>&)> fn);
public:
	static SQLHENV  m_hEnv;
	static SQLHDBC	ms_hdbc;
	static SQLHSTMT ms_hstmt;
	static CRITICAL_SECTION ms_DbSection;
private:
};



#endif