#include "stdafx.h"
#include "DailyExtend.h"
#include <time.h>
#include "BnsConfig.h"
#include "GameVariable.h"
#include <MyTools/Log.h>
#include "Person.h"
#if BnsProject_DLL
#include "../ConsoleDLL/SocketAPI.h"
#endif

#define _SELF L"DailyExtend.cpp"

VOID CDailyExtend::SetDailyTaskFinish(_In_ CONST std::wstring& wsDailyTaskName) CONST
{
	if (CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_DISABLE_UPDATEDAILYTASK) == 1)
		return;

#if BnsProject_DLL
	CSocketAPI().SetDailyTaskSchedule(wsDailyTaskName);
#endif
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CURRENT_DAILYTASK, 0xFFFFFFFF);
}
