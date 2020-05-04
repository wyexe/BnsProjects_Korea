// BnsProjects.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <MyTools/CLPublic.h>
#include <MyTools/Character.h>

extern "C" __declspec(dllexport) VOID WINAPIV GetComipleTime(_Out_ LPWSTR pwszText)
{
	MyTools::CCharacter::wstrcpy_my(pwszText, MyTools::CLPublic::GetCompileTime().c_str());
}