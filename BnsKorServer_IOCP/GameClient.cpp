#include "GameClient.h"
#include <MyTools/Log.h>
#include <MyTools/Character.h>
#include <MyTools/CLEchoException.h>
#include "BnsTeam.h"
#include "DbManager.h"
#include "CardExtend.h"
#include "CrossAreaTeam.h"
#include "BnsTrade.h"

#define _SELF L"GameClient.cpp"

std::atomic<DWORD> CGameClient::_dwGameClientCount = 0;

CGameClient::CGameClient(_In_ SOCKET Sock) : MyTools::CLSocketClient(Sock), _dwToken(NULL)
{
	_TeamContent = nullptr;
	_OnLineTick = ::GetTickCount64();
	_dwGameClientCount += 1;
}

CGameClient::~CGameClient()
{
	_dwGameClientCount -= 1;
	DisConnect();
}

VOID CGameClient::Remove()
{
	MyTools::CLEchoException::GetInstance().InvokeAction(__FUNCTIONW__, [=]
	{
		if (!_Account.wsAccountName.empty() && _Account.wsAccountName != L"Empty")
		{
			// 帐号掉线的时候, 直接把主线组队和日常组队的队员给删了!
			CBnsTaskTeam::GetInstance().DeleteTaskMember(_Account.wsAccountName);

			// 跨服组队也删掉
			CCrossAreaTeam::GetInstance().DeleteTeamMember(_Account.wsAccountName);

			// 删除交易
			CBnsTrade::GetInstance().RemoveAccount(_Account.wsAccountName, _Account.wsPlayerName);

			// 添加到掉线记录
			CBnsDbManager::GetInstance().AddBreakConnectLogToDb(_Account);

			// 清空相关卡号的内容
			if (_Account.dwCardCrc != NULL)
			{
				CCardExtend::GetInstance().SetCardContent(_Account.dwCardCrc, [](CardInfo& Card)
				{
					Card.wsPlayerName.clear();
					Card.wsAccountName.clear();
					Card.wsIp.clear();
					Card.bExit = FALSE;
					Card.dwToken = NULL;
				});
			}
		}

		auto FormatTimePtr = [](_In_ ULONGLONG ulTick)
		{
			DWORD dwMinute = static_cast<DWORD>((::GetTickCount64() - ulTick) / 1000 / 60);
			DWORD dwHour = dwMinute / 60;
			dwMinute -= dwHour * 60;

			return MyTools::CCharacter::MakeFormatText(L"%d小时%d分钟", dwHour, dwMinute);
		};
		LOG_CF_D(L"DisConnect[%s:%s], OnLine[%s]", _Account.wsAccountName.c_str(), _Account.wsPlayerName.c_str(), FormatTimePtr(_OnLineTick).c_str());
	});

	DisConnect();
}

VOID CGameClient::Add()
{
	_OnLineTick = ::GetTickCount64();
	_bExist = true;
}

BOOL CGameClient::IsOnLine() CONST
{
	return _Account.ulKeepALiveTick != NULL && (::GetTickCount64() - _Account.ulKeepALiveTick <= 60 * 1000);
}


BOOL CGameClient::IsKeepALiveTimeout() CONST
{
	return _Account.ulKeepALiveTick != NULL && (::GetTickCount64() - _Account.ulKeepALiveTick > 60 * 1000);
}

ClientAccountContent& CGameClient::GetAccountContent()
{
	return _Account;
}

std::shared_ptr<ClientTeamInfo> CGameClient::GetClientTeamInfo()
{
	if (_TeamContent == nullptr)
		_TeamContent = std::make_shared<ClientTeamInfo>();
	return _TeamContent;
}

std::shared_ptr<CrossAreaTeamMember> CGameClient::GetCrossAreaTeamMember()
{
	if (_CorssAreaTeamMemberContent == nullptr)
		_CorssAreaTeamMemberContent = std::make_shared<CrossAreaTeamMember>();

	return _CorssAreaTeamMemberContent;
}
