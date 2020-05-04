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
	// ��ǰ�����ڸ�������
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
			LOG_MSG_CF(L"�������Զ����µ�ַʧЧ��!");
			return 0;
		}

		dwOffset = MyTools::CLSearchBase::FindBase("8B0D????????8A??????????33??3B", 0x08E20606 - 0x08E2060C, 0x2, 0, L"bsengine_Shipping");
		if (dwOffset == NULL)
		{
			LOG_MSG_CF( L"�������Զ����µ�ַƫ��ʧЧ��!");
			return 0;
		}
		//LOG_CF_D( L"GameChart:[[%X]+%X]", dwBase, dwOffset);
	}
	return ReadDWORD(ReadDWORD(dwBase) + dwOffset);
}

CONST CGameVariable::CityInfo* CGameVariable::FindInstanceMapIdList(_In_ DWORD dwInstanceMapId) CONST
{
	CONST static std::vector<CityInfo> vInstanceMapId = {
		// ������
		{ L"�ݾ�", 0xAA3 }, { L"ɽ����Ѩ", 0xA39 }, { L"����կ���", 0xA2A }, { L"�����_ƽ�Ͻ���ǰ�����", 0x834 }, { L"�򿪵ķ�Ĺ", 0xA29 }, { L"��Ⱦ����", 0xA46 }, { L"���¿ӵ�", 0xA2C },
		{ L"��ҹɭ��", 0xA2B }, { L"���ܼӹ���", 0xA3C }, { L"���صĻ�ħʥ��", 0x1150 }, { L"��ħ��", 0xA2E }, { L"��Ĭ�ĺ�����", 0x128A }, { L"��ս�ĺ�ɭ��", 0x1282 }, { L"�������ֿ�", 0xA30 },
		{ L"����ʴ�ĺ�ǹ�岿��", 0x1770 }, { L"��������", 0x92F }, { L"���¾ݵ�", 0xA2F }, { L"��������", 0xA3A }, { L"����կӪ��", 0xA35 }, { L"���Ҷ�", 0x993 }, { L"������Ѩ", 0xA37 }, { L"�����ƽ�Ͻ���֧��BOSS", 0x939 },
		// ��Į
		{ L"��������", 0xBF4 }, { L"��������", 0xC06 }, { L"������", 0xC24 }, { L"��Ӱ�ݲֿ�", 0xBFF }, { L"�ɽٽ�������ֶ�", 0xBBF }, { L"������ַ", 0xA3E }, { L"���Ծ�������", 0xBF7 }, { L"��ɳ��", 0xC00 },
		{ L"��ɳ��", 0xC00 }, { L"������Ժ", 0xC07 }, { L"��ң�����䳡", 0xBF9 }, { L"������", 0xBC7 }, { L"��Ӱʯ��", 0xC08 }, { L"��ħ������", 0xBFD }, { L"����", 0xC04 }, { L"��ң�ɶ�Ѩ", 0xBFE },
		{ L"�ֿ�", 0xC16 }, { L"����Ĺ��", 0xC21 }, { L"���涴", 0xC05 }, { L"��ӰҪ��", 0xC08 }, { L"����֮��", 0xC18 }, { L"�߹��ų�Ѩ", 0xC02 }, { L"���岿��", 0xC03 }, { L"���޲���ֲ԰", 0x8FA },
		{ L"��ǹ����᳡", 0xC09 }, { L"�����̳", 0xBFB }, { L"��Ҥ��", 0xBD4 }, { L"��Ӱ���", 0xC0A }, { L"ʧ�Ķ�", 0xBFC },
		// ˮ��ƽԭ
		{ L"�����˶�������", 0x119E }, { L"�۹�����������", 0x1132 }, { L"�������ż�", 0x1199 }, { L"���������񷿼�", 0x135F },
		{ L"��ʯ��ɽ", 0x1144 }, { L"�����", 0x1149 }, { L"���鶴��", 0x1194 }, { L"�߹������쳧", 0x11A8 }, { L"���޶�Ѩ", 0x11AD }, { L"������Ӫ��", 0x11B2 }, { L"��ս����", 0x11B7 }, { L"���²ֿ�", 0x11E9 },
		{ L"�������Ѵ�", 0x11EE }, { L"֩�볲Ѩ", 0x11F3 }, { L"���޹ؿ�����", 0x10D4 }, { L"���޹ؿ�����", 0x11DF }, { L"ˮ�ܹ�", 0x11DA }, { L"���㷻�ֿ�", 0x11D5 }, { L"�����峲Ѩ", 0x11D0 }, { L"�۹��������", 0x11BD },
		{ L"Ѫ���峲Ѩ", 0x11BD }, { L"�����о���", 0x11C7 }, { L"ʯ�ֶ���", 0x11CB }, { L"ҹĻ����Ѫ������", 0x1277 }, { L"���纣�߲�������", 0x1275 }, { L"���߲�������", 0x1260 }, { L"�����Թ�", 0x1271 }, { L"Ѫ������", 0x1268 },
		// ����ɽ��
		{ L"����ﶴѨ", 0x15A4 }, { L"��ħ��", 0x13E9 }, { L"������Ѩ", 0x157F }, { L"������̳", 0x142B }, { L"����", 0x1679 }, { L"����", 0x167C }, { L"��Ŀȫ�ǵĽ�����", 0x15F6 }, { L"��Ŀȫ�ǵĽ�����", 0x15E1 },
		{ L"����Ĺ��", 0x1725 }, { L"�����Թ�", 0x171D }, { L"ѩ��", 0x170F }, { L"����", 0x127C }, { L"�����̳", 0x14B0 }, { L"ѩ�˶�", 0x1717 }, { L"���Źص�ج��", 0x1714 }, { L"ҹĻ��֮ƽԭ", 0x1581 },
		{ L"��������", 0x1491 }, { L"�ݷ�Ͽ��", 0x1482 }, { L"����Ժ", 0x148C }, { L"ѩŮ��", 0x149C }, { L"ѩ�ޱ���", 0x14A0 }, { L"����������", 0x14A3 }, { L"��Ժ", 0x15FF },
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
		{ L"���շ�", L"���շ�", 0x4D },
		// ������
		{ L"������", L"���ִ�", 0x1 }, { L"������", L"��ʬ��", 0x2 }, { L"������", L"����������", 0x45 }, { L"������", L"ɭ��Ӫ��", 0x32 }, { L"������", L"����Ⱦ�����", 0x3 },
		{ L"������", L"̿Ҥ��", 0x4A }, { L"������", L"������", 0x5 }, { L"������", L"���ܺ�", 0x67 }, { L"������", L"������é��", 0x6 }, { L"������", L"������Ӫ��", 0x7 },
		{ L"������", L"����ͤ", 0x35 }, { L"������", L"������", 0x8 }, { L"������", L"������Ѩ���", 0x36 }, { L"������", L"������վ", 0x9 }, { L"������", L"�����ƽ�Ͻ���", 0x34 },
		// ��Į
		{ L"��Į", L"�´�", 0xB }, { L"��Į", L"��Ԫ�ɵ�������", 0xA }, { L"��Į", L"��Ů��", 0xC }, { L"��Į", L"���������", 0x38 }, { L"��Į", L"����Ȫ1", 0xD },
		{ L"��Į", L"����Ȫ2", 0x39 }, { L"��Į", L"��Ҵ�", 0x11 }, { L"��Į", L"���Ծ���Ӫ", 0x12 }, { L"��Į", L"����Ӫ��", 0x13 }, { L"��Į", L"���º�", 0x14 },
		{ L"��Į", L"���ſ�ջ", 0x16 }, { L"��Į", L"���߶ɿ�", 0x10 }, { L"��Į", L"������", 0x15 }, { L"��Į", L"�����ߵķ���é��", 0x17 }, { L"��Į", L"�մɷ��ܶ�", 0x18 },
		{ L"��Į", L"���Ծ�ָ����", 0x19 }, { L"��Į", L"���Ծ��ַ���", 0x1B }, { L"��Į", L"������", 0x1C }, { L"��Į", L"���Ծ����ƶ�", 0x1A }, { L"��Į", L"���Ǵ�", 0x1D },
		{ L"��Į", L"����é��", 0x3B }, { L"��Į", L"���Ծ�����", 0x1E }, { L"��Į", L"��Ĺ�ߺ���", 0x1F }, { L"��Į", L"����Ĺ�����", 0x3A }, { L"��Į", L"���Ǹ���", 0x22 }, { L"��Į", L"ѧ����", 0x37 },
		// ˮ��ƽԭ
		{ L"ˮ��ƽԭ", L"�������", 0x26 }, { L"ˮ��ƽԭ", L"����ɭ���ͱ���", 0x27 }, { L"ˮ��ƽԭ", L"???", 0x28 }, { L"ˮ��ƽԭ", L"����ɭ�ֲ���", 0x29 }, { L"ˮ��ƽԭ", L"����Ӫ��", 0x43 },
		{ L"ˮ��ƽԭ", L"������", 0x24 }, { L"ˮ��ƽԭ", L"������վ", 0x23 }, { L"ˮ��ƽԭ", L"��ս����Ժ���", 0x44 }, { L"ˮ��ƽԭ", L"�����Ͻ�Ӫ��", 0x2C }, { L"ˮ��ƽԭ", L"����Ӫ��", 0x41 },
		{ L"ˮ��ƽԭ", L"̫��������", 0x42 }, { L"ˮ��ƽԭ", L"��ب����", 0x2D }, { L"ˮ��ƽԭ", L"ˮ���ܷ���", 0x3D }, { L"ˮ��ƽԭ", L"���㷻�ܶ�", 0x2A }, { L"ˮ��ƽԭ", L"���㷻����", 0x3C },
		{ L"ˮ��ƽԭ", L"�糵ĥ��", 0x3E }, { L"ˮ��ƽԭ", L"��ǹȹ���", 0x25 }, { L"ˮ��ƽԭ", L"������ջ", 0x3F }, { L"ˮ��ƽԭ", L"�����ƽ������֧��", 0x40 }, { L"ˮ��ƽԭ", L"�ϲ��ɿ�", 0x4B }, { L"ˮ��ƽԭ", L"������", 0x4C },
		//����ɽ��-��֮ƽԭ
		{ L"��֮ƽԭ", L"������ׯ", 0x63 }, { L"��֮ƽԭ", L"�۹���������", 0x62 }, { L"��֮ƽԭ", L"���ֵ�פ�����", 0x53 }, { L"��֮ƽԭ", L"���մ�", 0x51 }, { L"��֮ƽԭ", L"��ħ�����", 0x78 },
		{ L"��֮ƽԭ", L"Ⱦ��", 0x52 }, { L"��֮ƽԭ", L"������Ѩ", 0x73 }, { L"��֮ƽԭ", L"�ʺ��", 0x77 },
		//����ɽ��-������
		{ L"������", L"�����Ǿݵ�", 0x7A }, { L"������", L"��������", 0x6E }, { L"������", L"���ſ�ջ", 0x5E }, { L"������", L"�׶�������", 0x72 }, { L"������", L"���¹�", 0x5F }, { L"������", L"��ׯ԰", 0x6D },
		{ L"������", L"���ͥԺ", 0x6C }, { L"������", L"������", 0x71 }, { L"������", L"��ʷ��", 0x7C }, { L"������", L"������", 0x7B }, { L"������", L"����", 0x76 }, { L"������", L"ѩ�չ�", 0x65 },
		{ L"������", L"�ʹ����䳡", 0x70 }, { L"������", L"���¥", 0x66 }, { L"������", L"���Ժ", 0x6F }, { L"������", L"����ƹ�", 0x79 }, { L"������", L"������̳", 0x7D },
		//����ɽ��-������
		{ L"������", L"������", 0x54 }, { L"������", L"��ľ��", 0x55 }, { L"������", L"�����ַ���", 0x56 }, { L"������", L"��������Ȫ", 0x64 },
		//����ɽ��-����ѩԭ
		{ L"����ѩԭ", L"ѩŮ�����", 0x6A }, { L"����ѩԭ", L"ҩ�ݹ��ܶ�", 0x57 }, { L"����ѩԭ", L"ҩ�ݹ�����Ϣ��", 0x58 }, { L"����ѩԭ", L"ð�ռҵ�ҰӪ��", 0x59 }, { L"����ѩԭ", L"ѩ�ޱ������", 0x6B },
		{ L"����ѩԭ", L"����������", 0x5A }, { L"����ѩԭ", L"�����̲������", 0x69 }, { L"����ѩԭ", L"��ѩ��", 0x5B }, { L"����ѩԭ", L"�����ɱ�ɽ", 0x61 }, { L"����ѩԭ", L"ð�ռҵı�����", 0x5D },
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
	return ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(�����ַ) + �����ַƫ��3) + 0xC) + 0x4);
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
		LOG_CF_E(L"���ڸ�ʽת��ʧ��,Text:%s", wsText.c_str());
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
	swprintf_s(wszText, L"%dСʱ%d����", static_cast<DWORD>(diff / 60), static_cast<DWORD>(diff % 60));
	return std::wstring(wszText);
}

DWORD CGameVariable::GetHookHeightAddr() CONST
{
	static DWORD dwAddr = NULL;
	if (dwAddr == NULL)
	{
		// ����.rtf
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
		// ������.rtf
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
		//// ���ؼ���
		//{ em_Skill_Type::em_Skill_Type_Person, Sum_SkillId_Floor_1 }, { em_Skill_Type::em_Skill_Type_Person, Sum_SkillId_Floor_2 },
		//{ em_Skill_Type::em_Skill_Type_Cat, Sum_SkillId_Floor_F }, { em_Skill_Type::em_Skill_Type_Cat, Sum_SkillId_Floor_C },
		//// ǣţ��, ������
		//{ em_Skill_Type::em_Skill_Type_Person, Sum_SkillId_F }, { em_Skill_Type::em_Skill_Type_Person, Sum_SkillId_F_F },
		//// �����İ�����, 
		//{ em_Skill_Type::em_Skill_Type_Person, Sum_SkillId_F_4 }, { em_Skill_Type::em_Skill_Type_Cat, Sum_SkillId_C_C },
		//// è�˵�
		//{ em_Skill_Type::em_Skill_Type_Cat, Sum_SkillId_Floor_C1 },
	};

	return Vec;
}

CONST std::vector<CGameVariable::ClassesBossSkill>& CGameVariable::GetWarlockBossSkillVec() CONST
{
	static CONST std::vector<ClassesBossSkill> Vec =
	{
		// ���ؼ���
		//{ em_Skill_Type::em_Skill_Type_Person, Warlock_SkillId_Floor_1 }, { em_Skill_Type::em_Skill_Type_Person, Warlock_SkillId_Floor_2 },
		//{ em_Skill_Type::em_Skill_Type_Person, Warlock_SkillId_Floor_3 }, { em_Skill_Type::em_Skill_Type_Person, Warlock_SkillId_Floor_F },
	//	//  2 4
	//	{ em_Skill_Type::em_Skill_Type_Person, Warlock_SkillId_2 }, { em_Skill_Type::em_Skill_Type_Person, Warlock_SkillId_4_1 },
	//	// v 
	//	{ em_Skill_Type::em_Skill_Type_Person, Warlock_SkillId_V1 }, 
	//	// �ٻ���e��q
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
	LOG_C(MyTools::CLog::em_Log_Type::em_Log_Type_Custome, L"�ʺ�=%s,����=%s", MyAcount_Info.szUserName, MyAcount_Info.szPassWord);
	LOG_C(MyTools::CLog::em_Log_Type::em_Log_Type_Custome, L"������ID=%X,�ڼ�����ɫ=%d,��ɫ��=%s", MyAcount_Info.dwServerId, MyAcount_Info.dwOrderIndex, MyAcount_Info.szPlayerName);
	LOG_C(MyTools::CLog::em_Log_Type::em_Log_Type_Custome, L"�Ƿ��¼���=%s,����=%s", !MyAcount_Info.bLogin ? L"Yes" : L"No", MyAcount_Info.szCardNo);
	LOG_C(MyTools::CLog::em_Log_Type::em_Log_Type_Custome, L"GameCmd=%X,ְҵ=%X", MyAcount_Info.GameCmd, MyAcount_Info.dwClasses);
	LOG_C(MyTools::CLog::em_Log_Type::em_Log_Type_Custome, L"----------AccountInfo_Game--------------");
	LOG_C(MyTools::CLog::em_Log_Type::em_Log_Type_Custome, L"����ID=%X,���ھ��=%X", dwPid, hGameWnd);
	LOG_C(MyTools::CLog::em_Log_Type::em_Log_Type_Custome, L"-----------------AccountRunStatus-----------");
	LOG_C(MyTools::CLog::em_Log_Type::em_Log_Type_Custome, L"�Ƿ�ˢ��=%X,�Ƿ�ռ��=%X,�Ƿ��¼��=%X,�Ƿ���Ҫǿ�ƹر�=%X", AccountStatus.bDelete, AccountStatus.bExist, AccountStatus.bLogining, AccountStatus.bClose);
	LOG_C(MyTools::CLog::em_Log_Type::em_Log_Type_Custome, L"�ر�����=%X, �Ƿ��¼��=%X,�Ƿ�ɾ��=%X,��¼ʧ�ܴ���=%d", AccountStatus.emCloseResult, AccountStatus.bLogined, AccountStatus.bDelete, AccountStatus.uLoginFaildCount);
	LOG_C(MyTools::CLog::em_Log_Type::em_Log_Type_Custome, L"������ɫʧ�ܴ���=%d,����=%X", AccountStatus.uCreatePlayerFaildCount, AccountStatus.bFrozen);
}