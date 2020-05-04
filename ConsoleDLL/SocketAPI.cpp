#include "stdafx.h"
#include "SocketAPI.h"
#include <MyTools/Log.h>
#include "ConsoleClient.h"

#define _SELF L"SocketAPI.cpp"
VOID CSocketAPI::ConsoleKeepALive() CONST
{
	CConsoleClient::GetInstance().SyncSend(em_Sock_Msg_CONSOLE_KEEPALIVE, nullptr);
}

BOOL CSocketAPI::QueryCard(_In_ _Out_ std::vector<CardInfo>& VecCard) CONST
{
	MyTools::CLSocketBuffer SocketBuffer;
	SocketBuffer.InitializeHead(em_Sock_Msg_QUERY_CARDINFO);

	SocketBuffer << VecCard.size();
	for (CONST auto& itm : VecCard)
		SocketBuffer << itm.dwCardCrc;

	return CConsoleClient::GetInstance().SyncSend(SocketBuffer, [&VecCard](MyTools::CLSocketBuffer& SocketBuffer)
	{
		for (auto& itm : VecCard)
		{
			SocketBuffer >> reinterpret_cast<DWORD&>(itm.bExit);
			if (!itm.bExit)
				continue;

			SocketBuffer >> reinterpret_cast<DWORD&>(itm.nMaxHour) 
						 >> reinterpret_cast<DWORD&>(itm.nSurplusHour) 
						 >> itm.wsActiveTime;
		}
	});
}

BOOL CSocketAPI::GetServerAnnouncement(_Out_ std::wstring& wsAnnouncement) CONST
{
	MyTools::CLSocketBuffer SocketBuffer;
	SocketBuffer.InitializeHead(em_Sock_Msg_TITLE);

	return CConsoleClient::GetInstance().SyncSend(SocketBuffer, [&wsAnnouncement](MyTools::CLSocketBuffer& SocketBuffer)
	{
		SocketBuffer >> wsAnnouncement;
	});
}

BOOL CSocketAPI::Run(_In_ CONST std::wstring& wsIp, _In_ USHORT uPort) CONST
{
	return CConsoleClient::GetInstance().Run(wsIp, uPort, 15 * 1000);
}

VOID CSocketAPI::Stop() CONST
{
	CConsoleClient::GetInstance().Stop();
}

VOID CSocketAPI::KeepALive(_In_ CONST KeepALiveContent* pContent) CONST
{
	CConsoleClient::GetInstance().SyncSend(em_Sock_Msg_KEEPALIVE, [=](MyTools::CLSocketBuffer& SocketBuffer)
	{
		if (pContent != nullptr)
			SocketBuffer << TRUE << pContent->Level << pContent->Gold << pContent->dwTaskId << pContent->dwScheduleId << pContent->wsContent;
		else
			SocketBuffer << FALSE;
	});
}

BOOL CSocketAPI::DownLoadScript(_In_ _Out_ CiphertextScriptInfo& Content, _Out_opt_ std::wstring& wsErrText) CONST
{
	//assert(Content.wsScriptName.empty());
	MyTools::CLSocketBuffer SocketBuffer;
	SocketBuffer.InitializeHead(em_Sock_Msg_QUERY_SCRIPTKEY);

	SocketBuffer << Content.wsScriptName;

	BOOL bRetCode = FALSE;
	CConsoleClient::GetInstance().SyncSend(SocketBuffer, [&Content, &bRetCode, &wsErrText](MyTools::CLSocketBuffer& SocketBuffer)
	{
		SocketBuffer >> Content.wsScriptName >> Content.uExpressSize >> Content.uCipherTextSize >> Content.dwExpressCRC >> Content.dwCiphertextCRC;
		//LOG_CF_D(L"Client: %s >> %d >> %d >> %X >> %X", Content.wsScriptName.c_str(), Content.uExpressSize, Content.uCipherTextSize, Content.dwExpressCRC, Content.dwCiphertextCRC);
		SocketBuffer >> Content.wsAccountName;

		UINT uSize = 0;
		Content.pScriptContent = std::move(SocketBuffer.GetBuffer(uSize));
		if (uSize != Content.uCipherTextSize)
		{
			wsErrText = MyTools::CCharacter::MakeFormatText(L"下载的脚本长度[%d]不正确,正确的长度=[%d]", uSize, Content.uCipherTextSize);
			return;
		}
		if (MyTools::CLPublic::GetCRC32_DWORD(Content.pScriptContent.get(), Content.uCipherTextSize) != Content.dwCiphertextCRC)
		{
			for (DWORD i = 0; i < Content.uCipherTextSize; ++i)
				LOG_CF_D(L"i=%d -> [%X]", i , Content.pScriptContent.get()[i]);
			wsErrText = MyTools::CCharacter::MakeFormatText(L"下载的脚本长度CRC校验失败,正确的CRC=[%08X]", Content.dwCiphertextCRC);
			return;
		}

		bRetCode = TRUE;
	});

	return bRetCode;
}

VOID CSocketAPI::AlreadyLogin(_In_ float fVersion, _In_ CONST std::wstring& wsAccountName, _In_ CONST std::wstring& wsAccountPass, _In_ CONST std::wstring& wsCardNo) CONST
{
	CConsoleClient::GetInstance().SyncSend(em_Sock_Msg_LOGINGAME, [=](MyTools::CLSocketBuffer& SocketBuffer)
	{
		SocketBuffer << fVersion << wsAccountName << wsAccountPass << wsCardNo;
	});
}

VOID CSocketAPI::JoinTeam(_In_ em_JoinTeam_Type emType, _In_ CONST ClientTeamInfo* pClientTeamInfo, _Out_ SocketTeamResult& Result) CONST
{
	MyTools::CLSocketBuffer SocketBuffer;
	switch (emType)
	{
	case CSocketAPI::em_JoinTeam_Type::em_JoinTeam_Type_MainLine:
		SocketBuffer.InitializeHead(em_Sock_Msg_JOIN_TEAM_ORDER);
		break;
	case CSocketAPI::em_JoinTeam_Type::em_JoinTeam_Type_Gungoen:
		SocketBuffer.InitializeHead(em_Sock_Msg_JOIN_DUNGOEN_TEAM);
		break;
	default:
		return;
	}

	SocketBuffer << static_cast<bool>(pClientTeamInfo != nullptr);
	if (pClientTeamInfo != nullptr)
	{
		SocketBuffer << pClientTeamInfo->dwServerId
			<< pClientTeamInfo->dwTaskId
			<< pClientTeamInfo->dwScheduleId
			<< pClientTeamInfo->dwChannelId
			<< pClientTeamInfo->dwMaxMember
			<< pClientTeamInfo->SelfCti.dwPlayerId
			<< pClientTeamInfo->wsGroupTitle
			<< pClientTeamInfo->vTeamMember.size();

		for (CONST auto& itm : pClientTeamInfo->vTeamMember)
			SocketBuffer << itm.wsPlayerName << itm.dwPlayerId;
	}

	//CConsoleClient::GetInstance().AsyncSend(SocketBuffer);
	CConsoleClient::GetInstance().SyncSend(SocketBuffer, [&Result](MyTools::CLSocketBuffer& SocketBuffer)
	{
		DWORD dwCount = 0;
		SocketBuffer >> Result.Captain.wsPlayerName >> Result.Captain.dwPlayerId >> Result.dwTeamId >> dwCount;
		if (Result.Captain.wsPlayerName == L"" || Result.Captain.dwPlayerId == 0)
		{
			LOG_MSG_CF(L"Result.Captain.wsPlayerName.empty");
		}
		//LOG_CF_D(L"Captain=[%s:%X], TeamMeber.size=%d", Result.Captain.wsPlayerName.c_str(), Result.Captain.dwPlayerId, dwCount);
		for (decltype(dwCount) i = 0; i < dwCount; ++i)
		{
			ClientTeamMember TeamMember;
			SocketBuffer >> TeamMember.wsPlayerName >> TeamMember.dwPlayerId;
			//LOG_CF_D(L"Member:[%s:%X]", TeamMember.wsPlayerName.c_str(), TeamMember.dwPlayerId);
			Result.VecMember.push_back(std::move(TeamMember));
		}
	});
}

VOID CSocketAPI::LeaveTeam(_In_ em_JoinTeam_Type emType) CONST
{
	MyTools::CLSocketBuffer SocketBuffer;
	switch (emType)
	{
	case CSocketAPI::em_JoinTeam_Type::em_JoinTeam_Type_MainLine:
		SocketBuffer.InitializeHead(em_Sock_Msg_LEAVE_TEAM_ORDER);
		break;
	case CSocketAPI::em_JoinTeam_Type::em_JoinTeam_Type_Gungoen:
		SocketBuffer.InitializeHead(em_Sock_Msg_DISSOLUTION_DUNGOEN_TEAM);
		break;
	default:
		return;
	}

	CConsoleClient::GetInstance().SyncSend(SocketBuffer, nullptr);
}

VOID CSocketAPI::ClientTrade(_In_ em_Trade_Warehouse_Type emType, _In_ CONST VerItemSupport& Content, _Out_ ItemSupportResult& Result) CONST
{
	MyTools::CLSocketBuffer SocketBuffer;
	switch (emType)
	{
	case em_Trade_Warehouse_Type_ItemSupport:
		SocketBuffer.InitializeHead(em_Sock_Msg_CLIENT_ITEMSUPPORT);
		break;
	case em_Trade_Warehouse_Type_PickItem:
		SocketBuffer.InitializeHead(em_Sock_Msg_CLIENT_PICKITEM);
		break;
	default:
		return;
	}
	

	SocketBuffer << Content.dwServerId << Content.dwChannel << Content.vWareHouse.size();
	for (CONST auto& itm : Content.vWareHouse)
		SocketBuffer << itm;

	CConsoleClient::GetInstance().SyncSend(SocketBuffer, [&Result](MyTools::CLSocketBuffer& SocketBuffer)
	{
		DWORD dwStatus = 0;
		SocketBuffer >> dwStatus;
		if (dwStatus == 0)
		{
			Result.emDealStatus = em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_None;
			return;
		}

		Result.emDealStatus = em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_Channel;
		SocketBuffer >> Result.wsWareHouseName >> Result.dwChannel;
	});
}

VOID CSocketAPI::WarehouseTrade(_In_ em_Trade_Warehouse_Type emType, _In_ CONST WareHouseInfo& Content, _Out_ WarehouseActionResult& Result) CONST
{
	MyTools::CLSocketBuffer SocketBuffer;
	switch (emType)
	{
	case em_Trade_Warehouse_Type_ItemSupport:
		SocketBuffer.InitializeHead(em_Sock_Msg_CLIENT_WAREHOUSE_MAINTASK);
		break;
	case em_Trade_Warehouse_Type_PickItem:
		SocketBuffer.InitializeHead(em_Sock_Msg_CLIENT_WAREHOUSE_PICKITEM);
		break;
	default:
		return;
	}

	SocketBuffer << Content.wsWareHouseName << Content.dwChannel << Content.dwServerId;

	CConsoleClient::GetInstance().SyncSend(SocketBuffer, [&Result](MyTools::CLSocketBuffer& SocketBuffer)
	{
		SocketBuffer >> reinterpret_cast<DWORD&>(Result.emWarehouseType) >> reinterpret_cast<DWORD&>(Result.emIsPick) >> Result.wsPlayerName;
	});
}

VOID CSocketAPI::SetGungoenSchedule(_In_ DWORD dwTeamId, _In_ CONST std::wstring& wsScheduleName, _In_ UINT uScheduleIndex) CONST
{
	CConsoleClient::GetInstance().SyncSend(em_Sock_Msg_UPDATE_TEAM_SCHEDULE, [=](MyTools::CLSocketBuffer& SocketBuffer)
	{
		SocketBuffer << dwTeamId << wsScheduleName << uScheduleIndex;
	});
}

VOID CSocketAPI::GetGungoenSchedule(_In_ DWORD dwTeamId, _Out_ std::vector<TeamMemberSchedule>& VecResult) CONST
{
	MyTools::CLSocketBuffer SocketBuffer;
	SocketBuffer.InitializeHead(em_Sock_Msg_GET_TEAM_SCHEDULE);

	SocketBuffer << dwTeamId;
	CConsoleClient::GetInstance().SyncSend(SocketBuffer, [&VecResult](MyTools::CLSocketBuffer& SocketBuffer)
	{
		DWORD dwCount = 0;
		SocketBuffer >> dwCount;

		for (decltype(dwCount) i = 0; i < dwCount; ++i)
		{
			TeamMemberSchedule Member;
			SocketBuffer >> Member.wsAccountName >> Member.wsPlayerName >> Member.dwPlayerId >> Member.wsScheduleName >> reinterpret_cast<DWORD&>(Member.uScheduleIndex) >> Member.dwTaskId >> Member.dwScheduleId;
			VecResult.push_back(std::move(Member));
		}
	});
}

VOID CSocketAPI::SetTeamReadonly(_In_ DWORD dwTeamId) CONST
{
	CConsoleClient::GetInstance().SyncSend(em_Sock_Msg_SET_TEAM_READONLY, [=](MyTools::CLSocketBuffer& SocketBuffer)
	{
		SocketBuffer << dwTeamId;
	});
}

VOID CSocketAPI::AddTradeContentLog(_In_ CONST TradeContent& Content) CONST
{
	CConsoleClient::GetInstance().SyncSend(em_Sock_Msg_CLIENT_DEALMESSAGE, [=](MyTools::CLSocketBuffer& SocketBuffer)
	{
		SocketBuffer << Content.wsWareHouseName << Content.dwTradeMoney << Content.VecTradeItem.size();
		for (CONST auto& itm : Content.VecTradeItem)
			SocketBuffer << itm.wsItemName << itm.dwCount;
	});
}

VOID CSocketAPI::AddLoginContentLog(_In_ CONST std::wstring& wsPlayerName) CONST
{
	CConsoleClient::GetInstance().SyncSend(em_Sock_Msg_CLIENT_LOGINED_LOG, [=](MyTools::CLSocketBuffer& SocketBuffer)
	{
		SocketBuffer << wsPlayerName;
	});
}

VOID CSocketAPI::AddBannedContentLog() CONST
{
	CConsoleClient::GetInstance().SyncSend(em_Sock_Msg_CLIENT_BANNED, nullptr);
}

VOID CSocketAPI::AddAccountLog(_In_ CONST ClientAccountLog& Content) CONST
{
	CConsoleClient::GetInstance().SyncSend(em_Sock_Msg_SAVE_LOCAL_ACCOUNT_LOG, [Content](MyTools::CLSocketBuffer& SocketBuffer)
	{
		SocketBuffer << static_cast<DWORD>(Content.emType) << Content.wsContentText;
	});
}

DWORD CSocketAPI::GetLessPlayerChannel(_In_ CONST TeamChannelMsg& Content) CONST
{
	MyTools::CLSocketBuffer SocketBuffer;
	SocketBuffer.InitializeHead(em_Sock_Msg_GET_CHANNEL);

	SocketBuffer << Content.dwServerId << Content.dwChannel << Content.dwMaxChannel << Content.dwTaskId << Content.dwScheduleId;;

	DWORD dwChannel = 0;
	CConsoleClient::GetInstance().SyncSend(SocketBuffer, [&dwChannel](MyTools::CLSocketBuffer& SocketBuffer)
	{
		SocketBuffer >> dwChannel;
	});

	return dwChannel == 0 ? 1 : dwChannel;
}

VOID CSocketAPI::ReadCloudConfig(_In_ CONST std::wstring& wsPlayerName, _In_ _Out_ ConfigContent& Content) CONST
{
	MyTools::CLSocketBuffer SocketBuffer;
	SocketBuffer.InitializeHead(em_Sock_Msg_GET_CONFIG_VALUE);

	SocketBuffer << wsPlayerName << Content.wsConfigName;
	CConsoleClient::GetInstance().SyncSend(SocketBuffer, [&Content](MyTools::CLSocketBuffer& SocketBuffer)
	{
		SocketBuffer >> Content.wsConfigValue;
	});
}

VOID CSocketAPI::WriteCloudConfig(_In_ CONST ConfigContent& Content) CONST
{
	CConsoleClient::GetInstance().SyncSend(em_Sock_Msg_SET_CONFIG_VALUE, [Content](MyTools::CLSocketBuffer& SocketBuffer)
	{
		SocketBuffer << Content.wsConfigName << Content.wsConfigValue;
	});
}

DWORD CSocketAPI::CalcualteOTP() CONST
{
	MyTools::CLSocketBuffer SocketBuffer;
	SocketBuffer.InitializeHead(em_Sock_Msg_OTP);

	DWORD dwValue = 0;
	CConsoleClient::GetInstance().SyncSend(SocketBuffer, [&dwValue](MyTools::CLSocketBuffer& SocketBuffer)
	{
		SocketBuffer >> dwValue;
	});
	
	return dwValue;
}

BOOL CSocketAPI::GetDailyTaskSchedule(_Out_ std::vector<std::wstring>& VecResult) CONST
{
	MyTools::CLSocketBuffer SocketBuffer;
	SocketBuffer.InitializeHead(em_Sock_Msg_QUERY_DAILYTASK_SCHEDULE);

	return CConsoleClient::GetInstance().SyncSend(SocketBuffer, [&VecResult](MyTools::CLSocketBuffer& SocketBuffer)
	{
		DWORD dwCount = 0;
		SocketBuffer >> dwCount;
		for (decltype(dwCount) i = 0; i < dwCount; ++i)
		{
			std::wstring wsValue;
			SocketBuffer >> wsValue;
			VecResult.push_back(std::move(wsValue));
		}
	});
}

VOID CSocketAPI::SetDailyTaskSchedule(_In_ CONST std::wstring& wsDailyTaskName) CONST
{
	CConsoleClient::GetInstance().SyncSend(em_Sock_Msg_UPDATE_DAILYTASK_SCHEDULE, [wsDailyTaskName](MyTools::CLSocketBuffer& SocketBuffer)
	{
		SocketBuffer << wsDailyTaskName;
	});
}

DWORD CSocketAPI::JoinCrossAreaTeam() CONST
{
	MyTools::CLSocketBuffer SocketBuffer;
	SocketBuffer.InitializeHead(em_Sock_Msg_CROSSAREATEAM);


	DWORD dwTeamId = 0;
	CConsoleClient::GetInstance().SyncSend(SocketBuffer, [&dwTeamId](MyTools::CLSocketBuffer& SocketBuffer) {SocketBuffer >> dwTeamId; });


	return dwTeamId;
}

BOOL CSocketAPI::SetCrossAreaTeamContent(_In_ CONST CrossAreaTeamMember& Member) CONST
{
	MyTools::CLSocketBuffer SocketBuffer;
	SocketBuffer.InitializeHead(em_Sock_Msg_SET_CROSSAREATEAM_CONTENT);

	SocketBuffer << Member.Info.dwPlayerId << Member.dwRoomId << Member.wsGroupTitle << static_cast<BYTE>(Member.emClasses) << Member.SetDungoenName.size();
	for (CONST auto& itm : Member.SetDungoenName)
		SocketBuffer << itm;


	bool bSetSucc = false;
	CConsoleClient::GetInstance().SyncSend(SocketBuffer, [&bSetSucc](MyTools::CLSocketBuffer& SocketBuffer) {SocketBuffer >> bSetSucc; });


	return bSetSucc ? TRUE : FALSE;
}

CSocketAPI::em_CrossAreaTeam_ResultType CSocketAPI::GetTeamGroupContent(_In_ DWORD dwTeamId, _Out_ CrossAreaTeamGroup& Teamgroup) CONST
{
	MyTools::CLSocketBuffer SocketBuffer;
	SocketBuffer.InitializeHead(em_Sock_Msg_CROSSAREATEAM);

	SocketBuffer << dwTeamId;

	CSocketAPI::em_CrossAreaTeam_ResultType ResultType = em_CrossAreaTeam_ResultType::Timeout;
	CConsoleClient::GetInstance().SyncSend(SocketBuffer, [&Teamgroup, &ResultType](MyTools::CLSocketBuffer& SocketBuffer)
	{
		BOOL bFind = FALSE;
		SocketBuffer >> reinterpret_cast<DWORD&>(bFind);
		if (!bFind)
		{
			ResultType = em_CrossAreaTeam_ResultType::Undefine;
			return;
		}

		UINT uCount = 0;
		SocketBuffer >> Teamgroup.wsDungeonName
			>> Teamgroup.Captain.Info.wsPlayerName
			>> Teamgroup.Captain.Info.dwPlayerId 
			>> Teamgroup.Captain.dwRoomId 
			>> Teamgroup.dwDungeonMapIndex
			>> reinterpret_cast<DWORD&>(Teamgroup.emDifficulty)
			>> uCount;

		for (UINT i = 0;i < uCount; ++i)
		{
			CrossAreaTeamMember Member;
			SocketBuffer >> Member.Info.wsPlayerName >> Member.Info.dwPlayerId;
			Teamgroup.VecMember.push_back(std::move(Member));
		}
		ResultType = em_CrossAreaTeam_ResultType::Succ;
	});
	return ResultType;
}

VOID CSocketAPI::LeaveCrossAreaTeam(_In_ DWORD dwTeamId) CONST
{
	CConsoleClient::GetInstance().SyncSend(em_Sock_Msg_DELETE_CROSSAREATEAM, [dwTeamId](MyTools::CLSocketBuffer& SocketBuffer)
	{
		SocketBuffer << dwTeamId;
	});
}

VOID CSocketAPI::QueryAccountVirtualSeriaNumber(_In_ CONST std::vector<std::wstring>& VecAccount, _Out_ std::vector<SocketAccountVirtualSeriaNumber>& VecVirtualSeriaNumber) CONST
{
	MyTools::CLSocketBuffer SocketBuffer;
	SocketBuffer.InitializeHead(em_Sock_Msg_Query_ACCOUNTVIRTUALSERIANUMBER);
	SocketBuffer << VecAccount.size();
	for (auto& itm : VecAccount)
		SocketBuffer << itm;


	CConsoleClient::GetInstance().SyncSend(SocketBuffer, [&VecVirtualSeriaNumber](MyTools::CLSocketBuffer& ServerSocketBuffer)
	{
		UINT uCount = 0;
		ServerSocketBuffer >> uCount;
		for (UINT i = 0; i < uCount; ++i)
		{
			SocketAccountVirtualSeriaNumber Content;
			ServerSocketBuffer >> Content.wsAccountName >> Content.wsHardDiskSeriaNumber >> Content.wsMacAddr >> Content.wsNetCard;
			VecVirtualSeriaNumber.push_back(Content);
		}
	});
}
