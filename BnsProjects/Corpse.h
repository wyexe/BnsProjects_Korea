#ifndef __BNS_CORPSE_H__
#define __BNS_CORPSE_H__

#include "Player.h"

class CCorpse : public CPlayer
{
public:
	explicit CCorpse();
	explicit CCorpse(DWORD dwCorpseBase);
	~CCorpse();

	//
	virtual CONST Point	GetPoint()					CONST;
	//
	virtual float	GetDis()						CONST;
	//
	virtual CONST std::wstring& GetName()						CONST;
	virtual VOID    SetName()						CONST;
	//
	virtual BOOL	IsExist()						CONST;
	//
	virtual void	SetAngle()						CONST;
	//
	virtual float	GetDis(_In_ CONST Point& Tar)	CONST;
public:

	// Type
	DWORD	GetIdType()		CONST;

	// Take
	BOOL	TakeItem()		CONST;
	
	BOOL	TakeBall()		CONST;

	// IsCollect  判断任务是否可捡
	BOOL IsCollect()		CONST;
	BOOL IsTaskCollect()	CONST;

	bool operator <(const CCorpse& Corpse) CONST;
};

#endif