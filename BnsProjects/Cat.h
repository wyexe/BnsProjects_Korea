#ifndef __GITBNSPROJECTS_BNSPROJECTS_OBJECT_GAMEOBJECT_PET_PET_H__
#define __GITBNSPROJECTS_BNSPROJECTS_OBJECT_GAMEOBJECT_PET_PET_H__

#include "Player.h"

class CCat : public CPlayer
{
public:
	explicit CCat();
	explicit CCat(_In_ DWORD dwNodeBase_);
	~CCat();

	DWORD GetMasterId() CONST;
private:

};



#endif