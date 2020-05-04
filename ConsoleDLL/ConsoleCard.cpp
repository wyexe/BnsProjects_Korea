#include "stdafx.h"
#include "ConsoleCard.h"

#define _SELF L"ConsoleCard.cpp"

BOOL CConsoleCard::ExistCard(_In_ CONST std::wstring& wsCardNo, _In_ std::function<void(CardInfo&)> fnPtr)
{
	BOOL bRetCode = FALSE;
	LockVecCard.Access([this, &wsCardNo, &fnPtr, &bRetCode]
	{
		auto p = ExistCard([wsCardNo](CONST CardInfo& CardInfo_){ return CardInfo_.wsCardNo == wsCardNo; });
		if (p != nullptr)
		{
			bRetCode = TRUE;
			fnPtr(*p);
		}
	});
	return bRetCode;
}

CardInfo* CConsoleCard::ExistCard(_In_ std::function<BOOL(CONST CardInfo&)> fnPred)
{
	return MyTools::CLPublic::Vec_find_if(VecCard, fnPred);
}

BOOL CConsoleCard::ExistCard(_In_ DWORD dwCardCrc, _In_ std::function<void(CardInfo&)> fnPtr)
{
	BOOL bRetCode = FALSE;
	LockVecCard.Access([this, &dwCardCrc, &fnPtr, &bRetCode]
	{
		auto p = ExistCard([dwCardCrc](CONST CardInfo& CardInfo_){ return CardInfo_.dwCardCrc == dwCardCrc; });
		if (p != nullptr)
		{
			bRetCode = TRUE;
			fnPtr(*p);
		}
	});
	return bRetCode;
}

BOOL CConsoleCard::GetNotUseCard(_In_ CardInfo& CardInfo_)
{
	BOOL bRetCode = FALSE;
	LockVecCard.Access([&bRetCode, this, &CardInfo_]
	{
		auto p = ExistCard([](CONST CardInfo& tmpCardInfo){ return !tmpCardInfo.bExit;  });
		if (p != nullptr)
		{
			bRetCode = TRUE;
			p->bExit = TRUE;
			MyTools::CLPublic::SetPtr(&CardInfo_, *p);
		}
	});
	return bRetCode;
}

UINT CConsoleCard::GetCardCount() CONST
{
	UINT uCount = 0;
	LockVecCard.Access([&uCount, this]{uCount = VecCard.size(); });
	return uCount;
}

VOID CConsoleCard::AddCard(_In_ CONST CardInfo& CardInfo_)
{
	LockVecCard.Access([this, CardInfo_]{ VecCard.push_back(CardInfo_); });
}

VOID CConsoleCard::ForEach(_In_ std::function<VOID(CardInfo&)> fnPtr)
{
	LockVecCard.Access([this, fnPtr]
	{
		for (auto& itm : VecCard)
			fnPtr(itm);
	});
}

VOID CConsoleCard::RemoveCard(_In_ std::function<BOOL(CONST CardInfo&)> fnPtr)
{
	LockVecCard.Access([this, fnPtr]
	{
		MyTools::CLPublic::Vec_erase_if(VecCard, fnPtr);
	});
}

std::vector<CardInfo>& CConsoleCard::GetData()
{
	return VecCard;
}

CConsoleCard::CConsoleCard() : LockVecCard(L"LockVecCard")
{

}
