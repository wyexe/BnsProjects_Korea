#ifndef __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_COMMON_SERVERCOMMON_H__
#define __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_COMMON_SERVERCOMMON_H__

#include <set>
#include <MyTools/ToolsPublic.h>
#include <winsock2.h>

#define SZFILE_NAME_LOGIN_SHAREINFO					L"BNS_KOR_Login_Share_Info"

enum em_Sock_Msg
{
	em_Sock_Msg_ErrorText                         = 0x0,
	em_Sock_Msg_RECHARGECARD                      = 0x17,
	em_Sock_Msg_QUERY_CARDINFO                    = 0x2,
	em_Sock_Msg_KEEPALIVE                         = 0x3,
	em_Sock_Msg_QUERY_SCRIPTKEY                   = 0x4,
	em_Sock_Msg_CLIENTMSG                         = 0x1,
	em_Sock_Msg_TITLE                             = 0x6,
	em_Sock_Msg_GET_TASKSCRIPT                    = 0x7,
	em_Sock_Msg_GET_NEXT_TASKSCRIPT               = 0x8,
	em_Sock_Msg_LOGINGAME                         = 0x9,
	em_Sock_Msg_GETRC4_KEY                        = 0xA,
	em_Sock_Msg_JOIN_TEAM_ORDER                   = 0xB,
	em_Sock_Msg_LEAVE_TEAM_ORDER                  = 0xC,
	em_Sock_Msg_UPDATE_TEAM_ORDER                 = 0xD,
	em_Sock_Msg_DISSOLUTION_TEAM                  = 0xE,
	em_Sock_Msg_CLIENT_ITEMSUPPORT                = 0xF,
	em_Sock_Msg_CLIENT_WAREHOUSE_MAINTASK         = 0x10,
	em_Sock_Msg_CLIENT_WAREHOUSE_PICKITEM         = 0x11,
	em_Sock_Msg_CLIENT_PICKITEM                   = 0x12,
	em_Sock_Msg_CLIENT_GET_TEAMMEMBER_TASKINFO    = 0x13,
	em_Sock_Msg_CLIENT_CREATE_TEAMMEMBER_TASKINFO = 0x14,
	em_Sock_Msg_CLIENT_DEALMESSAGE                = 0x15,
	em_Sock_Msg_CLIENT_STOPCLIENT                 = 0x16,
	em_Sock_Msg_CLIENT_LOGINED_LOG                = 0x18,
	em_Sock_Msg_CLIENT_BANNED                     = 0x19,
	em_Sock_Msg_JOIN_DUNGOEN_TEAM                 = 0x1A,
	em_Sock_Msg_DISSOLUTION_DUNGOEN_TEAM          = 0x1B,
	em_Sock_Msg_GET_CHANNEL                       = 0x1C,
	em_Sock_Msg_SET_CONFIG_VALUE                  = 0x1D,
	em_Sock_Msg_GET_CONFIG_VALUE                  = 0x1E,
	em_Sock_Msg_UPDATE_TEAM_SCHEDULE              = 0x1F,
	em_Sock_Msg_GET_TEAM_SCHEDULE                 = 0x20,
	em_Sock_Msg_CONSOLE_KEEPALIVE                 = 0x21,
	em_Sock_Msg_SET_TEAM_READONLY                 = 0x22,
	em_Sock_Msg_SAVE_LOCAL_ACCOUNT_LOG            = 0x23,
	em_Sock_Msg_OTP                               = 0x24,
	em_Sock_Msg_QUERY_DAILYTASK_SCHEDULE          = 0x25,
	em_Sock_Msg_UPDATE_DAILYTASK_SCHEDULE         = 0x26,
	em_Sock_Msg_CROSSAREATEAM					  = 0x27,
	em_Sock_Msg_DELETE_CROSSAREATEAM			  = 0x28,
	em_Sock_Msg_SET_CROSSAREATEAM_CONTENT		  = 0x29,
	em_Sock_Msg_Query_ACCOUNTVIRTUALSERIANUMBER	  = 0x2A,
};

enum em_TeamMeber_Classes : BYTE
{
	// 力士
	em_TeamMeber_Classes_Destroyer = 0x5,
	// 刺客
	em_TeamMeber_Classes_Assassin = 0x7,
	// 剑士
	em_TeamMeber_Classes_BladeMaster = 0x1,
	// 召唤师
	em_TeamMeber_Classes_Summoner = 0x6,
	// 气功
	em_TeamMeber_Classes_ForceMaster = 0x3,
	// 拳师
	em_TeamMeber_Classes_KongFuMaster = 0x2,
	// 灵剑士
	em_TeamMeber_Classes_Danncer = 0x8,
	// 咒术师
	em_TeamMeber_Classes_Warlock = 0x9,
	// 气宗
	em_TeamMeber_Classes_KongFuForceMaster = 0xA,
	// 枪手
	em_TeamMeber_Classes_Gunner = 0x4,
	// 任意职业
	em_TeamMeber_Classes_Any = 0x0
};

struct SocketSession
{
	std::wstring wsClientIp;
	SOCKET sClient;
	SocketSession(_In_ std::wstring wsIp, _In_ SOCKET skClient)
	{
		wsClientIp = wsIp;
		sClient = skClient;
	}
	SocketSession()
	{
		wsClientIp = L"";
		sClient = NULL;
	}
};

struct KeepALiveContent
{
	BYTE Level = NULL;
	DWORD Gold = NULL;
	DWORD dwTaskId = NULL;
	DWORD dwScheduleId = NULL;
	std::wstring wsContent;
};

struct ClientAccountContent
{
	std::wstring wsVersion;		        // 客户端版本号
	std::wstring wsAccountName;         // 帐号
	std::wstring wsAccountPass;         // 密码
	std::wstring wsPlayerName;          // 玩家名称
	std::wstring wsIp;                  // IP
	std::wstring wsCardNo;              // 卡号
	DWORD		 dwToken;               // 令牌
	DWORD		 dwCardCrc;             // 卡号CRC
	ULONGLONG    ulKeepALiveTick;       // 心跳(判断是否断开连接)
	KeepALiveContent KeepALiveContent_; // 客户端心跳包的内容
	ClientAccountContent()
	{
		wsAccountName = wsAccountPass = wsCardNo = wsPlayerName = wsIp = L"Empty";
		dwCardCrc = dwToken = NULL;
		ulKeepALiveTick = NULL;
	}
};

struct SocketClientInfo : public std::enable_shared_from_this<SocketClientInfo>
{
	SocketSession	     SocketSession_;
	ClientAccountContent ClientAccountContent_;
	ULONGLONG		     ulKeepALiveTick;
	BOOL			     bRecving;
	ULONGLONG		     ulRecvTick;
	ULONGLONG		     ulHash;
	BOOL			     bClose;
};

struct WareHouseInfo
{
	std::wstring wsWareHouseName;
	DWORD	dwChannel;
	DWORD	dwServerId;
	ULONGLONG ulTick;
	WareHouseInfo()
	{
		dwChannel = dwServerId = NULL;
		ulTick = ::GetTickCount64();
	}
};

enum em_Deal_Warehouse_Status : DWORD
{
	em_Deal_Warehouse_Status_None,
	em_Deal_Warehouse_Status_Channel
};

enum em_Trade_Warehouse_Type : DWORD
{
	// 物资 -> (仓库号给小号)
	em_Trade_Warehouse_Type_ItemSupport,

	// 交易 -> (小号给仓库号)
	em_Trade_Warehouse_Type_PickItem
};

enum em_Deal_Warehouse_IsPick : DWORD
{
	em_Deal_Warehouse_IsPick_None,
	em_Deal_Warehouse_IsPick_Pick,
	em_Deal_Warehouse_IsPick_BackToSwitchPlayer
};

struct VerItemSupport
{
	DWORD					            dwChannel;
	DWORD					            dwServerId;
	std::wstring					    wsPlayerName;
	std::wstring					    wsAccountName;
	std::vector<std::wstring>			vWareHouse;
	VerItemSupport()
	{
		dwChannel = dwServerId = NULL;
	}
};

struct BnsServerInfo
{
	std::wstring					wsNiceName; // 1区
	std::wstring					wsRealName; // 강호연파
	DWORD					        dwServerId; // 0x18
};

struct TradeItem
{
	std::wstring					wsItemName;
	DWORD					        dwCount;
};

struct TradeContent
{
	std::wstring					wsWareHouseName;
	DWORD					        dwTradeMoney;
	std::wstring					wsClientIp;
	std::vector<TradeItem>			VecTradeItem;
};

struct ItemSupportResult
{
	std::wstring             wsWareHouseName;
	DWORD		             dwChannel;
	em_Deal_Warehouse_Status emDealStatus;

	ItemSupportResult() : dwChannel(NULL), emDealStatus(em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_None) {}
};

struct WarehouseActionResult
{
	em_Trade_Warehouse_Type		emWarehouseType;
	em_Deal_Warehouse_IsPick	emIsPick;
	std::wstring				wsPlayerName;
};

extern std::vector<std::vector<BnsServerInfo>> vBnsServerArray;

enum em_ClientAccountLog_Type : DWORD
{
	em_ClientAccountLog_Type_AccountLog,
	em_ClientAccountLog_Type_PlayerDaily,
	em_ClientAccountLog_Type_TaskUpgradeSchedule
};
struct ClientAccountLog
{
	std::wstring			wsContentText;
	em_ClientAccountLog_Type emType;
};

/////////验证/////////////////////////////////////////////////////////////////
struct CardInfo
{
	std::wstring wsCardNo;											// 卡号
	DWORD   dwCardCrc         = NULL;				                // 卡号Crc
	int		nMaxHour          = 0;				                    // 所有时间
	int		nSurplusHour      = 0;			                        // 剩余时间
	std::wstring wsActiveTime;										// 激活时间
	DWORD	dwToken           = NULL;				                // 令牌
	BOOL	bExit             = FALSE;					            // 是否被占用了   = TRUE表示占用  = FALSE表示木有被占用
	std::wstring wsIp       = L"127.0.0.1";							// 占用的IP
	std::wstring wsAccountName;										// 占用的帐号名
	std::wstring wsPlayerName;										// 占用的玩家名
	ULONGLONG dwTick64        = NULL;				                // 查询的时间
	ULONGLONG ulKeepALiveTick = NULL;		                        // 心跳的时间

	int GetSurplusHour() CONST
	{
		int nHour = nSurplusHour - (int)((::GetTickCount64() - dwTick64) / 1000 / 60 / 60);
		return nHour <= 0 ? 0 : nHour;
	}
};

typedef struct _VerCardInfo
{
	BOOL bExist;
	CardInfo Info;
	std::wstring  wstrMsg;
	_VerCardInfo()
	{
		bExist = FALSE;
		wstrMsg = L"";
	}
}VerCardInfo;

struct ConfigContent
{
	std::wstring wsConfigName;
	std::wstring wsConfigValue;

	ConfigContent() = default;
	ConfigContent(_In_ CONST std::wstring& wsConfigName_, _In_ CONST std::wstring& wsConfigValue_) : wsConfigName(wsConfigName_), wsConfigValue(wsConfigValue_) {}
	ConfigContent(_In_ std::wstring&& wsConfigName_, _In_ std::wstring&& wsConfigValue_) : wsConfigName(std::move(wsConfigName_)), wsConfigValue(std::move(wsConfigValue_)) {}
};

struct ClientConfigContent
{
	std::wstring wsAccountName;
	std::wstring wsPlayerName;
	ConfigContent ConfigContent_;

	std::wstring to_wstring() CONST;
};

struct Login_Account_Share_Info
{
	WCHAR wszPlayerName[64];
	WCHAR wszAccountName[64];
	WCHAR wszAccountPass[64];
	WCHAR wszAccountKey[64];
	DWORD dwValue;
};

struct Login_Share_Info
{
	Login_Account_Share_Info arrLoginAccount[50];
	UINT uCount;
};

///////主线组队 | 副本组队///////////////////////////////////////////////////////////////////
struct TeamChannelMsg
{
	DWORD					dwServerId = NULL;
	DWORD					dwChannel  = NULL;
	DWORD					dwTaskId   = NULL;
	DWORD					dwScheduleId = NULL;
	DWORD					dwMaxChannel = NULL;
};

struct TeamScheduleMsg
{
	DWORD					dwTeamId;
	std::wstring			wsAccountName;
	std::wstring			wsScheduleName;
	DWORD					uScheduleIndex;
};

struct BnsDungoenScheduleMember
{
	std::wstring wsAccountName;
	std::wstring wsPlayerName;
	DWORD        dwPlayerId;
	std::wstring wsDungoenSchedule;
	DWORD        uGatherIndex;
	DWORD        dwTaskId;
	DWORD        dwScheduleId;
};

struct ClientTeamMember
{
	std::wstring wsAccountName;
	std::wstring wsPlayerName;
	DWORD	dwPlayerId;
	ClientTeamMember()
	{
		wsPlayerName = L"";
		dwPlayerId = NULL;
	}
	ClientTeamMember(_In_ CONST std::wstring& wsName, _In_ DWORD dwId)
	{
		wsPlayerName = wsName;
		dwPlayerId = dwId;
	}
	ClientTeamMember& operator=(CONST ClientTeamMember& Cti)
	{
		this->wsAccountName = Cti.wsAccountName;
		this->wsPlayerName = Cti.wsPlayerName;
		this->dwPlayerId = Cti.dwPlayerId;
		return *this;
	}
};

struct ClientTeamInfo
{
	ClientTeamMember SelfCti;	// 自己
	DWORD	dwTaskId;			// 任务ID
	DWORD	dwScheduleId;		// 进度Id
	DWORD	dwServerId;			// 服务器Id
	DWORD	dwChannelId;		// 频道Id
	DWORD	dwMaxMember;		// 最大成员
	std::wstring wsGroupTitle;	// 
	std::vector<ClientTeamMember> vTeamMember; // 成员函数
	ClientTeamInfo()
	{
		dwTaskId = dwScheduleId = dwServerId = dwChannelId = dwMaxMember = NULL;
	}

	BOOL FindTeamMember(_In_ CONST std::wstring& wsPlayerName) const
	{
		return std::find_if(vTeamMember.begin(), vTeamMember.end(), [&wsPlayerName](_In_ CONST ClientTeamMember& ClientTeamMember_) {
			return wsPlayerName == ClientTeamMember_.wsPlayerName;
		}) != vTeamMember.end();
	}
};

///////跨服组队///////////////////////////////////////////////////////////////////
struct CrossAreaTeamMember
{
	ClientTeamMember                Info;
	std::set<std::wstring>			SetDungoenName;
	std::wstring					wsGroupTitle;
	em_TeamMeber_Classes			emClasses;
	ULONGLONG						ulTick;
	ULONGLONG						ulStartTick;
	DWORD							dwRoomId = NULL;
	bool operator == (CONST CrossAreaTeamMember& Member) CONST
	{
		return this->Info.wsAccountName == Member.Info.wsAccountName;
	}
};

enum em_CrossAreaTeam_Difficulty : DWORD
{
	em_CrossAreaTeam_Difficulty_Easy = 0x1,
	em_CrossAreaTeam_Difficulty_Hard = 0x2
};

struct CrossAreaTeamGroup
{
	CrossAreaTeamMember				 Captain;
	std::vector<CrossAreaTeamMember> VecMember;
	DWORD							 dwTeamId = NULL;
	std::wstring					 wsDungeonName;
	DWORD							 dwDungeonMapIndex;
	em_CrossAreaTeam_Difficulty		 emDifficulty;
};


////////虚拟硬件//////////////////////////////////////////////////////////////////
struct SocketAccountVirtualSeriaNumber
{
	std::wstring wsAccountName;
	std::wstring wsMacAddr;
	std::wstring wsNetCard;
	std::wstring wsHardDiskSeriaNumber;
};

#endif // !__GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_COMMON_SERVERCOMMON_H__
