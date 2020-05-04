#include "stdafx.h"
#include "DailyTask_Skill.h"
#include <MyTools/Log.h>
#include <MyTools/CLEchoException.h>
#include "GameFunction.h"
#include "SomeFun.h"
#include "Person.h"
#include "FarmSkill.h"
#include "ObjectFunction.h"

#define _SELF L"DailyTask_Skill.cpp"

CDailyTask_Skill::CDailyTask_Skill()
{
	
}

BOOL CDailyTask_Skill::LearningSkill(_In_ CONST std::wstring& wsSkillType, _In_ CONST std::vector<LearnSkillContent>& VecSkillContent)
{
	CONST std::map<std::wstring, DWORD> MapSkillType = 
	{
		{ L"�ٻ����ϵ" ,0xB },{ L"�ٻ����ϵ" ,0x15 },
		{ L"��������ϵ" ,0xB },{ L"��������ϵ" ,0x15 },
		{ L"��ʿ����ϵ" ,0xB },{ L"��ʿ�׵�ϵ" ,0x15 },
		{ L"������ϵ" ,0xB },{ L"������ϵ" ,0x15 },
		{ L"�̿Ͱ�ϵ" ,0xB },{ L"�̿���ϵ" ,0x15 },
		{ L"�齣ʿ��ϵ" ,0xB },{ L"�齣ʿ��ϵ" ,0x15 },
		{ L"ȭʦ��ϵ" ,0xB },{ L"ȭʦ��ϵ" ,0x15 },
		{ L"ǹ�ֻ�ϵ" ,0xB },{ L"ǹ�ְ�ϵ" ,0x15 },
	};
	
	auto itr = MapSkillType.find(wsSkillType);
	if (itr == MapSkillType.end())
	{
		LOG_MSG_CF(L"�����ڸü���ϵ[%s]", wsSkillType.c_str());
		return FALSE;
	}

	if (!GameRun || EMPTY_PERSONHP)
		return FALSE;

	CGameFunction::GetInstance().SwitchSkillPage(wsSkillType);

	auto Vec = VecSkillContent;
	for (auto& itm : Vec)
		itm.dwIndex += itr->second;

	return CGameFunction::GetInstance().StudySkill(Vec, itr->second == 0xB ? 0 : 1);
}

BOOL CDailyTask_Skill::IsCool_By_SkillId_In_Ui(_In_ DWORD dwSkillId) CONST
{
	auto Element = FindSkillById(dwSkillId);
	return Element == nullptr ? FALSE : Element->IsCool();
}

BOOL CDailyTask_Skill::IsCool_By_SkillName_In_Ui(_In_ CONST std::wstring& SkillName) CONST
{
	auto Element = FindSkillByName(SkillName);
	if (Element == nullptr)
		return FALSE;
	if (!Element->IsCool())
		return FALSE;
	if (!Element->IsActive())
		return FALSE;
	return TRUE;
}

BOOL CDailyTask_Skill::IsActiveCool_By_SkillId_In_Ui(_In_ DWORD dwSkillId) CONST
{
	auto Element = FindSkillById(dwSkillId);
	return Element == nullptr ? FALSE : (Element->IsCool() && Element->IsActive());
}

BOOL CDailyTask_Skill::UseSkill_By_SkillId_WhichCool(_In_ DWORD dwSkillId) CONST
{
	auto Element = FindSkillById(dwSkillId);
	if (Element == nullptr)
		return FALSE;
	else if (!Element->IsCool())
		return FALSE;
	else if (!Element->IsActive())
		return FALSE;

	return CFarmSkill::GetInstance().UseSkill_By_SkillId(dwSkillId);
}

BOOL CDailyTask_Skill::IsCool_By_SkillId_In_HideUi(_In_ DWORD dwSkillId) CONST
{
	return MyTools::CLPublic::Vec_find_if_Const(_VecHideSkillId, [dwSkillId](CONST DWORD itm) { return itm == dwSkillId; }) == nullptr;;
}

VOID CDailyTask_Skill::UseSkill_By_SkillId(_In_ DWORD dwSkillId, _In_ BOOL IsAsync) CONST
{
	CFarmSkill::GetInstance().UseSkill_BySkillId_In_Sleep(dwSkillId, IsAsync);
}

VOID CDailyTask_Skill::RefreshVecSkill()
{
	MyTools::CLEchoException::GetInstance().InvokeAction(__FUNCTIONW__, [&]
	{
		CObjectFunction::GetInstance().GetUiSkillList(_VecSkill);

		CObjectFunction::GetInstance().GetHideUiSkillList(_VecHideSkillId);
	});
}


BOOL CDailyTask_Skill::IsCool_By_KeyText_In_Ui(_In_ CONST std::wstring& wsKeyText) CONST
{
	auto Element = MyTools::CLPublic::Vec_find_if_Const(_VecSkill, [wsKeyText](CONST CUiSkill& Skill) { return Skill.GetTextKeyByIndex() == wsKeyText; });
	return Element == nullptr ? FALSE : Element->IsCool();
}

BOOL CDailyTask_Skill::IsActiveCool_By_KeyText_In_Ui(_In_ CONST std::wstring& wsKeyText) CONST
{
	auto Element = MyTools::CLPublic::Vec_find_if_Const(_VecSkill, [wsKeyText](CONST CUiSkill& Skill) { return Skill.GetTextKeyByIndex() == wsKeyText; });
	return Element == nullptr ? FALSE : Element->IsCool() && Element->IsActive();
}

CONST CUiSkill* CDailyTask_Skill::FindSkillById(_In_ DWORD dwSKillId) CONST
{
	return MyTools::CLPublic::Vec_find_if_Const(_VecSkill, [dwSKillId](CONST CUiSkill& Skill) { return Skill.GetId() == dwSKillId; });
}

CONST CUiSkill* CDailyTask_Skill::FindSkillByName(_In_ CONST std::wstring& SkillName) CONST
{
	return MyTools::CLPublic::Vec_find_if_Const(_VecSkill, [SkillName](CONST CUiSkill& Skill) { return Skill.GetName() == SkillName; });
}
