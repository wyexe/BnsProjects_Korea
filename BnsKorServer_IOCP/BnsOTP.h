#ifndef __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_OTP_BNSOTP_H__
#define __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_OTP_BNSOTP_H__

#include <MyTools/ClassInstance.h>
#include "ServerCommon.h"

class CBnsOTP : public MyTools::CClassInstance<CBnsOTP>
{
private:
	struct OTPContent
	{
		std::wstring wsAccountName;
		std::wstring wsAccountKey;

		OTPContent() {}
		OTPContent(_In_ CONST std::wstring& wsAccountName_, _In_ CONST std::wstring& wsAccountKey_) : wsAccountName(wsAccountName_), wsAccountKey(wsAccountKey_) {}
		OTPContent(_In_ std::wstring&& wsAccountName_, _In_ std::wstring&& wsAccountKey_) : wsAccountName(std::move(wsAccountName_)), wsAccountKey(std::move(wsAccountKey_)) {}
	};
public:
	CBnsOTP() : _Lock(L"CBnsOTP.Lock") {};
	~CBnsOTP() = default;

	// �����ݿ��ȡ�������ʺŵ�OTP
	BOOL Initialize();

	// ��OTP.txt��ȡ OTP�ʺ� ���浽���ݿ�
	BOOL ReadOTP_In_File_To_Db();

	// ����Account����OTP��KEY
	BOOL FindOTPKey_By_AccountName(_In_ CONST std::wstring& wsAccountName, _Out_opt_ std::wstring& wsAccountKey) CONST;

	// ����OTP
	BOOL CalculateOTP(_In_ CONST std::wstring& wsAccountKey, _Out_opt_ DWORD& dwKey) CONST;
private:
	// ��ȡ'OTP.txt'
	std::vector<OTPContent> ReadOTPFile() CONST;

private:
	MyTools::CLLock _Lock;
	std::map<std::wstring, std::wstring> _MapOTP;
};



#endif // !__GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_OTP_BNSOTP_H__
