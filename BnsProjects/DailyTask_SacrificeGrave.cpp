#include "stdafx.h"
#include "DailyTask_SacrificeGrave.h"
#include <MyTools/Log.h>
#include "Person.h"
#include "Monster.h"
#include "MonsterExtend.h"
#include "DailyTask_SacrificeGrave_Boss_JeGukGun.h"

#define _SELF L"DailyTask_SacrificeGrave.cpp"

CDailyTask_SacrificeGrave::CDailyTask_SacrificeGrave()
{

}

BOOL CDailyTask_SacrificeGrave::Run()
{
	if (GameRun && !EMPTY_PERSONHP)
	{
		CMonster Boss;
		if (CMonsterExtend::GetInstance().FindMonster_By_Name(Monster_张起副, CBaseObject::CreateFixPointInfo(100), &Boss))
		{
			SetBossEnviroment(Boss);
			_DailyTask_Move.MoveToTarget(Monster_张起副, 100);

			CDailyTask_SacrificeGrave_Boss_JeGuGun JeGuGun(Boss, _DailyTask_Move);
			JeGuGun.Kill();
			
			RestoreBossEnviroment();
			WaitToMemberDead();
		}
	}
	LOG_MSG_CF(L"Done!");
	return FALSE;
}

BOOL CDailyTask_SacrificeGrave::AccpetDailyTask() CONST
{
	return TRUE;
}

VOID CDailyTask_SacrificeGrave::StudyDailySkill() CONST
{

}

BOOL CDailyTask_SacrificeGrave::MoveToDungeon()
{
	return TRUE;
}

BOOL CDailyTask_SacrificeGrave::IsInDungeon() CONST
{
	return TRUE;
}
