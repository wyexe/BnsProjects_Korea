#include "DailyTask.h"
#include <MyTools/Character.h>
#include "DataBaseModel.h"
#include "SocketCommon.h"

CDailyTask::CDailyTask() : LockVecDailyTask(L"LockVecDailyTask")
{

}

BOOL CDailyTask::QueryDailyTaskSchedule(_In_ CONST ClientAccountContent& ClientAccount, _Out_ ByteBuffer& ByteBuffer_) CONST
{
	std::vector<std::wstring> tmpVecDailyTask;
	LockVecDailyTask.Access([this, &tmpVecDailyTask]{ tmpVecDailyTask = VecDailyTask; });

	std::vector<std::wstring> VecClientDailyTask;
	for (CONST auto& itm : tmpVecDailyTask)
	{
		std::wstring wsSQL;
		CCharacter::FormatText(wsSQL, L"exec QueryDailyTaskSchedule '%s','%s','%s'", ClientAccount.wsAccountName.c_str(), ClientAccount.wsPlayerName.c_str(), itm.c_str());

		WCHAR ResultText[MAX_PATH];
		if (!CDataBaseMdel::ExcuteSQL(wsSQL.c_str(), ResultText))
		{
			ByteBuffer_ << SOCKET_MESSAGE_MESSAGEBOX << L"Êý¾Ý¿â²Ù×÷Ê§°Ü!{exec QueryDailyTaskSchedule}";
			return TRUE;
		}

		if (ResultText[0] == L'0')
			VecClientDailyTask.push_back(itm);
	}
	

	ByteBuffer_ << SOCK_MSG_QUERY_DAILYTASK_SCHEDULE;
	ByteBuffer_ << VecClientDailyTask.size();
	for (CONST auto& itm : VecClientDailyTask)
		ByteBuffer_ << itm;

	return TRUE;
}

BOOL CDailyTask::SetDailyTaskScheduleFinish(_In_ CONST ClientAccountContent& ClientAccount, _In_ CONST std::wstring& wsDailyTaskName, _Out_ ByteBuffer& ByteBuffer_) CONST
{
	std::wstring wsSQL;
	CCharacter::FormatText(wsSQL, L"exec UpdateDailyTaskSchedule '%s','%s','%s'", ClientAccount.wsAccountName.c_str(), ClientAccount.wsPlayerName.c_str(), wsDailyTaskName.c_str());

	ByteBuffer_ << SOCK_MSG_UPDATE_DAILYTASK_SCHEDULE;
	return TRUE;
}

VOID CDailyTask::AddDailyTask(_In_ CONST std::wstring& wsDailyTaskName)
{
	LockVecDailyTask.Access([this, wsDailyTaskName]{VecDailyTask.push_back(wsDailyTaskName); });
}

VOID CDailyTask::ClearDailyTask()
{
	LockVecDailyTask.Access([this]{VecDailyTask.clear(); });
}
