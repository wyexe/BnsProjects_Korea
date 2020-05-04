#ifndef __GITBNSPROJECTS_BNSPROJECTS_OBJECT_OTHEROBJECT_TASK_TASKEXTEND_H__
#define __GITBNSPROJECTS_BNSPROJECTS_OBJECT_OTHEROBJECT_TASK_TASKEXTEND_H__

#include "GameVariable.h"

class CTaskExtend : public MyTools::CClassInstance<CTaskExtend>
{
public:
	CTaskExtend() = default;
	~CTaskExtend() = default;

	// ���������Ϣ
	static BOOL Print_MianTask_ArrayInfo(_In_ const DWORD* dwArray);
	static BOOL Print_BLTask_ArrayInfo(_In_ const DWORD* dwArray);
public:
	

	// ��������
	BOOL SumitTask_By_Parm(_In_ CONST SumitTask_Src& STC, _In_ CONST std::wstring& wsObjName, _In_ CONST std::wstring wsItemName) CONST;
	BOOL SumitTask_By_Item(_In_ CONST SumitTask_Src& STC, _In_ CONST std::wstring wsItemName) CONST;

	// �Ի�Npc����һֱ��F
	BOOL TalkNpc_And_VirtualButton(_In_ CONST std::wstring& wsObjectName) CONST;

	// ֧������
	BOOL PickBLTask_By_Parm(_In_ CONST SumitBLTask_Src& STC, _In_ std::function<BOOL(SumitBLTask_Src&)> fn) CONST;
	BOOL PickBLTask_By_Item(_In_ CONST SumitBLTask_Src& STC, _In_ CONST std::wstring& wsItemName) CONST;

	BOOL PickBLTask_By_Npc(_In_ CONST SumitBLTask_Src& STC, _In_ CONST std::wstring& wsNpcName) CONST;
private:
	DSIABLE_COPY_AND_ASSIGN(CTaskExtend)
};



#endif