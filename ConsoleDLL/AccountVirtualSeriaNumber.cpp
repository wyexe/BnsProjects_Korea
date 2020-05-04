#include "stdafx.h"
#include "AccountVirtualSeriaNumber.h"
#include <MyTools/Log.h>
#include <MyTools/Character.h>
#include "ConsoleVariable.h"
#include "SocketAPI.h"

#define _SELF L"AccountVirtualSeriaNumber.cpp"
CAccountVirtualSeriaNumber::CAccountVirtualSeriaNumber()
{
	_hFileMap = NULL;
	_pVirtualSeriaNumberShareContent = nullptr;
}

CAccountVirtualSeriaNumber::~CAccountVirtualSeriaNumber()
{
	if (_pVirtualSeriaNumberShareContent != nullptr)
	{
		::UnmapViewOfFile(_pVirtualSeriaNumberShareContent);
		_pVirtualSeriaNumberShareContent = nullptr;
	}
	if (_hFileMap != NULL)
	{
		::CloseHandle(_hFileMap);
		_hFileMap = NULL;
	}
}

VOID CAccountVirtualSeriaNumber::Create()
{
	_hFileMap = ::CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, NULL, sizeof(VirtualSeriaNumberShareContent), SHARENAME_VIRTUALSERIANUMBER);
	if (_hFileMap == NULL)
	{
		LOG_MSG_CF(L"CreateFileMappingW = NULL!!!");
		return;
	}


	_pVirtualSeriaNumberShareContent = reinterpret_cast<VirtualSeriaNumberShareContent *>(::MapViewOfFile(_hFileMap, FILE_MAP_READ | FILE_MAP_WRITE, NULL, NULL, sizeof(VirtualSeriaNumberShareContent)));
	if (_pVirtualSeriaNumberShareContent == nullptr)
	{
		LOG_MSG_CF(L"MapViewOfFile = NULL!!! Err=%d", ::GetLastError());
		return;
	}

	_pVirtualSeriaNumberShareContent->uCount = 0;
	//ZeroMemory(_pVirtualSeriaNumberShareContent, sizeof(VirtualSeriaNumberShareContent));
}

VOID CAccountVirtualSeriaNumber::Fill(_In_ CONST AccountVirtualSeriaNumberContent& Content)
{
	if (_pVirtualSeriaNumberShareContent->uCount >= MAX_ACCOUNT_VIRTUALSERIANUMBER)
	{
		LOG_MSG_CF(L"_pVirtualSeriaNumberShareContent->uCount[%d] >= %d", _pVirtualSeriaNumberShareContent->uCount, MAX_ACCOUNT_VIRTUALSERIANUMBER);
		return;
	}
	_pVirtualSeriaNumberShareContent->AccountContent[_pVirtualSeriaNumberShareContent->uCount++] = Content;
}

VOID CAccountVirtualSeriaNumber::Clear()
{
	//ZeroMemory(_pVirtualSeriaNumberShareContent, sizeof(VirtualSeriaNumberShareContent));
}

VOID CAccountVirtualSeriaNumber::Initialize()
{
	try
	{
		std::vector<std::wstring> VecAccount;
		for (int i = 0; i < g_pSharedInfo->nAccountCount; ++i)
		{
			auto& pAccGame = g_pSharedInfo->arrGameInfo[i];
			if (!pAccGame.AccountStatus.bExist)
				continue;

			std::wstring wsAccountName = pAccGame.GetAccName();
			VecAccount.push_back(wsAccountName);
		}

		std::vector<SocketAccountVirtualSeriaNumber> VecVirtualSeriaNumber;
		CSocketAPI().QueryAccountVirtualSeriaNumber(VecAccount, VecVirtualSeriaNumber);

		Clear();
		for (auto& itm : VecVirtualSeriaNumber)
		{
			AccountVirtualSeriaNumberContent Content;
			MyTools::CCharacter::wstrcpy_my(Content.wsAccountName, itm.wsAccountName.c_str());
			MyTools::CCharacter::wstrcpy_my(Content.wsHardDiskSeriaNumber, itm.wsHardDiskSeriaNumber.c_str());
			MyTools::CCharacter::wstrcpy_my(Content.wsMacAddr, itm.wsMacAddr.c_str());
			MyTools::CCharacter::wstrcpy_my(Content.wsNetCard, itm.wsNetCard.c_str());
			Fill(Content);
		}
	}
	catch (const std::exception& ex)
	{
		LOG_MSG_CF(L"Initialize:%s", MyTools::CCharacter::ASCIIToUnicode(ex.what()).c_str());
	}
	
}
