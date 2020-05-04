#include "stdafx.h"
#include "BnsHook.h"
#include "TaskExtend.h"
#include "Person.h"
#include "SomeFun.h"
#include <MyTools/Log.h>
#define _SELF L"BnsHook.cpp"

BOOL CBnsHook::SetHookTakeItemMode(_In_ BOOL bHook)
{
	static DWORD dwTakeItemParm1 = 0x0;
	static DWORD dwTakeItemParm2 = 0x0;
	static DWORD dwTakeBlueItemParm1 = 0x0;
	static DWORD dwTakeBlueItemParm2 = 0x0;
	DWORD dwProtect = NULL;

	if (bHook)
	{
		// 修改内存页属性
		::VirtualProtect((LPVOID)HookTakeItemJudge, 8, PAGE_EXECUTE_READWRITE, &dwProtect);
		::VirtualProtect((LPVOID)HookTakeItem_ForBlueAddr1, 8, PAGE_EXECUTE_READWRITE, &dwProtect);
		::VirtualProtect((LPVOID)HookTakeItem_ForBlueAddr2, 1, PAGE_EXECUTE_READWRITE, &dwProtect);

		// 保存现场
		dwTakeItemParm1 = ReadDWORD(HookTakeItemJudge + 0x0);
		dwTakeItemParm2 = ReadDWORD(HookTakeItemJudge + 0x4);
		dwTakeBlueItemParm1 = ReadDWORD(HookTakeItem_ForBlueAddr1 + 0x0);
		dwTakeBlueItemParm2 = ReadDWORD(HookTakeItem_ForBlueAddr1 + 0x4);

		// inline hook
		const DWORD dwCode1 = 0x000008BA;
		const DWORD dwCode2 = 0xFF909000;
		MyTools::CCharacter::WriteDWORD(HookTakeItemJudge + 0x0, dwCode1);
		MyTools::CCharacter::WriteDWORD(HookTakeItemJudge + 0x4, dwCode2);

		const DWORD dwCode3 = 0x00000B8;
		const DWORD dwCode4 = 0xFF909000;
		MyTools::CCharacter::WriteDWORD(HookTakeItem_ForBlueAddr1 + 0x0, dwCode3);
		MyTools::CCharacter::WriteDWORD(HookTakeItem_ForBlueAddr1 + 0x4, dwCode4);

		const BYTE bCode = 0xE8;
		MyTools::CCharacter::WriteBYTE(HookTakeItem_ForBlueAddr2, bCode);
	}
	else
	{
		if (dwTakeItemParm1 != NULL)
		{
			MyTools::CCharacter::WriteDWORD(HookTakeItemJudge + 0x0, dwTakeItemParm1);
			MyTools::CCharacter::WriteDWORD(HookTakeItemJudge + 0x4, dwTakeItemParm2);
			dwTakeItemParm1 = dwTakeItemParm2 = NULL;
		}
		if (dwTakeBlueItemParm1 != NULL)
		{
			MyTools::CCharacter::WriteDWORD(HookTakeItem_ForBlueAddr1 + 0x0, dwTakeBlueItemParm1);
			MyTools::CCharacter::WriteDWORD(HookTakeItem_ForBlueAddr1 + 0x4, dwTakeBlueItemParm2);
			dwTakeBlueItemParm1 = dwTakeBlueItemParm2 = NULL;
		}

		const BYTE bCode = 0x74;
		MyTools::CCharacter::WriteBYTE(HookTakeItem_ForBlueAddr2, bCode);
	}
	return TRUE;
}

__declspec(naked) VOID WINAPI HookDungoen()
{
	__asm
	{
		ADD ESP, 0xC;
		PUSH Hook副本不过图Addr + 5;
		RETN;
	}
}

BOOL CBnsHook::SetHookDungoenMode(_In_ BOOL bHook)
{
	static MyTools::MYHOOK_CONTENT HookContent;
	if (bHook)
	{
		HookContent.dwHookAddr = Hook副本不过图Addr;
		HookContent.dwFunAddr = reinterpret_cast<DWORD>(HookDungoen);
		HookContent.uNopCount = 0;
		MyTools::CLHook::Hook_Fun_Jmp_MyAddr(&HookContent);
	}
	else
	{
		if (HookContent.dwFunAddr != NULL)
		{
			MyTools::CLHook::UnHook_Fun_Jmp_MyAddr(&HookContent);
			HookContent.Release();
		}
	}
	return TRUE;
}

static DWORD dwBnsHookHeightAddr = NULL;
__declspec(naked) VOID WINAPI fnBnsHookHeight()
{
	static DWORD dwESP = NULL;
	static DWORD dwHookHeightEBX = NULL;

	__asm MOV dwHookHeightEBX, EBX;
	__asm MOV dwESP, ESP;
	__asm PUSHAD;
	__asm PUSHFD;

	static DWORD dwPersonAddr = NULL;
	dwPersonAddr = GetPersonPointAddr - 0x54;
	if (dwPersonAddr != dwHookHeightEBX)
	{
		__asm MOV EBX, dwHookHeightEBX;
		__asm MOVSS XMM0, DWORD PTR DS : [EBX + 0x5C];
		__asm MOV EBX, dwESP;
		__asm ADDSS XMM0, DWORD PTR DS : [EBX + 0x68];
		__asm MOV EBX, dwHookHeightEBX;
		__asm MOVSS DWORD PTR DS : [EBX + 0x5C], XMM0;
	}

	__asm POPFD;
	__asm POPAD;
	__asm MOV ECX, dwBnsHookHeightAddr;
	__asm ADD ECX, 0x5;
	__asm PUSH ECX;
	__asm RETN;
}

BOOL CBnsHook::HookHeight(_In_ BOOL bHook, _In_ float fHeight) CONST
{
	static MyTools::MYHOOK_CONTENT HookContent;
	if (bHook)
	{
		// Repeat Hook
		if (HookContent.dwHookAddr != NULL)
			return TRUE;

		// Hook 跳
		DWORD dwJmpHookAddr = CGameVariable::GetInstance().GetHookJmpAddr();
		DWORD dwOldValue1 = ReadDWORD(dwJmpHookAddr + 0x0);
		DWORD dwOldValue2 = ReadDWORD(dwJmpHookAddr + 0x4);
		MyTools::CCharacter::WriteDWORD(dwJmpHookAddr + 0x0, 0x90909090);
		MyTools::CCharacter::WriteBYTE(dwJmpHookAddr + 0x4, 0x90);
		MyTools::CCharacter::WriteBYTE(dwJmpHookAddr + 0x5, 0x90);

		// 开始跳!
		while (GameRun && !EMPTY_PERSONHP && CPerson::GetInstance().GetPoint().Z < fHeight)
		{
			CSomeFun::GetInstance().SimlationKey(VK_SPACE);
		}

		// restore
		MyTools::CCharacter::WriteDWORD(dwJmpHookAddr + 0x0, dwOldValue1);
		MyTools::CCharacter::WriteDWORD(dwJmpHookAddr + 0x4, dwOldValue2);

		HookContent.dwHookAddr = dwBnsHookHeightAddr = CGameVariable::GetInstance().GetHookHeightAddr();
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"HookContent.dwHookAddr=%X", HookContent.dwHookAddr);
		HookContent.dwFunAddr = reinterpret_cast<DWORD>(fnBnsHookHeight);;
		HookContent.uNopCount = 0;
		MyTools::CLHook::Hook_Fun_Jmp_MyAddr(&HookContent);
		CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_FLOATING) = 0x1;
	}
	else
	{
		// 高空坠落
		if (HookContent.dwHookAddr != NULL)
		{
			MyTools::CLHook::UnHook_Fun_Jmp_MyAddr(&HookContent);
			HookContent.Release();
		}
		CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_FLOATING) = 0x0;
	}
	return TRUE;
}
