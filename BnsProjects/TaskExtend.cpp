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
	LOG_CF_D( L"���߷�������:{ %X,%X,%X,%X,%X,%X,%X,%X }", dwArray[0],
		dwArray[1], dwArray[2], dwArray[3], dwArray[4], dwArray[5], dwArray[6], dwArray[7]);

	//////////Analysis////////////////////////////////////////////////////////////////
	DWORD dwTaskId = dwArray[1];
	DWORD dwNpcId = dwArray[5];
	DWORD dwItemId = dwArray[7];

	if (dwTaskId != NULL)
	{
		LOG_CF_D( L"����Id:%X, ����Id:%X", dwTaskId, dwArray[0]);
	}
	if (dwNpcId != NULL)
	{
		FixPointInfo FixPtInfo(CBaseObject::GetPersonPoint(), 30.0f);
		if (CNpcExtend::GetInstance().FindNpc_By_Id(dwNpcId, FixPtInfo, &Npc))
		{
			LOG_CF_D( L"������Npc:%s", Npc.GetName().c_str());
		}
		else if (CMonsterExtend::GetInstance().FindMonster_By_Id(dwNpcId, FixPtInfo, &Monster))
		{
			LOG_CF_D( L"������Monster:%s", Monster.GetName().c_str());
		}
		else if (CObjectFunction::GetInstance().FindALLObject_By_Id(dwNpcId, &TempObj))
		{
			LOG_CF_D( L"������NpcId:%X ��ȫ�ֶ�����������ҵ�,������:%X", TempObj.GetId(), TempObj.GetIdParm());
		}
	}
	else if (dwItemId != NULL)
	{
		CBag Bag;
		if (CBagFunction::GetInstance().ExistItem_By_Id(dwItemId, &Bag))
			LOG_CF_D( L"��ƷID:%X, Name:%s", Bag.GetGId(), Bag.GetName().c_str());
		else
			LOG_CF_D( L"���һ������:%X ��֪�������õ�!", dwItemId);
	}

	return TRUE;
}

BOOL CTaskExtend::Print_BLTask_ArrayInfo(_In_ const DWORD* dwArray)
{
	CNpc Npc;
	CMonster Monster;
	CPlayer TempObj;

	///////////Print Parm///////////////////////////////////////////////////////////////
	LOG_CF_D( L"֧�߷�������:{ %X,%X,%X,%X,%X }", dwArray[0],
		dwArray[1], dwArray[2], dwArray[3], dwArray[4]);

	//////////Analysis////////////////////////////////////////////////////////////////

	DWORD dwNpcId = dwArray[0];
	if (dwNpcId != NULL)
	{
		FixPointInfo FixPtInfo(CBaseObject::GetPersonPoint(), 30.0f);
		if (CNpcExtend::GetInstance().FindNpc_By_Id(dwNpcId, FixPtInfo, &Npc))
		{
			LOG_CF_D( L"��֧��Npc:%s", Npc.GetName().c_str());
		}
		else if (CMonsterExtend::GetInstance().FindMonster_By_Id(dwNpcId, FixPtInfo, &Monster))
		{
			LOG_CF_D( L"��֧��Monster:%s", Monster.GetName().c_str());
		}
		else if (CObjectFunction::GetInstance().FindALLObject_By_Id (dwNpcId, &TempObj))
		{
			LOG_CF_D( L"��֧��NpcId:%X ��ȫ�ֶ�����������ҵ�,������:%X", TempObj.GetId(), TempObj.GetIdParm());
		}
	}
	if (dwArray[2] != NULL)
	{
		LOG_CF_D( L"֧������Id:%X", dwArray[2]);
	}
	return TRUE;
}

BOOL CTaskExtend::SumitTask_By_Parm(_In_ CONST SumitTask_Src& STC, _In_ CONST std::wstring& wsObjName, _In_ CONST std::wstring wsItemName) CONST
{
	CPlayer TempObj;
	CBag Bag;

	SumitTask_Src SumitTask_Src_ = STC;
	FixPointInfo FixPtInfo(CBaseObject::GetPersonPoint(), 15.0f);
	/////�жϽ������Ƿ��Ӧ����/////////////////////////////////////////////////////////////////////
	TaskInfo TaskInfo_;
	if (!CTask::GetInstance().ExistTask(SumitTask_Src_.dwTaskId, &TaskInfo_))
	{
		LOG_CF_D( L"�����񲻴��ڣ�ID:%X", SumitTask_Src_.dwTaskId);
		return FALSE;
	}

	if (SumitTask_Src_.dwTaskId != TaskInfo_.GetTaskId())
	{
		LOG_CF_D( L"����Id����, Parm:%X, Now:%X", SumitTask_Src_.dwTaskId, TaskInfo_.GetTaskId());
		return FALSE;
	}

	////////д��ItemId//////////////////////////////////////////////////////////////////
	if (wsItemName != L"")
	{
		if (CBagFunction::GetInstance().ExistItem_By_Name(wsItemName, &Bag))
		{
			SumitTask_Src_.dwItemIndex = Bag.GetIndex() + 1;
		}
		else
		{
			LOG_CF_D( L"�Ҳ������������Ʒ:%s", wsItemName.c_str());
			return FALSE;
		}
	}

	BOOL bSumitTask = FALSE;
	auto fnSumitTask = [&SumitTask_Src_, &bSumitTask](_In_ CONST GameObjectID& GameObjectID_)
	{
		LOG_CF_D( L"������! NpcId=%X, TaskId=[%X,%X]", GameObjectID_.dwId, SumitTask_Src_.dwTaskId, SumitTask_Src_.dwScheduleId);
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
				LOG_CF_D( L"�ܵ�Npc�Ա߽�����:%s", itm.GetName().c_str());
				BnsMove(itm.GetPoint(), 3);
				fnSumitTask(itm.GetObjectId());
			}
		}
		else if (CMonsterExtend::GetInstance().FindMonster_By_Name(wsObjName, FixPtInfo, &Monster))
		{
			LOG_CF_D( L"�ܵ������Ա߽�����:%s", Monster.GetName().c_str());
			BnsMove(Monster.GetPoint(), 3);
			fnSumitTask(Monster.GetObjectId());
		}

		/*CMonsterExtend::GetInstance().FindMonster_By_Condiction(FixPtInfo, (CMonster*)nullptr, [&wsObjName, &fnSumitTask](CONST CMonster& Monster){
			if (Monster.GetName() == wsObjName) // ����������
				fnSumitTask(Monster.GetObjectId());
			return FALSE;
		});*/
	}

	if (!bSumitTask)
	{
		if (CObjectFunction::GetInstance().FindALLObject_By_Id(SumitTask_Src_.dwObjId, &TempObj)) // ������һ����ͬ��Id
		{
			SumitTask_Src_.dwObjIdParm = TempObj.GetIdParm();
			fnSumitTask(GameObjectID(TempObj.GetId(), TempObj.GetIdParm()));
		}
		else if (wsItemName == L"" && wsObjName == L"")
		{
			LOG_CF_D( L"����������, NpcId=%X,Ĭ�ϲ�����!", SumitTask_Src_.dwObjId);
			fnSumitTask(GameObjectID(SumitTask_Src_.dwObjId, SumitTask_Src_.dwObjIdParm));
		}
		else
		{
			LOG_CF_D( L"��Χ�Ҳ����������Npc,ID=%X", SumitTask_Src_.dwObjId);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CTaskExtend::SumitTask_By_Item(_In_ CONST SumitTask_Src& STC, _In_ CONST std::wstring wsItemName) CONST
{
	CBag Bag(0x0, 0x0);
	SumitTask_Src SumitTask_Src_ = STC;
	/////�жϽ������Ƿ��Ӧ����/////////////////////////////////////////////////////////////////////
	TaskInfo TaskInfo_;
	if (!CTask::GetInstance().ExistTask(SumitTask_Src_.dwTaskId, &TaskInfo_))
	{
		LOG_CF_D( L"�����񲻴��ڣ�ID:%X", SumitTask_Src_.dwTaskId);
		return FALSE;
	}

	if (SumitTask_Src_.dwTaskId != TaskInfo_.GetTaskId())
	{
		LOG_CF_D( L"����Id����, Parm:%X, Now:%X", SumitTask_Src_.dwTaskId, TaskInfo_.GetTaskId());
		return FALSE;
	}

	////////д��ItemId//////////////////////////////////////////////////////////////////
	if (wsItemName != L"")
	{
		if ( CBagFunction::GetInstance().ExistItem_By_Name(wsItemName, &Bag))
		{
			SumitTask_Src_.dwObjId = Bag.GetIndex() + 1;
			SumitTask_Src_.dwItemIndex = Bag.GetIndex() + 1; // �¼ӵģ� ��ǰ����Ʒ������=0
			SumitTask_Src_.dwObjId <<= 16;
			SumitTask_Src_.dwObjId += 0x2;
			SumitTask_Src_.dwItemIndex <<= 16;
			SumitTask_Src_.dwItemIndex += 0x2; 
		}
		else
		{
			LOG_CF_D( L"�Ҳ������������Ʒ:%s", wsItemName.c_str());
			return FALSE;
		}
	}
	else
	{
		LOG_CF_D( L"����������, dwItemindex=%X,Ĭ�ϲ�����!", STC.dwObjId);
	}

	CTask::SumitTask(SumitTask_Src_);
	return BnsSleep(1000);
}

BOOL CTaskExtend::PickBLTask_By_Parm(_In_ CONST SumitBLTask_Src& STC, _In_ std::function<BOOL(SumitBLTask_Src&)> fn) CONST
{
	SumitBLTask_Src SumitBLTask_Src_ = STC;
	// �жϽ������Ƿ��Ӧ����
	TaskInfo TaskInfo_;
	if (CTask::GetInstance().ExistTask(SumitBLTask_Src_.dwTaskId, &TaskInfo_))
	{
		LOG_CF_D( L"�������Ѿ�����:%X", SumitBLTask_Src_.dwTaskId);
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
		// д��NpcId��NpcIdParm
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
			LOG_CF_D( L"��Χ�Ҳ����������Npc");
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
		// д����Ʒλ��
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
			LOG_CF_D( L"�ܵ�Npc�Ա߽�����:%s", itm.GetName().c_str());
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
		LOG_CF_D( L"�ܵ������Ա߽�����:%s", Monster.GetName().c_str());
		BnsMove(Monster.GetPoint(), 3);
		BnsSleep(1000);
		CPersonFunction::GetInstance().Pickup(Monster.GetObjectId());
		BnsSleep(2000);
	}

	if (CObjectFunction::GetInstance().IsShow_In_GameUi(L"TalkControlPanel"))
	{
		// 10s�ڲ��ϰ�F, ֱ��Npc���촰����ʧ!
		if (CSomeFun::GetInstance().TimeOut_Condiction_GameRun(10 * 1000, []
		{
			CSomeFun::GetInstance().SimlationKey('F', 200, 1000);
			return CObjectFunction::GetInstance().IsShow_In_GameUi(L"TalkControlPanel");
		}))
		{
			LOG_CF_D( L"Npc�Ի���10s��ʱ!");
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(10 * 1000, []
			{
				CSomeFun::GetInstance().SimlationKey(VK_ESCAPE, 200, 1000);
				return CObjectFunction::GetInstance().IsShow_In_GameUi(L"TalkControlPanel");
			});
		}
		LOG_CF_D( L"�Ի���Ի����!");
	}
	return TRUE;
}
