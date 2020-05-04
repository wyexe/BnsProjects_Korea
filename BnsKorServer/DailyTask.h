#ifndef __GITBNSPROJECTS_BNSKORSERVER_DAILYTASK_DAILYTASK_H__
#define __GITBNSPROJECTS_BNSKORSERVER_DAILYTASK_DAILYTASK_H__

#include <MyTools/ClassInstance.h>
#include <MyTools/CLLock.h>

class ByteBuffer;
struct ClientAccountContent;
class CDailyTask : virtual public CClassInstance<CDailyTask>
{
public:
	CDailyTask();
	~CDailyTask() = default;

public:
	// 查询日常任务进度
	BOOL QueryDailyTaskSchedule(_In_ CONST ClientAccountContent&, _Out_ ByteBuffer& ByteBuffer_) CONST;

	// 设置日常任务进度=完成
	BOOL SetDailyTaskScheduleFinish(_In_ CONST ClientAccountContent&, _In_ CONST std::wstring& wsDailyTaskName, _Out_ ByteBuffer& ByteBuffer_) CONST;

	// 设置日常任务的副本名
	VOID AddDailyTask(_In_ CONST std::wstring& wsDailyTaskName);

	// 清除
	VOID ClearDailyTask();
private:
	std::vector<std::wstring> VecDailyTask;
	CLLock LockVecDailyTask;
};



#endif
