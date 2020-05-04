#include "stdafx.h"
#include "DailyScript.h"
#include <MyTools/Log.h>
#include "PersonFunction.h"
#include "Task.h"
#include "Person.h"
#include "DailyExtend.h"
#include "SomeFun.h"
#include "BnsGameFun.h"
#if BnsProject_DLL
#include "TransferScript.h"
#endif

#define _SELF L"DailyScript.cpp"

BOOL CDailyScript::Run(_In_ CONST std::wstring& wsDailyTaskName)
{
	if (!ReadDailyScript(wsDailyTaskName))
		return FALSE;

	if (!InitDailyScipt())
		return FALSE;

	return Start(wsDailyTaskName);
}

BOOL CDailyScript::ReadDailyScript(_In_ CONST std::wstring& wsDailyTaskName)
{
#if BnsProject_Dlg
	CONST std::wstring wsPath = L"C:\\Bns\\" + wsDailyTaskName;
	if (!MyTools::CLPublic::FileExit(wsPath.c_str()))
	{
		LOG_MSG_CF( L"读取脚本文件:%s 失败!", wsPath.c_str());
		return FALSE;
	}
	return Read(em_ReadScript_Type_FilePath, wsPath.c_str());
#else
	CTransferScript TransferScript;
	std::wstring wsScriptContent = TransferScript.DownLoadScript(wsDailyTaskName);
	return Read(CBnsScript::em_ReadScript_Type_Text, wsScriptContent);
#endif
}

BOOL CDailyScript::InitDailyScipt()
{
	if (ExistDefMethod(L"Head") == nullptr)
	{
		LOG_MSG_CF( L"不存在Head函数!");
		return FALSE;
	}

	return TRUE;
}

BOOL CDailyScript::Start(_In_ CONST std::wstring& wsDailyTaskName)
{
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CURRENT_DAILYTASK, 0);
	while (GameRun)
	{
		DWORD dwTaskId = CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_CURRENT_DAILYTASK);
		if (dwTaskId == 0xFFFFFFFF)
			break;

		std::wstring wsScriptName;
		if (dwTaskId != NULL)
		{
			TaskInfo TaskInfo_;
			if (!CTask::GetInstance().ExistTask(dwTaskId, &TaskInfo_))
			{
				LOG_CF_D( L"执行Body的时候无法找到任务:%X! 当任务完成了!", dwTaskId);
				CDailyExtend::GetInstance().SetDailyTaskFinish(wsDailyTaskName);
				break;
			}

			wsScriptName = MyTools::CCharacter::MakeFormatText(L"Task_%X", TaskInfo_.GetScheduleId());
		}
		else
			wsScriptName = L"Head";


		Excute(wsScriptName);
		if (CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_CURRENT_DAILYTASK) == 0)
		{
			LOG_CF_E( L"执行Head的时候无法找到任务:%X! 当任务完成了!", dwTaskId);
			CDailyExtend::GetInstance().SetDailyTaskFinish(wsDailyTaskName);
			break;
		}

		if (EMPTY_PERSONHP)
			CPersonFunction::GetInstance().Resurrect();

		BnsSleep(1000);
	}
	return TRUE;
}
