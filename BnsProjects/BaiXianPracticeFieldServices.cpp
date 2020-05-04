#include "stdafx.h"
#include "BaiXianPracticeFieldServices.h"
#include <MyTools/Log.h>
#include "ObjectFunction.h"
#include "GameUI.h"
#include "Person.h"
#include "SomeFun.h"
#include "GameSend.h"
#include "Player.h"
#include "PlayerExtend.h"
#include "PersonFunction.h"
#include "Farm.h"
#include "BagFunction.h"
#include "BnsConfig.h"
#include "Monster.h"
#include "GameFunction.h"

#define _SELF L"BaiXianPracticeFieldServices.cpp"

CONST static Point CityCenterPoint(-53735, -151, -8773);

DWORD CBaiXianPracticeFieldServices::GetPracticeInfoAddrByClasses(_In_ em_Player_Classes emPlayerClasses) CONST
{
	DWORD dwAddr = ReadDWORD(ReadDWORD(ReadDWORD(�����ַ) + �����ַƫ��1) + ����������ƫ��3);
	// Index=dwClasses * 8 - dwClasses = dwClasses * 7 
	return dwAddr + (static_cast<DWORD>(emPlayerClasses)* 8 - static_cast<DWORD>(emPlayerClasses)) * 4;
}

DWORD CBaiXianPracticeFieldServices::GetPracticeStageByClasses(_In_ em_Player_Classes emPlayerClasses) CONST
{
	DWORD dwAddr = GetPracticeInfoAddrByClasses(emPlayerClasses);
	for (int i = 1; i <= 10; ++i)
	{
		if ((ReadDWORD(dwAddr + ��������������ƫ�� + (i - 1) * 2) & 0xFF) != 0x3)
		{
			return static_cast<DWORD>(i);
		}
	}
	return 0;
}

DWORD CBaiXianPracticeFieldServices::GetPracticePrizesByClasses(_In_ em_Player_Classes emPlayerClasses) CONST
{
	DWORD dwAddr = GetPracticeInfoAddrByClasses(emPlayerClasses);
	for (int i = 1; i <= 10; ++i)
	{
		WORD wStageAndPrizes = ReadDWORD(dwAddr + ��������������ƫ�� + (i - 1) * 2) & 0xFFFF;
		if (((wStageAndPrizes >> 0x8) & 0xFF) != (wStageAndPrizes & 0xFF))
		{
			return i;
		}
	}
	return 0;
}

BOOL CBaiXianPracticeFieldServices::AcceptPracticePrizesByClassesAndIndex(_In_ em_Player_Classes emPlayerClasses, _In_ DWORD dwIndex) CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&emPlayerClasses, &dwIndex]
	{
		CGameSend::AcceptPrizesForBaiXianPracticeField(static_cast<DWORD>(emPlayerClasses), dwIndex);
	});
}

BOOL CBaiXianPracticeFieldServices::ExistShowChoiceHero() CONST
{
	return CObjectFunction::GetInstance().IsShow_In_GameUi(L"DuelBotTrainingSelectJobPanel");
}

BOOL CBaiXianPracticeFieldServices::ExistShowTrainingResult() CONST
{
	return CObjectFunction::GetInstance().IsShow_In_GameUi(L"DuelBotTrainingResultPanel");
}

BOOL CBaiXianPracticeFieldServices::Begin() CONST
{
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_BAIXIAN_DONE, FALSE);
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NMAE_ISPUASEHELPTHREAD, TRUE);

	ULONGLONG ulTick = ::GetTickCount64();
	while (GameRun && !CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_BAIXIAN_DONE))
	{
		while (GameRun && !EMPTY_PERSONHP && CPerson::GetInstance().GetMapId() != 0x1735)
		{
			LOG_CF_D( L"��ǰ��ͼ���ǰ���������!, dwMapId=%X", CPerson::GetInstance().GetMapId());
			if (CPerson::GetInstance().GetDis(Point(-54112, -616, -8776)) > 30.0f)
			{
				LOG_MSG_CF( L"��ǰ�������������Զ:%.2f��������!", CPerson::GetInstance().GetDis(Point(-54112, -616, -8776)));
				CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NMAE_ISPUASEHELPTHREAD, FALSE);
				return FALSE;
			}
			MoveToBaiXianPracticeField();
		}

		LOG_CF_D( L"��ʼ����!");
		BeginPracticeLoop(ulTick);
		if (IsTimeOut(ulTick))
		{
			LOG_CF_E( L"����ѵ������ʱ!");
			break;
		}
	}
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NMAE_ISPUASEHELPTHREAD, FALSE);
	return TRUE;
}

BOOL CBaiXianPracticeFieldServices::BeginPracticeLoop(_In_ CONST ULONGLONG& ulTick) CONST
{
	while (GameRun && !IsTimeOut(ulTick))
	{
		if (!ChoiceHero())
		{
			LOG_CF_D( L"ѡ��Ӣ�����! �˳�!");
			break;
		}

		// �ȴ��о�����!
		CPlayer Player;
		if (CSomeFun::GetInstance().TimeOut_Condiction_GameRun(10 * 1000, [&Player]{
			return !CPlayerExtend::GetInstance().GetTopPraticeNpc_By_Dis(CBaseObject::CreateFixPointInfo(50.0f), &Player);
		}))
		{
			LOG_CF_D( L"10s֮�ڶ��޷��ҵ�ѵ�����Ķ���!, ŭ��!");
			break;
		}

		// �ȴ�ս����ʼ!
		BnsSleep(4 * 1000);
		Kill(Player);

		// 10s֮���жϽ����Ƿ����
		if (CSomeFun::GetInstance().TimeOut_Condiction_GameRun(10 * 1000, [this]{ return !ExistShowTrainingResult(); }))
		{
			LOG_CF_E( L"10s֮�ڶ��޷����ֽ�������ҳ��!�˳�!");
			break;
		}

		// �ص�ѡ��Ӣ��ҳ��
		CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"DuelBotTrainingResultPanel", [](CONST CGameUi& DuelBotTrainingResultPanel)
		{
			DuelBotTrainingResultPanel.SetUiShow(FALSE);
			BnsSleep(1000);
		});

		CObjectFunction::GetInstance().Action_By_GameUiName(L"DuelBotTrainingSelectJobPanel", [](CONST CGameUi& DuelBotTrainingSelectJobPanel)
		{
			DuelBotTrainingSelectJobPanel.SetUiShow(TRUE);
			BnsSleep(1000);
		});
		if (!ExistShowChoiceHero())
		{
			LOG_CF_D( L"�޷���ѡ��Ӣ��UI��ʾ����! �˳�!");
			break;
		}

		if (CBagFunction::GetInstance().GetEndurance() <= 10)
		{
			LOG_CF_D( L"�����ĳ־öȲ���!, �˳�����!");
			break;
		}
	}

	// �˳�����ѵ����
	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([]{ CGameSend::ExitBaiXianPracticeField(); });
	BnsSleep(5 * 1000);
	return CPersonFunction::GetInstance().WaitGameLoad();
}

BOOL CBaiXianPracticeFieldServices::ChoiceHero() CONST
{
	CONST static std::vector<em_Player_Classes> PlayerClassesVec = {
		em_Player_Classes::em_Player_Classes_BladeMaster, em_Player_Classes::em_Player_Classes_KongFuMaster, 
		em_Player_Classes::em_Player_Classes_Destroyer, em_Player_Classes::em_Player_Classes_ForceMaster,
		em_Player_Classes::em_Player_Classes_Danncer, em_Player_Classes::em_Player_Classes_Assassin,
		em_Player_Classes::em_Player_Classes_Summoner, em_Player_Classes::em_Player_Classes_Warlock, 
		em_Player_Classes::em_Player_Classes_KongFuForceMaster
	};

	// wait to show DuelBotTrainingSelectJobPanel
	if (CSomeFun::GetInstance().TimeOut_Condiction_GameRun(10 * 1000, []{
		return !CObjectFunction::GetInstance().IsShow_In_GameUi(L"DuelBotTrainingSelectJobPanel");
	}))
	{
		LOG_CF_E( L"10s֮�ڶ��޷�����ѡ��Ӣ��ѡ��!");
		return FALSE;
	}

	BOOL bFinish = FALSE;
	for (DWORD i = 1; i <= 10 && GameRun && !bFinish; ++i)
	{
		UINT uHeroIndex = 0;
		for (CONST auto& itm : PlayerClassesVec)
		{
			// �����жϸ�ְҵ�Ľ�����ľ����ȡ��
			uHeroIndex += 1;
			if (itm == em_Player_Classes_Summoner)
				continue;

			DWORD dwPrizesIndex = GetPracticePrizesByClasses(itm);
			while (GameRun && dwPrizesIndex != 0)
			{
				LOG_CF_D( L"��ȡְҵ:%s �ĵ�%d����Ʒ!", CPlayer::GetTextByPlayerClasses(itm).c_str(), dwPrizesIndex);
				AcceptPracticePrizesByClassesAndIndex(itm, dwPrizesIndex);
				BnsSleep(3 * 1000);
				dwPrizesIndex = GetPracticePrizesByClasses(itm);
			}

			// �жϸ�ְҵ�Ƿ����������
			DWORD dwStage = GetPracticeStageByClasses(itm);
			if (dwStage == 0)
			{
				LOG_CF_D( L"ְҵ:%s �Ѿ��������������!", CPlayer::GetTextByPlayerClasses(itm).c_str());
				bFinish = TRUE;
				break;
			}
			if (dwStage > i)
				continue;

			// ����1�׵���ʿ�Ͳ���
			if (itm == em_Player_Classes_Destroyer && dwStage > 1)
				continue;

			DWORD dwStep = CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_BAIXIAN_STAGE_STEP);
			if (dwStage > dwStep)
			{
				LOG_CF_D( L"ְҵ:%s ��ǰ������:%d �Ѿ��������õĽ���:%d, ���������!", CPlayer::GetTextByPlayerClasses(itm).c_str(), dwStage, dwStep);
				bFinish = TRUE;
				break;
			}

			if (!CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"DuelBotTrainingSelectJobPanel", [&dwStage, &itm, &uHeroIndex](CONST CGameUi& DuelBotTrainingSelectJobPanel){
				LOG_CF_D( L"ѡ��ְҵ:%s, Ӣ��:%d ����:%d", CPlayer::GetTextByPlayerClasses(itm).c_str(), uHeroIndex, dwStage);
				CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&DuelBotTrainingSelectJobPanel, &dwStage, &uHeroIndex]{
					CGameSend::BeckonHeroForBaiXianPracticeField(DuelBotTrainingSelectJobPanel.GetObj(), uHeroIndex - 1, dwStage - 1);
				});
			}))
			{
				LOG_CF_E( L"�޷�������UI:DuelBotTrainingSelectJobPanel, û�������ִ��ѡӢ�۲���!");
				break;
			}

			return TRUE;
		}
	}

	if (GameRun && !EMPTY_PERSONHP)
	{
		LOG_CF_D( L"����ְҵ�Ѿ��������������!");
		CBnsConfig::BnsConfigContent ConfigContent_;
		ConfigContent_.wsConfigName = CLOUD_CONFIG_NAME_BAIXIAN_SECHEDULE;
		ConfigContent_.wsConfigValue = L"TRUE";
		CBnsConfig::GetInstance().SetSingleConfigValueByServer(ConfigContent_);
		CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_BAIXIAN_DONE, TRUE);
	}

	return FALSE;
}

BOOL CBaiXianPracticeFieldServices::Kill(_In_ CONST CPlayer& Player) CONST
{
	UINT uCount = 0;
	CSomeFun::GetInstance().SetSpeed(3.0f);
	if (CPerson::GetInstance().GetClasses() == em_Player_Classes::em_Player_Classes_Destroyer)
		CFarm::GetInstance().PK_Lux(Player, FALSE, [this, &uCount]{return uCount++ % 10 == 0 ? ExistShowTrainingResult() : FALSE; });
	else if (CPerson::GetInstance().GetClasses() == em_Player_Classes::em_Player_Classes_Summoner)
		CFarm::GetInstance().BaiXianPratice_Summoner(Player, FixPointInfo(Point(-54176, -48592, 6480), 5), [this, &uCount]{return uCount++ % 10 == 0 ? ExistShowTrainingResult() : FALSE; });
		
		

	while (GameRun && EMPTY_PERSONHP && BnsSleep(1000));
	return BnsSleep(3 * 1000);
}

BOOL CBaiXianPracticeFieldServices::MoveToBaiXianPracticeField() CONST
{
	CONST static Point RepairPoint(-53891, 335, -8773);

	CBag Hammer;
	if (CBagFunction::GetInstance().GetEndurance() <= 10 && CBagFunction::GetInstance().ExistHammer(&Hammer))
	{
		LOG_CF_D( L"�����ĳ־öȲ���!");
		BnsMove(RepairPoint, 1);
		BnsSleep(2 * 1000);
		CPersonFunction::GetInstance().WaitToLoadProgressBar(2 * 1000, 20 * 1000, [&]{Hammer.UseItem(); });
		LOG_CF_D( L"�������!");
	}
	BnsMove(CityCenterPoint, 1);
	BnsMove(Point(-54312, -796, -8773), 1);
	return CPersonFunction::GetInstance().WaitGameLoad();
}

BOOL CBaiXianPracticeFieldServices::IsTimeOut(_In_ CONST ULONGLONG& ulTick) CONST
{
	return static_cast<DWORD>(::GetTickCount64() - ulTick) >= CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_BAIXIAN_TIMEOUT) * 1000;
}

