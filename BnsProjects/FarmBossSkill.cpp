#include "stdafx.h"
#include "FarmBossSkill.h"
#include <MyTools/Log.h>
#include <MyTools/CLPublic.h>
#include <MyTools/TimeTick.h>
#include "Person.h"
#include "SomeFun.h"
#include "Monster.h"
#include "FarmSkill.h"
#include "PersonFunction.h"
#include "TeamMemberExtend.h"
#include "Farm.h"

#define _SELF L"FarmBossSkill.cpp"
CFarmBossSkill::CFarmBossSkill() : LockVecDefaultSkill(L"LockVecDefaultSkill")
{
	pBoss = nullptr;
	_Power = _Run = FALSE;
	hThread = NULL;
	_fnNormalAttackPtr = nullptr;
	_fnRetainPtr = nullptr;
}

CFarmBossSkill::~CFarmBossSkill()
{
	ReleaseBossObject();
}

VOID CFarmBossSkill::SetBossObject(_In_ CONST CMonster& Boss, _In_ CONST FixPointInfo& Range_)
{
	pBoss = &Boss;
	_Range = Range_;

	// 设置技能
	_VecDefaultSkill.clear();
	switch (CPerson::GetInstance().GetClasses())
	{
	case em_Player_Classes::em_Player_Classes_Summoner:
		_VecDefaultSkill = CGameVariable::GetInstance().GetSummonerBossSkillVec();
		break;
	case em_Player_Classes::em_Player_Classes_Warlock:
		_VecDefaultSkill = CGameVariable::GetInstance().GetWarlockBossSkillVec();
		break;
	default:
		break;
	}
}

VOID CFarmBossSkill::Run()
{
	if (_Run)
	{
		LOG_MSG_CF(L"_Run=TRUE, 是不是你忘记释放了?");
		return;
	}
	else if (pBoss == nullptr)
	{
		LOG_MSG_CF(L"BOSS=nullptr, 是不是你忘记设置了?");
		return;
	}
	else if (_fnNormalAttackPtr == nullptr)
	{
		LOG_MSG_CF(L"_fnNormalAttackPtr=nullptr, 是不是你忘记设置了?");
		return;
	}

	_Run = TRUE;
	_Power = FALSE;
	hThread = cbBEGINTHREADEX(NULL, NULL, _FarmBossSkillThread, this, NULL, NULL);
}

VOID CFarmBossSkill::ReleaseBossObject()
{
	pBoss = nullptr;
	_Run = FALSE;
	_Power = TRUE;
	if (hThread != NULL)
	{
		LOG_CF_D(L"等待_FarmBossSkillThread 结束~");
		::WaitForSingleObject(hThread, INFINITE);
		::CloseHandle(hThread);
		hThread = NULL;
	}
}

VOID CFarmBossSkill::SetDisableSkillId(_In_ DWORD dwSkillId)
{
	LockVecDefaultSkill.Access([this, dwSkillId]
	{
		MyTools::CLPublic::Vec_erase_if(_VecDefaultSkill, [dwSkillId](CGameVariable::ClassesBossSkill ClassesBossSkill_){ return dwSkillId == ClassesBossSkill_.dwSkillId; });
	});
}

VOID CFarmBossSkill::SetEnableSkillId(_In_ em_Skill_Type emSkillType, _In_ DWORD dwSkillId)
{
	LockVecDefaultSkill.Access([this, dwSkillId, emSkillType]
	{
		auto p = MyTools::CLPublic::Vec_find_if(_VecDefaultSkill, [dwSkillId](CGameVariable::ClassesBossSkill ClassesBossSkill_){ return ClassesBossSkill_.dwSkillId == dwSkillId; });
		if (p == nullptr)
			_VecDefaultSkill.push_back(CGameVariable::ClassesBossSkill{emSkillType, dwSkillId});
	});
}

VOID CFarmBossSkill::SetNormalAttack(_In_ std::function<VOID(VOID)> fnNormalAttackPtr)
{
	_fnNormalAttackPtr = fnNormalAttackPtr;
}

VOID CFarmBossSkill::SetClassesNormalAttack()
{
	switch (CPerson::GetInstance().GetClasses())
	{
	case em_Player_Classes::em_Player_Classes_Summoner:
		SetNormalAttack([]
		{
			if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_T2_Ui_D))
				CSomeFun::GetInstance().SimlationKey('T', 10);
			else
			{
				if (CPerson::GetInstance().GetMp() > 5)
					CSomeFun::GetInstance().SimlationKey('T', 10);
				else
					CSomeFun::GetInstance().SimlationKey('R', 10);
			}
		});

		SetRetain([]
		{
			if (CTeamMemberExtend::GetInstance().GetTeamMember_By_LessPercentHp(80, nullptr))
			{
				//LOG_CF_D( L"有队员80以下血了,加血");
				if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_Z))
					//CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Sum_SkillId_Z);
					CSomeFun::GetInstance().SimlationKey('Z', 10);
			}
		});
		break;
	case em_Player_Classes::em_Player_Classes_Warlock:
		SetNormalAttack([]
		{
			if (CPersonFunction::GetInstance().ExistBuff_Person(0x1B77403, nullptr) && GameRun)
			{
				if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Warlock_SkillId_God_4))
				{
					if (CFarmSkill::GetInstance().IsActive_By_SkillId_In_Ui(Warlock_SkillId_God_4))
					{
						//CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(Warlock_SkillId_God_4);
						CSomeFun::GetInstance().SimlationKey(VK_4, 10);
					}
				}
				if (CPerson::GetInstance().GetMp() > 6)
					CSomeFun::GetInstance().SimlationKey('T', 10);
				else
					CSomeFun::GetInstance().SimlationKey('R', 10);
			}
			else
			{
				if (CPerson::GetInstance().GetMp() > 6)
					CSomeFun::GetInstance().SimlationKey('T', 10);
				else
					CSomeFun::GetInstance().SimlationKey('R', 10);

				//CSomeFun::GetInstance().SimlationKey('F', 10);
			}
		});

		SetRetain([]
		{
			
		});
		break;
	default:
		break;
	}
}

VOID CFarmBossSkill::SetRetain(_In_ std::function<VOID(VOID)> fnRetainPtr)
{
	_fnRetainPtr = fnRetainPtr;
}

VOID CFarmBossSkill::Move(_In_ CONST Point& TarPoint, _In_ float fDis /*= 3.0f*/) CONST
{
	if (pBoss->GetDis(TarPoint) > fDis)
	{
		CPersonFunction::GetInstance().Move(TarPoint);
	}
}


VOID CFarmBossSkill::MoveToPoint(_In_ CONST Point& TarPoint, _In_ DWORD dwMaxTimeOut /* = 7 * 1000 */, _In_ float fDis /* = 3.0f */) CONST
{
	MyTools::CTimeTick TimeTick;
	while (GameRun && !EMPTY_PERSONHP)
	{
		if (CBaseObject::GetPersonDis(TarPoint) <= fDis)
			break;

		if (CBaseObject::GetPersonDis(TarPoint) >= 100)
		{
			LOG_CF_D( L"终点与当前坐标距离=%.2f, 超过100, 不走!:%d,%d,%d", CBaseObject::GetPersonDis(TarPoint), TarPoint.GetX(), TarPoint.GetY(), TarPoint.GetZ());
			break;
		}

		if (CPersonFunction::GetInstance().IsGameLoad())
		{
			LOG_CF_D( L"走路过程中出现过图……");
			CPersonFunction::GetInstance().WaitGameLoad();
			LOG_CF_D( L"走路过程中过图完毕, 不继续走了!");
			break;
		}

		if (static_cast<DWORD>(TimeTick.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Millisecond)) >= dwMaxTimeOut)//大于12秒没到直接退出
		{
			LOG_CF_E( L"TarPoint:%d,%d,%d, 距离:%.2f 无法到达目的地, 忽略该坐标!", TarPoint.GetX(), TarPoint.GetY(), TarPoint.GetZ(), CBaseObject::GetPersonDis(TarPoint));
			break;
		}

		if (!CPerson::GetInstance().IsMoving())
			Move(TarPoint);

		BnsSleep(100);
	}
}

VOID CFarmBossSkill::MoveToBoss(_In_ em_MoveType emMoveType, _In_ DWORD dwTimeOut /* = 7 * 1000 */, _In_ float fDis /* = 3.0f */) CONST
{
	auto fnSetTarPoint = [emMoveType, fDis, this](_Out_ Point& TarPoint)
	{
		switch (emMoveType)
		{
		case CFarmBossSkill::em_MoveType_Front:
			TarPoint = pBoss->GetFrontPoint(fDis);
			break;
		case CFarmBossSkill::em_MoveType_Left:
			TarPoint = pBoss->GetLeftPoint(fDis);
			break;
		case CFarmBossSkill::em_MoveType_Right:
			TarPoint = pBoss->GetRightPoint(fDis);
			break;
		case CFarmBossSkill::em_MoveType_Behind:
			TarPoint = pBoss->GetBehindPoint(fDis);
			break;
		default:
			LOG_MSG_CF(L"UnExist emMoveType=%X", emMoveType);
			break;
		}
	};

	MyTools::CTimeTick TimeTick;
	while (GameRun && !EMPTY_PERSONHP && !pBoss->IsDead() && _Run)
	{
		Point TarPoint;
		fnSetTarPoint(TarPoint);
		if (CPerson::GetInstance().GetDis(TarPoint) <= 2.0f)
			break;

		if (CPerson::GetInstance().GetDis(TarPoint) >= 100)
		{
			LOG_CF_D( L"终点与当前坐标距离=%.2f, 超过100, 不走!:%d,%d,%d", CBaseObject::GetPersonDis(TarPoint), TarPoint.GetX(), TarPoint.GetY(), TarPoint.GetZ());
			break;
		}

		if (CPersonFunction::GetInstance().IsGameLoad())
		{
			LOG_CF_D( L"走路过程中出现过图……");
			CPersonFunction::GetInstance().WaitGameLoad();
			LOG_CF_D( L"走路过程中过图完毕, 不继续走了!");
			break;
		}

		if (static_cast<DWORD>(TimeTick.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Millisecond)) >= dwTimeOut)
		{
			LOG_CF_E( L"TarPoint:%d,%d,%d, 距离:%.2f 无法到达目的地, 忽略该坐标!", TarPoint.GetX(), TarPoint.GetY(), TarPoint.GetZ(), CBaseObject::GetPersonDis(TarPoint));
			break;
		}

		if (!CPerson::GetInstance().IsMoving())
			Move(TarPoint);

		BnsSleep(100);
	}
}

VOID CFarmBossSkill::MoveToSixAngle(_In_ float fDis, _In_ DWORD dwTimeOut) CONST
{
	if (CPersonFunction::GetInstance().ExistBuff_Person(0x384BDEF, nullptr))
		fDis += 1.f;

	if (fabs(pBoss->GetDis() - fDis) <= 2.0f)
		return;

	std::vector<Point> VecPoint;
	CFarm::GetInstance().GetRoundPointList(FixPointInfo(pBoss->GetPoint(), fDis), 6, VecPoint);

	auto Index = CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex();
	if (Index < 0)
		return;

	CONST Point& HornPoint = VecPoint.at(Index);
	if (CPerson::GetInstance().GetDis(HornPoint) > 1)
	{
		MoveToPoint(HornPoint, dwTimeOut, 2);
	}
}

VOID CFarmBossSkill::SetPower(_In_ BOOL bPower)
{
	_Power = bPower;
}

DWORD WINAPI CFarmBossSkill::_FarmBossSkillThread(LPVOID lpParm)
{
	CFarmBossSkill* pCFarmBossSkill = static_cast<CFarmBossSkill *>(lpParm);

	while (GameRun && pCFarmBossSkill->_Run)
	{
		BnsSleep(100);
		if (EMPTY_PERSONHP)
			continue;
		if (pCFarmBossSkill->_Power)
			continue;
		if (pCFarmBossSkill->pBoss->GetDis() > 17.0f)
			continue;

		pCFarmBossSkill->pBoss->SetAngle();
		pCFarmBossSkill->LockVecDefaultSkill.Access([pCFarmBossSkill]
		{
			for (CONST auto& ClassesBossSkill_ : pCFarmBossSkill->_VecDefaultSkill)
			{
				if (CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(ClassesBossSkill_.dwSkillId))
				{
					CFarmSkill::GetInstance().UseSkill_By_SkillId(ClassesBossSkill_.dwSkillId, ClassesBossSkill_.emSkillType);
					break;
				}
			}
		});

		if (pCFarmBossSkill->_Power)
			continue;
		
		if (pCFarmBossSkill->_fnRetainPtr != nullptr)
			pCFarmBossSkill->_fnRetainPtr();

		pCFarmBossSkill->_fnNormalAttackPtr();
	}

	return 0;
}
