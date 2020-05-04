#include "stdafx.h"
#include "Skill.h"
#include <MyTools/Log.h>
#include "SomeFun.h"
#include "Person.h"
#include "GameUI.h"
#include "ObjectFunction.h"
#include "GameSend.h"
#include "BnsGameFun.h"
#include "FarmSkill.h"
#include "ConsoleLog.h"

#define _SELF L"Skill.cpp"

CSkill::CSkill(DWORD dwSkillBase)
{
	m_dwSkillBase = dwSkillBase;
	m_dwCooldownParm = m_dwSkillParm = m_dwSkillObj = NULL;
}

CSkill::~CSkill()
{

}

DWORD CSkill::GetBase() const
{
	return m_dwSkillBase;
}


DWORD CSkill::GetId() const
{
	return ReadDWORD(GetBase() + 0xC);
}

CONST std::wstring& CSkill::GetName() CONST
{
	return Name;
}


VOID CSkill::SetName()
{
	WCHAR wszText[MAX_OBJECT_NAME_LEN] = { 0 };
	if (m_dwSkillObj == NULL)
	{
		// 初始化
		m_dwSkillParm = CGameSend::GetSkillParm(GetId());
		m_dwSkillObj = CGameSend::GetSkillObj(GetId(), m_dwSkillParm);
	}
	if (m_dwSkillObj == NULL)
	{
		Name = L"Empty";
		return;
	}

	DWORD dwPtrName = ReadDWORD(CBaseObject::GetObjectNameAddr(ReadDWORD(m_dwSkillObj + 0x18), ReadDWORD(m_dwSkillObj + 0x1C)) + 0x18);
	if (dwPtrName != NULL && ReadDWORD(dwPtrName) != NULL)
	{
		MyTools::CCharacter::wstrcpy_my(wszText, (LPCWSTR)dwPtrName, _countof(wszText) - 1);
		Name = wszText;
	}
}


BOOL CSkill::UseSkill() CONST
{
	return UseSkill_By_Id(GetId());
}

BOOL CSkill::UseSkill_By_Id(_In_ DWORD dwSkillId, _In_ BOOL bNeedResult /* = FALSE */, em_Skill_Type emSkillType /* = em_Skill_Type_Person */)
{
	auto bResult = FALSE;
	for (auto ulTick = ::GetTickCount64(); static_cast<DWORD>(::GetTickCount64() - ulTick) < 1 * 1000 && !bResult;)
	{
		switch (emSkillType)
		{
		case em_Skill_Type_Person:
			CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&dwSkillId, &bResult]{ bResult = CGameSend::UseSkill(dwSkillId); });
			break;
		case em_Skill_Type_Cat:
			if (dwSkillId == Sum_SkillId_Q)
			{
				CSomeFun::GetInstance().SimlationKey('Q', 100);
				bResult = !CFarmSkill::GetInstance().IsCool_By_SkillId_In_HideUi(Sum_SkillId_Q);
			}
			else
				CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&dwSkillId, &bResult]{ bResult = CGameSend::UseSkillByCat(dwSkillId); });

			break;
		default:
			break;
		}
		if (!bNeedResult)
			break;
	}
	return bResult;
}

BOOL CSkill::IsCooldown()
{
	return GetCooldown() == 5 ? TRUE : FALSE;
}

DWORD CSkill::GetCooldownParm()
{
	if (m_dwCooldownParm != NULL)
		return m_dwCooldownParm;

	/*__try
	{
		DWORD dwValue = 0;
		
		if (ReadDWORD(技能冷却参数基址) == 0x0)
			return (DWORD)0;

		_asm
		{
			PUSHAD;
			MOV ECX, 技能冷却参数基址
			MOV ECX, DWORD PTR DS : [ECX];
			MOV ECX, DWORD PTR DS : [ECX];
			MOV EDX, DWORD PTR DS : [ECX];

			MOV EAX, m_dwSkillObj;
			PUSH EAX;

			MOV EAX, DWORD PTR DS : [EDX + 技能冷却参数偏移];
			CALL EAX;
			MOV dwValue, EAX;
			POPAD;
		}
		m_dwCooldownParm = dwValue;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"GetCooldownParm发生了异常");
	}*/
	return m_dwCooldownParm;
}

DWORD CSkill::GetCooldown()
{
	DWORD dwCooldownParm = GetCooldownParm();
	if (dwCooldownParm == NULL)
		return 0;

	DWORD dwValue = 0;
	__try
	{
		_asm
		{
			PUSHAD;
			MOV EAX, dwCooldownParm;
			PUSH EAX;
			;
			MOV EAX, m_dwSkillObj;
			PUSH EAX;
			;
			PUSH 0;
			;
			MOV EAX, 人物基址;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + 人物基址偏移1];
			MOV EAX, DWORD PTR DS : [EAX + 人物基址偏移2];
			MOV ECX, EAX;
			MOV EBX, DWORD PTR DS : [ECX + 技能冷却偏移];
			MOV EAX, 技能冷却CALL;
			CALL EAX;
			MOV dwValue, EAX;
			;
			POPAD;
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"GetCooldown发生了异常,dwSkillObj=%X,dwCooldownParm=%X", m_dwSkillObj, dwCooldownParm);
	}
	return dwValue;
}

BOOL CSkill::StudySkill(_In_ CONST std::vector<LearnSkillContent>& vlst, _In_ CONST DWORD Ntype)
{
	CConsoleLog(L"技能加点ing!!");
	for (CONST auto& LearnSkillContent_ : vlst)
	{
		LOG_CF_D( L"技能加点:%X %X", LearnSkillContent_.dwSkillId, LearnSkillContent_.dwIndex);
		CGameVariable::GetInstance().PushMainThreadFunPtr_synch([LearnSkillContent_]{CGameSend::AddSkillPoint(LearnSkillContent_.dwIndex, LearnSkillContent_.dwSkillId); });
	}

	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([Ntype]{CGameSend::AddSkillPoint_Send(Ntype); });
}

/*
BOOL CSkill::ForgotSkill()
{
	_CLStackTrace(L"CSkill::ForgotSkill");

	/ *while (GameRun && !EMPTY_PERSONHP && !CObjectFunction::GetInstance().IsShow_In_GameUi(L"SkillTrainPanel"))
	{
		LOG_CF_D( L"按K!");
		CSomeFun::GetInstance().SimlationKey('K');
		BnsSleep(1000);
	}
	CBnsGameFun::GetInstance().WriteConsoleLog(L"开始遗忘技能!");
	CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"SkillTrainPanel", [](CONST CGameUi& SkillTrainPanel){
		CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&SkillTrainPanel]{CGameSend::SetForgotSkill(SkillTrainPanel.GetObj()); });
	});

	BnsSleep(3 * 1000);

	CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"SkillTrainInitConfirmPanel", [](CONST CGameUi& SkillTrainInitConfirmPanel){

		CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&SkillTrainInitConfirmPanel]{CGameSend::ConfirmForgotSkill(SkillTrainInitConfirmPanel.GetObj()); });
		BnsSleep(3 * 1000);
		CGameVariable::GetInstance().PushMainThreadFunPtr_synch([]{	CGameSend::AddSkillPoint_Send(); });
	});
	BnsSleep(3 * 1000);
	while (GameRun && !EMPTY_PERSONHP && CObjectFunction::GetInstance().IsShow_In_GameUi(L"SkillTrainPanel"))
	{
		LOG_CF_D( L"按K!");
		CSomeFun::GetInstance().SimlationKey('K');
		BnsSleep(1000);
	}* /
	return BnsSleep(1000);
}*/

/*
DWORD CSkill::GetSkillPoint()
{
	_CLStackTrace(L"CSkill::GetSkillPoint");
	CONST static DWORD dwSkillPointFlag = 0x1;

	DWORD dwSkillPoint = 0;
	/ *CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"SystemMenuPanel", [&dwSkillPoint](CONST CGameUi& SystemMenuPanel){
		DWORD dwCount = (ReadDWORD(SystemMenuPanel.GetObj() + 0x4C + 0x4) - ReadDWORD(SystemMenuPanel.GetObj() + 0x4C)) / 64;
		if (dwCount > 100)
		{
			LOG_MSG_CF( L"获取技能点数失败!!!");
			return;
		}
		DWORD dwAddr = ReadDWORD(SystemMenuPanel.GetObj() + 0x4C);
		for (DWORD i = 0; i < dwCount; ++i)
		{
			if (ReadDWORD(dwAddr + i * 0x64) != dwSkillPointFlag)
				continue;

			dwSkillPoint = ReadDWORD(dwAddr + i * 0x64 + 0x14);
			return;
		}
		LOG_MSG_CF( L"获取技能点数失败!!!");
	});* /

	return dwSkillPoint;
}*/
