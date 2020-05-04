#ifndef __GITBNSPROJECTS_BNSPROJECTS_SOMEFUN_FARM_FARM_H__
#define __GITBNSPROJECTS_BNSPROJECTS_SOMEFUN_FARM_FARM_H__

#include "GameVariable.h"
#include "FarmTask.h"
struct TaskInfoParm;
class CFarm : public MyTools::CClassInstance<CFarm>, public CFarmTask
{
public:
	explicit CFarm() = default;
	~CFarm() = default;

public:
	BOOL ClearAroundMonster_By_Condiction(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<BOOL(void)> fn) CONST;

	BOOL FarmAround_By_PointList(_In_ CONST FixPointInfo& FixPtInfo, _In_ CONST std::vector<Point>& vPoint, _In_ std::function<BOOL(VOID)> fnComplete) CONST;

	BOOL TakeCorpse_By_PointList(_In_ CONST TaskInfoParm& TaskParm, _In_ CONST FixPointInfo& FixPtInfo, _In_ CONST std::vector<Point>& vPoint, _In_ CONST std::wstring& wsCorpseName) CONST;

	BOOL TakeCorpse(_In_ DWORD dwBuffId, _In_ CONST FixPointInfo& FixPtInfo, _In_ CONST std::wstring& wsCorpseName) CONST;

	BOOL TakeCorpse(_In_ CONST FixPointInfo& FixPtInfo, _In_ CONST std::wstring& wsCorpseName) CONST;

	BOOL TakeCorpse_By_Status(_In_ CONST FixPointInfo& FixPtInfo, _In_ CONST std::wstring& wsCorpseName) CONST;

	BOOL PickCorpse(_In_ DWORD dwBuffId, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	BOOL PickCorpse_By_PointList(_In_ DWORD dwBuffId, _In_ CONST FixPointInfo& FixPtInfo, _In_ CONST std::vector<Point>& vPoint) CONST;

	BOOL PickCorpse_By_PointList(_In_ CONST std::wstring& wsCorpseName, _In_ DWORD dwBuffId, _In_ CONST FixPointInfo& FixPtInfo, _In_ CONST std::vector<Point>& vPoint) CONST;

	BOOL TakeCorpseItem(_In_ CONST TaskInfoParm& TaskParm, _In_ CONST FixPointInfo& FixPtInfo, __in CONST std::wstring& wsCorpseName) CONST;

	BOOL TakeNpcItem(_In_ CONST TaskInfoParm& TaskParm, _In_ CONST FixPointInfo& FixPtInfo, _In_ CONST std::wstring& wsNpcName) CONST;

	BOOL TakeNpcItem(_In_ CONST std::wstring& wsNpcName, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	BOOL TakeNpcItem_By_PointList(_In_ CONST TaskInfoParm& TaskParm, _In_ CONST FixPointInfo& FixPtInfo, _In_ CONST std::wstring& wsNpcName, _In_ CONST std::vector<Point>& vPoint) CONST;

	BOOL TakeBall(_In_ DWORD dwBuffId, _In_ CONST TaskInfoParm& TaskParm, _In_ CONST FixPointInfo& FixPtInfo) CONST;
	
	BOOL TakeBall_By_PointList(_In_ DWORD dwBuffId, _In_ CONST TaskInfoParm& TaskParm, _In_ CONST FixPointInfo& FixPtInfo, _In_ CONST std::vector<Point>& vPoint) CONST;

	BOOL SetRandChannel() CONST;
	BOOL SetFixChannel(_In_ DWORD dwChannel, _In_opt_ BOOL bForce = FALSE) CONST;

	BOOL Pickup_By_Name(_In_ CONST std::wstring& wsName) CONST;
	BOOL Pickup_By_LongMai(_In_ CONST std::wstring& wsLongMaiName) CONST;
	BOOL Pickup_By_Flag(_In_ CONST FixPointInfo& FixPtInfo) CONST;
	BOOL Pickup_By_RepeatName(_In_ CONST std::wstring& wsName) CONST;

	BOOL FarmAround_By_Condition_To(_In_ std::function<BOOL(VOID)> fn) CONST;

	BOOL FarmCorpse(_In_ CONST std::wstring& wsCorpseName, _In_ CONST FixPointInfo& FixPtInfo) CONST;

	BOOL FarmAround_By_Round(_In_ CONST FixPointInfo& RoundInfo, _In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<BOOL(void)> fnExit) CONST;

	// ÕÙ»½ß÷
	BOOL BeckonCat(_In_ BOOL bIgnoreWarStatus = TRUE) CONST;
private:
	DSIABLE_COPY_AND_ASSIGN(CFarm)
};



#endif