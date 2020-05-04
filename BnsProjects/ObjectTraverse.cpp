#include "stdafx.h"
#include "ObjectTraverse.h"
#include <MyTools/Log.h>
#include <MyTools/CLEchoException.h>
#include "TeamMemberExtend.h"
#include "Person.h"
#include "UiSkill.h"
#include "GameUI.h"
#include "Skill.h"
#include "BagBase.h"
#include "LoginPlayer.h"
#include "TeamMember.h"
#include "GameSend.h"
#define _SELF L"ObjectTraverse.cpp"

//////////////////////////////////////////////////////////////////////////
UINT CObjectTraverse::GetSkillList(_Out_ std::vector<CSkill>& vlst) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<UINT>(__FUNCTIONW__,[&]
	{
		vlst.clear();
		DWORD dwRootTree = ReadDWORD(ReadDWORD(ReadDWORD(CPerson::GetInstance().GetBase() + 技能遍历偏移) + 0x8) + 0x4);
		TraverseSkill(dwRootTree, vlst);

		CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&vlst]
		{
			for (auto& itm : vlst)
				itm.SetName();
		});

		return vlst.size();
	});
}

BOOL CObjectTraverse::TraverseSkill(_In_ DWORD dwNode, _Out_ std::vector<CSkill>& vlst) CONST
{ 
	return MyTools::CLEchoException::GetInstance().InvokeFunc<UINT>(__FUNCTIONW__,[&] 
	{
		if (dwNode != NULL && ReadBYTE(dwNode + 0x15) == 0x0)
		{
			vlst.push_back(CSkill(dwNode));

			TraverseSkill(ReadDWORD(dwNode + 0x0), vlst);
			TraverseSkill(ReadDWORD(dwNode + 0x8), vlst);
		}
		return TRUE;
	});
}

//////////////////////////////////////////////////////////////////////////
UINT CObjectTraverse::GetBagList(_Out_ std::vector<CBag>& vlst, _In_opt_ em_Bag_Type emType /* = em_Bag_Type_Bag */) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<UINT>(__FUNCTIONW__,[&]
	{
		vlst.clear();

		DWORD dwRootBase = CPerson::GetInstance().GetBase();
		DWORD dwCount = GetMaxBagCount(emType);

		for (DWORD i = 0; i < dwCount; ++i)
		{
			DWORD dwItemObj = 0;
			if (emType == em_Bag_Type_Bag)
				dwItemObj = ReadDWORD(ReadDWORD(ReadDWORD(dwRootBase + 背包遍历偏移) + 0x2C) + i * 4);
			else if (emType == em_Bag_Type_Equi)
				dwItemObj = ReadDWORD(ReadDWORD(ReadDWORD(dwRootBase + 背包遍历偏移) + 0x1C) + i * 4);
			else if (emType == em_Bag_Type_Warehouse)
				dwItemObj = ReadDWORD(ReadDWORD(ReadDWORD(dwRootBase + 背包遍历偏移) + 0x3C) + i * 4);
			else if (emType == em_Bag_Type_Wardrobe)
				dwItemObj = ReadDWORD(ReadDWORD(ReadDWORD(dwRootBase + 背包遍历偏移) + 0x6C) + i * 4);

			if (dwItemObj == NULL)
				continue;

			vlst.push_back(CBag(dwItemObj, i));
		}

		CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&vlst] {
			for (auto& itm : vlst)
				itm.SetName();
		});


		return vlst.size();
	});
}

UINT CObjectTraverse::GetMaxBagCount(_In_ em_Bag_Type emBagType) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<DWORD>(__FUNCTIONW__,[&]
	{
		DWORD dwRootBase = CPerson::GetInstance().GetBase();
		switch (emBagType)
		{
		case CObjectTraverse::em_Bag_Type_Bag:
			return ReadDWORD(dwRootBase + 背包格子数量偏移 + 0x0) & 0xFF;
		case CObjectTraverse::em_Bag_Type_Equi:
			return DWORD(36);
		case CObjectTraverse::em_Bag_Type_Warehouse:
			return ReadDWORD(dwRootBase + 背包格子数量偏移 + 0x1) & 0xFF;
		case CObjectTraverse::em_Bag_Type_Wardrobe:
			return ReadDWORD(dwRootBase + 背包格子数量偏移 + 0x2) & 0xFF;
		default:
			break;
		}
		return DWORD(0);
	});
}

UINT CObjectTraverse::GetGameUIList(_Out_ std::map<std::wstring, CGameUi>& vlst) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<UINT>(__FUNCTIONW__,[&]
	{
		vlst.clear();

		DWORD dwRootBase = ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(人物基址) + 人物基址偏移4) + 0x4 + 0x4) + 0x4 + 0x8) + 0x4);
		TraverseGameUI(dwRootBase, vlst);
		return vlst.size();
	});
}

BOOL CObjectTraverse::TraverseGameUI(_In_ DWORD dwNode, _Out_ std::map<std::wstring, CGameUi>& vlst) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<UINT>(__FUNCTIONW__,[&]
	{
		if (ReadBYTE(dwNode + 0x1D) == 0x0)
		{
			CGameUi GUI(ReadDWORD(dwNode + 0x10));
			//DWORD dwShow = ReadDWORD(GUI.GetObj() + 0x38);
			DWORD dwNameLen1 = ReadDWORD(GUI.GetObj() + 0xC + 0x10 + 0x4);
			DWORD dwNameLen2 = ReadDWORD(GUI.GetObj() + 0xC + 0x10);
			if (GUI.GetId() != NULL && dwNameLen1 <= 0x1F && dwNameLen2 <= 0x1F && dwNameLen1 > NULL && dwNameLen2 > NULL && GUI.GetObj() != NULL && ReadDWORD(GUI.GetObj() + 0xC) != NULL && ReadDWORD(GUI.GetObj() + 0x38) <= 0x2)
			{
				vlst.insert({ GUI.GetName(), GUI });
			}

			if (ReadDWORD(dwNode + 0x0) != ReadDWORD(dwNode + 0x8))
			{
				TraverseGameUI(ReadDWORD(dwNode + 0x0), vlst);
				TraverseGameUI(ReadDWORD(dwNode + 0x8), vlst);
			}
			return TRUE;
		}
		return FALSE;
	});
}

UINT CObjectTraverse::GetPersonBuffList(_Out_ std::vector<GAMEOBJ_BUFF>& vlst) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<UINT>(__FUNCTIONW__,[&]
	{
		vlst.clear();

		//dd [[[[11502F8]+44]+20]+18+D628+18A8+214F8]+10
		DWORD dwSelfAddr = ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(人物基址) + 人物基址偏移3) + 0x1C) + 人物BUFF偏移5 + 人物BUFF偏移1 + 人物BUFF偏移2 + 人物BUFF偏移3) + 人物BUFF偏移_SELF;
		//LOG_CF_D(L"dwSelfAddr=%x", dwSelfAddr);
		for (UINT i = 0; i < 0x14; ++i)
		{
			if (ReadDWORD(dwSelfAddr + i * 人物BUFF_ARRAY_SIZE + 0x14) == NULL)
				continue;

			GAMEOBJ_BUFF GOB = { 0 };
			GOB.dwBuffBase = dwSelfAddr + i * 人物BUFF_ARRAY_SIZE;
			GOB.dwBuffId = ReadDWORD(GOB.dwBuffBase + 0x14);
			GOB.dwTargetId = ReadDWORD(GOB.dwBuffBase + 0x8);
			GOB.dwBuffCount = 0;
			vlst.push_back(GOB);
		}

		DWORD dwOtherAddr = ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(人物基址) + 人物基址偏移3) + 0x1C) + 人物BUFF偏移5 + 人物BUFF偏移1 + 人物BUFF偏移2 + 人物BUFF偏移3) + 人物BUFF偏移_OTHER;
		//LOG_CF_D(L"dwOtherAddr=%x", dwOtherAddr);
		for (UINT i = 0; i < 0x14; ++i)
		{
			if (ReadDWORD(dwOtherAddr + i * 人物BUFF_ARRAY_SIZE + 0x14) == NULL)
				continue;

			GAMEOBJ_BUFF GOB = { 0 };
			GOB.dwBuffBase = dwOtherAddr + i * 人物BUFF_ARRAY_SIZE;
			GOB.dwBuffId = ReadDWORD(GOB.dwBuffBase + 0x14);
			GOB.dwTargetId = ReadDWORD(GOB.dwBuffBase + 0x8);
			GOB.dwBuffCount = 0;
			vlst.push_back(GOB);
		}

		dwOtherAddr = ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(人物基址) + 人物基址偏移3) + 0x1C) + 人物BUFF偏移5 + 人物BUFF偏移1 + 人物BUFF偏移2 + 人物BUFF偏移3) + 怪物BUFF偏移_OTHER;
		//LOG_CF_D(L"怪物BUFF偏移_OTHER=%x", dwOtherAddr);
		for (UINT i = 0; i < 0x14; ++i)
		{
			if (ReadDWORD(dwOtherAddr + i * 人物BUFF_ARRAY_SIZE + 0x14) == NULL)
				continue;

			GAMEOBJ_BUFF GOB = { 0 };
			GOB.dwBuffBase = dwOtherAddr + i * 人物BUFF_ARRAY_SIZE;
			GOB.dwBuffId = ReadDWORD(GOB.dwBuffBase + 0x14);
			GOB.dwTargetId = ReadDWORD(GOB.dwBuffBase + 0x8);
			GOB.dwBuffCount = 0;
			vlst.push_back(GOB);
		}

		dwOtherAddr = ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(人物基址) + 人物基址偏移3) + 0x1C) + 人物BUFF偏移5 + 人物BUFF偏移1 + 人物BUFF偏移2 + 人物BUFF偏移3) + 系统BUFF偏移_OTHER;
		//LOG_CF_D(L"系统BUFF偏移_OTHER=%x", dwOtherAddr);
		for (UINT i = 0; i < 0x14; ++i)
		{
			if (ReadDWORD(dwOtherAddr + i * 人物BUFF_ARRAY_SIZE + 0x14) == NULL)
				continue;

			GAMEOBJ_BUFF GOB = { 0 };
			GOB.dwBuffBase = dwOtherAddr + i * 人物BUFF_ARRAY_SIZE;
			GOB.dwBuffId = ReadDWORD(GOB.dwBuffBase + 0x14);
			GOB.dwTargetId = ReadDWORD(GOB.dwBuffBase + 0x8);
			GOB.dwBuffCount = 0;
			vlst.push_back(GOB);
		}

		return vlst.size();
	});
}

UINT CObjectTraverse::GetMonsterBuffList(_Out_ std::vector<GAMEOBJ_BUFF>& vlst) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<UINT>(__FUNCTIONW__,[&]
	{
		// // dd [[[12B0A5C]+48]+1C]+14+7368+22A8C+18
		DWORD dwAddr = ReadDWORD(ReadDWORD(ReadDWORD(人物基址) + 人物基址偏移3) + 0x1C) + 怪物BUFF偏移1 + 怪物BUFF偏移2 + 怪物BUFF偏移3 + 怪物BUFF偏移4;

		auto fnAddToList = [&vlst](DWORD dwBuffBase)
		{
			if (ReadDWORD(dwBuffBase + 0x14) != NULL)
			{
				GAMEOBJ_BUFF GOB = { 0 };
				GOB.dwBuffBase = dwBuffBase;
				GOB.dwBuffId = ReadDWORD(GOB.dwBuffBase + 0x14);
				GOB.dwTargetId = ReadDWORD(GOB.dwBuffBase + 0x8);
				GOB.dwBuffCount = ReadDWORD(GOB.dwBuffBase + 0x20);
				vlst.push_back(GOB);
			}
		};

		dwAddr += 怪物BUFF偏移5;
		for (UINT i = 0; i < 0x23; ++i)
		{
			fnAddToList(dwAddr - 怪物BUFF偏移6);
			fnAddToList(dwAddr);
			fnAddToList(dwAddr + 怪物BUFF偏移6);
			dwAddr += 人物BUFF_ARRAY_SIZE;
		}

		return vlst.size();
	});
}


UINT CObjectTraverse::GetTalkMessage(_Out_ std::vector<TalkMsg_Src>& vlst) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<UINT>(__FUNCTIONW__,[&]
	{
		vlst.clear();

		CONST static int nMaxCount = 300;
		DWORD dwAddr = ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(人物基址) + 人物基址偏移1) + 聊天消息框偏移1 + 0x4) + 0x0);

		for (int i = 1; i < nMaxCount; ++i)
		{
			TalkMsg_Src TMS;
			TMS.dwBase = dwAddr + 聊天消息框偏移2 * i;

			DWORD dwMsgLength = ReadDWORD(TMS.dwBase + 0x4 + 0x10);
			if (dwMsgLength <= 0x7)
				continue;

			if (ReadDWORD(TMS.dwBase + 0x4) == NULL)
				continue;

			TMS.wsText = (LPCWSTR)ReadDWORD(TMS.dwBase + 0x4);
			vlst.push_back(TMS);

			//CCharacter::wstrcpy_my(TMS.wszMsg, (LPCWSTR)ReadDWORD(TMS.dwBase + 0x4), sizeof(TMS.wszMsg) / sizeof(WCHAR));

		}

		return vlst.size();
	});
}

UINT CObjectTraverse::GetTeamList(_Out_ std::vector<CTeamMember>& vlst) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<UINT>(__FUNCTIONW__,[&]
	{
		UINT uCount = CTeamMemberExtend::GetInstance().GetTeamMemberCount();
		if (uCount > 6)
			return UINT(0);

		vlst.clear();
		for (UINT i = 0; i < uCount; ++i)
		{
			DWORD dwBase = ReadDWORD(ReadDWORD(ReadDWORD(CPerson::GetInstance().GetBase() + 队伍遍历偏移) + 0x14) + i * 4);
			if (dwBase == NULL)
				continue;

			vlst.push_back(CTeamMember(dwBase, i));
		}

		for (auto& itm : vlst)
			itm.SetName();

		return vlst.size();
	});
}

UINT CObjectTraverse::GetLoginPlayerList(_Out_ std::vector<CLoginPlayer>& vlst) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<UINT>(__FUNCTIONW__,[&]
	{
		DWORD dwAddr = ReadDWORD(ReadDWORD(人物基址) + 人物基址偏移1 + 0x8) + 角色遍历偏移;
		DWORD dwPlayerCount = GetLoginPlayerCount();

		for (DWORD i = 0; i < dwPlayerCount; ++i)
			vlst.push_back(CLoginPlayer(ReadDWORD(dwAddr) + i * 0x30, i));
		return vlst.size();
	});
}

UINT CObjectTraverse::GetLoginPlayerCount() CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<DWORD>(__FUNCTIONW__,[&]
	{
		DWORD dwAddr = ReadDWORD(ReadDWORD(人物基址) + 人物基址偏移1 + 0x8) + 角色遍历偏移;
		DWORD dwPlayerCount = (ReadDWORD(dwAddr + 4) - ReadDWORD(dwAddr)) / 0x30;
		if (dwPlayerCount >= 0xA)
		{
			LOG_MSG_CF(L"角色数量现在是:%d个? 更新有问题? !", dwPlayerCount);
			return DWORD(0);
		}
		return dwPlayerCount;
	});
}

UINT CObjectTraverse::GetMaxLoginPlayerCount() CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<DWORD>(__FUNCTIONW__,[&]
	{
		DWORD dwAddr = ReadDWORD(ReadDWORD(人物基址) + 人物基址偏移1 - 0x4) + 0x10;
		DWORD dwPlayerCount = (ReadDWORD(dwAddr + 4) - ReadDWORD(dwAddr)) / 0x4;
		if (dwPlayerCount >= 0xA)
		{
			LOG_MSG_CF(L"角色数量现在是:%d个? 更新有问题? !", dwPlayerCount);
			return DWORD(0);
		}
		return dwPlayerCount;
	});
}

UINT CObjectTraverse::GetGameUiSkillList(_Out_ std::vector<CUiSkill>& vlst) CONST
{
	UINT uIndex = 0;
	auto fnOffset = [&vlst, &uIndex](DWORD dwOffset, DWORD dwHead)
	{
		DWORD dwUiPanelObj = ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(人物基址) + 人物基址偏移3) + 0x4 + 0x8) + UI技能遍历偏移1 - 0xC + UI技能遍历偏移2 + dwOffset + 0x4 + 0x4);
		if (dwUiPanelObj == NULL)
			return (UINT)0;

		DWORD dwAddr = ReadDWORD(dwUiPanelObj + dwHead);
		DWORD dwCount = (ReadDWORD(dwUiPanelObj + dwHead + 0x4) - ReadDWORD(dwUiPanelObj + dwHead)) / 4;
		if (dwCount >= 40)
		{
			LOG_CF_E(L"UiSkillListCount=%d, 更新错误了!", dwCount);
			return (UINT)0;
		}

		for (DWORD i = 0; i < dwCount; ++i)
		{
			CUiSkill UiSkill_(ReadDWORD(dwAddr + i * 4), uIndex++);
			if (UiSkill_.GetId() != NULL && UiSkill_.IsExist())
				vlst.push_back(UiSkill_);
		}
		//LOG_CF_D(L"vlst.size()=%d", vlst.size());
		return vlst.size();
	};

	return MyTools::CLEchoException::GetInstance().InvokeFunc<DWORD>(__FUNCTIONW__,[&]
	{
		// dd [[[[0x11EB3F8]+2C]+4+8]+732C-0C+49378+20+4+4]
		fnOffset(0x20, 0x28);
		uIndex = 20;
		fnOffset(0x1C, 0x30);
		return vlst.size();
	});
}

UINT CObjectTraverse::GetWeasponGemList(_Out_ std::vector<DWORD>& vlst) CONST
{
	static CONST std::vector<DWORD> vIgnoreList = 
	{
		0xFDF3, 0xFDE9, 0xFDEA, 0xFDEB, 0xFDED, 0xFDEE, 0xFDEF,
		0xFDF0, 0xFDF1, 0xFDF2
	};

	return MyTools::CLEchoException::GetInstance().InvokeFunc<DWORD>(__FUNCTIONW__,[&]
	{
		auto dwAddr = ReadDWORD(ReadDWORD(ReadDWORD(CPerson::GetInstance().GetBase() + 背包遍历偏移) + 0x10 + 0x8 + 0x4)) + 第一颗宝石偏移;
		std::vector<DWORD> lst;
		for (auto i = 0; i < 6; ++i)
		{
			DWORD dwItemId = ReadDWORD(dwAddr + i * 4);
			if (dwItemId == 0)
				break;

			lst.push_back(dwItemId);
		}

		vlst.clear();
		for (auto& itm : lst)
		{
			if (std::find_if(vIgnoreList.begin(), vIgnoreList.end(), [&itm](_In_ DWORD dwIgnoreId) {
				return dwIgnoreId == itm;
			}) == vIgnoreList.end())
			{
				vlst.push_back(itm);
			}
		}

		return vlst.size();
	});
}

UINT CObjectTraverse::GetHideUiSkillList(_Out_ std::vector<DWORD>& vlst) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<UINT>(__FUNCTIONW__,[&]
	{
		// dd [[[[[[123F534]+34]+80]+784]+64]+6*3*4-4]
		DWORD dwAddr = ReadDWORD(ReadDWORD(CPerson::GetInstance().GetBase() + 技能冷却偏移) + 隐藏UI技能偏移);

		for (int k = 0; k < 7; ++k)
		{
			DWORD dwCount = ReadDWORD(dwAddr + k * 3 * 4);
			if (dwCount == NULL)
				continue;

			DWORD dwHead = (dwAddr + k * 3 * 4 - 0x4);
			DWORD dwNode = ReadDWORD(ReadDWORD(dwHead + 0x0));
			//LOG_CF_D( L"dwHead=%X,dwNode=%X", dwHead, dwNode);
			int j = 0;
			int i = 0;
			// 红黑二叉树遍历
			DWORD dwLeapNode = 0;

			static const int nMaxLoopCount = 100;
			int nLoop1Count = 0;

			while (dwNode != ReadDWORD(dwHead) && nLoop1Count++ < nMaxLoopCount)
			{
				vlst.push_back(ReadDWORD(dwNode + 0x18));
				if (ReadBYTE(dwNode + 0x39) == 0)
				{
					dwLeapNode = ReadDWORD(dwNode + 0x8);
					if (ReadBYTE(dwLeapNode + 0x39) != NULL)
					{
						int nLoop2Count = 0;
						for (i = ReadDWORD(dwNode + 0x4); ReadBYTE(i + 0x39) == 0 && nLoop2Count++ < nMaxLoopCount; i = ReadDWORD(i + 0x4))
						{
							if (dwNode != ReadDWORD(i + 0x8))
								break;
							dwNode = i;
						}
						dwNode = i;
					}
					else
					{
						dwNode = ReadDWORD(dwNode + 0x8);

						int nLoop2Count = 0;
						for (j = ReadDWORD(dwLeapNode); ReadBYTE(j + 0x39) == 0 && nLoop2Count++ < nMaxLoopCount; j = ReadDWORD(j))
						{
							dwNode = j;
						}
					}
				}
			}
		}
		return vlst.size();
	});
}

UINT CObjectTraverse::GetUnopenGiftList(_Out_ std::vector<UnOpenGiftInfo>& vlst) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<DWORD>(__FUNCTIONW__,[&]
	{
		// dd [[[[[125145C]+3C]+10]+4+4]]
		DWORD dwAddr = ReadDWORD(ReadDWORD(ReadDWORD(人物基址) + 遍历礼箱偏移) + 0x10);
		DWORD dwCount = ReadDWORD(dwAddr + 0x18);
		WCHAR wszText[MAX_PATH];
		for (DWORD i = 0; i < dwCount; ++i)
		{
			UnOpenGiftInfo UnOpenGiftInfo_;
			UnOpenGiftInfo_.dwBase = ReadDWORD(ReadDWORD(dwAddr + 0xC) + i * 4);
			UnOpenGiftInfo_.dwParm = ReadDWORD(UnOpenGiftInfo_.dwBase);
			MyTools::CCharacter::wstrcpy_my(wszText, (LPCWSTR)ReadDWORD(UnOpenGiftInfo_.dwBase + 0x2C), _countof(wszText) - 1);
			UnOpenGiftInfo_.wsTitle = wszText;

			DWORD dwItemBase = UnOpenGiftInfo_.dwBase + 未打开礼箱物品偏移 + 0x4;
			DWORD dwItemCount = (ReadDWORD(dwItemBase + 0x4) - ReadDWORD(dwItemBase)) / 0x10;
			for (DWORD dwItemIndex = 0; dwItemIndex < dwItemCount; ++dwItemIndex)
			{
				UnOpenGiftItem UnOpenGiftItem_;
				memcpy(&UnOpenGiftItem_, (LPVOID)(ReadDWORD(dwItemBase) + 0x10 * dwItemIndex), 0x10);
				UnOpenGiftInfo_.vItem.push_back(UnOpenGiftItem_);

			}
			vlst.push_back(UnOpenGiftInfo_);
		}

		// 转换名称!
		CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&vlst] 
		{
			for (auto& gift_ : vlst)
			{
				for (auto& itm : gift_.vItem)
				{
					itm.ItemName = CBag::GetItemName(CGameSend::GetItemObj_By_ID(itm.dwItemId));
				}
			}
		});

		return vlst.size();
	});
}

UINT CObjectTraverse::GetGiftList(_Out_ std::vector<OpenGiftInfo>& vlst) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<DWORD>(__FUNCTIONW__,[&]
	{
		// dd [[[[[125145C]+3C]+10]+4+4]]
		DWORD dwAddr = ReadDWORD(ReadDWORD(ReadDWORD(人物基址) + 遍历礼箱偏移) + 0x10);
		DWORD dwCount = ReadDWORD(dwAddr + 0x20 + 0x4);
		WCHAR wszText[MAX_PATH];
		for (DWORD i = 0; i < dwCount; ++i)
		{
			OpenGiftInfo OpenGiftInfo_;
			OpenGiftInfo_.dwBase = ReadDWORD(ReadDWORD(dwAddr + 0x20) + i * 4);
			OpenGiftInfo_.dwParm = ReadDWORD(OpenGiftInfo_.dwBase);

			DWORD dwItemBase = OpenGiftInfo_.dwBase + 已打开礼箱物品偏移 + 0x4;
			DWORD dwItemCount = (ReadDWORD(dwItemBase + 0x4) - ReadDWORD(dwItemBase)) / 0x20;
			if (dwItemCount == 0)
				continue;

			for (DWORD dwItemIndex = 0; dwItemIndex < dwItemCount; ++dwItemIndex)
			{
				OpenGiftItem OpenGiftItem_;
				memcpy(&OpenGiftItem_, (LPVOID)(ReadDWORD(dwItemBase) + 0x20 * dwItemIndex), 0x20);
				OpenGiftInfo_.vItem.push_back(OpenGiftItem_);
			}

			MyTools::CCharacter::wstrcpy_my(wszText, (LPCWSTR)ReadDWORD(OpenGiftInfo_.dwBase + 0x24), _countof(wszText) - 1);
			/*if (ReadDWORD(OpenGiftInfo_.dwBase + 0x24 + 0x14) >= 0x1F)
				CCharacter::wstrcpy_my(wszText, (LPCWSTR)ReadDWORD(ReadDWORD(OpenGiftInfo_.dwBase + 0x24)), _countof(wszText) - 1);
			else
				CCharacter::wstrcpy_my(wszText, (LPCWSTR)ReadDWORD(OpenGiftInfo_.dwBase + 0x24), _countof(wszText) - 1);*/

			OpenGiftInfo_.wsTitle = wszText;
			vlst.push_back(OpenGiftInfo_);
		}

		// 转换名称!
		CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&vlst] {
			for (auto& gift_ : vlst)
			{
				for (auto& itm : gift_.vItem)
				{
					itm.ItemName = CBag::GetItemName(CGameSend::GetItemObj_By_ID(itm.dwItemId));
				}
			}
		});

		return vlst.size();
	});
}

BOOL CObjectTraverse::GetAuctionContent(_In_ CONST CGameUi& PartyAuctionPanel, _Out_ AuctionContent& AuctionContent_) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<DWORD>(__FUNCTIONW__,[&]
	{
		DWORD dwNode = ReadDWORD(PartyAuctionPanel.GetBase() + 竞拍偏移1);
		if (dwNode == NULL)
			return FALSE;

		DWORD dwArrayCount = (ReadDWORD(dwNode + 竞拍偏移2 + 0x4) - ReadDWORD(dwNode + 竞拍偏移2)) >> 0x6;
		for (DWORD i = 0; i < dwArrayCount; ++i)
		{
			AuctionItem AuctionItem_;
			AuctionItem_.dwBase = ReadDWORD(dwNode + 竞拍偏移2) + i * 0x40;
			AuctionItem_.dwItemId = ReadDWORD(AuctionItem_.dwBase + 0x0);
			AuctionItem_.wsItemName = CBag::GetItemName(CGameSend::GetItemObj_By_ID(AuctionItem_.dwItemId));

			AuctionContent_.VecAuctionItem.push_back(AuctionItem_);
		}

		AuctionContent_.Index = ReadDWORD(dwNode + 竞拍物品Index偏移);
		return TRUE;
	});
}
