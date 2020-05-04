#ifndef __GITBNSPROJECTS_CONSOLEDLL_FUNCTION_CONSOLELOG_H__
#define __GITBNSPROJECTS_CONSOLEDLL_FUNCTION_CONSOLELOG_H__

#include "ConsoleVariable.h"
#include <MyTools/ClassInstance.h>
#include <queue>

class CConsoleLog : public MyTools::CClassInstance<CConsoleLog>
{
public:
	CConsoleLog() = default;
	~CConsoleLog() = default;
	
	//
	VOID Add(_In_ em_ConsoleLog emType, _In_ CONST std::wstring& wsAccountName, _In_ CONST std::wstring& wsLogContent);

	//
	std::function<VOID(em_ConsoleLog, CONST std::wstring&, CONST std::wstring&)> GetAddPtr();
};


#endif // !__GITBNSPROJECTS_CONSOLEDLL_FUNCTION_CONSOLELOG_H__
