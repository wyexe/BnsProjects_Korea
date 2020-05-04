#include "BnsTrade.h"
#include <MyTools/Log.h>
#include <MyTools/CLPublic.h>
#include <MyTools/CLEchoException.h>
#include "BnsArea.h"

#define _SELF L"BnsDeal.cpp"

CBnsTrade::CBnsTrade()
{

}

BOOL CBnsTrade::JoinItemSupportDeal(_In_ CONST VerItemSupport& VerItemSupport_, _Out_ ItemSupportResult& Result)
{
	std::lock_guard<std::mutex> _Lock(_MtxItemSupport);

	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		INT uIndex = 0;
		if (ExistItemSupport(vItemSupport_OnLine, VerItemSupport_, uIndex))
		{
			auto& ItemSupportInfo_ = vItemSupport_OnLine.at(uIndex);
			ItemSupportInfo_.VerItemSupport_ = VerItemSupport_;

			// 
			Result.emDealStatus = ItemSupportInfo_.emDealStatus;

			// 
			switch (ItemSupportInfo_.emDealStatus)
			{
			case em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_None:
				//LOG_F(CLog::em_Log_Type::em_Log_Type_Debug, L"请求物资小号:[%s,%s] ServerId=%X, channel=%d 的心跳", VerItemSupport_.wsAccountName.c_str(), VerItemSupport_.wsPlayerName.c_str(), VerItemSupport_.dwServerId, VerItemSupport_.dwChannel);
				ItemSupportInfo_.ulKeepALiveTick = ::GetTickCount64();
				break;
			case em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_Channel:
				//LOG_CF_D( L"我是请求物资支援的小号:%s ,仓库号:%s 在频道%d, 先切换频道!", VerItemSupport_.wsPlayerName.c_str(), ItemSupportInfo_.WareHouseInfo_.wsWareHouseName.c_str(), ItemSupportInfo_.WareHouseInfo_.dwChannel);
				Result.wsWareHouseName = ItemSupportInfo_.WareHouseInfo_.wsWareHouseName;
				Result.dwChannel = ItemSupportInfo_.WareHouseInfo_.dwChannel;
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
			vItemSupport_OnLine.push_back(std::move(ItemSupportInfo_));

			LOG_CF_D(L"我是小号:不存在记录, 添加上去! CurName=[%s,%s], vItemSupport_OnLine.size()=%d, ServerId=%X, MaxCount=%d",
				VerItemSupport_.wsAccountName.c_str(),
				VerItemSupport_.wsPlayerName.c_str(),
				vItemSupport_OnLine.size(),
				VerItemSupport_.dwServerId,
				vItemSupport_OnLine.size()
			);

			Result.emDealStatus = em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_None;
		}

		return TRUE;
	});
}

BOOL CBnsTrade::ExistItemSupport(_In_ std::deque<ItemSupportInfo>& vlst, _In_ CONST VerItemSupport& VerItemSupport_, _Out_opt_ INT& uIndex)
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&] 
	{
		for (uIndex = 0; uIndex < static_cast<int>(vlst.size()); ++uIndex)
		{
			CONST auto& itm = vlst.at(uIndex);
			if (static_cast<DWORD>(::GetTickCount64() - itm.ulKeepALiveTick) >= MAX_DEAL_KEEPALIVE_TIMEOUT || (itm.bTake && static_cast<DWORD>(::GetTickCount64() - itm.ulTakeTick) >= MAX_DEAL_KEEPALIVE_TIMEOUT))
			{
				//LOG_CF_D( L"小号:%s 15s之内没仓库号接单! 超时%dms, 人在Server=%X,Channel=%d", itm.VerItemSupport_.wsPlayerName.c_str(), (DWORD)(::GetTickCount64() - itm.ulKeepALiveTick), itm.VerItemSupport_.dwServerId, itm.VerItemSupport_.dwChannel);
				vlst.erase(vlst.begin() + uIndex--);
				continue;
			}
			if (itm.VerItemSupport_.wsPlayerName == VerItemSupport_.wsPlayerName && CBnsArea::GetInstance().IsSame_Area(itm.VerItemSupport_.dwServerId, VerItemSupport_.dwServerId))
				return TRUE;
		}
		return FALSE;
	});
}

BOOL CBnsTrade::AcceptItemSupportDeal(_In_ CONST WareHouseInfo& WareHouseInfo_, _Out_ WarehouseActionResult& Result)
{
	std::lock_guard<std::mutex> _Lock(_MtxItemSupport);

	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&] 
	{
		FindWarehouse(WareHouseInfo_, [&](WareHouseInfo& _Warehouse)
		{
			if (static_cast<DWORD>(::GetTickCount64() - _Warehouse.ulTick >= 10 * 60 * 1000))
			{
				// 每10分钟重新计算交易心跳
				_Warehouse.ulTick = ::GetTickCount64();
			}

			BOOL bExist = ExistWareHouse(vItemSupport_OnLine, _Warehouse, TRUE, [&](ItemSupportInfo& ItemSupportInfo_)
			{
				_Warehouse.ulTick = ::GetTickCount64();
				ItemSupportInfo_.WareHouseInfo_ = _Warehouse;
				ItemSupportInfo_.bTake = TRUE;
				ItemSupportInfo_.ulTakeTick = ::GetTickCount64();

				Result.emWarehouseType = em_Trade_Warehouse_Type::em_Trade_Warehouse_Type_PickItem;
				Result.emIsPick = em_Deal_Warehouse_IsPick::em_Deal_Warehouse_IsPick_Pick;
				Result.wsPlayerName = ItemSupportInfo_.VerItemSupport_.wsPlayerName;

				LOG_CF_D(L"我是仓库号[%s,%X,%d]:存在合适的小号:%s! 准备交易! vItemSupport_OnLine=%d",
					ItemSupportInfo_.WareHouseInfo_.wsWareHouseName.c_str(),
					ItemSupportInfo_.WareHouseInfo_.dwServerId,
					ItemSupportInfo_.WareHouseInfo_.dwChannel,
					ItemSupportInfo_.VerItemSupport_.wsPlayerName.c_str(),
					vItemSupport_OnLine.size());
			});

			if (!bExist)
			{
				if (ExistWareHouse(vPickItem_OnLine, _Warehouse) && static_cast<DWORD>(::GetTickCount64() - _Warehouse.ulTick) >= 5 * 60 * 1000)
				{
					// 存在其他小号, 但是上一次交易成功的心跳必须在5分钟之后
					LOG_CF_D(L"我是仓库号,5分钟都没交易, 但是有其他线路的号! 让仓库号小退!");
					Result.emWarehouseType = em_Trade_Warehouse_Type_ItemSupport;
					Result.emIsPick = em_Deal_Warehouse_IsPick::em_Deal_Warehouse_IsPick_BackToSwitchPlayer;
				}
				else
				{
					Result.emWarehouseType = em_Trade_Warehouse_Type::em_Trade_Warehouse_Type_PickItem;
					Result.emIsPick = em_Deal_Warehouse_IsPick::em_Deal_Warehouse_IsPick_None;
				}
				//LOG_F(CLog::em_Log_Type::em_Log_Type_Debug, L"我是仓库号: 不存在合适的小号交易!ServerId=%X, Name=%s", WareHouseInfo_.dwServerId, WareHouseInfo_.wsWareHouseName.c_str());
			}
		});

		return TRUE;
	});
}

BOOL CBnsTrade::ExistWareHouse(_In_ std::deque<ItemSupportInfo>& vlst, _In_ CONST WareHouseInfo& WareHouseInfo_, _In_ BOOL bUnSameChannel, _In_ std::function<VOID(ItemSupportInfo&)> fn)
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&] 
	{
		for (int i = 0; i < static_cast<int>(vlst.size()); ++i)
		{
			auto& ItemSupportInfo_ = vlst.at(i);
			// 如果小号15s之内都没交易心跳, 就当掉线处理
			if (static_cast<DWORD>(::GetTickCount64() - ItemSupportInfo_.ulKeepALiveTick) >= MAX_DEAL_KEEPALIVE_TIMEOUT || (ItemSupportInfo_.bTake && static_cast<DWORD>(::GetTickCount64() - ItemSupportInfo_.ulTakeTick) >= MAX_DEAL_KEEPALIVE_TIMEOUT))
			{
				// 10s 心跳过后直接当掉线处理!
				//LOG_CF_D( L"小号:%s 15s之内没仓库号接单了! 超时%dms", ItemSupportInfo_.VerItemSupport_.wsPlayerName.c_str(), (DWORD)(::GetTickCount64() - ItemSupportInfo_.ulKeepALiveTick));
				vlst.erase(vlst.begin() + i--);
				continue;
			}

			// 对比服务器是否一致!
			if (CBnsArea::GetInstance().IsSame_Area(ItemSupportInfo_.VerItemSupport_.dwServerId, WareHouseInfo_.dwServerId) && !ItemSupportInfo_.bTake/*必须是还没给仓库号接单的*/)
			{
				// 仓库号搜索小号的仓库角色列表, 判断是否存在!
				if (MyTools::CLPublic::Vec_find_if_Const(ItemSupportInfo_.VerItemSupport_.vWareHouse, static_cast<std::wstring*>(nullptr), [&WareHouseInfo_](CONST std::wstring& wsWareHouseName) {return WareHouseInfo_.wsWareHouseName == wsWareHouseName; }))
				{
					// 如果这个一个还没切换过频道的小号, 先发送切换频道的包回去
					ItemSupportInfo_.emDealStatus = em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_Channel;
					ItemSupportInfo_.WareHouseInfo_ = WareHouseInfo_;
					//LOG_CF_D( L"bSameChannel=%X, 仓库号的频道=%d, 小号的频道=%d", bSameChannel, WareHouseInfo_.dwChannel, ItemSupportInfo_.VerItemSupport_.dwChannel);
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
	});
}

BOOL CBnsTrade::ExistWareHouse(_In_ std::deque<ItemSupportInfo>& vlst, _In_ CONST WareHouseInfo& WareHouseInfo_)
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&] 
	{
		for (CONST auto& itm : vlst)
		{
			if (CBnsArea::GetInstance().IsSame_Area(itm.VerItemSupport_.dwServerId, WareHouseInfo_.dwServerId))
			{
				// 仓库号搜索小号的仓库角色列表, 判断是否存在!
				if (MyTools::CLPublic::Vec_find_if_Const(itm.VerItemSupport_.vWareHouse, static_cast<std::wstring*>(nullptr), [&WareHouseInfo_](CONST std::wstring& wsWareHouseName) {return WareHouseInfo_.wsWareHouseName == wsWareHouseName; }))
					return TRUE;
			}
		}
		return FALSE;
	});
}

BOOL CBnsTrade::JoinPickItemtDeal(_In_ CONST VerItemSupport& VerItemSupport_, _Out_ ItemSupportResult& Result)
{
	std::lock_guard<std::mutex> _Lock(_MtxPickItem);

	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&] 
	{
		INT uIndex = 0;
		if (ExistItemSupport(vPickItem_OnLine, VerItemSupport_, uIndex))
		{
			auto& ItemSupportInfo_ = vPickItem_OnLine.at(uIndex);
			// 更新数据
			ItemSupportInfo_.VerItemSupport_ = VerItemSupport_;

			Result.emDealStatus = ItemSupportInfo_.emDealStatus;
			switch (ItemSupportInfo_.emDealStatus)
			{
			case em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_None:
				ItemSupportInfo_.ulKeepALiveTick = ::GetTickCount64(); // 设置心跳!
				break;
			case em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_Channel:
				ItemSupportInfo_.ulKeepALiveTick = ::GetTickCount64(); // 设置心跳!
																	   //LOG_C(CLog::em_Log_Type::em_Log_Type_Debug, L"我是收货小号%s: 仓库号情况:%X, Name=%s", VerItemSupport_.wsPlayerName.c_str(), (DWORD)ItemSupportInfo_.emDealStatus, ItemSupportInfo_.WareHouseInfo_.wsWareHouseName.c_str());
				Result.wsWareHouseName = ItemSupportInfo_.WareHouseInfo_.wsWareHouseName;
				Result.dwChannel = ItemSupportInfo_.WareHouseInfo_.dwChannel;
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
			LOG_C_D(L"我是小号:不存在记录, 添加上去! vPickItem_OnLine=%d,CurName=%s, ServerId=%X",
				vPickItem_OnLine.size(),
				VerItemSupport_.wsPlayerName.c_str(),
				VerItemSupport_.dwServerId);


			Result.emDealStatus = em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_None;
		}
		return TRUE;
	});
}

BOOL CBnsTrade::AcceptPickItemDeal(_In_ CONST WareHouseInfo& WareHouseInfo_, _Out_ WarehouseActionResult& Result)
{
	std::lock_guard<std::mutex> _Lock(_MtxPickItem);
	Result.emWarehouseType = em_Trade_Warehouse_Type::em_Trade_Warehouse_Type_PickItem;
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&] 
	{
		FindWarehouse(WareHouseInfo_, [&](WareHouseInfo& _WareHouse)
		{
			if (static_cast<DWORD>(::GetTickCount64() - _WareHouse.ulTick) >= 10 * 60 * 1000)
			{
				// 每10分钟重新计算交易心跳
				_WareHouse.ulTick = ::GetTickCount64();
			}

			BOOL bExist = ExistWareHouse(vPickItem_OnLine, _WareHouse, FALSE, [&](ItemSupportInfo& ItemSupportInfo_)
			{
				ItemSupportInfo_.WareHouseInfo_ = _WareHouse;
				_WareHouse.ulTick = ::GetTickCount64();
				ItemSupportInfo_.bTake = TRUE;
				ItemSupportInfo_.ulTakeTick = ::GetTickCount64();
				LOG_C_D(L"我是仓库号%s:存在合适的小号:%s! 准备交易! vPickItem_OnLine.size=%d",
					ItemSupportInfo_.WareHouseInfo_.wsWareHouseName.c_str(),
					ItemSupportInfo_.VerItemSupport_.wsPlayerName.c_str(),
					vPickItem_OnLine.size());

				//ItemSupportInfo_.ulKeepALiveTick = ::GetTickCount64(); // 不能设置心跳, 因为小号可能已经掉线了!

				Result.emIsPick = em_Deal_Warehouse_IsPick::em_Deal_Warehouse_IsPick_Pick;
				Result.wsPlayerName = ItemSupportInfo_.VerItemSupport_.wsPlayerName;
			});

			if (!bExist)
			{
				if (ExistWareHouse(vPickItem_OnLine, _WareHouse) && static_cast<DWORD>(::GetTickCount64() - _WareHouse.ulTick) >= 5 * 60 * 1000)
				{
					// 存在其他小号, 但是上一次交易成功的心跳必须在5分钟之后
					LOG_CF_D(L"我是仓库号,5分钟都没交易, 但是有其他线路的号! 让仓库号小退!");
					Result.emIsPick = em_Deal_Warehouse_IsPick::em_Deal_Warehouse_IsPick_BackToSwitchPlayer;
				}
				else
				{
					//LOG_C(CLog::em_Log_Type::em_Log_Type_Debug, L"我是仓库号: 不存在合适的小号交易!ServerId=%X, Name=%s, 频道:%d", WareHouseInfo_.dwServerId, WareHouseInfo_.wsWareHouseName.c_str(), WareHouseInfo_.dwChannel);
					Result.emIsPick = em_Deal_Warehouse_IsPick::em_Deal_Warehouse_IsPick_None;
				}
			}

		});

		return TRUE;
	});
}

VOID CBnsTrade::RemoveAccount(_In_ CONST std::wstring& wsAccountName, _In_ CONST std::wstring& wsPlayerName)
{
	MyTools::CLEchoException::GetInstance().InvokeAction(__FUNCTIONW__, [=]
	{
		RemoveAccountInItemSupport(wsAccountName);
		RemoveAccountInPickItem(wsAccountName);
		RemoveAccountInWarehouse(wsPlayerName);
	});
}

VOID CBnsTrade::FindWarehouse(_In_ CONST WareHouseInfo& tmpWareHouseInfo, _In_ std::function<VOID(WareHouseInfo&)> ActionPtr)
{
	std::lock_guard<std::mutex> _Lock(_MtxWarehouse);
	MyTools::CLEchoException::GetInstance().InvokeAction(__FUNCTIONW__, [=] 
	{
		auto itr = MapWareHouse.find(tmpWareHouseInfo.wsWareHouseName);
		if (itr == MapWareHouse.end())
		{
			WareHouseInfo _Warehouse = tmpWareHouseInfo;
			ActionPtr(_Warehouse);
			MapWareHouse.insert(std::make_pair(_Warehouse.wsWareHouseName, _Warehouse));
		}
		else
		{
			itr->second = tmpWareHouseInfo;
			ActionPtr(itr->second);
		}
	});
}

VOID CBnsTrade::RemoveAccountInItemSupport(_In_ CONST std::wstring& wsAccountName)
{
	std::lock_guard<std::mutex> _Lock(_MtxItemSupport);
	MyTools::CLPublic::Dque_erase_if(vItemSupport_OnLine, [wsAccountName](CONST ItemSupportInfo& info)
	{
		return info.VerItemSupport_.wsAccountName == wsAccountName;
	});
}

VOID CBnsTrade::RemoveAccountInPickItem(_In_ CONST std::wstring& wsAccountName)
{
	std::lock_guard<std::mutex> _Lock(_MtxPickItem);
	MyTools::CLPublic::Dque_erase_if(vPickItem_OnLine, [wsAccountName](CONST ItemSupportInfo& info)
	{
		return info.VerItemSupport_.wsAccountName == wsAccountName;
	});
}

VOID CBnsTrade::RemoveAccountInWarehouse(_In_ CONST std::wstring& wsPlayerName)
{
	std::lock_guard<std::mutex> _Lock(_MtxWarehouse);
	auto itr = MapWareHouse.find(wsPlayerName);;
	if (itr != MapWareHouse.end())
		MapWareHouse.erase(itr);
}
