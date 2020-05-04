#ifndef __GITBNSPROJECTS_BNSKORSERVER_OTP_ONETIMEPASSWORD_H__
#define __GITBNSPROJECTS_BNSKORSERVER_OTP_ONETIMEPASSWORD_H__

#include "DataBaseModel.h"
#include <deque>
#include <MyTools/CLLock.h>

class ByteBuffer;
class COneTimePassword : public virtual CClassInstance<COneTimePassword>
{
private:
	struct ThreadFunPtrInfo
	{
		HANDLE			 hEvent;
		std::wstring	 wsAccountName;
		std::wstring     wsKey;
		std::wstring	 wsValue;
	};
public:
	COneTimePassword();
	~COneTimePassword();

	// ��ֹʵ����
	COneTimePassword(CONST COneTimePassword&) = delete;
	COneTimePassword& operator = (CONST COneTimePassword&) = delete;

	// ���ļ���ȡOTP�ʺ� -> ���ݿ�
	BOOL ReadFileToDB();

	// �����ݿ����¶�ȡ���ڴ�
	VOID RefreshDBToMemory();

	// �Ƿ���� ��OTP
	BOOL FindOTPAccount(_In_ CONST std::wstring& wsAccountName, _Out_opt_ CDataBaseMdel::OTPAccountContent& OtpAccountContent_);

	// ����OTP
	BOOL CalcOTP(_In_ CONST std::wstring& wsAccountName, _Out_ std::wstring& wsKey);

	// 
	std::wstring CalcOPTValue(_In_ CONST std::wstring& wsAccountName, _In_ CONST std::wstring& wsKey);

	// ��ȡOTP
	VOID GetOTPKey(_In_ CONST std::wstring& wsAccountName, _Out_ ByteBuffer& ByteBuffer_);
private:

	static DWORD WINAPI _WokrThread(LPVOID lpParm);

	BOOL GetOTPValue_In_Java(_In_ ThreadFunPtrInfo* pThreadFunPtrInfo) CONST;
private:
	std::deque<CDataBaseMdel::OTPAccountContent> VecOTPAccount;
	CLLock OTPAccountLock;

	std::queue<ThreadFunPtrInfo*> QueueOTP;
	CLLock LockQueueOTP;

	std::wstring wsJvmDLLPath;
};



#endif // !__GITBNSPROJECTS_BNSKORSERVER_OTP_ONETIMEPASSWORD_H__
