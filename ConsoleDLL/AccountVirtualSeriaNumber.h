#ifndef __GITBNSPROJECTS_CONSOLEDLL_FUNCTION_VIRTUALSERIANUMBER_ACCOUNTVIRTUALSERIANUMBER_H__
#define __GITBNSPROJECTS_CONSOLEDLL_FUNCTION_VIRTUALSERIANUMBER_ACCOUNTVIRTUALSERIANUMBER_H__

#include <MyTools/ClassInstance.h>
#include "VirtualSeriaNumberCommon.h"


class CAccountVirtualSeriaNumber : public MyTools::CClassInstance<CAccountVirtualSeriaNumber>
{
public:
	CAccountVirtualSeriaNumber();
	~CAccountVirtualSeriaNumber();

	VOID Create();

	VOID Fill(_In_ CONST AccountVirtualSeriaNumberContent& Content);

	VOID Clear();

	VOID Initialize();
private:
	HANDLE _hFileMap;
	VirtualSeriaNumberShareContent* _pVirtualSeriaNumberShareContent;
};



#endif // !__GITBNSPROJECTS_CONSOLEDLL_FUNCTION_VIRTUALSERIANUMBER_ACCOUNTVIRTUALSERIANUMBER_H__
