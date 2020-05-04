#include "BnsTeam.h"
#include <MyTools/CLPublic.h>
#include <MyTools/Log.h>
#include <MyTools/CLEchoException.h>

#define _SELF L"BnsTeam.cpp"
CBnsTaskTeam::CBnsTaskTeam() : Lock_BnsTeam(L"CBnsTaskTeam_Lock"), Lock_DungoenTeam(L"Lock_DungoenTeam")
{
	m_dwTeamId = 0;
}

BOOL CBnsTaskTeam::ExistTeamMember(_In_ std::map<std::wstring, TeamGroup>& map_TeamGroup, _In_ CONST ClientTeamInfo& CtiMember, _In_ std::function<VOID(TeamMemberGroup&, TeamMemberInfo&)> Worker)
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&] 
	{
		std::wstring wsMapText;
		CreateMapText(CtiMember, wsMapText);
		auto itr = map_TeamGroup.find(wsMapText);
		if (itr == map_TeamGroup.end())
		{
			// UnExist Team Such as Conddtion!
			CreateTeamGroup(wsMapText, CtiMember, map_TeamGroup);
			return ExistTeamMember(map_TeamGroup, CtiMember, Worker);
		}

		return ExistMember_By_AccountName_In_TeamMemberGroup(CtiMember.SelfCti.wsAccountName, itr->second, Worker);
	});
}

BOOL CBnsTaskTeam::ExistTeamMember(_In_ std::map<std::wstring, TeamGroup>& map_TeamGroup, _In_ DWORD dwTeamId, _In_ std::function<VOID(TeamMemberGroup&)> Worker)
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		for (auto& itm : map_TeamGroup)
		{
			if (ExistMember_By_TeamId_In_TeamMemberGroup(dwTeamId, itm.second, Worker))
				return TRUE;
		}
		return FALSE;
	});
}

VOID CBnsTaskTeam::CreateMapText(_In_ CONST ClientTeamInfo& CtiMember, _Out_ std::wstring& wsText)
{
	MyTools::CLEchoException::GetInstance().InvokeAction(__FUNCTIONW__, [&]
	{
		wsText = MyTools::CCharacter::MakeFormatText(L"%X,[%X,%X],%d", CtiMember.dwServerId, CtiMember.dwTaskId, CtiMember.dwScheduleId, CtiMember.dwChannelId);
	});
}

VOID CBnsTaskTeam::CreateTeamGroup(_In_ CONST std::wstring& wsMapText, _In_ CONST ClientTeamInfo& CtiMember, _In_ std::map<std::wstring, TeamGroup>& map_TeamGroup)
{
	MyTools::CLEchoException::GetInstance().InvokeAction(__FUNCTIONW__, [&]
	{
		TeamGroup NewTeamGroup_;
		TeamMemberGroup NewTeamMemberGroup_;
		TeamMemberInfo NewTeamMemberInfo_;

		// Write Team Infomation
		NewTeamMemberGroup_.dwChannelId = CtiMember.dwChannelId;
		NewTeamMemberGroup_.dwScheduleId = CtiMember.dwScheduleId;
		NewTeamMemberGroup_.dwTaskId = CtiMember.dwTaskId;
		NewTeamMemberGroup_.dwServerId = CtiMember.dwServerId;
		NewTeamMemberGroup_.uMaxMemberCount = CtiMember.dwMaxMember;
		NewTeamMemberGroup_.dwTeamId = ++m_dwTeamId;

		// Write Captain Message(Creator was Captain, because UnExist Such as Condition)
		NewTeamMemberInfo_.bCaptain = TRUE;
		NewTeamMemberInfo_.Member = CtiMember.SelfCti;

		// Put Self Message to Team Message
		NewTeamMemberGroup_.MemberVec.push_back(NewTeamMemberInfo_);

		// Put Team Message to Group Team Message
		NewTeamGroup_.push_back(NewTeamMemberGroup_);
		map_TeamGroup.insert(std::map<std::wstring, TeamGroup>::value_type(wsMapText, NewTeamGroup_));
	});
}

BOOL CBnsTaskTeam::ExistMember_By_AccountName_In_TeamMemberGroup(_In_ CONST std::wstring& wsAccountName, _In_ TeamGroup& TeamGroup_, _In_ std::function<VOID(TeamMemberGroup&, TeamMemberInfo&)> Worker) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		for (auto& itm : TeamGroup_)
		{
			auto p = MyTools::CLPublic::Deque_find_if(itm.MemberVec, [&wsAccountName](TeamMemberInfo& Member) { return Member.Member.wsAccountName == wsAccountName; });
			if (p != nullptr)
			{
				Worker(itm, *p);
				return TRUE;
			}
		}
		return FALSE;
	});
}

BOOL CBnsTaskTeam::ExistMember_By_TeamId_In_TeamMemberGroup(_In_ DWORD dwTeamId, _In_ TeamGroup& TeamGroup_, _In_ std::function<VOID(TeamMemberGroup&)> Worker) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		auto p = MyTools::CLPublic::Deque_find_if(TeamGroup_, [&dwTeamId](TeamMemberGroup& itm) {return itm.dwTeamId == dwTeamId; });
		if (p != nullptr)
		{
			Worker(*p);
			return TRUE;
		}
		return FALSE;
	});
}

BOOL CBnsTaskTeam::FindCaptain_By_AccountName_In_TeamMemberGroup(_In_ TeamMemberGroup& TeamMemberGroup_, _In_ std::function<VOID(TeamMemberGroup&, TeamMemberInfo&)> Worker)
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		auto p = MyTools::CLPublic::Deque_find_if(TeamMemberGroup_.MemberVec, [](CONST TeamMemberInfo& TeamMemberInfo_) { return TeamMemberInfo_.bCaptain; });
		if (p != nullptr)
		{
			Worker(TeamMemberGroup_, *p);
			return TRUE;
		}
		return FALSE;
	});
}

BOOL CBnsTaskTeam::IsWork(_In_ CONST ClientTeamInfo& CtiMember) CONST
{
	return CtiMember.SelfCti.wsPlayerName != L"" && CtiMember.SelfCti.dwPlayerId != NULL;
}

BOOL CBnsTaskTeam::JoinTaskTeam(_In_ CONST ClientTeamInfo& CtiMember, _Out_ BnsTeamResult& Result)
{
	if (!IsWork(CtiMember))
	{
		LOG_CF_E(L"Client发送的队伍信息有误![%s:%X]", CtiMember.SelfCti.wsPlayerName.c_str(), CtiMember.SelfCti.dwPlayerId);
		return FALSE;
	}

	BOOL bRetCode = FALSE;
	Lock_BnsTeam.Access([&] {bRetCode = JoinTeam(CtiMember, TaskTeamMap, Result); });
	return bRetCode;
}


BOOL CBnsTaskTeam::JoinDungoenTeam(_In_ CONST ClientTeamInfo& CtiMember, _Out_ BnsTeamResult& Result)
{
	if (!IsWork(CtiMember))
	{
		LOG_CF_E(L"Client发送的队伍信息有误![%s,%X]", CtiMember.SelfCti.wsPlayerName.c_str(), CtiMember.SelfCti.dwPlayerId);
		return FALSE;
	}

	BOOL bRetCode = FALSE;
	Lock_DungoenTeam.Access([&] {bRetCode = JoinTeam(CtiMember, DungoenTeamMap, Result); });
	return bRetCode;
}


BOOL CBnsTaskTeam::FindVacantPosition_In_TeamMemberGroup(_In_ CONST ClientTeamInfo& CtiMember, _In_ std::map<std::wstring, TeamGroup>& map_TeamGroup, _In_ std::function<VOID(TeamMemberGroup&, TeamMemberInfo&)> Worker)
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		std::wstring wsMapText;
		CreateMapText(CtiMember, wsMapText);
		auto itr = map_TeamGroup.find(wsMapText);
		if (itr == map_TeamGroup.end())
		{
			LOG_CF_E(L"map.find失败!, wsMapText=%s", wsMapText.c_str());
			return FALSE;
		}

		auto pTeamMemberGroup = MyTools::CLPublic::Deque_find_if(itr->second, [CtiMember](TeamMemberGroup& itm)
		{
			// 当队伍标题!=""的时候, 需要新来的队员的队伍标题一致才可以加入!
			if (itm.wsGroupTitle != L"" && itm.wsGroupTitle != CtiMember.wsGroupTitle)
				return false;

			return itm.MemberVec.size() < itm.uMaxMemberCount;
		});
		if (pTeamMemberGroup != nullptr && !pTeamMemberGroup->IsReadOnly)
		{
			// 加入还有空位的队伍
			TeamMemberInfo Member;
			Member.bCaptain = FALSE;
			Member.Member = CtiMember.SelfCti;
			pTeamMemberGroup->MemberVec.push_back(Member);
			return FindCaptain_By_AccountName_In_TeamMemberGroup(*pTeamMemberGroup, Worker);
		}

		// 创建一支队伍!
		for (auto& itm : itr->second)
		{
			if (itm.MemberVec.size() < itm.uMaxMemberCount && !itm.IsReadOnly)
			{
				// Fill Self to this Team!
				// 加入
				TeamMemberInfo Member;
				Member.bCaptain = FALSE;
				Member.Member = CtiMember.SelfCti;
				itm.wsGroupTitle = CtiMember.wsGroupTitle;
				itm.MemberVec.push_back(Member);
				//LOG_CF_D( L"加入还有空位的队伍, 当前是[%s,%X], 队伍的成员:", CtiMember.SelfCti.wsPlayerName.c_str(), CtiMember.SelfCti.dwPlayerId);
				//for (CONST auto& TeamMemberInfo_ : itm.MemberVec)
				//	LOG_CF_D( L"[%s,%X, Captain:%X]", TeamMemberInfo_.Member.wsPlayerName.c_str(), TeamMemberInfo_.Member.dwPlayerId, TeamMemberInfo_.bCaptain);

				return FindCaptain_By_AccountName_In_TeamMemberGroup(itm, Worker);
			}
		}

		return CreateTeamMemberGroup(itr->second, CtiMember, Worker);
	});
}

BOOL CBnsTaskTeam::CreateTeamMemberGroup(_In_ TeamGroup& TeamGroup_, _In_ CONST ClientTeamInfo& CtiMember, _In_ std::function<VOID(TeamMemberGroup&, TeamMemberInfo&)> Worker)
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		TeamMemberGroup NewTeamMemberGroup_;

		// Fill Data!
		NewTeamMemberGroup_.dwChannelId = CtiMember.dwChannelId;
		NewTeamMemberGroup_.dwScheduleId = CtiMember.dwScheduleId;
		NewTeamMemberGroup_.dwTaskId = CtiMember.dwTaskId;
		NewTeamMemberGroup_.dwServerId = CtiMember.dwServerId;
		NewTeamMemberGroup_.uMaxMemberCount = CtiMember.dwMaxMember;
		NewTeamMemberGroup_.dwTeamId = ++m_dwTeamId;

		TeamMemberInfo Member;
		Member.bCaptain = TRUE;
		Member.Member = CtiMember.SelfCti;
		NewTeamMemberGroup_.MemberVec.push_back(Member);

		// Add to TeamGroup
		//LOG_CF_D( L"队长创建队伍:[%s,%X], MaxCount=%d", CtiMember.SelfCti.wsPlayerName.c_str(), CtiMember.SelfCti.dwPlayerId, TeamGroup_.size());
		TeamGroup_.push_back(NewTeamMemberGroup_);

		// *TeamGroup_.back().MemberVec.begin() = 队长
		auto& TeamMemberGroup_ = TeamGroup_.back();
		Worker(TeamMemberGroup_, *TeamMemberGroup_.MemberVec.begin());
		return TRUE;
	});
}

BOOL CBnsTaskTeam::DeleteTeamMember(_In_ CONST std::wstring& wsAccountName, _In_ std::map<std::wstring, TeamGroup>& map_TeamGroup)
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		for (auto itr = map_TeamGroup.begin(); itr != map_TeamGroup.end();)
		{
			auto& itm = itr->second;
			ExistMember_By_AccountName_In_TeamMemberGroup(wsAccountName, itm, [&itm, this](TeamMemberGroup& TeamMemberGroup_, TeamMemberInfo&)
			{
				DismissTeamMemberGroup(itm, TeamMemberGroup_);
			});

			itr = itr->second.size() == 0 ? map_TeamGroup.erase(itr) : ++itr;
		}
		return TRUE;
	});
}

BOOL CBnsTaskTeam::DismissTeamMemberGroup(_In_ TeamGroup& TeamGroup_, _In_ TeamMemberGroup& TeamMemberGroup_)
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		DWORD dwTeamId = TeamMemberGroup_.dwTeamId;
		return MyTools::CLPublic::Dque_erase_if(TeamGroup_, [&dwTeamId](TeamMemberGroup& TeamMemberGroup_) {return TeamMemberGroup_.dwTeamId == dwTeamId; });
	});
}

BOOL CBnsTaskTeam::JoinTeam(_In_ CONST ClientTeamInfo& CtiMember, _In_ std::map<std::wstring, TeamGroup>& map_TeamGroup, _Out_ BnsTeamResult& Result)
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		auto bExist = ExistTeamMember(map_TeamGroup, CtiMember, [this, &Result, &CtiMember](TeamMemberGroup& TeamMemberGroup_, TeamMemberInfo& TeamSelf)
		{
			TeamSelf.Member.dwPlayerId = CtiMember.SelfCti.dwPlayerId;
			TeamSelf.Member.wsPlayerName = CtiMember.SelfCti.wsPlayerName;
			auto bExistCaptain = FindCaptain_By_AccountName_In_TeamMemberGroup(TeamMemberGroup_, [&Result, &TeamSelf, &TeamMemberGroup_](TeamMemberGroup&, TeamMemberInfo& Captain)
			{
				// 返回此队伍的队长的Name和ID
				Result.SetCaptain(Captain.Member.wsPlayerName, Captain.Member.dwPlayerId);
				if (Captain.Member.wsAccountName != TeamSelf.Member.wsAccountName)
				{
					// 自己不是队长的时候, 只返回队长, 不需要返回队员, 浪费流量!
					// TeamMember Only Return Captain!
					Result.SetTeamId(TeamMemberGroup_.dwTeamId);
				}
				else
				{
					// 当自己是队长的时候, 返回队伍的所有成员
					//ServerBuffer << (TeamMemberGroup_.MemberVec.size() - 1); // 必须-1. 因为队长已经返回了 , 但是MemberVec里面还有队长
					for (auto& itm : TeamMemberGroup_.MemberVec)
					{
						if (itm.bCaptain)
							continue;

						Result.AddMember(itm.Member.wsPlayerName, itm.Member.dwPlayerId);
					}
					Result.SetTeamId(TeamMemberGroup_.dwTeamId);
				}
			});

			if (!bExistCaptain)
			{
				// 不存在队长? 那可能是队长也跑路了! 
				LOG_CF_D(L"找不到队长, 队长也跑路了! 解散这个队伍!");
				DeleteTeamMember(CtiMember.SelfCti.wsAccountName, DungoenTeamMap);
			}
		});

		// 不存在任何一只队伍, 只能创建一只队伍 || 寻找相同任务,并且有空位的队伍,加入
		if (!bExist)
		{
			auto bExistTeam = FindVacantPosition_In_TeamMemberGroup(CtiMember, map_TeamGroup, [&Result](TeamMemberGroup& TeamMemberGroup_, TeamMemberInfo& Captain)
			{
				// 返回队长, 如果是自己创建的队伍就返回自己, 如果是别人创建的, 那么这里的Captain=别人
				Result.SetCaptain(Captain.Member.wsPlayerName, Captain.Member.dwPlayerId);
				Result.SetTeamId(TeamMemberGroup_.dwTeamId);
			});

			if (!bExistTeam)
			{
				LOG_CF_E(L"创建队伍失败……怎么可能!!!");
				FindVacantPosition_In_TeamMemberGroup(CtiMember, map_TeamGroup, [&Result](TeamMemberGroup& TeamMemberGroup_, TeamMemberInfo& Captain)
				{
					Result.SetCaptain(Captain.Member.wsPlayerName, Captain.Member.dwPlayerId);
					Result.SetTeamId(TeamMemberGroup_.dwTeamId);
				});
			}
		}

		return TRUE;
	});
}

BOOL CBnsTaskTeam::DeleteDungoenTeamMember(_In_ CONST std::wstring& wsPlayerName)
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		return Lock_DungoenTeam.Access([&wsPlayerName, this]
		{
			for (auto& itm : DungoenTeamMap)
			{
				ExistMember_By_AccountName_In_TeamMemberGroup(wsPlayerName, itm.second, [&itm, this, wsPlayerName](TeamMemberGroup& TeamMemberGroup_, TeamMemberInfo& Tmi) {

					// 队伍就剩下一个人了, 直接解散掉!
					if (TeamMemberGroup_.MemberVec.size() <= 1)
					{
						LOG_CF_D(L"Server解散队:[%s,%s]", Tmi.Member.wsAccountName.c_str(), Tmi.Member.wsPlayerName.c_str());
						DismissTeamMemberGroup(itm.second, TeamMemberGroup_);
					}
					else // 否则只删除自己!
					{
						MyTools::CLPublic::Deque_find_if(TeamMemberGroup_.MemberVec, [wsPlayerName](CONST TeamMemberInfo& Ti) { return Ti.Member.wsPlayerName == wsPlayerName; });
						LOG_CF_D(L"Server离队:[%s,%s], 剩余%d个人", Tmi.Member.wsAccountName.c_str(), Tmi.Member.wsPlayerName.c_str(), TeamMemberGroup_.MemberVec.size());
					}
				});
			}
		});
	});
}

BOOL CBnsTaskTeam::DeleteTaskMember(_In_ CONST std::wstring& wsAccountName)
{
	return Lock_BnsTeam.Access([&wsAccountName, this] { DeleteTeamMember(wsAccountName, TaskTeamMap); });
}

BOOL CBnsTaskTeam::GetLessMemberChannel(_In_ CONST TeamChannelMsg& TeamChannelMsg_, _Out_ DWORD& dwChannel)
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		std::map<DWORD, DWORD> ChannelMap;
		GetSameChannelMap(TeamChannelMsg_, ChannelMap);

		// Find Empty Channel
		for (DWORD i = 1; i <= TeamChannelMsg_.dwMaxChannel; ++i)
		{
			if (ChannelMap.find(i) == ChannelMap.end())
			{
				dwChannel = i;
				return TRUE;
			}
		}

		// sort by less
		std::vector<std::pair<DWORD, DWORD>> ChannelVec(ChannelMap.begin(), ChannelMap.end());
		sort(ChannelVec.begin(), ChannelVec.end(), [](CONST std::pair<DWORD, DWORD>& p1, CONST std::pair<DWORD, DWORD>& p2) {return p1.second < p2.second; });


		dwChannel = (ChannelVec.size() == 0 ? 1 : ChannelVec.begin()->second);
		return TRUE;
	});
}

BOOL CBnsTaskTeam::GetSameChannelMap(_In_ CONST TeamChannelMsg& TeamChannelMsg_, _Out_opt_ std::map<DWORD, DWORD>& ChannelMap)
{
	// Get all of same channel in [TaskTeamMap] && calc same channel count
	return Lock_BnsTeam.Access([this, &TeamChannelMsg_, &ChannelMap] {
		for (CONST auto& itm : TaskTeamMap)
		{
			for (CONST auto& TeamMemberGroup_ : itm.second)
			{
				// same ServerId && dwTaskId && dwSecheduleId
				if (TeamMemberGroup_.dwServerId != TeamChannelMsg_.dwServerId)
					continue;
				else if (TeamMemberGroup_.dwTaskId != TeamChannelMsg_.dwTaskId)
					continue;
				else if (TeamMemberGroup_.dwScheduleId != TeamChannelMsg_.dwScheduleId)
					continue;

				auto itr = ChannelMap.find(TeamMemberGroup_.dwChannelId);
				if (itr == ChannelMap.end())
					ChannelMap.insert(std::make_pair(TeamMemberGroup_.dwChannelId, 1));
				else
					itr->second += 1;
			}
		}
	});
}

BOOL CBnsTaskTeam::UpdateDungoenSchedule(_In_ CONST TeamScheduleMsg& TeamScheduleMsg_)
{
	return Lock_DungoenTeam.Access([this, &TeamScheduleMsg_]
	{
		ExistTeamMember(DungoenTeamMap, TeamScheduleMsg_.dwTeamId, [this, &TeamScheduleMsg_](TeamMemberGroup& TeamMemberGroup_)
		{
			ActionInTeamGroupMemberByAccountName(TeamScheduleMsg_.wsAccountName, TeamMemberGroup_, [&TeamScheduleMsg_](TeamMemberInfo& itm)
			{
				itm.Dungoen.wsDungoenSchedule = TeamScheduleMsg_.wsScheduleName;
				itm.Dungoen.uGatherIndex = TeamScheduleMsg_.uScheduleIndex;
			});
		});
	});
}

BOOL CBnsTaskTeam::ActionInTeamGroupMemberByAccountName(_In_ CONST std::wstring& wsAccountName, _In_ TeamMemberGroup& TeamMemberGroup_, _In_ std::function<VOID(TeamMemberInfo&)> Worker) CONST
{
	return MyTools::CLPublic::Deque_find_if(TeamMemberGroup_.MemberVec, [&wsAccountName, &Worker](TeamMemberInfo& itm)
	{
		if (itm.Member.wsAccountName == wsAccountName)
		{
			Worker(itm);
			return TRUE;
		}
		return FALSE;
	}) != nullptr;
}

BOOL CBnsTaskTeam::GetDungoenSchedule(_In_ CONST TeamScheduleMsg& TeamScheduleMsg_, _Out_ std::vector<BnsDungoenScheduleMember>& Result)
{
	return Lock_DungoenTeam.Access([&]
	{
		ExistTeamMember(DungoenTeamMap, TeamScheduleMsg_.dwTeamId, [&](TeamMemberGroup& TeamMemberGroup_)
		{
			for (CONST auto& itm : TeamMemberGroup_.MemberVec)
			{
				BnsDungoenScheduleMember Member;
				Member.wsPlayerName = itm.Member.wsAccountName;
				Member.wsPlayerName = itm.Member.wsPlayerName;
				Member.dwPlayerId = itm.Member.dwPlayerId;
				Member.wsDungoenSchedule = itm.Dungoen.wsDungoenSchedule;
				Member.uGatherIndex = itm.Dungoen.uGatherIndex;
				Member.dwTaskId = TeamMemberGroup_.dwTaskId;
				Member.dwScheduleId = TeamMemberGroup_.dwScheduleId;
				Result.push_back(std::move(Member));
			}
		});
	});
}

BOOL CBnsTaskTeam::SetReadOnlyTeam(_In_ DWORD dwTeamId)
{
	BOOL bRetCode = FALSE;
	Lock_DungoenTeam.Access([&]
	{
		bRetCode = ExistTeamMember(DungoenTeamMap, dwTeamId, [](TeamMemberGroup& TeamMemberGroup_) {TeamMemberGroup_.IsReadOnly = TRUE; });
	});

	return bRetCode;
}

BOOL CBnsTaskTeam::FindTeamGroupByAccountName(_In_ CONST std::wstring& wsAccountName, _Out_ TeamMemberGroup& TeamContent) CONST
{
	BOOL bExist = FALSE;
	Lock_BnsTeam.Access([&] {bExist = FindTeamGroupByAccountName(wsAccountName, TaskTeamMap, TeamContent); });
	if (bExist)
		return TRUE;


	Lock_DungoenTeam.Access([&] {bExist = FindTeamGroupByAccountName(wsAccountName, DungoenTeamMap, TeamContent); });
	return bExist;
}

BOOL CBnsTaskTeam::FindTeamGroupByAccountName(_In_ CONST std::wstring& wsAccountName, _In_ CONST std::map<std::wstring, TeamGroup>& MapTeam, _Out_ TeamMemberGroup& TeamContent) CONST
{
	for (CONST auto& TeamGroup_ : MapTeam)
	{
		for (CONST auto& TeamMemberGroup_ : TeamGroup_.second)
		{
			for (CONST auto& Member : TeamMemberGroup_.MemberVec)
			{
				if (Member.Member.wsAccountName == wsAccountName)
				{
					TeamContent = TeamMemberGroup_;
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

VOID CBnsTaskTeam::PrintOnLineTeam() CONST
{
	Lock_BnsTeam.Access([=] 
	{
		LOG_CF_D(L"主线.TaskTeamMap.size=%d", TaskTeamMap.size());
	});
}
