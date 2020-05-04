#include "stdafx.h"
#include "ResAction.h"
#include <thread>
#include <MyTools/Log.h>
#include <MyTools/CLEchoException.h>
#include "BnsHook.h"
#include "SomeFun.h"
#include "PersonFunction.h"
#include "BagFunction.h"
#include "DropItem.h"
#include "ResExtend.h"
#include "Person.h"
#include "GameUI.h"
#include "ObjectFunction.h"
#include "GameSend.h"
#include "TeamMemberExtend.h"
#include "Npc.h"
#include "NpcExtend.h"
#include "BnsFilter.h"

#define _SELF L"ResAction.cpp"

BOOL CResAction::TakeDropItem(_In_ CONST FixPointInfo& FixPtInfo,_In_ float Range) CONST
{
	auto fnCheckTeamAuction = [this]
	{
		while (GameRun && CObjectFunction::GetInstance().IsShow_In_GameUi(UI_TEXT_PARTYAUCTION))
		{
			if (fnPartyAuctionPtr == nullptr)
			{
				if (CTeamMemberExtend::GetInstance().IsTeamCaptain(CPerson::GetInstance().GetName()))
					CSomeFun::GetInstance().SimlationKey('Y');
				else
					CSomeFun::GetInstance().SimlationKey('N');
				continue;
			}

			auto Index = CObjectFunction::GetInstance().GetIndex_By_Auction();
			CSomeFun::GetInstance().SimlationKey(fnPartyAuctionPtr() ? 'Y' : 'N');
			BnsSleep(2 * 1000);

			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(10 * 1000, [Index]
			{
				BnsSleep(2 * 1000);
				if (Index == CObjectFunction::GetInstance().GetIndex_By_Auction())
				{
					CSomeFun::GetInstance().SimlationKey('N');
					return TRUE;
				}
				return FALSE;
			});
		}
	};

	DWORD dwLastTakeId = 0;
	DWORD dwTakeItemCount = 0;

	CDropItem DropItem;
	CFilter::GetInstance().ClearTimeOutDropItemBlackList();
	while (GameRun && !EMPTY_PERSONHP && !CBagFunction::GetInstance().IsBagFull(CObjectFunction::em_Bag_Type_Bag, 3) && CResExtend::GetInstance().GetCurrentDropItem(FixPtInfo, DropItem))
	{
		LOG_CF_D(L"捡物,ID=%X", DropItem.GetId());
		auto ItmObj = DropItem.GetObjectId();
		DropItem.TakeItem();

		// 如果这次捡物的ID跟上一次的一毛一样, 就把次数 + 1, 否则置0
		dwTakeItemCount = (dwLastTakeId == ItmObj.dwId) ? dwTakeItemCount + 1 : 0;
		dwLastTakeId = ItmObj.dwId;

		// 等待pickupItemPanel出现
		BnsSleep(200);
		CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&ItmObj]{ CGameSend::SecondPickup(ItmObj.dwId, ItmObj.dwIdParm); });
		//CSomeFun::GetInstance().SimlationKey('F');
		BnsSleep(300);

		if (CObjectFunction::GetInstance().IsShow_In_GameUi(L"PickupItemPanel"))
		{
			LOG_CF_D( L"出现捡物UI,按Y");
			CSomeFun::GetInstance().SimlationKey('F');
			BnsSleep(200);
			CSomeFun::GetInstance().SimlationKey('Y');
			BnsSleep(300);
		}
		
		if (dwTakeItemCount >= 3)
		{
			LOG_CF_D(L"捡物:%X, 超过3次还失败, 列入黑名单!", ItmObj.dwId);
			CFilter::GetInstance().AddDropItemToBlackList(ItmObj.dwId);
			continue;
		}

		fnCheckTeamAuction();
	}

	std::thread t([fnCheckTeamAuction]
	{
		MyTools::CLEchoException::GetInstance().InvokeAction(__FUNCTIONW__, [fnCheckTeamAuction]
		{
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(5 * 1000, [fnCheckTeamAuction]
			{
				CPersonFunction::GetInstance().WaitGameLoad(FALSE, FALSE);
				fnCheckTeamAuction();
				BnsSleep(500);
				return TRUE;
			});
		});
		
	});
	t.detach();

	return BnsMove(FixPtInfo.FixPoint, 2.0f);
}

VOID CResAction::SetPartyAuctionPtr(_In_ std::function<BOOL(VOID)> fnPtr)
{
	fnPartyAuctionPtr = fnPtr;
}
