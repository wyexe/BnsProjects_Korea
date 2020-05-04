#include "VerManage.h"
#include "BnsDeal.h"
#include <MyTools/CLLock.h>
#include <MyTools/ByteBuffer.h>
#include <MyTools/CLResManager.h>
#include "SocketCommon.h"

#define _SELF L"VerManage.cpp"
//////////////////全局变量初始化////////////////////////////////////////////////////////
std::vector<std::vector<BnsServerInfo>> vBnsServerArray;
CVerManage::CVerManage()
{
}

CVerManage::~CVerManage()
{
}

BOOL CVerManage::UnPack(_In_ DWORD dwCmd, _In_ ByteBuffer& ClientByteBuffer,_Out_ LPVOID lpParm)
{
	BOOL bRetCode = TRUE;
	try
	{
		
		switch (dwCmd)
		{
		case SOCK_MSG_RECHARGECARD: // 充值 (Size+n*CardCrc)
		case SOCK_MSG_QUERY_CARDINFO: // 返回卡号详细内容 (Size+n*CardCrc)
		{
			DWORD dwSize = 0;
			ClientByteBuffer >> dwSize;
			{
				for (DWORD i = 0; i < dwSize && dwSize <= 100; ++i)
				{
					DWORD dwCrc = 0;
					ClientByteBuffer >> dwCrc;
					((std::vector<DWORD>*)lpParm)->push_back(dwCrc);
				}
			}
		}
		break;
		case SOCK_MSG_KEEPALIVE:// 心跳 (Size+n*CardCrc)
			ClientByteBuffer >> reinterpret_cast<KeepALiveContent*>(lpParm)->Level;
			ClientByteBuffer >> reinterpret_cast<KeepALiveContent*>(lpParm)->Gold;
			ClientByteBuffer >> reinterpret_cast<KeepALiveContent*>(lpParm)->wsContent;
			break;
		case SOCK_MSG_LOGINGAME:
			ClientByteBuffer >> reinterpret_cast<ClientAccountContent*>(lpParm)->wsVersion;
			ClientByteBuffer >> reinterpret_cast<ClientAccountContent*>(lpParm)->wsAccountName;
			ClientByteBuffer >> reinterpret_cast<ClientAccountContent*>(lpParm)->wsAccountPass;
			ClientByteBuffer >> reinterpret_cast<ClientAccountContent*>(lpParm)->wsCardNo;
			break;
		case SOCK_MSG_TITLE: // 公告 

			break;
		case SOCK_MSG_JOIN_TEAM_ORDER: case SOCK_MSG_LEAVE_TEAM_ORDER: case SOCK_MSG_UPDATE_TEAM_ORDER: case SOCK_MSG_JOIN_DUNGOEN_TEAM: case SOCK_MSG_DISSOLUTION_DUNGOEN_TEAM:
			ClientByteBuffer >> reinterpret_cast<ClientTeamInfo*>(lpParm)->dwServerId;
			ClientByteBuffer >> reinterpret_cast<ClientTeamInfo*>(lpParm)->dwTaskId;
			ClientByteBuffer >> reinterpret_cast<ClientTeamInfo*>(lpParm)->dwScheduleId;
			ClientByteBuffer >> reinterpret_cast<ClientTeamInfo*>(lpParm)->dwChannelId;
			ClientByteBuffer >> reinterpret_cast<ClientTeamInfo*>(lpParm)->dwMaxMember;
			ClientByteBuffer >> reinterpret_cast<ClientTeamInfo*>(lpParm)->SelfCti.wsPlayerName;
			ClientByteBuffer >> reinterpret_cast<ClientTeamInfo*>(lpParm)->SelfCti.dwPlayerId;
			ClientByteBuffer >> reinterpret_cast<ClientTeamInfo*>(lpParm)->SelfCti.wsAccountName;
			ClientByteBuffer >> reinterpret_cast<ClientTeamInfo*>(lpParm)->wsGroupTitle;
			{
				UINT uCount = 0;
				ClientByteBuffer >> uCount;
				for (UINT i = 0; i < uCount && uCount <= 10; ++i)
				{
					ClientTeamMember ClientTeamMember_;
					ClientByteBuffer >> ClientTeamMember_.wsPlayerName;
					ClientByteBuffer >> ClientTeamMember_.dwPlayerId;
					reinterpret_cast<ClientTeamInfo*>(lpParm)->vTeamMember.push_back(ClientTeamMember_);
				}
			}

			break;
		case SOCK_MSG_CLIENT_ITEMSUPPORT: case SOCK_MSG_CLIENT_PICKITEM:
		{
			UINT uCount = 0;
			VerItemSupport* pVerItemSupport = (VerItemSupport *)lpParm;
			ClientByteBuffer >> pVerItemSupport->dwServerId;
			ClientByteBuffer >> pVerItemSupport->dwChannel;
			ClientByteBuffer >> uCount;
			for (UINT i = 0; i < uCount && uCount <= 10; ++i)
			{
				std::wstring wsWareHouseName;
				ClientByteBuffer >> wsWareHouseName;
				pVerItemSupport->vWareHouse.push_back(wsWareHouseName);
			}
		}
		break;
		case SOCK_MSG_CLIENT_WAREHOUSE_MAINTASK: case SOCK_MSG_CLIENT_WAREHOUSE_PICKITEM:
		{
			CBnsDeal::WareHouseInfo* pWareHouseInfo = (CBnsDeal::WareHouseInfo*)lpParm;
			ClientByteBuffer >> pWareHouseInfo->wsWareHouseName;
			ClientByteBuffer >> pWareHouseInfo->dwChannel;
			ClientByteBuffer >> pWareHouseInfo->dwServerId;
		}
		break;
		case SOCK_MSG_CLIENT_CREATE_TEAMMEMBER_TASKINFO:
		{
			DWORD dwCount = 0;
			std::vector<ClientTeamTaskInfo>* pvlst = (std::vector<ClientTeamTaskInfo>*)lpParm;
			ClientByteBuffer >> dwCount;
			for (UINT i = 0; i < dwCount && i < 10; ++i)
			{
				ClientTeamTaskInfo ClientTeamTaskInfo_;
				ClientByteBuffer >> ClientTeamTaskInfo_.wsPlayerName;
				ClientByteBuffer >> ClientTeamTaskInfo_.dwTaskId;
				ClientByteBuffer >> ClientTeamTaskInfo_.dwScheduleId;
				pvlst->push_back(ClientTeamTaskInfo_);
			}
		}
		break;
		case SOCK_MSG_CLIENT_GET_TEAMMEMBER_TASKINFO:
		{
			ClientTeamTaskInfo* pClientTeamTaskInfo = (ClientTeamTaskInfo *)lpParm;
			ClientByteBuffer >> pClientTeamTaskInfo->wsPlayerName;
			ClientByteBuffer >> pClientTeamTaskInfo->dwTaskId;
			ClientByteBuffer >> pClientTeamTaskInfo->dwScheduleId;
		}
		break;
		case SOCK_MSG_UPDATE_TEAM_SCHEDULE:
		{
			TeamScheduleMsg* pTeamScheduleMsg = reinterpret_cast<TeamScheduleMsg*>(lpParm);
			ClientByteBuffer >> pTeamScheduleMsg->dwTeamId;
			ClientByteBuffer >> pTeamScheduleMsg->wsScheduleName;
			ClientByteBuffer >> pTeamScheduleMsg->uScheduleIndex;
		}
		break;
		case SOCK_MSG_GET_TEAM_SCHEDULE: case SOCK_MSG_SET_TEAM_READONLY:
		{
			TeamScheduleMsg* pTeamScheduleMsg = reinterpret_cast<TeamScheduleMsg*>(lpParm);
			ClientByteBuffer >> pTeamScheduleMsg->dwTeamId;
		}
		break;
		case SOCK_MSG_CLIENT_DEALMESSAGE:
		{
			VerDealInfo* pVerDealInfo = (VerDealInfo *)lpParm;
			ClientByteBuffer >> pVerDealInfo->wsClientName;
			ClientByteBuffer >> pVerDealInfo->wsClientAccountName;
			ClientByteBuffer >> pVerDealInfo->wsWareHouseName;
			ClientByteBuffer >> pVerDealInfo->dwDealMoney;
			DWORD dwCount = 0;
			ClientByteBuffer >> dwCount;
			for (DWORD i = 0; i < dwCount && i < 10; ++i)
			{
				VerDealItemInfo VerDealItemInfo_;
				ClientByteBuffer >> VerDealItemInfo_.wsItemName;
				ClientByteBuffer >> VerDealItemInfo_.dwCount;
				pVerDealInfo->vlst.push_back(VerDealItemInfo_);
			}
		}
		break;
		case SOCK_MSG_CLIENT_LOGINED_LOG:
			ClientByteBuffer >> reinterpret_cast<ClientAccountContent*>(lpParm)->wsPlayerName;
		break;
		case SOCK_MSG_GET_CHANNEL:
		{
			TeamChannelMsg* pTeamChannelMsg = reinterpret_cast<TeamChannelMsg*>(lpParm);
			ClientByteBuffer >> pTeamChannelMsg->dwServerId;
			ClientByteBuffer >> pTeamChannelMsg->dwChannel;
			ClientByteBuffer >> pTeamChannelMsg->dwMaxChannel;
			ClientByteBuffer >> pTeamChannelMsg->dwTaskId;
			ClientByteBuffer >> pTeamChannelMsg->dwScheduleId;
		}
		break;
		case SOCK_MSG_GET_CONFIG_VALUE: case SOCK_MSG_SET_CONFIG_VALUE:
			ClientByteBuffer >> reinterpret_cast<ClientConfigContent*>(lpParm)->wsAccountName;
			ClientByteBuffer >> reinterpret_cast<ClientConfigContent*>(lpParm)->wsPlayerName;
			ClientByteBuffer >> reinterpret_cast<ClientConfigContent*>(lpParm)->ConfigContent_.wsConfigName;
			ClientByteBuffer >> reinterpret_cast<ClientConfigContent*>(lpParm)->ConfigContent_.wsConfigValue;
		break;
		case SOCK_MSG_SAVE_LOCAL_ACCOUNT_LOG:
		
			ClientByteBuffer >> reinterpret_cast<DWORD&>(reinterpret_cast<ClientAccountLog*>(lpParm)->emType);
			ClientByteBuffer >> reinterpret_cast<ClientAccountLog*>(lpParm)->wsContentText;
		
		break;
		case SOCK_MSG_QUERY_DAILYTASK_SCHEDULE: case SOCK_MSG_UPDATE_DAILYTASK_SCHEDULE:
			ClientByteBuffer >> *reinterpret_cast<std::wstring*>(lpParm);
			break;
		default:
			bRetCode = FALSE;
			break;
		}
	}
	catch(...)
	{
		LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"UnPack出现异常,dwCmd=%X", dwCmd);
	}

	return bRetCode;
}

BOOL CVerManage::QueryServerCard(_Out_ std::deque<CardInfo>& vCard)
{
	::InitializeCriticalSection(&CDataBaseMdel::ms_DbSection);
	CDataBaseMdel::GetCardList(vCard);
	auto hExcuteSQLThread = cbBEGINTHREADEX(NULL, NULL, CDataBaseMdel::_ExcuteSQLThread, NULL, NULL, NULL);
	SetResDeleter(hExcuteSQLThread, [](HANDLE& hThread){::CloseHandle(hThread); });
	return TRUE;
}

BOOL CVerManage::IsExistCard(_In_ DWORD dwCardCrc, _Out_ CardInfo& CardInfo_)
{
	BOOL bExist = FALSE;
	DoAction_By_CardList([&CardInfo_, &bExist, &dwCardCrc](std::deque<CardInfo>& vlst)
	{
		auto p = CLPublic::Deque_find_if(vlst, [&dwCardCrc](CardInfo& Card){return dwCardCrc == Card.dwCardCrc; });
		if (p != nullptr)
		{
			bExist = TRUE;
			CardInfo_ = *p;
		}
	});
	return bExist;
}

BOOL CVerManage::IsExistCardList(_In_ CONST std::vector<DWORD>& VecCardCrc, _Out_ std::vector<VerCardInfo>& vCardList)
{
	try
	{
		for (CONST auto& CardCrc : VecCardCrc)
		{
			VerCardInfo VrCardInfo;
			if (IsExistCard(CardCrc, VrCardInfo.Info))
				VrCardInfo.bExist = TRUE;
			else
			{
				VrCardInfo.bExist = FALSE;
				VrCardInfo.Info.dwCardCrc = CardCrc;
			}

			vCardList.push_back(VrCardInfo);
		}
	}
	catch(...)
	{
		LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception,L"IsExistCardList出现异常");
	}
	return TRUE;
}

BOOL CVerManage::QueryClientCard(_In_ CONST std::vector<DWORD>& VecCardCrc, _In_ CONST ClientAccountContent& ClientAccount, _Out_ ByteBuffer& ServerByteBuffer)
{
	std::vector<VerCardInfo> vCardList;
	IsExistCardList(VecCardCrc, vCardList);

	ServerByteBuffer << (DWORD)SOCK_MSG_QUERY_CARDINFO;
	ServerByteBuffer << (DWORD)vCardList.size();
	for (auto& VrCard : vCardList)
	{
		if (!VrCard.bExist) // when Card UnExist, try Active Card for DB
			TryActiveCard(VrCard.Info.dwCardCrc, ClientAccount.wsIp, VrCard.Info);

		// 写入 卡号Crc << 卡号是否存在 << 最长时间 << 剩余时间 << 激活时间
		ServerByteBuffer << VrCard.Info.dwCardCrc << VrCard.bExist;
		if (VrCard.bExist)
			ServerByteBuffer << VrCard.Info.nMaxHour << VrCard.Info.GetSurplusHour() << VrCard.Info.wstrActiveTime;
	}
	return TRUE;
}

BOOL CVerManage::TryActiveCard(_In_ DWORD dwCardCrc, _In_ CONST std::wstring& wstrIp, _Out_ CardInfo& CardInfo_)
{
	WCHAR wszSQL[MAX_PATH] = { 0 };
	WCHAR wszText[MAX_PATH] = { 0 };

	swprintf_s(wszSQL, _countof(wszSQL) - 1, L"exec Proc_RechargeCard '%08X','%s'", dwCardCrc, wstrIp.c_str());
	if (!CDataBaseMdel::ExcuteSQL(wszSQL, wszText))
		return FALSE;

	if (!CCharacter::wstrstr_my(wszText, L"充值成功"))
	{
		LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"卡号尝试激活查询返回:%s, CRC=%08X, IP=%s", wszText, dwCardCrc, wstrIp.c_str());
		return FALSE;
	}

	CardInfo tmpCardInfo;
	tmpCardInfo.dwCardCrc = dwCardCrc;
	if (!CDataBaseMdel::GetCardInfo(tmpCardInfo) || tmpCardInfo.wstrCardNo.empty())
	{
		LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"查询不到卡号%08X", dwCardCrc);
		return FALSE;
	}

	if (tmpCardInfo.GetSurplusHour() <= 0)
	{
		LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"卡号%08X时间不足", dwCardCrc);
		return FALSE;
	}
	
	LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"卡号:%s 激活成功!", CardInfo_.wstrCardNo.c_str());
	CardInfo_ = tmpCardInfo;
	return DoAction_By_CardList([&CardInfo_](std::deque<CardInfo>& vlst){vlst.push_back(CardInfo_); });
}

BOOL CVerManage::KeepALive(_In_ CONST KeepALiveContent& KeepALiveContent_, _In_ ClientAccountContent& ClientAccount, _Out_ ByteBuffer& ServerByteBuffer)
{
	if (ClientAccount.dwCardCrc == NULL)
	{
		ServerByteBuffer << (DWORD)SOCKET_MESSAGE_MESSAGEBOX << std::string("你连卡号都不给我? 直接X掉没法解释了!");
		return TRUE;
	}

	DoAction_By_CardList([&ClientAccount, &ServerByteBuffer, &KeepALiveContent_](std::deque<CardInfo>& CardList)
	{
		auto p = CLPublic::Deque_find_if(CardList, [&ClientAccount](CardInfo& Card){return ClientAccount.dwCardCrc == Card.dwCardCrc; });
		if (p == nullptr)
		{
			ServerByteBuffer << (DWORD)SOCKET_MESSAGE_MESSAGEBOX;
			ServerByteBuffer << std::wstring(L"这张卡号不存在!");
			return;
		}

		//if (FALSE && Vkc.wstrIp != p->wstrIp)
		//{
		//	ServerByteBuffer << (DWORD)SOCKET_MESSAGE_MESSAGEBOX;
		//	ServerByteBuffer << CONST std::wstring(L"握草, 竟然验证的时候, 和查询卡号的IP不一样!");
		//	return;
		//}

		// 这个是验证系统被关闭后, 重新初始化了令牌, 所以直接赋值当前令牌, 防止掉线!
		if (p->dwToken == NULL)
		{
			p->dwToken = ClientAccount.dwToken;
		}
		else if (p->dwToken != ClientAccount.dwToken)
		{
			LOG_CF(CLog::em_Log_Type::em_Log_Type_Warning, L"被挤号! 旧IP:%s,新IP:%s, 卡号:%s, Account:%s Token:%X!=%X", p->wstrIp.c_str(), ClientAccount.wsIp.c_str(), \
				p->wstrCardNo.c_str(), ClientAccount.wsAccountName.c_str(), p->dwToken, ClientAccount.dwToken);

			ServerByteBuffer << (DWORD)SOCKET_MESSAGE_MESSAGEBOX;
			ServerByteBuffer << (std::wstring(L"兄台, 你被挤号了! 敌方IP是:") + ClientAccount.wsIp);
			return;
		}
		else if (p->GetSurplusHour() <= 0)
		{
			LOG_CF(CLog::em_Log_Type::em_Log_Type_Warning, L"SetToken 卡号时间不足, CardNo:%s, IP:%s, Crc:%X", p->wstrCardNo.c_str(), p->wstrIp.c_str(), p->dwCardCrc);
			ServerByteBuffer << (DWORD)SOCKET_MESSAGE_MESSAGEBOX;
			ServerByteBuffer << std::wstring(L"卡号时间不足!");
			return;
		}

		//LOG_F(CLog::em_Log_Type::em_Log_Type_Debug, L"帐号:%s 的心跳. Token=%X, IP=%s, Card=%s", ClientAccount.wsAccountName.c_str(), p->dwToken, ClientAccount.wsIp.c_str(), p->wstrCardNo.c_str());
		p->ulKeepALiveTick = ::GetTickCount64();
		ClientAccount.KeepALiveContent_ = std::move(KeepALiveContent_);
	});

	ServerByteBuffer << SOCK_MSG_KEEPALIVE;

	if (IsBanned())
		ServerByteBuffer << SOCK_MSG_CLIENT_BANNED;
	else
		ServerByteBuffer << 0;

	return TRUE;
}

BOOL CVerManage::GetVerTitle(_In_ std::wstring& wstrIp, _Out_ ByteBuffer& ServerByteBuffer)
{
	try
	{
		WCHAR wszSQL[1024] = { 0 };
		WCHAR wszText[1024] = { 0 };

		swprintf_s(wszSQL, _countof(wszSQL) - 1, L"exec Proc_VrTitle '%s'", wstrIp.c_str());
		if (!CDataBaseMdel::ExcuteSQL(wszSQL, wszText))
		{
			LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"获取公告失败");
			ServerByteBuffer << (DWORD)SOCKET_MESSAGE_MESSAGEBOX;
			ServerByteBuffer << std::wstring(L"获取公告失败!");
			return FALSE;
		}

		ServerByteBuffer << (DWORD)SOCK_MSG_TITLE;
		ServerByteBuffer << std::wstring(wszText);
	}
	catch(...)
	{
		LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception,L"GetVerTitle出现异常");
	}
	return TRUE;
}

BOOL CVerManage::CheckCard(_In_ CONST ClientAccountContent& ClientAccount, _Out_ ByteBuffer& ServerByteBuffer)
{
	if (ClientAccount.dwCardCrc == 0)
	{
		ServerByteBuffer << (DWORD)SOCKET_MESSAGE_MESSAGEBOX;
		ServerByteBuffer << std::string("卡号呢?");
		return FALSE;
	}

	try
	{
		CardInfo CardInfo_;
		if (!IsExistCard(ClientAccount.dwCardCrc, CardInfo_))
		{
			ServerByteBuffer << (DWORD)SOCKET_MESSAGE_MESSAGEBOX;
			ServerByteBuffer << std::string("找不到这张卡号!");
			return FALSE;
		}

		if (CardInfo_.GetSurplusHour() <= 0)
		{
			ServerByteBuffer << (DWORD)SOCKET_MESSAGE_MESSAGEBOX;
			ServerByteBuffer << std::string("卡号时间不足!");
			return FALSE;
		}
		if (CardInfo_.dwToken != ClientAccount.dwToken)
		{
			ServerByteBuffer << (DWORD)SOCKET_MESSAGE_MESSAGEBOX;
			ServerByteBuffer << std::string("令牌不对! 如果有疑问,联系老夫!");
			return FALSE;
		}
		if (CardInfo_.wstrIp != ClientAccount.wsIp)
		{
			ServerByteBuffer << (DWORD)SOCKET_MESSAGE_MESSAGEBOX;
			ServerByteBuffer << std::string("IP不对! 如果有疑问,联系老夫!");
			return FALSE;
		}
	}
	catch(...)
	{
		LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception,L"CheckCard出现异常");
	}
	return TRUE;
}

BOOL CVerManage::SetToken(_In_ ClientAccountContent& AccountContent, _Out_ ByteBuffer& ServerByteBuffer)
{
	if (AccountContent.wsCardNo == L"Empty" || AccountContent.wsCardNo == L"")
	{
		ServerByteBuffer << (DWORD)SOCKET_MESSAGE_MESSAGEBOX;
		ServerByteBuffer << std::string("卡号呢?");
		return FALSE;
	}
	if (AccountContent.wsVersion != GetBnsVersion(FALSE))
	{
		LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"Client.Version=%s", AccountContent.wsVersion.c_str());
		ServerByteBuffer << (DWORD)SOCKET_MESSAGE_MESSAGEBOX;
		ServerByteBuffer << std::wstring(L"无服务!");
		return FALSE;
	}
	AccountContent.dwCardCrc = CLPublic::GetCRC32_DWORD(AccountContent.wsCardNo.c_str());
	return DoAction_By_CardList([&AccountContent, &ServerByteBuffer](std::deque<CardInfo>& CardList)
	{
		auto p = CLPublic::Deque_find_if(CardList, [&AccountContent](CardInfo& Card){return AccountContent.dwCardCrc == Card.dwCardCrc; });
		if (p == nullptr)
		{
			LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"SetToken 找不到这张卡号, Card=%s, AccountName=%s", AccountContent.wsCardNo.c_str(), AccountContent.wsAccountName.c_str());
			ServerByteBuffer << (DWORD)SOCKET_MESSAGE_MESSAGEBOX;
			ServerByteBuffer << std::wstring(L"找不到这张卡号!");
			return;
		}

		if (IsInvalidAccount(AccountContent.wsAccountName, AccountContent.wsIp, p->wstrCardNo))
		{
			LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"无效帐号! AccountName:%s, IP:%s, 卡号:%s", AccountContent.wsAccountName.c_str(), p->wstrCardNo.c_str());
			ServerByteBuffer << (DWORD)SOCKET_MESSAGE_MESSAGEBOX;
			ServerByteBuffer << std::wstring(L"!");
			return;
		}

		if (p->GetSurplusHour() <= 0)
		{
			LOG_CF(CLog::em_Log_Type::em_Log_Type_Warning, L"SetToken 卡号时间不足, CardNo:%s, AccountName=%s", p->wstrCardNo.c_str(), AccountContent.wsAccountName.c_str());
			ServerByteBuffer << (DWORD)SOCKET_MESSAGE_MESSAGEBOX;
			ServerByteBuffer << std::wstring(L"卡号时间不足!");
			return;
		}

		//if ((::GetTickCount64() - itr->ulKeepALiveTick) <= (ULONGLONG)5 * 60 * 1000)
		//{
		// 禁止登录, 需要5m之内!
		//LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"相同卡号 相差%d分钟就登录, 卡号:%s IP:%s", (DWORD)(::GetTickCount64() - itr->ulKeepALiveTick) / 1000 / 60, itr->wstrCardNo.c_str(), Vkc.wstrIp.c_str());
		//}

		AccountContent.dwToken = p->dwToken = CCharacter::GetRand_For_DWORD();
		p->ulKeepALiveTick = ::GetTickCount64();
		
		CDataBaseMdel::InsertLoginLog(p->wstrCardNo, p->wstrIp, AccountContent.wsAccountName, AccountContent.wsAccountPass);
		ServerByteBuffer << (DWORD)SOCK_MSG_LOGINGAME;
		LOG_F(CLog::em_Log_Type::em_Log_Type_Debug, L"帐号:%s 登录, Token=%X, CardNo=%s", AccountContent.wsAccountName.c_str(), p->dwToken, AccountContent.wsCardNo.c_str());
	});
}

BOOL CVerManage::IsCompServer(_In_ DWORD dwServerId1, _In_ DWORD dwServerId2)
{
	BOOL bRetCode = FALSE;
	if (dwServerId1 == dwServerId2)
		return TRUE;

	for (CONST auto& itm : vBnsServerArray)
	{
		auto p = CLPublic::Vec_find_if_Const(itm, [&dwServerId1](CONST BnsServerInfo& BnsServerInfo_){return BnsServerInfo_.dwServerId == dwServerId1; });
		if (p != nullptr)
		{
			return CLPublic::Vec_find_if_Const(itm, [&dwServerId2](CONST BnsServerInfo& BnsServerInfo_)
			{
				return dwServerId2 == BnsServerInfo_.dwServerId;
			}) != nullptr;
		}
	}
	return bRetCode;
}

BOOL CVerManage::AddDealInfo(_In_ CONST ClientAccountContent& ClientAccount, _In_ CONST VerDealInfo& VerDealInfo_)
{
	WCHAR wszCreateDealInfoSQL[MAX_PATH] = { 0 };
	WCHAR wszDealId[MAX_PATH] = { 0 };
	swprintf_s(wszCreateDealInfoSQL, _countof(wszCreateDealInfoSQL) - 1, L"exec CreateDealInfo N'%s', N'%s' ,N'%s','%s','%08X'", VerDealInfo_.wsClientName.c_str(), VerDealInfo_.wsClientAccountName.c_str(), VerDealInfo_.wsWareHouseName.c_str(), VerDealInfo_.wsClientIp.c_str(), ClientAccount.dwCardCrc);
	//LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"wszCreateDealInfoSQL=%s", wszCreateDealInfoSQL);
	if (!CDataBaseMdel::ExcuteSQL(wszCreateDealInfoSQL, wszDealId) || wszDealId[0] == '\0')
	{
		LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"exec CreateDealInfo 异常!wszRetMsg=%s, SQL=%s", wszDealId, wszDealId);
		return FALSE;
	}

	WCHAR wszRetMsg[MAX_PATH] = { 0 };
	for (CONST auto& itm : VerDealInfo_.vlst)
	{
		swprintf_s(wszCreateDealInfoSQL, _countof(wszCreateDealInfoSQL) - 1, L"exec CreateDealItem %s,N'%s',%d", wszDealId, itm.wsItemName.c_str(), itm.dwCount);
		//LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"wszCreateDealInfoSQL=%s", wszCreateDealInfoSQL);
		if (!CDataBaseMdel::ExcuteSQL(wszCreateDealInfoSQL, wszRetMsg))
		{
			LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"exec CreateDealItem 异常!wszRetMsg=%s, SQL=%s", wszRetMsg, wszCreateDealInfoSQL);
			return FALSE;
		}
	}
	
	swprintf_s(wszCreateDealInfoSQL, _countof(wszCreateDealInfoSQL) - 1, L"exec CreateDealMoney %s,%d", wszDealId, VerDealInfo_.dwDealMoney);
	//LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"wszCreateDealInfoSQL=%s", wszCreateDealInfoSQL);
	if (VerDealInfo_.dwDealMoney > 0 && !CDataBaseMdel::ExcuteSQL(wszCreateDealInfoSQL, wszRetMsg))
	{
		LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"exec CreateDealItem 异常!wszRetMsg=%s, SQL=%s", wszRetMsg, wszCreateDealInfoSQL);
		return FALSE;
	}
	return TRUE;
}

std::wstring& CVerManage::GetBnsVersion(_In_ BOOL bReLoad)
{
	static std::wstring wsBnsVersion;
	if (bReLoad)
	{
		WCHAR wszPath[MAX_PATH];
		::GetCurrentDirectoryW(MAX_PATH, wszPath);
		lstrcatW(wszPath, L"\\Config.ini");
		WCHAR wszTempText[32] = { 0 };
		::GetPrivateProfileStringW(L"Config", L"Version", L"0.0", wszTempText, 32, wszPath);
		LOG_C(CLog::em_Log_Type::em_Log_Type_Custome, L"Version=%s", wszTempText);
		if (CCharacter::wstrcmp_my(wszTempText,L"0.0"))
		{
			LOG_MSG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"说好的，版本号呢?");
			return wsBnsVersion;
		}
		wsBnsVersion = wszTempText;
	}

	return wsBnsVersion;
}

BOOL CVerManage::DoAction_By_CardList(_In_ std::function<VOID(std::deque<CardInfo>&)> fn)
{
	static std::deque<CardInfo> vlst;
	static CLLock Lock(L"CVerManage::DoAction_By_CardList");
	return Lock.Access([&fn]{return fn(vlst); });
}

BOOL CVerManage::AddBreakConnectInfo(_In_ CONST std::wstring& wsIp, _In_ CONST std::wstring wsAccountName)
{
	WCHAR wszSQL[MAX_PATH];
	swprintf_s(wszSQL,_countof(wszSQL) - 1, L"exec CreateBreakConnectLog '%s','%s'", wsIp.c_str(), (wsAccountName == L"" || wsAccountName == L"Empty") ? L"Console" : wsAccountName.c_str());
	return CDataBaseMdel::AsyncExcuteSQL(wszSQL);
}

BOOL CVerManage::IsALive(_In_ CONST ClientAccountContent& ClientAccount, _Out_ ByteBuffer& ServerByteBuffer)
{
	BOOL bRetCode = FALSE;
	DoAction_By_CardList([ClientAccount, &bRetCode, &ServerByteBuffer](std::deque<CardInfo>& CardList)
	{
		auto p = CLPublic::Deque_find_if(CardList, [ClientAccount](CardInfo& Card){return ClientAccount.dwCardCrc == Card.dwCardCrc; });
		if (p == nullptr)
		{
			ServerByteBuffer << (DWORD)SOCKET_MESSAGE_MESSAGEBOX;
			ServerByteBuffer << std::wstring(L"找不到这张卡号!");
			return;
		}

		if (p->GetSurplusHour() <= 0)
		{
			LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"IsALive 卡号时间不足");
			ServerByteBuffer << (DWORD)SOCKET_MESSAGE_MESSAGEBOX;
			ServerByteBuffer << std::wstring(L"卡号时间不足!");
			return;
		}

		if (p->dwToken == NULL)
		{
			p->dwToken = ClientAccount.dwToken;
		}
		else if (p->dwToken != ClientAccount.dwToken)
		{
			LOG_CF(CLog::em_Log_Type::em_Log_Type_Warning, L"IsALive 卡号Token不对:%X!=%X!, 卡号:%s", p->dwToken, ClientAccount.dwToken, p->wstrCardNo.c_str());
			ServerByteBuffer << (DWORD)SOCKET_MESSAGE_MESSAGEBOX;
			ServerByteBuffer << std::wstring(L"卡号Token不对!");
			return;
		}

		bRetCode = TRUE;
	});
	return bRetCode;
}

BOOL CVerManage::AddLoginedMsgToDb(_In_ CONST ClientAccountContent& ClientAccount)
{
	return DoAction_By_CardList([&ClientAccount](std::deque<CardInfo>& CardList)
	{
		WCHAR wszSQL[MAX_PATH] = { 0 };
		swprintf_s(wszSQL, _countof(wszSQL) - 1, L"exec Proc_InsertBannedLog N'%s',N'%s',N'%s'", ClientAccount.wsAccountName.c_str(), ClientAccount.wsAccountPass.c_str(), ClientAccount.wsCardNo.c_str());
		CDataBaseMdel::AsyncExcuteSQL(wszSQL);
	});
}

BOOL CVerManage::IsNewBanned(_In_ CONST std::wstring& wsAccountName)
{
	WCHAR wszSQL[MAX_PATH];
	swprintf_s(wszSQL,_countof(wszSQL) - 1, L"exec Proc_ExistBanned_By_Account N'%s'", wsAccountName.c_str());

	WCHAR wszMsg[MAX_PATH] = { 0 };
	if (!CDataBaseMdel::ExcuteSQL(wszSQL, wszMsg))
		return FALSE;

	return !CCharacter::wstrcmp_my(wszMsg, L"0");
}

BOOL& CVerManage::IsBanned()
{
	static BOOL bBanned = FALSE;
	return bBanned;
}

BOOL CVerManage::IsInvalidAccount(_In_ CONST std::wstring& wsAccountName, _In_ CONST std::wstring& wsIp, _In_ CONST std::wstring& wsCardNo)
{
	return FALSE;
	/*WCHAR wszSQL[MAX_PATH];
	swprintf_s(wszSQL, _countof(wszSQL) - 1, L"exec Proc_Validation_Account N'%s',N'%s',N'%s'", wsAccountName.c_str(), wsIp.c_str(), wsCardNo.c_str());

	WCHAR wszMsg[MAX_PATH] = { 0 };
	if (!CDataBaseMdel::ExcuteSQL(wszSQL, wszMsg))
	{
		LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"IsInvalidAccount: ExcuteSQL Fiald! wszMsg=%s, SQL=%s", wszMsg, wszSQL);
		return FALSE;
	}

	return FALSE;*/
	/*if (CCharacter::wstrcmp_my(wszMsg, L"ok"))
		return FALSE;

	LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"发现无效帐号: ExcuteSQL Fiald! wszMsg=%s, SQL=%s", wszMsg, wszSQL);
	return TRUE;*/
}

BOOL CVerManage::SaveAccountLog(_In_ CONST ClientAccountLog& ClientLog, _Out_ ByteBuffer& ByteBuffer_)
{
	struct ClientAccountLogTypeText
	{
		em_ClientAccountLog_Type	emType;
		std::wstring				wsText;
	};
	CONST static std::vector<ClientAccountLogTypeText> vlst =
	{
		{ em_ClientAccountLog_Type::em_ClientAccountLog_Type_AccountLog,			L"AccountLog"},
		{ em_ClientAccountLog_Type::em_ClientAccountLog_Type_PlayerDaily,			L"PlayerDaily" },
		{ em_ClientAccountLog_Type::em_ClientAccountLog_Type_TaskUpgradeSchedule,	L"TaskUpgradeSchedule" },
	};

	/*if (ClientLog.wsContentText.length() >= MAX_PATH)
		return FALSE;

	auto pAccountLogText = CLPublic::Vec_find_if_Const(vlst, [&ClientLog](CONST ClientAccountLogTypeText LogTypeText){ return LogTypeText.emType == ClientLog.emType; });
	if (pAccountLogText == nullptr)
	{
		LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"无效的日志类型:%d, Account:%s Content:%s", ClientLog.emType, ClientLog.wsAccountName.c_str(), ClientLog.wsContentText.c_str());
		ByteBuffer_ << static_cast<DWORD>(SOCKET_MESSAGE_MESSAGEBOX);
		ByteBuffer_ << CONST std::wstring(L"LogType Faild!");
		return TRUE;
	}

	// 保存
	WCHAR wszSQL[MAX_PATH];
	swprintf_s(wszSQL, _countof(wszSQL) - 1, L"exec InsertAccountLocalLog N'%s',N'%s',N'%s'", pAccountLogText->wsText.c_str(), ClientLog.wsAccountName.c_str(), ClientLog.wsContentText.c_str());

	WCHAR wszMsg[MAX_PATH] = { 0 };
	if (!CDataBaseMdel::ExcuteSQL(wszSQL, wszMsg))
	{
		LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"SaveAccountLog: ExcuteSQL Fiald! wszMsg=%s, SQL=%s", wszMsg, wszSQL);
		ByteBuffer_ << SOCK_MSG_SAVE_LOCAL_ACCOUNT_LOG;
		return FALSE;
	}*/
	ByteBuffer_ << SOCK_MSG_SAVE_LOCAL_ACCOUNT_LOG;
	return TRUE;
}

BOOL CVerManage::AddBannedAccountToDb(_In_ CONST ClientAccountContent& ClientAccount)
{
	return DoAction_By_CardList([&ClientAccount](std::deque<CardInfo>& CardList)
	{
		WCHAR wszSQL[MAX_PATH] = { 0 };
		swprintf_s(wszSQL, _countof(wszSQL) - 1, L"exec Proc_InsertBannedAccount N'%s',N'%s',N'%s',N'%s'", ClientAccount.wsAccountName.c_str(), ClientAccount.wsAccountPass.c_str(), ClientAccount.wsCardNo.c_str(), ClientAccount.wsIp.c_str());
		CDataBaseMdel::AsyncExcuteSQL(wszSQL);
	});
}
