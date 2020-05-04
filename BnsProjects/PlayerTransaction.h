#ifndef __GITBNSPROJECTS_BNSPROJECTS_SOMEFUN_DEAL_PLAYERTRANSACTION_H__
#define __GITBNSPROJECTS_BNSPROJECTS_SOMEFUN_DEAL_PLAYERTRANSACTION_H__

#include "GameVariable.h"
#include "MailAction.h"
#include "../ConsoleDLL/SocketAPI.h"

class CGameUi;
class CPlayer;


class CPlayerTransaction : public MyTools::CClassInstance<CPlayerTransaction>
{
public:
	typedef CMailAction::MailItemContent MainTask_Transaction_Item;

private:
	struct MailedPlayerContnet
	{
		std::wstring  wsPlayerName;
		UINT				uCount;
	};
public:
	CPlayerTransaction() = default;
	~CPlayerTransaction() = default;

public:
	// 主线 仓库号对小号的交易
	enum _em_WarehouseType{ em_WarehouseType_Passive, em_WarehouseType_Active };
	enum em_Warehouse_Action{ em_Warehouse_Action_Medicine, em_Warehouse_Action_Eat, em_Warehouse_Action_Hammer, em_Warehouse_Action_Save, em_Warehouse_Action_City };
	BOOL Transaction_By_Task_To_WarehouseAccount(_In_ _em_WarehouseType em_WarehouseType, _In_ CONST std::vector<MainTask_Transaction_Item>& vItem, _In_ DWORD dwMoney, _In_ std::function<VOID(em_Warehouse_Action)> fnPtr);

	// 主线 小号对仓库号的交易(物资)
	BOOL Transaction_By_Task_ToMainTaskAccount(_In_ std::function<VOID(CONST std::wstring&)> fnPtr) CONST;

	// 收货小号对仓库号的交易
	BOOL Transaction_By_Finish(_In_ CONST std::vector<std::wstring>& vItem, _In_ DWORD dwSpareMoney, _In_ std::function<VOID(CONST std::wstring&)> fnPtr) CONST;

	// 判断物资和金钱是否足够
	BOOL IsExistItemList(_In_ CONST std::vector<std::wstring>& vItem, _In_ DWORD dwSpareMoney) CONST;

	// 仓库号给小号物品
	VOID SetVecWarehouseTradeItemName(_In_ CONST std::vector<std::wstring>& Vec);
private:
	// 放置交易物品
	BOOL SetTransactionItem_By_Name(_In_ CONST std::wstring& wsItemName, _In_ DWORD dwCount) CONST;

	BOOL PushItemToTradePanel(_In_ CONST CGameUi& TradeShelfPanel, _In_ CONST std::wstring& wsItemName, _In_ DWORD dwCount) CONST;

	// 放置交易金钱
	BOOL SetTransactionMoney(_In_ DWORD dwMoney) CONST;

	// 锁定交易框
	BOOL LockTradePanel() CONST;

	// 取消交易
	BOOL CancelDeal() CONST;

	// 自己是否锁定了交易框!
	BOOL IsLockSelfPanel() CONST;

	// 对方是否锁定交易框
	BOOL IsLockOtherPlayerPanel() CONST;

	// 确认交易!
	BOOL SumitTransaction() CONST;

	// 无限接收小号交易的东西
	BOOL DealWithPlayer_By_Other(_In_ _em_WarehouseType em_WarehouseType) CONST;

	// 获取放置物品的数量
	DWORD GetPushItemCount() CONST;

	//仓库号交易物品数量
	DWORD GetWarehousePushItemCount() CONST;

	// 交易锁定 by 给东西那边的
	BOOL LockAndSumitTransaction_By_PushPlayer() CONST;

	// 交易锁定 by 拿东西那边的
	BOOL LockAndSumitTransaction_By_PullPlayer() CONST;

	// 交易!
	BOOL ConformTransaction() CONST;

	// 仓库号邀请交易小号
	BOOL InviatePlayerDeal_By_Warehouse(_In_ CONST CPlayer& Player) CONST;

	// 购买物资
	BOOL BuyItem_By_List(_In_ CONST MainTask_Transaction_Item& Itm) CONST;
private:
	// 仓库号交易小号, 给主线物资
	BOOL DealWithPlayer_By_MainTask(_In_ CONST std::wstring& wsPlayerName) CONST;

	// 仓库号邮寄小号, 给主线物资
	BOOL MailWithPlayer_By_MainTask(_In_ CONST std::wstring& wsPlayerName);

	// 获取仓库号
	UINT GetWarehourseNameList(_Out_opt_ std::vector<std::wstring>& vNameList) CONST;

	// 小号从仓库号那里接收物资
	BOOL DealWithWarehour_By_MainTask(_In_ CONST CPlayer& Player) CONST;

	// 判断仓库号身上的物资是否足够
	BOOL IsEnoughItemSupport_By_WareHouseAccount(_In_ CONST std::vector<MainTask_Transaction_Item>& vItem, _Out_opt_ MainTask_Transaction_Item& LackItemName) CONST;

	// 小号给东西仓库!
	BOOL PlayerDealWithWarehouse(_In_ CONST CPlayer& Player, _In_ CONST std::vector<std::wstring>& vItem, _In_ DWORD dwSpareMoney) CONST;

	// 仓库号收货
	BOOL WareHouseDealWithSideAccount(_In_ CONST std::wstring& wsPlayerName) CONST;

	// 去仓库取物资
	BOOL TakeItemSupport_By_WareHouseNpc(_In_ CONST std::wstring& wsNpcName, _In_ CONST std::vector<MainTask_Transaction_Item>& vItem) CONST;

	// 把东西给存仓
	BOOL SaveItem_By_WareHouseNpc(_In_ CONST std::wstring& wsNpcName);

	// 接受交易
	BOOL AcceptDeal() CONST;

	// 仓库号换线
	VOID SetWarehouseChannel() CONST;
private: // SOCKET
	// 是否有练级小号请求物质支援!
	em_Deal_Warehouse_IsPick IsHasInviateTransaction_By_MainTaskAccount(_Out_opt_ std::wstring& wsPlayerName) CONST;

	// 是否有练级小号请求收货
	em_Deal_Warehouse_IsPick IsHasInviateTransaction_By_GiveItem(_Out_opt_ std::wstring& wsPlayerName) CONST;

	// 发送当前帐号去服务器, 等待收货!
	BOOL IsHasPickItem(_In_ CONST std::vector<std::wstring>& vWareHouse, _Out_opt_ std::wstring& wsPlayerName, _Out_opt_ DWORD& dwChannel, _Out_opt_ em_Deal_Warehouse_Status& emDealStatus) CONST;

	// 是否有物资支援的仓库号消息返回
	BOOL IsHasItemSupport(_In_ CONST std::vector<std::wstring>& vWareHouse, _Out_opt_ std::wstring& wsPlayerName, _Out_opt_ DWORD& dwChannel, _Out_opt_ em_Deal_Warehouse_Status& emDealStatus) CONST;

	// 发送交易内容到服务器
	BOOL SendTransactionInfo_To_Server(_In_ CONST std::wstring& wsWareHouseName, _In_ CONST std::vector<MainTask_Transaction_Item>& vlst, _In_ DWORD dwTrancationMoney) CONST;

	DSIABLE_COPY_AND_ASSIGN(CPlayerTransaction)
private:
	std::vector<std::wstring> _VecWarehouseTradeItemName;
	std::deque<MailedPlayerContnet> VecItemSupportPlayerName;
	std::vector<MainTask_Transaction_Item> m_vItem;
	DWORD m_dwMoney;
	std::wstring _wsNpcName;
	CSocketAPI SocketAPI;
};



#endif