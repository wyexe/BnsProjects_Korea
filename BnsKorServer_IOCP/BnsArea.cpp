#include "BnsArea.h"
#include <MyTools/CLPublic.h>

BOOL CBnsArea::IsSame_Area(_In_ DWORD dwServerId1, _In_ DWORD dwServerId2) CONST
{
	BOOL bRetCode = FALSE;
	if (dwServerId1 == dwServerId2)
		return TRUE;

	for (CONST auto& itm : _VecArea)
	{
		auto p = MyTools::CLPublic::Vec_find_if_Const(itm, [&dwServerId1](CONST BnsServerInfo& BnsServerInfo_) {return BnsServerInfo_.dwServerId == dwServerId1; });
		if (p != nullptr)
		{
			return MyTools::CLPublic::Vec_find_if_Const(itm, static_cast<BnsServerInfo*>(nullptr), [&dwServerId2](CONST BnsServerInfo& BnsServerInfo_) {return dwServerId2 == BnsServerInfo_.dwServerId; });;
		}
	}
	return bRetCode;
}

VOID CBnsArea::Initialize()
{
	_VecArea.clear();
	_VecArea.push_back({ { L"3区", L"경국지색", 0x3 } });
	_VecArea.push_back({ { L"23区", L"절세미인", 0x2E } });
	_VecArea.push_back({ { L"21区", L"일확천금", 0x4 },{ L"27区", L"천하태평", 0x17 },{ L"31区", L"파죽지세", 0x10 } });
	_VecArea.push_back({ { L"18区", L"이심전심", 0x5 },{ L"4区", L"공명정대", 0x1A },{ L"7区", L"금상첨화", 0x19 } });
	_VecArea.push_back({ { L"13区", L"영웅호걸", 0x34 },{ L"8区", L"대도무문", 0x1F },{ L"20区", L"일취월장", 0x16 } });
	_VecArea.push_back({ { L"9区", L"명불허전", 0x1 },{ L"5区", L"군계일학", 0x7 },{ L"17区", L"외유내강", 0x14 } });
	_VecArea.push_back({ { L"16区", L"유아독존", 0x9 },{ L"26区", L"천하무적", 0x6 },{ L"28区", L"区쾌도난마", 0xF } });
	_VecArea.push_back({ { L"6区", L"금강불괴", 0x2 },{ L"10区", L"백전백승", 0xE },{ L"29区", L"태산북두", 0x15 } });
	_VecArea.push_back({ { L"22区", L"전광석화", 0x11 },{ L"30区", L"태평성대", 0x12 },{ L"1区", L"강호연파", 0x18 },{ L"24区", L"천양무궁", 0x1C } });
	_VecArea.push_back({ { L"11区", L"비룡재천", 0x8 },{ L"32区", L"혈풍혈우", 0xA },{ L"15区", L"용호상박", 0xB },{ L"19区", L"일도양단", 0xC } });
	_VecArea.push_back({ { L"2区", L"건곤일척", 0xD },{ L"14区", L"용맹정진", 0x13 },{ L"12区", L"신검합일", 0x1B },{ L"25区", L"천우신조", 0x1D } });
}
