#ifndef __GITBNSPROJECTS_VERBNSKORSERVER_VERDEAL_BNSDEAL_H__
#define __GITBNSPROJECTS_VERBNSKORSERVER_VERDEAL_BNSDEAL_H__

#include "VerManage.h"
#include <MyTools/CLLock.h>

#define MAX_DEAL_KEEPALIVE_TIMEOUT	15 * 1000

class ByteBuffer;
class CBnsDeal : public CClassInstance<CBnsDeal>
{
public:
	typedef struct _WareHouseInfo
	{
		std::wstring wsWareHouseName;
		DWORD	dwChannel;
		DWORD	dwServerId;
		ULONGLONG ulTick;
		_WareHouseInfo()
		{
			dwChannel = dwServerId = NULL;
			ulTick = ::GetTickCount64();
		}
	}WareHouseInfo;
	typedef struct _ItemSupportInfo
	{
		ULONGLONG		ulTakeTick;
		BOOL			bTake;
		VerItemSupport	VerItemSupport_;
		em_Deal_Warehouse_Status emDealStatus;
		WareHouseInfo	WareHouseInfo_;
		ULONGLONG		ulKeepALiveTick;
		_ItemSupportInfo()
		{
			ulKeepALiveTick = ::GetTickCount64();
			emDealStatus = em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_None;
			bTake = FALSE;
			ulTakeTick = NULL;
		}
	}ItemSupportInfo;

public:
	CBnsDeal() = default;
	~CBnsDeal() = default;

public:
	// ������С�� ����֧Ԯ
	BOOL JoinItemSupportDeal(_In_ CONST VerItemSupport& VerItemSupport_, _Out_ ByteBuffer& ByteBuffer_);

	// �ֿ�� ����֧Ԯ
	BOOL AcceptItemSupportDeal(_In_ CONST WareHouseInfo& WareHouseInfo_, _Out_ ByteBuffer& ByteBuffer_);

	// �ջ�(С��)
	BOOL JoinPickItemtDeal(_In_ CONST VerItemSupport& VerItemSupport_, _Out_ ByteBuffer& ByteBuffer_);

	// // �ջ�(�ֿ��)
	BOOL AcceptPickItemDeal(_In_ CONST WareHouseInfo& WareHouseInfo_, _Out_ ByteBuffer& ByteBuffer_);
private:
	// �Ƿ����ItemSupport
	BOOL ExistItemSupport(_In_ std::deque<ItemSupportInfo>& vlst, _In_ CONST VerItemSupport& VerItemSupport_, _Out_opt_ INT& uIndex);

	// �Ƿ���ڲֿ��
	BOOL ExistWareHouse(_In_ std::deque<ItemSupportInfo>& vlst, _In_ CONST WareHouseInfo& WareHouseInfo_, _In_ BOOL bUnSameChannel, _In_ std::function<VOID(ItemSupportInfo&)> fn);
	BOOL ExistWareHouse(_In_ std::deque<ItemSupportInfo>& vlst, _In_ CONST WareHouseInfo& WareHouseInfo_);

	// �Ҳֿ��
	WareHouseInfo* FindWarehouse(_In_ CONST CBnsDeal::WareHouseInfo& tmpWareHouseInfo);
private:
	std::deque<ItemSupportInfo> vItemSupport_OnLine;
	std::deque<ItemSupportInfo> vPickItem_OnLine;
	std::map<std::wstring, WareHouseInfo>	MapWareHouse;
public:
	static CLLock vItemSupportLock;
	static CLLock vPickitemLock;
	static CLLock LockWareHouse;
};



#endif