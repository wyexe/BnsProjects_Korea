#ifndef __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_TEXT_SERVERCOMMAND_H__
#define __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_TEXT_SERVERCOMMAND_H__

#include <MyTools/ClassInstance.h>
#include <MyTools/CLExpression.h>

class CServerCommand : public MyTools::CExprFunBase, virtual public MyTools::CClassInstance<CServerCommand>
{
public:
	CServerCommand() = default;
	virtual ~CServerCommand() = default;

	virtual VOID Release();

	//
	VOID Initialize();

	//
	virtual std::vector<MyTools::ExpressionFunPtr>& GetVec();
private:
	//
	virtual VOID Help(CONST std::vector<std::wstring>& VecParam);

	//
	virtual VOID Refresh(CONST std::vector<std::wstring>& VecParam);

	//
	virtual VOID FindCard(CONST std::vector<std::wstring>& VecParam);

	//
	virtual VOID FindAccount(CONST std::vector<std::wstring>& VecParam);

	//
	virtual VOID FindPlayer(CONST std::vector<std::wstring>& VecParam);

	//
	virtual VOID FindTeamMember(CONST std::vector<std::wstring>& VecParam);

	//
	virtual VOID ClearLog(CONST std::vector<std::wstring>& VecParam);
};



#endif // !__GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_TEXT_SERVERCOMMAND_H__
