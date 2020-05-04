#include "stdafx.h"
#include "DailyTask_Move.h"
#include <MyTools/CLPublic.h>
#include <MyTools/Log.h>
#include "PersonFunction.h"
#include "Person.h"
#include "ConsoleLog.h"
#include "Farm.h"
#include "SomeFun.h"
#include "Monster.h"
#include "Npc.h"
#include "MonsterExtend.h"
#include "NpcExtend.h"

#define _SELF L"DailyTask_BeforeDoor.cpp"
BOOL CDailyTask_Move::MoveToFengYueGuanCenter()
{
	CONST static Point CenterPoint(-40580,8328,-1738);

	CONST static std::vector<Point> VecFengYueGuanToCenter = 
	{
		Point(-40136,17104,-1288),Point(-39908,16440,-1288),Point(-39832,15684,-1288),Point(-39744,14784,-1288),Point(-39644,13476,-1288),
		Point(-39600,12452,-1740),Point(-39588,11452,-1740),Point(-39744,10692,-1740),Point(-39908,9928,-1740),Point(-40152,9224,-1740),
		Point(-40552,8292,-1740), CenterPoint
	};
	CONST static std::vector<Point> VecDeadToCenter = 
	{
		Point(-46130,951,-1403),Point(-46013,1047,-1403),Point(-45641,1044,-1521),Point(-45360,1146,-1707),Point(-45247,1248,-1735),
		Point(-44952,1485,-1735),Point(-44677,1727,-1764),Point(-44401,1986,-1735),Point(-44130,2241,-1738),Point(-43859,2496,-1738),
		Point(-43583,2744,-1738),Point(-43303,2989,-1738),Point(-43053,3265,-1738),Point(-42840,3569,-1738),Point(-42671,3901,-1738),
		Point(-42512,4237,-1738),Point(-42353,4573,-1738),Point(-42226,4923,-1738),Point(-42088,5268,-1738),Point(-41920,5610,-1738),
		Point(-41771,5913,-1738),Point(-41607,6247,-1738),Point(-41427,6611,-1738),Point(-41263,6945,-1736),Point(-41127,7222,-1736),
		Point(-40960,7555,-1736),Point(-40786,7884,-1736),Point(-40622,8217,-1736),Point(-40554,8513,-1736),Point(-40541,8677,-1736),
		Point(-40538,8832,-1736),Point(-40540,9022,-1736),Point(-40556,9176,-1736),Point(-40596,9577,-1736),Point(-40620,9808,-1736),
		Point(-40516,9236,-1740),Point(-40528,8808,-1740),Point(-40580,8356,-1740),CenterPoint
	};

	if (CPerson::GetInstance().GetDis(CenterPoint) < 50.0f)
	{
		LOG_CF_D(L"江流市中心不远处~");
		CPersonFunction::GetInstance().MoveToPoint(CenterPoint, 3, 50);
	}

	auto PrDead = FindNearestPoint(VecDeadToCenter, CPerson::GetInstance().GetPoint(), 30);
	if (PrDead.first != -1)
	{
		LOG_CF_D(L"从死亡点跑路到副本中心 Index=%d", PrDead.first);
		return MoveToVecPoint(VecDeadToCenter, PrDead.first);
	}

	auto PrCenter = FindNearestPoint(VecFengYueGuanToCenter, CPerson::GetInstance().GetPoint(), 30);
	if (PrCenter.first != -1)
	{
		LOG_CF_D(L"从风月馆跑路到副本中心 Index=%d, dis=%.2f, Vec.size=%d", PrCenter.first, CPerson::GetInstance().GetDis(VecFengYueGuanToCenter.at(PrCenter.first)),VecFengYueGuanToCenter.size());
		return MoveToVecPoint(VecFengYueGuanToCenter, PrCenter.first);
	}

	LOG_CF_D(L"离风月馆和死亡点的距离都很远, 直接用遁术!");
	MoveToMap(0x5F);

	LOG_CF_D(L"从风月馆跑路到副本中心");
	return MoveToVecPoint(VecFengYueGuanToCenter, 0);
}

BOOL CDailyTask_Move::MoveToMap(_In_ DWORD dwMapId)
{
	// Set CheckPointTimeout = 0, UnCheck Move Timeout!
	DWORD dwOldTimeOut = CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CHECKPAUSEPOINT, NULL);

	CConsoleLog(L"准备使用遁术!");
	while ((!CPersonFunction::GetInstance().IsGameLoad() || CPersonFunction::GetInstance().GetCityId() != dwMapId) && GameRun && !EMPTY_PERSONHP)
	{
		if (CPerson::GetInstance().GetPersonStatus() != em_WarStatus_None)
		{
			LOG_CF_D(L"等待脱战中……");
			CFarm::GetInstance().ClearAround_TarIdSelf(CBaseObject::CreateFixPointInfo(16.0f));
			BnsSleep(3000);
			continue;
		}

		if (!CPerson::GetInstance().IsLoadProgressBar())
		{
			LOG_CF_D(L"使用遁术中…… dwCityId=%X", dwMapId);
			CPersonFunction::GetInstance().TransportCiry(dwMapId);
			BnsSleep(5 * 1000);
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(20 * 1000, [] { return !CPersonFunction::GetInstance().IsGameLoad() && CPerson::GetInstance().GetPersonStatus() == em_WarStatus_None; });
		}
	}
	CPersonFunction::GetInstance().WaitGameLoad();

	// Print Msg to Console!
	CConsoleLog(L"使用遁术成功!");


	// Resume Check Move Timeout!
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CHECKPAUSEPOINT, dwOldTimeOut);
	return BnsSleep(5 * 1000);
}

BOOL CDailyTask_Move::MoveToFengYueGuanSellNpc()
{
	CONST static std::vector<Point> VecPoint = 
	{
		Point(-40068,17028,-1288),Point(-40160,16196,-1288),Point(-40420,15680,-1288),Point(-40672,15092,-1288),Point(-40908,14712,-1288),
	};

	auto Pr = FindNearestPoint(VecPoint, CPerson::GetInstance().GetPoint());
	if (Pr.first == -1)
	{
		MoveToMap(0x5F);
		MoveToVecPoint(VecPoint, 0);
	}
	else
	{
		MoveToVecPoint(VecPoint, Pr.first);
	}
	
	return TRUE;
}

BOOL CDailyTask_Move::MoveToFengYueGuanRepair()
{
	CONST static std::vector<Point> VecPoint =
	{
		Point(-40900,9164,-1740),Point(-40952,9920,-1740)
	};

	return MoveToVecPoint(VecPoint, 0);
}

std::pair<int, float> CDailyTask_Move::FindNearestPoint(_In_ CONST std::vector<Point>& Vec, _In_ CONST Point& CompPoint, _In_ float fMinDis) CONST
{
	int Index = -1;
	for (UINT i = 0;i < Vec.size(); ++i)
	{
		float fDis = CBaseObject::Get3DDis(CompPoint, Vec.at(i));
		if (fDis < fMinDis)
		{
			fMinDis = fDis;
			Index = static_cast<int>(i);
		}
	}

	return std::make_pair(Index, fMinDis);
}

BOOL CDailyTask_Move::MoveToTarget(_In_ CONST std::wstring& wsName, _In_ float fMaxDis) CONST
{
	BOOL bExist = CMonsterExtend::GetInstance().DoAction_By_MonsterName(wsName, CBaseObject::CreateFixPointInfo(fMaxDis), [fMaxDis](CONST CMonster& Monster)
	{
		CPersonFunction::GetInstance().MoveToPoint(Monster.GetFrontPoint(16), 2, fMaxDis);
	});

	bExist = bExist && CNpcExtend::GetInstance().DoAction_By_ExistNpc_For_Name(wsName, CBaseObject::CreateFixPointInfo(fMaxDis), [fMaxDis](CONST CNpc& Npc)
	{
		CPersonFunction::GetInstance().MoveToPoint(Npc.GetPoint(), 16, fMaxDis);
	});

	return bExist;
}

BOOL CDailyTask_Move::MoveToVecPoint(_In_ CONST std::vector<Point>& Vec, _In_ int Index) CONST
{
	for (UINT i = Index; i < Vec.size() && GameRun && !EMPTY_PERSONHP; ++i)
	{
		BnsMove(Vec.at(i), 3);
	}

	return Vec.size() == 0 ? TRUE : CPerson::GetInstance().GetDis(Vec.back()) < 3.0f;
}

VOID CDailyTask_Move::MoveToVecPoint(_In_ CONST std::vector<Point>& Vec, _In_ std::function<BOOL(CONST Point&)> ExitPtr) CONST
{
	while (GameRun && !EMPTY_PERSONHP && CPerson::GetInstance().GetDis(Vec.back()) > 3)
	{
		int nIndex = MyTools::CLPublic::GetRecentlyIndexByVec(Vec, CPerson::GetInstance().GetPoint(), 30.0f * 50);
		if (nIndex == -1)
		{
			auto& first = Vec.front();
			LOG_CF_D(L"和所有坐标距离都>30, first[%d,%d,%d]", first.GetX(), first.GetY(), first.GetZ());
			break;
		}

		if (CPersonFunction::GetInstance().IsGameLoad())
		{
			LOG_CF_D(L"走路过程中出现过图……");
			CPersonFunction::GetInstance().WaitGameLoad();
			LOG_CF_D(L"走路过程中过图完毕, 不继续走了!");
			break;
		}

		for (UINT i = nIndex; i < Vec.size() && GameRun && !EMPTY_PERSONHP; ++i)
		{
			if (ExitPtr(Vec.at(i)))
			{
				LOG_CF_D(L"ExitPtr = true!");
				return;
			}
			BnsMove(Vec.at(i), 3);
		}
	}
	
}

BOOL CDailyTask_Move::FarmAndMove(_In_ CONST std::vector<Point>& VecPoint, _In_ float fKillDis) CONST
{
	for (UINT i = 0; i < VecPoint.size(); ++i)
	{
		auto& CurrrentPoint = VecPoint.at(i);
		CPersonFunction::GetInstance().Move(CurrrentPoint);
		while (GameRun && !EMPTY_PERSONHP && CPerson::GetInstance().GetDis(CurrrentPoint) > 3)
		{
			if (CPersonFunction::GetInstance().IsGameLoad())
			{
				LOG_CF_D(L"走路过程中出现过图……");
				CPersonFunction::GetInstance().WaitGameLoad();
				break;
			}

			auto Range = CBaseObject::CreateFixPointInfo(fKillDis);
			if (CMonsterExtend::GetInstance().GetAroundMonsterCount(Range) > 0)
			{
				CPersonFunction::GetInstance().Move(CPerson::GetInstance().GetPoint());
				CFarm::GetInstance().ClearAroundMonster(Range, [] { return FALSE; });
			}

			if (CurrrentPoint != VecPoint.back())
			{
				auto& NextPoint = VecPoint.at(i + 1);
				if (CPerson::GetInstance().GetDis(NextPoint) < CBaseObject::Get3DDis(NextPoint, CurrrentPoint))
				{
					// Move Next!
					LOG_CF_D(L"清完怪后发现与下一个坐标的距离更短, 就直接break! 走下一个坐标去了!");
					break;
				}
			}

			if (!CPerson::GetInstance().IsMoving())
			{
				CPersonFunction::GetInstance().Move(CurrrentPoint);
			}
			BnsSleep(100);
		}

	}
	return TRUE;
}