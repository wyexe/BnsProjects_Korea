#include "stdafx.h"
#include "GameFunction.h"
#include <MyTools/CLHook.h>
#include <MyTools/CLFile.h>
#include <MyTools/CLSearchBase.h>
#include <MyTools/CLProcess.h>
#include <MyTools/CLThread.h>
#include <MyTools/Log.h>
#include "NpcExtend.h"
#include "FarmBoss.h"
#include "PersonFunction.h"
#include "FarmSkill.h"
#include "BagFunction.h"
#include "GameSend.h"
#include "Person.h"
#include "GameUI.h"
#include "ObjectFunction.h"
#include "SomeFun.h"
#include "BnsConfig.h"
#include "Skill.h"
#include "BnsHook.h"

#define _SELF L"GameFunction.cpp"

//#pragma optimize( "HookSpeed", off )
int WINAPI HookSpeed(float f, int param)
{
	__asm
	{
		mov ebx, g_dwMoveBase;
		mov ebx, [ebx];
		mov ebx, [ebx + 人物坐标偏移1];
		mov ebx, [ebx];
		mov ebx, [ebx + 人物坐标偏移2];
		mov ebx, [ebx + 人物坐标偏移3];
		cmp ebx, ecx;
		jne EXIT;
		;
		lea ebx, f;
		fld[ebx];
		lea eax, g_fSpeed;
		fmul[eax];
		fstp[ebx];
	EXIT:;
		push param;
		push f;
		call g_dwSpeedCALL;
	}
}
//#pragma optimize( "HookSpeed", on )

void CALLBACK CGameFunction::TimerProc(HWND, UINT, UINT nTimerid, DWORD)
{
	if (nTimerid != 0x1000 && nTimerid != 0x1001)
		return;

	if (CPersonFunction::GetInstance().IsGameLoad())
		return;

	if (CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.bAccelerate)
	{
#if ENABLE_LOCAL_SPEED
#ifndef _DEBUG
		if (g_dwHookSpeedAddr != ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(g_dwMoveBase) + 人物坐标偏移1) + 0x0) + 人物坐标偏移2) + 人物坐标偏移3) + 0x0) + 0x178)
		{
			LOG_CF_E(L"重新加速!");
			g_dwHookSpeedAddr = ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(g_dwMoveBase) + 人物坐标偏移1) + 0x0) + 人物坐标偏移2) + 人物坐标偏移3) + 0x0) + 0x178;
			g_dwSpeedCALL = ReadDWORD(g_dwHookSpeedAddr);
			MyTools::CCharacter::WriteDWORD(g_dwHookSpeedAddr, (DWORD)HookSpeed);
		}
#endif
#elif ENABLE_GLOAB_SPEED
		static DWORD dwGloabSpeedBase = NULL;
		if (dwGloabSpeedBase == NULL)
		{
			dwGloabSpeedBase = MyTools::CLSearchBase::FindBase("83a6??????????8d4c24??c74424??????????e8", 0x02C3A8B1 - 0x02C3A8CB, 0x2, 0x0, L"bsengine_Shipping.dll");
			if (dwGloabSpeedBase == NULL)
			{
				LOG_MSG_CF(L"SetGloabSpeed 失败 dwBase = NULL!");
				return;
			}
		}
		if (ReadFloat(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(dwGloabSpeedBase) + 全局加速偏移1) + 全局加速偏移2) + 0) + 全局加速偏移3) != g_fSpeed)
		{
#ifndef _DEBUG
			DWORD dwAddr = ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(dwGloabSpeedBase) + 全局加速偏移1) + 全局加速偏移2) + 0) + 全局加速偏移3;
			MyTools::CCharacter::WriteFloat(dwAddr, g_fSpeed);
#endif
		}
#endif
	}

	CGameVariable::GetInstance().DoList_MainThreadFunPtr([](_In_ std::queue<CGameVariable::MainThreadFunPtrInfo>& vQueue)
	{
		if (!vQueue.empty())
		{
			auto& FunPtr = vQueue.front();
			FunPtr.MainThreadFunPtr_();
			if (FunPtr.hEvent != NULL)
				::SetEvent(FunPtr.hEvent);
			vQueue.pop();
		}
	});
}

BOOL CALLBACK CGameFunction::EnumSetWinName(HWND hLauncher, LPARAM lpParam)
{
	if (IsWindow(hLauncher) && IsWindowVisible(hLauncher) && (GetWindowLongW(hLauncher, GWL_EXSTYLE)&WS_EX_TOOLWINDOW) != WS_EX_TOOLWINDOW && (GetWindowLong(hLauncher, GWL_HWNDPARENT) == 0))
	{
		WCHAR wszWinText[64] = { 0 };
		WCHAR wszWinClass[64] = { 0 };
		
		if (GetClassNameW(hLauncher, wszWinClass, _countof(wszWinClass) / sizeof(WCHAR)) > 0 && GetWindowTextW(hLauncher, wszWinText, _countof(wszWinText)) > 0)
		{
			if (MyTools::CCharacter::wstrcmp_my(wszWinClass, L"LaunchUnrealUWindowsClient"))//如果能遍历到这个,并且是
			{
				DWORD PID;
				::GetWindowThreadProcessId(hLauncher, &PID);
				if (PID == ::GetCurrentProcessId())
				{
					LOG_CF_D( L"成功枚举到本窗口");
					*reinterpret_cast<HWND*>(lpParam) = hLauncher;
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}

static BOOL  gs_bGetDianKa = FALSE;
static DWORD gs_dwDianKaDay = 0;
static DWORD gs_dwDianKaHour = 0;
static DWORD gs_dwDianKaMinute = 0;
__declspec(naked) VOID WINAPI HookDianKa()
{
	__asm MOV EDI, ESP
	__asm PUSHAD
	_asm
	{
		MOV gs_dwDianKaMinute, EAX
			MOV EAX, DWORD PTR DS : [EDI + 0x1C]
			MOV gs_dwDianKaDay, EAX
			MOV EAX, DWORD PTR DS : [EDI + 0x14]
			MOV gs_dwDianKaHour, EAX
			MOV gs_bGetDianKa, 1
	}

	_asm
	{
		POPAD
			TEST ESI, ESI
			MOV EBP, EAX
			MOV EDI, EDX
	}
	__asm PUSH HookDianKaAddr + 0x6
	__asm RETN
}

__declspec(naked) VOID WINAPI HookD3d()
{
	__asm PUSHAD
	::Sleep(g_dwOptimizationTime);
	__asm POPAD
	__asm PUSH g_dwOldD3dCall
	__asm RETN
}



__declspec(naked) VOID WINAPI fnHookMonsterSkill()
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
		CGameFunction::AddMonsterSkill(dwEBX, ReadDWORD(dwESI + 0x10));

	__asm POPAD;
	__asm PUSH HookMonsterAddr + 0x17
	__asm RETN
}

VOID CGameFunction::AddMonsterSkill(_In_ DWORD dwMonsterBase, _In_ DWORD dwSkillId)
{
	if (dwMonsterBase != CFarmBoss::GetInstance().GetHookMonsterBase())
		return;

	if (CFarmBoss::GetInstance().IsHookByDlg())
	{
		
		DWORD dwResAddr = ReadDWORD(ReadDWORD(dwMonsterBase + 技能冷却偏移) + 0x8);
		dwResAddr = dwResAddr == 0 ? 0 : dwResAddr + 0xD0;
		if (dwResAddr != NULL && ReadDWORD(dwResAddr + 0x4) != 0xFFFFFFFF && ReadDWORD(ReadDWORD(dwResAddr) + 0x8) == dwSkillId)
		{
			CFarmBoss::MonsterSkillContent MonsterSkillContent_;
			MonsterSkillContent_.dwSkillId = dwSkillId;
			MonsterSkillContent_.dwResAddr = ReadDWORD(dwResAddr);
			CFarmBoss::GetInstance().AddBossSkillByQue(MonsterSkillContent_);
		}
		return;
	}

	CONST auto& DisableSkillSet = CFarmBoss::GetInstance().GetDisableBossSkillSet();
	if (DisableSkillSet.find(dwSkillId) != DisableSkillSet.end())
		return;

	static DWORD dwSameSkillIdCount = 0;
	static DWORD dwLastSkillId = 0;
	//5902266这个技能是雪人洞吸热BUFF
	if (dwSkillId == 0 || dwSkillId < 3000000 || dwSkillId > 6500000 || dwSkillId == 5902266 || dwSkillId == 5902217 ||
		dwSkillId == 5902218 || dwSkillId == 5902219 || dwSkillId == 5902216 || dwSkillId == 5902212 || dwSkillId == 5902215 ||
		dwSkillId == 5902211 || dwSkillId == 5902220 || dwSkillId == 5902233 || dwSkillId == 4270293) // BOSS没有放任何技能
		return;

	// 当前技能ID不等于上一个技能时，算下一个技能。
	if (dwSkillId != dwLastSkillId)
	{
		dwSameSkillIdCount = 0;
		LOG_CF_D( L"技能已变old=%d,.new=%d", dwLastSkillId, dwSkillId);
		dwLastSkillId = dwSkillId;
		CFarmBoss::GetInstance().GetMonsterSkillId() = dwSkillId;
		return;
	}

	dwSameSkillIdCount += 1;
	auto& vec = CFarmBoss::GetInstance().GetBossSkillVec();
	auto itr = vec.find(dwSkillId);
	if (itr == vec.end())
	{
		return ;
	}

	LOG_CF_D( L"相同技能ID:%d 过滤次数达到%d次了", itr->first, dwSameSkillIdCount);
	if (dwSameSkillIdCount == itr->second)
	{
		LOG_CF_D( L"相同技能ID:%d 过滤次数达到Max=%d", itr->first,  itr->second);
		CFarmBoss::GetInstance().GetMonsterSkillId() = dwSkillId;
		return;
	}
}

BOOL CGameFunction::HookMonsterSkill(_In_ BOOL bHook, _In_ DWORD dwMonsterBase) CONST
{
#ifndef _DEBUG
	static MyTools::MYHOOK_CONTENT MC;
	if (bHook)
	{
		LOG_CF_D( L"开始Hook,Base=%X", dwMonsterBase);
		CFarmBoss::GetInstance().SetHookMonsterBase(dwMonsterBase);
		MC.dwFunAddr = (DWORD)fnHookMonsterSkill;
		MC.dwHookAddr = HookMonsterAddr;
		MC.uNopCount = 0x1;
		MyTools::CLHook::Hook_Fun_Jmp_MyAddr(&MC);
	}
	else if (!bHook)
	{
		LOG_CF_D( L"关闭Hook! MC.dwAddr=%X,dwOldCode1=%X", MC.dwHookAddr, MC.dwOldCode1);
		MyTools::CLHook::UnHook_Fun_Jmp_MyAddr(&MC);
		ZeroMemory(&MC, sizeof(MC));
	}
#endif
	return TRUE;
}

BOOL CGameFunction::InitTimer() CONST
{
	if (CGameVariable::GetInstance().GetAccShareInfo() == NULL)
	{
		CGameVariable::GetInstance().GetAccShareInfo() = new ACCOUNT_INFO_GAME;
		CGameVariable::GetInstance().GetShareInfo() = new SHARED_INFO;
		ZeroMemory(CGameVariable::GetInstance().GetAccShareInfo(), sizeof(ACCOUNT_INFO_GAME));
		ZeroMemory(CGameVariable::GetInstance().GetShareInfo(), sizeof(SHARED_INFO));

		MyTools::CCharacter::wstrcpy_my(CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->szUserName, L"dlgTest");

		LOG_CF_D( L"正在枚举窗口……");
		while (EnumWindows((WNDENUMPROC)EnumSetWinName, reinterpret_cast<LPARAM>(&CGameVariable::GetInstance().GetAccShareInfo()->hGameWnd)))
			::Sleep(1000);

		// 判断是否二次注入!
		if (::GetModuleHandleW(L"BnsProjects.dll") != NULL)
		{
			LOG_CF_D( L"二次注入!");
			CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NMAE_MAINTHREADID, MyTools::CLThread::GetMainThreadId());
			g_dwTimerId = ::SetTimer(CGameVariable::GetInstance().GetAccShareInfo()->hGameWnd, 0x1001, 100, TimerProc);
			CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_FOODPERCENT, 50);
			CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_GETAROUNDMONSTER_DIS, 36);
			CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_KILLMONSTERR_MOVE_DIS, 2);
			return TRUE;
		}
	}

	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NMAE_MAINTHREADID, MyTools::CLThread::GetMainThreadId());
	g_dwTimerId = ::SetTimer(CGameVariable::GetInstance().GetAccShareInfo()->hGameWnd, 0x1000, 100, TimerProc);
	if (g_dwTimerId == NULL)
	{
		LOG_MSG_CF(L"SetTimer Faild! Err=%X,hWnd=%X", ::GetLastError(), CGameVariable::GetInstance().GetAccShareInfo()->hGameWnd);
		return FALSE;
	}

	// Hook D3d
	//LOG_CF_D( L"g_dwHookD3dAddr=%X,HookD3d=%X", g_dwHookD3dAddr, (DWORD)HookD3d);
	//CCharacter::WriteDWORD(g_dwHookD3dAddr, (DWORD)HookD3d);

	// Hook Speed
	//LOG_CF_D( L"g_dwHookSpeedAddr=%X,HookSpeed=%X, g_dwSpeedCALL=%X", g_dwHookSpeedAddr, (DWORD)HookSpeed, g_dwSpeedCALL);
#ifndef _DEBUG
#if ENABLE_LOCAL_SPEED
	if (CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.bAccelerate)
	{
		MyTools::CCharacter::WriteDWORD(g_dwHookSpeedAddr, (DWORD)HookSpeed);
	}
#endif
#endif

	// Hook Talk to Cmd
	//g_dwHookTalkCALL = CLHook::GetCall(HookTalkAddr, 0x400000, L"Client.exe");
	//LOG_CF_D( L"g_dwHookTalkCALL=%X", g_dwHookTalkCALL);

	//MYHOOK_CONTENT TalkHookContext;
	//ZeroMemory(&TalkHookContext, sizeof(TalkHookContext));
	//TalkHookContext.dwHookAddr = HookTalkAddr;
	//TalkHookContext.dwFunAddr = (DWORD)HookTalk;
	//TalkHookContext.uNopCount = 0x0;
	//CLHook::Hook_Fun_Jmp_MyAddr(&TalkHookContext);
	//g_vHookList.push_back(TalkHookContext);

	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_FOODPERCENT, 50);
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_GETAROUNDMONSTER_DIS, 36);
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_KILLMONSTERR_MOVE_DIS, 2);
	return TRUE;
}

BOOL CGameFunction::IsGameShow(_In_opt_ HWND* pWnd) CONST
{
	HWND pWnd_ = NULL;
	EnumWindows((WNDENUMPROC)EnumSetWinName, reinterpret_cast<LPARAM>(&pWnd_));
	MyTools::CLPublic::SetPtr(pWnd, pWnd_);
	return pWnd_ != NULL;
}

// 低于2h * 60
int CGameFunction::GetDianKa() CONST
{
	gs_bGetDianKa = FALSE;
	gs_dwDianKaDay = NULL;
	gs_dwDianKaHour = NULL;
	gs_dwDianKaMinute = NULL;

	auto fnHook = [](BOOL bHook)
	{
		static MyTools::MYHOOK_CONTENT HookContent;

		if (bHook)
		{
			ZeroMemory(&HookContent, sizeof(HookContent));
			HookContent.dwHookAddr = HookDianKaAddr;
			HookContent.dwFunAddr = (DWORD)HookDianKa;
			HookContent.uNopCount = 0x1;
			MyTools::CLHook::Hook_Fun_Jmp_MyAddr(&HookContent);
		}
		else if (!bHook && HookContent.dwHookAddr != NULL)
		{
			MyTools::CLHook::UnHook_Fun_Jmp_MyAddr(&HookContent);
		}
	};

	auto fnRefushDianKa = []
	{
		UINT uClausePanelCount = 0;
		while (!CObjectFunction::GetInstance().IsShow_In_GameUi(L"SelectCharacterPanel") && ++uClausePanelCount >= 10)
			Sleep(1000);

		if (uClausePanelCount < 10)
		{
			CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"SelectCharacterPanel", [](CONST CGameUi& SelectCharacterPanel){
				CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&SelectCharacterPanel]{CGameSend::RefushDianKa(SelectCharacterPanel.GetObj()); });
			});
		}

	};

	fnHook(TRUE);
	fnRefushDianKa();
	auto dwTick = ::GetTickCount64();
	while (!gs_bGetDianKa && GameRun)
	{
		if (static_cast<DWORD>(::GetTickCount64() - dwTick) >= 10 * 1000)
		{
			//LOG_CF_D(L"获取点卡时间超时!");
			fnHook(FALSE);
			return 0;
		}
		BnsSleep(100);
	}

	fnHook(FALSE);
	LOG_CF_D( L"点卡:%d,%d,%d", gs_dwDianKaDay, gs_dwDianKaHour, gs_dwDianKaMinute);
	return gs_dwDianKaDay * 24 * 60 + gs_dwDianKaHour * 60 + gs_dwDianKaMinute;
}


int CGameFunction::GetRecentlyPoint(_In_ CONST std::vector<Point>& vlst, _In_ CONST Point& StartPoint) CONST
{
	int nIndex = -1;
	float fDis = 30.0f;

	for (int i = 0; i < (int)vlst.size(); ++i)
	{
		float fTmpDis = CBaseObject::Get3DDis(vlst.at(i), StartPoint);
		if (fTmpDis < fDis)
		{
			fDis = fTmpDis;
			nIndex = i;
		}
	}
	return nIndex;
}

BOOL CGameFunction::Telport(_In_ CONST std::wstring& wsFileName) CONST
{
	std::wstring wsPath;
#if BnsProject_Dlg
	wsPath = L"C:\\Bns";
#else
	// 瞬移目录还没定下来
	wsPath = CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.szConsolePath;
	wsPath += L"\\BnsPath\\";
#endif
	wsPath += L"\\";
	wsPath += wsFileName;

	if (!MyTools::CLPublic::FileExit(wsPath.c_str()))
	{
		LOG_MSG_CF(L"瞬移文件:%s 不存在!", wsFileName.c_str());
		return FALSE;
	}

	// 等待非战斗状态!
	CPersonFunction::GetInstance().WaitForWarStatus();

	static DWORD dwTelprotCALL = NULL;
	if (dwTelprotCALL == NULL)
		dwTelprotCALL = MyTools::CLHook::GetCall(Hook瞬移地址, 0x400000, L"Client.exe");

	LOG_CF_D( L"dwTelprotCALL=%X", dwTelprotCALL);
	ULONG ulFileLen = 0;
	if (!MyTools::CLFile::ReadAsciiFileLen(wsPath.c_str(), ulFileLen))
	{
		LOG_MSG_CF(L"%s FileLen Invaild!", wsPath.c_str());
		return FALSE;
	}
	if (ulFileLen % MAX_TELPORT_SIZE != 0)
	{
		LOG_MSG_CF(L"%s FileLen=%d, Invaild!", wsPath.c_str(), (UINT)ulFileLen);
		return FALSE;
	}

	std::shared_ptr<CHAR> psBuffer = CBnsConfig::GetInstance().GetTelprotFile(wsPath, ulFileLen);
	if (psBuffer == nullptr)
		return FALSE;

	auto fnGetPointValue = [&psBuffer](_In_ int i, _In_ int nOffset)
	{
		DWORD dwAddr = ((DWORD*)psBuffer.get())[i * MAX_TELPORT_SIZE + nOffset];
		return ReadFloat((DWORD)&dwAddr);
	};

	std::vector<Point> vPoint;
	for (UINT i = 0; i < ulFileLen / (MAX_TELPORT_SIZE * 4); ++i)
		vPoint.push_back(Point(fnGetPointValue(i, 1), fnGetPointValue(i, 2), fnGetPointValue(i, 3)));

	int nIndex = CGameFunction::GetRecentlyPoint(vPoint, CBaseObject::GetPersonPoint());
	if (nIndex == -1)
	{
		LOG_CF_D( L"竟然离最近的瞬移点超过30m");
		return TRUE;
	}

	BnsMove(vPoint.at(nIndex), 2.0f);
	CPersonFunction::GetInstance().Move(vPoint.at(nIndex));
	::Sleep(2000);

	DWORD dwPersonBase = CPerson::GetInstance().GetBase();

	std::vector<DWORD*> AllocVec;
	//CBnsHook::GetInstance().SetHookDungoenMode(TRUE);
	for (int i = nIndex; i < (int)vPoint.size() && GameRun && !EMPTY_PERSONHP; ++i)
	{
		DWORD* dwArray = new DWORD[MAX_TELPORT_SIZE];
		if (dwArray == NULL)
		{
			LOG_CF_E( L"瞬移申请内存失败!");
			continue;
		}
		AllocVec.push_back(dwArray);
		memcpy(dwArray, ((DWORD*)psBuffer.get()) + i * MAX_TELPORT_SIZE, sizeof(DWORD) * MAX_TELPORT_SIZE);
		LOG_CF_D( L"i=%d,dwArray=%X, size=%d", i, dwArray, vPoint.size());
		CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&dwArray, &dwPersonBase]{
			CGameSend::Telport(dwArray, dwTelprotCALL, dwPersonBase);
		});
	}
	for (auto& itm : AllocVec)
		delete[] itm;

	BnsSleep(1000);
	LOG_CF_D( L"瞬移完毕!");
	return CSomeFun::GetInstance().SimlationKey(VK_SPACE, 200, 1000);
	//return CBnsHook::GetInstance().SetHookDungoenMode(FALSE);
}

BOOL CGameFunction::LineTelport(_In_ CONST FixPointInfo& FixPtInfo, _In_ CONST Point& TarPoint, _In_ DWORD dwMaxCount) CONST
{
	if (CPerson::GetInstance().GetDis(FixPtInfo.GetPoint()) > FixPtInfo.fDis)
	{
		LOG_CF_D( L"当前坐标不在瞬移点[%d,%d,%d]的范围%.2fm内", FixPtInfo.GetPoint().GetX(), FixPtInfo.GetPoint().GetY(), FixPtInfo.GetPoint().GetZ(), FixPtInfo.fDis);
		return TRUE;
	}

	DWORD dwCount = 0;
	//DWORD dwAddr = ReadDWORD(CPerson::GetInstance().GetBase() + 0x14) + 人物瞬移偏移2;
	//CONST DWORD dwTelprotCALL = CLHook::GetCall(Hook瞬移地址, 0x400000, L"Client.exe");
	while (GameRun && !EMPTY_PERSONHP && MyTools::CLPublic::GetDisBy2D(CPerson::GetInstance().GetPoint(), TarPoint) > 3 * 50 && dwCount++ < dwMaxCount)
	{
		LOG_CF_D( L"正在进行第%d次瞬移!", dwCount);

		Point CurPoint = CPerson::GetInstance().GetPoint();
		DWORD dwAddr = ReadDWORD(CPerson::GetInstance().GetBase() + 0x14) + 人物瞬移偏移2;

		float fDis = MyTools::CLPublic::GetDisBy3D(CurPoint, TarPoint);
		int nCount = (int)(fDis / 100) + 1;
		float fMoveX = (TarPoint.X - CurPoint.X) / nCount;
		float fMoveY = (TarPoint.Y - CurPoint.Y) / nCount;
		float fMoveZ = (TarPoint.Z - CurPoint.Z) / nCount;
		CONST DWORD dwTelprotCALL = MyTools::CLHook::GetCall(Hook瞬移地址, 0x400000, L"Client.exe");
		float fNextX = CurPoint.X, fNextY = CurPoint.Y, fNextZ = CurPoint.Z;
		for (int i = 0; i < nCount; ++i)
		{
			float fNowX = CurPoint.X + fMoveX * i;
			float fNowY = CurPoint.Y + fMoveY * i;
			float fNowZ = CurPoint.Z + fMoveZ * i;


			*(float*)(dwAddr + 0x4) = fNowX;
			*(float*)(dwAddr + 0x8) = fNowY;
			*(float*)(dwAddr + 0xC) = fNowZ;

			*(DWORD*)(dwAddr + 瞬移次数偏移) = 10;
			DWORD dwValue = (10 << 0x4) + 10;

			fNowX += fMoveX;
			fNowY += fMoveY;
			fNowZ += fMoveZ;

			*(float*)(dwAddr + dwValue * 4 - 0x40) = fNowX;
			*(float*)(dwAddr + dwValue * 4 - 0x3C) = fNowY;
			*(float*)(dwAddr + dwValue * 4 - 0x38) = fNowZ;
#define MAXMOVE 10
			for (int j = 1; j <= MAXMOVE; ++j)
			{
				fNextX += fMoveX / MAXMOVE;
				fNextY += fMoveY / MAXMOVE;
				fNextZ += fMoveZ / MAXMOVE;
				__asm
				{
					PUSH fNextZ;
					PUSH fNextY;
					PUSH fNextX;
					PUSH dwAddr;
					MOV EAX, 瞬移Buffer填充CALL;
					CALL EAX;
				}
			}
			*(BYTE*)(dwAddr + 设置瞬移浮空偏移) = 0x10;
			*(float*)(dwAddr + 瞬移距离偏移) = 0.25f;
			__asm
			{
				PUSH 0;
				PUSH 0;
				MOV EDI, dwAddr;
				MOV EAX, dwTelprotCALL;
				CALL EAX;
			}
			::Sleep(50);
		}
		//CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&TarPoint, &dwCount, &dwAddr, &dwTelprotCALL]{CGameSend::Telport(dwTelprotCALL, CPerson::GetInstance().GetBase()); });
		BnsSleep(1000);
		LOG_CF_D( L"第%d次瞬移完毕!", dwCount);
		CSomeFun::GetInstance().SimlationKey(VK_SPACE);
		BnsSleep(3000);
		CPersonFunction::GetInstance().WaitForWarStatus();
	}
	return TRUE;
}

BOOL CGameFunction::HeightTelport(_In_ CONST FixPointInfo& FixPtInfo, _In_ CONST Point& TarPoint, _In_ DWORD dwMaxCount) CONST
{
	if (CPerson::GetInstance().GetDis(FixPtInfo.GetPoint()) > FixPtInfo.fDis)
	{
		LOG_CF_D( L"当前坐标不在瞬移点[%d,%d,%d]的范围%.2fm内", FixPtInfo.GetPoint().GetX(), FixPtInfo.GetPoint().GetY(), FixPtInfo.GetPoint().GetZ(), FixPtInfo.fDis);
		return TRUE;
	}

	Point FixPoint = CPerson::GetInstance().GetPoint();

	DWORD dwCount = 0;
	CONST DWORD dwTelprotCALL = MyTools::CLHook::GetCall(Hook瞬移地址, 0x400000, L"Client.exe");
	while (GameRun && !EMPTY_PERSONHP && MyTools::CLPublic::GetDisBy2D(CPerson::GetInstance().GetPoint(), TarPoint) > 3 * 50 && dwCount++ < dwMaxCount)
	{
		DWORD dwSleepTime = 50 * dwCount;
		LOG_CF_D( L"正在进行第%d次瞬移!", dwCount);
		BnsMove(FixPoint, 1);
		BnsSleep(2000);

		Point CurPoint = CPerson::GetInstance().GetPoint();
		DWORD dwAddr = ReadDWORD(CPerson::GetInstance().GetBase() + 0x14) + 人物瞬移偏移2;

		// 计算Z
		float fDisZ = TarPoint.Z - CurPoint.Z;
		int nCountZ = (int)(fDisZ / 100) + 1;
		float fMoveZ = (TarPoint.Z - CurPoint.Z) / nCountZ;
		float fNextZ = CurPoint.Z;
		for (int i = 0; i < nCountZ; ++i)
		{
			float fNowX = CurPoint.X, fNowY = CurPoint.Y, fNowZ = CurPoint.Z + fMoveZ * i;

			*(float*)(dwAddr + 0x4) = fNowX;
			*(float*)(dwAddr + 0x8) = fNowY;
			*(float*)(dwAddr + 0xC) = fNowZ;

			*(DWORD*)(dwAddr + 瞬移次数偏移) = 10;
			DWORD dwValue = (10 << 0x4) + 10;

			fNowZ += fMoveZ;
			*(float*)(dwAddr + dwValue * 4 - 0x40) = fNowX;
			*(float*)(dwAddr + dwValue * 4 - 0x3C) = fNowY;
			*(float*)(dwAddr + dwValue * 4 - 0x38) = fNowZ;
#define MAXMOVE 10
			for (int j = 1; j <= MAXMOVE; ++j)
			{
				fNextZ += fMoveZ / MAXMOVE;
				__asm
				{
					PUSH fNextZ;
					PUSH fNowY;
					PUSH fNowX;
					PUSH dwAddr;
					MOV EAX, 瞬移Buffer填充CALL;
					CALL EAX;
				}
			}
			*(BYTE*)(dwAddr + 设置瞬移浮空偏移) = 0x10;
			*(float*)(dwAddr + 瞬移距离偏移) = 0.25f;
			__asm
			{
				PUSH 0;
				PUSH 0;
				MOV EDI, dwAddr;
				MOV EAX, dwTelprotCALL;
				CALL EAX;
			}
			::Sleep(dwSleepTime);
		}

		// 只计算X,Y
		float fDis = MyTools::CLPublic::GetDisBy2D(CurPoint, TarPoint);
		int nCount = (int)(fDis / 100) + 1;
		float fMoveX = (TarPoint.X - CurPoint.X) / nCount;
		float fMoveY = (TarPoint.Y - CurPoint.Y) / nCount;
		float fNextX = CurPoint.X, fNextY = CurPoint.Y;
		for (int i = 0; i < nCount; ++i)
		{
			float fNowX = CurPoint.X + fMoveX * i;
			float fNowY = CurPoint.Y + fMoveY * i;
			float fNowZ = fNextZ;


			*(float*)(dwAddr + 0x4) = fNowX;
			*(float*)(dwAddr + 0x8) = fNowY;
			*(float*)(dwAddr + 0xC) = fNowZ;

			*(DWORD*)(dwAddr + 瞬移次数偏移) = 10;
			DWORD dwValue = (10 << 0x4) + 10;

			fNowX += fMoveX;
			fNowY += fMoveY;

			*(float*)(dwAddr + dwValue * 4 - 0x40) = fNowX;
			*(float*)(dwAddr + dwValue * 4 - 0x3C) = fNowY;
			*(float*)(dwAddr + dwValue * 4 - 0x38) = fNowZ;
#define MAXMOVE 10
			for (int j = 1; j <= MAXMOVE; ++j)
			{
				fNextX += fMoveX / MAXMOVE;
				fNextY += fMoveY / MAXMOVE;
				__asm
				{
					PUSH fNextZ;
					PUSH fNextY;
					PUSH fNextX;
					PUSH dwAddr;
					MOV EAX, 瞬移Buffer填充CALL;
					CALL EAX;
				}
			}
			*(BYTE*)(dwAddr + 设置瞬移浮空偏移) = 0x10;
			*(float*)(dwAddr + 瞬移距离偏移) = 0.25f;
			__asm
			{
				PUSH 0;
				PUSH 0;
				MOV EDI, dwAddr;
				MOV EAX, dwTelprotCALL;
				CALL EAX;
			}
			::Sleep(dwSleepTime);
		}
		
		//CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&TarPoint, &dwCount, &dwAddr, &dwTelprotCALL]{CGameSend::Telport(dwTelprotCALL, CPerson::GetInstance().GetBase()); });
		BnsSleep(1000);
		LOG_CF_D( L"第%d次瞬移完毕!", dwCount);
		CSomeFun::GetInstance().SimlationKey(VK_SPACE);
		BnsSleep(3000);
		CPersonFunction::GetInstance().WaitForWarStatus();
	}
	return TRUE;
}

BOOL CGameFunction::FloatTelport(_In_ CONST FixPointInfo& FixPtInfo, _In_ CONST Point& TarPoint, _In_ DWORD dwMaxCount) CONST
{
	if (CPerson::GetInstance().GetDis(FixPtInfo.GetPoint()) > FixPtInfo.fDis)
	{
		LOG_CF_D( L"当前坐标不在瞬移点[%d,%d,%d]的范围%.2fm内", FixPtInfo.GetPoint().GetX(), FixPtInfo.GetPoint().GetY(), FixPtInfo.GetPoint().GetZ(), FixPtInfo.fDis);
		return TRUE;
	}

	DWORD dwCount = 0;
	//DWORD dwAddr = ReadDWORD(CPerson::GetInstance().GetBase() + 0x14) + 人物瞬移偏移2;
	CONST DWORD dwTelprotCALL = MyTools::CLHook::GetCall(Hook瞬移地址, 0x400000, L"Client.exe");
	while (GameRun && !EMPTY_PERSONHP && MyTools::CLPublic::GetDisBy2D(CPerson::GetInstance().GetPoint(), TarPoint) > 3 * 50 && dwCount++ < dwMaxCount)
	{
		LOG_CF_D( L"正在进行第%d次瞬移!", dwCount);

		auto fnChangZ = [](CONST Point& CurPoint, CONST Point& TarPoint)
		{
			// 计算Z
			__try
			{
				DWORD dwAddr = ReadDWORD(CPerson::GetInstance().GetBase() + 0x14) + 人物瞬移偏移2;

				float fDis = MyTools::CLPublic::GetDisBy3D(CurPoint, TarPoint);
				int nCount = (int)(fDis / 100) + 1;
				float fMoveX = (TarPoint.X - CurPoint.X) / nCount;
				float fMoveY = (TarPoint.Y - CurPoint.Y) / nCount;
				float fMoveZ = (TarPoint.Z - CurPoint.Z) / nCount;
				CONST DWORD dwTelprotCALL = MyTools::CLHook::GetCall(Hook瞬移地址, 0x400000, L"Client.exe");
				float fNextX = CurPoint.X, fNextY = CurPoint.Y, fNextZ = CurPoint.Z;
				for (int i = 0; i < nCount; ++i)
				{
					float fNowX = CurPoint.X + fMoveX * i;
					float fNowY = CurPoint.Y + fMoveY * i;
					float fNowZ = CurPoint.Z + fMoveZ * i;


					*(float*)(dwAddr + 0x4) = fNowX;
					*(float*)(dwAddr + 0x8) = fNowY;
					*(float*)(dwAddr + 0xC) = fNowZ;

					*(DWORD*)(dwAddr + 瞬移次数偏移) = 10;
					DWORD dwValue = (10 << 0x4) + 10;

					fNowX += fMoveX;
					fNowY += fMoveY;
					fNowZ += fMoveZ;

					*(float*)(dwAddr + dwValue * 4 - 0x40) = fNowX;
					*(float*)(dwAddr + dwValue * 4 - 0x3C) = fNowY;
					*(float*)(dwAddr + dwValue * 4 - 0x38) = fNowZ;
#define MAXMOVE 10
					for (int j = 1; j <= MAXMOVE; ++j)
					{
						fNextX += fMoveX / MAXMOVE;
						fNextY += fMoveY / MAXMOVE;
						fNextZ += fMoveZ / MAXMOVE;
						__asm
						{
							PUSH fNextZ;
							PUSH fNextY;
							PUSH fNextX;
							PUSH dwAddr;
							MOV EAX, 瞬移Buffer填充CALL;
							CALL EAX;
						}
					}
					*(BYTE*)(dwAddr + 设置瞬移浮空偏移) = 0x10;
					*(float*)(dwAddr + 瞬移距离偏移) = 0.25f;
					__asm
					{
						PUSH 0;
						PUSH 0;
						MOV EDI, dwAddr;
						MOV EAX, dwTelprotCALL;
						CALL EAX;
					}
					::Sleep(50);
				}
			}
			__except(1)
			{
				LOG_CF_E( L"Z Exception");
			}
		};

		Point CurPoint = CPerson::GetInstance().GetPoint();
		fnChangZ(CurPoint, Point(CurPoint.X, CurPoint.Y, CurPoint.Z + 100));
		CurPoint.Z += 100;

		// 只计算X,Y
		DWORD dwAddr = ReadDWORD(CPerson::GetInstance().GetBase() + 0x14) + 人物瞬移偏移2;
		float fDis = MyTools::CLPublic::GetDisBy2D(CurPoint, TarPoint);
		int nCount = (int)(fDis / 100) + 1;
		float fMoveX = (TarPoint.X - CurPoint.X) / nCount;
		float fMoveY = (TarPoint.Y - CurPoint.Y) / nCount;
		float fNextX = CurPoint.X, fNextY = CurPoint.Y;
		__try
		{
			for (int i = 0; i < nCount; ++i)
			{
				float fNowX = CurPoint.X + fMoveX * i;
				float fNowY = CurPoint.Y + fMoveY * i;
				float fNowZ = CurPoint.Z;


				*(float*)(dwAddr + 0x4) = fNowX;
				*(float*)(dwAddr + 0x8) = fNowY;
				*(float*)(dwAddr + 0xC) = fNowZ;

				*(DWORD*)(dwAddr + 瞬移次数偏移) = 10;
				DWORD dwValue = (10 << 0x4) + 10;

				fNowX += fMoveX;
				fNowY += fMoveY;

				*(float*)(dwAddr + dwValue * 4 - 0x40) = fNowX;
				*(float*)(dwAddr + dwValue * 4 - 0x3C) = fNowY;
				*(float*)(dwAddr + dwValue * 4 - 0x38) = fNowZ;
#define MAXMOVE 10
				for (int j = 1; j <= MAXMOVE; ++j)
				{
					fNextX += fMoveX / MAXMOVE;
					fNextY += fMoveY / MAXMOVE;
					__asm
					{
						PUSH fNowZ;
						PUSH fNextY;
						PUSH fNextX;
						PUSH dwAddr;
						MOV EAX, 瞬移Buffer填充CALL;
						CALL EAX;
					}
				}
				*(BYTE*)(dwAddr + 设置瞬移浮空偏移) = 0x10;
				*(float*)(dwAddr + 瞬移距离偏移) = 0.25f;
				__asm
				{
					PUSH 0;
					PUSH 0;
					MOV EDI, dwAddr;
					MOV EAX, dwTelprotCALL;
					CALL EAX;
				}
				::Sleep(50);
			}

		}
		__except (1)
		{
			LOG_CF_E( L"X,Y Telprot Exception!");
		}
		
		fnChangZ(Point(fNextX, fNextY, CurPoint.Z), TarPoint);

		//CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&TarPoint, &dwCount, &dwAddr, &dwTelprotCALL]{CGameSend::Telport(dwTelprotCALL, CPerson::GetInstance().GetBase()); });
		BnsSleep(5000);
		LOG_CF_D( L"第%d次瞬移完毕!", dwCount);
		CSomeFun::GetInstance().SimlationKey(VK_SPACE);
		BnsSleep(3000);
		CPersonFunction::GetInstance().WaitForWarStatus();
	}
	return TRUE;
}

BOOL CGameFunction::StudySkill(_In_ CONST std::vector<LearnSkillContent>& vlst, _In_ CONST DWORD Ntype) CONST
{
	CGameUi SkillBook3Panel;
	while (!CObjectFunction::GetInstance().IsShow_In_GameUi(L"SkillBook3Panel") && GameRun)
	{
		CSomeFun::GetInstance().SimlationKey('K');
		BnsSleep(1000);
	}

	BnsSleep(1000);
	CObjectFunction::GetInstance().Action_By_GameUiName(L"SkillBook3Panel", [vlst, Ntype](CONST CGameUi& SkillBook3Panel)
	{
		CSkill::StudySkill(vlst, Ntype);
		BnsSleep(3 * 1000);
		CSomeFun::GetInstance().SimlationKey('K');
	});

	return TRUE;
}

BOOL CGameFunction::SwitchSkillPage(_In_ CONST std::wstring& wsSkillType) CONST
{
	auto fnPtrSwitchSkillPage = [](int Ntype)
	{
		LOG_CF_D( L"正在切换技能系! Type=%d", Ntype);
		CGameUi SkillBook3Panel;
		while (!CObjectFunction::GetInstance().IsShow_In_GameUi(L"SkillBook3Panel") && GameRun)
		{
			LOG_CF_D(L"打开技能列表!");
			CSomeFun::GetInstance().SimlationKey('K');
			BnsSleep(1000);
		}

		BnsSleep(1000);
		CObjectFunction::GetInstance().Action_By_GameUiName(L"SkillBook3Panel", [Ntype](CONST CGameUi& SkillBook3Panel)
		{
			CGameVariable::GetInstance().PushMainThreadFunPtr_synch([SkillBook3Panel]{ CGameSend::SwitchSkillPage(SkillBook3Panel.GetBase()); });
			BnsSleep(2000);
			CSomeFun::GetInstance().SimlationKey('Y');
			BnsSleep(2000);
			CGameVariable::GetInstance().PushMainThreadFunPtr_synch([Ntype]{CGameSend::AddSkillPoint_Send(Ntype); });
			BnsSleep(3000);
			CSomeFun::GetInstance().SimlationKey('K');
			BnsSleep(2000);
		});
	};

	LOG_CF_D( L"wsSkillType=%s", wsSkillType.c_str());
	if (wsSkillType == L"召唤大地系")//1
	{
		// 向日葵
		while (GameRun && !EMPTY_PERSONHP && !CFarmSkill::GetInstance().GetSkill_By_Name(L"해바라기", nullptr))
		{
			fnPtrSwitchSkillPage(1);
		}
	}
	else if (wsSkillType == L"召唤清风系")//0
	{
		// 马蜂
		while (GameRun && !EMPTY_PERSONHP && !CFarmSkill::GetInstance().GetSkill_By_Name(L"말벌", nullptr))
		{
			fnPtrSwitchSkillPage(0);
		}
	}
	else if (wsSkillType == L"咒术寒冰系")//0
	{
		// 应龙苍闪
		while (GameRun && !EMPTY_PERSONHP && !CFarmSkill::GetInstance().GetSkill_By_Name(L"폭마령", nullptr))
		{
			fnPtrSwitchSkillPage(0);
		}
	}
	else if (wsSkillType == L"咒术暗黑系")//1
	{
		// 黑炎龙
		while (GameRun && !EMPTY_PERSONHP && !CFarmSkill::GetInstance().GetSkill_By_Name(L"흑염룡", nullptr))
		{
			fnPtrSwitchSkillPage(1);
		}
	}
	else if (wsSkillType == L"剑士火焰系")//0
	{
		// 火龙连斩
#ifdef BnsProject_TaiWan
		while (GameRun && !EMPTY_PERSONHP && !CFarmSkill::GetInstance().GetSkill_By_Name(L"火龍連斬", nullptr))
		{
			fnPtrSwitchSkillPage(0);
		}
#endif // BnsProject_TaiWan
#ifdef BnsProject_Korea
		while (GameRun && !EMPTY_PERSONHP && !CFarmSkill::GetInstance().GetSkill_By_Name(L"화룡연참", nullptr))
		{
			fnPtrSwitchSkillPage(0);
		}
#endif // BnsProject_Korea


	
	}
	else if (wsSkillType == L"剑士雷电系")//1
	{
		// 夺命剑
#ifdef BnsProject_TaiWan
		while (GameRun && !EMPTY_PERSONHP && !CFarmSkill::GetInstance().GetSkill_By_Name(L"奪命劍", nullptr))
		{
			fnPtrSwitchSkillPage(1);
		}
#endif // BnsProject_TaiWan
#ifdef BnsProject_Korea
		while (GameRun && !EMPTY_PERSONHP && !CFarmSkill::GetInstance().GetSkill_By_Name(L"급소베기", nullptr))
		{
			fnPtrSwitchSkillPage(1);
		}
#endif // BnsProject_Korea
		
		
	}
	else if (wsSkillType == L"气功火系")//0
	{
		// 炎龙啸
		while (GameRun && !EMPTY_PERSONHP && !CFarmSkill::GetInstance().GetSkill_By_Name(L"폭열신장", nullptr) && !CFarmSkill::GetInstance().GetSkill_By_Name(L"폭열염포", nullptr))
		{
			fnPtrSwitchSkillPage(0);
		}
	}
	else if (wsSkillType == L"气功冰系")//1
	{
		// 冰白寒炮
		while (GameRun && !EMPTY_PERSONHP && !CFarmSkill::GetInstance().GetSkill_By_Name(L"빙백한포", nullptr))
		{
			fnPtrSwitchSkillPage(1);
		}
	}
	else if (wsSkillType == L"刺客雷系")//0
	{
		// 雷电斩
		while (GameRun && !EMPTY_PERSONHP && !CFarmSkill::GetInstance().GetSkill_By_Name(L"뇌격살", nullptr))
		{
			fnPtrSwitchSkillPage(0);
		}
	}
	else if (wsSkillType == L"刺客暗系")//1
	{
		// 暗黑杀
		while (GameRun && !EMPTY_PERSONHP && !CFarmSkill::GetInstance().GetSkill_By_Name(L"암흑살", nullptr))
		{
			fnPtrSwitchSkillPage(1);
		}
	}
	else if (wsSkillType == L"灵剑士风系")//0
	{
		// 台风斩
		while (GameRun && !EMPTY_PERSONHP && !CFarmSkill::GetInstance().GetSkill_By_Name(L"태풍참", nullptr))
		{
			fnPtrSwitchSkillPage(0);
		}
	}
	else if (wsSkillType == L"灵剑士雷系")//1
	{
		// 雷炎闪
		while (GameRun && !EMPTY_PERSONHP && !CFarmSkill::GetInstance().GetSkill_By_Name(L"뇌연섬", nullptr))
		{
			fnPtrSwitchSkillPage(1);
		}
	}
	else if (wsSkillType == L"拳师风系")//0
	{
		// 回天脚
		while (GameRun && !EMPTY_PERSONHP && !CFarmSkill::GetInstance().GetSkill_By_Name(L"회천각", nullptr))
		{
			fnPtrSwitchSkillPage(0);
		}
	}
	else if (wsSkillType == L"拳师火系")//1
	{
		// 爆裂脚
		while (GameRun && !EMPTY_PERSONHP && !CFarmSkill::GetInstance().GetSkill_By_Name(L"백열각", nullptr))
		{
			fnPtrSwitchSkillPage(1);
		}
	}
	else if (wsSkillType == L"枪手火系")//0
	{
		// 火系右键技能
		while (GameRun && !EMPTY_PERSONHP && !CFarmSkill::GetInstance().GetSkill_By_Name(L"연환", nullptr))
		{
			fnPtrSwitchSkillPage(0);
		}
	}
	else if (wsSkillType == L"枪手暗系")//1
	{
		// 暗系右键技能
		while (GameRun && !EMPTY_PERSONHP && !CFarmSkill::GetInstance().GetSkill_By_Name(L"망상", nullptr))
		{
			fnPtrSwitchSkillPage(1);
		}
	}
	else
	{
		LOG_MSG_CF(L"无效技能系:%s", wsSkillType.c_str());
	}
	LOG_CF_D( L"切换技能系完毕!!");
	return TRUE;
}

BOOL CGameFunction::GrowthEqui(_In_ CBag::em_Equi_Type emType)
{
	BOOL bGrowthFinish = FALSE;
	CONST std::wstring wsSacrificeName = (emType == CBag::em_Equi_Type_Weapon) ? Item_洪门武魂 : Item_洪门宝玉;

	// 是否还有祭品 // 判断是否强化完毕
	while (GameRun && !EMPTY_PERSONHP)
	{
		CBag GrowthEquiItem;
		if (!CBagFunction::GetInstance().GetEqui_By_Type(emType, &GrowthEquiItem))
		{
			LOG_CF_D( L"无法找到需要进化的装备类型:%X", emType);
			break;
		}

		CBag Sacrifice;
		if (!CBagFunction::GetInstance().ExistItem_By_Name(wsSacrificeName, &Sacrifice))
		{
			LOG_CF_D( L"没祭品了!");
			break;
		}

		UINT uCount = CBagFunction::GetInstance().GetItemCount_By_Name(wsSacrificeName);
		LOG_CF_D( L"强化前祭品数量=%d", uCount);
		Sacrifice.EquiEnhanced(GrowthEquiItem.GetSId());
		BnsSleep(2 * 1000);
		UINT uNowCount = CBagFunction::GetInstance().GetItemCount_By_Name(wsSacrificeName);
		LOG_CF_D( L"强化后祭品数量=%d", uNowCount);
		if (uNowCount == uCount)
		{
			LOG_CF_D( L"强化祭品数量并没有减少, 还是%d个， 强化完毕!", uCount);
			bGrowthFinish = TRUE;
			break;
		}
		BnsSleep(2 * 1000);
	}
	return bGrowthFinish;
}

BOOL CGameFunction::InitNavigation() CONST
{
	typedef int(__stdcall * TUserLogin)(const char * szIP, const char * szUser, const char * szPassWord);

#if BnsProject_DLL
	std::wstring wsPath = CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.szConsolePath;
	wsPath += L"\\CopyFile\\NavClient.dll";
#else
	std::wstring wsPath = L"C:\\Bns\\NavClient.dll";
#endif
	if (!MyTools::CLPublic::FileExit(wsPath.c_str()))
	{
		LOG_MSG_CF(L"丢失文件:%s", wsPath.c_str());
		return FALSE;
	}
	HMODULE hmDLL = ::LoadLibraryW(wsPath.c_str());
	if (hmDLL == NULL)
	{
		LOG_MSG_CF(L"加载DLL:%s 失败", wsPath.c_str());
		return FALSE;
	}
	TUserLogin NavLogin = (TUserLogin)::GetProcAddress(hmDLL, "UserLogin");
	if (NavLogin == nullptr)
	{
		LOG_MSG_CF(L"无法找到NavClient的导出函数:UserLogin!");
		return FALSE;
	}

	int nRetCode = NavLogin("112.74.105.214:10121", "chchch25", "123456");
	if (nRetCode == -30)
	{
		LOG_CF_D( L"重复登录!");
		return TRUE;
	}
	else if (nRetCode < 0)
	{
		LOG_MSG_CF(L"NavClient登录失败,错误代码:%d", nRetCode);
		return FALSE;
	}
	LOG_CF_D( L"nRetCode=%d", nRetCode);
	return TRUE;
}

BOOL CGameFunction::NavFindPath(_In_ CONST std::string& csTarMapName, _In_ CONST Point& TarPoint, _Out_opt_ std::vector<Point>& vPoint) CONST
{
	typedef int(__stdcall * TRequestMapNav)(const char * szMap, float x, float y, float z, float x1, float y1, float z1);
	typedef int(__stdcall * TGetNextNavPos)(float * x, float * y, float * z);

	static TRequestMapNav RequestMapNav = nullptr;
	static TGetNextNavPos GetNextNavPos = nullptr;

	if (RequestMapNav == nullptr)
	{
#if BnsProject_DLL
		std::wstring wsPath = CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.szConsolePath;
		wsPath += L"\\CopyFile\\NavClient.dll";
#else
		std::wstring wsPath = L"C:\\Bns\\NavClient.dll";
#endif
		if (!MyTools::CLPublic::FileExit(wsPath.c_str()))
		{
			LOG_MSG_CF(L"丢失文件:%s", wsPath.c_str());
			return FALSE;
		}
		HMODULE hmDLL = ::LoadLibraryW(wsPath.c_str());
		if (hmDLL == NULL)
		{
			LOG_MSG_CF(L"加载DLL失败:%s", wsPath.c_str());
			return FALSE;
		}
		RequestMapNav = (TRequestMapNav)::GetProcAddress(hmDLL, "RequestMapNav");
		GetNextNavPos = (TGetNextNavPos)::GetProcAddress(hmDLL, "GetNextNavPos");
		if (RequestMapNav == nullptr || GetNextNavPos == nullptr)
		{
			LOG_MSG_CF(L"无法找到NavClient的导出函数:RequestMapNav && GetNextNavPos! Addr=[%X,%X]", RequestMapNav, GetNextNavPos);
			return FALSE;
		}
	}

	Point CurPoint = CPerson::GetInstance().GetPoint();
	int nRetCode = RequestMapNav(csTarMapName.c_str(), CurPoint.X, CurPoint.Y, CurPoint.Z, TarPoint.X, TarPoint.Y, TarPoint.Z);
	LOG_CF_D( L"导航寻路返回Code:%d", nRetCode);
	if (nRetCode >= 0)
	{
		Point tmpPoint;
		while ((nRetCode = GetNextNavPos(&tmpPoint.X, &tmpPoint.Y, &tmpPoint.Z)) >= 0 && GameRun)
			vPoint.push_back(tmpPoint);

		if (nRetCode > 0 || nRetCode == -8004)
		{
			LOG_CF_D( L"导航寻路返回的坐标数量:%d", vPoint.size());
			return TRUE;
		}

		LOG_MSG_CF(L"取坐标失败, 错误代码:%d", nRetCode);
		return FALSE;
	}

	switch (nRetCode)
	{
	case -2002:
		if (InitNavigation())
			return NavFindPath(csTarMapName, TarPoint, vPoint);
	default:
		LOG_MSG_CF(L"取坐标失败, 错误代码:%d", nRetCode);
		return FALSE;
	}
}

