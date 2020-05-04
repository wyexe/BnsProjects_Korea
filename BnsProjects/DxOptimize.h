#ifndef __GITBNSPROJECTS_BNSPROJECTS_SOMEFUN_DX_DXOPTIMIZE_H__
#define __GITBNSPROJECTS_BNSPROJECTS_SOMEFUN_DX_DXOPTIMIZE_H__

#include "GameVariable.h"


class CDxOptimize : public MyTools::CClassInstance<CDxOptimize>
{
public:
	CDxOptimize();
	~CDxOptimize() = default;

	BOOL Start();
	BOOL Stop();
private:
	BOOL GetDLLModule();
private:
	HMODULE hmDLL;
private:
	DSIABLE_COPY_AND_ASSIGN(CDxOptimize);
};




#endif