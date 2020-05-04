#ifndef __GITBNSPROJECTS_CONSOLEDLL_CONSOLECOMMAND_H__
#define __GITBNSPROJECTS_CONSOLEDLL_CONSOLECOMMAND_H__

#include <MyTools/CLExpression.h>
#include <MyTools/ClassInstance.h>
class CCmdLogService : private MyTools::CExprFunBase, public MyTools::CClassInstance<CCmdLogService>
{
public:
	CCmdLogService() = default;
	virtual ~CCmdLogService() = default;

	virtual std::vector<MyTools::ExpressionFunPtr>& GetVec();

	virtual VOID Release();

private:
	virtual VOID Help(CONST std::vector<std::wstring>&);

	virtual VOID SetValue(CONST std::vector<std::wstring>& Vec);
private:

};



#endif // !__GITBNSPROJECTS_CONSOLEDLL_CONSOLECOMMAND_H__
