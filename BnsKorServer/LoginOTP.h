#ifndef __GITBNSPROJECTS_BNSKORSERVER_OTP_LOGINOTP_H__
#define __GITBNSPROJECTS_BNSKORSERVER_OTP_LOGINOTP_H__

#include <MyTools/ClassInstance.h>

struct Login_Share_Info;
class CLoginOTP : public CClassInstance<CLoginOTP>
{
private:
	struct LoginAccountContent
	{
		std::wstring wsPlayerName;
		std::wstring wsAccountName;
		std::wstring wsAccountPass;
		std::wstring wsKey;
	};
public:
	CLoginOTP() = default;
	~CLoginOTP() = default;

	VOID Run();
private:
	VOID ReadAccountFile();

	VOID InitShareMemory();

	VOID SetShareMemory();

	static DWORD WINAPI _WorkThread(_In_ LPVOID lpParm);
private:
	std::vector<LoginAccountContent> _VecLoginAccountContent;
	Login_Share_Info* _pLoginShareInfo;
};



#endif // !__GITBNSPROJECTS_BNSKORSERVER_OTP_LOGINOTP_H__
