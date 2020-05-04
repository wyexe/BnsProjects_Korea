#include "stdafx.h"
#include "DailyTask_Common.h"
#include <MyTools/Log.h>
#include "PersonFunction.h"
#include "Person.h"
#include "Monster.h"
#include "FarmBossSkill.h"
#include "DailyTask_BossSkill.h"
#include "DailyTask_Move.h"
#include "SomeFun.h"
#include "GameSend.h"
#include "Farm.h"
#include "MonsterExtend.h"
#include "TeamMember.h"
#include "TeamMemberExtend.h"
#include "FarmSkill.h"
#include "ResAction.h"

#define _SELF L"DailyTask_Common.cpp"

CDailyTask_Common::CDailyTask_Common() : _dwDisableBeckonCat(NULL), _dwKillMonsterMovDis(NULL), _dwMoveFlag(NULL)
{

}

VOID CDailyTask_Common::MoveToPoint(_In_ CONST Point& Pt, _In_ CONST FixPointInfo& Range) CONST
{
	while (GameRun && !EMPTY_PERSONHP && CPerson::GetInstance().GetDis(Pt) > 3)
	{
		CFarm::GetInstance().ClearAroundMonster(Range, [] { return FALSE; });
		if (CPersonFunction::GetInstance().IsGameLoad())
		{
			LOG_CF_D(L"走路过程中出现过图……");
			CPersonFunction::GetInstance().WaitGameLoad();
			LOG_CF_D(L"走路过程中过图完毕, 不继续走了!");
			break;
		}
		if (!CPerson::GetInstance().IsMoving())
		{
			CPersonFunction::GetInstance().Move(Pt);
			BnsSleep(100);
			continue;
		}
	}
}

VOID CDailyTask_Common::SetBossEnviroment(_In_ CONST CMonster& Boss)
{
	CDailyTask_BossSkill::GetInstance().HookMonsterSkill(Boss.GetBase());

	_dwDisableBeckonCat = CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_DISABLE_BECKONCAT, TRUE);
	_dwKillMonsterMovDis = CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_KILLMONSTERR_MOVE_DIS, 8);

	CFarmBossSkill::GetInstance().SetClassesNormalAttack();
	CPersonFunction::GetInstance().SetMoveFlag(MOVE_FLAG_DISABLE_JUMP | MOVE_FLAG_DISABLE_SPEED | MOVE_FLAG_DISABLE_SS);
}

VOID CDailyTask_Common::RestoreBossEnviroment()
{
	CFarmBossSkill::GetInstance().ReleaseBossObject();
	CPersonFunction::GetInstance().RestoreMoveFlag();
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_DISABLE_BECKONCAT, _dwDisableBeckonCat);
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_KILLMONSTERR_MOVE_DIS, _dwKillMonsterMovDis);
	CDailyTask_BossSkill::GetInstance().Clear();
}

VOID CDailyTask_Common::Move(DWORD dwMoveFlag)
{
	if (_dwMoveFlag == dwMoveFlag)
		return;
	else if (_dwMoveFlag != NULL)
	{
		// restore Pop SimKey
		Move_GameSend(_dwMoveFlag, FALSE);
		_dwMoveFlag = NULL;
	}
	else if (dwMoveFlag == 0) // stop move
	{
		if (_dwMoveFlag == NULL)
			return;

		Move_GameSend(_dwMoveFlag, FALSE);
		_dwMoveFlag = NULL;
		return;
	}


	Move_GameSend(dwMoveFlag, TRUE);
	_dwMoveFlag = dwMoveFlag;
}

VOID CDailyTask_Common::StopMove_SingleFlag(DWORD dwMoveFlag)
{
	switch (dwMoveFlag)
	{
	case PERSON_MOVE_W:
		break;
	case PERSON_MOVE_S:
		break;
	case PERSON_MOVE_A:
		break;
	case PERSON_MOVE_D:
		break;
	default:
		return;
	}
	Move_GameSend(dwMoveFlag, FALSE);
}

VOID CDailyTask_Common::StopMove_MultiFlag(DWORD dwMoveFlag)
{
	if (dwMoveFlag == _dwMoveFlag)
	{
		Move_GameSend(dwMoveFlag, FALSE);
		_dwMoveFlag = NULL;
	}
}

VOID CDailyTask_Common::Move_GameSend(_In_ DWORD dwMoveFlag, _In_ BOOL IsPush)
{
	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([=]
	{
		if (dwMoveFlag == PERSON_MOVE_W)
			CGameSend::SimulationKey('W', IsPush);
		if (dwMoveFlag == PERSON_MOVE_S)
			CGameSend::SimulationKey('S', IsPush);
		if (dwMoveFlag == PERSON_MOVE_A)
			CGameSend::SimulationKey('A', IsPush);
		if (dwMoveFlag == PERSON_MOVE_D)
			CGameSend::SimulationKey('D', IsPush);
	});
}

BOOL CDailyTask_Common::IsFaceToBoss(_In_ CONST CMonster& Boss, _In_ float fRange) CONST
{
	if (Boss.GetDis() > fRange)
		return FALSE;

	return CPerson::GetInstance().GetDis(Boss.GetFacePoint(fRange)) > fRange ? FALSE : TRUE;
}

BOOL CDailyTask_Common::WaitToMemberDead() CONST
{
	// 禁止3分钟不动
	DWORD dwOldValue = CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CHECKPAUSEPOINT, FALSE);

	LOG_CF_D(L"当前人物HP=%d", CPerson::GetInstance().GetPercentHp());
	while (GameRun)
	{
		// 是不是所有人都死光了
		if (CTeamMemberExtend::GetInstance().GetTeamMemberCountByCondition([](CONST CTeamMember& TeamMember) { return TeamMember.GetPercentHp() == NULL; }) == CTeamMemberExtend::GetInstance().GetTeamMemberCount())
		{
			LOG_CF_D(L"所有队友都死光了!");
			break;
		}

		if (EMPTY_PERSONHP)
		{
			BnsSleep(1000);
			continue;
		}

		// 判断在线的队伍人数必须全部复活!
		CTeamMember TeamMeber;
		BOOL bExistDeadMember = CTeamMemberExtend::GetInstance().FindTeamMember_By_Condiction(&TeamMeber, [](CONST CTeamMember& TeamMember)
		{
			LOG_CF_D(L"队友[%s] HP=%d", TeamMember.GetName().c_str(), TeamMember.GetPercentHp());
			return TeamMember.IsConnect() && TeamMember.GetPercentHp() == NULL && TeamMember.GetDis(CPerson::GetInstance().GetPoint()) < 50.0f && TeamMember.GetName() != CPerson::GetInstance().GetName();
		});

		if (!bExistDeadMember)
		{
			LOG_CF_D(L"所有队友都是已经存活的状态! break;");
			break;
		}

		// 没死的去帮队友复活
		LOG_CF_D(L"跑去复活队友:%s, SelfHP=%d", TeamMeber.GetName().c_str(), CPerson::GetInstance().GetPercentHp());
		CPersonFunction::GetInstance().MoveToPoint(TeamMeber.GetPoint(), 2, 50);
		BnsSleep(2 * 1000);
		CFarmSkill::GetInstance().UseSkill_By_SkillId(SKILLID_RELIVE);
		BnsSleep(3 * 1000);
		CPersonFunction::GetInstance().WaitForProgressBar(20 * 1000);
	}
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CHECKPAUSEPOINT, dwOldValue);
	return TRUE;
}

VOID CDailyTask_Common::MoveThenKill(_In_ CONST std::vector<Point>& VecPoint) CONST
{
	CDailyTask_Move().MoveToVecPoint(VecPoint, [](CONST Point&) { return FALSE; });
	CFarm::GetInstance().ClearAroundMonster(CBaseObject::CreateFixPointInfo(16), [] { return FALSE; });
	CResAction::GetInstance().TakeDropItem(CBaseObject::CreateFixPointInfo(16));
}
