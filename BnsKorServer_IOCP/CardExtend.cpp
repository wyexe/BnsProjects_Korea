#include "CardExtend.h"
#include "DbManager.h"
#include <iostream>
#include <MyTools/Character.h>
#include <MyTools/CLPublic.h>
#include <MyTools/Log.h>
#include <MyTools/CLEchoException.h>

#define _SELF L"CardExtend.cpp"
BOOL CCardExtend::Initialize()
{
	BOOL bRetCode = FALSE;
	_Lock.Access([&] 
	{ 
		bRetCode = CBnsDbManager::GetInstance().GetEffectiveCardList(_MapCard); 
		std::wcout << L"Effective Card Count=" << _MapCard.size() << std::endl;
	});
	return bRetCode;
}

BOOL CCardExtend::FindCard_By_CRC(_In_ DWORD dwCardCrc, _Out_ CardInfo& CardInfo_) CONST
{
	BOOL bExist = FALSE;
	_Lock.Access([&] 
	{
		auto itr = _MapCard.find(dwCardCrc);
		if (itr != _MapCard.end())
		{
			bExist = TRUE;
			CardInfo_ = itr->second;
		}
	});

	return bExist;
}

BOOL CCardExtend::FindCard_By_Condition(_Out_ CardInfo& CardInfo_, _In_ std::function<BOOL(CONST CardInfo&)> Finder) CONST
{
	BOOL bExist = FALSE;
	_Lock.Access([&]
	{
		for (CONST auto& itm : _MapCard)
		{
			if (Finder(itm.second))
			{
				CardInfo_ = itm.second;
				bExist = TRUE;
				break;
			}
		}
	});

	return bExist;
}

BOOL CCardExtend::FindCard_By_CardNo(_In_ CONST std::wstring& wsCardNo, _Out_ CardInfo& Card) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		return FindCard_By_CRC(MyTools::CLPublic::GetCRC32_DWORD(wsCardNo.c_str()), Card);
	});
}

BOOL CCardExtend::SetCardContent(_In_ DWORD dwCardCrc, _In_ std::function<VOID(CardInfo&)> ActionPtr)
{
	BOOL bExist = FALSE;
	_Lock.Access([&]
	{
		auto itr = _MapCard.find(dwCardCrc);
		if (itr != _MapCard.end())
		{
			bExist = TRUE;
			ActionPtr(itr->second);
		}
	});

	return bExist;
}

BOOL CCardExtend::IsCardEffective(_In_ CONST ClientAccountContent& AccountContent, _Out_ std::wstring& wsErrText) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		if (AccountContent.dwCardCrc == NULL)
		{
			wsErrText = L"Card=0";
			return FALSE;
		}

		CardInfo CardInfo_;
		if (!FindCard_By_CRC(AccountContent.dwCardCrc, CardInfo_))
		{
			wsErrText = MyTools::CCharacter::MakeFormatText(L"不存在该卡号:%08X", AccountContent.dwCardCrc);
			return FALSE;
		}

		if (CardInfo_.GetSurplusHour() <= 0)
		{
			wsErrText = MyTools::CCharacter::MakeFormatText(L"卡号时间不足");
			return FALSE;
		}

		if (CardInfo_.dwToken != AccountContent.dwToken)
		{
			wsErrText = MyTools::CCharacter::MakeFormatText(L"令牌不对! 如果有疑问,联系老夫!");
			return FALSE;
		}

		if (CardInfo_.wsIp != AccountContent.wsIp)
		{
			wsErrText = MyTools::CCharacter::MakeFormatText(L"IP不对! 如果有疑问,联系老夫!");
			return FALSE;
		}

		return TRUE;
	});
}

BOOL CCardExtend::TryActiveCard(_In_ CONST ClientAccountContent& AccountContent, _In_ DWORD dwCrc, _Out_ CardInfo& Card)
{
	if (!CBnsDbManager::GetInstance().ActiveCard(AccountContent, dwCrc, Card))
		return FALSE;

	_Lock.Access([this, Card] {_MapCard.insert(std::make_pair(Card.dwCardCrc, Card)); });
	return TRUE;
}

BOOL CCardExtend::ComparedToken(_In_ DWORD dwCardCrc, _In_ DWORD dwToken) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		CardInfo Card;
		if (!FindCard_By_CRC(dwCardCrc, Card))
			return FALSE;

		return Card.dwToken == dwToken ? TRUE : FALSE;
	});
}

VOID CCardExtend::Print()
{
	_Lock.Access([=] 
	{
		LOG_CF_D(L"Card.size=%d", _MapCard.size());
	});
}
