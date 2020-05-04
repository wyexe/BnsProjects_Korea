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
	// ���� �ֿ�Ŷ�С�ŵĽ���
	enum _em_WarehouseType{ em_WarehouseType_Passive, em_WarehouseType_Active };
	enum em_Warehouse_Action{ em_Warehouse_Action_Medicine, em_Warehouse_Action_Eat, em_Warehouse_Action_Hammer, em_Warehouse_Action_Save, em_Warehouse_Action_City };
	BOOL Transaction_By_Task_To_WarehouseAccount(_In_ _em_WarehouseType em_WarehouseType, _In_ CONST std::vector<MainTask_Transaction_Item>& vItem, _In_ DWORD dwMoney, _In_ std::function<VOID(em_Warehouse_Action)> fnPtr);

	// ���� С�ŶԲֿ�ŵĽ���(����)
	BOOL Transaction_By_Task_ToMainTaskAccount(_In_ std::function<VOID(CONST std::wstring&)> fnPtr) CONST;

	// �ջ�С�ŶԲֿ�ŵĽ���
	BOOL Transaction_By_Finish(_In_ CONST std::vector<std::wstring>& vItem, _In_ DWORD dwSpareMoney, _In_ std::function<VOID(CONST std::wstring&)> fnPtr) CONST;

	// �ж����ʺͽ�Ǯ�Ƿ��㹻
	BOOL IsExistItemList(_In_ CONST std::vector<std::wstring>& vItem, _In_ DWORD dwSpareMoney) CONST;

	// �ֿ�Ÿ�С����Ʒ
	VOID SetVecWarehouseTradeItemName(_In_ CONST std::vector<std::wstring>& Vec);
private:
	// ���ý�����Ʒ
	BOOL SetTransactionItem_By_Name(_In_ CONST std::wstring& wsItemName, _In_ DWORD dwCount) CONST;

	BOOL PushItemToTradePanel(_In_ CONST CGameUi& TradeShelfPanel, _In_ CONST std::wstring& wsItemName, _In_ DWORD dwCount) CONST;

	// ���ý��׽�Ǯ
	BOOL SetTransactionMoney(_In_ DWORD dwMoney) CONST;

	// �������׿�
	BOOL LockTradePanel() CONST;

	// ȡ������
	BOOL CancelDeal() CONST;

	// �Լ��Ƿ������˽��׿�!
	BOOL IsLockSelfPanel() CONST;

	// �Է��Ƿ��������׿�
	BOOL IsLockOtherPlayerPanel() CONST;

	// ȷ�Ͻ���!
	BOOL SumitTransaction() CONST;

	// ���޽���С�Ž��׵Ķ���
	BOOL DealWithPlayer_By_Other(_In_ _em_WarehouseType em_WarehouseType) CONST;

	// ��ȡ������Ʒ������
	DWORD GetPushItemCount() CONST;

	//�ֿ�Ž�����Ʒ����
	DWORD GetWarehousePushItemCount() CONST;

	// �������� by �������Ǳߵ�
	BOOL LockAndSumitTransaction_By_PushPlayer() CONST;

	// �������� by �ö����Ǳߵ�
	BOOL LockAndSumitTransaction_By_PullPlayer() CONST;

	// ����!
	BOOL ConformTransaction() CONST;

	// �ֿ�����뽻��С��
	BOOL InviatePlayerDeal_By_Warehouse(_In_ CONST CPlayer& Player) CONST;

	// ��������
	BOOL BuyItem_By_List(_In_ CONST MainTask_Transaction_Item& Itm) CONST;
private:
	// �ֿ�Ž���С��, ����������
	BOOL DealWithPlayer_By_MainTask(_In_ CONST std::wstring& wsPlayerName) CONST;

	// �ֿ���ʼ�С��, ����������
	BOOL MailWithPlayer_By_MainTask(_In_ CONST std::wstring& wsPlayerName);

	// ��ȡ�ֿ��
	UINT GetWarehourseNameList(_Out_opt_ std::vector<std::wstring>& vNameList) CONST;

	// С�ŴӲֿ�������������
	BOOL DealWithWarehour_By_MainTask(_In_ CONST CPlayer& Player) CONST;

	// �жϲֿ�����ϵ������Ƿ��㹻
	BOOL IsEnoughItemSupport_By_WareHouseAccount(_In_ CONST std::vector<MainTask_Transaction_Item>& vItem, _Out_opt_ MainTask_Transaction_Item& LackItemName) CONST;

	// С�Ÿ������ֿ�!
	BOOL PlayerDealWithWarehouse(_In_ CONST CPlayer& Player, _In_ CONST std::vector<std::wstring>& vItem, _In_ DWORD dwSpareMoney) CONST;

	// �ֿ���ջ�
	BOOL WareHouseDealWithSideAccount(_In_ CONST std::wstring& wsPlayerName) CONST;

	// ȥ�ֿ�ȡ����
	BOOL TakeItemSupport_By_WareHouseNpc(_In_ CONST std::wstring& wsNpcName, _In_ CONST std::vector<MainTask_Transaction_Item>& vItem) CONST;

	// �Ѷ��������
	BOOL SaveItem_By_WareHouseNpc(_In_ CONST std::wstring& wsNpcName);

	// ���ܽ���
	BOOL AcceptDeal() CONST;

	// �ֿ�Ż���
	VOID SetWarehouseChannel() CONST;
private: // SOCKET
	// �Ƿ�������С����������֧Ԯ!
	em_Deal_Warehouse_IsPick IsHasInviateTransaction_By_MainTaskAccount(_Out_opt_ std::wstring& wsPlayerName) CONST;

	// �Ƿ�������С�������ջ�
	em_Deal_Warehouse_IsPick IsHasInviateTransaction_By_GiveItem(_Out_opt_ std::wstring& wsPlayerName) CONST;

	// ���͵�ǰ�ʺ�ȥ������, �ȴ��ջ�!
	BOOL IsHasPickItem(_In_ CONST std::vector<std::wstring>& vWareHouse, _Out_opt_ std::wstring& wsPlayerName, _Out_opt_ DWORD& dwChannel, _Out_opt_ em_Deal_Warehouse_Status& emDealStatus) CONST;

	// �Ƿ�������֧Ԯ�Ĳֿ����Ϣ����
	BOOL IsHasItemSupport(_In_ CONST std::vector<std::wstring>& vWareHouse, _Out_opt_ std::wstring& wsPlayerName, _Out_opt_ DWORD& dwChannel, _Out_opt_ em_Deal_Warehouse_Status& emDealStatus) CONST;

	// ���ͽ������ݵ�������
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