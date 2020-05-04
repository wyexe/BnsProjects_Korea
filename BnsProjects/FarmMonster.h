#ifndef __GITBNSPROJECTS_BNSPROJECTS_SOMEFUN_FARM_FARMMONSTER_H__
#define __GITBNSPROJECTS_BNSPROJECTS_SOMEFUN_FARM_FARMMONSTER_H__

#include "GameVariable.h"
/************************************************************************/
/* 基础的打怪功能, 只有单独的杀怪, 不提供循环!                                                                     */
/************************************************************************/

class CMonster;
class CPlayer;
class CSkill;
class CCat;
class CFarmMonster
{
public:
	CFarmMonster();
	~CFarmMonster() = default;

	// class Player std::vector to std::vector<T>
	template<typename T>
	static BOOL vPlayerTo(_In_ std::vector<CPlayer>& vPlayer, _Out_ std::vector<T>& vlst)
	{
		vlst.clear();
		for (CONST auto& itm : vPlayer)
		{
			T tmpT(itm);
			tmpT.GetDis();
			vlst.push_back(T);
		}
		sort(vlst.begin(), vlst.end());
		return TRUE;
	}

	template<typename T>
	static BOOL TovMonster(_In_ std::vector<T> vlst, _Out_ std::vector<CMonster>& vMonster)
	{
		vMonster.clear();
		for (CONST T& itm : vlst)
		{
			CMonster Monster(itm);
			Monster.GetDis();
			vMonster.push_back(Monster);
		}
		sort(vMonster.begin(), vMonster.end());
		return TRUE;
	}


	template<typename T>
	static BOOL SortList_By_PersonDis(_In_ std::vector<T> vlst, _In_ float fMaxDis)
	{
		for (auto& itr = vlst.begin(); itr != vlst.end();)
		{
			float fDis = itr->GetDis();
			if (fDis < fMaxDis)
			{
				itr->SetDis(fDis);
				itr++;
			}
			else
			{
				itr = vlst.erase(itr);
			}
		}
		sort(vlst.begin(), vlst.end());
		return TRUE;
	}
public:
	enum em_KillMonster_Round_Mode
	{
		// 气宗
		em_KillMonster_Round_Mode_KFFM,
		// 普通召唤模式
		em_KillMonster_Round_Mode_SUMMONER,
		// 雪玉宫冰火鸟模式
		em_KillMonster_Round_Mode_XYG_IceFireBird,
		// 咒术师
		em_KillMonster_Round_Mode_WarLock
	};

public:
	// 移动到怪物旁边
	BOOL Move_To_Monster(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo,_In_ BOOL Summoner_flag = TRUE) CONST;

	VOID SetMoveToMonster(_In_ BOOL bMove);

	// 使用AOE清怪
	BOOL KillMonster_By_AOE(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo, _In_ BOOL bIsForce, _In_ std::function<UINT(VOID)> fnGetAroundCount) CONST;

	// 使用单体技能清怪
	BOOL KillMonster_By_Single(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo, _In_ BOOL bIsForce, _In_ std::function<UINT(VOID)> fnGetAroundCount) CONST;

	// 杀狗
	BOOL KillMonster_By_Dog(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	// 开启当靠近某个怪物的时候, 就用S后退
	BOOL BackwardWhenClosing(_In_ CONST CMonster& Monster, _In_ BOOL bRun, _In_ float fClosingDis, _In_ float fBackwardDis) CONST;

	// 召回召唤兽(不断按E)
	BOOL CallBackCat(_In_ BOOL bRun) CONST;

	//////力士////////////////////////////////////////////////////////////////////
	// 力士单体技能杀怪
	BOOL Lux_Single(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo, _In_ BOOL bIsForce, _In_ std::function<UINT(VOID)> fnGetAroundCount) CONST;

	// 力士AOE技能杀怪
	BOOL Lux_AOE(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo, _In_ BOOL bIsForce, _In_ std::function<UINT(VOID)> fnGetAroundCount) CONST;

	// 力士PK
	BOOL PK_Lux(_In_ CONST CPlayer& Player, _In_ BOOL bLastResult, _In_ std::function<BOOL(VOID)> fnTerminate) CONST;

	// 力士PK_new
	BOOL PK_Lux_Pk( _In_ CONST CPlayer& Player, _In_ BOOL bLastResult, _In_ std::function<BOOL(VOID)> fnTerminate) CONST;

	// 力士使用V
	BOOL UseLux_V(_In_ CONST CMonster& Monster) CONST;

	// 使用力士的技能
	BOOL UseLuxSkill(_In_ CONST CMonster& Monster, _In_ BOOL bKaDaoMode) CONST;

	//////气宗////////////////////////////////////////////////////////////////////
	enum { em_Pose_KongFuMaster = 0x1, em_Pose_ForeMaster = 0x2 };
	BOOL Kffm_AOE(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo, _In_ BOOL bIsForce, _In_ std::function<UINT(VOID)> fnGetAroundCount) CONST;

	BOOL Kffm_Singe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	BOOL Pk_Kffm(_In_ CPlayer& Player, _In_ CONST FixPointInfo& RoundPoint, _In_ BOOL bLastResult, _In_ std::function<BOOL(VOID)> fnTerminte) CONST;

	BOOL Kffm_KillDog(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	VOID SetPlayerClassesRoundMode() CONST;

	BOOL KillMonster_By_Round(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo, _In_ CONST FixPointInfo& RoundPoint) CONST;

	BOOL GetRoundPointList(_In_ CONST FixPointInfo& RoundPoint, _In_ UINT uCount, _Out_ std::vector<Point>& vlst) CONST;

	BOOL ExistSpecialMonster(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	BOOL IsLoseHp(_In_ CONST CMonster& Monster, _In_ DWORD& dwLastHp, _In_ ULONGLONG& ulTick, _In_ ULONGLONG ulMaxTimeOut = 10 * 1000) CONST;

	BOOL Kffm_Fm_Round(_In_ CONST CMonster& Monster) CONST;

	////////召唤师//////////////////////////////////////////////////////////////////
	BOOL Summoner_Singe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	BOOL Summoner_UseSkill(_In_ CONST CMonster& Monster) CONST;

	BOOL Summoner_Round(_In_ CONST CMonster& Monster, _In_ CONST CCat& newcaat) CONST;

	BOOL Summoner_KillDog(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	BOOL Pk_Summoner(_In_ CONST CPlayer& Player, _In_ CONST FixPointInfo& Round, _In_ std::function<BOOL(VOID)> fnTerminate) CONST;

	BOOL BaiXianPratice_Summoner(_In_ CONST CPlayer& Player, _In_ CONST FixPointInfo& Round, _In_ std::function<BOOL(VOID)> fnTerminate) CONST;

	BOOL CallBackCat() CONST;

	VOID CallBackCat_Time(_In_ DWORD dwTime) CONST;
		
	// 雪玉宫 冰火鸟
	BOOL XueYuGong_IceFireBird(_In_ CONST CMonster& Monster, _In_ CONST CCat& newcaat) CONST;

	////咒术师//////////////////////////////////////////////////////////////////////0xE1,
	BOOL Warlock_Singe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	BOOL Warlock_Aoe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	BOOL Warlock_Round(_In_ CONST CMonster& Monster, _In_ CONST CCat& newcaat) CONST;


	////剑士//////////////////////////////////////////////////////////////////////
	BOOL BladeMaster_Singe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	BOOL BladeMaster_Aoe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	BOOL BladeMaster_Round(_In_ CONST CMonster& Monster, _In_ CONST CCat& newcaat) CONST;

	////气功//////////////////////////////////////////////////////////////////////
	
	BOOL ForceMaster_Singe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	BOOL ForceMaster_Round(_In_ CONST CMonster& Monster, _In_ CONST CCat& newcaat) CONST;

	////刺客//////////////////////////////////////////////////////////////////////

	BOOL Assassin_Singe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST;
	
	////灵剑士//////////////////////////////////////////////////////////////////////
	BOOL Danncer_Singe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	BOOL Danncer_Aoe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	////拳师//////////////////////////////////////////////////////////////////////
	BOOL KongFuMaster_Singe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	////枪手//////////////////////////////////////////////////////////////////////
	BOOL Gunner_Singe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	BOOL Gunner_Aoe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST;
private:
	DSIABLE_COPY_AND_ASSIGN(CFarmMonster)
private:
	BOOL _bMove;
};


#endif