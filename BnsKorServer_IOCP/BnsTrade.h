#ifndef __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_TRADE_BNSTRADE_H__
#define __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_TRADE_BNSTRADE_H__

#include <deque>
#include <MyTools/ClassInstance.h>
#include "ServerCommon.h"

#define MAX_DEAL_KEEPALIVE_TIMEOUT	15 * 1000

class CBnsTrade : public MyTools::CClassInstance<CBnsTrade>
{
public:
	struct ItemSupportInfo
	{
		ULONGLONG		ulTakeTick;
		BOOL			bTake;
		VerItemSupport	VerItemSupport_;
		em_Deal_Warehouse_Status emDealStatus;
		WareHouseInfo	WareHouseInfo_;
		ULONGLONG		ulKeepALiveTick;
		ItemSupportInfo()
		{
			ulKeepALiveTick = ::GetTickCount64();
			emDealStatus = em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_None;
			bTake = FALSE;
			ulTakeTick = NULL;
		}
	};

public:
	CBnsTrade();
	~CBnsTrade() = default;

public:
	// ������С�� ����֧Ԯ
	BOOL JoinItemSupportDeal(_In_ CONST VerItemSupport& VerItemSupport_, _Out_ ItemSupportResult& Result);

	// �ֿ�� ����֧Ԯ
	BOOL AcceptItemSupportDeal(_In_ CONST WareHouseInfo& WareHouseInfo_, _Out_ WarehouseActionResult& Result);

	// �ջ�(С��)
	BOOL JoinPickItemtDeal(_In_ CONST VerItemSupport& VerItemSupport_, _Out_ ItemSupportResult& Result);

	// // �ջ�(�ֿ��)
	BOOL AcceptPickItemDeal(_In_ CONST WareHouseInfo& WareHouseInfo_, _Out_ WarehouseActionResult& Result);

	//
	VOID RemoveAccount(_In_ CONST std::wstring& wsAccountName, _In_ CONST std::wstring& wsPlayerName);
private:
	// �Ƿ����ItemSupport
	BOOL ExistItemSupport(_In_ std::deque<ItemSupportInfo>& vlst, _In_ CONST VerItemSupport& VerItemSupport_, _Out_opt_ INT& uIndex);

	// �Ƿ���ڲֿ��
	BOOL ExistWareHouse(_In_ std::deque<ItemSupportInfo>& vlst, _In_ CONST WareHouseInfo& WareHouseInfo_, _In_ BOOL bUnSameChannel, _In_ std::function<VOID(ItemSupportInfo&)> fn);
	BOOL ExistWareHouse(_In_ std::deque<ItemSupportInfo>& vlst, _In_ CONST WareHouseInfo& WareHouseInfo_);

	// �Ҳֿ��
	VOID FindWarehouse(_In_ CONST WareHouseInfo& tmpWareHouseInfo, _In_ std::function<VOID(WareHouseInfo&)> ActionPtr);

	//
	VOID RemoveAccountInItemSupport(_In_ CONST std::wstring& wsAccountName);
	VOID RemoveAccountInPickItem(_In_ CONST std::wstring& wsAccountName);
	VOID RemoveAccountInWarehouse(_In_ CONST std::wstring& wsPlayerName);
private:
	std::deque<ItemSupportInfo> vItemSupport_OnLine;
	std::deque<ItemSupportInfo> vPickItem_OnLine;
	std::map<std::wstring, WareHouseInfo>	MapWareHouse;

	std::mutex _MtxPickItem;
	std::mutex _MtxItemSupport;
	std::mutex _MtxWarehouse;
};


#endif // !__GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_TRADE_BNSTRADE_H__
