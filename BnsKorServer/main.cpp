#include <iostream>
#include <string>
#include <iterator>
#include <MyTools/CLFile.h>
#include <MyTools/CLPublic.h>
#include "MiniDump.h"
#include <set>
#include <map>
#include <vector>
#include <thread>
#include "VerSocket.h"
#include "ClientConfig.h"
#include <MyTools/ClassInstance.h>
#include <MyTools/Log.h>
#include <MyTools/CLResManager.h>
#include <MyTools/CLProcess.h>
#include "OneTimePassword.h"
#include "LoginOTP.h"
#define _SELF L"main.cpp"
BOOL InitBnsServerArray()
{
	vBnsServerArray.push_back({ { L"3区", L"경국지색", 0x3 } });
	vBnsServerArray.push_back({ { L"23区", L"절세미인", 0x2E } });
	vBnsServerArray.push_back({ { L"21区", L"일확천금", 0x4 }, { L"27区", L"천하태평", 0x17 }, { L"31区", L"파죽지세", 0x10 } });
	vBnsServerArray.push_back({ { L"18区", L"이심전심", 0x5 }, { L"4区", L"공명정대", 0x1A }, { L"7区", L"금상첨화", 0x19 } });
	vBnsServerArray.push_back({ { L"13区", L"영웅호걸", 0x34 }, { L"8区", L"대도무문", 0x1F }, { L"20区", L"일취월장", 0x16 } });
	vBnsServerArray.push_back({ { L"9区", L"명불허전", 0x1 }, { L"5区", L"군계일학", 0x7 }, { L"17区", L"외유내강", 0x14 } });
	vBnsServerArray.push_back({ { L"16区", L"유아독존", 0x9 }, { L"26区", L"천하무적", 0x6 }, { L"28区", L"区쾌도난마", 0xF } });
	vBnsServerArray.push_back({ { L"6区", L"금강불괴", 0x2 }, { L"10区", L"백전백승", 0xE }, { L"29区", L"태산북두", 0x15 } });
	vBnsServerArray.push_back({ { L"22区", L"전광석화", 0x11 }, { L"30区", L"태평성대", 0x12 }, { L"1区", L"강호연파", 0x18 }, { L"24区", L"천양무궁", 0x1C } });
	vBnsServerArray.push_back({ { L"11区", L"비룡재천", 0x8 }, { L"32区", L"혈풍혈우", 0xA }, { L"15区", L"용호상박", 0xB }, { L"19区", L"일도양단", 0xC } });
	vBnsServerArray.push_back({ { L"2区", L"건곤일척", 0xD }, { L"14区", L"용맹정진", 0x13 }, { L"12区", L"신검합일", 0x1B }, { L"25区", L"천우신조", 0x1D } });

	
	return TRUE;
}

VOID InitializeLog()
{
	WCHAR wszLogPath[MAX_PATH];
	::GetCurrentDirectoryW(MAX_PATH, wszLogPath);
	::lstrcatW(wszLogPath, L"\\SaveLog\\");
	if (!CLPublic::FileExit(wszLogPath))
		CLFile::CreateMyDirectory(wszLogPath);

	CLog::GetInstance().SetClientName(L"KorServer", wszLogPath, FALSE, 20 * 1024 * 1000);
}

void TestOtp()
{
	std::wcout << L"Input Your AccountName:";
	std::wstring wsAccountName;
	std::wcin >> wsAccountName;

	std::wcout << L"Input Your Key:";
	std::wstring wsKey;
	std::wcin >> wsKey;

	for (;;)
	{
		std::wcout << COneTimePassword::GetInstance().CalcOPTValue(wsAccountName, wsKey).c_str() << std::endl;
		::Sleep(1000);
	}
}

VOID ChoiceMode()
{ 
	BOOL bTestOTP = FALSE;
	for (;;)
	{
		std::wstring wsCmdText;
		std::wcout << L"Input Your Cmd:" << std::endl;
		std::wcin >> wsCmdText;
		if (wsCmdText == L"OTP")
		{
			TestOtp();
		}
		else if (wsCmdText == L"TestOTP" && !bTestOTP)
		{
			std::wcout << L"Reading Account.txt" << std::endl;
			::CreateMutexW(NULL, FALSE, L"CL_BNS_KOR");
			CLoginOTP::GetInstance().Run();
			bTestOTP = TRUE;
		}
		else if (wsCmdText == L"Server")
		{
			return;
		}
		else if (wsCmdText == L"Kill")
		{
			CLProcess::TerminateProc_For_ProcName(L"Client.exe");
		}
		else
			std::wcout << L"Invalid Cmd!" << std::endl;
	}
	
}

int main(int argc, char* argv[])
{
	// 多开判断!
	InitializeLog();
	/*if (::OpenMutexW(MUTEX_ALL_ACCESS, FALSE, L"NewBnsKorServer") != NULL)
	{
		std::cout << "……你开多个验证服务器有意义嘛?" << std::endl;
		Sleep(5000);
		return 0;
	}*/
	// Create Mutex!
	::CreateMutexW(NULL, FALSE, L"NewBnsKorServer");


	ChoiceMode();

	
	SetConsole_Language_CHINA;
	//TestOtp();

	// 注册Dump路径!
	::GetCurrentDirectoryW(MAX_PATH, g_wszDumpPath);
	RegDumpFunction();

	// Init GameServer infomation
	InitBnsServerArray();

	// set break connect funptr
	CSocketServer::GetInstance().SetBreakConnect([](std::shared_ptr<SocketClientInfo> SocketClientBufferPtr)
	{
		//LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"帐号:%s IP%s 断开连接!", pSocketClientBuffer->wsAccountName.c_str(), pSocketClientBuffer->SocketSession_.wsClientIp.c_str());
		//LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"IP:%s Account:%s, 断开连接!", pSocketClientBuffer->SocketSession_.wsClientIp.c_str(), pSocketClientBuffer->wsAccountName.c_str());

		// Remove Team Member when Account break connect!
		if (SocketClientBufferPtr->ClientAccountContent_.wsAccountName != L"Empty")
		{
			CBnsTaskTeam::GetInstance().DeleteTaskMember(SocketClientBufferPtr->ClientAccountContent_.wsAccountName);
			//CBnsTaskTeam::GetInstance().DeleteDungoenTeamMember(pSocketClientBuffer->wsAccountName);
		}

		// Add Account infomaction to db when account break connect!
		CVerManage::AddBreakConnectInfo(SocketClientBufferPtr->SocketSession_.wsClientIp, SocketClientBufferPtr->ClientAccountContent_.wsAccountName);
	});

	


	// Get all of Db Card!
	CVerManage::DoAction_By_CardList([](std::deque<CardInfo>& vlst){CVerManage::QueryServerCard(vlst); });

	// 清除过期的云配置文件
	CClientConfig::GetInstance().ClearDBConfig(30);

	// 初始化云配置文件!
	CClientConfig::GetInstance().Initialize();

	// Get All of Script file to memory!
	CUpdateFile::UpdateScript();

	// create handle socket thread
	CONST DWORD dwThreadCount = 5;
	ULONGLONG ulKeepALiveThreadArray[dwThreadCount] = { 0 };
	for (UINT i = 0; i < dwThreadCount; ++i)
	{
		auto hWorkThread = cbBEGINTHREADEX(NULL, NULL, CVerSocket::_WorkThread, &ulKeepALiveThreadArray[i], NULL, NULL);
		SetResDeleter(hWorkThread, [](HANDLE& hThread){::CloseHandle(hThread); });
		if (hWorkThread == NULL)
		{
			LOG_MSG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"CreateThread _WorkThread Fiald!");
		}
	}

	// create check online account thread
	auto hKeepALiveThread = cbBEGINTHREADEX(NULL, NULL, CVerSocket::_KeepALiveThread, ulKeepALiveThreadArray, NULL, NULL);
	SetResDeleter(hKeepALiveThread, [](HANDLE& hThread){::CloseHandle(hThread); });
	if (hKeepALiveThread == NULL)
	{
		LOG_MSG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"CreateThread _KeepALiveThread Fiald!");
	}

	// 
	CVerManage::GetBnsVersion(TRUE);
	COneTimePassword::GetInstance().RefreshDBToMemory();

	// run socket!
	std::cout << "Running……" << std::endl;
	CSocketServer::GetInstance().Run(12376, 15);
	LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"Runing……");
	for (;;)
	{
		std::string str;
		std::getline(std::cin, str);

		if (str == "UpdateScript")
		{
			SYSTEMTIME stLocal;
			::GetLocalTime(&stLocal);

			CHAR szText[64] = { 0 };
			sprintf_s(szText,_countof(szText) - 1, "%d月%d日 %d:%d:%d :", stLocal.wMonth, stLocal.wDay, stLocal.wHour, stLocal.wMinute, stLocal.wSecond);
			
			std::cout << szText << (CUpdateFile::UpdateScript() ? "Succ!" : "Fiald!") << std::endl;
		}
		else if (str == "OTP")
		{
			COneTimePassword::GetInstance().ReadFileToDB();
		}
		else if (str == "RefushCard")
		{
			// refresh all of card in db
			std::deque<CardInfo> tmpList;
			CDataBaseMdel::GetCardList(tmpList);

			CVerManage::DoAction_By_CardList([&tmpList](std::deque<CardInfo>& vlst){

				for (auto& itm : tmpList)
				{
					auto p = CLPublic::Deque_find_if(vlst, [&itm](CONST CardInfo& Card){ return Card.wstrCardNo == itm.wstrCardNo; });
					if (p != nullptr)
						itm = *p;
				}

				vlst.clear();
				vlst.assign(tmpList.begin(), tmpList.end());
			});
			std::cout << "Succ!" << std::endl;
		}
		else if (str.find("FindCard") != -1)
		{
			int nIndex = str.find("-");
			if (nIndex == -1)
				continue;

			str = str.substr(nIndex + 1);
			std::cout << "搜索的卡号是:" << str.c_str() << std::endl;
			CONST std::wstring wsCardNo = CCharacter::ASCIIToUnicode(str);
			CVerManage::DoAction_By_CardList([&wsCardNo](std::deque<CardInfo>& vlst)
			{
				auto p = CLPublic::Deque_find_if(vlst, [&wsCardNo](CONST CardInfo& Card){return Card.wstrCardNo == wsCardNo; });
				if (p == nullptr)
				{
					std::wcout << L"Empty" << std::endl;
					return;
				}
				
				LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"Crc=%X,Hour=%d,IP=%s,Card=%s", p->dwCardCrc, p->GetSurplusHour(), p->wstrIp.c_str(), p->wstrCardNo.c_str());
			});
		}
		else if (str == "PrintTaskTeam")
		{
			CBnsTaskTeam::GetInstance().PrintTaskTeam();
		}
		else if (str.find("Find TeamMember By ") != -1)
		{
			if (str.find("AccountName") != -1)
			{
				int nIndex = str.find("-");
				if (nIndex != -1)
				{
					str = str.substr(0, nIndex + 1);
					CBnsTaskTeam::GetInstance().SearchGroupByAccountName(CCharacter::ASCIIToUnicode(str), [](CONST TeamMemberGroup& itm){
						LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"Server=%X,Channel=%X,Task=[%X,%X],MaxCount=%d,MemberCount=%d", \
							itm.dwServerId,itm.dwChannelId,itm.dwChannelId,itm.dwTaskId,itm.dwScheduleId, itm.uMaxMemberCount, itm.MemberVec.size());

						for (CONST auto& TeamMember : itm.MemberVec)
						{
							LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"%s,[%s,%s,%X]", TeamMember.bCaptain ? L"队长" : L"队员", TeamMember.Member.wsAccountName.c_str(), \
								TeamMember.Member.wsPlayerName.c_str(), TeamMember.Member.dwPlayerId);

						}
					});
					continue;
				}
			}
			else if (str.find("TaskId") != -1 || str.find("TaskID") != -1)
			{
				auto GetTaskInfoParm_By_ScriptParm = [](CONST std::wstring& wstr)
				{
					TaskInfoParm TaskInfoParm_;
					std::vector<std::wstring> vlst;

					if (CCharacter::Split(wstr, L",", vlst, Split_Option_RemoveEmptyEntries | Split_Option_KeepOnly) == 2)
					{
						TaskInfoParm_.dwTaskId = static_cast<DWORD>(wcstol(vlst.at(0).c_str(), nullptr, 16));
						TaskInfoParm_.dwScheduleId = static_cast<DWORD>(wcstol(vlst.at(1).c_str(), nullptr, 16));
					}
					else
					{
						LOG_MSG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"无法分解任务ID! %s", wstr.c_str());
					}

					return TaskInfoParm_;
				};

				int nIndex = str.find("-");
				if (nIndex != -1)
				{
					str = str.substr(0, nIndex + 1);
					CBnsTaskTeam::GetInstance().SearchGroupByTask(GetTaskInfoParm_By_ScriptParm(CCharacter::ASCIIToUnicode(str)), [](CONST TeamMemberGroup& itm){
						LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"Server=%X,Channel=%X,Task=[%X,%X],MaxCount=%d,MemberCount=%d", \
							itm.dwServerId, itm.dwChannelId, itm.dwChannelId, itm.dwTaskId, itm.dwScheduleId, itm.uMaxMemberCount, itm.MemberVec.size());

						for (CONST auto& TeamMember : itm.MemberVec)
						{
							LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"%s,[%s,%s,%X]", TeamMember.bCaptain ? L"队长" : L"队员", TeamMember.Member.wsAccountName.c_str(), \
								TeamMember.Member.wsPlayerName.c_str(), TeamMember.Member.dwPlayerId);

						}
					});
					continue;
				}
			}
			else if (str.find("Dungoen") != -1)
			{
				int nIndex = str.find("-");
				if (nIndex != -1)
				{
					str = str.substr(0, nIndex + 1);
					CBnsTaskTeam::GetInstance().SearchGroupByDungoneName(CCharacter::ASCIIToUnicode(str), [](CONST TeamMemberGroup& itm){
						LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"Server=%X,Channel=%X,Task=[%X,%X],MaxCount=%d,MemberCount=%d,Dungone=%s,IsReadOnly=%d", \
							itm.dwServerId, itm.dwChannelId, itm.dwChannelId, itm.dwTaskId, itm.dwScheduleId, itm.uMaxMemberCount, itm.MemberVec.size(), itm.wsDungoenName.c_str(), itm.IsReadOnly);

						for (CONST TeamMemberInfo& TeamMember : itm.MemberVec)
						{
							LOG_CF(CLog::em_Log_Type::em_Log_Type_Debug, L"%s,[%s,%s,%X],Dungone:[%s,%d]", TeamMember.bCaptain ? L"队长" : L"队员", TeamMember.Member.wsAccountName.c_str(), \
								TeamMember.Member.wsPlayerName.c_str(), TeamMember.Member.dwPlayerId, TeamMember.Dungoen.wsDungoenSchedule.c_str(), TeamMember.Dungoen.uGatherIndex);
						}
					});
					continue;
				}
			}
			
			std::cout << "Invalid 'Find TeamMember By' Parmamter!" << std::endl;
		}
		else if (str == "TestOTP")
		{
			std::wcout << L"Input Your AccountName:";
			std::wstring wsAccountName;
			std::wcin >> wsAccountName;

			std::wcout << L"Input Your Key:";
			std::wstring wsKey;
			std::wcin >> wsKey;

			std::wcout << COneTimePassword::GetInstance().CalcOPTValue(wsAccountName, wsKey).c_str() << std::endl;
		}
		else
		{
			std::cout << "Invalid cmd!" << std::endl;
		}
	}
	
	return 0;
}