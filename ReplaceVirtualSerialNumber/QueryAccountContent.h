#include <Windows.h>
#include <string>
#include <vector>
#include "../ConsoleDLL/VirtualSeriaNumberCommon.h"

class CQueryAccountContent
{
public:
	CQueryAccountContent()
	{
		_hFileMap = NULL;
		_pVirtualSeriaNumberShareContent = nullptr;
	}
	~CQueryAccountContent()
	{
		if (_pVirtualSeriaNumberShareContent != nullptr)
		{
			::UnmapViewOfFile(_pVirtualSeriaNumberShareContent);
			_pVirtualSeriaNumberShareContent = nullptr;
		}
		if (_hFileMap != NULL)
		{
			::CloseHandle(_hFileMap);
			_hFileMap = NULL;
		}
	}

	VOID Create()
	{
		_hFileMap = ::OpenFileMapping(FILE_MAP_READ, FALSE, SHARENAME_VIRTUALSERIANUMBER);
		if (_hFileMap == NULL)
		{
			::MessageBoxW(NULL, L"SHARENAME_VIRTUALSERIANUMBER = NULL", L"", NULL);
			return;
		}

		_pVirtualSeriaNumberShareContent = reinterpret_cast<VirtualSeriaNumberShareContent *>(::MapViewOfFile(_hFileMap, FILE_MAP_READ, NULL, NULL, sizeof(VirtualSeriaNumberShareContent)));
		if (_pVirtualSeriaNumberShareContent == nullptr)
		{
			::MessageBoxW(NULL, L"_pVirtualSeriaNumberShareContent = NULL", L"", NULL);
			return;
		}
	}

	std::wstring& Trim_W(std::wstring& s)
	{
		try
		{
			if (s.empty())
				return s;

			std::wstring::iterator c;
			for (c = s.begin(); c != s.end() && iswspace(*c++);); s.erase(s.begin(), --c);

			// Erase whitespace after the string

			for (c = s.end(); c != s.begin() && iswspace(*--c);); s.erase(++c, s.end());
		}
		catch (...)
		{
			
		}
		return s;
	}

	BOOL Query(_In_ CONST std::wstring& wsAccountName, _Out_ AccountVirtualSeriaNumberContent* pContent) CONST
	{
		if (_pVirtualSeriaNumberShareContent == nullptr)
			return FALSE;
		
		for (UINT i = 0;i <_pVirtualSeriaNumberShareContent->uCount; ++i)
		{
			if (std::wstring(_pVirtualSeriaNumberShareContent->AccountContent[i].wsAccountName) == wsAccountName)
			{
				if (pContent != nullptr)
					*pContent = _pVirtualSeriaNumberShareContent->AccountContent[i];

				return TRUE;
			}
		}

		return FALSE;
	}

	VOID SetMacAddr(_In_ BYTE* pMacAddress, _In_ CONST AccountVirtualSeriaNumberContent& Content)
	{
		std::vector<std::wstring> VecMacAddr;
		if (Split(Content.wsMacAddr, L"-", VecMacAddr) < 6)
		{
			MessageBoxW(NULL, Content.wsMacAddr, L"Invalid Mac Addr", NULL);
			return;
		}

		for (int i = 0;i <6; ++i)
		{
			pMacAddress[i] = static_cast<BYTE>(_wtoi(VecMacAddr.at(i).c_str()));
		}
	}

	

	VOID SetDescription(_In_ LPSTR szDescription, _In_ CONST AccountVirtualSeriaNumberContent& Content)
	{
		UnicodeToASCII(Content.wsNetCard, szDescription);
	}

	std::string GetHardSerivaNumber(_In_ CONST AccountVirtualSeriaNumberContent& Content)
	{
		CHAR szText[1024] = { 0 };
		return std::string(UnicodeToASCII(Content.wsHardDiskSeriaNumber, szText));
	}

private:
	UINT Split(_In_ CONST std::wstring& wsText, _In_ CONST std::wstring& wsFormat, _Out_opt_ std::vector<std::wstring>& vlst)
	{
		try
		{
			std::wstring wsTmpText = wsText;
			size_t nIndex = wsTmpText.find(wsFormat);
			while (nIndex != -1)
			{
				vlst.push_back(wsTmpText.substr(0, nIndex));

				wsTmpText = wsTmpText.substr(nIndex + wsFormat.length());
				nIndex = wsTmpText.find(wsFormat);
			}
			if (!wsTmpText.empty() || vlst.size() > 0)
				vlst.push_back(wsTmpText);
		}
		catch (...)
		{
			
		}

		return static_cast<UINT>(vlst.size());
	}

	LPSTR UnicodeToASCII(LPCWSTR lpszWText, LPSTR lpszText)
	{
		__try
		{
			int ansiLen = ::WideCharToMultiByte(CP_ACP, NULL, lpszWText, static_cast<int>(wcslen(lpszWText)), NULL, 0, NULL, NULL);
			char* szAnsi = new char[ansiLen + 1];
			::WideCharToMultiByte(CP_ACP, NULL, lpszWText, static_cast<int>(wcslen(lpszWText)), szAnsi, ansiLen, NULL, NULL);
			szAnsi[ansiLen] = '\0';
			memcpy(lpszText, szAnsi, ansiLen);
			delete[] szAnsi;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{

		}
		return lpszText;
	}
private:
	VirtualSeriaNumberShareContent* _pVirtualSeriaNumberShareContent;
	HANDLE _hFileMap;
};

