#include "stdafx.h"
#include "TaskUpgradeMgr.h"
#include <MyTools/CLPublic.h>
#include <MyTools/Log.h>
#include "BnsConfig.h"
#include "LoginPlayerExtend.h"
#include "SomeFun.h"
#include "LoginPlayer.h"
#include "ObjectFunction.h"
#include "BnsGameFun.h"
#include "Person.h"
#include "PersonFunction.h"
#include "GameFunction.h"
#include "BagFunction.h"
#include "Farm.h"
#include "MailAction.h"
#include "BagAction.h"
#include "BnsScript.h"
#include "SocketConfig.h"
#include "ConsoleLog.h"
#define _SELF L"TaskUpgradeMgr.cpp"


CTaskUpgradeMgr::CTaskUpgradeMgr() : RefBnsGameFun(CBnsGameFun::GetInstance())
{
	_IsDeletePlayerPtr = nullptr;
}

BOOL CTaskUpgradeMgr::Run_By_BlackAccount()
{
	// 进入游戏
	RefBnsGameFun.ToSeletedPlayer();
	ToSpecialMode();

	// 判断是否有>=45级的角色, 没有的话就直接跳过这个帐号
	if (!ExistMoreThanLevel45_In_Account(nullptr, [](CONST CLoginPlayer&){return TRUE; }))
	{
		RefBnsGameFun.SetLoginFaild(L"黑号模式下不存在超过45级的角色", CBnsGameFun::em_LoginFaild_DeleteAccount);
		return FALSE;
	}

	// 判断是否有超过45级的力士角色。
	CLoginPlayer LoginPlayer_;
	while (!ExistMoreThanLevel45_In_Account(&LoginPlayer_, [](CONST CLoginPlayer& LoginPlayer_){ return !LoginPlayer_.IsDeleteStatus() && LoginPlayer_.GetLoginPlayerClasses() == em_Player_Classes_KongFuForceMaster; }))
	{
		if (CObjectFunction::GetInstance().GetMaxLoginPlayerCount() == CObjectFunction::GetInstance().GetLoginPlayerCount())
		{
			RefBnsGameFun.SetLoginFaild(L"黑号模式下不存在超过45级的角色", CBnsGameFun::em_LoginFaild_DeleteAccount);
			return FALSE;
		}

		LOG_CF_D( L"黑号模式, 并没有可用的角色!");
		// 如果没45级以上的力士. 那就判断是否有45级以上的角色, 有的话就创建45级的气拳， 没有的话就创建1级的力士
		DWORD dwIndex = CObjectFunction::GetInstance().GetLoginPlayerCount();
		em_Player_Classes emPlayerClasses = em_Player_Classes_KongFuForceMaster;

		LOG_CF_D( L"黑号模式, 已经存在45级以上的力士, 那就创建气宗! Index=%d", dwIndex);
		emPlayerClasses = em_Player_Classes_KongFuForceMaster;
		CLoginPlayerExtend::GetInstance().CreatePlayer(CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.dwAreaId, dwIndex, emPlayerClasses);
	}

	// 开始进入游戏
	LOG_CF_D( L"开始游戏! 选择第%d个角色", LoginPlayer_.GetIndex());
	RefBnsGameFun.StartGame_By_Index(LoginPlayer_.GetIndex());

	LOG_CF_D( L"开始执行脚本!");
	RefBnsGameFun.WaitForThread(CBnsGameFun::em_Excute_Type_PK_TaskUpgrade);

	
	RefBnsGameFun.SetLoginFaild(L"刷完了一个角色, 换下一个帐号!", CBnsGameFun::em_LoginFaild_DeleteAccount);
	return TRUE;
}

BOOL CTaskUpgradeMgr::Run_By_WhiteAccount()
{
	RefBnsGameFun.ToSeletedPlayer();
	ToSpecialMode();

	for (;;)
	{
		// 判断是否所有角色都完成了, 是的话就换帐号
		if (GetCount_By_DeleteStatus() == CObjectFunction::GetInstance().GetMaxLoginPlayerCount())
		{
			RefBnsGameFun.SetLoginFaild(L"所有帐号都是删除状态, 完成该帐号了!", CBnsGameFun::em_LoginFaild_DeleteAccount);
			break;
		}

		// 判断是否有超过45级的力士角色。
		CLoginPlayer LoginPlayer_;
		if (!ExistMoreThanLevel45_In_Account(&LoginPlayer_, [](CONST CLoginPlayer& LoginPlayer_){ return !LoginPlayer_.IsDeleteStatus() && LoginPlayer_.GetLevel() >= 45; }))
		{
			LOG_CF_D( L"白号模式下, 并没有可用的角色!");
			while (!CLoginPlayerExtend::GetInstance().ExistStandardLoginPlayer(&LoginPlayer_))
			{
				// 如果没45级以上的力士. 那就判断是否有45级以上的角色, 有的话就创建45级的气拳， 没有的话就创建1级的力士
				DWORD dwIndex = CObjectFunction::GetInstance().GetLoginPlayerCount();
				em_Player_Classes emPlayerClasses = em_Player_Classes_Destroyer;

				if (ExistMoreThanLevel45_In_Account(nullptr, [](CONST CLoginPlayer&){ return TRUE; }))
				{
					LOG_CF_D( L"白号模式下, 已经存在45级以上的力士, 那就创建气宗! Index=%d", dwIndex);
					emPlayerClasses = em_Player_Classes_KongFuForceMaster;
				}
				else
					LOG_CF_D( L"白号模式下, 不存在45级的力士号. 那就创建力士角色! Index=%d", dwIndex);

				CLoginPlayerExtend::GetInstance().CreatePlayer(CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.dwAreaId, dwIndex, emPlayerClasses);
			}
		}
		else
			LOG_CF_D( L"白号模式下, 有存在角色, 那就进这个号!");

		// 获取进去的名称
		CONST std::wstring wsLoginPlayerName = LoginPlayer_.GetName();

		// 开始进入游戏
		LOG_CF_D( L"开始游戏! 选择第%d个角色", LoginPlayer_.GetIndex());
		RefBnsGameFun.StartGame_By_Index(LoginPlayer_.GetIndex());

		LOG_CF_D( L"开始执行脚本!");
		RefBnsGameFun.WaitForThread(CBnsGameFun::em_Excute_Type_PK_TaskUpgrade);

		// 退回选择角色!
		LOG_CF_D( L"退回选择角色!");
		RefBnsGameFun.BackToSwitchAccount();

		DeletePlayer_From_MainTask(wsLoginPlayerName);
	}
	return TRUE;
}

BOOL CTaskUpgradeMgr::ExistMoreThanLevel45_In_Account(_In_ CLoginPlayer* pLoginPlayer, _In_ std::function<BOOL(CONST CLoginPlayer&)> fnOtherCondition) CONST
{
	std::vector<CLoginPlayer> vlst;
	CObjectFunction::GetInstance().GetLoginPlayerList(vlst);

	return MyTools::CLPublic::Vec_find_if(vlst, pLoginPlayer, fnOtherCondition);
}

BOOL CTaskUpgradeMgr::IsMoreThanLevel16_In_ALL_Account() CONST
{
	std::vector<CLoginPlayer> vlst;
	// 是否帐号满了
	if (CObjectFunction::GetInstance().GetLoginPlayerList(vlst) == 0)
		return FALSE;

	for (CONST auto& itm : vlst)
	{
		if (itm.GetLevel() < 16)
			return FALSE;
	}
	return TRUE;
}

int CTaskUpgradeMgr::GetEmptyLocationCount() CONST
{
	std::vector<CLoginPlayer> vlst;
	CObjectFunction::GetInstance().GetLoginPlayerList(vlst);
	if (vlst.size() >= CObjectFunction::GetInstance().GetMaxLoginPlayerCount())
	{
		LOG_CF_D(L"CLoginPlayervlst.size=%d", vlst.size());
		return -1;
	}

	return static_cast<int>(vlst.size());
}

UINT CTaskUpgradeMgr::GetCount_By_DeleteStatus() CONST
{
	std::vector<CLoginPlayer> vlst;
	CObjectFunction::GetInstance().GetLoginPlayerList(vlst);

	UINT uCount = 0;
	for (CONST auto& itm : vlst)
	{
		uCount += itm.IsDeleteStatus() ? 1 : 0;
	}
	return uCount;
}

BOOL CTaskUpgradeMgr::Run_By_FreeAccount()
{
	RefBnsGameFun.ToSeletedPlayer();
	// 判断是否所有角色都完成了, 是的话就换帐号
	if (GetCount_By_DeleteStatus() == CObjectFunction::GetInstance().GetMaxLoginPlayerCount())
	{
		RefBnsGameFun.SetLoginFaild(L"所有帐号都是删除状态, 完成该帐号了!", CBnsGameFun::em_LoginFaild_DeleteAccount);
	}

	ToSpecialMode();
	UINT nIndex = CLoginPlayerExtend::GetInstance().GetLoginPlayerIndex();
	// 开始进入游戏
	LOG_CF_D( L"开始游戏! 选择第%d个角色", nIndex);
	for (UINT i = nIndex; i < 5; ++i)
	{
		RefBnsGameFun.StartGame_By_Index(i);

		LOG_CF_D( L"开始执行脚本!");
		RefBnsGameFun.WaitForThread(CBnsGameFun::em_Excute_Type_PK_TaskUpgrade);

		// 退回选择角色!
		LOG_CF_D( L"退回选择角色!");
		RefBnsGameFun.BackToSwitchAccount();

		CGameVariable::GetInstance().Action_Variable_By_Name(VARIABLE_NAME_FINISH_ALL_MAINTASK, TRUE, [this, &i]{
			if (CObjectFunction::GetInstance().GetMaxLoginPlayerCount() == CObjectFunction::GetInstance().GetLoginPlayerCount())
			{
				LOG_CF_D(L"角色满了!");
				return;
			}

			DWORD dwIndex = i + 1;
			em_Player_Classes emPlayerClasses = em_Player_Classes_Destroyer;

			if (ExistMoreThanLevel45_In_Account(nullptr, [](CONST CLoginPlayer&){ return TRUE; }))
			{
				LOG_CF_D( L"白号模式下, 已经存在45级以上的力士, 那就创建气宗! Index=%d", dwIndex);
				emPlayerClasses = em_Player_Classes_KongFuForceMaster;
			}
			else
				LOG_CF_D( L"白号模式下, 不存在45级的力士号. 那就创建力士角色! Index=%d", dwIndex);

			CLoginPlayerExtend::GetInstance().CreatePlayer(CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.dwAreaId, dwIndex, emPlayerClasses);
		});
	}

	return TRUE;
}

BOOL CTaskUpgradeMgr::Run_By_DesocoryAccount()
{
	RefBnsGameFun.ToSeletedPlayer();
	ToSpecialMode();

	for (;;)
	{
		// 判断是否所有角色都完成了, 是的话就换帐号
		if (GetCount_By_DeleteStatus() == CObjectFunction::GetInstance().GetMaxLoginPlayerCount())
		{
			//CBnsConfig::GetInstance().WriteAccountLog( L"力士模式:当前帐号所有角色都是删除状态, 换下一个帐号!");
			//LOG_CF_D( L"所有帐号都是删除状态, 完成该帐号了!");
			//SetLoginFaild(CBnsGameFun::em_LoginFaild_DeleteAccount);
			CConsoleLog(L"等待删除角色ing!!!");
			::Sleep(5 * 1000);
			continue;
		}

		// 判断是否有超过45级的力士角色。
		CLoginPlayer LoginPlayer_;
		if (!ExistMoreThanLevel45_In_Account(&LoginPlayer_, [](CONST CLoginPlayer& LoginPlayer_){ return !LoginPlayer_.IsDeleteStatus(); }))
		{
			LOG_CF_D( L"白号模式下, 并没有可用的角色!");
			while (!CLoginPlayerExtend::GetInstance().ExistStandardLoginPlayer(&LoginPlayer_))
			{
				// 如果没45级以上的力士. 那就判断是否有45级以上的角色, 有的话就创建45级的气拳， 没有的话就创建1级的力士
				DWORD dwIndex = CObjectFunction::GetInstance().GetLoginPlayerCount();
				em_Player_Classes emPlayerClasses = em_Player_Classes_Destroyer;
				LOG_CF_D( L"白号模式下, 不存在45级的力士号. 那就创建力士角色! Index=%d", dwIndex);
				CLoginPlayerExtend::GetInstance().CreatePlayer(CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.dwAreaId, dwIndex, emPlayerClasses);
			}
		}
		else
			LOG_CF_D( L"白号模式下, 有存在角色, 那就进这个号!");

		// 获取进去的名称
		CONST std::wstring wsLoginPlayerName = LoginPlayer_.GetName();

		// 开始进入游戏
		LOG_CF_D( L"开始游戏! 选择第%d个角色", LoginPlayer_.GetIndex());
		RefBnsGameFun.StartGame_By_Index(LoginPlayer_.GetIndex());

		LOG_CF_D( L"开始执行脚本!");
		RefBnsGameFun.WaitForThread(CBnsGameFun::em_Excute_Type_PK_TaskUpgrade);

		// 退回选择角色!
		LOG_CF_D( L"退回选择角色!");
		RefBnsGameFun.BackToSwitchAccount();

		DeletePlayer_From_MainTask(wsLoginPlayerName);
	}
}

BOOL CTaskUpgradeMgr::Run_By_DeleteAccount()
{
	RefBnsGameFun.ToSeletedPlayer();

	LOG_CF_D( L"删除数量:%d, 当前数量:%d", GetCount_By_DeleteStatus(), CObjectFunction::GetInstance().GetLoginPlayerCount());
	CLoginPlayerExtend::GetInstance().DeletePlayer_By_Condition([](CONST CLoginPlayer&){ return TRUE; });
	
	RefBnsGameFun.SetLoginFaild(L"所有帐号都是删除状态, 完成该帐号了!", CBnsGameFun::em_LoginFaild_DeleteAccount);

	return TRUE;
}

BOOL CTaskUpgradeMgr::Run_By_DeleteAccount2()
{
	RefBnsGameFun.ToSeletedPlayer();
	ToSpecialMode();
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NMAE_JUDGE_MAIL_TITLE, TRUE);
	LOG_CF_D( L"当前需要删除数量:%d, 当前角色数量:%d", GetCount_By_DeleteStatus(), CObjectFunction::GetInstance().GetLoginPlayerCount());
	CLoginPlayerExtend::GetInstance().DeletePlayer_By_Condition([](CONST CLoginPlayer&){ return TRUE; });
	Sleep(5000);
	std::vector<CLoginPlayer> vlst;
	CObjectFunction::GetInstance().GetLoginPlayerList(vlst);
	LOG_CF_D( L"删号模式2:当前帐号还有未删除的角色%d,%d", GetCount_By_DeleteStatus(),vlst.size());

	for (CONST auto& itm : vlst)
	{
		LOG_CF_D( L"当前角色名是%s,删除状态是%d", itm.GetName().c_str(), itm.IsDeleteStatus());
		if (!itm.IsDeleteStatus())
		{
			// 开始进入游戏
			LOG_CF_D( L"开始游戏! 选择第%d个角色", itm.GetIndex());
			RefBnsGameFun.StartGame_By_Index(itm.GetIndex());


			LOG_CF_D( L"收邮件");
			CMailAction::GetInstance().RecvMail();
			Sleep(5000);
			LOG_CF_D( L"退出门派");
			CPersonFunction::GetInstance().ExitSchool();
			Sleep(5000);

			// 退回选择角色!
			LOG_CF_D( L"退回选择角色!");
			RefBnsGameFun.BackToSwitchAccount();

			//
			LOG_CF_D( L"删除角色:%s, 等级:%d", itm.GetName().c_str(), itm.GetLevel());
			CLoginPlayerExtend::GetInstance().DeletePlayer(itm.GetIndex(), 3000);
		}
	}

	LOG_CF_D( L"当前帐号角色删除状态有%d个", GetCount_By_DeleteStatus());

	if (GetCount_By_DeleteStatus() < CObjectFunction::GetInstance().GetMaxLoginPlayerCount())
		RefBnsGameFun.SetLoginFaild(L"所有帐号都是删除状态, 完成该帐号了!", CBnsGameFun::em_LoginFaild_DeleteAccount);
	else
		RefBnsGameFun.SetLoginFaild(L"删号模式:当前帐号还有未删除的角色", CBnsGameFun::em_LoginFaild_DeleteAccount);

	return TRUE;
}

BOOL CTaskUpgradeMgr::Run_By_FreeWhiteAccount()
{
	RefBnsGameFun.ToSeletedPlayer();
	// 判断是否所有角色都完成了, 是的话就换帐号
	if (GetCount_By_DeleteStatus() == CObjectFunction::GetInstance().GetMaxLoginPlayerCount())
	{
		RefBnsGameFun.SetLoginFaild(L"白号力士免费模式:当前帐号所有角色都是删除状态, 换下一个帐号!", CBnsGameFun::em_LoginFaild_DeleteAccount);
		return TRUE;
	}

	// 进入特殊模式
	ToSpecialMode();

	// 判断是否有超过45级的力士角色。
	CLoginPlayer LoginPlayer_;
	if (!ExistMoreThanLevel45_In_Account(&LoginPlayer_, [](CONST CLoginPlayer& LoginPlayer_){ return !LoginPlayer_.IsDeleteStatus() && LoginPlayer_.GetLevel() >= 45; }))
	{
		RefBnsGameFun.SetLoginFaild(L"白号力士免费模式:该帐号不存在45级以上的力士号, 换下一个帐号!", CBnsGameFun::em_LoginFaild_DeleteAccount);
		return TRUE;
	}

	LOG_CF_D( L"白号模式下, 有存在角色, 那就进这个号!");
	// 获取进去的名称
	CONST std::wstring wsLoginPlayerName = LoginPlayer_.GetName();

	// 开始进入游戏
	LOG_CF_D( L"开始游戏! 选择第%d个角色", LoginPlayer_.GetIndex());
	RefBnsGameFun.StartGame_By_Index(LoginPlayer_.GetIndex());

	LOG_CF_D( L"开始执行脚本!");
	RefBnsGameFun.WaitForThread(CBnsGameFun::em_Excute_Type_PK_TaskUpgrade);

	RefBnsGameFun.SetLoginFaild(L"白号力士免费模式:刷完, 换下一个帐号!", CBnsGameFun::em_LoginFaild_DeleteAccount);
	return TRUE;
}

BOOL CTaskUpgradeMgr::Run_By_AddSkillAccount()
{
	RefBnsGameFun.ToSeletedPlayer();
	// 判断是否所有角色都完成了, 是的话就换帐号
	if (GetCount_By_DeleteStatus() == CObjectFunction::GetInstance().GetMaxLoginPlayerCount())
	{
		RefBnsGameFun.SetLoginFaild(L"所有帐号都是删除状态, 完成该帐号了!", CBnsGameFun::em_LoginFaild_DeleteAccount);
		return TRUE;
	}

	// 开始进入游戏
	RefBnsGameFun.StartGame_By_Index(CLoginPlayerExtend::GetInstance().GetLoginPlayerIndex());

	LOG_CF_D( L"开始执行脚本!");
	RefBnsGameFun.WaitForThread(CBnsGameFun::em_Excute_Type_PK_TaskUpgrade);
	RefBnsGameFun.SetLoginFaild(L"加点模式:刷完, 换下一个帐号!", CBnsGameFun::em_LoginFaild_DeleteAccount);
	return TRUE;
}

BOOL CTaskUpgradeMgr::Run_By_DeletePlayer()
{
	RefBnsGameFun.ToSeletedPlayer();
	RefBnsGameFun.WaitForThread(CBnsGameFun::em_Excute_Type_DeletePlayer);
	::Sleep(INFINITE);
	return TRUE;
}

BOOL CTaskUpgradeMgr::IsExistWarehouse_By_LoginPlayer(_Out_opt_ CLoginPlayer* pLoginPlayer) CONST
{
	std::vector<CLoginPlayer> vlst;
	CObjectFunction::GetInstance().GetLoginPlayerList(vlst);

	return MyTools::CLPublic::Vec_find_if(vlst, pLoginPlayer,[](CONST CLoginPlayer& LoginPlayer){return CGameVariable::GetInstance().ExistSpicalName(LoginPlayer.GetName()) == CGameVariable::em_SpicalName_Type_Warehouse; });
}

BOOL CTaskUpgradeMgr::ToWarehouseMode() CONST
{
	CConsoleLog(L"进入仓库号模式!");
	CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->GameCmd = em_GameCmd_Start;
	MyTools::CLog::GetInstance().SetSyncSendLog();
	for (;;)
	{
		::Sleep(1000);
		if (CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->GameCmd != em_GameCmd_Start)
			continue;

		CLoginPlayer WarehouseLoginPlayer;
		if (!IsExistWarehouse_By_LoginPlayer(&WarehouseLoginPlayer))
		{
			LOG_MSG_CF( L"貌似堆栈溢出了! 竟然");
			return FALSE;
		}
		RefBnsGameFun.StartGame_By_Index(WarehouseLoginPlayer.GetIndex());

		// 设置不检查坐标超时!
		CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_WAREHOUSE_MODE, TRUE);
		CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CHECKPAUSEPOINT, FALSE);
		RefBnsGameFun.WaitForThread(CBnsGameFun::em_Excute_Type_Warehouse);

		LOG_CF_D( L"退回选择角色!");
		RefBnsGameFun.BackToSwitchAccount();
	}
}

BOOL CTaskUpgradeMgr::Run_By_SummonerAccount()
{
	RefBnsGameFun.ToSeletedPlayer();
	ToSpecialMode();

	DeletePlayer_From_OtherClasses();
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NMAE_DELETE_ALL_PLAYER, TRUE);
	for (;;)
	{
		CGameVariable::GetInstance().GetAccShareInfo()->AccountStatus.emLoginStep = em_Login_Step::em_Login_Step_ChoicePlayer;

		// 判断是否所有角色都完成了, 是的话就换帐号
		if (GetCount_By_DeleteStatus() == CObjectFunction::GetInstance().GetMaxLoginPlayerCount())
		{
			CConsoleLog(L"等待删除角色ing!!!");
			::Sleep(5 * 1000);
			continue;
		}

		std::vector<CLoginPlayer> VecLoginPlayer;
		CObjectFunction::GetInstance().GetLoginPlayerList(VecLoginPlayer);
		for (CONST auto& itm : VecLoginPlayer)
		{
			LOG_CF_D( L"Full LoginPlayer: Index=%d, Classes=%s, Level=%d, Server=%d, Name=%s, IsDelete=%d", \
				itm.GetIndex(), CPlayer::GetTextByPlayerClasses(itm.GetLoginPlayerClasses()).c_str(), itm.GetLevel(), itm.GetServerId(), itm.GetName().c_str(), itm.IsDeleteStatus());
		}

		MyTools::CLPublic::Vec_erase_if(VecLoginPlayer, [](CONST CLoginPlayer& LoginPlayer_){ return LoginPlayer_.IsDeleteStatus() || LoginPlayer_.GetLoginPlayerClasses() != em_Player_Classes::em_Player_Classes_Summoner; });
		for (CONST auto& itm : VecLoginPlayer)
		{
			LOG_CF_D( L"erase LoginPlayer: Index=%d, Classes=%s, Level=%d, Server=%d, Name=%s", \
				itm.GetIndex(), CPlayer::GetTextByPlayerClasses(itm.GetLoginPlayerClasses()).c_str(), itm.GetLevel(), itm.GetServerId(), itm.GetName().c_str());
		}

		for (auto& itm : VecLoginPlayer)
			itm.SetFinishMainTask(CBnsConfig::GetInstance().GetSingleConfigValueByServer(itm.GetName(), L"完成主线") == L"1" ? TRUE : FALSE);

		// 判断是否完成了帐号
		MyTools::CLPublic::Vec_erase_if(VecLoginPlayer, [](CONST CLoginPlayer& LoginPlayer_){ return LoginPlayer_.GetFinishMainTask(); });
		for (CONST auto& itm : VecLoginPlayer)
		{
			LOG_CF_D( L"Remain LoginPlayer: Index=%d, Classes=%s, Level=%d, Server=%d, Name=%s", \
				itm.GetIndex(), CPlayer::GetTextByPlayerClasses(itm.GetLoginPlayerClasses()).c_str(), itm.GetLevel(), itm.GetServerId(), itm.GetName().c_str());
		}

		// 判断是否有超过45级的召唤角色。
		CLoginPlayer LoginPlayer_;
		if (VecLoginPlayer.size() == 0)
		{
			LOG_CF_D( L"召唤模式下, 并没有可用的角色!");

			DWORD dwIndex = CObjectFunction::GetInstance().GetLoginPlayerCount();
			if (dwIndex == CObjectFunction::GetInstance().GetMaxLoginPlayerCount())
			{
				// 必须是5个角色都刷完才删除
				//LOG_CF_D( L"当前所有角色都满了!, 完成该帐号了!");
				//CLoginPlayerExtend::GetInstance().DeletePlayer(0, 3 * 1000);
				CConsoleLog(L"所有角色都满了!没有可用的角色!");
				::Sleep(INFINITE);
				/*while (CObjectFunction::GetInstance().GetLoginPlayerCount() == dwIndex)
				{
					CBnsConfig::GetInstance().WriteAccountLog(L"等待删除角色中……");
					::Sleep(5 * 1000);
				}*/
			}

			dwIndex = CObjectFunction::GetInstance().GetLoginPlayerCount();
			LOG_CF_D( L"召唤模式下, 不存在45级的召唤号. 那就创建召唤角色! Index=%d", dwIndex);
			CLoginPlayerExtend::GetInstance().CreatePlayer(CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.dwAreaId, dwIndex, em_Player_Classes::em_Player_Classes_Summoner, FALSE);
			if (CObjectFunction::GetInstance().GetLoginPlayerCount() == dwIndex)
				continue;

			if (CObjectFunction::GetInstance().GetLoginPlayerList(VecLoginPlayer) != dwIndex + 1)
				continue;
			MyTools::CLPublic::SetPtr(&LoginPlayer_, VecLoginPlayer.at(dwIndex));
		}
		else
		{
			LoginPlayer_ = VecLoginPlayer.at(0);
			LOG_CF_D( L"召唤模式下, 有存在角色%s, 等级:%d, 那就进这个号!", LoginPlayer_.GetName().c_str(), LoginPlayer_.GetLevel());
		}

		LOG_CF_D( L"召唤模式下%d", LoginPlayer_.GetServerId());

		if (LoginPlayer_.GetServerId() != CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.dwAreaId)
		{
			RefBnsGameFun.SetLoginFaild(MyTools::CCharacter::MakeFormatText(L"%s:与本机SID:%X 不同", 
				LoginPlayer_.GetName().c_str(), 
				CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.dwAreaId), 
				CBnsGameFun::em_LoginFaild_DeleteAccount);
		}

		// 获取进去的名称
		CONST std::wstring wsLoginPlayerName = LoginPlayer_.GetName();

		// 开始进入游戏
		LOG_CF_D( L"开始游戏! 选择第%d个角色", LoginPlayer_.GetIndex());
		RefBnsGameFun.StartGame_By_Index(LoginPlayer_.GetIndex());

		LOG_CF_D( L"开始执行脚本!");
		RefBnsGameFun.WaitForThread(CBnsGameFun::em_Excute_Type_PK_TaskUpgrade);
		if (CGameVariable::GetInstance().GetAccShareInfo()->AccountStatus.bClose)
		{
			LOG_CF_D( L"bClose=TRUE; 等待客户端结束!");
			::Sleep(INFINITE);
		}

		// 退回选择角色!
		LOG_CF_D( L"退回选择角色!");
		RefBnsGameFun.BackToSwitchAccount();
		DeletePlayer_From_MainTask(wsLoginPlayerName);
		
		CGameVariable::GetInstance().Action_Variable_By_Name(VARIABLE_NAME_SWITCHACCOUNT_MODE, TRUE, [this]
		{
			RefBnsGameFun.SetLoginFaild(L"召唤换号模式: 单角色完成主线, 换下一个帐号!", CBnsGameFun::em_LoginFaild_DeleteAccount);
		});

		CGameVariable::GetInstance().Action_Variable_By_Name(VARIABLE_NAME_DELETEACCOUNT, TRUE, [this]
		{
			RefBnsGameFun.SetLoginFaild(L"删号=TRUE", CBnsGameFun::em_LoginFaild_DeleteAccount);
		});
	}
}

BOOL CTaskUpgradeMgr::Run_By_SummonerAccount2()
{
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_DISABLE_DELETEPLAYER, TRUE);
	return Run_By_SummonerAccount();
}

BOOL CTaskUpgradeMgr::RUn_By_SummonerAccount_Capter7()
{
	return Run_By_SummonerAccount();
}

BOOL CTaskUpgradeMgr::Run_By_SummonerAccount1_Level50()
{
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_SUMMONER_LEVEL50_MODE, 0x1);
	return Run_By_SummonerAccount_Level50();
}

BOOL CTaskUpgradeMgr::Run_By_SummonerAccount_Level50()
{
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_DISABLE_DELETEPLAYER, TRUE);
	RefBnsGameFun.ToSeletedPlayer();
	ToSpecialMode();

	if (!IsExistPlayerInLevel50Mode())
	{
		RefBnsGameFun.SetLoginFaild(L"召唤50级模式:所有角色都<50级并且>16级,放弃这个号!", CBnsGameFun::em_LoginFaild_DeleteAccount);
	}

	LOG_CF_D( L"召唤50级模式删除所有16级以下的号!");
	CLoginPlayerExtend::GetInstance().DeletePlayer_By_Condition([](CONST CLoginPlayer& LoginPlayer){ return !LoginPlayer.IsDeleteStatus() && LoginPlayer.GetLevel() <= 16; });

	for (;;)
	{
		std::vector<CLoginPlayer> VecLoginPlayer;
		CObjectFunction::GetInstance().GetLoginPlayerList(VecLoginPlayer);
		MyTools::CLPublic::Vec_erase_if(VecLoginPlayer, [](CONST CLoginPlayer& LoginPlayer_){ return LoginPlayer_.IsDeleteStatus() || LoginPlayer_.GetLevel() < 50 || LoginPlayer_.GetLoginPlayerClasses() != em_Player_Classes::em_Player_Classes_Summoner; });

		for (auto& itm : VecLoginPlayer)
			itm.SetFinishMainTask(CBnsConfig::GetInstance().GetSingleConfigValueByServer(itm.GetName(), L"50级召唤模式完成主线") == L"1" ? TRUE : FALSE);

		// 召唤50级模式1, 只要有刷完号的, 就直接怒换下一个帐号
		CGameVariable::GetInstance().Action_Variable_By_Name(VARIABLE_NAME_SUMMONER_LEVEL50_MODE, 0x1, [&VecLoginPlayer, this]
		{
			if (MyTools::CLPublic::Vec_find_if(VecLoginPlayer, [](CONST CLoginPlayer& LoginPlayer_){ return LoginPlayer_.GetFinishMainTask(); }) != nullptr)
			{
				RefBnsGameFun.SetLoginFaild(L"召唤50级模式1:刷完, 换下一个帐号!", CBnsGameFun::em_LoginFaild_DeleteAccount);
			}
		});

		// 先判断是否有50级以上的角色是没完成主线的
		if (MyTools::CLPublic::Vec_find_if(VecLoginPlayer, [](CONST CLoginPlayer& LoginPlayer_){ return !LoginPlayer_.GetFinishMainTask(); }) == nullptr)
		{
			LOG_CF_D( L"不存在50级以上的召唤号是完成主线状态的, 判断是否有等级劵");
			// 判断是否完成了帐号
			if (!CLoginPlayerExtend::GetInstance().ExistLevelTick())
			{
				ActionForUnExistLevelTick(VecLoginPlayer);
			}
		}

		MyTools::CLPublic::Vec_erase_if(VecLoginPlayer, [](CONST CLoginPlayer& LoginPlayer_){ return !LoginPlayer_.GetFinishMainTask(); });

		// 判断是否有超过45级的召唤角色。
		CLoginPlayer LoginPlayer_;
		if (!ExistMoreThanLevel45_In_Account(&LoginPlayer_, [VecLoginPlayer](CONST CLoginPlayer& LoginPlayer_)
		{  
			// 删除状态 和 <50级的不要
			if (LoginPlayer_.IsDeleteStatus() || LoginPlayer_.GetLevel() < 50 || LoginPlayer_.GetLoginPlayerClasses() != em_Player_Classes::em_Player_Classes_Summoner)
				return FALSE;

			// 没有一个帐号完成了主线
			if (VecLoginPlayer.size() == 0)
				return TRUE;

			// VecLoginPlayer必定size=1, >= 2已经在上面排除了
			return LoginPlayer_.GetIndex() != VecLoginPlayer.begin()->GetIndex() ? TRUE : FALSE;
		}))
		{
			DWORD dwIndex = CObjectFunction::GetInstance().GetLoginPlayerCount();
			while (dwIndex == CObjectFunction::GetInstance().GetMaxLoginPlayerCount())
			{
				CConsoleLog(L"50级召唤模式下满角色了……等待删号!");
				BnsSleep(10 * 1000);
				dwIndex = CObjectFunction::GetInstance().GetLoginPlayerCount();
			}

			LOG_CF_D( L"召唤模式下, 不存在50级的召唤号. 那就创建召唤角色! Index=%d", dwIndex);
			if (!CLoginPlayerExtend::GetInstance().IsUseLevelTick())
			{
				LOG_CF_D( L"使用劵!");
				CLoginPlayerExtend::GetInstance().UseKfmVolume();
			}
			
			::Sleep(3 * 1000);
			UINT uCount = CObjectFunction::GetInstance().GetLoginPlayerCount();
			CLoginPlayerExtend::GetInstance().CreatePlayer(CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.dwAreaId, dwIndex, em_Player_Classes::em_Player_Classes_Summoner, TRUE);
			if (CObjectFunction::GetInstance().GetLoginPlayerCount() == uCount)
				continue;

			if(CObjectFunction::GetInstance().GetLoginPlayerList(VecLoginPlayer) != uCount + 1)
				continue;
			MyTools::CLPublic::SetPtr(&LoginPlayer_, VecLoginPlayer.at(uCount));
		}
		CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_DISABLE_SWITCHVIEW, TRUE);
		LOG_CF_D( L"召唤模式下, 有存在角色%s, 等级:%d, 那就进这个号!", LoginPlayer_.GetName().c_str(), LoginPlayer_.GetLevel());

		if (LoginPlayer_.GetServerId() != CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.dwAreaId)
		{
			RefBnsGameFun.SetLoginFaild(MyTools::CCharacter::MakeFormatText(L"%s:与本机SID:%X 不同", 
				LoginPlayer_.GetName().c_str(), 
				CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.dwAreaId), 
				CBnsGameFun::em_LoginFaild_DeleteAccount);
		}

		// 获取进去的名称
		CONST std::wstring wsLoginPlayerName = LoginPlayer_.GetName();

		// 开始进入游戏
		LOG_CF_D( L"开始游戏! 选择第%d个角色", LoginPlayer_.GetIndex());
		RefBnsGameFun.StartGame_By_Index(LoginPlayer_.GetIndex());

		LOG_CF_D( L"开始执行脚本!");
		RefBnsGameFun.WaitForThread(CBnsGameFun::em_Excute_Type_PK_TaskUpgrade);

		// 退回选择角色!
		LOG_CF_D( L"退回选择角色!bClose=%X", CGameVariable::GetInstance().GetAccShareInfo()->AccountStatus.bClose);
		RefBnsGameFun.BackToSwitchAccount();

		CGameVariable::GetInstance().Action_Variable_By_Name(VARIABLE_NAME_FINISH_ALL_MAINTASK, TRUE, [wsLoginPlayerName, this]
		{
			CBnsConfig::BnsConfigContent ConfigContent_;
			ConfigContent_.wsConfigName = L"50级召唤模式完成主线";
			ConfigContent_.wsConfigValue = L"1";
			CBnsConfig::GetInstance().SetSingleConfigValueByServer(ConfigContent_);

			WriteDeletePlayerLog(wsLoginPlayerName);
		});

		CGameVariable::GetInstance().Action_Variable_By_Name(VARIABLE_NAME_DELETEACCOUNT, TRUE, [this]
		{
			RefBnsGameFun.SetLoginFaild(L"删号=TRUE", CBnsGameFun::em_LoginFaild_DeleteAccount);
		});
	}
}

BOOL CTaskUpgradeMgr::Run_By_SummonerAccount_Speed_Level50()
{
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_DISABLE_DELETEPLAYER, TRUE);
	RefBnsGameFun.ToSeletedPlayer();
	ToSpecialMode();

	if (!IsExistPlayerInLevel50Mode())
	{
		RefBnsGameFun.SetLoginFaild(L"召唤50级模式:所有角色都<50级并且>16级,放弃这个号!", CBnsGameFun::em_LoginFaild_DeleteAccount);
	}

	LOG_CF_D( L"召唤50级模式删除所有16级以下的号!");
	CLoginPlayerExtend::GetInstance().DeletePlayer_By_Condition([](CONST CLoginPlayer& LoginPlayer){ return !LoginPlayer.IsDeleteStatus() && LoginPlayer.GetLevel() <= 16; });

	for (;;)
	{
		std::vector<CLoginPlayer> VecLoginPlayer;
		CObjectFunction::GetInstance().GetLoginPlayerList(VecLoginPlayer);
		MyTools::CLPublic::Vec_erase_if(VecLoginPlayer, [](CONST CLoginPlayer& LoginPlayer_){ return LoginPlayer_.IsDeleteStatus() || LoginPlayer_.GetLevel() < 50 || LoginPlayer_.GetLoginPlayerClasses() != em_Player_Classes::em_Player_Classes_Summoner; });

		for (auto& itm : VecLoginPlayer)
			itm.SetFinishMainTask(CBnsConfig::GetInstance().GetSingleConfigValueByServer(itm.GetName(), L"50级召唤模式完成主线") == L"1" ? TRUE : FALSE);

		// 召唤50级模式1, 只要有刷完号的, 就直接怒换下一个帐号
		CGameVariable::GetInstance().Action_Variable_By_Name(VARIABLE_NAME_SUMMONER_LEVEL50_MODE, 0x1, [&VecLoginPlayer, this]
		{
			if (MyTools::CLPublic::Vec_find_if(VecLoginPlayer, [](CONST CLoginPlayer& LoginPlayer_){ return LoginPlayer_.GetFinishMainTask(); }) != nullptr)
			{
				RefBnsGameFun.SetLoginFaild(L"召唤50级模式1:刷完, 换下一个帐号!", CBnsGameFun::em_LoginFaild_DeleteAccount);
			}
		});

		// 先判断是否有50级以上的角色是没完成主线的
		if (MyTools::CLPublic::Vec_find_if(VecLoginPlayer, [](CONST CLoginPlayer& LoginPlayer_){ return !LoginPlayer_.GetFinishMainTask(); }) == nullptr)
		{
			LOG_CF_D( L"不存在50级以上的召唤号是完成主线状态的, 判断是否有等级劵");
			// 判断是否完成了帐号
			if (!CLoginPlayerExtend::GetInstance().ExistLevelTick())
			{
				ActionForUnExistLevelTick(VecLoginPlayer);
			}
		}

		MyTools::CLPublic::Vec_erase_if(VecLoginPlayer, [](CONST CLoginPlayer& LoginPlayer_){ return !LoginPlayer_.GetFinishMainTask(); });

		// 判断是否有超过45级的召唤角色。
		CLoginPlayer LoginPlayer_;
		if (!ExistMoreThanLevel45_In_Account(&LoginPlayer_, [VecLoginPlayer](CONST CLoginPlayer& LoginPlayer_)
		{
			// 删除状态 和 <50级的不要
			if (LoginPlayer_.IsDeleteStatus() || LoginPlayer_.GetLevel() < 50 || LoginPlayer_.GetLoginPlayerClasses() != em_Player_Classes::em_Player_Classes_Summoner)
				return FALSE;

			// 没有一个帐号完成了主线
			if (VecLoginPlayer.size() == 0)
				return TRUE;

			// VecLoginPlayer必定size=1, >= 2已经在上面排除了
			return LoginPlayer_.GetIndex() != VecLoginPlayer.begin()->GetIndex() ? TRUE : FALSE;
		}))
		{
			DWORD dwIndex = CObjectFunction::GetInstance().GetLoginPlayerCount();
			while (dwIndex == CObjectFunction::GetInstance().GetMaxLoginPlayerCount())
			{
				CConsoleLog(L"50级召唤模式下满角色了……等待删号!");
				BnsSleep(10 * 1000);
				dwIndex = CObjectFunction::GetInstance().GetLoginPlayerCount();
			}

			LOG_CF_D( L"召唤模式下, 不存在50级的召唤号. 那就创建召唤角色! Index=%d", dwIndex);
			if (!CLoginPlayerExtend::GetInstance().IsUseLevelTick())
			{
				LOG_CF_D( L"使用劵!");
				CLoginPlayerExtend::GetInstance().UseKfmVolume();
			}

			::Sleep(3 * 1000);
			UINT uCount = CObjectFunction::GetInstance().GetLoginPlayerCount();
			CLoginPlayerExtend::GetInstance().CreatePlayer(CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.dwAreaId, dwIndex, em_Player_Classes::em_Player_Classes_Summoner, TRUE);
			if (CObjectFunction::GetInstance().GetLoginPlayerCount() == uCount)
				continue;

			if (CObjectFunction::GetInstance().GetLoginPlayerList(VecLoginPlayer) != uCount + 1)
				continue;
			MyTools::CLPublic::SetPtr(&LoginPlayer_, VecLoginPlayer.at(uCount));
		}
		CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_DISABLE_SWITCHVIEW, TRUE);
		LOG_CF_D( L"召唤模式下, 有存在角色%s, 等级:%d, 那就进这个号!", LoginPlayer_.GetName().c_str(), LoginPlayer_.GetLevel());

		if (LoginPlayer_.GetServerId() != CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.dwAreaId)
		{
			RefBnsGameFun.SetLoginFaild(MyTools::CCharacter::MakeFormatText(L"%s:与本机SID:%X 不同", 
				LoginPlayer_.GetName().c_str(), 
				CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.dwAreaId), 
				CBnsGameFun::em_LoginFaild_DeleteAccount);
		}

		// 获取进去的名称
		CONST std::wstring wsLoginPlayerName = LoginPlayer_.GetName();

		// 开始进入游戏
		LOG_CF_D( L"开始游戏! 选择第%d个角色", LoginPlayer_.GetIndex());
		RefBnsGameFun.StartGame_By_Index(LoginPlayer_.GetIndex());

		LOG_CF_D( L"开始执行脚本!");
		RefBnsGameFun.WaitForThread(CBnsGameFun::em_Excute_Type_PK_TaskUpgrade);

		// 退回选择角色!
		LOG_CF_D( L"退回选择角色!bClose=%X", CGameVariable::GetInstance().GetAccShareInfo()->AccountStatus.bClose);
		RefBnsGameFun.BackToSwitchAccount();

		CGameVariable::GetInstance().Action_Variable_By_Name(VARIABLE_NAME_FINISH_ALL_MAINTASK, TRUE, [wsLoginPlayerName, this]
		{
			CBnsConfig::BnsConfigContent ConfigContent_;
			ConfigContent_.wsConfigName = L"50级召唤模式完成主线";
			ConfigContent_.wsConfigValue = L"1";
			CBnsConfig::GetInstance().SetSingleConfigValueByServer(ConfigContent_);

			WriteDeletePlayerLog(wsLoginPlayerName);
		});

		CGameVariable::GetInstance().Action_Variable_By_Name(VARIABLE_NAME_DELETEACCOUNT, TRUE, [this]
		{
			RefBnsGameFun.SetLoginFaild(L"删号=TRUE", CBnsGameFun::em_LoginFaild_DeleteAccount);
		});
	}
}

VOID CTaskUpgradeMgr::ActionForUnExistLevelTick(_In_ CONST std::vector<CLoginPlayer>& VecLoginPlayer) CONST
{
	if (VecLoginPlayer.size() == 0)
		RefBnsGameFun.SetLoginFaild(MyTools::CCharacter::MakeFormatText(L"%s:该号应该是没领劵", CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.wszTypeName), CBnsGameFun::em_LoginFaild_DeleteAccount);
	else
		RefBnsGameFun.SetLoginFaild(MyTools::CCharacter::MakeFormatText(L"%s:刷完, 换下一个帐号!", CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.wszTypeName), CBnsGameFun::em_LoginFaild_DeleteAccount);
}

BOOL CTaskUpgradeMgr::Run_By_SummonerSwitchAccount()
{
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_SWITCHACCOUNT_MODE, TRUE);
	return Run_By_SummonerAccount();
}

BOOL CTaskUpgradeMgr::Run_By_ExcuteScript()
{
	return RefBnsGameFun.ExcuteScript_Type();
}

BOOL CTaskUpgradeMgr::Run_By_CreatePlayer()
{
	RefBnsGameFun.ToSeletedPlayer();
	ToSpecialMode();

	for (;;)
	{
		if (!CLoginPlayerExtend::GetInstance().ExistLevelTick())
		{
			std::vector<CLoginPlayer> VecLoginPlayer;
			CObjectFunction::GetInstance().GetLoginPlayerList(VecLoginPlayer);
			MyTools::CLPublic::Vec_erase_if(VecLoginPlayer, [](CONST CLoginPlayer& LoginPlayer_){ return LoginPlayer_.IsDeleteStatus() || LoginPlayer_.GetLevel() < 50 || LoginPlayer_.GetLoginPlayerClasses() != em_Player_Classes::em_Player_Classes_Summoner; });
			ActionForUnExistLevelTick(VecLoginPlayer);
		}

		LOG_CF_D( L"使用劵!");
		CLoginPlayerExtend::GetInstance().UseKfmVolume();
		::Sleep(3 * 1000);
		CLoginPlayerExtend::GetInstance().CreatePlayer(CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.dwAreaId, CObjectFunction::GetInstance().GetLoginPlayerCount(), em_Player_Classes::em_Player_Classes_Summoner, TRUE);
	}
}

BOOL CTaskUpgradeMgr::WriteDeletePlayerLog(_In_ CONST std::wstring& wsPlayerName) CONST
{
	SYSTEMTIME CurrentTime;
	::GetLocalTime(&CurrentTime);

	// 写入到Server
	CBnsConfig::GetInstance().SaveLocalTimeConfigToServer(wsPlayerName, L"角色完成时间");

	// 获取角色完成时间
	
	WCHAR wszText[MAX_PATH] = { 0 };
	auto wsCreatePlayerTime = CBnsConfig::GetInstance().GetSingleConfigValueByServer(wsPlayerName, L"角色创建时间");
	if (wsCreatePlayerTime == L"")
	{
		LOG_CF_E( L"无法获取Server的角色创建时间! 可能是中途转过来的号!");
		swprintf_s(wszText, _countof(wszText), L"无法获取服务器上的角色创建时间, 无法计算时长, 完成时间是:%04d-%02d-%02d %02d:%02d:%02d",
			CurrentTime.wYear, CurrentTime.wMonth, CurrentTime.wDay, CurrentTime.wHour, CurrentTime.wMinute, CurrentTime.wSecond);
	}
	else
	{
		SYSTEMTIME SysTimeCreatePlayer = CGameVariable::ConvertTextToSysTime(wsCreatePlayerTime);
		if (SysTimeCreatePlayer.wDay == NULL)
		{
			LOG_CF_E(L"获取服务器的创建日期格式错误,返回的是:%s, 无法计算时长, 完成时间是:%04d-%02d-%02d %02d:%02d:%02d", 
				wsCreatePlayerTime.c_str(), 
				CurrentTime.wYear, 
				CurrentTime.wMonth, 
				CurrentTime.wDay, 
				CurrentTime.wHour, 
				CurrentTime.wMinute, 
				CurrentTime.wSecond);
		}

		// 获取删除完毕的时间
		swprintf_s(wszText, 
			_countof(wszText) - 1, 
			L"帐号完成日期:%04d-%02d-%02d %02d:%02d:%02d, 创建日期:%04d-%02d-%02d %02d:%02d:%02d, 用时:%s", 
			CurrentTime.wYear, 
			CurrentTime.wMonth, 
			CurrentTime.wDay, 
			CurrentTime.wHour, 
			CurrentTime.wMinute, 
			CurrentTime.wSecond, 
			SysTimeCreatePlayer.wYear, 
			SysTimeCreatePlayer.wMonth, 
			SysTimeCreatePlayer.wDay, 
			SysTimeCreatePlayer.wHour, 
			SysTimeCreatePlayer.wMinute, 
			SysTimeCreatePlayer.wSecond, 
			CGameVariable::GetSpendHourForText(SysTimeCreatePlayer, CurrentTime).c_str());
	}

	 std::wstring wsPath;
	wsPath = CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.szConsolePath;
	wsPath += L"\\Setting\\TaskUpgradeSchedule.txt";

	WCHAR wszAppName[MAX_PATH];
	swprintf_s(wszAppName, _countof(wszAppName) - 1, L"%s,%s", CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->szUserName, CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->szPassWord);
	::WritePrivateProfileStringW(wszAppName, CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->szPlayerName, wszText, wsPath.c_str());

	// 保存到服务器一份!
	CSocketConfig().SaveAccountLog_To_Server(em_ClientAccountLog_Type::em_ClientAccountLog_Type_TaskUpgradeSchedule, std::wstring(wszText));
	return TRUE;
}

BOOL CTaskUpgradeMgr::Run(_In_ CONST std::wstring& wsFunName)
{
	auto& Vec = GetVecModePtr();
	auto p = MyTools::CLPublic::Vec_find_if(Vec, [wsFunName](CONST ModeTextPtr& ModeTextPtr_){ return ModeTextPtr_.wsFunName == wsFunName; });
	if (p == nullptr)
	{
		LOG_MSG_CF( L"该模式:%s 不存在!", wsFunName.c_str());
		return FALSE;
	}
	return p->fnPtr();
}

BOOL CTaskUpgradeMgr::DeletePlayer_From_MainTask(_In_ CONST std::wstring& wsLoginPlayerName) CONST
{
	return CGameVariable::GetInstance().Action_Variable_By_Name(VARIABLE_NAME_FINISH_ALL_MAINTASK, TRUE, [&wsLoginPlayerName, this]
	{
		// 保存该帐号的执行状态是执行主线任务完毕
		LOG_CF_D( L"保存角色:%s 执行完所有的主线!, IsDisableDeletePlayer=%d", wsLoginPlayerName.c_str(), CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_DISABLE_DELETEPLAYER));
		CBnsConfig::BnsConfigContent ConfigContent_;
		ConfigContent_.wsConfigName = L"完成主线";
		ConfigContent_.wsConfigValue = L"1";
		CBnsConfig::GetInstance().SetSingleConfigValueByServer(ConfigContent_);
		CGameVariable::GetInstance().Action_Variable_By_Name(VARIABLE_NAME_DISABLE_DELETEPLAYER, FALSE, [&wsLoginPlayerName, this]
		{
			BOOL bDeleteAllPlayer = FALSE;
			if (_IsDeletePlayerPtr != nullptr)
			{
				if (_IsDeletePlayerPtr())
				{
					LOG_CF_D(L"_IsDeletePlayerPtr=TRUE, 执行删除");
					bDeleteAllPlayer = TRUE;
				}
				else
				{
					LOG_CF_D(L"_IsDeletePlayerPtr=FALSE, 不执行删除");
					return;
				}
			}
			else
				bDeleteAllPlayer = CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NMAE_DELETE_ALL_PLAYER) == TRUE ? TRUE : FALSE;

			// 删除该帐号
			LOG_CF_D( L"删除角色:%s,bDeleteAllPlayer=%X", wsLoginPlayerName.c_str(), bDeleteAllPlayer);
			CLoginPlayerExtend::GetInstance().DeletePlayer_By_Condition([&wsLoginPlayerName, bDeleteAllPlayer](CONST CLoginPlayer& itm){return itm.GetName() == wsLoginPlayerName; });// || bDeleteAllPlayer

		});

		// 获取删除时间, 并且更新到日志里面
		WriteDeletePlayerLog(wsLoginPlayerName);
	});
}

BOOL CTaskUpgradeMgr::DeletePlayer_From_OtherClasses() CONST
{
	LOG_CF_D( L"尝试删除非召唤职业的角色!");
	return CLoginPlayerExtend::GetInstance().DeletePlayer_By_Condition([](CONST CLoginPlayer& LoginPlayer)
	{
		return LoginPlayer.GetLoginPlayerClasses() != em_Player_Classes::em_Player_Classes_Summoner;
	});
}

BOOL CTaskUpgradeMgr::DeletePlayer_From_OtherAllClasses(_In_ DWORD dwIndex) CONST
{
	LOG_CF_D( L"尝试删除所有职业角色");
	return CLoginPlayerExtend::GetInstance().DeletePlayer_By_Condition([dwIndex](CONST CLoginPlayer& LoginPlayer){ return LoginPlayer.GetIndex() <= dwIndex; });
}

BOOL CTaskUpgradeMgr::ToSpecialMode() CONST
{
	if (IsExistWarehouse_By_LoginPlayer(nullptr))
		return ToWarehouseMode();

	if (IsExistMail_By_LoginPlayer(nullptr))
		return ToMailMode();

	return FALSE;
}

BOOL CTaskUpgradeMgr::IsExistMail_By_LoginPlayer(_Out_opt_ CLoginPlayer* pLoginPlayer) CONST
{
	std::vector<CLoginPlayer> vlst;
	CObjectFunction::GetInstance().GetLoginPlayerList(vlst);

	return MyTools::CLPublic::Vec_find_if(vlst, pLoginPlayer, [](CONST CLoginPlayer& LoginPlayer){return CGameVariable::GetInstance().ExistSpicalName(LoginPlayer.GetName()) == CGameVariable::em_SpicalName_Type_Mail; });
}

BOOL CTaskUpgradeMgr::ToMailMode() CONST
{
	CConsoleLog(L"进入邮寄模式!");
	CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->GameCmd = em_GameCmd_Start;
	for (;;)
	{
		::Sleep(1000);
		if (CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->GameCmd != em_GameCmd_Start)
			continue;

		CLoginPlayer MailLoginPlayer;
		if (!IsExistMail_By_LoginPlayer(&MailLoginPlayer))
		{
			LOG_MSG_CF( L"貌似堆栈溢出了! 竟然");
			return FALSE;
		}
		RefBnsGameFun.StartGame_By_Index(MailLoginPlayer.GetIndex());

		// 设置不检查坐标超时!
		CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_WAREHOUSE_MODE, TRUE);
		CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CHECKPAUSEPOINT, FALSE);
		RefBnsGameFun.WaitForThread(CBnsGameFun::em_Excute_Type_Mail);

		LOG_CF_D( L"退回选择角色!");
		RefBnsGameFun.BackToSwitchAccount();
	}
}

BOOL CTaskUpgradeMgr::Run_By_Clean()
{
	RefBnsGameFun.ToSeletedPlayer();
	ToSpecialMode();

	std::vector<CLoginPlayer> VecLoginPlayer;
	CObjectFunction::GetInstance().GetLoginPlayerList(VecLoginPlayer);

	// 遇到有非召唤角色的帐号，删除所有角色，剔除帐号
	std::wstring wsLogText;
	auto pLoginPlayer = MyTools::CLPublic::Vec_find_if(VecLoginPlayer, [](CONST CLoginPlayer& LoginPlayer_){ return LoginPlayer_.GetLoginPlayerClasses() != em_Player_Classes::em_Player_Classes_Summoner; });
	if (pLoginPlayer != nullptr)
	{
		CLoginPlayerExtend::GetInstance().DeletePlayer_By_Condition([](CONST CLoginPlayer& LoginPlayer_){ return !LoginPlayer_.IsDeleteStatus() && (LoginPlayer_.GetLoginPlayerClasses() != em_Player_Classes::em_Player_Classes_Summoner || LoginPlayer_.GetLevel() < 50); });
		DWORD dwArea = GetPlayerAreaByCleanMode();
		if (CLoginPlayerExtend::GetInstance().ExistLevelTick()) // 获取可用召唤角色的大区
			wsLogText = MyTools::CCharacter::MakeFormatText(L"整理模式:存在可用帐号[%X,%s,%s] ,有等级劵, 删除所有非召唤角色", dwArea, CGameVariable::GetInstance().GetAccShareInfo()->GetAccName(), CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->szPassWord);
		else if (dwArea == NULL)
			wsLogText = MyTools::CCharacter::MakeFormatText(L"整理模式:遇到有非召唤角色的帐号, 没有可用角色, 并且没有等级劵:[%s] ，删除所有角色，剔除帐号 ", CGameVariable::GetInstance().GetAccShareInfo()->GetAccName());
		else
			wsLogText = MyTools::CCharacter::MakeFormatText(L"整理模式:遇到有非召唤角色的帐号, 并且没有等级劵:[%s,%s,%s] ，删除所有角色，剔除帐号 ", CGameVariable::GetInstance().GetAccShareInfo()->GetAccName(), pLoginPlayer->GetName().c_str(), CPlayer::GetTextByPlayerClasses(pLoginPlayer->GetLoginPlayerClasses()).c_str());


		RefBnsGameFun.SetLoginFaild(wsLogText, CBnsGameFun::em_LoginFaild_DeleteAccount);
	}

	// 不存在召唤角色, 但是有等级卷
	DWORD dwArea = GetPlayerAreaByCleanMode();
	if (CLoginPlayerExtend::GetInstance().ExistLevelTick())
		wsLogText = MyTools::CCharacter::MakeFormatText(L"整理模式:存在可用帐号[%X,%s,%s], 有等级劵, 没其他非召唤角色", dwArea, CGameVariable::GetInstance().GetAccShareInfo()->GetAccName(), CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->szPassWord);
	else
	{
		// 没有等级卷, 判断是否有50级角色, 如果没有就把号也删了
		if (dwArea == NULL)
			wsLogText = MyTools::CCharacter::MakeFormatText(L"整理模式:没召唤职业:[%s,%s]，并且没卷的号，剔除帐号!", CGameVariable::GetInstance().GetAccShareInfo()->GetAccName(), CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->szPassWord);
		else
			wsLogText = MyTools::CCharacter::MakeFormatText(L"整理模式:存在可用帐号[%X,%s,%s], 没等级卷, 但是存在50级的召唤角色!", dwArea, CGameVariable::GetInstance().GetAccShareInfo()->GetAccName(), CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->szPassWord);
	}


	RefBnsGameFun.SetLoginFaild(wsLogText, CBnsGameFun::em_LoginFaild_DeleteAccount);
	

	return TRUE;
}

DWORD CTaskUpgradeMgr::GetPlayerAreaByCleanMode() CONST
{
	std::vector<CLoginPlayer> VecLoginPlayer;
	CObjectFunction::GetInstance().GetLoginPlayerList(VecLoginPlayer);

	auto pLoginPlayer = MyTools::CLPublic::Vec_find_if(VecLoginPlayer, [](CONST CLoginPlayer& LoginPlayer_){ return LoginPlayer_.GetLoginPlayerClasses() == em_Player_Classes::em_Player_Classes_Summoner && !LoginPlayer_.IsDeleteStatus() && LoginPlayer_.GetLevel() >= 50; });
	return pLoginPlayer == nullptr ? NULL : pLoginPlayer->GetServerId();
}

BOOL CTaskUpgradeMgr::IsExistPlayerInLevel50Mode() CONST
{
	std::vector<CLoginPlayer> VecLoginPlayer;
	if (CObjectFunction::GetInstance().GetLoginPlayerList(VecLoginPlayer) != CObjectFunction::GetInstance().GetMaxLoginPlayerCount())
		return TRUE;

	for (CONST auto& itm : VecLoginPlayer)
	{
		if (itm.GetLevel() <= 16 || (itm.GetLevel() >= 50 && itm.GetLoginPlayerClasses() == em_Player_Classes::em_Player_Classes_Summoner))
			return TRUE;
	}
	return FALSE;
}

BOOL CTaskUpgradeMgr::Run_By_SummonerAccount_Safe()
{
	return Run_By_SummonerAccount();
}

BOOL CTaskUpgradeMgr::Run_By_StarConinMode()
{
	RefBnsGameFun.ToSeletedPlayer();
	ToSpecialMode();

	LOG_CF_D( L"进入星币兑换模式!");

	std::vector<CLoginPlayer> VecLoginPlayer;
	CObjectFunction::GetInstance().GetLoginPlayerList(VecLoginPlayer);

	MyTools::CLPublic::Vec_erase_if(VecLoginPlayer, [](CONST CLoginPlayer& LoginPlayer_){ return LoginPlayer_.GetLevel() != 55 || LoginPlayer_.IsDeleteStatus(); });

	LOG_CF_D( L"符合条件的角色有%d个", VecLoginPlayer.size());
	std::vector<std::wstring> VecPlayerName;
	for (CONST auto& itm : VecLoginPlayer)
	{
		LOG_CF_D( L"[%s,%d]", itm.GetName().c_str(), itm.GetLevel());
		VecPlayerName.push_back(itm.GetName());
	}

	for (CONST auto& wsPlayerName : VecPlayerName)
	{
		VecLoginPlayer.clear();
		CObjectFunction::GetInstance().GetLoginPlayerList(VecLoginPlayer);

		auto pLoginPlayer = MyTools::CLPublic::Vec_find_if(VecLoginPlayer, [wsPlayerName](CONST CLoginPlayer& LoginPlayer){ return LoginPlayer.GetName() == wsPlayerName; });
		if (pLoginPlayer == nullptr)
		{
			LOG_CF_E( L"准备兑换星币的角色:%s 突然不存在了!", wsPlayerName.c_str());
			continue;
		}

		if (pLoginPlayer->GetServerId() != CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.dwAreaId)
		{
			RefBnsGameFun.SetLoginFaild(MyTools::CCharacter::MakeFormatText(L"%s:与本机SID:%X 不同",
				pLoginPlayer->GetName().c_str(), 
				CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.dwAreaId), 
				CBnsGameFun::em_LoginFaild_DeleteAccount);
		}

		RefBnsGameFun.StartGame_By_Index(pLoginPlayer->GetIndex());

		// 开始进入游戏
		LOG_CF_D( L"开始游戏! 选择第%d个角色", pLoginPlayer->GetIndex());
		RefBnsGameFun.StartGame_By_Index(pLoginPlayer->GetIndex());

		CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_LASTPLAYER_EXCHANGEMODE, wsPlayerName == VecPlayerName.back() ? TRUE : FALSE);

		LOG_CF_D( L"开始执行脚本!");
		RefBnsGameFun.WaitForThread(CBnsGameFun::em_Excute_Type_PK_TaskUpgrade);
		if (CGameVariable::GetInstance().GetAccShareInfo()->AccountStatus.bClose)
		{
			LOG_CF_D( L"bClose=TRUE; 等待客户端结束!");
			::Sleep(INFINITE);
		}

		// 退回选择角色!
		LOG_CF_D( L"退回选择角色!");
		RefBnsGameFun.BackToSwitchAccount();
	}

	DeletePlayer_From_OtherClasses();
	RefBnsGameFun.SetLoginFaild(L"星币兑换模式完成", CBnsGameFun::em_LoginFaild_DeleteAccount);
	return TRUE;
}

BOOL CTaskUpgradeMgr::Run_By_Classes(_In_ em_Player_Classes emPlayerClasses, _In_ CONST std::wstring& wsClassesText, std::function<em_Player_Classes(DWORD)> GetCreatePlayerClassesPtr)
{
	RefBnsGameFun.ToSeletedPlayer();
	ToSpecialMode();

	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NMAE_DELETE_ALL_PLAYER, TRUE);
	for (;;)
	{
		// 设置登录步骤
		CGameVariable::GetInstance().GetAccShareInfo()->AccountStatus.emLoginStep = em_Login_Step::em_Login_Step_ChoicePlayer;

		// 判断是否所有角色都完成了, 是的话就换帐号
		if (GetCount_By_DeleteStatus() == CObjectFunction::GetInstance().GetMaxLoginPlayerCount())
		{
			RefBnsGameFun.SetLoginFaild(MyTools::CCharacter::MakeFormatText(L"%s模式: 所有角色都完成换下一个帐号!", wsClassesText.c_str()), CBnsGameFun::em_LoginFaild_DeleteAccount);
			return FALSE;
		}
		
		/*if (GetCount_By_DeleteStatus() > 0)
		{
			LOG_CF_D( L"还有角色正在删除,等待");
			RefBnsGameFun.WriteConsoleLog(L"等待删除角色ing!!!");
			::Sleep(5 * 1000);
			continue;
		}*/
		
		/*if (GetCount_By_DeleteStatus() == CObjectFunction::GetInstance().GetMaxLoginPlayerCount())
		{
		RefBnsGameFun.WriteConsoleLog(L"等待删除角色ing!!!");
		::Sleep(30 * 1000);
		continue;
		}*/

		std::vector<CLoginPlayer> VecLoginPlayer;
		// 获取所有的角色
		CObjectFunction::GetInstance().GetLoginPlayerList(VecLoginPlayer);

		// 输出而已
		for (CONST auto& itm : VecLoginPlayer)
		{
			LOG_CF_D( L"Full LoginPlayer: Index=%d, Classes=%s, Level=%d, Server=%d, Name=%s, IsDelete=%d", \
				itm.GetIndex(), CPlayer::GetTextByPlayerClasses(itm.GetLoginPlayerClasses()).c_str(), itm.GetLevel(), itm.GetServerId(), itm.GetName().c_str(), itm.IsDeleteStatus());
		}

		// 根据模式过滤掉不需要的职业和已经是删除状态的角色, 比如说刺客模式就过滤了所有非刺客的职业
		RemoveClasses_By_Mode(emPlayerClasses, VecLoginPlayer, GetCreatePlayerClassesPtr);

		// 输出而已
		for (CONST auto& itm : VecLoginPlayer)
		{
			LOG_CF_D( L"erase LoginPlayer: Index=%d, Classes=%s, Level=%d, Server=%d, Name=%s", \
				itm.GetIndex(), CPlayer::GetTextByPlayerClasses(itm.GetLoginPlayerClasses()).c_str(), itm.GetLevel(), itm.GetServerId(), itm.GetName().c_str());
		}

		// 把剩下的角色全部发去到服务器查询是否完成主线
		for (auto& itm : VecLoginPlayer)
		{
			auto wsConfigValue = CBnsConfig::GetInstance().GetSingleConfigValueByServer(itm.GetName(), L"完成主线");
			itm.SetFinishMainTask(wsConfigValue == L"1" ? TRUE : FALSE);
		}

		// 将所有已经完成主线的角色给过滤掉
		MyTools::CLPublic::Vec_erase_if(VecLoginPlayer, [](CONST CLoginPlayer& LoginPlayer_){ return LoginPlayer_.GetFinishMainTask(); });
		for (CONST auto& itm : VecLoginPlayer)
		{
			LOG_CF_D( L"Remain LoginPlayer: Index=%d, Classes=%s, Level=%d, Server=%d, Name=%s", \
				itm.GetIndex(), CPlayer::GetTextByPlayerClasses(itm.GetLoginPlayerClasses()).c_str(), itm.GetLevel(), itm.GetServerId(), itm.GetName().c_str());
		}

		// 判断是否有超过45级的召唤角色。
		CLoginPlayer LoginPlayer_;
		
		CONST std::wstring wsPlayerName = LoginPlayer_.GetName();
		if (VecLoginPlayer.size() == 0)
		{
			LOG_CF_D( L"%s下, 并没有可用的角色!", wsClassesText.c_str());

			DWORD dwIndex = CObjectFunction::GetInstance().GetLoginPlayerCount();
			
			if ((wsClassesText == L"练级模式" || wsClassesText == L"练级模式2") && dwIndex == CObjectFunction::GetInstance().GetMaxLoginPlayerCount())
			{
				CConsoleLog(L"练级模式所有角色都满了!尝试删除所有角色!");
				// 这里的wsPlayerName必定是NULL， 因为从上面看来, LoginPlayer_.GetName是没初始化过的
				DeletePlayer_From_OtherAllClasses(4);
				::Sleep(1000);
				continue;
			}
			else
			{
				if (dwIndex == CObjectFunction::GetInstance().GetMaxLoginPlayerCount())
				{
					/*RefBnsGameFun.WriteConsoleLog(L"所有角色都满了!没有可用的角色!尝试删除前4个角色");
					DeletePlayer_From_OtherAllClasses(3);
					::Sleep(1000);
					continue;*/
					CConsoleLog(L"所有角色都满了!没有可用的角色!换帐号吧.");
					//DeletePlayer_From_OtherAllClasses(4);
					::Sleep(1000);
					continue;
				}
			}

			dwIndex = CObjectFunction::GetInstance().GetLoginPlayerCount();
			em_Player_Classes emCreatePlayerClasses = GetCreatePlayerClassesPtr == nullptr ? emPlayerClasses : GetCreatePlayerClassesPtr(dwIndex);

			LOG_CF_D( L"%s下, 不存在号. 那就创建角色[%s]! Index=%d", wsClassesText.c_str(), CPlayer::GetTextByPlayerClasses(emCreatePlayerClasses).c_str(), dwIndex);
			CLoginPlayerExtend::GetInstance().CreatePlayer(CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.dwAreaId, dwIndex, emCreatePlayerClasses, FALSE);

			// 如果当前角色数量 == 还没创建之前的Index, 就表示创建角色失败
			if (CObjectFunction::GetInstance().GetLoginPlayerCount() == dwIndex)
				continue;

			// 这里重新遍历了角色
			if (CObjectFunction::GetInstance().GetLoginPlayerList(VecLoginPlayer) != dwIndex + 1)
				continue;

			// 并且把遍历后的角色列表的最后一个设置成当前上号的角色
			MyTools::CLPublic::SetPtr(&LoginPlayer_, VecLoginPlayer.at(dwIndex));

			
		}
		else
		{
			LoginPlayer_ = VecLoginPlayer.at(0);
			LOG_CF_D( L"%s下, 有存在角色%s, 等级:%d, 那就进这个号!",wsClassesText.c_str(), LoginPlayer_.GetName().c_str(), LoginPlayer_.GetLevel());
		}

		// 过滤掉跟Config.ini里面的Server不同的角色
		if (LoginPlayer_.GetServerId() != CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.dwAreaId)
		{
			RefBnsGameFun.SetLoginFaild(MyTools::CCharacter::MakeFormatText(L"%s:与本机SID:%X 不同", 
				LoginPlayer_.GetName().c_str(), 
				CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.dwAreaId), 
				CBnsGameFun::em_LoginFaild_DeleteAccount);
		}


		// 开始进入游戏
		LOG_CF_D( L"开始游戏! 选择第%d个角色", LoginPlayer_.GetIndex());
		RefBnsGameFun.StartGame_By_Index(LoginPlayer_.GetIndex());

		LOG_CF_D( L"开始执行脚本!");
		RefBnsGameFun.WaitForThread(CBnsGameFun::em_Excute_Type_PK_TaskUpgrade);
		if (CGameVariable::GetInstance().GetAccShareInfo()->AccountStatus.bClose)
		{
			LOG_CF_D( L"bClose=TRUE; 等待客户端结束!");
			::Sleep(INFINITE);
		}

		// 获取进去的名称
		CONST std::wstring wsLoginPlayerName = LoginPlayer_.GetName();


		// 退回选择角色!
		LOG_CF_D( L"退回选择角色!");
		RefBnsGameFun.BackToSwitchAccount();

		// 删除当前角色(里面还会发包到服务器设置该角色已经完成主线了, 而且还要记录完成日期到本地txt)
		LOG_CF_D( L"当前需要删除角色名%s", wsLoginPlayerName.c_str());
		DeletePlayer_From_MainTask(wsLoginPlayerName);

		CGameVariable::GetInstance().Action_Variable_By_Name(VARIABLE_NAME_DELETEACCOUNT, TRUE, [this]
		{
			// 通知Console将这个帐号从Account.txt删了, 进行下一个帐号
			RefBnsGameFun.SetLoginFaild(L"删号=TRUE", CBnsGameFun::em_LoginFaild_DeleteAccount);
		});
	}

}

BOOL CTaskUpgradeMgr::Run_By_Classes2(_In_ em_Player_Classes emPlayerClasses, _In_ CONST std::wstring& wsClassesText, std::function<em_Player_Classes(DWORD)> GetCreatePlayerClassesPtr)
{
	RefBnsGameFun.ToSeletedPlayer();
	ToSpecialMode();

	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NMAE_DELETE_ALL_PLAYER, TRUE);
	for (;;)
	{
		// 设置登录步骤
		CGameVariable::GetInstance().GetAccShareInfo()->AccountStatus.emLoginStep = em_Login_Step::em_Login_Step_ChoicePlayer;

		// 判断是否所有角色都完成了, 是的话就换帐号
		if (GetCount_By_DeleteStatus() == CObjectFunction::GetInstance().GetMaxLoginPlayerCount())
		{
			RefBnsGameFun.SetLoginFaild(MyTools::CCharacter::MakeFormatText(L"%s模式: 所有角色都完成换下一个帐号!", wsClassesText.c_str()), CBnsGameFun::em_LoginFaild_DeleteAccount);
			return FALSE;
		}

		/*if (GetCount_By_DeleteStatus() > 0)
		{
		LOG_CF_D( L"还有角色正在删除,等待");
		RefBnsGameFun.WriteConsoleLog(L"等待删除角色ing!!!");
		::Sleep(5 * 1000);
		continue;
		}*/

		/*if (GetCount_By_DeleteStatus() == CObjectFunction::GetInstance().GetMaxLoginPlayerCount())
		{
		RefBnsGameFun.WriteConsoleLog(L"等待删除角色ing!!!");
		::Sleep(30 * 1000);
		continue;
		}*/

		std::vector<CLoginPlayer> VecLoginPlayer;
		// 获取所有的角色
		CObjectFunction::GetInstance().GetLoginPlayerList(VecLoginPlayer);

		// 输出而已
		for (CONST auto& itm : VecLoginPlayer)
		{
			LOG_CF_D( L"Full LoginPlayer: Index=%d, Classes=%s, Level=%d, Server=%d, Name=%s, IsDelete=%d", \
				itm.GetIndex(), CPlayer::GetTextByPlayerClasses(itm.GetLoginPlayerClasses()).c_str(), itm.GetLevel(), itm.GetServerId(), itm.GetName().c_str(), itm.IsDeleteStatus());
		}

		// 根据模式过滤掉不需要的职业和已经是删除状态的角色, 比如说刺客模式就过滤了所有非刺客的职业
		RemoveClasses_By_Mode2(emPlayerClasses, VecLoginPlayer, GetCreatePlayerClassesPtr);

		// 输出而已
		for (CONST auto& itm : VecLoginPlayer)
		{
			LOG_CF_D( L"erase LoginPlayer: Index=%d, Classes=%s, Level=%d, Server=%d, Name=%s", \
				itm.GetIndex(), CPlayer::GetTextByPlayerClasses(itm.GetLoginPlayerClasses()).c_str(), itm.GetLevel(), itm.GetServerId(), itm.GetName().c_str());
		}

		// 把剩下的角色全部发去到服务器查询是否完成主线
		for (auto& itm : VecLoginPlayer)
		{
			auto wsConfigValue = CBnsConfig::GetInstance().GetSingleConfigValueByServer(itm.GetName(), L"完成主线");
			itm.SetFinishMainTask(wsConfigValue == L"1" ? TRUE : FALSE);
		}

		// 将所有已经完成主线的角色给过滤掉
		MyTools::CLPublic::Vec_erase_if(VecLoginPlayer, [](CONST CLoginPlayer& LoginPlayer_){ return LoginPlayer_.GetFinishMainTask(); });
		for (CONST auto& itm : VecLoginPlayer)
		{
			LOG_CF_D( L"Remain LoginPlayer: Index=%d, Classes=%s, Level=%d, Server=%d, Name=%s", \
				itm.GetIndex(), CPlayer::GetTextByPlayerClasses(itm.GetLoginPlayerClasses()).c_str(), itm.GetLevel(), itm.GetServerId(), itm.GetName().c_str());
		}

		// 判断是否有超过45级的召唤角色。
		CLoginPlayer LoginPlayer_;

		CONST std::wstring wsPlayerName = LoginPlayer_.GetName();
		if (VecLoginPlayer.size() == 0)
		{
			LOG_CF_D( L"%s下, 并没有可用的角色!", wsClassesText.c_str());

			DWORD dwIndex = CObjectFunction::GetInstance().GetLoginPlayerCount();

			if ((wsClassesText == L"练级模式" || wsClassesText == L"练级模式2") && dwIndex == CObjectFunction::GetInstance().GetMaxLoginPlayerCount())
			{
				CConsoleLog(L"练级模式所有角色都满了!尝试删除所有角色!");
				// 这里的wsPlayerName必定是NULL， 因为从上面看来, LoginPlayer_.GetName是没初始化过的
				DeletePlayer_From_OtherAllClasses(4);
				::Sleep(1000);
				continue;
			}
			else
			{
				if (dwIndex == CObjectFunction::GetInstance().GetMaxLoginPlayerCount())
				{
					/*RefBnsGameFun.WriteConsoleLog(L"所有角色都满了!没有可用的角色!尝试删除前4个角色");
					DeletePlayer_From_OtherAllClasses(3);
					::Sleep(1000);
					continue;*/
					CConsoleLog(L"所有角色都满了!没有可用的角色!尝试删除所有角色");
					DeletePlayer_From_OtherAllClasses(4);
					::Sleep(1000);
					continue;
				}
			}

			dwIndex = CObjectFunction::GetInstance().GetLoginPlayerCount();
			em_Player_Classes emCreatePlayerClasses = GetCreatePlayerClassesPtr == nullptr ? emPlayerClasses : GetCreatePlayerClassesPtr(dwIndex);

			LOG_CF_D( L"%s下, 不存在号. 那就创建角色[%s]! Index=%d", wsClassesText.c_str(), CPlayer::GetTextByPlayerClasses(emCreatePlayerClasses).c_str(), dwIndex);
			CLoginPlayerExtend::GetInstance().CreatePlayer(CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.dwAreaId, dwIndex, emCreatePlayerClasses, FALSE);

			// 如果当前角色数量 == 还没创建之前的Index, 就表示创建角色失败
			if (CObjectFunction::GetInstance().GetLoginPlayerCount() == dwIndex)
				continue;

			// 这里重新遍历了角色
			if (CObjectFunction::GetInstance().GetLoginPlayerList(VecLoginPlayer) != dwIndex + 1)
				continue;

			// 并且把遍历后的角色列表的最后一个设置成当前上号的角色
			MyTools::CLPublic::SetPtr(&LoginPlayer_, VecLoginPlayer.at(dwIndex));


		}
		else
		{
			LoginPlayer_ = VecLoginPlayer.at(0);
			LOG_CF_D( L"%s下, 有存在角色%s, 等级:%d, 那就进这个号!", wsClassesText.c_str(), LoginPlayer_.GetName().c_str(), LoginPlayer_.GetLevel());
		}

		// 过滤掉跟Config.ini里面的Server不同的角色
		if (LoginPlayer_.GetServerId() != CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.dwAreaId)
		{
			RefBnsGameFun.SetLoginFaild(MyTools::CCharacter::MakeFormatText(L"%s:与本机SID:%X 不同",
				LoginPlayer_.GetName().c_str(),
				CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.dwAreaId),
				CBnsGameFun::em_LoginFaild_DeleteAccount);
		}


		// 开始进入游戏
		LOG_CF_D( L"开始游戏! 选择第%d个角色", LoginPlayer_.GetIndex());
		RefBnsGameFun.StartGame_By_Index(LoginPlayer_.GetIndex());

		LOG_CF_D( L"开始执行脚本!");
		RefBnsGameFun.WaitForThread(CBnsGameFun::em_Excute_Type_PK_TaskUpgrade);
		if (CGameVariable::GetInstance().GetAccShareInfo()->AccountStatus.bClose)
		{
			LOG_CF_D( L"bClose=TRUE; 等待客户端结束!");
			::Sleep(INFINITE);
		}

		// 获取进去的名称
		CONST std::wstring wsLoginPlayerName = LoginPlayer_.GetName();


		// 退回选择角色!
		LOG_CF_D( L"退回选择角色!");
		RefBnsGameFun.BackToSwitchAccount();

		// 删除当前角色(里面还会发包到服务器设置该角色已经完成主线了, 而且还要记录完成日期到本地txt)
		LOG_CF_D( L"当前需要删除角色名%s", wsLoginPlayerName.c_str());
		DeletePlayer_From_MainTask(wsLoginPlayerName);

		CGameVariable::GetInstance().Action_Variable_By_Name(VARIABLE_NAME_DELETEACCOUNT, TRUE, [this]
		{
			// 通知Console将这个帐号从Account.txt删了, 进行下一个帐号
			RefBnsGameFun.SetLoginFaild(L"删号=TRUE", CBnsGameFun::em_LoginFaild_DeleteAccount);
		});
	}

}

VOID CTaskUpgradeMgr::RemoveClasses_By_Mode(_In_ em_Player_Classes emPlayerClasses, _In_ _Out_opt_ std::vector<CLoginPlayer>& VecLoginPlayer, std::function<em_Player_Classes(DWORD)> GetCreatePlayerClassesPtr) CONST
{
	CONST auto& VecClasses = GetPlayerPool();
	MyTools::CLPublic::Vec_erase_if(VecLoginPlayer, [=](CONST CLoginPlayer& itm)
	{
		if (itm.IsDeleteStatus())
			return TRUE;

		auto emLoginPlayerClasses = itm.GetLoginPlayerClasses();
		if (GetCreatePlayerClassesPtr == nullptr &&  emLoginPlayerClasses != emPlayerClasses)
			return TRUE;

		if (!MyTools::CLPublic::Vec_find_if_Const(VecClasses, [emLoginPlayerClasses](CONST em_Player_Classes emClasses){ return emLoginPlayerClasses == emClasses; }))
		{
			LOG_CF_D(L"该角色%s不存在角色池里", itm.GetName().c_str());
			return TRUE;
		}
		
		//else if (GetCreatePlayerClassesPtr != nullptr && itm.GetLoginPlayerClasses() != GetCreatePlayerClassesPtr(itm.GetIndex()))
		//	return TRUE;

		return FALSE;
	});
}

VOID CTaskUpgradeMgr::RemoveClasses_By_Mode2(_In_ em_Player_Classes emPlayerClasses, _In_ _Out_opt_ std::vector<CLoginPlayer>& VecLoginPlayer, std::function<em_Player_Classes(DWORD)> GetCreatePlayerClassesPtr) CONST
{
	CONST auto& VecClasses = GetPlayerPool2();
	MyTools::CLPublic::Vec_erase_if(VecLoginPlayer, [=](CONST CLoginPlayer& itm)
	{
		if (itm.IsDeleteStatus())
			return TRUE;

		auto emLoginPlayerClasses = itm.GetLoginPlayerClasses();
		if (GetCreatePlayerClassesPtr == nullptr &&  emLoginPlayerClasses != emPlayerClasses)
			return TRUE;

		if (!MyTools::CLPublic::Vec_find_if_Const(VecClasses, [emLoginPlayerClasses](CONST em_Player_Classes emClasses){ return emLoginPlayerClasses == emClasses; }))
		{
			LOG_CF_D(L"该角色%s不存在角色池里", itm.GetName().c_str());
			return TRUE;
		}

		//else if (GetCreatePlayerClassesPtr != nullptr && itm.GetLoginPlayerClasses() != GetCreatePlayerClassesPtr(itm.GetIndex()))
		//	return TRUE;

		return FALSE;
	});
}

CONST std::vector<em_Player_Classes>& CTaskUpgradeMgr::GetPlayerPool() CONST
{
	CONST static std::vector<em_Player_Classes> VecClasses =
	{
		em_Player_Classes::em_Player_Classes_Assassin,
		em_Player_Classes::em_Player_Classes_KongFuMaster,
		em_Player_Classes::em_Player_Classes_ForceMaster,
		em_Player_Classes::em_Player_Classes_Gunner,
		em_Player_Classes::em_Player_Classes_BladeMaster,
		em_Player_Classes::em_Player_Classes_Warlock,
		em_Player_Classes::em_Player_Classes_Danncer
	};

	return VecClasses;
}

CONST std::vector<em_Player_Classes>& CTaskUpgradeMgr::GetPlayerPool2() CONST
{
	CONST static std::vector<em_Player_Classes> VecClasses =
	{
		em_Player_Classes::em_Player_Classes_KongFuMaster,
		em_Player_Classes::em_Player_Classes_ForceMaster,
		em_Player_Classes::em_Player_Classes_BladeMaster,
		em_Player_Classes::em_Player_Classes_Warlock,
		em_Player_Classes::em_Player_Classes_Assassin,
		em_Player_Classes::em_Player_Classes_Danncer
	};

	return VecClasses;
}

BOOL CTaskUpgradeMgr::Run_By_Warlock()
{
	return Run_By_Classes(em_Player_Classes::em_Player_Classes_Warlock, L"咒术模式", nullptr);
}

BOOL CTaskUpgradeMgr::Run_By_BladeMaster()
{
	return Run_By_Classes(em_Player_Classes::em_Player_Classes_BladeMaster, L"剑士模式", nullptr);
}

BOOL CTaskUpgradeMgr::Run_By_Danncer()
{
	return Run_By_Classes(em_Player_Classes::em_Player_Classes_Danncer, L"灵剑模式", nullptr);
}

BOOL CTaskUpgradeMgr::Run_By_ForceMaster()
{
	return Run_By_Classes(em_Player_Classes::em_Player_Classes_ForceMaster, L"气功模式", nullptr);
}

BOOL CTaskUpgradeMgr::Run_By_KongFuForceMaster()
{
	return Run_By_Classes(em_Player_Classes::em_Player_Classes_KongFuForceMaster, L"气拳模式", nullptr);
}

BOOL CTaskUpgradeMgr::Run_By_KongFuMaster()
{
	return Run_By_Classes(em_Player_Classes::em_Player_Classes_KongFuMaster, L"拳师模式", nullptr);
}

BOOL CTaskUpgradeMgr::Run_By_Assassin()
{
	return Run_By_Classes(em_Player_Classes::em_Player_Classes_Assassin, L"刺客模式", nullptr);
}

//枪手模式
BOOL CTaskUpgradeMgr::Run_By_Gunner()
{
	return Run_By_Classes(em_Player_Classes::em_Player_Classes_Gunner, L"枪手模式", nullptr);
}


BOOL CTaskUpgradeMgr::Run_By_UpgradeLevelMode()
{
	_IsDeletePlayerPtr = []
	{
		return TRUE;
		/*std::vector<CLoginPlayer> VecLoginPlayer;
		return CObjectFunction::GetInstance().GetLoginPlayerList(VecLoginPlayer) == CObjectFunction::GetInstance().GetMaxLoginPlayerCount();*/
	};


	return Run_By_Classes(em_Player_Classes::em_Player_Classes_Assassin, L"练级模式", [this](DWORD dwIndex)
	{
		// Get ALl of LoginPlayer
		// dwIndex == 0 ? Rand()  : rand((dwIndex - 1).Classes)
		CONST auto& VecClasses = GetPlayerPool();
		if (dwIndex == 0)
		{
			auto emClasses = VecClasses.at(MyTools::CCharacter::GetRand(VecClasses.size() - 1, 0));
			LOG_CF_D(L"当前没有角色，建立的角色%s", CPlayer::GetTextByPlayerClasses(emClasses).c_str());
			return emClasses;
		}

		std::vector<CLoginPlayer> VecLoginPlayer;
		CObjectFunction::GetInstance().GetLoginPlayerList(VecLoginPlayer);
		auto& CurrentLoginPlayer = VecLoginPlayer.back(); // 获取dwindex对象

		// 复制职业列表
		auto VecCopyClasses = VecClasses;
		MyTools::CLPublic::Vec_erase_if(VecCopyClasses, [CurrentLoginPlayer](em_Player_Classes emClasses){ return CurrentLoginPlayer.GetLoginPlayerClasses() == emClasses; });
		auto emClasses = VecCopyClasses.at(MyTools::CCharacter::GetRand(VecCopyClasses.size() - 1, 0));

		LOG_CF_D(L"上一个职业是%s,当前创建的职业是%s", 
			CPlayer::GetTextByPlayerClasses(CurrentLoginPlayer.GetLoginPlayerClasses()).c_str(), 
			CPlayer::GetTextByPlayerClasses(emClasses).c_str()
			);

		return emClasses;
	});
	
}

BOOL CTaskUpgradeMgr::Run_By_UpgradeLevelMode2()
{
	_IsDeletePlayerPtr = []
	{
		return TRUE;
		/*std::vector<CLoginPlayer> VecLoginPlayer;
		return CObjectFunction::GetInstance().GetLoginPlayerList(VecLoginPlayer) == CObjectFunction::GetInstance().GetMaxLoginPlayerCount();*/
	};


	return Run_By_Classes2(em_Player_Classes::em_Player_Classes_Assassin, L"练级模式2", [this](DWORD dwIndex)
	{
		// Get ALl of LoginPlayer
		// dwIndex == 0 ? Rand()  : rand((dwIndex - 1).Classes)
		CONST auto& VecClasses = GetPlayerPool2();
		if (dwIndex == 0)
		{
			auto emClasses = VecClasses.at(MyTools::CCharacter::GetRand(VecClasses.size() - 1, 0));
			LOG_CF_D(L"当前没有角色，建立的角色%s", CPlayer::GetTextByPlayerClasses(emClasses).c_str());
			return emClasses;
		}

		std::vector<CLoginPlayer> VecLoginPlayer;
		CObjectFunction::GetInstance().GetLoginPlayerList(VecLoginPlayer);
		auto& CurrentLoginPlayer = VecLoginPlayer.back(); // 获取dwindex对象

		// 复制职业列表
		auto VecCopyClasses = VecClasses;
		MyTools::CLPublic::Vec_erase_if(VecCopyClasses, [CurrentLoginPlayer](em_Player_Classes emClasses){ return CurrentLoginPlayer.GetLoginPlayerClasses() == emClasses; });
		auto emClasses = VecCopyClasses.at(MyTools::CCharacter::GetRand(VecCopyClasses.size() - 1, 0));

		LOG_CF_D(L"上一个职业是%s,当前创建的职业是%s",
			CPlayer::GetTextByPlayerClasses(CurrentLoginPlayer.GetLoginPlayerClasses()).c_str(),
			CPlayer::GetTextByPlayerClasses(emClasses).c_str()
			);

		return emClasses;
	});
}

std::vector<CTaskUpgradeMgr::ModeTextPtr>& CTaskUpgradeMgr::GetVecModePtr()
{
	static std::vector<ModeTextPtr> Vec =
	{
		{ L"黑号模式", std::bind(&CTaskUpgradeMgr::Run_By_BlackAccount, this) },
		{ L"白号模式", std::bind(&CTaskUpgradeMgr::Run_By_WhiteAccount, this) },
		{ L"免费模式", std::bind(&CTaskUpgradeMgr::Run_By_FreeAccount, this) },
		{ L"删号模式", std::bind(&CTaskUpgradeMgr::Run_By_DeleteAccount, this) },
		{ L"力士模式", std::bind(&CTaskUpgradeMgr::Run_By_DesocoryAccount, this) },
		{ L"白号力士免费模式", std::bind(&CTaskUpgradeMgr::Run_By_FreeWhiteAccount, this) },
		{ L"加点模式", std::bind(&CTaskUpgradeMgr::Run_By_AddSkillAccount, this) },
		{ L"手动模式", std::bind(&CTaskUpgradeMgr::Run_By_DeletePlayer, this) },
		{ L"召唤模式", std::bind(&CTaskUpgradeMgr::Run_By_SummonerAccount, this) },
		{ L"召唤模式2", std::bind(&CTaskUpgradeMgr::Run_By_SummonerAccount2, this) },
		{ L"ExcuteScript", std::bind(&CTaskUpgradeMgr::Run_By_ExcuteScript, this) },
		{ L"召唤50级模式", std::bind(&CTaskUpgradeMgr::Run_By_SummonerAccount_Level50, this) },
		{ L"召唤模式7章", std::bind(&CTaskUpgradeMgr::RUn_By_SummonerAccount_Capter7, this) },
		{ L"召唤50级模式1", std::bind(&CTaskUpgradeMgr::Run_By_SummonerAccount1_Level50, this) },
		{ L"创建角色模式", std::bind(&CTaskUpgradeMgr::Run_By_CreatePlayer, this) },
		{ L"整理模式", std::bind(&CTaskUpgradeMgr::Run_By_Clean, this) },
		{ L"保号召唤模式", std::bind(&CTaskUpgradeMgr::Run_By_SummonerAccount_Safe, this) },
		{ L"召唤50级加速模式", std::bind(&CTaskUpgradeMgr::Run_By_SummonerAccount_Speed_Level50, this) },
		{ L"星币兑换模式", std::bind(&CTaskUpgradeMgr::Run_By_StarConinMode, this) },
		{ L"咒术模式", std::bind(&CTaskUpgradeMgr::Run_By_Warlock, this) },
		{ L"剑士模式", std::bind(&CTaskUpgradeMgr::Run_By_BladeMaster, this) },
		{ L"灵剑模式", std::bind(&CTaskUpgradeMgr::Run_By_Danncer, this) },
		{ L"气功模式", std::bind(&CTaskUpgradeMgr::Run_By_ForceMaster, this) },
		{ L"气拳模式", std::bind(&CTaskUpgradeMgr::Run_By_KongFuForceMaster, this) },
		{ L"拳师模式", std::bind(&CTaskUpgradeMgr::Run_By_KongFuMaster, this) },
		{ L"刺客模式", std::bind(&CTaskUpgradeMgr::Run_By_Assassin, this) },
		{ L"枪手模式", std::bind(&CTaskUpgradeMgr::Run_By_Gunner, this) },
		{ L"练级模式", std::bind(&CTaskUpgradeMgr::Run_By_UpgradeLevelMode, this) },
		{ L"练级模式2", std::bind(&CTaskUpgradeMgr::Run_By_UpgradeLevelMode2, this) },
		{ L"删号模式2", std::bind(&CTaskUpgradeMgr::Run_By_DeleteAccount2, this) },
		
	};
	return Vec;
}

