#ifndef __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_TASK_DAILYTASK_H__
#define __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_TASK_DAILYTASK_H__

#include <MyTools/ClassInstance.h>

struct ClientAccountContent;
class CDailyTask : public MyTools::CClassInstance<CDailyTask>
{
public:
	CDailyTask() : _Lock(L"CDailyTask.Lock") {}
	~CDailyTask() = default;

	// 重新读取'日常'目录下的日常任务名
	VOID ReFillVecDailyTaskName(_In_ CONST std::vector<std::wstring>& Vec);

	// 返回这个帐号还没完成的日常任务名
	BOOL QueryDailyTaskSchedule(_In_ CONST ClientAccountContent& AccountContent, _Out_ std::vector<std::wstring>& VecDailyTaskName) CONST;

	// 设置帐号的日常任务=完成
	VOID SetDailyTaskScheduleFinish(_In_ CONST ClientAccountContent& AccountContent, _In_ CONST std::wstring& wsDailyTaskName) CONST;
private:
	std::vector<std::wstring> _VecDailyTasName;
	MyTools::CLLock _Lock;
};



#endif // !__GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_TASK_DAILYTASK_H__
