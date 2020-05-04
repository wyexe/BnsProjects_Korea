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
	// ��ѯ�ճ��������
	BOOL QueryDailyTaskSchedule(_In_ CONST ClientAccountContent&, _Out_ ByteBuffer& ByteBuffer_) CONST;

	// �����ճ��������=���
	BOOL SetDailyTaskScheduleFinish(_In_ CONST ClientAccountContent&, _In_ CONST std::wstring& wsDailyTaskName, _Out_ ByteBuffer& ByteBuffer_) CONST;

	// �����ճ�����ĸ�����
	VOID AddDailyTask(_In_ CONST std::wstring& wsDailyTaskName);

	// ���
	VOID ClearDailyTask();
private:
	std::vector<std::wstring> VecDailyTask;
	CLLock LockVecDailyTask;
};



#endif
