#ifndef __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_CONTENTONLINE_ONLINEOUTPUT_H__
#define __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_CONTENTONLINE_ONLINEOUTPUT_H__

#include <MyTools/ClassInstance.h>

struct CardInfo;
class COnLineOutput : public MyTools::CClassInstance<COnLineOutput>
{
public:
	COnLineOutput() = default;
	~COnLineOutput() = default;

	//
	VOID PrintPlayerKeepALiveContent(_In_ CONST std::wstring& wsPlayerName) CONST;

	//
	VOID PrintCardByPlayerName(_In_ CONST std::wstring& wsPlayerName) CONST;

	//
	VOID PrintCardByAccountName(_In_ CONST std::wstring& wsAccountName, _Out_ std::wstring* pwsPlayerName) CONST;

	//
	VOID PrintCardContent(_In_ CONST CardInfo& Card) CONST;

	//
	VOID PrintTeamByAccount(_In_ CONST std::wstring& wsAccountName) CONST;
private:

};



#endif // !__GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_CONTENTONLINE_ONLINEOUTPUT_H__
