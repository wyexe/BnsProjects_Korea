#ifndef __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVVER_OTP_BNSDLGLOGINOTP_H__
#define __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVVER_OTP_BNSDLGLOGINOTP_H__

#include <MyTools/ClassInstance.h>

// 以下是dlg界面注入DLL后, 直接用服务器计算出OTP来测试登录用的
struct Login_Share_Info;
class CBnsDlgLoginOTP
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
	CBnsDlgLoginOTP() = default;
	~CBnsDlgLoginOTP() = default;

	// 开始运行
	VOID Run();
private:
	// 读取'Account.txt'
	VOID ReadAccountFile();

	// 初始化内存共享
	VOID InitShareMemory();

	// 设置内容
	VOID SetShareMemory();

	// 不断更新帐号的OTP的线程
	static DWORD WINAPI _WorkThread(_In_ LPVOID lpParm);
private:
	std::vector<LoginAccountContent> _VecLoginAccountContent;
	Login_Share_Info* _pLoginShareInfo;
};



#endif // !__GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVVER_OTP_BNSDLGLOGINOTP_H__
