#include "GamePlayerExtend.h"
#include <MyTools/Log.h>

#define _SELF L"GamePlayerExtend.cpp"
VOID CGamePlayerExtend::Initialize()
{
	CONST static std::map<std::wstring, em_TeamMeber_Classes> Vec =
	{
		{ L"?",em_TeamMeber_Classes::em_TeamMeber_Classes_Any },
		{ L"ǹ��",em_TeamMeber_Classes::em_TeamMeber_Classes_Gunner },
		{ L"����",em_TeamMeber_Classes::em_TeamMeber_Classes_KongFuForceMaster },
		{ L"����ʦ",em_TeamMeber_Classes::em_TeamMeber_Classes_Warlock },
		{ L"�齣ʿ",em_TeamMeber_Classes::em_TeamMeber_Classes_Danncer },
		{ L"ȭʦ",em_TeamMeber_Classes::em_TeamMeber_Classes_KongFuMaster },
		{ L"����",em_TeamMeber_Classes::em_TeamMeber_Classes_ForceMaster },
		{ L"�ٻ�ʦ",em_TeamMeber_Classes::em_TeamMeber_Classes_Summoner },
		{ L"��ʿ",em_TeamMeber_Classes::em_TeamMeber_Classes_BladeMaster },
		{ L"�̿�",em_TeamMeber_Classes::em_TeamMeber_Classes_Assassin },
		{ L"��ʿ",em_TeamMeber_Classes::em_TeamMeber_Classes_Destroyer },
	};

	_MapClassesText = Vec;
}

em_TeamMeber_Classes CGamePlayerExtend::FindPlayerClasses(_In_ CONST std::wstring& wsClassesText) CONST
{
	auto itr = _MapClassesText.find(wsClassesText);
	if (itr == _MapClassesText.end())
	{
		LOG_MSG_CF(L"������ְҵ[%s] in _MapClassesText", wsClassesText.c_str());
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
