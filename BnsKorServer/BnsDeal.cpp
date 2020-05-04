#include "BnsDeal.h"
#include <MyTools/ByteBuffer.h>

CLLock CBnsDeal::vItemSupportLock(L"CBnsDeal::vItemSupportLock");
CLLock CBnsDeal::vPickitemLock(L"CBnsDeal::vPickitemLock");
CLLock CBnsDeal::LockWareHouse(L"CBnsDeal::LockWareHouse");
#define _SELF L"BnsDeal.cpp"


BOOL CBnsDeal::JoinItemSupportDeal(_In_ CONST VerItemSupport& VerItemSupport_, _Out_ ByteBuffer& ByteBuffer_)
{
	return vItemSupportLock.Access([this, &VerItemSupport_, &ByteBuffer_]
	{
		INT uIndex = 0;
		if (ExistItemSupport(vItemSupport_OnLine, VerItemSupport_, uIndex))
		{
			auto& ItemSupportInfo_ = vItemSupport_OnLine.at(uIndex);
			ItemSupportInfo_.VerItemSupport_ = VerItemSupport_;

			ByteBuffer_ << SOCK_MSG_CLIENT_ITEMSUPPORT;
			ByteBuffer_ << static_cast<DWORD>(ItemSupportInfo_.emDealStatus);
			switch (ItemSupportInfo_.emDealStatus)
			{
			case em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_None:
				//LOG_F(CLog::em_Log_Type::em_Log_Type_Debug, L"��������С��:[%s,%s] ServerId=%X, channel=%d ������", VerItemSupport_.wsAccountName.c_str(), VerItemSupport_.wsPlayerName.c_str(), VerItemSupport_.dwServerId, VerItemSupport_.dwChannel);
				ItemSupportInfo_.ulKeepALiveTick = ::GetTickCount64();
				break;
			case em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_Channel:
				//LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"������������֧Ԯ��С��:%s ,�ֿ��:%s ��Ƶ��%d, ���л�Ƶ��!", VerItemSupport_.wsPlayerName.c_str(), ItemSupportInfo_.WareHouseInfo_.wsWareHouseName.c_str(), ItemSupportInfo_.WareHouseInfo_.dwChannel);
				ByteBuffer_ << ItemSupportInfo_.WareHouseInfo_.wsWareHouseName;
				ByteBuffer_ << ItemSupportInfo_.WareHouseInfo_.dwChannel;
				ItemSupportInfo_.ulKeepALiveTick = ::GetTickCount64();
				//vItemSupport_OnLine.erase(vItemSupport_OnLine.begin() + uIndex);
				break;
			default:
				break;
			}
		}
		else
		{
			ItemSupportInfo ItemSupportInfo_;
			ItemSupportInfo_.VerItemSupport_ = VerItemSupport_;
			vItemSupport_OnLine.push_back(ItemSupportInfo_);
			LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"����С��:�����ڼ�¼, �����ȥ! CurName=[%s,%s], vItemSupport_OnLine.size()=%d, ServerId=%X, MaxCount=%d", \
				VerItemSupport_.wsAccountName.c_str(), VerItemSupport_.wsPlayerName.c_str(), vItemSupport_OnLine.size(), VerItemSupport_.dwServerId, vItemSupport_OnLine.size());

			ByteBuffer_ << SOCK_MSG_CLIENT_ITEMSUPPORT;
			ByteBuffer_ << static_cast<DWORD>(em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_None);
		}
	});
}

BOOL CBnsDeal::ExistItemSupport(_In_ std::deque<ItemSupportInfo>& vlst, _In_ CONST VerItemSupport& VerItemSupport_, _Out_opt_ INT& uIndex)
{
	for (uIndex = 0; uIndex < static_cast<int>(vlst.size()); ++uIndex)
	{
		CONST auto& itm = vlst.at(uIndex);
		if (static_cast<DWORD>(::GetTickCount64() - itm.ulKeepALiveTick) >= MAX_DEAL_KEEPALIVE_TIMEOUT || (itm.bTake && static_cast<DWORD>(::GetTickCount64() - itm.ulTakeTick) >= MAX_DEAL_KEEPALIVE_TIMEOUT))
		{
			//LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"С��:%s 15s֮��û�ֿ�Žӵ�! ��ʱ%dms, ����Server=%X,Channel=%d", itm.VerItemSupport_.wsPlayerName.c_str(), (DWORD)(::GetTickCount64() - itm.ulKeepALiveTick), itm.VerItemSupport_.dwServerId, itm.VerItemSupport_.dwChannel);
			vlst.erase(vlst.begin() + uIndex--);
			continue;
		}
		if (itm.VerItemSupport_.wsPlayerName == VerItemSupport_.wsPlayerName && CVerManage::IsCompServer(itm.VerItemSupport_.dwServerId, VerItemSupport_.dwServerId))
			return TRUE;
	}
	return FALSE;
}

BOOL CBnsDeal::AcceptItemSupportDeal(_In_ CONST WareHouseInfo& WareHouseInfo_, _Out_ ByteBuffer& ByteBuffer_)
{
	return vItemSupportLock.Access([this, &WareHouseInfo_, &ByteBuffer_]
	{
		auto pWareHouseInfo = FindWarehouse(WareHouseInfo_);
		if (pWareHouseInfo == nullptr)
		{
			LOG_C(CLog::em_Log_Type::em_Log_Type_Exception, L"pWareHouseInfo=nullptr");
			ByteBuffer_ << SOCK_MSG_CLIENT_WAREHOUSE_PICKITEM;
			ByteBuffer_ << (DWORD)em_Deal_Warehouse_IsPick::em_Deal_Warehouse_IsPick_None;
		}
		else if (static_cast<DWORD>(::GetTickCount64() - pWareHouseInfo->ulTick >= 10 * 60 * 1000))
		{
			// ÿ10�������¼��㽻������
			pWareHouseInfo->ulTick = ::GetTickCount64();
		}

		BOOL bExist = ExistWareHouse(vItemSupport_OnLine, WareHouseInfo_, TRUE, [&ByteBuffer_, &WareHouseInfo_, &pWareHouseInfo](ItemSupportInfo& ItemSupportInfo_)
		{
			pWareHouseInfo->ulTick = ::GetTickCount64();
			ItemSupportInfo_.WareHouseInfo_ = WareHouseInfo_;
			ItemSupportInfo_.bTake = TRUE;
			ItemSupportInfo_.ulTakeTick = ::GetTickCount64();
			ByteBuffer_ << SOCK_MSG_CLIENT_WAREHOUSE_MAINTASK;
			ByteBuffer_ << (DWORD)em_Deal_Warehouse_IsPick::em_Deal_Warehouse_IsPick_Pick;
			ByteBuffer_ << ItemSupportInfo_.VerItemSupport_.wsPlayerName;

			LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"���ǲֿ��[%s,%X,%d]:���ں��ʵ�С��:%s! ׼������!", \
				ItemSupportInfo_.WareHouseInfo_.wsWareHouseName.c_str(), ItemSupportInfo_.WareHouseInfo_.dwServerId, ItemSupportInfo_.WareHouseInfo_.dwChannel, ItemSupportInfo_.VerItemSupport_.wsPlayerName.c_str());
		});

		if (!bExist)
		{
			if (ExistWareHouse(vPickItem_OnLine, WareHouseInfo_) && static_cast<DWORD>(::GetTickCount64() - pWareHouseInfo->ulTick) >= 5 * 60 * 1000)
			{
				// ��������С��, ������һ�ν��׳ɹ�������������5����֮��
				LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"���ǲֿ��,5���Ӷ�û����, ������������·�ĺ�! �òֿ��С��!");
				ByteBuffer_ << SOCK_MSG_CLIENT_WAREHOUSE_PICKITEM;
				ByteBuffer_ << (DWORD)em_Deal_Warehouse_IsPick::em_Deal_Warehouse_IsPick_BackToSwitchPlayer;
			}
			else
			{
				ByteBuffer_ << SOCK_MSG_CLIENT_WAREHOUSE_MAINTASK;
				ByteBuffer_ << (DWORD)em_Deal_Warehouse_IsPick::em_Deal_Warehouse_IsPick_None;
			}
			//LOG_F(CLog::em_Log_Type::em_Log_Type_Debug, L"���ǲֿ��: �����ں��ʵ�С�Ž���!ServerId=%X, Name=%s", WareHouseInfo_.dwServerId, WareHouseInfo_.wsWareHouseName.c_str());
		}
	});
}

BOOL CBnsDeal::ExistWareHouse(_In_ std::deque<ItemSupportInfo>& vlst, _In_ CONST WareHouseInfo& WareHouseInfo_, _In_ BOOL bUnSameChannel, _In_ std::function<VOID(ItemSupportInfo&)> fn)
{
	for (int i = 0; i < static_cast<int>(vlst.size()); ++i)
	{
		auto& ItemSupportInfo_ = vlst.at(i);
		// ���С��15s֮�ڶ�û��������, �͵����ߴ���
		if (static_cast<DWORD>(::GetTickCount64() - ItemSupportInfo_.ulKeepALiveTick) >= MAX_DEAL_KEEPALIVE_TIMEOUT || (ItemSupportInfo_.bTake && static_cast<DWORD>(::GetTickCount64() - ItemSupportInfo_.ulTakeTick) >= MAX_DEAL_KEEPALIVE_TIMEOUT))
		{
			// 10s ��������ֱ�ӵ����ߴ���!
			//LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"С��:%s 15s֮��û�ֿ�Žӵ���! ��ʱ%dms", ItemSupportInfo_.VerItemSupport_.wsPlayerName.c_str(), (DWORD)(::GetTickCount64() - ItemSupportInfo_.ulKeepALiveTick));
			vlst.erase(vlst.begin() + i--);
			continue;
		}

		// �Աȷ������Ƿ�һ��!
		if (CVerManage::IsCompServer(ItemSupportInfo_.VerItemSupport_.dwServerId, WareHouseInfo_.dwServerId) && !ItemSupportInfo_.bTake/*�����ǻ�û���ֿ�Žӵ���*/)
		{
			// �ֿ������С�ŵĲֿ��ɫ�б�, �ж��Ƿ����!
			if (CLPublic::Vec_find_if_Const(ItemSupportInfo_.VerItemSupport_.vWareHouse, [&WareHouseInfo_](CONST std::wstring& wsWareHouseName){return WareHouseInfo_.wsWareHouseName == wsWareHouseName; }) != nullptr)
			{
				// ������һ����û�л���Ƶ����С��, �ȷ����л�Ƶ���İ���ȥ
				ItemSupportInfo_.emDealStatus = em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_Channel;
				ItemSupportInfo_.WareHouseInfo_ = WareHouseInfo_;
				//LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"bSameChannel=%X, �ֿ�ŵ�Ƶ��=%d, С�ŵ�Ƶ��=%d", bSameChannel, WareHouseInfo_.dwChannel, ItemSupportInfo_.VerItemSupport_.dwChannel);
				if (bUnSameChannel || WareHouseInfo_.dwChannel == ItemSupportInfo_.VerItemSupport_.dwChannel)
				{
					fn(ItemSupportInfo_);
					return TRUE;
				}
				continue;
			}
		}
	}
	return FALSE;
}

BOOL CBnsDeal::ExistWareHouse(_In_ std::deque<ItemSupportInfo>& vlst, _In_ CONST WareHouseInfo& WareHouseInfo_)
{
	for (CONST auto& itm : vlst)
	{
		if (CVerManage::IsCompServer(itm.VerItemSupport_.dwServerId, WareHouseInfo_.dwServerId))
		{
			// �ֿ������С�ŵĲֿ��ɫ�б�, �ж��Ƿ����!
			if (CLPublic::Vec_find_if_Const(itm.VerItemSupport_.vWareHouse, [&WareHouseInfo_](CONST std::wstring& wsWareHouseName){return WareHouseInfo_.wsWareHouseName == wsWareHouseName; }) != nullptr)
				return TRUE;
		}
	}
	return FALSE;
}

BOOL CBnsDeal::JoinPickItemtDeal(_In_ CONST VerItemSupport& VerItemSupport_, _Out_ ByteBuffer& ByteBuffer_)
{
	return vPickitemLock.Access([this, &VerItemSupport_, &ByteBuffer_]
	{
		INT uIndex = 0;
		if (ExistItemSupport(vPickItem_OnLine, VerItemSupport_, uIndex))
		{
			auto& ItemSupportInfo_ = vPickItem_OnLine.at(uIndex);
			// ��������
			ItemSupportInfo_.VerItemSupport_ = VerItemSupport_;

			ByteBuffer_ << SOCK_MSG_CLIENT_PICKITEM;
			ByteBuffer_ << static_cast<DWORD>(ItemSupportInfo_.emDealStatus);
			switch (ItemSupportInfo_.emDealStatus)
			{
			case em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_None:
				ItemSupportInfo_.ulKeepALiveTick = ::GetTickCount64(); // ��������!
				break;
			case em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_Channel:
				ItemSupportInfo_.ulKeepALiveTick = ::GetTickCount64(); // ��������!
				//LOG_C(CLog::em_Log_Type::em_Log_Type_Debug, L"�����ջ�С��%s: �ֿ�����:%X, Name=%s", VerItemSupport_.wsPlayerName.c_str(), (DWORD)ItemSupportInfo_.emDealStatus, ItemSupportInfo_.WareHouseInfo_.wsWareHouseName.c_str());
				ByteBuffer_ << ItemSupportInfo_.WareHouseInfo_.wsWareHouseName;
				ByteBuffer_ << ItemSupportInfo_.WareHouseInfo_.dwChannel;
				break;
			default:
				break;
			}
		}
		else
		{
			ItemSupportInfo ItemSupportInfo_;
			ItemSupportInfo_.VerItemSupport_ = VerItemSupport_;
			ItemSupportInfo_.ulKeepALiveTick = ::GetTickCount64();
			vPickItem_OnLine.push_back(ItemSupportInfo_);
			LOG_C(CLog::em_Log_Type::em_Log_Type_Debug, L"����С��:�����ڼ�¼, �����ȥ! CurName=%s, vPickItem_OnLine.size()=%d, ServerId=%X, MaxCount=%d", \
				VerItemSupport_.wsPlayerName.c_str(), vPickItem_OnLine.size(), VerItemSupport_.dwServerId, vPickItem_OnLine.size());
			ByteBuffer_ << SOCK_MSG_CLIENT_PICKITEM;
			ByteBuffer_ << static_cast<DWORD>(em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_None);
		}
	});
}

BOOL CBnsDeal::AcceptPickItemDeal(_In_ CONST WareHouseInfo& WareHouseInfo_, _Out_ ByteBuffer& ByteBuffer_)
{
	return vPickitemLock.Access([this, &WareHouseInfo_, &ByteBuffer_]
	{
		auto pWareHouseInfo = FindWarehouse(WareHouseInfo_);
		if (pWareHouseInfo == nullptr)
		{
			LOG_C(CLog::em_Log_Type::em_Log_Type_Exception, L"pWareHouseInfo=nullptr");
			ByteBuffer_ << SOCK_MSG_CLIENT_WAREHOUSE_PICKITEM;
			ByteBuffer_ << (DWORD)em_Deal_Warehouse_IsPick::em_Deal_Warehouse_IsPick_None;
		}
		else if (static_cast<DWORD>(::GetTickCount64() - pWareHouseInfo->ulTick) >= 10 * 60 * 1000)
		{
			// ÿ10�������¼��㽻������
			pWareHouseInfo->ulTick = ::GetTickCount64();
		}

		BOOL bExist = ExistWareHouse(vPickItem_OnLine, WareHouseInfo_, FALSE, [&ByteBuffer_, &WareHouseInfo_, &pWareHouseInfo](ItemSupportInfo& ItemSupportInfo_)
		{
			ItemSupportInfo_.WareHouseInfo_ = WareHouseInfo_;
			pWareHouseInfo->ulTick = ::GetTickCount64();
			ItemSupportInfo_.bTake = TRUE;
			ItemSupportInfo_.ulTakeTick = ::GetTickCount64();
			LOG_C(CLog::em_Log_Type::em_Log_Type_Debug, L"���ǲֿ��%s:���ں��ʵ�С��:%s! ׼������!", ItemSupportInfo_.WareHouseInfo_.wsWareHouseName.c_str(), ItemSupportInfo_.VerItemSupport_.wsPlayerName.c_str());
			//ItemSupportInfo_.ulKeepALiveTick = ::GetTickCount64(); // ������������, ��ΪС�ſ����Ѿ�������!
			ByteBuffer_ << SOCK_MSG_CLIENT_WAREHOUSE_PICKITEM;
			ByteBuffer_ << (DWORD)em_Deal_Warehouse_IsPick::em_Deal_Warehouse_IsPick_Pick;
			ByteBuffer_ << ItemSupportInfo_.VerItemSupport_.wsPlayerName;
		});

		if (!bExist)
		{
			if (ExistWareHouse(vPickItem_OnLine, WareHouseInfo_) && static_cast<DWORD>(::GetTickCount64() - pWareHouseInfo->ulTick) >= 5 * 60 * 1000)
			{
				// ��������С��, ������һ�ν��׳ɹ�������������5����֮��
				LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"���ǲֿ��,5���Ӷ�û����, ������������·�ĺ�! �òֿ��С��!");
				ByteBuffer_ << SOCK_MSG_CLIENT_WAREHOUSE_PICKITEM;
				ByteBuffer_ << (DWORD)em_Deal_Warehouse_IsPick::em_Deal_Warehouse_IsPick_BackToSwitchPlayer;
			}
			else
			{
				//LOG_C(CLog::em_Log_Type::em_Log_Type_Debug, L"���ǲֿ��: �����ں��ʵ�С�Ž���!ServerId=%X, Name=%s, Ƶ��:%d", WareHouseInfo_.dwServerId, WareHouseInfo_.wsWareHouseName.c_str(), WareHouseInfo_.dwChannel);
				ByteBuffer_ << SOCK_MSG_CLIENT_WAREHOUSE_PICKITEM;
				ByteBuffer_ << (DWORD)em_Deal_Warehouse_IsPick::em_Deal_Warehouse_IsPick_None;
			}
		}
	});
}

CBnsDeal::WareHouseInfo* CBnsDeal::FindWarehouse(_In_ CONST CBnsDeal::WareHouseInfo& tmpWareHouseInfo)
{
	CBnsDeal::WareHouseInfo* pWareHouseInfo = nullptr;
	LockWareHouse.Access([this, &pWareHouseInfo, tmpWareHouseInfo]
	{
		auto itr = MapWareHouse.find(tmpWareHouseInfo.wsWareHouseName);
		if (itr == MapWareHouse.end())
		{
			MapWareHouse.insert(std::make_pair(tmpWareHouseInfo.wsWareHouseName, tmpWareHouseInfo));
			pWareHouseInfo = &MapWareHouse[tmpWareHouseInfo.wsWareHouseName];
		}
		else
		{
			MapWareHouse[tmpWareHouseInfo.wsWareHouseName] = tmpWareHouseInfo;
			pWareHouseInfo = &itr->second;
		}
	});
	return pWareHouseInfo;
}
