#ifndef __GITBNSPROJECTS_CONSOLEDLL_FUNCTION_ACCOUNTMGR_H__
#define __GITBNSPROJECTS_CONSOLEDLL_FUNCTION_ACCOUNTMGR_H__

#include "ConsoleVariable.h"
#include <MyTools/ClassInstance.h>

class CAccountMgr : public MyTools::CClassInstance<CAccountMgr>
{
public:
	CAccountMgr();
	~CAccountMgr();

	BOOL SetPkLog(_In_ ACCOUNT_INFO_GAME& ACCOUNT_INFO_GAME_);

	BOOL SetResetAccount(_In_ ACCOUNT_INFO_GAME& ACCOUNT_INFO_GAME_);

	BOOL SetClearAccount(_In_ ACCOUNT_INFO_GAME& ACCOUNT_INFO_GAME_);

	// ����˺�
	BOOL AddAccount(_In_ LPCWSTR pwszAccountName, _In_ LPCWSTR pwszAccountPass);

	// 
	PACCOUNT_INFO_GAME IsExistAccount(_In_ LPCWSTR pwszAccountName);

	// ���ض����ʺŽ��в���
	BOOL HandleAccount(_In_ CONST std::wstring& wsAccountName, _In_ std::function<BOOL(ACCOUNT_INFO_GAME&)> f);

	// ɾ���ʺ�
	BOOL DeleteAccount(_In_ LPCWSTR pwszAccountName);

	// �����˺�
	BOOL RestartAccount(_In_ LPCWSTR pwszAccountName);

	VOID HandleEachAccount(std::function<void(ACCOUNT_INFO_GAME& AccountGame)> AcionPtr);
};



#endif