#include "stdafx.h"
#include "Npc.h"
#include <MyTools/Log.h>
#include "Person.h"
#include "SomeFun.h"
#include "BagBase.h"
#include "GameSend.h"

#define _SELF L"Npc.cpp"

CNpc::CNpc(_In_ DWORD dwNpcBase) : CPlayer(dwNpcBase)
{
	
}

CNpc::CNpc()
{

}

CNpc::~CNpc()
{

}

DWORD CNpc::GetNpcType() CONST
{
	return ReadDWORD(GetBase() + 0xE) & 0xFFFF;
}

CONST std::wstring& CNpc::GetName() CONST
{
	return Name;
	
}

VOID CNpc::SetName() CONST
{
	WCHAR wszText[MAX_OBJECT_NAME_LEN] = { 0 };
	DWORD dwAddr = ReadDWORD(GetBase() + 怪物属性偏移);
	DWORD dwValue = ReadDWORD(GetObjectNameAddr(ReadDWORD(dwAddr + 0x1C), ReadDWORD(dwAddr + 0x20)) + 0x18);
	if (dwValue != NULL && ReadDWORD(dwValue) != NULL)
		MyTools::CCharacter::wstrcpy_my(wszText, (LPCWSTR)dwValue, _countof(wszText) - 1);

	Name = wszText;
}

bool CNpc::operator<(__in const CNpc& GameObj) CONST
{
	return this->SortDis() < GameObj.SortDis();
}

BOOL CNpc::Mail(_In_ LPCWSTR pwszPlayerName, _In_ DWORD dwMoney, _In_ std::vector<CBag>& vlst) CONST
{
	auto fnMAIL_TEXT_GAME = [](__in LPCWSTR pwszText, __out MAIL_TEXT_GAME &wstr)
	{
		int length = wcslen(pwszText);
		if (length > 7)
		{
			wstr.pwStr = new wchar_t[length + 1]; // 懒得释放了, 反正就一丢丢内存
			wcscpy_s(wstr.pwStr, length + 1, pwszText);
		}
		else
			wcscpy_s((wstr.wchArr), length + 1, pwszText);

		wstr.count = length;
		wstr.size = length > 0x7 ? 0xF : 0x7;
	};

	if (dwMoney == 0 && vlst.size() == 0)
	{
		LOG_CF_D( L"没邮寄钱和物品, return!");
		return TRUE;
	}
	
	// 
	MailParm_Src _MailParm;
	ZeroMemory(&_MailParm, sizeof(_MailParm));

	// Text
	fnMAIL_TEXT_GAME(pwszPlayerName, _MailParm.Player);
	fnMAIL_TEXT_GAME(MAIL_TITLE_TEXT, _MailParm.Title);
	fnMAIL_TEXT_GAME(L"", _MailParm.Content);

	// Item
	for (auto& Item : vlst)
	{
		MailItem Mi;

		Mi.dwId = Item.GetSId();
		Mi.dwCount = Item.GetMailCount();
		_MailParm.vlst.push_back(Mi);
	}
	// 
	_MailParm.dwMoney = dwMoney;
	_MailParm.dwParm = 0x1;

	DWORD dwLastMoney = CPerson::GetInstance().GetMoney();

	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([this, &_MailParm]{
		CGameSend::Mail(GetId(), GetIdParm(), &_MailParm);
	});

	auto fnDeleteMem = [](__in MAIL_TEXT_GAME &wstr)
	{
		if (wstr.count > 7)
			delete [] wstr.pwStr;
	};

	::Sleep(5000);
	fnDeleteMem(_MailParm.Content);
	fnDeleteMem(_MailParm.Title);
	return dwLastMoney != CPerson::GetInstance().GetMoney();
}

BOOL CNpc::BuyItem(_In_ DWORD dwItemIndex/*从0开始*/, _In_ DWORD dwCount) CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([this, &dwItemIndex, &dwCount]{CGameSend::BuyItem_By_Npc(GetId(), GetIdParm(), dwCount, dwItemIndex); });
}

BOOL CNpc::IsExist() CONST
{
	if (GetMaxHp() == 0 || GetPoint().IsZero())
		return FALSE;
	return TRUE;
}

BOOL CNpc::Sell(_In_ CONST CBag& Item) CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([this, &Item]{CGameSend::SellItem_By_Npc(Item.GetSId(), Item.GetCount(), GetId(), GetIdParm()); });
}

BOOL CNpc::ExpandInvenSlot() CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([this]{CGameSend::ExpandInvenSlot(GetId(), GetIdParm(), 0x3); });
}

BOOL CNpc::ExpandClothSlot() CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([this]{CGameSend::ExpandInvenSlot(GetId(), GetIdParm(), 0x6); });
}

BOOL CNpc::SaveItem(_In_ DWORD dwItemSID, _In_ DWORD dwCount, _In_ DWORD dwWareHourseSID) CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([this, &dwItemSID, &dwCount, &dwWareHourseSID]{CGameSend::SaveItem(GetId(), GetIdParm(), dwItemSID, dwCount, dwWareHourseSID); });
}

BOOL CNpc::TakeNpcItem() CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([this]{CGameSend::TakeItem_By_Npc(GetId(), GetIdParm()); });
}

BOOL CNpc::RecvMail(_In_ DWORD dwDeliveryPanelObj, _In_ DWORD dwMailParm) CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([this, &dwDeliveryPanelObj, &dwMailParm]{CGameSend::RecvMail(GetId(), GetIdParm(), dwDeliveryPanelObj, dwMailParm); });
}

