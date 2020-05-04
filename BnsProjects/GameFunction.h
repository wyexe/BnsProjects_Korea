#ifndef __BNS_GAMEFUNCTION_H__
#define __BNS_GAMEFUNCTION_H__

#include "GameVariable.h"
#include "BagBase.h"
class CGameFunction : public MyTools::CClassInstance<CGameFunction>
{
public:
	explicit CGameFunction() = default;
	~CGameFunction() = default;

	// Hook 怪物技能
	static VOID AddMonsterSkill(_In_ DWORD dwMonsterBase, _In_ DWORD dwSkillId);
private:
	// 时钟循环
	static void CALLBACK TimerProc(HWND hWnd, UINT nMsg, UINT nTimerid, DWORD dwTime);
	// 枚举当前窗口
	static BOOL CALLBACK EnumSetWinName(HWND h, LPARAM l);
	
public:

	// 初始化时钟
	BOOL InitTimer() CONST;

	BOOL IsGameShow(_In_opt_ HWND* pWnd) CONST;

	int GetDianKa() CONST;

	BOOL HookMonsterSkill(_In_ BOOL bHook, _In_ DWORD dwMonsterBase) CONST;

	int GetRecentlyPoint(_In_ CONST std::vector<Point>& vlst, _In_ CONST Point& StartPoint) CONST;

	// 瞬移
	BOOL Telport(_In_ CONST std::wstring& wsFileName) CONST;

	// 瞬移
	BOOL LineTelport(_In_ CONST FixPointInfo& FixPtInfo, _In_ CONST Point& TarPoint, _In_ DWORD dwMaxCount) CONST;
	BOOL HeightTelport(_In_ CONST FixPointInfo& FixPtInfo, _In_ CONST Point& TarPoint, _In_ DWORD dwMaxCount) CONST;
	BOOL FloatTelport(_In_ CONST FixPointInfo& FixPtInfo, _In_ CONST Point& TarPoint, _In_ DWORD dwMaxCount) CONST;

	// 学习技能
	BOOL StudySkill(_In_ CONST std::vector<LearnSkillContent>& vlst,_In_ CONST DWORD Ntype) CONST;
	BOOL SwitchSkillPage(_In_ CONST std::wstring& wsSkillType) CONST;

	// 装备强化(强化完毕返回TRUE)
	BOOL GrowthEqui(_In_ CBag::em_Equi_Type emType);
	
	// 初始化导航寻路
	BOOL InitNavigation() CONST;

	// 寻路
	BOOL NavFindPath(_In_ CONST std::string& csTarMapName, _In_ CONST Point& TarPoint, _Out_opt_ std::vector<Point>& vPoint) CONST;

	
private:
	DSIABLE_COPY_AND_ASSIGN(CGameFunction)
};



#endif