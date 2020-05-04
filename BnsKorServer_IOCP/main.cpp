#include <iostream>
#include "GameServer.h"
#include <MyTools/Log.h>
#include <MyTools/Character.h>
#include <MyTools/CLFile.h>
#include <MyTools/CLPublic.h>
#include "MiniDump.h"

VOID SetLogSaveDirectory()
{
	WCHAR wszPath[MAX_PATH] = { 0 };
	::GetCurrentDirectoryW(MAX_PATH, wszPath);

	std::wstring wsPath = MyTools::CCharacter::MakeFormatText(L"%s\\SaveLog\\", wszPath);
	if (!MyTools::CLPublic::FileExist(wsPath))
	{
		MyTools::CLFile::CreateMyDirectory(wsPath.c_str());
	}

	MyTools::CLog::GetInstance().SetClientName(L"KorBnsServer", wsPath);
}

VOID ExistServer()
{
	static WCHAR wszMutexName[32] = {L"CL_BNS_SERVER"};
	if (::OpenMutexW(MUTEX_ALL_ACCESS, FALSE, wszMutexName) != NULL)
	{
		std::wcout << L"Server Already Exist!" << std::endl;
		::Sleep(INFINITE);
	}

	::CreateMutexW(NULL, FALSE, wszMutexName);
}

int main()
{
	::GetCurrentDirectoryW(MAX_PATH, g_wszDumpPath);

	MyTools::CCharacter::SetConsoleLanguage();
	MyTools::CCharacter::SetSpecialCharacterMode();
	ExistServer();
	SetLogSaveDirectory();

	CGameServer GameServer;
	GameServer.InitializeBnsService();
	if(GameServer.Run(12376, 10000))
		std::wcout << L"Running....." << std::endl;
	else
		std::wcout << L"Run Server Faild!" << std::endl;

	::Sleep(INFINITE);
	return 0;
}