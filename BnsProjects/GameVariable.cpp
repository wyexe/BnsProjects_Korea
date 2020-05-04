#include "stdafx.h"
#include "GameVariable.h"
#include <iterator>
#include <time.h>
#include <map>
#include <set>
#include <MyTools/CLLock.h>
#include <MyTools/CLHook.h>
#include <MyTools/CLSearchBase.h>
#include <MyTools/Log.h>
#include "SomeFun.h"

#define _SELF L"GameVariable.cpp"

DWORD g_dwMoveBase = NULL;
DWORD g_dwMoveStatusOffset = NULL;
em_GameCmd g_GameStatus = em_GameCmd_None;
DWORD g_dwTimerId = NULL;
DWORD g_dwSimulationBase = NULL;
DWORD g_dwSimulationCALL = NULL;
DWORD g_dwOptimizationTime = NULL;
DWORD g_dwOldD3dCall = NULL;
DWORD g_dwHookD3dAddr = NULL;
DWORD g_dwHookSpeedAddr = NULL;
DWORD g_dwSpeedCALL = NULL;
float g_fSpeed = 1.0f;
BOOL g_bExitGame = FALSE;
BOOL g_bUninstall = FALSE;



PSHARED_INFO& CGameVariable::GetShareInfo() CONST
{
	static PSHARED_INFO pShareInfo = nullptr;
	return pShareInfo;
}

PACCOUNT_INFO_GAME& CGameVariable::GetAccShareInfo() CONST
{
	static PACCOUNT_INFO_GAME pAccShareInfo = nullptr;
	return pAccShareInfo;
}

DWORD& CGameVariable::GetVariable_By_Name(_In_ CONST std::wstring& wsName) CONST
{
	static std::map<std::wstring, DWORD> GlobalVariableMap;
	static MyTools::CLLock Lock(L"CGameVariable::GetVariable_By_Name");


	DWORD* pdwValue = nullptr;
	Lock.Access([&pdwValue, &wsName]
	{
		auto itr = GlobalVariableMap.find(wsName);
		if (itr != GlobalVariableMap.end())
			pdwValue = &itr->second;
		else
			GlobalVariableMap.insert(std::map<std::wstring, DWORD>::value_type(wsName, 0));
	});

	return pdwValue != nullptr ? *pdwValue : GetVariable_By_Name(wsName);
}


ULONGLONG& CGameVariable::GetTickVariable_By_Name(_In_ CONST std::wstring& wsName) CONST
{
	static std::map<std::wstring, ULONGLONG> GlobalVariableMap;
	static MyTools::CLLock Lock(L"CGameVariable::GetTickVariable_By_Name");


	ULONGLONG* pdwValue = nullptr;
	Lock.Access([&pdwValue, &wsName]{
		auto itr = GlobalVariableMap.find(wsName);
		if (itr != GlobalVariableMap.end())
			pdwValue = &itr->second;
		else
			GlobalVariableMap.insert(std::map<std::wstring, ULONGLONG>::value_type(wsName, 0));
	});

	return pdwValue != nullptr ? *pdwValue : GetTickVariable_By_Name(wsName);
}


BOOL CGameVariable::Action_Map_Kffm_Fm_SpecialMonster(_In_ std::function<VOID(std::map<std::wstring, FixPointInfo>&)> fn) CONST
{
	static std::map<std::wstring, FixPointInfo> map_;
	return GetVarLock().Access([&fn]{fn(map_); });
}

BOOL CGameVariable::DoList_HookList(_In_ std::function<VOID(std::vector<MyTools::MYHOOK_CONTENT>&)> fn) CONST
{
	static std::vector<MyTools::MYHOOK_CONTENT> vlst;
	fn(vlst);
	return TRUE;
}

CONST std::wstring CGameVariable::GetCurMapInfo(_In_ DWORD dwCurrentCityId, _In_ DWORD dwCurrentMapId) CONST
{
	// 当前人物在副本里面
	if (FindInstanceMapIdList(dwCurrentMapId) != nullptr)
		return L"";
	
	CONST ProvinceInfo* pProvinceInfo = FindProvinceInfoList(dwCurrentCityId);

	return pProvinceInfo == nullptr ? L"" : pProvinceInfo->wsProvinceName;
}

CGameVariable::em_SpicalName_Type CGameVariable::ExistSpicalName(_In_ CONST std::wstring& wsPlayerName) CONST
{
	auto pShareInfo = GetShareInfo();
	for (UINT i = 0; i < pShareInfo->ConsoleContent.GlobalAccount.Mail.uCount; ++i)
	{
		if (MyTools::CCharacter::wstrcmp_my(wsPlayerName.c_str(), pShareInfo->ConsoleContent.GlobalAccount.Mail.wszPlayerName[i]))
			return em_SpicalName_Type::em_SpicalName_Type_Mail;
	}

	for (UINT i = 0; i < pShareInfo->ConsoleContent.GlobalAccount.Warehouse.uCount; ++i)
	{
		if (MyTools::CCharacter::wstrcmp_my(wsPlayerName.c_str(), pShareInfo->ConsoleContent.GlobalAccount.Warehouse.Warehouse[i].wszPlayerName))
			return em_SpicalName_Type::em_SpicalName_Type_Warehouse;
	}
	
	return em_SpicalName_Type::em_SpicalName_Type_None;
}

DWORD CGameVariable::SetVariable_By_Name(_In_ CONST std::wstring& wsName, DWORD dwValue) CONST
{
	auto dwOldValue = GetVariable_By_Name(wsName);
	GetVariable_By_Name(wsName) = dwValue;
	return dwOldValue;
}

BOOL CGameVariable::Action_Variable_By_Name(_In_ CONST std::wstring& wsName, _In_ DWORD dwValue, _In_ std::function<VOID(VOID)> fn) CONST
{
	if (GetVariable_By_Name(wsName) == dwValue)
		fn();
	return TRUE;
}

DWORD CGameVariable::GetGameChartValue() CONST
{
	static DWORD dwBase = NULL;
	static DWORD dwOffset = NULL;
	if (dwBase == NULL)
	{
		dwBase = MyTools::CLSearchBase::FindBase("E8????????C605????????008B??????????80B9????????00", 0x090F90EA - 0x090F90F6, 0x2, 0, L"bsengine_Shipping");
		if (dwBase == NULL)
		{
			LOG_MSG_CF(L"动画的自动更新地址失效拉!");
			return 0;
		}

		dwOffset = MyTools::CLSearchBase::FindBase("8B0D????????8A??????????33??3B", 0x08E20606 - 0x08E2060C, 0x2, 0, L"bsengine_Shipping");
		if (dwOffset == NULL)
		{
			LOG_MSG_CF( L"动画的自动更新地址偏移失效拉!");
			return 0;
		}
		//LOG_CF_D( L"GameChart:[[%X]+%X]", dwBase, dwOffset);
	}
	return ReadDWORD(ReadDWORD(dwBase) + dwOffset);
}

CONST CGameVariable::CityInfo* CGameVariable::FindInstanceMapIdList(_In_ DWORD dwInstanceMapId) CONST
{
	CONST static std::vector<CityInfo> vInstanceMapId = {
		// 御龙林
		{ L"枯井", 0xAA3 }, { L"山贼巢穴", 0xA39 }, { L"黑龙寨阵地", 0xA2A }, { L"冲角团_平南舰队前沿阵地", 0x834 }, { L"掘开的坟墓", 0xA29 }, { L"污染洞窟", 0xA46 }, { L"地下坑道", 0xA2C },
		{ L"幻夜森林", 0xA2B }, { L"秘密加工厂", 0xA3C }, { L"隐秘的幻魔圣地", 0x1150 }, { L"恶魔洞", 0xA2E }, { L"沉默的海盗船", 0x128A }, { L"激战的黑森林", 0x1282 }, { L"绿明湖仓库", 0xA30 },
		{ L"被侵蚀的黑枪族部落", 0x1770 }, { L"修炼洞窟", 0x92F }, { L"悬崖据点", 0xA2F }, { L"龙脉洞窟", 0xA3A }, { L"黑龙寨营地", 0xA35 }, { L"松岩洞", 0x993 }, { L"海岸洞穴", 0xA37 }, { L"冲角团平南舰队支部BOSS", 0x939 },
		// 大漠
		{ L"祠堂密室", 0xBF4 }, { L"神秘树洞", 0xC06 }, { L"浊气洞", 0xC24 }, { L"幻影草仓库", 0xBFF }, { L"渡劫教土门阵分舵", 0xBBF }, { L"巨岩遗址", 0xA3E }, { L"王霸军发迹地", 0xBF7 }, { L"风沙洞", 0xC00 },
		{ L"黄沙洞", 0xC00 }, { L"咆哮寺院", 0xC07 }, { L"逍遥派练武场", 0xBF9 }, { L"南天寺", 0xBC7 }, { L"暗影石窟", 0xC08 }, { L"驱魔树树根", 0xBFD }, { L"天鸟巢", 0xC04 }, { L"逍遥派洞穴", 0xBFE },
		{ L"仓库", 0xC16 }, { L"红莲墓地", 0xC21 }, { L"骨面洞", 0xC05 }, { L"暗影要塞", 0xC08 }, { L"武神之翼", 0xC18 }, { L"瓦罐团巢穴", 0xC02 }, { L"蛙族部落", 0xC03 }, { L"娃娃草种植园", 0x8FA },
		{ L"黑枪族宴会场", 0xC09 }, { L"斗神祭坛", 0xBFB }, { L"土窑坟", 0xBD4 }, { L"暗影奉安堂", 0xC0A }, { L"失心洞", 0xBFC },
		// 水月平原
		{ L"擎天盟东部基地", 0x119E }, { L"帝国军法器部队", 0x1132 }, { L"永灵族遗迹", 0x1199 }, { L"武神塔武神房间", 0x135F },
		{ L"灵石矿山", 0x1144 }, { L"永灵峰", 0x1149 }, { L"永灵洞窟", 0x1194 }, { L"瓦罐团酿造厂", 0x11A8 }, { L"猛兽洞穴", 0x11AD }, { L"狼神族营地", 0x11B2 }, { L"斗战神庙", 0x11B7 }, { L"地下仓库", 0x11E9 },
		{ L"悲鸣避难处", 0x11EE }, { L"蜘蛛巢穴", 0x11F3 }, { L"鳄鲛关卡东侧", 0x10D4 }, { L"鳄鲛关卡西侧", 0x11DF }, { L"水蛙宫", 0x11DA }, { L"大鱼坊仓库", 0x11D5 }, { L"鱼人族巢穴", 0x11D0 }, { L"帝国军运输队", 0x11BD },
		{ L"血猪族巢穴", 0x11BD }, { L"格物研究所", 0x11C7 }, { L"石怪洞窟", 0x11CB }, { L"夜幕笼罩血浪鲨湾", 0x1277 }, { L"暴风海蛇补给基地", 0x1275 }, { L"海蛇补给基地", 0x1260 }, { L"螺旋迷宫", 0x1271 }, { L"血浪鲨湾", 0x1268 },
		// 白青山脉
		{ L"禾武帮洞穴", 0x15A4 }, { L"浊魔洞", 0x13E9 }, { L"马贼巢穴", 0x157F }, { L"悬浮祭坛", 0x142B }, { L"雷狱", 0x1679 }, { L"雷狱", 0x167C }, { L"面目全非的江流市", 0x15F6 }, { L"面目全非的江流市", 0x15E1 },
		{ L"遗忘墓地", 0x1725 }, { L"分裂迷宫", 0x171D }, { L"雪玉宫", 0x170F }, { L"冰库", 0x127C }, { L"复活祭坛", 0x14B0 }, { L"雪人洞", 0x1717 }, { L"鬼门关的噩梦", 0x1714 }, { L"夜幕风之平原", 0x1581 },
		{ L"三天王洞", 0x1491 }, { L"凛风峡谷", 0x1482 }, { L"云林院", 0x148C }, { L"雪女洞", 0x149C }, { L"雪兽冰窟", 0x14A0 }, { L"黑龙教隐身处", 0x14A3 }, { L"后院", 0x15FF },
	};
	
	for (CONST auto& itm : vInstanceMapId)
	{
		if (itm.dwCityId == dwInstanceMapId)
			return &itm;
	}
	return nullptr;
}

CONST CGameVariable::ProvinceInfo* CGameVariable::FindProvinceInfoList(IN DWORD dwCityId) CONST
{
	CONST static std::vector<ProvinceInfo> vProvinceInfo = {
		{ L"无日峰", L"无日峰", 0x4D },
		// 御龙林
		{ L"御龙林", L"竹林村", 0x1 }, { L"御龙林", L"焚尸岗", 0x2 }, { L"御龙林", L"竹林卫岗哨", 0x45 }, { L"御龙林", L"森林营地", 0x32 }, { L"御龙林", L"被污染的渔村", 0x3 },
		{ L"御龙林", L"炭窑坊", 0x4A }, { L"御龙林", L"绿明村", 0x5 }, { L"御龙林", L"青蛙湖", 0x67 }, { L"御龙林", L"流浪者茅屋", 0x6 }, { L"御龙林", L"松树林营地", 0x7 },
		{ L"御龙林", L"松岩亭", 0x35 }, { L"御龙林", L"松林寺", 0x8 }, { L"御龙林", L"海岸洞穴入口", 0x36 }, { L"御龙林", L"巨岩驿站", 0x9 }, { L"御龙林", L"冲角团平南舰队", 0x34 },
		// 大漠
		{ L"大漠", L"孤村", 0xB }, { L"大漠", L"建元成道守卫队", 0xA }, { L"大漠", L"巫女庵", 0xC }, { L"大漠", L"浊气洞入口", 0x38 }, { L"大漠", L"润泽泉1", 0xD },
		{ L"大漠", L"润泽泉2", 0x39 }, { L"大漠", L"俞家村", 0x11 }, { L"大漠", L"王霸军军营", 0x12 }, { L"大漠", L"浪人营地", 0x13 }, { L"大漠", L"腊月湖", 0x14 },
		{ L"大漠", L"土门客栈", 0x16 }, { L"大漠", L"亡者渡口", 0x10 }, { L"大漠", L"毒风崖", 0x15 }, { L"大漠", L"流浪者的废弃茅屋", 0x17 }, { L"大漠", L"陶瓷坊总舵", 0x18 },
		{ L"大漠", L"王霸军指挥所", 0x19 }, { L"大漠", L"王霸军讨伐队", 0x1B }, { L"大漠", L"悲鸣村", 0x1C }, { L"大漠", L"王霸军白纹队", 0x1A }, { L"大漠", L"灰狼村", 0x1D },
		{ L"大漠", L"守卫茅屋", 0x3B }, { L"大漠", L"王霸军卫队", 0x1E }, { L"大漠", L"盗墓者黑市", 0x1F }, { L"大漠", L"红莲墓地入口", 0x3A }, { L"大漠", L"蛮骨岗哨", 0x22 }, { L"大漠", L"学者庙", 0x37 },
		// 水月平原
		{ L"水月平原", L"永灵村落", 0x26 }, { L"水月平原", L"白雾森林屯兵所", 0x27 }, { L"水月平原", L"???", 0x28 }, { L"水月平原", L"古树森林藏身处", 0x29 }, { L"水月平原", L"猎人营地", 0x43 },
		{ L"水月平原", L"海树村", 0x24 }, { L"水月平原", L"丘陵驿站", 0x23 }, { L"水月平原", L"斗战神庙院入口", 0x44 }, { L"水月平原", L"鳄鲛老将营地", 0x2C }, { L"水月平原", L"赤鲛营地", 0x41 },
		{ L"水月平原", L"太尚门哨所", 0x42 }, { L"水月平原", L"残亘道观", 0x2D }, { L"水月平原", L"水族蛙废墟", 0x3D }, { L"水月平原", L"大鱼坊总舵", 0x2A }, { L"水月平原", L"大鱼坊岗哨", 0x3C },
		{ L"水月平原", L"风车磨坊", 0x3E }, { L"水月平原", L"丰登谷工会", 0x25 }, { L"水月平原", L"海岸客栈", 0x3F }, { L"水月平原", L"冲角团平东舰队支部", 0x40 }, { L"水月平原", L"南部渡口", 0x4B }, { L"水月平原", L"武神塔", 0x4C },
		//白青山脉-风之平原
		{ L"风之平原", L"海岸村庄", 0x63 }, { L"风之平原", L"帝国军补给队", 0x62 }, { L"风之平原", L"绿林盗驻地入口", 0x53 }, { L"风之平原", L"丰收村", 0x51 }, { L"风之平原", L"浊魔窟入口", 0x78 },
		{ L"风之平原", L"染坊", 0x52 }, { L"风之平原", L"马贼巢穴", 0x73 }, { L"风之平原", L"彩虹湖", 0x77 },
		//白青山脉-江流市
		{ L"江流市", L"黄鼠狼据点", 0x7A }, { L"江流市", L"禾武帮黑市", 0x6E }, { L"江流市", L"风闻客栈", 0x5E }, { L"江流市", L"首都警卫队", 0x72 }, { L"江流市", L"风月馆", 0x5F }, { L"江流市", L"赵庄园", 0x6D },
		{ L"江流市", L"清风庭院", 0x6C }, { L"江流市", L"凤林寺", 0x71 }, { L"江流市", L"御史府", 0x7C }, { L"江流市", L"勤政殿", 0x7B }, { L"江流市", L"雷狱", 0x76 }, { L"江流市", L"雪琳宫", 0x65 },
		{ L"江流市", L"皇宫比武场", 0x70 }, { L"江流市", L"庆会楼", 0x66 }, { L"江流市", L"神惠院", 0x6F }, { L"江流市", L"寒风酒馆", 0x79 }, { L"江流市", L"天命祭坛", 0x7D },
		//白青山脉-白桦林
		{ L"白桦林", L"白林寺", 0x54 }, { L"白桦林", L"伐木场", 0x55 }, { L"白桦林", L"北方讨伐队", 0x56 }, { L"白桦林", L"兽人族温泉", 0x64 },
		//白青山脉-北方雪原
		{ L"北方雪原", L"雪女洞入口", 0x6A }, { L"北方雪原", L"药草工总舵", 0x57 }, { L"北方雪原", L"药草工的休息地", 0x58 }, { L"北方雪原", L"冒险家的野营地", 0x59 }, { L"北方雪原", L"雪兽冰窟入口", 0x6B },
		{ L"北方雪原", L"北方警戒所", 0x5A }, { L"北方雪原", L"黑龙教藏身处入口", 0x69 }, { L"北方雪原", L"冰雪村", 0x5B }, { L"北方雪原", L"白青派本山", 0x61 }, { L"北方雪原", L"冒险家的避难所", 0x5D },
	};

	for (CONST auto& itm : vProvinceInfo)
	{
		if (itm.dwCityId == dwCityId)
			return &itm;
	}
	return nullptr;
}


CGameVariable::CGameVariable()
{
	_fnHelpThreadLoopMethodPtr = nullptr;
}

BOOL CGameVariable::DoList_MainThreadFunPtr(_In_ std::function<VOID(std::queue<MainThreadFunPtrInfo>&)> fn) CONST
{
	static std::queue<MainThreadFunPtrInfo> vQueue;
	static MyTools::CLLock Lock(L"CGameVariable::DoList_MainThreadFunPtr");
	return Lock.Access([&fn]{
		fn(vQueue);
	});
}

BOOL CGameVariable::PushMainThreadFunPtr_synch(_In_ MainThreadFunPtr MainThreadFunPtr_) CONST
{
	if (::GetCurrentThreadId() != GetVariable_By_Name(VARIABLE_NMAE_MAINTHREADID))
	{
		HANDLE hEvent = ::CreateEventW(NULL, FALSE, FALSE, NULL);
		DoList_MainThreadFunPtr([MainThreadFunPtr_, hEvent](_In_ std::queue<MainThreadFunPtrInfo>& vQueue)
		{
			vQueue.push({ MainThreadFunPtr_, hEvent });
		});
		::WaitForSingleObject(hEvent, INFINITE);
		::CloseHandle(hEvent);
	}
	else
	{
		MainThreadFunPtr_();
	}
	
	return TRUE;
}

BOOL CGameVariable::ClearMainThreadFunPtr() CONST
{
	LOG_CF_D(L"ClearMainThreadFunPtr!");
	return DoList_MainThreadFunPtr([](_In_ std::queue<MainThreadFunPtrInfo>& vQueue)
	{
		while (!vQueue.empty())
		{
			vQueue.pop();
		}
	});
}

BOOL CGameVariable::IsMainTask_By_Id(_In_ DWORD dwTaskId) CONST
{
	CONST static std::set<DWORD> vMainTaskId = {
		0xDD, 0xDE, 0xDF, 0xE0, 0xE7, 0xE8, 0xE3, 0xF3, 0x12D,  \
		0x12E, 0x12F, 0x1F1, 0x130, 0x131, 0x132, 0x133, 0x134, \
		0x135, 0x136, 0x137, 0x138, 0x1EB, 0x1EC, 0x139, 0x1EE, \
		0x13A, 0x1F0, 0x13B, 0x13C, 0x1ED, 0x13D, 0x13E, 0x1EF, \
		0x13F, 0x140, 0x1F5, 0x21C, 0x1F6, 0x1F7, 0x1F8, 0x1F9, \
		0x1FA, 0x1FB, 0x1FC, 0x1FD, 0x1FE, 0x1FF, 0x200, 0x201, \
		0x202, 0x203, 0x204, 0x205, 0x206, 0x207, 0x208, 0x209, \
		0x20A, 0x20B, 0x20C, 0x20D, 0x20E, 0x20F, 0x210, 0x21D, \
		0x211, 0x212, 0x213, 0x214, 0x215, 0x21E, 0x216, 0x217, \
		0x218, 0x219, 0x21A, 0x385, 0x386, 0x387, 0x388, 0x389, \
		0x38A, 0x38B, 0x38C, 0x3A3, 0x38D, 0x38E, 0x38F, 0x390, \
		0x391, 0x392, 0x393, 0x394, 0x395, 0x396, 0x397, 0x399, \
		0x39A, 0x39B, 0x39E, 0x39C, 0x39D, 0x39F, 0x3A0, 0x58C, \
		0x58D, 0x58E, 0x58F, 0x590, 0x591, 0x592, 0x593, 0x594, \
		0x595, 0x596, 0x597, 0x598, 0x5B4, 0x599, 0x59A, 0x59B, \
		0x59C, 0x59D, 0x59E, 0x59F, 0x5A0, 0x5A1, 0x5A2, 0x5A3, \
		0x5A4, 0x5A5, 0x5A6, 0x5A7, 0x5A8, 0x5A9, 0x5AA, 0x5B0, \
		0x5AB, 0x5AC, 0x5AD, 0x5AE, 0x5AF, 0x2B7, 0x3A4, 0x3A5, \
		0x3E6, 0x3A6, 0x48D, 0x4D1, 0x3A7, 0x3A8, 0x48F, 0x3A9, \
		0x3AA, 0x3E3, 0x3B2, 0x3B3, 0x3B4, 0x3E2, 0x3B5, 0x3B6, \
		0x3E7, 0x3E1, 0x4D3, 0x3E4, 0x3E5, 0x5E6, 0x5E7, 0x5E8, \
		0x5E9, 0x5EA, 0x5EB, 0x5EC, 0x5ED, 0x5EE, 0x5EF, 0x5F0, 0x64B, \
		0x64C, 0x666, 0x667, 0x668, 0x669, 0x66A, 0x66B, 0x66C, 0x66D, \
		0x66E, 0x66F, 0x670, 0x671, 0x672, 0x673, 0x674, 0x675, 0x676, 0x677, 0x678, 0x679, \
		0x684,0x685,0x686,\
		0x606,0x607,0x608,0x609,0x60B,\
		0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0x4C8,0x4C9,0xEA,0xEB,\
		0xE1, 0xE2,0xED,0xEE,0xFC,0xFD,0xE4,0xE5,0x148,0x14E,0x6A6,0x6A7
	};
	return vMainTaskId.find(dwTaskId) != vMainTaskId.end();
}

BOOL CGameVariable::IsSpecialNpcTask(_In_ CONST TaskInfoParm& Ti) CONST
{
	CONST static std::vector<TaskInfoParm> SpecialNpcTaskVec = 
	{
		{ 0x653, 0x5 }, { 0x653, 0x6 }
	};
	return MyTools::CLPublic::Vec_find_if_Const(SpecialNpcTaskVec, [&Ti](CONST TaskInfoParm& Tip){ return Tip == Ti; }) != nullptr;
}

MyTools::CLLock& CGameVariable::GetVarLock() CONST
{
	static MyTools::CLLock Lock(L"GetVarLock");
	return Lock;
}

BOOL CGameVariable::Action_Set_GiftFilterName(_In_ std::function<VOID(std::set<std::wstring>&)> fn) CONST
{
	static std::set<std::wstring> set_;
	return GetVarLock().Access([&fn]{fn(set_);});
}

ULONGLONG& CGameVariable::GetSetSwitchChannelTick() CONST
{
	static ULONGLONG ulTick = NULL;
	return ulTick;
}

DWORD CGameVariable::GetEnvironmentValue() CONST
{
	return ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(人物基址) + 人物基址偏移3) + 0xC) + 0x4);
}

time_t CGameVariable::SystemTimeToTime_t(const SYSTEMTIME& st)
{
	tm temptm = { st.wSecond,
		st.wMinute,
		st.wHour,
		st.wDay,
		st.wMonth - 1,
		st.wYear - 1900,
		st.wDayOfWeek,
		0,
		0 };
	return mktime(&temptm);
}

SYSTEMTIME CGameVariable::ConvertTextToSysTime(_In_ CONST std::wstring& wsText)
{
	SYSTEMTIME SysTime = { 0 };
	std::vector<std::wstring> TextVec;

	// split text
	MyTools::CCharacter::Split(wsText, L"-", TextVec, Split_Option_RemoveEmptyEntries | Split_Option_KeepOnly);
	if (TextVec.size() != 6)
	{
		LOG_CF_E(L"日期格式转换失败,Text:%s", wsText.c_str());
		ZeroMemory(&SysTime, sizeof(SysTime));
		return SysTime;
	}

	SysTime.wYear = static_cast<WORD>(_wtoi(TextVec.at(0).c_str()));
	SysTime.wMonth = static_cast<WORD>(_wtoi(TextVec.at(1).c_str()));
	SysTime.wDay = static_cast<WORD>(_wtoi(TextVec.at(2).c_str()));
	SysTime.wHour = static_cast<WORD>(_wtoi(TextVec.at(3).c_str()));
	SysTime.wMinute = static_cast<WORD>(_wtoi(TextVec.at(4).c_str()));
	SysTime.wSecond = static_cast<WORD>(_wtoi(TextVec.at(5).c_str()));
	return SysTime;
}

CONST std::wstring CGameVariable::GetSpendHourForText(_In_ CONST SYSTEMTIME& SysTime1, _In_ CONST SYSTEMTIME& SysTime2)
{
	FILETIME FileTime1;
	FILETIME FileTime2;
	SystemTimeToFileTime(&SysTime1, &FileTime1);
	SystemTimeToFileTime(&SysTime2, &FileTime2);

	ULARGE_INTEGER t1, t2;
	memcpy(&t1, &FileTime1, sizeof(t1));
	memcpy(&t2, &FileTime2, sizeof(t2));

	auto diff = (t1.QuadPart < t2.QuadPart) ? (t2.QuadPart - t1.QuadPart) : (t1.QuadPart - t2.QuadPart);
	// Convert to Minute
	diff /= 10000000 * 60;

	WCHAR wszText[32] = { 0 };
	swprintf_s(wszText, L"%d小时%d分钟", static_cast<DWORD>(diff / 60), static_cast<DWORD>(diff % 60));
	return std::wstring(wszText);
}

DWORD CGameVariable::GetHookHeightAddr() CONST
{
	static DWORD dwAddr = NULL;
	if (dwAddr == NULL)
	{
		// 浮空.rtf
		dwAddr = MyTools::CLSearchBase::FindAddr("F30F584424??F30F11435C8B", 0x0A58C79E - 0x0A58C7A4, 0, L"bsengine_Shipping");
		if (dwAddr == NULL)
			LOG_MSG_CF(L"GetHookHeightAddr = NULL;");
	}
	return dwAddr;
}

DWORD CGameVariable::GetHookJmpAddr() CONST
{
	static DWORD dwAddr = NULL;
	if (dwAddr == NULL)
	{
		// 无限跳.rtf
		dwAddr = MyTools::CLSearchBase::FindAddr("3C0D74??85??0F84", 0x0A5F6457 - 0x0A5F6476, 0, L"bsengine_Shipping");
		if (dwAddr == NULL)
			LOG_MSG_CF(L"GetHookJmpAddr = NULL;");
	}
	return dwAddr;
}

VOID CGameVariable::SetForceBackToSwitchPlayer() CONST
{
	CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->GameCmd = em_GameCmd::em_GameCmd_Exit;
}

std::wstring& CGameVariable::CurrentScriptName() CONST
{
	static std::wstring wsScriptName;
	return wsScriptName;
}

CONST Share_Global_Warehouse* CGameVariable::GetWarehouseConfig() CONST
{
	for (UINT i = 0; i < CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.Warehouse.uCount; ++i)
	{
		if (MyTools::CCharacter::wstrcmp_my(CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->szPlayerName, CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.Warehouse.Warehouse[i].wszPlayerName))
		{
			return &CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.Warehouse.Warehouse[i];
		}
	}
	return nullptr;
}

CONST Share_Global_Warehouse* CGameVariable::GetWarehouseConfig(_In_ CONST std::wstring& wsPlayerName) CONST
{
	for (UINT i = 0; i < CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.Warehouse.uCount; ++i)
	{
		if (MyTools::CCharacter::wstrcmp_my(wsPlayerName.c_str(), CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.Warehouse.Warehouse[i].wszPlayerName))
		{
			return &CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.Warehouse.Warehouse[i];
		}
	}
	return nullptr;
}

CONST std::vector<CGameVariable::ClassesBossSkill>& CGameVariable::GetSummonerBossSkillVec() CONST
{
	static CONST std::vector<ClassesBossSkill> Vec =
	{
		//// 倒地技能
		//{ em_Skill_Type::em_Skill_Type_Person, Sum_SkillId_Floor_1 }, { em_Skill_Type::em_Skill_Type_Person, Sum_SkillId_Floor_2 },
		//{ em_Skill_Type::em_Skill_Type_Cat, Sum_SkillId_Floor_F }, { em_Skill_Type::em_Skill_Type_Cat, Sum_SkillId_Floor_C },
		//// 牵牛花, 板栗球
		//{ em_Skill_Type::em_Skill_Type_Person, Sum_SkillId_F }, { em_Skill_Type::em_Skill_Type_Person, Sum_SkillId_F_F },
		//// 隐身后的板栗球, 
		//{ em_Skill_Type::em_Skill_Type_Person, Sum_SkillId_F_4 }, { em_Skill_Type::em_Skill_Type_Cat, Sum_SkillId_C_C },
		//// 猫扑到
		//{ em_Skill_Type::em_Skill_Type_Cat, Sum_SkillId_Floor_C1 },
	};

	return Vec;
}

CONST std::vector<CGameVariable::ClassesBossSkill>& CGameVariable::GetWarlockBossSkillVec() CONST
{
	static CONST std::vector<ClassesBossSkill> Vec =
	{
		// 倒地技能
		//{ em_Skill_Type::em_Skill_Type_Person, Warlock_SkillId_Floor_1 }, { em_Skill_Type::em_Skill_Type_Person, Warlock_SkillId_Floor_2 },
		//{ em_Skill_Type::em_Skill_Type_Person, Warlock_SkillId_Floor_3 }, { em_Skill_Type::em_Skill_Type_Person, Warlock_SkillId_Floor_F },
	//	//  2 4
	//	{ em_Skill_Type::em_Skill_Type_Person, Warlock_SkillId_2 }, { em_Skill_Type::em_Skill_Type_Person, Warlock_SkillId_4_1 },
	//	// v 
	//	{ em_Skill_Type::em_Skill_Type_Person, Warlock_SkillId_V1 }, 
	//	// 召唤兽e和q
	//	{ em_Skill_Type::em_Skill_Type_Cat, Warlock_SkillId_E }, { em_Skill_Type::em_Skill_Type_Cat, Warlock_SkillId_Q },
	};
	return Vec;
}

VOID CGameVariable::SetStopGame() CONST
{
	CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->GameCmd = em_GameCmd::em_GameCmd_Stop;
}

VOID _Account_Info_GAME::Print()
{
	LOG_C(MyTools::CLog::em_Log_Type::em_Log_Type_Custome, L"----------AccountInfo---------------");
	LOG_C(MyTools::CLog::em_Log_Type::em_Log_Type_Custome, L"帐号=%s,密码=%s", MyAcount_Info.szUserName, MyAcount_Info.szPassWord);
	LOG_C(MyTools::CLog::em_Log_Type::em_Log_Type_Custome, L"服务器ID=%X,第几个角色=%d,角色名=%s", MyAcount_Info.dwServerId, MyAcount_Info.dwOrderIndex, MyAcount_Info.szPlayerName);
	LOG_C(MyTools::CLog::em_Log_Type::em_Log_Type_Custome, L"是否登录完毕=%s,卡号=%s", !MyAcount_Info.bLogin ? L"Yes" : L"No", MyAcount_Info.szCardNo);
	LOG_C(MyTools::CLog::em_Log_Type::em_Log_Type_Custome, L"GameCmd=%X,职业=%X", MyAcount_Info.GameCmd, MyAcount_Info.dwClasses);
	LOG_C(MyTools::CLog::em_Log_Type::em_Log_Type_Custome, L"----------AccountInfo_Game--------------");
	LOG_C(MyTools::CLog::em_Log_Type::em_Log_Type_Custome, L"进程ID=%X,窗口句柄=%X", dwPid, hGameWnd);
	LOG_C(MyTools::CLog::em_Log_Type::em_Log_Type_Custome, L"-----------------AccountRunStatus-----------");
	LOG_C(MyTools::CLog::em_Log_Type::em_Log_Type_Custome, L"是否刷完=%X,是否占用=%X,是否登录中=%X,是否需要强制关闭=%X", AccountStatus.bDelete, AccountStatus.bExist, AccountStatus.bLogining, AccountStatus.bClose);
	LOG_C(MyTools::CLog::em_Log_Type::em_Log_Type_Custome, L"关闭理由=%X, 是否登录过=%X,是否删除=%X,登录失败次数=%d", AccountStatus.emCloseResult, AccountStatus.bLogined, AccountStatus.bDelete, AccountStatus.uLoginFaildCount);
	LOG_C(MyTools::CLog::em_Log_Type::em_Log_Type_Custome, L"创建角色失败次数=%d,冻结=%X", AccountStatus.uCreatePlayerFaildCount, AccountStatus.bFrozen);
}