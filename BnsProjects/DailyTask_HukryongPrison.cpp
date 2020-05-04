#include "stdafx.h"
#include "DailyTask_HukryongPrison.h"
#include <MyTools/Log.h>
#include "Person.h"
#include "Monster.h"
#include "MonsterExtend.h"
#include "DailyTask_HukryongPrison_Boss_BuKa.h"

#define _SELF L"DailyTask_HukryongPrison.cpp"
CDailyTask_HukryongPrison::CDailyTask_HukryongPrison()
{

}

BOOL CDailyTask_HukryongPrison::Run()
{
	if (GameRun && !EMPTY_PERSONHP)
	{
		CMonster Boss;
		if (CMonsterExtend::GetInstance().FindMonster_By_Name(Monster_²¼¿¨, CBaseObject::CreateFixPointInfo(100), &Boss))
		{
			SetBossEnviroment(Boss);
			_DailyTask_Move.MoveToTarget(Monster_²¼¿¨, 100);

			CDailyTask_HukryongPrison_Boss_BuKa BuKa(Boss, _DailyTask_Move);
			BuKa.Kill();
			RestoreBossEnviroment();
			WaitToMemberDead();
		}
	}
	LOG_MSG_CF(L"Done!");
	return FALSE;
}


BOOL CDailyTask_HukryongPrison::AccpetDailyTask() CONST
{
	return TRUE;
}

VOID CDailyTask_HukryongPrison::StudyDailySkill() CONST
{
	
}

BOOL CDailyTask_HukryongPrison::MoveToDungeon()
{
	return TRUE;
}

BOOL CDailyTask_HukryongPrison::IsInDungeon() CONST
{
	return TRUE;
}
