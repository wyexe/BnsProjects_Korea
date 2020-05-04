#ifndef __GITBNSPROJECTS_BNSPROJECTS_OBJECTS_OBJECTTRAVERSE_H__
#define __GITBNSPROJECTS_BNSPROJECTS_OBJECTS_OBJECTTRAVERSE_H__

#include "GameVariable.h"
#include <MyTools/Log.h>

class CUiSkill;
class CLoginPlayer;
class CGameUi;
class CTeamMember;
class CTask;
class CPerson;
class CCorpse;
class CBag;
class CSkill;
class CDropItem;
class CNpc;
class CMonster;
class CObjectTraverse
{
public:
	CObjectTraverse() = default;
	~CObjectTraverse() = default;

	enum em_GameObject_Type
	{
		em_GameObject_Type_Monster,
		em_GameObject_Type_Npc,
		em_GameObject_Type_Player,
		em_GameObject_Type_DropItem,
		em_GameObject_Type_Corpse,
		em_GameObject_Type_MonsterCorpse,
		em_GameObject_Type_Pet,
		em_GameObject_Type_ALL,
		em_GameObject_Type_PracticePlayer,
	};

	enum em_Bag_Type
	{
		em_Bag_Type_Bag,
		em_Bag_Type_Equi,
		em_Bag_Type_Warehouse,
		em_Bag_Type_Wardrobe
	};

public:
	//////Skill////////////////////////////////////////////////////////////////////
	UINT GetSkillList(_Out_ std::vector<CSkill>& vlst) CONST;

	UINT GetGameUiSkillList(_Out_ std::vector<CUiSkill>& vlst) CONST;

	///////Bag///////////////////////////////////////////////////////////////////
	UINT GetBagList(_Out_ std::vector<CBag>& vlst, _In_opt_ em_Bag_Type emType = em_Bag_Type_Bag) CONST;
	UINT GetMaxBagCount(_In_ em_Bag_Type emBagType) CONST;

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	UINT GetSortGameObjectList(_Out_ std::vector<T>& vlst, _In_ em_GameObject_Type emType, _In_ CONST FixPointInfo& FixPtInfo) CONST
	{
		std::vector<T> vObjectList;
		DWORD dwTreeRoot = ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(人物基址) + 人物基址偏移1) + 怪物遍历_偏移 + 0x4) + 0x4);
		//LOG_CF_D( L"dwTreeRoot=%x", dwTreeRoot);
		TraverseGameObject(dwTreeRoot, emType, vObjectList);
		//LOG_CF_D( L"TraverseGameObject=%d", vObjectList.size());
		vlst.clear();
		std::for_each(vObjectList.begin(), vObjectList.end(), [&vlst, &FixPtInfo](T& Obj){
			Obj.SetDis(Obj.GetDis(FixPtInfo.FixPoint));
			if (FixPtInfo.fDis == -1 || Obj.SortDis() < FixPtInfo.fDis)
				vlst.push_back(Obj);
		});

		if (emType != em_GameObject_Type_ALL)
		{
			CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&vlst, &emType, &FixPtInfo]
			{
				for (auto i = 0; i < static_cast<int>(vlst.size()); ++i)
				{
					vlst.at(i).SetName();
					if (FixPtInfo.fDis != -1 && (emType == em_GameObject_Type_Monster || emType == em_GameObject_Type_Npc) && vlst.at(i).GetName().empty())
					{
						vlst.erase(vlst.begin() + i--);
					}
				}
			});
		}
		
		sort(vlst.begin(), vlst.end());
		return vlst.size();
	}

	////////GameUI//////////////////////////////////////////////////////////////////
	UINT GetGameUIList(_Out_ std::map<std::wstring, CGameUi>& vlst) CONST;

	///////BUFF///////////////////////////////////////////////////////////////////
	UINT GetPersonBuffList(_Out_ std::vector<GAMEOBJ_BUFF>& vlst) CONST;
	UINT GetMonsterBuffList(_Out_ std::vector<GAMEOBJ_BUFF>& vlst) CONST;

	////////TalkMessage//////////////////////////////////////////////////////////////////
	UINT GetTalkMessage(_Out_ std::vector<TalkMsg_Src>& vlst) CONST;

	//////Team////////////////////////////////////////////////////////////////////
	UINT GetTeamList(_Out_ std::vector<CTeamMember>& vlst) CONST;

	///////LoginPlayer///////////////////////////////////////////////////////////////////
	UINT GetMaxLoginPlayerCount() CONST;
	UINT GetLoginPlayerList(_Out_ std::vector<CLoginPlayer>& vlst) CONST;
	UINT GetLoginPlayerCount() CONST;

	///////WeasponGem///////////////////////////////////////////////////////////////////
	UINT GetWeasponGemList(_Out_ std::vector<DWORD>& vlst) CONST;


	///////HideUiSkillList///////////////////////////////////////////////////////////////////
	UINT GetHideUiSkillList(_Out_ std::vector<DWORD>& vlst) CONST;


	///////Gift///////////////////////////////////////////////////////////////////
	UINT GetUnopenGiftList(_Out_ std::vector<UnOpenGiftInfo>& vlst) CONST;
	UINT GetGiftList(_Out_ std::vector<OpenGiftInfo>& vlst) CONST;

	// Auction
	BOOL GetAuctionContent(_In_ CONST CGameUi& PartyAuctionPanel, _Out_ AuctionContent& AuctionContent_) CONST;
private:
	BOOL TraverseGameUI( _In_ DWORD dwNode, _Out_ std::map<std::wstring, CGameUi>& vlst) CONST;

	BOOL TraverseSkill(_In_ DWORD dwNode, _Out_ std::vector<CSkill>& vlst) CONST;

	template<typename T>
	UINT TraverseGameObject(_In_ DWORD dwNode, _In_ em_GameObject_Type emType, _Out_ std::vector<T>& vlst) CONST
	{
		if (dwNode != NULL && ReadBYTE(dwNode + 0x21) == 0x0)
		{
			DWORD dwObj = ReadDWORD(dwNode + 0x18);

			if (emType == em_GameObject_Type_Monster)
			{
				if ((ReadDWORD(dwObj + 周围遍历分类偏移) & 0xFF) == 0x4 && (ReadDWORD(ReadDWORD(ReadDWORD(dwObj + 怪物属性偏移) + 0x10) + 0x0) & 0xFFFF) == 'M')
				{
					T Monster(dwObj);
					if (Monster.IsExist())
						vlst.push_back(Monster);
				}
			}
			else if (emType == em_GameObject_Type_Npc)
			{
				if ((ReadDWORD(dwObj + 周围遍历分类偏移) & 0xFF) == 0x4 && (ReadDWORD(ReadDWORD(ReadDWORD(dwObj + 怪物属性偏移) + 0x10) + 0x0) & 0xFFFF) == 'C')
				{
					T Npc(dwObj);
					if (Npc.IsExist())
						vlst.push_back(Npc);
				}
			}
			else if (emType == em_GameObject_Type_Player)
			{
				if ((ReadDWORD(dwObj + 周围遍历分类偏移) & 0xFF) == 0x1)
				{
					T Player(dwObj);
					if (Player.IsExist())
						vlst.push_back(Player);
				}
			}
			else if (emType == em_GameObject_Type_DropItem)
			{
				if ((ReadDWORD(dwObj + 周围遍历分类偏移) & 0xFF) == 0xB0)
				{
					T DropItem(dwObj);
					if (DropItem.IsExist())
						vlst.push_back(DropItem);
				}
			}
			else if (emType == em_GameObject_Type_Corpse)
			{
				if ((ReadDWORD(dwObj + 周围遍历分类偏移) & 0xFF) == 0x20)
				{
					T Corpse(dwObj);
					if (Corpse.IsExist())
						vlst.push_back(Corpse);
				}
			}
			else if (emType == em_GameObject_Type_MonsterCorpse)
			{
				//LOG_CF_D( L"em=%d", ReadDWORD(dwObj + 周围遍历分类偏移) & 0xFF);
				if ((ReadDWORD(dwObj + 周围遍历分类偏移) & 0xFF) == 0x90)
				{
					T Corpse(dwObj);
					if (Corpse.IsExist())
						vlst.push_back(Corpse);
				}
			}
			else if (emType == em_GameObject_Type_Pet)
			{
				if ((ReadDWORD(dwObj + 周围遍历分类偏移) & 0xFF) == 0x2)
				{
					T Corpse(dwObj);
					if (Corpse.IsExist())
						vlst.push_back(Corpse);
				}
			}
			else if (emType == em_GameObject_Type_ALL)
			{
				vlst.push_back(T(dwObj));
			}
			else if (emType == em_GameObject_Type_PracticePlayer)
			{
				if ((ReadDWORD(dwObj + 周围遍历分类偏移) & 0xFF) == 0xF0)
				{
					T Player(dwObj);
					if (Player.IsExist())
						vlst.push_back(Player);
				}
			}

			// 左树递归
			TraverseGameObject(ReadDWORD(dwNode + 0x0), emType, vlst);
			TraverseGameObject(ReadDWORD(dwNode + 0x8), emType, vlst);
		}
		return TRUE;
	}
};




#endif