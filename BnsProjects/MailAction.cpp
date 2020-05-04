#include "stdafx.h"
#include "MailAction.h"
#include <MyTools/Log.h>
#include "SomeFun.h"
#include "PersonFunction.h"
#include "NpcExtend.h"
#include "BagFunction.h"
#include "Person.h"
#include "GameUI.h"
#include "Npc.h"
#include "SomeFun.h"
#include "ObjectFunction.h"
#include "GameSend.h"
#include "BnsGameFun.h"
#include "ConsoleLog.h"

#define _SELF L"MailAction.cpp"

BOOL CMailAction::RecvEmptyMail(_In_ DWORD DeliveryPanelObj) CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&DeliveryPanelObj]{CGameSend::RecvEmptyMail(DeliveryPanelObj); });
}

DWORD CMailAction::GetMailCount() CONST
{
	DWORD dwAddr = ReadDWORD(CPerson::GetInstance().GetBase() + �ʼĲ���ƫ��);
	return (ReadDWORD(dwAddr + 0xC) - ReadDWORD(dwAddr + 0x8)) / 0x130;
}

DWORD CMailAction::GetMailParm(_In_ DWORD dwCompValue) CONST
{
	DWORD dwArrayHead = ReadDWORD(ReadDWORD(CPerson::GetInstance().GetBase() + �ʼĲ���ƫ��) + 0x8);
	DWORD dwMailCount = GetMailCount();

	for (DWORD i = 0; i < dwMailCount; ++i)
	{
		if (ReadDWORD(dwArrayHead + i * 0x130) == dwCompValue)
			return (DWORD)(dwArrayHead + i * 0x130);
	}
	return NULL;
}

BOOL CMailAction::SetRecvMailItem(_In_ DWORD DeliveryPanelObj) CONST
{
	DWORD dwMailCount = GetMailCount();
	dwMailCount = (dwMailCount >= 10 ? 10 : dwMailCount);

	auto fnSetMailItem = [](_In_ DWORD dwUiObj, _In_ DWORD dwIndexParm)
	{
		__try
		{
			_asm
			{
				PUSHAD;
				MOV ESI, dwUiObj;
				MOV EAX, ESI;
				ADD EAX, �ʼ�ƫ��3;
				PUSH EAX;

				MOV EAX, dwIndexParm;
				ADD EAX, �ʼ�ƫ��1;
				ADD ESI, EAX;

				MOV EAX, �������ʼ�CALL;
				CALL EAX;

				POPAD;
			}
			return TRUE;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			LOG_CF_E( L"fnSetMailItem�������쳣");
		}
		return FALSE;
	};

	for (DWORD i = 0; i < dwMailCount && BnsSleep(500); ++i)
	{
		std::wstring wsTitle;
		if (!GetMailTitle(i, wsTitle))
			continue;
		BOOL bJUDGE_Title = CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NMAE_JUDGE_MAIL_TITLE) == TRUE ? TRUE : FALSE;
		//LOG_CF_D( L"��׼��:%d", bJUDGE_Title);
		if (!bJUDGE_Title)
		{
			if (wsTitle != MAIL_TITLE_TEXT)
			{
				LOG_CF_D( L"�ʼ�������:%s, ����! ��׼��:%s", wsTitle.c_str(), MAIL_TITLE_TEXT);
				continue;
			}
		}
		


		DWORD dwIndex = dwMailCount - i - 1;
		//dwIndex += dwIndex * 8;
		//dwIndex = dwIndex << 4;
		dwIndex *= 0x98;
		LOG_CF_D(L"SetMail Title=%s, dwIndex=%X, i=%d", wsTitle.c_str(), dwIndex, i);
		CGameVariable::GetInstance().PushMainThreadFunPtr_synch([fnSetMailItem, DeliveryPanelObj, dwIndex]{fnSetMailItem(DeliveryPanelObj, dwIndex); });
	}

	return TRUE;
}

BOOL CMailAction::Mail(_In_ CONST std::wstring& wsNpcName, _In_ CONST std::vector<MailItemContent>& ItemStrList, _In_ DWORD dwSurplusMoney) CONST
{
	CNpc Npc;
	if (!CNpcExtend::GetInstance().FindNpc_By_Name(wsNpcName, CBaseObject::CreateFixPointInfo(30), &Npc))
	{
		CConsoleLog(MyTools::CCharacter::MakeFormatText(L"30m��Χ֮���Ҳ����ջ��ʼ�Npc:%s", wsNpcName.c_str()));
		return FALSE;
	}

	BnsMove(Npc.GetPoint(), 3.0f);
	BnsSleep(2 * 1000);

	std::vector<CBag> ItemList;
	DWORD dwMailMoney = CPerson::GetInstance().GetMoney() >= dwSurplusMoney ? CPerson::GetInstance().GetMoney() - dwSurplusMoney : 0;
	if (ItemList.size() == 0) // ��������Ʒ�������, ���������е�Ǯȫ��ת��
	{
		// ����=0.031
		dwMailMoney = CPerson::GetInstance().GetMoney() - dwSurplusMoney;
		dwMailMoney -= static_cast<DWORD>((static_cast<float>(dwMailMoney)* 0.031f));
		LOG_CF_D( L"û����Ʒ�������, ���ջ���������, ���ȥ�Ľ�Ǯ=%d, ������:%d", dwMailMoney, static_cast<DWORD>((static_cast<float>(CPerson::GetInstance().GetMoney() - dwSurplusMoney)* 0.031f)));
	}

	do
	{
		ItemList.clear();
		SetMailItemToList(ItemStrList, ItemList);
		LOG_CF_D( L"�ʼ���Ʒ����:%d, ���=%d", ItemList.size(), dwMailMoney);

		LPCWSTR pwszWarehouseName = CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.Mail.wszPlayerName[CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.Mail.uIndex];
		LOG_CF_D( L"pwszWarehouseName=%s", pwszWarehouseName);
		auto bRetCode = Npc.Mail(pwszWarehouseName, CPerson::GetInstance().GetMoney() > dwMailMoney ? dwMailMoney : 0, ItemList);
		LOG_CF_D( L"���ֿ��:%s �����ʼ�%s!", pwszWarehouseName, bRetCode ? L"�ɹ�" : L"ʧ��");
		if (!bRetCode)
		{
			CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.Mail.uIndex += 1;
			continue;
		}

		ItemList.clear();
		SetMailItemToList(ItemStrList, ItemList);
		if (ItemList.size() != 0)
		{
			LOG_CF_D( L"���ϻ����ʼĵ���Ʒû����, �����ʼ�!");
			continue;
		}

		break;
	} while (GameRun && !EMPTY_PERSONHP && CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.Mail.uIndex < CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.Mail.uCount);
	LOG_CF_D( L"�ʼ����!");
	CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.Mail.uIndex = 0;
	return TRUE;
}

BOOL CMailAction::MailItemSupport(_In_ CONST std::wstring& wsNpcName, _In_ CONST std::wstring& wsPlayerName, _In_ CONST std::vector<MailItemContent>& ItemStrList, _In_ DWORD dwMailMoney_) CONST
{
	CNpc Npc;
	if (!CNpcExtend::GetInstance().FindNpc_By_Name(wsNpcName, CBaseObject::CreateFixPointInfo(30), &Npc))
	{
		CConsoleLog(MyTools::CCharacter::MakeFormatText(L"30m��Χ֮���Ҳ��������ʼ�Npc:%s", wsNpcName.c_str()));
		return FALSE;
	}

	BnsMove(Npc.GetPoint(), 3.0f);
	BnsSleep(2 * 1000);

	std::vector<MailItemContent> VecItem = ItemStrList;
	LOG_CF_D( L"VecItem.size()=%d", VecItem.size());
	while (GameRun && !EMPTY_PERSONHP)
	{
		DWORD dwMailMoney = CPerson::GetInstance().GetMoney() > dwMailMoney_ ? dwMailMoney_ : 0;

		std::vector<CBag> ItemList;
		SetMailSupportItemToList(VecItem, ItemList);
		LOG_CF_D( L"�ʼ���Ʒ����:%d, ���=%d, Name=%s", ItemList.size(), dwMailMoney, wsPlayerName.c_str());

		auto bRetCode = Npc.Mail(wsPlayerName.c_str(), dwMailMoney, ItemList);
		LOG_CF_D( L"���ֿ��:%s �����ʼ�%s!", wsPlayerName.c_str(), bRetCode ? L"�ɹ�" : L"ʧ��");
		if (!bRetCode)
			return FALSE;

		if (VecItem.size() == 0)
			break;

		LOG_CF_D( L"���ϻ����ʼĵ���Ʒû����, �����ʼ�!");
	}

	
	return TRUE;
}

BOOL CMailAction::ExistMailItem(_In_ CONST std::vector<std::wstring>& ItemStrList) CONST
{
	for (const auto& itmstr : ItemStrList)
	{
		if (CBagFunction::GetInstance().ExistItem_By_Name(itmstr, (CBag *)nullptr))
			return TRUE;
	}
	return FALSE;
}


BOOL CMailAction::RecvMail() CONST
{
	DWORD dwMailCount = GetMailCount();
	if (dwMailCount == 0)
		return FALSE;
	LOG_CF_D( L"��ǰ�ʼ�����:%d", dwMailCount);

	//if (!CObjectFunction::GetInstance().IsShow_In_GameUi(L"DeliveryPanel"))
	//{
	//	CSomeFun::GetInstance().SimlationKey(VK_F6);
	//	BnsSleep(2000);
	//	return TRUE;
	//}

	CObjectFunction::GetInstance().Action_By_GameUiName(L"DeliveryPanel", [this](CONST CGameUi& DeliveryPanel)
	{
		CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&DeliveryPanel, this]{CGameSend::SetShowUi(DeliveryPanel.GetObj(),TRUE); });
		BnsSleep(2 * 1000);
	});

	CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"DeliveryPanel", [this](CONST CGameUi& DeliveryPanel)
	{
		SetRecvMailItem(DeliveryPanel.GetObj());
		CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&DeliveryPanel, this]{CGameSend::RecvAllMail(DeliveryPanel.GetObj()); });
		BnsSleep(2 * 1000);
	});
	//LOG_CF_D( L"1!");
	CSomeFun::GetInstance().SimlationKey('Y');
	BnsSleep(3 * 1000);
	while (GameRun && CObjectFunction::GetInstance().IsShow_In_GameUi(L"SystemProgressConfirmPanel"))
	{
		LOG_CF_D( L"���ʼ�ing!");
		CSomeFun::GetInstance().SimlationKey('Y');
		BnsSleep(3 * 1000);
	}
	//LOG_CF_D( L"2!");
	while (GameRun && CObjectFunction::GetInstance().IsShow_In_GameUi(L"DeliveryPanel"))
	{
		LOG_CF_D( L"�ر����ʼ�����!");
		/*CSomeFun::GetInstance().SimlationKey('Y', 200, 2 * 1000);
		CSomeFun::GetInstance().SimlationKey(VK_F6, 200, 2 * 1000);
		CSomeFun::GetInstance().SimlationKey(VK_RETURN, 200, 2 * 1000);*/
		CObjectFunction::GetInstance().Action_By_GameUiName(L"DeliveryPanel", [this](CONST CGameUi& DeliveryPanel)
		{
			CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&DeliveryPanel, this]{CGameSend::SetShowUi(DeliveryPanel.GetObj(), FALSE); });
			BnsSleep(2 * 1000);
		});
	}

	CObjectFunction::GetInstance().Action_By_GameUiName(L"DeliveryPanel", [this](CONST CGameUi& DeliveryPanel)
	{
		CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&DeliveryPanel, this]{CGameSend::SetShowUi(DeliveryPanel.GetObj(), FALSE); });
		BnsSleep(2 * 1000);
	});
	return CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"ChatInputPanel", [](CONST CGameUi&){CSomeFun::GetInstance().SimlationKey(VK_RETURN, 200, 1000); });
}

BOOL CMailAction::RecvUnOpenGift(_In_ CONST std::vector<std::wstring>& FilterNameVec) CONST
{
	std::vector<UnOpenGiftInfo> vlst;
	CObjectFunction::GetInstance().GetUnopenGiftList(vlst);

	auto fnExistFilterName = [&FilterNameVec](_In_ CONST UnOpenGiftInfo& itm)
	{
		for (CONST auto& ItmName : itm.vItem)
		{
			auto p = MyTools::CLPublic::Vec_find_if_Const(FilterNameVec, [&ItmName](CONST  std::wstring& Name){return Name == ItmName.ItemName; });
			if (p != nullptr)
				return TRUE;
		}
		return FALSE;
	};


	LOG_CF_D(L"һ����%d��δ����������!", vlst.size());
	for (CONST auto& itm : vlst)
	{
		if (GameRun && fnExistFilterName(itm))
		{
			LOG_CF_D( L"���ڴ�����, ������:%s", itm.wsTitle.c_str());
			CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&itm]{ CGameSend::OpenGiftBox(itm.dwParm); });
			BnsSleep(1000);
		}
	}
	return BnsSleep(5 * 1000);
}

BOOL CMailAction::RecvOpenGift(_In_ CONST std::vector<std::wstring>& FilterNameVec) CONST
{
	std::vector<OpenGiftInfo> vlst;
	CObjectFunction::GetInstance().GetGiftList(vlst);

	auto fnExistFilterName = [&FilterNameVec](_In_ CONST OpenGiftInfo& itm, _Out_opt_ OpenGiftItem& OpenGiftItem_)
	{
		for (CONST auto& ItmName : itm.vItem)
		{
			auto p = MyTools::CLPublic::Vec_find_if_Const(FilterNameVec, [&ItmName](CONST  std::wstring& Name){return Name == ItmName.ItemName; });
			if (p != nullptr)
			{
				OpenGiftItem_ = ItmName;
				return TRUE;
			}
		}
		return FALSE;
	};

	LOG_CF_D( L"һ����%d������������!", vlst.size());
	for (CONST auto& itm : vlst)
	{
		OpenGiftItem OpenGiftItem_;
		if (GameRun && fnExistFilterName(itm, OpenGiftItem_))
		{
			LOG_CF_D( L"���ڽ�������, ������:%s", itm.wsTitle.c_str());
			LOG_CF_D( L"RecvGift(%X,%X,%X,%X)", OpenGiftItem_.dwItemId, OpenGiftItem_.dwGiftParm, OpenGiftItem_.dwGiftID, itm.dwParm);
			CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&itm, &OpenGiftItem_]{ CGameSend::RecvGift(OpenGiftItem_.dwItemId, OpenGiftItem_.dwGiftParm, OpenGiftItem_.dwGiftID, itm.dwParm); });
			BnsSleep(5000);
		}
	}
	return TRUE;
}

BOOL CMailAction::GetMailTitle(_In_ DWORD dwIndex, _Out_opt_ std::wstring& wsTitle) CONST
{
	if (dwIndex >= GetMailCount())
	{
		return FALSE;
	}

	WCHAR wszText[32] = { 0 };
	DWORD dwNamePtr = ReadDWORD(ReadDWORD(CPerson::GetInstance().GetBase() + �ʼĲ���ƫ��) + 0x8) + dwIndex * 0x130 + 0x30;
	if (ReadDWORD(dwNamePtr + 0x14) >= 0xF)
		MyTools::CCharacter::wstrcpy_my(wszText, reinterpret_cast<CONST WCHAR*>(ReadDWORD(dwNamePtr)));
	else
		MyTools::CCharacter::wstrcpy_my(wszText, reinterpret_cast<CONST WCHAR*>(dwNamePtr));

	wsTitle = wszText;
	LOG_CF_D( L"�����ʼ�, ������:%s", wsTitle.c_str());
	return TRUE;
}

UINT CMailAction::SetMailSupportItemToList(_In_ std::vector<MailItemContent>& ItemStrList, _Out_opt_ std::vector<CBag>& VecBag) CONST
{
	std::vector<CBag> vBagLst;
	if (CObjectFunction::GetInstance().GetBagList(vBagLst, CObjectFunction::em_Bag_Type_Bag) == 0)
		return 0;

	for (int i = 0; i < static_cast<int>(ItemStrList.size()); ++i)
	{
		if (VecBag.size() == 3)
			break;

		for (auto& Item : vBagLst)
		{
			if (Item.GetName() == ItemStrList.at(i).wsItemName)
			{
				Item.SetMailCount(ItemStrList.at(i).dwCount);
				LOG_CF_D( L"����ʼ���Ʒ:%s, ����:%d", ItemStrList.at(i).wsItemName.c_str(), Item.GetMailCount());
				VecBag.push_back(Item);

				// ���������㹻�������ֱ��break
				if (ItemStrList.at(i).dwCount >= Item.GetMailCount())
					break;
			}
		}
		ItemStrList.erase(ItemStrList.begin() + i--);
	}
	return VecBag.size();
}

UINT CMailAction::SetMailItemToList(_In_ CONST std::vector<MailItemContent>& ItemStrList, _Out_opt_ std::vector<CBag>& VecBag) CONST
{
	std::vector<CBag> vBagLst;
	if (CObjectFunction::GetInstance().GetBagList(vBagLst, CObjectFunction::em_Bag_Type_Bag) == 0)
		return 0;

	for (int i = 0; i < static_cast<int>(ItemStrList.size()); ++i)
	{
		if (VecBag.size() == 3)
			break;

		for (auto& Item : vBagLst)
		{
			if (VecBag.size() == 3)
				break;

			if (Item.GetName() == ItemStrList.at(i).wsItemName)
			{
				Item.SetMailCount(ItemStrList.at(i).dwCount);
				LOG_CF_D( L"����ʼ���Ʒ:%s, ����:%d", ItemStrList.at(i).wsItemName.c_str(), Item.GetMailCount());
				VecBag.push_back(Item);
			}
		}

	}
	return VecBag.size();
}
