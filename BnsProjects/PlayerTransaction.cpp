#include "stdafx.h"
#include "PlayerTransaction.h"
#include <MyTools/Log.h>
#include "BagFunction.h"
#include "PersonFunction.h"
#include "SomeFun.h"
#include "PlayerExtend.h"
#include "Farm.h"
#include "BagAction.h"
#include "Person.h"
#include "GameUI.h"
#include "ObjectFunction.h"
#include "GameSend.h"
#include "Npc.h"
#include "NpcExtend.h"
#include "BnsFilter.h"
#include "BnsGameFun.h"
#include "MailAction.h"
#include "ConsoleLog.h"

#define _SELF L"PlayerTransaction.cpp"

BOOL CPlayerTransaction::Transaction_By_Task_To_WarehouseAccount(_In_ _em_WarehouseType em_WarehouseType, _In_ CONST std::vector<MainTask_Transaction_Item>& vItem, _In_ DWORD dwMoney, _In_ std::function<VOID(em_Warehouse_Action)> fnPtr)
{
	m_vItem = vItem;
	m_dwMoney = dwMoney;

	std::wstring wsPlayerName;
	BOOL bCheckItemSupport = TRUE;
	BOOL bSleep = TRUE;
	MainTask_Transaction_Item LackItemName(L"", 0);

	LOG_CF_D( L"Name=%s,ServerId=%X", CPerson::GetInstance().GetName().c_str(), CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->dwServerId);
	fnPtr(em_Warehouse_Action_City);
	SetWarehouseChannel();
	while (GameRun && !EMPTY_PERSONHP)
	{
		// 判断从服务器上是否有小号发来交易请求
		BnsSleep(bSleep ? 2000 : 0);
		bSleep = TRUE;

		// 判断是否有人要交易自己!
		if (DealWithPlayer_By_Other(em_WarehouseType))
		{
			CBagFunction::GetInstance().CleanBag();
			continue;
		}

		// Only RecvItem in Owner
		if (em_WarehouseType == em_WarehouseType_Passive)
			continue;

		// 如果当前身上背包格子数<9的时候, 就扔包子(仓库和衣柜都挤不下了)
		if (CBagFunction::GetInstance().IsBagFull(CObjectFunction::em_Bag_Type_Bag, 8 + 1))
		{
			auto fnDiscardItem = [](_In_ CONST CBag& itm)
			{
				itm.DiscardItem();
				BnsSleep(2000);
			};

			// 甩一组战红和坐红!
			LOG_CF_D( L"背包格子数量<9,尝试扔包子和药");
			CBagFunction::GetInstance().CleanBag();
			CBagFunction::GetInstance().DoItem_By_ExistItem_To_Name(包子, fnDiscardItem);
			CBagFunction::GetInstance().DoItem_By_ExistItem_To_Name(恢复药, fnDiscardItem);
			continue;
		}

		auto emPick_ItemSupport = IsHasInviateTransaction_By_GiveItem(wsPlayerName);
		if (emPick_ItemSupport == em_Deal_Warehouse_IsPick::em_Deal_Warehouse_IsPick_Pick)
		{
			CConsoleLog(MyTools::CCharacter::MakeFormatText(L"小号来请求收货交易, Name:%s", wsPlayerName.c_str()));
			LOG_CF_D( L"有收货号来请求交易, Name:%s", wsPlayerName.c_str());
			WareHouseDealWithSideAccount(wsPlayerName);
			CConsoleLog(L"收货交易完毕!");
			bCheckItemSupport = TRUE;

			// 每收货交易一次, 都执行一次存仓操作(里面有判断是否需要存仓步骤)
			CBagFunction::GetInstance().CleanBag();
			if (CBagFunction::GetInstance().IsBagFull(CObjectFunction::em_Bag_Type_Bag, 8 + 1))
			{
				LOG_CF_D( L"执行存仓操作!");
				fnPtr(CPlayerTransaction::em_Warehouse_Action::em_Warehouse_Action_Save);
				//SaveItem_By_WareHouseNpc(WarehouseNpcName);
			}
			
			bSleep = FALSE;
			continue;
		}
		else if (emPick_ItemSupport == em_Deal_Warehouse_IsPick::em_Deal_Warehouse_IsPick_BackToSwitchPlayer)
		{
			LOG_CF_D( L"请求物资,Server返回em_Deal_Warehouse_IsPick_BackToSwitchPlayer, break!");
			break;
		}

		// 低于要物资支持的金额的话, 就不去服务器当物资仓库号!
		if (CPerson::GetInstance().GetMoney() < dwMoney)
		{
			CConsoleLog(L"物资金币不足!");
			continue;
		}


		/*if (!IsEnoughItemSupport_By_WareHouseAccount(vItem, LackItemName) && bCheckItemSupport)
		{
			LOG_CF_D( L"物资:%s 不足! 去仓库取货", LackItemName.wsItemName.c_str());
			CBnsGameFun::GetInstance().WriteConsoleLog(L"物资:%s 不足! 去仓库取货", LackItemName.wsItemName.c_str());

			// 包子和药特殊处理!
			//if (LackItemName.wsItemName != 恢复药 && LackItemName.wsItemName != 包子 && LackItemName.wsItemName != 修理锤)
			//	TakeItemSupport_By_WareHouseNpc(WarehouseNpcName, vItem); // 去仓库取货!
			//else
			//	CBagAction::GetInstance().PullItemByWardrobeWithList(WarehouseNpcName, CBagAction::tagPullWardrobeItm{ LackItemName.wsItemName, 4 * 1000 }); // 去衣柜取!
		}*/

		if (!IsEnoughItemSupport_By_WareHouseAccount(vItem, LackItemName))
		{
			LOG_CF_D( L"物资:%s 不足! 购买物资!", LackItemName.wsItemName.c_str());
			if (LackItemName.wsItemName == 恢复药)
				fnPtr(em_Warehouse_Action::em_Warehouse_Action_Medicine);
			else if (LackItemName.wsItemName == 包子)
				fnPtr(em_Warehouse_Action::em_Warehouse_Action_Eat);
			else if (LackItemName.wsItemName == 修理锤)
				fnPtr(em_Warehouse_Action::em_Warehouse_Action_Hammer);
			// 去买物资
		//	BuyItem_By_List(LackItemName);
		}

		if (!IsEnoughItemSupport_By_WareHouseAccount(vItem, LackItemName))
		{
			CConsoleLog(MyTools::CCharacter::MakeFormatText(L"物资不足:%s!", LackItemName.wsItemName.c_str()));
			LOG_CF_D( L"物资不足:%s!", LackItemName.wsItemName.c_str());
			bCheckItemSupport = FALSE;
			continue;
		}

		auto emPick = IsHasInviateTransaction_By_MainTaskAccount(wsPlayerName);
		if (emPick == em_Deal_Warehouse_IsPick::em_Deal_Warehouse_IsPick_Pick)
		{
			CConsoleLog(MyTools::CCharacter::MakeFormatText(L"小号来请求物资交易, Name:%s", wsPlayerName.c_str()));
			LOG_CF_D( L"有练级小号来请求物资交易, Name:%s", wsPlayerName.c_str());
#if CLIENT_DEALWAY == ENABLE_CLIENT_DEALWAY_DEAL
			CBnsGameFun::GetInstance().WriteConsoleLog(L"对小号:%s 物资交易完毕!", wsPlayerName.c_str());
			DealWithPlayer_By_MainTask(wsPlayerName);
#elif CLIENT_DEALWAY == ENABLE_CLIENT_DEALWAY_MAIL
			MailWithPlayer_By_MainTask(wsPlayerName);
			CConsoleLog(MyTools::CCharacter::MakeFormatText(L"对小号:%s 物资邮寄完毕!", wsPlayerName.c_str()));
#endif
			
			CBagFunction::GetInstance().CleanBag();
			bSleep = FALSE;
			continue;
		}
		else if (emPick == em_Deal_Warehouse_IsPick::em_Deal_Warehouse_IsPick_BackToSwitchPlayer)
		{
			LOG_CF_D( L"请求收货,Server返回em_Deal_Warehouse_IsPick_BackToSwitchPlayer, break!");
			break;
		}
	}
	return TRUE;
}

BOOL CPlayerTransaction::SetTransactionItem_By_Name(_In_ CONST std::wstring& wsItemName, _In_ DWORD dwCount) CONST
{
	if (CBagFunction::GetInstance().GetItemCount_By_Name(wsItemName) < dwCount)
	{
		LOG_CF_D( L"交易物品:%s 数量不足%d个", wsItemName.c_str(), dwCount);
		return FALSE;
	}

	DWORD dwPanelCount = GetPushItemCount();
	BOOL bShow = TRUE;
	while (GameRun && !EMPTY_PERSONHP && dwPanelCount == GetPushItemCount() && bShow)
	{
		CObjectFunction::GetInstance().Action_By_GameUiName(L"TradeShelfPanel", [&](CONST CGameUi& TradeShelfPanel)
		{
			if (!TradeShelfPanel.IsShow())
			{
				bShow = FALSE;
				LOG_CF_E(L"TradeShelfPanel = FALSE");
				return;
			}

			PushItemToTradePanel(TradeShelfPanel, wsItemName, dwCount);
		});

		BnsSleep(1000);
	}
	LOG_CF_D(L"放置物品:%s 完毕!", wsItemName.c_str());
	return TRUE;
}

BOOL CPlayerTransaction::PushItemToTradePanel(_In_ CONST CGameUi& TradeShelfPanel, _In_ CONST std::wstring& wsItemName, _In_ DWORD dwCount) CONST
{
	std::vector<CBag> vlst;
	CObjectFunction::GetInstance().GetBagList(vlst, CObjectFunction::em_Bag_Type_Bag);

	UINT uTotalCount = 0;
	for (CONST auto& itm : vlst)
	{
		if (itm.GetName() != wsItemName)
			continue;

		if (uTotalCount < dwCount)
		{
			DWORD dwTransactionItemCount = dwCount - uTotalCount;
			uTotalCount += itm.GetCount();

			// Max Transaction ALL of Item Count
			if (GetPushItemCount() >= 8)
				break;

			// 判断物品数量是否减少
			DWORD dwPushCount = itm.GetCount() >= dwTransactionItemCount ? dwTransactionItemCount : itm.GetCount();
			LOG_CF_D(L"放置物品:%s 数量=%d, MaxCount=%d,面板上放置的数量=%d", wsItemName.c_str(), dwPushCount, uTotalCount, GetPushItemCount());
			itm.PushTransactionItem(TradeShelfPanel.GetObj(), dwPushCount);
		}
		if (uTotalCount >= dwCount)
		{
			break;
		}
	}

	return TRUE;
}

BOOL CPlayerTransaction::LockTradePanel() CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([]{ CGameSend::LockTransaction(); });
}

BOOL CPlayerTransaction::IsLockSelfPanel() CONST
{
	auto bRetCode = FALSE;
	CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"TradeShelfPanel", [&bRetCode](CONST CGameUi& TradeShelfPanel){
		bRetCode = ReadBYTE(ReadDWORD(TradeShelfPanel.GetObj() + 放置交易物品偏移) + 是否锁定自己的交易框偏移) == 0x1;
	});
	return bRetCode;
}

BOOL CPlayerTransaction::IsLockOtherPlayerPanel() CONST
{
	auto bRetCode = FALSE;
	CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"TradeShelfPanel", [&bRetCode](CONST CGameUi& TradeShelfPanel){
		bRetCode = ReadBYTE(ReadDWORD(TradeShelfPanel.GetObj() + 放置交易物品偏移) + 对方是否锁定交易偏移) == 0x1;
	});
	return bRetCode;
}

BOOL CPlayerTransaction::SumitTransaction() CONST
{
	auto bRetCode = FALSE;
	CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"TradeShelfPanel", [&bRetCode](CONST CGameUi& TradeShelfPanel){
		bRetCode = CGameVariable::GetInstance().PushMainThreadFunPtr_synch([TradeShelfPanel]{CGameSend::SumitTransaction(ReadDWORD(TradeShelfPanel.GetObj() + 放置交易物品偏移)); });
	});
	return bRetCode;
}

BOOL CPlayerTransaction::Transaction_By_Task_ToMainTaskAccount(_In_ std::function<VOID(CONST std::wstring&)> fnPtr) CONST
{
	static CONST ULONGLONG dwMaxTimeOut = 10 * 60 * 1000;
	DWORD dwChannel = 0;
	std::wstring wsPlayerName;

	// 获取配置文件的所有仓库号
	std::vector<std::wstring> vNameList;
	if (GetWarehourseNameList(vNameList) == 0)
	{
		LOG_MSG_CF(L"大兄弟！ 收货号呢?");
		return FALSE;
	}
	UINT uBagCount = CBagFunction::GetInstance().TotalOccupyItem(CObjectFunction::em_Bag_Type::em_Bag_Type_Bag);
	LOG_CF_D( L"Name=%s,ServerId=%X", CPerson::GetInstance().GetName().c_str(), CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->dwServerId);

	auto fnIsRecvMailSucc = [uBagCount]
	{
		return CBagFunction::GetInstance().TotalOccupyItem(CObjectFunction::em_Bag_Type::em_Bag_Type_Bag) > uBagCount;
	};
	
	auto dwTick = ::GetTickCount64();
	while (GameRun && !EMPTY_PERSONHP && !fnIsRecvMailSucc())
	{
		if (::GetTickCount64() - dwTick >= dwMaxTimeOut)
		{
			LOG_CF_D( L"10分钟超时! 10分钟内都没人给物资! 闪人!");
			return 0;
		}
#if CLIENT_DEALWAY == ENABLE_CLIENT_DEALWAY_DEAL
		if (CObjectFunction::GetInstance().IsShow_In_GameUi(L"TradeGuestConfirmPanel"))
		{
			// 看看是不是仓库号邀请咱们的!
			CPlayer Player;
			if (!CPlayerExtend::GetInstance().FindPlayer_By_Id(CPerson::GetInstance().GetInviateId_By_Transaction(), CBaseObject::CreateFixPointInfo(30), &Player))
			{
				LOG_CF_D( L"30m找不到交易邀请者!ID=%X", CPerson::GetInstance().GetInviateId_By_Transaction());
				CancelDeal();
				BnsSleep(2000);
				continue;
			}

			if (CLPublic::Vec_find_if(vNameList, [&Player](CONST std::wstring& wsName){ return wsName == Player.GetName(); }) == nullptr)
			{
				LOG_CF_D( L"非仓库号申请交易! 怒拒绝!");
				CSomeFun::GetInstance().SimlationKey('N', 200, 800);
				return FALSE;
			}

			LOG_CF_D( L"是仓库号发出的申请交易!");
			DealWithWarehour_By_MainTask(Player);
			break;
		}
#endif
		
		DWORD dwSleepTime = 60 * 1000;
		// 获取是否有仓库号接单了! 返回仓库号所在的频道!
		em_Deal_Warehouse_Status emDealStatus = em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_None;
		if (!IsHasItemSupport(vNameList, wsPlayerName, dwChannel, emDealStatus))
		{
			dwSleepTime = 2* 1000;
		}
#if CLIENT_DEALWAY == ENABLE_CLIENT_DEALWAY_MAIL
		else
			LOG_CF_D(L"有仓库号接单了 Name=%s, 去检查邮件!", wsPlayerName.c_str());

		CSomeFun::GetInstance().TimeOut_Condiction_GameRun(dwSleepTime, [fnIsRecvMailSucc]
		{
			LOG_CF_D(L"检查邮件!");
			BnsSleep(1000);
			CMailAction::GetInstance().RecvMail();
			return !fnIsRecvMailSucc();
		});
#elif CLIENT_DEALWAY == ENABLE_CLIENT_DEALWAY_DEAL
		if (emDealStatus == em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_Channel)
		{
			LOG_CF_D(L"仓库号:%s 接单了!", wsPlayerName.c_str());
			auto pWarehouseConfig = CGameVariable::GetInstance().GetWarehouseConfig(wsPlayerName);
			if (pWarehouseConfig == nullptr)
			{
				CBnsGameFun::GetInstance().WriteConsoleLog(L"服务器上传来的仓库号:%s 并不在当前配置文件中!", wsPlayerName.c_str());
				CGameVariable::GetInstance().SetStopGame();
			}

			CBnsGameFun::GetInstance().WriteConsoleLog(L"去仓库号所在的城市:%s", pWarehouseConfig->wszCityName);
			fnPtr(pWarehouseConfig->wszCityName);
			LOG_CF_D(L"先去仓库号的频道!, Name=%s, 频道=%d, 当前频道=%d", wsPlayerName.c_str(), dwChannel, CPerson::GetInstance().GetChannel());
			CFarm::GetInstance().SetFixChannel(dwChannel, TRUE);
			BnsSleep(2000);
			continue;
		}
#endif
	}
	return TRUE;
}

BOOL CPlayerTransaction::DealWithPlayer_By_MainTask(_In_ CONST std::wstring& wsPlayerName) CONST
{
	CPlayer Player;

	//CSomeFun::GetInstance().SimlationKey('Esc', 10);
	CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"TradeShelfPanel", [](CONST CGameUi& TradeShelfPanel){
		LOG_CF_D( L"估计是上一个界面留下来的, 直接关闭!");
		TradeShelfPanel.CloseGameWindow();
		BnsSleep(1000);
	});

	BOOL bExistPlayer = !CSomeFun::GetInstance().TimeOut_Condiction_GameRun(10 * 1000, [&wsPlayerName, &Player]
	{
		BnsSleep(1000);
		return !CPlayerExtend::GetInstance().FindPlayer_By_Name(wsPlayerName, CBaseObject::CreateFixPointInfo(30.0f), &Player);
	});;
	
	if (!bExistPlayer)
	{
		LOG_CF_D( L"10s之内 30m范围内找不到练级小号:%s, 当前坐标:[%d,%d,%d]附近的玩家如下:", wsPlayerName.c_str(), 
			CPerson::GetInstance().GetPoint().GetX(), CPerson::GetInstance().GetPoint().GetY(), CPerson::GetInstance().GetPoint().GetZ());
		return FALSE;
	}
	CConsoleLog(L"正在走向小号……");
	BnsMove(Player.GetPoint(), 2);

	if (CSomeFun::GetInstance().TimeOut_Condiction_GameRun(15 * 1000, [&Player]{
		return Player.GetDis() > 3.0f;
	}))
	{
		LOG_CF_D( L"15秒内对方都没靠近自己3m, 距离是:%.2f! 放弃他!", Player.GetDis());
		return FALSE;
	}

	if (!InviatePlayerDeal_By_Warehouse(Player))
	{
		CancelDeal();
		return FALSE;
	}

	// 放物资放钱!
	LOG_CF_D( L"对方已经接受交易申请!");
	CConsoleLog(L"对方接受交易! 准备放物资");
	BnsSleep(1000);
	for (CONST auto& itm : m_vItem)
	{
		LOG_CF_D( L"放置物品:[%s,%d]", itm.wsItemName.c_str(), itm.dwCount);
		SetTransactionItem_By_Name(itm.wsItemName, itm.dwCount);
	}
	// Push Special Item
	//SetTransactionItem_By_Name(L"약선 용봉탕", 15);

	// Push Money
	SetTransactionMoney(m_dwMoney);
	BnsSleep(500);

	return LockAndSumitTransaction_By_PushPlayer();
}

BOOL CPlayerTransaction::MailWithPlayer_By_MainTask(_In_ CONST std::wstring& wsPlayerName)
{
	enum { MAX_REPEAT_MAIL_COUNT = 30 };
	auto p = MyTools::CLPublic::Deque_find_if(VecItemSupportPlayerName, [wsPlayerName](CONST MailedPlayerContnet& MailedPlayerContnet_){ return MailedPlayerContnet_.wsPlayerName == wsPlayerName; });
	if (p != nullptr)
	{
		if (p->uCount++ >= MAX_REPEAT_MAIL_COUNT)
		{
			LOG_CF_D( L"角色:%s 反复请求邮寄物资超过%d次", wsPlayerName.c_str(), MAX_REPEAT_MAIL_COUNT);
			MyTools::CLPublic::Dque_erase_if(VecItemSupportPlayerName, [wsPlayerName](CONST MailedPlayerContnet& MailedPlayerContnet_){ return MailedPlayerContnet_.wsPlayerName == wsPlayerName; });
			return TRUE;
		}
		LOG_CF_D( L"角色:%s 已经成功邮寄了物资了!, 就不重复邮寄了! 请求次数=%d", wsPlayerName.c_str(), p->uCount);
		return TRUE;
	}

	CONST auto pWarehouseConfig = CGameVariable::GetInstance().GetWarehouseConfig();
	if (pWarehouseConfig == nullptr)
	{
		LOG_MSG_CF(L"当前角色并不是仓库号!!!");
		return FALSE;
	}

	//auto VecItem = m_vItem;
	//VecItem.push_back(MainTask_Transaction_Item(L"약선 용봉탕", 15));

	BOOL bRetCode = CMailAction::GetInstance().MailItemSupport(pWarehouseConfig->wszNpcName,wsPlayerName, m_vItem, m_dwMoney);
	if (!bRetCode)
		return FALSE;

	MailedPlayerContnet MailedPlayerContnet_;
	MailedPlayerContnet_.uCount = 0;
	MailedPlayerContnet_.wsPlayerName = wsPlayerName;
	VecItemSupportPlayerName.push_back(std::move(MailedPlayerContnet_));
	return TRUE;
}

BOOL CPlayerTransaction::SetTransactionMoney(_In_ DWORD dwMoney) CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&dwMoney]{ CGameSend::PushTransactionMoney(dwMoney); });
}

UINT CPlayerTransaction::GetWarehourseNameList(_Out_opt_ std::vector<std::wstring>& vNameList) CONST
{
	vNameList.clear();
	auto& pShare = CGameVariable::GetInstance().GetShareInfo();
	for (UINT i = 0; i < pShare->ConsoleContent.GlobalAccount.Warehouse.uCount; ++i)
		vNameList.push_back(std::wstring(pShare->ConsoleContent.GlobalAccount.Warehouse.Warehouse[i].wszPlayerName));
	return vNameList.size();
}

BOOL CPlayerTransaction::DealWithWarehour_By_MainTask(_In_ CONST CPlayer&) CONST
{
	if (CSomeFun::GetInstance().TimeOut_Condiction_GameRun(20 * 1000, [this]
	{
		AcceptDeal();
		BnsSleep(2000);
		return !CObjectFunction::GetInstance().IsShow_In_GameUi(L"TradeShelfPanel");
	}))
	{
		LOG_CF_E( L"20秒内一直接受交易都失败,UI:TradeGuestConfirmPanel 一直都显示存在!");
		CancelDeal();
		return FALSE;
	}

	BnsSleep(2000);
	if (!CObjectFunction::GetInstance().IsShow_In_GameUi(L"TradeShelfPanel"))
	{
		LOG_CF_D( L"找不到UI:TradeShelfPanel");
		CancelDeal();
		return FALSE;
	}

	return LockAndSumitTransaction_By_PullPlayer();
}

BOOL CPlayerTransaction::Transaction_By_Finish(_In_ CONST std::vector<std::wstring>& vItem, _In_ DWORD dwSpareMoney, _In_ std::function<VOID(CONST std::wstring&)> fnPtr) CONST
{
	DWORD dwChannel = 0;
	std::wstring wsPlayerName;
	std::vector<std::wstring> vNameList;

	if(GetWarehourseNameList(vNameList) == 0)
	{
		LOG_MSG_CF(L"你得填一个仓库号呐!");
		return FALSE;
	}

	LOG_CF_D( L"Name=%s,ServerId=%X", CPerson::GetInstance().GetName().c_str(), CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->dwServerId);
	while (GameRun && !EMPTY_PERSONHP)
	{
		// 判断物资和金钱都不足了, 就表示东西都给收走了!
		if (!IsExistItemList(vItem, dwSpareMoney))
		{
			LOG_CF_D( L"物资和金钱都给足了, 收货完毕!");
			break;
		}
		if (CObjectFunction::GetInstance().IsShow_In_GameUi(L"TradeGuestConfirmPanel"))
		{
			// 看看是不是仓库号邀请咱们的!
			CPlayer Player;
			if (!CPlayerExtend::GetInstance().FindPlayer_By_Id(CPerson::GetInstance().GetInviateId_By_Transaction(), CBaseObject::CreateFixPointInfo(30), &Player))
			{
				LOG_CF_D( L"30m找不到交易邀请者!ID=%X", CPerson::GetInstance().GetInviateId_By_Transaction());
				CancelDeal();
				BnsSleep(2000);
				continue;
			}

			if (MyTools::CLPublic::Vec_find_if(vNameList, [&Player](CONST std::wstring& wsName){ return wsName == Player.GetName(); }) == nullptr)
			{
				LOG_CF_D( L"非仓库号申请交易! 怒拒绝!");
				CSomeFun::GetInstance().SimlationKey('N', 200, 800);
				return FALSE;
			}

			LOG_CF_D( L"是仓库号发出的申请交易!");
			PlayerDealWithWarehouse(Player, vItem, dwSpareMoney);
			BnsSleep(1000);
			//continue;
			break;
		}

		// 获取是否有仓库号接单了! 返回仓库号所在的频道!
		em_Deal_Warehouse_Status emDealStatus = em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_None;
		if (!IsHasPickItem(vNameList, wsPlayerName, dwChannel, emDealStatus))
		{
			BnsSleep(2000);
			continue;
		}

		LOG_CF_D( L"仓库号:%s 接单了!", wsPlayerName.c_str());
		auto pWarehouseConfig = CGameVariable::GetInstance().GetWarehouseConfig(wsPlayerName);
		if (pWarehouseConfig == nullptr)
		{
			CConsoleLog(MyTools::CCharacter::MakeFormatText(L"服务器上传来的仓库号:%s 并不在当前配置文件中!", wsPlayerName.c_str()));
			CGameVariable::GetInstance().SetStopGame();
		}

		CConsoleLog(MyTools::CCharacter::MakeFormatText(L"去仓库号所在的城市:%s", pWarehouseConfig->wszCityName));
		fnPtr(pWarehouseConfig->wszCityName);
		LOG_CF_D( L"先去仓库号的频道!, Name=%s, 频道=%d, 当前频道=%d", wsPlayerName.c_str(), dwChannel, CPerson::GetInstance().GetChannel());
		CFarm::GetInstance().SetFixChannel(dwChannel, TRUE);

		CSomeFun::GetInstance().TimeOut_Condiction_GameRun(15 * 1000, []
		{
			BnsSleep(1000);
			return !CObjectFunction::GetInstance().IsShow_In_GameUi(L"TradeGuestConfirmPanel");
		});
	}
	return TRUE;
}

BOOL CPlayerTransaction::IsExistItemList(_In_ CONST std::vector<std::wstring>& vItem, _In_ DWORD dwSpareMoney) CONST
{
	if (CObjectFunction::GetInstance().IsShow_In_GameUi(L"TradeShelfPanel"))
	{
		LOG_CF_D( L"UI:TradeShelfPanel 已经存在了, 先关闭再判断物资!");
		CancelDeal();
		return FALSE;
	}

	if (CPerson::GetInstance().GetMoney() > dwSpareMoney)
		return TRUE;

	for (CONST auto& itm : vItem)
	{
		if (CBagFunction::GetInstance().ExistItem_By_Name(itm, nullptr))
			return TRUE;
	}
	return FALSE;
}

VOID CPlayerTransaction::SetVecWarehouseTradeItemName(_In_ CONST std::vector<std::wstring>& Vec)
{
	_VecWarehouseTradeItemName = Vec;
}

BOOL CPlayerTransaction::PlayerDealWithWarehouse(_In_ CONST CPlayer& Player, _In_ CONST std::vector<std::wstring>& vItem, _In_ DWORD dwSpareMoney) CONST
{;
	if (CSomeFun::GetInstance().TimeOut_Condiction_GameRun(20 * 1000, [this]
	{
		AcceptDeal();
		BnsSleep(2000);
		return CObjectFunction::GetInstance().IsShow_In_GameUi(L"TradeGuestConfirmPanel");
	}))
	{
		LOG_CF_E( L"20秒内一直接受交易都失败,UI:TradeGuestConfirmPanel 一直都显示存在!");
		CancelDeal();
		return FALSE;
	}

	BnsSleep(2000);
	if (!CObjectFunction::GetInstance().IsShow_In_GameUi(L"TradeShelfPanel"))
	{
		LOG_CF_D( L"找不到UI:TradeShelfPanel");
		return FALSE;
	}

	// 直接交易物品
	std::vector<MainTask_Transaction_Item> vDealInfo;
	for (UINT i = 0; i < vItem.size() && GetPushItemCount() < 8; ++i)
	{
		UINT uMaxCount = CBagFunction::GetInstance().GetItemCount_By_Name(vItem.at(i));
		if (uMaxCount != 0)
		{
			// 添加交易物品到服务器列表
			vDealInfo.push_back(MainTask_Transaction_Item(vItem.at(i), uMaxCount));

			// 放置交易物品到窗口上
			SetTransactionItem_By_Name(vItem.at(i), uMaxCount);
		}
	}

	DWORD dwTransactionMoney = 0;
	if (CPerson::GetInstance().GetMoney() >= dwSpareMoney)
	{
		dwTransactionMoney = CPerson::GetInstance().GetMoney() - dwSpareMoney;
		SetTransactionMoney(dwTransactionMoney);
		BnsSleep(500);
	}

	BOOL bTimeoutt = CSomeFun::GetInstance().TimeOut_Condiction_GameRun(20 * 1000, [this]
	{
		BnsSleep(1000);
		return GetWarehousePushItemCount() == 0;
	});
	if (bTimeoutt)
	{
		LOG_CF_E(L"20秒之内，仓库号都没有放物品上来，交易取消");
		CancelDeal();
		return FALSE;
	}

	LOG_CF_D(L"仓库号放置物品数量%d", GetWarehousePushItemCount());
	// Lock and Sumit
	LockAndSumitTransaction_By_PushPlayer();
	
	// send to server
	return SendTransactionInfo_To_Server(Player.GetName(), vDealInfo, dwTransactionMoney);
}

BOOL CPlayerTransaction::IsHasItemSupport(_In_ CONST std::vector<std::wstring>& vWareHouse, _Out_opt_ std::wstring& wsPlayerName, _Out_opt_ DWORD& dwChannel, _Out_opt_ em_Deal_Warehouse_Status& emDealStatus) CONST
{
	VerItemSupport ItemSupport;
	ItemSupport.dwServerId = CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->dwServerId;
	ItemSupport.dwChannel = CPerson::GetInstance().GetChannel();
	ItemSupport.vWareHouse.assign(vWareHouse.begin(), vWareHouse.end());


	ItemSupportResult Result;
	SocketAPI.ClientTrade(em_Trade_Warehouse_Type::em_Trade_Warehouse_Type_ItemSupport, ItemSupport, Result);


	if (Result.emDealStatus == em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_None)
		return FALSE;


	wsPlayerName = Result.wsWareHouseName;
	dwChannel = Result.dwChannel;
	emDealStatus = Result.emDealStatus;
	return TRUE;
}

em_Deal_Warehouse_IsPick CPlayerTransaction::IsHasInviateTransaction_By_MainTaskAccount(_Out_opt_ std::wstring& wsPlayerName) CONST
{
	WareHouseInfo Info;
	Info.wsWareHouseName = CPerson::GetInstance().GetName();
	Info.dwChannel = CPerson::GetInstance().GetChannel();
	Info.dwServerId = CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->dwServerId;

	WarehouseActionResult Result;
	SocketAPI.WarehouseTrade(em_Trade_Warehouse_Type::em_Trade_Warehouse_Type_ItemSupport, Info, Result);

	if (Result.emIsPick == em_Deal_Warehouse_IsPick::em_Deal_Warehouse_IsPick_Pick)
		wsPlayerName = Result.wsPlayerName;

	return Result.emIsPick;
}

em_Deal_Warehouse_IsPick CPlayerTransaction::IsHasInviateTransaction_By_GiveItem(_Out_opt_ std::wstring& wsPlayerName) CONST
{
	WareHouseInfo Info;
	Info.wsWareHouseName = CPerson::GetInstance().GetName();
	Info.dwChannel = CPerson::GetInstance().GetChannel();
	Info.dwServerId = CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->dwServerId;

	WarehouseActionResult Result;
	SocketAPI.WarehouseTrade(em_Trade_Warehouse_Type::em_Trade_Warehouse_Type_PickItem, Info, Result);

	if (Result.emIsPick == em_Deal_Warehouse_IsPick::em_Deal_Warehouse_IsPick_Pick)
		wsPlayerName = Result.wsPlayerName;

	return Result.emIsPick;
}

BOOL CPlayerTransaction::IsHasPickItem(_In_ CONST std::vector<std::wstring>& vWareHouse, _Out_opt_ std::wstring& wsPlayerName, _Out_opt_ DWORD& dwChannel, _Out_opt_ em_Deal_Warehouse_Status& emDealStatus) CONST
{
	VerItemSupport ItemSupport;
	ItemSupport.dwServerId = CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->dwServerId;
	ItemSupport.dwChannel = CPerson::GetInstance().GetChannel();
	ItemSupport.vWareHouse.assign(vWareHouse.begin(), vWareHouse.end());


	ItemSupportResult Result;
	SocketAPI.ClientTrade(em_Trade_Warehouse_Type::em_Trade_Warehouse_Type_PickItem, ItemSupport, Result);


	if (Result.emDealStatus == em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_None)
		return FALSE;


	wsPlayerName = Result.wsWareHouseName;
	dwChannel = Result.dwChannel;
	emDealStatus = Result.emDealStatus;
	return TRUE;
}

BOOL CPlayerTransaction::IsEnoughItemSupport_By_WareHouseAccount(_In_ CONST std::vector<MainTask_Transaction_Item>& vItem, _Out_opt_ MainTask_Transaction_Item& LackItemName) CONST
{
	for (CONST auto& MainTask_Transaction_Item_ : vItem)
	{
		if (CBagFunction::GetInstance().GetItemCount_By_Name(MainTask_Transaction_Item_.wsItemName) < MainTask_Transaction_Item_.dwCount)
		{
			LackItemName = MainTask_Transaction_Item_;
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CPlayerTransaction::WareHouseDealWithSideAccount(_In_ CONST std::wstring& wsPlayerName) CONST
{
	CPlayer Player;
	if (CObjectFunction::GetInstance().IsShow_In_GameUi(L"TradeShelfPanel"))
	{
		LOG_CF_D( L"估计是上一个界面留下来的, 直接关闭!");
		CancelDeal();
		return FALSE;
	}

	if(CSomeFun::GetInstance().TimeOut_Condiction_GameRun(5 * 1000, [&wsPlayerName, &Player]{
		return !CPlayerExtend::GetInstance().FindPlayer_By_Name(wsPlayerName, CBaseObject::CreateFixPointInfo(30.0f), &Player);
	}))
	{
		LOG_CF_D( L"5s之内30m范围内找不到练级小号:%s", wsPlayerName.c_str());
		return FALSE;
	}

	CConsoleLog(L"正在走向小号……");
	BnsMove(Player.GetPoint(), 2);

	if (CSomeFun::GetInstance().TimeOut_Condiction_GameRun(30 * 1000, [&Player]{
		return Player.GetDis() > 3.0f;
	}))
	{
		LOG_CF_D( L"3秒内对方都没靠近自己3m, 距离是:%.2f! 放弃他!", Player.GetDis());
		return FALSE;
	}

	// 等待发出邀请交易成功!
	if (CSomeFun::GetInstance().TimeOut_Condiction_GameRun(20 * 1000, [&Player, &wsPlayerName]{
		BnsSleep(3000);
		// 向该玩家发出交易邀请
		LOG_CF_D( L"向玩家:%s发出交易邀请!", wsPlayerName.c_str());
		CConsoleLog(MyTools::CCharacter::MakeFormatText(L"向玩家:%s发出交易邀请!", wsPlayerName.c_str()));
		Player.InviteTansaction();
		return !CObjectFunction::GetInstance().IsShow_In_GameUi(L"TradeShelfPanel");
	}))
	{
		LOG_CF_D( L"20秒内对方都没接受邀请! 放弃他!");
		CancelDeal();
		return FALSE;
	}

	if (!CObjectFunction::GetInstance().IsShow_In_GameUi(L"TradeShelfPanel"))
	{
		LOG_CF_D( L"20秒内对方都没接受邀请! 放弃他!");
		CancelDeal();
		return FALSE;
	}

	CConsoleLog(MyTools::CCharacter::MakeFormatText(L"对方已经接受交易!"));
	LOG_CF_D( L"对方已经接受交易!");
	BnsSleep(1000);
	return LockAndSumitTransaction_By_PullPlayer();
}

BOOL CPlayerTransaction::SaveItem_By_WareHouseNpc(_In_ CONST std::wstring& wsNpcName)
{
	// 如果当前背包格子数量少于8个, 就去存仓!
	if (!CBagFunction::GetInstance().IsBagFull(CObjectFunction::em_Bag_Type_Bag, 8 + 1))
		return TRUE;

	// 判断仓库是否满了
	if (!CBagFunction::GetInstance().IsBagFull(CObjectFunction::em_Bag_Type_Warehouse, 8 + 1))
	{
		LOG_CF_D(L"存入仓库!");
		CBagAction::GetInstance().SaveItem(CObjectFunction::em_Bag_Type_Warehouse, wsNpcName);
		return TRUE;
	}

	// 判断衣柜是否满了
	if (!CBagFunction::GetInstance().IsBagFull(CObjectFunction::em_Bag_Type_Wardrobe, 8 + 1))
	{
		LOG_CF_D(L"存入衣柜!");
		CBagAction::GetInstance().SaveItem(CObjectFunction::em_Bag_Type_Wardrobe, wsNpcName);
		return TRUE;
	}

	// 如果背包还是满的话, 那么就存点药和包子(必须当前背包少于8个格子&&衣柜必须大于8个格子)
	if (CBagFunction::GetInstance().IsBagFull(CObjectFunction::em_Bag_Type_Bag, 8 + 1) && !CBagFunction::GetInstance().IsBagFull(CObjectFunction::em_Bag_Type_Wardrobe, 8 + 1))
	{
		// 存4组药和4组包子
		LOG_CF_D( L"怒存4组包子和4组药!");
		CFilter::GetInstance().AddItemFilter(恢复药, CFilter::ItemFilterType::Filter_Item_Save);
		CFilter::GetInstance().AddItemFilter(包子, CFilter::ItemFilterType::Filter_Item_Save);

		CBagAction::GetInstance().SaveItem(CObjectFunction::em_Bag_Type_Wardrobe, wsNpcName);

		CFilter::GetInstance().AddItemFilter(恢复药, CFilter::ItemFilterType::Filter_Item_Default);
		CFilter::GetInstance().AddItemFilter(包子, CFilter::ItemFilterType::Filter_Item_Default);
	}

	return TRUE;
}

BOOL CPlayerTransaction::TakeItemSupport_By_WareHouseNpc(_In_ CONST std::wstring& wsNpcName, _In_ CONST std::vector<MainTask_Transaction_Item>& vItem) CONST
{
	// 获取哪个物资不足!
	std::vector<std::wstring> vPullItem;
	for (CONST auto& MainTask_Transaction_Item_ : vItem)
	{
		if (CBagFunction::GetInstance().GetItemCount_By_Name(MainTask_Transaction_Item_.wsItemName) < MainTask_Transaction_Item_.dwCount)
			vPullItem.push_back(MainTask_Transaction_Item_.wsItemName);
	}
	return CBagAction::GetInstance().PullItem_By_WareHouse(wsNpcName, vPullItem);
}

BOOL CPlayerTransaction::DealWithPlayer_By_Other(_In_ _em_WarehouseType em_WarehouseType) CONST
{
	if (!CObjectFunction::GetInstance().IsShow_In_GameUi(L"TradeGuestConfirmPanel"))
		return FALSE;

	LOG_CF_D( L"有号发出申请交易!");
	if (em_WarehouseType == em_WarehouseType_Active) // 主动交易情况下不接被动交易的锅
	{
		return CSomeFun::GetInstance().SimlationKey('N', 200, 800);
	}

	if (CSomeFun::GetInstance().TimeOut_Condiction_GameRun(20 * 1000, [this]{
		AcceptDeal();
		BnsSleep(2000);
		return CObjectFunction::GetInstance().IsShow_In_GameUi(L"TradeGuestConfirmPanel");
	}))
	{
		LOG_CF_E( L"20秒内一直接受交易都失败,UI:TradeGuestConfirmPanel 一直都显示存在!");
		CancelDeal();
		return FALSE;
	}

	BnsSleep(2000);
	if (!CObjectFunction::GetInstance().IsShow_In_GameUi(L"TradeShelfPanel"))
	{
		LOG_CF_D( L"找不到UI:TradeShelfPanel 找不到交易主界面的UI!");
		CancelDeal();
		return FALSE;
	}
	return LockAndSumitTransaction_By_PullPlayer();
}

BOOL CPlayerTransaction::CancelDeal() CONST
{
	CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"TradeGuestConfirmPanel", [](CONST CGameUi& TradeGuestConfirmPanel){ TradeGuestConfirmPanel.CloseGameWindow(); });
	CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"TradeShelfPanel", [](CONST CGameUi& TradeShelfPanel){ TradeShelfPanel.CloseGameWindow(); });
	BnsSleep(1000);
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([]{CGameSend::CancelDeal(); });
}

BOOL CPlayerTransaction::SendTransactionInfo_To_Server(_In_ CONST std::wstring& wsWareHouseName, _In_ CONST std::vector<MainTask_Transaction_Item>& vlst, _In_ DWORD dwTrancationMoney) CONST
{
	TradeContent Content;
	Content.dwTradeMoney = dwTrancationMoney;
	Content.wsWareHouseName = wsWareHouseName;
	for (CONST auto& itm : vlst)
	{
		TradeItem Item;
		Item.wsItemName = itm.wsItemName;
		Item.dwCount = itm.dwCount;
		Content.VecTradeItem.push_back(std::move(Item));
	}
	

	SocketAPI.AddTradeContentLog(Content);
	return TRUE;
}

BOOL CPlayerTransaction::AcceptDeal() CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([]{CGameSend::AcceptDeal(); });
}

DWORD CPlayerTransaction::GetPushItemCount() CONST
{
	DWORD dwPushItemCount = 0;
	CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"TradeShelfPanel", [&dwPushItemCount](CONST CGameUi& TradeShelfPanel)
	{
		dwPushItemCount = ReadDWORD(ReadDWORD(TradeShelfPanel.GetObj() + 放置交易物品偏移) + 交易物品数量偏移);
	});
	return dwPushItemCount;
}

DWORD CPlayerTransaction::GetWarehousePushItemCount() CONST
{
	DWORD dwPushItemCount = 0;
	CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"TradeShelfPanel", [&dwPushItemCount](CONST CGameUi& TradeShelfPanel)
	{
		dwPushItemCount = ReadDWORD(ReadDWORD(TradeShelfPanel.GetObj() + 放置交易物品偏移) + 仓库号交易物品数量偏移);
	});
	return dwPushItemCount;
}

BOOL CPlayerTransaction::LockAndSumitTransaction_By_PushPlayer() CONST
{
	// 锁定交易界面, 并且等待对方也锁定
	LockTradePanel();
	CConsoleLog(L"锁定交易界面, 并且等待对方也锁定");
	UINT uCount = 0;
	if (CSomeFun::GetInstance().TimeOut_Condiction_GameRun(60 * 1000, [this, &uCount]
	{
		BnsSleep(1000);
		if (uCount++ % 5 == 0 && !CObjectFunction::GetInstance().IsShow_In_GameUi(L"TradeShelfPanel"))
		{
			return false;
		}
		return !IsLockOtherPlayerPanel();
	}))
	{
		LOG_CF_D( L"对方60秒内也没锁定, 取消交易!");
		CancelDeal();
		return FALSE;
	}

	return ConformTransaction();
}

BOOL CPlayerTransaction::LockAndSumitTransaction_By_PullPlayer() CONST
{
	// Give Item
	LOG_CF_D(L"_VecWarehouseTradeItemName.size=%d", _VecWarehouseTradeItemName.size());
	if (_VecWarehouseTradeItemName.size() != 0)
	{
		for (CONST auto& itm : _VecWarehouseTradeItemName)
		{
			CBag Item;
			if (CBagFunction::GetInstance().ExistItem_By_Name(itm, &Item))
			{
				LOG_CF_D(L"现在给小号的物品是%s", Item.GetName().c_str());
				SetTransactionItem_By_Name(Item.GetName(), 5);
				break;
			}
		}
	}
	

	// 等待对方锁定! 60s
	CConsoleLog(L"等待对方锁定交易!");
	UINT uCount = 0;
	if (CSomeFun::GetInstance().TimeOut_Condiction_GameRun(120 * 1000, [this, &uCount]
	{
		BnsSleep(1000);
		if (uCount++ % 5 == 0 && !CObjectFunction::GetInstance().IsShow_In_GameUi(L"TradeShelfPanel"))
		{
			return false;
		}
		return !IsLockOtherPlayerPanel();
	}))
	{
		LOG_CF_D( L"120秒之内对方都没锁定, 直接放弃!");
		CancelDeal();
		return FALSE;
	}

	CConsoleLog(L"对方已经锁定交易了!");
	LockTradePanel();
	BnsSleep(500);
	
	return ConformTransaction();
}

BOOL CPlayerTransaction::ConformTransaction() CONST
{
	SumitTransaction();
	if (CSomeFun::GetInstance().TimeOut_Condiction_GameRun(10 * 1000, []
	{
		return CObjectFunction::GetInstance().IsShow_In_GameUi(L"TradeShelfPanel");
	}))
	{
		LOG_CF_D( L"对方10秒之内没点确认交易, 取消交易");
		CancelDeal();
		return FALSE;
	}
	LOG_CF_D( L"交易完毕!");
	CConsoleLog(L"交易完毕");
	return CSomeFun::GetInstance().SimlationKey('Y', 200, 800);
}

BOOL CPlayerTransaction::InviatePlayerDeal_By_Warehouse(_In_ CONST CPlayer& Player) CONST
{
	if (CSomeFun::GetInstance().TimeOut_Condiction_GameRun(30 * 1000, [&Player]{return Player.GetDis() > 3.0f;}))
	{
		LOG_CF_D( L"30秒内对方都没靠近自己3m, 距离是:%.2f! 放弃他!", Player.GetDis());
		return FALSE;
	}

	// 等待发出邀请交易成功!
	if (CSomeFun::GetInstance().TimeOut_Condiction_GameRun(20 * 1000, [&Player]
	{
		BnsSleep(3000);
		// 向该玩家发出交易邀请
		LOG_CF_D( L"向玩家:%s发出交易邀请!", Player.GetName().c_str());
		CConsoleLog(MyTools::CCharacter::MakeFormatText(L"向玩家:%s发出交易邀请!", Player.GetName().c_str()));
		Player.InviteTansaction();
		return !CObjectFunction::GetInstance().IsShow_In_GameUi(L"TradeInviterConfirmPanel") && !CObjectFunction::GetInstance().IsShow_In_GameUi(L"TradeShelfPanel");
	}))
	{
		LOG_CF_D( L"20秒内对方都没接受邀请! 放弃他!");
		CancelDeal();
		return FALSE;
	}
	return CObjectFunction::GetInstance().IsShow_In_GameUi(L"TradeShelfPanel");
}


BOOL CPlayerTransaction::BuyItem_By_List(_In_ CONST MainTask_Transaction_Item& MTitm) CONST
{
	struct ItemIndex
	{
		std::wstring wsNpcName;
		std::wstring wsItemName;
		UINT uIndex;
		UINT uCount;
	};

	CONST static std::vector<ItemIndex> vlst = 
	{
		{ L"노점방", 包子, 0, 1000 },
		{ L"노점방", 恢复药, 1, 1000 },
		{ L"이무기", 修理锤, 0, 1000 },
	};

	ItemIndex ItemIndex_;
	if (!MyTools::CLPublic::Vec_find_if(vlst, &ItemIndex_, [&MTitm](CONST ItemIndex& itm){return itm.wsItemName == MTitm.wsItemName;}))
	{
		LOG_CF_E( L"不支持该%s物品物资的购买!", MTitm.wsItemName.c_str());
		return FALSE;
	}

	return CNpcExtend::GetInstance().DoAction_By_ExistNpc_For_Name(ItemIndex_.wsNpcName, CBaseObject::CreateFixPointInfo(30), [&ItemIndex_](CONST CNpc& Npc)
	{
		BnsMove(Npc.GetPoint(), 3.0f);
		BnsSleep(2000);

		CConsoleLog(MyTools::CCharacter::MakeFormatText(L"购买物资:%s, 数量:%d", ItemIndex_.wsItemName.c_str(), ItemIndex_.uCount));
		LOG_CF_D( L"购买物资:%s, 数量:%d", ItemIndex_.wsItemName.c_str(), ItemIndex_.uCount);
		Npc.BuyItem(ItemIndex_.uIndex, ItemIndex_.uCount);
		BnsSleep(500);
	});
}

VOID CPlayerTransaction::SetWarehouseChannel() CONST
{
	CONST auto pWarehouseConfig = CGameVariable::GetInstance().GetWarehouseConfig();
	if (pWarehouseConfig == nullptr)
	{
		LOG_MSG_CF(L"当前角色并不是仓库号!!!");
		return;
	}

	CConsoleLog(MyTools::CCharacter::MakeFormatText(L"仓库号换到配置文件的频道:%d", pWarehouseConfig->dwChannel));
	if (pWarehouseConfig->dwChannel == 0)
	{
		LOG_CF_D( L"0频道,固定频道!");
		return;
	}
	CFarm::GetInstance().SetFixChannel(pWarehouseConfig->dwChannel, TRUE);
	CConsoleLog(MyTools::CCharacter::MakeFormatText(L"到达频道:%d,开始运行!", pWarehouseConfig->dwChannel));
}
