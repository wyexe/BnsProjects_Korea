#include "stdafx.h"
#include "GameDetection.h"
#include <MyTools/Log.h>
#include <MyTools/LdrHeader.h>
#include <MyTools/Character.h>

#define _SELF L"GameDetection.cpp"

CGameDetection::~CGameDetection()
{
	Stop();
}

BOOL CGameDetection::Run()
{
	MEMORY_BASIC_INFORMATION mbi = { 0 };
	HMODULE hmNtDLL = ::GetModuleHandleW(L"ntdll.dll");
	HMODULE hmKer32DLL = ::GetModuleHandleW(L"Kernel32.dll");

	ZwQueryVirtualMemoryPtr Ptr = reinterpret_cast<ZwQueryVirtualMemoryPtr>(::GetProcAddress(hmNtDLL, "ZwQueryVirtualMemory"));
	if (Ptr == nullptr)
	{
		LOG_MSG_CF(L"UnExist 'ZwQueryVirtualMemory' in 'ntdll'");
		return FALSE;
	}


	VirtualQueryPtr VqPtr = reinterpret_cast<VirtualQueryPtr>(::GetProcAddress(hmKer32DLL, "VirtualQuery"));
	if (VqPtr == nullptr)
	{
		LOG_MSG_CF(L"UnExist 'VirtualQuery' in 'Kernel32'");
		return FALSE;
	}

	NtQuerySystemInformationPtr QsiPtr = reinterpret_cast<NtQuerySystemInformationPtr>(::GetProcAddress(hmNtDLL, "NtQuerySystemInformation"));
	if (QsiPtr == nullptr)
	{
		LOG_MSG_CF(L"UnExist 'NtQuerySystemInformation' in 'ntdll'");
		return FALSE;
	}


	MyTools::CLdrHeader LdrHeader;
	if (!LdrHeader.InlineHook(Ptr, reinterpret_cast<void **>(&NewZwQueryVirtualMemory), reinterpret_cast<void **>(&_RealZwQueryVirtualMemoryPtr)))
	{
		LOG_MSG_CF(L"Hook ZwQueryVirtualMemory Faild");
		return FALSE;
	}
	if (!LdrHeader.InlineHook(VqPtr, reinterpret_cast<void **>(&NewVirtualQuery), reinterpret_cast<void **>(&_RealVirtualQueryPtr)))
	{
		LOG_MSG_CF(L"Hook VirtualQuery Faild");
		return FALSE;
	}
	if (!LdrHeader.InlineHook(QsiPtr, reinterpret_cast<void **>(&NewNtQuerySystemInformation), reinterpret_cast<void **>(&_RealNtQuerySystemInformationPtr)))
	{
		LOG_MSG_CF(L"Hook NtQuerySystemInformation Faild");
		return FALSE;
	}

	return TRUE;
}

VOID CGameDetection::Stop()
{
	MyTools::CLdrHeader LdrHeader;
	if (_RealZwQueryVirtualMemoryPtr != nullptr)
	{
		LdrHeader.UnInlineHook(NewZwQueryVirtualMemory, _RealZwQueryVirtualMemoryPtr);
		_RealZwQueryVirtualMemoryPtr = nullptr;
	}
	if (_RealVirtualQueryPtr != nullptr)
	{
		LdrHeader.UnInlineHook(NewVirtualQuery, _RealVirtualQueryPtr);
		_RealVirtualQueryPtr = nullptr;
	}
	if (_RealNtQuerySystemInformationPtr != nullptr)
	{
		LdrHeader.UnInlineHook(NewNtQuerySystemInformation, _RealNtQuerySystemInformationPtr);
		_RealNtQuerySystemInformationPtr = nullptr;
	}
}

NTSTATUS WINAPI CGameDetection::NewZwQueryVirtualMemory(_In_ HANDLE ProcessHandle, _In_opt_ PVOID BaseAddress, _In_ MEMORY_INFORMATION_CLASS MemoryInformationClass, _Out_ PVOID MemoryInformation, _In_ SIZE_T MemoryInformationLength, _Out_opt_ PSIZE_T ReturnLength)
{
	if (BaseAddress == 0)
	{
		LOG_CF_E(L"可能是检测[ZwQueryVirtualMemory]...");
		return -1;
	}
	return CGameDetection::GetInstance()._RealZwQueryVirtualMemoryPtr(ProcessHandle, BaseAddress, MemoryInformationClass, MemoryInformation, MemoryInformationLength, ReturnLength);
}

SIZE_T WINAPI CGameDetection::NewVirtualQuery(_In_opt_ LPCVOID lpAddress, _Out_writes_bytes_to_(dwLength, return) PMEMORY_BASIC_INFORMATION lpBuffer, _In_ SIZE_T dwLength)
{
	if (lpAddress == 0)
	{
		LOG_CF_E(L"可能是检测[VirtualQuery]...");
		return 0;
	}
	return CGameDetection::GetInstance()._RealVirtualQueryPtr(lpAddress, lpBuffer, dwLength);
}

NTSTATUS WINAPI CGameDetection::NewNtQuerySystemInformation(_In_ SYSTEM_INFORMATION_CLASS SystemInformationClass, _Inout_ PVOID SystemInformation, _In_ ULONG SystemInformationLength, _Out_opt_ PULONG ReturnLength)
{
	if (SystemInformationClass != SystemProcessInformation)
	{
		return CGameDetection::GetInstance()._RealNtQuerySystemInformationPtr(SystemInformationClass, SystemInformation, SystemInformationLength, ReturnLength);
	}


	NTSTATUS NtRetCode = CGameDetection::GetInstance()._RealNtQuerySystemInformationPtr(SystemInformationClass, SystemInformation, SystemInformationLength, ReturnLength);
	if (NT_SUCCESS(NtRetCode))
	{
		SYSTEM_PROCESS_INFORMATION * pPrevent = NULL;
		for (SYSTEM_PROCESS_INFORMATION * pCurrent = reinterpret_cast<SYSTEM_PROCESS_INFORMATION *>(SystemInformation); pCurrent != nullptr; pCurrent = reinterpret_cast<SYSTEM_PROCESS_INFORMATION *>(reinterpret_cast<PUCHAR>(pCurrent) + pCurrent->NextEntryOffset))
		{
			if (pCurrent->ImageName.Buffer != NULL && reinterpret_cast<DWORD>(pCurrent->UniqueProcessId) != ::GetCurrentProcessId() && MyTools::CCharacter::MakeTextToLower(std::wstring(pCurrent->ImageName.Buffer)) == L"client.exe")
			{
				if (pCurrent->NextEntryOffset == NULL)
				{
					pPrevent->NextEntryOffset = NULL;
					break;
				}
				else
				{
					// Add (+=)... ingnore Current Node
					pPrevent->NextEntryOffset += pCurrent->NextEntryOffset;
					continue;
				}
			}
			else if (pCurrent->NextEntryOffset == NULL)
			{
				break;
			}


			pPrevent = pCurrent;
		}
	}
	return NtRetCode;
}
