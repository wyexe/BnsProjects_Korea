#ifndef __GITBNSPROJECTS_BNSPROJECTS_OBJECT_OTHEROBJECT_TASK_TASKEXTEND_H__
#define __GITBNSPROJECTS_BNSPROJECTS_OBJECT_OTHEROBJECT_TASK_TASKEXTEND_H__

#include "GameVariable.h"

class CTaskExtend : public MyTools::CClassInstance<CTaskExtend>
{
public:
	CTaskExtend() = default;
	~CTaskExtend() = default;

	// 输出任务信息
	static BOOL Print_MianTask_ArrayInfo(_In_ const DWORD* dwArray);
	static BOOL Print_BLTask_ArrayInfo(_In_ const DWORD* dwArray);
public:
	

	// 主线任务
	BOOL SumitTask_By_Parm(_In_ CONST SumitTask_Src& STC, _In_ CONST std::wstring& wsObjName, _In_ CONST std::wstring wsItemName) CONST;
	BOOL SumitTask_By_Item(_In_ CONST SumitTask_Src& STC, _In_ CONST std::wstring wsItemName) CONST;

	// 对话Npc并且一直按F
	BOOL TalkNpc_And_VirtualButton(_In_ CONST std::wstring& wsObjectName) CONST;

	// 支线任务
	BOOL PickBLTask_By_Parm(_In_ CONST SumitBLTask_Src& STC, _In_ std::function<BOOL(SumitBLTask_Src&)> fn) CONST;
	BOOL PickBLTask_By_Item(_In_ CONST SumitBLTask_Src& STC, _In_ CONST std::wstring& wsItemName) CONST;

	BOOL PickBLTask_By_Npc(_In_ CONST SumitBLTask_Src& STC, _In_ CONST std::wstring& wsNpcName) CONST;
private:
	DSIABLE_COPY_AND_ASSIGN(CTaskExtend)
};



#endif