#include "DataBaseModel.h"
#include <MyTools/CLLock.h>

#define _SELF L"DataBaseModel.cpp"

SQLHDBC	CDataBaseMdel::ms_hdbc = SQL_NULL_HDBC;
SQLHSTMT CDataBaseMdel::ms_hstmt = SQL_NULL_HSTMT;
SQLHENV CDataBaseMdel::m_hEnv = SQL_NULL_HENV;
CRITICAL_SECTION CDataBaseMdel::ms_DbSection;

CDataBaseMdel::CDataBaseMdel()
{
}

CDataBaseMdel::~CDataBaseMdel()
{
}

VOID CDataBaseMdel::Lock()
{
	::EnterCriticalSection(&ms_DbSection);
}

VOID CDataBaseMdel::UnLock()
{
	Clear();
	::LeaveCriticalSection(&ms_DbSection);
}

VOID CDataBaseMdel::PrintSQLErrText(_In_ SQLSMALLINT fHandleType, _In_ SQLHANDLE handle, _In_ int ConnInd)
{
	RETCODE plm_retcode = SQL_SUCCESS;
	SQLWCHAR plm_szSqlState[MAX_PATH] = L"", plm_szErrorMsg[MAX_PATH] = L"";
	SDWORD plm_pfNativeError = 0L;
	SWORD plm_pcbErrorMsg = 0;
	SQLSMALLINT plm_cRecNmbr = 1;
	SDWORD plm_SS_MsgState = 0, plm_SS_Severity = 0;
	SQLINTEGER plm_Rownumber = 0;
	USHORT plm_SS_Line;
	SQLSMALLINT plm_cbSS_Procname, plm_cbSS_Srvname;
	SQLCHAR plm_SS_Procname[MAXNAME], plm_SS_Srvname[MAXNAME];

	while (plm_retcode != SQL_NO_DATA_FOUND) 
	{
		plm_retcode = SQLGetDiagRec(fHandleType, handle, plm_cRecNmbr, plm_szSqlState, &plm_pfNativeError, plm_szErrorMsg, MAX_PATH - 1, &plm_pcbErrorMsg);
		if (plm_retcode != SQL_NO_DATA_FOUND) 
		{
			if (ConnInd) 
			{
				plm_retcode = SQLGetDiagField(fHandleType, handle, plm_cRecNmbr, SQL_DIAG_ROW_NUMBER, &plm_Rownumber, SQL_IS_INTEGER, NULL);

				plm_retcode = SQLGetDiagField(fHandleType, handle, plm_cRecNmbr, SQL_DIAG_SS_LINE, &plm_SS_Line, SQL_IS_INTEGER, NULL);

				plm_retcode = SQLGetDiagField(fHandleType, handle, plm_cRecNmbr, SQL_DIAG_SS_MSGSTATE, &plm_SS_MsgState, SQL_IS_INTEGER, NULL);

				plm_retcode = SQLGetDiagField(fHandleType, handle, plm_cRecNmbr, SQL_DIAG_SS_SEVERITY, &plm_SS_Severity, SQL_IS_INTEGER, NULL);

				plm_retcode = SQLGetDiagField(fHandleType, handle, plm_cRecNmbr, SQL_DIAG_SS_PROCNAME, &plm_SS_Procname, sizeof(plm_SS_Procname), &plm_cbSS_Procname);

				plm_retcode = SQLGetDiagField(fHandleType, handle, plm_cRecNmbr, SQL_DIAG_SS_SRVNAME, &plm_SS_Srvname, sizeof(plm_SS_Srvname), &plm_cbSS_Srvname);
			}

			LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"SqlState=%s", plm_szSqlState);
			LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"NativeError=%d", plm_pfNativeError);
			LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"ErrorMsg=%s", plm_szErrorMsg);
			LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"pcbErrorMsg=%d", plm_pcbErrorMsg);

			if (ConnInd) 
			{
				LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"ODBCRowNumber=%d", plm_Rownumber);
				LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"SSrvrLine=%d", plm_Rownumber);
				LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"SSrvrMsgState=%d", plm_SS_MsgState);
				LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"SSrvrSeverity=%d", plm_SS_Severity);
				LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"SSrvrProcname=%s", plm_SS_Procname);
				LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"SSrvrSrvname=%s", plm_SS_Srvname);
			}
		}

		plm_cRecNmbr++;   // Increment to next diagnostic record.
	}
}

BOOL CDataBaseMdel::OpenSQLCon()
{
	// 配置ODBC必须是用户Dns
	WCHAR wszDns[32] = { L"BnsKor" };
	WCHAR wszUser[32] = { L"GameUser" };
	WCHAR wszPass[32] = { L"123123" };

	RETCODE RetCode = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &m_hEnv);
	if (RetCode != SQL_SUCCESS)
	{
		LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"分配环境失败:SQLAllocHandle,SQL_HANDLE_ENV, RetCode=%d", RetCode);
		return FALSE;
	}

	// 设置环境属性 https://msdn.microsoft.com/zh-cn/library/ms403305.aspx
	RetCode = SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
	if (RetCode != SQL_SUCCESS && RetCode != SQL_SUCCESS_WITH_INFO)
	{
		LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"设置环境属性失败:SQLSetEnvAttr, RetCode=%d", RetCode);
		return FALSE;
	}
	
	// 分配连接
	RetCode = SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &ms_hdbc);
	if (RetCode != SQL_SUCCESS && RetCode != SQL_SUCCESS_WITH_INFO)
	{
		LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"分配连接失败:SQLAllocHandle, RetCode=%d", RetCode);
		return FALSE;
	}

	// 创建数据库连接
	RetCode = SQLConnectW(ms_hdbc, wszDns, SQL_NTS, wszUser, SQL_NTS, wszPass, SQL_NTS);
	if (RetCode != SQL_SUCCESS && RetCode != SQL_SUCCESS_WITH_INFO)
	{
		LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"创建数据库连接失败:SQLConnectW,RetCode=%X", RetCode);
		PrintSQLErrText(SQL_HANDLE_DBC, ms_hdbc, FALSE);
		return FALSE;
	}
	return TRUE;
}

VOID CDataBaseMdel::Clear()
{
	// 释放语句句柄
	if (ms_hstmt != SQL_NULL_HSTMT)
	{
		SQLCloseCursor(ms_hstmt);
		SQLFreeHandle(SQL_HANDLE_STMT, ms_hstmt);
		ms_hstmt = SQL_NULL_HSTMT;
	}

	// 断开数据库连接
	if (ms_hdbc != SQL_NULL_HDBC)
	{
		SQLDisconnect(ms_hdbc);
		SQLFreeHandle(SQL_HANDLE_DBC, ms_hdbc);
		ms_hdbc = SQL_NULL_HDBC;
	}

	if (m_hEnv != SQL_NULL_HENV)
	{
		SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
		m_hEnv = SQL_NULL_HENV;
	}
	
}

BOOL CDataBaseMdel::GetCardList(_Out_ std::deque<CardInfo>& vlst)
{
	Lock();
	if (!OpenSQLCon())
	{
		LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"OpenSQLCon Faild!");
		UnLock();
		return FALSE;
	}


	RETCODE RetCode = SQLAllocHandle(SQL_HANDLE_STMT, ms_hdbc, &ms_hstmt);
	if (RetCode != SQL_SUCCESS && RetCode != SQL_SUCCESS_WITH_INFO)
	{
		LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"SQLAllocHandle SQL_HANDLE_STMT失败! RetCode=%X", RetCode);
		UnLock();
		return FALSE;
	}
	
	WCHAR wszSQL[MAX_PATH];
	swprintf_s(wszSQL, _countof(wszSQL) - 1, L"exec Proc_GetAllCard");

	
	RetCode = SQLExecDirectW(ms_hstmt, (SQLWCHAR*)wszSQL, SQL_NTS);
	if (RetCode != SQL_SUCCESS && RetCode != SQL_SUCCESS_WITH_INFO)
	{
		LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"SQLExecDirectW 失败! RetCode=%X", RetCode);
		PrintSQLErrText(SQL_HANDLE_STMT, ms_hstmt, TRUE);
		UnLock();
		return FALSE;
	}

	// 特么的必须设置存储过程的SET NOCOUNT ON,否则结果不能返回~!
	while (SQLFetch(ms_hstmt) == SQL_SUCCESS)
	{
		tmpCard tmpCard_;
		SQLGetData(ms_hstmt, 1, SQL_C_WCHAR, tmpCard_.wszCardNo, 64, NULL);
		SQLGetData(ms_hstmt, 2, SQL_C_WCHAR, tmpCard_.wszSurplusHour, 64, NULL);
		SQLGetData(ms_hstmt, 3, SQL_C_WCHAR, tmpCard_.wszActiveDate, 64, NULL);
		

		CardInfo Card;
		Card.wstrCardNo = tmpCard_.wszCardNo;
		if (Card.wstrCardNo == L"")
			continue;

		Card.dwCardCrc = CLPublic::GetCRC32_DWORD(tmpCard_.wszCardNo);
		Card.nSurplusHour = _wtoi(tmpCard_.wszSurplusHour);
		Card.nMaxHour = _wtoi(std::wstring(tmpCard_.wszCardNo).substr(0, 4).c_str());
		Card.wstrActiveTime = tmpCard_.wszActiveDate;
		Card.dwToken = NULL;
		Card.bExit = FALSE;
		Card.wstrIp = L"0.0.0.0";
		Card.dwTick64 = ::GetTickCount64();
		Card.ulKeepALiveTick = ::GetTickCount64();
		vlst.push_back(Card);
	}
	UnLock();
	return TRUE;
}


BOOL CDataBaseMdel::GetClientConfigList(_In_ std::deque<tmpClientConfig>& vlst)
{
	Lock();
	if (!OpenSQLCon())
	{
		LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"OpenSQLCon Faild!");
		UnLock();
		return FALSE;
	}


	RETCODE RetCode = SQLAllocHandle(SQL_HANDLE_STMT, ms_hdbc, &ms_hstmt);
	if (RetCode != SQL_SUCCESS)
	{
		LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"SQLAllocHandle SQL_HANDLE_STMT失败! RetCode=%X", RetCode);
		UnLock();
		return FALSE;
	}

	WCHAR wszSQL[MAX_PATH];
	swprintf_s(wszSQL, _countof(wszSQL) - 1, L"exec proc_InitConfig");


	RetCode = SQLExecDirectW(ms_hstmt, (SQLWCHAR*)wszSQL, SQL_NTS);
	if (RetCode != SQL_SUCCESS && RetCode != SQL_SUCCESS_WITH_INFO)
	{
		LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"SQLExecDirectW 失败! RetCode=%X, SQL=%s", RetCode, wszSQL);
		PrintSQLErrText(SQL_HANDLE_STMT, ms_hstmt, TRUE);
		UnLock();
		return FALSE;
	}

	// 特么的必须设置存储过程的SET NOCOUNT ON,否则结果不能返回~!
	while (SQLFetch(ms_hstmt) == SQL_SUCCESS)
	{
		tmpClientConfig tmpClientConfig_;

		WCHAR wszText[MAX_PATH] = { 0 };
		SQLGetData(ms_hstmt, 1, SQL_C_WCHAR, wszText, _countof(wszText) - 1, NULL);
		tmpClientConfig_.wsAccountName = wszText;

		SQLGetData(ms_hstmt, 2, SQL_C_WCHAR, wszText, _countof(wszText) - 1, NULL);
		tmpClientConfig_.wsPlayerName = wszText;

		SQLGetData(ms_hstmt, 3, SQL_C_WCHAR, wszText, _countof(wszText) - 1, NULL);
		tmpClientConfig_.wsConfigName = wszText;

		SQLGetData(ms_hstmt, 4, SQL_C_WCHAR, wszText, _countof(wszText) - 1, NULL);
		tmpClientConfig_.wsConfigValue = wszText;

		vlst.push_back(tmpClientConfig_);
	}
	UnLock();
	return TRUE;
}


BOOL CDataBaseMdel::GetCardInfo(_Out_ CardInfo& Card)
{
	Lock();
	if (!OpenSQLCon())
	{
		LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"OpenSQLCon Faild!");
		UnLock();
		return FALSE;
	}

	WCHAR wszSQL[MAX_PATH];
	swprintf_s(wszSQL, _countof(wszSQL) - 1, L"exec Proc_GetCard '%08X'", Card.dwCardCrc);

	RETCODE RetCode = SQLAllocHandle(SQL_HANDLE_STMT, ms_hdbc, &ms_hstmt);
	if (RetCode != SQL_SUCCESS)
	{
		LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"SQLAllocHandle SQL_HANDLE_STMT失败! RetCode=%X", RetCode);
		UnLock();
		return FALSE;
	}

	RetCode = SQLExecDirectW(ms_hstmt, (SQLWCHAR*)wszSQL, SQL_NTS);
	if (RetCode != SQL_SUCCESS && RetCode != SQL_SUCCESS_WITH_INFO)
	{
		LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"SQLExecDirectW 失败! RetCode=%X, SQL=%s", RetCode, wszSQL);
		PrintSQLErrText(SQL_HANDLE_STMT, ms_hstmt, TRUE);
		UnLock();
		return FALSE;
	}

	// 特么的必须设置存储过程的SET NOCOUNT ON,否则结果不能返回~!
	if (SQLFetch(ms_hstmt) == SQL_SUCCESS)
	{
		tmpCard tmpCard_;
		SQLGetData(ms_hstmt, 1, SQL_C_WCHAR, tmpCard_.wszCardNo, 64, NULL);
		SQLGetData(ms_hstmt, 2, SQL_C_WCHAR, tmpCard_.wszSurplusHour, 64, NULL);
		SQLGetData(ms_hstmt, 3, SQL_C_WCHAR, tmpCard_.wszActiveDate, 64, NULL);

		Card.wstrCardNo = tmpCard_.wszCardNo;
		if (Card.wstrCardNo != L"")
		{
			Card.dwCardCrc = CLPublic::GetCRC32_DWORD(tmpCard_.wszCardNo);
			Card.nSurplusHour = _wtoi(tmpCard_.wszSurplusHour);
			Card.nMaxHour = _wtoi(std::wstring(tmpCard_.wszCardNo).substr(0, 4).c_str());
			Card.wstrActiveTime = tmpCard_.wszActiveDate;
			Card.dwToken = NULL;
			Card.bExit = FALSE;
			Card.wstrIp = L"0.0.0.0";
			Card.dwTick64 = ::GetTickCount64();
			Card.ulKeepALiveTick = ::GetTickCount64();
		}
	}
	UnLock();
	return TRUE;
}

BOOL CDataBaseMdel::ExcuteSQL(LPCWSTR pwszSQL, _Out_ LPWSTR pwszMsg)
{
	Lock();
	if (!OpenSQLCon())
	{
		LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"OpenSQLCon失败! SQL=%s", pwszSQL);
		UnLock();
		return FALSE;
	}

	RETCODE RetCode = SQLAllocHandle(SQL_HANDLE_STMT, ms_hdbc, &ms_hstmt);
	if (RetCode != SQL_SUCCESS)
	{
		LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"SQLAllocHandle SQL_HANDLE_STMT失败! RetCode=%X", RetCode);
		UnLock();
		return FALSE;
	}

	WCHAR wszSQL[MAX_PATH];
	CCharacter::wstrcpy_my(wszSQL, pwszSQL, _countof(wszSQL) - 1);

	RetCode = SQLExecDirectW(ms_hstmt, wszSQL, SQL_NTS);
	if (RetCode != SQL_SUCCESS && RetCode != SQL_SUCCESS_WITH_INFO)
	{
		LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"SQLExecDirectW 失败! RetCode=%X, SQL=%s", RetCode, wszSQL);
		PrintSQLErrText(SQL_HANDLE_STMT, ms_hstmt, TRUE);
		UnLock();
		return FALSE;
	}

	BOOL bRetCode = FALSE;
	if (SQLFetch(ms_hstmt) == SQL_SUCCESS)
	{
		SQLGetData(ms_hstmt, 1, SQL_C_WCHAR, pwszMsg, MAX_PATH, NULL);
		bRetCode = TRUE;
	}

	UnLock();
	return bRetCode;
}

BOOL CDataBaseMdel::InsertLoginLog(_In_ CONST std::wstring& wsCardNo, _In_ CONST std::wstring& wsIp, _In_ CONST std::wstring& wsAccountName, _In_ CONST std::wstring& wsAccountPass)
{
	WCHAR wszText[MAX_PATH];

	swprintf_s(wszText, _countof(wszText) - 1, L"exec InsertLoginLog '%s','%s','%s','%s'", wsCardNo.c_str(), wsIp.c_str(), wsAccountName.c_str(), wsAccountPass.c_str());
	return DoSQL([&wszText](std::queue<std::wstring>& vlst){vlst.push(wszText); });
}

DWORD WINAPI CDataBaseMdel::_ExcuteSQLThread(LPVOID lpParm)
{
	std::wstring wsSQL;
	BOOL bExist = FALSE;
	WCHAR wszText[MAX_PATH];
	for (;;)
	{
		DoSQL([&bExist, &wsSQL](std::queue<std::wstring>& vlst)
		{
			if (vlst.size() > 0)
			{
				bExist = TRUE;
				wsSQL = vlst.front();
				vlst.pop();
			}
			else
				bExist = FALSE;
		});

		if (!bExist)
		{
			::Sleep(100);
			continue;
		}
		ExcuteSQL(wsSQL.c_str(), wszText);
	}
	return 0;
}

BOOL CDataBaseMdel::DoSQL(_In_ std::function<VOID(std::queue<std::wstring>&)> fn)
{;
	static std::queue<std::wstring> vlst;
	static CLLock Lock(L"CDataBaseMdel::DoSQL");
	
	return Lock.Access([&fn]{fn(vlst); });
}

BOOL CDataBaseMdel::GetOTPAccountList(_Out_ std::deque<OTPAccountContent>& vlst)
{
	Lock();
	if (!OpenSQLCon())
	{
		LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"OpenSQLCon Faild!");
		UnLock();
		return FALSE;
	}


	RETCODE RetCode = SQLAllocHandle(SQL_HANDLE_STMT, ms_hdbc, &ms_hstmt);
	if (RetCode != SQL_SUCCESS && RetCode != SQL_SUCCESS_WITH_INFO)
	{
		LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"SQLAllocHandle SQL_HANDLE_STMT失败! RetCode=%X", RetCode);
		UnLock();
		return FALSE;
	}

	WCHAR wszSQL[MAX_PATH];
	swprintf_s(wszSQL, _countof(wszSQL) - 1, L"exec GetOTPAccountContent");


	RetCode = SQLExecDirectW(ms_hstmt, (SQLWCHAR*)wszSQL, SQL_NTS);
	if (RetCode != SQL_SUCCESS && RetCode != SQL_SUCCESS_WITH_INFO)
	{
		LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"SQLExecDirectW 失败! RetCode=%X, SQL=%s", RetCode, wszSQL);
		PrintSQLErrText(SQL_HANDLE_STMT, ms_hstmt, TRUE);
		UnLock();
		return FALSE;
	}

	struct tmpOTPAccountContent
	{
		WCHAR wszAccountName[64];
		WCHAR wszAccountKey[64];
	};

	// 特么的必须设置存储过程的SET NOCOUNT ON,否则结果不能返回~!
	while (SQLFetch(ms_hstmt) == SQL_SUCCESS)
	{
		tmpOTPAccountContent tmpOTPAccountContent_;
		SQLGetData(ms_hstmt, 1, SQL_C_WCHAR, tmpOTPAccountContent_.wszAccountName, 64 * sizeof(WCHAR), NULL);
		SQLGetData(ms_hstmt, 2, SQL_C_WCHAR, tmpOTPAccountContent_.wszAccountKey, 64 * sizeof(WCHAR), NULL);

		OTPAccountContent OTPAccountContent_;
		OTPAccountContent_.wsAccountName = tmpOTPAccountContent_.wszAccountName;
		OTPAccountContent_.wsAccountKey = tmpOTPAccountContent_.wszAccountKey;
		vlst.push_back(std::move(OTPAccountContent_));
	}
	UnLock();
	return TRUE;
}

BOOL CDataBaseMdel::AsyncExcuteSQL(_In_ CONST std::wstring& wsSQLText)
{
	return DoSQL([wsSQLText](std::queue<std::wstring>& vlst){ vlst.push(wsSQLText); });
}

