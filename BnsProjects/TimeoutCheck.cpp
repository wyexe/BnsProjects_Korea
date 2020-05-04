#include "stdafx.h"
#include "TimeoutCheck.h"
#include <MyTools/Log.h>
#include <MyTools/CLEchoException.h>
#include "ConsoleLog.h"
#include "Person.h"
#include "BnsConfig.h"
#include "Task.h"
#include "PersonFunction.h"
#include "SomeFun.h"

#define _SELF L"TimeoutCheck.cpp"

CTimeoutCheck::CTimeoutCheck()
{
	_ulMoveTick = ::GetTickCount64();
	_ulTaskTick = ::GetTickCount64();
	_LastTask = CTask::GetInstance().GetMianTaskInfo();
}

BOOL CTimeoutCheck::Check()
{
	PrintPlayerMsgToConsole();
	return CheckUnMove() && CheckTaskTimeout();
}

VOID CTimeoutCheck::PrintPlayerMsgToConsole()
{
	MyTools::CLEchoException::GetInstance().InvokeAction(__FUNCTIONW__, [&] 
	{
		if (_uTimeTick++ % 60 == 0)
		{
			LOG_CF_D(L"%s=%d,%s=%d,%s=%d,%s=%d", \
				VARIABLE_NMAE_MIANTASK_TIMEOUT, CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NMAE_MIANTASK_TIMEOUT), \
				VARIABLE_NAME_CHECKPAUSEPOINT, CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_CHECKPAUSEPOINT), \
				VARIABLE_NAME_TASKTIMEOUTID, CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_TASKTIMEOUTID), \
				VARIABLE_NAME_ESCAPE_TIMEOUT, CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_ESCAPE_TIMEOUT));

			CGameVariable::GetInstance().Action_Variable_By_Name(VARIABLE_NAME_WAREHOUSE_MODE, TRUE, []
			{
				CConsoleLog(MyTools::CCharacter::MakeFormatText(L"%d��,%d��", CPerson::GetInstance().GetLevel(), CPerson::GetInstance().GetMoney() / 100 / 100));
			});

			_uTimeTick = 1;
		}
	});
}

BOOL CTimeoutCheck::CheckUnMove()
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		DWORD dwCheckPausePointTimeOut = CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_CHECKPAUSEPOINT);
		if (dwCheckPausePointTimeOut == NULL)
		{
			_ulMoveTick = ::GetTickCount64();
			return TRUE;
		}

		if (CBaseObject::GetPersonDis(_LastPersonPoint) > 2.0f)
		{
			_ulMoveTick = ::GetTickCount64();
			_LastPersonPoint = CBaseObject::GetPersonPoint();
		}
		else if (static_cast<DWORD>(::GetTickCount64() - _ulMoveTick) >= dwCheckPausePointTimeOut * 1000)
		{
			CBnsConfig::GetInstance().WriteAccountLog(MyTools::CCharacter::MakeFormatText(L"����%d���Ӳ���, ǿ��С��!", dwCheckPausePointTimeOut / 60));
			CGameVariable::GetInstance().SetForceBackToSwitchPlayer();
			return FALSE;
		}

		return TRUE;
	});
}

BOOL CTimeoutCheck::CheckTaskTimeout()
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		DWORD dwTaskId = CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_TASKTIMEOUTID);
		if (dwTaskId == 0) // �����κ�����ʱ���
		{
			_ulTaskTick = ::GetTickCount64();
			return TRUE;
		}

		TaskInfoParm NowTaskInfo;
		if (dwTaskId == 1) // = 1 ��ʾ������
		{
			NowTaskInfo = CTask::GetInstance().GetMianTaskInfo();
			if (NowTaskInfo.dwTaskId == 0)
			{
				LOG_CF_E(L"GetMianTaskInfo.TaskID=0");
				return TRUE;
			}
		}
		else
		{
			TaskInfo Ti;
			if (!CTask::GetInstance().ExistTask(dwTaskId, &Ti)) // ����������ID
			{
				_ulTaskTick = ::GetTickCount64();
				return TRUE;
			}

			NowTaskInfo = Ti.GetTaskInfoParm();
		}

		DWORD dwTimeOut = CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NMAE_MIANTASK_TIMEOUT);
		DWORD dwEscapeTimeOut = CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_ESCAPE_TIMEOUT);
		if (_LastTask != NowTaskInfo)
		{
			LOG_CF_D(L"��ǰ������ID=[%X,%X],���ڱ���![%X,%X]", _LastTask.dwTaskId, _LastTask.dwScheduleId, NowTaskInfo.dwTaskId, NowTaskInfo.dwScheduleId);
#if BnsProject_DLL
			CSomeFun::GetInstance().SendKeepALive(&NowTaskInfo, L"");
#endif
			_LastTask = NowTaskInfo;
			_ulTaskTick = ::GetTickCount64();

			CConsoleLog(em_ConsoleLog::em_ConsoleLog_Status, MyTools::CCharacter::MakeFormatText(L"%d��,%d��,%d�Ǳ�,����[%X,%X]",
				CPerson::GetInstance().GetLevel(),
				CPerson::GetInstance().GetMoney() / 100 / 100,
				CPerson::GetInstance().GetStarCoin(),
				_LastTask.dwTaskId,
				_LastTask.dwScheduleId));

			// ��������, ȥ���ϴ��ص�ԭ��
			CConsoleLog(em_ConsoleLog::em_ConsoleLog_Result, L"�����һ�!");
		}
		else if (static_cast<DWORD>(::GetTickCount64() - _ulTaskTick) >= dwTimeOut * 1000)
		{
			CBnsConfig::GetInstance().WriteAccountLog(MyTools::CCharacter::MakeFormatText(L"����%d������[%X,%X]��û��,ǿ�ƶϿ�����!", dwTimeOut, NowTaskInfo.dwTaskId, NowTaskInfo.dwScheduleId));
			CGameVariable::GetInstance().SetForceBackToSwitchPlayer();
			return FALSE;
		}
		else if (dwEscapeTimeOut != NULL && static_cast<DWORD>(::GetTickCount64() - _ulTaskTick) >= dwEscapeTimeOut * 1000)
		{
			LOG_CF_E(L"����%d������[%X,%X]��û��,ʹ������!", dwEscapeTimeOut, NowTaskInfo.dwTaskId, NowTaskInfo.dwScheduleId);
			CPersonFunction::GetInstance().DoEscape();
			BnsSleep(5 * 1000);
		}

		return TRUE;
	});
}