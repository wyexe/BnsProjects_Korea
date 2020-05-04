#ifndef __GITBNSPROJECTS_BNSPROJECTS_SCRIPT_DAILYSCRIPT_H__
#define __GITBNSPROJECTS_BNSPROJECTS_SCRIPT_DAILYSCRIPT_H__

#include "GameVariable.h"
#include "BnsScript.h"

class CDailyScript : public CBnsScript
{
public:
	explicit CDailyScript() = default;
	~CDailyScript() = default;

	BOOL Run(_In_ CONST std::wstring& wsDailyTaskName);

private:
	// ��ȡ�ű��ļ�
	BOOL ReadDailyScript(_In_ CONST std::wstring& wsDailyTaskName);

	// ��ʼ��
	BOOL InitDailyScipt();

	// 
	BOOL Start(_In_ CONST std::wstring& wsDailyTaskName);
};



#endif