#ifndef __GITBNSPROJECTS_BNSPROJECTS_OBJECT_OTHEROBJECT_CORPSE_RESEXTEND_H__
#define __GITBNSPROJECTS_BNSPROJECTS_OBJECT_OTHEROBJECT_CORPSE_RESEXTEND_H__

#include "GameVariable.h"

class CCorpse;
class CDropItem;
class CResExtend : public MyTools::CClassInstance<CResExtend>
{
public:
	CResExtend() = default;
	~CResExtend() = default;

public:
	//////尸体////////////////////////////////////////////////////////////////////

	// 查找尸体
	BOOL FindCorpse_By_Id(_In_ DWORD dwCorpseId, _In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CCorpse* pCorpse) CONST;
	BOOL FindCorpse_By_Name(_In_ CONST std::wstring& wsCorpseName, _In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CCorpse* pCorpse) CONST;
	BOOL FindCorpse_By_Condition(_In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CCorpse* pCorpse, std::function<BOOL(CONST CCorpse&)> fnFindCorpse) CONST;

	BOOL DoAction_By_ExistCorpse_For_Name(_In_ CONST std::wstring& wsCorpseName, _In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<VOID(CONST CCorpse&)> Worker) CONST;

	// 返回尸体列表
	UINT GetAroundCorpseCount(_In_ CONST FixPointInfo& FixPtInfo) CONST;
	UINT GetAroundCorpseList(_In_ CONST FixPointInfo& FixPtInfo, _Out_ std::vector<CCorpse>& vlst) CONST;
	UINT GetCorpseList_By_Condiction(_In_ CONST FixPointInfo& FixPtInfo, _Out_ std::vector<CCorpse>& vlst, _In_ std::function<BOOL(CONST CCorpse&)> fnFind) CONST;
	BOOL Action_Corpse_By_MinDis(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<VOID(CONST CCorpse&)> Worker) CONST;

	////地面物品//////////////////////////////////////////////////////////////////////

	// 返回物品列表
	UINT GetAroundDropItem(_In_ CONST FixPointInfo& FixPtInfo, _Out_ std::vector<CDropItem>& vlst) CONST;
	BOOL GetCurrentDropItem(_In_ CONST FixPointInfo& FixPtInfo, _Out_ CDropItem& DropItem) CONST;

	UINT GetAroundCorpseDropItem(_In_ CONST FixPointInfo& FixPtInfo, _Out_ std::vector<CDropItem>& vlst) CONST;

	// 返回僵尸尸体
	BOOL FindDropItemCorpse_By_Id(_In_ DWORD dwId, _In_ CONST FixPointInfo& FixPtInfo, _In_  CDropItem* pDropItem) CONST;

	BOOL FindDropItem_By_Condition(_In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CDropItem* pDropItem, std::function<BOOL(CONST CDropItem&)> fnFind) CONST;


	//////球////////////////////////////////////////////////////////////////////

	// 返回球
	BOOL FindTopBall(_In_ CONST FixPointInfo& FixPtInfo, _In_ CONST std::vector<DWORD>& FilterBallIdVec, _Out_ CCorpse* pBall) CONST;
	BOOL FindBall_By_Id(_In_ DWORD dwId, _In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CCorpse* pBall) CONST;
	BOOL FindBall_By_Condition(_In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CCorpse* pBall, std::function<BOOL(CONST CCorpse&)> fnFind) CONST;
	UINT GetAroundBallList(_In_ CONST FixPointInfo& FixPtInfo, _Out_ std::vector<CCorpse>& vlst) CONST;
	 
private:
	DSIABLE_COPY_AND_ASSIGN(CResExtend)
};



#endif