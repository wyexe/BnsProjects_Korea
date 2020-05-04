#ifndef __GITBNSPROJECTS_BNSPROJECTS_GAME_GAMELOWDESIGN_GAMEDETECTION_H__
#define __GITBNSPROJECTS_BNSPROJECTS_GAME_GAMELOWDESIGN_GAMEDETECTION_H__

#include <winternl.h>
#include <MyTools/ClassInstance.h>

class CGameDetection : public MyTools::CClassInstance<CGameDetection>
{
private:
	typedef enum _MEMORY_INFORMATION_CLASS {
		MemoryBasicInformation
	} MEMORY_INFORMATION_CLASS;

	using ZwQueryVirtualMemoryPtr = NTSTATUS(WINAPI *)(
		_In_      HANDLE                   ProcessHandle,
		_In_opt_  PVOID                    BaseAddress,
		_In_      MEMORY_INFORMATION_CLASS MemoryInformationClass,
		_Out_     PVOID                    MemoryInformation,
		_In_      SIZE_T                   MemoryInformationLength,
		_Out_opt_ PSIZE_T                  ReturnLength
		);

	using VirtualQueryPtr = SIZE_T(WINAPI *)(
		_In_opt_ LPCVOID lpAddress,
		_Out_writes_bytes_to_(dwLength, return) PMEMORY_BASIC_INFORMATION lpBuffer,
		_In_ SIZE_T dwLength
		);

	using NtQuerySystemInformationPtr = NTSTATUS(WINAPI *)(
		_In_      SYSTEM_INFORMATION_CLASS SystemInformationClass,
		_Inout_   PVOID                    SystemInformation,
		_In_      ULONG                    SystemInformationLength,
		_Out_opt_ PULONG                   ReturnLength);
public:
	CGameDetection () = default;
	~CGameDetection ();

	BOOL Run();

	VOID Stop();
private:
	static NTSTATUS WINAPI NewZwQueryVirtualMemory(_In_ HANDLE ProcessHandle, _In_opt_ PVOID BaseAddress, _In_ MEMORY_INFORMATION_CLASS MemoryInformationClass, _Out_ PVOID MemoryInformation, _In_ SIZE_T MemoryInformationLength, _Out_opt_ PSIZE_T ReturnLength);

	static SIZE_T WINAPI NewVirtualQuery(_In_opt_ LPCVOID lpAddress, _Out_writes_bytes_to_(dwLength, return) PMEMORY_BASIC_INFORMATION lpBuffer, _In_ SIZE_T dwLength);

	static NTSTATUS WINAPI NewNtQuerySystemInformation(_In_ SYSTEM_INFORMATION_CLASS SystemInformationClass, _Inout_ PVOID SystemInformation, _In_ ULONG SystemInformationLength, _Out_opt_ PULONG ReturnLength);
private:
	ZwQueryVirtualMemoryPtr _RealZwQueryVirtualMemoryPtr = nullptr;
	VirtualQueryPtr			_RealVirtualQueryPtr = nullptr;
	NtQuerySystemInformationPtr _RealNtQuerySystemInformationPtr = nullptr;
};



#endif // !__GITBNSPROJECTS_BNSPROJECTS_GAME_GAMELOWDESIGN_GAMEDETECTION_H__
