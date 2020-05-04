#ifndef __GITBNSPROJECTS_BNSPROJECTS_GAME_GRAMMAR_GAMEEXPRESSION_H__
#define __GITBNSPROJECTS_BNSPROJECTS_GAME_GRAMMAR_GAMEEXPRESSION_H__

#include <MyTools/CLExpression.h>
#include <MyTools/ClassInstance.h>
#include <MyTools/CLHook.h>

class CGameExprFun : public MyTools::CExprFunBase, virtual public MyTools::CClassInstance<CGameExprFun>
{
public:
	CGameExprFun();
	virtual ~CGameExprFun();

	virtual VOID Release();

	virtual std::vector<MyTools::ExpressionFunPtr>& GetVec();
private:
	virtual VOID Help(CONST std::vector<std::wstring>& VecParm);

	VOID GetMonster(CONST std::vector<std::wstring>& VecParm);

	VOID GetNpc(CONST std::vector<std::wstring>& VecParm);

	VOID GetSkill(CONST std::vector<std::wstring>& VecParm);

	VOID GetBag(CONST std::vector<std::wstring>& VecParm);

	VOID GetPlayer(CONST std::vector<std::wstring>& VecParm);

	VOID GetTask(CONST std::vector<std::wstring>& VecParm);

	VOID GetUi(CONST std::vector<std::wstring>& VecParm);

	VOID GetCorpse(CONST std::vector<std::wstring>& VecParm);

	VOID GetCityId(CONST std::vector<std::wstring>& VecParm);

	VOID GetPersonBuff(CONST std::vector<std::wstring>& VecParm);

	VOID GetMonsterBuff(CONST std::vector<std::wstring>& VecParm);

	VOID GetTalk(CONST std::vector<std::wstring>& VecParm);

	VOID GetTeam(CONST std::vector<std::wstring>& VecParm);

	VOID GetLoginPlayer(CONST std::vector<std::wstring>& VecParm);

	VOID GetUiSkill(CONST std::vector<std::wstring>& VecParm);

	VOID SetSpeed(CONST std::vector<std::wstring>& VecParm);

	VOID HookMainTask(CONST std::vector<std::wstring>& VecParm);

	VOID HookBLTask(CONST std::vector<std::wstring>& VecParm);

	VOID SearchMemBase(CONST std::vector<std::wstring>& VecParm);

	VOID HookTelport(CONST std::vector<std::wstring>& VecParm);

	VOID Telport(CONST std::vector<std::wstring>& VecParm);

	VOID HookMonsterSkill(CONST std::vector<std::wstring>& VecParm);

	VOID CreatePlayer(CONST std::vector<std::wstring>& VecParm);

	VOID DeletePlayer(CONST std::vector<std::wstring>& VecParm);

	VOID GetFilter(CONST std::vector<std::wstring>& VecParm);

	VOID GetGameVar(CONST std::vector<std::wstring>& VecParm);

	VOID GetAccGame(CONST std::vector<std::wstring>& VecParm);

	VOID ReleaseHook(_In_ _Out_opt_ MyTools::MYHOOK_CONTENT& HookContent);

	VOID ReleaseMonsterSkillHook();

	VOID WatchUi(CONST std::vector<std::wstring>& VecParm);

	VOID HookHeight(CONST std::vector<std::wstring>& VecParm);

	VOID RunBnsScriptFile(CONST std::vector<std::wstring>& VecParm);

	VOID RecvMail(CONST std::vector<std::wstring>& VecParm);

	VOID UseCatSkill(CONST std::vector<std::wstring>& VecParm);

	VOID Run(CONST std::vector<std::wstring>& VecParm);

	VOID Move(CONST std::vector<std::wstring>& VecParm);

	VOID SetVar(CONST std::vector<std::wstring>& VecParm);

	VOID LeaveTeam(CONST std::vector<std::wstring>&);

	VOID Test(CONST std::vector<std::wstring>&);

	VOID UseSkill(CONST std::vector<std::wstring>&);

	static unsigned WINAPI _ShowMonsterSkillIdThread(LPVOID lpParm);
	static unsigned WINAPI _BnsScriptThread(LPVOID lpParm);
private:
	VOID KillThread(_In_ _Out_ HANDLE& hThread);
private:
	MyTools::MYHOOK_CONTENT	MainHookContent;
	MyTools::MYHOOK_CONTENT	BLHookContent;
	MyTools::MYHOOK_CONTENT	TelportHookContent;
	MyTools::MYHOOK_CONTENT	HeightHookContent;

	// Hook MonsterSkill
	BOOL			bRunHookMonsterSkill;
	HANDLE			hHookMonsterSkillEvent;
	HANDLE			_hBnsScriptThread;
	BOOL			_RunBnsScriptThread;
};


#endif // !__GITBNSPROJECTS_BNSPROJECTS_GAME_GRAMMAR_GAMEEXPRESSION_H__
