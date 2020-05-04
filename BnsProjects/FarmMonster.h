#ifndef __GITBNSPROJECTS_BNSPROJECTS_SOMEFUN_FARM_FARMMONSTER_H__
#define __GITBNSPROJECTS_BNSPROJECTS_SOMEFUN_FARM_FARMMONSTER_H__

#include "GameVariable.h"
/************************************************************************/
/* �����Ĵ�ֹ���, ֻ�е�����ɱ��, ���ṩѭ��!                                                                     */
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
		// ����
		em_KillMonster_Round_Mode_KFFM,
		// ��ͨ�ٻ�ģʽ
		em_KillMonster_Round_Mode_SUMMONER,
		// ѩ�񹬱�����ģʽ
		em_KillMonster_Round_Mode_XYG_IceFireBird,
		// ����ʦ
		em_KillMonster_Round_Mode_WarLock
	};

public:
	// �ƶ��������Ա�
	BOOL Move_To_Monster(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo,_In_ BOOL Summoner_flag = TRUE) CONST;

	VOID SetMoveToMonster(_In_ BOOL bMove);

	// ʹ��AOE���
	BOOL KillMonster_By_AOE(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo, _In_ BOOL bIsForce, _In_ std::function<UINT(VOID)> fnGetAroundCount) CONST;

	// ʹ�õ��弼�����
	BOOL KillMonster_By_Single(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo, _In_ BOOL bIsForce, _In_ std::function<UINT(VOID)> fnGetAroundCount) CONST;

	// ɱ��
	BOOL KillMonster_By_Dog(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	// ����������ĳ�������ʱ��, ����S����
	BOOL BackwardWhenClosing(_In_ CONST CMonster& Monster, _In_ BOOL bRun, _In_ float fClosingDis, _In_ float fBackwardDis) CONST;

	// �ٻ��ٻ���(���ϰ�E)
	BOOL CallBackCat(_In_ BOOL bRun) CONST;

	//////��ʿ////////////////////////////////////////////////////////////////////
	// ��ʿ���弼��ɱ��
	BOOL Lux_Single(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo, _In_ BOOL bIsForce, _In_ std::function<UINT(VOID)> fnGetAroundCount) CONST;

	// ��ʿAOE����ɱ��
	BOOL Lux_AOE(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo, _In_ BOOL bIsForce, _In_ std::function<UINT(VOID)> fnGetAroundCount) CONST;

	// ��ʿPK
	BOOL PK_Lux(_In_ CONST CPlayer& Player, _In_ BOOL bLastResult, _In_ std::function<BOOL(VOID)> fnTerminate) CONST;

	// ��ʿPK_new
	BOOL PK_Lux_Pk( _In_ CONST CPlayer& Player, _In_ BOOL bLastResult, _In_ std::function<BOOL(VOID)> fnTerminate) CONST;

	// ��ʿʹ��V
	BOOL UseLux_V(_In_ CONST CMonster& Monster) CONST;

	// ʹ����ʿ�ļ���
	BOOL UseLuxSkill(_In_ CONST CMonster& Monster, _In_ BOOL bKaDaoMode) CONST;

	//////����////////////////////////////////////////////////////////////////////
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

	////////�ٻ�ʦ//////////////////////////////////////////////////////////////////
	BOOL Summoner_Singe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	BOOL Summoner_UseSkill(_In_ CONST CMonster& Monster) CONST;

	BOOL Summoner_Round(_In_ CONST CMonster& Monster, _In_ CONST CCat& newcaat) CONST;

	BOOL Summoner_KillDog(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	BOOL Pk_Summoner(_In_ CONST CPlayer& Player, _In_ CONST FixPointInfo& Round, _In_ std::function<BOOL(VOID)> fnTerminate) CONST;

	BOOL BaiXianPratice_Summoner(_In_ CONST CPlayer& Player, _In_ CONST FixPointInfo& Round, _In_ std::function<BOOL(VOID)> fnTerminate) CONST;

	BOOL CallBackCat() CONST;

	VOID CallBackCat_Time(_In_ DWORD dwTime) CONST;
		
	// ѩ�� ������
	BOOL XueYuGong_IceFireBird(_In_ CONST CMonster& Monster, _In_ CONST CCat& newcaat) CONST;

	////����ʦ//////////////////////////////////////////////////////////////////////0xE1,
	BOOL Warlock_Singe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	BOOL Warlock_Aoe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	BOOL Warlock_Round(_In_ CONST CMonster& Monster, _In_ CONST CCat& newcaat) CONST;


	////��ʿ//////////////////////////////////////////////////////////////////////
	BOOL BladeMaster_Singe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	BOOL BladeMaster_Aoe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	BOOL BladeMaster_Round(_In_ CONST CMonster& Monster, _In_ CONST CCat& newcaat) CONST;

	////����//////////////////////////////////////////////////////////////////////
	
	BOOL ForceMaster_Singe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	BOOL ForceMaster_Round(_In_ CONST CMonster& Monster, _In_ CONST CCat& newcaat) CONST;

	////�̿�//////////////////////////////////////////////////////////////////////

	BOOL Assassin_Singe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST;
	
	////�齣ʿ//////////////////////////////////////////////////////////////////////
	BOOL Danncer_Singe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	BOOL Danncer_Aoe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	////ȭʦ//////////////////////////////////////////////////////////////////////
	BOOL KongFuMaster_Singe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	////ǹ��//////////////////////////////////////////////////////////////////////
	BOOL Gunner_Singe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	BOOL Gunner_Aoe(_In_ CONST CMonster& Monster, _In_ CONST FixPointInfo& FixPtInfo) CONST;
private:
	DSIABLE_COPY_AND_ASSIGN(CFarmMonster)
private:
	BOOL _bMove;
};


#endif