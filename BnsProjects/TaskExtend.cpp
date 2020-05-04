#include "stdafx.h"
#include "TaskExtend.h"
#include <MyTools/Log.h>
#include "NpcExtend.h"
#include "MonsterExtend.h"
#include "ObjectFunction.h"
#include "BagFunction.h"
#include "SomeFun.h"
#include "PersonFunction.h"
#include "Monster.h"
#include "Npc.h"
#include "Task.h"

#define _SELF L"TaskExtend.cpp"

BOOL CTaskExtend::Print_MianTask_ArrayInfo(_In_ const DWORD* dwArray)
{
	CNpc Npc;
	CMonster Monster;
	CPlayer TempObj;

	///////////Print Parm///////////////////////////////////////////////////////////////
	//DWORD dwArray[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	LOG_CF_D( L"主线发包参数:{ %X,%X,%X,%X,%X,%X,%X,%X }", dwArray[0],
		dwArray[1], dwArray[2], dwArray[3], dwArray[4], dwArray[5], dwArray[6], dwArray[7]);

	//////////Analysis////////////////////////////////////////////////////////////////
	DWORD dwTaskId = dwArray[1];
	DWORD dwNpcId = dwArray[5];
	DWORD dwItemId = dwArray[7];

	if (dwTaskId != NULL)
	{
		LOG_CF_D( L"主线Id:%X, 进度Id:%X", dwTaskId, dwArray[0]);
	}
	if (dwNpcId != NULL)
	{
		FixPointInfo FixPtInfo(CBaseObject::GetPersonPoint(), 30.0f);
		if (CNpcExtend::GetInstance().FindNpc_By_Id(dwNpcId, FixPtInfo, &Npc))
		{
			LOG_CF_D( L"交主线Npc:%s", Npc.GetName().c_str());
		}
		else if (CMonsterExtend::GetInstance().FindMonster_By_Id(dwNpcId, FixPtInfo, &Monster))
		{
			LOG_CF_D( L"交主线Monster:%s", Monster.GetName().c_str());
		}
		else if (CObjectFunction::GetInstance().FindALLObject_By_Id(dwNpcId, &TempObj))
		{
			LOG_CF_D( L"交主线NpcId:%X 在全局对象遍历里面找到,分类是:%X", TempObj.GetId(), TempObj.GetIdParm());
		}
	}
	else if (dwItemId != NULL)
	{
		CBag Bag;
		if (CBagFunction::GetInstance().ExistItem_By_Id(dwItemId, &Bag))
			LOG_CF_D( L"物品ID:%X, Name:%s", Bag.GetGId(), Bag.GetName().c_str());
		else
			LOG_CF_D( L"最后一个参数:%X 不知道干嘛用的!", dwItemId);
	}

	return TRUE;
}

BOOL CTaskExtend::Print_BLTask_ArrayInfo(_In_ const DWORD* dwArray)
{
	CNpc Npc;
	CMonster Monster;
	CPlayer TempObj;

	///////////Print Parm///////////////////////////////////////////////////////////////
	LOG_CF_D( L"支线发包参数:{ %X,%X,%X,%X,%X }", dwArray[0],
		dwArray[1], dwArray[2], dwArray[3], dwArray[4]);

	//////////Analysis////////////////////////////////////////////////////////////////

	DWORD dwNpcId = dwArray[0];
	if (dwNpcId != NULL)
	{
		FixPointInfo FixPtInfo(CBaseObject::GetPersonPoint(), 30.0f);
		if (CNpcExtend::GetInstance().FindNpc_By_Id(dwNpcId, FixPtInfo, &Npc))
		{
			LOG_CF_D( L"交支线Npc:%s", Npc.GetName().c_str());
		}
		else if (CMonsterExtend::GetInstance().FindMonster_By_Id(dwNpcId, FixPtInfo, &Monster))
		{
			LOG_CF_D( L"交支线Monster:%s", Monster.GetName().c_str());
		}
		else if (CObjectFunction::GetInstance().FindALLObject_By_Id (dwNpcId, &TempObj))
		{
			LOG_CF_D( L"交支线NpcId:%X 在全局对象遍历里面找到,分类是:%X", TempObj.GetId(), TempObj.GetIdParm());
		}
	}
	if (dwArray[2] != NULL)
	{
		LOG_CF_D( L"支线任务Id:%X", dwArray[2]);
	}
	return TRUE;
}

BOOL CTaskExtend::SumitTask_By_Parm(_In_ CONST SumitTask_Src& STC, _In_ CONST std::wstring& wsObjName, _In_ CONST std::wstring wsItemName) CONST
{
	CPlayer TempObj;
	CBag Bag;

	SumitTask_Src SumitTask_Src_ = STC;
	FixPointInfo FixPtInfo(CBaseObject::GetPersonPoint(), 15.0f);
	/////判断交任务是否对应主线/////////////////////////////////////////////////////////////////////
	TaskInfo TaskInfo_;
	if (!CTask::GetInstance().ExistTask(SumitTask_Src_.dwTaskId, &TaskInfo_))
	{
		LOG_CF_D( L"该任务不存在，ID:%X", SumitTask_Src_.dwTaskId);
		return FALSE;
	}

	if (SumitTask_Src_.dwTaskId != TaskInfo_.GetTaskId())
	{
		LOG_CF_D( L"任务Id不对, Parm:%X, Now:%X", SumitTask_Src_.dwTaskId, TaskInfo_.GetTaskId());
		return FALSE;
	}

	////////写入ItemId//////////////////////////////////////////////////////////////////
	if (wsItemName != L"")
	{
		if (CBagFunction::GetInstance().ExistItem_By_Name(wsItemName, &Bag))
		{
			SumitTask_Src_.dwItemIndex = Bag.GetIndex() + 1;
		}
		else
		{
			LOG_CF_D( L"找不到交任务的物品:%s", wsItemName.c_str());
			return FALSE;
		}
	}

	BOOL bSumitTask = FALSE;
	auto fnSumitTask = [&SumitTask_Src_, &bSumitTask](_In_ CONST GameObjectID& GameObjectID_)
	{
		LOG_CF_D( L"交任务! NpcId=%X, TaskId=[%X,%X]", GameObjectID_.dwId, SumitTask_Src_.dwTaskId, SumitTask_Src_.dwScheduleId);
		SumitTask_Src_.dwObjId = GameObjectID_.dwId;
		SumitTask_Src_.dwObjIdParm = GameObjectID_.dwIdParm;

		CTask::SumitTask(SumitTask_Src_);
		BnsSleep(1000);
		bSumitTask = TRUE;
	};

	if (wsObjName != L"")
	{
		std::vector<CNpc> NpcList;
		CMonster Monster;
		if (CNpcExtend::GetInstance().FindNpc_By_Name(wsObjName, FixPtInfo, NpcList))
		{
			for (CONST auto& itm : NpcList)
			{
				LOG_CF_D( L"跑到Npc旁边交任务:%s", itm.GetName().c_str());
				BnsMove(itm.GetPoint(), 3);
				fnSumitTask(itm.GetObjectId());
			}
		}
		else if (CMonsterExtend::GetInstance().FindMonster_By_Name(wsObjName, FixPtInfo, &Monster))
		{
			LOG_CF_D( L"跑到怪物旁边交任务:%s", Monster.GetName().c_str());
			BnsMove(Monster.GetPoint(), 3);
			fnSumitTask(Monster.GetObjectId());
		}

		/*CMonsterExtend::GetInstance().FindMonster_By_Condiction(FixPtInfo, (CMonster*)nullptr, [&wsObjName, &fnSumitTask](CONST CMonster& Monster){
			if (Monster.GetName() == wsObjName) // 交怪物任务
				fnSumitTask(Monster.GetObjectId());
			return FALSE;
		});*/
	}

	if (!bSumitTask)
	{
		if (CObjectFunction::GetInstance().FindALLObject_By_Id(SumitTask_Src_.dwObjId, &TempObj)) // 试着找一下相同的Id
		{
			SumitTask_Src_.dwObjIdParm = TempObj.GetIdParm();
			fnSumitTask(GameObjectID(TempObj.GetId(), TempObj.GetIdParm()));
		}
		else if (wsItemName == L"" && wsObjName == L"")
		{
			LOG_CF_D( L"交特殊任务, NpcId=%X,默认不处理!", SumitTask_Src_.dwObjId);
			fnSumitTask(GameObjectID(SumitTask_Src_.dwObjId, SumitTask_Src_.dwObjIdParm));
		}
		else
		{
			LOG_CF_D( L"周围找不到交任务的Npc,ID=%X", SumitTask_Src_.dwObjId);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CTaskExtend::SumitTask_By_Item(_In_ CONST SumitTask_Src& STC, _In_ CONST std::wstring wsItemName) CONST
{
	CBag Bag(0x0, 0x0);
	SumitTask_Src SumitTask_Src_ = STC;
	/////判断交任务是否对应主线/////////////////////////////////////////////////////////////////////
	TaskInfo TaskInfo_;
	if (!CTask::GetInstance().ExistTask(SumitTask_Src_.dwTaskId, &TaskInfo_))
	{
		LOG_CF_D( L"该任务不存在，ID:%X", SumitTask_Src_.dwTaskId);
		return FALSE;
	}

	if (SumitTask_Src_.dwTaskId != TaskInfo_.GetTaskId())
	{
		LOG_CF_D( L"任务Id不对, Parm:%X, Now:%X", SumitTask_Src_.dwTaskId, TaskInfo_.GetTaskId());
		return FALSE;
	}

	////////写入ItemId//////////////////////////////////////////////////////////////////
	if (wsItemName != L"")
	{
		if ( CBagFunction::GetInstance().ExistItem_By_Name(wsItemName, &Bag))
		{
			SumitTask_Src_.dwObjId = Bag.GetIndex() + 1;
			SumitTask_Src_.dwItemIndex = Bag.GetIndex() + 1; // 新加的， 以前交物品类任务=0
			SumitTask_Src_.dwObjId <<= 16;
			SumitTask_Src_.dwObjId += 0x2;
			SumitTask_Src_.dwItemIndex <<= 16;
			SumitTask_Src_.dwItemIndex += 0x2; 
		}
		else
		{
			LOG_CF_D( L"找不到交任务的物品:%s", wsItemName.c_str());
			return FALSE;
		}
	}
	else
	{
		LOG_CF_D( L"交特殊任务, dwItemindex=%X,默认不处理!", STC.dwObjId);
	}

	CTask::SumitTask(SumitTask_Src_);
	return BnsSleep(1000);
}

BOOL CTaskExtend::PickBLTask_By_Parm(_In_ CONST SumitBLTask_Src& STC, _In_ std::function<BOOL(SumitBLTask_Src&)> fn) CONST
{
	SumitBLTask_Src SumitBLTask_Src_ = STC;
	// 判断交任务是否对应主线
	TaskInfo TaskInfo_;
	if (CTask::GetInstance().ExistTask(SumitBLTask_Src_.dwTaskId, &TaskInfo_))
	{
		LOG_CF_D( L"此任务已经存在:%X", SumitBLTask_Src_.dwTaskId);
		return TRUE;
	}

	if (fn(SumitBLTask_Src_))
		CTask::PickLBTask(SumitBLTask_Src_);

	return BnsSleep(2 * 1000);
}

BOOL CTaskExtend::PickBLTask_By_Npc(_In_ CONST SumitBLTask_Src& STC, _In_ CONST std::wstring& wsNpcName) CONST
{
	return PickBLTask_By_Parm(STC, [&wsNpcName](SumitBLTask_Src& SumitBLTask_Src_)
	{
		// 写入NpcId和NpcIdParm
		CNpc Npc;
		CMonster Monster;
		CPlayer TempObj;

		FixPointInfo FixPtInfo(CBaseObject::GetPersonPoint(), 15.0f);
		if (CNpcExtend::GetInstance().FindNpc_By_Name(wsNpcName, FixPtInfo, &Npc))
		{
			SumitBLTask_Src_.dwNpcId = Npc.GetId();
			SumitBLTask_Src_.dwNpcIdParm = Npc.GetIdParm();
			BnsMove(Npc.GetPoint(), 2);
		}
		else if (CMonsterExtend::GetInstance().FindMonster_By_Name(wsNpcName, FixPtInfo, &Monster))
		{
			SumitBLTask_Src_.dwNpcId = Monster.GetId();
			SumitBLTask_Src_.dwNpcIdParm = Monster.GetIdParm();
			BnsMove(Monster.GetPoint(), 2);
		}
		else if (CObjectFunction::GetInstance().FindALLObject_By_Id(SumitBLTask_Src_.dwNpcId, &TempObj))
		{
			SumitBLTask_Src_.dwNpcIdParm = TempObj.GetIdParm();
		}
		else
		{
			LOG_CF_D( L"周围找不到交任务的Npc");
			return FALSE;
		}
		return TRUE;
	});
}

BOOL CTaskExtend::PickBLTask_By_Item(_In_ CONST SumitBLTask_Src& STC, _In_ CONST std::wstring& wsItemName) CONST
{
	return PickBLTask_By_Parm(STC, [&wsItemName](SumitBLTask_Src& SumitBLTask_Src_)
	{
		//STC.dwParm4
		// 写入物品位置
		CBag Item;
		if (!CBagFunction::GetInstance().ExistItem_By_Name(wsItemName, &Item))
			return FALSE;

		SumitBLTask_Src_.dwParm4 = Item.GetIndex();
		return TRUE;
	});
}

BOOL CTaskExtend::TalkNpc_And_VirtualButton(_In_ CONST std::wstring& wsObjectName) CONST
{
	std::vector<CNpc> NpcList;
	CMonster Monster;
	if (CNpcExtend::GetInstance().FindNpc_By_Name(wsObjectName, CBaseObject::CreateFixPointInfo(15), NpcList))
	{
		for (CONST auto& itm : NpcList)
		{
			LOG_CF_D( L"跑到Npc旁边交任务:%s", itm.GetName().c_str());
			BnsMove(itm.GetPoint(), 3);
			BnsSleep(1000);
			CPersonFunction::GetInstance().Pickup(itm.GetObjectId());
			BnsSleep(2000);
			if (CObjectFunction::GetInstance().IsShow_In_GameUi(L"TalkControlPanel"))
				break;
		}
	}
	else if (CMonsterExtend::GetInstance().FindMonster_By_Name(wsObjectName, CBaseObject::CreateFixPointInfo(15), &Monster))
	{
		LOG_CF_D( L"跑到怪物旁边交任务:%s", Monster.GetName().c_str());
		BnsMove(Monster.GetPoint(), 3);
		BnsSleep(1000);
		CPersonFunction::GetInstance().Pickup(Monster.GetObjectId());
		BnsSleep(2000);
	}

	if (CObjectFunction::GetInstance().IsShow_In_GameUi(L"TalkControlPanel"))
	{
		// 10s内不断按F, 直到Npc聊天窗口消失!
		if (CSomeFun::GetInstance().TimeOut_Condiction_GameRun(10 * 1000, []
		{
			CSomeFun::GetInstance().SimlationKey('F', 200, 1000);
			return CObjectFunction::GetInstance().IsShow_In_GameUi(L"TalkControlPanel");
		}))
		{
			LOG_CF_D( L"Npc对话框10s超时!");
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(10 * 1000, []
			{
				CSomeFun::GetInstance().SimlationKey(VK_ESCAPE, 200, 1000);
				return CObjectFunction::GetInstance().IsShow_In_GameUi(L"TalkControlPanel");
			});
		}
		LOG_CF_D( L"对话框对话完毕!");
	}
	return TRUE;
}
