#include "stdafx.h"
#include "DxOptimize.h"
#include <d3d9.h>
#include <MyTools/Log.h>
#include <MyTools/CLSearchBase.h>
#include <MyTools/CLHook.h>

#define _SELF L"DxOptimize.cpp"

DWORD dwPresentAddr = NULL;
DWORD dwPresentValue = NULL;

CDxOptimize::CDxOptimize()
{
	hmDLL = NULL;
}


static DWORD dwDirectDx9 = 0;
static DWORD dwRestoreaddr = 0;
__declspec(naked) VOID WINAPI fnHookDxParamter()
{
	__asm
	{
		MOV dwDirectDx9, EAX;
		MOV EDX, DWORD PTR DS : [EAX];
		MOV EDX, DWORD PTR DS : [EDX + 0x44];
		PUSH dwRestoreaddr;
		RETN;
	}
}

HRESULT WINAPI NewPresent(LPDIRECT3DDEVICE9 pDevice, CONST RECT *pSourceRect, CONST RECT *pDestRect, HWND hDestWindowOverride, CONST RGNDATA *pDirtyRegion)
{
	pDevice->BeginScene();
	pDevice->Clear(NULL, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, NULL);
	pDevice->EndScene();

	typedef HRESULT(WINAPI *fnPresent)(LPDIRECT3DDEVICE9, CONST RECT *, CONST RECT *, HWND , CONST RGNDATA *);
	fnPresent pfnPresent = reinterpret_cast<fnPresent>(dwPresentValue);
	return pfnPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}


BOOL CDxOptimize::Start()
{
	if (!GetDLLModule())
		return FALSE;

	// inline Hook
	/*LPDIRECT3D9 pDirectDx9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (pDirectDx9 == nullptr)
	{
		LOG_CF_D( L"Direct3DCreate9 Faild!");
		return FALSE;
	}

	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;
	D3DPRESENT_PARAMETERS pPresentParms;
	ZeroMemory(&pPresentParms, sizeof(pPresentParms));
	pPresentParms.Windowed = TRUE;
	pPresentParms.BackBufferFormat = D3DFMT_UNKNOWN;
	pPresentParms.SwapEffect = D3DSWAPEFFECT_DISCARD;
	if (FAILED(pDirectDx9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, GetDesktopWindow(), D3DCREATE_SOFTWARE_VERTEXPROCESSING, &pPresentParms, &pD3DDevice)))
	{
		LOG_CF_D( L"CreateDevice Faild!");
		return;
	}

	// Get Virtual Table!
	DWORD dwVirtualTable = ReadDWORD(reinterpret_cast<DWORD>(pD3DDevice));
	DWORD dwRetJmpAddr = ReadDWORD(dwVirtualTable + 0x44) + 0x5;
	DWORD dwHookAddr = ReadDWORD(dwVirtualTable + 0x44);*/

	// Hook Virtual Table!
	MyTools::MYHOOK_CONTENT HookDxAddrContent;
	HookDxAddrContent.dwHookAddr = MyTools::CLSearchBase::FindAddr("8B108B5244??????8D", 0x97B4D0A - 0x97B4D0A, 0, L"bsengine_Shipping.dll");
	HookDxAddrContent.dwFunAddr = reinterpret_cast<DWORD>(fnHookDxParamter);

	// Get Restore Addr
	dwRestoreaddr = HookDxAddrContent.dwHookAddr + 0x5;

	// Hook!
	dwDirectDx9 = NULL;
	MyTools::CLHook::Hook_Fun_Jmp_MyAddr(&HookDxAddrContent);
	//LOG_MSG_CF( L"HookAddr=%X, dwFunAddr=%X, NewPresent=%X", HookDxAddrContent.dwHookAddr, HookDxAddrContent.dwFunAddr, reinterpret_cast<DWORD>(NewPresent));
	while (dwDirectDx9 == NULL && GameRun)
		::Sleep(100);

	MyTools::CLHook::UnHook_Fun_Jmp_MyAddr(&HookDxAddrContent);
	if (dwDirectDx9 == NULL)
		return FALSE;

	dwPresentAddr = ReadDWORD(dwDirectDx9) + 0x44;
	dwPresentValue = ReadDWORD(dwPresentAddr);
	return MyTools::CCharacter::WriteDWORD(dwPresentAddr, reinterpret_cast<DWORD>(NewPresent));
}

BOOL CDxOptimize::GetDLLModule()
{
	while (GameRun && hmDLL == NULL)
	{
		hmDLL = ::GetModuleHandleW(L"d3d9.dll");
		::Sleep(100);
	}
	return hmDLL != NULL;
}

BOOL CDxOptimize::Stop()
{
	if (dwPresentAddr != NULL && dwPresentValue != NULL)
		MyTools::CCharacter::WriteDWORD(dwPresentAddr, dwPresentValue);

	dwPresentAddr = dwPresentValue = NULL;
	return TRUE;
}
