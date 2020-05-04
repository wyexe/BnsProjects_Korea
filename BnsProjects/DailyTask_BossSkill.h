#ifndef __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_COMMON_DAILYTASK_BOSSSKILL_H__
#define __GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_COMMON_DAILYTASK_BOSSSKILL_H__

#include <deque>
#include "GameVariable.h"

class CDailyTask_BossSkill : public MyTools::CClassInstance<CDailyTask_BossSkill>
{
public:
	struct TickBossSkill
	{
		DWORD dwSkillId;
		ULONGLONG ulTick;

		TickBossSkill(_In_ DWORD dwId) : dwSkillId(dwId)
		{
			ulTick = ::GetTickCount64();
		}

		VOID BeforeSleep(_In_ DWORD dwSleepTime) CONST;

		DWORD GetBeforeSleepTime(_In_ DWORD dwSleepTime) CONST;

		DWORD SpendTime() CONST;
	};

	struct InvokeMultiBossSkillContent
	{
		DWORD dwSkillId;
		std::function<VOID(VOID)> MethodPtr;
	};

	using MultiBossSkillPtr = std::function<VOID(CONST TickBossSkill&)>;
public:
	CDailyTask_BossSkill();
	~CDailyTask_BossSkill() = default;

	// 
	VOID HookMonsterSkill(_In_ DWORD dwMonsterBase);

	//
	VOID AddBossSkillId(_In_ DWORD dwBossSkillId);

	//
	VOID SetMaxSameSkillTimeout(_In_ DWORD dwMaxTimeout);

	//
	std::pair<bool, TickBossSkill> GetBossSkillId();

	//
	std::pair<bool, TickBossSkill> GetUnSameBossSkillId();

	//
	VOID Clear();

public:
	static VOID EchoMonsterSkillId(_In_ DWORD dwMonsterBase, _In_ DWORD dwSkillId);
private:
	DWORD _dwMonsterBase;
	MyTools::MYHOOK_CONTENT _HookContentMonsterSkill;
	std::deque<TickBossSkill> _QueSkill;
	std::mutex _MtxQue;
	std::map<DWORD, ULONGLONG> _MapRecentlySkill;
	DWORD _dwMaxSameSkillTimeout;
};



#endif // !__GITBNSPROJECTS_BNSPROJECTS_GAME_SCRIPT_BOSS_COMMON_DAILYTASK_BOSSSKILL_H__
