#include "stdafx.h"
#include "CmdLogService.h"
#include <MyTools/Log.h>
#include "ConsoleFun.h"
#include "SortWindowService.h"

#define _SELF L"ConsoleCommand.cpp"
std::vector<MyTools::ExpressionFunPtr>& CCmdLogService::GetVec()
{
	static  std::vector<MyTools::ExpressionFunPtr> Vec = 
	{
		{ std::bind(&CCmdLogService::Help, &CCmdLogService::GetInstance(),std::placeholders::_1), L"Help" },
		{ std::bind(&CCmdLogService::SetValue, &CCmdLogService::GetInstance(),std::placeholders::_1), L"SetValue" },
	};

	return Vec;
}

VOID CCmdLogService::Release()
{

}

VOID CCmdLogService::Help(CONST std::vector<std::wstring>&)
{
	for (CONST auto& itm : GetVec())
		LOG_CF_D(L"Name=%s", itm.wsFunName.c_str());
}

VOID CCmdLogService::SetValue(CONST std::vector<std::wstring>& Vec)
{
	if (Vec.size() < 1)
	{
		LOG_CF_D(L"Vec.size=0");
		return;
	}

	struct SetValueMethodPtr
	{
		std::wstring				wsCmd;
		std::function<VOID(bool)>	MethodPtr;
	};

	CONST static std::map<std::wstring, std::function<VOID(bool)>> MapSetValueMethodPtr = 
	{
		{ L"SetPrintSortBnsWinValue",std::bind(&CSortWindowService::SetPrintSortBnsWin,MyTools::InvokeClassPtr<CSortWindowService>(), std::placeholders::_1) },
	};

	auto itr = MapSetValueMethodPtr.find(Vec.at(0));
	if (itr != MapSetValueMethodPtr.end())
	{
		itr->second(Vec.at(0) == L"0" ? false : true);
	}
}
