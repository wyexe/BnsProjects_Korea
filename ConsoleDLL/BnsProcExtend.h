#ifndef __GITBNSPROJECTS_CONSOLEDLL_SERVICE_PROCESS_BNSPROCEXTEND_H__
#define __GITBNSPROJECTS_CONSOLEDLL_SERVICE_PROCESS_BNSPROCEXTEND_H__

#include "ConsoleVariable.h"
class CBnsProcExtend
{
public:
	CBnsProcExtend() = default;
	~CBnsProcExtend() = default;

	VOID DestoryBnsClient() CONST;

	BOOL KillGameClientByPId(_In_ DWORD dwPid) CONST;
private:
	VOID GetVecBnsProcId(std::vector<DWORD>& VecPid) CONST;
};



#endif // !__GITBNSPROJECTS_CONSOLEDLL_SERVICE_PROCESS_BNSPROCEXTEND_H__
