#include "stdafx.h"
#include "AccountMgr.h"
#include <MyTools/Log.h>
#include <MyTools/CLPublic.h>
#include "ConsoleCard.h"
#include "ConsoleLog.h"
#include "ConsoleFun.h"
#define _SELF L"AccountMgr.cpp"

CAccountMgr::CAccountMgr()
{
}

CAccountMgr::~CAccountMgr()
{
}

BOOL CAccountMgr::SetPkLog(_In_ ACCOUNT_INFO_GAME& ACCOUNT_INFO_GAME_)
{
	if (ACCOUNT_INFO_GAME_.GetLoginFaildCount() >= 3)
		CBnsConfig::GetInstance().WriteAccountLog(&ACCOUNT_INFO_GAME_, L"连续登录3次都失败!");
	if (ACCOUNT_INFO_GAME_.AccountStatus.uCreatePlayerFaildCount >= 3)
		CBnsConfig::GetInstance().WriteAccountLog(&ACCOUNT_INFO_GAME_, L"连续创建角色3次都失败!");

	return TRUE;
}

BOOL CAccountMgr::SetResetAccount(_In_ ACCOUNT_INFO_GAME& AccountGame)
{
	//fn(AccountGame.MyAcount_Info.szUserName, L"掉线了! 重新上号!");
	LOG_CF_D( L"帐号:[%s,%X] 掉线了!重新上号!", AccountGame.MyAcount_Info.szUserName, AccountGame.dwPid);
	AccountGame.AccountStatus.bClose = FALSE;
	CConsoleFun::GetInstance().KillGame_By_AccountName(&AccountGame); // 关闭游戏
	return CConsoleFun::GetInstance().RunGame(&AccountGame);					// 重启启动
}

BOOL CAccountMgr::SetClearAccount(_In_ ACCOUNT_INFO_GAME& ACCOUNT_INFO_GAME_)
{
	if (!CConsoleCard::GetInstance().ExistCard(ACCOUNT_INFO_GAME_.MyAcount_Info.szCardNo, [](CardInfo& CardInfo_) {CardInfo_.bExit = FALSE; })) // 把这个帐号的卡号"归还" 出来, 让新号可以用!
	{
		LOG_CF_E(L"卡号分配不足!");
		return TRUE;
	}

	// 把该帐号清除!
	if (ACCOUNT_INFO_GAME_.AccountStatus.bDelete)
	{
		LOG_CF_D(L"bDelete = TRUE,删除Account.txt的该帐号[%s]", ACCOUNT_INFO_GAME_.GetAccName());
		CBnsConfig::GetInstance().ClearAccountConfig(ACCOUNT_INFO_GAME_.GetAccName());
	}

	CConsoleFun::GetInstance().KillGame_By_AccountName(&ACCOUNT_INFO_GAME_); // 关闭游戏
	ACCOUNT_INFO_GAME_.AccountStatus.bExist = FALSE;
	//ZeroMemory(&ACCOUNT_INFO_GAME_, sizeof(ACCOUNT_INFO_GAME)); // 清空整个结构， 给别的帐号用
	return TRUE;
}

BOOL CAccountMgr::AddAccount(_In_ LPCWSTR pwszAccountName, _In_ LPCWSTR pwszAccountPass)
{
	if (IsExistAccount(pwszAccountName) != nullptr)
		return TRUE;

	auto pAccountGame = g_pSharedInfo->ExistEmptyArrGameInfo(); // 找一个bExist = FALSE的空位
	if (pAccountGame == nullptr)
	{
		LOG_MSG_CF(L"最大容纳上限%d个帐号已满!", MAX_GAME_COUNT);
		return FALSE;
	}

	MyTools::CCharacter::wstrcpy_my(pAccountGame->MyAcount_Info.szUserName, pwszAccountName, _countof(pAccountGame->MyAcount_Info.szUserName) - 1);
	MyTools::CCharacter::wstrcpy_my(pAccountGame->MyAcount_Info.szPassWord, pwszAccountPass, _countof(pAccountGame->MyAcount_Info.szPassWord) - 1);
	pAccountGame->AccountStatus.bExist = TRUE;
	g_pSharedInfo->nAccountCount += 1; // 暂时没用
	return TRUE;
}

PACCOUNT_INFO_GAME CAccountMgr::IsExistAccount(_In_ LPCWSTR pwszAccountName)
{
	auto itr = std::find_if(std::begin(g_pSharedInfo->arrGameInfo), std::end(g_pSharedInfo->arrGameInfo), [&pwszAccountName](ACCOUNT_INFO_GAME& AccountGame) 
	{
		return MyTools::CCharacter::wstrcmp_my(pwszAccountName, AccountGame.MyAcount_Info.szUserName);
	});

	return itr == std::end(g_pSharedInfo->arrGameInfo) ? nullptr : itr;
}

BOOL CAccountMgr::HandleAccount(_In_ CONST std::wstring& wsAccountName, _In_ std::function<BOOL(ACCOUNT_INFO_GAME&)> AcionPtr)
{
	auto pAccountGame = IsExistAccount(wsAccountName.c_str());
	return pAccountGame != nullptr ? AcionPtr(*pAccountGame) : FALSE;
}

BOOL CAccountMgr::DeleteAccount(_In_ LPCWSTR pwszAccountName)
{
	CBnsConfig::GetInstance().ClearAccountConfig(pwszAccountName);

	return HandleAccount(pwszAccountName, [&](ACCOUNT_INFO_GAME& AccountGame)
	{
		if (AccountGame.AccountStatus.bLogining)
		{
			LOG_CF_D(L"删除的帐号正在登录中……怒关闭!");
		}
		if (AccountGame.dwPid != NULL) // 该帐号正在运行, 怒强制关闭!
		{
			CConsoleFun::GetInstance().KillGame_By_AccountName(&AccountGame);
		}

		// 已经分配了卡号资源, 回收
		CConsoleCard::GetInstance().ExistCard(AccountGame.MyAcount_Info.szCardNo, [&AccountGame](CardInfo& CardInfo_)
		{
			LOG_CF_D(L"回收该帐号%s的卡号资源!", AccountGame.MyAcount_Info.szUserName);
			CardInfo_.bExit = FALSE;
		});

		g_pSharedInfo->nAccountCount -= 1;
		ZeroMemory(&AccountGame, sizeof(ACCOUNT_INFO_GAME));
		return TRUE;
	});
}

BOOL CAccountMgr::RestartAccount(_In_ LPCWSTR pwszAccountName)
{
	return HandleAccount(pwszAccountName, [&](ACCOUNT_INFO_GAME& AccountGame)
	{
		if (AccountGame.AccountStatus.bLogining)
		{
			LOG_CF_D(L"重登的帐号正在登录中……怒关闭!");
		}

		if (AccountGame.dwPid != NULL) // 该帐号正在运行, 怒强制关闭!
		{
			LOG_CF_D(L"重启该帐号, 怒关闭Account!");
			AccountGame.AccountStatus.bClose = TRUE;
			AccountGame.AccountStatus.emCloseResult = em_Close_Result_ReStart;
			CConsoleFun::GetInstance().KillGame_By_AccountName(&AccountGame);
		}
		return TRUE;
	});
}

VOID CAccountMgr::HandleEachAccount(std::function<void(ACCOUNT_INFO_GAME& AccountGame)> AcionPtr)
{
	std::for_each(begin(g_pSharedInfo->arrGameInfo), end(g_pSharedInfo->arrGameInfo), AcionPtr);
}
