#include "stdafx.h"
#include "FarmSkill.h"
#include <MyTools/Log.h>
#include <MyTools/CLEchoException.h>
#include "SomeFun.h"
#include "Person.h"
#include "ObjectFunction.h"
#include "UiSkill.h"
#include "Skill.h"

#define _SELF L"FarmSkill.cpp"
 
CFarmSkill::CFarmSkill()
{
	
}


BOOL CFarmSkill::UseSkill_By_SkillId(_In_ DWORD dwSkillId, _In_ em_Skill_Type emSkillType /* = em_Skill_Type_Person */)
{
	return UseSkill_BySkillId_In_Sleep(dwSkillId, TRUE, emSkillType);
}

BOOL CFarmSkill::UseSkill_By_SkillId_WhichCool(_In_ DWORD dwSkillId, _In_ em_Skill_Type emSkillType /*= em_Skill_Type_Person*/)
{
	return !IsCool_By_SkillId_In_Ui(dwSkillId) ? FALSE : UseSkill_By_SkillId(dwSkillId, emSkillType);
}

BOOL CFarmSkill::UseSkill_BySkillId_In_Sleep(_In_ DWORD dwSkillId, _In_ BOOL bNeedResult /* = TRUE */, _In_ em_Skill_Type emSkillType /* = em_Skill_Type_Person */)
{
	// 直接粗暴的施法!
	return CSkill::UseSkill_By_Id(dwSkillId, bNeedResult, emSkillType);
}

BOOL CFarmSkill::GetSkill_By_Name(_In_ CONST std::wstring& wsSkillName, _Out_opt_ CSkill* pSkill /* = nullptr OPTIONAL */) CONST
{
	return GetSkill_By_Condiction([&wsSkillName](CONST CSkill& Skill){
		return wsSkillName == Skill.GetName();
	}, pSkill);
}

BOOL CFarmSkill::GetSkill_By_Id(_In_ DWORD dwSkillId, _Out_opt_ CSkill* pSkill /*= nullptr OPTIONAL*/) CONST
{
	return GetSkill_By_Condiction([&dwSkillId](CONST CSkill& Skill){
		return dwSkillId == Skill.GetId();
	}, pSkill);
}

BOOL CFarmSkill::GetSkill_By_Condiction(_In_ std::function<BOOL(CONST CSkill&)> fn, _Out_opt_ CSkill* pSkill /* = nullptr */) CONST
{
	std::vector<CSkill> vlst;
	CObjectFunction::GetInstance().GetSkillList(vlst);

	return MyTools::CLPublic::Vec_find_if(vlst, pSkill, fn);
}

BOOL CFarmSkill::UseFloorSkill(_In_ em_Player_Classes emPlayerClasses)
{
	CONST static  std::vector<DWORD> vlst = {
		// 力士
		Lux_SkillId_Floor_Tab, Lux_SkillId_Floor_1, Lux_SkillId_Floor_2,
		// 气宗
		Kffm_Kfm_SkillId_Floor_Tab, Kffm_Kfm_SkillId_Floor_1, Kffm_Kfm_SkillId_Floor_2, Kffm_Kfm_SkillId_Floor_22, 
		Kffm_Kfm_SkillId_Floor_1 + 0x1, Kffm_Kfm_SkillId_Floor_2 + 0x2, 0x7642, Kffm_Kfm_SkillId_Floor_F,
		/// 召唤
		Sum_SkillId_Floor_1, Sum_SkillId_Floor_2, Sum_SkillId_Floor_F, Sum_SkillId_T_Floor, Sum_SkillId_T_Floor2,
		//咒术
		Warlock_SkillId_Floor_1, Warlock_SkillId_Floor_2, Warlock_SkillId_Floor_3,
		//剑士
		BladeMaster_SkillId_Floor_Tab, BladeMaster_SkillId_Floor_1, BladeMaster_SkillId_Floor_3, BladeMaster_SkillId_Floor_4,BladeMaster_SkillId_Floor_F,
		//气功
		ForceMaster_SkillId_Floor_Tab, ForceMaster_SkillId_Floor_1,
		//刺客
		Assassin_SkillId_Floor_Tab, Assassin_SkillId_Floor_Tab1, Assassin_SkillId_Floor_Tab2, Assassin_SkillId_Floor_1, Assassin_SkillId_Floor_2, Assassin_SkillId_Floor_11, Assassin_SkillId_Floor_21,Assassin_SkillId_Floor_F,
		//灵剑士
		Danncer_SkillId_Floor_Tab, Danncer_SkillId_Floor_F,Danncer_SkillId_Floor_1, Danncer_SkillId_Floor_2,
		//拳师
		KongFuMaster_SkillId_Floor_Tab, KongFuMaster_SkillId_Floor_Tab1, KongFuMaster_SkillId_Floor_1, KongFuMaster_SkillId_Floor_2,
		//枪手
		Gunner_SkillId_Floor_Tab, Gunner_SkillId_Floor_1
	};

	CONST static std::vector<DWORD> CatVec = {
		Sum_SkillId_Floor_F,Sum_SkillId_Floor_C
	};

	for (CONST auto& itm : vlst)
	{
		if (IsCool_By_SkillId_In_Ui(itm))
		{
			//LOG_CF_D(L"使用技能:%X", itm);
			return CSkill::UseSkill_By_Id(itm, FALSE);
		}
	}

	if (emPlayerClasses != em_Player_Classes::em_Player_Classes_Summoner)
		return FALSE;

	for (CONST auto& itm : CatVec)
	{
		if (IsCool_By_SkillId_In_Ui(itm))
		{
			return CSkill::UseSkill_By_Id(itm, FALSE, em_Skill_Type_Cat);
		}
	}

	return FALSE;
}

BOOL CFarmSkill::UseSkill_By_SS()
{
	static ULONGLONG ulTick = 0;
	if (static_cast<DWORD>(::GetTickCount64() - ulTick) < 12 * 1000)
		return FALSE;

	em_Player_Classes emPlayerClasses = CPerson::GetInstance().GetClasses();
	switch (emPlayerClasses)
	{
	case em_Player_Classes_Destroyer:
		if (IsCool_By_SkillId_In_HideUi(Lux_SkillId_SS))
			return UseSkill_By_SkillId(Lux_SkillId_SS);
		break;
	case em_Player_Classes_Assassin:
		if (IsCool_By_SkillId_In_HideUi(Assassin_SkillId_SS))
			return UseSkill_By_SkillId(Assassin_SkillId_SS);
	case em_Player_Classes_BladeMaster:
		if (IsCool_By_SkillId_In_Ui(BladeMaster_SkillId_SS))
			return UseSkill_By_SkillId(BladeMaster_SkillId_SS);
	case em_Player_Classes_Summoner:
		if (IsCool_By_SkillId_In_HideUi(Sum_SkillId_SS))
			return UseSkill_By_SkillId(Sum_SkillId_SS);
		break;
	case em_Player_Classes_ForceMaster:
		if (IsCool_By_SkillId_In_HideUi(ForceMaster_SkillId_SS))
			return UseSkill_By_SkillId(ForceMaster_SkillId_SS);
	case em_Player_Classes_KongFuMaster:
		break;
	case em_Player_Classes_Danncer:
		if (IsCool_By_SkillId_In_HideUi(Danncer_SkillId_SS))
			return UseSkill_By_SkillId(Danncer_SkillId_SS);
	case em_Player_Classes_Warlock:
		if (IsCool_By_SkillId_In_HideUi(Warlock_SkillId_SS))
			return UseSkill_By_SkillId(Warlock_SkillId_SS);
	case em_Player_Classes_KongFuForceMaster:
		if (IsCool_By_SkillId_In_HideUi(Kffm_Fm_SkillId_SS))
			return UseSkill_By_SkillId(Kffm_Fm_SkillId_SS);
	case em_Player_Classes_Gunner:
		if (IsCool_By_SkillId_In_HideUi(Gunner_SkillId_SS))
			return UseSkill_By_SkillId(Gunner_SkillId_SS);
	default:
		break;
	}
	ulTick = ::GetTickCount64();
	return FALSE;
}

BOOL CFarmSkill::IsCool_By_SkillId_In_Ui(_In_ DWORD dwSkillId)
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__,[&]
	{
		if (IsDisableSkill(dwSkillId))
			return FALSE;

		CUiSkill UiSkill_;
		if (!GetUiSkill_By_Id(dwSkillId, &UiSkill_))
			return FALSE;

		return (UiSkill_.IsCool() && UiSkill_.IsActive()) ? TRUE : FALSE;
	});
}

BOOL CFarmSkill::IsCool_By_SkillId_In_Ui(_In_ DWORD dwSkillId, _In_ BOOL bPassSkillActive)
{
	if (!bPassSkillActive)
		return IsCool_By_SkillId_In_Ui(dwSkillId);

	CUiSkill UiSkill_;
	if (!GetUiSkill_By_Id(dwSkillId, &UiSkill_))
		return FALSE;

	return UiSkill_.IsCool();
}

BOOL CFarmSkill::GetUiSkill_By_Id(_In_ DWORD dwSkillId, _Out_opt_ CUiSkill* pUiSkill) CONST
{
	std::vector<CUiSkill> vlst;
	CObjectFunction::GetInstance().GetUiSkillList(vlst);

	return MyTools::CLPublic::Vec_find_if(vlst, pUiSkill, [&dwSkillId](CONST CUiSkill& UiSkill_){return UiSkill_.GetId() == dwSkillId; });
}

BOOL CFarmSkill::IsExist_By_SkillId_In_Ui(_In_ DWORD dwSkillId) CONST
{
	return GetUiSkill_By_Id(dwSkillId, nullptr);
}

BOOL CFarmSkill::UseSkill_By_Destoer_E()
{
	if (CPerson::GetInstance().GetLevel() >= 36 && IsCool_By_SkillId_In_HideUi(Lux_SkillId_E))
	{
		LOG_CF_D( L"用E!");
		UseSkill_By_SkillId(Lux_SkillId_E);
		return TRUE;
	}
	return FALSE;
}

BOOL CFarmSkill::UseSkill_By_Destoer_Q()
{
	if (IsCool_By_SkillId_In_HideUi(Lux_SkillId_Q))
	{
		UseSkill_By_SkillId(Lux_SkillId_Q);
		return TRUE;
	}
	return FALSE;
}

BOOL CFarmSkill::IsActive_By_SkillId_In_Ui(_In_ DWORD dwSkillId) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__,[&]
	{
		CUiSkill UiSkill;
		return !GetUiSkill_By_Id(dwSkillId, &UiSkill) ? FALSE : UiSkill.IsActive();
	});
}

BOOL CFarmSkill::IsCool_By_SkillId_In_HideUi(_In_ DWORD dwSkillId)
{
	if (IsDisableSkill(dwSkillId))
		return FALSE;

	std::vector<DWORD> vHideSkill;
	CObjectFunction::GetInstance().GetHideUiSkillList(vHideSkill);

	return MyTools::CLPublic::Vec_find_if(vHideSkill, [dwSkillId](DWORD itm) { return itm == dwSkillId; }) == nullptr;
}

BOOL CFarmSkill::ExistQTE() CONST
{
	std::vector<CUiSkill> vlst;
	CObjectFunction::GetInstance().GetUiSkillList(vlst);
	std::sort(vlst.begin(), vlst.end());
	return vlst.size() >= 2 && (vlst.at(vlst.size() - 2).GetId() == vlst.at(vlst.size() - 1).GetId()) && vlst.at(vlst.size() - 1).IsActive() && vlst.at(vlst.size() - 1).IsCool();
}

BOOL CFarmSkill::IsUnEnableSkill3() CONST
{
	if (CPerson::GetInstance().GetTargetId() == NULL)
		return FALSE;

	CUiSkill UiSkill_;
	if (!GetUiSkill_By_Id(Sum_SkillId_3, &UiSkill_))
		return FALSE;

	return UiSkill_.IsCool() && !UiSkill_.IsActive();
}

VOID CFarmSkill::WaitForCool_By_SkillId(DWORD dwSkillId)
{
	while (GameRun && !EMPTY_PERSONHP && !IsCool_By_SkillId_In_Ui(dwSkillId))
	{
		BnsSleep(300);
	}
}

VOID CFarmSkill::UseULT()
{
	if (IsCool_By_SkillId_In_Ui(Sum_SkillId_G))
	{
		UseSkill_BySkillId_In_Sleep(Sum_SkillId_G);
		BnsSleep(5 * 1000);
	}
}

VOID CFarmSkill::AddDisableSkillId(_In_ DWORD dwSkillId)
{
	std::lock_guard<std::mutex> _Lock(_MtxSetDisbaleSkillId);

	if (_SetDisableSkillId.find(dwSkillId) == _SetDisableSkillId.end())
	{
		_SetDisableSkillId.emplace(dwSkillId);
	}
}

VOID CFarmSkill::RemoveDisableSkillId(_In_ DWORD dwSkillId)
{
	std::lock_guard<std::mutex> _Lock(_MtxSetDisbaleSkillId);

	auto itr = _SetDisableSkillId.find(dwSkillId);
	if (itr != _SetDisableSkillId.end())
	{
		_SetDisableSkillId.erase(itr);
	}
}

BOOL CFarmSkill::IsDisableSkill(_In_ DWORD dwSkillId)
{
	std::lock_guard<std::mutex> _Lock(_MtxSetDisbaleSkillId);

	return _SetDisableSkillId.find(dwSkillId) != _SetDisableSkillId.end();
}
