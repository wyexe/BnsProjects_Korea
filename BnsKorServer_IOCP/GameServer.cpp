#include "GameServer.h"
#include <MyTools/SocketBuffer.h>
#include <MyTools/CLPublic.h>
#include <MyTools/CLAsync.h>
#include <MyTools/CLEchoException.h>
#include <MyTools/Log.h>
#include <thread>
#include <future>
#include "GameClient.h"
#include "BnsArea.h"
#include "CardExtend.h"
#include "CloudConfigExtend.h"
#include "DbManager.h"
#include "BnsOTP.h"
#include "ScriptFile.h"
#include "CrossAreaTeam.h"
#include "GamePlayerExtend.h"
#include "BnsTeam.h"
#include "ServerCommand.h"
#include "EchoPacket.h"


#define _SELF L"GameServer.cpp"

MyTools::CLSocketClient* CGameServer::CreateNewSocketClient(_In_ SOCKET ClientSock)
{
	return new CGameClient(ClientSock);
}

VOID CGameServer::ReleaseSocketClient(_In_ MyTools::CLSocketClient* pSocketClient)
{
	pSocketClient->DisConnect();
	pSocketClient->Remove();
	delete dynamic_cast<CGameClient *>(pSocketClient);
	pSocketClient = nullptr;
}

BOOL CGameServer::EchoRecive(_In_  MyTools::CLSocketClient* pSocketClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer)
{
	em_Sock_Msg emSockMsg;
	// Test
	MyTools::CLSocketBuffer tmpSocketBuffer = *pSocketBuffer;
	auto Error = pSocketBuffer->VerDataPtr(emSockMsg);
	if (Error != MyTools::CLSocketBuffer::em_SocketBuffer_Error_None)
	{
		LOG_CF_E(L"数据包效验错误!. Err=%d", Error);

		DWORD dwSize, dwValue1, dwValue2,dwValue3 =  0;
		tmpSocketBuffer >> dwSize >> dwValue1 >> dwValue2 >> dwValue3;
		LOG_CF_E(L"dwSize=%d, dwValue1=%X, dwValue2=%X,dwValue3=%X, BufferSize=%d", dwSize, dwValue1, dwValue2, dwValue3, tmpSocketBuffer.size());
		return FALSE;
	}

	
	CGameClient* pGameClient = dynamic_cast<CGameClient*>(pSocketClient);
	auto itr = _VecEchoPacketPtr.find(emSockMsg);
	if (itr != _VecEchoPacketPtr.end())
	{
		if (_VecCheckCard.find(emSockMsg) == _VecCheckCard.end())
		{
			if (!CEchoPacket::GetInstance().CheckCard(*pGameClient, pSocketBuffer))
			{
				return PostSend(pSocketClient, pSocketBuffer);
			}
		}

		//LOG_CF_D(L"Echo Packet[%X], Account[%s]", static_cast<DWORD>(emSockMsg), pGameClient->GetAccountContent().wsAccountName.c_str());
		itr->second(*pGameClient, pSocketBuffer);
		if (pSocketBuffer->size() != 0)
		{
			//LOG_CF_D(L"Send[%X] Account[%s]", static_cast<DWORD>(emSockMsg), pGameClient->GetAccountContent().wsAccountName.c_str());
			return PostSend(pSocketClient, pSocketBuffer);
		}
		

		// Send Empty Buffer to Client
		MyTools::CLSocketBuffer SocketBuffer;
		SocketBuffer.InitializeHead<em_Sock_Msg>(emSockMsg);
		return PostSend(pSocketClient, &SocketBuffer);
	}
	else
	{
		LOG_CF_E(L"emSockMsg=%X, UnExist", static_cast<DWORD>(emSockMsg));
	}

	return FALSE;
}


VOID CGameServer::PrintOnLinePlayerThread()
{
	for (;;)
	{
		UINT uOnLineCount = 0 ;
		InvokeVecSocketClient([this, &uOnLineCount]
		{
			for (auto& itm : _VecSocketClient)
			{
				if (itm->IsOnLine())
				{
					uOnLineCount += 1;
				}
				else if (itm->IsKeepALiveTimeout())
				{
					//LOG_C_D(L"DisConnect [%X]", itm->GetSocket());
					itm->DisConnect();
				}
			}
		});

		LOG_CF_D(L"在线有效卡号数量=%d, AcceptCount=%d", uOnLineCount, _uAccpetCount.load());
		//CBnsTaskTeam::GetInstance().PrintOnLineTeam();
		::Sleep(1000 * 60);
	}
}

template<typename T>
VOID InitializeServices()
{
	T::GetInstance().Initialize();
}

VOID CGameServer::InitializeBnsService()
{
	_VecCheckCard = { em_Sock_Msg_RECHARGECARD,em_Sock_Msg_QUERY_CARDINFO,em_Sock_Msg_TITLE,em_Sock_Msg_LOGINGAME,em_Sock_Msg_CONSOLE_KEEPALIVE,em_Sock_Msg_Query_ACCOUNTVIRTUALSERIANUMBER };
	
	/*_VecEchoPacketPtr =
	{
	{ em_Sock_Msg_QUERY_CARDINFO, std::bind(&CEchoPacket::QueryCard, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2) },
	{ em_Sock_Msg_KEEPALIVE, std::bind(&CEchoPacket::KeepALive, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2) },
	{ em_Sock_Msg_QUERY_SCRIPTKEY, std::bind(&CEchoPacket::DownloadScript, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2) },
	{ em_Sock_Msg_TITLE, std::bind(&CEchoPacket::Announcement, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2) },
	{ em_Sock_Msg_LOGINGAME, std::bind(&CEchoPacket::GameLogin, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2) },
	{ em_Sock_Msg_JOIN_TEAM_ORDER, std::bind(&CEchoPacket::JoinTeam_MainLine, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2) },
	{ em_Sock_Msg_LEAVE_TEAM_ORDER, std::bind(&CEchoPacket::LeaveTeam_MainLine, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2) },
	{ em_Sock_Msg_DISSOLUTION_TEAM, std::bind(&CEchoPacket::LeaveTeam_Gungoen, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2) },
	{ em_Sock_Msg_CLIENT_ITEMSUPPORT, std::bind(&CEchoPacket::ItemSupport_GameAccount, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2) },
	{ em_Sock_Msg_CLIENT_PICKITEM, std::bind(&CEchoPacket::PickItem_GameAccount, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2) },
	{ em_Sock_Msg_CLIENT_WAREHOUSE_MAINTASK, std::bind(&CEchoPacket::ItemSupport_Warehouse, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2) },
	{ em_Sock_Msg_CLIENT_WAREHOUSE_PICKITEM, std::bind(&CEchoPacket::PickItem_Warehouse, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2) },
	{ em_Sock_Msg_UPDATE_TEAM_SCHEDULE, std::bind(&CEchoPacket::SetGungoenSchedule, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2) },
	{ em_Sock_Msg_GET_TEAM_SCHEDULE, std::bind(&CEchoPacket::GetGungoenSchedule, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2) },
	{ em_Sock_Msg_SET_TEAM_READONLY, std::bind(&CEchoPacket::SetTeamReadonly, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2) },
	{ em_Sock_Msg_CLIENT_DEALMESSAGE, std::bind(&CEchoPacket::SaveTradeData, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2) },
	{ em_Sock_Msg_CLIENT_LOGINED_LOG, std::bind(&CEchoPacket::SaveLoginData, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2) },
	{ em_Sock_Msg_CLIENT_BANNED, std::bind(&CEchoPacket::Banned, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2) },
	{ em_Sock_Msg_GET_CHANNEL, std::bind(&CEchoPacket::GetLessPersonChannel, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2) },
	{ em_Sock_Msg_GET_CONFIG_VALUE, std::bind(&CEchoPacket::GetCouldConfigValue, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2) },
	{ em_Sock_Msg_SET_CONFIG_VALUE, std::bind(&CEchoPacket::SetCouldConfigValue, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2) },
	{ em_Sock_Msg_CONSOLE_KEEPALIVE, std::bind(&CEchoPacket::ConsoleKeepALive, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2) },
	{ em_Sock_Msg_SAVE_LOCAL_ACCOUNT_LOG, std::bind(&CEchoPacket::SaveAccountLogData, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2) },
	{ em_Sock_Msg_OTP, std::bind(&CEchoPacket::CalculateOTP, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2) },
	{ em_Sock_Msg_QUERY_DAILYTASK_SCHEDULE, std::bind(&CEchoPacket::GetDailyTaskSchedule, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2) },
	{ em_Sock_Msg_UPDATE_DAILYTASK_SCHEDULE, std::bind(&CEchoPacket::SetDailyTaskSchedule, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2) },
	{ em_Sock_Msg_CROSSAREATEAM, std::bind(&CEchoPacket::JoinCrossAreaTeam, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2) },
	{ em_Sock_Msg_DELETE_CROSSAREATEAM, std::bind(&CEchoPacket::LeaveCrossAreaTeam, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2) },
	{ em_Sock_Msg_SET_CROSSAREATEAM_CONTENT, std::bind(&CEchoPacket::SetCrossAreaTeamContent, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2) },
	{ em_Sock_Msg_Query_ACCOUNTVIRTUALSERIANUMBER, std::bind(&CEchoPacket::QueryAccountVirtualSeriaNumber, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2) },
	};*/

	_VecEchoPacketPtr.insert(std::make_pair(em_Sock_Msg_QUERY_CARDINFO, std::bind(&CEchoPacket::QueryCard, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2)));
	_VecEchoPacketPtr.insert(std::make_pair(em_Sock_Msg_KEEPALIVE, std::bind(&CEchoPacket::KeepALive, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2)));
	_VecEchoPacketPtr.insert(std::make_pair(em_Sock_Msg_QUERY_SCRIPTKEY, std::bind(&CEchoPacket::DownloadScript, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2)));
	_VecEchoPacketPtr.insert(std::make_pair(em_Sock_Msg_TITLE, std::bind(&CEchoPacket::Announcement, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2)));
	_VecEchoPacketPtr.insert(std::make_pair(em_Sock_Msg_LOGINGAME, std::bind(&CEchoPacket::GameLogin, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2)));
	_VecEchoPacketPtr.insert(std::make_pair(em_Sock_Msg_JOIN_TEAM_ORDER, std::bind(&CEchoPacket::JoinTeam_MainLine, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2)));
	_VecEchoPacketPtr.insert(std::make_pair(em_Sock_Msg_LEAVE_TEAM_ORDER, std::bind(&CEchoPacket::LeaveTeam_MainLine, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2)));
	_VecEchoPacketPtr.insert(std::make_pair(em_Sock_Msg_DISSOLUTION_TEAM, std::bind(&CEchoPacket::LeaveTeam_Gungoen, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2)));
	_VecEchoPacketPtr.insert(std::make_pair(em_Sock_Msg_CLIENT_ITEMSUPPORT, std::bind(&CEchoPacket::ItemSupport_GameAccount, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2)));
	_VecEchoPacketPtr.insert(std::make_pair(em_Sock_Msg_CLIENT_PICKITEM, std::bind(&CEchoPacket::PickItem_GameAccount, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2)));
	_VecEchoPacketPtr.insert(std::make_pair(em_Sock_Msg_CLIENT_WAREHOUSE_MAINTASK, std::bind(&CEchoPacket::ItemSupport_Warehouse, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2)));
	_VecEchoPacketPtr.insert(std::make_pair(em_Sock_Msg_CLIENT_WAREHOUSE_PICKITEM, std::bind(&CEchoPacket::PickItem_Warehouse, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2)));
	_VecEchoPacketPtr.insert(std::make_pair(em_Sock_Msg_UPDATE_TEAM_SCHEDULE, std::bind(&CEchoPacket::SetGungoenSchedule, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2)));
	_VecEchoPacketPtr.insert(std::make_pair(em_Sock_Msg_GET_TEAM_SCHEDULE, std::bind(&CEchoPacket::GetGungoenSchedule, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2)));
	_VecEchoPacketPtr.insert(std::make_pair(em_Sock_Msg_SET_TEAM_READONLY, std::bind(&CEchoPacket::SetTeamReadonly, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2)));
	_VecEchoPacketPtr.insert(std::make_pair(em_Sock_Msg_CLIENT_DEALMESSAGE, std::bind(&CEchoPacket::SaveTradeData, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2)));
	_VecEchoPacketPtr.insert(std::make_pair(em_Sock_Msg_CLIENT_LOGINED_LOG, std::bind(&CEchoPacket::SaveLoginData, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2)));
	_VecEchoPacketPtr.insert(std::make_pair(em_Sock_Msg_CLIENT_BANNED, std::bind(&CEchoPacket::Banned, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2)));
	_VecEchoPacketPtr.insert(std::make_pair(em_Sock_Msg_GET_CHANNEL, std::bind(&CEchoPacket::GetLessPersonChannel, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2)));
	_VecEchoPacketPtr.insert(std::make_pair(em_Sock_Msg_GET_CONFIG_VALUE, std::bind(&CEchoPacket::GetCouldConfigValue, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2)));
	_VecEchoPacketPtr.insert(std::make_pair(em_Sock_Msg_SET_CONFIG_VALUE, std::bind(&CEchoPacket::SetCouldConfigValue, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2)));
	_VecEchoPacketPtr.insert(std::make_pair(em_Sock_Msg_CONSOLE_KEEPALIVE, std::bind(&CEchoPacket::ConsoleKeepALive, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2)));
	_VecEchoPacketPtr.insert(std::make_pair(em_Sock_Msg_SAVE_LOCAL_ACCOUNT_LOG, std::bind(&CEchoPacket::SaveAccountLogData, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2)));
	_VecEchoPacketPtr.insert(std::make_pair(em_Sock_Msg_OTP, std::bind(&CEchoPacket::CalculateOTP, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2)));
	_VecEchoPacketPtr.insert(std::make_pair(em_Sock_Msg_QUERY_DAILYTASK_SCHEDULE, std::bind(&CEchoPacket::GetDailyTaskSchedule, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2)));
	_VecEchoPacketPtr.insert(std::make_pair(em_Sock_Msg_UPDATE_DAILYTASK_SCHEDULE, std::bind(&CEchoPacket::SetDailyTaskSchedule, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2)));
	_VecEchoPacketPtr.insert(std::make_pair(em_Sock_Msg_CROSSAREATEAM, std::bind(&CEchoPacket::JoinCrossAreaTeam, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2)));
	_VecEchoPacketPtr.insert(std::make_pair(em_Sock_Msg_DELETE_CROSSAREATEAM, std::bind(&CEchoPacket::LeaveCrossAreaTeam, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2)));
	_VecEchoPacketPtr.insert(std::make_pair(em_Sock_Msg_SET_CROSSAREATEAM_CONTENT, std::bind(&CEchoPacket::SetCrossAreaTeamContent, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2)));
	_VecEchoPacketPtr.insert(std::make_pair(em_Sock_Msg_Query_ACCOUNTVIRTUALSERIANUMBER, std::bind(&CEchoPacket::QueryAccountVirtualSeriaNumber, &CEchoPacket::GetInstance(), std::placeholders::_1, std::placeholders::_2)));

	InitializeServices<CBnsDbManager>();
	InitializeServices<CBnsArea>();
	InitializeServices<CCardExtend>();
	InitializeServices<CScriptFile>();
	InitializeServices<CBnsOTP>();
	InitializeServices<CServerCommand>();
	InitializeServices<CGamePlayerExtend>();
	InitializeServices<CCrossAreaTeam>();
	InitializeServices<CCloudConfigExtend>();

	// 清除30天前的云配置
	CBnsDbManager::GetInstance().CleanRecentlyCloudConfig(30);
	MyTools::CLAsync::GetInstance().Run();

	std::thread tPrintOnLinePlayer(&CGameServer::PrintOnLinePlayerThread, this);
	tPrintOnLinePlayer.detach();
}