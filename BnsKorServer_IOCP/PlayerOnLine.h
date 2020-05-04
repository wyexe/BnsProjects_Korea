#ifndef __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_CONTENTONLINE_PLAYERONLINE_H__
#define __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_CONTENTONLINE_PLAYERONLINE_H__

#include <MyTools/ClassInstance.h>
#include "ServerCommon.h"

class CPlayerOnLine : public MyTools::CClassInstance<CPlayerOnLine>
{
public:
	CPlayerOnLine() = default;
	~CPlayerOnLine() = default;


	// 
	VOID SetKeepALiveContent(_In_ CONST std::wstring& wsPlayerName, _In_ CONST KeepALiveContent& Content);
	VOID SetKeepALiveContent(_In_ CONST std::wstring& wsPlayerName, _In_ CONST std::wstring& wsContent);

	// 
	BOOL GetKeepALiveContent(_In_ CONST std::wstring& wsPlayerName, _Out_ KeepALiveContent& Content);

private:
	std::map<std::wstring, KeepALiveContent> _MapPlayer;
	std::mutex								 _MtxPlayerOnLineContent;
};



#endif // !__GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_CONTENTONLINE_PLAYERONLINE_H__
