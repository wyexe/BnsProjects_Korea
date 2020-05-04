#ifndef __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_AREA_BNSAREA_H__
#define __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_AREA_BNSAREA_H__

#include <MyTools/ClassInstance.h>

class CBnsArea : public MyTools::CClassInstance<CBnsArea>
{
public:
	struct BnsServerInfo
	{
		std::wstring					wsNiceName; // 1区
		std::wstring					wsRealName; // 강호연파
		DWORD					        dwServerId; // 0x18
	};

	typedef std::vector<BnsServerInfo> BnsSingleArea;

public:
	CBnsArea() = default;
	~CBnsArea() = default;

	// 
	BOOL IsSame_Area(_In_ DWORD dwServerId1, _In_ DWORD dwServerId2) CONST;

	//
	VOID Initialize();
private:
	std::vector<BnsSingleArea> _VecArea;
};



#endif // !__GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_AREA_BNSAREA_H__
