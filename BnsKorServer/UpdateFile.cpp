#include "UpdateFile.h"
#include <iostream>
#include <algorithm>
#include <zlib.h>
#include <MyTools/Character.h>
#include <MyTools/CLPublic.h>
#include <MyTools/CLFile.h>
#include <MyTools/TimeTick.h>
#include <MyTools/Log.h>
#include <MyTools/Script.h>
#include "DailyTask.h"

#pragma comment(lib,"zdll.lib")
#define _SELF L"UpdateFile.cpp"

ExpressScriptList vExpressScriptList;

CUpdateFile::CUpdateFile()
{
}

CUpdateFile::~CUpdateFile()
{
}

BOOL CUpdateFile::UpdateScript()
{
	std::wcout << L"读取脚本中……" << std::endl;
	// 读取所有的*.inf文件
	std::vector<ExpressScriptInfo> vlst;
	GetScriptFile(vlst);
	if (vlst.size() == 0 || !ExistRepeatFile(vlst))
	{
		LOG_MSG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"无法解析到脚本!");
		return FALSE;
	}
	vExpressScriptList.ScriptLock.Access([&vlst]{
		vExpressScriptList.vScript.clear();
		vExpressScriptList.vScript.assign(vlst.begin(), vlst.end());
		vExpressScriptList.fVersion += 0.1f;
	});
	return TRUE;
}

BOOL CUpdateFile::GetScriptFile(_In_ std::vector<ExpressScriptInfo>& vScript)
{
	auto fnReadDirectory = [&vScript](_In_ LPCWSTR pwszDirectory)
	{
		WIN32_FIND_DATA FindData;
		WCHAR wszPath[MAX_PATH] = { 0 };

		::GetCurrentDirectoryW(MAX_PATH, wszPath);
		lstrcatW(wszPath, L"\\TaskScript\\");
		lstrcatW(wszPath, pwszDirectory);
		lstrcatW(wszPath, L"\\*.inf");

		HANDLE hFile = ::FindFirstFileW(wszPath, &FindData);
		if (hFile == INVALID_HANDLE_VALUE)
			return TRUE;

		do
		{
			if (CCharacter::wstrstr_my(FindData.cFileName, L".inf") /*!(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)*/)
			{
				ExpressScriptInfo Esi;
				Esi.wsCapter = pwszDirectory;
				Esi.wsScriptName = FindData.cFileName;
				Esi.dwScriptNameHash = CLPublic::GetHash(Esi.wsScriptName);

				std::wstring wsFileName;
				CCharacter::GetRemoveLeft(Esi.wsScriptName, L".inf", wsFileName);
				if (CCharacter::wstrcmp_my(pwszDirectory, L"日常"))
					CDailyTask::GetInstance().AddDailyTask(wsFileName);

				if (!ReadScriptFile(Esi))
					break;

				vScript.push_back(Esi);
			}
		} while (::FindNextFileW(hFile, &FindData));
		::FindClose(hFile);
		return TRUE;
	};

	CDailyTask::GetInstance().ClearDailyTask();
	vScript.clear();
	return fnReadDirectory(L"1章") && fnReadDirectory(L"2章") && fnReadDirectory(L"3章") && fnReadDirectory(L"4章")
		&& fnReadDirectory(L"5章") && fnReadDirectory(L"6章") && fnReadDirectory(L"7章") && fnReadDirectory(L"初章")
		&& fnReadDirectory(L"简章") && fnReadDirectory(L"日常") && fnReadDirectory(L"8章") && fnReadDirectory(L"Script") && fnReadDirectory(L"一次性");
}

BOOL CUpdateFile::ReadScriptFile(_In_ _Out_ ExpressScriptInfo& Esi)
{
	WCHAR wszPath[MAX_PATH] = { 0 };
	::GetCurrentDirectoryW(MAX_PATH, wszPath);
	lstrcatW(wszPath, L"\\TaskScript\\");
	lstrcatW(wszPath, Esi.wsCapter.c_str());
	lstrcatW(wszPath, L"\\");
	lstrcatW(wszPath, Esi.wsScriptName.c_str());
	
	std::wstring wsContent;
	CLFile::ReadUnicodeFile(wszPath, wsContent);
	wsContent.insert(wsContent.end(), '\0');
	Bytef* source = (Bytef *)wsContent.c_str();
	Esi.dwScriptSize = wsContent.length() * 2 + 2;
	Esi.dwExpressCRC = CLPublic::GetCRC32_DWORD(reinterpret_cast<CHAR*>(source), Esi.dwScriptSize);
	Esi.dwContentLength = compressBound(Esi.dwScriptSize);


	std::shared_ptr<CHAR> pszBuffer(new CHAR[Esi.dwContentLength], [](CHAR* p){ delete[] p; });
	compress((Bytef *)pszBuffer.get(), &Esi.dwContentLength, source, Esi.dwScriptSize);
	//LOG_C(CLog::em_Log_Type_Debug, L"compress Script!");
	Esi.dwCiphertextCRC = CLPublic::GetCRC32_DWORD(pszBuffer.get(), Esi.dwContentLength);
	Esi.pScriptContent = pszBuffer;
	return TRUE;
}

BOOL CUpdateFile::GetExpressScriptInfo(_In_ LPCSTR, _Out_ ExpressScriptInfo&)
{
	/*DWORD dwFileNameCrc = CLPublic::GetCRC32_DWORD(pszFileName, strlen(pszFileName));
	vExpressScriptList.ScriptLock.Access([&Esi, &dwFileNameCrc]{
		auto& itr = std::find_if(vExpressScriptList.vScript.begin(), vExpressScriptList.vScript.end(), [&dwFileNameCrc](ExpressScriptInfo& tmpEsi){
			return tmpEsi.dwScriptNameCrc == dwFileNameCrc;
		});

		if (itr != vExpressScriptList.vScript.end())
			Esi = *itr;
	});*/
	return TRUE;
}

std::wstring CUpdateFile::QueryCapter_By_ExpressScript(_In_ DWORD dwTaskId)
{
	WCHAR wszText[32];
	swprintf_s(wszText, _countof(wszText) - 1, L"%X.inf", dwTaskId);
	DWORD dwScriptNameHash = CLPublic::GetHash(wszText);

	std::wstring wsCapter;
	vExpressScriptList.ScriptLock.Access([dwScriptNameHash, &wsCapter]
	{
		auto p = CLPublic::Vec_find_if(vExpressScriptList.vScript, [dwScriptNameHash](CONST ExpressScriptInfo& itm){ return itm.dwScriptNameHash == dwScriptNameHash; });
		wsCapter = p != nullptr ? p->wsCapter : L"";
	});

	return wsCapter;
}

BOOL CUpdateFile::ExistRepeatFile(_In_ CONST std::vector<ExpressScriptInfo>& VecScriptInfo)
{
	for (auto itr = VecScriptInfo.begin(); itr != VecScriptInfo.end();++itr)
	{
		int nCount = std::count_if(VecScriptInfo.begin(), VecScriptInfo.end(), [itr](CONST ExpressScriptInfo ExpScript){ return ExpScript.dwScriptNameHash == itr->dwScriptNameHash; });
		if (nCount > 1)
		{
			std::wcout << nCount << L"个重复脚本:" << itr->wsScriptName.c_str() << std::endl;
			return FALSE;
		}
	}
	return TRUE;
}

