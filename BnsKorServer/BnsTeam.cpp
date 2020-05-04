#include "BnsTeam.h"
#include <MyTools/ByteBuffer.h>
#define _SELF L"BnsTeam.cpp"

CBnsTaskTeam::CBnsTaskTeam() : Lock_BnsTeam(L"CBnsTaskTeam_Lock"), Lock_DungoenTeam(L"Lock_DungoenTeam")
{
	m_dwTeamId = 0;
}

BOOL CBnsTaskTeam::ExistTeamMember(_In_ std::map<std::wstring, TeamGroup>& map_TeamGroup, _In_ CONST ClientTeamInfo& CtiMember, _In_ std::function<VOID(TeamMemberGroup&,TeamMemberInfo&)> Worker)
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
}

BOOL CBnsTaskTeam::ExistTeamMember(_In_ std::map<std::wstring, TeamGroup>& map_TeamGroup, _In_ DWORD dwTeamId, _In_ std::function<VOID(TeamMemberGroup&)> Worker)
{
	for (auto& itm : map_TeamGroup)
	{
		if (ExistMember_By_TeamId_In_TeamMemberGroup(dwTeamId, itm.second, Worker))
			return TRUE;
	}
	return FALSE;
}

VOID CBnsTaskTeam::CreateMapText(_In_ CONST ClientTeamInfo& CtiMember, _Out_ std::wstring& wsText)
{
	WCHAR wszText[MAX_PATH];
	// ServerId,[TaskId,SecheduleId],Channel
	swprintf_s(wszText, _countof(wszText) - 1, L"%X,[%X,%X],%d", CtiMember.dwServerId, CtiMember.dwTaskId, CtiMember.dwScheduleId, CtiMember.dwChannelId);
	wsText = wszText;
}

VOID CBnsTaskTeam::CreateTeamGroup(_In_ CONST std::wstring& wsMapText, _In_ CONST ClientTeamInfo& CtiMember, _In_ std::map<std::wstring, TeamGroup>& map_TeamGroup)
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
}

BOOL CBnsTaskTeam::ExistMember_By_AccountName_In_TeamMemberGroup(_In_ CONST std::wstring& wsAccountName, _In_ TeamGroup& TeamGroup_, _In_ std::function<VOID(TeamMemberGroup&, TeamMemberInfo&)> Worker) CONST
{
	for (auto& itm : TeamGroup_)
	{
		auto p = CLPublic::Deque_find_if(itm.MemberVec, [&wsAccountName](TeamMemberInfo& Member){ return Member.Member.wsAccountName == wsAccountName; });
		if (p != nullptr)
		{
			Worker(itm, *p);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CBnsTaskTeam::ExistMember_By_TeamId_In_TeamMemberGroup(_In_ DWORD dwTeamId, _In_ TeamGroup& TeamGroup_, _In_ std::function<VOID(TeamMemberGroup&)> Worker) CONST
{
	auto p = CLPublic::Deque_find_if(TeamGroup_, [&dwTeamId](TeamMemberGroup& itm){return itm.dwTeamId == dwTeamId; });
	if (p != nullptr)
	{
		Worker(*p);
		return TRUE;
	}
	return FALSE;
}

BOOL CBnsTaskTeam::FindCaptain_By_AccountName_In_TeamMemberGroup(_In_ TeamMemberGroup& TeamMemberGroup_, _In_ std::function<VOID(TeamMemberGroup&, TeamMemberInfo&)> Worker)
{
	auto p = CLPublic::Deque_find_if(TeamMemberGroup_.MemberVec, [](CONST TeamMemberInfo& TeamMemberInfo_){ return TeamMemberInfo_.bCaptain; });
	if (p != nullptr)
	{
		Worker(TeamMemberGroup_,*p);
		return TRUE;
	}
	return FALSE;
}

BOOL CBnsTaskTeam::IsWork(_In_ CONST ClientTeamInfo& CtiMember) CONST
{
	return CtiMember.SelfCti.wsPlayerName != L"" && CtiMember.SelfCti.dwPlayerId != NULL;
}

BOOL CBnsTaskTeam::JoinTaskTeam(_In_ CONST ClientTeamInfo& CtiMember, _Out_ ByteBuffer& ServerBuffer)
{
	if (!IsWork(CtiMember))
	{
		LOG_CF(CLog::em_Log_Type_Exception, L"Client���͵Ķ�����Ϣ����![%s,%X]", CtiMember.SelfCti.wsPlayerName.c_str(), CtiMember.SelfCti.dwPlayerId);
		
		ServerBuffer << SOCK_MSG_JOIN_TEAM_ORDER << L"" << 0;
		return TRUE;
	}

	return Lock_BnsTeam.Access([&CtiMember, &ServerBuffer,this]{
		JoinTeam(SOCK_MSG_JOIN_TEAM_ORDER, CtiMember, TaskTeamMap, ServerBuffer);
	});
}


BOOL CBnsTaskTeam::JoinDungoenTeam(_In_ CONST ClientTeamInfo& CtiMember, _Out_ ByteBuffer& ServerBuffer)
{
	if (!IsWork(CtiMember))
	{
		LOG_CF(CLog::em_Log_Type_Exception, L"Client���͵Ķ�����Ϣ����![%s,%X]", CtiMember.SelfCti.wsPlayerName.c_str(), CtiMember.SelfCti.dwPlayerId);
		ServerBuffer << SOCK_MSG_JOIN_TEAM_ORDER << L"" << 0;
		return TRUE;
	}

	return Lock_DungoenTeam.Access([&CtiMember, &ServerBuffer, this]{
		JoinTeam(SOCK_MSG_JOIN_DUNGOEN_TEAM, CtiMember, DungoenTeamMap, ServerBuffer);
	});
}


BOOL CBnsTaskTeam::FindVacantPosition_In_TeamMemberGroup(_In_ CONST ClientTeamInfo& CtiMember, _In_ std::map<std::wstring, TeamGroup>& map_TeamGroup, _In_ std::function<VOID(TeamMemberGroup&, TeamMemberInfo&)> Worker)
{
	std::wstring wsMapText;
	CreateMapText(CtiMember, wsMapText);
	auto itr = map_TeamGroup.find(wsMapText);
	if (itr == map_TeamGroup.end())
	{
		LOG_CF(CLog::em_Log_Type_Exception, L"map.findʧ��!, wsMapText=%s", wsMapText.c_str());
		return FALSE;
	}

	auto pTeamMemberGroup = CLPublic::Deque_find_if(itr->second, [CtiMember](TeamMemberGroup& itm)
	{
		// ���������!=""��ʱ��, ��Ҫ�����Ķ�Ա�Ķ������һ�²ſ��Լ���!
		if (itm.wsGroupTitle != L"" && itm.wsGroupTitle != CtiMember.wsGroupTitle)
			return false;

		return itm.MemberVec.size() < itm.uMaxMemberCount; 
	});
	if (pTeamMemberGroup != nullptr && !pTeamMemberGroup->IsReadOnly)
	{
		// ���뻹�п�λ�Ķ���
		TeamMemberInfo Member;
		Member.bCaptain = FALSE;
		Member.Member = CtiMember.SelfCti;
		pTeamMemberGroup->MemberVec.push_back(Member);
		return FindCaptain_By_AccountName_In_TeamMemberGroup(*pTeamMemberGroup, Worker);
	}

	// ����һ֧����!
	for (auto& itm : itr->second)
	{
		if (itm.MemberVec.size() < itm.uMaxMemberCount && !itm.IsReadOnly)
		{
			// Fill Self to this Team!
			// ����
			TeamMemberInfo Member;
			Member.bCaptain = FALSE;
			Member.Member = CtiMember.SelfCti;
			itm.wsGroupTitle = CtiMember.wsGroupTitle;
			itm.MemberVec.push_back(Member);
			//LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"���뻹�п�λ�Ķ���, ��ǰ��[%s,%X], ����ĳ�Ա:", CtiMember.SelfCti.wsPlayerName.c_str(), CtiMember.SelfCti.dwPlayerId);
			//for (CONST auto& TeamMemberInfo_ : itm.MemberVec)
			//	LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"[%s,%X, Captain:%X]", TeamMemberInfo_.Member.wsPlayerName.c_str(), TeamMemberInfo_.Member.dwPlayerId, TeamMemberInfo_.bCaptain);

			return FindCaptain_By_AccountName_In_TeamMemberGroup(itm, Worker);
		}
	}

	return CreateTeamMemberGroup(itr->second,CtiMember, Worker);
}

BOOL CBnsTaskTeam::CreateTeamMemberGroup(_In_ TeamGroup& TeamGroup_, _In_ CONST ClientTeamInfo& CtiMember, _In_ std::function<VOID(TeamMemberGroup&, TeamMemberInfo&)> Worker)
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
	//LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"�ӳ���������:[%s,%X], MaxCount=%d", CtiMember.SelfCti.wsPlayerName.c_str(), CtiMember.SelfCti.dwPlayerId, TeamGroup_.size());
	TeamGroup_.push_back(NewTeamMemberGroup_);

	// *TeamGroup_.back().MemberVec.begin() = �ӳ�
	auto& TeamMemberGroup_ = TeamGroup_.back();
	Worker(TeamMemberGroup_, *TeamMemberGroup_.MemberVec.begin());
	return TRUE;
}

BOOL CBnsTaskTeam::DeleteTeamMember(_In_ CONST std::wstring& wsAccountName, _In_ std::map<std::wstring, TeamGroup>& map_TeamGroup)
{
	for (auto& itm : map_TeamGroup)
	{
		ExistMember_By_AccountName_In_TeamMemberGroup(wsAccountName, itm.second, [&itm, this](TeamMemberGroup& TeamMemberGroup_, TeamMemberInfo&){
			DismissTeamMemberGroup(itm.second, TeamMemberGroup_);
		});
	}

	return TRUE;
}

BOOL CBnsTaskTeam::DismissTeamMemberGroup(_In_ TeamGroup& TeamGroup_, _In_ TeamMemberGroup& TeamMemberGroup_)
{
	DWORD dwTeamId = TeamMemberGroup_.dwTeamId;
	return CLPublic::Dque_erase_if(TeamGroup_, [&dwTeamId](TeamMemberGroup& TeamMemberGroup_){
		return TeamMemberGroup_.dwTeamId == dwTeamId;
	});
}

BOOL CBnsTaskTeam::JoinTeam(_In_ DWORD dwMsg, _In_ CONST ClientTeamInfo& CtiMember, _In_ std::map<std::wstring, TeamGroup>& map_TeamGroup, _Out_ ByteBuffer& ServerBuffer)
{
	auto bExist = ExistTeamMember(map_TeamGroup, CtiMember, [this, &ServerBuffer, &dwMsg, &CtiMember](TeamMemberGroup& TeamMemberGroup_, TeamMemberInfo& TeamSelf)
	{
		TeamSelf.Member.dwPlayerId = CtiMember.SelfCti.dwPlayerId;
		TeamSelf.Member.wsPlayerName = CtiMember.SelfCti.wsPlayerName;
		auto bExistCaptain = FindCaptain_By_AccountName_In_TeamMemberGroup(TeamMemberGroup_, [&ServerBuffer, &dwMsg, &TeamSelf, &TeamMemberGroup_](TeamMemberGroup&, TeamMemberInfo& Captain)
		{
			// ���ش˶���Ķӳ���Name��ID
			ServerBuffer << dwMsg << Captain.Member.wsPlayerName << Captain.Member.dwPlayerId;

			if (Captain.Member.wsAccountName != TeamSelf.Member.wsAccountName)
			{
				// �Լ����Ƕӳ���ʱ��, ֻ���ضӳ�, ����Ҫ���ض�Ա, �˷�����!
				// TeamMember Only Return Captain!
				ServerBuffer << static_cast<UINT>(0) << TeamMemberGroup_.dwTeamId;
			}
			else
			{
				// ���Լ��Ƕӳ���ʱ��, ���ض�������г�Ա
				ServerBuffer << (TeamMemberGroup_.MemberVec.size() - 1); // ����-1. ��Ϊ�ӳ��Ѿ������� , ����MemberVec���滹�жӳ�
				for (auto& itm : TeamMemberGroup_.MemberVec)
				{
					if (itm.bCaptain)
						continue;
					ServerBuffer << itm.Member.wsPlayerName << itm.Member.dwPlayerId;
				}
				ServerBuffer << TeamMemberGroup_.dwTeamId;
			}
		});

		if (!bExistCaptain)
		{
			// �����ڶӳ�? �ǿ����Ƕӳ�Ҳ��·��! 
			LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"�Ҳ����ӳ�, �ӳ�Ҳ��·��! ��ɢ�������!");
			DeleteTeamMember(CtiMember.SelfCti.wsAccountName, DungoenTeamMap);
		}
	});

	// �������κ�һֻ����, ֻ�ܴ���һֻ���� || Ѱ����ͬ����,�����п�λ�Ķ���,����
	if (!bExist)
	{
		auto bExistTeam = FindVacantPosition_In_TeamMemberGroup(CtiMember, map_TeamGroup, [&ServerBuffer, &dwMsg](TeamMemberGroup& TeamMemberGroup_, TeamMemberInfo& Captain){
			ServerBuffer << dwMsg << Captain.Member.wsPlayerName << Captain.Member.dwPlayerId;

			ServerBuffer << static_cast<UINT>(0) << TeamMemberGroup_.dwTeamId;
		});

		if (!bExistTeam)
		{
			LOG_CF(CLog::em_Log_Type_Exception, L"��������ʧ�ܡ�����ô����!!!");
			FindVacantPosition_In_TeamMemberGroup(CtiMember, map_TeamGroup, [&ServerBuffer, &dwMsg](TeamMemberGroup& TeamMemberGroup_, TeamMemberInfo& Captain){
				ServerBuffer << dwMsg << Captain.Member.wsPlayerName << Captain.Member.dwPlayerId;

				ServerBuffer << static_cast<UINT>(0) << TeamMemberGroup_.dwTeamId;
			});
			//ServerBuffer << dwMsg << CtiMember.SelfCti.wsPlayerName << CtiMember.SelfCti.dwPlayerId;
		}
	}

	return TRUE;
}

BOOL CBnsTaskTeam::DeleteDungoenTeamMember(_In_ CONST std::wstring& wsPlayerName)
{
	return Lock_DungoenTeam.Access([&wsPlayerName, this]
	{ 
		for (auto& itm : DungoenTeamMap)
		{
			ExistMember_By_AccountName_In_TeamMemberGroup(wsPlayerName, itm.second, [&itm, this, wsPlayerName](TeamMemberGroup& TeamMemberGroup_, TeamMemberInfo& Tmi){

				// �����ʣ��һ������, ֱ�ӽ�ɢ��!
				if (TeamMemberGroup_.MemberVec.size() <= 1)
				{
					LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"Server��ɢ��:[%s,%s]", Tmi.Member.wsAccountName.c_str(), Tmi.Member.wsPlayerName.c_str());
					DismissTeamMemberGroup(itm.second, TeamMemberGroup_);
				}
				else // ����ֻɾ���Լ�!
				{
					CLPublic::Deque_find_if(TeamMemberGroup_.MemberVec, [wsPlayerName](CONST TeamMemberInfo& Ti){ return Ti.Member.wsPlayerName == wsPlayerName; });
					LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"Server���:[%s,%s], ʣ��%d����", Tmi.Member.wsAccountName.c_str(), Tmi.Member.wsPlayerName.c_str(), TeamMemberGroup_.MemberVec.size());
				}
			});
		}
	});
}

BOOL CBnsTaskTeam::DeleteTaskMember(_In_ CONST std::wstring& wsAccountName)
{
	return Lock_BnsTeam.Access([&wsAccountName, this]{ DeleteTeamMember(wsAccountName, TaskTeamMap); });
}

BOOL CBnsTaskTeam::GetLessMemberChannel(_In_ CONST TeamChannelMsg& TeamChannelMsg_, _Out_ ByteBuffer& ByteBuffer_)
{
	std::map<DWORD, DWORD> ChannelMap;
	GetSameChannelMap(TeamChannelMsg_, ChannelMap);
	
	// Find Empty Channel
	for (DWORD i = 1; i <= TeamChannelMsg_.dwMaxChannel; ++i)
	{
		if (ChannelMap.find(i) == ChannelMap.end())
		{
			ByteBuffer_ << SOCK_MSG_GET_CHANNEL << i;
			return TRUE;
		}
	}
	
	// sort by less
	std::vector<std::pair<DWORD, DWORD>> ChannelVec(ChannelMap.begin(), ChannelMap.end());
	sort(ChannelVec.begin(), ChannelVec.end(), [](CONST std::pair<DWORD, DWORD>& p1, CONST std::pair<DWORD, DWORD>& p2){return p1.second < p2.second; });

	
	ByteBuffer_ << SOCK_MSG_GET_CHANNEL << (ChannelVec.size() == 0 ? 1 : ChannelVec.begin()->second);
	return TRUE;
}

BOOL CBnsTaskTeam::GetSameChannelMap(_In_ CONST TeamChannelMsg& TeamChannelMsg_, _Out_opt_ std::map<DWORD, DWORD>& ChannelMap)
{
	// Get all of same channel in [TaskTeamMap] && calc same channel count
	return Lock_BnsTeam.Access([this, &TeamChannelMsg_, &ChannelMap]{
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

BOOL CBnsTaskTeam::SearchGroupByAccountName(_In_ CONST std::wstring& wsAccountName, _In_ std::function<VOID(CONST TeamMemberGroup&)> Worker)
{
	LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"�����������:-------------");
	Lock_BnsTeam.Access([this, &wsAccountName, &Worker]{ SearchGroupByAccountName(TaskTeamMap, wsAccountName, Worker); });
	LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"������������:-------------");
	Lock_DungoenTeam.Access([this, &wsAccountName, &Worker]{ SearchGroupByAccountName(DungoenTeamMap, wsAccountName, Worker); });
	LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"��ȡ���:---------------");
	return TRUE;
}

BOOL CBnsTaskTeam::SearchGroupByAccountName(_In_ std::map<std::wstring, TeamGroup>& TeamGroupMap, _In_ CONST std::wstring& wsAccountName, _In_ std::function<VOID(CONST TeamMemberGroup&)> Worker) CONST
{
	for (auto& itm : TeamGroupMap)
	{
		ExistMember_By_AccountName_In_TeamMemberGroup(wsAccountName, itm.second, [&Worker](TeamMemberGroup& TeamMemberGroup_, TeamMemberInfo&){
			Worker(TeamMemberGroup_);
		});
	}
	return TRUE;
}

BOOL CBnsTaskTeam::SearchGroupByTask(_In_ CONST TaskInfoParm& Tip, _In_ std::function<VOID(CONST TeamMemberGroup&)> Worker) CONST
{
	LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"�����������:-------------");
	Lock_BnsTeam.Access([this, &Tip, &Worker]{ SearchGroupByTask(TaskTeamMap, Tip, Worker); });
	LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"������������:-------------");
	Lock_DungoenTeam.Access([this, &Tip, &Worker]{ SearchGroupByTask(DungoenTeamMap, Tip, Worker); });
	LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"��ȡ���:---------------");
	return TRUE;
}

BOOL CBnsTaskTeam::SearchGroupByTask(_In_ CONST std::map<std::wstring, TeamGroup>& TeamGroupMap, _In_ CONST TaskInfoParm& Tip, _In_ std::function<VOID(CONST TeamMemberGroup&)> Worker) CONST
{
	for (CONST auto& itm : TeamGroupMap)
	{
		for (CONST auto& TeamMemberGroup_ : itm.second)
		{
			if (TeamMemberGroup_.dwTaskId == Tip.dwTaskId && TeamMemberGroup_.dwScheduleId == Tip.dwScheduleId)
			{
				Worker(TeamMemberGroup_);
			}
		}
	}
	return TRUE;
}

BOOL CBnsTaskTeam::SearchGroupByDungoneName(_In_ CONST std::wstring& wsDungoneName, _In_ std::function<VOID(CONST TeamMemberGroup&)> Worker) CONST
{
	LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"������������:-------------");
	Lock_DungoenTeam.Access([this, &wsDungoneName, &Worker]
	{  
		for (CONST auto& itm : DungoenTeamMap)
		{
			for (CONST auto& TeamMemberGroup_ : itm.second)
			{
				if (TeamMemberGroup_.wsDungoenName == wsDungoneName)
				{
					Worker(TeamMemberGroup_);
				}
			}
		}
	});
	LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"��ȡ���:---------------");
	return TRUE;
}

BOOL CBnsTaskTeam::UpdateDungoenSchedule(_In_ CONST TeamScheduleMsg& TeamScheduleMsg_, _Out_ ByteBuffer& ByteBuffer_)
{
	return Lock_DungoenTeam.Access([this, &TeamScheduleMsg_, &ByteBuffer_]
	{
		ExistTeamMember(DungoenTeamMap, TeamScheduleMsg_.dwTeamId, [this, &TeamScheduleMsg_](TeamMemberGroup& TeamMemberGroup_)
		{
			ActionInTeamGroupMemberByAccountName(TeamScheduleMsg_.wsAccountName, TeamMemberGroup_, [&TeamScheduleMsg_](TeamMemberInfo& itm)
			{
				itm.Dungoen.wsDungoenSchedule = TeamScheduleMsg_.wsScheduleName;
				itm.Dungoen.uGatherIndex = TeamScheduleMsg_.uScheduleIndex;
			});
		});

		ByteBuffer_ << SOCK_MSG_UPDATE_TEAM_SCHEDULE;
	});
}

BOOL CBnsTaskTeam::ActionInTeamGroupMemberByAccountName(_In_ CONST std::wstring& wsAccountName, _In_ TeamMemberGroup& TeamMemberGroup_, _In_ std::function<VOID(TeamMemberInfo&)> Worker) CONST
{
	return CLPublic::Deque_find_if(TeamMemberGroup_.MemberVec, [&wsAccountName, &Worker](TeamMemberInfo& itm)
	{
		if (itm.Member.wsAccountName == wsAccountName)
		{
			Worker(itm);
			return TRUE;
		}
		return FALSE;
	}) != nullptr;
}

BOOL CBnsTaskTeam::PrintTaskTeam() CONST
{
	return Lock_BnsTeam.Access([this]
	{
		for (CONST auto& TaskTeam : TaskTeamMap)
		{
			for (CONST auto& itm : TaskTeam.second)
			{
				LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"Server=%X,Channel=%X,Task=[%X,%X],MaxCount=%d,MemberCount=%d", \
					itm.dwServerId, itm.dwChannelId, itm.dwChannelId, itm.dwTaskId, itm.dwScheduleId, itm.uMaxMemberCount, itm.MemberVec.size());

				for (CONST auto& TeamMember : itm.MemberVec)
				{
					LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"%s,[%s,%s,%X]", TeamMember.bCaptain ? L"�ӳ�" : L"��Ա", TeamMember.Member.wsAccountName.c_str(), \
						TeamMember.Member.wsPlayerName.c_str(), TeamMember.Member.dwPlayerId);
				}
			}
		}
	});
}

BOOL CBnsTaskTeam::GetDungoenSchedule(_In_ CONST TeamScheduleMsg& TeamScheduleMsg_, _Out_ ByteBuffer& ByteBuffer_)
{
	return Lock_DungoenTeam.Access([this, &TeamScheduleMsg_, &ByteBuffer_]
	{
		BOOL bExist = ExistTeamMember(DungoenTeamMap, TeamScheduleMsg_.dwTeamId, [&TeamScheduleMsg_, &ByteBuffer_](TeamMemberGroup& TeamMemberGroup_)
		{
			ByteBuffer_ << SOCK_MSG_GET_TEAM_SCHEDULE;
			ByteBuffer_ << TeamMemberGroup_.MemberVec.size();
			for (CONST auto& itm : TeamMemberGroup_.MemberVec)
			{
				ByteBuffer_ << itm.Member.wsAccountName;
				ByteBuffer_ << itm.Member.wsPlayerName;
				ByteBuffer_ << itm.Member.dwPlayerId;
				ByteBuffer_ << itm.Dungoen.wsDungoenSchedule;
				ByteBuffer_ << itm.Dungoen.uGatherIndex;
				ByteBuffer_ << TeamMemberGroup_.dwTaskId;
				ByteBuffer_ << TeamMemberGroup_.dwScheduleId;
			}
		});

		if (!bExist)
		{
			ByteBuffer_ << SOCK_MSG_GET_TEAM_SCHEDULE << 0;
		}
	});
}

BOOL CBnsTaskTeam::SetReadOnlyTeam(_In_ DWORD dwTeamId, _Out_ ByteBuffer& ByteBuffer_)
{
	return Lock_DungoenTeam.Access([&dwTeamId, this, &ByteBuffer_]
	{
		BOOL bExist = ExistTeamMember(DungoenTeamMap, dwTeamId, [](TeamMemberGroup& TeamMemberGroup_){
			TeamMemberGroup_.IsReadOnly = TRUE;
		});
		ByteBuffer_ << SOCK_MSG_SET_TEAM_READONLY << bExist;
	});
}
