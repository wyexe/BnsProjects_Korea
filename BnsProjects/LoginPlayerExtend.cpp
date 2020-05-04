#include "stdafx.h"
#include "LoginPlayerExtend.h"
#include <MyTools/Log.h>
#include <time.h>
#include "GameUI.h"
#include "ObjectFunction.h"
#include "BnsGameFun.h"
#include "Player.h"
#include "LoginPlayer.h"
#include <MyTools/CLFile.h>
#include "GameSend.h"
#include "BnsConfig.h"
#include "ConsoleLog.h"

#define _SELF L"LoginPlayerExtend.cpp"

BOOL CLoginPlayerExtend::CreatePlayer(_In_ DWORD dwAreaId, _In_ DWORD dwIndex, _In_ em_Player_Classes emPlayerClasses, _In_ BOOL IsQuickLevel) CONST
{
	std::wstring wsPlayerName;
	int nCount = Min_NameLength + 4;
	while (GetLoginPlayerStatus_By_Index(dwIndex) == em_LoginPlayer_UnExist && nCount < 10)
	{
		wsPlayerName.clear();
		GetRandPlayerName(wsPlayerName, nCount++);
		if (emPlayerClasses == em_Player_Classes_KongFuForceMaster)
		{
			LOG_CF_D(L"创建气宗需要使用劵!");
			CLoginPlayerExtend::GetInstance().UseKfmVolume();
			::Sleep(3 * 1000);
		}

		CConsoleLog(MyTools::CCharacter::MakeFormatText(L"正在创建角色:%s,dwAreaId=%d,dwIndex=%d,emPlayerClasses=%d,IsQuickLevel=%d", wsPlayerName.c_str(), dwAreaId, dwIndex, emPlayerClasses, IsQuickLevel));
		CGameVariable::GetInstance().PushMainThreadFunPtr_synch([=]{CGameSend::CreatePlayer(wsPlayerName.c_str(), dwAreaId, dwIndex, emPlayerClasses, IsQuickLevel); });
		::Sleep(3 * 1000);
	}

	if (GetLoginPlayerStatus_By_Index(dwIndex) == em_LoginPlayer_UnExist)
	{
		CGameVariable::GetInstance().GetAccShareInfo()->AccountStatus.uLoginFaildCount += 1;
		CBnsGameFun::GetInstance().SetCloseGame(MyTools::CCharacter::MakeFormatText(L"创建角色失败, 次数=%d!", CGameVariable::GetInstance().GetAccShareInfo()->AccountStatus.uLoginFaildCount), em_Close_Result_ReStart);
		return FALSE;
	}

	return CBnsConfig::GetInstance().SaveLocalTimeConfigToServer(wsPlayerName.c_str(), L"角色创建时间");
}

VOID CLoginPlayerExtend::GetRandPlayerName(_Out_ std::wstring& wsPlayerName, _In_ int nNameLength) CONST
{
	static std::wstring wsText = L"";
#if BnsProject_DLL
	std::wstring wsPath = CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.szConsolePath;
	wsPath += L"\\Setting\\Font.txt";
#else
	std::wstring wsPath = L"C:\\Bns\\Font.txt";
#endif
	if (!MyTools::CLPublic::FileExit(wsPath.c_str()))
	{
		LOG_MSG_CF(L"缺少文件:%s", wsPath.c_str());
		return;
	}
	// 读取韩文文本
	if (wsText.empty() && (!MyTools::CLFile::ReadUnicodeFile(wsPath, wsText) || wsText == L""))
	{
		LOG_MSG_CF(L"文件内容错误:%s", wsPath.c_str());
		return;
	}

	// 前面4个字必须是韩文
	for(int i = 0;i < Min_NameLength; ++i)
	{
		auto wsRandText = wsText.substr(MyTools::CCharacter::GetRand(wsText.length() - 1, 0), 1);
		if (wsRandText == L" ")
			continue;

		wsPlayerName += wsRandText;
	}

	// 判断还需要写入几个字符
	for (int i = 0; i < nNameLength - Min_NameLength; ++i)
	{
		if ((MyTools::CCharacter::GetRand(10, 0) % 2) == 0) // 加韩文
			wsPlayerName += wsText.substr(MyTools::CCharacter::GetRand(wsText.length() - 1, 0), 1);
		else // 加英文
			wsPlayerName.append(1, static_cast<WCHAR>(MyTools::CCharacter::GetRand(L'Z', L'a')));
	}
}

DWORD CLoginPlayerExtend::GetLevel_By_Index(_In_ DWORD dwIndex) CONST
{
	return ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(人物基址) + 人物基址偏移1 - 0x4) + 0x10) + dwIndex * 4) + 0x12E) & 0xFF;
}

DWORD CLoginPlayerExtend::GetLoginPlayerIndex() CONST
{
	DWORD dwIndex = 0;
	CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"SelectCharacterPanel", [&dwIndex](CONST CGameUi& SelectCharacterPanel){dwIndex = ReadDWORD(SelectCharacterPanel.GetObj() + 选择角色偏移) & 0xFF; });
	return dwIndex;
}

BOOL CLoginPlayerExtend::SetLoginPlayerIndex(_In_ UINT uIndex) CONST
{
	return CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"SelectCharacterPanel", [&uIndex](CONST CGameUi& SelectCharacterPanel){*(BYTE*)(SelectCharacterPanel.GetObj() + 选择角色偏移) = (BYTE)uIndex; });
}

BOOL CLoginPlayerExtend::DeletePlayer(_In_ DWORD dwIndex, _In_ DWORD dwSleepTime /*= NULL*/) CONST
{
	return CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"SelectCharacterPanel", [&dwIndex, &dwSleepTime](CONST CGameUi& SelectCharacterPanel){
		DWORD dwIndexParm = ReadDWORD(SelectCharacterPanel.GetObj() + dwIndex * 0x2C + 0x74) & 0xFF;
		if (dwIndexParm == 0xFF)
		{
			LOG_CF_D( L"角色索引:%d 不能被删除!", dwIndex);
			return;
		}
		CConsoleLog(MyTools::CCharacter::MakeFormatText(L"删除第%d个角色!", dwIndex + 1));
		CGameSend::DeletePlayer(dwIndexParm);
		::Sleep(dwSleepTime);
	});
}

em_LoginPlayerStatus CLoginPlayerExtend::GetLoginPlayerStatus_By_Index(_In_ DWORD dwIndex) CONST
{
	std::vector<CLoginPlayer> vlst;
	if (CObjectFunction::GetInstance().GetLoginPlayerList(vlst) <= dwIndex)
		return em_LoginPlayer_UnExist;

	return vlst.at(dwIndex).IsDeleteStatus() ? em_LoginPlayer_Deleted : em_LoginPlayer_Normal;
}

DWORD CLoginPlayerExtend::GetLoginPlayerServerId_By_Index(_In_ DWORD dwIndex) CONST
{
	std::vector<CLoginPlayer> vlst;
	if (CObjectFunction::GetInstance().GetLoginPlayerList(vlst) <= dwIndex)
		return 0;

	return vlst.at(dwIndex).GetServerId();
}

BOOL CLoginPlayerExtend::DeletePlayer_By_Condition(_In_ std::function<BOOL(CONST CLoginPlayer&)> fnCondition) CONST
{
	std::vector<CLoginPlayer> vlst;
	CObjectFunction::GetInstance().GetLoginPlayerList(vlst);

	for (UINT i = 0; i < vlst.size(); ++i)
	{
		auto& itm = vlst.at(i);
		if (!itm.IsDeleteStatus() && fnCondition(itm))
		{
			PlayerclassesText PlayerclassesText_;
			if (CPlayer::FindPlayerclassesText(&PlayerclassesText_, [&itm](CONST PlayerclassesText& PlayerclassesText_){
				return PlayerclassesText_.emPlayerClasses == itm.GetLoginPlayerClasses();
			}))
			{
				if (CGameVariable::GetInstance().ExistSpicalName(itm.GetName()) != CGameVariable::em_SpicalName_Type_None)
				{
					LOG_MSG_CF( L"帐号:%s是特殊号(仓库||邮寄), 不能删!", itm.GetName().c_str());
					continue;
				}
				LOG_CF_D( L"删除角色:%s, 等级:%d, 职业:%s", itm.GetName().c_str(), itm.GetLevel(), PlayerclassesText_.wsText.c_str());
				DeletePlayer(i, 3000);
				continue;
			}
			LOG_MSG_CF( L"握草, 这是什么职业编码:%X", (DWORD)itm.GetLoginPlayerClasses());
		}
	}
	return TRUE;
}

BOOL CLoginPlayerExtend::ExistStandardLoginPlayer(_Out_opt_ CLoginPlayer* pLoginPlayer) CONST
{
	std::vector<CLoginPlayer> vlst;
	CObjectFunction::GetInstance().GetLoginPlayerList(vlst);

	for (CONST auto& itm : vlst)
	{
		if (!itm.IsDeleteStatus())
		{
			MyTools::CLPublic::SetPtr(pLoginPlayer, itm);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CLoginPlayerExtend::ExistLoginPlayer_By_Name(_In_ CONST std::wstring& wsLoginPlayerName, _Out_opt_ CLoginPlayer* pLoginPlayer) CONST
{
	std::vector<CLoginPlayer> vlst;
	CObjectFunction::GetInstance().GetLoginPlayerList(vlst);

	return MyTools::CLPublic::Vec_find_if(vlst, pLoginPlayer, [&wsLoginPlayerName](CONST CLoginPlayer& LoginPlayer_){return LoginPlayer_.GetName() == wsLoginPlayerName; });
}

BOOL CLoginPlayerExtend::IsExistStanadrdLoginPlayerName(_In_ CONST std::wstring& wsLoginPlayerName)
{
	return wsLoginPlayerName[0] == L'W' && wsLoginPlayerName[1] == L's' && wsLoginPlayerName[2] == L'p';
}

BOOL CLoginPlayerExtend::UseKfmVolume() CONST
{
	CConsoleLog(L"使用等级劵!");
	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([]{ CGameSend::UseKfmVolume1(); });
	::Sleep(3 * 1000);
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([]{ CGameSend::UseKfmVolume2(); });
}

BOOL CLoginPlayerExtend::ExistLevelTick() CONST
{
	DWORD dwValue = 0;
	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&dwValue]{ dwValue = CGameSend::GetLevelTickObj(); });
	if (dwValue != NULL)
	{
		LOG_CF_D( L"Exist LevelTick Object!");
		return TRUE;
	}

	return IsUseLevelTick();
}

BOOL CLoginPlayerExtend::IsUseLevelTick() CONST
{
	if (ReadDWORD(ReadDWORD(ReadDWORD(人物基址) + 人物基址偏移1 + 8) + 0x3C) != NULL)
	{
		return TRUE;
	}
	DWORD dwAddr = ReadDWORD(ReadDWORD(人物基址) + 人物基址偏移1 + 8);
	return ReadDWORD(dwAddr + 0x3C) != NULL || ReadDWORD(dwAddr + 获取等级劵数量偏移) != NULL;
}
