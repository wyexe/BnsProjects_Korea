#include "ScriptFile.h"
#include <iostream>
#include <MyTools/CLFile.h>
#include <MyTools/CLPublic.h>
#include <MyTools/Character.h>
#include <MyTools/Script.h>
#include <MyTools/CLEchoException.h>
#include <MyTools/Log.h>
#include "zlib.h"
#include "DailyTask.h"

#pragma comment(lib,"zdll.lib")

#define _SELF L"ScriptFile.cpp"
VOID CScriptFile::Initialize()
{
	ReadScriptFileToMemory();
}

BOOL CScriptFile::ReadScriptFileToMemory()
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&] 
	{
		std::wcout << L"Reading Script ..." << std::endl;

		CONST static std::vector<std::wstring> VecDirectory =
		{
			L"1ÕÂ",L"2ÕÂ",L"3ÕÂ",L"4ÕÂ",L"5ÕÂ",L"6ÕÂ",L"7ÕÂ",L"³õÕÂ",L"¼òÕÂ",L"ÈÕ³£",L"8ÕÂ",L"Script",L"Ò»´ÎÐÔ"
		};

		BOOL bRetCode = FALSE;
		_Lock.Access([this, &bRetCode]
		{
			bRetCode = TraverseDirectory(VecDirectory);
			if (!bRetCode)
				_MapScript.clear();
		});

		if (!bRetCode)
		{
			std::wcout << L"Read Script File Faild!" << std::endl;
			::Sleep(INFINITE);
			return FALSE;
		}

		std::wcout << L"Done!" << std::endl;

		return ReLoadDailyTask();
	});
}

BOOL CScriptFile::FindScriptText_By_ScriptName(_In_ CONST std::wstring& wsScriptName, _Out_opt_ CipherScriptText& Content) CONST
{
	BOOL bExist = FALSE;
	_Lock.Access([&] 
	{
		auto itr = _MapScript.find(wsScriptName);
		if (itr != _MapScript.end())
		{
			bExist = TRUE;
			Content = itr->second;
		}
	});

	return bExist;
}

BOOL CScriptFile::TraverseDirectory(_In_ CONST std::vector<std::wstring> VecDirectory)
{
	_bRepeat = FALSE;
	_MapScript.clear();
	for (CONST auto& itm : VecDirectory)
	{
		WIN32_FIND_DATA FindData;
		WCHAR wszPath[MAX_PATH] = { 0 };

		::GetCurrentDirectoryW(MAX_PATH, wszPath);
		lstrcatW(wszPath, L"\\TaskScript\\");
		lstrcatW(wszPath, itm.c_str());
		lstrcatW(wszPath, L"\\*.inf");
		std::wcout << wszPath << std::endl;
		HANDLE hFile = ::FindFirstFileW(wszPath, &FindData);
		if (hFile == INVALID_HANDLE_VALUE)
			continue;

		do
		{
			if (MyTools::CCharacter::wstrstr_my(FindData.cFileName, L".inf") /*!(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)*/)
			{
				CipherScriptText Content;
				Content.wsScriptName = FindData.cFileName;
				Content.wsCapter = itm;


				WCHAR wszScriptFilePath[MAX_PATH] = { 0 };
				::GetCurrentDirectoryW(MAX_PATH, wszScriptFilePath);
				if (!ReadScriptFileContent(MyTools::CCharacter::MakeFormatText(L"%s\\TaskScript\\%s\\%s", wszScriptFilePath, itm.c_str(), Content.wsScriptName.c_str()), Content))
					return FALSE;
			}
		} while (::FindNextFileW(hFile, &FindData));
		::FindClose(hFile);

		std::wcout << L"Read Directory[" << itm.c_str() << L"] Script Content Done!" << std::endl;
	}
	return !TestRepeatScriptFile() && TestEffectiveScriptContent();
}

BOOL CScriptFile::ReadScriptFileContent(_In_ CONST std::wstring& wsPath, _In_ _Out_ CipherScriptText& Content)
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		std::wstring wsContent;
		if (!MyTools::CLFile::ReadUnicodeFile(wsPath, wsContent))
		{
			std::wcout << L"File '" << wsPath << L"' is Empty!!!" << std::endl;
			return FALSE;
		}

		wsContent.insert(wsContent.end(), '\0');
		auto ExpressBuffer = reinterpret_cast<CONST Bytef *>(wsContent.c_str());

		std::shared_ptr<WCHAR> pExpressScript(new WCHAR[wsContent.length()], [](WCHAR* p) {delete[] p; });
		memcpy(pExpressScript.get(), wsContent.c_str(), wsContent.length() * sizeof(WCHAR));

		Content.pExpressScript = pExpressScript;
		Content.uExpressSize = wsContent.length() * sizeof(WCHAR) + 2;
		Content.dwExpressTextCrc = MyTools::CLPublic::GetCRC32_DWORD(reinterpret_cast<LPCSTR>(ExpressBuffer), Content.uExpressSize);
		Content.uCipherTextSize = compressBound(Content.uExpressSize);

		std::shared_ptr<CHAR> Buffer(new CHAR[Content.uCipherTextSize], [](CHAR* p) { delete[] p; });
		compress(reinterpret_cast<Bytef *>(Buffer.get()), reinterpret_cast<uLongf *>(&Content.uCipherTextSize), ExpressBuffer, Content.uExpressSize);
		Content.dwCipherTextCrc = MyTools::CLPublic::GetCRC32_DWORD(Buffer.get(), Content.uCipherTextSize);
		Content.pScriptBuffer = std::move(Buffer);

		if (_MapScript.find(Content.wsScriptName) != _MapScript.end())
		{
			std::wcout << L"Script File Repat is:" << wsPath.c_str() << std::endl;
			_bRepeat = TRUE;
			return FALSE;
		}
		_MapScript.insert(std::make_pair(Content.wsScriptName, std::move(Content)));
		return TRUE;
	});
}

BOOL CScriptFile::ReLoadDailyTask() CONST
{
	return MyTools::CLEchoException::GetInstance().InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		std::vector<std::wstring> Vec;
		_Lock.Access([this, &Vec]
		{
			for (CONST auto& itm : _MapScript)
			{
				if (itm.second.wsCapter != L"ÈÕ³£")
					continue;

				Vec.push_back(itm.second.wsScriptName);
			}
		});

		CDailyTask::GetInstance().ReFillVecDailyTaskName(Vec);
		return TRUE;
	});
}

BOOL CScriptFile::TestRepeatScriptFile()
{
	return _bRepeat;
}

BOOL CScriptFile::TestEffectiveScriptContent() CONST
{
	std::wcout << L"Test Effective Script ..." << std::endl;
	BOOL bRetCode = TRUE;
#ifndef _DEBUG
	for (CONST auto& itm : _MapScript)
	{
		MyTools::CScript Script;
		if (!Script.Read(MyTools::CScript::em_ReadScript_Type::em_ReadScript_Type_Text, itm.second.pExpressScript.get()))
		{
			std::wcout << L"Read Script:[" << itm.second.wsScriptName.c_str() << L"] Faild!!!" << std::endl;
			bRetCode = FALSE;
			break;
		}
	}
#endif // !_DEBUG
	std::wcout << L"Test Effective Script" << (bRetCode ? L"Done!" : L"Faild") << std::endl;
	return bRetCode;
}

BOOL CScriptFile::FindCapterByTaskId(_In_ DWORD dwTaskId, _Out_ std::wstring& wsCapter) CONST
{
	BOOL bExist = FALSE;
	_Lock.Access([&]
	{
		auto itr = _MapScript.find(MyTools::CCharacter::MakeFormatText(L"%X.inf", dwTaskId));
		if (itr != _MapScript.end())
		{
			bExist = TRUE;
			wsCapter = itr->second.wsCapter;
		}
	});

	return bExist;
}

VOID CScriptFile::Print() CONST
{
	_Lock.Access([=] 
	{
		LOG_CF_D(L"Script.size=%d", _MapScript.size());
	});
}
