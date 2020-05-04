#include "BnsOTP.h"
#include <iostream>
#include <future>
#include <MyTools/Character.h>
#include <MyTools/CLPublic.h>
#include <MyTools/CLFile.h>
#include <MyTools/CLEchoException.h>
#include "DbManager.h"
#include "CLHmacSha1.h"

BOOL CBnsOTP::Initialize()
{
	BOOL bRetCode = FALSE;
	_Lock.Access([&] 
	{ 
		bRetCode = CBnsDbManager::GetInstance().GetOTPContent(_MapOTP); 
		std::wcout << L"Total OTP Key=" << _MapOTP.size() << std::endl;
	});
	return bRetCode;
}

BOOL CBnsOTP::ReadOTP_In_File_To_Db()
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		auto Vec = ReadOTPFile();
		if (Vec.size() == 0)
			return FALSE;

		std::wcout << L"Exist OTP Count:" << Vec.size() << L", Ready Save To DB" << std::endl;
		for (CONST auto& itm : Vec)
			CBnsDbManager::GetInstance().AsyncExcuteSQL(MyTools::CCharacter::MakeFormatText(L"exec InsertOTPAccountContent '%s','%s'", itm.wsAccountName.c_str(), itm.wsAccountKey.c_str()));

		std::wcout << L"Done!" << std::endl;

		// Reload
		return Initialize();
	});
}

BOOL CBnsOTP::FindOTPKey_By_AccountName(_In_ CONST std::wstring& wsAccountName, _Out_opt_ std::wstring& wsAccountKey) CONST
{
	BOOL bExist = FALSE;
	_Lock.Access([&] 
	{
		auto itr = _MapOTP.find(wsAccountName);
		if (itr != _MapOTP.end())
		{
			bExist = TRUE;
			wsAccountKey = itr->second;
		}
	});
	return bExist;
}

BOOL CBnsOTP::CalculateOTP(_In_ CONST std::wstring& wsAccountKey, _Out_opt_ DWORD& dwKey) CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&] 
	{
		return CLHmacSha1::GetInstance().CalcOTPValue(wsAccountKey, dwKey);
	});
}

std::vector<CBnsOTP::OTPContent> CBnsOTP::ReadOTPFile() CONST
{
	WCHAR wszPath[MAX_PATH] = { 0 };
	::GetCurrentDirectoryW(MAX_PATH, wszPath);
	::lstrcatW(wszPath, L"\\OTP.txt");

	if (!MyTools::CLPublic::FileExist(wszPath))
	{
		std::wcout << L"'OTP.txt' UnExist in CurrentDirectory!" << std::endl;
		return std::vector<OTPContent>();
	}

	std::wstring wsContent;
	if (!MyTools::CLFile::ReadUnicodeFile(wszPath, wsContent))
	{
		std::wcout << L"Read File 'OTP.txt' Faild!" << std::endl;
		return std::vector<OTPContent>();
	}

	// ReWrite File
	//MyTools::CLFile::WriteUnicodeFile(wszPath, L"");
	
	std::vector<OTPContent> VecAccountOTP;
	std::vector<std::wstring> VecText;

	MyTools::CCharacter::Split(wsContent, L"\r\n", VecText, Split_Option_KeepOnly | Split_Option_RemoveEmptyEntries);
	for (CONST auto& itm : VecText)
	{
		std::vector<std::wstring> VecParam;
		if (MyTools::CCharacter::Split(itm, L",",VecParam, Split_Option_KeepOnly | Split_Option_RemoveEmptyEntries) != 2)
		{
			std::wcout << L"Account Format Invalid:" << itm << std::endl;
			continue;
		}

		VecAccountOTP.emplace_back(std::move(VecParam.at(0)), std::move(VecParam.at(1)));
	}

	return std::move(VecAccountOTP);
}
