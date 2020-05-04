#include "DailyTask.h"
#include <MyTools/Character.h>
#include <MyTools/CLEchoException.h>
#include "DbManager.h"

VOID CDailyTask::ReFillVecDailyTaskName(_In_ CONST std::vector<std::wstring>& Vec)
{
	_Lock.Access([&] 
	{
		_VecDailyTasName.clear();
		for (CONST auto& itm : Vec)
		{
			if (itm.find(L".inf") == -1)
				_VecDailyTasName.push_back(itm);
			else
			{
				std::wstring wsTaskName;
				MyTools::CCharacter::GetRemoveLeft(itm, L".inf", wsTaskName);
				_VecDailyTasName.push_back(wsTaskName);
			}
		}
	});
}

BOOL CDailyTask::QueryDailyTaskSchedule(_In_ CONST ClientAccountContent& AccountContent, _Out_ std::vector<std::wstring>& VecDailyTaskName) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		std::vector<std::wstring> Vec;
		_Lock.Access([&Vec, this] { Vec = _VecDailyTasName; });

		for (CONST auto& itm : Vec)
		{
			std::wstring wsResult;
			if (!CBnsDbManager::GetInstance().ExcuteSQL_SingleResult(MyTools::CCharacter::MakeFormatText(L"exec QueryDailyTaskSchedule '%s','%s','%s'", AccountContent.wsAccountName.c_str(), AccountContent.wsPlayerName.c_str(), itm.c_str()), wsResult))
				return FALSE;

			if (wsResult[0] == L'0')
				VecDailyTaskName.push_back(itm);
		}

		return TRUE;
	});
}

VOID CDailyTask::SetDailyTaskScheduleFinish(_In_ CONST ClientAccountContent& AccountContent, _In_ CONST std::wstring& wsDailyTaskName) CONST
{
	MyTools::CLEchoException::GetInstance().InvokeAction(__FUNCTIONW__, [=]
	{
		CBnsDbManager::GetInstance().AsyncExcuteSQL(MyTools::CCharacter::MakeFormatText(L"exec UpdateDailyTaskSchedule '%s','%s','%s'", AccountContent.wsAccountName.c_str(), AccountContent.wsPlayerName.c_str(), wsDailyTaskName.c_str()));
	});
}
