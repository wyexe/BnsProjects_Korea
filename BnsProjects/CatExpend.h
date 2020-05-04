#ifndef __GITBNSPROJECTS_BNSPROJECTS_OBJECT_GAMEOBJECT_CAT_CATEXPEND_H__
#define __GITBNSPROJECTS_BNSPROJECTS_OBJECT_GAMEOBJECT_CAT_CATEXPEND_H__

#include "Cat.h"

class CCatExpend : public MyTools::CClassInstance<CCatExpend>
{
public:
	CCatExpend() = default;
	~CCatExpend() = default;

public:
	BOOL FindSelfCat(_Out_ CCat* pCat);
	BOOL FindCatById(_In_ DWORD dwId, _In_ CONST FixPointInfo& FixPtInfo, _Out_ CCat* pCat) CONST;
	BOOL FindCatByName(_In_ CONST std::wstring& wsCatName, _In_ CONST FixPointInfo& FixPtInfo, _Out_ CCat* pCat) CONST;
	BOOL FindCatByCondition(_In_ CONST FixPointInfo& FixPtInfo, _Out_ CCat* pCat, _In_ std::function<BOOL(CONST CCat&)> Finder) CONST;

private:

};


#endif