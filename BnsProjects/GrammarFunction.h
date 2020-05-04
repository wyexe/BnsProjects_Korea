#ifndef __BNS_GRAMMARFUNCTION_H__
#define __BNS_GRAMMARFUNCTION_H__

#include "GameVariable.h"
#include <MyTools/CLGrammar.h>

#define MAX_RETMSG_SIZE	 1024
class CGrammarFunction
{
public:
	explicit CGrammarFunction();
	~CGrammarFunction();

	// SetSpeed 10
	static DWORD WINAPI SetSpeed(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg);

	// HookMianTask -1 -1
	static DWORD WINAPI HookMainTask(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg);

	// HookBLTask -1 -1
	static DWORD WINAPI HookBLTask(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg);

	// GetCityId
	static DWORD WINAPI GetCityId(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg);

	// TakeItem -Npc -100.0
	static DWORD WINAPI TakeItem(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg);

	// SumitMianTask
	static DWORD WINAPI SumitMainTask(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg);

	// PickBLTask
	static DWORD WINAPI PickBLTask(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg);

	// HookEquiEvolution
	static DWORD WINAPI HookEquiEvolution(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg);

	// SearchMachindeCode
	static DWORD WINAPI SearchMachineCode(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg);

	// SetUserName
	static DWORD WINAPI SetAccountName(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg);

	// SearchBase
	static DWORD WINAPI SearchMemBase(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg);

	// Hook Telport
	static DWORD WINAPI HookTelport(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg);

	// Telport
	static DWORD WINAPI Telport(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg);

	// HookSkill
	static DWORD WINAPI HookMonsterSkill(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg);

	// DeletePlayer
	static DWORD WINAPI DeletePlayer(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg);

	// HookAddSkill
	static DWORD WINAPI HookAddSkill(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg);

	// CreatePlayer
	static DWORD WINAPI CreatePlayer(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg);

	// ShowVariable
	static DWORD WINAPI ShowVariable(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg);

	// HookShopStore
	static DWORD WINAPI HookShopStore(std::vector<MyTools::GrammarContext>& vlst, LPWSTR pwszRetMsg);
private:

};


#endif