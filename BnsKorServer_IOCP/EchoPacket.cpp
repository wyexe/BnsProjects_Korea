#include "EchoPacket.h"
#include <MyTools/Character.h>
#include <MyTools/CLPublic.h>
#include <MyTools/CLEchoException.h>
#include <MyTools/Log.h>
#include "CardExtend.h"
#include "GameClient.h"
#include "ScriptFile.h"
#include "DbManager.h"
#include "BnsTeam.h"
#include "BnsTrade.h"
#include "BnsOTP.h"
#include "DailyTask.h"
#include "CloudConfigExtend.h"
#include "CrossAreaTeam.h"

#define _SELF L"EchoPacket.cpp"
BOOL CEchoPacket::QueryCard(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	// Extract
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&] 
	{
		auto VecCardCrc = ExtractPacket<std::vector<DWORD>>([&pSocketBuffer]
		{
			DWORD dwCount = 0;
			std::vector<DWORD> VecCardCrc;

			*pSocketBuffer >> dwCount;
			for (DWORD i = 0; i < dwCount && i <= 100; ++i)
			{
				DWORD dwValue = 0;
				*pSocketBuffer >> dwValue;
				VecCardCrc.push_back(std::move(dwValue));
			}
			pSocketBuffer->clear();
			return VecCardCrc;
		});

		pSocketBuffer->InitializeHead<em_Sock_Msg>(em_Sock_Msg::em_Sock_Msg_QUERY_CARDINFO);
		for (CONST auto& itm : VecCardCrc)
		{
			CardInfo Card;
			//LOG_C_D(L"CardCrc=%X", itm);
			if (!CCardExtend::GetInstance().FindCard_By_CRC(itm, Card))
			{
				// Try to Active Card
				//LOG_C_D(L"Try Active Card [%X]", itm);
				if (!CCardExtend::GetInstance().TryActiveCard(GameClient.GetAccountContent(), itm, Card))
				{
					*pSocketBuffer << FALSE;
					continue;
				}
			}

			//LOG_C_D(L"Card Crc=%X, CardNo=%s", itm, Card.wsCardNo.c_str());
			*pSocketBuffer << TRUE << Card.nMaxHour << Card.GetSurplusHour() << Card.wsActiveTime;
		}

		return TRUE;
	});
}

BOOL CEchoPacket::KeepALive(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		BOOL bEmpty = FALSE;
		auto Content = ExtractPacket<KeepALiveContent>([&pSocketBuffer, &bEmpty]
		{
			KeepALiveContent Content;
			*pSocketBuffer >> reinterpret_cast<DWORD&>(bEmpty);
			if (bEmpty)
			{
				*pSocketBuffer >> Content.Level >> Content.Gold >> Content.dwTaskId >> Content.dwScheduleId >> Content.wsContent;
			}

			pSocketBuffer->clear();
			return Content;
		});

		// 
		CCardExtend::GetInstance().SetCardContent(GameClient.GetAccountContent().dwCardCrc, [](CardInfo& Card) { Card.ulKeepALiveTick = ::GetTickCount64(); });
		GameClient.GetAccountContent().ulKeepALiveTick = ::GetTickCount64();

		// Check wsContent is Empty
		GameClient.GetAccountContent().ulKeepALiveTick = ::GetTickCount64();
		pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_KEEPALIVE);
		*pSocketBuffer << _bBanned;
		return TRUE;
	});
}

BOOL CEchoPacket::DownloadScript(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		auto wsScriptName = ExtractPacket<std::wstring>([&pSocketBuffer]
		{
			std::wstring wsScriptName;
			*pSocketBuffer >> wsScriptName;
			pSocketBuffer->clear();

			return wsScriptName;
		});

		if (wsScriptName.find(L".inf") == -1)
			wsScriptName += L".inf";

		CScriptFile::CipherScriptText ScriptContent;
		if (!CScriptFile::GetInstance().FindScriptText_By_ScriptName(wsScriptName, ScriptContent))
		{
			pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_ErrorText);
			*pSocketBuffer << MyTools::CCharacter::MakeFormatText(L"脚本:%s 并没有存在服务器上!", wsScriptName.c_str());
			return TRUE;
		}

		pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_QUERY_SCRIPTKEY);
		*pSocketBuffer << ScriptContent.wsScriptName << ScriptContent.uExpressSize/*明文大小*/ << ScriptContent.uCipherTextSize/*密文大小*/ << ScriptContent.dwExpressTextCrc/*明文CRC*/ << ScriptContent.dwCipherTextCrc/*密文CRC*/;
		*pSocketBuffer << GameClient.GetAccountContent().wsAccountName;

		//LOG_CF_D(L"Server:Name=%s << %d << %d << %X << %X", ScriptContent.wsScriptName.c_str(), ScriptContent.uExpressSize, ScriptContent.uCipherTextSize, ScriptContent.dwExpressTextCrc, ScriptContent.dwCipherTextCrc);
		pSocketBuffer->SetDataPtr(ScriptContent.pScriptBuffer.get(), ScriptContent.uCipherTextSize);
		return TRUE;
	});
}

BOOL CEchoPacket::GameLogin(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		auto& Account = GameClient.GetAccountContent();
		ExtractPacket<int>([&]
		{
			float fVersion = 0;
			*pSocketBuffer >> fVersion >> Account.wsAccountName >> Account.wsAccountPass >> Account.wsCardNo;
			Account.wsVersion = std::to_wstring(fVersion);

			pSocketBuffer->clear();
			return TRUE;
		});

		//LOG_C_D(L"Start Login[%s,%s], Socket=[%X]", Account.wsAccountName.c_str(), Account.wsAccountPass.c_str(), GameClient.GetSocket());
		auto bExist = CCardExtend::GetInstance().SetCardContent(MyTools::CLPublic::GetCRC32_DWORD(Account.wsCardNo.c_str()), [&Account](CardInfo& Card)
		{
			Card.dwToken = Account.dwToken = MyTools::CCharacter::GetRand_For_DWORD();
			Card.ulKeepALiveTick = ::GetTickCount64();
			Card.wsAccountName = Account.wsAccountName;
			Card.wsIp = Account.wsIp;
			Card.bExit = TRUE;
			Account.dwCardCrc = Card.dwCardCrc;
		});


		if (!bExist)
		{
			pSocketBuffer->InitializeHead(em_Sock_Msg_ErrorText);
			*pSocketBuffer << MyTools::CCharacter::MakeFormatText(L"此卡号:%s 不存在!", Account.wsCardNo.c_str());
			return TRUE;
		}

		return TRUE;
	});
}

BOOL CEchoPacket::Announcement(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		std::wstring wsTitle;
		CBnsDbManager::GetInstance().GetClientTitle(GameClient.GetAccountContent().wsIp, wsTitle);

		pSocketBuffer->clear();
		pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_TITLE);

		*pSocketBuffer << wsTitle;
		return TRUE;
	});
}

BOOL CEchoPacket::JoinTeam_MainLine(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		bool bExistData = false;
		*pSocketBuffer >> bExistData;
		if (bExistData)
		{
			GameClient.GetClientTeamInfo()->SelfCti.wsAccountName = GameClient.GetAccountContent().wsAccountName;
			GameClient.GetClientTeamInfo()->SelfCti.wsPlayerName = GameClient.GetAccountContent().wsPlayerName;
			ExtractTeamPacket(pSocketBuffer, GameClient.GetClientTeamInfo());
		}

		CBnsTaskTeam::BnsTeamResult Result;
		CBnsTaskTeam::GetInstance().JoinTaskTeam(*GameClient.GetClientTeamInfo().get(), Result);

		pSocketBuffer->clear();
		pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_JOIN_TEAM_ORDER);
		*pSocketBuffer << Result.Captain.wsPlayerName << Result.Captain.dwPlayerId << Result.dwTeamId << static_cast<DWORD>(Result.VecTeamMember.size());
		for (CONST auto& itm : Result.VecTeamMember)
			*pSocketBuffer << itm.wsPlayerName << itm.dwPlayerId;

		return TRUE;
	});
}

BOOL CEchoPacket::LeaveTeam_MainLine(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	pSocketBuffer->clear();
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		CBnsTaskTeam::GetInstance().DeleteTaskMember(GameClient.GetAccountContent().wsAccountName);
		return TRUE;
	});
}

BOOL CEchoPacket::JoinTeam_Gungoen(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		bool bExistData = false;
		*pSocketBuffer >> bExistData;
		if (bExistData)
		{
			GameClient.GetClientTeamInfo()->SelfCti.wsAccountName = GameClient.GetAccountContent().wsAccountName;
			GameClient.GetClientTeamInfo()->SelfCti.wsPlayerName = GameClient.GetAccountContent().wsPlayerName;
			ExtractTeamPacket(pSocketBuffer, GameClient.GetClientTeamInfo());
		}


		CBnsTaskTeam::BnsTeamResult Result;
		CBnsTaskTeam::GetInstance().JoinDungoenTeam(*GameClient.GetClientTeamInfo().get(), Result);

		pSocketBuffer->clear();
		pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_JOIN_DUNGOEN_TEAM);
		*pSocketBuffer << Result.Captain.wsPlayerName << Result.Captain.dwPlayerId << Result.dwTeamId << static_cast<DWORD>(Result.VecTeamMember.size());
		for (CONST auto& itm : Result.VecTeamMember)
			*pSocketBuffer << itm.wsPlayerName << itm.dwPlayerId;

		return TRUE;
	});
}

BOOL CEchoPacket::LeaveTeam_Gungoen(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	pSocketBuffer->clear();
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		CBnsTaskTeam::GetInstance().DeleteDungoenTeamMember(GameClient.GetAccountContent().wsAccountName);
		return TRUE;
	});
}

BOOL CEchoPacket::ItemSupport_GameAccount(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		VerItemSupport Content;
		Content.wsAccountName = GameClient.GetAccountContent().wsAccountName;
		Content.wsPlayerName = GameClient.GetAccountContent().wsPlayerName;

		ExtractClientTradePacket(pSocketBuffer, Content);

		ItemSupportResult Result;
		CBnsTrade::GetInstance().JoinItemSupportDeal(Content, Result);

		pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_CLIENT_ITEMSUPPORT);


		*pSocketBuffer << (Result.emDealStatus == em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_None ? 0 : 1);
		if (Result.emDealStatus == em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_Channel)
			*pSocketBuffer << Result.wsWareHouseName << Result.dwChannel;

		return TRUE;
	});
}

BOOL CEchoPacket::PickItem_GameAccount(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		VerItemSupport Content;
		Content.wsAccountName = GameClient.GetAccountContent().wsAccountName;
		Content.wsPlayerName = GameClient.GetAccountContent().wsPlayerName;
		ExtractClientTradePacket(pSocketBuffer, Content);

		ItemSupportResult Result;
		CBnsTrade::GetInstance().JoinPickItemtDeal(Content, Result);

		pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_CLIENT_PICKITEM);


		*pSocketBuffer << (Result.emDealStatus == em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_None ? 0 : 1);
		if (Result.emDealStatus == em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_Channel)
			*pSocketBuffer << Result.wsWareHouseName << Result.dwChannel;

		return TRUE;
	});
}

BOOL CEchoPacket::ItemSupport_Warehouse(_In_ CGameClient&, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		auto WareHouseInfo_ = ExtractPacket<WareHouseInfo>([&pSocketBuffer]
		{
			WareHouseInfo Info;
			*pSocketBuffer >> Info.wsWareHouseName;
			*pSocketBuffer >> Info.dwChannel;
			*pSocketBuffer >> Info.dwServerId;

			pSocketBuffer->clear();
			return Info;
		});

		WarehouseActionResult Result;
		CBnsTrade::GetInstance().AcceptItemSupportDeal(WareHouseInfo_, Result);

		pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_CLIENT_WAREHOUSE_MAINTASK);
		*pSocketBuffer << static_cast<DWORD>(Result.emWarehouseType);
		*pSocketBuffer << static_cast<DWORD>(Result.emIsPick);
		*pSocketBuffer << Result.wsPlayerName;

		return TRUE;
	});
}

BOOL CEchoPacket::PickItem_Warehouse(_In_ CGameClient&, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		auto WareHouseInfo_ = ExtractPacket<WareHouseInfo>([&pSocketBuffer]
		{
			WareHouseInfo Info;
			*pSocketBuffer >> Info.wsWareHouseName;
			*pSocketBuffer >> Info.dwChannel;
			*pSocketBuffer >> Info.dwServerId;

			pSocketBuffer->clear();
			return Info;
		});

		WarehouseActionResult Result;
		CBnsTrade::GetInstance().AcceptPickItemDeal(WareHouseInfo_, Result);

		pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_CLIENT_WAREHOUSE_PICKITEM);
		*pSocketBuffer << static_cast<DWORD>(Result.emWarehouseType);
		*pSocketBuffer << static_cast<DWORD>(Result.emIsPick);
		*pSocketBuffer << Result.wsPlayerName;

		return TRUE;
	});
}

BOOL CEchoPacket::SetGungoenSchedule(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		auto Msg = ExtractPacket<TeamScheduleMsg>([&pSocketBuffer, &GameClient]
		{
			TeamScheduleMsg Msg;
			*pSocketBuffer >> Msg.dwTeamId >> Msg.wsScheduleName >> Msg.uScheduleIndex;

			Msg.wsAccountName = GameClient.GetAccountContent().wsAccountName;
			pSocketBuffer->clear();
			return Msg;
		});

		CBnsTaskTeam::GetInstance().UpdateDungoenSchedule(Msg);
		return TRUE;
	});
}

BOOL CEchoPacket::GetGungoenSchedule(_In_ CGameClient&, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		auto Msg = ExtractPacket<TeamScheduleMsg>([&pSocketBuffer]
		{
			TeamScheduleMsg Msg;
			*pSocketBuffer >> Msg.dwTeamId;

			pSocketBuffer->clear();
			return Msg;
		});

		std::vector<BnsDungoenScheduleMember> VecResult;
		CBnsTaskTeam::GetInstance().GetDungoenSchedule(Msg, VecResult);

		pSocketBuffer->InitializeHead(em_Sock_Msg_GET_TEAM_SCHEDULE);
		*pSocketBuffer << VecResult.size();
		for (CONST auto& itm : VecResult)
		{
			*pSocketBuffer << itm.wsAccountName
				<< itm.wsPlayerName
				<< itm.dwPlayerId
				<< itm.wsDungoenSchedule
				<< itm.uGatherIndex
				<< itm.dwTaskId
				<< itm.dwScheduleId;
		}

		return TRUE;
	});
}

BOOL CEchoPacket::SetTeamReadonly(_In_ CGameClient&, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		auto Msg = ExtractPacket<TeamScheduleMsg>([&pSocketBuffer]
		{
			TeamScheduleMsg Msg;
			*pSocketBuffer >> Msg.dwTeamId;

			pSocketBuffer->clear();
			return Msg;
		});

		CBnsTaskTeam::GetInstance().SetReadOnlyTeam(Msg.dwTeamId);
		return TRUE;
	});
}

BOOL CEchoPacket::SaveTradeData(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		auto Content = ExtractPacket<TradeContent>([&pSocketBuffer]
		{
			DWORD dwCount = 0;
			TradeContent Content;
			*pSocketBuffer >> Content.wsWareHouseName >> Content.dwTradeMoney >> dwCount;
			for (DWORD i = 0; i < 100 && i < dwCount; ++i)
			{
				TradeItem Item;
				*pSocketBuffer >> Item.wsItemName >> Item.dwCount;
				Content.VecTradeItem.push_back(std::move(Item));
			}

			pSocketBuffer->clear();
			return Content;
		});

		CBnsDbManager::GetInstance().AddTradeContentToDb(GameClient.GetAccountContent(), Content);
		return TRUE;
	});
}

BOOL CEchoPacket::SaveLoginData(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		auto& Account = GameClient.GetAccountContent();
		*pSocketBuffer >> Account.wsPlayerName;
		pSocketBuffer->clear();

		CCardExtend::GetInstance().SetCardContent(MyTools::CLPublic::GetCRC32_DWORD(Account.wsCardNo.c_str()), [Account](CardInfo& Card)
		{
			Card.wsPlayerName = Account.wsPlayerName;
		});

		CBnsDbManager::GetInstance().AddLoginLogToDb(GameClient.GetAccountContent());
		return TRUE;
	});
}

BOOL CEchoPacket::Banned(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	pSocketBuffer->clear();
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		LOG_CF_D(L"Banned Account[%s]", GameClient.GetAccountContent().wsAccountName.c_str());
		if (CBnsDbManager::GetInstance().IsNewBanned(GameClient.GetAccountContent().wsAccountName))
		{
			LOG_CF_D(L"Add Account[%s] to DB", GameClient.GetAccountContent().wsAccountName.c_str());
			CBnsDbManager::GetInstance().AddBannedRecordToDb(GameClient.GetAccountContent());
		}
		return TRUE;
	});
}

BOOL CEchoPacket::GetLessPersonChannel(_In_ CGameClient&, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		auto Msg = ExtractPacket<TeamChannelMsg>([&pSocketBuffer]
		{
			TeamChannelMsg Msg;
			*pSocketBuffer >> Msg.dwServerId >> Msg.dwChannel >> Msg.dwMaxChannel >> Msg.dwTaskId >> Msg.dwScheduleId;

			pSocketBuffer->clear();
			return Msg;
		});

		DWORD dwChannel = 0;
		CBnsTaskTeam::GetInstance().GetLessMemberChannel(Msg, dwChannel);

		pSocketBuffer->InitializeHead(em_Sock_Msg_GET_CHANNEL);
		*pSocketBuffer << dwChannel;
		return TRUE;
	});
}

BOOL CEchoPacket::GetCouldConfigValue(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		auto Content = ExtractPacket<ClientConfigContent>([&pSocketBuffer, &GameClient]
		{
			ClientConfigContent Content;
			*pSocketBuffer >> Content.wsPlayerName >> Content.ConfigContent_.wsConfigName;

			Content.wsAccountName = GameClient.GetAccountContent().wsAccountName;
			pSocketBuffer->clear();
			return Content;
		});

		std::wstring wsValue;
		if (!CCloudConfigExtend::GetInstance().FindConfigValue(Content, wsValue))
			wsValue = L"0";

		pSocketBuffer->InitializeHead(em_Sock_Msg_GET_CONFIG_VALUE);
		*pSocketBuffer << wsValue;

		return TRUE;
	});
}

BOOL CEchoPacket::SetCouldConfigValue(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		auto Content = ExtractPacket<ClientConfigContent>([&pSocketBuffer, &GameClient]
		{
			ClientConfigContent Content;
			*pSocketBuffer >> Content.ConfigContent_.wsConfigName >> Content.ConfigContent_.wsConfigValue;

			Content.wsAccountName = GameClient.GetAccountContent().wsAccountName;
			Content.wsPlayerName = GameClient.GetAccountContent().wsPlayerName;
			pSocketBuffer->clear();
			return Content;
		});

		CCloudConfigExtend::GetInstance().WriteConfigValue(Content);
		return TRUE;
	});
}

BOOL CEchoPacket::ConsoleKeepALive(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	pSocketBuffer->clear();
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		GameClient.GetAccountContent().ulKeepALiveTick = ::GetTickCount64();
		return TRUE;
	});
}

BOOL CEchoPacket::SaveAccountLogData(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		auto AccountLog = ExtractPacket<ClientAccountLog>([&pSocketBuffer]
		{
			ClientAccountLog AccountLog;
			*pSocketBuffer >> reinterpret_cast<DWORD&>(AccountLog.emType) >> AccountLog.wsContentText;


			pSocketBuffer->clear();
			return AccountLog;
		});

		CBnsDbManager::GetInstance().AddAccountLogToDb(GameClient.GetAccountContent(), AccountLog);
		return TRUE;
	});
}

BOOL CEchoPacket::CalculateOTP(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		pSocketBuffer->clear();

		std::wstring wsKey;
		if (!CBnsOTP::GetInstance().FindOTPKey_By_AccountName(GameClient.GetAccountContent().wsAccountName, wsKey))
		{
			pSocketBuffer->InitializeHead(em_Sock_Msg_ErrorText);
			*pSocketBuffer << MyTools::CCharacter::MakeFormatText(L"帐号[%s] 不存在OTP的Key!", GameClient.GetAccountContent().wsAccountName.c_str());
			return TRUE;
		}

		DWORD dwValue = 0;
		CBnsOTP::GetInstance().CalculateOTP(wsKey, dwValue);

		pSocketBuffer->InitializeHead(em_Sock_Msg_OTP);
		*pSocketBuffer << dwValue;
		return TRUE;
	});
}

BOOL CEchoPacket::GetDailyTaskSchedule(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		std::vector<std::wstring> VecResult;
		CDailyTask::GetInstance().QueryDailyTaskSchedule(GameClient.GetAccountContent(), VecResult);

		pSocketBuffer->clear();
		pSocketBuffer->InitializeHead(em_Sock_Msg_QUERY_DAILYTASK_SCHEDULE);

		*pSocketBuffer << VecResult.size();
		for (CONST auto& itm : VecResult)
			*pSocketBuffer << itm;

		return TRUE;
	});
}

BOOL CEchoPacket::SetDailyTaskSchedule(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		std::wstring wsDailyTaskName;
		*pSocketBuffer >> wsDailyTaskName;
		pSocketBuffer->clear();

		CDailyTask::GetInstance().SetDailyTaskScheduleFinish(GameClient.GetAccountContent(), wsDailyTaskName);
		return TRUE;
	});
}

BOOL CEchoPacket::CheckCard(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		auto& Account = GameClient.GetAccountContent();
		if (Account.dwCardCrc == NULL)
		{
			pSocketBuffer->clear();
			pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_ErrorText);
			*pSocketBuffer << L"该帐号的CRC=0";
			return FALSE;
		}
		else if (Account.dwToken == NULL)
		{
			pSocketBuffer->clear();
			pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_ErrorText);
			*pSocketBuffer << L"该帐号的Token=0";
			return FALSE;
		}

		CardInfo Card;
		if (!CCardExtend::GetInstance().FindCard_By_CRC(Account.dwCardCrc, Card))
		{
			pSocketBuffer->clear();
			pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_ErrorText);
			*pSocketBuffer << L"该卡号CRC不存在!";
			return FALSE;
		}
		else if (Card.dwCardCrc != Account.dwCardCrc)
		{
			pSocketBuffer->clear();
			pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_ErrorText);
			*pSocketBuffer << MyTools::CCharacter::MakeFormatText(L"你被挤号了! 敌方帐号是:%s, IP:%s", Card.wsAccountName.c_str(), Card.wsIp.c_str());
			return FALSE;
		}

		Account.ulKeepALiveTick = ::GetTickCount64();
		return TRUE;
	});
}

BOOL CEchoPacket::JoinCrossAreaTeam(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		DWORD dwTeamId = 0;
		*pSocketBuffer >> dwTeamId;
		if (dwTeamId != NULL)
		{
			pSocketBuffer->clear();

			CrossAreaTeamGroup TeamGroup;
			if (!CCrossAreaTeam::GetInstance().GetTeamMemberContent(dwTeamId, TeamGroup))
			{
				// 如果搜索不到这个队伍ID, 就回发FALSE到Client通知 此ID不可用
				pSocketBuffer->InitializeHead(em_Sock_Msg_CROSSAREATEAM);
				*pSocketBuffer << FALSE;
				return TRUE;
			}
			else
			{
				// 回发队长的消息
				*pSocketBuffer << TRUE
					<< TeamGroup.wsDungeonName
					<< TeamGroup.Captain.Info.wsPlayerName
					<< TeamGroup.Captain.Info.dwPlayerId
					<< TeamGroup.Captain.dwRoomId
					<< TeamGroup.dwDungeonMapIndex
					<< static_cast<DWORD>(TeamGroup.emDifficulty);

				// 如果只是队员, 那么只要队长的ID就够了, 其他队员不需要
				if (GameClient.GetAccountContent().wsAccountName != TeamGroup.Captain.Info.wsAccountName)
					*pSocketBuffer << static_cast<UINT>(0);
				else
				{
					// 队长需要知道所有队员的ID
					*pSocketBuffer << static_cast<UINT>(TeamGroup.VecMember.size());
					for (CONST auto& itm : TeamGroup.VecMember)
						*pSocketBuffer << itm.Info.wsPlayerName << itm.Info.dwPlayerId;
				}

				return TRUE;
			}
		}
		else
		{
			auto& Member = *GameClient.GetCrossAreaTeamMember();

			pSocketBuffer->InitializeHead(em_Sock_Msg_CROSSAREATEAM);
			if (CCrossAreaTeam::GetInstance().JoinTeam(Member, dwTeamId))
				*pSocketBuffer << dwTeamId; // 返回队伍ID, 然后客户端用这个队伍ID来查
			else
				*pSocketBuffer << static_cast<DWORD>(0);

			//return TRUE;
		}
		return TRUE;
	});
}


BOOL CEchoPacket::SetCrossAreaTeamContent(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	//GameClient.GetCrossAreaTeamMember()
	// 解包
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		auto Member = ExtractPacket<CrossAreaTeamMember>([&]
		{
			CrossAreaTeamMember Member;
			// 帐号和角色名都是已经存在了, 不需要客户端再发一波
			Member.Info.wsAccountName = GameClient.GetAccountContent().wsAccountName;
			Member.Info.wsPlayerName = GameClient.GetAccountContent().wsPlayerName;

			UINT uLength = 0;
			//                       玩家ID					房间号				标题(过滤工作室)                         职业                  副本数量
			*pSocketBuffer >> Member.Info.dwPlayerId >> Member.dwRoomId >> Member.wsGroupTitle >> reinterpret_cast<BYTE&>(Member.emClasses) >> uLength;

			std::wstring wsDungeonName;
			for (UINT i = 0; i < uLength; ++i)
			{
				*pSocketBuffer >> wsDungeonName;
				Member.SetDungoenName.insert(wsDungeonName);
			}

			pSocketBuffer->clear();
			return Member;
		});

		*GameClient.GetCrossAreaTeamMember() = Member;

		*pSocketBuffer << true;
		return TRUE;
	});
}

BOOL CEchoPacket::LeaveCrossAreaTeam(_In_ CGameClient& GameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		DWORD dwTeamId = 0;
		*pSocketBuffer >> dwTeamId;

		CCrossAreaTeam::GetInstance().DeleteTeamGroup(GameClient.GetAccountContent().wsAccountName);
		CCrossAreaTeam::GetInstance().DeleteTeamMember(GameClient.GetAccountContent().wsAccountName);
		return TRUE;
	});
}


BOOL CEchoPacket::QueryAccountVirtualSeriaNumber(_In_ CGameClient&, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	// 解包
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		auto VecAccount = ExtractPacket<std::vector<std::wstring>>([&]
		{
			UINT uCount = 0;
			std::vector<std::wstring> VecAccount;

			*pSocketBuffer >> uCount;
			for (UINT i = 0; i < 100 && i < uCount; ++i)
			{
				std::wstring wsText;
				*pSocketBuffer >> wsText;
				VecAccount.push_back(wsText);
			}

			pSocketBuffer->clear();
			return VecAccount;
		});

		std::vector<SocketAccountVirtualSeriaNumber> VecSocketAccountVirtualSeriaNumber;
		for (auto& wsAccountName : VecAccount)
		{
			SocketAccountVirtualSeriaNumber Content;
			if (CBnsDbManager::GetInstance().QueryAccountSeriaNumber(wsAccountName, Content))
			{
				VecSocketAccountVirtualSeriaNumber.push_back(std::move(Content));
			}
		}

		pSocketBuffer->InitializeHead(em_Sock_Msg_Query_ACCOUNTVIRTUALSERIANUMBER);
		*pSocketBuffer << VecSocketAccountVirtualSeriaNumber.size();
		for (auto& itm : VecSocketAccountVirtualSeriaNumber)
		{
			*pSocketBuffer << itm.wsAccountName << itm.wsHardDiskSeriaNumber << itm.wsMacAddr << itm.wsNetCard;
		}

		return TRUE;
	});
}

VOID CEchoPacket::ExtractTeamPacket(_In_ MyTools::CLSocketBuffer* pSocketBuffer, _Out_ std::shared_ptr<ClientTeamInfo> CtiTeam) CONST
{
	MyTools::CLEchoException::GetInstance().InvokeAction(__FUNCTIONW__, [&]
	{
		DWORD dwCount = 0;
		*pSocketBuffer >> CtiTeam->dwServerId
			>> CtiTeam->dwTaskId
			>> CtiTeam->dwScheduleId
			>> CtiTeam->dwChannelId
			>> CtiTeam->dwMaxMember
			>> CtiTeam->SelfCti.dwPlayerId
			>> CtiTeam->wsGroupTitle
			>> dwCount;


		for (DWORD i = 0; i < dwCount && i < 10; ++i)
		{
			ClientTeamMember Member;
			*pSocketBuffer >> Member.wsPlayerName >> Member.dwPlayerId;
			CtiTeam->vTeamMember.push_back(std::move(Member));
		}
	});
}

VOID CEchoPacket::ExtractClientTradePacket(_In_ MyTools::CLSocketBuffer* pSocketBuffer, _Out_ VerItemSupport& ItemSupport) CONST
{
	MyTools::CLEchoException::GetInstance().InvokeAction(__FUNCTIONW__, [&]
	{
		DWORD dwCount = 0;
		*pSocketBuffer >> ItemSupport.dwServerId
			>> ItemSupport.dwChannel
			>> dwCount;


		for (DWORD i = 0; i < dwCount && i < 20; ++i)
		{
			std::wstring wsWarehouseName;
			*pSocketBuffer >> wsWarehouseName;
			ItemSupport.vWareHouse.push_back(std::move(wsWarehouseName));
		}

		pSocketBuffer->clear();
	});
}
