#include "stdafx.h"
#include "TransferScript.h"
#include <zlib.h>
#include <MyTools/Log.h>
#include "GameVariable.h"
#include "BnsGameFun.h"
#include "SomeFun.h"
#include "ConsoleLog.h"

#pragma comment(lib,"zdll.lib")
#define _SELF L"TransferScript.cpp"
std::wstring CTransferScript::DownLoadScript(_In_ CONST std::wstring& wsScriptName) CONST
{
	std::wstring wsComprScriptName = wsScriptName.find(L".inf") != -1 ? wsScriptName : (wsScriptName + L".inf");


	CSocketAPI::CiphertextScriptInfo CiphertextContent;
	CiphertextContent.wsScriptName = wsComprScriptName;
	

	LOG_CF_D(L"Downloading Script[%s]", wsComprScriptName.c_str());
	std::wstring wsErrText, wsScriptContent;
	while (GameRun)
	{
		if (!_SocketAPI.DownLoadScript(CiphertextContent, wsErrText))
		{
			LOG_CF_E(L"DownLoadScript Faild! wsErrText=%s", wsErrText.c_str());
			CConsoleLog(wsErrText.c_str());
			BnsSleep(10 * 1000);
			continue;
		}
		if (CiphertextContent.wsScriptName != wsComprScriptName)
		{
			LOG_CF_E(L"DownLoadScript Faild! Recv Account[%s] != CurrentAccountName[%s]", CiphertextContent.wsScriptName.c_str(), wsComprScriptName.c_str());
			BnsSleep(10 * 1000);
			continue;
		}
		if (CiphertextContent.wsAccountName != CGameVariable::GetInstance().GetAccShareInfo()->GetAccName())
		{
			LOG_CF_E(L"DownLoadScript Faild! Recv Script[%s] != CurrentScript%s[", CiphertextContent.wsAccountName.c_str(), CGameVariable::GetInstance().GetAccShareInfo()->GetAccName());
			BnsSleep(10 * 1000);
			continue;
		}
		
		if (DecodeScript(CiphertextContent, wsScriptContent))
			break;

		BnsSleep(10 * 1000);
	}

	return wsScriptContent;
}

VOID CTransferScript::QueryDailyTask(_Out_ std::vector<std::wstring>& VecDailyTask) CONST
{
	while (GameRun && !_SocketAPI.GetDailyTaskSchedule(VecDailyTask))
	{
		CConsoleLog(L"查询服务器上的日常任务失败");
		BnsSleep(10 * 1000);
	}
}

BOOL CTransferScript::DecodeScript(_In_ _Out_ CSocketAPI::CiphertextScriptInfo& CiphertextContent, _Out_ std::wstring& wsScriptContent) CONST
{
	// Express Buffer
	std::shared_ptr<WCHAR> ExpressBuffer = MyTools::CLPublic::AllocArraySharePtr<WCHAR>(CiphertextContent.uExpressSize);

	uncompress(reinterpret_cast<Bytef*>(ExpressBuffer.get()), &CiphertextContent.uExpressSize, reinterpret_cast<Bytef*>(CiphertextContent.pScriptContent.get()), CiphertextContent.uCipherTextSize);

	if (MyTools::CLPublic::GetCRC32_DWORD(reinterpret_cast<CONST CHAR*>(ExpressBuffer.get()), CiphertextContent.uExpressSize) != CiphertextContent.dwExpressCRC)
	{
		CConsoleLog(MyTools::CCharacter::MakeFormatText(L"脚本:%s 明文CRC校验错误!", CiphertextContent.wsScriptName.c_str()));
		return FALSE;
	}

	wsScriptContent = ExpressBuffer.get();
	return TRUE;
}

