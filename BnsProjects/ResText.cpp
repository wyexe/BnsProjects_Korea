#include "stdafx.h"
#include "ResText.h"
#include <MyTools/Log.h>
#include "GameVariable.h"
#include "BnsFilter.h"
#include "Person.h"

#define _SELF L"ResText.cpp"


VOID CResText::Initialize()
{
	SetErrorPtr([](CONST std::wstring& wsErrText)
	{
		StopGame;
		LOG_MSG_CF(CLog::em_Log_Type_Exception, wsErrText.c_str());
	});
	// 物品过滤
	RegisterItemFilterText();

	RegisterMonsterFilterText();
}

VOID CResText::RegisterItemFilterText()
{
	CONST static std::vector<CFilter::ItemFilterVarInfo> VecText =
	{
		{ L"丢", CFilter::Filter_Item_Drop },
		{ L"卖", CFilter::Filter_Item_Sell },
		{ L"存", CFilter::Filter_Item_Save },
		{ L"分解", CFilter::Filter_Item_Explain },
		{ L"保留", CFilter::Filter_Item_Keep },
		{ L"默认", CFilter::Filter_Item_Default }
	};
	Register(L"VecItemFilterText", &VecText);
}

VOID CResText::RegisterMonsterFilterText()
{
	CONST static std::vector<CFilter::MonsterFilterVarInfo> VecText =
	{
		{ L"优先", CFilter::Filter_Monster_High },
		{ L"必杀", CFilter::Filter_Monster_Default },
		{ L"不杀", CFilter::Filter_Monster_Low }
	};
	Register(L"VecMonsterFilterText", &VecText);
}

VOID CResText::RegisterItemSellText()
{
	CONST static std::wstring wsText1 = L"잡동사니";
	Register(L"杂物", &wsText1);

	CONST static std::wstring wsText2 = L"광물재료";
	Register(L"矿物材料", &wsText2);
}

VOID CResText::RegisterDisConnectText()
{
	CONST static std::vector<CPerson::ReasonText> VecText = 
	{
		{ L"与游戏服务器断开连接, 估计VPN挂了", CPerson::em_LoseConnect_Reason::em_LoseConnect_Reason_LoseConnect },
		{ L"正常状态呐……不该啊!", CPerson::em_LoseConnect_Reason::em_LoseConnect_Reason_Normal },
		{ L"其他原因断开连接的! 暂时不知道是为嘛!", CPerson::em_LoseConnect_Reason::em_LoseConnect_Reason_Other },
		{ L"被挤号啦!!!", CPerson::em_LoseConnect_Reason::em_LoseConnect_Reason_SqueezeAccount },
	};

	Register(L"断开连接文本", &VecText);
}

VOID CResText::RegisterPlayerClassesText()
{
	CONST static std::vector<PlayerclassesText> VecText = 
	{
		{ em_Player_Classes_Destroyer, L"力士" },
		{ em_Player_Classes_Assassin, L"刺客" },
		{ em_Player_Classes_BladeMaster, L"剑士" },
		{ em_Player_Classes_Summoner, L"召唤师" },
		{ em_Player_Classes_ForceMaster, L"气功" },
		{ em_Player_Classes_KongFuMaster, L"拳师" },
		{ em_Player_Classes_Danncer, L"灵剑士" },
		{ em_Player_Classes_Warlock, L"咒术师" },
		{ em_Player_Classes_KongFuForceMaster, L"气宗" }
	};
	Register(L"角色职业", &VecText);
}

VOID CResText::RegisterItemDiscardName()
{
	/////丢弃的紫色八卦名字/////////////////////////////////////////////////////////////////////
	CONST static std::vector<std::wstring> VecBaGuaText =
	{
		// 鬼门关八卦          雪人洞八卦           修罗王八卦
		L"봉인된 갈마보패", L"봉인된 설인보패", L"봉인된 수라보패",
	};

	Register(L"丢弃的紫色八卦名字", &VecBaGuaText); 
}

VOID CResText::ReigisterItemName()
{
	CONST static std::wstring wsName1 = L"회복약";
	Register(L"恢复药", &wsName1);

	CONST static std::wstring wsName2 = L"만능 수리도구";
	Register(L"快速修理锤", &wsName2);

	CONST static std::wstring wsName3 = L"염료";
	Register(L"染料", &wsName3);

	CONST static std::wstring wsName4 = L"성장부적";
	Register(L"经验符", &wsName4);

	CONST static std::wstring wsName5 = L"";
	Register(L"", &wsName5);

	CONST static std::vector<std::wstring> VecWeasponBoxText = 
	{
		L"백청무기", L"갈마무기",
	};
	Register(L"武器箱子", &VecWeasponBoxText);

	CONST static std::vector<std::wstring> VecGungoenBoxText =
	{
		L"공로상", L"전리품",
	};
	Register(L"副本箱子", &VecGungoenBoxText);

	CONST static std::vector<std::wstring> VecDiscardItemTypeText = 
	{
		// 项链    戒指     耳环		手镯    腰带		手套
		L"목걸이", L"반지", L"귀걸이", L"팔찌", L"허리띠", L"장갑"
	};
	Register(L"丢弃首饰", &VecDiscardItemTypeText);
}

