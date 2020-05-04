#include "stdafx.h"
#include "DailyTask_BossSkill.h"
#include <MyTools/Log.h>
#include <thread>
#include "SomeFun.h"

#define _SELF L"DailyTask_BossSkill.cpp"
CDailyTask_BossSkill::CDailyTask_BossSkill() : _dwMonsterBase(NULL)
{
	_HookContentMonsterSkill.Release();
	_dwMaxSameSkillTimeout = 10 * 1000;
}

static __declspec(naked) VOID WINAPI HookMonsterSkillPtr()
{
	static DWORD dwEBX, dwESI;
	__asm MOV ECX, DWORD PTR DS : [EBX + 技能冷却偏移];;

	__asm
	{
		PUSHAD;
		MOV dwEBX, EBX;
		MOV dwESI, ESI;
		MOV EDX, DWORD PTR SS : [EBP + 0x10];
		MOV EAX, DWORD PTR DS : [ECX];
		MOV EAX, DWORD PTR DS : [EAX + 0x10];
		PUSH EDX;
		PUSH ESI;
		PUSH EDI;
		CALL EAX;
	}
	if (dwEBX != NULL && dwESI != NULL)
		CDailyTask_BossSkill::EchoMonsterSkillId(dwEBX, ReadDWORD(dwESI + 0x10));

	__asm POPAD;
	__asm PUSH HookMonsterAddr + 0x17
	__asm RETN
}

VOID CDailyTask_BossSkill::HookMonsterSkill(_In_ DWORD dwMonsterBase)
{
	Clear();


	_dwMonsterBase = dwMonsterBase;
	_HookContentMonsterSkill.dwFunAddr = (DWORD)HookMonsterSkillPtr;
	_HookContentMonsterSkill.dwHookAddr = HookMonsterAddr;
	_HookContentMonsterSkill.uNopCount = 0x1;
	MyTools::CLHook::Hook_Fun_Jmp_MyAddr(&_HookContentMonsterSkill);
}

VOID CDailyTask_BossSkill::AddBossSkillId(_In_ DWORD dwBossSkillId)
{
	TickBossSkill itm(dwBossSkillId);
	std::lock_guard<std::mutex> _Lock(_MtxQue);

	auto Element = MyTools::CLPublic::Deque_find_if(_QueSkill, [dwBossSkillId](CONST TickBossSkill& itm) { return itm.dwSkillId == dwBossSkillId; });
	if (Element == nullptr)
	{
		_QueSkill.push_back(itm);
		return;
	}
}


VOID CDailyTask_BossSkill::SetMaxSameSkillTimeout(_In_ DWORD dwMaxTimeout)
{
	_dwMaxSameSkillTimeout = dwMaxTimeout;
}

std::pair<bool, CDailyTask_BossSkill::TickBossSkill> CDailyTask_BossSkill::GetBossSkillId()
{
	std::lock_guard<std::mutex> _Lock(_MtxQue);
	static std::map<DWORD, ULONGLONG> MapRecentlySkill;

	while (!_QueSkill.empty())
	{
		auto itm = std::move(_QueSkill.front());
		_QueSkill.erase(_QueSkill.begin());

		// 呆在技能池太久的ID不要了!
		if (::GetTickCount64() - itm.ulTick >= 5 * 1000)
			continue;

		auto itr = _MapRecentlySkill.find(itm.dwSkillId);
		if (itr == _MapRecentlySkill.end())
			_MapRecentlySkill.insert(std::make_pair(itm.dwSkillId, ::GetTickCount64()));
		else
		{
			// 距离上一个Same技能必须超过10s才算有效的技能
			if (::GetTickCount64() - itr->second <= _dwMaxSameSkillTimeout)
				continue;


			// 更新技能时间
			itr->second = ::GetTickCount64();
		}

		return std::make_pair(true, itm);
	}

	static TickBossSkill EmptyTickBossSkill(0);
	return std::make_pair(false, EmptyTickBossSkill);
}

std::pair<bool, CDailyTask_BossSkill::TickBossSkill> CDailyTask_BossSkill::GetUnSameBossSkillId()
{
	std::lock_guard<std::mutex> _Lock(_MtxQue);
	static DWORD dwLastSkillId = 0;


	while (!_QueSkill.empty())
	{
		auto itm = std::move(_QueSkill.front());
		_QueSkill.erase(_QueSkill.begin());

		// 呆在技能池太久的ID不要了!
		if (::GetTickCount64() - itm.ulTick >= 5 * 1000)
			continue;

		// Repeat Skill Id
		if (dwLastSkillId == itm.dwSkillId)
			continue;


		dwLastSkillId = itm.dwSkillId;
		return std::make_pair(true, itm);
	}

	static TickBossSkill EmptyTickBossSkill(0);
	return std::make_pair(false, EmptyTickBossSkill);
}

VOID CDailyTask_BossSkill::Clear()
{
	std::lock_guard<std::mutex> _Lock(_MtxQue);

	if (_HookContentMonsterSkill.dwHookAddr != NULL)
	{
		MyTools::CLHook::UnHook_Fun_Jmp_MyAddr(&_HookContentMonsterSkill);
		_HookContentMonsterSkill.Release();
	}
	

	_QueSkill.clear();
}

VOID CDailyTask_BossSkill::EchoMonsterSkillId(_In_ DWORD dwMonsterBase, _In_ DWORD dwSkillId)
{
	auto& DailyTask_BossSkill = CDailyTask_BossSkill::GetInstance();
	if (dwSkillId < 3000000 || dwSkillId > 6500000 || DailyTask_BossSkill._dwMonsterBase != dwMonsterBase)
		return;

	CDailyTask_BossSkill::GetInstance().AddBossSkillId(dwSkillId);
}

VOID CDailyTask_BossSkill::TickBossSkill::BeforeSleep(_In_ DWORD dwSleepTime) CONST
{
	auto dwTime = GetBeforeSleepTime(dwSleepTime);
	BnsSleep(dwTime);
}

DWORD CDailyTask_BossSkill::TickBossSkill::GetBeforeSleepTime(_In_ DWORD dwSleepTime) CONST
{
	auto dwTime = static_cast<DWORD>(::GetTickCount64() - ulTick);
	return dwTime >= dwSleepTime ? 0 : dwSleepTime - dwTime;
}

DWORD CDailyTask_BossSkill::TickBossSkill::SpendTime() CONST
{
	return static_cast<DWORD>(::GetTickCount64() - ulTick);
}
