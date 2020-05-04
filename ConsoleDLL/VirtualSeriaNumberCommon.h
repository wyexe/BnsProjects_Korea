#include <Windows.h>
#include <string>

#define SHARENAME_VIRTUALSERIANUMBER L"CL_VIR_SER"
#define MAX_ACCOUNT_VIRTUALSERIANUMBER 1000

struct AccountVirtualSeriaNumberContent
{
	WCHAR wsAccountName[64];
	WCHAR wsMacAddr[32];
	WCHAR wsNetCard[64];
	WCHAR wsHardDiskSeriaNumber[32];
};

struct VirtualSeriaNumberShareContent
{
	AccountVirtualSeriaNumberContent AccountContent[MAX_ACCOUNT_VIRTUALSERIANUMBER];
	UINT uCount;
};