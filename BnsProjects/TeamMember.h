#ifndef __GITBNSPROJECTS_BNSPROJECTS_OBJECTS_GAMEOBJECTS_TEAMMEMBER_H__
#define __GITBNSPROJECTS_BNSPROJECTS_OBJECTS_GAMEOBJECTS_TEAMMEMBER_H__

#include "BaseObject.h"
class CTeamMember : public CBaseObject
{
public:
	explicit CTeamMember();
	explicit CTeamMember(DWORD dwNodeBase, DWORD dwIndex_);
	~CTeamMember();

	// Member Name
	virtual CONST std::wstring& GetName() CONST;
	virtual VOID SetName() CONST;

	// Member Point
	virtual CONST Point GetPoint() CONST;

	virtual float GetDis() CONST;

	virtual float GetDis(_In_ CONST Point& Tar) CONST;

	virtual BOOL IsExist() CONST;

	virtual void SetAngle() CONST;

public:
	// Member Channel
	DWORD GetChannel() CONST;

	// Is Captain
	BOOL IsCaptain() CONST;
	BOOL SetCaptain();

	// Is lose connect
	BOOL IsConnect() CONST;

	// Leave Team
	BOOL DeleteMember() CONST;

	// GetPercentHp
	DWORD GetPercentHp() CONST;

	DWORD GetIndex() CONST;

	// MapId
	DWORD GetMapId() CONST;
private:
	DWORD dwIndex;
	mutable std::wstring Name_;
};



#endif