#include "stdafx.h"
#include "CatExpend.h"
#include "ObjectFunction.h"
#include "Person.h"

BOOL CCatExpend::FindSelfCat(_Out_ CCat* pCat)
{
	if (pCat != nullptr)
	{
		pCat->SetBase(ReadDWORD(ReadDWORD(ReadDWORD(人物基址) + 人物基址偏移1) + 人物基址偏移2 + 4));
		return pCat->GetBase() != NULL;
	}
	return FALSE;
}

BOOL CCatExpend::FindCatById(_In_ DWORD dwId, _In_ CONST FixPointInfo& FixPtInfo, _Out_ CCat* pCat) CONST
{
	return FindCatByCondition(FixPtInfo, pCat, [&dwId](CONST CCat& Cat){
		return Cat.GetId() == dwId;
	});
}

BOOL CCatExpend::FindCatByName(_In_ CONST std::wstring& wsCatName, _In_ CONST FixPointInfo& FixPtInfo, _Out_ CCat* pCat) CONST
{
	return FindCatByCondition(FixPtInfo, pCat, [&wsCatName](CONST CCat& Cat){
		return Cat.GetName() == wsCatName;
	});
}

BOOL CCatExpend::FindCatByCondition(_In_ CONST FixPointInfo& FixPtInfo, _Out_ CCat* pCat, _In_ std::function<BOOL(CONST CCat&)> Finder) CONST
{
	return CObjectFunction::GetInstance().FindElement_By_ObjectTraverse<CCat>(CObjectFunction::em_GameObject_Type_Pet, FixPtInfo, pCat, Finder);
}
