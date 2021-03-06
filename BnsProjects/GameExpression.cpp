#include "stdafx.h"
#include <map>
#include <thread>
#include "GameExpression.h"
#include <MyTools/Log.h>
#include <MyTools/CLFile.h>
#include <MyTools/CLSearchBase.h>
#include "ObjectFunction.h"
#include "Monster.h"
#include "MonsterExtend.h"
#include "Npc.h"
#include "NpcExtend.h"
#include "Player.h"
#include "PlayerExtend.h"
#include "Person.h"
#include "PersonFunction.h"
#include "Corpse.h"
#include "ResExtend.h"
#include "Language.h"
#include "Skill.h"
#include "UiSkill.h"
#include "BagBase.h"
#include "Task.h"
#include "GameUI.h"
#include "TeamMember.h"
#include "TeamMemberExtend.h"
#include "LoginPlayer.h"
#include "LoginPlayerExtend.h"
#include "SomeFun.h"
#include "UiSkill.h"
#include "ScanBase.h"
#include "BnsFilter.h"
#include "TaskExtend.h"
#include "GameFunction.h"
#include "FarmBoss.h"
#include "BnsScript.h"
#include "ConsoleLog.h"
#include "DailyTask_SnowHole.h"

#define _SELF L"GameExpression"

CGameExprFun::CGameExprFun() : bRunHookMonsterSkill(FALSE), _hBnsScriptThread(NULL), _RunBnsScriptThread(FALSE)
{
	hHookMonsterSkillEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
}

CGameExprFun::~CGameExprFun()
{
	Release();
}

VOID CGameExprFun::Release()
{
	StopGame;

	ReleaseHook(MainHookContent);
	ReleaseHook(BLHookContent);
	ReleaseHook(TelportHookContent);
	ReleaseHook(HeightHookContent);
	ReleaseMonsterSkillHook();

	KillThread(_hBnsScriptThread);
}

std::vector<MyTools::ExpressionFunPtr>& CGameExprFun::GetVec()
{
	static std::vector<MyTools::ExpressionFunPtr> Vec =
	{
		{ std::bind(&CGameExprFun::Help, this, std::placeholders::_1), L"Help" },
		{ std::bind(&CGameExprFun::GetMonster, this, std::placeholders::_1), L"GetMonster" },
		{ std::bind(&CGameExprFun::GetNpc, this, std::placeholders::_1), L"GetNpc" },
		{ std::bind(&CGameExprFun::GetSkill, this, std::placeholders::_1), L"GetSkill" },
		{ std::bind(&CGameExprFun::GetBag, this, std::placeholders::_1), L"GetBag" },
		{ std::bind(&CGameExprFun::GetPlayer, this, std::placeholders::_1), L"GetPlayer" },
		{ std::bind(&CGameExprFun::GetTask, this, std::placeholders::_1), L"GetTask" },
		{ std::bind(&CGameExprFun::GetUi, this, std::placeholders::_1), L"GetUi" },
		{ std::bind(&CGameExprFun::GetCorpse, this, std::placeholders::_1), L"GetCorpse" },
		{ std::bind(&CGameExprFun::GetCityId, this, std::placeholders::_1), L"GetCityId" },
		{ std::bind(&CGameExprFun::GetPersonBuff, this, std::placeholders::_1), L"GetPersonBuff" },
		{ std::bind(&CGameExprFun::GetMonsterBuff, this, std::placeholders::_1), L"GetMonsterBuff" },
		{ std::bind(&CGameExprFun::GetTalk, this, std::placeholders::_1), L"GetTalk" },
		{ std::bind(&CGameExprFun::GetTeam, this, std::placeholders::_1), L"GetTeam" },
		{ std::bind(&CGameExprFun::GetLoginPlayer, this, std::placeholders::_1), L"GetLoginPlayer" },
		{ std::bind(&CGameExprFun::GetUiSkill, this, std::placeholders::_1), L"GetUiSkill" },
		{ std::bind(&CGameExprFun::SetSpeed, this, std::placeholders::_1), L"SetSpeed" },
		{ std::bind(&CGameExprFun::HookMainTask, this, std::placeholders::_1), L"HookMainTask" },
		{ std::bind(&CGameExprFun::HookBLTask, this, std::placeholders::_1), L"HookBLTask" },
		{ std::bind(&CGameExprFun::SearchMemBase, this, std::placeholders::_1), L"SearchMemBase" },
		{ std::bind(&CGameExprFun::HookTelport, this, std::placeholders::_1), L"HookTelport" },
		{ std::bind(&CGameExprFun::HookMonsterSkill, this, std::placeholders::_1), L"HookMonsterSkill" },
		{ std::bind(&CGameExprFun::CreatePlayer, this, std::placeholders::_1), L"CreatePlayer" },
		{ std::bind(&CGameExprFun::DeletePlayer, this, std::placeholders::_1), L"DeletePlayer" },
		{ std::bind(&CGameExprFun::GetFilter, this, std::placeholders::_1), L"GetFilter" },
		{ std::bind(&CGameExprFun::GetGameVar, this, std::placeholders::_1), L"GetGameVar" },
		{ std::bind(&CGameExprFun::GetAccGame, this, std::placeholders::_1), L"GetAccGame" },
		{ std::bind(&CGameExprFun::Telport, this, std::placeholders::_1), L"Telport" },
		{ std::bind(&CGameExprFun::WatchUi, this, std::placeholders::_1), L"WatchUi" },
		{ std::bind(&CGameExprFun::HookHeight, this, std::placeholders::_1), L"HookHeight" },
		{ std::bind(&CGameExprFun::RunBnsScriptFile, this, std::placeholders::_1), L"RunBnsScriptFile" },
		{ std::bind(&CGameExprFun::RecvMail, this, std::placeholders::_1), L"RecvMail" },
		{ std::bind(&CGameExprFun::UseCatSkill, this, std::placeholders::_1), L"UseCatSkill" },
		{ std::bind(&CGameExprFun::Run, this, std::placeholders::_1), L"Run" },
		{ std::bind(&CGameExprFun::Move, this, std::placeholders::_1), L"Move" },
		{ std::bind(&CGameExprFun::SetVar, this, std::placeholders::_1), L"SetVar" },
		{ std::bind(&CGameExprFun::LeaveTeam, this, std::placeholders::_1), L"LeaveTeam" },
		{ std::bind(&CGameExprFun::Test, this, std::placeholders::_1), L"Test" },
		{ std::bind(&CGameExprFun::UseSkill, this, std::placeholders::_1), L"UseSkill" },
		
	};
	return Vec;
}

VOID CGameExprFun::Help(CONST std::vector<std::wstring>&)
{
	auto& Vec = GetVec();
	for (CONST auto& itm : Vec)
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"FunctionName=%s", itm.wsFunName.c_str());

}

VOID CGameExprFun::GetMonster(CONST std::vector<std::wstring>& VecParm)
{
	if (VecParm.size() < 1)
	{
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"GetMonster(-1 || dis)");
		return;
	}
	float fDis = static_cast<float>(_wtof(VecParm.at(0).c_str()));

	std::vector<CMonster> VecMonster;
	CMonsterExtend::GetInstance().GetAroundMonsterList(CBaseObject::CreateFixPointInfo(fDis), VecMonster);
	for (auto& itm : VecMonster)
	{
		Point MonsterPoint = itm.GetPoint();
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"Base=%X,Type=%X,ID=%X,Name=%s,Point=[%d,%d,%d],HP/MAXHP=%d/%d,dis=%.2f,Tar=%X,Name=%s", \
			itm.GetBase(), itm.GetMonsterType(), itm.GetId(), itm.GetName().c_str(), MonsterPoint.GetX(), MonsterPoint.GetY(), MonsterPoint.GetZ(), \
			itm.GetHp(), itm.GetMaxHp(), itm.GetDis(), itm.GetTargetId(), CLanguage::GetInstance().GetOtherText_By_LocalText(itm.GetName()).c_str());
	}
}

VOID CGameExprFun::GetNpc(CONST std::vector<std::wstring>& VecParm)
{
	if (VecParm.size() < 1)
	{
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"GetNpc(-1 || dis)");
		return;
	}
	float fDis = static_cast<float>(_wtof(VecParm.at(0).c_str()));

	std::vector<CNpc> NpcVec;
	CNpcExtend::GetInstance().GetAroundNpcList(CBaseObject::CreateFixPointInfo(fDis), NpcVec);
	for (auto& itm : NpcVec)
	{
		Point NpcPoint = itm.GetPoint();
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"Base=%X,Type=%X,ID=%X,Name=%s,Point=[%d,%d,%d],HP/MAXHP=%d/%d,dis=%.2f,Tar=%X,Name=%s", \
			itm.GetBase(), itm.GetNpcType(), itm.GetId(), itm.GetName().c_str(), NpcPoint.GetX(), NpcPoint.GetY(), NpcPoint.GetZ(), \
			itm.GetHp(), itm.GetMaxHp(), itm.GetDis(), itm.GetTargetId(), CLanguage::GetInstance().GetOtherText_By_LocalText(itm.GetName()).c_str());
	}
}

VOID CGameExprFun::GetSkill(CONST std::vector<std::wstring>& VecParm)
{
	std::vector<CSkill> VecSkill;
	CObjectFunction::GetInstance().GetSkillList(VecSkill);
	for (auto& itm : VecSkill)
	{
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"Base=%X,ID=%X,Name=%s,Name=%s", itm.GetBase(), itm.GetId(), itm.GetName().c_str(), \
			CLanguage::GetInstance().GetOtherText_By_LocalText(itm.GetName()).c_str());
	}
}

VOID CGameExprFun::GetBag(CONST std::vector<std::wstring>& VecParm)
{
	std::vector<CBag> VecBag;
	CObjectFunction::GetInstance().GetBagList(VecBag, CObjectFunction::em_Bag_Type::em_Bag_Type_Bag);
	for (auto& itm : VecBag)
	{
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"Base=%X,Index=%d,Gid=%X,Name=%s,Color=%s,Count=%d,Type=%X,TypeName=%s,Sid=%X,Name=%s", \
			itm.GetBase(), itm.GetIndex(), itm.GetGId(), itm.GetName().c_str(), itm.GetColorName().c_str(), itm.GetCount(), itm.GetItemType(), \
			itm.GetTypeName_Dlg().c_str(), itm.GetSId(), CLanguage::GetInstance().GetOtherText_By_LocalText(itm.GetName()).c_str());
	}
}

VOID CGameExprFun::GetPlayer(CONST std::vector<std::wstring>& VecParm)
{
	if (VecParm.size() < 1)
	{
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"GetPlayer(-1 || dis)");
		return;
	}
	float fDis = static_cast<float>(_wtof(VecParm.at(0).c_str()));

	std::vector<CPlayer> VecPlayer;
	CObjectFunction::GetInstance().GetSortGameObjectList(VecPlayer, CObjectFunction::em_GameObject_Type::em_GameObject_Type_Player, CBaseObject::CreateFixPointInfo(fDis));
	for (auto& itm : VecPlayer)
	{
		Point PlayerPoint = itm.GetPoint();
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"Base=%X,Type=%X,ID=%X,Name=%s,Point=[%d,%d,%d],dis=%.2f,Classes=%s", 
			itm.GetBase(),
			itm.GetIdParm(),
			itm.GetId(), 
			itm.GetName().c_str(),
			PlayerPoint.GetX(), 
			PlayerPoint.GetY(), 
			PlayerPoint.GetZ(), 
			itm.GetDis(), 
			itm.GetClassesText().c_str());
	}
}

VOID CGameExprFun::GetTask(CONST std::vector<std::wstring>& VecParm)
{
	std::vector<TaskInfo> VecTask;
	CTask::GetInstance().GetTaskInfo(VecTask);

	auto MainTask = CTask::GetInstance().GetMianTaskInfo();
	for (auto& itm : VecTask)
	{
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"TaskId=%X,ScheduleId=%X,Name=%s,KillCount=%d,TaskIdBase=%X,ScheduleIdBase=%X,IsMainTask=%s", \
			itm.GetTaskId(), itm.GetScheduleId(), itm.GetName().c_str(), CTask::GetInstance().QueryTaskScheduleCount(itm.GetTaskInfoParm()), \
			itm.dwObj_TaskId, itm.dwObj_ScheduleId, itm.GetTaskId() == MainTask.dwTaskId ? L"Yes" : L"No");
	}
}

VOID CGameExprFun::GetUi(CONST std::vector<std::wstring>& VecParm)
{
	std::wstring wsParmacter = VecParm.size() == 0 ? L"" : VecParm.at(0);

	auto fnFilter = [](CONST CGameUi& GameUi, CONST std::wstring& wsFilterText)
	{
		if (wsFilterText == L"")
			return TRUE;
		else if (wsFilterText == L"1")
			return GameUi.IsShow();
		else if (wsFilterText == L"2")
			return GameUi.IsShow() ? FALSE : TRUE;
		
		return GameUi.GetName().find(wsFilterText) != -1 ? TRUE : FALSE;
	};

	
	std::map<std::wstring, CGameUi> VecUi;
	CObjectFunction::GetInstance().GetGameUIList(VecUi);
	for (auto& itm : VecUi)
	{
		if (!fnFilter(itm.second,wsParmacter))
			continue;

		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"Base=%X,ID=%X,IsShow=%s,Name=%s", itm.second.GetBase(), itm.second.GetId(), \
			itm.second.IsShow() ? L"Yes" : L"No", itm.second.GetName().c_str());
	}
}

VOID CGameExprFun::GetCorpse(CONST std::vector<std::wstring>& VecParm)
{
	if (VecParm.size() < 1)
	{
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"GetCorpse(-1 || dis)");
		return;
	}
	float fDis = static_cast<float>(_wtof(VecParm.at(0).c_str()));

	std::vector<CCorpse> VecCorpse;
	CResExtend::GetInstance().GetAroundCorpseList(CBaseObject::CreateFixPointInfo(fDis), VecCorpse);
	for (auto& itm : VecCorpse)
	{
		auto CorpsePoint = itm.GetPoint();
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"Base=%X,Type=%X,ID=%X,Point=[%d,%d,%d],dis=%.2f,+1C=%X,Name=%s,Name=%s", \
			itm.GetBase(), itm.GetIdType(),itm.GetId(), CorpsePoint.GetX(), CorpsePoint.GetY(), CorpsePoint.GetZ(), itm.GetDis(), \
			ReadBYTE(itm.GetBase() + 0x20), itm.GetName().c_str(), CLanguage::GetInstance().GetOtherText_By_LocalText(itm.GetName()).c_str());
	}
}

VOID CGameExprFun::GetCityId(CONST std::vector<std::wstring>& VecParm)
{
	LOG_C(MyTools::CLog::em_Log_Type_Custome, L"Now CityId=%X", CPersonFunction::GetInstance().GetCityId());
}

VOID CGameExprFun::GetPersonBuff(CONST std::vector<std::wstring>& VecParm)
{
	std::vector<GAMEOBJ_BUFF> VecBuff;
	CObjectFunction::GetInstance().GetPersonBuffList(VecBuff);
	for (auto& itm : VecBuff)
	{
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"Base=%X,ID=%X,TarID=%X,Count=%d", itm.dwBuffBase, itm.dwBuffId, itm.dwTargetId, itm.dwBuffCount);
	}
}

VOID CGameExprFun::GetMonsterBuff(CONST std::vector<std::wstring>& VecParm)
{
	std::vector<GAMEOBJ_BUFF> VecBuff;
	CObjectFunction::GetInstance().GetMonsterBuffList(VecBuff);
	for (auto& itm : VecBuff)
	{
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"Base=%X,ID=%X,TarID=%X,Count=%d", itm.dwBuffBase, itm.dwBuffId, itm.dwTargetId, itm.dwBuffCount);
	}
}

VOID CGameExprFun::GetTalk(CONST std::vector<std::wstring>& VecParm)
{
	std::vector<TalkMsg_Src> VecTalk;
	CObjectFunction::GetInstance().GetTalkMessage(VecTalk);
	for (auto& itm : VecTalk)
	{
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"Base=%X,Text=%s", itm.dwBase, itm.wsText.c_str());
	}
}

VOID CGameExprFun::GetTeam(CONST std::vector<std::wstring>& VecParm)
{
	std::vector<CTeamMember> VecTeam;
	CObjectFunction::GetInstance().GetTeamList(VecTeam);
	for (auto& itm : VecTeam)
	{
		auto MemberPoint = itm.GetPoint();
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"Base=%X,Name=%s,IsCpatain=%s,IsConnect=%s,Channel=%d,PerHp=%d,Point=[%d,%d,%d],dis=%.2f,MapId=%X", \
			itm.GetBase(), itm.GetName().c_str(), itm.IsCaptain() ? L"Yes" : L"No", itm.IsCaptain() ? L"Yes" : L"No", itm.GetChannel(), itm.GetPercentHp(), \
			MemberPoint.GetX(), MemberPoint.GetY(), MemberPoint.GetZ(), itm.GetDis(), itm.GetMapId());
	}
}

VOID CGameExprFun::GetLoginPlayer(CONST std::vector<std::wstring>& VecParm)
{
	std::vector<CLoginPlayer> VecPlayer;
	CObjectFunction::GetInstance().GetLoginPlayerList(VecPlayer);
	for (auto& itm : VecPlayer)
	{
		PlayerclassesText PlayerclassesText_;
		BOOL bRetCode = CPlayer::FindPlayerclassesText(&PlayerclassesText_, [itm](CONST PlayerclassesText& PlayerclassesText_){
			return PlayerclassesText_.emPlayerClasses == itm.GetLoginPlayerClasses();
		});
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"uIndex=%d,Name=%s,Level=%d,IsDelete=%s,Classe=%s,ServerId=%X", \
			itm.GetIndex(), 
			itm.GetName().c_str(), 
			itm.GetLevel(), 
			itm.IsDeleteStatus() ? L"Yes" : L"No", 
			bRetCode ? PlayerclassesText_.wsText.c_str() : MyTools::CCharacter::MakeFormatText(L"未知:%X", itm.GetLoginPlayerClasses(), itm.GetServerId()).c_str());
	}
}

VOID CGameExprFun::GetUiSkill(CONST std::vector<std::wstring>& VecParm)
{
	std::vector<CUiSkill> VecSkill;
	CObjectFunction::GetInstance().GetUiSkillList(VecSkill);
	for (auto& itm : VecSkill)
	{
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"Base=%X,ID=%X,IsCool=%s,IsActive=%s,KeyText=%s", itm.GetBase(), \
			itm.GetId(), itm.IsCool() ? L"Yes" : L"No", itm.IsActive() ? L"Yes" : L"No", itm.GetTextKeyByIndex().c_str());
	}
}

VOID CGameExprFun::SetSpeed(CONST std::vector<std::wstring>& VecParm)
{
	if (VecParm.size() < 1)
	{
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"SetSpeed(fSpeed)");
		return;
	}
	float fSpeed = static_cast<float>(_wtof(VecParm.at(0).c_str()));
	CSomeFun::GetInstance().SetSpeed(fSpeed);
}

static DWORD gs_dwEnable_MainTask_Send = 0;
__declspec(naked) VOID WINAPI fnExprHookMainTask()
{
	static DWORD dwArray[8];
	__asm PUSHAD
	//LOG_CF_D( L"fnHookMianTask");
	_asm
	{
		LEA ECX, dwArray;

		MOV AL, BYTE PTR DS : [ESI + 0x12];	//第一个参数 1
		MOV BYTE PTR DS : [ECX], AL;

		MOV AX, WORD PTR DS : [ESI + 0x10];	//第二个参数 DD
		MOV WORD PTR DS : [ECX + 4], AX;

			//MOV EAX, DWORD PTR DS : [ESI]
			//MOV DWORD PTR DS : [ECX + 8], EAX		//第三个参数 发包内容(不取)

		MOV AL, BYTE PTR DS : [ESI + 0x13];	//第四个参数 0
		MOV BYTE PTR DS : [ECX + 0xC], AL;

		MOV AL, BYTE PTR DS : [ESI + 0x14];	//第五个参数 FF
		MOV BYTE PTR DS : [ECX + 0x10], AL;

		MOV EAX, DWORD PTR DS : [ESI + 0x18];
		MOV DWORD PTR DS : [ECX + 0x14], EAX;	//第六个参数 1

		MOV EAX, DWORD PTR DS : [ESI + 0x1C];
		MOV DWORD PTR DS : [ECX + 0x18], EAX;	//第七个参数 435AB

		MOV AX, WORD PTR DS : [ESI + 0x20];
		MOV WORD PTR DS : [ECX + 0x1C], AX;		//第八个参数 0
	}
	CTaskExtend::Print_MianTask_ArrayInfo(dwArray);

	__asm POPAD
	if (gs_dwEnable_MainTask_Send == 0)
	{
		__asm MOV EAX, 发包CALL
		__asm CALL EAX
	}

	__asm PUSH HookTaskCALL_Addr + 0x5
	__asm RETN
}

VOID CGameExprFun::HookMainTask(CONST std::vector<std::wstring>& VecParm)
{
	if (VecParm.size() < 2)
	{
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"HookMainTask(1=开启Hook 0=关闭,1=禁止提交任务 0=取消)");
		return;
	}

	if (VecParm.at(0) == L"1")
	{
		if (MainHookContent.dwHookAddr == NULL)
		{
			MainHookContent.dwFunAddr = reinterpret_cast<DWORD>(fnExprHookMainTask);
			MainHookContent.dwHookAddr = HookTaskCALL_Addr;
			MainHookContent.uNopCount = 0;
			MyTools::CLHook::Hook_Fun_Jmp_MyAddr(&MainHookContent);
		}
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"Hook完毕!");
	}
	else
	{
		gs_dwEnable_MainTask_Send = FALSE;
		ReleaseHook(MainHookContent);
	}

	gs_dwEnable_MainTask_Send = VecParm.at(1) == L"1" ? TRUE : FALSE;
}

static DWORD gs_dwEnable_BLTask_Send = 0;
__declspec(naked) VOID WINAPI fnExprHookBLTask()
{
	static DWORD dwArray[5];

	_asm
	{
		PUSHAD;
		LEA EAX, dwArray;

		MOV EBX, DWORD PTR DS : [ESI + 0x18];	//第一个参数 1
		MOV DWORD PTR DS : [EAX], EBX;

		MOV EBX, DWORD PTR DS : [ESI + 0x1C];	//第二个参数 DD
		MOV DWORD PTR DS : [EAX + 4], EBX;

		MOV BX, WORD PTR DS : [ESI + 0x10];
		MOV WORD PTR DS : [EAX + 8], BX;	//第三个参数 任务ID

		MOV DI, WORD PTR DS : [ESI + 0x20];
		MOV WORD PTR DS : [EAX + 0xC], DI;		//第四个参数


		MOV BL, BYTE PTR DS : [ESI + 0x12];	//第五个参数 FF (+0x12)
		MOV BYTE PTR DS : [EAX + 0x10], BL;
	}
	CTaskExtend::Print_BLTask_ArrayInfo(dwArray);

	__asm POPAD
	if (gs_dwEnable_BLTask_Send == 0)
	{
		__asm MOV EAX, 发包CALL
		__asm CALL EAX
	}

	__asm PUSH HookBLTaskCALL_Addr + 0x5
	__asm RETN
}

VOID CGameExprFun::HookBLTask(CONST std::vector<std::wstring>& VecParm)
{
	if (VecParm.size() < 2)
	{
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"HookBLTask(1=开启Hook 0=关闭,1=禁止提交任务 0=取消)");
		return;
	}

	if (VecParm.at(0) == L"1")
	{
		if (BLHookContent.dwHookAddr == NULL)
		{
			BLHookContent.dwFunAddr = reinterpret_cast<DWORD>(fnExprHookBLTask);
			BLHookContent.dwHookAddr = HookTaskCALL_Addr;
			BLHookContent.uNopCount = 0;
			MyTools::CLHook::Hook_Fun_Jmp_MyAddr(&BLHookContent);
		}
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"Hook完毕!");
	}
	else
	{
		gs_dwEnable_BLTask_Send = FALSE;
		ReleaseHook(BLHookContent);
	}

	gs_dwEnable_BLTask_Send = VecParm.at(1) == L"1" ? TRUE : FALSE;
}

VOID CGameExprFun::SearchMemBase(CONST std::vector<std::wstring>& VecParm)
{
	CScanBase::Search();
}

static DWORD dwTelportCall = NULL;
static PDWORD pdwTelportMem = NULL;
VOID WINAPI ExprSaveTelportFile()
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, L"C:\\1.BnsPath", L"ab+");
	if (pFile == nullptr)
	{
		LOG_CF_D( L"ReadScriptFile Fiald! Path:C:\\1.BnsPath");
		return;
	}

	fseek(pFile, 0, SEEK_END);
	fwrite(pdwTelportMem, sizeof(DWORD), MAX_TELPORT_SIZE, pFile);
	fclose(pFile);
}

__declspec(naked) VOID WINAPI fnExprHookTelport()
{
	static DWORD dwEbp = NULL;
	__asm MOV dwEbp, EBP;
	__asm PUSHAD;
	pdwTelportMem = new DWORD[MAX_TELPORT_SIZE];
	memcpy(pdwTelportMem, (void*)dwEbp, sizeof(DWORD) * MAX_TELPORT_SIZE);
	ExprSaveTelportFile();
	__asm POPAD
	__asm MOV EAX, dwTelportCall
	__asm CALL EAX
	__asm PUSH Hook瞬移地址 + 0x5
	__asm RETN
}

VOID CGameExprFun::HookTelport(CONST std::vector<std::wstring>& VecParm)
{
	if (VecParm.size() < 1)
	{
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"HookTelport(1=开启 0=关闭)");
		return;
	}

	if (dwTelportCall == NULL)
		dwTelportCall = MyTools::CLHook::GetCall(Hook瞬移地址, 0x400000, L"Client.exe");

	if (VecParm.at(0) == L"1")
	{
		if (TelportHookContent.dwHookAddr == NULL)
		{
			TelportHookContent.dwFunAddr = reinterpret_cast<DWORD>(fnExprHookTelport);
			TelportHookContent.dwHookAddr = Hook瞬移地址;
			TelportHookContent.uNopCount = 0x0;

			MyTools::CLHook::Hook_Fun_Jmp_MyAddr(&TelportHookContent);
		}
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"Hook完毕!");
	}
	else
	{
		ReleaseHook(TelportHookContent);
	}
}

std::shared_ptr<CHAR> ExprGetTelprotFile(_In_ CONST std::wstring& wsPath, _In_ CONST ULONG& ulFileLen)
{
	FILE* pFile = nullptr;
	fopen_s(&pFile, MyTools::CCharacter::UnicodeToASCII(wsPath).c_str(), "rb");
	if (pFile == nullptr)
	{
		LOG_MSG_CF( L"ReadTelportFile Fiald! Path:%s", wsPath.c_str());
		return nullptr;
	}


	std::shared_ptr<CHAR> psContent(new CHAR[ulFileLen], [](CHAR* p){delete[] p; });
	fseek(pFile, 0, SEEK_SET);
	fread(psContent.get(), sizeof(CHAR), (size_t)ulFileLen, pFile);
	fclose(pFile);
	return psContent;
}

VOID CGameExprFun::Telport(CONST std::vector<std::wstring>& VecParm)
{
	ULONG ulFileLen = 0;
	if (!MyTools::CLFile::ReadAsciiFileLen(L"C:\\1.BnsPath", ulFileLen))
	{
		LOG_MSG_CF( L"1.BnsPath FileLen Invaild!");
		return;
	}
	if (ulFileLen % MAX_TELPORT_SIZE != 0)
	{
		LOG_MSG_CF( L"1.BnsPath FileLen=%d, Invaild!", (UINT)ulFileLen);
		return;
	}

	std::shared_ptr<CHAR> psBuffer = ExprGetTelprotFile(L"C:\\1.BnsPath", ulFileLen);
	if (psBuffer == nullptr)
		return;

	auto fnGetPointValue = [&psBuffer](_In_ int i, _In_ int nOffset)
	{
		DWORD dwAddr = ((DWORD*)psBuffer.get())[i * MAX_TELPORT_SIZE + nOffset];
		return ReadFloat((DWORD)&dwAddr);
	};

	std::vector<Point> vPoint;
	for (UINT i = 0; i < ulFileLen / (MAX_TELPORT_SIZE * 4); ++i)
		vPoint.push_back(Point(fnGetPointValue(i, 1), fnGetPointValue(i, 2), fnGetPointValue(i, 3)));

	int nIndex = CGameFunction::GetInstance().GetRecentlyPoint(vPoint, CBaseObject::GetPersonPoint());
	if (nIndex == -1)
	{
		LOG_CF_E( L"竟然离最近的点超过30m!");
		return;
	}

	BnsMove(vPoint.at(nIndex), 2.0f);
	::Sleep(3000);

	if (dwTelportCall == NULL)
		dwTelportCall = MyTools::CLHook::GetCall(Hook瞬移地址, 0x400000, L"Client.exe");

	DWORD dwPersonBase = CPerson::GetInstance().GetBase();
	for (int i = nIndex; i < (int)vPoint.size(); ++i)
	{
		DWORD dwArray[MAX_TELPORT_SIZE] = { 0 };
		memcpy(dwArray, ((DWORD*)psBuffer.get()) + i * MAX_TELPORT_SIZE, sizeof(DWORD) * MAX_TELPORT_SIZE);

		__asm
		{
			PUSHAD;
			PUSH 0;
			PUSH 0;
			MOV EAX, dwPersonBase;
			MOV EAX, DWORD PTR DS : [EAX + 0x14];
			ADD EAX, 人物瞬移偏移2;
			MOV ECX, DWORD PTR DS : [EAX + 人物瞬移偏移];
			MOV WORD PTR DS : [EAX + 人物瞬移偏移], CX;
			LEA EAX, dwArray;
			//PUSH EAX;
			MOV EDI, EAX;
			MOV EAX, dwTelportCall;
			CALL EAX;
			POPAD;
		}

		::Sleep(10);
	}
	MessageBoxW(NULL, L"End", L"", NULL);
}

unsigned WINAPI CGameExprFun::_ShowMonsterSkillIdThread(LPVOID lpParm)
{
	CGameExprFun* pExprFun = reinterpret_cast<CGameExprFun*>(lpParm);
	/*while (pExprFun->bRunHookMonsterSkill)
	{
		DWORD dwSkillID = CFarmBoss::GetInstance().GetBossSkillByQue();
		if (dwSkillID != NULL)
		{
			LOG_C(MyTools::CLog::em_Log_Type_Custome, L"%d", dwSkillID);
			continue;
		}
		::Sleep(10);
	}*/
	::SetEvent(pExprFun->hHookMonsterSkillEvent);
	return 0;
}

VOID CGameExprFun::KillThread(_In_ _Out_ HANDLE& hThread)
{
	if (hThread != NULL)
	{
		::WaitForSingleObject(hThread, INFINITE);
		::CloseHandle(hThread);
		hThread = NULL;
	}
}

VOID CGameExprFun::HookMonsterSkill(CONST std::vector<std::wstring>& VecParm)
{
	if (VecParm.size() != 2)
	{
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"HookMonsterSkill(1=开启 0=关闭, MonsterName)");
		return;
	}

	auto fnRunThread = [this](CONST CPlayer& Target)
	{
		bRunHookMonsterSkill = TRUE;
		CFarmBoss::GetInstance().IsHookByDlg() = TRUE;
		CGameFunction::GetInstance().HookMonsterSkill(TRUE, Target.GetBase());
		cbBEGINTHREADEX(NULL, NULL, _ShowMonsterSkillIdThread, this, NULL, NULL);
	};

	if (VecParm.at(0) == L"0")
	{
		ReleaseMonsterSkillHook();
	}
	else
	{
		BOOL bExist = CMonsterExtend::GetInstance().DoAction_By_MonsterName(VecParm.at(0), CBaseObject::CreateFixPointInfo(-1), [fnRunThread](CONST CMonster& Monster)
		{
			fnRunThread(Monster);
		});

		if (bExist)
		{
			LOG_C(MyTools::CLog::em_Log_Type_Custome, L"Hook怪物技能成功!");
			return;
		}

		bExist = CNpcExtend::GetInstance().DoAction_By_ExistNpc_For_Name(VecParm.at(0), CBaseObject::CreateFixPointInfo(-1), [fnRunThread](CONST CNpc& Npc)
		{
			fnRunThread(Npc);
		});

		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"%s", bExist ? L"Hook Npc 技能成功!" : L"不存在这样名字的Npc和怪物!");
	}
}

VOID CGameExprFun::CreatePlayer(CONST std::vector<std::wstring>& VecParm)
{
	if (VecParm.size() < 1)
	{
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"CreatePlayer(ServerId)");
		return;
	}
	DWORD dwServerId = static_cast<DWORD>(wcstol(VecParm.at(0).c_str(), nullptr, 16));
	CLoginPlayerExtend::GetInstance().CreatePlayer(dwServerId, CObjectFunction::GetInstance().GetLoginPlayerCount(), em_Player_Classes::em_Player_Classes_Summoner);
}

VOID CGameExprFun::DeletePlayer(CONST std::vector<std::wstring>& VecParm)
{
	if (VecParm.size() < 1)
	{
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"DeletePlayer(Index)");
		return;
	}
	DWORD dwIndex = static_cast<DWORD>(wcstol(VecParm.at(0).c_str(), nullptr, 16));
	CLoginPlayerExtend::GetInstance().DeletePlayer(dwIndex);
}

VOID CGameExprFun::GetFilter(CONST std::vector<std::wstring>& VecParm)
{
	
}

VOID CGameExprFun::GetGameVar(CONST std::vector<std::wstring>& VecParm)
{
	if (VecParm.size() < 1)
	{
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"GetGameVar(VarName)");
		return;
	}
	DWORD dwValue = CGameVariable::GetInstance().GetVariable_By_Name(VecParm.at(0));
	LOG_C(MyTools::CLog::em_Log_Type::em_Log_Type_Custome, L"Dec %s=%d", VecParm.at(0).c_str(), dwValue);
	LOG_C(MyTools::CLog::em_Log_Type::em_Log_Type_Custome, L"Hex %s=%X", VecParm.at(0).c_str(), dwValue);
}

VOID CGameExprFun::GetAccGame(CONST std::vector<std::wstring>& VecParm)
{
	CGameVariable::GetInstance().GetAccShareInfo()->Print();
}

VOID CGameExprFun::ReleaseHook(_In_ _Out_opt_ MyTools::MYHOOK_CONTENT& HookContent)
{
	if (HookContent.dwHookAddr != NULL)
	{
		MyTools::CLHook::UnHook_Fun_Jmp_MyAddr(&HookContent);
		HookContent.Release();
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"解除Hook完毕!");
	}
}

VOID CGameExprFun::ReleaseMonsterSkillHook()
{
	if (bRunHookMonsterSkill)
	{
		bRunHookMonsterSkill = FALSE;
		CFarmBoss::GetInstance().IsHookByDlg() = FALSE;
		CGameFunction::GetInstance().HookMonsterSkill(FALSE, NULL);
		::WaitForSingleObject(hHookMonsterSkillEvent, INFINITE);
		::ResetEvent(hHookMonsterSkillEvent);
	}
	
	LOG_C(MyTools::CLog::em_Log_Type::em_Log_Type_Custome, L"关闭Hook完毕!");
}

VOID CGameExprFun::WatchUi(CONST std::vector<std::wstring>& VecParm)
{
	static std::map<std::wstring, BOOL> UiMap;
	if (VecParm.size() < 1)
	{
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"WatchUi(1=开启 0=关闭)");
		return;
	}

	if (VecParm.at(0) == L"1")
	{
		std::map<std::wstring, CGameUi> VecUi;
		CObjectFunction::GetInstance().GetGameUIList(VecUi);

		UiMap.clear();
		for (auto& itm : VecUi)
			UiMap.insert(std::make_pair(itm.first, itm.second.IsShow()));
		LOG_C(MyTools::CLog::em_Log_Type::em_Log_Type_Custome, L"记录完毕!");
	}
	else
	{
		std::map<std::wstring, CGameUi> VecUi;
		CObjectFunction::GetInstance().GetGameUIList(VecUi);

		for (auto& NowItem : VecUi)
		{
			auto itr = UiMap.find(NowItem.first);
			if (itr != UiMap.end() && itr->second != NowItem.second.IsShow())
			{
				LOG_C(MyTools::CLog::em_Log_Type::em_Log_Type_Custome, L"Ui:%s Show: %X to %X", itr->first.c_str(), itr->second, NowItem.second.IsShow());
				continue;
			}
			else if (itr == UiMap.end())
			{
				LOG_C(MyTools::CLog::em_Log_Type::em_Log_Type_Custome, L"Add New Ui:%s, Show:%X", NowItem.first.c_str(), NowItem.second.IsShow());
				continue;
			}
		}
	}
}

static DWORD dwHookHeightAddr = NULL;
static DWORD dwHookHeightEBX = NULL;
__declspec(naked) VOID WINAPI fnHookHeight()
{
	static DWORD dwESP = NULL;
	__asm MOV dwHookHeightEBX, EBX;
	__asm MOV dwESP, ESP;
	__asm PUSHAD;
	__asm PUSHFD;

	static DWORD dwPersonAddr = NULL;
	dwPersonAddr = GetPersonPointAddr - 0x54;
	if (dwPersonAddr != dwHookHeightEBX)
	{
		__asm MOV EBX, dwHookHeightEBX;
		__asm MOVSS XMM0, DWORD PTR DS : [EBX + 0x5C];
		__asm MOV EBX, dwESP;
		__asm ADDSS XMM0, DWORD PTR DS : [EBX + 0x68];
		__asm MOV EBX, dwHookHeightEBX;
		__asm MOVSS DWORD PTR DS : [EBX + 0x5C], XMM0;
	}

	__asm POPFD;
	__asm POPAD;
	__asm MOV ECX, dwHookHeightAddr;
	__asm ADD ECX, 0x5;
	__asm PUSH ECX;
	__asm RETN;
}

VOID CGameExprFun::HookHeight(CONST std::vector<std::wstring>& VecParm)
{
	if (VecParm.size() < 2)
	{
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"HookHeight(1=开启 0关闭, 高度)");
		return;
	}

	float fHeightValue = static_cast<float>(_wtof(VecParm.at(1).c_str()));
	if (VecParm.at(0) == L"1")
	{
		if (HeightHookContent.dwHookAddr != NULL)
			return;

		DWORD dwJmpHookAddr = MyTools::CLSearchBase::FindAddr("3C0D74??85??0F84", 0x0A5F6457 - 0x0A5F6476, 0, L"bsengine_Shipping");
		if (dwJmpHookAddr == NULL)
		{
			LOG_MSG_CF( L"SetHeight.dwJmpHookAddr=NULL");
			return;
		}

		dwHookHeightAddr = MyTools::CLSearchBase::FindAddr("F30F584424??F30F11435C8B", 0x0A58C79E - 0x0A58C7A4, 0, L"bsengine_Shipping");
		if (dwHookHeightAddr == NULL)
		{
			LOG_MSG_CF( L"SetHeight.dwNopHookAddr=NULL");
			return;
		}
		DWORD dwOldValue1 = ReadDWORD(dwJmpHookAddr + 0x0);
		DWORD dwOldValue2 = ReadDWORD(dwJmpHookAddr + 0x4);
		MyTools::CCharacter::WriteDWORD(dwJmpHookAddr + 0x0, 0x90909090);
		MyTools::CCharacter::WriteBYTE(dwJmpHookAddr + 0x4, 0x90);
		MyTools::CCharacter::WriteBYTE(dwJmpHookAddr + 0x5, 0x90);

		while (CPerson::GetInstance().GetPoint().Z < fHeightValue)
		{
			//LOG_C(MyTools::CLog::em_Log_Type_Custome, L"当前高度=%.2f < %.2f", CPerson::GetInstance().GetPoint().Z, fHeightValue);
			CSomeFun::GetInstance().SimlationKey(VK_SPACE);
		}

		// restore
		MyTools::CCharacter::WriteDWORD(dwJmpHookAddr + 0x0, dwOldValue1);
		MyTools::CCharacter::WriteDWORD(dwJmpHookAddr + 0x4, dwOldValue2);

		HeightHookContent.dwHookAddr = dwHookHeightAddr;
		HeightHookContent.dwFunAddr = reinterpret_cast<DWORD>(fnHookHeight);
		HeightHookContent.uNopCount = 0;
		MyTools::CLHook::Hook_Fun_Jmp_MyAddr(&HeightHookContent);
	}
	else
	{
		ReleaseHook(HeightHookContent);
	}

}

unsigned WINAPI CGameExprFun::_BnsScriptThread(LPVOID lpParm)
{
	/*CBnsScript BnsScript;
	WCHAR wszScriptPath[64] = { L"C:\\Bns\\" };

	lstrcatW(wszScriptPath, L"Bns.inf");
	LOG_CF_D( L"读取文件:%s", wszScriptPath);
	if (!BnsScript.Read(CBnsScript::em_ReadScript_Type_FilePath, wszScriptPath))
	{
		LOG_MSG_CF(L"解析脚本:%s失败!", wszScriptPath);
		return 0;
	}

	WCHAR wszScriptFunName[32] = { L"main" };
	LOG_CF_D( L"执行函数:%s", wszScriptFunName);
	BnsScript.Excute(wszScriptFunName);
	CPersonFunction::GetInstance().Resurrect();
	BnsScript.ReleaseBnsScript();*/
	CBnsScript BnsScript;
	CDailyTask_SnowHole SnowHole;
	SnowHole.Run();
	BnsScript.ReleaseBnsScript();
	*reinterpret_cast<BOOL*>(lpParm) = FALSE;
	return 0;
}

VOID CGameExprFun::RunBnsScriptFile(CONST std::vector<std::wstring>& VecParm)
{
	if (VecParm.size() < 1)
	{
		LOG_MSG_CF(L"Parm.size < 1");
		return;
	}

	if (VecParm.at(0) == L"0")
	{
		StopGame;
		if (_hBnsScriptThread != NULL)
		{
			LOG_C_D(L"Terminating hBnsScript~");
			WaitForSingleObject(_hBnsScriptThread, INFINITE);
			::CloseHandle(_hBnsScriptThread);
			_hBnsScriptThread = NULL;
		}
		
		LOG_C_D(L"Terminating hBnsScript Done!");
		return;
	}
	
	if (_hBnsScriptThread != NULL && _RunBnsScriptThread)
	{
		LOG_C_D(L"Already Run~!");
		return;
	}

	StartGame;
	_RunBnsScriptThread = TRUE;
	_hBnsScriptThread = cbBEGINTHREADEX(NULL, NULL, _BnsScriptThread, &_RunBnsScriptThread, NULL, NULL);
}

VOID CGameExprFun::RecvMail(CONST std::vector<std::wstring>& VecParm)
{
	StartGame;
	CMailAction::GetInstance().RecvMail();
}

VOID CGameExprFun::UseCatSkill(CONST std::vector<std::wstring>& VecParm)
{
	if (VecParm.size()  == 0)
	{
		LOG_MSG_CF(L"Parm.size < 1");
		return;
	}
	StartGame;
	auto dwSkillId = wcstol(VecParm.at(0).c_str(), NULL, 16);
	LOG_C_D(L"dwSkillId=%X", dwSkillId);
	CSkill::UseSkill_By_Id(dwSkillId, TRUE, em_Skill_Type::em_Skill_Type_Cat);
}

VOID CGameExprFun::Run(CONST std::vector<std::wstring>& VecParm)
{
	CPersonFunction::GetInstance().Run(CPersonFunction::em_Run_Type::em_Run_Type_OnFloor);
}

VOID CGameExprFun::Move(CONST std::vector<std::wstring>& VecParm)
{
	if (VecParm.size() != 3)
	{
		LOG_MSG_CF(L"Parm.size < ");
		return;
	}

	Point Pt;
	Pt.X = static_cast<float>(_wtof(VecParm.at(0).c_str()));
	Pt.Y = static_cast<float>(_wtof(VecParm.at(1).c_str()));
	Pt.Z = static_cast<float>(_wtof(VecParm.at(2).c_str()));

	CPersonFunction::GetInstance().Move(Pt);
}

VOID CGameExprFun::SetVar(CONST std::vector<std::wstring>& VecParm)
{
	if (VecParm.size() != 2)
	{
		LOG_MSG_CF(L"Parm.size != 2");
		return;
	}

	LOG_CF_D(L"VarName=%s, Value=%d", VecParm.at(0).c_str(), _wtoi(VecParm.at(1).c_str()));
	CGameVariable::GetInstance().SetVariable_By_Name(VecParm.at(0), _wtoi(VecParm.at(1).c_str()));
}

VOID CGameExprFun::LeaveTeam(CONST std::vector<std::wstring>&)
{
	if (CTeamMemberExtend::GetInstance().GetTeamMemberCount() != 0)
	{
		CTeamMemberExtend::GetInstance().LeaveTeam();
	}
}

VOID CGameExprFun::Test(CONST std::vector<std::wstring>&)
{
	
}

VOID CGameExprFun::UseSkill(CONST std::vector<std::wstring>& VecParam)
{
	if (VecParam.size() < 1)
	{
		LOG_MSG_CF(L"Parm.size != 2");
		return;
	}

	DWORD dwSkillId = wcstol(VecParam.at(0).c_str(), nullptr, 16);
	LOG_CF_D(L"dwSkillId=%X", dwSkillId);
	CSkill::UseSkill_By_Id(dwSkillId, FALSE, em_Skill_Type_Person);
}

