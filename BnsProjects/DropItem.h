#ifndef __BNS_DROPITEM_H__
#define __BNS_DROPITEM_H__

#include "Player.h"
class CDropItem : public CPlayer
{
public:
	explicit CDropItem(DWORD dwDropItemBase);
	explicit CDropItem();
	~CDropItem();

	// Point
	virtual CONST Point	GetPoint() CONST;

	//
	virtual float GetDis() CONST;
	virtual float GetDis(_In_ CONST Point& Tar) CONST;
	//
	virtual BOOL  IsExist() CONST;
 
	//
	virtual void  SetAngle() CONST;

	//
	virtual CONST std::wstring& GetName() CONST;

public:
	// TakeItem
	BOOL		 TakeItem() CONST;
	
	// PickCorpse
	BOOL		PickCorpse() CONST;

	DWORD		GetMonsterId() CONST;

	//判断是否可捡，捡物用()
	BOOL		IsDropItem() CONST;

	BOOL		IsCorpse() CONST;

	bool operator <(const CDropItem& DropItem) CONST;
};

#endif