#ifndef __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_PLAYER_GAMEPLAYEREXTEND_H__
#define __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_PLAYER_GAMEPLAYEREXTEND_H__

#include <MyTools/ClassInstance.h>
#include "ServerCommon.h"

class CGamePlayerExtend : public MyTools::CClassInstance<CGamePlayerExtend>
{
public:
	CGamePlayerExtend() = default;
	~CGamePlayerExtend() = default;

	VOID Initialize();

	em_TeamMeber_Classes FindPlayerClasses(_In_ CONST std::wstring& wsClassesText) CONST;

	std::wstring FindPlayerClasses(_In_ em_TeamMeber_Classes emPlayerClasses) CONST;
private:
	std::map<std::wstring, em_TeamMeber_Classes> _MapClassesText;
};



#endif // !__GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_PLAYER_GAMEPLAYEREXTEND_H__
