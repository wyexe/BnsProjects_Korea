#ifndef __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVVER_OTP_BNSDLGLOGINOTP_H__
#define __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVVER_OTP_BNSDLGLOGINOTP_H__

#include <MyTools/ClassInstance.h>

// ������dlg����ע��DLL��, ֱ���÷����������OTP�����Ե�¼�õ�
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

	// ��ʼ����
	VOID Run();
private:
	// ��ȡ'Account.txt'
	VOID ReadAccountFile();

	// ��ʼ���ڴ湲��
	VOID InitShareMemory();

	// ��������
	VOID SetShareMemory();

	// ���ϸ����ʺŵ�OTP���߳�
	static DWORD WINAPI _WorkThread(_In_ LPVOID lpParm);
private:
	std::vector<LoginAccountContent> _VecLoginAccountContent;
	Login_Share_Info* _pLoginShareInfo;
};



#endif // !__GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVVER_OTP_BNSDLGLOGINOTP_H__
