#include "DbManager.h"
#include <MyTools/Log.h>
#include <MyTools/CLPublic.h>
#include <MyTools/Character.h>
#include <MyTools/CLEchoException.h>

#define _SELF L"DbManager.cpp"
BOOL CBnsDbManager::GetEffectiveCardList(_Out_ std::map<DWORD, CardInfo>& MapCard) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		std::vector<Table> VecResult;
		if (!ExcuteSQL(L"exec Proc_GetAllCard", 3, VecResult))
			return FALSE;

		if (VecResult.size() == 0)
		{
			LOG_CF_E(L"一张有效卡号都没有~");
			return FALSE;
		}

		MapCard.clear();
		for (CONST auto& itm : VecResult)
		{
			if (itm.size() != 3)
			{
				LOG_CF_E(L"SQLTable.size=%d, != 3", itm.size());
				return FALSE;
			}

			CardInfo CardInfo_;
			CardInfo_.wsCardNo = itm.at(0);
			CardInfo_.nSurplusHour = std::stoi(itm.at(1));
			CardInfo_.wsActiveTime = itm.at(2);
			CardInfo_.nMaxHour = std::stoi(itm.at(0).substr(0, 4));
			CardInfo_.dwTick64 = ::GetTickCount64();
			CardInfo_.ulKeepALiveTick = ::GetTickCount64();
			CardInfo_.dwCardCrc = MyTools::CLPublic::GetCRC32_DWORD(CardInfo_.wsCardNo.c_str());

			MapCard.insert(std::make_pair(CardInfo_.dwCardCrc, std::move(CardInfo_)));
		}

		return MapCard.empty() ? FALSE : TRUE;
	});
}

BOOL CBnsDbManager::GetCloudConfigList(_Out_ std::map<std::wstring, std::wstring>& MapConfig) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		std::vector<Table> VecResult;
		if (!ExcuteSQL(L"exec proc_InitConfig", 4, VecResult))
			return FALSE;

		if (VecResult.size() == 0)
		{
			LOG_CF_E(L"VecResult.size=0");
			return FALSE;
		}

		MapConfig.clear();
		for (CONST auto& itm : VecResult)
		{
			if (itm.size() != 4)
			{
				LOG_CF_E(L"SQLTable.size=%d, != 4", itm.size());
				return FALSE;
			}

			ClientConfigContent ClientConfigContent_;
			ClientConfigContent_.wsAccountName = itm.at(0);
			ClientConfigContent_.wsPlayerName = itm.at(1);
			ClientConfigContent_.ConfigContent_.wsConfigName = itm.at(2);

			MapConfig.insert(std::make_pair(ClientConfigContent_.to_wstring(), itm.at(3)));
		}

		return MapConfig.empty() ? FALSE : TRUE;
	});
}

BOOL CBnsDbManager::FindCard_By_CRC(_In_ DWORD dwCardCrc, _Out_ CardInfo& Card) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		std::vector<Table> VecResult;

		if (!ExcuteSQL(MyTools::CCharacter::MakeFormatText(L"exec Proc_GetCard '%08X'", dwCardCrc), 3, VecResult))
			return FALSE;

		if (VecResult.size() == 0)
		{
			//LOG_CF_E(L"UnExist CardCRC[%X]", dwCardCrc);
			return FALSE;
		}

		CONST auto& itm = VecResult.at(0);
		if (itm.size() != 3)
		{
			LOG_CF_E(L"SQLTable.size=%d, != 3", itm.size());
			return FALSE;
		}

		Card.wsCardNo = itm.at(0);
		Card.nSurplusHour = std::stoi(itm.at(1));
		Card.wsActiveTime = itm.at(2);
		Card.nMaxHour = std::stoi(Card.wsCardNo.substr(0, 4));
		Card.dwTick64 = ::GetTickCount64();
		Card.ulKeepALiveTick = ::GetTickCount64();
		Card.dwCardCrc = dwCardCrc;
		return TRUE;
	});
}

BOOL CBnsDbManager::AddLoginLogToDb(_In_ CONST ClientAccountContent& ClientAccountContent_) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		AsyncExcuteSQL(MyTools::CCharacter::MakeFormatText(L"exec InsertLoginLog '%s','%s','%s','%s'", \
			ClientAccountContent_.wsCardNo.c_str(),
			ClientAccountContent_.wsIp.c_str(),
			ClientAccountContent_.wsAccountName.c_str(),
			ClientAccountContent_.wsAccountPass.c_str()));

		return TRUE;
	});
}

BOOL CBnsDbManager::GetOTPContent(_Out_ std::map<std::wstring, std::wstring>& MapOTP) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		std::vector<Table> VecResult;
		if (!ExcuteSQL(L"exec GetOTPAccountContent", 2, VecResult))
			return FALSE;

		if (VecResult.size() == 0)
		{
			LOG_CF_E(L"VecResult.size=0");
			return FALSE;
		}

		MapOTP.clear();
		for (CONST auto& itm : VecResult)
		{
			if (itm.size() != 2)
			{
				LOG_CF_E(L"SQLTable.size=%d, != 4", itm.size());
				return FALSE;
			}

			MapOTP.insert(std::make_pair(itm.at(0), itm.at(1)));
		}

		return MapOTP.empty() ? FALSE : TRUE;
	});
}

BOOL CBnsDbManager::AddBreakConnectLogToDb(_In_ CONST ClientAccountContent& ClientAccountContent_) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		AsyncExcuteSQL(MyTools::CCharacter::MakeFormatText(L"exec Proc_InsertBannedLog N'%s',N'%s',N'%s'", \
			ClientAccountContent_.wsCardNo.c_str(),
			ClientAccountContent_.wsAccountName.c_str(),
			ClientAccountContent_.wsAccountPass.c_str()));

		return TRUE;
	});
}

BOOL CBnsDbManager::AddBannedRecordToDb(_In_ CONST ClientAccountContent& ClientAccountContent_) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		auto wsSQL = MyTools::CCharacter::MakeFormatText(L"exec Proc_InsertBannedAccount N'%s',N'%s',N'%s',N'%s'", \
			ClientAccountContent_.wsAccountName.c_str(),
			ClientAccountContent_.wsAccountPass.c_str(),
			ClientAccountContent_.wsCardNo.c_str(),
			ClientAccountContent_.wsIp.c_str());

		AsyncExcuteSQL(wsSQL);
		return TRUE;
	});
}

BOOL CBnsDbManager::AddAccountLogToDb(_In_ CONST ClientAccountContent& ClientAccountContent_, _In_ CONST ClientAccountLog& AccountLog) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		struct ClientAccountLogTypeText
		{
			em_ClientAccountLog_Type	emType;
			std::wstring				wsText;
		};
		CONST static std::vector<ClientAccountLogTypeText> vlst =
		{
			{ em_ClientAccountLog_Type::em_ClientAccountLog_Type_AccountLog,			L"AccountLog" },
			{ em_ClientAccountLog_Type::em_ClientAccountLog_Type_PlayerDaily,			L"PlayerDaily" },
			{ em_ClientAccountLog_Type::em_ClientAccountLog_Type_TaskUpgradeSchedule,	L"TaskUpgradeSchedule" },
		};

		if (AccountLog.wsContentText.length() >= MAX_PATH)
			return FALSE;

		auto pAccountLogText = MyTools::CLPublic::Vec_find_if_Const(vlst, [&AccountLog](CONST ClientAccountLogTypeText LogTypeText) { return LogTypeText.emType == AccountLog.emType; });
		if (pAccountLogText == nullptr)
		{
			LOG_CF_E(L"无效的日志类型:%d, Account:%s Content:%s", static_cast<int>(AccountLog.emType), ClientAccountContent_.wsAccountName.c_str(), AccountLog.wsContentText.c_str());
			return FALSE;
		}

		//LOG_CF_D(L"AccountLog[%s]", ClientAccountContent_.wsAccountName.c_str());

		auto wsSQL = MyTools::CCharacter::MakeFormatText(L"exec InsertAccountLocalLog N'%s',N'%s',N'%s'", pAccountLogText->wsText.c_str(), ClientAccountContent_.wsAccountName.c_str(), AccountLog.wsContentText.c_str());

		//LOG_CF_D(L"AccountLog.SQL=%s", wsSQL.c_str());
		AsyncExcuteSQL(wsSQL);
		return TRUE;
	});
}

BOOL CBnsDbManager::AddTradeContentToDb(_In_ CONST ClientAccountContent& ClientAccountContent_, _In_ CONST TradeContent& TradeContent_) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		std::wstring wsResult;
		std::wstring wsSQL = MyTools::CCharacter::MakeFormatText(L"exec CreateDealInfo N'%s', N'%s' ,N'%s','%s','%08X'", \
			ClientAccountContent_.wsPlayerName.c_str(),
			ClientAccountContent_.wsAccountName.c_str(),
			TradeContent_.wsWareHouseName.c_str(),
			ClientAccountContent_.wsIp.c_str(),
			ClientAccountContent_.dwCardCrc);

		BOOL bRetCode = ExcuteSQL_SingleResult(wsSQL, wsResult);
		if (!bRetCode)
		{
			LOG_CF_E(L"exec CreateDealInfo 异常!wszRetMsg=%s, SQL=%s", wsResult.c_str(), wsSQL.c_str());
			return FALSE;
		}

		for (CONST auto& itm : TradeContent_.VecTradeItem)
			AsyncExcuteSQL(MyTools::CCharacter::MakeFormatText(L"exec CreateDealItem %s,N'%s',%d", wsResult.c_str(), itm.wsItemName.c_str(), itm.dwCount));

		if (TradeContent_.dwTradeMoney)
			AsyncExcuteSQL(MyTools::CCharacter::MakeFormatText(L"exec CreateDealMoney %s,%d", wsResult.c_str(), TradeContent_.dwTradeMoney));

		return TRUE;
	});
}

BOOL CBnsDbManager::GetClientTitle(_In_ CONST std::wstring& wsIp, _Out_ std::wstring& wsTitle) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		return ExcuteSQL_SingleResult(MyTools::CCharacter::MakeFormatText(L"exec Proc_VrTitle '%s'", wsIp.c_str()), wsTitle);
	});
}

BOOL CBnsDbManager::ActiveCard(_In_ CONST ClientAccountContent& ClientAccountContent_, _In_ DWORD dwCardCrc, _Out_ CardInfo& CardInfo_) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		std::wstring wsResult;

		BOOL bRetCode = ExcuteSQL_SingleResult(MyTools::CCharacter::MakeFormatText(L"exec Proc_RechargeCard '%08X','%s'", dwCardCrc, ClientAccountContent_.wsIp.c_str()), wsResult);
		if (!bRetCode)
		{
			return FALSE;
		}

		if (wsResult.find(L"充值成功") == -1)
		{
			LOG_CF_D(L"卡号尝试激活查询返回:%s, CRC=%08X, IP=%s", wsResult.c_str(), dwCardCrc, ClientAccountContent_.wsIp.c_str());
			return FALSE;
		}

		return FindCard_By_CRC(dwCardCrc, CardInfo_);
	});
}

BOOL CBnsDbManager::CleanRecentlyCloudConfig(_In_ UINT uDay) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		return ExcuteSQL_NoneResult(MyTools::CCharacter::MakeFormatText(L"exec ClearAccoutConfig %d", uDay));
	});
}

BOOL CBnsDbManager::IsNewBanned(_In_ CONST std::wstring& wsAccountName) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		std::wstring wsResult;
		if (!ExcuteSQL_SingleResult(MyTools::CCharacter::MakeFormatText(L"exec Proc_ExistBanned_By_Account N'%s'", wsAccountName.c_str()), wsResult))
			return FALSE;

		return wsResult == L"0" ? TRUE : FALSE;
	});
}


BOOL CBnsDbManager::QueryAccountSeriaNumber(_In_ std::wstring& wsAccountName, _Out_ SocketAccountVirtualSeriaNumber& Content) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		std::vector<Table> VecResult;
		if (!ExcuteSQL(MyTools::CCharacter::MakeFormatText(L"exec Query_AccountVirtualSeriaNumber N'%s'", wsAccountName.c_str()), 4, VecResult))
			return FALSE;

		if (VecResult.size() == 0)
		{
			LOG_CF_E(L"不存在该账号[%s]的虚拟硬件号, 不可能啊!", wsAccountName.c_str());
			return FALSE;
		}

		CONST auto& itm = VecResult.at(0);
		if (itm.size() != 4)
		{
			LOG_CF_E(L"SQLTable.size=%d, != 4", itm.size());
			return FALSE;
		}

		Content.wsAccountName = itm.at(0);
		Content.wsHardDiskSeriaNumber = itm.at(1);
		Content.wsMacAddr = itm.at(2);
		Content.wsNetCard = itm.at(3);

		return TRUE;
	});
}

VOID CBnsDbManager::Initialize()
{
	_wsDns = L"BnsKor";
	_wsDbUser = L"GameUser";
	_wsDbPass = L"lg_db_korea";
}
