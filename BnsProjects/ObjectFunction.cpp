#include "stdafx.h"
#include "ObjectFunction.h"
#include <MyTools/CLEchoException.h>
#include "GameFunction.h"
#include "Player.h"
#include "GameUI.h"
#include "UiSkill.h"

#define _SELF L"ObjectFunction.cpp"

BOOL CObjectFunction::ExistGameUI(_In_ CONST std::wstring& wsUiName, _Out_opt_ CGameUi* pGameUi) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__,[&]
	{
		BOOL bRetCode = FALSE;
		CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&pGameUi, this, &wsUiName, &bRetCode] {
			std::map<std::wstring, CGameUi> vlst;
			GetGameUIList(vlst);
			auto itr = vlst.find(wsUiName);
			if (itr != vlst.end())
			{
				MyTools::CLPublic::SetPtr(pGameUi, itr->second);
				bRetCode = TRUE;
			}
		});
		return bRetCode;
	});
}


BOOL CObjectFunction::IsShow_In_GameUi(_In_ CONST std::wstring& wsUiName) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__,[&]
	{
		CGameUi GameUi;
		/*if (ExistGameUI(wsUiName, &GameUi))
		{
			LOG_CF_D( L"Exist GameUi:%s, Show=%d", wsUiName.c_str(), GameUi.IsShow());
		}*/
		return ExistGameUI(wsUiName, &GameUi) && GameUi.IsShow();
	});
}


BOOL CObjectFunction::Action_By_ShowGameUi_Name(_In_ CONST std::wstring& wsUiName, _In_ std::function<VOID(CONST CGameUi&)> Worker) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__,[&]
	{
		CGameUi GameUi;
		if (ExistGameUI(wsUiName, &GameUi) && GameUi.IsShow())
		{
			Worker(GameUi);
			return TRUE;
		}
		return FALSE;
	});
}

BOOL CObjectFunction::Action_By_GameUiName(_In_ CONST std::wstring& wsUiName, _In_ std::function<VOID(CONST CGameUi&)> Worker) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__,[&]
	{
		CGameUi GameUi;
		if (ExistGameUI(wsUiName, &GameUi))
		{
			Worker(GameUi);
			return TRUE;
		}
		return FALSE;
	});
}


BOOL CObjectFunction::FindALLObject_By_Id(_In_ DWORD dwObjId, _In_ CPlayer* pTempObj) CONST
{
	return FindElement_By_ObjectTraverse<CPlayer>(em_GameObject_Type_ALL, FixPointInfo(Point(), -1), pTempObj, [&dwObjId](CONST CPlayer& TempObj){ return TempObj.GetId() == dwObjId; });
}

BOOL CObjectFunction::FindTalkMsg_By_Condiction(_In_ std::function<BOOL(TalkMsg_Src& TalkMsg_)> fnCondiction) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__,[&]
	{
		std::vector<TalkMsg_Src> vlst;
		CObjectFunction::GetTalkMessage(vlst);

		return MyTools::CLPublic::Vec_find_if(vlst, fnCondiction) != nullptr;
	});
}

BOOL CObjectFunction::SetTalkMsg_By_Condition(_In_ CONST std::wstring& wsText, _In_ std::function<BOOL(TalkMsg_Src& TalkMsg_)> fnCondiction) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__,[&]
	{
		std::vector<TalkMsg_Src> vlst;
		CObjectFunction::GetTalkMessage(vlst);

		auto pTalkMsgSrc = MyTools::CLPublic::Vec_find_if(vlst, fnCondiction);
		if (pTalkMsgSrc == nullptr)
			return FALSE;

		MyTools::CCharacter::WriteDWORD(pTalkMsgSrc->dwBase + 0x4 + 0x10, wsText.length());
		MyTools::CCharacter::wstrcpy_my((LPWSTR)ReadDWORD(pTalkMsgSrc->dwBase + 0x4), wsText.c_str());
		return TRUE;
	});
}

UINT CObjectFunction::GetUiSkillList(_Out_ std::vector<CUiSkill>& vlst) CONST
{
	vlst.clear();
	return GetGameUiSkillList(vlst);
}

UINT CObjectFunction::GetWeasponGemInfoList(_Out_ std::vector<WeasponGemInfo>& vlst) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<UINT>(__FUNCTIONW__,[&]
	{
		std::vector<DWORD> lst;
		GetWeasponGemList(lst);

		vlst.clear();
		for (auto& itm : lst)
		{
			WeasponGemInfo WeasponGemInfo_;
			WeasponGemInfo_.dwItemId = itm;
			WeasponGemInfo_.dwItemObj = CBag::GetItemObj_By_ID(WeasponGemInfo_.dwItemId);
			if (WeasponGemInfo_.dwItemObj != NULL)
			{
				CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&WeasponGemInfo_] {
					WeasponGemInfo_.wsGemName = CBag::GetItemName(WeasponGemInfo_.dwItemObj);
				});
			}

			vlst.push_back(WeasponGemInfo_);
		}

		return vlst.size();
	});
}

BOOL CObjectFunction::ExistMonsterBuff(_In_ DWORD dwTarId, _In_ DWORD dwBuffID) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__,[&]
	{
		std::vector<GAMEOBJ_BUFF> vlst;
		GetMonsterBuffList(vlst);

		return MyTools::CLPublic::Vec_find_if(vlst, static_cast<GAMEOBJ_BUFF*>(nullptr), [&dwBuffID, &dwTarId](CONST GAMEOBJ_BUFF& MonsterBuff) {return MonsterBuff.dwTargetId == dwTarId && MonsterBuff.dwBuffId == dwBuffID; });
	});
}

int CObjectFunction::GetIndex_By_Auction() CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<UINT>(__FUNCTIONW__,[&]
	{
		int Index = -1;
		Action_By_ShowGameUi_Name(UI_TEXT_PARTYAUCTION, [this, &Index](CONST CGameUi& GameUi_)
		{
			AuctionContent AuctionContent_;
			if (GetAuctionContent(GameUi_, AuctionContent_))
				Index = static_cast<int>(AuctionContent_.Index);
		});

		return Index;
	});
}

BOOL CObjectFunction::GetTeamAuctionContent(_Out_ AuctionContent& AuctionContent_) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<UINT>(__FUNCTIONW__,[&]
	{
		BOOL bRetCode = FALSE;
		Action_By_ShowGameUi_Name(UI_TEXT_PARTYAUCTION, [&bRetCode, this, &AuctionContent_](_In_ CONST CGameUi& GameUi_)
		{
			bRetCode = GetAuctionContent(GameUi_, AuctionContent_);
		});
		return bRetCode && AuctionContent_.VecAuctionItem.size() != 0 && AuctionContent_.VecAuctionItem.size() > AuctionContent_.Index;
	});
}
