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
		CBnsConfig::GetInstance().WriteAccountLog(&ACCOUNT_INFO_GAME_, L"������¼3�ζ�ʧ��!");
	if (ACCOUNT_INFO_GAME_.AccountStatus.uCreatePlayerFaildCount >= 3)
		CBnsConfig::GetInstance().WriteAccountLog(&ACCOUNT_INFO_GAME_, L"����������ɫ3�ζ�ʧ��!");

	return TRUE;
}

BOOL CAccountMgr::SetResetAccount(_In_ ACCOUNT_INFO_GAME& AccountGame)
{
	//fn(AccountGame.MyAcount_Info.szUserName, L"������! �����Ϻ�!");
	LOG_CF_D( L"�ʺ�:[%s,%X] ������!�����Ϻ�!", AccountGame.MyAcount_Info.szUserName, AccountGame.dwPid);
	AccountGame.AccountStatus.bClose = FALSE;
	CConsoleFun::GetInstance().KillGame_By_AccountName(&AccountGame); // �ر���Ϸ
	return CConsoleFun::GetInstance().RunGame(&AccountGame);					// ��������
}

BOOL CAccountMgr::SetClearAccount(_In_ ACCOUNT_INFO_GAME& ACCOUNT_INFO_GAME_)
{
	if (!CConsoleCard::GetInstance().ExistCard(ACCOUNT_INFO_GAME_.MyAcount_Info.szCardNo, [](CardInfo& CardInfo_) {CardInfo_.bExit = FALSE; })) // ������ʺŵĿ���"�黹" ����, ���ºſ�����!
	{
		LOG_CF_E(L"���ŷ��䲻��!");
		return TRUE;
	}

	// �Ѹ��ʺ����!
	if (ACCOUNT_INFO_GAME_.AccountStatus.bDelete)
	{
		LOG_CF_D(L"bDelete = TRUE,ɾ��Account.txt�ĸ��ʺ�[%s]", ACCOUNT_INFO_GAME_.GetAccName());
		CBnsConfig::GetInstance().ClearAccountConfig(ACCOUNT_INFO_GAME_.GetAccName());
	}

	CConsoleFun::GetInstance().KillGame_By_AccountName(&ACCOUNT_INFO_GAME_); // �ر���Ϸ
	ACCOUNT_INFO_GAME_.AccountStatus.bExist = FALSE;
	//ZeroMemory(&ACCOUNT_INFO_GAME_, sizeof(ACCOUNT_INFO_GAME)); // ��������ṹ�� ������ʺ���
	return TRUE;
}

BOOL CAccountMgr::AddAccount(_In_ LPCWSTR pwszAccountName, _In_ LPCWSTR pwszAccountPass)
{
	if (IsExistAccount(pwszAccountName) != nullptr)
		return TRUE;

	auto pAccountGame = g_pSharedInfo->ExistEmptyArrGameInfo(); // ��һ��bExist = FALSE�Ŀ�λ
	if (pAccountGame == nullptr)
	{
		LOG_MSG_CF(L"�����������%d���ʺ�����!", MAX_GAME_COUNT);
		return FALSE;
	}

	MyTools::CCharacter::wstrcpy_my(pAccountGame->MyAcount_Info.szUserName, pwszAccountName, _countof(pAccountGame->MyAcount_Info.szUserName) - 1);
	MyTools::CCharacter::wstrcpy_my(pAccountGame->MyAcount_Info.szPassWord, pwszAccountPass, _countof(pAccountGame->MyAcount_Info.szPassWord) - 1);
	pAccountGame->AccountStatus.bExist = TRUE;
	g_pSharedInfo->nAccountCount += 1; // ��ʱû��
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
			LOG_CF_D(L"ɾ�����ʺ����ڵ�¼�С���ŭ�ر�!");
		}
		if (AccountGame.dwPid != NULL) // ���ʺ���������, ŭǿ�ƹر�!
		{
			CConsoleFun::GetInstance().KillGame_By_AccountName(&AccountGame);
		}

		// �Ѿ������˿�����Դ, ����
		CConsoleCard::GetInstance().ExistCard(AccountGame.MyAcount_Info.szCardNo, [&AccountGame](CardInfo& CardInfo_)
		{
			LOG_CF_D(L"���ո��ʺ�%s�Ŀ�����Դ!", AccountGame.MyAcount_Info.szUserName);
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
			LOG_CF_D(L"�صǵ��ʺ����ڵ�¼�С���ŭ�ر�!");
		}

		if (AccountGame.dwPid != NULL) // ���ʺ���������, ŭǿ�ƹر�!
		{
			LOG_CF_D(L"�������ʺ�, ŭ�ر�Account!");
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
