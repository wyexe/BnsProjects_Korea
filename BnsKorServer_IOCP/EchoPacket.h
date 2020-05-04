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

	// 查询卡号剩余的时间
	BOOL QueryCard(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// 心跳
	BOOL KeepALive(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// 下载主线脚本
	BOOL DownloadScript(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// 游戏登录
	BOOL GameLogin(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// 公告
	BOOL Announcement(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// 主线组队
	BOOL JoinTeam_MainLine(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// 离队
	BOOL LeaveTeam_MainLine(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// 副本组队
	BOOL JoinTeam_Gungoen(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// 副本离队
	BOOL LeaveTeam_Gungoen(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// 物资(小号)
	BOOL ItemSupport_GameAccount(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// 收货(小号)
	BOOL PickItem_GameAccount(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// 物资(仓库号)
	BOOL ItemSupport_Warehouse(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// 收货(仓库号)
	BOOL PickItem_Warehouse(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// 修改副本进度
	BOOL SetGungoenSchedule(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// 获取副本进度
	BOOL GetGungoenSchedule(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// 设置队伍只读
	BOOL SetTeamReadonly(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// 交易完毕的数据(保存交易物品和金币到数据库)
	BOOL SaveTradeData(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// 游戏登录完毕的消息
	BOOL SaveLoginData(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// 封号的消息
	BOOL Banned(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// 获取最少人的频道
	BOOL GetLessPersonChannel(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// 获取云配置的值
	BOOL GetCouldConfigValue(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// 设置云配置的值
	BOOL SetCouldConfigValue(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// 控制台的心跳
	BOOL ConsoleKeepALive(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// AccountLog的日志
	BOOL SaveAccountLogData(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// OTP
	BOOL CalculateOTP(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// 查询日常任务的进度
	BOOL GetDailyTaskSchedule(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// 修改日常任务进度
	BOOL SetDailyTaskSchedule(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	//  校验卡号是否正常(心跳或者其他函数检查卡号时间)
	BOOL CheckCard(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// 跨服组队
	BOOL JoinCrossAreaTeam(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// 设置跨服组队的内容
	BOOL SetCrossAreaTeamContent(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// 跨服组队离队
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
