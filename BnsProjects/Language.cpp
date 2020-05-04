#include "stdafx.h"
#include "Language.h"
#include <MyTools/CLFile.h>
#include <MyTools/Log.h>

#define _SELF L"Language.cpp"
CLanguage::CLanguage()
{
	auto ulTick = ::GetTickCount64();
#if BnsProject_Dlg_China
	static std::wstring wsChinaText;
	CLFile::ReadUnicodeFile(L"C:\\国服资源名字.txt", wsChinaText);
	InsertKeyValueMap(Map_Language_Local, wsChinaText);

	static std::wstring wsKoreaText;
	CLFile::ReadUnicodeFile(L"C:\\韩服资源名字.txt", wsKoreaText);
	InsertValueKeyMap(Map_Language_Other, wsKoreaText);
#else
	static std::wstring wsChinaText;
	MyTools::CLFile::ReadUnicodeFile(L"C:\\韩服资源名字.txt", wsChinaText);
	InsertKeyValueMap(Map_Language_Local, wsChinaText);

	static std::wstring wsKoreaText;
	MyTools::CLFile::ReadUnicodeFile(L"C:\\国服资源名字.txt", wsKoreaText);
	InsertValueKeyMap(Map_Language_Other, wsKoreaText);
#endif
	LOG_CF_D(L"读取资源文件花费%dms", static_cast<DWORD>(::GetTickCount64() - ulTick));
}

VOID CLanguage::InsertKeyValueMap(_In_ std::map<std::wstring, std::vector<stringView>>& Mp, _In_ CONST std::wstring& wsText)
{
	UINT uTextIndex = 0;
	while (uTextIndex < wsText.length())
	{
		auto uIndex = wsText.find_first_of(L"\r\n", uTextIndex);
		if (uIndex == -1)
			break;

		// Test--aaaaaaaa\r\n…………………………
		auto nSplitTextIndex = wsText.find_first_of(L"--", uTextIndex);
		if (nSplitTextIndex != -1)
		{
			// Key
			std::wstring wsKeyName = wsText.substr(uTextIndex, nSplitTextIndex - uTextIndex);

			// Value
			stringView stringView_(wsText);
			stringView_.uIndex = nSplitTextIndex + 2;
			stringView_.uLength = (uIndex - uTextIndex)/*Line Length*/ - wsKeyName.length()/*keyName*/ - 2/*--*/;

			auto itr = Mp.find(wsKeyName);
			if (itr == Mp.end())
			{
				std::vector<stringView> Vec;
				Vec.push_back(std::move(stringView_));
				Mp.insert(std::make_pair(std::move(wsKeyName), std::move(Vec)));
			}
			else
			{
				itr->second.push_back(std::move(stringView_));
			}
		}

		uTextIndex = uIndex + 2/*\r\n*/;
	}
}

VOID CLanguage::InsertValueKeyMap(_In_ std::map<std::wstring, std::vector<stringView>>& Mp, _In_ CONST std::wstring& wsText)
{
	UINT uTextIndex = 0;
	while (uTextIndex < wsText.length())
	{
		auto uIndex = static_cast<int>(wsText.find_first_of(L"\r\n", uTextIndex));
		if (uIndex == -1)
			break;

		// Test--aaaaaaaa\r\n…………………………
		auto nSplitTextIndex = static_cast<int>(wsText.find_first_of(L"--", uTextIndex));
		if (nSplitTextIndex != -1 && uIndex - nSplitTextIndex - 2 > 0)
		{
			// Key
			std::wstring wsKeyName = wsText.substr(nSplitTextIndex + 2, uIndex - nSplitTextIndex - 2);
			// Value
			stringView stringView_(wsText);
			stringView_.uIndex = uTextIndex;
			stringView_.uLength = nSplitTextIndex - uTextIndex;

			auto itr = Mp.find(wsKeyName);
			if (itr == Mp.end())
			{
				std::vector<stringView> Vec;
				Vec.push_back(std::move(stringView_));
				Mp.insert(std::make_pair(std::move(wsKeyName), std::move(Vec)));
			}
			else
			{
				itr->second.push_back(std::move(stringView_));
			}
		}

		uTextIndex = uIndex + 2/*\r\n*/;
	}
}

CONST std::wstring CLanguage::GetOtherText_By_LocalText(_In_ CONST std::wstring& wsText)
{
	if (wsText == L"")
		return std::wstring(L"Empty");

	auto itr = Map_Language_Local.find(wsText);
	if (itr == Map_Language_Local.end())
		return  std::wstring(L"LocalEmpty");

	auto KoreaItr = Map_Language_Other.end();
	BOOL bRetCode = MyTools::CLPublic::Vec_find_if(itr->second, [&KoreaItr, this](CONST stringView& stringView_)
	{
		KoreaItr = Map_Language_Other.find(stringView_.GetText());
		return KoreaItr != Map_Language_Other.end();
	}) != nullptr;

	if (!bRetCode)
		return std::wstring(L"OtherEmpty");

	return KoreaItr->second.begin()->GetText();
}
