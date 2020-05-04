#ifndef __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_SOCKET_ECHOPACKET_ECHOPACKET_H__
#define __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_SOCKET_ECHOPACKET_ECHOPACKET_H__

#include <MyTools/ClassInstance.h>
#include <MyTools/SocketBuffer.h>

class CGameClient;
struct ClientTeamInfo;
struct VerItemSupport;
class CEchoPacket : public MyTools::CClassInstance<CEchoPacket>
{
public:
	CEchoPacket() : _bBanned(FALSE) {};
	~CEchoPacket() = default;

	// ��ѯ����ʣ���ʱ��
	BOOL QueryCard(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// ����
	BOOL KeepALive(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// �������߽ű�
	BOOL DownloadScript(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// ��Ϸ��¼
	BOOL GameLogin(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// ����
	BOOL Announcement(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// �������
	BOOL JoinTeam_MainLine(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// ���
	BOOL LeaveTeam_MainLine(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// �������
	BOOL JoinTeam_Gungoen(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// �������
	BOOL LeaveTeam_Gungoen(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// ����(С��)
	BOOL ItemSupport_GameAccount(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// �ջ�(С��)
	BOOL PickItem_GameAccount(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// ����(�ֿ��)
	BOOL ItemSupport_Warehouse(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// �ջ�(�ֿ��)
	BOOL PickItem_Warehouse(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// �޸ĸ�������
	BOOL SetGungoenSchedule(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// ��ȡ��������
	BOOL GetGungoenSchedule(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// ���ö���ֻ��
	BOOL SetTeamReadonly(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// ������ϵ�����(���潻����Ʒ�ͽ�ҵ����ݿ�)
	BOOL SaveTradeData(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// ��Ϸ��¼��ϵ���Ϣ
	BOOL SaveLoginData(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// ��ŵ���Ϣ
	BOOL Banned(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// ��ȡ�����˵�Ƶ��
	BOOL GetLessPersonChannel(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// ��ȡ�����õ�ֵ
	BOOL GetCouldConfigValue(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// ���������õ�ֵ
	BOOL SetCouldConfigValue(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// ����̨������
	BOOL ConsoleKeepALive(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// AccountLog����־
	BOOL SaveAccountLogData(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// OTP
	BOOL CalculateOTP(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// ��ѯ�ճ�����Ľ���
	BOOL GetDailyTaskSchedule(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// �޸��ճ��������
	BOOL SetDailyTaskSchedule(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	//  У�鿨���Ƿ�����(������������������鿨��ʱ��)
	BOOL CheckCard(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// ������
	BOOL JoinCrossAreaTeam(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// ���ÿ����ӵ�����
	BOOL SetCrossAreaTeamContent(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// ���������
	BOOL LeaveCrossAreaTeam(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	//
	BOOL QueryAccountVirtualSeriaNumber(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;
private:
	VOID ExtractTeamPacket(_In_ MyTools::CLSocketBuffer* pSocketBuffer, _Out_ std::shared_ptr<ClientTeamInfo> CtiTeam) CONST;

	VOID ExtractClientTradePacket(_In_ MyTools::CLSocketBuffer* pSocketBuffer, _Out_ VerItemSupport& ItemSupport) CONST;
private:
	template<typename T>
	static T ExtractPacket(_In_ std::function<T()> MethodPtr)
	{
		return MethodPtr();
	}

private:
	mutable BOOL _bBanned;
};



#endif // !__GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_SOCKET_ECHOPACKET_ECHOPACKET_H__
