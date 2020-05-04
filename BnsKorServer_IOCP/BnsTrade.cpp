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
				//LOG_F(CLog::em_Log_Type::em_Log_Type_Debug, L"��������С��:[%s,%s] ServerId=%X, channel=%d ������", VerItemSupport_.wsAccountName.c_str(), VerItemSupport_.wsPlayerName.c_str(), VerItemSupport_.dwServerId, VerItemSupport_.dwChannel);
				ItemSupportInfo_.ulKeepALiveTick = ::GetTickCount64();
				break;
			case em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_Channel:
				//LOG_CF_D( L"������������֧Ԯ��С��:%s ,�ֿ��:%s ��Ƶ��%d, ���л�Ƶ��!", VerItemSupport_.wsPlayerName.c_str(), ItemSupportInfo_.WareHouseInfo_.wsWareHouseName.c_str(), ItemSupportInfo_.WareHouseInfo_.dwChannel);
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

			LOG_CF_D(L"����С��:�����ڼ�¼, �����ȥ! CurName=[%s,%s], vItemSupport_OnLine.size()=%d, ServerId=%X, MaxCount=%d",
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
				//LOG_CF_D( L"С��:%s 15s֮��û�ֿ�Žӵ�! ��ʱ%dms, ����Server=%X,Channel=%d", itm.VerItemSupport_.wsPlayerName.c_str(), (DWORD)(::GetTickCount64() - itm.ulKeepALiveTick), itm.VerItemSupport_.dwServerId, itm.VerItemSupport_.dwChannel);
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
				// ÿ10�������¼��㽻������
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

				LOG_CF_D(L"���ǲֿ��[%s,%X,%d]:���ں��ʵ�С��:%s! ׼������! vItemSupport_OnLine=%d",
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
					// ��������С��, ������һ�ν��׳ɹ�������������5����֮��
					LOG_CF_D(L"���ǲֿ��,5���Ӷ�û����, ������������·�ĺ�! �òֿ��С��!");
					Result.emWarehouseType = em_Trade_Warehouse_Type_ItemSupport;
					Result.emIsPick = em_Deal_Warehouse_IsPick::em_Deal_Warehouse_IsPick_BackToSwitchPlayer;
				}
				else
				{
					Result.emWarehouseType = em_Trade_Warehouse_Type::em_Trade_Warehouse_Type_PickItem;
					Result.emIsPick = em_Deal_Warehouse_IsPick::em_Deal_Warehouse_IsPick_None;
				}
				//LOG_F(CLog::em_Log_Type::em_Log_Type_Debug, L"���ǲֿ��: �����ں��ʵ�С�Ž���!ServerId=%X, Name=%s", WareHouseInfo_.dwServerId, WareHouseInfo_.wsWareHouseName.c_str());
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
			// ���С��15s֮�ڶ�û��������, �͵����ߴ���
			if (static_cast<DWORD>(::GetTickCount64() - ItemSupportInfo_.ulKeepALiveTick) >= MAX_DEAL_KEEPALIVE_TIMEOUT || (ItemSupportInfo_.bTake && static_cast<DWORD>(::GetTickCount64() - ItemSupportInfo_.ulTakeTick) >= MAX_DEAL_KEEPALIVE_TIMEOUT))
			{
				// 10s ��������ֱ�ӵ����ߴ���!
				//LOG_CF_D( L"С��:%s 15s֮��û�ֿ�Žӵ���! ��ʱ%dms", ItemSupportInfo_.VerItemSupport_.wsPlayerName.c_str(), (DWORD)(::GetTickCount64() - ItemSupportInfo_.ulKeepALiveTick));
				vlst.erase(vlst.begin() + i--);
				continue;
			}

			// �Աȷ������Ƿ�һ��!
			if (CBnsArea::GetInstance().IsSame_Area(ItemSupportInfo_.VerItemSupport_.dwServerId, WareHouseInfo_.dwServerId) && !ItemSupportInfo_.bTake/*�����ǻ�û���ֿ�Žӵ���*/)
			{
				// �ֿ������С�ŵĲֿ��ɫ�б�, �ж��Ƿ����!
				if (MyTools::CLPublic::Vec_find_if_Const(ItemSupportInfo_.VerItemSupport_.vWareHouse, static_cast<std::wstring*>(nullptr), [&WareHouseInfo_](CONST std::wstring& wsWareHouseName) {return WareHouseInfo_.wsWareHouseName == wsWareHouseName; }))
				{
					// ������һ����û�л���Ƶ����С��, �ȷ����л�Ƶ���İ���ȥ
					ItemSupportInfo_.emDealStatus = em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_Channel;
					ItemSupportInfo_.WareHouseInfo_ = WareHouseInfo_;
					//LOG_CF_D( L"bSameChannel=%X, �ֿ�ŵ�Ƶ��=%d, С�ŵ�Ƶ��=%d", bSameChannel, WareHouseInfo_.dwChannel, ItemSupportInfo_.VerItemSupport_.dwChannel);
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
				// �ֿ������С�ŵĲֿ��ɫ�б�, �ж��Ƿ����!
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
			// ��������
			ItemSupportInfo_.VerItemSupport_ = VerItemSupport_;

			Result.emDealStatus = ItemSupportInfo_.emDealStatus;
			switch (ItemSupportInfo_.emDealStatus)
			{
			case em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_None:
				ItemSupportInfo_.ulKeepALiveTick = ::GetTickCount64(); // ��������!
				break;
			case em_Deal_Warehouse_Status::em_Deal_Warehouse_Status_Channel:
				ItemSupportInfo_.ulKeepALiveTick = ::GetTickCount64(); // ��������!
																	   //LOG_C(CLog::em_Log_Type::em_Log_Type_Debug, L"�����ջ�С��%s: �ֿ�����:%X, Name=%s", VerItemSupport_.wsPlayerName.c_str(), (DWORD)ItemSupportInfo_.emDealStatus, ItemSupportInfo_.WareHouseInfo_.wsWareHouseName.c_str());
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
			LOG_C_D(L"����С��:�����ڼ�¼, �����ȥ! vPickItem_OnLine=%d,CurName=%s, ServerId=%X",
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
				// ÿ10�������¼��㽻������
				_WareHouse.ulTick = ::GetTickCount64();
			}

			BOOL bExist = ExistWareHouse(vPickItem_OnLine, _WareHouse, FALSE, [&](ItemSupportInfo& ItemSupportInfo_)
			{
				ItemSupportInfo_.WareHouseInfo_ = _WareHouse;
				_WareHouse.ulTick = ::GetTickCount64();
				ItemSupportInfo_.bTake = TRUE;
				ItemSupportInfo_.ulTakeTick = ::GetTickCount64();
				LOG_C_D(L"���ǲֿ��%s:���ں��ʵ�С��:%s! ׼������! vPickItem_OnLine.size=%d",
					ItemSupportInfo_.WareHouseInfo_.wsWareHouseName.c_str(),
					ItemSupportInfo_.VerItemSupport_.wsPlayerName.c_str(),
					vPickItem_OnLine.size());

				//ItemSupportInfo_.ulKeepALiveTick = ::GetTickCount64(); // ������������, ��ΪС�ſ����Ѿ�������!

				Result.emIsPick = em_Deal_Warehouse_IsPick::em_Deal_Warehouse_IsPick_Pick;
				Result.wsPlayerName = ItemSupportInfo_.VerItemSupport_.wsPlayerName;
			});

			if (!bExist)
			{
				if (ExistWareHouse(vPickItem_OnLine, _WareHouse) && static_cast<DWORD>(::GetTickCount64() - _WareHouse.ulTick) >= 5 * 60 * 1000)
				{
					// ��������С��, ������һ�ν��׳ɹ�������������5����֮��
					LOG_CF_D(L"���ǲֿ��,5���Ӷ�û����, ������������·�ĺ�! �òֿ��С��!");
					Result.emIsPick = em_Deal_Warehouse_IsPick::em_Deal_Warehouse_IsPick_BackToSwitchPlayer;
				}
				else
				{
					//LOG_C(CLog::em_Log_Type::em_Log_Type_Debug, L"���ǲֿ��: �����ں��ʵ�С�Ž���!ServerId=%X, Name=%s, Ƶ��:%d", WareHouseInfo_.dwServerId, WareHouseInfo_.wsWareHouseName.c_str(), WareHouseInfo_.dwChannel);
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
