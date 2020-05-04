#include "stdafx.h"
#include "DailyTask_Item.h"
#include <MyTools/Log.h>
#include "BagFunction.h"
#include "ObjectFunction.h"
#include "BagAction.h"
#include "Person.h"
#include "PersonFunction.h"
#include "ConsoleLog.h"
#include "Npc.h"
#include "NpcExtend.h"
#include "SomeFun.h"
#include "BnsFilter.h"

#define _SELF L"DailyTask_Item.cpp"

BOOL CDailyTask_Item::IsRepairWeaspon(_In_ DWORD dwValue) CONST
{
	return CBagFunction::GetInstance().GetEndurance() < dwValue;
}

VOID CDailyTask_Item::RepairWeaspon(_In_ DWORD dwValue) CONST
{
	// 最多修理3次, 有可能是锤子数量不足!
	for (int i = 0; i < 3 && GameRun && !EMPTY_PERSONHP && CBagFunction::GetInstance().GetEndurance() < dwValue; ++i)
	{
		LOG_CF_D(L"需要修理武器!");
		CBag Hammer;
		if (CBagFunction::GetInstance().ExistHammer(&Hammer))
		{
			CPersonFunction::GetInstance().WaitToLoadProgressBar(2 * 1000, 20 * 1000, [&] {Hammer.UseItem(); });
			LOG_CF_D(L"修理完毕!");
		}
		else
		{
			LOG_MSG_CF(L"没锤子了!");
			StopGame;
			break;
		}
	}
}

BOOL CDailyTask_Item::CheckDailyTaskInitializeItem() CONST
{
	// 需要自动交易吗?
	if (CBagFunction::GetInstance().GetItemCount_By_Name(恢复药) < 20)
	{
		CConsoleLog(L"恢复药不足20个!");
		return FALSE;
	}
	if (CBagFunction::GetInstance().GetItemCount_By_Name(包子) < 20)
	{
		CConsoleLog(L"包子不足20个!");
		return FALSE;
	}

	return TRUE;
}

BOOL CDailyTask_Item::IsDealWithDungeonItem() CONST
{
	return CBagFunction::GetInstance().IsBagFull(CObjectFunction::em_Bag_Type_Bag, 16);
}

BOOL CDailyTask_Item::DealWithDungeonItem_Sell(_In_ CONST std::wstring& wsNpcName) CONST
{
	CONST static std::set<std::wstring> SetDropTypeName =
	{
		Item_项链,Item_戒指,Item_耳环,Item_手镯,Item_腰带,Item_手套
	};

	return CBagAction::GetInstance().SellItem_By_Condition(wsNpcName, [](CONST CBag& itm) 
	{
		std::wstring wsItemTypeName = itm.GetTypeName();
		CONST std::wstring wsItemName = itm.GetName();

		if (itm.GetColor() != CBag::em_ItemColor::em_ItemColor_Legend && SetDropTypeName.find(wsItemTypeName) != SetDropTypeName.end())
		{
			LOG_CF_D(L"丢弃所有首饰:%s", wsItemName.c_str());
			return TRUE;
		}

		// 白青武器箱不要
		if (wsItemTypeName == Item_箱子 && wsItemName.find(Item_白青武器) != -1)
		{
			LOG_CF_D(L"白青武器箱不要[%s]", wsItemName.c_str());
			return TRUE;
		}

		CBag::em_Equi_Type emEquiType = static_cast<CBag::em_Equi_Type>(itm.GetItemType());

		// 所有武器不是传说的, 都丢了
		if (emEquiType == CBag::em_Equi_Type_Weapon && itm.GetColor() != CBag::em_ItemColor_Legend)
		{
			LOG_CF_D(L"所有非传说的武器都丢了:%s", wsItemName.c_str());
			return TRUE;
		}

		if ((emEquiType == CBag::em_Equi_Type::em_Equi_Type_LatestFashion) || (emEquiType == CBag::em_Equi_Type::em_Equi_Type_Cloth && wsItemName != Item_浑天教教徒服 && wsItemName != Item_武林盟盟徒服))
		{
			LOG_CF_D(L"所有非阵营衣服都丢了!");
			return TRUE;
		}

		// 紫色以下的八卦也不要
		if (emEquiType >= CBag::em_Equi_Type::em_Equi_Type_Diagrams1 && emEquiType <= CBag::em_Equi_Type::em_Equi_Type_Diagrams8 && itm.GetColor() < CBag::em_ItemColor::em_ItemColor_Pink)
		{
			LOG_CF_D(L"紫色以下的八卦[%s]一律丢了!", wsItemName.c_str());
			return TRUE;
		}

		return FALSE;
	});
}

VOID CDailyTask_Item::DealWithDungeonItem() CONST
{
	LOG_CF_D(L"开启所有的日常功勋盒!");
	CBagAction::GetInstance().OpenBox_By_Condition([](CONST CBag& itm) { return itm.GetName().find(Item_功勋箱) != -1; });

	LOG_CF_D(L"开启所有的战利品");
	CBagAction::GetInstance().OpenBox_By_Condition([](CONST CBag& itm) { return itm.GetName().find(Item_战利品) != -1; });

	LOG_CF_D(L"开启八卦箱子");
	CBagAction::GetInstance().OpenBox_By_Condition([](CONST CBag& itm) { return itm.GetName() == Item_常墨蛇蝎八卦箱; });

	LOG_CF_D(L"经验符");
	CBagAction::GetInstance().UseItem_By_Condition([](CONST CBag& itm) { return itm.GetTypeName() == Item_成长护符; });

	LOG_CF_D(L"处理衣服");
	DealWithCloth();

	LOG_CF_D(L"处理宝石");
	DealWithGem();

	LOG_CF_D(L"处理副本箱子");
	DealWithDungeonBox();

	LOG_CF_D(L"分解");
	CBagAction::GetInstance().ExplainItem();
}

VOID CDailyTask_Item::DealWithDungeonBox() CONST
{
	CONST static std::vector<std::wstring> VecBoxName =
	{
		// 普通日常
		L"마골카 일반 전리품", L"자칸 일반 전리품", L"자칸 도전 전리품",L"백광 보화 상자"
		// 黑龙教                              
		Item_黑龙教地下监狱功勋箱,
	};
	for (CONST auto& BoxName : VecBoxName)
	{
		CBagAction::GetInstance().OpenBox_By_Condition([BoxName](CONST CBag& itm) { return itm.GetName() == BoxName; });
	}
}

VOID CDailyTask_Item::DealWithDungeon_Drop() CONST
{
	LOG_CF_D(L"处理副本物品(丢)!");
	CONST static std::set<std::wstring> SetDropBaGuaName =
	{
		Item_封印的蛇蝎八卦牌, Item_封印的雪人八卦牌, Item_封印的修罗八卦牌
	};

	CBagAction::GetInstance().DiscardItem_By_Condition([](CONST CBag& itm)
	{
		std::wstring wsItemName = itm.GetName();

		// 物品过滤=丢弃
		if (CFilter::GetInstance().ExistItemFilter(wsItemName) == CFilter::ItemFilterType::Filter_Item_Drop)
			return TRUE;

		// 武器箱子
		if (wsItemName.find(Item_白青武器) != -1 || wsItemName.find(Item_蛇蝎武器) != -1)
			return TRUE;

		// 丢弃的紫色八卦
		if (itm.GetColor() == CBag::em_ItemColor_Pink && SetDropBaGuaName.find(wsItemName) != SetDropBaGuaName.end())
			return TRUE;

		// 过滤掉非装备!
		if (!itm.IsEquiment())
			return FALSE;

		if (itm.GetColor() > CBag::em_ItemColor::em_ItemColor_Blue)
			return FALSE;

		LOG_CF_D(L"丢副本装备[%s], 颜色[%s]", wsItemName.c_str(), itm.GetColorName().c_str());
		return TRUE;
	});
}

VOID CDailyTask_Item::DealWithCloth() CONST
{
	// 判断身上有没有这件服饰, 没的话就穿了, 有的话就丢了
	static CONST std::set<CBag::em_Equi_Type> SetDropEquiType =
	{
		CBag::em_Equi_Type::em_Equi_Type_Glasses, CBag::em_Equi_Type::em_Equi_Type_Cloth,
		CBag::em_Equi_Type::em_Equi_Type_Hat, CBag::em_Equi_Type::em_Equi_Type_LatestFashion
	};

	CBagAction::GetInstance().DiscardItem_By_Condition([](CONST CBag& itm)
	{
		CONST std::wstring wsItemName = itm.GetName();
		if (itm.GetItemType() == NULL || wsItemName == Item_浑天教教徒服 || wsItemName == Item_武林盟盟徒服)
			return FALSE;

		if (itm.GetTypeName() == Item_染料)
		{
			LOG_CF_D(L"染料丢了!");
			return TRUE;
		}

		CBag::em_Equi_Type emEquiType = static_cast<CBag::em_Equi_Type>(itm.GetItemType());
		if (SetDropEquiType.find(emEquiType) == SetDropEquiType.end()) // 只丢这些种类的衣服(帽子,眼镜都是属于衣服分类)
			return FALSE;

		// 判断这件服饰身上是否存在
		if (!CBagFunction::GetInstance().GetEqui_By_Type(emEquiType, nullptr))
		{
			LOG_CF_D(L"身上没这件服饰,穿上[%s]", wsItemName.c_str());
			itm.WearEqui();
			BnsSleep(500);
			return FALSE;
		}

		LOG_CF_D(L"这件类型的服饰[%s]已经有了,丢了!", wsItemName.c_str());
		return TRUE;
	});
}

VOID CDailyTask_Item::DealWithGem() CONST
{
	// 所有三角全丢了
	std::vector<CBag> VecItem;
	CObjectFunction::GetInstance().GetBagList(VecItem, CObjectFunction::em_Bag_Type::em_Bag_Type_Bag);
	for (CONST auto& itm : VecItem)
	{
		CONST std::wstring wsName = itm.GetName();
		//LOG_CF_D(L"[%s].size=%d", wsName.c_str(),wsName.length());
		if (wsName.length() != 5)
			continue;

		if (wsName[1] != L'각' || wsName[4] != L'옥')
			continue;

		// 三 ||四
		if (wsName[0] == L'삼'/* || wsName[0] == L'사'*/)
		{
			itm.DiscardItem();
			BnsSleep(500);
		}
	}
}