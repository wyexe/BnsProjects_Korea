#include "OneTimePassword.h"
#include <iostream>
#include "CLHmacSha1.h"
#include <MyTools/CLFile.h>
#include <MyTools/ByteBuffer.h>
#include <MyTools/CLResManager.h>

#define _SELF L"OneTimePassword.cpp"

COneTimePassword::COneTimePassword() : OTPAccountLock(L"COneTimePassword::OTPAccountLock"), LockQueueOTP(L"COneTimePassword::LockQueueOTP")
{
	auto hWorkThread = cbBEGINTHREADEX(NULL, NULL, _WokrThread, this, NULL, NULL);
	SetResDeleter(hWorkThread, [](HANDLE& hThread){::CloseHandle(hThread); });
}

COneTimePassword::~COneTimePassword()
{
	
}

BOOL COneTimePassword::ReadFileToDB()
{
	WCHAR wszPath[MAX_PATH] = { 0 };
	::GetCurrentDirectoryW(MAX_PATH, wszPath);
	lstrcatW(wszPath, L"\\OTP.txt");
	
	std::wstring wsContent;
	CLFile::ReadUnicodeFile(wszPath, wsContent);
	
	// Split Account
	std::vector<CDataBaseMdel::OTPAccountContent> tmpVecOTPAccount;
	std::vector<std::wstring> VecAccountText;
	CCharacter::Split(wsContent, L"\r\n", VecAccountText, Split_Option_RemoveEmptyEntries | Split_Option_KeepOnly);
	for (CONST auto& itm : VecAccountText)
	{
		std::vector<std::wstring> VecParm;
		UINT uSize = CCharacter::Split(itm, L",", VecParm, Split_Option_RemoveEmptyEntries | Split_Option_KeepOnly);
		if (uSize != 2)
		{
			std::wcout << L"无法识别OTP.txt的帐号格式:" << itm.c_str() << L", 将忽略这个!" << std::endl;
			continue;
		}

		CDataBaseMdel::OTPAccountContent OTPAccountContent_;
		OTPAccountContent_.wsAccountName = CCharacter::Trim_W(VecParm.at(0));
		OTPAccountContent_.wsAccountKey = CCharacter::Trim_W(VecParm.at(1));
		tmpVecOTPAccount.push_back(std::move(OTPAccountContent_));
	}

	std::wcout << L"读取到的OTP帐号一共有:" << tmpVecOTPAccount.size() << L"个, 准备存往数据库……" << std::endl;
	WCHAR wszRetText[MAX_PATH];
	WCHAR wszSQLText[MAX_PATH];

	for (CONST auto& itm : tmpVecOTPAccount)
	{
		swprintf_s(wszSQLText, _countof(wszSQLText) - 1, L"exec InsertOTPAccountContent '%s','%s'", itm.wsAccountName.c_str(), itm.wsAccountKey.c_str());

		ZeroMemory(wszRetText, sizeof(wszRetText));
		if (!CDataBaseMdel::ExcuteSQL(wszSQLText, wszRetText))
		{
			LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"执行保存OTP帐号失败,Text=%s", wszRetText);
			return FALSE;
		}
	}

	CLFile::WriteUnicodeFile(wszPath, L"");

	//if (CLPublic::FileExist(wszPath))
	//	::DeleteFileW(wszPath);

	RefreshDBToMemory();
	return TRUE;
}

VOID COneTimePassword::RefreshDBToMemory()
{
	std::wcout << L"正在重新从数据库读取OTP帐号" << std::endl;
	OTPAccountLock.Access([this]
	{
		VecOTPAccount.clear();
		CDataBaseMdel::GetOTPAccountList(VecOTPAccount);
	});
	std::wcout << L"读取OTP帐号完毕!" << std::endl;
}

BOOL COneTimePassword::FindOTPAccount(_In_ CONST std::wstring& wsAccountName, _Out_opt_ CDataBaseMdel::OTPAccountContent& OtpAccountContent_)
{
	BOOL bExist = FALSE;
	OTPAccountLock.Access([&bExist, this, wsAccountName, &OtpAccountContent_]
	{
		auto p = CLPublic::Deque_find_if(VecOTPAccount, [wsAccountName](CONST CDataBaseMdel::OTPAccountContent& OtpAccount){ return OtpAccount.wsAccountName == wsAccountName; });
		if (p != nullptr)
		{
			bExist = TRUE;
			OtpAccountContent_ = *p;
		}
	});
	return bExist;
}

BOOL COneTimePassword::CalcOTP(_In_ CONST std::wstring& wsAccountName, _Out_ std::wstring& wsKey)
{
	CDataBaseMdel::OTPAccountContent OtpAccountContent_;
	if (!FindOTPAccount(wsAccountName, OtpAccountContent_))
		return FALSE;

	wsKey = CalcOPTValue(wsAccountName, OtpAccountContent_.wsAccountKey);
	return !wsKey.empty();
}

std::wstring COneTimePassword::CalcOPTValue(_In_ CONST std::wstring& wsAccountName, _In_ CONST std::wstring& wsKey)
{
	ThreadFunPtrInfo ThreadFunPtrInfo_;
	ThreadFunPtrInfo_.wsAccountName = wsAccountName;
	ThreadFunPtrInfo_.wsKey = wsKey;
	ThreadFunPtrInfo_.hEvent = ::CreateEventW(NULL, FALSE, FALSE, NULL);
	LockQueueOTP.Access([&ThreadFunPtrInfo_, this]{ QueueOTP.push(&ThreadFunPtrInfo_); });
	if (::WaitForSingleObject(ThreadFunPtrInfo_.hEvent, 10 * 1000) == WAIT_TIMEOUT)
		return L"";

	return ThreadFunPtrInfo_.wsValue;
}

VOID COneTimePassword::GetOTPKey(_In_ CONST std::wstring& wsAccountName, _Out_ ByteBuffer& ByteBuffer_)
{
	std::wstring wsKey;
	if (CalcOTP(wsAccountName, wsKey))
		ByteBuffer_ << SOCK_MSG_OTP << wsAccountName << TRUE << wsKey;
	else
		ByteBuffer_ << SOCK_MSG_OTP << wsAccountName << FALSE;
}

DWORD WINAPI COneTimePassword::_WokrThread(LPVOID lpParm)
{
	COneTimePassword* pthis = reinterpret_cast<COneTimePassword*>(lpParm);

	ThreadFunPtrInfo* pThreadFunPtrInfo = nullptr;
	for (;;)
	{
		Sleep(50);

		pThreadFunPtrInfo = nullptr;
		pthis->LockQueueOTP.Access([&pthis, &pThreadFunPtrInfo]
		{
			if (pthis->QueueOTP.empty())
				return;

			pThreadFunPtrInfo = pthis->QueueOTP.front();
			pthis->QueueOTP.pop();
		});
		if (pThreadFunPtrInfo == nullptr)
			continue;

		for (int i = 0; i < 10; ++i)
		{
			if (pthis->GetOTPValue_In_Java(pThreadFunPtrInfo))
				break;

			LOG_CF_E(L"[%s]计算OTP失败, 第%d次重新计算", pThreadFunPtrInfo->wsAccountName.c_str(), i);
		}
		::SetEvent(pThreadFunPtrInfo->hEvent);
	}
}

BOOL COneTimePassword::GetOTPValue_In_Java(_In_ ThreadFunPtrInfo* pThreadFunPtrInfo) CONST
{
	CLHmacSha1::GetInstance().CalcOTPValue(pThreadFunPtrInfo->wsKey, pThreadFunPtrInfo->wsValue);
	return TRUE;
}

