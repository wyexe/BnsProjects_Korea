#ifndef __GITBNSPROJECTS_BNSPROJECTS_DAILY_DAILYEXTEND_H__
#define __GITBNSPROJECTS_BNSPROJECTS_DAILY_DAILYEXTEND_H__

#include "GameStruct.h"
#include <MyTools/ClassInstance.h>
class CDailyExtend : public MyTools::CClassInstance<CDailyExtend>
{
public:
	CDailyExtend() = default;
	~CDailyExtend() = default;

	VOID SetDailyTaskFinish(_In_ CONST std::wstring& wsDailyTaskName) CONST;
private:
	DSIABLE_COPY_AND_ASSIGN(CDailyExtend);
};



#endif