#ifndef __BNS_TASK_H__
#define __BNS_TASK_H__

#include "GameVariable.h"
#include "BaseObject.h"

typedef struct _TaskInfo
{
	DWORD dwObj_TaskId;				// ����Id
	DWORD dwObj_ScheduleId;			// ����Id

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
	
	// ������������
	BOOL GetTaskInfo(_Out_ std::vector<TaskInfo>& vlst) CONST;

	// ��������
	TaskInfoParm GetMianTaskInfo() CONST;

	// ��ȡ�������ɶ�
	DWORD QueryTaskScheduleCount(_In_ CONST TaskInfoParm& Ti) CONST;

	// ����static, ֻ�ܵ���һ��
	BOOL ExistTask(_In_ DWORD dwTaskId, _Out_ TaskInfo* pTask) CONST;

	// �ж������Ƿ������
	BOOL IsCompletionTask(_In_  CONST TaskInfoParm& TaskInfoParm_) CONST;
	BOOL IsCompletionMainTask(_In_ DWORD dwScheduleId) CONST;
public:
	// ����������
	static BOOL SumitTask(_In_ CONST SumitTask_Src& TaskSrc);

	// ��֧������
	static BOOL PickLBTask(_In_ CONST SumitBLTask_Src& TaskSrc);

private:
	BOOL GetMainTaskInfo(_Out_ TaskInfo& Ti) CONST;

	// ��ȡ������Ϣ
	BOOL GetTaskInfo_By_List(_In_ DWORD dwTaskId, _In_ CONST std::vector<TaskInfo>& vlst, _Out_ TaskInfo* pTaskInfo) CONST;

	// ��ȡ��ǰ���е�֧��Text
	std::wstring GetALLTaskText() CONST;
};



#endif