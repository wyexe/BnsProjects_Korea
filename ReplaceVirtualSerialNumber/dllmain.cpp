// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <windows.h>
#include <winioctl.h>
#include <iphlpapi.h>
#include <vector>
#include <mutex>
#include "InlineHook.h"
#include "QueryAccountContent.h"

CQueryAccountContent QueryAccountContent;
AccountVirtualSeriaNumberContent VirtualSeriaNumberContent;
namespace pervent_ban_account
{
	typedef BOOL (WINAPI *FnType_DeviceIoControl)(
			_In_ HANDLE hDevice,
			_In_ DWORD dwIoControlCode,
			_In_reads_bytes_opt_(nInBufferSize) LPVOID lpInBuffer,
			_In_ DWORD nInBufferSize,
			_Out_writes_bytes_to_opt_(nOutBufferSize, *lpBytesReturned) LPVOID lpOutBuffer,
			_In_ DWORD nOutBufferSize,
			_Out_opt_ LPDWORD lpBytesReturned,
			_Inout_opt_ LPOVERLAPPED lpOverlapped
			);

	typedef ULONG (WINAPI *FnType_GetAdaptersInfo)(
			_Out_writes_bytes_opt_(*SizePointer) PIP_ADAPTER_INFO AdapterInfo,
			_Inout_                         PULONG           SizePointer
			);

	static FnType_DeviceIoControl real_DeviceIoControl = NULL;
	static FnType_GetAdaptersInfo real_GetAdaptersInfo = NULL;

	int GetRand(int nMax, int nMin)
	{
		static DWORD dwSeed = ::GetTickCount();
		srand(dwSeed);
		dwSeed = rand();
		return (nMin + rand() % (nMax - nMin + 1));
	}

	namespace hd_serial_process
	{
		BOOL RetrieveIdeInfoByIoControlData(LPVOID pOutData, DWORD dwSize, std::string & serial_num, std::string & model_num, std::string & revision_num)
		{
			if (dwSize != 0x210)
			{
				//这里我的电脑是210
				//OutputDebugStr(L"!!!硬盘信息输出BUFFER 大小 != 210 :%d", dwSize);
				char szText[1024] = { 0 };
				wsprintfA(szText, "硬盘信息输出BUFFER 大小 != 210 :%d", dwSize);
				::MessageBoxA(nullptr, szText, "serial_num", NULL);
				return FALSE;
			}
			auto fn_retrieve_ide_info = [](DWORD diskdata[256], std::string & hd_serial_num, std::string & hd_name, std::string & revision)
			{
				auto fn_ConvertToString = [](DWORD disk_data[256], int first_index, int last_index, char * buf)
				{
					int index = 0;
					int position = 0;
					for (index = first_index; index <= last_index; index++)
					{
						buf[position++] = (char)(disk_data[index] / 256);
						buf[position++] = (char)(disk_data[index] % 256);
					}

					buf[position] = '\0';
					for (index = position - 1; index > 0 && isspace(buf[index]); index--)
						buf[index] = '\0';

					return buf;
				};

				char serialNumber[1024] = { 0 }; //硬盘序列号
				char modelNumber[1024] = { 0 };  //硬盘名称
				char revisionNumber[1024] = { 0 }; //修订号
												   //char bufferSize[32]; //缓存字节数
				fn_ConvertToString(diskdata, 10, 19, serialNumber);
				fn_ConvertToString(diskdata, 27, 46, modelNumber);
				fn_ConvertToString(diskdata, 23, 26, revisionNumber);

				hd_serial_num = serialNumber;
				hd_name = modelNumber;
				revision = revisionNumber;
			};

			DWORD disk_data[256] = { 0 };
			USHORT *pIdSector = (USHORT *)
				((PSENDCMDOUTPARAMS)pOutData)->bBuffer;
			for (int k = 0; k < 256; k++)
				disk_data[k] = pIdSector[k];
			fn_retrieve_ide_info(disk_data, serial_num, model_num, revision_num);
			return TRUE;
		}


		void ChangeHeadInfoDataToNewString(DWORD diskdata[256], int first_index, int last_index, const  std::string & new_str)
		{
			int index = first_index;
			for (int nstr_index = 0; index <= last_index; index++, nstr_index++)
			{
				BYTE * p1 = ((BYTE*)&diskdata[index]) + 0;
				BYTE * p2 = ((BYTE*)&diskdata[index]) + 1;
				DWORD new_str_index = nstr_index * 2;
				if (new_str_index > new_str.length() - 1)
					break;
				*p2 = new_str[new_str_index];
				if (new_str_index + 1 > new_str.length() - 1)
					break;
				*p1 = new_str[new_str_index + 1];
			}
		};

		//篡改游戏获取到的硬盘数据信息
		BOOL ReplaceHdSerialInfo(_In_ LPVOID pOutData, _In_ DWORD, _In_ const std::string & my_hd_serial)
		{
			DWORD disk_data[256] = { 0 };
			USHORT *pIdSector = (USHORT *)
				((PSENDCMDOUTPARAMS)pOutData)->bBuffer;
			for (int k = 0; k < 256; k++)
				disk_data[k] = pIdSector[k];
			ChangeHeadInfoDataToNewString(disk_data, 10, 19, my_hd_serial);

			for (int k = 0; k < 256; k++)
				pIdSector[k] = static_cast<USHORT>(disk_data[k]);
			return TRUE;
		}


		BOOL WINAPI My_DeviceIoControl(
				_In_ HANDLE hDevice,
				_In_ DWORD dwIoControlCode,
				_In_reads_bytes_opt_(nInBufferSize) LPVOID lpInBuffer,
				_In_ DWORD nInBufferSize,
				_Out_writes_bytes_to_opt_(nOutBufferSize, *lpBytesReturned) LPVOID lpOutBuffer,
				_In_ DWORD nOutBufferSize,
				_Out_opt_ LPDWORD lpBytesReturned,
				_Inout_opt_ LPOVERLAPPED lpOverlapped)
		{
			//先让执行原来函数DeviceIoControl
			const BOOL api_ret = real_DeviceIoControl(hDevice, dwIoControlCode, lpInBuffer, nInBufferSize, lpOutBuffer, nOutBufferSize, lpBytesReturned, lpOverlapped);
			if (!api_ret)
				return api_ret;

			if (dwIoControlCode == SMART_RCV_DRIVE_DATA)
			{

				std::string serial_num, model_num, reversion_num;
				if (!RetrieveIdeInfoByIoControlData(lpOutBuffer, *lpBytesReturned, serial_num, model_num, reversion_num))
					return api_ret;

				ReplaceHdSerialInfo(lpOutBuffer, *lpBytesReturned, QueryAccountContent.GetHardSerivaNumber(VirtualSeriaNumberContent));
			}

			return api_ret;
		}
	}

	namespace net_card_process
	{

		//篡改网卡信息
		void ProcessAdaptersInfo(PIP_ADAPTER_INFO AdapterInfo, int)
		{
			//修改MAC
			//00-FF-1A-72-4E-5E
			QueryAccountContent.SetMacAddr(AdapterInfo->Address, VirtualSeriaNumberContent);

			//篡改网卡描述信息 Realtek PCIe GBE Family Controller
			QueryAccountContent.SetDescription(AdapterInfo->Description, VirtualSeriaNumberContent);

			//修改IP地址
			static std::string local_ip_addr = []()
			{
				char buff[256] = { 0 };
				sprintf_s(buff, "192.168.0.%d", GetRand(255, 0));
				return std::string(buff);
			}();
			strcpy_s(AdapterInfo->IpAddressList.IpAddress.String, local_ip_addr.c_str());

			//修改网关地址
			static std::string local_gateway = []()
			{
				char buff[256] = { 0 };
				sprintf_s(buff, "192.168.0.%d", GetRand(255, 0));
				return std::string(buff);
			}();
			strcpy_s(AdapterInfo->GatewayList.IpAddress.String, local_gateway.c_str());
		}



		ULONG WINAPI my_GetAdaptersInfo( __out_bcount_opt(*SizePointer)  PIP_ADAPTER_INFO AdapterInfo, __inout PULONG SizePointer)
		{
			ULONG api_ret = real_GetAdaptersInfo(AdapterInfo, SizePointer);
			if (api_ret == ERROR_SUCCESS)
			{
				PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
				int index = 0;
				do {
					if (strcmp(pAdapterInfo->GatewayList.IpAddress.String, "0.0.0.0") != 0)
						ProcessAdaptersInfo(pAdapterInfo, index);
					pAdapterInfo = pAdapterInfo->Next;    // Progress through linked list
					index++;
				} while (pAdapterInfo);                    // Terminate if last adapter
			}

			return api_ret;
		}
	}
}

VOID Hook()
{
	std::wstring wsCmdLine = ::GetCommandLineW();
	auto Index = wsCmdLine.find(L"Account:");
	if (Index == -1)
		return;

	QueryAccountContent.Create();
	auto wsAccountName = wsCmdLine.substr(Index + std::wstring(L"Account:").length());
	wsAccountName = QueryAccountContent.Trim_W(wsAccountName);
	if (QueryAccountContent.Query(wsAccountName, &VirtualSeriaNumberContent ))
	{
		pervent_ban_account::real_DeviceIoControl = (pervent_ban_account::FnType_DeviceIoControl)(GetProcAddress(LoadLibraryW(L"Kernel32.dll"), "DeviceIoControl"));
		pervent_ban_account::real_GetAdaptersInfo = (pervent_ban_account::FnType_GetAdaptersInfo)(GetProcAddress(LoadLibraryW(L"Iphlpapi.dll"), "GetAdaptersInfo"));
		if (pervent_ban_account::real_GetAdaptersInfo == NULL)
		{
			::MessageBoxW(NULL, L"UnExist Iphlpapi.dll", L"", NULL);
			return;
		}
		if (pervent_ban_account::real_DeviceIoControl == NULL)
		{
			::MessageBoxW(NULL, L"UnExist Kernel32.dll", L"", NULL);
			return;
		}
		InlineHook::InlineHook((VOID*)(GetProcAddress(LoadLibraryW(L"Kernel32.dll"), "DeviceIoControl")), (VOID*)(pervent_ban_account::hd_serial_process::My_DeviceIoControl), (VOID **)&pervent_ban_account::real_DeviceIoControl);
		InlineHook::InlineHook((VOID*)(GetProcAddress(LoadLibraryW(L"Iphlpapi.dll"), "GetAdaptersInfo")), (VOID*)(pervent_ban_account::net_card_process::my_GetAdaptersInfo), (VOID **)&pervent_ban_account::real_GetAdaptersInfo);
	}
	else
	{
		WCHAR wszText[1024] = { 0 };
		wsprintfW(wszText, L"UnExist Account[%s]", wsAccountName.c_str());
		::MessageBoxW(NULL, wszText, L"Error", NULL);


	}
	
}

BOOL APIENTRY DllMain( HMODULE,
                       DWORD  ul_reason_for_call,
                       LPVOID
					 )
{
	static std::once_flag HookFlag;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		std::call_once(HookFlag, Hook);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

