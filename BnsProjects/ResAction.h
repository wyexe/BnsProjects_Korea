#ifndef __GITBNSPROJECTS_BNSPROJECTS_OBJECT_OTHEROBJECT_CORPSE_RESACTION_H__
#define __GITBNSPROJECTS_BNSPROJECTS_OBJECT_OTHEROBJECT_CORPSE_RESACTION_H__

#include "GameVariable.h"

class CResAction : public MyTools::CClassInstance<CResAction>
{
public:
	CResAction() = default;
	~CResAction() = default;

public:
	// ºÒŒÔ-ºÒπ÷ŒÔµÙ¬‰
	BOOL TakeDropItem(_In_ CONST FixPointInfo& FixPtInfo,_In_ float Ringe = 30.0f) CONST;

	// …Ë÷√æ∫≈ƒ
	VOID SetPartyAuctionPtr(_In_ std::function<BOOL(VOID)> fnPtr);
private:

private:
	std::function<BOOL(VOID)> fnPartyAuctionPtr;
private:
	DSIABLE_COPY_AND_ASSIGN(CResAction)
};



#endif