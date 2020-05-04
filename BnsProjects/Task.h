#ifndef __BNS_TASK_H__
#define __BNS_TASK_H__

#include "GameVariable.h"
#include "BaseObject.h"

typedef struct _TaskInfo
{
	DWORD dwObj_TaskId;				// 任务Id
	DWORD dwObj_ScheduleId;			// 进度Id

	DWORD GetTaskId() CONST
	{
		return ReadDWORD(dwObj_TaskId + 0x8);
	}
	DWORD GetScheduleId() CONST
	{
		return ReadDWORD(dwObj_ScheduleId + 0xA) & 0xFF;
	}
	_TaskInfo()
	{
		dwObj_ScheduleId = NULL;
		dwObj_TaskId = NULL;
	}
	CONST std::wstring GetName() CONST
	{
		std::wstring Name;
		CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&Name, this]
		{
			WCHAR wszText[64] = { 0 };
			DWORD dwValue = ReadDWORD(CBaseObject::GetObjectNameAddr(ReadDWORD(dwObj_TaskId + 0x18), ReadDWORD(dwObj_TaskId + 0x1C)) + 0x18);
			if (dwValue != NULL && ReadDWORD(dwValue) != NULL)
				MyTools::CCharacter::wstrcpy_my(wszText, (LPCWSTR)dwValue, _countof(wszText) - 1);
			Name = wszText;
		});
	
		return Name;
	}
	TaskInfoParm GetTaskInfoParm() CONST
	{
		return TaskInfoParm(GetTaskId(), GetScheduleId());
	}
}TaskInfo;


class CTask : public MyTools::CClassInstance<CTask>
{
public:
	CTask() = default;
	~CTask() = default;
	
	// 遍历所有任务
	BOOL GetTaskInfo(_Out_ std::vector<TaskInfo>& vlst) CONST;

	// 主线任务
	TaskInfoParm GetMianTaskInfo() CONST;

	// 获取任务的完成度
	DWORD QueryTaskScheduleCount(_In_ CONST TaskInfoParm& Ti) CONST;

	// 返回static, 只能调用一次
	BOOL ExistTask(_In_ DWORD dwTaskId, _Out_ TaskInfo* pTask) CONST;

	// 判断任务是否完成了
	BOOL IsCompletionTask(_In_  CONST TaskInfoParm& TaskInfoParm_) CONST;
	BOOL IsCompletionMainTask(_In_ DWORD dwScheduleId) CONST;
public:
	// 交主线任务
	static BOOL SumitTask(_In_ CONST SumitTask_Src& TaskSrc);

	// 交支线任务
	static BOOL PickLBTask(_In_ CONST SumitBLTask_Src& TaskSrc);

private:
	BOOL GetMainTaskInfo(_Out_ TaskInfo& Ti) CONST;

	// 获取任务信息
	BOOL GetTaskInfo_By_List(_In_ DWORD dwTaskId, _In_ CONST std::vector<TaskInfo>& vlst, _Out_ TaskInfo* pTaskInfo) CONST;

	// 获取当前所有的支线Text
	std::wstring GetALLTaskText() CONST;
};



#endif