#ifndef __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_TASK_DAILYTASK_H__
#define __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_TASK_DAILYTASK_H__

#include <MyTools/ClassInstance.h>

struct ClientAccountContent;
class CDailyTask : public MyTools::CClassInstance<CDailyTask>
{
public:
	CDailyTask() : _Lock(L"CDailyTask.Lock") {}
	~CDailyTask() = default;

	// ���¶�ȡ'�ճ�'Ŀ¼�µ��ճ�������
	VOID ReFillVecDailyTaskName(_In_ CONST std::vector<std::wstring>& Vec);

	// ��������ʺŻ�û��ɵ��ճ�������
	BOOL QueryDailyTaskSchedule(_In_ CONST ClientAccountContent& AccountContent, _Out_ std::vector<std::wstring>& VecDailyTaskName) CONST;

	// �����ʺŵ��ճ�����=���
	VOID SetDailyTaskScheduleFinish(_In_ CONST ClientAccountContent& AccountContent, _In_ CONST std::wstring& wsDailyTaskName) CONST;
private:
	std::vector<std::wstring> _VecDailyTasName;
	MyTools::CLLock _Lock;
};



#endif // !__GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_TASK_DAILYTASK_H__
