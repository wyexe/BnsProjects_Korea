#include "stdafx.h"
#include "PlayerKill.h"
#include <MyTools/Log.h>
#include "BagFunction.h"
#include "PersonFunction.h"
#include "SomeFun.h"
#include "PlayerExtend.h"
#include "TaskExtend.h"
#include "NpcExtend.h"
#include "Person.h"
#include "Task.h"
#include "Farm.h"
#include "Npc.h"
#include "GameSend.h"
#include "BnsGameFun.h"
#include "ObjectFunction.h"
#include "GameUI.h"
#include "ConsoleLog.h"

#define _SELF L"PlayerKill.cpp"
CPlayerKill::CPlayerKill()
{
	bMatchResult = bFirst = TRUE;
}


em_StartPK_Status CPlayerKill::GetStartPKStatus()
{
	DWORD dwStatus = ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(人物基址) + 人物基址偏移1) + PK偏移) + 0x10) + PK状态偏移) & 0xFF;
	switch (dwStatus)
	{
	case 0:
		return em_StartPK_Status_None;
	case 1:
		return em_StartPK_Status_Wait;
	case 2:
		return em_StartPK_Status_Done;
	default:
		LOG_MSG_CF( L"GetStartPKStatus=%d", dwStatus);
		break;
	}
	return em_StartPK_Status_None;
}

BOOL CPlayerKill::ClickPK()
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([]{CGameSend::StartPK(); });
}

BOOL CPlayerKill::ClickExit()
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([]{CGameSend::ExitPK(); });
}

BOOL CPlayerKill::IsExistPKRoom()
{
	DWORD dwAddr = ReadDWORD(ReadDWORD(ReadDWORD(人物基址) + 人物基址偏移1) + PK偏移);
	if (dwAddr == NULL)
		return FALSE;

	return ReadDWORD(ReadDWORD(dwAddr + 0x10) + 0x18) == 0x1 ? TRUE : FALSE;
}

BOOL CPlayerKill::ClickEndPK()
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([]{CGameSend::EndPK(); });
}

BOOL CPlayerKill::ClickReadyPk()
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([]{CGameSend::ReadyPk(); });
}


em_PKing_Status CPlayerKill::GetPKingStatus()
{
	DWORD dwStatus = ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(人物基址) + 人物基址偏移1) + PK偏移 + 0x4) + 0x10) + 0x10) & 0xFF;
	switch (dwStatus)
	{
	case 1:
		return em_PKing_Status_Load;
	case 2: 
		return em_PKing_Status_Ready;
	case 3:
		return em_PKing_Status_PK;
	case 4:
		return em_PKing_Status_Lose;
	default:
		break;
	}
	return em_PKing_Status_Wait;
}

BOOL CPlayerKill::IsLoadPkRoom()
{
	return ReadDWORD(CPerson::GetInstance().GetBase() + PK过图偏移) == 0 ? FALSE : TRUE;
}

BOOL CPlayerKill::Zy_IsLoadPkRoom()
{
	return CGameVariable::GetInstance().GetEnvironmentValue() == 5 ? TRUE : FALSE;
}

BOOL CPlayerKill::Zy_IsExistPKRoom()
{
	return CGameVariable::GetInstance().GetEnvironmentValue() == 4 ? TRUE : FALSE;
}

BOOL CPlayerKill::StartPK()
{
#if BnsProject_DLL
	// 判断任务是否完成了
	if (IsFinishPK())
		return UseItem_By_Pk();
#endif
	
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NMAE_ISPUASEHELPTHREAD, TRUE);
	//if (!IsExistPKRoom() && !InToPkRoom())
	//{
	//	LOG_CF_D(L"直接结束PK!");
	//	return FALSE;
	//}
	auto ulTick = ::GetTickCount64();
	CConsoleLog(L"开始PK!");

	if (!InToPkRoom())
	{
		LOG_CF_D( L"InToPkRoom返回false,直接结束PK!");
		return FALSE;
	}

	while (GameRun && !IsFinishPK_Condition())
	{
		if (GetStartPKStatus() == em_StartPK_Status_None)
		{
			CConsoleLog(MyTools::CCharacter::MakeFormatText(L"PK已用时:%d分钟", static_cast<DWORD>(::GetTickCount64() - ulTick) / 1000 / 60));
			LOG_CF_D( L"正在PK房间等待战斗, 开始匹配吧，少年!");
			
			if (!StartMatchingPlayer())
			{
				LOG_CF_D( L"貌似没匹配成功…… continue;");
				continue;
			}
			LOG_CF_D( L"开始PK前的准备");
			BeforePK();
			LOG_CF_D( L"开始PK!");
			if (!FightPK())
			{
				CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NMAE_ISPUASEHELPTHREAD, FALSE);
				LOG_CF_D( L"FightPK Return FALSE;");
				CGameVariable::GetInstance().SetForceBackToSwitchPlayer();
				return FALSE;
			}

			LOG_CF_D( L"结束PK!");
		}
		BnsSleep(1 * 1000);
	}
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NMAE_ISPUASEHELPTHREAD, FALSE);

	LOG_CF_D( L"PK打完收工!");
	if (GameRun)
		ClickExit();

	BnsSleep(10 * 1000);
	CPersonFunction::GetInstance().WaitGameLoad();
	return UseItem_By_Pk();
}

BOOL CPlayerKill::InToPkRoom()
{
	auto ulTick = ::GetTickCount64();
	do
	{
		if (Zy_IsLoadPkRoom())
		{
			LOG_CF_D( L"已经在等待PK过图了……");
			BnsSleep(1000);
		}
		else
		{
			CSomeFun::GetInstance().SimlationKey(VK_F8);
			BnsSleep(2000);
			CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"TotalGateConfirmPanel", [](CONST CGameUi& TotalGateConfirmPanel)
			{
				CGameVariable::GetInstance().PushMainThreadFunPtr_synch([TotalGateConfirmPanel]{ CGameSend::IntoRoom(TotalGateConfirmPanel.GetBase(), CGameSend::em_Room_Type::em_Room_Type_PK); });
			});
			
			BnsSleep(5000);
			LOG_CF_D( L"等待PK过图……");
		}

		if (Zy_IsExistPKRoom())
		{
			LOG_CF_D( L"已经成功进入PK场……");
			BnsSleep(1000);
			break;
		}

		if (static_cast<DWORD>(::GetTickCount64() - ulTick) >= 4 * 60 * 1000)
		{
			LOG_CF_D( L"PK等待超过4分钟，退出不再PK……");
			return FALSE;
		}
	} while (GameRun);
	//} while (CSomeFun::GetInstance().TimeOut_Condiction_GameRun(15 * 1000, []{return !CPersonFunction::GetInstance().IsGameLoad(); }));

	//while (GameRun && IsLoadPkRoom())
	//	BnsSleep(1000);
	
	//LOG_CF_D( L"成功进入PK房间");
	return TRUE;
}

BOOL CPlayerKill::IsFinishPK_Condition()
{
#if BnsProject_DLL
	// 判断2个支线都不存在, 并非首次进入
	if (!CTask::GetInstance().ExistTask(0x146, nullptr) && !CTask::GetInstance().ExistTask(0x147, nullptr) && !bFirst)
	{
		LOG_CF_D( L"PK任务完毕!");
		return TRUE;
	}
#endif
	return FALSE;
}

BOOL CPlayerKill::StartMatchingPlayer()
{
	while (GameRun && GetStartPKStatus() == em_StartPK_Status_None)
	{
		LOG_CF_D( L"开始PK匹配玩家!");
		ClickPK();
		BnsSleep(5 * 1000);
	}

	while (GameRun)
	{
		if (GetStartPKStatus() == em_StartPK_Status_Done)
		{
			LOG_CF_D( L"成功匹配到玩家!");
			break;
		}

		if (GetStartPKStatus() == em_StartPK_Status_None)
		{
			LOG_CF_D( L"貌似超时了都木有匹配到玩家!");
			return FALSE;
		}

		BnsSleep(1 * 1000);
	}

	UINT uCount = 0;
	while (GameRun)
	{
		if (GetPKingStatus() == em_PKing_Status_Load)
		{
			BnsSleep(3 * 1000);
			CPersonFunction::GetInstance().WaitGameLoad();
			LOG_CF_D( L"成功进入PK场了!");
			break;
		}

		BnsSleep(1 * 1000);
		if (uCount++ >= 3 * 60)
		{
			LOG_CF_D( L"3min内都没成功进入PK场, 放弃!");
			ClickEndPK();
			BnsSleep(10 * 1000);
			CPersonFunction::GetInstance().WaitGameLoad();
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CPlayerKill::BeforePK()
{
	if (!CTask::GetInstance().ExistTask(0x147, nullptr)) // 如果147任务不存在, 就强制输
		bMatchResult = FALSE;

	// 召唤猫
	CFarm::GetInstance().BeckonCat();

	// 修理武器
	CBag Hammer(0x0, 0x0);
	if (CBagFunction::GetInstance().GetEndurance() == NULL && CBagFunction::GetInstance().ExistHammer(&Hammer))
	{
		if (CBaseObject::GetPersonDis(Point(4580, -82544, 5908)) < 20.0f) // 情愿崖比武场
			CPersonFunction::GetInstance().MoveToPoint(Point(4580, -82544, 5908), 3.0f);
		else if (CBaseObject::GetPersonDis(Point(-3380, 82616, -2232)) < 20.0f) // 
			CPersonFunction::GetInstance().MoveToPoint(Point(-3380, 82616, -2232), 3.0f);
		else if (CBaseObject::GetPersonDis(Point(-107, 78093, -3917)) < 20.0f) // 黄山八卦堂
			CPersonFunction::GetInstance().MoveToPoint(Point(-3380, 82616, -2232), 3.0f);
		else if (CBaseObject::GetPersonDis(Point(-106148, -678, 7862)) < 20.0f) // 武王的神殿
			CPersonFunction::GetInstance().MoveToPoint(Point(-106148, -678, 7862), 3.0f);
		else
		{
			LOG_CF_D( L"找不到修理的地方!");
			bFirst = FALSE;
			return TRUE;
		}

		BnsSleep(2000);
		LOG_CF_D( L"使用锤子!");
		Hammer.UseItem();
		BnsSleep(10 * 1000);
	}

	bFirst = FALSE;
	return TRUE;
}

BOOL CPlayerKill::FightPK()
{
	auto fnLeave = [this]
	{
		do
		{
			LOG_CF_D( L"战斗结束, 离开PK场");
			ClickEndPK();
			BnsSleep(10 * 1000);
			CPersonFunction::GetInstance().WaitGameLoad();
		} while (GetStartPKStatus() != em_StartPK_Status_None && GameRun);
	};

	DWORD dwPlayerId = NULL;
	while (GameRun)
	{
		ClickReadyPk();
		LOG_CF_D( L"等待PK……!");
		BnsSleep(5 * 1000);
		CPersonFunction::GetInstance().WaitGameLoad();
		DWORD dwBean = CPerson::GetInstance().GetPkBean();
		if (CSomeFun::GetInstance().TimeOut_Condiction_GameRun(100 * 1000, [this, &dwBean]{ return GetPKingStatus() != em_PKing_Status_PK && dwBean == CPerson::GetInstance().GetPkBean(); }))
		{
			LOG_CF_E( L"100s都搜索对手失败,直接小退");
			return FALSE;
		}

		// 有可能刚开始就结束了!
		if (dwBean != CPerson::GetInstance().GetPkBean())
		{
			EndPK();
			fnLeave();
			return TRUE;
		}

		BnsSleep(1 * 1000);
		// 获取最近的一个玩家
		CPlayer Player;
		Point FixPoint = CBaseObject::GetPersonPoint();
		if (CPlayerExtend::GetInstance().GetTopPlayer_By_Dis(CBaseObject::CreateFixPointInfo(50), &Player) == 0 || Player.GetBase() == NULL)
		{
			LOG_CF_D( L"50m内找不到玩家!");
			EndPK();
			fnLeave();
			return TRUE;
		}
		dwPlayerId = Player.GetId();

		// 战斗逻辑
		//CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NMAE_ISPUASEHELPTHREAD, 0x2);
		LOG_CF_D( L"开始战斗!");
		while (GameRun)
		{
			if (EMPTY_PERSONHP || Player.GetHp() == NULL || !CPlayerExtend::GetInstance().FindPlayer_By_Id(dwPlayerId, CBaseObject::CreateFixPointInfo(200), nullptr) || dwBean != CPerson::GetInstance().GetPkBean())
			{
				bMatchResult = !EMPTY_PERSONHP;
				LOG_CF_D( L"战斗结束,Sleep 3s后就离开,战斗结果是:%s", bMatchResult ? L"赢" : L"输");
				BnsSleep(7 * 1000);
				if (dwBean != CPerson::GetInstance().GetPkBean())
				{
					EndPK();	
					fnLeave();
					return TRUE;
				}
				break;
			}
			KillPlayer(dwBean, &Player);
			BnsSleep(300);
		}

		while (GameRun && GetPKingStatus() == em_PKing_Status_Wait)
			BnsSleep(1000);

		BnsSleep(5 * 1000);
	}
	return TRUE;
}

BOOL CPlayerKill::EndPK()
{
	TaskInfo TaskInfo_;
	if (CTask::GetInstance().ExistTask(0x147, &TaskInfo_) && TaskInfo_.GetScheduleId() == 0x2)
	{
		static CONST SumitTask_Src STS(0x2, 0x147, 0x0, 0x1, 0xFF, 0x20147, 0x0, 0x0);
		CTaskExtend::GetInstance().SumitTask_By_Parm(STS, L"", L"");
		BnsSleep(1000);
	}

	if (CTask::GetInstance().ExistTask(0x146, &TaskInfo_) && TaskInfo_.GetScheduleId() == 0x2)
	{
		CONST static SumitTask_Src STS(0x2, 0x146, 0x0, 0x1, 0xFF, 0x20147, 0x0, 0x0);
		CTaskExtend::GetInstance().SumitTask_By_Parm(STS, L"", L"");
		BnsSleep(1000);
	}

	return TRUE;
}

BOOL CPlayerKill::KillPlayer(_In_ DWORD dwBean, _In_ CPlayer* pPlayer)
{
	static BOOL stepto = FALSE;
	CONST static std::vector<Point> vRound = {
		Point(-112, 78080, -3920), Point(4636, -86984, 5092), Point(-3380, 82616, -2232),
		Point(-107, 78093, -3917), Point(-106164, -600, 7856)
	};
	
	auto pRound = MyTools::CLPublic::Vec_find_if_Const(vRound, [](CONST Point& itm){return CBaseObject::GetPersonDis(itm) < 50.0f; });
	if (pRound == nullptr)
	{
		LOG_CF_E( L"此PK场竟然没有记录!");
		return TRUE;
	}

	auto fnTerminate = [&dwBean]
	{
		//LOG_CF_D( L"PK豆OLD=%d，PK豆NEW=%d", dwBean, CPerson::GetInstance().GetPkBean());
		return CPerson::GetInstance().GetPkBean() != dwBean;
	};

	CSomeFun::GetInstance().SetSpeed(2.5f);
	//if (CPerson::GetInstance().GetClasses() == em_Player_Classes_Destroyer) // 力士
	//	CFarm::GetInstance().PK_Lux_Pk(*pPlayer, bMatchResult, fnTerminate);
	//else if (CPerson::GetInstance().GetClasses() == em_Player_Classes_KongFuForceMaster)
	//	CFarm::GetInstance().Pk_Kffm(*pPlayer, FixPointInfo(*itr, 8.0f), bMatchResult, fnTerminate);
	
	switch (CPerson::GetInstance().GetClasses())
	{
	case em_Player_Classes::em_Player_Classes_Destroyer:
		CFarm::GetInstance().PK_Lux_Pk(*pPlayer, bMatchResult, fnTerminate);
		break;
	case em_Player_Classes::em_Player_Classes_KongFuForceMaster:
		CFarm::GetInstance().Pk_Kffm(*pPlayer, FixPointInfo(*pRound, 8.0f), bMatchResult, fnTerminate);
		break;
	case em_Player_Classes::em_Player_Classes_Summoner:
		CFarm::GetInstance().Pk_Summoner(*pPlayer, FixPointInfo(*pRound, 6.0f), fnTerminate);
		break;
	default:
		break;
	}
	
	return TRUE;
}

BOOL CPlayerKill::UseItem_By_Pk()
{
	//영석 주머니 灵石袋子
	LOG_CF_D( L"准备开灵石袋子!");
	CBag Item(0x0, 0x0);
	while (CBagFunction::GetInstance().ExistItem_By_Name(L"영석 주머니", &Item))
	{
		Item.OpenBox();
		BnsSleep(1000);
		CPersonFunction::GetInstance().WaitForProgressBar();
	}
	return TRUE;
}

BOOL CPlayerKill::IsFinishPK()
{
	try
	{
		CNpc Npc;
		if (!CNpcExtend::GetInstance().FindNpc_By_Name(L"비무 게시판", CBaseObject::CreateFixPointInfo(50), &Npc))
		{
			LOG_MSG_CF( L"附近找不到PK场!");
			return TRUE;
		}

		CPersonFunction::GetInstance().MoveToPoint(Npc.GetPoint(), 3.0f);
		BnsSleep(2000);

		// 接任务
		if (!CTask::GetInstance().ExistTask(0x146, nullptr))
		{
			CONST static SumitBLTask_Src STS = { 0x59, 0x49ACB, 0x146, 0x0, 0x3 };
			CTaskExtend::GetInstance().PickBLTask_By_Npc(STS, Npc.GetName());
			BnsSleep(3 * 1000);
		}
		if (!CTask::GetInstance().ExistTask(0x147, nullptr))
		{
			CONST static SumitBLTask_Src STS = { 0x59, 0x49ACB, 0x147, 0x0, 0x3 };
			CTaskExtend::GetInstance().PickBLTask_By_Npc(STS, Npc.GetName());
			BnsSleep(3 * 1000);
		}

		if (!CTask::GetInstance().ExistTask(0x146, nullptr) && !CTask::GetInstance().ExistTask(0x147, nullptr))
			return TRUE;

	}
	catch (...)
	{
		LOG_CF_E( L"IsFinishPK出现异常");
	}
	return FALSE;
}
