#include "stdafx.h"
#include "GameSend.h"
#include <MyTools/Log.h>

#define _SELF L"GameSend.cpp"

CGameSend::CGameSend()
{
}

CGameSend::~CGameSend()
{
}

BOOL CGameSend::UseSkill(_In_ DWORD dwSkillId)
{
	__try
	{
		BOOL bResult = FALSE;
		__asm
		{
			PUSHAD;
			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��2];
			MOV ESI, DWORD PTR DS : [EAX + ������ȴƫ��];
			PUSH dwSkillId;

			MOV EDI, ESI;
			MOV EAX, �ͷż���CALL;
			CALL EAX;
			MOV bResult, EAX;
			POPAD;
		}
		return (bResult & 0xFF) == 0x1 ? TRUE : FALSE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"UseSkill�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::UseItem(_In_ DWORD dwItemParm)
{
	__try
	{
		_asm
		{
			PUSHAD;

			PUSH 0;
			PUSH dwItemParm;
			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��2];
			MOV EAX, DWORD PTR DS : [EAX + ��������ƫ��];
			PUSH EAX;

			MOV EAX, ��Ʒʹ��CALL;
			CALL EAX;

			POPAD;
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"UseItem�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::WearEqui(_In_ DWORD dwEquiType, _In_ DWORD dwParm)
{
	__try
	{
		LOG_CF_D( L"dwEquiType=%X, dwParm=%X", dwEquiType, dwParm);
		_asm
		{
			PUSHAD;
			PUSH dwEquiType;
			PUSH 0;
			PUSH dwParm;

			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��2];
			MOV EAX, DWORD PTR DS : [EAX + ������ȴƫ��];
			PUSH EAX;

			MOV EAX, ��װ��CALL;
			CALL EAX;
			POPAD;
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"WearEqui�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::RelieveEqui(_In_ DWORD dwEquiType)
{
	__try
	{
		_asm
		{
			PUSHAD;
			PUSH dwEquiType;

			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��2];
			MOV EAX, DWORD PTR DS : [EAX + ��������ƫ��];
			PUSH EAX;

			MOV EAX, ��װ��CALL;
			CALL EAX;
			POPAD;
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"RelieveEqui�������쳣");
	}
	return FALSE;
}


BOOL CGameSend::HidePlayer()
{
	__try
	{
		_asm
		{
			PUSHAD
			MOV DL, 0x0
			MOV EAX, �������CALL
			CALL EAX
			POPAD
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"HidePlayer�������쳣");
	}
	return FALSE;
}


BOOL CGameSend::ExitSchool()
{
	__try
	{
		_asm
		{
			PUSHAD
			MOV EAX, �˳�����CALL
			CALL EAX
			POPAD
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"ExitSchool�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::TakeItem_By_Task(_In_ DWORD dwId, _In_ DWORD dwIdParm)
{
	__try
	{
		_asm
		{
			PUSHAD;
			PUSH dwIdParm;
			PUSH dwId;

			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��2];
			MOV EAX, DWORD PTR DS : [EAX + ������ȴƫ��];
			PUSH EAX;

			MOV ESI, EAX;

			MOV EAX, �������CALL;
			CALL EAX;

			//MOV BYTE PTR DS : [ESI + ���¼���ƫ��], 0x1;
			POPAD;
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"TakeItem_By_Task�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::TakeItem_By_Npc(_In_ DWORD dwId, _In_ DWORD dwIdParm)
{
	__try
	{
		_asm
		{
			PUSHAD;
			PUSH dwIdParm;
			PUSH dwId;
			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��2];
			MOV EAX, DWORD PTR DS : [EAX + ������ȴƫ��];
			PUSH EAX;

			MOV EAX, ��NPC����CALL;
			CALL EAX;
			POPAD;
		}

		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"TakeItem_By_Task�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::DiscardItem(_In_ DWORD dwItemSId, _In_ DWORD dwCount)
{
	__try
	{
		_asm
		{
			PUSHAD;
			PUSH 0;
			PUSH dwItemSId;

			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��2];
			MOV EAX, DWORD PTR DS : [EAX + ��������ƫ��];
			PUSH EAX;

			MOV ECX, dwCount;
			MOV EAX, ������ƷCALL;
			CALL EAX;
			POPAD;
		};
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"DiscardItem�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::ExplainItem(_In_ DWORD dwItemSId)
{
	__try
	{
		_asm
		{
			PUSHAD;

			PUSH 0;
			PUSH 0;
			PUSH 0;
			PUSH -1;
			PUSH 0;
			PUSH 0;
			PUSH 0;
			PUSH dwItemSId;

			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��2];
			MOV EAX, DWORD PTR DS : [EAX + ��������ƫ��];
			PUSH EAX;

			//MOV ECX, 0x0;
			//OR CL, 0xFF;
			MOV EBX, ��Ʒ�ֽ�CALL;
			CALL EBX;

			POPAD;
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"ExplainItem�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::SwitchChannel(_In_ DWORD dwChannel)
{
	__try
	{
		_asm
		{
			PUSHAD;

			PUSH dwChannel;
			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��2];


			MOV EBX, ѡ��Ƶ��CALL;
			CALL EBX;

			POPAD;
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"SwitchChannel�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::BackToSwitchPlayer()
{
	__try
	{
		using CallDefine = void(WINAPI *)(DWORD dwPaem);
		CallDefine Call = reinterpret_cast<CallDefine>(С��CALL);
		Call(ReadDWORD(ReadDWORD(ReadDWORD(�����ַ) + �����ַƫ��3) + 0x1C));
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"BackToSwitchPlayer�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::SetGem(_In_ DWORD dwItemSId, _In_ DWORD dwIndex, _In_ DWORD dwInventory2PanelObj)
{
	__try
	{
		_asm
		{
			PUSHAD;

			PUSH 0;
			PUSH dwItemSId;
			PUSH dwIndex;
			MOV EAX, dwInventory2PanelObj;
			ADD EAX, ��Ƕ��ʯƫ��;
			PUSH EAX;
			MOV EAX, ��Ƕ��ʯCALL;
			CALL EAX;

			POPAD;
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"SetGem�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::RelieveGem(_In_ DWORD dwIndex)
{
	__try
	{
		_asm
		{
			PUSHAD;
#ifdef BnsProject_Korea
			PUSH 0;
#endif // BnsProject_Korea
			PUSH dwIndex;
			PUSH 0;
			PUSH 0x10001; // �̶�����, ��ʾ����

			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��2];
			MOV EAX, DWORD PTR DS : [EAX + ��������ƫ��];
#ifdef BnsProject_Korea
			//PUSH EAX;
			MOV EBX, EAX;
#endif // BnsProject_Korea
#ifdef BnsProject_TaiWan
			MOV EBX, EAX;
#endif // BnsProject_TaiWan
			MOV EAX, ȡ��ʯCALL;
			CALL EAX;
			
			POPAD;
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"RelieveGem�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::SimulationKey(_In_ DWORD dwAscii, _In_ BOOL bPush)
{
	auto fnSimulationKey = [](_In_ DWORD dwAscii, _In_ BOOL bPop)
	{
		__try
		{
			DWORD dwValue = 0;
			_asm
			{
				PUSHAD

				MOV ESI, g_dwSimulationBase
				MOV ESI, DWORD PTR DS : [ESI]
				MOV ESI, DWORD PTR DS : [ESI]
				MOV ECX, DWORD PTR DS : [ESI + 0x58]

				PUSH dwAscii
				ADD ECX, 0x60
				MOV EAX, g_dwSimulationCALL
				CALL EAX
				MOV dwValue, EAX
				CMP EAX,0
				JE lb_Exit

				MOV EDX, EAX
				MOV EDX, DWORD PTR DS : [EDX + 0x4]
				MOV ESI, g_dwSimulationBase
				MOV ESI, DWORD PTR DS : [ESI]
				MOV ESI, DWORD PTR DS : [ESI]
				MOV ECX, DWORD PTR DS : [ESI + 0x20]
				MOV EAX, DWORD PTR DS : [ECX]
				MOV EAX, DWORD PTR DS : [EAX + 0x10]

				PUSH 0
				PUSH ECX
				XOR EDI, EDI
				NEG EDI
				SBB EDI, EDI
				AND EDI, 0x2

				MOV EBX, bPop
				CMP EBX, 1
				JE lb_bPop
				PUSH EDI
				JMP lb_Quit
			lb_bPop : ;
				PUSH 1

			lb_Quit:;

				PUSH EDX
				MOV EDX, dwValue
				MOV EDX, DWORD PTR DS : [EDX]
				PUSH EDX
				PUSH 0
				ADD ESI, 0x4
				PUSH ESI
				CALL EAX
				
			lb_Exit:;
				POPAD
			}
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			LOG_CF_E( L"fnSimulationKey�������쳣");
		}
	};

	fnSimulationKey(dwAscii, !bPush);
	return TRUE;
}

BOOL CGameSend::TakeItem_By_DropItem(_In_ DWORD dwId, _In_ DWORD dwIdParm)
{
	__try
	{
		_asm
		{
			PUSHAD;

			PUSH 0;
			PUSH 2;
			PUSH dwIdParm;
			PUSH dwId;

			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��2];
			MOV ECX, EAX;
			MOV ECX, DWORD PTR DS : [ECX + ������ȴƫ��];
			PUSH ECX;

			MOV ESI, ECX;

			MOV EAX, ���¼���CALL;
			CALL EAX;
			//MOV BYTE PTR DS : [ESI + ���¼���ƫ��], 0x1;
			POPAD;
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"TakeItem_By_DropItem�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::CleanBag()
{
	__try
	{
		_asm
		{
			PUSHAD;
			;
			PUSH 0;
			PUSH 0;
			PUSH 2;
			;
			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��2];
			MOV EAX, DWORD PTR DS : [EAX + ��������ƫ��];
			PUSH EAX;
			;
			MOV EBX, ������CALL;
			CALL EBX;
			;
			POPAD;
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"CleanBag�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::SellItem_By_Npc(_In_ DWORD dwItemSId, _In_ DWORD dwCount, _In_ DWORD dwNpcId, _In_ DWORD dwNpcIdParm)
{
	__try
	{
		BYTE dwBuff[256] = { 0 };
		*(DWORD*)((DWORD)dwBuff + ��NPCƫ�� + 0x0) = dwNpcId;
		*(DWORD*)((DWORD)dwBuff + ��NPCƫ�� + 0x4) = dwNpcIdParm;

		_asm
		{
			PUSH dwCount
			PUSH 0
			PUSH dwItemSId
			LEA EAX, dwBuff
			MOV EBX, ����ƷCALL
			CALL EBX
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"SellItem_By_Npc�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::BuyItem_By_Npc(_In_ DWORD dwNpcId, _In_ DWORD dwNpcIdParm, _In_ DWORD dwCount, _In_ DWORD dwIndex)
{
	__try
	{
		LPVOID lpAddr = AllocGameMem(0x28);
		_asm
		{
			PUSHAD;

			MOV EAX, lpAddr;

			MOV EBX, ����Ʒ�������CALL;
			CALL EBX;

			MOV EBX, EAX;
			MOV EDX, dwNpcId;
			MOV DWORD PTR DS : [EBX + 0x10], EDX;

			MOV EDX, dwNpcIdParm;
			MOV DWORD PTR DS : [EBX + 0x14], EDX;

			MOV WORD PTR DS : [EBX + 0x18], 0;

			MOV EDX, dwIndex;
			MOV BYTE PTR DS : [EBX + 0x19], DL;

			MOV EDX, dwCount;
			MOV WORD PTR DS : [EBX + 0x1A], DX;

			MOV EDX, 0;
			MOV DWORD PTR DS : [EBX + 0x1C], 0

			PUSH 0;
			MOV ESI, EBX;
			MOV EAX, ����CALL;
			CALL EAX;
			ADD ESP, 0x4;

			POPAD;
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"BuyItem_By_Npc�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::BuyItem_By_Bag(_In_ DWORD dwPageIndex, _In_ DWORD dwIndex, _In_ DWORD dwCount, _In_ DWORD dwItemIdParm)
{
	__try
	{
		//LOG_CF_D( L"dwArray[2]=%X", dwArray[2]);

		LPVOID lpArray = AllocGameMem(0xC);
		((DWORD*)lpArray)[0] = ����������Ʒ��������;
		((DWORD*)lpArray)[2] = (dwItemIdParm >> 0x10) + (0x1 << 0x10);

		LPVOID lpAddr = AllocGameMem(0x28);
		_asm
		{
			PUSHAD;
			MOV EAX, lpAddr;
			MOV EBX, ����Ʒ�������CALL;
			CALL EBX;

			MOV EBX, EAX;
			MOV EDX, 0;
			MOV DWORD PTR DS : [EBX + 0xC], EDX;
			MOV DWORD PTR DS : [EBX + 0x10], EDX;
			MOV DWORD PTR DS : [EBX + 0x14], EDX;

			MOV EDX, dwPageIndex;
			MOV WORD PTR DS : [EBX + 0x18], DX;

			MOV EDX, dwIndex;
			MOV BYTE PTR DS : [EBX + 0x19], DL;

			MOV EDX, dwCount;
			MOV WORD PTR DS : [EBX + 0x1A], DX;

			MOV EDX, 0x1;
			MOV DWORD PTR DS : [EBX + 0x1C], EDX;

			MOV EDX, lpArray;
			MOV DWORD PTR DS : [EBX + 0x20], EDX;
			MOV DWORD PTR DS : [EBX + 0x24], EDX;

			PUSH 0;
			MOV ESI, EBX;
			MOV EAX, ����CALL;
			CALL EAX;
			ADD ESP, 0x4;
			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"BuyItem_By_Bag�������쳣");
	}
	return FALSE;
}


BOOL CGameSend::CreateMoreBag()
{
	__try
	{
		LPVOID lpAddr = AllocGameMem(0x30);
		_asm
		{
			PUSHAD;

			MOV EAX, lpAddr;
			

			MOV EBX, 0x0;
			MOV WORD PTR DS : [EAX + 0x4], ����������ID;
			MOV DWORD PTR DS : [EAX + 0x8], EBX;
			MOV BYTE PTR DS : [EAX + 0xC], BL;
			MOV DWORD PTR DS : [EAX], ��������������;
			MOV DWORD PTR DS : [EAX + 0x24], EBX;
			MOV DWORD PTR DS : [EAX + 0x28], EBX;
			MOV WORD PTR DS : [EAX + 0x20], BX;

			MOV ESI, EAX;

			MOV DWORD PTR DS : [ESI + 0x18], 0;
			MOV DWORD PTR DS : [ESI + 0x1C], 0;
			MOV BYTE PTR DS : [ESI + 0x10], 0x2;
			PUSH 0;
			MOV EDX, ����CALL;
			CALL EDX;
			ADD ESP, 0x4;

			POPAD;

		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"CreateMoreBag�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::ExpandInvenSlot(_In_ DWORD dwNpcId, _In_ DWORD dwNpcIdParm, _In_ DWORD dwType)
{
	__try
	{
		LPVOID lpAddr = AllocGameMem(0x30);
		_asm
		{
			PUSHAD;

			MOV EAX, lpAddr;

			MOV EBX, 0x0;
			MOV WORD PTR DS : [EAX + 0x4], ����������ID;
			MOV DWORD PTR DS : [EAX + 0x8], EBX;
			MOV BYTE PTR DS : [EAX + 0xC], BL;
			MOV DWORD PTR DS : [EAX], ��������������;
			MOV DWORD PTR DS : [EAX + 0x24], EBX;
			MOV DWORD PTR DS : [EAX + 0x28], EBX;
			MOV WORD PTR DS : [EAX + 0x20], BX;

			MOV ESI, EAX;

			MOV EBX, dwNpcId;
			MOV DWORD PTR DS : [ESI + 0x18], EBX;

			MOV EBX, dwNpcIdParm;
			MOV DWORD PTR DS : [ESI + 0x1C], EBX;

			MOV EBX, dwType;
			MOV BYTE PTR DS : [ESI + 0x10], BL;
			PUSH 0;
			MOV EDX, ����CALL;
			CALL EDX;
			ADD ESP, 0x4;

			POPAD;
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"ExpandInvenSlot�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::TransportCity(_In_ DWORD dwTarCityId)
{
	__try
	{
		//LPVOID lpAddr = AllocGameMem(0x14);
		_asm
		{
			//	PUSHAD;

			//	MOV EAX, lpAddr;

			//	MOV WORD PTR DS : [EAX + 0x4], ��������ID;

			//	MOV DWORD PTR DS : [EAX + 0x8], 0x3;

			//	MOV BYTE PTR DS : [EAX + 0xC], 0x0;

			//	MOV DWORD PTR DS : [EAX], ������������;

			//	MOV ESI, EAX;

			//	MOV EAX, ��ȡ��ǰ����IDCALL;
			//	CALL EAX;

			//	MOV WORD PTR DS : [ESI + 0x10], AX;

			//	MOV EAX, dwTarCityId;
			//	MOV WORD PTR DS : [ESI + 0x12], AX;

			//	PUSH 0;
			//	MOV EAX, ����CALL;
			//	CALL EAX;
			//	ADD ESP, 0x4;

			//	POPAD;
			mov eax, �����ַ;
			mov eax, [eax];
			mov eax, [eax + �����ַƫ��1];
			mov eax, [eax + �����ַƫ��2];
			mov esi, [eax + ����ƫ��];
			mov ebx, dwTarCityId;
			mov eax, ����CALL2;
			call eax;
		}

			
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"TransportCity�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::ItemGrowth(_In_ DWORD dwGrowthItemSId, _In_ DWORD dwSacrificeSId, _In_ DWORD)
{
	__try
	{
		_asm
		{
			PUSHAD;

			PUSH 0;
			//PUSH dwLuckyCharmSId;	// ���֮��
			PUSH 0;
			PUSH 0
			PUSH dwSacrificeSId;	// ��Ʒ
			PUSH 0;
			PUSH dwGrowthItemSId;	// Ҫ�ɳ�����Ʒ

			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��2];
			MOV ECX, DWORD PTR DS : [EAX + ��������ƫ��];

			MOV EAX, ����ǿ��CALL;
			CALL EAX;

			POPAD;
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"ItemGrowth�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::ItemEvolution(_In_ DWORD dwGrowthItemSId, _In_ DWORD dwType, _In_ DWORD dwSacrificeGId, _In_ DWORD dwEvolutionId)
{
	LOG_CF_D( L"Equi=%X, SacrificeSId=%X", dwGrowthItemSId, dwSacrificeGId);
	__try
	{
		_asm
		{
			PUSHAD;
			PUSH 0;

			PUSH 0;
			PUSH dwType;
			PUSH dwSacrificeGId; // ��ƷID

			PUSH 0;
			PUSH dwGrowthItemSId; // ����ID
			PUSH 0;
			PUSH dwEvolutionId;

			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��2];
			MOV ECX, DWORD PTR DS : [EAX + ��������ƫ��];
			PUSH ECX;

			MOV EAX, ��������CALL;
			CALL EAX;

			POPAD;
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"ItemEvolution�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::ItemEndEvolution(_In_ DWORD dwEqumentSId, _In_ DWORD dwSacrificeSId, _In_ DWORD dwEvoltionId)
{
	__try
	{
		_asm
		{
			PUSHAD;

			PUSH 0;
			PUSH 0;
			PUSH 1;

			// ��ƷID
			PUSH dwSacrificeSId;

			PUSH 0;
			PUSH dwEqumentSId;

			PUSH 0;
			PUSH dwEvoltionId;

			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��2];
			MOV ECX, DWORD PTR DS : [EAX + ��������ƫ��];

			MOV EAX, װ��ͻ��CALL;
			CALL EAX;

			POPAD;
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"ItemEndEvolution�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::UnsealItem(_In_ DWORD dwUnsealItemSId, _In_ DWORD dwUntieId, _In_ DWORD dwUntieCount , _In_ DWORD nType)
{
	__try
	{
		__asm
		{
			PUSHAD;

			PUSH dwUntieCount;
			PUSH 1;
			PUSH dwUntieId;
			PUSH nType;
			PUSH 0;
			PUSH dwUnsealItemSId;
			

			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��2];
			MOV ECX, EAX;
			MOV ECX, DWORD PTR DS : [ECX + ��������ƫ��];

			MOV EAX, ��ӡCALL;
			CALL EAX;

			POPAD;
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"UnsealItem�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::OpenBox(_In_ DWORD dwBoxSID, _In_ BOOL bSpecial)
{
	__try
	{
		_asm
		{
			PUSHAD;

			PUSH 0;
			PUSH 0;
			PUSH 0;
			PUSH - 1;
			PUSH bSpecial;
			PUSH 1;
			PUSH 0;

			PUSH dwBoxSID;

			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��2];
			MOV EAX, DWORD PTR DS : [EAX + ��������ƫ��];
			PUSH EAX;

			MOV EAX, ������CALL;
			CALL EAX;

			POPAD;
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"OpenBox�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::OpenBox_By_Key(_In_ BOOL, _In_ DWORD dwKeyId, _In_ DWORD, _In_ DWORD dwBoxSId)
{
	__try
	{
		__asm
		{
			PUSHAD;

			PUSH 1;
			PUSH 1;
			PUSH dwKeyId;
			PUSH 0;
			PUSH 0;
			PUSH 1;
			PUSH 0;
			PUSH dwBoxSId;

			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��2];
			MOV EAX, DWORD PTR DS : [EAX + ��������ƫ��];
			PUSH EAX;

			MOV EAX, ������CALL;
			CALL EAX;

			POPAD;
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"OpenBox_By_Key�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::Move(_In_ float fX, _In_ float fY, _In_ float fZ)
{
	__try
	{
		static DWORD dwMoveCALL = NULL;
		if (dwMoveCALL == NULL)
		{
			FARPROC fMove = ::GetProcAddress(::GetModuleHandleW(L"bsengine_Shipping.dll"), "EInterfaceGetInstance");
			dwMoveCALL = fMove();
		}
		_asm
		{
			PUSHAD;
			PUSHFD;
			MOV EAX, dwMoveCALL;
			MOV EDX, DWORD PTR DS : [EAX];
			MOV ECX, EAX;
			MOV EAX, DWORD PTR DS : [EDX + ��·ƫ��];
			PUSH fZ;
			PUSH fY;
			PUSH fX;
			CALL EAX;
			POPFD;
			POPAD;
		}
//#pragma region TestCode
//		__asm
//		{
//			PUSHAD;
//			PUSHFD;
//
//			PUSH fZ;
//			PUSH fY;
//			PUSH fX;
//
//			MOV EAX, �����ַ;
//			MOV EAX, [EAX];
//			MOV EAX, [EAX + 0x58];
//			ADD EAX, 0x8;
//			MOV EAX, [EAX + 0x2C];
//			PUSH EAX;
//
//			MOV EAX, 0x069B1E0;
//			CALL EAX;
//
//			POPFD;
//			POPAD;
//		}
//#pragma endregion
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"Move�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::Mail(_In_ DWORD dwNpcId, _In_ DWORD dwNpcIdParm, _In_ MailParm_Src* pMailParm)
{
	__try
	{
		_asm
		{
			PUSHAD;

			PUSH dwNpcIdParm;
			PUSH dwNpcId;

			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��2];
			MOV EAX, DWORD PTR DS : [EAX + �ʼĲ���ƫ��];
			PUSH EAX;

			MOV ECX, pMailParm;
			MOV EAX, �ʼ�CALL;
			CALL EAX;

			POPAD;
		}

		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"Mail�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::SaveItem(_In_ DWORD dwNpcId, _In_ DWORD dwNpcIdParm, _In_ DWORD dwItemSId, _In_ DWORD dwCount, _In_ DWORD dwWareHourseSId)
{
	__try
	{
		static DWORD dwArray1[32];
		static DWORD dwArray2[32];
		static DWORD dwArray3[32];

		ZeroMemory(dwArray1, sizeof(dwArray1));
		ZeroMemory(dwArray2, sizeof(dwArray2));
		ZeroMemory(dwArray3, sizeof(dwArray3));

		dwArray1[0] = 0xFFFF0000 + dwCount;
		dwArray2[0] = dwWareHourseSId;
		dwArray3[0] = dwItemSId;
		

		_asm
		{
			PUSHAD;

			PUSH dwNpcIdParm;
			PUSH dwNpcId;
			PUSH 1;

			LEA EAX, dwArray1;
			PUSH EAX;

			LEA EAX, dwArray2;
			PUSH EAX;

			LEA EAX, dwArray3;
			PUSH EAX;

			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��2];
			MOV EAX, DWORD PTR DS : [EAX + ��������ƫ��];
			PUSH EAX;

			MOV EAX, ���CALL;
			CALL EAX;

			POPAD;
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"SaveItem�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::SumitMainTask(_In_ CONST SumitTask_Src* pTaskSrc)
{
	__try
	{
		DWORD dwScheduleId = pTaskSrc->dwScheduleId;
		DWORD dwTaskId = pTaskSrc->dwTaskId;
		DWORD dwParm3 = pTaskSrc->dwParm3;
		DWORD dwParm4 = pTaskSrc->dwParm4;
		DWORD dwParm5 = pTaskSrc->dwParm5;
		DWORD dwObjId = pTaskSrc->dwObjId;
		DWORD dwObjIdParm = pTaskSrc->dwObjIdParm;
		DWORD dwItemIndex = pTaskSrc->dwItemIndex;

		LPVOID lpAddr = AllocGameMem(0x48);
		_asm
		{
			PUSHAD;
			MOV EAX, lpAddr;

			MOV EBX, ȡ����IDCALL;
			CALL EBX;

			MOV ESI, EAX;

			MOV ECX, dwScheduleId;
			MOV BYTE PTR DS : [ESI + 0x12], CL;

			MOV EAX, dwTaskId;
			MOV WORD PTR DS : [ESI + 0x10], AX;

			PUSH dwParm3;

			MOV EDX, dwParm4;
			MOV BYTE PTR DS : [ESI + 0x13], DL;

			MOV EAX, dwParm5;
			MOV BYTE PTR DS : [ESI + 0x14], AL;

			MOV EAX, dwObjId;
			MOV DWORD PTR DS : [ESI + 0x18], EAX;

			MOV EAX, dwObjIdParm;
			MOV DWORD PTR DS : [ESI + 0x1C], EAX;

			MOV EDI, dwItemIndex;
			MOV WORD PTR DS : [ESI + 0x20], DI;

			MOV EAX, ����CALL;
			CALL EAX;
			ADD ESP, 0x4;

			POPAD;
		}

		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"SumitMainTask�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::SumitSpecialMainTask(_In_ CONST SumitTask_Src* pTaskSrc)
{
	__try
	{
		DWORD dwScheduleId = pTaskSrc->dwScheduleId;
		DWORD dwTaskId = pTaskSrc->dwTaskId;
		//DWORD dwParm3 = pTaskSrc->dwParm3;
		DWORD dwParm4 = pTaskSrc->dwParm4;
		DWORD dwParm5 = pTaskSrc->dwParm5;
		DWORD dwObjId = pTaskSrc->dwObjId;
		DWORD dwObjIdParm = pTaskSrc->dwObjIdParm;
		//DWORD dwItemIndex = pTaskSrc->dwItemIndex;

		BOOL bSpecial = CGameVariable::GetInstance().IsSpecialNpcTask(TaskInfoParm(dwTaskId, dwScheduleId));
		if (bSpecial)
			__asm PUSH 1;
		else
			__asm PUSH 0;

		_asm
		{
			PUSH dwObjIdParm;
			PUSH dwObjId;
			PUSH dwParm5;
			PUSH dwParm4;
			PUSH dwScheduleId;
			PUSH dwTaskId;

			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS:[EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��2];
			MOV EAX, DWORD PTR DS : [EAX + ������ƫ��];
			PUSH EAX;

			MOV EAX, ������CALL;
			CALL EAX;
		}

		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"SumitSpecialMainTask�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::PickBLTask(_In_ CONST SumitBLTask_Src* pTaskSrc)
{
	__try
	{
		DWORD dwObjId = pTaskSrc->dwNpcId;
		DWORD dwObjIdParm = pTaskSrc->dwNpcIdParm;
		DWORD dwTaskId = pTaskSrc->dwTaskId;
		DWORD dwParm4 = pTaskSrc->dwParm4;
		DWORD dwParm5 = pTaskSrc->dwParm5;

		LPVOID lpAddr = AllocGameMem(0x40);
		_asm
		{
			PUSHAD;

			MOV EAX, lpAddr;

			MOV EBX, ȡ֧������IDCALL;
			CALL EBX;
			MOV ESI, EAX;

			MOV EBX, dwObjId;
			MOV DWORD PTR DS : [ESI + 0x18], EBX;

			MOV EBX, dwObjIdParm;
			MOV DWORD PTR DS : [ESI + 0x1C], EBX;

			MOV EBX, dwTaskId;
			MOV WORD PTR DS : [ESI + 0x10], BX;

			PUSH 0;

			MOV EDI, dwParm4;
			SHR EDI, 0x10;
			MOV WORD PTR DS : [ESI + 0x20], DI;
			MOV BYTE PTR DS : [ESI + 0x13], 0x0;

			MOV EBX, dwParm5;
			MOV BYTE PTR DS : [ESI + 0x12], BL;

			MOV EDX, ����CALL;
			CALL EDX;
			ADD ESP, 0x4;

			POPAD;
		}

		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"SumitBLTask�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::RefushChannel()
{
	__try
	{
		_asm
		{
			PUSHAD

			MOV EAX, ˢ��Ƶ��CALL
			CALL EAX

			POPAD
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"RefushChannel�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::DeletePlayer(_In_ DWORD dwIndexParm)
{
	__try
	{
		LOG_CF_D( L"dwIndexParm=%d", dwIndexParm);
		_asm
		{
			PUSHAD;

			PUSH dwIndexParm;

			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EBX, DWORD PTR DS : [EAX + 0x30];

			MOV EAX, ɾ����ɫCALL;
			CALL EAX;

			POPAD;
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"DeletePlayer�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::CreatePlayer(_In_ LPCWSTR pwszPlayerName, _In_ DWORD dwAreaId, _In_ DWORD, _In_ DWORD dwClasses, _In_ BOOL bIsQuickLevel)
{
	__try
	{
		static DWORD dwArray[4096];
		ZeroMemory(dwArray, sizeof(dwArray));
		
		switch (dwClasses)
		{
		case static_cast<DWORD>(em_Player_Classes::em_Player_Classes_Summoner):
			LOG_CF_D(L"������ɫ:�ٻ�ʦ");
			dwArray[0] = 0x4F060203;
			dwArray[1] = 0x01060A18;
			dwArray[2] = 0x100C2D0C;
			dwArray[3] = 0x04031802;
			dwArray[4] = 0x01020201;
			dwArray[5] = 0x39000201;
			dwArray[6] = 0x00000000;
			dwArray[7] = 0x21260E00;
			dwArray[8] = 0x28242600;
			dwArray[9] = 0xDD05F1F1;
			dwArray[10] = 0x0A142DEC;
			dwArray[11] = 0x0A0A05FB;
			dwArray[12] = 0x14051400;
			dwArray[13] = 0xF1F1001E;
			dwArray[14] = 0x00000000;
			dwArray[15] = 0x00000000;
			dwArray[16] = 0x00000000;
			dwArray[17] = 0x1932000A;
			dwArray[18] = 0x00002323;
			dwArray[19] = 0x00D89CC4;
			dwArray[20] = 0x002314C4;
			dwArray[21] = 0x00000000;
			dwArray[22] = 0x000000F6;
			dwArray[23] = 0x00000000;
			break;
		case static_cast<DWORD>(em_Player_Classes::em_Player_Classes_KongFuForceMaster):
			dwArray[0] = (0x10050102 & 0xFF00FFFF) + (dwClasses << 0x10);
			LOG_CF_D( L"dwArray[0]=%X, dwClasses=%X, Name=%s", dwArray[0], dwClasses, pwszPlayerName);
			dwArray[1] = 0x010B0400;
			dwArray[2] = 0x0E05170C;
			dwArray[3] = 0x01010201;
			dwArray[4] = 0x01020101;
			dwArray[5] = 0x32010104;
			dwArray[6] = 0x30262626;
			dwArray[7] = 0x18300043;
			dwArray[8] = 0x0E0E0000;
			dwArray[11] = 0x00000005;
			dwArray[17] = 0x000A0000;
			dwArray[23] = 0x00FB0000;
			break;
		case static_cast<DWORD>(em_Player_Classes::em_Player_Classes_Warlock) :
			LOG_CF_D(L"������ɫ:����ʦ");
			dwArray[0] = 0x09090204;
			dwArray[1] = 0x01060D00;
			dwArray[2] = 0x0E0E170C;
			dwArray[3] = 0x01031801;
			dwArray[4] = 0x01020201;
			dwArray[5] = 0x35000102;
			dwArray[6] = 0x00000000;
			dwArray[7] = 0x56643000;
			dwArray[8] = 0x050A1D21;
			dwArray[9] = 0x00000000;
			dwArray[10] = 0xECC93200;
			dwArray[11] = 0xF6000000;
			dwArray[12] = 0x0005FB00;
			dwArray[13] = 0x0A140000;
			dwArray[14] = 0x001E00FB;
			dwArray[23] = 0x0B000000;
			//dwArray[29] = 0x00180000;
			//dwArray[30] = 0X00000000;
			//dwArray[31] = 0x0018CFB4;
			//dwArray[32] = 0x39034C40;
			//dwArray[33] = 0x0018CAD8;
			//dwArray[34] = 0x0018CA90;
			//dwArray[35] = 0x0018D00C;
			//dwArray[36] = 0x0BC9ADC8;
			//dwArray[37] = 0x00000000;
			//dwArray[38] = 0x00000000;
			//dwArray[39] = 0x0018CF60;
			//dwArray[40] = 0x39034C40;
			//dwArray[41] = 0x0018CAF8;
			//dwArray[42] = 0x00000000;
			//dwArray[43] = 0xE9D318BE;
			LOG_CF_D(L"dwArray[0]=%X, dwClasses=%X, Name=%s", dwArray[0], dwClasses, pwszPlayerName);
			break;
		case static_cast<DWORD>(em_Player_Classes::em_Player_Classes_ForceMaster):
			LOG_CF_D(L"������ɫ:����");
			dwArray[0] = 0x12030201;
			dwArray[1] = 0x0B080B00;
			dwArray[2] = 0x10091D0C;
			dwArray[3] = 0x04032701;
			dwArray[4] = 0x01040301;
			dwArray[5] = 0x39000603;
			dwArray[6] = 0x00000000;
			dwArray[7] = 0x391D1D00;
			dwArray[8] = 0x0A262100;
			dwArray[9] = 0xBA00CEBF;
			dwArray[10] = 0xA6FB5037;
			dwArray[11] = 0xBAD80019;
			dwArray[12] = 0xCE28ECBA;
			dwArray[13] = 0xDDCEE2E2;
			dwArray[14] = 0x19E250C4;
			dwArray[15] = 0x9CCED800;
			dwArray[16] = 0x00002DFB;
			dwArray[17] = 0xEC00CEF1;
			dwArray[18] = 0xF1F10000;
			dwArray[19] = 0x00D800E2;
			dwArray[20] = 0x00CE00C4;
			dwArray[21] = 0x00CE00E2;
			dwArray[22] = 0xD80A00F1;
			dwArray[23] = 0x0B000F9C;
			LOG_CF_D(L"dwArray[0]=%X, dwClasses=%X, Name=%s", dwArray[0], dwClasses, pwszPlayerName);
			break;
		case static_cast<DWORD>(em_Player_Classes::em_Player_Classes_Danncer):
			LOG_CF_D(L"������ɫ:�齣");
			dwArray[0] = 0x01080203;
			dwArray[1] = 0x01060A02;
			dwArray[2] = 0x1003050C;
			dwArray[3] = 0x02031802;
			dwArray[4] = 0x01020201;
			dwArray[5] = 0x39000201;
			dwArray[6] = 0x00000000;
			dwArray[7] = 0x00000E00;
			dwArray[8] = 0x05050E00;
			dwArray[23] = 0x0B000000;
			LOG_CF_D(L"dwArray[0]=%X, dwClasses=%X, Name=%s", dwArray[0], dwClasses, pwszPlayerName);
			break;
		case static_cast<DWORD>(em_Player_Classes::em_Player_Classes_BladeMaster) :
			LOG_CF_D(L"������ɫ:��ʿ");
			dwArray[0] = 0x09010204;
			dwArray[1] = 0x01060D00;
			dwArray[2] = 0x0E0E170C;
			dwArray[3] = 0x01031801;
			dwArray[4] = 0x01020201;
			dwArray[5] = 0x35000102;
			dwArray[6] = 0x00000000;
			dwArray[7] = 0x56643000;
			dwArray[8] = 0x050A1D21;
			dwArray[9] = 0x00000000;
			dwArray[10] = 0xECC93200;
			dwArray[11] = 0xF6000000;
			dwArray[12] = 0x0005FB00;
			dwArray[13] = 0x0A140000;
			dwArray[14] = 0x001E00FB;
			dwArray[23] = 0x0B000000;
			LOG_CF_D(L"dwArray[0]=%X, dwClasses=%X, Name=%s", dwArray[0], dwClasses, pwszPlayerName);
			/*dwArray[0] = 0x03010104;
			dwArray[1] = 0x01090200;
			dwArray[2] = 0x0E03180C;
			dwArray[3] = 0x02010201;
			dwArray[4] = 0x07030102;
			dwArray[5] = 0x2B010106;
			dwArray[6] = 0x00000000;
			dwArray[7] = 0x343E0043;
			dwArray[8] = 0x05260E00;
			dwArray[9] = 0x0AF1ECB0;
			dwArray[10] = 0x00F6E7C9;
			dwArray[11] = 0xDD0F00F1;
			dwArray[12] = 0xF10000BF;
			dwArray[13] = 0x00000A14;
			dwArray[14] = 0x14D3050F;
			dwArray[15] = 0xC428F61E;
			dwArray[16] = 0x0000DDBA;
			dwArray[17] = 0x00050000;*/
			break;
		case static_cast<DWORD>(em_Player_Classes::em_Player_Classes_KongFuMaster) :
			LOG_CF_D(L"������ɫ:ȭʦ");
			dwArray[0] = 0x09020204;
			dwArray[1] = 0x01060D00;
			dwArray[2] = 0x0E0E170C;
			dwArray[3] = 0x01031801;
			dwArray[4] = 0x01020201;
			dwArray[5] = 0x35000102;
			dwArray[6] = 0x00000000;
			dwArray[7] = 0x56643000;
			dwArray[8] = 0x050A1D21;
			dwArray[10] = 0xECC93200;
			dwArray[11] = 0xF6000000;
			dwArray[12] = 0x0005FB00;
			dwArray[13] = 0x0A140000;
			dwArray[14] = 0x001E00FB;
			dwArray[23] = 0x0B000000;
			LOG_CF_D(L"dwArray[0]=%X, dwClasses=%X, Name=%s", dwArray[0], dwClasses, pwszPlayerName);
			break;
		case static_cast<DWORD>(em_Player_Classes::em_Player_Classes_Assassin) :
			LOG_CF_D(L"������ɫ:�̿�");
			dwArray[0] = 0x09070204;
			dwArray[1] = 0x01060D00;
			dwArray[2] = 0x0E0E170C;
			dwArray[3] = 0x01031801;
			dwArray[4] = 0x01020201;
			dwArray[5] = 0x35000102;
			dwArray[6] = 0x00000000;
			dwArray[7] = 0x56643000;
			dwArray[8] = 0x050A1D21;
			dwArray[9] = 0x00000000;
			dwArray[10] = 0xECC93200;
			dwArray[11] = 0xF6000000;
			dwArray[12] = 0x0005FB00;
			dwArray[13] = 0x0A140000;
			dwArray[14] = 0x001E00FB;
			dwArray[23] = 0x0B000000;
			LOG_CF_D(L"dwArray[0]=%X, dwClasses=%X, Name=%s", dwArray[0], dwClasses, pwszPlayerName);
			break;
		case static_cast<DWORD>(em_Player_Classes::em_Player_Classes_Gunner) :
			LOG_CF_D(L"������ɫ:ǹ��");
			dwArray[0] = 0x09040204;
			dwArray[1] = 0x01060D00;
			dwArray[2] = 0x0E0E170C;
			dwArray[3] = 0x01031801;
			dwArray[4] = 0x01020201;
			dwArray[5] = 0x35000102;
			dwArray[6] = 0x00000000;
			dwArray[7] = 0x56643000;
			dwArray[8] = 0x050A1D21;
			dwArray[9] = 0x00000000;
			dwArray[10] = 0xECC93200;
			dwArray[11] = 0xF6000000;
			dwArray[12] = 0x0005FB00;
			dwArray[13] = 0x0A140000;
			dwArray[14] = 0x001E00FB;
			dwArray[23] = 0x0B000000;
			LOG_CF_D(L"dwArray[0]=%X, dwClasses=%X, Name=%s", dwArray[0], dwClasses, pwszPlayerName);
			//[24] 0078007A   1��
			break;
		default:
			LOG_CF_E(L"������ɫ:δ֪����");
			break;
		}
		
		MyTools::CCharacter::wstrcpy_my((LPWSTR)&dwArray[24], pwszPlayerName);
		_asm
		{
			PUSHAD;
			PUSH bIsQuickLevel;
			LEA EAX, dwArray;
			PUSH EAX;
			MOV ECX, dwAreaId
			PUSH ECX;
			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + ������ɫƫ��];
			PUSH EAX;
			MOV EAX, ������ɫCALL;
			CALL EAX;
			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"CreatePlayer�������쳣");
	}
	return TRUE;
}

BOOL CGameSend::StartPK()
{
	__try
	{
		_asm
		{
			PUSHAD
			MOV EDI, �����ַ
			MOV EDI, DWORD PTR DS : [EDI]
			MOV EDI, DWORD PTR DS : [EDI + �����ַƫ��1]
			MOV EDI, DWORD PTR DS : [EDI + PKƫ��]
			MOV EDI, DWORD PTR DS : [EDI + 0x10]
			MOV EAX, PKCALL
			CALL EAX
			POPAD
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"StartPK�������쳣");
		g_bExitGame = TRUE;
	}
	return FALSE;
}

BOOL CGameSend::ExitPK()
{
	__try
	{
		_asm
		{
			PUSHAD
			MOV EDI, �����ַ
			MOV EDI, DWORD PTR DS : [EDI]
			MOV EDI, DWORD PTR DS : [EDI + �����ַƫ��1]
			MOV EDI, DWORD PTR DS : [EDI + PKƫ��]
			MOV EDI, DWORD PTR DS : [EDI + 0x10]
			MOV EAX, EDI
			MOV EBX, �뿪PK����CALL
			CALL EBX
			POPAD
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"ExitPK�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::EndPK()
{
	__try
	{
		_asm
		{
			PUSHAD
			MOV EDI, �����ַ
			MOV EDI, DWORD PTR DS : [EDI]
			MOV EDI, DWORD PTR DS : [EDI + �����ַƫ��1]
			MOV EDI, DWORD PTR DS : [EDI + PKƫ�� + 0x4]
			MOV EDI, DWORD PTR DS : [EDI + 0x10]
			MOV ECX, EDI
			MOV EBX, PKս������CALL
			CALL EBX
			POPAD
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"EndPK�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::ReadyPk()
{
	__try
	{
		_asm
		{
			PUSHAD
			MOV EAX, �����ַ
			MOV EAX, DWORD PTR DS : [EAX]
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1]
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��2]

			MOV ECX, DWORD PTR DS : [EAX + 0xC]
			MOV EDX, DWORD PTR DS : [EAX + 0x8]

			PUSH ECX
			PUSH EDX

			MOV EDI, �����ַ
			MOV EDI, DWORD PTR DS : [EDI]
			MOV EDI, DWORD PTR DS : [EDI + �����ַƫ��1]
			MOV EDI, DWORD PTR DS : [EDI + PKƫ�� + 0x4]
			MOV EDI, DWORD PTR DS : [EDI + 0x10]
			MOV EAX, EDI

			MOV EDI, PK׼��CALL
			CALL EDI

			POPAD
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"ReadyPk�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::PickCorpse(_In_ DWORD dwId, _In_ DWORD dwIdParm)
{
	__try
	{
		LPVOID lpAddr = AllocGameMem(0x18);
		_asm
		{
			PUSHAD;

			MOV EAX, lpAddr;

			MOV ESI, EAX;

			MOV EBX, ��ʬ�巢������2;
			MOV DWORD PTR DS : [ESI + 0x0], EBX;

			MOV EBX, ��ʬ�巢������1;
			MOV DWORD PTR DS : [ESI + 0x4], EBX;

			MOV DWORD PTR DS : [ESI + 0x8], 0;

			MOV BYTE PTR DS : [ESI + 0xC], 0;

			MOV EBX, dwId;
			MOV DWORD PTR DS : [ESI + 0x10], EBX;

			MOV EBX, dwIdParm;
			MOV DWORD PTR DS : [ESI + 0x14], EBX;

			PUSH 0;
			MOV EDX, ����CALL;
			CALL EDX;
			ADD ESP, 0x4;

			POPAD;
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"PickCorpse�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::RefushDianKa(_In_ DWORD dwPanelObj)
{
	__try
	{
		_asm
		{
			PUSHAD
			MOV EAX, ��ȡ�㿨ʱ��Base
			MOV EAX, DWORD PTR DS : [EAX]
			SUB EAX, 0x3
			SUB EAX, 0x2
			MOV EDI, dwPanelObj
			MOV ESI, ��ȡ�㿨ʱ��CALL
			CALL ESI
			POPAD
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"RefushDianKa�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::SwitchAngel(_In_ DWORD dwPanelObj)
{
	__try
	{
		_asm
		{
			MOV ECX, dwPanelObj
			MOV BL, 0
			MOV EAX, �޸��Ӿ�CALL
			CALL EAX
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"SwitchAngel�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::Login(_In_ DWORD dwAccountAddr, _In_ DWORD dwPassAddr)
{
	__try
	{
		_asm
		{
			PUSHAD
			PUSH dwPassAddr
			PUSH dwAccountAddr
			MOV EAX, ��¼CALL
			CALL EAX
			POPAD
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"Login�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::RecvMail(_In_ DWORD dwNpcId, _In_ DWORD dwNpcIdParm, _In_ DWORD dwUiObj, _In_ DWORD dwMailParm)
{
	__try
	{
		DWORD dwEBX = dwUiObj + �ʼ�ƫ��1 + 8;
		DWORD dwParm = ReadDWORD(dwMailParm + 0x64 + 0x4);
		DWORD dwParm1 = ReadDWORD(dwParm + 0x4);
		DWORD dwParm2 = ReadDWORD(dwParm + 0x0);

		dwParm1 = (dwParm1 == 0xFFFFFFFF ? 0 : dwParm1);
		dwParm2 = (dwParm2 == 0xFFFFFFFF ? 0 : dwParm2);

		_asm
		{
			PUSHAD
			PUSH dwNpcIdParm
			PUSH dwNpcId
			MOV EBX, dwEBX

			MOV ECX, dwParm1
			PUSH ECX
			MOV ECX, dwParm2
			PUSH ECX

			MOV EAX, ���ʼ�CALL
			CALL EAX

			POPAD
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"RecvMail�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::RecvEmptyMail(_In_ DWORD dwUiObj)
{
	__try
	{
		DWORD dwEAX = dwUiObj + �ʼ�ƫ��1 + 8;
		_asm
		{
			PUSHAD
			PUSH 0
			MOV EAX, dwEAX
			MOV ESI, ������Ʒ�ʼ�CALL
			CALL ESI
			POPAD
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"RecvEmptyMail�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::RecvAllMail(_In_ DWORD dwUiObj)
{
	__try
	{
		_asm
		{
			PUSHAD
			MOV ECX, dwUiObj
			MOV EDI, DWORD PTR DS : [ECX + ���ʼ�ƫ��]
			PUSH EDI
			MOV EAX, �������ʼ�CALL
			CALL EAX
			POPAD
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"RecvAllMail�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::SetAuctionQuality(_In_ em_Auction_Quality emAuctionQuality)
{
	__try
	{
		DWORD dwAuctionQuality = (DWORD)emAuctionQuality;
		_asm
		{
			PUSHAD
			PUSH 0
			PUSH 0
			PUSH 0
			PUSH 0
			PUSH dwAuctionQuality
			PUSH 0x0 // 0 = ����ʧȥ ,0x1 ˳��ʰȡ
			MOV EAX, ���þ���Ʒ��CALL
			CALL EAX
			POPAD
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"SetAuctionQuality�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::InviteTeamMember(_In_ DWORD dwId, _In_ DWORD dwIdParm)
{
	__try
	{
		_asm
		{
			PUSHAD;
			PUSH 0;
			PUSH 0;
			PUSH dwIdParm;
			PUSH dwId;

			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��2];
			MOV EAX, DWORD PTR DS : [EAX + �������ƫ��];

			PUSH EAX;
			MOV EAX, �����ԱCALL;
			CALL EAX;
			POPAD;
		}

		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"InviteTeamMember�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::LeaveTeam()
{
	__try
	{
		_asm
		{
			PUSHAD
			MOV EAX, �뿪����CALL
			CALL EAX
			POPAD
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"LeaveTeam�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::TakeBall(_In_ DWORD dwId, _In_ DWORD dwIdParm)
{
	__try
	{
		LPVOID lpAddr = AllocGameMem(0x18);
		_asm
		{
			PUSHAD;
			MOV EAX, lpAddr;
			MOV ESI, EAX;

			MOV EBX, ��JB��������;
			MOV DWORD PTR DS : [ESI + 0x0], EBX;

			MOV EBX, ��JB����ID;
			MOV DWORD PTR DS : [ESI + 0x4], EBX;

			MOV DWORD PTR DS : [ESI + 0x8], 0;

			MOV BYTE PTR DS : [ESI + 0xC], 0;

			MOV EBX, dwId;
			MOV DWORD PTR DS : [ESI + 0x10], EBX;

			MOV EBX, dwIdParm;
			MOV DWORD PTR DS : [ESI + 0x14], EBX;

			PUSH 0;
			MOV EDX, ����CALL;
			CALL EDX;
			ADD ESP, 0x4;

			POPAD;
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"TakeBall�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::Pickup(_In_ DWORD dwId, _In_ DWORD dwIdParm)
{
	__try
	{
		_asm
		{
			PUSHAD;
			PUSH 0;
			PUSH 2;
			PUSH dwIdParm;
			PUSH dwId;

			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��2];
			MOV EAX, DWORD PTR DS : [EAX + һ�μ���ƫ��];
			PUSH EAX

			MOV EAX, һ�μ���CALL;
			CALL EAX;
			POPAD;
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"TakeItem�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::Telport(_In_ DWORD* pArray, _In_ DWORD dwCALL, _In_ DWORD dwPersonBase)
{
	__try
	{
		__asm
		{
			PUSHAD;
			PUSH 0;
			PUSH 0;
			MOV EAX, dwPersonBase;
			MOV EAX, DWORD PTR DS : [EAX + 0x14];
			ADD EAX, ����˲��ƫ��2;
			MOV ECX, DWORD PTR DS : [EAX + ����˲��ƫ��];
			MOV EAX, pArray
			MOV WORD PTR DS : [EAX + ����˲��ƫ��], CX;
			//PUSH EAX;
			MOV EDI, EAX;
			MOV EAX, dwCALL;
			CALL EAX;
			POPAD;
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"Telport�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::Telport(_In_ DWORD dwCALL, _In_ DWORD dwPersonBase)
{
	__try
	{
		__asm
		{
			PUSHAD;
			PUSH 0;
			PUSH 0;
			MOV EAX, dwPersonBase;
			MOV EAX, DWORD PTR DS : [EAX + 0x14];
			ADD EAX, ����˲��ƫ��2;
			MOV EDI, EAX;
			MOV EAX, dwCALL;
			CALL EAX;
			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"Telport�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::SwitchNewMainTask(_In_ DWORD dwUiObj)
{
	__try
	{
		_asm
		{
			PUSHAD;
			MOV EDI, dwUiObj;
			PUSH EDI;
			MOV EAX, �л�������CALL;
			CALL EAX;
			POPAD;
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"SwitchNewMainTask�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::AddSkillPoint(_In_ DWORD dwIndex, _In_ DWORD dwLearnId)
{
	__try
	{
		__asm
		{
			PUSHAD;
			MOV ECX, dwIndex;

			MOV EDX, dwLearnId;
			MOV EAX, ���ܼӵ�CALL;
			CALL EAX;

			POPAD;
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"AddSkillPoint�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::AddSkillPoint_Send(_In_ DWORD dwSkillBook3Panel)
{
	__try
	{
		__asm
		{
			PUSHAD;
			MOV EAX, �����ַ
			MOV EAX, [EAX]
			MOV EAX, [EAX + �����ַƫ��1]
			MOV EAX, [EAX + �����ַƫ��2]
			MOV EAX, [EAX + ���ܱ���ƫ��]
			PUSH dwSkillBook3Panel
			PUSH EAX
			MOV EAX, ���ܼӵ㷢��CALL
			CALL EAX;

			POPAD;
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"AddSkillPoint_Send�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::SwitchSkillPage(_In_ DWORD dwSkillBook3Panel)
{
	__try
	{
		DWORD dwArray[64] = { 0 };
		dwArray[5] = 0x1; // +0x14 = 0x1
		__asm
		{
			PUSHAD;
			MOV EDX, dwSkillBook3Panel;
			LEA ECX, dwArray;
			MOV EAX, �л�����ϵCALL;
			CALL EAX;
			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"SwitchSkillPage�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::ActionCALL(_In_ DWORD dwActionId)
{
	__try
	{
		_asm
		{
			PUSHAD;
			PUSH 0;
			PUSH 0;
			PUSH 0;
			PUSH dwActionId;

			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��2];
			MOV EAX, DWORD PTR DS : [EAX + ������ƫ��];
			PUSH EAX;

			MOV EAX, ����CALL;
			CALL EAX;
			POPAD;
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"ActionCALL�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::TalkTo(_In_ LPCWSTR pwszPlayerName, _In_ LPCWSTR pwszContent)
{
	__try
	{
		//DWORD dwPlayerLen = wcslen(pwszPlayerName);
		//DWORD dwContentLen = wcslen(pwszContent);
		static DWORD dwArray[32] = { 0 };

		LPVOID lpAddr = AllocGameMem(0x30);
		_asm
		{
			MOV EAX, lpAddr
			MOV EBX, 0x5DDD80;
			CALL EBX;

			MOV EBX, pwszContent;
			PUSH EBX;
			LEA ESI, DWORD PTR DS : [EDI + 0x28];
			OR EAX, 0xFFFFFFFF;
			MOV EBX, 0x0C5D4A0;
			CALL EBX;

			MOV EBX, pwszPlayerName;
			PUSH EBX;
			LEA ESI, DWORD PTR DS : [EDI + 0x14];
			OR EAX, 0xFFFFFFFF;
			MOV EBX, 0x0C5D4A0;
			CALL EBX;

			PUSH 0;
			MOV ESI, EAX;
			MOV EAX, ����CALL;
			CALL EAX;
			ADD ESP, 0x4;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"TalkTo�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::InviteTansaction(_In_ DWORD dwPlayerId, _In_ DWORD dwPlayerIdParm)
{
	__try
	{
		_asm
		{
			PUSHAD;
			PUSH dwPlayerIdParm;
			PUSH dwPlayerId;
			MOV EAX, ������CALL;
			CALL EAX;
			POPAD;
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"InviteTansaction�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::PushTransactionItem(_In_ DWORD dwTradeShelfPanelObj, _In_ DWORD dwSId, _In_ DWORD dwCount)
{
	__try
	{
		_asm
		{
			PUSHAD;
			PUSH dwCount;
			PUSH 0;
			PUSH dwSId;
			MOV ECX, dwTradeShelfPanelObj;
			MOV ECX, DWORD PTR DS : [ECX + ���ý�����Ʒƫ��];
			MOV EAX, ���ý�����ƷCALL;
			CALL EAX;
			POPAD;
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"PushTransactionItem�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::LockTransaction()
{
	__try
	{
		_asm
		{
			PUSHAD;
			MOV EAX, �������׿�CALL;
			CALL EAX;
			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"LockTransaction�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::SumitTransaction(_In_ DWORD Addres)
{
	__try
	{
		_asm
		{
			PUSHAD;
			MOV EDI, Addres
			MOV EAX, ����CALL;
			CALL EAX;
			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"SumitTransaction�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::PushTransactionMoney(_In_ DWORD dwMoney)
{
	__try
	{
		_asm
		{
			PUSHAD;
			PUSH 0;
			PUSH dwMoney;
			MOV EAX, ���ý��׽�ǮCALL;
			CALL EAX;
			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"PushTransactionMoney�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::CancelDeal()
{
	__try
	{
		_asm
		{
			PUSHAD;
			MOV EAX, ȡ������CALL;
			CALL EAX;
			POPAD;
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"CancelDeal�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::AcceptDeal()
{
	__try
	{
		_asm
		{
			PUSHAD;
			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��2];
			MOV EBX, ���ܽ���CALL;
			CALL EBX;
			POPAD;
		}
		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"AcceptDeal�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::Escape()
{
	__try
	{
		_asm
		{
			PUSHAD;
			MOV EAX, ����CALL;
			CALL EAX;
			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"CancelDeal�������쳣");
	}
	return FALSE;
}

LPVOID CGameSend::AllocGameMem(_In_ DWORD dwSize)
{
	LPVOID lpAddr = nullptr;
	_asm
	{
		PUSH dwSize;
		MOV EAX, ��Ϸ�����ڴ�CALL;
		CALL EAX;
		ADD ESP, 4;
		MOV lpAddr, EAX
	}
	ZeroMemory(lpAddr, dwSize);
	return lpAddr;
}

DWORD CGameSend::GetItemObj_By_ID(_In_ DWORD dwItemId)
{
	__try
	{
		DWORD dwResult = 0;
		_asm
		{
			PUSHAD;
			PUSH 1;
			PUSH dwItemId;

			MOV EAX, ����ID����OBJ��ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV ECX, DWORD PTR DS : [EAX + ����ID����OBJƫ��1];
			MOV EDX, DWORD PTR DS : [ECX];
			MOV EDX, DWORD PTR DS : [EDX + ����ID����OBJƫ��2];
			CALL EDX;
			MOV dwResult, EAX;
			POPAD;
		}
		return dwResult;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"GetItemObj_By_ID�������쳣");
	}
	return NULL;
}

DWORD CGameSend::GetCurrentCityId()
{
	__try
	{
		DWORD dwCityId = 0;
		_asm
		{
			PUSHAD;
			MOV EAX, ��ȡ��ǰ����IDCALL;
			CALL EAX;
			MOV dwCityId, EAX;
			POPAD;
		}
		return dwCityId;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"GetCurrentCityId�������쳣");
	}
	return NULL;
}

DWORD CGameSend::GetItemType(_In_ DWORD dwItemId)
{
	__try
	{
		DWORD dwValue = 0;

		_asm
		{
			PUSHAD;
			PUSH 1;
			PUSH dwItemId;
			MOV EAX, ��ȡ��Ʒ����CALL;
			CALL EAX;
			ADD ESP, 0x8;
			MOV dwValue, EAX;
			POPAD;
		}
		return dwValue & 0xFF;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"GetItemType�������쳣");
	}
	return 0;
}

DWORD CGameSend::GetItemTypeNameAddr(_In_ DWORD dwParm1, _In_ DWORD dwParm2)
{
	__try
	{
		DWORD dwValue = 0;
		_asm
		{
			PUSHAD;
			PUSH dwParm2;
			PUSH dwParm1;

			MOV EAX, ���ַ��ػ�ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV ECX, DWORD PTR DS : [EAX + ���ַ���ƫ��1];
			MOV EDX, DWORD PTR DS : [ECX];
			MOV EDX, DWORD PTR DS : [EDX + ���ַ���ƫ��2 - 0x4];
			CALL EDX;
			MOV dwValue, EAX;
			POPAD;
		}
		return dwValue;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"GetItemTypeNameAddr�������쳣");
	}
	return NULL;
}

//VOID CGameSend::ConfirmForgotSkill(_In_ DWORD dwUiObj)
//{
//	__try
//	{
//		_asm
//		{
//			PUSHAD;
//			MOV EBX, dwUiObj;
//			PUSH EBX;
//			MOV EAX, ��������ȷ��CALL;
//			CALL EAX;
//			POPAD;
//		}
//	}
//	__except (EXCEPTION_EXECUTE_HANDLER)
//	{
//		LOG_CF_E( L"fnConfirmForgotSkill�����쳣!");
//	}
//}

BOOL CGameSend::AcceptTeamInvite(_In_ DWORD dwObj)
{
	__try
	{
		DWORD dwEsi = dwObj;
		_asm
		{
			PUSHAD;
			MOV ESI, dwEsi;
			MOV EDX, DWORD PTR DS : [ESI];
			MOV EAX, DWORD PTR DS : [EDX + 0x60];
			MOV ECX, ESI;
			CALL EAX;

			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"AcceptInvite�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::CloseGameWindow(_In_ DWORD dwObj)
{
	__try
	{
		_asm
		{
			PUSHAD;
			MOV ECX, dwObj;
			MOV EDX, DWORD PTR DS : [ECX];
			MOV EAX, DWORD PTR DS : [EDX + ����UIƫ��];

			PUSH 1;
			PUSH 0;
			CALL EAX;
			POPAD;
		}

		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"CloseGameWindow�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::CloseGameUi_Task_50Level(_In_ DWORD JumpingLevelPanel)
{
	__try
	{
		_asm
		{
			PUSHAD;
			MOV ECX, JumpingLevelPanel;
			MOV EDX, DWORD PTR DS : [ECX];
			MOV EAX, DWORD PTR DS : [EDX + �˳�50��UIƫ��];
			CALL EAX;
			POPAD;
		}

		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"CloseGameUi_Task_50Level�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::SetShowUi(_In_ DWORD dwObj, _In_ BOOL bShow)
{
	__try
	{
		_asm
		{
			PUSHAD;
			MOV EBX, dwObj;
			MOV EAX, DWORD PTR DS : [EBX];
			MOV EDX, DWORD PTR DS : [EAX + ����UIƫ��];
			PUSH 1;
			PUSH bShow;
			MOV ECX, EBX;
			CALL EDX;
			POPAD;
		}

		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"CloseBaiXianPraticeUi�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::ClickYes(_In_ DWORD dwObj)
{
	__try
	{
		_asm
		{
			PUSHAD;
			MOV EDI, dwObj;
			MOV EDX, DWORD PTR DS : [EDI];
			MOV EAX, DWORD PTR DS : [EDX + 0x5C];
			MOV ECX, EDI;
			CALL EAX;
			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"ClickYes�������쳣");
	}
	return FALSE;
}

DWORD CGameSend::GetSkillParm(_In_ DWORD dwSkillId)
{
	__try
	{
		DWORD dwValue = 0;

		_asm
		{
			PUSHAD;
			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��2];
			MOV ECX, EAX;
			MOV ECX, DWORD PTR DS : [ECX + ���ܱ���ƫ��];
			MOV EBX, 0x100;
			MOV ESI, dwSkillId;
			MOV EAX, ��ȡ���ܲ���CALL;
			CALL EAX;
			MOV dwValue, EDX;
			POPAD;
		}

		return dwValue;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"GetSkillParm�������쳣");
	}
	return NULL;
}

DWORD CGameSend::GetSkillObj(_In_ DWORD dwSkillId, _In_ DWORD dwSkillParm)
{
	__try
	{
		DWORD dwValue = 0;

		_asm
		{
			PUSHAD;
			MOV EBX, dwSkillParm;
			MOV EAX, ���ַ��ػ�ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV ECX, DWORD PTR DS : [EAX + �������ַ���ƫ��1];
			MOV EDI, DWORD PTR DS : [ECX];
			MOV EAX, DWORD PTR DS : [EDI + ���ַ���ƫ��2];
			PUSH EBX;
			PUSH dwSkillId;
			CALL EAX;
			MOV dwValue, EAX;
			POPAD;
		}
		return dwValue;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"GetSkillObj�������쳣");
	}
	return NULL;
}

DWORD CGameSend::QueryTaskScheduleCount(_In_ DWORD dwTaskId)
{
	__try
	{
		DWORD dwValue = 0;
		_asm
		{
			PUSHAD;
			PUSH dwTaskId;
			MOV EAX, ��ǰ����ɱ������CALL;
			CALL EAX;
			MOV dwValue, EAX;
			POPAD;
		}
		return dwValue;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"QueryTaskScheduleCount�������쳣");
	}
	return 0;
}

BOOL CGameSend::OpenGiftBox(_In_ DWORD dwGiftId)
{
	__try
	{
		_asm
		{
			PUSHAD;
			PUSH 0;
			PUSH dwGiftId;
			MOV EAX, ������CALL;
			CALL EAX;
			POPAD;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"OpenGiftBox�������쳣");
	}
	return TRUE;
}

BOOL CGameSend::RecvGift(_In_ DWORD dwItemId, _In_ DWORD dwGiftParm, _In_ DWORD dwGiftID, _In_ DWORD dwBoxId)
{
	__try
	{
		_asm
		{
			PUSHAD;
			PUSH dwItemId;
			PUSH dwGiftParm;
			PUSH dwGiftID;
			PUSH 0;
			PUSH dwBoxId;

			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + ��������ƫ��];
			MOV EDI, DWORD PTR DS : [EAX + 0x10];

			MOV EBX, �����CALL;
			CALL EBX;
			POPAD;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"RecvGift�������쳣");
	}
	return TRUE;
}

BOOL CGameSend::SecondPickup(_In_ DWORD dwId, _In_ DWORD dwParm)
{
	__try
	{
		_asm
		{
			PUSHAD;
//#ifdef BnsProject_TaiWan
//			PUSH 0;
//#endif // BnsProject_TaiWan
			PUSH dwParm;
			PUSH dwId;
			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��2];
#ifdef BnsProject_Korea
			MOV EAX, DWORD PTR DS : [EAX + ������ȴƫ��];
			MOV ESI, DWORD PTR DS : [EAX + 0x4];
			PUSH ESI;
			MOV EAX, ���μ���CALL;
			CALL EAX;
			POPAD;
#endif // BnsProject_Korea
#ifdef BnsProject_TaiWan
			MOV ESI, DWORD PTR DS : [EAX + ������ȴƫ��];
			PUSH ESI;
			MOV EAX, ���μ���CALL;
			CALL EAX;
			POPAD;
#endif // BnsProject_TaiWan
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"SecondPickup�������쳣");
	}
	return TRUE;
}

DWORD CGameSend::GetLevelTickObj()
{
	__try
	{
		DWORD dwValue = 0;
		_asm
		{
			PUSHAD;
			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + 0x58];
			MOV EAX, DWORD PTR DS : [EAX + 0x10];
			PUSH 6;
			PUSH EAX;
			MOV EAX, ��ȡ������CALL;
			CALL EAX;
			MOV dwValue, EAX;
			POPAD;
		}
		return dwValue;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"GetLevelTickObj�������쳣");
	}
	return NULL;
}

BOOL CGameSend::UseKfmVolume1()
{
	__try
	{
		//DWORD dwValue = GetLevelTickObj();
		_asm
		{
			PUSHAD;
			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + 0x58];
			MOV EAX, DWORD PTR DS : [EAX + 0x10];
			PUSH 6;
			PUSH EAX;
			MOV EAX, ��ȡ������CALL;
			CALL EAX;
			TEST EAX, EAX;
			JE lb_Exit;
			MOV ECX, DWORD PTR DS : [EAX + 0x4];
			MOV EDX, DWORD PTR DS : [EAX];
			PUSH ECX;
			PUSH EDX;
			MOV EAX, ʹ�����ڄ�CALL1;
			CALL EAX;
		lb_Exit:;
			POPAD;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"UseKfmVolume1�������쳣");
	}
	return TRUE;
}

BOOL CGameSend::UseKfmVolume2()
{
	__try
	{
		_asm
		{
			PUSHAD;
			MOV EAX, ʹ�����ڄ�CALL2;
			CALL EAX;
			POPAD;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"UseKfmVolume2�������쳣");
	}
	return TRUE;
}

BOOL CGameSend::RemoveTeamMember(_In_ DWORD dwIndex)
{
	__try
	{
		_asm
		{
			PUSHAD;
			PUSH dwIndex;
			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��2];
			MOV EAX, DWORD PTR DS : [EAX + ��������ƫ��];
			PUSH EAX;
			MOV EAX, �߳���ԱCALL;
			CALL EAX;
			POPAD;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"RemoveTeamMember�������쳣");
	}
	return TRUE;
}

BOOL CGameSend::InitializeCatForSummoner(_In_ DWORD dwNpcId, _In_ DWORD dwNpcIdParm, _In_ LPCWSTR wsCatName, _In_ DWORD dwCatParamter)
{
	__try
	{
		LPVOID lpAddr = AllocGameMem(0x28);

		_asm
		{
			PUSHAD;
			MOV EDI, lpAddr;

			MOV EBX, �ٻ���è��������2;
			MOV DWORD PTR DS : [EDI + 0x0], EBX;
			MOV EBX, �ٻ���è��������1;
			MOV DWORD PTR DS : [EDI + 0x4], EBX;
			MOV EBX, dwCatParamter;
			MOV DWORD PTR DS : [EDI + 0x10], EBX;
			MOV EBX, dwNpcId;
			MOV DWORD PTR DS : [EDI + 0x18], EBX;
			MOV EBX, dwNpcIdParm;
			MOV DWORD PTR DS : [EDI + 0x1C], EBX;
			
			// PUSH Name
			LEA EBX, wsCatName;
			MOV EBX, DWORD PTR DS : [EBX];
			PUSH EBX;

			// Set Name for lpAddr
			LEA ESI, DWORD PTR DS : [EDI + 0x20];
			OR EAX, 0xFFFFFFFF;
			MOV EBX, �ٻ���èCALL;
			CALL EBX;

			PUSH 0;
			MOV ESI, EDI;

			MOV EAX, ����CALL;
			CALL EAX;

			ADD ESP, 0x4;
			POPAD;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"InitializeCatForSummoner�������쳣");
	}
	return TRUE;
}

BOOL CGameSend::InitializeCatForSummoner(_In_ DWORD JumpingLevelPanelBase, _In_ DWORD dwIndex, _In_ LPCWSTR wsCatName)
{
	__try
	{
		LPVOID lpAddr = AllocGameMem(0x28);
		DWORD dwParm1 = ReadDWORD(JumpingLevelPanelBase + �ٻ���èƫ��1) & 0xFF;
		DWORD dwParm2 = ReadDWORD(JumpingLevelPanelBase + �ٻ���èƫ��2) & 0xFFFF;
		_asm
		{
			PUSHAD;
			MOV EDI, lpAddr;

			MOV EBX, �ٻ���è��������2;
			MOV DWORD PTR DS : [EDI + 0x0], EBX;
			MOV EBX, �ٻ���è��������1;
			MOV DWORD PTR DS : [EDI + 0x4], EBX;
			MOV EBX, dwParm1;
			MOV BYTE PTR DS : [EDI + 0x12], BL;
			MOV EBX, dwParm2;
			MOV WORD PTR DS : [EDI + 0x10], BX;
			MOV EBX, dwIndex;
			MOV BYTE PTR DS : [EDI + 0x13], BL;

			// PUSH Name
			LEA EBX, wsCatName;
			MOV EBX, DWORD PTR DS : [EBX];
			PUSH EBX;

			// Set Name for lpAddr
			LEA ESI, DWORD PTR DS : [EDI + 0x20];
			OR EAX, 0xFFFFFFFF;
			MOV EBX, �ٻ���èCALL;
			CALL EBX;

			PUSH 0;
			MOV ESI, EDI;

			MOV EAX, ����CALL;
			CALL EAX;

			ADD ESP, 0x4;
			POPAD;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"InitializeCatForSummoner�������쳣");
	}
	return TRUE;
}

BOOL CGameSend::UseSkillByCat(_In_ DWORD dwSkillId)
{
	__try
	{
		BOOL bResult = FALSE;
		__asm
		{
			PUSHAD;
			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��2 + 0x4];
			MOV ESI, DWORD PTR DS : [EAX + ������ȴƫ��];
			MOV ECX, ESI;

			MOV EDX, dwSkillId;
			MOV EAX, �ٻ�è�ͷż���CALL;
			CALL EAX;
			MOV bResult, EAX;
			POPAD;
		}
		return (bResult & 0xFF) == 0x1 ? TRUE : FALSE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"UseSkillByCat�������쳣");
	}
	return TRUE;
}

BOOL CGameSend::AcceptPrizesForBaiXianPracticeField(_In_ DWORD dwOpponentClasses, _In_ DWORD dwPrizesIndex)
{
	__try
	{
		BOOL bResult = FALSE;
		__asm
		{
			PUSHAD;
			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV ESI, DWORD PTR DS : [EAX + ����������ƫ��3];
			MOV EAX, ESI;

			PUSH dwPrizesIndex;
			PUSH dwOpponentClasses;
			MOV ECX, ������������ȡ����CALL;
			CALL ECX;
			MOV bResult, EAX;
			POPAD;
		}
		return (bResult & 0xFF) == 0x1 ? TRUE : FALSE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"AcceptPrizesForBaiXianPracticeField�������쳣");
	}
	return TRUE;
}

BOOL CGameSend::BeckonHeroForBaiXianPracticeField(_In_ DWORD dwDuelBotTrainingSelectJobPanel, _In_ DWORD dwHeroIndex, _In_ DWORD dwStageIndex)
{
	__try
	{
		BOOL bResult = FALSE;
		__asm
		{
			PUSHAD;
			MOV ESI, dwDuelBotTrainingSelectJobPanel;
			MOV EAX, dwHeroIndex;
			MOV DWORD PTR DS : [ESI + ����������ƫ��1], EAX;
			MOV EAX, dwStageIndex;
			MOV DWORD PTR DS : [ESI + ����������ƫ��2], EAX;
			MOV EAX, 0x0;
			MOV DWORD PTR DS : [ESI + ����������ƫ��4], EAX;

			MOV ECX, �����������ٻ�Ӣ��CALL;
			CALL ECX;
			MOV bResult, EAX;
			POPAD;
		}
		return (bResult & 0xFF) == 0x1 ? TRUE : FALSE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"BeckonHeroForBaiXianPracticeField�������쳣");
	}
	return TRUE;
}

BOOL CGameSend::ExitBaiXianPracticeField()
{
	__try
	{
		BOOL bResult = FALSE;
		__asm
		{
			PUSHAD;
			MOV EAX, �����������˳�CALL;
			CALL EAX;
			MOV bResult, EAX;
			POPAD;
		}
		return (bResult & 0xFF) == 0x1 ? TRUE : FALSE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"ExitBaiXianPracticeField�������쳣");
	}
	return TRUE;
}

BOOL CGameSend::TurntableBySpaceTimeTurret(_In_ DWORD dwIndex)
{
	__try
	{
		BOOL bResult = FALSE;
		__asm
		{
			PUSHAD;
			PUSH dwIndex;
			MOV EAX, ʱ��֮��ת��CALL;
			CALL EAX;
			MOV bResult, EAX;
			POPAD;
		}
		return (bResult & 0xFF) == 0x1 ? TRUE : FALSE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"TurntableBySpaceTimeTurret�������쳣");
	}
	return TRUE;
}

BOOL CGameSend::AcceptPrizesForSpaceTimeTurret()
{
	__try
	{
		BOOL bResult = FALSE;
		__asm
		{
			PUSHAD;
			MOV EAX, ʱ��֮����ȡ����CALL;
			CALL EAX;
			MOV bResult, EAX;
			POPAD;
		}
		return (bResult & 0xFF) == 0x1 ? TRUE : FALSE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"AcceptPrizesForSpaceTimeTurret�������쳣");
	}
	return TRUE;
}

BOOL CGameSend::SetAngle(_In_ DWORD dwAngle)
{
	__try
	{
		BOOL bResult = FALSE;
		LPVOID lpAddr = AllocGameMem(0x14);
		__asm
		{
			PUSHAD;
			MOV EAX, lpAddr;
			MOV EBX, ���ýǶȷ�������1;
			MOV WORD PTR DS : [EAX + 0x4], BX;
			MOV EBX, ���ýǶȷ�������2;
			MOV DWORD PTR DS : [EAX + 0x0], EBX;
			MOV EBX, dwAngle;
			MOV WORD PTR DS : [EAX + 0x12], BX;
			PUSH 0;
			MOV ESI, EAX;
			MOV EAX, ����CALL;
			CALL EAX;
			ADD ESP, 0x4;
			POPAD;
		}
		return (bResult & 0xFF) == 0x1 ? TRUE : FALSE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"UseContinuedSkillByCat�������쳣");
	}
	return TRUE;
}

BOOL CGameSend::AddPlayerAttribute(_In_ DWORD dwPowerAttribute, _In_ DWORD dwHpAttributeCount)
{
	LOG_CF_D( L"UseContinuedSkillByCat-%x,%d", dwPowerAttribute, dwHpAttributeCount);
	__try
	{
		BOOL bResult = FALSE;
		//LPVOID lpAddr = AllocGameMem(0x14);
		LPVOID lpAddr = AllocGameMem(0x1C);
		__asm
		{
			PUSHAD;
			MOV EAX, lpAddr;
			MOV EBX, �����Ե㷢������1;
			MOV WORD PTR DS : [EAX + 0x4], BX;
			MOV EBX, �����Ե㷢������2;
			MOV DWORD PTR DS : [EAX + 0x0], EBX;
			MOV EBX, dwPowerAttribute;
			MOV WORD PTR DS : [EAX + 0x10], BX;
			MOV EBX, dwHpAttributeCount;
			MOV WORD PTR DS : [EAX + 0x12], BX;

			PUSH 0;
			MOV ESI, EAX;
			MOV EAX, ����CALL;
			CALL EAX;
			ADD ESP, 0x4;
			POPAD;
		}
		return (bResult & 0xFF) == 0x1 ? TRUE : FALSE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"UseContinuedSkillByCat�������쳣");
	}
	return TRUE;
}

BOOL CGameSend::IntoRoom(_In_ DWORD TotalGateConfirmPanelBase, _In_ em_Room_Type emRoom)
{
	__try
	{
		if (emRoom == em_Room_Type::em_Room_Type_PK)
			MyTools::CCharacter::WriteDWORD(TotalGateConfirmPanelBase + ���븱���ռ�ƫ��, 0x2);
		else
			MyTools::CCharacter::WriteDWORD(TotalGateConfirmPanelBase + ���븱���ռ�ƫ��, 0x3);

		__asm PUSHAD;
		if (emRoom == em_Room_Type::em_Room_Type_PK)
			__asm PUSH 1;
		else
			__asm PUSH 2;

		_asm
		{
			PUSH TotalGateConfirmPanelBase;
			MOV EAX, ���븱���ռ�CALL;
			CALL EAX;
			POPAD;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"IntoRoom�������쳣");
	}
	return TRUE;
}

BOOL CGameSend::SummonerTakeGift(_In_ DWORD JumpingLevelPanelBase)
{
	__try
	{
		__asm
		{
			PUSHAD;
			MOV EDI, JumpingLevelPanelBase;
			MOV EAX, �ٻ�������CALL;
			CALL EAX;
			POPAD;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"SummonerTakeGift�������쳣");
	}
	return TRUE;
}

BOOL CGameSend::LeaveSkillTrainRoom()
{
	__try
	{
		DWORD dwAddr = ReadDWORD(ReadDWORD(ReadDWORD(�����ַ) + �����ַƫ��1) + ����ѵ����ƫ��);
		if (dwAddr == NULL)
			return FALSE;

		__asm
		{
			PUSHAD;
			PUSH dwAddr;
			MOV EAX, �뿪����ѵ����CALL;
			CALL EAX;
			POPAD;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"LeaveSkillTrainRoom�������쳣");
	}
	return TRUE;
}

BOOL CGameSend::LeaveSkillTrainRoom_Confirm()
{
	__try
	{
		__asm
		{
			PUSHAD;
			MOV EAX, �뿪��������CALL;
			CALL EAX;
			POPAD;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"LeaveSkillTrainRoom_Confirm�������쳣");
	}
	return TRUE;
}

BOOL CGameSend::UseTicket_By_Level50()
{
	/*__try
	{
	__asm
	{
	PUSHAD
	MOV EAX, �����ַ;
	MOV EAX, DWORD PTR DS: [EAX];
	MOV EAX, DWORD PTR DS: [EAX + ʹ��50����ƫ��1];
	MOV EAX, DWORD PTR DS: [EAX + ʹ��50����ƫ��2];
	PUSH 6;
	PUSH EAX;
	MOV EAX, ʹ��50����CALL1;
	CALL EAX;
	MOV ESI, EAX;
	MOV EAX, DWORD PTR DS: [ESI + 4];
	MOV ECX, DWORD PTR DS: [ESI];
	PUSH EAX;
	PUSH ECX;
	MOV EAX, ʹ��50����CALL2;
	CALL EAX;
	POPAD
	}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
	LOG_CF_E( L"UseTicket_By_Level50�������쳣");
	}*/
	return TRUE;
}

BOOL CGameSend::Talk(_In_ LPCWSTR pwszText, _In_ DWORD ChatInputPanelBase)
{
	__try
	{
		static WCHAR wszTalkText[1024] = { 0 };
		MyTools::CCharacter::wstrcpy_my(wszTalkText, pwszText, _countof(wszTalkText) - 1);

		__asm
		{
			PUSHAD;
			LEA EAX, wszTalkText;
			PUSH EAX;
			PUSH ChatInputPanelBase;
			MOV EAX, ����CALL;
			CALL EAX;
			POPAD;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"Talk�������쳣");
	}
	return TRUE;
}

BOOL CGameSend::InputOTP(_In_ LPCWSTR pwszOtpText)
{
	__try
	{
		static WCHAR wszText[32];
		MyTools::CCharacter::wstrcpy_my(wszText, pwszOtpText);
		__asm
		{
			PUSHAD;
			LEA ECX, wszText;
			MOV EAX, ����OTPCALL;
			CALL EAX;
			POPAD;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"InputOTP�������쳣");
	}
	return TRUE;
}

BOOL CGameSend::Resurrect()
{
	__try
	{
		BOOL bResult = FALSE;
		LPVOID lpAddr = AllocGameMem(0x14);
		__asm
		{
			PUSHAD;
			MOV EAX, lpAddr;

			MOV EBX, ���������1;
			MOV WORD PTR DS : [EAX + 0x4], BX;

			MOV EBX, 0;
			MOV DWORD PTR DS : [EAX + 0x8], EBX;
			MOV BYTE PTR DS : [EAX + 0xC], BL;

			MOV EBX, ���������2;
			MOV DWORD PTR DS : [EAX], EBX;

			MOV ECX, 0;
			MOV DWORD PTR DS : [EAX + 0x8], ECX;
			MOV BYTE PTR DS : [EAX + 0xC], CL;

			PUSH 0;
			MOV ESI, EAX;

			MOV CL, 0x0;
			MOV BYTE PTR DS : [EAX + 0x10], CL;
			MOV EAX, ����CALL;
			CALL EAX;
			ADD ESP, 0x4;

			MOV bResult, EAX;
			POPAD;
		}
		return (bResult & 0xFF) == 0x1 ? TRUE : FALSE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"Resurrect�������쳣");
	}
	return TRUE;
}

VOID CGameSend::ReceiveLvPrize()
{
	__try
	{
		__asm
		{
			PUSHAD;
			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��2];
			MOV ECX, DWORD PTR DS : [EAX + ��ȡ���ƫ��];
			PUSH 0xFFFFFFFF;
			MOV EAX, ��ȡ���CALL;
			CALL EAX;
			POPAD;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"ReceiveLvPrize�������쳣");
	}
}

BOOL CGameSend::TakeStarCoin(_In_ CONST StarCoinContent& StarCoinContent_)
{
	__try
	{
		DWORD dwParm1 = static_cast<DWORD>(StarCoinContent_.wParm1);
		DWORD dwParm2 = static_cast<DWORD>(StarCoinContent_.wParm2);

		BOOL bResult = FALSE;
		LPVOID lpAddr = AllocGameMem(0x14);
		__asm
		{
			PUSHAD;
			MOV EAX, lpAddr;

			MOV EBX, ��ȡ�Ǳҷ�������1;
			MOV WORD PTR DS : [EAX + 0x4], BX;

			MOV EBX, 0;
			MOV DWORD PTR DS : [EAX + 0x8], EBX;
			MOV BYTE PTR DS : [EAX + 0xC], BL;

			MOV EBX, ��ȡ�Ǳҷ�������2;
			MOV DWORD PTR DS : [EAX], EBX;

			MOV ECX, 0;
			MOV DWORD PTR DS : [EAX + 0x8], ECX;
			MOV BYTE PTR DS : [EAX + 0xC], CL;

			PUSH 0;
			MOV ESI, EAX;

			MOV ECX, dwParm1;
			MOV WORD PTR DS : [EAX + 0x10], CX;

			MOV ECX, dwParm2;
			MOV WORD PTR DS : [EAX + 0x12], CX;

			MOV EAX, ����CALL;
			CALL EAX;
			ADD ESP, 0x4;

			MOV bResult, EAX;
			POPAD;
		}
		return (bResult & 0xFF) == 0x1 ? TRUE : FALSE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"TakeStarCoin�������쳣");
	}
	return TRUE;
}

BOOL CGameSend::ShopStarCoinStore(_In_ DWORD dwItemId, _In_ DWORD dwGoodsStorePanel)
{
	//
	__try
	{
		DWORD dwArray[3] = { 0 };
		dwArray[0] = dwItemId;
		dwArray[1] = 0x47;
		dwArray[2] = 0x101;

		__asm
		{
			PUSHAD;

			LEA EAX, dwArray;
			PUSH EAX;
			PUSH dwGoodsStorePanel;

			MOV EAX, �Ǳ��̵깺��CALL;
			CALL EAX;

			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"ShopStarCoinStore�������쳣");
	}
	return TRUE;
}

BOOL CGameSend::SetDifficultyForTeam(_In_ emTeamDifficulty emDifficulty)
{
	__try
	{
		DWORD dwValue = static_cast<DWORD>(emDifficulty);
		__asm
		{
			PUSHAD;

			PUSH dwValue;
			MOV EAX, ����Ѷ�CALL;
			CALL EAX;

			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"SetDifficultyForTeam�������쳣");
	}
	return TRUE;
}

BOOL CGameSend::StrengthenBaGua(_In_ DWORD dwEquiSid, _In_ DWORD dwSacrificeSid)
{
	__try
	{
		__asm
		{
			PUSHAD;

			PUSH dwSacrificeSid;
			PUSH 0;
			PUSH dwEquiSid;

			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��2];
			MOV ECX, DWORD PTR DS : [EAX + ��������ƫ��];

			MOV EAX, ����ǿ��CALL;
			CALL EAX;

			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"SetDifficultyForTeam�������쳣");
	}
	return TRUE;
}

DWORD CGameSend::GetBaGuaAttribute(_In_ DWORD dwId)
{
	__try
	{
		DWORD dwValue = 0;
		__asm
		{
			PUSHAD;

			MOV EAX, ���ַ��ػ�ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV ECX, DWORD PTR DS : [EAX + ��������ƫ��];
			MOV ESI, DWORD PTR DS : [ECX];
			MOV EDX, DWORD PTR DS : [ESI + ��������CALLƫ��];

			PUSH 0;
			PUSH dwId;
			CALL EDX;
			MOV dwValue, EAX;
			POPAD;
		}
		return dwValue;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"GetBaGuaAttribute�������쳣");
	}
	return NULL;
}

BOOL CGameSend::CreatePassWorkRoom(_In_ DWORD dwPass)
{
	__try
	{
		//LOG_CF_D( L"dwArray[2]=%X", dwArray[2]);
		LPVOID lpAddr = AllocGameMem(0x14);
		_asm
		{
			PUSHAD;
			
			MOV EAX, lpAddr;

			MOV EBX, �����������䷢������1;
			MOV DWORD PTR DS : [EAX + 0x0], EBX;

			MOV EBX, �����������䷢������2;
			MOV DWORD PTR DS : [EAX + 0x4], EBX;

			MOV EBX, 0x2;
			MOV DWORD PTR DS : [EAX + 0x8], EBX;

			MOV EBX, dwPass;
			MOV WORD PTR DS : [EAX + 0x10], BX;

			PUSH 0;
			MOV ESI, EAX;
			MOV EAX, ����CALL;
			CALL EAX;
			ADD ESP, 0x4;
			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"CreatePassWorkRoom�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::FindDungoenRoom_By_Id(_In_ DWORD dwId, _In_ DWORD dwPass)
{
	__try
	{
		LPVOID lpAddr = AllocGameMem(0x28);
		_asm
		{
			PUSHAD;

			MOV EAX, lpAddr;

			MOV EBX, Ѱ�ҷ��䷢������1;
			MOV DWORD PTR DS : [EAX + 0x0], EBX;

			MOV EBX, Ѱ�ҷ��䷢������2;
			MOV DWORD PTR DS : [EAX + 0x4], EBX;

			MOV DWORD PTR DS : [EAX + 0x8],  0x2;

			MOV EBX , dwId;
			MOV DWORD PTR DS : [EAX + 0x10], EBX;

			MOV EBX, dwPass;
			MOV DWORD PTR DS : [EAX + 0x20], EBX;

			PUSH 0;
			MOV ESI, EAX;
			MOV EAX, ����CALL;
			CALL EAX;
			ADD ESP, 0x4;
			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"FindDungoenRoom_By_Id�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::IsRoomReady(_In_ DWORD dwRoomBase)
{
	__try
	{
		DWORD dwValue = 0;
		_asm
		{
			PUSHAD;
			MOV ECX, dwRoomBase;
			MOV EAX, �����Ƿ�׼��CALL;
			CALL EAX;
			MOV EBX, dwValue;
			MOV BYTE PTR DS : [EBX], AL;
			POPAD;
		}
		return dwValue == 1 ? TRUE : FALSE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"IsRoomReady�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::ReadyForGungoenRoom(_In_ DWORD dwRoomBase)
{
	__try
	{
		_asm
		{
			PUSHAD;
			PUSH 0;
			MOV EBX, dwRoomBase;
			MOV EAX, ����׼��CALL;
			CALL EAX;
			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"ReadyForGungoenRoom�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::IsRoomReadyForCaptain(_In_ DWORD dwRoomBase)
{
	__try
	{
		DWORD dwValue = 0;
		_asm
		{
			PUSHAD;
			MOV ECX, dwRoomBase;
			MOV EAX, �ӳ��Ƿ�׼��CALL;
			CALL EAX;
			MOV EBX, dwValue;
			MOV BYTE PTR DS : [EBX], AL;
			POPAD;
		}
		return dwValue == 1 ? TRUE : FALSE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"IsRoomReadyForCaptain�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::ChoiceGungeonMap(_In_ DWORD dwMapId, _In_ DWORD dwRoomBase)
{
	__try
	{
		_asm
		{
			PUSHAD;
			MOV EAX, dwMapId;
			MOV EBX, dwRoomBase;
			MOV ECX, ѡ�񸱱�CALL;
			CALL ECX;
			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"ChoiceGungeonMap�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::ChoiceGungeonDifficulty(_In_ DWORD dwValue, _In_ DWORD dwRoomBase)
{
	__try
	{
		_asm
		{
			PUSHAD;
			
			PUSH dwValue;
			MOV EDI, dwRoomBase;
			MOV EAX, ѡ�񸱱��Ѷ�CALL;
			CALL EAX;

			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"ChoiceGungeonMap�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::BackToCrossAreaTeamMainForm()
{
	__try
	{
		_asm
		{
			PUSHAD;

			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + ��������ƫ��3];
			MOV EAX, DWORD PTR DS : [EAX + ��������ƫ��2];

			PUSH 0;
			PUSH EAX;

			MOV EAX, �˻ؿ����ӽ���CALL;
			CALL EAX;

			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"BackToCrossAreaTeamMainForm�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::ExitCrossAreaTeamRoom()
{
	__try
	{
		_asm
		{
			PUSHAD;

			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + ��������ƫ��3];
			MOV EAX, DWORD PTR DS : [EAX + ��������ƫ��2];

			MOV ECX, �˳������ӷ���CALL;
			CALL ECX;

			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"ExitCrossAreaTeamRoom�������쳣");
	}
	return FALSE;
}

BOOL CGameSend::ExitCrossAreaTeamMainForm()
{
	__try
	{
		_asm
		{
			PUSHAD;

			MOV EAX, �����ַ;
			MOV EAX, DWORD PTR DS : [EAX];
			MOV EAX, DWORD PTR DS : [EAX + �����ַƫ��1];
			MOV EAX, DWORD PTR DS : [EAX + ��������ƫ��3];
			MOV EAX, DWORD PTR DS : [EAX + ��������ƫ��2];

			MOV ECX, �˳������ӽ���CALL;
			CALL ECX;

			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"ExitCrossAreaTeamMainForm�������쳣");
	}
	return FALSE;
}