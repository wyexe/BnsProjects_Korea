#include "stdafx.h"
#include "SwAccPkMgr.h"
#include <MyTools/Log.h>
#include "BnsConfig.h"
#include "GameFunction.h"
#include "LoginPlayerExtend.h"
#include "SomeFun.h"
#include "ConsoleLog.h"

#define _SELF L"SwAccPkMrg.cpp"

CSwAccPkMgr::CSwAccPkMgr(_In_ PSHARED_INFO PSHARED_INFO_, _In_ PACCOUNT_INFO_GAME PACCOUNT_INFO_GAME_)
{
	pShare = PSHARED_INFO_;
	pAccGame = PACCOUNT_INFO_GAME_;
}

CSwAccPkMgr::~CSwAccPkMgr()
{
}

BOOL CSwAccPkMgr::ReadPkConfig()
{
	CBnsConfig BnsConfig;

	/*PkScheduleConfig PkScheduleConfig_;
	if (BnsConfig.GetPkScheduleConfig(pAccGame->GetAccName(), PkScheduleConfig_, pShare))
	{
		pAccGame->PkSetting.emPkSchedule = (PkScheduleConfig_.emPkSchedule == em_Pk_Schedule_None ? em_Pk_Schedule_TaskUpgrade : PkScheduleConfig_.emPkSchedule);
		pAccGame->GetOrderIndex() = PkScheduleConfig_.uOrderIndex;
		LogMsgBox(LOG_LEVEL_NORMAL L"���ʺŴ���PK�����ļ�, �ϴεĽ����:%d->%s, ˢ����%d����ɫ", PkScheduleConfig_.emPkSchedule, GetPkScheduleText().c_str(), PkScheduleConfig_.uOrderIndex);
	}
	else
	{
		// ��ʼ����ֵ
		if (pAccGame->PkSetting.emPkSchedule == em_Pk_Schedule_None) // ���!="" ��ô�϶��ǵ��ߵ�, ��ȥ�޸��˳���־!
			pAccGame->PkSetting.emPkSchedule = em_Pk_Schedule_TaskUpgrade;

		LogMsgBox(LOG_LEVEL_NORMAL L"���ʺŲ�����PK�����ļ�, ���º�!");
	}*/
	return TRUE;
}

CONST std::wstring& CSwAccPkMgr::GetPkScheduleText()
{
	static  std::wstring wsText = L"";
	switch (pAccGame->PkSetting.emPkSchedule)
	{
	case em_Pk_Schedule_ActiveCard:
		wsText = L"�쿨�׶�";
		break;
	case em_Pk_Schedule_PK:
		wsText = L"PK�׶�";
		break;
	case em_Pk_Schedule_TaskUpgrade:
		wsText = L"�����׶�";
		break;
	case em_Pk_Schedule_AfterPK:
		wsText = L"PK��׶�";
		break;
	default:
		wsText = L"NULL";
		break;
	}
	return wsText;
}

BOOL CSwAccPkMgr::TaskUpgrade_PK()
{
	for (int i = 0; i < 5; ++i)
	{
		CConsoleLog(MyTools::CCharacter::MakeFormatText(L"�����׶�: %d����ɫ", i + 1));
		::Sleep(1000);

		em_LoginPlayerStatus emPlayerStatus = CLoginPlayerExtend::GetInstance().GetLoginPlayerStatus_By_Index(i);
		if (emPlayerStatus == em_LoginPlayer_UnExist) // û�н�ɫ
		{
			LOG_CF_D(L"������%d����ɫ", i + 1);
			for (int j = i; j < 5; ++j)
				CLoginPlayerExtend::GetInstance().CreatePlayer(pShare->ConsoleContent.GlobalAccount.dwAreaId, j, em_Player_Classes_Destroyer);

			i--;
			continue;
		}

		if (emPlayerStatus == em_LoginPlayer_Deleted) // ɾ��״̬�Ľ�ɫ������
			continue;

		DWORD dwLevel = CLoginPlayerExtend::GetInstance().GetLevel_By_Index(i);
		if (dwLevel > 15 && i == 4) // ���һ����ɫͻȻ16����, ������, ֱ������һ��ģʽ!
			break;
		else if (dwLevel > 15 && i == 0 && CGameFunction::GetInstance().GetDianKa() > 0) // ��¼ʱ�����1����ɫ��16�������һ�ȡʱ������ʱ��ģ���ô���Ǿ͹̶�����Ϊ���Ѿ������4�׶Σ����Ƕ�ӦPK�������
			break;
		else if (dwLevel > 15)
		{
			CConsoleLog(MyTools::CCharacter::MakeFormatText(L"�����׶�: %d����ɫ����15��,�ȴ�ɾ���С���", i + 1));
			CLoginPlayerExtend::GetInstance().DeletePlayer(i, 5 * 1000);
			if (CLoginPlayerExtend::GetInstance().GetLoginPlayerStatus_By_Index(i) == em_LoginPlayer_Normal)
			{
				SetLoginFaild(MyTools::CCharacter::MakeFormatText(L"�����׶ν�ɫ%d ����15��, ����ɾ����ɫ��Ч!", i + 1), em_LoginFaild_UnDeleteAccount);
			}

			if (CSomeFun::GetInstance().TimeOut_Condiction(6 * 60 * 1000 + 1 * 60 * 1000, [&i, this]{ return CLoginPlayerExtend::GetInstance().GetLoginPlayerStatus_By_Index(i) != em_LoginPlayer_UnExist; }))
			{
				SetLoginFaild(MyTools::CCharacter::MakeFormatText(L"�����׶ν�ɫ%d ����15��, ����ɾ����ɫ6������ʧ��!", i + 1), em_LoginFaild_UnDeleteAccount);
			}

			// ���´������ŵ��߼�
			i--;
			continue;
		}

		if (dwLevel >= 15 && i < (int)pAccGame->GetOrderIndex()) // i == 2   Index == 4
		{
			LOG_CF_D( L"�����׶�: %d������ȼ�:%d, ���ڽ��ȱ�,����!", i + 1, dwLevel, pAccGame->GetOrderIndex() + 1);
			continue;
		}

		// ����PK����
		pAccGame->GetOrderIndex() = i;
		SavePkConfig();

		CBnsGameFun::GetInstance().StartGame_By_Index(i);
		// ��ʼִ�нű�, ���ҵȴ��߳̽���!
		CBnsGameFun::WaitForThread(CBnsGameFun::em_Excute_Type_PK_TaskUpgrade);
		CBnsGameFun::GetInstance().BackToSwitchAccount();

		// ��Ϸ״̬���Զ�����С��
		if (CGameVariable::GetInstance().GetEnvironmentValue() != 0x2)
			CBnsGameFun::GetInstance().BackToSwitchAccount();

		// ����PK����
		pAccGame->GetOrderIndex() = i;
		SavePkConfig();
	}
	return TRUE;
}

BOOL CSwAccPkMgr::AccountPK_PK()
{
	BOOL bExitStage = FALSE;
	for (int i = 0; i < 5 && !bExitStage; ++i)
	{
		CConsoleLog(MyTools::CCharacter::MakeFormatText(L"%s�׶�: %d����ɫ", pAccGame->PkSetting.emPkSchedule == em_Pk_Schedule_PK ? L"PK" : L"PK��", i + 1));

		::Sleep(1000);

		em_LoginPlayerStatus emPlayerStatus = CLoginPlayerExtend::GetInstance().GetLoginPlayerStatus_By_Index(i);
		if (emPlayerStatus == em_LoginPlayer_UnExist) // û�н�ɫ
		{
			LOG_CF_D( L"������%d����ɫ", i + 1);
			for (int j = i; j < 5; ++j)
				CLoginPlayerExtend::GetInstance().CreatePlayer(pShare->ConsoleContent.GlobalAccount.dwAreaId, j, em_Player_Classes_Destroyer);
			i--;
			continue;
		}

		if (emPlayerStatus == em_LoginPlayer_Deleted) // ɾ��״̬�Ľ�ɫ������
			continue;

		// �жϵ㿨
		int nMinutes = CGameFunction::GetInstance().GetDianKa();
		if (nMinutes <= MINTIME_DIANKA)
		{
			SetLoginFaild(MyTools::CCharacter::MakeFormatText(L"ˢ����! ʣ��ʱ��:%d����", nMinutes), em_LoginFaild_DeleteAccount);
		}

		// ��ȡ��ǰ��i����ɫ�ĵȼ�, ������ʺŵĵȼ� < 16���� ���ҵ��ڽ��ȼ�¼�� ��ô�����ӽ��ȼ�¼�� ����ִ��
		DWORD dwLevel = CLoginPlayerExtend::GetInstance().GetLevel_By_Index(i);
		if (dwLevel >= 16 && i < (int)pAccGame->GetOrderIndex()) // i == 2   Index == 4
		{
			//LogMsgBox(LOG_LEVEL_NORMAL L"%s�׶�: %d������ȼ�:%d, ���ڽ��ȱ�,����!", pAccGame->PkSetting.emPkSchedule == em_Pk_Schedule_PK ? L"PK" : L"PK��", i + 1, dwLevel, pAccGame->GetOrderIndex() + 1);
			continue;
		}
		
		//5.��PK����ִ�е���5����ɫʱ�����ǵ�4����ɫ���ʱ����ǰ��4����ɫɾ�������Ҽ�¼ʱ�䡣6���Ӻ󽫵�ǰPK����ִ�еĽ�ɫλ����1���˺�ˢ�굱ǰ��ɫ��PK������ʵ�����Ѿ���5����ɫ��ˢ���ˡ�������һ�׶Ρ�
		/*if (i == 4)
		{
			for (int i = 0; i < 4; ++i)
				CSomeFun::DeletePlayer(i, 3 * 1000);

			pAccGame->GetOrderIndex() = 0;
			SavePkConfig();
			bExitStage = TRUE;
		}
		else
		{
			// ����PK����
			pAccGame->GetOrderIndex() = i;
			SavePkConfig();
		}*/
		pAccGame->GetOrderIndex() = i;
		SavePkConfig();

		// ѡ���ɫ -> ��Ϸ����
		CBnsGameFun::GetInstance().StartGame_By_Index(i);

		// ��ʼִ�нű�, ���ҵȴ��߳̽���!
		CBnsGameFun::WaitForThread(CBnsGameFun::em_Excute_Type_PK_PK);

		CBnsGameFun::GetInstance().BackToSwitchAccount();

		pAccGame->GetOrderIndex() = i + 1;
		SavePkConfig();
	}
	return TRUE;
}

BOOL CSwAccPkMgr::AfterPK_PK()
{
	int nMinutes = CGameFunction::GetInstance().GetDianKa();
	if (nMinutes <= MINTIME_DIANKA)
	{
		SetLoginFaild(MyTools::CCharacter::MakeFormatText(L"ˢ����! ʣ��ʱ��:%d����", nMinutes), em_LoginFaild_DeleteAccount);
	}

	// ���׶λص�����׶�!
	pAccGame->GetOrderIndex() = 1;
	pAccGame->PkSetting.emPkSchedule = em_Pk_Schedule_PK;
	return TRUE;
}

BOOL CSwAccPkMgr::Run(std::function<BOOL(void)> fnExit)
{
	/*if (!CBnsGameFun::InitGame([this](CONST std::wstring& wsLog){ return CBnsConfig::WriteAccountLog(pShare, pAccGame, wsLog.c_str()); }))
	{
		LogMsgBox(LOG_LEVEL_NORMALL"InitGame Faild!");
		return FALSE;
	}

	if (!CBnsGameFun::IntoSwtichPlayer([this](CONST std::wstring& wsLog){ return CBnsConfig::WriteAccountLog(pShare, pAccGame, wsLog.c_str()); }))
	{
		LogMsgBox(LOG_LEVEL_NORMAL L"IntoSwtichPlayer Faild!");
		return FALSE;
	}

	int nIndex = 0;
	 std::wstring wsPlayerName;
	if (!CBnsGameFun::GetInstance().DeleteAccount_By_Wps(nIndex, wsPlayerName))
	{
		CBnsConfig::GetInstance().WriteAccountLog( L"��ɫ%d ������: %s �����Լ��ĺ�!", nIndex, wsPlayerName.c_str());
		SetLoginFaild(em_LoginFaild_DeleteAccount);
		return FALSE;
	}*/
	return SwitchAccProc(fnExit);
}

BOOL CSwAccPkMgr::SwitchAccProc(std::function<BOOL(void)> fnExit)
{
	// ��ȡ����
	ReadPkConfig();

	BOOL bExit = FALSE;
	while (!bExit)
	{
		::Sleep(1000);
		switch (pAccGame->PkSetting.emPkSchedule)
		{
		case em_Pk_Schedule_TaskUpgrade:
			// 1.������15�����л���ɫֻ�жϽ�ɫ�Ƿ�15�����Ƿ������굱ǰ�����������Ƿ�15��Ϊ��־����ɫλ��++��ֱ��5����ɫȫ��15������ɫλ�ñ�����ini���¼���Է���������������жϵ����ݡ�
			TaskUpgrade_PK();
			pAccGame->PkSetting.emPkSchedule = em_Pk_Schedule_ActiveCard;
			pAccGame->GetOrderIndex() = 0;
			break;
		case em_Pk_Schedule_ActiveCard:
			// 2.�Զ��쿨����ini��Ľ�ɫλ����1
			if (ActiveCard() > 0)
			{
				pAccGame->PkSetting.emPkSchedule = em_Pk_Schedule_PK;
				pAccGame->GetOrderIndex() = 0;
			}
			else
			{
				//CBnsConfig::WriteAccountLog(pShare, pAccGame, L"�쿨��,�㿨ʱ��=0!");
				SetLoginFaild(L"�쿨��,�㿨ʱ��=0!", em_LoginFaild_DeleteAccount);
			}
			break;
		case em_Pk_Schedule_PK:
			// 4.PK����ִ�У��ж�������������2��PK������ɣ�ȥ�ʼġ��ʼ�ִ����ɺ����жϵ�ǰ�������Ƿ����ʼ���Ʒ��ֱ���ʼĳ����ɫλ��++
			AccountPK_PK();
			SetLoginFaild(L"ˢ��5��PK��ɫ!", em_LoginFaild_DeleteAccount);
			break;
		case em_Pk_Schedule_AfterPK:
			AfterPK_PK();
			break;
		default:
			LOG_MSG_CF( L"δ֪��emPkSchedule=%X", pAccGame->PkSetting.emPkSchedule);
			bExit = TRUE;
			break;
		}
		// �������
		bExit = fnExit();
		SavePkConfig();
	}
	//SetLoginFaild(em_LoginFaild_DeleteAccount);
	return TRUE;
}

BOOL CSwAccPkMgr::SavePkConfig()
{
	return TRUE;
}

int CSwAccPkMgr::ActiveCard()
{
	int nMinuite = CGameFunction::GetInstance().GetDianKa();
	if (nMinuite > 0)
		return nMinuite;

	WCHAR wszReceiveCardPath[MAX_PATH] = { 0 };
	swprintf_s(wszReceiveCardPath, _countof(wszReceiveCardPath) - 1, L"%s\\CopyFile\\ReceiveCard.dll", CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.szConsolePath);
	if (!MyTools::CLPublic::FileExit(wszReceiveCardPath))
	{
		LOG_MSG_CF( L"ȱ��ReceiveCard.dll!");
		return 0;
	}

	HMODULE hmDLL = ::LoadLibraryW(wszReceiveCardPath);
	if (hmDLL == NULL)
	{
		LOG_MSG_CF( L"ReceiveCard.dll�쳣!");
		return 0;
	}

	typedef int(WINAPIV* pfnActiveCard)(void);
	pfnActiveCard fnActiveCard = (pfnActiveCard)::GetProcAddress(hmDLL, "ActiveCard");
	if (fnActiveCard == nullptr)
	{
		LOG_MSG_CF( L"ReceiveCard.dll���������쳣!");
		return 0;
	}

	// ���ļ�, �Ƿ��쿨�ɹ�!
	CConsoleLog(L"�쿨�С���");
	int nRetCode = fnActiveCard();
	if (nRetCode == 0)
	{
		//CBnsConfig::GetInstance().WriteAccountLog( L"�쿨ʧ��!");
		return 0;
	}

	nMinuite = CGameFunction::GetInstance().GetDianKa();
	if (nMinuite <= MINTIME_DIANKA)
	{
		//CBnsConfig::GetInstance().WriteAccountLog( L"�㿨ʱ�䲻��! ʣ��ʱ��:%d����", nMinuite);
		return 0;
	}

	return nMinuite;
}
