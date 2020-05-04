#include "stdafx.h"
#include "DailyTask_Boss_Template.h"
#include "Monster.h"
#include "Person.h"

CDailyTask_Boss_Template::CDailyTask_Boss_Template(_In_ CONST CMonster& Boss, _In_ CONST CDailyTask_Move& DailyTask_Move, CONST FixPointInfo& BossRange) : _Boss(Boss), _DailyTask_Move(DailyTask_Move), _BossRange(BossRange) 
{

}

BOOL CDailyTask_Boss_Template::IsFaceToBoss(_In_ float fRange) CONST
{
	if (_Boss.GetDis() > fRange)
		return FALSE;

	return CPerson::GetInstance().GetDis(_Boss.GetFacePoint(fRange)) > fRange ? FALSE : TRUE;
}
