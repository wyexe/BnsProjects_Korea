#include "stdafx.h"
#include "BagAction.h"
#include <MyTools/Log.h>
#include <MyTools/TimeTick.h>
#include "ObjectFunction.h"
#include "BnsFilter.h"
#include "SomeFun.h"
#include "NpcExtend.h"
#include "PersonFunction.h"
#include "BagFunction.h"
#include "Person.h"
#include "Npc.h"
#include "GameSend.h"

#define _SELF L"BagAction.cpp"

BOOL CBagAction::ExplainItem() CONST
{
	std::vector<CBag> vlst;
	CObjectFunction::GetInstance().GetBagList(vlst, CObjectFunction::em_Bag_Type_Bag);

	// 根据物品属性 判断该物品是否分解
	auto fnIsExplain_By_Item = [](_In_ CONST CBag& Bag)
	{
		DWORD dwItemType = Bag.GetItemType();
		if (dwItemType == 0) // 非装备物品不能分解 
			return FALSE;

		// 绿色和白色的可以分解 
		CBag::em_ItemColor emColor = Bag.GetColor();
		if (emColor == CBag::em_ItemColor_Gray || emColor == CBag::em_ItemColor_White || emColor == CBag::em_ItemColor_Green)
		{
			LOG_CF_D(L"分解绿色以下的装备:[%s]", Bag.GetName().c_str());
			return TRUE;
		}

		// 封印的物品也分解了 ?
		//if (Bag.GetName() == L"封印的" && (emColor == CBag::em_ItemColor_Gray || emColor == CBag::em_ItemColor_White || emColor == CBag::em_ItemColor_Green))
		//	return TRUE;

		return FALSE;
	};

	for (CONST auto& itm : vlst)
	{
		auto FilterType = CFilter::GetInstance().ExistItemFilter(itm.GetName());
		if (FilterType != CFilter::ItemFilterType::Filter_Item_Default)
		{
			if (FilterType == CFilter::ItemFilterType::Filter_Item_Explain)
			{
				LOG_CF_D(L"分解物品过滤的指定物品:[%s]", itm.GetName().c_str());
				itm.Explain();
				BnsSleep(500);
			}
		}
		else if (fnIsExplain_By_Item(itm))
		{
			itm.Explain();
			BnsSleep(500);
			continue;
		}
	}

	return TRUE;
}

BOOL CBagAction::DiscardItem() CONST
{
	std::vector<CBag> vlst;
	CObjectFunction::GetInstance().GetBagList(vlst, CObjectFunction::em_Bag_Type_Bag);


	for (CONST auto& itm : vlst)
	{
		if (CFilter::GetInstance().ExistItemFilter(itm.GetName()) == CFilter::ItemFilterType::Filter_Item_Drop)
		{
			LOG_CF_D(L"丢弃物品过滤的指定物品:[%s]", itm.GetName().c_str());
			itm.DiscardItem();
			BnsSleep(500);
		}
	}
	return TRUE;
}

BOOL CBagAction::DiscardItem_By_Type(_In_ CONST std::vector<CBag::em_Equi_Type>& VecType) CONST
{
	std::vector<CBag> VecItem;
	CObjectFunction::GetInstance().GetBagList(VecItem, CObjectFunction::em_Bag_Type::em_Bag_Type_Bag);

	for (CONST auto& itm : VecItem)
	{
		DWORD dwType = itm.GetItemType();
		if (dwType == NULL)
			continue;

		if (MyTools::CLPublic::Vec_find_if_Const(VecType, [dwType](CBag::em_Equi_Type emType){ return static_cast<DWORD>(emType) == dwType; }) == nullptr)
			continue;

		if (CFilter::GetInstance().ExistItemFilter(itm.GetName()) == CFilter::ItemFilterType::Filter_Item_Keep)
			continue;

		itm.Explain();
		BnsSleep(500);
	}
	return TRUE;
}

BOOL CBagAction::SupplementHp() CONST
{
	CBag itm;
	if (CBagFunction::GetInstance().ExistItem_By_Name(恢复药, &itm))
	{
		LOG_CF_D( L"使用物品:%s", itm.GetName().c_str());
		itm.UseItem();
	}
	return TRUE;
}

BOOL CBagAction::EquiGrowth(_In_ CBag::em_Equi_Type emType, _In_ CONST std::wstring& wsSacrificeName) CONST
{
	switch (emType)
	{
		// 武器
	case CBag::em_Equi_Type_Weapon:
		//if (FALSE)
		//{
		//	LOG_CF_D( L"武器已经不能继续强化了~!");
		//	return FALSE;
		//}
		break;

		// 首饰
	case CBag::em_Equi_Type_Necklace: case CBag::em_Equi_Type_Earring: case CBag::em_Equi_Type_Ring: case CBag::em_Equi_Type_Bracelet: case CBag::em_Equi_Type_Belt:
		//if (FALSE)
		//{
		//	LOG_CF_D( L"首饰已经不能继续强化了~!");
		//	return FALSE;
		//}
		break;
	default:
		LOG_CF_D( L"该类型:%X, 不支持强化", emType);
		return FALSE;
	}

	
	CBag Equi; // 进化的装备
	if (!CBagFunction::GetInstance().GetEqui_By_Type(emType, &Equi))
	{
		LOG_CF_D( L"无法在身上找到装备:%X!", emType);
		return FALSE;
	}

	CBag Sacrifice; // 祭品
	while (GameRun && !EMPTY_PERSONHP && CBagFunction::GetInstance().ExistItem_By_Name(wsSacrificeName, &Sacrifice))
	{
		// 强化
		LOG_CF_D( L"开始强化, 装备:%s, 祭品:%s", Equi.GetName().c_str(), Sacrifice.GetName().c_str());
		Sacrifice.EquiEnhanced(Equi.GetSId(), NULL);
		BnsSleep(5 * 1000);
	}

	return TRUE;
}

BOOL CBagAction::BaGuaGrowth(_In_ CONST std::wstring& wsSacrificeName) CONST
{
	std::vector<CBag> VecEqui;
	CObjectFunction::GetInstance().GetBagList(VecEqui, CObjectFunction::em_Bag_Type::em_Bag_Type_Equi);

	for (CONST auto& itm : VecEqui)
	{
		DWORD dwItemType = itm.GetItemType();
		if (dwItemType < static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams1) || dwItemType > static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams8))
			continue;
		else if (itm.IsGrowthed())
			continue;

		CBagFunction::GetInstance().DoItem_By_ExistItem_To_Name(wsSacrificeName, [itm](CONST CBag& Sacrifice)
		{
			LOG_CF_D(L"开始强化八卦[%s], 祭品[%s]", itm.GetName().c_str(), Sacrifice.GetName().c_str());
			itm.GrowthBaGua(Sacrifice.GetSId());
			BnsSleep(5 * 1000);
		});
	}
	return TRUE;
}

BOOL CBagAction::WearEqui_By_Name(_In_ CONST std::wstring& wsEquiName) CONST
{
	CBag Equi;
	if (!CBagFunction::GetInstance().ExistItem_By_Name(wsEquiName, &Equi))
		LOG_CF_D( L"穿装备:%s 不存在");
	else
		Equi.WearEqui();

	return TRUE;
}

BOOL CBagAction::RollWheel_By_ItemName(_In_ CONST std::wstring&, _In_ DWORD) CONST
{
	/*Point FixPoint = CGameObject::GetPersonPoint();

	CCorpse Corpse(0x0);
	if (!FindCorpse_By_Id(dwWheelId, &Corpse, &FixPoint, 10.0f))
	{
		LOG_CF_D( L"找不到转盘:%X", dwWheelId);
		return FALSE;
	}

	CSomeFun::MoveToPoint(Corpse.GetPoint(), 2.0f);
	if (CGameObject::GetPerson()->GetDis(Corpse.GetPoint()) >= 5.0f)
	{
		LOG_CF_D( L"无法靠近大转盘! Dis=%.2f", CGameObject::GetPerson()->GetDis(Corpse.GetPoint()));
		return FALSE;
	}

	CBag Item(0x0, 0x0);
	while (GameRun && !EMPTY_PERSONHP && CSomeFun::ExistItem_By_Name(wsItemName, &Item) && !CSomeFun::IsBagFull())
	{
		// 开始摇
		Corpse.TakeItem();
		BnsSleep(4 * 1000);

		for (int i = 0; i < 4; ++i)
		{
			CSomeFun::SimlationKey('F');
			BnsSleep(1000);
		}
	}*/

	// 是不是一定要摇到8个八卦才穿?
	// 是不是一定要开到XX武器?
	return TRUE;
}

BOOL CBagAction::SaveItem(_In_ int nBagType, _In_ CONST std::wstring& wsNpcName) CONST
{
	CNpc Npc;
	if (!CNpcExtend::GetInstance().FindNpc_By_Name(wsNpcName, CBaseObject::CreateFixPointInfo(30.0f), &Npc))
	{
		LOG_CF_D( L"30m范围内找不到仓库Npc:%s", wsNpcName.c_str());
		return FALSE;
	}

	if (Npc.GetDis() > 3.0f)
	{
		BnsMove(Npc.GetPoint(), 3);
		BnsSleep(2000);
	}

	std::vector<CBag> vlst;
	CObjectFunction::em_Bag_Type emBagType = static_cast<CObjectFunction::em_Bag_Type>(nBagType);
	CObjectFunction::GetInstance().GetBagList(vlst, CObjectFunction::em_Bag_Type_Bag);
	for (CONST auto& itm : vlst)
	{
		if (CFilter::GetInstance().ExistItemFilter(itm.GetName()) == CFilter::ItemFilterType::Filter_Item_Save)
		{
			DWORD dwWareHouseEmptySId = 0;
			if (!CBagFunction::GetInstance().GetEmptyItemObj(emBagType, dwWareHouseEmptySId))
				break;

			Npc.SaveItem(itm.GetSId(), itm.GetCount(), dwWareHouseEmptySId);
			BnsSleep(300);
		}
	}
	return TRUE;
}

BOOL CBagAction::PullItem_By_WareHouse(_In_ CONST std::wstring& wsNpcName, _In_ CONST std::vector<std::wstring>& vItem) CONST
{
	CNpc Npc;
	if (!CNpcExtend::GetInstance().FindNpc_By_Name(wsNpcName, CBaseObject::CreateFixPointInfo(30.0f), &Npc))
	{
		LOG_CF_D( L"30m范围内找不到仓库Npc:%s", wsNpcName.c_str());
		return FALSE;
	}

	if (Npc.GetDis() > 3.0f)
	{
		BnsMove(Npc.GetPoint(), 3);
		BnsSleep(2000);
	}

	CBag itm;
	for (int i = 0; i < static_cast<int>(vItem.size()) && GameRun && !EMPTY_PERSONHP;)
	{
		if (!ExistItemByNameWithType(vItem.at(i), CObjectFunction::em_Bag_Type_Warehouse, &itm))
		{
			i += 1;
			continue;
		}
		DWORD dwEmptyBagId = 0;
		if (!CBagFunction::GetInstance().GetEmptyItemObj(CObjectFunction::em_Bag_Type_Bag, dwEmptyBagId))
		{
			LOG_CF_D( L"背包满了!");
			break;
		}

		LOG_CF_D( L"从仓库取出物品:%s", itm.GetName().c_str());
		Npc.SaveItem(itm.GetSId(), itm.GetCount(), dwEmptyBagId);
		BnsSleep(1000);
	}
	return TRUE;
}


BOOL CBagAction::PullItemByWardrobeWithList(_In_ CONST std::wstring& wsNpcName, _In_ CONST CBagAction::tagPullWardrobeItm& PullItm) CONST
{
	CNpc Npc;
	if (!CNpcExtend::GetInstance().FindNpc_By_Name(wsNpcName, CBaseObject::CreateFixPointInfo(30.0f), &Npc))
	{
		LOG_CF_D( L"30m范围内找不到仓库Npc:%s", wsNpcName.c_str());
		return FALSE;
	}

	if (Npc.GetDis() > 3.0f)
	{
		BnsMove(Npc.GetPoint(), 3);
		BnsSleep(2000);
	}

	CBag itm;
	UINT uTotal = 0;
	while (GameRun && !EMPTY_PERSONHP && uTotal < PullItm.uCount && ExistItemByNameWithType(PullItm.wsItmName, CObjectFunction::em_Bag_Type_Wardrobe, &itm))
	{
		DWORD dwEmptyBagId = 0;
		if (!CBagFunction::GetInstance().GetEmptyItemObj(CObjectFunction::em_Bag_Type_Bag, dwEmptyBagId))
		{
			LOG_CF_E( L"背包满了!");
			break;
		}

		UINT uCount = PullItm.uCount - uTotal;
		uCount = itm.GetCount() > uCount ? uCount : itm.GetCount();
		uTotal += uCount;

		LOG_CF_D( L"从衣柜取出物品:%s 数量:%d", itm.GetName().c_str(), uCount);
		Npc.SaveItem(itm.GetSId(), uCount, dwEmptyBagId);
		BnsSleep(1000);
	}
	return TRUE;
}


BOOL CBagAction::QuickRepairWeaspon() CONST
{
	// 需要武器持久度低于5
	if (CBagFunction::GetInstance().GetEndurance() >= 5)
		return FALSE;
	else if (!CBagFunction::GetInstance().ExistItem_By_Name(快速修理锤, nullptr))
		return FALSE;
	// 需要非战斗状态!
	else if (!CPersonFunction::GetInstance().WaitForWarStatus())
		return FALSE;

	return CBagFunction::GetInstance().DoItem_By_ExistItem_To_Name(快速修理锤, [](CONST CBag& Hammer){
		Hammer.UseItem();
		BnsSleep(2 * 1000);
		CPersonFunction::GetInstance().WaitForProgressBar(10 * 1000);
	});
}

BOOL CBagAction::Buy(_In_ CONST std::wstring& wsItemName, _In_ DWORD dwPageIndex, _In_ DWORD dwIndex, _In_ DWORD dwCount) CONST
{
	CBag Itm;
	if (!CBagFunction::GetInstance().ExistItem_By_Name(wsItemName, &Itm))
		return FALSE;

	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&wsItemName, &dwPageIndex, &dwIndex, &dwCount, &Itm]{
		CGameSend::BuyItem_By_Bag(dwPageIndex, dwIndex, dwCount, Itm.GetSId());
	});
}

BOOL CBagAction::RemoveALLGem() CONST
{
	std::vector<DWORD> vGem;
	while (GameRun && !EMPTY_PERSONHP && CObjectFunction::GetInstance().GetWeasponGemList(vGem) != 0)
	{
		for (int i = 0; i < 5 && vGem.size() > 0; ++i, CObjectFunction::GetInstance().GetWeasponGemList(vGem))
		{
			LOG_CF_D( L"正在移除第%d个宝石", i);
			CBag::RemoveGem_By_Index(i);
			BnsSleep(5 * 1000);
		}
	}
	return TRUE;
}

BOOL CBagAction::ExistItemByNameWithType(_In_ CONST std::wstring& wsItemName, _In_ int nBagType, _Out_opt_ CBag* pBag) CONST
{
	std::vector<CBag> vlst;
	CObjectFunction::GetInstance().GetBagList(vlst, static_cast<CObjectFunction::em_Bag_Type>(nBagType));

	return MyTools::CLPublic::Vec_find_if(vlst, pBag, [&wsItemName](CONST CBag& itm){return itm.GetName() == wsItemName; });
}

VOID CBagAction::DiscardItem_By_Condition(_In_ std::function<BOOL(CONST CBag&)> fnItemFilter) CONST
{
	std::vector<CBag> vlst;
	CObjectFunction::GetInstance().GetBagList(vlst, CObjectFunction::em_Bag_Type::em_Bag_Type_Bag);
	for (CONST auto& itm : vlst)
	{
		if (fnItemFilter(itm))
		{
			LOG_CF_D( L"丢副本装备:%s, 颜色:%s", itm.GetName().c_str(), itm.GetColorName().c_str());
			itm.DiscardItem();
			BnsSleep(500);
		}
	}
}

VOID CBagAction::UseItem_By_Condition(_In_ std::function<BOOL(CONST CBag&)> fnItemFilter) CONST
{
	std::vector<CBag> vlst;
	CObjectFunction::GetInstance().GetBagList(vlst, CObjectFunction::em_Bag_Type::em_Bag_Type_Bag);
	for (CONST auto& itm : vlst)
	{
		if (fnItemFilter(itm))
		{
			LOG_CF_D( L"正在使用物品:%s", itm.GetName().c_str());
			itm.UseItem();
			BnsSleep(500);
		}
	}
}

BOOL CBagAction::OpenBox_By_Condition(_In_ std::function<BOOL(CONST CBag&)> fnItemFilter) CONST
{
	BOOL bRetCode = TRUE;
	while (GameRun && !EMPTY_PERSONHP && bRetCode && !CBagFunction::GetInstance().IsBagFull(CObjectFunction::em_Bag_Type::em_Bag_Type_Bag, 5))
	{
		bRetCode = CBagFunction::GetInstance().DoItem_By_EixstItem(fnItemFilter, [](CONST CBag& itm)
		{
			LOG_CF_D( L"正在开箱子:%s", itm.GetName().c_str());
			itm.OpenBox();
			BnsSleep(500);
		});
	}
	return TRUE;
}

BOOL CBagAction::Explain_By_Condition(_In_ std::function<BOOL(CONST CBag&)> fnItemFilter) CONST
{
	BOOL bRetCode = TRUE;
	while (GameRun && !EMPTY_PERSONHP && bRetCode && !CBagFunction::GetInstance().IsBagFull(CObjectFunction::em_Bag_Type::em_Bag_Type_Bag, 5))
	{
		bRetCode = CBagFunction::GetInstance().DoItem_By_EixstItem(fnItemFilter, [](CONST CBag& itm)
		{
			LOG_CF_D( L"正在分解:%s", itm.GetName().c_str());
			itm.Explain();
			BnsSleep(500);
		});
	}
	return TRUE;
}

BOOL CBagAction::SellItem_By_Condition(_In_ CONST std::wstring& wsNpcName, _In_ std::function<BOOL(CONST CBag&)> fnItemFilter) CONST
{
	CNpc Npc;
	if (!CNpcExtend::GetInstance().FindNpc_By_Name(wsNpcName, CBaseObject::CreateFixPointInfo(10), &Npc))
	{
		LOG_CF_E(L"10m之内都没有Npc[%s]", wsNpcName.c_str());
		return FALSE;
	}

	BnsMove(Npc.GetPoint(), 1);
	BnsSleep(2000);

	std::vector<CBag> VecItem;
	CObjectFunction::GetInstance().GetBagList(VecItem, CObjectFunction::em_Bag_Type::em_Bag_Type_Bag);
	for (CONST auto& itm : VecItem)
	{
		if (fnItemFilter(itm))
		{
			LOG_CF_D(L"正在出售物品[%s]", itm.GetName().c_str());
			Npc.Sell(itm);
			BnsSleep(500);
		}
	}

	return TRUE;
}
