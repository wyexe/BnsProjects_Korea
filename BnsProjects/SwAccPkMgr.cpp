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
		LogMsgBox(LOG_LEVEL_NORMAL L"此帐号存在PK配置文件, 上次的结果是:%d->%s, 刷到第%d个角色", PkScheduleConfig_.emPkSchedule, GetPkScheduleText().c_str(), PkScheduleConfig_.uOrderIndex);
	}
	else
	{
		// 初始化赋值
		if (pAccGame->PkSetting.emPkSchedule == em_Pk_Schedule_None) // 如果!="" 那么肯定是掉线的, 不去修改退出标志!
			pAccGame->PkSetting.emPkSchedule = em_Pk_Schedule_TaskUpgrade;

		LogMsgBox(LOG_LEVEL_NORMAL L"此帐号不存在PK配置文件, 是新号!");
	}*/
	return TRUE;
}

CONST std::wstring& CSwAccPkMgr::GetPkScheduleText()
{
	static  std::wstring wsText = L"";
	switch (pAccGame->PkSetting.emPkSchedule)
	{
	case em_Pk_Schedule_ActiveCard:
		wsText = L"领卡阶段";
		break;
	case em_Pk_Schedule_PK:
		wsText = L"PK阶段";
		break;
	case em_Pk_Schedule_TaskUpgrade:
		wsText = L"练级阶段";
		break;
	case em_Pk_Schedule_AfterPK:
		wsText = L"PK后阶段";
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
		CConsoleLog(MyTools::CCharacter::MakeFormatText(L"练级阶段: %d个角色", i + 1));
		::Sleep(1000);

		em_LoginPlayerStatus emPlayerStatus = CLoginPlayerExtend::GetInstance().GetLoginPlayerStatus_By_Index(i);
		if (emPlayerStatus == em_LoginPlayer_UnExist) // 没有角色
		{
			LOG_CF_D(L"创建第%d个角色", i + 1);
			for (int j = i; j < 5; ++j)
				CLoginPlayerExtend::GetInstance().CreatePlayer(pShare->ConsoleContent.GlobalAccount.dwAreaId, j, em_Player_Classes_Destroyer);

			i--;
			continue;
		}

		if (emPlayerStatus == em_LoginPlayer_Deleted) // 删除状态的角色不动它
			continue;

		DWORD dwLevel = CLoginPlayerExtend::GetInstance().GetLevel_By_Index(i);
		if (dwLevel > 15 && i == 4) // 最后一个角色突然16级了, 不管了, 直接走下一个模式!
			break;
		else if (dwLevel > 15 && i == 0 && CGameFunction::GetInstance().GetDianKa() > 0) // 登录时如果第1个角色是16级，并且获取时间是有时间的，那么我们就固定的认为，已经进入第4阶段，就是对应PK后的流程
			break;
		else if (dwLevel > 15)
		{
			CConsoleLog(MyTools::CCharacter::MakeFormatText(L"练级阶段: %d个角色超过15级,等待删除中……", i + 1));
			CLoginPlayerExtend::GetInstance().DeletePlayer(i, 5 * 1000);
			if (CLoginPlayerExtend::GetInstance().GetLoginPlayerStatus_By_Index(i) == em_LoginPlayer_Normal)
			{
				SetLoginFaild(MyTools::CCharacter::MakeFormatText(L"练级阶段角色%d 超过15级, 并且删除角色无效!", i + 1), em_LoginFaild_UnDeleteAccount);
			}

			if (CSomeFun::GetInstance().TimeOut_Condiction(6 * 60 * 1000 + 1 * 60 * 1000, [&i, this]{ return CLoginPlayerExtend::GetInstance().GetLoginPlayerStatus_By_Index(i) != em_LoginPlayer_UnExist; }))
			{
				SetLoginFaild(MyTools::CCharacter::MakeFormatText(L"练级阶段角色%d 超过15级, 并且删除角色6分钟内失败!", i + 1), em_LoginFaild_UnDeleteAccount);
			}

			// 重新处理该序号的逻辑
			i--;
			continue;
		}

		if (dwLevel >= 15 && i < (int)pAccGame->GetOrderIndex()) // i == 2   Index == 4
		{
			LOG_CF_D( L"练级阶段: %d个人物等级:%d, 低于进度表,跳过!", i + 1, dwLevel, pAccGame->GetOrderIndex() + 1);
			continue;
		}

		// 保存PK进度
		pAccGame->GetOrderIndex() = i;
		SavePkConfig();

		CBnsGameFun::GetInstance().StartGame_By_Index(i);
		// 开始执行脚本, 并且等待线程结束!
		CBnsGameFun::WaitForThread(CBnsGameFun::em_Excute_Type_PK_TaskUpgrade);
		CBnsGameFun::GetInstance().BackToSwitchAccount();

		// 游戏状态下自动给他小退
		if (CGameVariable::GetInstance().GetEnvironmentValue() != 0x2)
			CBnsGameFun::GetInstance().BackToSwitchAccount();

		// 保存PK进度
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
		CConsoleLog(MyTools::CCharacter::MakeFormatText(L"%s阶段: %d个角色", pAccGame->PkSetting.emPkSchedule == em_Pk_Schedule_PK ? L"PK" : L"PK后", i + 1));

		::Sleep(1000);

		em_LoginPlayerStatus emPlayerStatus = CLoginPlayerExtend::GetInstance().GetLoginPlayerStatus_By_Index(i);
		if (emPlayerStatus == em_LoginPlayer_UnExist) // 没有角色
		{
			LOG_CF_D( L"创建第%d个角色", i + 1);
			for (int j = i; j < 5; ++j)
				CLoginPlayerExtend::GetInstance().CreatePlayer(pShare->ConsoleContent.GlobalAccount.dwAreaId, j, em_Player_Classes_Destroyer);
			i--;
			continue;
		}

		if (emPlayerStatus == em_LoginPlayer_Deleted) // 删除状态的角色不动它
			continue;

		// 判断点卡
		int nMinutes = CGameFunction::GetInstance().GetDianKa();
		if (nMinutes <= MINTIME_DIANKA)
		{
			SetLoginFaild(MyTools::CCharacter::MakeFormatText(L"刷完了! 剩余时间:%d分钟", nMinutes), em_LoginFaild_DeleteAccount);
		}

		// 获取当前第i个角色的等级, 如果此帐号的等级 < 16级， 并且低于进度记录， 那么就无视进度记录， 往下执行
		DWORD dwLevel = CLoginPlayerExtend::GetInstance().GetLevel_By_Index(i);
		if (dwLevel >= 16 && i < (int)pAccGame->GetOrderIndex()) // i == 2   Index == 4
		{
			//LogMsgBox(LOG_LEVEL_NORMAL L"%s阶段: %d个人物等级:%d, 低于进度表,跳过!", pAccGame->PkSetting.emPkSchedule == em_Pk_Schedule_PK ? L"PK" : L"PK后", i + 1, dwLevel, pAccGame->GetOrderIndex() + 1);
			continue;
		}
		
		//5.当PK流程执行到第5个角色时，就是第4个角色完成时，将前面4个角色删除，并且记录时间。6分钟后将当前PK任务执行的角色位置置1。此后刷完当前角色的PK任务，其实就是已经将5个角色都刷完了。进入下一阶段。
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
			// 保存PK进度
			pAccGame->GetOrderIndex() = i;
			SavePkConfig();
		}*/
		pAccGame->GetOrderIndex() = i;
		SavePkConfig();

		// 选择角色 -> 游戏内容
		CBnsGameFun::GetInstance().StartGame_By_Index(i);

		// 开始执行脚本, 并且等待线程结束!
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
		SetLoginFaild(MyTools::CCharacter::MakeFormatText(L"刷完了! 剩余时间:%d分钟", nMinutes), em_LoginFaild_DeleteAccount);
	}

	// 将阶段回到第五阶段!
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
		CBnsConfig::GetInstance().WriteAccountLog( L"角色%d 的名称: %s 不是自己的号!", nIndex, wsPlayerName.c_str());
		SetLoginFaild(em_LoginFaild_DeleteAccount);
		return FALSE;
	}*/
	return SwitchAccProc(fnExit);
}

BOOL CSwAccPkMgr::SwitchAccProc(std::function<BOOL(void)> fnExit)
{
	// 读取进度
	ReadPkConfig();

	BOOL bExit = FALSE;
	while (!bExit)
	{
		::Sleep(1000);
		switch (pAccGame->PkSetting.emPkSchedule)
		{
		case em_Pk_Schedule_TaskUpgrade:
			// 1.练级到15级，切换角色只判断角色是否到15级。是否已做完当前该做的任务及是否到15级为标志，角色位置++。直至5个角色全部15级。角色位置必须在ini里记录，以方便掉线上线重新判断的依据。
			TaskUpgrade_PK();
			pAccGame->PkSetting.emPkSchedule = em_Pk_Schedule_ActiveCard;
			pAccGame->GetOrderIndex() = 0;
			break;
		case em_Pk_Schedule_ActiveCard:
			// 2.自动领卡。将ini里的角色位置置1
			if (ActiveCard() > 0)
			{
				pAccGame->PkSetting.emPkSchedule = em_Pk_Schedule_PK;
				pAccGame->GetOrderIndex() = 0;
			}
			else
			{
				//CBnsConfig::WriteAccountLog(pShare, pAccGame, L"领卡后,点卡时间=0!");
				SetLoginFaild(L"领卡后,点卡时间=0!", em_LoginFaild_DeleteAccount);
			}
			break;
		case em_Pk_Schedule_PK:
			// 4.PK任务执行，判断任务完成情况。2个PK任务完成，去邮寄。邮寄执行完成后，再判断当前背包里是否还有邮寄物品。直到邮寄出后角色位置++
			AccountPK_PK();
			SetLoginFaild(L"刷完5个PK角色!", em_LoginFaild_DeleteAccount);
			break;
		case em_Pk_Schedule_AfterPK:
			AfterPK_PK();
			break;
		default:
			LOG_MSG_CF( L"未知的emPkSchedule=%X", pAccGame->PkSetting.emPkSchedule);
			bExit = TRUE;
			break;
		}
		// 保存进度
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
		LOG_MSG_CF( L"缺少ReceiveCard.dll!");
		return 0;
	}

	HMODULE hmDLL = ::LoadLibraryW(wszReceiveCardPath);
	if (hmDLL == NULL)
	{
		LOG_MSG_CF( L"ReceiveCard.dll异常!");
		return 0;
	}

	typedef int(WINAPIV* pfnActiveCard)(void);
	pfnActiveCard fnActiveCard = (pfnActiveCard)::GetProcAddress(hmDLL, "ActiveCard");
	if (fnActiveCard == nullptr)
	{
		LOG_MSG_CF( L"ReceiveCard.dll导出函数异常!");
		return 0;
	}

	// 打开文件, 是否领卡成功!
	CConsoleLog(L"领卡中……");
	int nRetCode = fnActiveCard();
	if (nRetCode == 0)
	{
		//CBnsConfig::GetInstance().WriteAccountLog( L"领卡失败!");
		return 0;
	}

	nMinuite = CGameFunction::GetInstance().GetDianKa();
	if (nMinuite <= MINTIME_DIANKA)
	{
		//CBnsConfig::GetInstance().WriteAccountLog( L"点卡时间不足! 剩余时间:%d分钟", nMinuite);
		return 0;
	}

	return nMinuite;
}
