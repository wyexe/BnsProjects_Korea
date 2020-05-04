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
				//LOG_F(CLog::em_Log_Type::em_Log_Type_Debug, L"请求物资小号:[%s,%s] ServerId=%X, channel=%d 的心跳", VerItemSupport_.wsAccountName.c_str(), VerItemSupport_.wsPlayerName.c_str(), VerItemSupport_.dwServerId, VerItemSupport_.dwChannel);
				ItemSupportInfo_.ulKeepALiveTick = ::GetTickCount64();
				break;
			case em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_Channel:
				//LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"我是请求物资支援的小号:%s ,仓库号:%s 在频道%d, 先切换频道!", VerItemSupport_.wsPlayerName.c_str(), ItemSupportInfo_.WareHouseInfo_.wsWareHouseName.c_str(), ItemSupportInfo_.WareHouseInfo_.dwChannel);
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
			LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"我是小号:不存在记录, 添加上去! CurName=[%s,%s], vItemSupport_OnLine.size()=%d, ServerId=%X, MaxCount=%d", \
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
			//LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"小号:%s 15s之内没仓库号接单! 超时%dms, 人在Server=%X,Channel=%d", itm.VerItemSupport_.wsPlayerName.c_str(), (DWORD)(::GetTickCount64() - itm.ulKeepALiveTick), itm.VerItemSupport_.dwServerId, itm.VerItemSupport_.dwChannel);
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
			// 每10分钟重新计算交易心跳
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

			LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"我是仓库号[%s,%X,%d]:存在合适的小号:%s! 准备交易!", \
				ItemSupportInfo_.WareHouseInfo_.wsWareHouseName.c_str(), ItemSupportInfo_.WareHouseInfo_.dwServerId, ItemSupportInfo_.WareHouseInfo_.dwChannel, ItemSupportInfo_.VerItemSupport_.wsPlayerName.c_str());
		});

		if (!bExist)
		{
			if (ExistWareHouse(vPickItem_OnLine, WareHouseInfo_) && static_cast<DWORD>(::GetTickCount64() - pWareHouseInfo->ulTick) >= 5 * 60 * 1000)
			{
				// 存在其他小号, 但是上一次交易成功的心跳必须在5分钟之后
				LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"我是仓库号,5分钟都没交易, 但是有其他线路的号! 让仓库号小退!");
				ByteBuffer_ << SOCK_MSG_CLIENT_WAREHOUSE_PICKITEM;
				ByteBuffer_ << (DWORD)em_Deal_Warehouse_IsPick::em_Deal_Warehouse_IsPick_BackToSwitchPlayer;
			}
			else
			{
				ByteBuffer_ << SOCK_MSG_CLIENT_WAREHOUSE_MAINTASK;
				ByteBuffer_ << (DWORD)em_Deal_Warehouse_IsPick::em_Deal_Warehouse_IsPick_None;
			}
			//LOG_F(CLog::em_Log_Type::em_Log_Type_Debug, L"我是仓库号: 不存在合适的小号交易!ServerId=%X, Name=%s", WareHouseInfo_.dwServerId, WareHouseInfo_.wsWareHouseName.c_str());
		}
	});
}

BOOL CBnsDeal::ExistWareHouse(_In_ std::deque<ItemSupportInfo>& vlst, _In_ CONST WareHouseInfo& WareHouseInfo_, _In_ BOOL bUnSameChannel, _In_ std::function<VOID(ItemSupportInfo&)> fn)
{
	for (int i = 0; i < static_cast<int>(vlst.size()); ++i)
	{
		auto& ItemSupportInfo_ = vlst.at(i);
		// 如果小号15s之内都没交易心跳, 就当掉线处理
		if (static_cast<DWORD>(::GetTickCount64() - ItemSupportInfo_.ulKeepALiveTick) >= MAX_DEAL_KEEPALIVE_TIMEOUT || (ItemSupportInfo_.bTake && static_cast<DWORD>(::GetTickCount64() - ItemSupportInfo_.ulTakeTick) >= MAX_DEAL_KEEPALIVE_TIMEOUT))
		{
			// 10s 心跳过后直接当掉线处理!
			//LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"小号:%s 15s之内没仓库号接单了! 超时%dms", ItemSupportInfo_.VerItemSupport_.wsPlayerName.c_str(), (DWORD)(::GetTickCount64() - ItemSupportInfo_.ulKeepALiveTick));
			vlst.erase(vlst.begin() + i--);
			continue;
		}

		// 对比服务器是否一致!
		if (CVerManage::IsCompServer(ItemSupportInfo_.VerItemSupport_.dwServerId, WareHouseInfo_.dwServerId) && !ItemSupportInfo_.bTake/*必须是还没给仓库号接单的*/)
		{
			// 仓库号搜索小号的仓库角色列表, 判断是否存在!
			if (CLPublic::Vec_find_if_Const(ItemSupportInfo_.VerItemSupport_.vWareHouse, [&WareHouseInfo_](CONST std::wstring& wsWareHouseName){return WareHouseInfo_.wsWareHouseName == wsWareHouseName; }) != nullptr)
			{
				// 如果这个一个还没切换过频道的小号, 先发送切换频道的包回去
				ItemSupportInfo_.emDealStatus = em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_Channel;
				ItemSupportInfo_.WareHouseInfo_ = WareHouseInfo_;
				//LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"bSameChannel=%X, 仓库号的频道=%d, 小号的频道=%d", bSameChannel, WareHouseInfo_.dwChannel, ItemSupportInfo_.VerItemSupport_.dwChannel);
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
			// 仓库号搜索小号的仓库角色列表, 判断是否存在!
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
			// 更新数据
			ItemSupportInfo_.VerItemSupport_ = VerItemSupport_;

			ByteBuffer_ << SOCK_MSG_CLIENT_PICKITEM;
			ByteBuffer_ << static_cast<DWORD>(ItemSupportInfo_.emDealStatus);
			switch (ItemSupportInfo_.emDealStatus)
			{
			case em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_None:
				ItemSupportInfo_.ulKeepALiveTick = ::GetTickCount64(); // 设置心跳!
				break;
			case em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_Channel:
				ItemSupportInfo_.ulKeepALiveTick = ::GetTickCount64(); // 设置心跳!
				//LOG_C(CLog::em_Log_Type::em_Log_Type_Debug, L"我是收货小号%s: 仓库号情况:%X, Name=%s", VerItemSupport_.wsPlayerName.c_str(), (DWORD)ItemSupportInfo_.emDealStatus, ItemSupportInfo_.WareHouseInfo_.wsWareHouseName.c_str());
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
			LOG_C(CLog::em_Log_Type::em_Log_Type_Debug, L"我是小号:不存在记录, 添加上去! CurName=%s, vPickItem_OnLine.size()=%d, ServerId=%X, MaxCount=%d", \
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
			// 每10分钟重新计算交易心跳
			pWareHouseInfo->ulTick = ::GetTickCount64();
		}

		BOOL bExist = ExistWareHouse(vPickItem_OnLine, WareHouseInfo_, FALSE, [&ByteBuffer_, &WareHouseInfo_, &pWareHouseInfo](ItemSupportInfo& ItemSupportInfo_)
		{
			ItemSupportInfo_.WareHouseInfo_ = WareHouseInfo_;
			pWareHouseInfo->ulTick = ::GetTickCount64();
			ItemSupportInfo_.bTake = TRUE;
			ItemSupportInfo_.ulTakeTick = ::GetTickCount64();
			LOG_C(CLog::em_Log_Type::em_Log_Type_Debug, L"我是仓库号%s:存在合适的小号:%s! 准备交易!", ItemSupportInfo_.WareHouseInfo_.wsWareHouseName.c_str(), ItemSupportInfo_.VerItemSupport_.wsPlayerName.c_str());
			//ItemSupportInfo_.ulKeepALiveTick = ::GetTickCount64(); // 不能设置心跳, 因为小号可能已经掉线了!
			ByteBuffer_ << SOCK_MSG_CLIENT_WAREHOUSE_PICKITEM;
			ByteBuffer_ << (DWORD)em_Deal_Warehouse_IsPick::em_Deal_Warehouse_IsPick_Pick;
			ByteBuffer_ << ItemSupportInfo_.VerItemSupport_.wsPlayerName;
		});

		if (!bExist)
		{
			if (ExistWareHouse(vPickItem_OnLine, WareHouseInfo_) && static_cast<DWORD>(::GetTickCount64() - pWareHouseInfo->ulTick) >= 5 * 60 * 1000)
			{
				// 存在其他小号, 但是上一次交易成功的心跳必须在5分钟之后
				LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"我是仓库号,5分钟都没交易, 但是有其他线路的号! 让仓库号小退!");
				ByteBuffer_ << SOCK_MSG_CLIENT_WAREHOUSE_PICKITEM;
				ByteBuffer_ << (DWORD)em_Deal_Warehouse_IsPick::em_Deal_Warehouse_IsPick_BackToSwitchPlayer;
			}
			else
			{
				//LOG_C(CLog::em_Log_Type::em_Log_Type_Debug, L"我是仓库号: 不存在合适的小号交易!ServerId=%X, Name=%s, 频道:%d", WareHouseInfo_.dwServerId, WareHouseInfo_.wsWareHouseName.c_str(), WareHouseInfo_.dwChannel);
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
