#ifndef __GITBNSPROJECTS_BNSPROJECTS_OBJECTS_FARMSKILL_FARSKILL_H__
#define __GITBNSPROJECTS_BNSPROJECTS_OBJECTS_FARMSKILL_FARSKILL_H__

#include "GameVariable.h"
#include <MyTools/CLLock.h>

class CSkill;
class CUiSkill;
class CPlayer;
class CFarmSkill : public MyTools::CClassInstance<CFarmSkill>
{
public:
	CFarmSkill() ;
	~CFarmSkill() = default;

public:

	// 使用力士的E技能,还没冷却完毕返回FALSE
	BOOL UseSkill_By_Destoer_E();

	// 使用力士的Q技能, 还没冷却完毕返回FALSE
	BOOL UseSkill_By_Destoer_Q();

	// 判断技能是否 不能使用
	BOOL IsActive_By_SkillId_In_Ui(_In_ DWORD dwSkillId) CONST;

	// 判断技能ID是否冷却完毕(技能冷却CALL+UI)
	BOOL IsCool_By_SkillId_In_Ui(_In_ DWORD dwSkillId);

	// 判断技能ID是否存在 在UI上
	BOOL IsExist_By_SkillId_In_Ui(_In_ DWORD dwSkillId) CONST;

	// 判断技能ID是否冷却完毕(技能冷却CALL+UI) , 参数2表示 是否忽略技能是否被亮了
	BOOL IsCool_By_SkillId_In_Ui(_In_ DWORD dwSkillId, _In_ BOOL bPassSkillActive);

	// 使用技能_ById(等待施法完毕)
	BOOL UseSkill_By_SkillId(_In_ DWORD dwSkillId, _In_ em_Skill_Type emSkillType = em_Skill_Type_Person);
	BOOL UseSkill_By_SkillId_WhichCool(_In_ DWORD dwSkillId, _In_ em_Skill_Type emSkillType = em_Skill_Type_Person);

	// 使用技能_ById(参数决定是否等待施法完毕)
	BOOL UseSkill_BySkillId_In_Sleep(_In_ DWORD dwSkillId, _In_ BOOL bNeedResult = TRUE, _In_ em_Skill_Type emSkillType = em_Skill_Type_Person);

	// 查询技能 By Name
	BOOL	GetSkill_By_Name(_In_ CONST std::wstring& wsSkillName, _Out_opt_ CSkill* pSkill = nullptr) CONST;
	BOOL	GetSkill_By_Id(_In_ DWORD dwSkillId, _Out_opt_ CSkill* pSkill = nullptr) CONST;
	BOOL	GetSkill_By_Condiction(_In_ std::function<BOOL(CONST CSkill&)> fn, _Out_opt_ CSkill* pSkill = nullptr) CONST;

	// 查询技能 By ID In Ui
	BOOL	GetUiSkill_By_Id(_In_ DWORD dwSkillId, _Out_opt_ CUiSkill* pUiSkill) CONST;

	// 倒地技能（参数=职业）
	BOOL UseFloorSkill(_In_ em_Player_Classes emPlayerClasses);

	// 使用SS(内置分类了职业)
	BOOL UseSkill_By_SS();

	// 判断隐藏技能系列是否冷却完毕
	BOOL IsCool_By_SkillId_In_HideUi(_In_ DWORD dwSkillId);

	// 判断是否出现QTE了
	BOOL ExistQTE() CONST;

	// 判断是否花粉技能 灰了
	BOOL IsUnEnableSkill3() CONST;

	// 等待技能冷却
	VOID WaitForCool_By_SkillId(DWORD dwSkillId);

	// 使用大招
	VOID UseULT();

	// 被禁用的技能
	VOID AddDisableSkillId(_In_ DWORD dwSkillId);
	VOID RemoveDisableSkillId(_In_ DWORD dwSkillId);
	BOOL IsDisableSkill(_In_ DWORD dwSkillId);

private:
	std::mutex _MtxSetDisbaleSkillId;
	std::set<DWORD> _SetDisableSkillId;
private:
	DSIABLE_COPY_AND_ASSIGN(CFarmSkill)
};


#endif