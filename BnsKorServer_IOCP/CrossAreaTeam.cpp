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

	// 设置心跳
	Member.ulStartTick = Member.ulTick = ::GetTickCount64();

	// 如果存在 在队伍组里面, 就返回TeamId给客户端
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


	// 开始调度组队
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
	// 必须先删除TeamMemberName, 因为删除了下面的这个, 整个引用就无效了
	RemoveTeamMemberName(wsAccountName);
	RemoveTeamMember(wsAccountName);
}

VOID CCrossAreaTeam::Print()
{
	std::lock_guard<std::mutex> Lck(_MtxCrossAreaTeam);

	LOG_CF_D(L"----------副本列表----------");
	for (CONST auto& itm : _SetDungeonName)
		LOG_CF_D(L"%s", itm.c_str());

	LOG_CF_D(L"--------等待组队帐号列表---------");
	for (CONST auto& itm : _SetTeamMemberName)
		LOG_CF_D(L"%s", itm.c_str());

	
	for (CONST auto& itm : _MapTeamMember)
	{
		LOG_CF_D(L"--------过滤标题[%s]下的队员----------", itm.first.c_str());
		for (CONST auto& Member : itm.second)
			PrintCrossAreaTeamMember(Member.second);
	}

	for (CONST auto& itm : _MapCrossAreaTeamGroup)
	{
		LOG_CF_D(L"队伍ID=[%X], 副本[%s]:",itm.first,itm.second.wsDungeonName.c_str());
		PrintCrossAreaTeamGroup(itm.second);
	}
}

VOID CCrossAreaTeam::Initialize()
{
	CONST static std::vector<DungeonClassesText> VecText =
	{
		// 注意, 所有非随机的职业, 优先级最好是排到前面, 有固定职业的排在前面
		{ L"简单的黑龙教地下监狱",0, 6,{ L"刺客",L"刺客" ,L"?" ,L"?" ,L"?" ,L"?" } },
		{ L"困难的黑龙教地下监狱",0, 6,{ L"刺客",L"刺客" ,L"?" ,L"?" ,L"?" ,L"?" } },
		{ L"简单的冰库",0,6, {L"?",L"?" ,L"?" ,L"?" ,L"?" ,L"?" }},
		{ L"简单的沉默的海盗船",0,6,{L"?",L"?" ,L"?" ,L"?" ,L"?" ,L"?" }},
		{ L"简单的鬼门关",0,6,{L"?",L"?" ,L"?" ,L"?" ,L"?" ,L"?" }},
		{ L"简单的黑森林",0,6,{L"?",L"?" ,L"?" ,L"?" ,L"?" ,L"?" }},
		{ L"简单的雪人洞",0,6,{L"?",L"?" ,L"?" ,L"?" ,L"?" ,L"?" }},
		{ L"简单的雪玉宫",0,6,{L"?",L"?" ,L"?" ,L"?" ,L"?" ,L"?" } },
	};

	_MapDungeonClasses.clear();
	for (CONST auto& itm : VecText)
	{
		if (itm.VecClasses.size() > itm.uMaxTeamMember)
		{
			LOG_MSG_CF(L"副本[%s]的最大队伍数量=%d,你现在设置了%d个职业", itm.wsDungeonName.c_str(), itm.uMaxTeamMember, itm.VecClasses.size());
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
		// 更新心跳
		MemberItr->second.ulTick = Member.ulTick;
		// 更新ID, 主要是怕游戏掉线上来后ID会变化
		MemberItr->second.Info = Member.Info;
	}
}

UINT CCrossAreaTeam::FindSameDungoen(_In_ CONST std::wstring& wsDungeonName, _In_ VecCrossAreaTeamMember& MapMember, _Out_ std::vector<CrossAreaTeamMember*>& VecItr)
{
	for (auto itr = MapMember.begin(); itr != MapMember.end();)
	{
		// 去除心跳超时的帐号
		if (!IsKeepALive(itr->second))
		{
			CON_LOG_CF_D(_bPrint, L"帐号[%s:%s] 心跳超时! 删除该帐号", itr->second.Info.wsAccountName.c_str(), itr->second.Info.wsPlayerName.c_str());
			
			// 删除 SetName
			RemoveTeamMemberName(itr->second.Info.wsAccountName);
			itr = MapMember.erase(itr);
			continue;
		} 

		// 判断该帐号 是否有这个副本
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
		// 获取相同Title的所有帐号
		auto& VecMember = VecMemberPair.second;
		for (auto& wsDungeonName : _SetDungeonName)
		{
			// 获取该副本下的职业限定列表
			auto ClassesItr = _MapDungeonClasses.find(wsDungeonName);
			if (ClassesItr == _MapDungeonClasses.end())
			{
				LOG_CF_E(L"UnExist Dungeon[%s] in Setting!!! Undefine~", wsDungeonName.c_str());
				continue;
			}

			CONST auto& VecClasses = ClassesItr->second;

			std::vector<CrossAreaTeamMember*> VecSameDungoenMember;
			// 寻找相同的副本+相同的Title的队友
			if (FindSameDungoen(wsDungeonName, VecMember, VecSameDungoenMember) >= VecClasses.size())
			{
				// 开始调度相同副本+相同Title的是否能组一波队伍( 第一个过滤条件=相同的副本+相同的GroupTitle )
				// 一个副本的模式必定是一样的(刷雪人不可能用固定职业模式, 刷黑龙不可能用随机职业模式)
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
			// 设定的职业找不到就直接break!
			return FALSE;
		}

		VecMemberClasses.push_back(*itr);
		VecMember.erase(itr);
	}

	// 将他们组成一个队伍
	AddtoCrossAreaTeamGroup(wsDungeonName, VecMemberClasses);

	// 删除
	for (auto& itm : VecMemberClasses)
	{
		// 从等待的队伍中删除
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
	// 递归组队
	//return VecItr.size() >= 6 ? SchedulingSameDungeon(wsDungeonName, MapMember, VecItr, VecClasses) : TRUE;
}

VOID CCrossAreaTeam::AddtoCrossAreaTeamGroup(_In_ CONST std::wstring& wsDungeonName, _In_ std::vector<CrossAreaTeamMember*>& VecMember)
{
	static DWORD dwTeamId = 0;

	CrossAreaTeamGroup Teamgroup;
	Teamgroup.dwTeamId = ++dwTeamId;
	Teamgroup.wsDungeonName = wsDungeonName;
	Teamgroup.Captain = *VecMember.at(0);
	CON_LOG_CF_D(_bPrint, L"新建跨服副本队伍[%s],ID=%X,队长[%s:%s]", wsDungeonName.c_str(), Teamgroup.dwTeamId, Teamgroup.Captain.Info.wsAccountName.c_str(), Teamgroup.Captain.Info.wsPlayerName.c_str());

	for (auto itr = VecMember.begin() + 1; itr != VecMember.end(); ++itr)
	{
		CON_LOG_CF_D(_bPrint, L"队员:[%s:%s]", (*itr)->Info.wsAccountName.c_str(), (*itr)->Info.wsPlayerName.c_str());
		Teamgroup.VecMember.push_back(**itr);
	}

	_MapCrossAreaTeamGroup.insert(std::make_pair(Teamgroup.dwTeamId, std::move(Teamgroup)));
}

VOID CCrossAreaTeam::PrintCrossAreaTeamMember(_In_ CONST CrossAreaTeamMember& Member) CONST
{
	std::wstring wsText;
	for (CONST auto& itm : Member.SetDungoenName)
		wsText += itm + L",";

	LOG_CF_D(L"[%s:%s] 过滤标题=[%s], 职业=[%s], 副本=[%s], 上次心跳在[%d]秒前, 已经排队了[%d]秒",
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
	LOG_CF_D(L"队长:");
	PrintCrossAreaTeamMember(Teamgroup.Captain);
	
	for (CONST auto& itm : Teamgroup.VecMember)
	{
		LOG_CF_D(L"队员:");
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

