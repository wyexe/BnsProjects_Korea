#include "stdafx.h"
#include "GrammarFunction.h"
#include <stdio.h>
#include <MyTools/CLHook.h>
#include <MyTools/CLFile.h>
#include <MyTools/CLThread.h>
#include <MyTools/Log.h>
#include "ScanBase.h"
#include "TaskExtend.h"
#include "PersonFunction.h"
#include "FarmBoss.h"
#include "MonsterExtend.h"
#include "SomeFun.h"
#include "GameFunction.h"
#include "BaseObject.h"
#include "Monster.h"
#include "MonsterExtend.h"
#include "Npc.h"
#include "NpcExtend.h"
#include "ObjectFunction.h"
#include "PersonFunction.h"
#include "Task.h"
#include "Person.h"
#include "Corpse.h"
#include "LoginPlayerExtend.h"
#include "Language.h"
#include "PlayerExtend.h"
#include <MyTools/CLSearchBase.h>
#define _SELF L"GrammarFunction.cpp"

#define Check_Grammar_ParmCount(pfnName, nParmCount, vlst)										\
	if (!MyTools::CLGrammar::Check_ParmCount_By_GrammarList(nParmCount, vlst))							\
	{																							\
		swprintf_s(pwszRetMsg, MAX_RETMSG_SIZE, L"%s ParmCount: %d , Now:%d", pfnName, nParmCount, vlst.size());	\
		return 0;																				\
	}

CGrammarFunction::CGrammarFunction()
{
}

CGrammarFunction::~CGrammarFunction()
{
}

DWORD WINAPI CGrammarFunction::SetSpeed(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg)
{
	Check_Grammar_ParmCount(L"SetSpeed", 1, vlst);

	float fSpeed = MyTools::CLGrammar::Query_FLOATParm_By_GrammarList(1, vlst);
	g_fSpeed = fSpeed;
	swprintf_s(pwszRetMsg, MAX_RETMSG_SIZE, L"速度调整为:%.2f", fSpeed);
	return TRUE;
}

static DWORD gs_dwEnable_MainTask_Send = 0;
__declspec(naked) VOID WINAPI fnHookMianTask()
{
	static DWORD dwArray[8];
	__asm PUSHAD
	LOG_CF_D( L"fnHookMianTask");
	_asm
	{
		LEA ECX, dwArray

		MOV AL, BYTE PTR DS : [ESI + 0x12]	//第一个参数 1
		MOV BYTE PTR DS : [ECX], AL

		MOV AX, WORD PTR DS : [ESI + 0x10]	//第二个参数 DD
		MOV WORD PTR DS : [ECX + 4], AX

		//MOV EAX, DWORD PTR DS : [ESI]
		//MOV DWORD PTR DS : [ECX + 8], EAX		//第三个参数 发包内容(不取)

		MOV AL, BYTE PTR DS : [ESI + 0x13]	//第四个参数 0
		MOV BYTE PTR DS : [ECX + 0xC], AL

		MOV AL, BYTE PTR DS : [ESI + 0x14]	//第五个参数 FF
		MOV BYTE PTR DS : [ECX + 0x10], AL

		MOV EAX, DWORD PTR DS : [ESI + 0x18]
		MOV DWORD PTR DS : [ECX + 0x14], EAX	//第六个参数 1

		MOV EAX, DWORD PTR DS : [ESI + 0x1C]
		MOV DWORD PTR DS : [ECX + 0x18], EAX	//第七个参数 435AB

		MOV AX, WORD PTR DS : [ESI + 0x20]
		MOV WORD PTR DS : [ECX + 0x1C], AX		//第八个参数 0
	}
	CTaskExtend::Print_MianTask_ArrayInfo(dwArray);

	__asm POPAD
	if (gs_dwEnable_MainTask_Send == 0)
	{
		__asm MOV EAX, 发包CALL
		__asm CALL EAX
	}

	__asm PUSH HookTaskCALL_Addr + 0x5
	__asm RETN
}

static DWORD gs_dwEnable_BLTask_Send = 0;
__declspec(naked) VOID WINAPI fnHookBLTask()
{
	static DWORD dwArray[5];

	_asm
	{
			PUSHAD
			LEA EAX, dwArray

			MOV EBX, DWORD PTR DS : [ESI + 0x18]	//第一个参数 1
			MOV DWORD PTR DS : [EAX], EBX

			MOV EBX, DWORD PTR DS : [ESI + 0x1C]	//第二个参数 DD
			MOV DWORD PTR DS : [EAX + 4], EBX

			MOV BX, WORD PTR DS : [ESI + 0x10]
			MOV WORD PTR DS : [EAX + 8], BX		//第三个参数 任务ID

			MOV DI, WORD PTR DS : [ESI + 0x20]
			MOV WORD PTR DS : [EAX + 0xC], DI		//第四个参数


			MOV BL, BYTE PTR DS : [ESI + 0x12]	//第五个参数 FF (+0x12)
			MOV BYTE PTR DS : [EAX + 0x10], BL
	}
	CTaskExtend::Print_BLTask_ArrayInfo(dwArray);

	__asm POPAD
	if (gs_dwEnable_BLTask_Send == 0)
	{
		__asm MOV EAX, 发包CALL
		__asm CALL EAX
	}

	__asm PUSH HookBLTaskCALL_Addr + 0x5
	__asm RETN
}

DWORD WINAPI CGrammarFunction::HookMainTask(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg)
{
	Check_Grammar_ParmCount(L"HookMainTask", 2, vlst);

	DWORD dwMode = MyTools::CLGrammar::Query_DWORDParm_By_GrammarList(1, vlst);
	gs_dwEnable_MainTask_Send = MyTools::CLGrammar::Query_DWORDParm_By_GrammarList(2, vlst);
	static MyTools::MYHOOK_CONTENT HC;

	if (dwMode == 1)
	{
		if (HC.dwFunAddr != NULL)
		{
			swprintf_s(pwszRetMsg, MAX_RETMSG_SIZE, L"别重复Hook, 已经在Hook了!");
			return FALSE;
		}

		HC.dwFunAddr = (DWORD)fnHookMianTask;
		HC.dwHookAddr = HookTaskCALL_Addr;
		HC.uNopCount = 0x0;

		MyTools::CLHook::Hook_Fun_Jmp_MyAddr(&HC);
		swprintf_s(pwszRetMsg, MAX_RETMSG_SIZE, L"Hook主线完毕");
	}
	else if (dwMode == 0)
	{
		if (HC.dwFunAddr != NULL)
			MyTools::CLHook::UnHook_Fun_Jmp_MyAddr(&HC);

		ZeroMemory(&HC, sizeof(HC));
	}
	else
	{
		swprintf_s(pwszRetMsg, MAX_RETMSG_SIZE, L"Parm Err:%d", dwMode);
	}

	return TRUE;
}

DWORD WINAPI CGrammarFunction::HookBLTask(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg)
{
	Check_Grammar_ParmCount(L"HookBLTask", 2, vlst);

	DWORD dwMode = MyTools::CLGrammar::Query_DWORDParm_By_GrammarList(1, vlst);
	gs_dwEnable_BLTask_Send = MyTools::CLGrammar::Query_DWORDParm_By_GrammarList(2, vlst);
	static MyTools::MYHOOK_CONTENT HC;

	if (dwMode == 1)
	{
		if (HC.dwFunAddr != NULL)
		{
			swprintf_s(pwszRetMsg, MAX_RETMSG_SIZE, L"别重复Hook, 已经在Hook了!");
			return FALSE;
		}
		HC.dwFunAddr = (DWORD)fnHookBLTask;
		HC.dwHookAddr = HookBLTaskCALL_Addr;
		HC.uNopCount = 0x0;

		MyTools::CLHook::Hook_Fun_Jmp_MyAddr(&HC);
		swprintf_s(pwszRetMsg, MAX_RETMSG_SIZE, L"Hook支线完毕");
	}
	else if (dwMode == 0)
	{
		if (HC.dwFunAddr != NULL)
			MyTools::CLHook::UnHook_Fun_Jmp_MyAddr(&HC);

		ZeroMemory(&HC, sizeof(HC));
	}
	else
	{
		swprintf_s(pwszRetMsg, MAX_RETMSG_SIZE, L"Parm Err:%d", dwMode);
	}
	return TRUE;
}

DWORD WINAPI CGrammarFunction::GetCityId(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg)
{
	Check_Grammar_ParmCount(L"GetCityId", 0, vlst);

	swprintf_s(pwszRetMsg, MAX_RETMSG_SIZE, L"CityId:0x%X", CPersonFunction::GetInstance().GetCityId());
	return TRUE;
}

DWORD WINAPI CGrammarFunction::TakeItem(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg)
{
	Check_Grammar_ParmCount(L"TakeItem", 2, vlst);

	float fDis = MyTools::CLGrammar::Query_FLOATParm_By_GrammarList(2, vlst);
	LPCWSTR pwszType = MyTools::CLGrammar::Query_LPWSTRParm_By_GrammarList(1, vlst);

	if (MyTools::CCharacter::wstrcmp_my(pwszType,L"Monster"))
	{
		std::vector<CMonster> VecMonster;
		if (CObjectFunction::GetInstance().GetSortGameObjectList(VecMonster, CObjectFunction::em_GameObject_Type_Monster, CBaseObject::CreateFixPointInfo(fDis)) == 0)
		{
			swprintf_s(pwszRetMsg, MAX_RETMSG_SIZE, L"范围:%.2f 没Monster", fDis);
			return TRUE;
		}

		BnsMove(VecMonster.begin()->GetPoint(), 3.0f);

		BnsSleep(1000);
		if (VecMonster.begin()->GetDis() > 3.0f)
		{
			swprintf_s(pwszRetMsg, MAX_RETMSG_SIZE, L"走不到该Monster:%s", VecMonster.begin()->GetName().c_str());
			return TRUE;
		}

		VecMonster.begin()->TakeNpcItem();
		swprintf_s(pwszRetMsg, MAX_RETMSG_SIZE, L"捡起:%s", VecMonster.begin()->GetName().c_str());
	}
	else if (MyTools::CCharacter::wstrcmp_my(pwszType,L"Npc"))
	{
		std::vector<CNpc> VecNpc;
		if (CObjectFunction::GetInstance().GetSortGameObjectList(VecNpc, CObjectFunction::em_GameObject_Type_Npc, CBaseObject::CreateFixPointInfo(fDis)) == 0)
		{
			swprintf_s(pwszRetMsg, MAX_RETMSG_SIZE, L"范围:%.2f 没Npc", fDis);
			return TRUE;
		}

		BnsMove(VecNpc.begin()->GetPoint(), 3.0f);
		
		BnsSleep(1000);
		if (VecNpc.begin()->GetDis() > 3.0f)
		{
			swprintf_s(pwszRetMsg, MAX_RETMSG_SIZE, L"走不到该Npc:%s", VecNpc.begin()->GetName().c_str());
			return TRUE;
		}

		VecNpc.begin()->TakeNpcItem();
		swprintf_s(pwszRetMsg, MAX_RETMSG_SIZE, L"捡起:%s", VecNpc.begin()->GetName().c_str());
	}
	else if (MyTools::CCharacter::wstrcmp_my(pwszType, L"Corpse"))
	{
		std::vector<CCorpse> VecCorpse;
		if (CObjectFunction::GetInstance().GetSortGameObjectList(VecCorpse, CObjectFunction::em_GameObject_Type_Corpse, CBaseObject::CreateFixPointInfo(fDis)) == 0)
		{
			swprintf_s(pwszRetMsg, MAX_RETMSG_SIZE, L"范围:%.2f 没尸体", fDis);
			return TRUE;
		}

		BnsMove(VecCorpse.begin()->GetPoint(), 3.0f);

		BnsSleep(1000);
		if (VecCorpse.begin()->GetDis() > 3.0f)
		{
			swprintf_s(pwszRetMsg, MAX_RETMSG_SIZE, L"走不到该尸体");
			return TRUE;
		}

		VecCorpse.begin()->TakeItem();
		BnsSleep(1000);

		CPersonFunction::GetInstance().WaitForProgressBar();
		swprintf_s(pwszRetMsg, MAX_RETMSG_SIZE, L"捡起尸体");
	}
	else
	{
		swprintf_s(pwszRetMsg, MAX_RETMSG_SIZE, L"Type Err: %s", pwszType);
	}

	return TRUE;
}

DWORD WINAPI CGrammarFunction::SumitMainTask(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg)
{
	Check_Grammar_ParmCount(L"SumitMainTask", 8, vlst);

	DWORD dwArray[8] = { 0 };
	for (int i = 0; i < 8; ++i)
		dwArray[i] = MyTools::CLGrammar::Query_DWORDParm_By_GrammarList(i + 1, vlst);

	SumitTask_Src STS ;
	STS.dwTaskId     = dwArray[1];
	STS.dwObjId      = dwArray[5];
	STS.dwObjIdParm  = dwArray[6];
	STS.dwItemIndex  = dwArray[7];
	STS.dwScheduleId = dwArray[0];
	STS.dwParm3      = dwArray[2];
	STS.dwParm4      = dwArray[3];
	STS.dwParm5      = dwArray[4];

	CTask::SumitTask(STS);
	swprintf_s(pwszRetMsg, MAX_RETMSG_SIZE, L"已交!");
	return TRUE;
}

DWORD WINAPI CGrammarFunction::PickBLTask(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg)
{
	Check_Grammar_ParmCount(L"PickBLTask", 5, vlst);

	DWORD dwArray[5] = { 0 };
	for (int i = 0; i < 5; ++i)
		dwArray[i] = MyTools::CLGrammar::Query_DWORDParm_By_GrammarList(i + 1, vlst);

	SumitBLTask_Src STS;
	STS.dwNpcId     = dwArray[0];
	STS.dwNpcIdParm = dwArray[1];
	STS.dwTaskId    = dwArray[2];
	STS.dwParm4     = dwArray[3];
	STS.dwParm5     = dwArray[4];

	CTask::PickLBTask(STS);
	swprintf_s(pwszRetMsg, MAX_RETMSG_SIZE, L"已接!");
	return TRUE;
}
static DWORD dwHookEquiEvolutionAddr = 0;
static DWORD dwEvolutionId = 0;
__declspec(naked) VOID WINAPI fnHookEquiEvolution()
{
	__asm MOV EAX, DWORD PTR DS : [ESP + 0x4];
	__asm MOV dwEvolutionId, EAX;
	__asm PUSHAD
	LOG_CF_D( L"dwEvolutionId=%X", dwEvolutionId);
	__asm POPAD
	__asm ADD ESP, 0x24;
	__asm MOV EAX, dwHookEquiEvolutionAddr;
	__asm ADD EAX, 5;
	__asm PUSH EAX;
	__asm RETN
}

DWORD WINAPI CGrammarFunction::HookEquiEvolution(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg)
{
	Check_Grammar_ParmCount(L"HookEquiEvolution", 1, vlst);

	DWORD dwMode = MyTools::CLGrammar::Query_DWORDParm_By_GrammarList(1, vlst);
	static MyTools::MYHOOK_CONTENT HC;

	if (dwMode == 1)
	{
		if (HC.dwFunAddr != NULL)
		{
			swprintf_s(pwszRetMsg, MAX_RETMSG_SIZE, L"别重复Hook, 已经在Hook了!");
			return FALSE;
		}
		dwHookEquiEvolutionAddr = MyTools::CLSearchBase::FindAddr("83c410c2040083??03", 0x7DADFF - 0x7DAE6D, 0, L"Client.exe");
		if (dwHookEquiEvolutionAddr == NULL || ReadBYTE(dwHookEquiEvolutionAddr) != 0xE8)
		{
			swprintf_s(pwszRetMsg, MAX_RETMSG_SIZE, L"dwHookEquiEvolutionAddr = 0");
			return FALSE;
		}
		HC.dwFunAddr = (DWORD)fnHookEquiEvolution;
		HC.dwHookAddr = dwHookEquiEvolutionAddr;
		HC.uNopCount = 0x0;

		MyTools::CLHook::Hook_Fun_Jmp_MyAddr(&HC);
		swprintf_s(pwszRetMsg, MAX_RETMSG_SIZE, L"Hook装备进化完毕");
	}
	else if (dwMode == 0)
	{
		if (HC.dwFunAddr != NULL)
			MyTools::CLHook::UnHook_Fun_Jmp_MyAddr(&HC);

		ZeroMemory(&HC, sizeof(HC));
	}
	else
	{
		swprintf_s(pwszRetMsg, MAX_RETMSG_SIZE, L"Parm Err:%d", dwMode);
	}
	return TRUE;
}

DWORD WINAPI CGrammarFunction::SearchMachineCode(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg)
{
	Check_Grammar_ParmCount(L"SearchMachineCode", 0, vlst);

	return 0;
}

DWORD WINAPI CGrammarFunction::SetAccountName(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg)
{
	Check_Grammar_ParmCount(L"SetAccountName", 1, vlst);
	return TRUE;
}

DWORD WINAPI CGrammarFunction::SearchMemBase(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg)
{
	Check_Grammar_ParmCount(L"SearchMemBase", 0, vlst);

	return CScanBase::Search();
}

DWORD dwTelportCall = NULL;
PDWORD pdwTelportMem = NULL;
VOID WINAPI SaveTelportFile()
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, L"C:\\1.BnsPath", L"ab+");
	if (pFile == nullptr)
	{
		LOG_CF_D( L"ReadScriptFile Fiald! Path:C:\\1.BnsPath");
		return;
	}

	fseek(pFile, 0, SEEK_END);
	fwrite(pdwTelportMem, sizeof(DWORD), MAX_TELPORT_SIZE, pFile);
	fclose(pFile);
}

__declspec(naked) VOID WINAPI fnHookTelport()
{
	static DWORD dwEbp = NULL;
	__asm MOV dwEbp, EBP;
	__asm PUSHAD;
	pdwTelportMem = new DWORD[MAX_TELPORT_SIZE];
	memcpy(pdwTelportMem, (void*)dwEbp, sizeof(DWORD) * MAX_TELPORT_SIZE);
	SaveTelportFile();
	__asm POPAD
	__asm MOV EAX, dwTelportCall
	__asm CALL EAX
	__asm PUSH Hook瞬移地址 + 0x5
	__asm RETN
}


DWORD WINAPI CGrammarFunction::HookTelport(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg)
{
	Check_Grammar_ParmCount(L"HookTelport", 1, vlst);
	DWORD dwMode = MyTools::CLGrammar::Query_DWORDParm_By_GrammarList(1, vlst);
	static MyTools::MYHOOK_CONTENT HC;

	if (dwTelportCall == NULL)
		dwTelportCall = MyTools::CLHook::GetCall(Hook瞬移地址, 0x400000, L"Client.exe");

	if (dwMode == 1)
	{
		if (HC.dwFunAddr != NULL)
		{
			swprintf_s(pwszRetMsg, MAX_RETMSG_SIZE, L"别重复Hook, 已经在Hook了!");
			return FALSE;
		}
		HC.dwFunAddr = (DWORD)fnHookTelport;
		HC.dwHookAddr = Hook瞬移地址;
		HC.uNopCount = 0x0;

		MyTools::CLHook::Hook_Fun_Jmp_MyAddr(&HC);
		swprintf_s(pwszRetMsg, MAX_RETMSG_SIZE, L"Hook瞬移完毕");
	}
	else if (dwMode == 0)
	{
		if (HC.dwFunAddr != NULL)
			MyTools::CLHook::UnHook_Fun_Jmp_MyAddr(&HC);

		ZeroMemory(&HC, sizeof(HC));
	}
	else
	{
		swprintf_s(pwszRetMsg, MAX_RETMSG_SIZE, L"Parm Err:%d", dwMode);
	}
	return TRUE;
}

std::shared_ptr<CHAR> GetTelprotFile(_In_ CONST std::wstring& wsPath, _In_ CONST ULONG& ulFileLen)
{
	FILE* pFile = nullptr;
	fopen_s(&pFile, MyTools::CCharacter::UnicodeToASCII(wsPath).c_str(), "rb");
	if (pFile == nullptr)
	{
		LOG_MSG_CF( L"ReadTelportFile Fiald! Path:%s", wsPath.c_str());
		return nullptr;
	}


	std::shared_ptr<CHAR> psContent(new CHAR[ulFileLen], [](CHAR* p){delete[] p; });
	fseek(pFile, 0, SEEK_SET);
	fread(psContent.get(), sizeof(CHAR), (size_t)ulFileLen, pFile);
	fclose(pFile);
	return psContent;
}
DWORD WINAPI CGrammarFunction::Telport(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg)
{
	ULONG ulFileLen = 0;
	if (!MyTools::CLFile::ReadAsciiFileLen(L"C:\\1.BnsPath", ulFileLen))
	{
		LOG_MSG_CF( L"1.BnsPath FileLen Invaild!");
		return 0;
	}
	if (ulFileLen % MAX_TELPORT_SIZE != 0)
	{
		LOG_MSG_CF( L"1.BnsPath FileLen=%d, Invaild!", (UINT)ulFileLen);
		return 0;
	}

	std::shared_ptr<CHAR> psBuffer = GetTelprotFile(L"C:\\1.BnsPath", ulFileLen);
	if (psBuffer == nullptr)
		return 0;

	auto fnGetPointValue = [&psBuffer](_In_ int i, _In_ int nOffset)
	{
		DWORD dwAddr = ((DWORD*)psBuffer.get())[i * MAX_TELPORT_SIZE + nOffset];
		return ReadFloat((DWORD)&dwAddr);
	};

	std::vector<Point> vPoint;
	for (UINT i = 0; i < ulFileLen / (MAX_TELPORT_SIZE * 4); ++i)
		vPoint.push_back(Point(fnGetPointValue(i, 1), fnGetPointValue(i, 2), fnGetPointValue(i, 3)));

	for (auto& itm : vPoint)
	{
		LOG_CF_D(L"Point:%d,%d,%d", itm.GetX(),itm.GetY(), itm.GetZ());
	}

	int nIndex = CGameFunction::GetInstance().GetRecentlyPoint(vPoint, CBaseObject::GetPersonPoint());
	if (nIndex == -1)
	{
		LOG_MSG_CF( L"竟然离最近的点超过30m!");
		return 0;
	}

	BnsMove(vPoint.at(nIndex), 2.0f);
	::Sleep(3000);

	if (dwTelportCall == NULL)
		dwTelportCall = MyTools::CLHook::GetCall(Hook瞬移地址, 0x400000, L"Client.exe");

	DWORD dwPersonBase = CPerson::GetInstance().GetBase();
	for (int i = nIndex; i < (int)vPoint.size(); ++i)
	{
		DWORD dwArray[MAX_TELPORT_SIZE] = { 0 };
		memcpy(dwArray, ((DWORD*)psBuffer.get()) + i * MAX_TELPORT_SIZE, sizeof(DWORD) * MAX_TELPORT_SIZE);

		__asm
		{
			PUSHAD;
			PUSH 0;
			PUSH 0;
			MOV EAX, dwPersonBase;
			MOV EAX, DWORD PTR DS : [EAX + 0x14];
			ADD EAX, 人物瞬移偏移2;
			MOV ECX, DWORD PTR DS : [EAX + 人物瞬移偏移];
			MOV WORD PTR DS : [EAX + 人物瞬移偏移], CX;
			LEA EAX, dwArray;
			//PUSH EAX;
			MOV EDI, EAX;
			MOV EAX, dwTelportCall;
			CALL EAX;
			POPAD;
		}
		
		::Sleep(10);
	}
	MessageBoxW(NULL, L"End", L"", NULL);
	return 0;
}

BOOL bRunMonsterSkillIdThread = FALSE;
unsigned WINAPI _ShowMonsterSkillIdThread(LPVOID lpParm)
{
	WCHAR wszName[MAX_PATH];
	CFarmBoss::MonsterSkillContent MonsterSkillContent_;
	while (bRunMonsterSkillIdThread)
	{
		if(CFarmBoss::GetInstance().GetBossSkillByQue(MonsterSkillContent_))
		{
			if (ReadDWORD(MonsterSkillContent_.dwResAddr + 0x8) == MonsterSkillContent_.dwSkillId)
			{
				DWORD dwResId = ReadDWORD(MonsterSkillContent_.dwResAddr + 0x18);
				DWORD dwNamePtr = CBaseObject::GetObjectNameAddr(dwResId, 0);
				if (dwNamePtr == NULL)
				{
					LOG_CF_D( L"SkillID=[%d,%X], ResID=%X, [Name=Empty]", MonsterSkillContent_.dwSkillId, MonsterSkillContent_.dwSkillId, dwResId);
				}
				else
				{
					MyTools::CCharacter::wstrcpy_my(wszName, (WCHAR*)(ReadDWORD(dwNamePtr + 0x18)), MAX_PATH);
					std::wstring wsText = CLanguage::GetInstance().GetOtherText_By_LocalText(wszName);
					LOG_CF_D( L"SkillID=[%d,%X], ResID=%X, Name=[%s,%s]", MonsterSkillContent_.dwSkillId, MonsterSkillContent_.dwSkillId, dwResId, wszName, wsText.c_str());
				}
			}
			else
			{
				LOG_CF_D(L"EmptyName, SkillID=%d, ResAddr=%X", MonsterSkillContent_.dwSkillId, MonsterSkillContent_.dwResAddr);
			}
			
			continue;
		}
		::Sleep(10);
	}
	return 0;
}

DWORD WINAPI CGrammarFunction::HookMonsterSkill(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg)
{
	static BOOL bHook = FALSE;
	static HANDLE hThread = NULL;

	DWORD dwMode = MyTools::CLGrammar::Query_DWORDParm_By_GrammarList(1, vlst);
	if (dwMode == 0) // 关闭Hook 
	{
		if (!bHook)
			MyTools::CCharacter::wstrcpy_my(pwszRetMsg, L"已经关过了!");
		else
		{
			bRunMonsterSkillIdThread = FALSE;
			MyTools::CLThread::TryTriminateThread(hThread);
			CFarmBoss::GetInstance().IsHookByDlg() = FALSE;
			CGameFunction::GetInstance().HookMonsterSkill(FALSE, NULL);
			MyTools::CCharacter::wstrcpy_my(pwszRetMsg, L"关闭怪物技能Hook!");
		}
		bHook = FALSE;
		return 0;
	}
	
	if (bHook)
	{
		MyTools::CCharacter::wstrcpy_my(pwszRetMsg, L"已经Hook了, 不要重复Hook!");
		return 0;
	}
	
	if (MyTools::CLGrammar::Query_LPWSTRParm_By_GrammarList(2, vlst) == nullptr || MyTools::CLGrammar::Query_LPWSTRParm_By_GrammarList(3, vlst) == nullptr)
	{
		MyTools::CCharacter::wstrcpy_my(pwszRetMsg, L"第2和3个参数呢?");
		return 0;
	}
	
	LPCWSTR pwszFilterType = MyTools::CLGrammar::Query_LPWSTRParm_By_GrammarList(2, vlst);
	LPCWSTR pwszFilterText = MyTools::CLGrammar::Query_LPWSTRParm_By_GrammarList(3, vlst);

	CMonster Monster(NULL);
	if (MyTools::CCharacter::wstrcmp_my(pwszFilterType, L"ID"))
	{
		DWORD dwId = wcstol(pwszFilterText, nullptr, 16);
		if (CMonsterExtend::GetInstance().FindMonster_By_Id(dwId, CBaseObject::CreateFixPointInfo(100), &Monster))
		{
			bRunMonsterSkillIdThread = bHook = TRUE;
			CFarmBoss::GetInstance().IsHookByDlg() = TRUE;
			CGameFunction::GetInstance().HookMonsterSkill(bHook, Monster.GetBase());
			cbBEGINTHREADEX(NULL, NULL, _ShowMonsterSkillIdThread, NULL, NULL, NULL);
			MyTools::CCharacter::wstrcpy_my(pwszRetMsg, L"Hook怪物技能完毕!");
			return 0;
		}

		CNpc Npc;
		if (CNpcExtend::GetInstance().FindNpc_By_Id(dwId, CBaseObject::CreateFixPointInfo(100), &Npc))
		{
			bRunMonsterSkillIdThread = bHook = TRUE;
			CFarmBoss::GetInstance().IsHookByDlg() = TRUE;
			CGameFunction::GetInstance().HookMonsterSkill(bHook, Npc.GetBase());
			cbBEGINTHREADEX(NULL, NULL, _ShowMonsterSkillIdThread, NULL, NULL, NULL);
			MyTools::CCharacter::wstrcpy_my(pwszRetMsg, L"Hook怪物技能完毕!");
			return 0;
		}

		CPlayer Player;
		if (CPlayerExtend::GetInstance().FindPlayer_By_Id(dwId, CBaseObject::CreateFixPointInfo(10), &Player))
		{
			bRunMonsterSkillIdThread = bHook = TRUE;
			CFarmBoss::GetInstance().IsHookByDlg() = TRUE;
			CGameFunction::GetInstance().HookMonsterSkill(bHook, Player.GetBase());
			cbBEGINTHREADEX(NULL, NULL, _ShowMonsterSkillIdThread, NULL, NULL, NULL);
			MyTools::CCharacter::wstrcpy_my(pwszRetMsg, L"Hook怪物技能完毕!");
			return 0;
		}
		
		MyTools::CCharacter::wstrcpy_my(pwszRetMsg, L"附近找不到这个ID的怪物||Npc!");
		return 0;
	}
	else if (MyTools::CCharacter::wstrcmp_my(pwszFilterType, L"Name"))
	{
		if (CMonsterExtend::GetInstance().FindMonster_By_Name(pwszFilterText, CBaseObject::CreateFixPointInfo(100), &Monster))
		{
			bRunMonsterSkillIdThread = bHook = TRUE;
			CFarmBoss::GetInstance().IsHookByDlg() = TRUE;
			CGameFunction::GetInstance().HookMonsterSkill(bHook, Monster.GetBase());
			cbBEGINTHREADEX(NULL, NULL, _ShowMonsterSkillIdThread, NULL, NULL, NULL);
			MyTools::CCharacter::wstrcpy_my(pwszRetMsg, L"Hook怪物技能完毕!");
			return 0;
		}

		CNpc Npc;
		if (CNpcExtend::GetInstance().FindNpc_By_Name(pwszFilterText, CBaseObject::CreateFixPointInfo(100), &Npc))
		{
			bRunMonsterSkillIdThread = bHook = TRUE;
			CFarmBoss::GetInstance().IsHookByDlg() = TRUE;
			CGameFunction::GetInstance().HookMonsterSkill(bHook, Npc.GetBase());
			cbBEGINTHREADEX(NULL, NULL, _ShowMonsterSkillIdThread, NULL, NULL, NULL);
			MyTools::CCharacter::wstrcpy_my(pwszRetMsg, L"Hook怪物技能完毕!");
			return 0;
		}
	}
	else
	{
		MyTools::CCharacter::wstrcpy_my(pwszRetMsg, L"这个是什么鬼的分类?");
		return 0;
	}

	return 0;
}

DWORD WINAPI CGrammarFunction::DeletePlayer(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg)
{
	DWORD dwIndex = MyTools::CLGrammar::Query_DWORDParm_By_GrammarList(1, vlst);
	CLoginPlayerExtend::GetInstance().DeletePlayer(dwIndex);
	MyTools::CCharacter::wstrcpy_my(pwszRetMsg, L"删除完毕!");
	return 0;

}

static DWORD gs_dwAddSkillId = 0;
__declspec(naked) VOID WINAPI fnHookAddSkill()
{
	__asm MOV gs_dwAddSkillId, EDX;
	__asm PUSHAD
	LOG_CF_D( L"技能加点:%X", gs_dwAddSkillId);
	__asm
	{
		POPAD;
		MOV EAX, 人物基址;
		MOV EAX, DWORD PTR DS : [EAX];
		PUSH (技能加点CALL + 0x1 + 0x5);
		RETN;
	}
}

DWORD WINAPI CGrammarFunction::HookAddSkill(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg)
{
	static BOOL bHook = FALSE;
	static MyTools::MYHOOK_CONTENT HC;
	DWORD dwMode = MyTools::CLGrammar::Query_DWORDParm_By_GrammarList(1, vlst);

	if (dwMode == 0) // Close Hook
	{
		if (bHook)
		{
			MyTools::CLHook::UnHook_Fun_Jmp_MyAddr(&HC);
			bHook = FALSE;
		}
		MyTools::CCharacter::wstrcpy_my(pwszRetMsg, L"关闭Hook完毕!");
		return 0;
	}

	if (bHook)
	{
		MyTools::CCharacter::wstrcpy_my(pwszRetMsg, L"Hook已经开了好嘛!");
		return 0;
	}

	HC.dwFunAddr = (DWORD)fnHookAddSkill;
	HC.dwHookAddr = 技能加点CALL + 1;
	HC.uNopCount = 0;
	MyTools::CLHook::Hook_Fun_Jmp_MyAddr(&HC);

	bHook = TRUE;
	MyTools::CCharacter::wstrcpy_my(pwszRetMsg, L"Hook完毕!");
	return 0;
}

DWORD WINAPI CGrammarFunction::CreatePlayer(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg)
{
	DWORD dwArea = MyTools::CLGrammar::Query_DWORDParm_By_GrammarList(1, vlst);

	CLoginPlayerExtend::GetInstance().CreatePlayer(dwArea, CObjectFunction::GetInstance().GetLoginPlayerCount(), em_Player_Classes_ForceMaster);
	return 0;
}

DWORD WINAPI CGrammarFunction::ShowVariable(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg)
{
	CONST std::wstring& wsVariableName = MyTools::CLGrammar::Query_LPWSTRParm_By_GrammarList(1, vlst);
	swprintf_s(pwszRetMsg, 1024 - 1, L"%s=%X", wsVariableName.c_str(), CGameVariable::GetInstance().GetVariable_By_Name(wsVariableName));
	LOG_CF_D( L"%s", pwszRetMsg);
	return 0;
}

DWORD dwHookShopStoreAddr = 0;
__declspec(naked) VOID WINAPI fnHookShopStore()
{
	static DWORD dwID = 0;
	__asm MOV EAX, DWORD PTR DS : [ESP + 0x4];
	__asm MOV EAX, DWORD PTR DS : [EAX];
	__asm MOV dwID, EAX;
	__asm PUSHAD;

	LOG_CF_D( L"%X", dwID);
	__asm POPAD;
	//__asm MOV EAX, 星币商店购物CALL;
	//__asm CALL EAX;
	__asm ADD ESP, 0x8;
	__asm PUSH dwHookShopStoreAddr;
	__asm RETN;
}

DWORD WINAPI CGrammarFunction::HookShopStore(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg)
{
	static BOOL bHook = FALSE;
	static MyTools::MYHOOK_CONTENT HC;
	DWORD dwMode = MyTools::CLGrammar::Query_DWORDParm_By_GrammarList(1, vlst);

	if (dwMode == 0) // Close Hook
	{
		if (bHook)
		{
			MyTools::CLHook::UnHook_Fun_Jmp_MyAddr(&HC);
			bHook = FALSE;
		}
		MyTools::CCharacter::wstrcpy_my(pwszRetMsg, L"关闭Hook完毕!");
		return 0;
	}

	if (bHook)
	{
		MyTools::CCharacter::wstrcpy_my(pwszRetMsg, L"Hook已经开了好嘛!");
		return 0;
	}

	DWORD dwHookAddr = MyTools::CLSearchBase::FindAddr("C38B??1039??????????75??????E8??????????????8B", 0xC54F99 - 0x00C54FA7, 0x0, L"Client.exe");
	dwHookShopStoreAddr = dwHookAddr + 0x5;

	HC.dwFunAddr = (DWORD)fnHookShopStore;
	HC.dwHookAddr = dwHookAddr;
	HC.uNopCount = 0;
	MyTools::CLHook::Hook_Fun_Jmp_MyAddr(&HC);

	bHook = TRUE;
	MyTools::CCharacter::wstrcpy_my(pwszRetMsg, L"Hook完毕!");
	return 0;
}
