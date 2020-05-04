#include "GamePlayerExtend.h"
#include <MyTools/Log.h>

#define _SELF L"GamePlayerExtend.cpp"
VOID CGamePlayerExtend::Initialize()
{
	CONST static std::map<std::wstring, em_TeamMeber_Classes> Vec =
	{
		{ L"?",em_TeamMeber_Classes::em_TeamMeber_Classes_Any },
		{ L"枪手",em_TeamMeber_Classes::em_TeamMeber_Classes_Gunner },
		{ L"气宗",em_TeamMeber_Classes::em_TeamMeber_Classes_KongFuForceMaster },
		{ L"咒术师",em_TeamMeber_Classes::em_TeamMeber_Classes_Warlock },
		{ L"灵剑士",em_TeamMeber_Classes::em_TeamMeber_Classes_Danncer },
		{ L"拳师",em_TeamMeber_Classes::em_TeamMeber_Classes_KongFuMaster },
		{ L"气功",em_TeamMeber_Classes::em_TeamMeber_Classes_ForceMaster },
		{ L"召唤师",em_TeamMeber_Classes::em_TeamMeber_Classes_Summoner },
		{ L"剑士",em_TeamMeber_Classes::em_TeamMeber_Classes_BladeMaster },
		{ L"刺客",em_TeamMeber_Classes::em_TeamMeber_Classes_Assassin },
		{ L"力士",em_TeamMeber_Classes::em_TeamMeber_Classes_Destroyer },
	};

	_MapClassesText = Vec;
}

em_TeamMeber_Classes CGamePlayerExtend::FindPlayerClasses(_In_ CONST std::wstring& wsClassesText) CONST
{
	auto itr = _MapClassesText.find(wsClassesText);
	if (itr == _MapClassesText.end())
	{
		LOG_MSG_CF(L"不存在职业[%s] in _MapClassesText", wsClassesText.c_str());
		::Sleep(INFINITE);
	}

	return itr->second;
}

std::wstring CGamePlayerExtend::FindPlayerClasses(_In_ em_TeamMeber_Classes emPlayerClasses) CONST
{
	for (CONST auto& itr : _MapClassesText)
	{
		if (itr.second == emPlayerClasses)
		{
			return itr.first;
		}
	}

	return std::wstring(L"EmptyClasses");
}
