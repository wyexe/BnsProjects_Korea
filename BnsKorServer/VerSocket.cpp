#include "VerSocket.h"
#include "BnsDeal.h"
#include <queue>
#include "ClientConfig.h"
#include "OneTimePassword.h"
#include "DailyTask.h"
//////////////宏////////////////////////////////////////////////////////////

#define _SELF L"VerSocket.cpp"



CVerSocket::CVerSocket()
{
}

CVerSocket::~CVerSocket()
{
}


DWORD WINAPI CVerSocket::_KeepALiveThread(LPVOID lpParm)
{
	auto pulThreadKeepALiveArray = static_cast<ULONGLONG*>(lpParm);

	UINT uCount = 0;
	for (;;)
	{
		::Sleep(5 * 1000);
		if (uCount++ % 12 == 0)
		{
			int nCount = 0;
			int nMaxCount = 0;
			CVerManage::DoAction_By_CardList([&nCount, &nMaxCount](std::deque<CardInfo>& CardList)
			{
				for (CONST auto& itm : CardList)
				{
					if (itm.dwToken != NULL && static_cast<DWORD>(::GetTickCount64() - itm.ulKeepALiveTick) <= 60 * 1000)
						nCount += 1;
				}
				nMaxCount = CardList.size();
			});
			LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"1分钟之内在线数量=%d, 卡号MaxCount=%d", nCount, nMaxCount);
		}

		for (int i = 0; i < 5; ++i)
		{
			if (static_cast<DWORD>(::GetTickCount64() - pulThreadKeepALiveArray[i]) >= 10 * 1000)
			{
				LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception,L"CVerSocket::_WorkThread线程已经停止运行了!");
				break;
			}
		}
	}
}

DWORD WINAPI CVerSocket::_WorkThread(LPVOID lpParm)
{
	auto pulKeepALiveTick = static_cast<ULONGLONG*>(lpParm);
	BOOL bExist = FALSE;
	for (;;)
	{
		std::shared_ptr<SocketClientBuffer> SocketClientBufferPtr;
		*pulKeepALiveTick = ::GetTickCount64();
		if (!CSocketServer::GetInstance().GetWaitToProcessData(bExist, SocketClientBufferPtr))
			continue;

		ByteBuffer ClientByteBuffer;
		ByteBuffer ServerByteBuffer;
		std::wstring wstrIp;
		DWORD dwClientCmd = 0;

		wstrIp = SocketClientBufferPtr->SocketClientInfoPtr->SocketSession_.wsClientIp;
		ClientByteBuffer = SocketClientBufferPtr->ByteBuffer_;
		ClientByteBuffer >> dwClientCmd;
		ServerByteBuffer << dwClientCmd;
		switch (dwClientCmd)
		{
		case SOCK_MSG_QUERY_CARDINFO:
		{
			std::vector<DWORD> VecCardCrc;
			CVerManage::UnPack(dwClientCmd, ClientByteBuffer, &VecCardCrc);
			CVerManage::QueryClientCard(VecCardCrc, SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_, ServerByteBuffer);
			SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_.wsAccountName = L"Console";
		}
			break;
		case SOCK_MSG_KEEPALIVE:
		{
			KeepALiveContent KeepALiveContent_;
			CVerManage::UnPack(dwClientCmd, ClientByteBuffer, &KeepALiveContent_);
			CVerManage::KeepALive(KeepALiveContent_, SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_, ServerByteBuffer);
		}
			break;
		case SOCK_MSG_GET_TASKSCRIPT:
			_GetTaskScriptFileList(SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_, ClientByteBuffer, ServerByteBuffer);
			break;
		case SOCK_MSG_GET_NEXT_TASKSCRIPT:
			break;
		case SOCK_MSG_LOGINGAME:
		{
			SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_.wsIp = wstrIp;
			CVerManage::UnPack(dwClientCmd, ClientByteBuffer, &SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_);
									
								   CVerManage::SetToken(SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_, ServerByteBuffer);
		}
			break;
		case SOCK_MSG_TITLE:
		{
							   CVerManage::GetVerTitle(wstrIp, ServerByteBuffer);
		}
			break;
		case SOCK_MSG_JOIN_TEAM_ORDER:
		{
			ClientTeamInfo CtiMember;
			CVerManage::UnPack(dwClientCmd, ClientByteBuffer, &CtiMember);
			if (CVerManage::IsALive(SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_, ServerByteBuffer))
				CBnsTaskTeam::GetInstance().JoinTaskTeam(CtiMember, ServerByteBuffer);

		}
			break;
		case SOCK_MSG_LEAVE_TEAM_ORDER: case SOCK_MSG_DISSOLUTION_TEAM:
		{
			ClientTeamInfo CtiMember;
			CVerManage::UnPack(dwClientCmd, ClientByteBuffer, &CtiMember);
			CBnsTaskTeam::GetInstance().DeleteTaskMember(CtiMember.SelfCti.wsAccountName);
			ServerByteBuffer << dwClientCmd;
		}
			break;
		case SOCK_MSG_JOIN_DUNGOEN_TEAM:
		{
			ClientTeamInfo CtiMember;
			CVerManage::UnPack(dwClientCmd, ClientByteBuffer, &CtiMember);
			if (CVerManage::IsALive(SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_, ServerByteBuffer))
				CBnsTaskTeam::GetInstance().JoinDungoenTeam(CtiMember, ServerByteBuffer);
		}
			break;
		case SOCK_MSG_DISSOLUTION_DUNGOEN_TEAM:
		{
			ClientTeamInfo CtiMember;
			CVerManage::UnPack(dwClientCmd, ClientByteBuffer, &CtiMember);
			CBnsTaskTeam::GetInstance().DeleteDungoenTeamMember(CtiMember.SelfCti.wsAccountName);
			ServerByteBuffer << SOCK_MSG_DISSOLUTION_DUNGOEN_TEAM;
		}
			break;
		case SOCK_MSG_CLIENT_ITEMSUPPORT:
		{
			VerItemSupport VerItemSupport_;
			VerItemSupport_.wsPlayerName = SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_.wsPlayerName;
			VerItemSupport_.wsAccountName = SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_.wsAccountName;
			CVerManage::UnPack(dwClientCmd, ClientByteBuffer, &VerItemSupport_);
			if (CVerManage::IsALive(SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_, ServerByteBuffer))
			{
				CBnsDeal::GetInstance().JoinItemSupportDeal(VerItemSupport_, ServerByteBuffer);
			}
		}
			break;
		case SOCK_MSG_CLIENT_PICKITEM:
		{
			VerItemSupport VerItemSupport_;
			VerItemSupport_.wsPlayerName = SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_.wsPlayerName;
			VerItemSupport_.wsAccountName = SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_.wsAccountName;
			CVerManage::UnPack(dwClientCmd, ClientByteBuffer, &VerItemSupport_);
			if (CVerManage::IsALive(SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_, ServerByteBuffer))
			{
				CBnsDeal::GetInstance().JoinPickItemtDeal(VerItemSupport_, ServerByteBuffer);
			}
		}
			break;
		case SOCK_MSG_CLIENT_WAREHOUSE_MAINTASK:
		{
			CBnsDeal::WareHouseInfo WareHouseInfo_;
			CVerManage::UnPack(dwClientCmd, ClientByteBuffer, &WareHouseInfo_);
			if (CVerManage::IsALive(SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_, ServerByteBuffer))
			{
				CBnsDeal::GetInstance().AcceptItemSupportDeal(WareHouseInfo_, ServerByteBuffer);
			}
		}
			break;
		case SOCK_MSG_CLIENT_WAREHOUSE_PICKITEM:
		{
			CBnsDeal::WareHouseInfo WareHouseInfo_;
			CVerManage::UnPack(dwClientCmd, ClientByteBuffer, &WareHouseInfo_);
			if (CVerManage::IsALive(SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_, ServerByteBuffer))
			{
				CBnsDeal::GetInstance().AcceptPickItemDeal(WareHouseInfo_, ServerByteBuffer);
			}
		}
			break;
		case SOCK_MSG_UPDATE_TEAM_SCHEDULE:
		{
			TeamScheduleMsg TeamScheduleMsg_;
			CVerManage::UnPack(dwClientCmd, ClientByteBuffer, &TeamScheduleMsg_);
			TeamScheduleMsg_.wsAccountName = SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_.wsAccountName;
			if (CVerManage::IsALive(SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_, ServerByteBuffer))
			{
				CBnsTaskTeam::GetInstance().UpdateDungoenSchedule(TeamScheduleMsg_, ServerByteBuffer);
			}
		}
		break;
		case SOCK_MSG_GET_TEAM_SCHEDULE:
		{
			TeamScheduleMsg TeamScheduleMsg_;
			CVerManage::UnPack(dwClientCmd, ClientByteBuffer, &TeamScheduleMsg_);
			if (CVerManage::IsALive(SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_, ServerByteBuffer))
			{
				CBnsTaskTeam::GetInstance().GetDungoenSchedule(TeamScheduleMsg_, ServerByteBuffer);
			}
		}
		break;
		case SOCK_MSG_SET_TEAM_READONLY:
		{
			TeamScheduleMsg TeamScheduleMsg_;
			CVerManage::UnPack(dwClientCmd, ClientByteBuffer, &TeamScheduleMsg_);
			if (CVerManage::IsALive(SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_, ServerByteBuffer))
			{
				CBnsTaskTeam::GetInstance().SetReadOnlyTeam(TeamScheduleMsg_.dwTeamId, ServerByteBuffer);
			}
		}
		break;
		case SOCK_MSG_CLIENT_DEALMESSAGE:
		{
			VerDealInfo VerDealInfo_;
			VerDealInfo_.wsClientIp = wstrIp;
			CVerManage::UnPack(dwClientCmd, ClientByteBuffer, &VerDealInfo_);
			CVerManage::AddDealInfo(SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_, VerDealInfo_);
			ServerByteBuffer << SOCK_MSG_CLIENT_DEALMESSAGE;

		}
			break;
		case SOCK_MSG_CLIENT_LOGINED_LOG:
			CVerManage::UnPack(dwClientCmd, ClientByteBuffer, &SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_);
			CVerManage::AddLoginedMsgToDb(SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_);
			ServerByteBuffer << SOCK_MSG_CLIENT_LOGINED_LOG;
			break;
		case SOCK_MSG_CLIENT_BANNED:
			if (CVerManage::IsNewBanned(SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_.wsAccountName))
			{
				// 封号了, 给所有在线的Client的心跳回发封号的判断!
				CVerManage::AddBannedAccountToDb(SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_);
				CVerManage::IsBanned() = TRUE;
			}
			ServerByteBuffer << SOCK_MSG_CLIENT_BANNED;
			break;
		case SOCK_MSG_GET_CHANNEL:
		{
			TeamChannelMsg TeamChannelMsg_;
			CVerManage::UnPack(dwClientCmd, ClientByteBuffer, &TeamChannelMsg_);
			if (CVerManage::IsALive(SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_, ServerByteBuffer))
			{
				CBnsTaskTeam::GetInstance().GetLessMemberChannel(TeamChannelMsg_, ServerByteBuffer);
			}
		}
			break;
		case SOCK_MSG_GET_CONFIG_VALUE:
		{
			ClientConfigContent ClientConfigContent_;
			CVerManage::UnPack(dwClientCmd, ClientByteBuffer, &ClientConfigContent_);
			if (CVerManage::IsALive(SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_, ServerByteBuffer))
				CClientConfig::GetInstance().GetConfigValueByName(ClientConfigContent_, ServerByteBuffer);
		}
		break;
		case SOCK_MSG_SET_CONFIG_VALUE:
		{
			ClientConfigContent ClientConfigContent_;
			CVerManage::UnPack(dwClientCmd, ClientByteBuffer, &ClientConfigContent_);
			if (CVerManage::IsALive(SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_, ServerByteBuffer))
			{
				CClientConfig::GetInstance().SetConfigValueByName(ClientConfigContent_);
				ServerByteBuffer << dwClientCmd;
			}
		}
		break;
		case SOCK_MSG_CONSOLE_KEEPALIVE:
			ServerByteBuffer << dwClientCmd;
			break;
		case SOCK_MSG_SAVE_LOCAL_ACCOUNT_LOG:
		{
			ClientAccountLog ClientLog;
			CVerManage::UnPack(dwClientCmd, ClientByteBuffer, &ClientLog);
			if (CVerManage::IsALive(SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_, ServerByteBuffer))
				CVerManage::SaveAccountLog(ClientLog, ServerByteBuffer);
		}
		break;
		case SOCK_MSG_OTP:
		{
			std::wstring wsAccountName;
			ClientByteBuffer >> wsAccountName;
			if (CVerManage::IsALive(SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_, ServerByteBuffer))
				COneTimePassword::GetInstance().GetOTPKey(wsAccountName, ServerByteBuffer);
		}
			
		break;
		case SOCK_MSG_QUERY_DAILYTASK_SCHEDULE:
			if (CVerManage::IsALive(SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_, ServerByteBuffer))
				CDailyTask::GetInstance().QueryDailyTaskSchedule(SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_, ServerByteBuffer);
		break;
		case SOCK_MSG_UPDATE_DAILYTASK_SCHEDULE:
		{
			std::wstring wsDailyTaskName;
			CVerManage::UnPack(dwClientCmd, ClientByteBuffer, &wsDailyTaskName);
			if (CVerManage::IsALive(SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_, ServerByteBuffer))
				CDailyTask::GetInstance().SetDailyTaskScheduleFinish(SocketClientBufferPtr->SocketClientInfoPtr->ClientAccountContent_, wsDailyTaskName, ServerByteBuffer);
		}
		break;
		default:
			LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"异常指令:%X", dwClientCmd);
			ServerByteBuffer << (DWORD)SOCK_MSG_CLIENTMSG << L"Invalid cmd";
			break;
		}

		if (ServerByteBuffer.size() == 4)
			ServerByteBuffer << dwClientCmd;

		// 写入返回的消息ID, 方便在socket层可以输出日志
		SocketClientBufferPtr->dwMsg = dwClientCmd;
		CSocketServer::GetInstance().Send(SocketClientBufferPtr, ServerByteBuffer);
	}
	//return 0;
}

// 接收脚本文件列表
BOOL CVerSocket::_GetTaskScriptFileList(_In_ CONST ClientAccountContent& ClientAccount , _In_ ByteBuffer& ClientByteBuffer, _Out_ ByteBuffer& ServerByteBuffer)
{
	std::wstring wsScripttName;
	float fVersion = 0.0f;
	ClientByteBuffer >> fVersion;
	ClientByteBuffer >> wsScripttName;

	if (!CVerManage::IsALive(ClientAccount, ServerByteBuffer))
		return FALSE;

	return vExpressScriptList.ScriptLock.Access([&ServerByteBuffer, wsScripttName, &fVersion, ClientAccount]
	{
		// format:
		// Size + (FileName + Length + Content) * Size
		auto pExpressScriptInfo = CLPublic::Vec_find_if(vExpressScriptList.vScript, [wsScripttName](CONST ExpressScriptInfo& ExpScriptInfo){ return ExpScriptInfo.wsScriptName == wsScripttName; });
		if (pExpressScriptInfo == nullptr)
		{
			WCHAR wszText[32];
			swprintf_s(wszText, _countof(wszText) - 1, L"脚本:%s 并没有存在服务器上!", wsScripttName.c_str());
			ServerByteBuffer << SOCKET_MESSAGE_MESSAGEBOX;
			ServerByteBuffer << wszText;
			return;
		}

		// Flag << MaxSize << SurplusSize 
		ServerByteBuffer << SOCK_MSG_GET_TASKSCRIPT;
		ServerByteBuffer << vExpressScriptList.fVersion;
		ServerByteBuffer << pExpressScriptInfo->wsScriptName;
		ServerByteBuffer << pExpressScriptInfo->dwScriptSize;
		ServerByteBuffer << pExpressScriptInfo->dwContentLength;
		ServerByteBuffer << pExpressScriptInfo->dwExpressCRC;
		ServerByteBuffer << pExpressScriptInfo->dwCiphertextCRC;
		ServerByteBuffer.append(pExpressScriptInfo->pScriptContent.get(), pExpressScriptInfo->dwContentLength);
		ServerByteBuffer << ClientAccount.wsAccountName;
	});
}
