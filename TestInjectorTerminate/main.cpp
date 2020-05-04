#include <iostream>
#include <Windows.h>
#include <winternl.h>
#include <MyTools/LdrHeader.h>
#include <MyTools/Character.h>
#include <MyTools/CLProcess.h>
#pragma comment(lib,"ntdll.lib")


using NtQuerySystemInformationPtr = NTSTATUS(WINAPI *)(
	_In_      SYSTEM_INFORMATION_CLASS SystemInformationClass,
	_Inout_   PVOID                    SystemInformation,
	_In_      ULONG                    SystemInformationLength,
	_Out_opt_ PULONG                   ReturnLength);

NtQuerySystemInformationPtr RealNtQuerySystemInformationPtr = nullptr;

NTSTATUS WINAPI NewNtQuerySystemInformation(_In_      SYSTEM_INFORMATION_CLASS SystemInformationClass,
	_Inout_   PVOID                    SystemInformation,
	_In_      ULONG                    SystemInformationLength,
	_Out_opt_ PULONG                   ReturnLength)
{
	if (SystemInformationClass != SystemProcessInformation)
	{
		return RealNtQuerySystemInformationPtr(SystemInformationClass, SystemInformation, SystemInformationLength, ReturnLength);
	}


	NTSTATUS NtRetCode = RealNtQuerySystemInformationPtr(SystemInformationClass, SystemInformation, SystemInformationLength, ReturnLength);
	if (NT_SUCCESS(NtRetCode))
	{
		SYSTEM_PROCESS_INFORMATION * pPrevent = NULL;
		for (SYSTEM_PROCESS_INFORMATION * pCurrent = (SYSTEM_PROCESS_INFORMATION *)SystemInformation; ; pCurrent = (SYSTEM_PROCESS_INFORMATION *)((PUCHAR)pCurrent + pCurrent->NextEntryOffset))
		{
			DWORD dwPid = ::GetCurrentProcessId();
			if (dwPid == (DWORD)pCurrent->UniqueProcessId)
			{
				std::cout << "" << std::endl;
			}
			if (pCurrent->ImageName.Buffer != NULL && MyTools::CCharacter::MakeTextToLower(std::wstring(pCurrent->ImageName.Buffer)) == L"chrome.exe" )
			{
				
				//std::wcout << pCurrent->ImageName.Buffer << std::endl;
				if (pCurrent->NextEntryOffset == NULL)
				{
					pPrevent->NextEntryOffset = NULL;
					break;
				}
				else
				{
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

void StartDaemonScan()
{
	NtQuerySystemInformationPtr Ptr = (NtQuerySystemInformationPtr)::GetProcAddress(::GetModuleHandleW(L"ntdll.dll"), "NtQuerySystemInformation");

	MyTools::CLdrHeader LdrHeader;
	LdrHeader.InlineHook(Ptr, reinterpret_cast<void **>(&NewNtQuerySystemInformation), reinterpret_cast<void **>(&RealNtQuerySystemInformationPtr));
}

int main(int nArgsCount, char** pCmd)
{
	StartDaemonScan();

	NTSTATUS status;
	PVOID buffer;

	typedef SYSTEM_PROCESS_INFORMATION* PSYSTEM_PROCESS_INFO;
	PSYSTEM_PROCESS_INFO spi;

	buffer = VirtualAlloc(NULL, 1024 * 1024, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE); // We need to allocate a large buffer because the process list can be large.

	if (!buffer)
	{
		printf("Error: Unable to allocate memory for process list (%d)", GetLastError());
		return -1;
	}

	printf("Process list allocated at address %p", buffer);
	spi = (PSYSTEM_PROCESS_INFO)buffer;

	if (!NT_SUCCESS(status = NtQuerySystemInformation(SystemProcessInformation, spi, 1024 * 1024, NULL)))
	{
		printf("LOG_TO_CONSOLEError: Unable to query process list (%x)", status);

		VirtualFree(buffer, 0, MEM_RELEASE);
		return -1;
	}

	for (PSYSTEM_PROCESS_INFO pCurrent = (PSYSTEM_PROCESS_INFO)spi; ; pCurrent = (PSYSTEM_PROCESS_INFO)((PUCHAR)pCurrent + pCurrent->NextEntryOffset))
	{
		if (pCurrent->ImageName.Buffer != NULL)
		{
			std::wcout << pCurrent->ImageName.Buffer << std::endl;
		}

		if (pCurrent->NextEntryOffset == NULL)
		{
			break;
		}
	}

	MyTools::CLProcess::Is_Exist_Process_For_ProcName(L"chrome.exe");


	printf("LOG_TO_CONSOLEPress any key to continue.LOG_TO_CONSOLE");
	getchar();

	VirtualFree(buffer, 0, MEM_RELEASE); // Free the allocated buffer.
	
	return 0;
}