#include "CrossAreaTeam.h"
#include <MyTools/Log.h>
#include <MyTools/CLPublic.h>
#include "GamePlayerExtend.h"

#define _SELF L"CrossAreaTeam.cpp"

CCrossAreaTeam::CCrossAreaTeam()
{
	_bPrint = false;
}

BOOL CCrossAreaTeam::JoinTeam(_In_ CrossAreaTeamMember& Member, _Out_ DWORD& dwTeamId)
{
	std::lock_guard<std::mutex> Lck(_MtxCrossAreaTeam);

	// ��������
	Member.ulStartTick = Member.ulTick = ::GetTickCount64();

	// ������� �ڶ���������, �ͷ���TeamId���ͻ���
	auto pCrossAreaTeamGroup = ExistCrossAreaTeamGroup(Member);
	if (pCrossAreaTeamGroup != nullptr)
	{
		dwTeamId = pCrossAreaTeamGroup->dwTeamId;
		return TRUE;
	}
	
	// 
	if (!ExistCrossAreaTeamMember(Member))
		AddToCrossAreaTeamMember(Member);
	else
		UpdateTickInTeamMember(Member);


	// ��ʼ�������
	Scheduling();
	return FALSE;
}


BOOL CCrossAreaTeam::GetTeamMemberContent(_In_ DWORD dwTeamId, _Out_ CrossAreaTeamGroup& Result)
{
	std::lock_guard<std::mutex> Lck(_MtxCrossAreaTeam);
	auto itr = _MapCrossAreaTeamGroup.find(dwTeamId);
	if (itr == _MapCrossAreaTeamGroup.end())
		return FALSE;

	Result = itr->second;
	return TRUE;
}

VOID CCrossAreaTeam::DeleteTeamGroup(_In_ CONST std::wstring& wsAccountName)
{
	std::lock_guard<std::mutex> Lck(_MtxCrossAreaTeam);
	for (auto itr = _MapCrossAreaTeamGroup.begin(); itr != _MapCrossAreaTeamGroup.end(); ++itr)
	{
		if (ExistAccountNameInTeamGroup(wsAccountName, itr->second))
		{
			_MapCrossAreaTeamGroup.erase(itr);
			break;
		}
	}
}

VOID CCrossAreaTeam::DeleteTeamMember(_In_ CONST std::wstring& wsAccountName)
{
	std::lock_guard<std::mutex> Lck(_MtxCrossAreaTeam);
	// ������ɾ��TeamMemberName, ��Ϊɾ������������, �������þ���Ч��
	RemoveTeamMemberName(wsAccountName);
	RemoveTeamMember(wsAccountName);
}

VOID CCrossAreaTeam::Print()
{
	std::lock_guard<std::mutex> Lck(_MtxCrossAreaTeam);

	LOG_CF_D(L"----------�����б�----------");
	for (CONST auto& itm : _SetDungeonName)
		LOG_CF_D(L"%s", itm.c_str());

	LOG_CF_D(L"--------�ȴ�����ʺ��б�---------");
	for (CONST auto& itm : _SetTeamMemberName)
		LOG_CF_D(L"%s", itm.c_str());

	
	for (CONST auto& itm : _MapTeamMember)
	{
		LOG_CF_D(L"--------���˱���[%s]�µĶ�Ա----------", itm.first.c_str());
		for (CONST auto& Member : itm.second)
			PrintCrossAreaTeamMember(Member.second);
	}

	for (CONST auto& itm : _MapCrossAreaTeamGroup)
	{
		LOG_CF_D(L"����ID=[%X], ����[%s]:",itm.first,itm.second.wsDungeonName.c_str());
		PrintCrossAreaTeamGroup(itm.second);
	}
}

VOID CCrossAreaTeam::Initialize()
{
	CONST static std::vector<DungeonClassesText> VecText =
	{
		// ע��, ���з������ְҵ, ���ȼ�������ŵ�ǰ��, �й̶�ְҵ������ǰ��
		{ L"�򵥵ĺ����̵��¼���",0, 6,{ L"�̿�",L"�̿�" ,L"?" ,L"?" ,L"?" ,L"?" } },
		{ L"���ѵĺ����̵��¼���",0, 6,{ L"�̿�",L"�̿�" ,L"?" ,L"?" ,L"?" ,L"?" } },
		{ L"�򵥵ı���",0,6, {L"?",L"?" ,L"?" ,L"?" ,L"?" ,L"?" }},
		{ L"�򵥵ĳ�Ĭ�ĺ�����",0,6,{L"?",L"?" ,L"?" ,L"?" ,L"?" ,L"?" }},
		{ L"�򵥵Ĺ��Ź�",0,6,{L"?",L"?" ,L"?" ,L"?" ,L"?" ,L"?" }},
		{ L"�򵥵ĺ�ɭ��",0,6,{L"?",L"?" ,L"?" ,L"?" ,L"?" ,L"?" }},
		{ L"�򵥵�ѩ�˶�",0,6,{L"?",L"?" ,L"?" ,L"?" ,L"?" ,L"?" }},
		{ L"�򵥵�ѩ��",0,6,{L"?",L"?" ,L"?" ,L"?" ,L"?" ,L"?" } },
	};

	_MapDungeonClasses.clear();
	for (CONST auto& itm : VecText)
	{
		if (itm.VecClasses.size() > itm.uMaxTeamMember)
		{
			LOG_MSG_CF(L"����[%s]������������=%d,������������%d��ְҵ", itm.wsDungeonName.c_str(), itm.uMaxTeamMember, itm.VecClasses.size());
			::Sleep(INFINITE);
		}

		std::vector<em_TeamMeber_Classes> VecClasses;
		for (CONST auto& ClassesText : itm.VecClasses)
			VecClasses.push_back(CGamePlayerExtend::GetInstance().FindPlayerClasses(ClassesText));

		_MapDungeonClasses.insert(std::make_pair(itm.wsDungeonName, std::move(VecClasses)));
	}
}

VOID CCrossAreaTeam::Test()
{
	

	Print();
}

CONST CrossAreaTeamGroup* CCrossAreaTeam::ExistCrossAreaTeamGroup(_In_ CONST CrossAreaTeamMember& Member) CONST
{
	// 
	for (CONST auto& itm : _MapCrossAreaTeamGroup)
	{
		if (itm.second.Captain == Member)
			return &itm.second;

		if (MyTools::CLPublic::Vec_find_if_Const(itm.second.VecMember, [Member](CONST CrossAreaTeamMember& GroupMember) { return GroupMember == Member; }))
		{
			return &itm.second;
		}
	}
	return nullptr;
}


BOOL CCrossAreaTeam::ExistCrossAreaTeamMember(_In_ CONST CrossAreaTeamMember& Member) CONST
{
	return _SetTeamMemberName.find(Member.Info.wsAccountName) != _SetTeamMemberName.end();
}

VOID CCrossAreaTeam::AddToCrossAreaTeamMember(_In_ CONST CrossAreaTeamMember& Member)
{
	CON_LOG_CF_D(_bPrint, L"Player[%s:%s] UnExist CrossArea Team, Add to Wait Team", Member.Info.wsAccountName.c_str(), Member.Info.wsPlayerName.c_str());
	_SetTeamMemberName.insert(Member.Info.wsAccountName);
	for (CONST auto& itm : Member.SetDungoenName)
	{ 
		// 
		if (_SetDungeonName.find(itm) == _SetDungeonName.end())
			_SetDungeonName.insert(itm);
	}

	auto itr = _MapTeamMember.find(Member.wsGroupTitle);
	if (itr == _MapTeamMember.end())
	{
		VecCrossAreaTeamMember MapMember;
		MapMember.insert(std::make_pair(Member.Info.wsAccountName, Member));
		_MapTeamMember.insert(std::make_pair(Member.wsGroupTitle, MapMember));
	}
	else
	{
		itr->second.insert(std::make_pair(Member.Info.wsAccountName, Member));
	}
}

VOID CCrossAreaTeam::UpdateTickInTeamMember(_In_ CONST CrossAreaTeamMember& Member)
{
	auto itr = _MapTeamMember.find(Member.wsGroupTitle);
	if (itr == _MapTeamMember.end())
		return;

	auto MemberItr = itr->second.find(Member.Info.wsAccountName);
	if (MemberItr != itr->second.end())
	{
		// ��������
		MemberItr->second.ulTick = Member.ulTick;
		// ����ID, ��Ҫ������Ϸ����������ID��仯
		MemberItr->second.Info = Member.Info;
	}
}

UINT CCrossAreaTeam::FindSameDungoen(_In_ CONST std::wstring& wsDungeonName, _In_ VecCrossAreaTeamMember& MapMember, _Out_ std::vector<CrossAreaTeamMember*>& VecItr)
{
	for (auto itr = MapMember.begin(); itr != MapMember.end();)
	{
		// ȥ��������ʱ���ʺ�
		if (!IsKeepALive(itr->second))
		{
			CON_LOG_CF_D(_bPrint, L"�ʺ�[%s:%s] ������ʱ! ɾ�����ʺ�", itr->second.Info.wsAccountName.c_str(), itr->second.Info.wsPlayerName.c_str());
			
			// ɾ�� SetName
			RemoveTeamMemberName(itr->second.Info.wsAccountName);
			itr = MapMember.erase(itr);
			continue;
		} 

		// �жϸ��ʺ� �Ƿ����������
		if (itr->second.SetDungoenName.find(wsDungeonName) != itr->second.SetDungoenName.end())
		{
			VecItr.push_back(&itr->second);
		}

		++itr;
	}

	return VecItr.size();
}

VOID CCrossAreaTeam::Scheduling()
{
	for (auto& VecMemberPair : _MapTeamMember)
	{
		// ��ȡ��ͬTitle�������ʺ�
		auto& VecMember = VecMemberPair.second;
		for (auto& wsDungeonName : _SetDungeonName)
		{
			// ��ȡ�ø����µ�ְҵ�޶��б�
			auto ClassesItr = _MapDungeonClasses.find(wsDungeonName);
			if (ClassesItr == _MapDungeonClasses.end())
			{
				LOG_CF_E(L"UnExist Dungeon[%s] in Setting!!! Undefine~", wsDungeonName.c_str());
				continue;
			}

			CONST auto& VecClasses = ClassesItr->second;

			std::vector<CrossAreaTeamMember*> VecSameDungoenMember;
			// Ѱ����ͬ�ĸ���+��ͬ��Title�Ķ���
			if (FindSameDungoen(wsDungeonName, VecMember, VecSameDungoenMember) >= VecClasses.size())
			{
				// ��ʼ������ͬ����+��ͬTitle���Ƿ�����һ������( ��һ����������=��ͬ�ĸ���+��ͬ��GroupTitle )
				// һ��������ģʽ�ض���һ����(ˢѩ�˲������ù̶�ְҵģʽ, ˢ���������������ְҵģʽ)
				SchedulingSameDungeon(wsDungeonName, VecMember, VecSameDungoenMember, VecClasses);
			}

			
		}
	}
}

BOOL CCrossAreaTeam::SchedulingSameDungeon(_In_ CONST std::wstring& wsDungeonName, _In_ VecCrossAreaTeamMember& MapMember, _In_ std::vector<CrossAreaTeamMember*>& VecMember, _In_ CONST std::vector<em_TeamMeber_Classes>& VecClasses)
{
	std::vector<CrossAreaTeamMember*> VecMemberClasses;
	for (auto& Classes : VecClasses)
	{
		auto itr = std::find_if(VecMember.begin(), VecMember.end(), [Classes](CrossAreaTeamMember* pMember) { return Classes == em_TeamMeber_Classes::em_TeamMeber_Classes_Any || pMember->emClasses == Classes; });
		if (itr == VecMember.end())
		{
			// �趨��ְҵ�Ҳ�����ֱ��break!
			return FALSE;
		}

		VecMemberClasses.push_back(*itr);
		VecMember.erase(itr);
	}

	// ���������һ������
	AddtoCrossAreaTeamGroup(wsDungeonName, VecMemberClasses);

	// ɾ��
	for (auto& itm : VecMemberClasses)
	{
		// �ӵȴ��Ķ�����ɾ��
		auto itr = _SetTeamMemberName.find(itm->Info.wsAccountName);
		if (itr != _SetTeamMemberName.end())
			_SetTeamMemberName.erase(itr);
	}

	
	// 
	for (auto& itm : VecMemberClasses)
	{
		auto itr = MapMember.find(itm->Info.wsAccountName);
		if (itr != MapMember.end())
			MapMember.erase(itr);
	}

	return TRUE;
	// �ݹ����
	//return VecItr.size() >= 6 ? SchedulingSameDungeon(wsDungeonName, MapMember, VecItr, VecClasses) : TRUE;
}

VOID CCrossAreaTeam::AddtoCrossAreaTeamGroup(_In_ CONST std::wstring& wsDungeonName, _In_ std::vector<CrossAreaTeamMember*>& VecMember)
{
	static DWORD dwTeamId = 0;

	CrossAreaTeamGroup Teamgroup;
	Teamgroup.dwTeamId = ++dwTeamId;
	Teamgroup.wsDungeonName = wsDungeonName;
	Teamgroup.Captain = *VecMember.at(0);
	CON_LOG_CF_D(_bPrint, L"�½������������[%s],ID=%X,�ӳ�[%s:%s]", wsDungeonName.c_str(), Teamgroup.dwTeamId, Teamgroup.Captain.Info.wsAccountName.c_str(), Teamgroup.Captain.Info.wsPlayerName.c_str());

	for (auto itr = VecMember.begin() + 1; itr != VecMember.end(); ++itr)
	{
		CON_LOG_CF_D(_bPrint, L"��Ա:[%s:%s]", (*itr)->Info.wsAccountName.c_str(), (*itr)->Info.wsPlayerName.c_str());
		Teamgroup.VecMember.push_back(**itr);
	}

	_MapCrossAreaTeamGroup.insert(std::make_pair(Teamgroup.dwTeamId, std::move(Teamgroup)));
}

VOID CCrossAreaTeam::PrintCrossAreaTeamMember(_In_ CONST CrossAreaTeamMember& Member) CONST
{
	std::wstring wsText;
	for (CONST auto& itm : Member.SetDungoenName)
		wsText += itm + L",";

	LOG_CF_D(L"[%s:%s] ���˱���=[%s], ְҵ=[%s], ����=[%s], �ϴ�������[%d]��ǰ, �Ѿ��Ŷ���[%d]��",
		Member.Info.wsAccountName.c_str(),
		Member.Info.wsPlayerName.c_str(),
		Member.wsGroupTitle.c_str(),
		CGamePlayerExtend::GetInstance().FindPlayerClasses(Member.emClasses).c_str(),
		wsText.c_str(),
		(::GetTickCount64() - Member.ulTick) / 1000,
		(::GetTickCount64() - Member.ulStartTick) / 1000);

}

VOID CCrossAreaTeam::PrintCrossAreaTeamGroup(_In_ CONST CrossAreaTeamGroup& Teamgroup) CONST
{
	LOG_CF_D(L"�ӳ�:");
	PrintCrossAreaTeamMember(Teamgroup.Captain);
	
	for (CONST auto& itm : Teamgroup.VecMember)
	{
		LOG_CF_D(L"��Ա:");
		PrintCrossAreaTeamMember(itm);
	}
}

BOOL CCrossAreaTeam::ExistAccountNameInTeamGroup(_In_ CONST std::wstring& wsAccountName, _In_ CONST CrossAreaTeamGroup& Teamgroup)
{
	if (Teamgroup.Captain.Info.wsAccountName == wsAccountName)
		return TRUE;


	for (CONST auto& itm : Teamgroup.VecMember)
	{
		if (itm.Info.wsAccountName == wsAccountName)
			return TRUE;
	}

	return FALSE;
}

BOOL CCrossAreaTeam::IsKeepALive(_In_ CONST CrossAreaTeamMember& Member) CONST
{
	return ::GetTickCount64() - Member.ulTick <= 30 * 1000;
}

VOID CCrossAreaTeam::RemoveTeamMember(_In_ CONST std::wstring& wsAccountName)
{
	for (auto& itm : _MapTeamMember)
	{
		auto itr = itm.second.find(wsAccountName);
		if (itr != itm.second.end())
		{
			itm.second.erase(itr);
			break;
		}
	}
}

VOID CCrossAreaTeam::RemoveTeamMemberName(_In_ CONST std::wstring& wsAccountName)
{
	auto Nameitr = _SetTeamMemberName.find(wsAccountName);
	if (Nameitr != _SetTeamMemberName.end())
		_SetTeamMemberName.erase(Nameitr);
}

