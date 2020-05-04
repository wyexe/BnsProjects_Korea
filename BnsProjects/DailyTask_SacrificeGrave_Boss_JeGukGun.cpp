#include "stdafx.h"
#include "DailyTask_SacrificeGrave_Boss_JeGukGun.h"

CDailyTask_SacrificeGrave_Boss_JeGuGun::CDailyTask_SacrificeGrave_Boss_JeGuGun(_In_ CONST CMonster& Boss, _In_ CONST CDailyTask_Move& DailyTask_Move)
	: CDailyTask_Boss_Template(Boss, DailyTask_Move, FixPointInfo(Point(), 0))
{

}

BOOL CDailyTask_SacrificeGrave_Boss_JeGuGun::Kill()
{
	return TRUE;
}

VOID CDailyTask_SacrificeGrave_Boss_JeGuGun::KillBoss_BladeMaster()
{

}

VOID CDailyTask_SacrificeGrave_Boss_JeGuGun::Skill_BladeMaster()
{

}

VOID CDailyTask_SacrificeGrave_Boss_JeGuGun::KillBoss_ForceMaster()
{

}

VOID CDailyTask_SacrificeGrave_Boss_JeGuGun::Skill_ForceMaster()
{

}

VOID CDailyTask_SacrificeGrave_Boss_JeGuGun::NormalMove()
{

}

VOID CDailyTask_SacrificeGrave_Boss_JeGuGun::MeleeMove()
{

}

VOID CDailyTask_SacrificeGrave_Boss_JeGuGun::FarRangeMove()
{

}
