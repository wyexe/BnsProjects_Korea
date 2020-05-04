#include "stdafx.h"
#include "Task.h"
#include <MyTools/Log.h>
#include <MyTools/CLEchoException.h>
#include "SomeFun.h"
#include "Person.h"
#include "GameSend.h"
#include "PersonFunction.h"
#include "BnsGameFun.h"
#include "ConsoleLog.h"

#define _SELF L"Task.cpp"

BOOL CTask::GetTaskInfo(_Out_ std::vector<TaskInfo>& vlst) CONST
{
#define MAX_TASK_COUNT	0x3A
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		vlst.clear();
		DWORD dwTaskBase = ReadDWORD(CPerson::GetInstance().GetBase() + 大任务偏移);
		for (int i = 0; i < 0x3A; ++i)
		{
			TaskInfo Ti;
			Ti.dwObj_ScheduleId = dwTaskBase + ((i << 4) - i) * 8;
			if (ReadDWORD(Ti.dwObj_ScheduleId) == NULL)
				continue;

			Ti.dwObj_TaskId = ReadDWORD(ReadDWORD(Ti.dwObj_ScheduleId));
			if (Ti.dwObj_TaskId == NULL)
				continue;

			vlst.push_back(Ti);
		}
		return TRUE;
	});
}

BOOL CTask::GetMainTaskInfo(__out TaskInfo& Ti) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&] 
	{
		for (int i = 0; i < 10; ++i)
		{
			std::vector<TaskInfo> vlst;
			GetTaskInfo(vlst);
			for (auto& itm : vlst)
			{
				if (CGameVariable::GetInstance().IsMainTask_By_Id(itm.GetTaskId()))
				{
					Ti = itm;
					return TRUE;
				}
			}
			BnsSleep(1000);
		}
		return FALSE;
	});
}

DWORD CTask::QueryTaskScheduleCount(_In_ CONST TaskInfoParm& Ti) CONST
{
	DWORD dwValue = 0;
	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&Ti, &dwValue]{ 
		dwValue = CGameSend::QueryTaskScheduleCount(Ti.dwTaskId); 
	});

	return dwValue;
}

BOOL CTask::SumitTask(CONST SumitTask_Src& TaskSrc)
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&TaskSrc]{ CGameSend::SumitSpecialMainTask(&TaskSrc); });
	//return //CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&TaskSrc]{CGameSend::SumitMainTask(&TaskSrc); });
}

BOOL CTask::PickLBTask(CONST SumitBLTask_Src& TaskSrc)
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&TaskSrc]{
		CGameSend::PickBLTask(&TaskSrc);
	});
}

BOOL CTask::GetTaskInfo_By_List(_In_ DWORD dwTaskId, _In_ CONST std::vector<TaskInfo>& vlst, _Out_ TaskInfo* pTaskInfo) CONST
{
	return MyTools::CLPublic::Vec_find_if(vlst, pTaskInfo, [&dwTaskId](CONST TaskInfo& Ti){
		return Ti.GetTaskId() == dwTaskId;
	});
}

BOOL CTask::ExistTask(__in DWORD dwTaskId, _Out_ TaskInfo* pTask) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&] 
	{
		std::vector<TaskInfo> vlst;

		GetTaskInfo(vlst);
		return GetTaskInfo_By_List(dwTaskId, vlst, pTask);
	});
}

BOOL CTask::IsCompletionTask(_In_ CONST TaskInfoParm& TaskInfoParm_) CONST
{
	if (TaskInfoParm_.dwTaskId == 0) // Default return TRUE
		return TRUE;

	std::vector<TaskInfo> TaskList;
	CTask::GetTaskInfo(TaskList);

	TaskInfo TaskInfo_;
	CTask::GetTaskInfo_By_List(TaskInfoParm_.dwTaskId, TaskList, &TaskInfo_);
	if (!CTask::GetTaskInfo_By_List(TaskInfoParm_.dwTaskId, TaskList, &TaskInfo_) || TaskInfo_.GetScheduleId() != TaskInfoParm_.dwScheduleId)
		return TRUE;

	return FALSE;
}

BOOL CTask::IsCompletionMainTask(_In_ DWORD dwScheduleId) CONST
{
	BOOL bRetCode = FALSE;
	TaskInfo TaskInfo_;

	if (CTask::GetMainTaskInfo(TaskInfo_))
	{
		bRetCode = TaskInfo_.GetScheduleId() != dwScheduleId;
	}

	return bRetCode;
}

TaskInfoParm CTask::GetMianTaskInfo() CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<TaskInfoParm>(__FUNCTIONW__, [&] 
	{
		TaskInfo Ti;
		for (int i = 0; i < 10 && GameRun; ++i)
		{
			// 过图
			CPersonFunction::GetInstance().WaitGameLoad();
			if (GetMainTaskInfo(Ti))
				return Ti.GetTaskInfoParm();
			LOG_CF_E(L"找不到主线！添加新的主线ID到GameVariable.List! Task:%s", GetALLTaskText().c_str());
			BnsSleep(1000);

#if BnsProject_Dlg
			// 注入窗口下执行弹窗
			LOG_MSG_CF(L"注入窗口下找不到主线!");
			return TaskInfoParm();
#endif
		}

		if (GameRun)
		{
			CBnsGameFun::GetInstance().SetCloseGame(L"连续10次都没找到主线…大退!", em_Close_Result::em_Close_Result_ReStart);
		}
		return TaskInfoParm();
	});
}

std::wstring CTask::GetALLTaskText() CONST
{
	std::vector<TaskInfo> vlst;
	GetTaskInfo(vlst);
	
	std::wstring wsText;
	for (CONST auto& itm : vlst)
	{
		WCHAR wszText[32];
		swprintf_s(wszText, _countof(wszText) - 1, L"[%X-%X] &&", itm.GetTaskId(), itm.GetScheduleId());
		wsText += wszText;
	}
	return wsText;
}
