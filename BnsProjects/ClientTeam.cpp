#include "stdafx.h"
#include "ClientTeam.h"
#include <MyTools/Log.h>
#include <MyTools/CLThread.h>
#include "TeamMemberExtend.h"
#include "PlayerExtend.h"
#include "Person.h"
#include "SomeFun.h"
#include "Task.h"
#include "ObjectFunction.h"
#include "TeamMember.h"
#include "GameUI.h"
#include "Farm.h"
#include "PersonFunction.h"

#define _SELF L"ClientTeam.cpp"

#define MAX_TEAM_WAIT_TIME 120 * 1000
CClientTeam::CClientTeam() : _hJoinTeamThread(NULL)
{
	
}


BOOL CClientTeam::BnsScript_JoinTeam(_In_ DWORD dwMaxMember, _In_ BOOL bWait)
{
#if BnsProject_DLL
	SetClientTeamInfo(dwMaxMember);

	LOG_CF_D( L"SendJoinTeam, dwMaxMember=%d, bWait=%d", dwMaxMember, bWait);
	StopJoinTeamThread();
	while (GameRun)
	{
		auto Result = JoinTeam(CSocketAPI::em_JoinTeam_Type::em_JoinTeam_Type_MainLine, TRUE);
		if (Result.dwTeamId == NULL)
		{
			LOG_CF_D( L"GetRecvJoinTeamName().size() = NULL");
			return FALSE;
		}

		if (CPerson::GetInstance().GetName() == Result.Captain.wsPlayerName)
		{
			// ���Ƕӳ�
			LOG_CF_D( L"����%s ����, �Ҿ��Ƕӳ���!", Result.Captain.wsPlayerName.c_str());
			if (JoinTeam_Captain(CSocketAPI::em_JoinTeam_Type::em_JoinTeam_Type_MainLine, bWait))
			{
				if (CTeamMemberExtend::GetInstance().GetTeamMemberCount() > 0)
				{
					LOG_CF_D( L"�޸ľ�����Ʒ����ɫ����!");
					CTeamMemberExtend::GetInstance().SetAuctionQuality(em_Auction_Quality_Extreme);
				}
				break;
			}
		}
		else
		{
			// ���Ƕ�Ա
			LOG_CF_D( L"���Ƕ�Ա, �ӳ���:%s,ID:%X", Result.Captain.wsPlayerName.c_str(), Result.Captain.dwPlayerId);
			if (JoinTeam_Member(CSocketAPI::em_JoinTeam_Type::em_JoinTeam_Type_MainLine, Result.Captain, bWait))
				break;
		}
	}
	
	// �����Զ������߳�
	_bRunThread = TRUE;
	_hJoinTeamThread = cbBEGINTHREADEX(NULL, NULL, _JoinTeamThread, this, NULL, NULL);
	if (_hJoinTeamThread == NULL)
		LOG_MSG_CF( L"�����߳�_JoinTeamThreadʧ��!");

#else
	LOG_CF_D( L"ע������²������!");
#endif
	return TRUE;
}

BOOL CClientTeam::BnsScript_LeaveTeam()
{
#if BnsProject_DLL
	// Get Current PlayerInfo
	SetClientTeamInfo(0);

	// Stop JoinTeam Thread
	StopJoinTeamThread();

	// ����Լ���ǰ���ж���, ��ô���˳�����
	LOG_CF_D( L"�������!, ��ǰ��������=%d", CTeamMemberExtend::GetInstance().GetTeamMemberCount());


	_SocketAPI.LeaveTeam(CSocketAPI::em_JoinTeam_Type::em_JoinTeam_Type_MainLine);
	while (CTeamMemberExtend::GetInstance().GetTeamMemberCount() != NULL)
	{
		LOG_CF_D(L"��ʼ��Ӳ���!");
		CTeamMemberExtend::GetInstance().LeaveTeam();
		BnsSleep(3 * 1000);
		CPersonFunction::GetInstance().WaitGameLoad();
	}

	_dwTeamId = NULL;
#else
	LOG_CF_D( L"ע������²������!");
#endif
	return TRUE;
}

BOOL CClientTeam::SetClientTeamInfo(_In_ DWORD dwMaxMemberCount)
{
	TaskInfoParm Ti = CTask::GetInstance().GetMianTaskInfo();
#if BnsProject_DLL
	do
	{
		_ClientTeamInfo.dwServerId = CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->dwServerId;
		_ClientTeamInfo.dwChannelId = CPerson::GetInstance().GetChannel();
		_ClientTeamInfo.dwMaxMember = dwMaxMemberCount;
		_ClientTeamInfo.dwScheduleId = Ti.dwScheduleId;
		_ClientTeamInfo.dwTaskId = Ti.dwTaskId;
		_ClientTeamInfo.SelfCti.wsPlayerName = CPerson::GetInstance().GetName();
		_ClientTeamInfo.SelfCti.dwPlayerId = CPerson::GetInstance().GetId();
		_ClientTeamInfo.SelfCti.wsAccountName = CGameVariable::GetInstance().GetAccShareInfo()->GetAccName();
		// maybe wsPlayerName = L"" ?  should make loop!
	} while (GameRun && _ClientTeamInfo.SelfCti.wsPlayerName == L"" && BnsSleep(1000));

	LOG_CF_D( L"������ID=%X, Ƶ��=%d,����Ա=%d,����=[%X,%X],��ǰ��ɫ=[%s,%X]", 
		_ClientTeamInfo.dwServerId, 
		_ClientTeamInfo.dwChannelId, 
		_ClientTeamInfo.dwMaxMember,
		_ClientTeamInfo.dwTaskId,
		_ClientTeamInfo.dwScheduleId, 
		_ClientTeamInfo.SelfCti.wsPlayerName.c_str(),
		_ClientTeamInfo.SelfCti.dwPlayerId
		);
#endif
	return TRUE;
}

BOOL CClientTeam::SetClientGungoenTeamInfo(_In_ CONST TaskInfoParm& TaskInfoParm_, _In_ DWORD dwMaxMemberCount)
{
#if BnsProject_DLL
	ClientTeamInfo ClientTeamInfo_;

	do
	{
		ClientTeamInfo_.dwServerId = CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->dwServerId;
		ClientTeamInfo_.dwChannelId = 0;
		ClientTeamInfo_.dwMaxMember = dwMaxMemberCount;
		ClientTeamInfo_.dwScheduleId = TaskInfoParm_.dwScheduleId;
		ClientTeamInfo_.dwTaskId = TaskInfoParm_.dwTaskId;
		ClientTeamInfo_.SelfCti.wsPlayerName = CPerson::GetInstance().GetName();
		ClientTeamInfo_.SelfCti.dwPlayerId = CPerson::GetInstance().GetId();
		ClientTeamInfo_.SelfCti.wsAccountName = CGameVariable::GetInstance().GetAccShareInfo()->GetAccName();
		// maybe wsPlayerName = L"" ?  should make loop!
	} while (GameRun && ClientTeamInfo_.SelfCti.wsPlayerName == L"" && BnsSleep(1000));

	LOG_CF_D( L"������ID=%X, Ƶ��=%d,����Ա=%d,����=[%X,%X],��ǰ��ɫ=[%s,%X]", ClientTeamInfo_.dwServerId, ClientTeamInfo_.dwChannelId, ClientTeamInfo_.dwMaxMember, ClientTeamInfo_.dwTaskId, ClientTeamInfo_.dwScheduleId, \
		ClientTeamInfo_.SelfCti.wsPlayerName.c_str(), ClientTeamInfo_.SelfCti.dwPlayerId);

	_ClientTeamInfo = ClientTeamInfo_;
#endif
	return TRUE;
}
#if BnsProject_DLL
BOOL CClientTeam::InvitePlayer_By_List(_In_ CONST CSocketAPI::SocketTeamResult& TeamContent) CONST
{
	std::wstring wsSelfName = CPerson::GetInstance().GetName();

	struct InviteClientTeamMember
	{
		std::wstring     wsPlayerName;
		DWORD64			 dwInviteTick;
		InviteClientTeamMember(_In_ CONST std::wstring& wsPlayerName_)
		{
			wsPlayerName = wsPlayerName_;
			dwInviteTick = ::GetTickCount64();
		}
	};
	static std::map<std::wstring, ULONGLONG> MapInvitedMember;

	for (CONST auto& Member : TeamContent.VecMember)
	{
		if (wsSelfName == Member.wsPlayerName)
			continue;

		// �Ѿ��ڶ�����˱�����!
		if (CTeamMemberExtend::GetInstance().ExistTeamMember(Member.wsPlayerName, nullptr))
			continue;

		auto itr = MapInvitedMember.find(Member.wsPlayerName);

		// ͬһ���� 15s ����һ�Σ� ������Ϸ��T������
		if (itr != MapInvitedMember.end())
		{
			if (static_cast<DWORD>(::GetTickCount64() - itr->second) >= 15 * 1000)
				MapInvitedMember.erase(itr);

			continue;
		}

		LOG_CF_D( L"�ӳ�:%s�������:%s, ID=%X", CPerson::GetInstance().GetName().c_str(), Member.wsPlayerName.c_str(), Member.dwPlayerId);
		CPlayer::InviteTeamMember(Member.dwPlayerId);
		MapInvitedMember.emplace(std::make_pair(Member.wsPlayerName, ::GetTickCount64()));
		BnsSleep(500);
	}
	return TRUE;
}

BOOL CClientTeam::JoinTeam_Captain(_In_ CSocketAPI::em_JoinTeam_Type emType, _In_ BOOL bWait)
{
	CONST std::wstring wsSelfName = CPerson::GetInstance().GetName();
	auto dwTick = ::GetTickCount64();
	auto uCount = NULL;
	CSocketAPI::SocketTeamResult Result;
	DWORD dwMaxTimeOut = CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_JOINTEAM_TIMEOUT) * 1000;
	dwMaxTimeOut = dwMaxTimeOut = 0 ? MAX_TEAM_WAIT_TIME : dwMaxTimeOut;

	
	do
	{
		if (!bWait && static_cast<DWORD>(::GetTickCount64() - dwTick >= dwMaxTimeOut))
		{
			LOG_CF_D( L"���Ƕӳ�, ����ȴ���ʱ:%ds! ���ٵ���!", dwMaxTimeOut / 1000);
			break;
		}

		if (uCount++ % 3 == 0) // ÿ3��ȥ����������һ������, ���ҷ��ص�ǰ����!
		{
			Result = JoinTeam(emType, FALSE);
			for (CONST auto& itm : Result.VecMember)
				LOG_C_D(L"Member=[%s,%s]", itm.wsAccountName.c_str(), itm.wsPlayerName.c_str());
		}

		if (!DoCaptain(Result))
			return FALSE;

		BnsSleep(1000);
	} while (GameRun && CTeamMemberExtend::GetInstance().GetTeamMemberCount() < _ClientTeamInfo.dwMaxMember);

	return TRUE;
}

BOOL CClientTeam::JoinTeam_Member(_In_ CSocketAPI::em_JoinTeam_Type emType, CONST ClientTeamMember& CtiCaptain, _In_ BOOL bWait)
{
	CONST std::wstring wsSelfName = CPerson::GetInstance().GetName();
	auto dwTick = ::GetTickCount64();
	auto uCount = NULL;
	auto Captain = CtiCaptain;

	DWORD dwMaxTimeOut = CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_JOINTEAM_TIMEOUT) * 1000;
	dwMaxTimeOut = dwMaxTimeOut = 0 ? MAX_TEAM_WAIT_TIME : dwMaxTimeOut;


	do
	{
		if (!bWait && static_cast<DWORD>(::GetTickCount64() - dwTick) >= dwMaxTimeOut)
		{
			LOG_CF_D( L"���Ƕ�Ա, ����ȴ���ʱ:%ds! ���ٵ���!", dwMaxTimeOut / 1000);
			break;
		}

		if (++uCount % 5 == 0) // ÿ5��ȥ����������һ������, ���ҷ��ص�ǰ�ӳ�!
		{
			auto Result = JoinTeam(emType, FALSE);
			Captain = Result.Captain;
			LOG_C_D(L"�ӳ�=[%s,%s]", Captain.wsAccountName.c_str(), Captain.wsPlayerName.c_str());
			if (Captain.wsPlayerName == L"")
			{
				LOG_CF_E( L"�ӷ�������ȡ�ӳ���������null��! Captain=[%s,%X]", Captain.wsPlayerName.c_str(), Captain.dwPlayerId);
				uCount = 0;
				continue;
			}
		}

		// �Ƿ���������!
		if (!DoMember(Captain))
			return FALSE;

		BnsSleep(1000);
	} while (GameRun && CTeamMemberExtend::GetInstance().GetTeamMemberCount() < _ClientTeamInfo.dwMaxMember);
	return TRUE;
}
#endif
unsigned WINAPI CClientTeam::_JoinTeamThread(LPVOID lpParm)
{	
#if BnsProject_DLL
	CClientTeam* pCClientTeam = (CClientTeam *)lpParm;

	UINT uCount = 0;
	DWORD dwTeamMemberCount = 0;
	CONST std::wstring wsSelfName = CPerson::GetInstance().GetName();
	while (GameRun && pCClientTeam->_bRunThread)
	{
		BnsSleep(1000);

		/*CTeamMember TeamMember;
		if (GameRun && CTeamMemberExtend::GetInstance().IsTeamMember_LoseConnect(&TeamMember) && CTeamMemberExtend::GetInstance().IsTeamCaptain(wsSelfName))
		{
			LOG_CF_D( L"��Ա:[%s,%d]������, ŭT!", TeamMember.GetName().c_str(), TeamMember.GetIndex());
			TeamMember.DeleteMember();
			continue;
		}*/

		//if (CTeamMemberExtend::GetInstance().GetTeamMemberCount() == pCClientTeam->m_ClientTeamInfo.dwMaxMember)
		//	continue;
		CPersonFunction::GetInstance().WaitGameLoad();
		if (++uCount % 5 == 0) // ÿ3��ȥ����������һ������, ���ҷ��ص�ǰ����!
		{
			auto Result = pCClientTeam->JoinTeam(CSocketAPI::em_JoinTeam_Type::em_JoinTeam_Type_MainLine, TRUE);
			auto& Captain = Result.Captain;
			//LOG_CF_D( L"��ǰ��ɫ:%s, ���صĶӳ���:%s", wsSelfName.c_str(), Captain.wsPlayerName.c_str());
			if (Captain.wsPlayerName == L"") // reset ClientTeamInfo to server!
			{
				LOG_CF_E( L"Server���صĶӳ�=null,Content:[%s,%X]", Captain.wsPlayerName.c_str(), Captain.dwPlayerId);
				pCClientTeam->SetClientTeamInfo(pCClientTeam->_ClientTeamInfo.dwMaxMember);
				continue;
			}
			//for (CONST auto& itm : vlst)
			//	LOG_CF_D( L"Server���صĶ�Ա:%s,ID:%X", itm.wsPlayerName.c_str(), itm.dwPlayerId);

			if (Captain.wsPlayerName == wsSelfName)
				pCClientTeam->DoCaptain(Result);
			else
				pCClientTeam->DoMember(Captain);

			// �����������仯��ʱ��, �ж��Լ��Ƕӳ�������һ������
			if (CTeamMemberExtend::GetInstance().GetTeamMemberCount() != dwTeamMemberCount && CTeamMemberExtend::GetInstance().IsTeamCaptain(wsSelfName))
			{
				CTeamMemberExtend::GetInstance().SetAuctionQuality(em_Auction_Quality_Extreme);
				dwTeamMemberCount = CTeamMemberExtend::GetInstance().GetTeamMemberCount();
			}
		}
	}
#endif
	return 0;
}
#if BnsProject_DLL
BOOL CClientTeam::DoCaptain(_In_ CONST CSocketAPI::SocketTeamResult& TeamContent) CONST
{

	// �ж��Լ��ڶ��������Ƿ�ӳ�, ������Ƕӳ�, �����˶�
	if (CTeamMemberExtend::GetInstance().GetTeamMemberCount() != 0 && !CTeamMemberExtend::GetInstance().IsTeamCaptain(CPerson::GetInstance().GetName()))
	{
		LOG_CF_D( L"��Server�������Ƕӳ�:%s, �����ڶ����ﲻ�Ƕӳ�, ��ŭ�˶�", CPerson::GetInstance().GetName().c_str());
		CTeamMemberExtend::GetInstance().LeaveTeam();
		return FALSE;
	}

	return InvitePlayer_By_List(TeamContent);
}

BOOL CClientTeam::DoMember(_In_ CONST ClientTeamMember& CtiCaptain) CONST
{
	// �ж��Լ��Ƿ�ӳ�, ����Ƕӳ�, ��ô�ͱ�ʾ�������ӳ��Ѿ������, ���˶�
	if (CTeamMemberExtend::GetInstance().GetTeamMemberCount() != 0 && CTeamMemberExtend::GetInstance().IsTeamCaptain(CPerson::GetInstance().GetName()))
	{
		LOG_CF_D( L"��Server�����Ҳ��Ƕӳ�:%s, �����ڶ������Ƕӳ�, ��ŭ�˶�", CPerson::GetInstance().GetName().c_str());
		CTeamMemberExtend::GetInstance().LeaveTeam();
		return FALSE;
	}
	
	// �Ƿ���������!
	CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"InvitedPartyConfirmPanel", [&CtiCaptain](CONST CGameUi& InvitedPartyConfirmPanel)
	{
		if (ReadDWORD(InvitedPartyConfirmPanel.GetObj() + ��������ӳ�IDƫ��) != CtiCaptain.dwPlayerId)
		{
			DWORD dwInvitedId = ReadDWORD(InvitedPartyConfirmPanel.GetObj() + ��������ӳ�IDƫ��);
			std::wstring wsName = L"Empty";

			CPlayer Player;
			if (CPlayerExtend::GetInstance().FindPlayer_By_Id(dwInvitedId, CBaseObject::CreateFixPointInfo(-1), &Player))
				wsName = Player.GetName();

			LOG_CF_D( L"�ܾ��Ƕӳ�:%s������, �����ID:%X, Name=%s",CtiCaptain.wsPlayerName.c_str(),  dwInvitedId, wsName.c_str());
			CSomeFun::GetInstance().SimlationKey('N', 200, 800);
			return;
		}

		LOG_CF_D( L"���ܶӳ�:%s ������!", CtiCaptain.wsPlayerName.c_str());
		InvitedPartyConfirmPanel.AcceptTeamInvite();
		BnsSleep(2000);

		/*if (CTeamMemberExtend::GetInstance().GetTeamMemberCount() != 0)
		{
			CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"WindowConfirmPanel_Main", [](CONST CGameUi& WindowConfirmPanel_Main){
				LOG_CF_D( L"ȥ�ӳ���Ƶ��!");
				WindowConfirmPanel_Main.ClickYes();
				BnsSleep(1000);
				CSomeFun::GetInstance().SimlationKey('Y');
				BnsSleep(1000);
			});
		}*/
	});
	return TRUE;
}
#endif
VOID CClientTeam::StopJoinTeamThread()
{
	_bRunThread = FALSE;
	::WaitForSingleObject(_hJoinTeamThread, INFINITE);
	::CloseHandle(_hJoinTeamThread);
	_hJoinTeamThread = NULL;
}


TeamDungeonSchedule& CClientTeam::GetTeamDungoenSchedule()
{
	return _TeamDungeonSchedule;
}

DWORD CClientTeam::GetTeamId() CONST
{
	return _dwTeamId;
}

VOID CClientTeam::SetDungeonSchedule(_In_ CONST std::wstring& wsScheduleText, _In_ UINT uScheduleIndex) CONST
{
#if BnsProject_DLL
	_SocketAPI.SetGungoenSchedule(_dwTeamId, wsScheduleText, uScheduleIndex);
#endif
}


DWORD CClientTeam::GetChannelForServer(_In_ CONST TaskInfoParm& TaskParam) CONST
{
#if BnsProject_DLL
	TeamChannelMsg Content;
	Content.dwServerId = CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->dwServerId;
	Content.dwChannel = CPerson::GetInstance().GetChannel();
	Content.dwMaxChannel = CPersonFunction::GetInstance().GetMaxChannel();
	Content.dwTaskId = TaskParam.dwTaskId;
	Content.dwScheduleId = TaskParam.dwScheduleId;
	return _SocketAPI.GetLessPlayerChannel(Content);
#else
	return 0;
#endif
}
#if BnsProject_DLL
CSocketAPI::SocketTeamResult CClientTeam::JoinTeam(_In_ CSocketAPI::em_JoinTeam_Type emType, _In_ BOOL bFirst)
{
	CSocketAPI::SocketTeamResult Result;
	_SocketAPI.JoinTeam(emType, bFirst ? &_ClientTeamInfo : nullptr, Result);

	_dwTeamId = Result.dwTeamId;
	return Result;
}

VOID CClientTeam::LeaveTeam(_In_ CSocketAPI::em_JoinTeam_Type emType)
{
	_SocketAPI.LeaveTeam(emType);
}
#endif
BOOL CClientTeam::BnsScript_LeaveGungoenTeam()
{
#if BnsProject_DLL
	// Get Current PlayerInfo
	SetClientTeamInfo(0);

	// ����Լ���ǰ���ж���, ��ô���˳�����
	LOG_CF_D( L"�������!, ��ǰ��������=%d", CTeamMemberExtend::GetInstance().GetTeamMemberCount());
	LeaveTeam(CSocketAPI::em_JoinTeam_Type::em_JoinTeam_Type_Gungoen);
	while (GameRun && CTeamMemberExtend::GetInstance().GetTeamMemberCount() != NULL)
	{
		LOG_CF_D( L"��ʼ��Ӳ���!");
		CTeamMemberExtend::GetInstance().LeaveTeam();
		BnsSleep(3 * 1000);
		CPersonFunction::GetInstance().WaitGameLoad();
	}

	_TeamDungeonSchedule.Clear();
	_dwTeamId = NULL;
#else
	LOG_CF_D( L"ע������²������!");
#endif
	return TRUE;
}

BOOL CClientTeam::BnsScript_JoinGungoenTeam(_In_ CONST TaskInfoParm& TaskInfoParm_, _In_ DWORD dwMaxMember, _In_ BOOL bWait)
{
#if BnsProject_DLL

	// �����ǰ�Ѿ������ڶ���������, �ȷ�����ӵ���������ɢ����!
	if (CTeamMemberExtend::GetInstance().GetTeamMemberCount() == 0)
		BnsScript_LeaveGungoenTeam();

	// �����ǰ���ڶ���, �ж���߽����Ƿ�=0, ���!=0, �Ͳ����ٵ���!
	if (CTeamMemberExtend::GetInstance().GetTeamMemberCount() != 0)
	{
		RefreshGunoneTeamSchedule();
		// ��ȡ���������˵Ľ���, �ж�����˵Ľ���
		TeamMemberSchedule MaxSceduleTeamMember;
		if (_TeamDungeonSchedule.FindByMaxScheduleIndex(MaxSceduleTeamMember))
		{
			if (TaskInfoParm(MaxSceduleTeamMember.dwTaskId, MaxSceduleTeamMember.dwScheduleId) == TaskInfoParm_)
			{
				// �������� > ���ڵĽ��ȵ�ʱ��, ֱ��break;
				if (MaxSceduleTeamMember.uScheduleIndex > 0)
				{
					LOG_CF_D( L"�������=[%s,%d], �����Ѿ���ʼ����, ������!", MaxSceduleTeamMember.wsScheduleName.c_str(), MaxSceduleTeamMember.uScheduleIndex);
					return TRUE;
				}
			}
			else
			{
				LOG_CF_D( L"�ҵ�ǰ��ӵ�������:[%X,%X], ���Ƕ����������:[%X,%X], ���!", TaskInfoParm_.dwTaskId, TaskInfoParm_.dwScheduleId, MaxSceduleTeamMember.dwTaskId, MaxSceduleTeamMember.dwScheduleId);
				BnsScript_LeaveGungoenTeam();
			}
		}
	}

	SetClientGungoenTeamInfo(TaskInfoParm_, dwMaxMember);

	LOG_CF_D( L"Send JoinGungoenTeam, dwMaxMember=%d, bWait=%d", dwMaxMember, bWait);
	
	while (GameRun)
	{
		auto Result = JoinTeam(CSocketAPI::em_JoinTeam_Type::em_JoinTeam_Type_Gungoen, TRUE);
		if (CPerson::GetInstance().GetName() == Result.Captain.wsPlayerName)
		{
			// ���Ƕӳ�
			LOG_CF_D( L"����%s ����, �Ҿ��Ƕӳ���!", Result.Captain.wsPlayerName.c_str());
			if (JoinTeam_Captain(CSocketAPI::em_JoinTeam_Type::em_JoinTeam_Type_Gungoen, bWait))
			{
				if (CTeamMemberExtend::GetInstance().GetTeamMemberCount() > 0)
				{
					LOG_CF_D( L"�޸ľ�����Ʒ����ɫ����!");
					CTeamMemberExtend::GetInstance().SetAuctionQuality(em_Auction_Quality_Extreme);
					// Send Dissolution to Server
					//BnsSleep(5 * 1000);
					//CClientSocket::SendLeaveTeam(SOCK_MSG_DISSOLUTION_DUNGOEN_TEAM, m_ClientTeamInfo);
				}
				break;
			}
		}
		else
		{
			// ���Ƕ�Ա
			LOG_CF_D( L"���Ƕ�Ա, �ӳ���:%s,ID:%X", Result.Captain.wsPlayerName.c_str(), Result.Captain.dwPlayerId);
			if (JoinTeam_Member(CSocketAPI::em_JoinTeam_Type::em_JoinTeam_Type_Gungoen,Result.Captain, bWait))
				break;
		}
	}


	_TeamDungeonSchedule.dwTeamId = _dwTeamId;
	_SocketAPI.SetTeamReadonly(_dwTeamId);
#else
	LOG_CF_D( L"ע������²������!");
#endif
	return TRUE;
}

VOID CClientTeam::RefreshGunoneTeamSchedule()
{
#if BnsProject_DLL
	_SocketAPI.GetGungoenSchedule(_dwTeamId, _TeamDungeonSchedule.VecMember);
#else
	
#endif
}
