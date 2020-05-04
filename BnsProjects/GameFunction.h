#ifndef __BNS_GAMEFUNCTION_H__
#define __BNS_GAMEFUNCTION_H__

#include "GameVariable.h"
#include "BagBase.h"
class CGameFunction : public MyTools::CClassInstance<CGameFunction>
{
public:
	explicit CGameFunction() = default;
	~CGameFunction() = default;

	// Hook ���＼��
	static VOID AddMonsterSkill(_In_ DWORD dwMonsterBase, _In_ DWORD dwSkillId);
private:
	// ʱ��ѭ��
	static void CALLBACK TimerProc(HWND hWnd, UINT nMsg, UINT nTimerid, DWORD dwTime);
	// ö�ٵ�ǰ����
	static BOOL CALLBACK EnumSetWinName(HWND h, LPARAM l);
	
public:

	// ��ʼ��ʱ��
	BOOL InitTimer() CONST;

	BOOL IsGameShow(_In_opt_ HWND* pWnd) CONST;

	int GetDianKa() CONST;

	BOOL HookMonsterSkill(_In_ BOOL bHook, _In_ DWORD dwMonsterBase) CONST;

	int GetRecentlyPoint(_In_ CONST std::vector<Point>& vlst, _In_ CONST Point& StartPoint) CONST;

	// ˲��
	BOOL Telport(_In_ CONST std::wstring& wsFileName) CONST;

	// ˲��
	BOOL LineTelport(_In_ CONST FixPointInfo& FixPtInfo, _In_ CONST Point& TarPoint, _In_ DWORD dwMaxCount) CONST;
	BOOL HeightTelport(_In_ CONST FixPointInfo& FixPtInfo, _In_ CONST Point& TarPoint, _In_ DWORD dwMaxCount) CONST;
	BOOL FloatTelport(_In_ CONST FixPointInfo& FixPtInfo, _In_ CONST Point& TarPoint, _In_ DWORD dwMaxCount) CONST;

	// ѧϰ����
	BOOL StudySkill(_In_ CONST std::vector<LearnSkillContent>& vlst,_In_ CONST DWORD Ntype) CONST;
	BOOL SwitchSkillPage(_In_ CONST std::wstring& wsSkillType) CONST;

	// װ��ǿ��(ǿ����Ϸ���TRUE)
	BOOL GrowthEqui(_In_ CBag::em_Equi_Type emType);
	
	// ��ʼ������Ѱ·
	BOOL InitNavigation() CONST;

	// Ѱ·
	BOOL NavFindPath(_In_ CONST std::string& csTarMapName, _In_ CONST Point& TarPoint, _Out_opt_ std::vector<Point>& vPoint) CONST;

	
private:
	DSIABLE_COPY_AND_ASSIGN(CGameFunction)
};



#endif