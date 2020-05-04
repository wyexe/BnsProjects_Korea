#ifndef __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_COMMON_DAILYTASK_SKILL_H__
#define __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_COMMON_DAILYTASK_SKILL_H__

#include <thread>
#include <atomic>
#include "GameVariable.h"
#include "UiSkill.h"

class CDailyTask_Skill
{
public:
	CDailyTask_Skill();
	~CDailyTask_Skill() = default;

	// Ñ§Ï°¼¼ÄÜ
	BOOL LearningSkill(_In_ CONST std::wstring& wsSkillType, _In_ CONST std::vector<LearnSkillContent>& VecSkillContent);

	// 
	BOOL IsCool_By_SkillId_In_Ui(_In_ DWORD dwSkillId) CONST;

	// 
	BOOL IsCool_By_SkillName_In_Ui(_In_ CONST std::wstring& SkillName) CONST;

	//
	BOOL IsActiveCool_By_SkillId_In_Ui(_In_ DWORD dwSkillId) CONST;

	//
	BOOL UseSkill_By_SkillId_WhichCool(_In_ DWORD dwSkillId) CONST;

	// 
	BOOL IsCool_By_SkillId_In_HideUi(_In_ DWORD dwSkillId) CONST;

	//
	VOID UseSkill_By_SkillId(_In_ DWORD dwSkillId, _In_ BOOL IsAsync) CONST;

	//
	VOID RefreshVecSkill();

	//
	BOOL IsCool_By_KeyText_In_Ui(_In_ CONST std::wstring& wsKeyText) CONST;

	//
	BOOL IsActiveCool_By_KeyText_In_Ui(_In_ CONST std::wstring& wsKeyText) CONST;
private:
	CONST CUiSkill* FindSkillById(_In_ DWORD dwSKillId) CONST;
	CONST CUiSkill* FindSkillByName(_In_ CONST std::wstring& SkillName) CONST;
private:
	std::vector<CUiSkill> _VecSkill;
	std::vector<DWORD> _VecHideSkillId;
};


#endif // !__GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_COMMON_DAILYTASK_SKILL_H__
