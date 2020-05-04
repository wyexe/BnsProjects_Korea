#include "stdafx.h"
#include "InstanceZoneScript.h"
#include <MyTools/Log.h>
#include "PersonFunction.h"
#include "Task.h"
#include "Person.h"
#include "BnsGameFun.h"
#if BnsProject_DLL
#include "TransferScript.h"
#endif

#define _SELF L"InstanceZoneScript.cpp"

BOOL CInstanceZoneScript::Run(_In_ CONST std::wstring& wsScriptFileName, _In_ CONST _em_InstanceZoneScript_Exit_Type emExitType, _In_ CONST std::wstring& wsExitParm)
{
	LOG_CF_D( L"���������ű�:%s", wsScriptFileName.c_str());
	if (!ReadInstanceZoneScript(wsScriptFileName))
		return FALSE;

	if (!InitInstanceZoneScipt())
		return FALSE;

	return Start(emExitType, wsExitParm);
}

BOOL CInstanceZoneScript::ReadInstanceZoneScript(_In_ CONST std::wstring& wsScriptFileName)
{
#if BnsProject_Dlg
	 std::wstring wsPath = L"C:\\Bns\\";
	wsPath += wsScriptFileName;

	if (!MyTools::CLPublic::FileExit(wsPath.c_str()))
	{
		LOG_CF_D( L"��ȡ�ű��ļ�:%s ʧ��!", wsPath.c_str());
		return FALSE;
	}

	return Read(CBnsScript::em_ReadScript_Type_FilePath, wsPath.c_str());
#else
	CTransferScript TransferScript;
	std::wstring wsScriptContent = TransferScript.DownLoadScript(wsScriptFileName);
	return Read(em_ReadScript_Type_Text, wsScriptContent);
#endif
}

BOOL CInstanceZoneScript::InitInstanceZoneScipt()
{
	if (ExistDefMethod(L"End") == nullptr)
	{
		LOG_MSG_CF( L"������End����!");
		return FALSE;
	}

	if (ExistDefMethod(L"Do") == nullptr)
	{
		LOG_MSG_CF( L"������Do����!");
		return FALSE;
	}

	if (ExistDefMethod(L"main") == nullptr)
	{
		LOG_MSG_CF( L"������main����!");
		return FALSE;
	}

	return TRUE;
}

BOOL CInstanceZoneScript::Start(_In_ CONST _em_InstanceZoneScript_Exit_Type emExitType, _In_ CONST std::wstring& wsExitParm)
{
	UINT uCount = 0;
	BOOL bExitInstanceZoneScript = FALSE;

	Excute(L"main");
	while (GameRun && !bExitInstanceZoneScript)
	{
		BOOL bRetCode = Excute(L"Do");
		if (EMPTY_PERSONHP)
		{
			CPersonFunction::GetInstance().Resurrect();
			Excute(L"End");
		}

		switch (emExitType)
		{
		case Script_Exit_Type_Level:
			if (CPerson::GetInstance().GetLevel() >= (DWORD)_wtoi(wsExitParm.c_str()))
			{
				LOG_CF_D( L"����%d, ������ %d��, break;", CPerson::GetInstance().GetLevel(), _wtoi(wsExitParm.c_str()));
				bExitInstanceZoneScript = TRUE;
			}
			break;
		case Script_Exit_Type_Count: // �ɹ�ִ�к���������+1
			uCount += bRetCode ? 1 : 0;
			if (uCount >= (DWORD)_wtoi(wsExitParm.c_str()))
			{
				LOG_CF_D( L"��ǰ����:%d  ������ %d��, break;", uCount, _wtoi(wsExitParm.c_str()));
				bExitInstanceZoneScript = TRUE;
			}
			break;
		case Script_Exit_Type_Task:
		{
									  std::vector<std::wstring> vParm;
									  MyTools::CCharacter::Split(wsExitParm, L",", vParm, Split_Option_RemoveEmptyEntries | Split_Option_KeepOnly);
									  if (vParm.size() != 2)
									  {
										  DWORD dwTaskId = wcstol(vParm.at(0).c_str(), nullptr, 16);
										  DWORD dwScheduleId = wcstol(vParm.at(1).c_str(), nullptr, 16);
										  bExitInstanceZoneScript = CTask::GetInstance().IsCompletionTask(TaskInfoParm(dwTaskId, dwScheduleId));
										  LOG_CF_D( L"�������״̬:%d", bExitInstanceZoneScript);
									  }
									  
		}
			break;
		default:
			break;
		}
	}
	return TRUE;
}

