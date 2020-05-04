#include "stdafx.h"
#include "BagBase.h"
#include <MyTools/Log.h>
#include "SomeFun.h"
#include "BaseObject.h"
#include "GameUI.h"
#include "GameSend.h"
#include "PersonFunction.h"
#include "Person.h"
#define _SELF L"Bag.cpp"

CBag::CBag(DWORD dwBagBase, UINT uIndex) : m_dwBagBase(dwBagBase), m_uIndex(uIndex), uMailCount(NULL)
{
	
}

CBag::CBag() : m_dwBagBase(NULL), m_uIndex(NULL), uMailCount(NULL)
{
	
}

CBag::~CBag()
{

}

DWORD CBag::GetBase() CONST
{
	return m_dwBagBase;
}

DWORD CBag::GetGId() CONST
{
	return ReadDWORD(GetBase() + 0x20);
}

DWORD CBag::GetSId() CONST
{
	DWORD dwSId = ReadDWORD(GetBase() + 0x1C);
	return (((dwSId >> 0x10) & 0xFF) << 0x10) + (dwSId & 0xFF);
}

DWORD CBag::GetCount() CONST
{
	DWORD dwCount = ReadDWORD(GetBase() + 0x2E) & 0xFFFF;
	return (dwCount > 10000 || dwCount == 0) ? 1 : dwCount;
}

CONST std::wstring CBag::GetColorName() CONST
{
	struct _GameColorSrc
	{
		em_ItemColor emColor;
		WCHAR wszColorName[8];
	};

	const int nMaxColorLen = 7;
	static _GameColorSrc ColorArray[nMaxColorLen] = {
		em_ItemColor_Gray, L"灰色",
		em_ItemColor_White, L"白色",
		em_ItemColor_Green, L"绿色",
		em_ItemColor_Blue, L"蓝色",
		em_ItemColor_Pink, L"粉色",
		em_ItemColor_Gold, L"金色",
		em_ItemColor_Legend, L"传说",
	};

	em_ItemColor emColor = GetColor();

	for (int i = 0; i < nMaxColorLen; ++i)
	{
		if (ColorArray[i].emColor == emColor)
		{
			return std::wstring(ColorArray[i].wszColorName);
		}
	}

	return std::wstring(L"未知");
}

CBag::em_ItemColor CBag::GetColor() CONST
{
	return (em_ItemColor)ReadBYTE(ReadDWORD(GetBase() + 0xC) + 物品颜色偏移);
}


UINT CBag::GetIndex() CONST
{
	return m_uIndex;
}

BOOL CBag::Explain() CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([this]{CGameSend::ExplainItem(GetSId()); });
}

DWORD CBag::GetItemType_By_GId(_In_ DWORD dwGId)
{
	DWORD dwItemType = 0;
	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&dwItemType, dwGId]{dwItemType = CGameSend::GetItemType(dwGId); });
	return dwItemType;
}

DWORD CBag::GetItemType() CONST
{
	return GetItemType_By_GId(GetGId());
}


DWORD CBag::GetItemType_Dlg() CONST
{
	return CGameSend::GetItemType(GetGId());
}


BOOL CBag::UseItem() CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([this]{CGameSend::UseItem(GetSId()); });
}

CONST std::wstring CBag::GetTypeName() CONST
{
	std::wstring wsTypeName;
	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([this, &wsTypeName]{
		DWORD dwAttribute = ReadDWORD(物品右上角分类名字基址 + (8 << 0x6) + 0x28);
		WCHAR wszText[128] = { L"Name.item.game-category-3." };
		lstrcatW(wszText, (LPCWSTR)ReadDWORD(dwAttribute + (ReadDWORD(ReadDWORD(GetBase() + 0xC) + 0x26) & 0xFF) * 4));

		DWORD dwAddr = ReadDWORD(CGameSend::GetItemTypeNameAddr((DWORD)&wszText, ReadDWORD(ReadDWORD(名字返回基址) + 0x28)) + 0x18);
		if (dwAddr != NULL && ReadDWORD(dwAddr) != NULL)
			wsTypeName = (LPCWSTR)dwAddr;
	});

	return wsTypeName;
}


CONST std::wstring CBag::GetTypeName_Dlg() CONST
{
	std::wstring wsTypeName;

	DWORD dwAttribute = ReadDWORD(物品右上角分类名字基址 + (8 << 0x6) + 0x28);
	WCHAR wszText[128] = { L"Name.item.game-category-3." };
	lstrcatW(wszText, (LPCWSTR)ReadDWORD(dwAttribute + (ReadDWORD(ReadDWORD(GetBase() + 0xC) + 0x26) & 0xFF) * 4));

	auto fnGetBagAddr = [](DWORD dwParm1, DWORD dwParm2)
	{
		__try
		{
			DWORD dwValue = 0;
			_asm
			{
				PUSHAD;
				PUSH dwParm2;
				PUSH dwParm1;

				MOV EAX, 名字返回基址;
				MOV EAX, DWORD PTR DS : [EAX];
				MOV ECX, DWORD PTR DS : [EAX + 名字返回偏移1];
				MOV EDX, DWORD PTR DS : [ECX];
				MOV EDX, DWORD PTR DS : [EDX + 名字返回偏移2 - 0x4];
				CALL EDX;
				MOV dwValue, EAX;
				POPAD;
			}
			return dwValue;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			LOG_CF_E( L"GetObjectNameAddr发生了异常");
		}
		return (DWORD)0;
	};

	DWORD dwAddr = ReadDWORD(fnGetBagAddr((DWORD)&wszText, ReadDWORD(ReadDWORD(名字返回基址) + 0x28)) + 0x18);
	if (dwAddr != NULL)
		wsTypeName = (LPCWSTR)dwAddr;

	return wsTypeName;
}


BOOL CBag::OpenBox() CONST
{
	// Set Force Open the Box
	DWORD dwAddr = ReadDWORD(CPerson::GetInstance().GetBase() + 背包遍历偏移) + 强制开箱子偏移;
	MyTools::CCharacter::WriteBYTE(dwAddr, 0x0);

	BOOL bSpecail = CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_OPENBOX_SPECIAL) != NULL ? TRUE : FALSE;
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([this, bSpecail]{CGameSend::OpenBox(GetSId(), bSpecail); });
}

BOOL CBag::EquiEnhanced(_In_ DWORD dwGrowthItemSId, _In_ DWORD dwLuckyCharmSId /* = NULL */) CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([this, &dwGrowthItemSId, &dwLuckyCharmSId]{CGameSend::ItemGrowth(dwGrowthItemSId, GetSId(), dwLuckyCharmSId); });
}

BOOL CBag::EquiEvolution(_In_ CONST CBag& Weaspon, _In_ DWORD dwEvolutionId) CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([=]
	{
		CGameSend::ItemEvolution(Weaspon.GetSId(), Weaspon.GetItemType(), GetGId(), dwEvolutionId);
	});
}

BOOL CBag::EquiEndEvolution(_In_ DWORD dwGrowthItemSId, _In_ DWORD dwEvoltionId) CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([=]
	{
		CGameSend::ItemEndEvolution(GetSId(), dwGrowthItemSId, dwEvoltionId);
	});
}

BOOL CBag::EquiUntie(_In_ DWORD dwUntieId, _In_ DWORD dwUntieCount, _In_ DWORD nType) CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([=]
	{
		CGameSend::UnsealItem(GetSId(), dwUntieId, dwUntieCount,nType);
	});
}

BOOL CBag::OpenBox_By_Key(_In_ DWORD dwKeyId, _In_ DWORD dwKeyCount) CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([=]
	{
		CGameSend::OpenBox_By_Key(FALSE, dwKeyId, dwKeyCount, GetSId());
	});
}

BOOL CBag::WearEqui() CONST
{
	DWORD dwItemTypeId = GetItemType();
	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([=]
	{
		CGameSend::WearEqui(dwItemTypeId, GetSId());
	});

	BnsSleep(2000);
	CPersonFunction::GetInstance().WaitForProgressBar(5 * 1000);
	return BnsSleep(1000);
}

BOOL CBag::DiscardItem() CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([this]
	{
		CGameSend::DiscardItem(GetSId(), GetCount());
	});
}

BOOL CBag::SetGem(_In_ UINT uIndex, _In_ CONST CGameUi& Inventory2Panel) CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([=]
	{
		CGameSend::SetGem(GetSId(), uIndex, Inventory2Panel.GetObj());
	});
}

BOOL CBag::PushTransactionItem(_In_ DWORD TradeShelfPanelObj, _In_ DWORD dwCount) CONST
{
	if (dwCount > GetCount())
		return FALSE;

	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([=]
	{
		CGameSend::PushTransactionItem(TradeShelfPanelObj, GetSId(), dwCount);
	});
}

std::wstring CBag::GetItemName(_In_ DWORD dwItemObj) // static
{
	WCHAR wszName[MAX_OBJECT_NAME_LEN] = { 0 };
	if (dwItemObj != NULL)
	{
		DWORD dwValue = ReadDWORD(CBaseObject::GetObjectNameAddr(ReadDWORD(dwItemObj + 背包返回名字参数偏移1), ReadDWORD(dwItemObj + 背包返回名字参数偏移2)) + 0x18);
		if (dwValue != NULL && ReadDWORD(dwValue) != NULL)
			MyTools::CCharacter::wstrcpy_my(wszName, (LPCWSTR)dwValue, _countof(wszName) - 1);
	}
	return std::wstring(wszName);
}

DWORD CBag::GetItemObj_By_ID(_In_ DWORD dwItemId) // static
{
	DWORD dwItemObj = 0;
	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&]
	{
		dwItemObj = CGameSend::GetItemObj_By_ID(dwItemId);
	});
	
	return dwItemObj;
}

BOOL CBag::RemoveGem_By_Index(_In_ int nIndex) // static
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([nIndex]{CGameSend::RelieveGem(nIndex); });
}

CONST std::wstring& CBag::GetName() CONST
{
	return Name;
}

VOID CBag::SetName() CONST
{
	Name = GetItemName(ReadDWORD(GetBase() + 0xC));
}

BOOL CBag::RelieveEqui() CONST
{
	DWORD dwItemType = GetItemType();
	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&]{CGameSend::RelieveEqui(dwItemType); });
	BnsSleep(2000);
	CPersonFunction::GetInstance().WaitForProgressBar(5 * 1000);
	return BnsSleep(1000);
}

BOOL CBag::IsEquiment() CONST
{
	if (GetItemType() != NULL)
		return TRUE;

	return GetName().find(Item_封印的) != -1;
}

UINT CBag::GetMailCount() CONST
{
	return uMailCount;
}

VOID CBag::SetMailCount(_In_ UINT uMailCount_)
{
	if (uMailCount_ == 0)
		uMailCount = GetCount();
	else if (uMailCount_ < GetCount())
		uMailCount = uMailCount_;
	else
		uMailCount = GetCount();
}

BOOL CBag::IsGrowthed() CONST
{
	return GetExtendBaGuaAttributeId() != NULL;
}

BOOL CBag::GrowthBaGua(_In_ DWORD dwSacrificeSid) CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([this, dwSacrificeSid]{ CGameSend::StrengthenBaGua(GetSId(), dwSacrificeSid); });
}

DWORD CBag::GetExtendBaGuaAttributeId() CONST
{
	return ReadDWORD(GetBase() + 物品八卦ID偏移) & 0xFFFF;
}
