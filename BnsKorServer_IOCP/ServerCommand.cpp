#include "ServerCommand.h"
#include <MyTools/Log.h>
#include <MyTools/CmdLog.h>
#include "CardExtend.h"
#include "CloudConfigExtend.h"
#include "BnsOTP.h"
#include "ScriptFile.h"
#include "OnLineOutput.h"

#define _SELF L"ServerCommand.cpp"

VOID CServerCommand::Release()
{

}

VOID CServerCommand::Initialize()
{
	MyTools::CCmdLog::GetInstance().Run(L"KorBnsServer", GetVec());
}

std::vector<MyTools::ExpressionFunPtr>& CServerCommand::GetVec()
{
	static std::vector<MyTools::ExpressionFunPtr> Vec = 
	{
		{ std::bind(&CServerCommand::Help, this, std::placeholders::_1), L"Help" },
		{ std::bind(&CServerCommand::Refresh, this, std::placeholders::_1), L"Refresh" },
		{ std::bind(&CServerCommand::FindCard, this, std::placeholders::_1), L"FindCard" },
		{ std::bind(&CServerCommand::FindAccount, this, std::placeholders::_1), L"FindAccount" },
		{ std::bind(&CServerCommand::FindPlayer, this, std::placeholders::_1), L"FindPlayer" },
		{ std::bind(&CServerCommand::FindTeamMember, this, std::placeholders::_1), L"FindTeamMember" },
		{ std::bind(&CServerCommand::ClearLog, this, std::placeholders::_1), L"ClearLog" },
	};

	return Vec;
}

VOID CServerCommand::Help(CONST std::vector<std::wstring>&)
{
	auto& Vec = GetVec();
	for (CONST auto& itm : Vec)
		LOG_CF(MyTools::CLog::em_Log_Type::em_Log_Type_Custome, L"FunctionName=%s", itm.wsFunName.c_str());
}

VOID CServerCommand::Refresh(CONST std::vector<std::wstring>& VecParam)
{
	if (VecParam.size() == 0)
	{
		LOG_CF_E(L"UnExist Type Param!");
		return;
	}

	auto& TypText = VecParam.at(0);
	if (TypText == L"OTP")
		CBnsOTP::GetInstance().ReadOTP_In_File_To_Db();
	else if (TypText == L"CloudConfig")
		CCloudConfigExtend::GetInstance().Initialize();
	else if (TypText == L"Card")
		CCardExtend::GetInstance().Initialize();
	else if (TypText == L"Script")
		CScriptFile::GetInstance().Initialize();
	else
	{
		LOG_CF_E(L"Ivalid Type Param!");
		return;
	}
}


VOID CServerCommand::FindCard(CONST std::vector<std::wstring>& VecParam)
{
	if (VecParam.size() == 0)
	{
		LOG_CF_E(L"UnExist CardNo!");
		return;
	}

	CardInfo Card;
	if (!CCardExtend::GetInstance().FindCard_By_CardNo(VecParam.at(0), Card))
	{
		LOG_CF_E(L"UnExist CardNo[%s]!", VecParam.at(0).c_str());
		return;
	}

	COnLineOutput::GetInstance().PrintCardContent(Card);
}

VOID CServerCommand::FindAccount(CONST std::vector<std::wstring>& VecParam)
{
	if (VecParam.size() == 0)
	{
		LOG_CF_E(L"UnExist PlayerName!");
		return;
	}

	CONST auto& wsAccountName = VecParam.at(0);

	std::wstring wsPlayerName;
	COnLineOutput::GetInstance().PrintCardByAccountName(wsAccountName, &wsPlayerName);
	if (!wsPlayerName.empty())
		COnLineOutput::GetInstance().PrintPlayerKeepALiveContent(wsPlayerName);
}

VOID CServerCommand::FindPlayer(CONST std::vector<std::wstring>& VecParam)
{
	if (VecParam.size() == 0)
	{
		LOG_CF_E(L"UnExist PlayerName!");
		return;
	}

	CONST auto& wsPlayerName = VecParam.at(0);
	COnLineOutput::GetInstance().PrintPlayerKeepALiveContent(wsPlayerName);
	COnLineOutput::GetInstance().PrintCardByPlayerName(wsPlayerName);
}

VOID CServerCommand::FindTeamMember(CONST std::vector<std::wstring>& VecParam)
{
	if (VecParam.size() == 0)
	{
		LOG_CF_E(L"UnExist AccountName!");
		return;
	}

	COnLineOutput::GetInstance().PrintTeamByAccount(VecParam.at(0));
}

VOID CServerCommand::ClearLog(CONST std::vector<std::wstring>&)
{
	MyTools::CLog::GetInstance().Clear();
}
