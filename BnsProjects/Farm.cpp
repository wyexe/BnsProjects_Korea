#include "stdafx.h"
#include "Farm.h"
#include <MyTools/Log.h>
#include <MyTools/CLEchoException.h>
#include "MonsterExtend.h"
#include "PersonFunction.h"
#include "NpcExtend.h"
#include "SomeFun.h"
#include "TaskExtend.h"
#include "ResAction.h"
#include "BnsHook.h"
#include "Person.h"
#include "Npc.h"
#include "Monster.h"
#include "FarmTask.h"
#include "Task.h"
#include "Corpse.h"
#include "ResExtend.h"
#include "DropItem.h"
#include "ObjectFunction.h"
#include "GameUI.h"
#include "BnsGameFun.h"
#include "Cat.h"
#include "CatExpend.h"

#define _SELF L"Farm.cpp"

BOOL CFarm::ClearAroundMonster_By_Condiction(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<BOOL(void)> fn) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__,[&]
	{
		while (GameRun && !EMPTY_PERSONHP && !fn())
		{
			CNpc Npc;
			CMonster Monster;

			BnsSleep(500);
			CMonsterExtend::GetInstance().GetTopMonster_By_Dis(FixPtInfo, &Monster);
			CNpcExtend::GetInstance().GetTopNpc_By_Dis(FixPtInfo, &Npc);

			if (Npc.GetBase() == NULL && Monster.GetBase() != NULL) // 附近只有怪物
			{
				LOG_CF_D(L"附近只有怪物");
				CFarm::GetInstance().ClearAroundMonster(FixPtInfo, fn);
				continue;
			}
			else if (Npc.GetBase() != NULL && Monster.GetBase() == NULL) // 附近只有Npc
			{
				LOG_CF_D(L"附近只有Npc");
				CFarm::GetInstance().ClearAroundNpc(FixPtInfo, fn);
				continue;
			}
			else if (Npc.GetBase() != NULL && Monster.GetBase() != NULL) // 谁近打谁!
			{
				LOG_CF_D(L"Npc的距离是:%.2f, 怪物的距离是:%.2f", Npc.GetDis(), Monster.GetDis());
				LOG_CF_D(L"周围Npc数量=%d, Monster=%d", CNpcExtend::GetInstance().GetAroundNpcCount(FixPtInfo), CMonsterExtend::GetInstance().GetAroundMonsterCount(FixPtInfo));
				if (Npc.GetDis() < Monster.GetDis())
					CFarm::GetInstance().ClearAroundNpc(FixPtInfo, fn);
				else
					CFarm::GetInstance().ClearAroundMonster(FixPtInfo, fn);
				continue;
			}
		}

		CFarm::GetInstance().EatFood(CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_FOODPERCENT));
		return TRUE;
	});
}

BOOL CFarm::FarmAround_By_PointList(_In_ CONST FixPointInfo& FixPtInfo, _In_ CONST std::vector<Point>& vPoint, _In_ std::function<BOOL(VOID)> fnComplete) CONST
{
	auto fnFarmAround_By_TaskId = [&fnComplete](_In_ CONST FixPointInfo& FixPtInfo)
	{
		CFarm::GetInstance().Farm_By_Condiction(FixPtInfo, 
			[]{ return CMonsterExtend::GetInstance().GetAroundMonsterCount(CBaseObject::CreateFixPointInfo(5.0f)); },
			[&fnComplete](std::vector<CMonster>& vlst, CONST FixPointInfo& FixPtInfo){
			return CMonsterExtend::GetInstance().GetAroundMonsterList(FixPtInfo, vlst) != 0 && !fnComplete();
		});

		//if (CMonsterExtend::GetInstance().GetAroundMonsterCount(FixPtInfo) == 0)
		//	CResAction::GetInstance().TakeDropItem(FixPtInfo);
		return fnComplete();
	};

	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__,[&]
	{
		int nIndex = 0;
		BOOL bCompleteMission = FALSE;
		while (GameRun && !EMPTY_PERSONHP && !bCompleteMission)
		{
			// 先顺着坐标走
			for (nIndex = 0; nIndex < static_cast<int>(vPoint.size()) && !bCompleteMission && GameRun && !EMPTY_PERSONHP; ++nIndex)
			{
				LOG_CF_D( L"正时针方向 去第%d个坐标 刷怪", nIndex);
				BnsMove(vPoint.at(nIndex), 3.0f);
				if (fnFarmAround_By_TaskId(CBaseObject::CreateFixPointInfo(FixPtInfo.fDis)))
					bCompleteMission = TRUE;
				BnsMove(vPoint.at(nIndex), 3.0f);
			}

			for (int i = nIndex - 1; i >= 0 && GameRun && !EMPTY_PERSONHP; --i)
			{
				LOG_CF_D( L"逆时针方向 第%d个坐标 刷怪", i);
				BnsMove(vPoint.at(i), 3.0f);
				if (!bCompleteMission && fnFarmAround_By_TaskId(CBaseObject::CreateFixPointInfo(FixPtInfo.fDis)))
				{
					bCompleteMission = TRUE;
					BnsMove(vPoint.at(i), 3.0f);
				}
			}
			BnsSleep(100);
		}
		LOG_CF_D(L"回原点完毕!");
		return TRUE;
	});
}

BOOL CFarm::TakeCorpse_By_PointList(_In_ CONST TaskInfoParm& TaskParm, _In_ CONST FixPointInfo& FixPtInfo, _In_ CONST std::vector<Point>& vPoint, _In_ CONST std::wstring& wsCorpseName) CONST
{
	auto fnFarmAround_By_TaskId = [&TaskParm, &wsCorpseName, this](_In_ CONST FixPointInfo& FixPtInfo)
	{
		if (CResExtend::GetInstance().GetAroundCorpseCount(FixPtInfo) > 0)
			TakeCorpse(NULL, FixPtInfo, wsCorpseName);

		CFarm::GetInstance().ClearAroundMonster(FixPtInfo, []{return FALSE; });
		return CTask::GetInstance().IsCompletionTask(TaskParm);
	};

	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__,[&]
	{
		int nIndex = 0;
		BOOL bCompleteMission = FALSE;
		while (GameRun && !EMPTY_PERSONHP && !bCompleteMission)
		{
			// 先顺着坐标走
			for (nIndex = 0; nIndex < static_cast<int>(vPoint.size()) && !bCompleteMission && GameRun && !EMPTY_PERSONHP; ++nIndex)
			{
				LOG_CF_D(L"正时针方向 去第%d个坐标 刷怪", nIndex);
				BnsMove(vPoint.at(nIndex), 2.0f);
				if (fnFarmAround_By_TaskId(CBaseObject::CreateFixPointInfo(FixPtInfo.fDis)))
					bCompleteMission = TRUE;
				BnsMove(vPoint.at(nIndex), 2.0f);
			}

			for (int i = nIndex - 1; i >= 0 && GameRun && !EMPTY_PERSONHP; --i)
			{
				LOG_CF_D(L"逆时针方向 第%d个坐标 刷怪", i);
				BnsMove(vPoint.at(i), 2.0f);
				if (!bCompleteMission && fnFarmAround_By_TaskId(CBaseObject::CreateFixPointInfo(FixPtInfo.fDis)))
				{
					bCompleteMission = TRUE;
					BnsMove(vPoint.at(i), 2.0f);
				}
			}
			BnsSleep(500);
		}

		// Clear Around monster
		return CFarm::GetInstance().ClearAround_TarIdSelf(FixPtInfo);
	});
}

BOOL CFarm::TakeCorpse(_In_ DWORD dwBuffId, _In_ CONST FixPointInfo& FixPtInfo, _In_ CONST std::wstring& wsCorpseName) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__,[&]
	{
		BOOL bExist = FALSE;
		while (GameRun && !EMPTY_PERSONHP && !CPersonFunction::GetInstance().ExistBuff_Person(dwBuffId, (GAMEOBJ_BUFF *)nullptr) && !bExist)
		{
			BnsSleep(500);
			CFarm::GetInstance().ClearAround_TarIdSelf(FixPtInfo);

			std::vector<CCorpse> vlst;
			CResExtend::GetInstance().GetAroundCorpseList(FixPtInfo, vlst);
			LOG_CF_D(L"附近符合条件的尸体数量:%d, 范围:%.2f", vlst.size(), FixPtInfo.fDis);
			if (vlst.size() == 0)
			{
				if (dwBuffId == NULL)
					break;
				continue;
			}



			CCorpse Corpse;
			if (MyTools::CLPublic::Vec_find_if(vlst, &Corpse, [&wsCorpseName](CONST CCorpse& itm) {return !(wsCorpseName != L"" && itm.GetName() != wsCorpseName); }))
			{
				LOG_CF_D(L"捡尸体,ID:%X", Corpse.GetId());
				Corpse.Take();
			}

			if (dwBuffId == NULL || CPersonFunction::GetInstance().ExistBuff_Person(dwBuffId, (GAMEOBJ_BUFF *)nullptr))
				bExist = TRUE;
		}

		return TRUE;
	});
}

BOOL CFarm::TakeCorpse(_In_ CONST FixPointInfo& FixPtInfo, _In_ CONST std::wstring& wsCorpseName) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__,[&]
	{
		while (GameRun && !EMPTY_PERSONHP)
		{
			BnsSleep(500);
			CFarm::GetInstance().ClearAround_TarIdSelf(FixPtInfo);

			std::vector<CCorpse> vlst;
			if (CResExtend::GetInstance().GetAroundCorpseList(FixPtInfo, vlst) == 0)
				break;

			CCorpse Corpse;
			if (!MyTools::CLPublic::Vec_find_if(vlst, &Corpse, [&wsCorpseName](CONST CCorpse& itm) {return  itm.GetName() == wsCorpseName && itm.IsCollect(); }))
				break;

			LOG_CF_D(L"捡尸体,ID:%X", Corpse.GetId());
			Corpse.Take();

			if (!Corpse.IsCollect())
			{
				LOG_CF_D(L"采集成功! 状态已经改变");
				break;
			}

		}
		return TRUE;
	});
}

BOOL CFarm::TakeCorpse_By_Status(_In_ CONST FixPointInfo& FixPtInfo, _In_ CONST std::wstring& wsCorpseName) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__,[&]
	{
		BOOL bExist = FALSE;
		while (GameRun && !EMPTY_PERSONHP && !bExist)
		{
			BnsSleep(500);
			CFarm::GetInstance().ClearAround_TarIdSelf(FixPtInfo);

			std::vector<CCorpse> vlst;
			if (CResExtend::GetInstance().GetAroundCorpseList(FixPtInfo, vlst) == 0)
				continue;

			CCorpse Corpse;
			if (MyTools::CLPublic::Vec_find_if(vlst, &Corpse, [&wsCorpseName](CONST CCorpse& itm) {return !(wsCorpseName != L"" && itm.GetName() != wsCorpseName); }))
			{
				LOG_CF_D(L"捡尸体,ID:%X", Corpse.GetId());
				Corpse.Take();

				if (!Corpse.IsCollect())
				{
					LOG_CF_D(L"采集成功! 状态已经改变");
					bExist = TRUE;
				}
			}
		}

		return TRUE;
	});
}

BOOL CFarm::PickCorpse(_In_ DWORD dwBuffId, _In_ CONST FixPointInfo& FixPtInfo) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__,[&]
	{
		while (GameRun && !EMPTY_PERSONHP && !CPersonFunction::GetInstance().ExistBuff_Person(dwBuffId, (GAMEOBJ_BUFF *)nullptr))
		{
			BnsSleep(500);
			CMonster Monster;
			if (!CMonsterExtend::GetInstance().GetTopMonster_By_Dis(FixPtInfo, &Monster))
				continue;

			DWORD dwMosterId = Monster.GetId();
			CFarm::GetInstance().KillMonster_By_Single(Monster, FixPtInfo, TRUE, [] {return 1; });
			BnsSleep(1500);

			CDropItem Corpse;
			if (!CResExtend::GetInstance().FindDropItemCorpse_By_Id(dwMosterId, CBaseObject::CreateFixPointInfo(FixPtInfo.fDis), &Corpse))
			{
				LOG_CF_D(L"这个怪物打死后并木有掉下尸体!");
				continue;
			}
			LOG_CF_D(L"抱起一个尸体,ID:%X", Corpse.GetId());
			BnsSleep(1000);
			Corpse.PickCorpse();
			//Corpse.Take();
			BnsSleep(2000);
		}

		CResAction::GetInstance().TakeDropItem(FixPtInfo);
		return BnsMove(FixPtInfo.FixPoint, 3.0f);
	});
}

BOOL CFarm::PickCorpse_By_PointList(_In_ DWORD dwBuffId, _In_ CONST FixPointInfo& FixPtInfo, _In_ CONST std::vector<Point>& vPoint) CONST
{
	auto fnPickCorpse = [](_In_ CONST FixPointInfo& FixPtInfo)
	{
		BnsSleep(500);

		CMonster Monster;
		if (!CMonsterExtend::GetInstance().GetTopMonster_By_Dis(FixPtInfo, &Monster))
			return FALSE;

		DWORD dwMonsterId = Monster.GetId();
		CFarm::GetInstance().KillMonster_By_Single(Monster, FixPtInfo, TRUE, []{return 1; });
		BnsSleep(1000);
		CDropItem Corpse;
		if (!CResExtend::GetInstance().FindDropItemCorpse_By_Id(dwMonsterId, FixPtInfo, &Corpse))
		{
			LOG_CF_D( L"这个怪物打死后并木有掉下尸体!");
			return FALSE;
		}
		BnsSleep(1000);
		Corpse.PickCorpse();
		//Corpse.Take();
		BnsSleep(1000);
		LOG_CF_D( L"抱起一个尸体,ID:%X", Corpse.GetId());
		return CPerson::GetInstance().IsPickStatus();
	};

	auto fnComplete = [&dwBuffId]
	{
		return CPersonFunction::GetInstance().ExistBuff_Person(dwBuffId, nullptr);
	};

	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__,[&]
	{
		int nIndex = 0;
		BOOL bComplete = fnComplete();
		while (GameRun && !EMPTY_PERSONHP && !bComplete)
		{
			// 先顺着坐标走
			for (nIndex = 0; nIndex < static_cast<int>(vPoint.size()) && !bComplete && GameRun && !EMPTY_PERSONHP; ++nIndex)
			{
				LOG_CF_D(L"正在顺方向 走第%d 个坐标", nIndex);
				BnsMove(vPoint.at(nIndex), 3.0f);
				fnPickCorpse(CBaseObject::CreateFixPointInfo(FixPtInfo.fDis));
				if (fnComplete())
					bComplete = TRUE;
				BnsMove(vPoint.at(nIndex), 3.0f);
			}

			// 逆向走回去
			for (int i = nIndex - 1; i >= 0 && GameRun && !EMPTY_PERSONHP; --i)
			{
				LOG_CF_D(L"正在顺方向 走第%d 个坐标", i);
				BnsMove(vPoint.at(i), 3.0f);
				if (bComplete)
					continue;

				fnPickCorpse(CBaseObject::CreateFixPointInfo(FixPtInfo.fDis));
				if (fnComplete())
				{
					bComplete = TRUE;
					BnsMove(vPoint.at(i), 3.0f);
				}

			}
			BnsSleep(500);
		}

		return TRUE;
	});
}

BOOL CFarm::PickCorpse_By_PointList(_In_ CONST std::wstring& wsCorpseName, _In_ DWORD dwBuffId, _In_ CONST FixPointInfo& FixPtInfo, _In_ CONST std::vector<Point>& vPoint) CONST
{
	auto fnPickCorpse = [this, &wsCorpseName](_In_ CONST FixPointInfo& FixPtInfo)
	{
		BnsSleep(500);
		TakeCorpse(NULL, FixPtInfo, wsCorpseName);
	};

	auto fnComplete = [&dwBuffId]
	{
		return CPersonFunction::GetInstance().ExistBuff_Person(dwBuffId, nullptr);
	};

	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__,[&]
	{
		int nIndex = 0;
		BOOL bComplete = fnComplete();
		while (GameRun && !EMPTY_PERSONHP && !bComplete)
		{
			// 先顺着坐标走
			for (nIndex = 0; nIndex < static_cast<int>(vPoint.size()) && !bComplete && GameRun && !EMPTY_PERSONHP; ++nIndex)
			{
				LOG_CF_D(L"正在顺方向 走第%d 个坐标", nIndex);
				BnsMove(vPoint.at(nIndex), 3.0f);
				fnPickCorpse(CBaseObject::CreateFixPointInfo(FixPtInfo.fDis));
				if (fnComplete())
					bComplete = TRUE;
				BnsMove(vPoint.at(nIndex), 3.0f);
			}

			// 逆向走回去
			for (int i = nIndex - 1; i >= 0 && GameRun && !EMPTY_PERSONHP; --i)
			{
				LOG_CF_D(L"正在顺方向 走第%d 个坐标", i);
				BnsMove(vPoint.at(i), 3.0f);
				if (bComplete)
					continue;

				fnPickCorpse(CBaseObject::CreateFixPointInfo(FixPtInfo.fDis));
				if (fnComplete())
				{
					bComplete = TRUE;
					BnsMove(vPoint.at(i), 3.0f);
				}

			}
			BnsSleep(500);
		}

		return TRUE;
	});
}

BOOL CFarm::TakeCorpseItem(_In_ CONST TaskInfoParm& TaskParm, _In_ CONST FixPointInfo& FixPtInfo, __in CONST std::wstring& wsCorpseName) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__,[&]
	{
		while (GameRun && !EMPTY_PERSONHP)
		{
			if (TaskParm.dwTaskId != NULL && CTask::GetInstance().IsCompletionTask(TaskParm))
			{
				LOG_CF_D(L"捡尸体任务完成!");
				break;
			}

			BnsSleep(500);
			CFarm::GetInstance().ClearAroundMonster(FixPtInfo, [] {return FALSE; });

			std::vector<CCorpse> vlst;
			if (CResExtend::GetInstance().GetAroundCorpseList(FixPtInfo, vlst) == 0)
			{
				if (TaskParm.dwTaskId == NULL)
				{
					LOG_CF_D(L"附近找不到尸体, 并且TaskID=0, 不找了!");
					break;
				}
				continue;
			}

			CCorpse Corpse;
			if (MyTools::CLPublic::Vec_find_if(vlst, &Corpse, [&wsCorpseName](CONST CCorpse& itm) { return !(wsCorpseName != L"" && itm.GetName() != wsCorpseName); }))
			{
				Corpse.Take();
				if (TaskParm.dwTaskId == 0)
					break;
			}
		}
		return TRUE;
	});
}

BOOL CFarm::TakeNpcItem(_In_ CONST TaskInfoParm& TaskParm, _In_ CONST FixPointInfo& FixPtInfo, _In_ CONST std::wstring& wsNpcName) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__,[&]
	{
		while (GameRun && !EMPTY_PERSONHP && !CTask::GetInstance().IsCompletionTask(TaskParm))
		{
			BnsSleep(500);
			std::vector<CNpc> vlst;
			if (CNpcExtend::GetInstance().GetAroundNpcList(FixPtInfo, vlst) == 0)
				continue;

			CNpc Npc;
			if (MyTools::CLPublic::Vec_find_if(vlst, &Npc, [&wsNpcName](CONST CNpc& itm) { return !(wsNpcName != L"" && itm.GetName() != wsNpcName); }))
			{
				Npc.Take(CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_TAKENPCTIME));
				CFarm::GetInstance().ClearAroundMonster(FixPtInfo, [] {return FALSE; });
				if (TaskParm.dwTaskId == 0)
					break;
			}
		}
		return TRUE;
	});
}

BOOL CFarm::TakeNpcItem(_In_ CONST std::wstring& wsNpcName, _In_ CONST FixPointInfo& FixPtInfo) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__,[&]
	{
		CNpc Npc;
		if (CNpcExtend::GetInstance().FindNpc_By_Condiction(FixPtInfo, &Npc, [&wsNpcName](CONST CNpc& Npc) {
			return wsNpcName == L"" || wsNpcName == Npc.GetName();
		}))
		{
			LOG_CF_D(L"捡Npc!, Id=%X, Name:%s", Npc.GetId(), Npc.GetName().c_str());
			Npc.Take(CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_TAKENPCTIME));
			return TRUE;
		}

		return FALSE;
	});
}

BOOL CFarm::TakeNpcItem_By_PointList(_In_ CONST TaskInfoParm& TaskParm, _In_ CONST FixPointInfo& FixPtInfo, _In_ CONST std::wstring& wsNpcName, _In_ CONST std::vector<Point>& vPoint) CONST
{
	auto fnComplete = [&TaskParm]
	{
		return CTask::GetInstance().IsCompletionTask(TaskParm);
	};

	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__,[&]
	{
		BOOL bComplete = fnComplete();
		int nIndex = 0;
		while (GameRun && !EMPTY_PERSONHP && !bComplete)
		{
			// 先顺着坐标走
			for (nIndex = 0; nIndex < static_cast<int>(vPoint.size()) && !bComplete && GameRun && !EMPTY_PERSONHP; ++nIndex)
			{
				LOG_CF_D(L"正在顺方向 走第%d 个坐标", nIndex);
				BnsMove(vPoint.at(nIndex), 3.0f);
				CFarm::GetInstance().ClearAroundMonster(CBaseObject::CreateFixPointInfo(FixPtInfo.fDis), [] {return FALSE; });
				TakeNpcItem(wsNpcName, CBaseObject::CreateFixPointInfo(FixPtInfo.fDis));
				if (fnComplete())
					bComplete = TRUE;

				BnsMove(vPoint.at(nIndex), 3.0f);
			}

			// 逆向走回去
			for (int i = nIndex - 1; i >= 0 && GameRun && !EMPTY_PERSONHP; --i)
			{
				LOG_CF_D(L"正在逆向走第%d个坐标", i);
				BnsMove(vPoint.at(i), 3.0f);
				if (!bComplete)
				{
					CFarm::GetInstance().ClearAroundMonster(CBaseObject::CreateFixPointInfo(FixPtInfo.fDis), [] {return FALSE; });
					TakeNpcItem(wsNpcName, CBaseObject::CreateFixPointInfo(FixPtInfo.fDis));
					BnsMove(vPoint.at(i), 3.0f);
				}
			}
			BnsSleep(500);
		}
		return TRUE;
	});
}

BOOL CFarm::TakeBall(_In_ DWORD dwBuffId, _In_ CONST TaskInfoParm& TaskParm, _In_ CONST FixPointInfo& FixPtInfo) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__,[&]
	{
		BOOL bRetCode = FALSE;
		while (GameRun && !EMPTY_PERSONHP)
		{
			BnsSleep(500);

			if (dwBuffId != NULL && CPersonFunction::GetInstance().ExistBuff_Person(dwBuffId, nullptr))
			{
				LOG_CF_D(L"身上有BUFFID:%X 捡球完成!", dwBuffId);
				bRetCode = TRUE;
				break;
			}
			if (TaskParm.dwTaskId != NULL && CTask::GetInstance().IsCompletionTask(TaskParm))
			{
				LOG_CF_D(L"任务:%X 进度已经变了! 捡球完成!", TaskParm.dwTaskId);
				bRetCode = TRUE;
				break;
			}
			if (TaskParm.dwTaskId == NULL && CPerson::GetInstance().IsPickStatus())
			{
				LOG_CF_D(L"当前是捡物的状态了! 强制算任务完成!");
				bRetCode = TRUE;
				break;
			}

			CFarm::GetInstance().ClearAroundMonster(FixPtInfo, [] {return FALSE; }); // 先清周围的怪

			CCorpse Corpse;
			if (!CResExtend::GetInstance().FindTopBall(FixPtInfo, std::vector<DWORD>(), &Corpse)) // 找最近的球
				break;

			LOG_CF_D(L"捡球,ID:%X", Corpse.GetId());
			Corpse.Take(NULL);
		}

		BnsMove(FixPtInfo.FixPoint, 3.0f);
		return bRetCode;
	});
}

BOOL CFarm::SetRandChannel() CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__,[&]
	{
		DWORD dwChannel = (CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->dwSortIndex % 5) + 1;
		LOG_CF_D(L"窗口排序:%d,切换频道%d", CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->dwSortIndex, dwChannel);
		return CPersonFunction::GetInstance().SwitchChannel(dwChannel);
	});
}

BOOL CFarm::SetFixChannel(_In_ DWORD dwChannel, _In_opt_ BOOL bForce) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__,[&]
	{
		DWORD dwCount = 0;
		dwChannel = (dwChannel == 0) ? CPersonFunction::GetInstance().GetMaxChannel() : dwChannel;

		while (CPerson::GetInstance().GetChannel() != dwChannel && GameRun && !EMPTY_PERSONHP && dwChannel != NULL)
		{
			BnsSleep(3000);
			if (CPerson::GetInstance().GetPersonStatus() != em_WarStatus_None)
			{
				LOG_CF_D(L"等待脱战中……");
				CFarm::GetInstance().ClearAround_TarIdSelf(CBaseObject::CreateFixPointInfo(16.0f));
				continue;
			}

			if (dwChannel <= 0)
			{
				LOG_CF_D(L"已经没有线路可换了,下次要换的线路是%d了", dwChannel);
				break;
			}

			LOG_CF_D(L"dwChannel=%d", dwChannel);
			CPersonFunction::GetInstance().SwitchChannel(dwChannel);
			BnsSleep(5 * 1000);

			// Is Force to Switch Target Channel!
			if (bForce)
				continue;

			if (dwCount++ == 6)
			{
				LOG_CF_D(L"换线次数达到6次, 不换了");
				break;
			}

			if (dwCount++ % 2 == 0)
			{
				dwChannel -= 1;
				if (dwChannel <= 1)
				{
					LOG_CF_D(L"当前频道已经=1,不能继续换频道了");
					break;
				}
				LOG_CF_D(L"换线次数达到%d次, 去下一个频道:%d", dwCount, dwChannel);
			}
		}

		CGameVariable::GetInstance().GetSetSwitchChannelTick() = ::GetTickCount64();
		return TRUE;
	});
}

BOOL CFarm::Pickup_By_Name(_In_ CONST std::wstring& wsName) CONST
{
	BOOL		bRetCode = FALSE;
	FixPointInfo FixPtInfo = CBaseObject::CreateFixPointInfo(6.0f);


	auto fnPickup_By_Monster = [&wsName, &FixPtInfo]
	{
		BOOL		bRetCode = FALSE;
		CMonster	Monster;

		if (CMonsterExtend::GetInstance().FindMonster_By_Name(wsName,FixPtInfo, &Monster))
		{
			CPersonFunction::GetInstance().Pickup(Monster.GetObjectId());
			bRetCode = TRUE;
		}

		return bRetCode;
	};

	auto fnPickup_By_Npc = [&wsName, &FixPtInfo]
	{
		BOOL		bRetCode = FALSE;
		CNpc		Npc;

		if (CNpcExtend::GetInstance().FindNpc_By_Name(wsName, FixPtInfo, &Npc))
		{
			CPersonFunction::GetInstance().Pickup(Npc.GetObjectId());
			bRetCode = TRUE;
		}

		return bRetCode;
	};

	auto fnPickup_By_Corpse = [&wsName, &FixPtInfo]
	{
		BOOL		bRetCode = FALSE;
		CCorpse		Corpse;

		if (CResExtend::GetInstance().FindCorpse_By_Name(wsName, FixPtInfo, &Corpse))
		{
			CPersonFunction::GetInstance().Pickup(Corpse.GetObjectId());
			bRetCode = TRUE;
		}

		return bRetCode;
	};


	if (fnPickup_By_Monster() || fnPickup_By_Npc() || fnPickup_By_Corpse())
		bRetCode = TRUE;
	else
		LOG_CF_D(L"找不到对象:%s", wsName.c_str());

	return bRetCode;
}

BOOL CFarm::Pickup_By_LongMai(_In_ CONST std::wstring& wsLongMaiName) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__,[&]
	{
		std::vector<CCorpse> vlst;
		CObjectFunction::GetInstance().GetSortGameObjectList(vlst, CObjectFunction::em_GameObject_Type_Corpse, CBaseObject::CreateFixPointInfo(-1));

		std::for_each(vlst.begin(), vlst.end(), [&wsLongMaiName, this](CCorpse& Corpse) {
			if (Corpse.GetName() == wsLongMaiName)
			{
				LOG_CF_D(L"找到龙脉了");
				while (CPerson::GetInstance().GetPersonStatus() != em_WarStatus_None && GameRun && !EMPTY_PERSONHP)
				{
					CFarm::GetInstance().ClearAround_TarIdSelf(CBaseObject::CreateFixPointInfo(16.0f));
					BnsSleep(1000);
				}
				CPersonFunction::GetInstance().Pickup(Corpse.GetObjectId());
				CSomeFun::GetInstance().SimlationKey('F');
				BnsSleep(1000);
				LOG_CF_D(L"捡起龙脉完毕!");
			}
		});

		return TRUE;
	});
}

BOOL CFarm::TakeBall_By_PointList(_In_ DWORD dwBuffId, _In_ CONST TaskInfoParm& TaskParm, _In_ CONST FixPointInfo& FixPtInfo, _In_ CONST std::vector<Point>& vPoint) CONST
{
	int nIndex = 0;
	auto fnComplete = [&TaskParm, &dwBuffId]
	{
		return  (TaskParm.dwTaskId != NULL && CTask::GetInstance().IsCompletionTask(TaskParm)) ||
				(dwBuffId != NULL && CPersonFunction::GetInstance().ExistBuff_Person(dwBuffId, nullptr)) || 
				CPerson::GetInstance().IsPickStatus();
	};
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__,[&]
	{
		BOOL bComplete = fnComplete();
		while (GameRun && !EMPTY_PERSONHP && !bComplete)
		{
			// 先顺着坐标走
			for (nIndex = 0; nIndex < static_cast<int>(vPoint.size()) && !bComplete && GameRun && !EMPTY_PERSONHP; ++nIndex)
			{
				auto& Pt = vPoint.at(nIndex);
				LOG_CF_D(L"正在顺向走第%d个坐标,(%d,%d,%d)", nIndex, Pt.GetX(), Pt.GetY(), Pt.GetZ());
				BnsMove(Pt, 3.0f);
				if (TakeBall(dwBuffId, TaskParm, CBaseObject::CreateFixPointInfo(FixPtInfo.fDis)))
					bComplete = TRUE;
				BnsMove(Pt, 3.0f);
			}

			// 逆向走回去
			for (int i = nIndex - 1; i >= 0 && GameRun && !EMPTY_PERSONHP; --i)
			{
				auto& Pt = vPoint.at(i);
				LOG_CF_D(L"正在逆向走第%d个坐标,(%d,%d,%d)", i, Pt.GetX(), Pt.GetY(), Pt.GetZ());

				BnsMove(Pt, 3.0f);
				if (!bComplete && TakeBall(dwBuffId, TaskParm, CBaseObject::CreateFixPointInfo(FixPtInfo.fDis)))
				{
					bComplete = TRUE;
					BnsMove(Pt, 3.0f);
				}
			}
			BnsSleep(500);
		}
		return TRUE;
	});
}

BOOL CFarm::Pickup_By_Flag(_In_ CONST FixPointInfo&) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__,[&]
	{
		if (!CObjectFunction::GetInstance().IsShow_In_GameUi(L"EnvObjectToolTipPanel"))
		{
			LOG_CF_D(L"并没有显示F!");
			return FALSE;
		}

		CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"EnvObjectToolTipPanel", [](CONST CGameUi& EnvObjectToolTipPanel) {
			CPersonFunction::GetInstance().Pickup(GameObjectID(ReadDWORD(EnvObjectToolTipPanel.GetObj() + 0x40), ReadDWORD(EnvObjectToolTipPanel.GetObj() + 0x44)));
		});

		CSomeFun::GetInstance().SimlationKey('F');
		BnsSleep(1000);
		CPersonFunction::GetInstance().WaitForProgressBar(4 * 1000);

		while (CObjectFunction::GetInstance().IsShow_In_GameUi(L"PickupItemPanel") && GameRun)
		{
			LOG_CF_D(L"PickupItemPanel");
			CSomeFun::GetInstance().SimlationKey('F');
			BnsSleep(1000);
		}

		return TRUE;
	});
}

BOOL CFarm::FarmAround_By_Condition_To(_In_ std::function<BOOL(VOID)> fn) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__,[&]
	{
		auto FixPoint = CBaseObject::GetPersonPoint();
		auto dwTick = GetTickCount64();
		float fDis = 5.0f;
		while (GameRun && !EMPTY_PERSONHP)
		{
			if (CPerson::GetInstance().GetPersonStatus() != em_WarStatus_None)
			{
				// 30s 之内还是战斗状态!
				if (fDis == 5.0f && static_cast<DWORD>(::GetTickCount64() - dwTick) >= 30 * 1000)
				{
					LOG_CF_D(L"30s 之内还是战斗状态! 增加到16m的范围!");
					fDis = 16.0f;
				}
				CFarm::GetInstance().ClearAround_TarIdSelf(FixPointInfo(CBaseObject::GetPersonPoint(), fDis));
				BnsSleep(1000);
				continue;
			}
			if (!fn())
			{
				LOG_CF_D(L"条件完毕! break;!");
				break;
			}
			BnsSleep(500);
		}

		return BnsMove(FixPoint, 2);
	});
}

BOOL CFarm::Pickup_By_RepeatName(_In_ CONST std::wstring& wsName) CONST
{
	BOOL		bRetCode = FALSE;
	FixPointInfo FixPtInfo = CBaseObject::CreateFixPointInfo(10.0f);


	auto fnPickup_By_Monster = [&wsName, &FixPtInfo]
	{
		BOOL		bRetCode = FALSE;
		CMonster	Monster;

		CMonsterExtend::GetInstance().DoAction_By_MonsterName(wsName, FixPtInfo, [&bRetCode](CONST CMonster& Monster){
			CPersonFunction::GetInstance().Pickup(Monster.GetObjectId());
			::Sleep(1000);
			CPersonFunction::GetInstance().WaitForProgressBar();
			bRetCode = TRUE;
		});

		return bRetCode;
	};

	auto fnPickup_By_Npc = [&wsName, &FixPtInfo]
	{
		BOOL		bRetCode = FALSE;
		CNpc		Npc;

		CNpcExtend::GetInstance().DoAction_By_ExistNpc_For_Name(wsName, FixPtInfo, [&bRetCode](CONST CNpc& Npc){
			CPersonFunction::GetInstance().Pickup(Npc.GetObjectId());
			::Sleep(1000);
			CPersonFunction::GetInstance().WaitForProgressBar();
			bRetCode = TRUE;
		});
		return bRetCode;
	};

	auto fnPickup_By_Corpse = [&wsName, &FixPtInfo]
	{
		BOOL		bRetCode = FALSE;
		CCorpse		Corpse;

		CResExtend::GetInstance().DoAction_By_ExistCorpse_For_Name(wsName, FixPtInfo, [&bRetCode](CONST CCorpse& Corpse){
			CPersonFunction::GetInstance().Pickup(Corpse.GetObjectId());
			::Sleep(1000);
			CPersonFunction::GetInstance().WaitForProgressBar();
			bRetCode = TRUE;
		});

		return bRetCode;
	};


	if (fnPickup_By_Monster() || fnPickup_By_Npc() || fnPickup_By_Corpse())
		bRetCode = TRUE;
	else
		LOG_CF_D( L"找不到对象:%s", wsName.c_str());

	return bRetCode;
}

BOOL CFarm::FarmCorpse(_In_ CONST std::wstring& wsCorpseName, _In_ CONST FixPointInfo& FixPtInfo) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__,[&]
	{
		while (GameRun && !EMPTY_PERSONHP)
		{
			BnsSleep(500);

			std::vector<CCorpse> vlst;
			if (CResExtend::GetInstance().GetAroundCorpseList(FixPtInfo, vlst) == 0)
				break;

			for (CONST auto& itm : vlst)
			{
				if (itm.GetName() != wsCorpseName && wsCorpseName != L"")
					continue;

				if (!itm.IsCollect())
					continue;

				if (itm.GetDis() > 2.0f)
				{
					BnsMove(itm.GetPoint(), 2);
					BnsSleep(1000);
				}
				itm.SetAngle();
				BnsSleep(500);
				CSomeFun::GetInstance().SimlationKey('R');
				Sleep(100);
				CSomeFun::GetInstance().SimlationKey('T');
				break;
			}
		}
		return TRUE;
	});
}

BOOL CFarm::FarmAround_By_Round(_In_ CONST FixPointInfo& RoundInfo, _In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<BOOL(void)> fnExit) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__,[&]
	{
		std::vector<CMonster> vlst;
		while (GameRun && !EMPTY_PERSONHP && !fnExit())
		{
			if (CMonsterExtend::GetInstance().GetAroundMonsterList(FixPtInfo, vlst) == 0)
			{
				BnsSleep(500);
				continue;
			}

			CMonster& Monster = vlst.at(0);
			LOG_CF_D(L"Monster.Name=%s", Monster.GetName().c_str());
			CFarm::GetInstance().KillMonster_By_Round(Monster, FixPtInfo, RoundInfo);
		}
		return TRUE;
	});
}

BOOL CFarm::BeckonCat(_In_ BOOL bIgnoreWarStatus) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__,[&]
	{
		if (CPerson::GetInstance().GetClasses() != em_Player_Classes::em_Player_Classes_Summoner)
			return TRUE;


		if (CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_DISABLE_BECKONCAT) == 1)
			return TRUE;

		static BOOL sbBeckonCat = FALSE;
		if (sbBeckonCat)
		{
			LOG_CF_D(L"BeckonCat Loop");
			return TRUE;
		}

		// 设置禁止召唤猫, 并且保存原来的值, 防止递归死循环!
		sbBeckonCat = TRUE;

		CCat Cat;
		if (!CCatExpend::GetInstance().FindSelfCat(&Cat))
		{
			LOG_CF_D(L"找不到猫……应该还没拿猫!");
			sbBeckonCat = FALSE;
			return TRUE;
		}

		Point FixPoint = CPerson::GetInstance().GetPoint();
		BOOL bCALLCat = Cat.GetHp() == NULL;


		while (GameRun && !EMPTY_PERSONHP && Cat.GetHp() == NULL)
		{
			if (!bIgnoreWarStatus && CPerson::GetInstance().IsWarOfStatus())
			{
				LOG_CF_D(L"召唤喵的时候发现现在是战斗状态……!HP=%d", CPerson::GetInstance().GetPercentHp());
				ClearAround_TarIdSelf(CBaseObject::CreateFixPointInfo(17.0f));
				continue;
			}
			LOG_CF_D(L"开始召唤!");
			CSomeFun::GetInstance().SimlationKey(VK_TAB);
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(6 * 1000, [&Cat]
			{
				return Cat.GetHp() == NULL;
			});
		}

		// 恢复召唤喵!
		sbBeckonCat = FALSE;

		// 回到原点
		return (!EMPTY_PERSONHP && GameRun && bCALLCat) ? BnsMove(FixPoint, 3) : FALSE;
	});
}
