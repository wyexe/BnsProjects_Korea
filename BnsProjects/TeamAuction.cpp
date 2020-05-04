#include "stdafx.h"
#include "TeamAuction.h"
#include <MyTools/Log.h>
#include "ResAction.h"
#include "BagBase.h"
#include "BagFunction.h"
#include "ObjectFunction.h"

#define _SELF L"TeamAuction.cpp"

VOID CTeamAuction::SetAuction_By_Gungoen_HeiLongJiao() CONST
{
	CONST static std::vector<CBagFunction::Equi_Type> Vec = 
	{
		{ L"자칸령", CBag::em_Equi_Type::em_Equi_Type_Soul },
		{ L"자칸귀걸이", CBag::em_Equi_Type::em_Equi_Type_Earring },
	};

	CResAction::GetInstance().SetPartyAuctionPtr([]
	{
		AuctionContent AuctionContent_;
		if (!CObjectFunction::GetInstance().GetTeamAuctionContent(AuctionContent_))
		{
			LOG_CF_E(L"竞拍的时候无法获取竞拍物品内容!!!");
			return FALSE;
		}

		
		CONST auto& CurrentAuctionitem = AuctionContent_.GetCurrentAuctionItem();
		if (CurrentAuctionitem.wsItemName == Item_黑龙教封印的八卦牌) // 八卦牌
		{
			// 判断身上的八卦牌是否凑够一套了
			CBag Equi;
			if (!CBagFunction::GetInstance().GetEqui_By_Type(CBag::em_Equi_Type::em_Equi_Type_Diagrams1, &Equi))
				return FALSE;
			if (Equi.GetName() == Item_黑龙教八卦牌)
				return FALSE;

			// 获取此物品的Type
			DWORD dwBaGuaType = CBag::GetItemType_By_GId(CurrentAuctionitem.dwItemId);
			if (dwBaGuaType == NULL)
			{
				LOG_CF_E(L"八卦[%s] 的Type=%X", CurrentAuctionitem.wsItemName.c_str(), dwBaGuaType);
				return FALSE;
			}

			// 获取整个背包的被封印的八卦!
			std::vector<CBag> VecItem;
			CBagFunction::GetInstance().GetItemList_By_Name(Item_黑龙教封印的八卦牌, VecItem);

			auto p = MyTools::CLPublic::Vec_find_if(VecItem, [dwBaGuaType](CONST CBag& itm){  return itm.GetItemType() == dwBaGuaType; });
			if (p != nullptr)
			{
				LOG_CF_D(L"我是[%s], 这件封印的八卦[%X]在背包里面已经有了, 不要了!", dwBaGuaType);
				return FALSE;
			}

			LOG_CF_D(L"我是[%s], 这件封印的八卦类型[%X] 没有, 参加竞拍!", dwBaGuaType);
			return TRUE;
		}
		else if (CurrentAuctionitem.wsItemName == L"") // 金币那个
		{
			LOG_CF_D(L"发现出钱那个玩意了!!!");
			return FALSE;
		}

		auto p = MyTools::CLPublic::Vec_find_if_Const(Vec, [CurrentAuctionitem](CONST CBagFunction::Equi_Type& EquiType){ return EquiType.wsEquiName == CurrentAuctionitem.wsItemName; });
		if (p != nullptr)
		{
			// 判断身上或者背包有没有这个玩意
			if (CBagFunction::GetInstance().ExistItem_By_Name(p->wsEquiName, nullptr) && CBagFunction::GetInstance().GetEqui_By_Type(p->emEquiType, nullptr))
				return FALSE;

			return TRUE;
		}

		return TRUE;
	});
}

VOID CTeamAuction::SetAuction_Null() CONST
{
	CResAction::GetInstance().SetPartyAuctionPtr(nullptr);
}

