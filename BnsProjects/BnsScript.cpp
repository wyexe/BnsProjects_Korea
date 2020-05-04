#include "stdafx.h"
#include "BnsScript.h"
#include <thread>
#include <process.h>
#include <MyTools/CLSearchBase.h>
#include <MyTools/CLEchoException.h>
#include <MyTools/Log.h>
#include "PlayerKill.h"
#include "InstanceZoneScript.h"
#include "FarmSkill.h"
#include "PersonFunction.h"
#include "BagFunction.h"
#include "FarmSkill.h"
#include "FarmSpecialTask.h"
#include "FarmTask.h"
#include "NpcExtend.h"
#include "MonsterExtend.h"
#include "ResExtend.h"
#include "ResAction.h"
#include "TaskExtend.h"
#include "BagAction.h"
#include "BnsFilter.h"
#include "MailAction.h"
#include "SomeFun.h"
#include "GameFunction.h"
#include "FarmBoss.h"
#include "TeamMemberExtend.h"
#include "Person.h"
#include "Npc.h"
#include "Monster.h"
#include "Corpse.h"
#include "GameUI.h"
#include "Task.h"
#include "ObjectFunction.h"
#include "BnsGameFun.h"
#include "DailyExtend.h"
#include "BnsConfig.h"
#include "DailyScript.h"
#include "GameSend.h"
#include "TeamMember.h"
#include "Skill.h"
#include "Cat.h"
#include "CatExpend.h"
#include "BaiXianPracticeFieldServices.h"
#include "FindPathScript.h"
#include "BnsHook.h"
#include "ScriptExtend.h"
#include "TeamAuction.h"
#include "ConsoleLog.h"
#include "DailyTask_Move.h"
#if BnsProject_DLL
#include "ClientTeam.h"
#include "TaskUpgradeMgr.h"
#include "TransferScript.h"
#endif


#define _SELF L"BnsScript.cpp"

HANDLE CBnsScript::hHelpThread = NULL;
BOOL CBnsScript::bExistHelpThread = FALSE;
CBnsScript::CBnsScript()
{
	_bExitScript = FALSE;
	_dwInstanceTimeOutCount = 0;
	_ulInstanceTimeOutTick = NULL;

	_fnExceptionPtr = []{ return TRUE; };
	_fnLogPtr = [](CONST std::wstring& wsFileName, int nLine, CONST std::wstring& wsText)
	{
		MyTools::CLog::GetInstance().Print(L"Script", wsFileName.c_str(), nLine, LOG_TYPE_CONSOLE | LOG_TYPE_FILE, MyTools::CLog::em_Log_Type::em_Log_Type_Exception, TRUE, wsText.c_str());
	};
	_fnIfPtr = [](_In_ CONST Script_Code_If& ScriptCodeIf)
	{
		return CScriptExtend::GetInstance().Script_If(ScriptCodeIf);
	};
	_fnWhilePtr = [](_In_ CONST Script_Code_If& ScriptCodeIf)
	{
		return CScriptExtend::GetInstance().Script_While(ScriptCodeIf);
	};

	CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->dwClasses = static_cast<DWORD>(CPerson::GetInstance().GetClasses());
	if (hHelpThread == NULL)
		hHelpThread = cbBEGINTHREADEX(NULL, NULL, _HelpThread, this, NULL, NULL);

	BindCustomeMethod();
}

CBnsScript::~CBnsScript()
{
	
}

VOID CBnsScript::BindCustomeMethod()
{
	std::vector<Script_Code_Custome_Method> VecCustMethod = 
	{
		{ L"走到",				std::bind(&CBnsScript::Script_Move,							this) },
		{ L"使用技能",			std::bind(&CBnsScript::Script_UseSkill,						this) },
		{ L"Sleep",				std::bind(&CBnsScript::Script_Sleep,						this) },
		{ L"范围清怪",			std::bind(&CBnsScript::Script_Farm_By_Dis,					this) },
		{ L"卖",				std::bind(&CBnsScript::Script_Sell,							this) },
		{ L"按键",				std::bind(&CBnsScript::Script_Simulationkey,				this) },
		{ L"使用物品",			std::bind(&CBnsScript::Script_UseItem,						this) },
		{ L"对准",				std::bind(&CBnsScript::Script_SetAngle,						this) },
		{ L"任务清怪",			std::bind(&CBnsScript::Script_Farm_By_Task,					this) },
		{ L"捡物",				std::bind(&CBnsScript::Script_TakeItem_By_DropItem,			this) },
		{ L"捡尸体",			std::bind(&CBnsScript::Script_TakeItem_By_Corpse,			this) },
		{ L"捡Npc",				std::bind(&CBnsScript::Script_TakeItem_By_Npc,				this) },
		{ L"范围捡Npc",			std::bind(&CBnsScript::Script_TakeItem_By_Npc_By_SetPoint,	this) },
		{ L"MessageBox",		std::bind(&CBnsScript::Script_MessageBox,					this) },
		{ L"穿装备",			std::bind(&CBnsScript::Script_WearEqui,						this) },
		{ L"遁术",				std::bind(&CBnsScript::Script_DeliverCity,					this) },
		{ L"过图",				std::bind(&CBnsScript::Script_GameChat,						this) },
		{ L"加速",				std::bind(&CBnsScript::Script_SetSpeed,						this) },
		{ L"优化",				std::bind(&CBnsScript::Script_SetOptimization,				this) },
		{ L"整理背包",			std::bind(&CBnsScript::Script_CleanBag,						this) },
		{ L"交主线任务",		std::bind(&CBnsScript::Script_SumitMainTask,				this) },
		{ L"接支线任务",		std::bind(&CBnsScript::Script_PickBLTask,					this) },
		{ L"设置坐标",			std::bind(&CBnsScript::Script_SetPoint,						this) },
		{ L"走最近点",			std::bind(&CBnsScript::Script_MovePoint_By_SetPoint,		this) },
		{ L"倒着走",			std::bind(&CBnsScript::Script_Back_MovePoint_By_SetPoint,	this) },
		{ L"装备强化",			std::bind(&CBnsScript::Script_GrowthEqui,					this) },
		{ L"开箱子",			std::bind(&CBnsScript::Script_OpenBox,						this) },
		{ L"钥匙开箱子",		std::bind(&CBnsScript::Script_OpenBox_By_Key,				this) },
		{ L"分解",				std::bind(&CBnsScript::Script_ExplainEqui,					this) },
		{ L"切换固定频道",		std::bind(&CBnsScript::Script_SetFixChannel,				this) },
		{ L"切换随机频道",		std::bind(&CBnsScript::Script_SetRandChannel,				this) },
		{ L"邮寄",				std::bind(&CBnsScript::Script_Mail,							this) },
		{ L"设置怪物过滤",		std::bind(&CBnsScript::Script_FilterMonster,				this) },
		{ L"跟随Npc",			std::bind(&CBnsScript::Script_FollowNpc,					this) },
		{ L"设置物品过滤",		std::bind(&CBnsScript::Script_FilterItem,					this) },
		{ L"买",				std::bind(&CBnsScript::Script_Buy,							this) },
		{ L"扩展背包",			std::bind(&CBnsScript::Script_ExpandBag,					this) },
		{ L"执行脚本",			std::bind(&CBnsScript::Script_ExcuteScript,					this) },
		{ L"移动到目标",		std::bind(&CBnsScript::Script_MoveToTarget,					this) },
		{ L"镶嵌宝石",			std::bind(&CBnsScript::Script_SetGem,						this) },
		{ L"取宝石",			std::bind(&CBnsScript::Script_ClearGem,						this) },
		{ L"解印",				std::bind(&CBnsScript::Script_Untie,						this) },
		{ L"装备突破",			std::bind(&CBnsScript::Script_EquiEvolution,				this) },
		{ L"装备进化",			std::bind(&CBnsScript::Script_EquiEndEvolution,				this) },
		{ L"脱装备",			std::bind(&CBnsScript::Script_RemoveEqui,					this) },
		{ L"修理武器",			std::bind(&CBnsScript::Script_RepairWeapon,					this) },
		{ L"存入衣柜",			std::bind(&CBnsScript::Script_SaveCloth,					this) },
		{ L"抱起尸体",			std::bind(&CBnsScript::Script_PickCorpse,					this) },
		{ L"清除怪物过滤",		std::bind(&CBnsScript::Script_ClearMonsterFilter,			this) },
		{ L"抱起僵尸尸体",		std::bind(&CBnsScript::Script_PickDropItem,					this) },
		{ L"走路",				std::bind(&CBnsScript::Script_MoveTo,						this) },
		{ L"执行主线任务",		std::bind(&CBnsScript::Script_ExcuteTask,					this) },
		{ L"设置标志",			std::bind(&CBnsScript::Script_SetExitFlag,					this) },
		{ L"范围捡尸体",		std::bind(&CBnsScript::Script_TakeCorpse_By_Dis,			this) },
		{ L"任务是否完成",		std::bind(&CBnsScript::Script_IsTaskDone,					this) },
		{ L"吃坐红",			std::bind(&CBnsScript::Script_EatFood,						this) },
		{ L"吃药",				std::bind(&CBnsScript::Script_EatMedicine,					this) },
		{ L"设置邮寄物品",		std::bind(&CBnsScript::Script_SetMailItem,					this) },
		{ L"开始PK",			std::bind(&CBnsScript::Script_StartPk,						this) },
		{ L"站着捡",			std::bind(&CBnsScript::Script_PickCorpse_No_Move,			this) },
		{ L"测试弹窗",			std::bind(&CBnsScript::Script_TestMessageBox,				this) },
		{ L"测试邮寄",			std::bind(&CBnsScript::Script_TestMail,						this) },
		{ L"范围抱起僵尸尸体",	std::bind(&CBnsScript::Script_PickDropItem_By_SetPoint,		this) },
		{ L"组队",				std::bind(&CBnsScript::Script_JoinTeam,						this) },
		{ L"离队",				std::bind(&CBnsScript::Script_LeaveTeam,					this) },
		{ L"关闭对话界面",		std::bind(&CBnsScript::Script_CloseNpcTalkDlg,				this) },
		{ L"设置邮寄失败",		std::bind(&CBnsScript::Script_SetMailFialdText,				this) },
		{ L"必捡尸体",			std::bind(&CBnsScript::Script_TakeCorpse_By_Status,			this) },
		{ L"收邮件",			std::bind(&CBnsScript::Script_RecvMail,						this) },
		{ L"判断任务进度",		std::bind(&CBnsScript::Script_TaskScheduleExist,			this) },
		{ L"任务捡球",			std::bind(&CBnsScript::Script_Task_TaskBall,				this) },
		{ L"捡任务",			std::bind(&CBnsScript::Script_Pickup,						this) },
		{ L"瞬移",				std::bind(&CBnsScript::Script_Telport,						this) },
		{ L"直线瞬移",			std::bind(&CBnsScript::Script_LineTelport,					this) },
		{ L"高度瞬移",			std::bind(&CBnsScript::Script_HeightTelport,				this) },
		{ L"清怪",				std::bind(&CBnsScript::Script_ClearAroundMonster,			this) },
		{ L"群怪",				std::bind(&CBnsScript::Script_AttractMonster,				this) },
		{ L"设置打怪捡物",		std::bind(&CBnsScript::Script_SetAutoTakItemMode,			this) },
		{ L"设置卡刀模式",		std::bind(&CBnsScript::Script_SetKaDaoMode,					this) },
		{ L"石像颜色任务",		std::bind(&CBnsScript::Script_ChooseColorTask,				this) },
		{ L"冰火罩子任务",		std::bind(&CBnsScript::Script_IceFireShieldTask,			this) },
		{ L"杀BOSS",			std::bind(&CBnsScript::Script_KillBoss,						this) },
		{ L"杀怪",				std::bind(&CBnsScript::Script_FarmAround,					this) },
		{ L"设置变量",			std::bind(&CBnsScript::Script_SetVariable,					this) },
		{ L"技能加点",			std::bind(&CBnsScript::Script_UpateSkill,					this) },
		{ L"动作",				std::bind(&CBnsScript::Script_DoAction,						this) },
		{ L"学习技能",			std::bind(&CBnsScript::Script_StartLearnSkill,				this) },
		{ L"遗忘所有技能",		std::bind(&CBnsScript::Script_ForgotSkill,					this) },
		{ L"存仓",				std::bind(&CBnsScript::Script_SaveWareHouse,				this) },
		{ L"轮殴4个BOSS",		std::bind(&CBnsScript::Script_Farm4Boss,					this) },
		{ L"设置仓库号交易物品", std::bind(&CBnsScript::Script_SetWareHouseItem,			this) },
		{ L"仓库号交易",		std::bind(&CBnsScript::Script_WareHouseDeal,				this) },
		{ L"取仓库号物品",		std::bind(&CBnsScript::Script_TakeWareHouse,				this) },
		{ L"发呆",				std::bind(&CBnsScript::Script_Trance,						this) },
		{ L"给仓库号物品",		std::bind(&CBnsScript::Script_GiveWareHouse,				this) },
		{ L"气宗杀狗",			std::bind(&CBnsScript::Script_KillDog_By_Kffm,				this) },
		{ L"设置圆形走位",		std::bind(&CBnsScript::Script_SetKffm_Fm_Round,				this) },
		{ L"执行特殊任务",		std::bind(&CBnsScript::Script_ExcuteSpecialTask,			this) },
		{ L"范围杀BOSS",		std::bind(&CBnsScript::Script_KillBoss_By_Dis,				this) },
		{ L"引BOSS",			std::bind(&CBnsScript::Script_AttractBoss,					this) },
		{ L"破门而入",			std::bind(&CBnsScript::Script_BreakDoor,					this) },
		{ L"圆形走位清怪",		std::bind(&CBnsScript::Script_SetKffm_Fm_Round_Around,		this) },
		{ L"等级技能加点",		std::bind(&CBnsScript::Script_AddSkill_By_Level,			this) },
		{ L"背包购物",			std::bind(&CBnsScript::Script_Buy_By_Bag,					this) },
		{ L"取仓",				std::bind(&CBnsScript::Script_PullWareHouse,				this) },
		{ L"日常任务进度",		std::bind(&CBnsScript::Script_UpdateDailyTask,				this) },
		{ L"任务换线",			std::bind(&CBnsScript::Script_TaskChannel,					this) },
		{ L"怪物存在",			std::bind(&CBnsScript::Script_ExistMonster,					this) },
		{ L"怪物不存在",		std::bind(&CBnsScript::Script_UnExistMonster,				this) },
		{ L"设置技能使用",		std::bind(&CBnsScript::Script_SetDisableSkill,				this) },
		{ L"逃脱",				std::bind(&CBnsScript::Script_Escape,						this) },
		{ L"设置接收礼箱名字",	std::bind(&CBnsScript::Script_SetRecvGiftName,				this) },
		{ L"开始接收礼箱",		std::bind(&CBnsScript::Script_StartRecvGift,				this) },
		{ L"等待怪物死亡",		std::bind(&CBnsScript::Script_ExistMonster_HP,				this) },
		{ L"范围抱起尸体",		std::bind(&CBnsScript::Script_PickCorpse_By_SetPoint,		this) },
		{ L"副本组队",			std::bind(&CBnsScript::Script_JoinDungoenTeam,				this) },
		{ L"副本离队",			std::bind(&CBnsScript::Script_LeaveDungoenTeam,				this) },
		{ L"设置副本超时",		std::bind(&CBnsScript::Script_SetDungoenTime,				this) },
		{ L"判断副本超时",		std::bind(&CBnsScript::Script_IsDungoenTimeOut,				this) },
		{ L"判断接日常任务",	std::bind(&CBnsScript::Script_JudgePickDailyTask,			this) },
		{ L"队伍集合",			std::bind(&CBnsScript::Script_TeamGather,					this) },
		{ L"召唤拿猫",			std::bind(&CBnsScript::Script_TakeCatForSummoner,			this) },
		{ L"设置副本次数超时",	std::bind(&CBnsScript::Script_SetTimeoutByInstanceCount,	this) },
		{ L"切换队伍频道",		std::bind(&CBnsScript::Script_SetTeamChannel,				this) },
		{ L"给猫回血",			std::bind(&CBnsScript::Script_SupplementCat,				this) },
		{ L"等待开门",			std::bind(&CBnsScript::Script_WaitToOpenTheDoor,			this) },
		{ L"圆形走位任务清怪",	std::bind(&CBnsScript::Script_ClearMonsterForTaskByRound,	this) },
		{ L"等待怪物接近",		std::bind(&CBnsScript::Script_WaitToExistMonster,			this) },
		{ L"白仙修练场",		std::bind(&CBnsScript::Script_BaiXianPratice,				this) },
		{ L"设置死亡坐标",		std::bind(&CBnsScript::Script_SetDeadPoint,					this) },
		{ L"初始化脚本坐标",	std::bind(&CBnsScript::Script_InitDungonePoint,				this) },
		{ L"清空脚本坐标",		std::bind(&CBnsScript::Script_ClearDungonePoint,			this) },
		{ L"清除死亡坐标",		std::bind(&CBnsScript::Script_ClearDeadPoint,				this) },
		{ L"设置死亡回调",		std::bind(&CBnsScript::Script_SetDeadCallBack,				this) },
		{ L"扩展背包空间",		std::bind(&CBnsScript::Script_ExpandWarehouse,				this) },
		{ L"鬼门关防守",		std::bind(&CBnsScript::Script_GuiMenDefence,				this) },
		{ L"最低队伍人数",		std::bind(&CBnsScript::Script_SetLeaseTeamMemberCount,		this) },
		{ L"设置云配置",		std::bind(&CBnsScript::Script_SetCloudConfig,				this) },
		{ L"黑森林防守",		std::bind(&CBnsScript::Script_BlackForestDefence,			this) },
		{ L"设置队伍云进度",	std::bind(&CBnsScript::Script_SetTeamSchedule,				this) },
		{ L"等待怪物靠近",		std::bind(&CBnsScript::Script_WaitToClose,					this) },
		{ L"召回猫",			std::bind(&CBnsScript::Script_CallBackCat,					this) },
		{ L"副本走最近点",		std::bind(&CBnsScript::Script_DungoneMove_By_SetPoint,		this) },
		{ L"离开副本传送门",	std::bind(&CBnsScript::Script_LeaveGungone,					this) },
		{ L"捡盾反弹",			std::bind(&CBnsScript::Script_TakeShield_To_Rebound,		this) },
		{ L"海盗船目标清怪",	std::bind(&CBnsScript::Script_TarClear_SilentBoat,			this) },
		{ L"检查物品数量",		std::bind(&CBnsScript::Script_CheckItemCount,				this) },
		{ L"加属性点",			std::bind(&CBnsScript::Script_AddPlayerAttribute,			this) },
		{ L"召唤50级任务",		std::bind(&CBnsScript::Script_QuickSummonerLevel,			this) },
		{ L"设置浮空",			std::bind(&CBnsScript::Script_SetFloating,					this) },
		{ L"浮空走路",			std::bind(&CBnsScript::Script_MoveOnSky,					this) },
		{ L"喊话",				std::bind(&CBnsScript::Script_Talk,							this) },
		{ L"邮寄停止挂机",		std::bind(&CBnsScript::Script_Mail_StopGame,				this) },
		{ L"按键对话",			std::bind(&CBnsScript::Script_NpcTalk,						this) },
		{ L"跑",				std::bind(&CBnsScript::Script_Run,							this) },
		{ L"复活",				std::bind(&CBnsScript::Script_Resurrect,					this) },
		{ L"神行跳",			std::bind(&CBnsScript::Script_JumpRun,						this) },
		{ L"领取星币",			std::bind(&CBnsScript::Script_TakeStarCoin,					this) },
		{ L"星币商店",			std::bind(&CBnsScript::Script_ShopStore,					this) },
		{ L"雪玉宫冰火鸟",		std::bind(&CBnsScript::Script_XueYuGong_IceFireBird,		this) },
		{ L"执行日常任务",		std::bind(&CBnsScript::Script_ExcuteDailyTask,				this) },
		{ L"执行一次性任务",	std::bind(&CBnsScript::Script_ExcuteOneTimeTask,			this) },
		{ L"设置副本进度",		std::bind(&CBnsScript::Script_SetDungoenSchedule,			this) },
		{ L"设置副本难度",		std::bind(&CBnsScript::Script_SetDifficultyForTeam,			this) },
		{ L"设置捡物竞拍",		std::bind(&CBnsScript::Script_SetPartyAuction,				this) },
		{ L"解印八卦",			std::bind(&CBnsScript::Script_UntieBaGua,					this) },
		{ L"坐标集合",			std::bind(&CBnsScript::Script_PointGather,					this) },
		{ L"副本开门",			std::bind(&CBnsScript::Script_OpenDoor,						this) },
		{ L"判断设置坐标距离",  std::bind(&CBnsScript::Script_CheckSetPointDis,				this) },
		{ L"Stop",				std::bind(&CBnsScript::Script_Stop,							this) },
		{ L"添加仓库号交易物品", std::bind(&CBnsScript::Script_AddWarehouseItem, this) },
		{ L"领取级别奖励", std::bind(&CBnsScript::Script_ReceiveLvPrize, this) },
	};
	
	for (CONST auto& itm : VecCustMethod)
		AddCustomeFunAddr(itm);
}

BOOL CBnsScript::ReleaseBnsScript()
{
	LOG_CF_D(L"CBnsScript::ReleaseBnsScript!");
	bExistHelpThread = TRUE;
	if (hHelpThread != NULL && ::WaitForSingleObject(hHelpThread, 3000) == WAIT_TIMEOUT)
		::TerminateThread(hHelpThread, 0);

#if BnsProject_DLL
	CClientTeam::GetInstance().StopJoinTeamThread();
#endif
	::CloseHandle(hHelpThread);
	hHelpThread = NULL;
	return TRUE;
}
 
unsigned WINAPI CBnsScript::_HelpThread(LPVOID lpParm)
{
	MyTools::CLEchoException::GetInstance().InvokeAction(__FUNCTIONW__, [&] 
	{
		em_Player_Classes emPlayerClasses = CPerson::GetInstance().GetClasses();

		static_cast<CBnsScript *>(lpParm)->bExistHelpThread = FALSE;
		while (GameRun && !static_cast<CBnsScript *>(lpParm)->bExistHelpThread)
		{
			BnsSleep(300);
			DWORD dwValue = CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NMAE_ISPUASEHELPTHREAD);
			if (dwValue == 0x1)
				continue;

			if (EMPTY_PERSONHP)
				continue;

#if BnsProject_Dlg
			if (CPersonFunction::GetInstance().IsGameChart() && GameRun)
			{
				//CSomeFun::GetInstance().SimlationKey(VK_ESCAPE);
				MyTools::CLPublic::SimulationKey(VK_ESCAPE, CGameVariable::GetInstance().GetAccShareInfo()->hGameWnd);
				continue;
		}
#endif

			// 是否有动画出现
			if (CPersonFunction::GetInstance().IsGameLoad())
				continue;

			CSomeFun::GetInstance().SwitchViewOfAngel();

			// 是否要用倒地技能
			if (CFarmSkill::GetInstance().UseFloorSkill(emPlayerClasses))
				continue;

			// 是否需要啃药
			CBnsScript::SupplementHp(CPerson::GetInstance().GetLevel() >= 45 ? 60 : 30);

		}
	});
	LOG_CF_D( L"退出吃药,倒地技能,动画 !线程! 如果是真的需要退出的请无视这条!bExistHelpThread=%d,GameRun=%d ", static_cast<CBnsScript *>(lpParm)->bExistHelpThread, GameRun ? 1 : 0);
	return 0;
}

BOOL CBnsScript::SupplementHp(DWORD dwPercentHp)
{
	static ULONGLONG dwUseItemTick = 0;
	if (CPerson::GetInstance().GetPercentHp() > dwPercentHp || static_cast<DWORD>(::GetTickCount64() - dwUseItemTick) < 60 * 1000)
		return FALSE;

	return CBagFunction::GetInstance().DoItem_By_ExistItem_To_Name(恢复药, [&dwPercentHp](CONST CBag& itm){
		//LOG_CF_D( L"吃药了!HP=%%%d, 设置=%%%d", CPerson::GetInstance().GetPercentHp(), dwPercentHp);
		itm.UseItem();
		BnsSleep(300);
		if (CPersonFunction::GetInstance().ExistBuff_Person(0x46A95BA, nullptr)) // 恢复BUFF, 使用成功!
		{
			LOG_CF_D( L"身上有恢复BUFF, 使用成功!");
			dwUseItemTick = ::GetTickCount64();
		}
	});
}

CONST std::wstring CBnsScript::GetUnEmptyObjName(CONST std::wstring& cwstr)
{
	return cwstr == L"0" ? std::wstring(L"") : cwstr;
}

BOOL CBnsScript::IsExcuteNext()
{
	BOOL bRetCode = TRUE;
	while (GameRun && EMPTY_PERSONHP && CFindPathScript::GetInstance().ExistDeadPoint())
	{
		LOG_CF_D( L"存在死亡坐标! 开始跑!");
		CPersonFunction::GetInstance().Resurrect();
		CSomeFun::GetInstance().SetSpeed(3.0f);
		CFindPathScript::GetInstance().MoveToDeadPoint(CPerson::GetInstance().GetPoint());
		LOG_CF_D( L"重新执行上一条命令!");
		bRetCode = FALSE;
	}
	return bRetCode;
}


RealBnsScriptFunction(Script_Move)
{
	BnsScript_CheckParmCount(3);
	Point TarPoint(BnsScript_GetFLOATParm(0), BnsScript_GetFLOATParm(1), BnsScript_GetFLOATParm(2));
	//LOG_CF_D(L"走到(%d,%d,%d)", TarPoint.GetX(), TarPoint.GetY(),TarPoint.GetZ());
	return BnsMove(TarPoint, CSomeFun::GetInstance().GetSpeed() > 3.0f ? 3.0f : 2.0f);
}

RealBnsScriptFunction(Script_MoveTo)
{
	BnsScript_CheckParmCount(4);
	return CPersonFunction::GetInstance().MoveToPoint(Point(BnsScript_GetFLOATParm(0), BnsScript_GetFLOATParm(1), BnsScript_GetFLOATParm(2)), 3.0f, BnsScript_GetFLOATParm(3));
}

RealBnsScriptFunction(Script_UseSkill)
{
	BnsScript_CheckParmCount(1);
	CFarmSkill::GetInstance().UseSkill_By_SkillId(BnsScript_GetDWORDParm(0));
	return TRUE;
}

RealBnsScriptFunction(Script_Sleep)
{
	BnsScript_CheckParmCount(1);

	DWORD dwSleepTime = BnsScript_GetDexParm(0);
	if (dwSleepTime < 5 * 1000)
		return BnsSleep(dwSleepTime);

	// 记录当前Tick
	auto ulTick = ::GetTickCount64();
	CBagAction::GetInstance().QuickRepairWeaspon();

	// 如果发现修理锤子耗时超过了它休眠的时间, 那就直接return
	DWORD dwSpendTime = static_cast<DWORD>(::GetTickCount64() - ulTick);
	if (dwSpendTime >= dwSleepTime)
		return TRUE;

	// 减去需要Sleep的Tick
	dwSleepTime -= dwSpendTime;
	return BnsSleep(dwSleepTime);
}

RealBnsScriptFunction(Script_Farm_By_Dis)
{
	BnsScript_CheckParmCount(1);

	return CFarm::GetInstance().ClearAroundMonster(FixPointInfo(CBaseObject::GetPersonPoint(), BnsScript_GetFLOATParm(0)), []{return FALSE; });
}
 
RealBnsScriptFunction(Script_Sell)
{
	BnsScript_CheckParmCount(1);
	CNpcExtend::GetInstance().SellItem_By_Npc(BnsScript_GetWCHARParm(0));
	return TRUE;
}

RealBnsScriptFunction(Script_Simulationkey)
{
	BnsScript_CheckParmCount(1);

	struct AsciiKey
	{
	 std::wstring wsText;
		DWORD dwKeyAscii;
	};
	CONST static std::vector<AsciiKey> KeyVec = {
		{ L"1", VK_1 },		{ L"2", VK_2 },		{ L"3", VK_3 },		{ L"4", VK_4 },
		{ L"R", 'R' },		{ L"T", 'T' },		{ L"Q", 'Q' },		{ L"E", 'E' },
		{ L"Z", 'Z' },		{ L"X", 'X' },		{ L"C", 'C' },		{ L"Y", 'Y' },
		{ L"N", 'N' },		{ L"C", 'C' },		{ L"Tab", VK_TAB },	{ L"Jump", VK_SPACE },
		{ L"F", 'F' }, { L"Esc", VK_ESCAPE }, { L"V", 'V' }, { L"K", 'K' }, 
		{ L"8", VK_8 }, { L"Q", 'Q' }, { L"X", 'X' }, { L"W", 'W' },{ L"A", 'A' },{ L"D", 'D' },{ L"S", 'S' },
	};

	AsciiKey AsciiKey_;
	if (MyTools::CLPublic::Vec_find_if(KeyVec, &AsciiKey_, [&](CONST AsciiKey& itm){ return itm.wsText == BnsScript_GetWCHARParm(0); }))
	{
		return AsciiKey_.dwKeyAscii == VK_ESCAPE ? MyTools::CLPublic::SimulationKey(VK_ESCAPE, CGameVariable::GetInstance().GetAccShareInfo()->hGameWnd) : CSomeFun::GetInstance().SimlationKey(AsciiKey_.dwKeyAscii);
	}

	LOG_MSG_CF(L"按键:%s 不支持!", BnsScript_GetWCHARParm(0).c_str());
	return FALSE;
}

RealBnsScriptFunction(Script_UseItem)
{
	BnsScript_CheckParmCount(2);

	CBagFunction::GetInstance().DoItem_By_ExistItem_To_Name(BnsScript_GetWCHARParm(0), [&](CONST CBag& itm){
		itm.UseItem();
		BnsSleep(BnsScript_GetDexParm(1));
	});
	return TRUE;
}

RealBnsScriptFunction(Script_SetAngle)
{
	BnsScript_CheckParmCount(2);
	auto wsType = BnsScript_GetWCHARParm(0);

	if (wsType == L"Npc")
		CNpcExtend::GetInstance().DoAction_By_ExistNpc_For_Name(BnsScript_GetWCHARParm(1), CBaseObject::CreateFixPointInfo(30.0f), [](CONST CNpc& Npc){ Npc.SetAngle(); });
	else if (wsType == L"Monster")
		CMonsterExtend::GetInstance().DoAction_By_MonsterName(BnsScript_GetWCHARParm(1), CBaseObject::CreateFixPointInfo(30.0f), [](CONST CMonster& Monster){ Monster.SetAngle(); });
	else if (wsType == L"Corpse")
		CResExtend::GetInstance().DoAction_By_ExistCorpse_For_Name(BnsScript_GetWCHARParm(1), CBaseObject::CreateFixPointInfo(30.0f), [](CONST CCorpse& Corpse){ Corpse.SetAngle(); });
	else if (wsType == L"Point")
		CBaseObject::SetAngle(CSomeFun::GetInstance().GetPoint_By_ScriptParm(BnsScript_GetWCHARParm(1)));
	else
		LOG_MSG_CF(L"Invaild Type:%s", wsType.c_str());

	return TRUE;
}

RealBnsScriptFunction(Script_Farm_By_Task)
{
	// TaskId, ScheduleId, Dis
	BnsScript_CheckParmCount(3);

	TaskInfoParm TaskParm(BnsScript_GetDWORDParm(0), BnsScript_GetDWORDParm(1));
	
	_VecPoint.push_back(CPerson::GetInstance().GetPoint());
	CFarm::GetInstance().FarmAround_By_PointList(CBaseObject::CreateFixPointInfo(BnsScript_GetFLOATParm(2)), _VecPoint, [&TaskParm]{return CTask::GetInstance().IsCompletionTask(TaskParm); });
	_VecPoint.clear();
	return TRUE;
}

RealBnsScriptFunction(Script_TakeItem_By_DropItem)
{
	BnsScript_CheckParmCount(1);

	return CResAction::GetInstance().TakeDropItem(CBaseObject::CreateFixPointInfo(BnsScript_GetFLOATParm(0)), BnsScript_GetFLOATParm(0));
}

RealBnsScriptFunction(Script_TakeItem_By_Corpse)
{
	BnsScript_CheckParmCount(4);

	TaskInfoParm TaskInfoParm_(BnsScript_GetDWORDParm(0), BnsScript_GetDWORDParm(1));
	return (TaskInfoParm_.dwTaskId == 0 && TaskInfoParm_.dwScheduleId == 0) ? \
		CFarm::GetInstance().TakeCorpse(NULL, CBaseObject::CreateFixPointInfo(BnsScript_GetFLOATParm(2)), GetUnEmptyObjName(BnsScript_GetWCHARParm(3))) : \
		CFarm::GetInstance().TakeCorpseItem(TaskInfoParm_, CBaseObject::CreateFixPointInfo(BnsScript_GetFLOATParm(2)), GetUnEmptyObjName(BnsScript_GetWCHARParm(3)));
}

RealBnsScriptFunction(Script_PickCorpse)
{
	BnsScript_CheckParmCount(3);

	return CFarm::GetInstance().TakeCorpse(BnsScript_GetDWORDParm(1), CBaseObject::CreateFixPointInfo(BnsScript_GetFLOATParm(2)), GetUnEmptyObjName(BnsScript_GetWCHARParm(0)));
}

RealBnsScriptFunction(Script_PickDropItem)
{
	BnsScript_CheckParmCount(2);

	return CFarm::GetInstance().PickCorpse(BnsScript_GetDWORDParm(0), CBaseObject::CreateFixPointInfo(BnsScript_GetFLOATParm(1)));
}

RealBnsScriptFunction(Script_PickDropItem_By_SetPoint)
{
	BnsScript_CheckParmCount(2);

	_VecPoint.push_back(CPerson::GetInstance().GetPoint());
	CFarm::GetInstance().PickCorpse_By_PointList(BnsScript_GetDWORDParm(0), CBaseObject::CreateFixPointInfo(BnsScript_GetFLOATParm(1)), _VecPoint);
	_VecPoint.clear();
	return TRUE;
}

RealBnsScriptFunction(Script_TakeItem_By_Npc)
{
	BnsScript_CheckParmCount(3);

	TaskInfoParm TaskInfoParm_(BnsScript_GetDWORDParm(0), BnsScript_GetDWORDParm(1));
	return TaskInfoParm_.dwTaskId == NULL && TaskInfoParm_.dwScheduleId == NULL ? \
		CFarm::GetInstance().TakeNpcItem(L"", CBaseObject::CreateFixPointInfo(BnsScript_GetFLOATParm(2))) : \
		CFarm::GetInstance().TakeNpcItem(TaskInfoParm_, CBaseObject::CreateFixPointInfo(BnsScript_GetFLOATParm(2)), L"");
}

RealBnsScriptFunction(Script_TakeItem_By_Npc_By_SetPoint)
{
	BnsScript_CheckParmCount(4);

	_VecPoint.push_back(CPerson::GetInstance().GetPoint());
	CFarm::GetInstance().TakeNpcItem_By_PointList(TaskInfoParm(BnsScript_GetDWORDParm(0), BnsScript_GetDWORDParm(1)), CBaseObject::CreateFixPointInfo(BnsScript_GetFLOATParm(2)), BnsScript_GetWCHARParm(3), _VecPoint);
	_VecPoint.clear();
	return TRUE;
}

RealBnsScriptFunction(Script_MessageBox)
{
	BnsScript_CheckParmCount(1);

	if (GameRun && !_bExitScript)
		LOG_MSG_CF(L"%s", BnsScript_GetWCHARParm(0).c_str());

	return TRUE;
}

RealBnsScriptFunction(Script_WearEqui)
{
	BnsScript_CheckParmCount(1);
	CONST std::wstring wsName = BnsScript_GetWCHARParm(0);
	UINT uFindCount = 0;
	UINT uWearCount = 0;
	while (GameRun && !EMPTY_PERSONHP && uFindCount < 3)
	{
		CBag Item;
		// 从背包里面搜索装备
		if (!CBagFunction::GetInstance().ExistItem_By_Name(wsName, &Item))
		{
			LOG_CF_D( L"物品:%s 不存在", wsName.c_str());
			// 是不是穿在身上了
			if (CBagFunction::GetInstance().ExistEqui_By_Name(wsName,nullptr))
			{
				LOG_CF_D( L"身上已经存在该装备:%s", wsName.c_str());
				break;
			}

			uFindCount += 1;
			BnsSleep(5000);
			continue;
		}

		// 获取背包装备的类型
		CBag::em_Equi_Type emEquiType = static_cast<CBag::em_Equi_Type>(Item.GetItemType());

		LOG_CF_D( L"穿装备!:%s", wsName.c_str());
		Item.WearEqui();
		CBag Equi;
		// 根据类型去获取当前身上的装备
		if (CBagFunction::GetInstance().GetEqui_By_Type(emEquiType, &Equi))
		{
			LOG_CF_D( L"身上的装备是:%s, Type=%X", Equi.GetName().c_str(), Equi.GetItemType());
			// 判断身上的装备名字 是否和 传进来的参数名称一样
			if (Equi.GetName() == wsName)
			{
				LOG_CF_D( L"穿装备成功!");
				break;
			}
			if (uWearCount++ >= 3)
			{
				LOG_CF_D(L"3次穿装备都失败, 先脱装备好了!");
				Equi.RelieveEqui();
				uWearCount = 0;
			}
		}

	}
	return TRUE;
}
 
RealBnsScriptFunction(Script_DeliverCity)
{
	BnsScript_CheckParmCount(1);

	// Set CheckPointTimeout = 0, UnCheck Move Timeout!
	DWORD dwOldTimeOut = CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CHECKPAUSEPOINT, NULL);

	DWORD dwCityId = BnsScript_GetDWORDParm(0);
	CConsoleLog(L"准备使用遁术!");
	while ((!CPersonFunction::GetInstance().IsGameLoad() || CPersonFunction::GetInstance().GetCityId() != dwCityId)  && GameRun && !EMPTY_PERSONHP)
	{
		if (CPerson::GetInstance().GetPersonStatus() != em_WarStatus_None)
		{
			LOG_CF_D( L"等待脱战中……");
			CFarm::GetInstance().ClearAround_TarIdSelf(CBaseObject::CreateFixPointInfo(16.0f));
			BnsSleep(3000);
			continue;
		}

		if (!CPerson::GetInstance().IsLoadProgressBar())
		{
			LOG_CF_D( L"使用遁术中…… dwCityId=%X", dwCityId);
			CPersonFunction::GetInstance().TransportCiry(dwCityId);
			BnsSleep(5 * 1000);
			CSomeFun::GetInstance().TimeOut_Condiction_GameRun(20 * 1000, []{ return !CPersonFunction::GetInstance().IsGameLoad() && CPerson::GetInstance().GetPersonStatus() == em_WarStatus_None; });
		}
	}
	CPersonFunction::GetInstance().WaitGameLoad();

	// Print Msg to Console!
	CConsoleLog(L"使用遁术成功!");

	LOG_CF_D( L"使用遁术成功! 等待过图完毕!");

	// Resume Check Move Timeout!
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CHECKPAUSEPOINT, dwOldTimeOut);
	return BnsSleep(3 * 1000);
}

RealBnsScriptFunction(Script_GameChat)
{
	BnsScript_CheckParmCount(0);
	return CPersonFunction::GetInstance().WaitGameLoad();
}

RealBnsScriptFunction(Script_SetSpeed)
{
	BnsScript_CheckParmCount(1);

	CSomeFun::GetInstance().SetSpeed(BnsScript_GetFLOATParm(0));
	return TRUE;
}

RealBnsScriptFunction(Script_SetOptimization)
{
	BnsScript_CheckParmCount(1);

	CSomeFun::GetInstance().SetOptimization(BnsScript_GetDWORDParm(0));
	return TRUE;
}

RealBnsScriptFunction(Script_CleanBag)
{
	BnsScript_CheckParmCount(0);

	CBagAction::GetInstance().ExplainItem();
	CBagAction::GetInstance().DiscardItem();
	CBagFunction::GetInstance().CleanBag();
	return TRUE;
}

RealBnsScriptFunction(Script_SumitMainTask)
{
	BnsScript_CheckParmCount(10);
	SumitTask_Src SumitTask_Src_;
	SumitTask_Src_.dwScheduleId = BnsScript_GetDWORDParm(2);
	SumitTask_Src_.dwTaskId     = BnsScript_GetDWORDParm(3);
	SumitTask_Src_.dwParm3      = BnsScript_GetDWORDParm(4);
	SumitTask_Src_.dwParm4      = BnsScript_GetDWORDParm(5);
	SumitTask_Src_.dwParm5      = BnsScript_GetDWORDParm(6);
	SumitTask_Src_.dwObjId      = BnsScript_GetDWORDParm(7);
	SumitTask_Src_.dwObjIdParm  = BnsScript_GetDWORDParm(8);
	SumitTask_Src_.dwItemIndex  = NULL;
	CONST std::wstring& Type_	= BnsScript_GetWCHARParm(0);


	if (CObjectFunction::GetInstance().IsShow_In_GameUi(L"TalkControlPanel"))
	{
		LOG_CF_D( L"关闭Npc聊天框!");
		MyTools::CLPublic::SimulationKey(VK_ESCAPE, CGameVariable::GetInstance().GetAccShareInfo()->hGameWnd);
		BnsSleep(2000);
	}

	LOG_CF_D( L"wstrType=%s", Type_.c_str());

	if (Type_ == L"Npc")
		CTaskExtend::GetInstance().SumitTask_By_Parm(SumitTask_Src_, GetUnEmptyObjName(BnsScript_GetWCHARParm(1)), GetUnEmptyObjName(BnsScript_GetWCHARParm(9)));
	else if (Type_ == L"Item")
		CTaskExtend::GetInstance().SumitTask_By_Item(SumitTask_Src_, GetUnEmptyObjName(BnsScript_GetWCHARParm(1)));
	else if (Type_ == L"Item2")
		CTaskExtend::GetInstance().SumitTask_By_Item(SumitTask_Src_, GetUnEmptyObjName(BnsScript_GetWCHARParm(1)));
	else if (Type_ == L"Parm")
		CTaskExtend::GetInstance().SumitTask_By_Parm(SumitTask_Src_, std::wstring(L""), std::wstring(L""));
	else
		LOG_MSG_CF(L"pwstrType Err: %s", Type_.c_str());

	return TRUE;
}

RealBnsScriptFunction(Script_PickBLTask)
{
	BnsScript_CheckParmCount(7);

	// Record Sumit BL Task Content!
	_LastPickBLTaskContent.SumitBLTask_Src_.dwNpcId     = BnsScript_GetDWORDParm(2);
	_LastPickBLTaskContent.SumitBLTask_Src_.dwNpcIdParm = BnsScript_GetDWORDParm(3);
	_LastPickBLTaskContent.SumitBLTask_Src_.dwTaskId    = BnsScript_GetDWORDParm(4);
	_LastPickBLTaskContent.SumitBLTask_Src_.dwParm4     = BnsScript_GetDWORDParm(5);
	_LastPickBLTaskContent.SumitBLTask_Src_.dwParm5     = BnsScript_GetDWORDParm(6);
	_LastPickBLTaskContent.wsName                       = BnsScript_GetWCHARParm(1);
	_LastPickBLTaskContent.wsType                       = BnsScript_GetWCHARParm(0);


	if (_LastPickBLTaskContent.wsType == L"Npc")
		CTaskExtend::GetInstance().PickBLTask_By_Npc(_LastPickBLTaskContent.SumitBLTask_Src_, BnsScript_GetWCHARParm(1));
	else if (_LastPickBLTaskContent.wsType == L"Item")
		CTaskExtend::GetInstance().PickBLTask_By_Item(_LastPickBLTaskContent.SumitBLTask_Src_, BnsScript_GetWCHARParm(1));
	else if (_LastPickBLTaskContent.wsType == L"Parm")
		CTaskExtend::GetInstance().PickBLTask_By_Parm(_LastPickBLTaskContent.SumitBLTask_Src_, [](SumitBLTask_Src&){ return TRUE; });
	else
		LOG_MSG_CF(L"pwstrType Err: %s", _LastPickBLTaskContent.wsType.c_str());

	return TRUE;
}

RealBnsScriptFunction(Script_SetPoint)
{
	BnsScript_CheckParmCount(3);
	//LOG_CF_D( L"设置坐标(%d,%d,%d)", (int)BnsScript_GetFLOATParm(0), (int)BnsScript_GetFLOATParm(1), (int)BnsScript_GetFLOATParm(2));

	_VecPoint.push_back(Point(BnsScript_GetFLOATParm(0), BnsScript_GetFLOATParm(1), BnsScript_GetFLOATParm(2)));
	return TRUE;
}

RealBnsScriptFunction(Script_MovePoint_By_SetPoint)
{
	BnsScript_CheckParmCount(0);

	float fMinDis = MAX_MOVE_DIS;

	INT uIndex = -1;
	for (UINT i = 0; i < _VecPoint.size(); ++i)
	{
		float fDis = fabs(CBaseObject::GetPersonDis(_VecPoint.at(i)));
		if (fDis < fMinDis)
		{
			uIndex = i;
			fMinDis = fDis;
		}
	}

	if (uIndex == -1)
	{
		fMinDis = 10000;
		Point NearestPoint;
		for (CONST auto& itm : _VecPoint)
		{
			float fDis = fabs(CBaseObject::GetPersonDis(itm));
			if (fDis < fMinDis)
			{
				fMinDis = fDis;
				NearestPoint = itm;
			}
		}

		if (fMinDis < 10000)
		{
			LOG_CF_D(L"离当前坐标最近的坐标是:[%d,%d,%d], 距离是:%.2f, 超过了%.2f, 不走!", NearestPoint.GetX(), NearestPoint.GetY(), NearestPoint.GetZ(), CBaseObject::GetPersonDis(NearestPoint), MAX_MOVE_DIS);
		}
		_VecPoint.clear();
		return TRUE;
	}

	auto& tmpPoint = _VecPoint.at(uIndex);
	LOG_CF_D(L"开始走最近点, 从第%d个坐标[%d,%d,%d]开始走! 距离:%.2f", uIndex, tmpPoint.GetX(), tmpPoint.GetY(), tmpPoint.GetZ(), fMinDis);
	for (UINT i = static_cast<UINT>(uIndex); i < _VecPoint.size() && GameRun && !EMPTY_PERSONHP; ++i)
	{
		CPersonFunction::GetInstance().WaitGameLoad();

		if (!BnsMove(_VecPoint.at(i), CSomeFun::GetInstance().GetSpeed() > 3.0f ? 3.0f : 2.0f))
		{
			LOG_CF_E(L"走最近点检测到无法到达的坐标,需要重新执行任务!");
			_VecPoint.clear();
			return FALSE;
		}
	}

	_VecPoint.clear();
	return TRUE;
}

RealBnsScriptFunction(Script_Back_MovePoint_By_SetPoint)
{
	BnsScript_CheckParmCount(0);

	float fMinDis = MAX_MOVE_DIS;

	INT uIndex = -1;
	for (UINT i = 0; i < _VecPoint.size(); ++i)
	{
		float fDis = fabs(CBaseObject::GetPersonDis(_VecPoint.at(i)));
		if (fDis < fMinDis)
		{
			uIndex = i;
			fMinDis = fDis;
		}
	}

	if (uIndex == -1)
	{
		//LOG_CF_D( L"没有离设置坐标最近点至少是%.2fm的坐标", MAX_MOVE_DIS);
		_VecPoint.clear();
		return TRUE;
	}

	for (INT i = uIndex; i >= 0 && GameRun && !EMPTY_PERSONHP; --i)
	{
		CPersonFunction::GetInstance().WaitGameLoad();

		if (!BnsMove(_VecPoint.at(i), 1.0f))
		{
			LOG_CF_E( L"倒着走 检测到无法到达的坐标,需要重新执行任务!");
			_VecPoint.clear();
			return FALSE;
		}
	}

	_VecPoint.clear();
	return TRUE;
}

RealBnsScriptFunction(Script_GrowthEqui)
{
	BnsScript_CheckParmCount(1);

	CBagFunction::Equi_Type EquiType;
	if (CBagFunction::GetInstance().GetEquiType_By_Text(BnsScript_GetWCHARParm(0), EquiType))
		CGameFunction::GetInstance().GrowthEqui(EquiType.emEquiType);
	return TRUE;
}

RealBnsScriptFunction(Script_OpenBox)
{
	BnsScript_CheckParmCount(1);

	CBag BoxItem;
	while (GameRun && !EMPTY_PERSONHP && CBagFunction::GetInstance().ExistItem_By_Name(BnsScript_GetWCHARParm(0), &BoxItem))
	{
		CPersonFunction::GetInstance().WaitToLoadProgressBar(2 * 1000, 10 * 1000, [&BoxItem]{BoxItem.OpenBox();});
	}
	return TRUE;
}

RealBnsScriptFunction(Script_OpenBox_By_Key)
{
	BnsScript_CheckParmCount(3);

	CBag BoxItem;
	CBag KeyItem;
	if (CBagFunction::GetInstance().ExistItem_By_Name(BnsScript_GetWCHARParm(0), &BoxItem) && CBagFunction::GetInstance().ExistItem_By_Name(BnsScript_GetWCHARParm(1), &KeyItem))
	{
		CPersonFunction::GetInstance().WaitToLoadProgressBar(2 * 1000, 10 * 1000, [&]{
			BoxItem.OpenBox_By_Key(KeyItem.GetGId(), BnsScript_GetDWORDParm(2));
		});
	}
	return TRUE;
}

RealBnsScriptFunction(Script_ExplainEqui)
{
	BnsScript_CheckParmCount(0);
	return CBagAction::GetInstance().ExplainItem();
}

RealBnsScriptFunction(Script_SetFixChannel)
{
	BnsScript_CheckParmCount(0);

	return CFarm::GetInstance().SetFixChannel(0);
}

RealBnsScriptFunction(Script_SetRandChannel)
{
	BnsScript_CheckParmCount(0);

	return CFarm::GetInstance().SetRandChannel();
}

RealBnsScriptFunction(Script_FilterMonster)
{
	BnsScript_CheckParmCount(2);

	if (!CFilter::GetInstance().AddMonsterFilter(BnsScript_GetWCHARParm(0), BnsScript_GetWCHARParm(1)))
	{
		LOG_MSG_CF(L"错误的怪物过滤类型:%s", BnsScript_GetWCHARParm(1).c_str());
		return TRUE;
	}

	return TRUE;
}

RealBnsScriptFunction(Script_FollowNpc)
{
	BnsScript_CheckParmCount(5);
	std::wstring wsPlayerName = BnsScript_GetWCHARParm(0);
	FixPointInfo FixPtInfo = CBaseObject::CreateFixPointInfo(30);

	CMonster Monster;
	CNpc Npc;

	return CMonsterExtend::GetInstance().DoAction_By_MonsterName(BnsScript_GetWCHARParm(0), CBaseObject::CreateFixPointInfo(30), [&](CONST CMonster& Monster){
		CFarm::GetInstance().FollowPlayer(Monster, Point(BnsScript_GetFLOATParm(1), BnsScript_GetFLOATParm(2), BnsScript_GetFLOATParm(3)), BnsScript_GetFLOATParm(4));
	}) ? TRUE : CNpcExtend::GetInstance().DoAction_By_ExistNpc_For_Name(BnsScript_GetWCHARParm(0), CBaseObject::CreateFixPointInfo(30), [&](CONST CNpc& Npc){
		CFarm::GetInstance().FollowPlayer(Npc, Point(BnsScript_GetFLOATParm(1), BnsScript_GetFLOATParm(2), BnsScript_GetFLOATParm(3)), BnsScript_GetFLOATParm(4));
	});
}

RealBnsScriptFunction(Script_FilterItem)
{
	BnsScript_CheckParmCount(2);

	if (!CFilter::GetInstance().AddItemFilter(BnsScript_GetWCHARParm(0), BnsScript_GetWCHARParm(1)))
	{
		LOG_MSG_CF(L"错误的物品过滤类型:%s", BnsScript_GetWCHARParm(1).c_str());
	}
	
	return TRUE;
}

RealBnsScriptFunction(Script_Buy)
{
	BnsScript_CheckParmCount(3);
	
	CNpcExtend::GetInstance().DoAction_By_ExistNpc_For_Name(BnsScript_GetWCHARParm(0), CBaseObject::CreateFixPointInfo(10), [&](CONST CNpc& Npc){
		BnsMove(Npc.GetPoint(), 2.0f);
		BnsSleep(2 * 1000);
		Npc.BuyItem(BnsScript_GetDexParm(1), BnsScript_GetDexParm(2));
	});
	return TRUE;
}

RealBnsScriptFunction(Script_ExpandBag)
{
	BnsScript_CheckParmCount(1);

	DWORD dwBagCount = BnsScript_GetDexParm(0);
	while (CObjectFunction::GetInstance().GetMaxBagCount(CObjectFunction::em_Bag_Type_Bag) < dwBagCount)
	{
		LOG_CF_D( L"开背包!, 当前背包容量是:%d, 脚本背包容量是:%d", CObjectFunction::GetInstance().GetMaxBagCount(CObjectFunction::em_Bag_Type_Bag), dwBagCount);
		CBagFunction::GetInstance().CreateMoreBag();
		BnsSleep(1000);
	}
	return TRUE;
}

RealBnsScriptFunction(Script_ExpandWarehouse)
{
	BnsScript_CheckParmCount(3);

	CONST std::wstring& wsType	=	BnsScript_GetWCHARParm(0);
	CONST std::wstring& wsNpcName =	BnsScript_GetWCHARParm(1);
	DWORD dwCount		=	BnsScript_GetDexParm(2);

	if (wsType == L"仓库")
	{
		CNpc Npc;
		while (CObjectFunction::GetInstance().GetMaxBagCount(CObjectFunction::em_Bag_Type_Warehouse) < dwCount)
		{
			LOG_CF_D( L"开背包!, 当前背包容量是:%d, 脚本背包容量是:%d", CObjectFunction::GetInstance().GetMaxBagCount(CObjectFunction::em_Bag_Type_Warehouse), dwCount);
			if (!CNpcExtend::GetInstance().FindNpc_By_Name(wsNpcName,CBaseObject::CreateFixPointInfo(30.0f), &Npc))
			{
				LOG_CF_E( L"附近找不到仓库Npc:%s 来扩展背包!!", wsNpcName.c_str());
				break;
			}

			BnsMove(Npc.GetPoint(), 1);
			BnsSleep(1000);
			Npc.ExpandInvenSlot();
			BnsSleep(1000);
		}
	}
	else if (wsType == L"衣柜")
	{
		CNpc Npc;
		while (CObjectFunction::GetInstance().GetMaxBagCount(CObjectFunction::em_Bag_Type_Warehouse) < dwCount)
		{
			LOG_CF_D( L"开背包!, 当前背包容量是:%d, 脚本背包容量是:%d", CObjectFunction::GetInstance().GetMaxBagCount(CObjectFunction::em_Bag_Type_Warehouse), dwCount);
			if (!CNpcExtend::GetInstance().FindNpc_By_Name(wsNpcName, CBaseObject::CreateFixPointInfo(30.0f), &Npc))
			{
				LOG_CF_E( L"附近找不到仓库Npc:%s 来扩展背包!!", wsNpcName.c_str());
				break;
			}

			BnsMove(Npc.GetPoint(), 1);
			BnsSleep(1000);
			Npc.ExpandClothSlot();
			BnsSleep(1000);
		}
	}
	else if (wsType == L"背包")
	{
		while (CObjectFunction::GetInstance().GetMaxBagCount(CObjectFunction::em_Bag_Type_Bag) < dwCount)
		{
			LOG_CF_D( L"开背包!, 当前背包容量是:%d, 脚本背包容量是:%d", CObjectFunction::GetInstance().GetMaxBagCount(CObjectFunction::em_Bag_Type_Bag), dwCount);
			CBagFunction::GetInstance().CreateMoreBag();
			BnsSleep(1000);
		}
	}
	return TRUE;
}

RealBnsScriptFunction(Script_ExcuteScript)
{
	BnsScript_CheckParmCount(3);

	CONST std::wstring wstrType = BnsScript_GetWCHARParm(1);
	auto InstanceZoneScript = std::make_shared<CInstanceZoneScript>();
	CInstanceZoneScript::_em_InstanceZoneScript_Exit_Type emExitType;
	if (wstrType == L"Count")
		emExitType = CInstanceZoneScript::Script_Exit_Type_Count;
	else if (wstrType == L"Level")
		emExitType = CInstanceZoneScript::Script_Exit_Type_Level;
	else if (wstrType == L"Task")
		emExitType = CInstanceZoneScript::Script_Exit_Type_Task;
	else
	{
		LOG_MSG_CF(L"Invalid Type:%s", wstrType.c_str());
		return FALSE;
	}

	// 关闭任务超时判断
	DWORD dwValue = CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_TASKTIMEOUTID, NULL);

	InstanceZoneScript->Run(BnsScript_GetWCHARParm(0), emExitType, BnsScript_GetWCHARParm(2));

	// 还原任务超时判断
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_TASKTIMEOUTID, dwValue);
	return TRUE;
}

RealBnsScriptFunction(Script_MoveToTarget)
{
	BnsScript_CheckParmCount(3);

	CONST std::wstring wsType = BnsScript_GetWCHARParm(0);
	
	FixPointInfo FixPtInfo = CBaseObject::CreateFixPointInfo(BnsScript_GetFLOATParm(2));

	if (wsType == L"Npc")
	{
		CNpcExtend::GetInstance().DoAction_By_ExistNpc_For_Name(BnsScript_GetWCHARParm(1), FixPtInfo, [&FixPtInfo](CONST CNpc& Npc){
			CPersonFunction::GetInstance().MoveToPoint(Npc.GetPoint(), 2.0f, FixPtInfo.fDis);
			BnsSleep(1000);
		});
	}
	else if (wsType == L"Monster")
	{
		CMonsterExtend::GetInstance().DoAction_By_MonsterName(BnsScript_GetWCHARParm(1), FixPtInfo, [&FixPtInfo](CONST CMonster& Monster){
			CPersonFunction::GetInstance().MoveToPoint(Monster.GetFrontPoint(2.0f), 2.0f, FixPtInfo.fDis);
			BnsSleep(1000);
		});
	}
	else if (wsType == L"Corpse")
	{
		CResExtend::GetInstance().DoAction_By_ExistCorpse_For_Name(wsType, FixPtInfo, [&FixPtInfo](CONST CCorpse& Corpse){
			CPersonFunction::GetInstance().MoveToPoint(Corpse.GetPoint(), 2.0f, FixPtInfo.fDis);
			BnsSleep(1000);
		});
	}
	else if (wsType == L"Boss")
	{
		CMonsterExtend::GetInstance().DoAction_By_MonsterName(BnsScript_GetWCHARParm(1), FixPtInfo, [&FixPtInfo](CONST CMonster& Monster){
			CPersonFunction::GetInstance().MoveToPoint(Monster.GetFrontPoint(2.0f), 2.0f, FixPtInfo.fDis);
			BnsSleep(1000);
		});
	}
	else
	{
		LOG_MSG_CF(L"Invaild Type:%s", wsType.c_str());
	}

	
	
	return TRUE;
}

RealBnsScriptFunction(Script_SetGem)
{
	BnsScript_CheckParmCount(2);

	CBag Gem;
	if (!CBagFunction::GetInstance().ExistItem_By_Name(BnsScript_GetWCHARParm(0), &Gem))
	{
		LOG_CF_D( L"并没有这个宝石:%s", BnsScript_GetWCHARParm(0).c_str());
		return TRUE;
	}

	if (CBagFunction::GetInstance().ExistGem_By_Weaspon(Gem.GetGId()))
	{
		LOG_CF_D( L"宝石:%s 已经存在在武器上了, 不能镶嵌重复的宝石!", BnsScript_GetWCHARParm(0).c_str());
		return TRUE;
	}

	while (GameRun && !CObjectFunction::GetInstance().IsShow_In_GameUi(L"Inventory2Panel"))
	{
		CSomeFun::GetInstance().SimlationKey('I', 200, 800);
	}

	// 警告, 如果有背包的一颗宝石， 和武器那颗宝石一毛一样的话……
	while (GameRun && !EMPTY_PERSONHP)
	{
		std::vector<DWORD> vGem;
		CObjectFunction::GetInstance().GetWeasponGemList(vGem);
		UINT uCount = vGem.size();
		LOG_CF_D( L"宝石装在第%d个孔上!", uCount);
		CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"Inventory2Panel", [&Gem, &uCount](CONST CGameUi& itm){
			Gem.SetGem(static_cast<int>(uCount), itm);
		});
		BnsSleep(3000);
		
		// 判断是否装宝石成功了
		CObjectFunction::GetInstance().GetWeasponGemList(vGem);
		if (vGem.size() != uCount)
		{
			LOG_CF_D( L"装宝石后的数量=%d个, 装成功!", vGem.size());
			break;
		}
	}
	
	return TRUE;
}

RealBnsScriptFunction(Script_ClearGem)
{
	BnsScript_CheckParmCount(0);

	return CBagAction::GetInstance().RemoveALLGem();
}

RealBnsScriptFunction(Script_Untie)
{
	BnsScript_CheckParmCount(3);

	CBag Equi;
	CBag Untie;

	while (GameRun && !EMPTY_PERSONHP && CBagFunction::GetInstance().ExistItem_By_Name(BnsScript_GetWCHARParm(0), &Equi) && CBagFunction::GetInstance().ExistItem_By_Name(BnsScript_GetWCHARParm(1), &Untie))
	{
		Equi.EquiUntie(Untie.GetGId(), BnsScript_GetDWORDParm(2), Untie.GetName().find(L"洪門") != -1 ? 3 : 0);
		BnsSleep(2 * 1000);
	}
	return TRUE;
}

RealBnsScriptFunction(Script_EquiEvolution)
{
	// 装备突破(武器名,祭品名,ID)
	BnsScript_CheckParmCount(3);

	CBag Equi; // 进化的装备(必须在背包)
	CBag Sacrifice; // 进化所需的祭品

	if (!CBagFunction::GetInstance().ExistItem_By_Name(BnsScript_GetWCHARParm(0), &Equi))
	{
		LOG_CF_D(L"找不到武器[%s]", Equi.GetName().c_str());
		return TRUE;
	}	

	if (!CBagFunction::GetInstance().ExistItem_By_Name(BnsScript_GetWCHARParm(1), &Sacrifice))
	{
		LOG_CF_D(L"并突破的祭品没有%s!", BnsScript_GetWCHARParm(1).c_str());
		return TRUE;
	}
	
	Sacrifice.EquiEvolution(Equi, BnsScript_GetDWORDParm(2));
	BnsSleep(2000);
	return TRUE;
}

RealBnsScriptFunction(Script_EquiEndEvolution)
{
	BnsScript_CheckParmCount(3);

	CBag Equi;
	CBag Sacrifice;
	if (CBagFunction::GetInstance().ExistItem_By_Name(BnsScript_GetWCHARParm(0), &Equi) && CBagFunction::GetInstance().ExistItem_By_Name(BnsScript_GetWCHARParm(1), &Sacrifice))
		Equi.EquiEndEvolution(Sacrifice.GetGId(), BnsScript_GetDWORDParm(2));
	return TRUE;
}

RealBnsScriptFunction(Script_RemoveEqui)
{
	BnsScript_CheckParmCount(1);

	CBag Equi;
	// 根据类型去获取当前身上的装备
	if (CBagFunction::GetInstance().GetEqui_By_Type(static_cast<CBag::em_Equi_Type>(BnsScript_GetDWORDParm(0)), &Equi))
		Equi.RelieveEqui();
	return TRUE;;
}

RealBnsScriptFunction(Script_RepairWeapon)
{
	BnsScript_CheckParmCount(1);

	if (CBagFunction::GetInstance().GetEndurance() > BnsScript_GetDexParm(0))
		return TRUE;

	// 最多修理3次, 有可能是锤子数量不足!
	for (int i = 0; i < 3 && GameRun && !EMPTY_PERSONHP && CBagFunction::GetInstance().GetEndurance() < BnsScript_GetDexParm(0); ++i)
	{
		LOG_CF_D( L"需要修理武器!");
		CBag Hammer;
		if (CBagFunction::GetInstance().ExistHammer(&Hammer))
		{
			CPersonFunction::GetInstance().WaitToLoadProgressBar(2 * 1000, 20 * 1000, [&]{Hammer.UseItem(); });
			LOG_CF_D( L"修理完毕!");
		}
		else
		{
			LOG_CF_D(L"没锤子了!");
			break;
		}
	}

	
	return TRUE;
}

RealBnsScriptFunction(Script_SaveCloth)
{
	BnsScript_CheckParmCount(1);

	CBagAction::GetInstance().SaveItem(CObjectFunction::em_Bag_Type::em_Bag_Type_Wardrobe, BnsScript_GetWCHARParm(0));
	return TRUE;
}

RealBnsScriptFunction(Script_ClearMonsterFilter)
{
	CFilter::GetInstance().ClearMonsterFilter();
	return TRUE;
}

RealBnsScriptFunction(Script_ExcuteTask)
{
	BnsScript_CheckParmCount(0);

#ifdef BnsProject_DLL
	auto TaskContent = CTask::GetInstance().GetMianTaskInfo();
	CSomeFun::GetInstance().SendKeepALive(&TaskContent, L"主线中……");
#endif // BnsProject_DLL


	try
	{
		CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CHECKPAUSEPOINT, 3 * 60);
		CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_TASKTIMEOUTID, TRUE);
		auto bExitScript = FALSE;
		while (GameRun && !bExitScript)
		{
			BOOL bRetCode = TRUE;
			std::thread MainTaskThread([&bExitScript, &bRetCode]
			{
				auto pBnsScript = std::make_shared<CBnsScript>();
				auto MainTaskInfo = CTask::GetInstance().GetMianTaskInfo();
				if (MainTaskInfo.dwTaskId == NULL || MainTaskInfo.dwScheduleId == NULL)
				{
					LOG_CF_E( L"找不到主线!");
					bRetCode = FALSE;
					return;
				}
				LOG_CF_D( L"MainTaskInfo=[%X,%X]", MainTaskInfo.dwTaskId, MainTaskInfo.dwScheduleId);
				
#if BnsProject_DLL
				CConsoleLog(MyTools::CCharacter::MakeFormatText(L"正在下载主线%X……", MainTaskInfo.dwTaskId));
				CTransferScript TransferScript;
				std::wstring wsScriptContent = TransferScript.DownLoadScript(MyTools::CCharacter::MakeFormatText(L"%X.inf", MainTaskInfo.dwTaskId));
				if (GameRun && !pBnsScript->Read(em_ReadScript_Type::em_ReadScript_Type_Text,wsScriptContent))
				{
					LOG_MSG_CF(L"解析脚本失败!");
					bRetCode = FALSE;
					return;
				}
#else
				std::wstring wsScriptPath = MyTools::CCharacter::MakeFormatText(L"C:\\Bns\\%X.inf", MainTaskInfo.dwTaskId);
				LOG_CF_D( L"读取脚本文件:%s", wsScriptPath.c_str());
				if (!pBnsScript->Read(em_ReadScript_Type::em_ReadScript_Type_FilePath, wsScriptPath))
				{
					LOG_MSG_CF(L"解析脚本失败!");
					bRetCode = FALSE;
					return;
				}
#endif

				CConsoleLog(L"主线中……");
				while (GameRun)
				{
					TaskInfoParm tmpTaskInfo = CTask::GetInstance().GetMianTaskInfo();

					LOG_CF_D( L"当前主线=[%X,%X]", tmpTaskInfo.dwTaskId, tmpTaskInfo.dwScheduleId);
					if (tmpTaskInfo.dwTaskId != MainTaskInfo.dwTaskId)
					{
						//LOG_CF_D( L"主线Id已经变了, break;");
						break;
					}

					std::wstring wsScriptMethodName = MyTools::CCharacter::MakeFormatText(L"Task_%X", tmpTaskInfo.dwScheduleId);
					LOG_CF_D( L"执行函数:%s", wsScriptMethodName.c_str());
					bRetCode = pBnsScript->Excute(wsScriptMethodName);
					LOG_CF_D( L"执行函数:%s 完毕,bRetCode=%d", wsScriptMethodName.c_str(), bRetCode);
					if (EMPTY_PERSONHP)
					{
						CPersonFunction::GetInstance().Resurrect();
						continue;
					}
					else if (pBnsScript->IsExitScript())
					{
						LOG_CF_D( L"IsExitScript=true,执行任务结束!");
						bExitScript = TRUE;
						break;
					}
					else if (!bRetCode)
					{
						LOG_CF_D( L"执行函数:%s,返回FALSE, 重新开始执行主线!", wsScriptMethodName.c_str());
						break;
					}
					BnsSleep(1000);
				}
			});
			MainTaskThread.join();
			if (!bRetCode)
				return FALSE;

			BnsSleep(1000);
		}
		//LOG_CF_D( L"GameRun=%d, bExistScript=%d", GameRun, bExitScript);
	}
	catch(...)
	{
		LOG_CF_E( L"Script_ExcuteTask出现异常");
	}
	return TRUE;
}

RealBnsScriptFunction(Script_ExcuteFileFun)
{
	BnsScript_CheckParmCount(2);

	BOOL bRetCode = FALSE;
	std::thread ExcuteScriptThread([this, &bRetCode]
	{
		auto pBnsScript = std::make_shared<CBnsScript>();
#if BnsProject_DLL
		CTransferScript TransferScript;
		std::wstring wsScriptContent = TransferScript.DownLoadScript(BnsScript_GetWCHARParm(0));
		if (!pBnsScript->Read(em_ReadScript_Type_Text, wsScriptContent))
		{
			LOG_MSG_CF(L"解析脚本失败!");
			return;
		}
#else
		std::wstring wsScriptPath = MyTools::CCharacter::MakeFormatText(L"C:\\Bns\\%s", BnsScript_GetWCHARParm(0).c_str());
		LOG_CF_D( L"读取脚本文件:%s", wsScriptPath.c_str());
		if (!pBnsScript->Read(em_ReadScript_Type_FilePath, wsScriptPath))
		{
			LOG_MSG_CF(L"解析脚本失败!");
			return;
		}
#endif

		CONST std::wstring wsExcuteFunName = BnsScript_GetWCHARParm(1);
		LOG_CF_D( L"执行函数:%s", wsExcuteFunName.c_str());
		bRetCode = pBnsScript->Excute(wsExcuteFunName.c_str());
		LOG_CF_D( L"函数:%s bRetCode=%X", wsExcuteFunName.c_str(), bRetCode);
	});
	ExcuteScriptThread.join();
	return bRetCode;
}

BOOL CBnsScript::IsExitScript()
{
	return _bExitScript;
}

RealBnsScriptFunction(Script_SetExitFlag)
{
	BnsScript_CheckParmCount(1);
	CONST std::wstring cwsType = BnsScript_GetWCHARParm(0);
	if (cwsType.find(L"PK_") != -1)
	{
		if (cwsType == L"PK_15" && CGameVariable::GetInstance().GetAccShareInfo()->PkSetting.emPkSchedule == em_Pk_Schedule_TaskUpgrade)
		{
			_bExitScript = TRUE;
			//LOG_CF_D( L"成功执行到退出标志:%s, 退出所有!", cwsType.c_str());
			return FALSE;
		}
		else if (cwsType == L"PK_15" && CGameVariable::GetInstance().GetAccShareInfo()->PkSetting.emPkSchedule == em_Pk_Schedule_PK)
			return TRUE; // PK进度来到15级的点 直接跳过!
		else if (cwsType == L"PK_16" && CGameVariable::GetInstance().GetAccShareInfo()->PkSetting.emPkSchedule == em_Pk_Schedule_TaskUpgrade)
		{
			//LOG_CF_D( L"成功执行到退出标志:%s, 退出所有!", cwsType.c_str());
			return FALSE;
		}
		else if (cwsType == L"PK_16" && CGameVariable::GetInstance().GetAccShareInfo()->PkSetting.emPkSchedule == em_Pk_Schedule_PK)
		{
			_bExitScript = TRUE;
			//LOG_CF_D( L"成功执行到退出标志:%s, 仅仅退出作用域!", cwsType.c_str());
			return FALSE;
		}
	}
	else if (cwsType == L"完成主线")
	{
		_bExitScript = TRUE;
		CConsoleLog(L"完成主线!");
		CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_FINISH_ALL_MAINTASK, TRUE);
	}
	else if (cwsType == L"退出游戏")
	{
		CBnsGameFun::GetInstance().SetCloseGame(L"脚本设置要退出游戏!", em_Close_Result_ReStart);
	}
	return TRUE;
}

RealBnsScriptFunction(Script_TakeCorpse_By_Dis)
{
	BnsScript_CheckParmCount(4);
	
	CFarm::GetInstance().TakeCorpse_By_PointList(TaskInfoParm(BnsScript_GetDWORDParm(0), BnsScript_GetDWORDParm(1)), CBaseObject::CreateFixPointInfo(BnsScript_GetFLOATParm(2)), _VecPoint, GetUnEmptyObjName(BnsScript_GetWCHARParm(3)));
	_VecPoint.clear();
	return TRUE;
}

RealBnsScriptFunction(Script_IsTaskDone)
{
	BnsScript_CheckParmCount(3);

	TaskInfo TaskInfo_;
	if (!CTask::GetInstance().ExistTask(BnsScript_GetDWORDParm(0), &TaskInfo_))
	{
		//LOG_CF_D( L"任务ID:%X 不存在!", BnsScript_GetDWORDParm(0));
		return TRUE;
	}

	if (TaskInfo_.GetScheduleId() != BnsScript_GetDWORDParm(1))
	{
		//LOG_CF_D( L"任务ID:%X 的进度Id:%X != %X, 不执行回调函数", BnsScript_GetDWORDParm(0), pTaskInfo->GetScheduleId(), BnsScript_GetDWORDParm(1));
		return TRUE;
	}

	BnsSleep(1000);
	ExuteExtendLoop(BnsScript_GetWCHARParm(2), *_pCurrentScriptCode);
	return TRUE;
}

RealBnsScriptFunction(Script_EatFood)
{
	BnsScript_CheckParmCount(1);
	
	DWORD dwValue = BnsScript_GetDexParm(0);
	// 食物 만두
	LOG_CF_D( L"当前人物血量=%d, 设置的是:%d", CPerson::GetInstance().GetPercentHp(), dwValue);

	while (CPerson::GetInstance().GetPercentHp() < dwValue && GameRun && !EMPTY_PERSONHP && CBagFunction::GetInstance().ExistItem_By_Name(包子, nullptr))
	{
		CFarm::GetInstance().ClearAround_TarIdSelf(CBaseObject::CreateFixPointInfo(16.0f));
		LOG_CF_D( L"已清除当前目标是自己的怪物");
		CFarm::GetInstance().EatFood(dwValue);
		BnsSleep(1000);
	}

	return TRUE;
}

RealBnsScriptFunction(Script_EatMedicine)
{
	BnsScript_CheckParmCount(1);

	DWORD dwValue = BnsScript_GetDexParm(0);
	if (CPerson::GetInstance().GetPercentHp() >= dwValue)
		return TRUE;

	SupplementHp(dwValue);
	return TRUE;
}

RealBnsScriptFunction(Script_SetMailItem)
{
	BnsScript_CheckParmCount(1);
	
	if (!MyTools::CLPublic::Vec_find_if(_VecMailItem, static_cast<std::wstring*>(nullptr), [&](CONST  std::wstring& itm){return itm == BnsScript_GetWCHARParm(0);}))
	{
		_VecMailItem.push_back(BnsScript_GetWCHARParm(0));
	}

	return TRUE;
}

RealBnsScriptFunction(Script_Mail)
{
	BnsScript_CheckParmCount(2);

	std::vector<CMailAction::MailItemContent> VecMailItem;
	for (auto& itm : _VecMailItem)
		VecMailItem.push_back(CMailAction::MailItemContent(itm));

	CMailAction::GetInstance().Mail(BnsScript_GetWCHARParm(0), VecMailItem, BnsScript_GetDexParm(1));
	return TRUE;
}

RealBnsScriptFunction(Script_TestMail)
{
	BnsScript_CheckParmCount(3);

	return TRUE;
}

unsigned WINAPI _PKThread(LPVOID lpParm)
{
	CPlayerKill Pk;
	Pk.StartPK();
	return 0;
}

RealBnsScriptFunction(Script_StartPk)
{
	BnsScript_CheckParmCount(0);

	// 关闭任务超时判断
	DWORD dwPointTimeout = CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CHECKPAUSEPOINT, NULL);
	DWORD dwValue = CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_TASKTIMEOUTID, NULL);

	LOG_CF_D(L"关闭坐标超时和检查任务变化!");

	//((CBnsScript*)lpCustomeFunParm)->bExistHelpThread = TRUE;
	LOG_CF_D( L"当前读取配置=%d", CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.IsPK);
	if (!CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.IsPK)
	{
		HANDLE hThread = cbBEGINTHREADEX(NULL, NULL, _PKThread, NULL, NULL, NULL);
		::WaitForSingleObject(hThread, INFINITE);
		::CloseHandle(hThread);
		hThread = NULL;
	}
	else
		LOG_CF_D( L"放弃PK");


	LOG_CF_D(L"PK完毕!还原坐标检查和任务超时!");
	// 还原任务超时判断
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_TASKTIMEOUTID, dwValue);
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CHECKPAUSEPOINT, dwPointTimeout);
	
	return TRUE;
}

RealBnsScriptFunction(Script_PickCorpse_No_Move)
{
	BnsScript_CheckParmCount(2);

	return CFarm::GetInstance().TakeCorpseItem(TaskInfoParm(0, 0), CBaseObject::CreateFixPointInfo(BnsScript_GetFLOATParm(1)), GetUnEmptyObjName(BnsScript_GetWCHARParm(0)));
}

RealBnsScriptFunction(Script_TestMessageBox)
{
#if BnsProject_DLL
	
#else
	BnsScript_CheckParmCount(1);

	if (GameRun && !_bExitScript)
		LOG_MSG_CF(L"%s", BnsScript_GetWCHARParm(0).c_str());
#endif
	return TRUE;
}

RealBnsScriptFunction(Script_JoinTeam)
{
	BnsScript_CheckParmCount(2);
#if BnsProject_DLL
	if (CTeamMemberExtend::GetInstance().GetTeamMemberCount() < BnsScript_GetDexParm(0))
	{
		DWORD dwOldValue = CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CHECKPAUSEPOINT, FALSE);
		CClientTeam::GetInstance().BnsScript_JoinTeam(BnsScript_GetDexParm(0), BnsScript_GetDexParm(1) == 0 ? FALSE : TRUE);
		CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CHECKPAUSEPOINT, dwOldValue);
	}
#else
	LOG_CF_D( L"注入测试下不能组队!");
#endif
	return TRUE;
}

RealBnsScriptFunction(Script_JoinDungoenTeam)
{
	BnsScript_CheckParmCount(3);
#if BnsProject_DLL

	std::vector<std::wstring> vParm;
	MyTools::CCharacter::Split(BnsScript_GetWCHARParm(2), L",", vParm, Split_Option_RemoveEmptyEntries | Split_Option_KeepOnly);
	if (vParm.size() == 2)
	{
		TaskInfoParm TaskParm;
		TaskParm.dwTaskId = wcstol(vParm.at(0).c_str(), nullptr, 16);
		TaskParm.dwScheduleId = wcstol(vParm.at(1).c_str(), nullptr, 16);

		if (CTeamMemberExtend::GetInstance().GetTeamMemberCount() < BnsScript_GetDexParm(0))
		{
			DWORD dwOldValue = CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CHECKPAUSEPOINT, FALSE);
			CClientTeam::GetInstance().BnsScript_JoinGungoenTeam(TaskParm, BnsScript_GetDexParm(0), BnsScript_GetDexParm(1) == 0 ? FALSE : TRUE);
			CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CHECKPAUSEPOINT, dwOldValue);
		}

		CClientTeam::GetInstance().SetDungeonSchedule(L"组队完毕!", 0);
	}
	else
	{
		LOG_MSG_CF(L"参数不对! 应该是[a,b]才对! 现在是:%s", BnsScript_GetWCHARParm(1).c_str());
		return FALSE;
	}

#else
	//CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->dwTeamSchedule = 0;
	InitTeamGatherShareMemory();
	if (CPerson::GetInstance().GetId() == _pTeamGatherContent->ArrMember[0].dwPlayerId)
	{
		LOG_C_D(L"我是队长!");
		// 队长
		std::map<DWORD, ULONGLONG> MapDisableInvite;
		while (GameRun && !EMPTY_PERSONHP && CTeamMemberExtend::GetInstance().GetTeamMemberCount() != BnsScript_GetDexParm(0))
		{
			for (UINT i = 1; i < _pTeamGatherContent->uCount; ++i)
			{
				auto itr = MapDisableInvite.find(_pTeamGatherContent->ArrMember[i].dwPlayerId);
				if (itr != MapDisableInvite.end())
				{
					if (::GetTickCount64() - itr->second >= 15 * 1000)
						MapDisableInvite.erase(itr);
					else
						continue;
				}
				if (CTeamMemberExtend::GetInstance().FindTeamMember_By_Condiction(nullptr, [this, i](CONST CTeamMember& TeamMember){ return TeamMember.GetId() == _pTeamGatherContent->ArrMember[i].dwPlayerId; }))
				{
					continue;
				}

				LOG_C_D(L"邀请队员加入队伍:%X", _pTeamGatherContent->ArrMember[i].dwPlayerId);
				CPlayer::InviteTeamMember(_pTeamGatherContent->ArrMember[i].dwPlayerId);
				MapDisableInvite.insert(std::make_pair(_pTeamGatherContent->ArrMember[i].dwPlayerId, ::GetTickCount64()));
				BnsSleep(500);
			}
			BnsSleep(1000);
		}
		
	}
	else
	{
		LOG_C_D(L"我队员!");
		// 队员
		while (GameRun && !EMPTY_PERSONHP && CTeamMemberExtend::GetInstance().GetTeamMemberCount() != BnsScript_GetDexParm(0))
		{
			if (CTeamMemberExtend::GetInstance().GetTeamMemberCount() != 0)
			{
				BnsSleep(500);
				continue;
			}

			// 是否有人邀请!
			CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"InvitedPartyConfirmPanel", [this](CONST CGameUi& InvitedPartyConfirmPanel)
			{
				if (ReadDWORD(InvitedPartyConfirmPanel.GetObj() + 队伍邀请队长ID偏移) != _pTeamGatherContent->ArrMember[0].dwPlayerId)
				{
					CSomeFun::GetInstance().SimlationKey('N', 200, 800);
					return;
				}

				LOG_CF_D( L"接受队长的邀请!");
				InvitedPartyConfirmPanel.AcceptTeamInvite();
				BnsSleep(2000);
			});
			BnsSleep(1000);
		}
	}

	for (UINT i = 0; i < _pTeamGatherContent->uCount; ++i)
	{
		_pTeamGatherContent->ArrMember[i].uScheduleIndex = 0;
		MyTools::CCharacter::wstrcpy_my(_pTeamGatherContent->ArrMember[i].wszScheduleName, L"组队完毕!");
	}
#endif
	return TRUE;
}

RealBnsScriptFunction(Script_LeaveTeam)
{
	BnsScript_CheckParmCount(0);
#if BnsProject_DLL
	CClientTeam::GetInstance().BnsScript_LeaveTeam();
#else
	LOG_CF_D( L"注入测试下不能离队!");
#endif
	return TRUE;
}

RealBnsScriptFunction(Script_LeaveDungoenTeam)
{
	BnsScript_CheckParmCount(0);
#if BnsProject_DLL
	CClientTeam::GetInstance().BnsScript_LeaveGungoenTeam();
#else
	LOG_CF_D( L"注入测试下不能离队!");
#endif
	return TRUE;
}

RealBnsScriptFunction(Script_CloseNpcTalkDlg)
{
	BnsScript_CheckParmCount(0);
	
	if (CSomeFun::GetInstance().TimeOut_Condiction_GameRun(30 * 1000, []{
		BnsSleep(1000);
		return !CObjectFunction::GetInstance().IsShow_In_GameUi(L"TalkControlPanel");
	}))
	{
		LOG_CF_D(L"等待Npc对话框出来30s超时!, 放弃!");
		return TRUE;
	}


	while (GameRun && !EMPTY_PERSONHP && CObjectFunction::GetInstance().IsShow_In_GameUi(L"TalkControlPanel"))
	{
		LOG_CF_D(L"关闭Npc聊天框!");
		MyTools::CLPublic::SimulationKey(VK_ESCAPE, CGameVariable::GetInstance().GetAccShareInfo()->hGameWnd);
		BnsSleep(2000);
	}
	//
	return TRUE;
}

RealBnsScriptFunction(Script_SetMailFialdText)
{
	// 判断是否还有物品在身上
	for (auto& itm : _VecMailItem)
	{
		if (CBagFunction::GetInstance().ExistItem_By_Name(itm, nullptr))
		{
			LOG_CF_E(L"人物是:%s 第%d个角色 邮寄失败!", CPerson::GetInstance().GetName().c_str(), CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->dwOrderIndex + 1);
			StopGame;
			return TRUE;
		}
	}

	return TRUE;
}

RealBnsScriptFunction(Script_TakeCorpse_By_Status)
{
	BnsScript_CheckParmCount(2);

	return CFarm::GetInstance().TakeCorpse_By_Status(CBaseObject::CreateFixPointInfo(BnsScript_GetFLOATParm(1)), GetUnEmptyObjName(BnsScript_GetWCHARParm(0)));
}

RealBnsScriptFunction(Script_RecvMail)
{
	BnsScript_CheckParmCount(1);
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CHECKPAUSEPOINT, FALSE);

	while (GameRun && !EMPTY_PERSONHP)
	{
		if (CMailAction::GetInstance().GetMailCount() == 0)
		{
			BnsSleep(1000);
			continue;
		}

		CNpc Npc;
		CONST std::wstring wsNpcName = BnsScript_GetWCHARParm(0);
		if (!CNpcExtend::GetInstance().FindNpc_By_Name(wsNpcName, CBaseObject::CreateFixPointInfo(30), &Npc))
		{
			LOG_MSG_CF(L"30m范围之内不存在仓库Npc:%s", wsNpcName.c_str());
			return TRUE;
		}
		if (Npc.GetDis() > 3)
		{
			BnsMove(Npc.GetPoint(), 2);
			BnsSleep(2000);
		}

		CMailAction::GetInstance().RecvMail();
		BnsSleep(1000);
		CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"ChatInputPanel", [](CONST CGameUi&){CSomeFun::GetInstance().SimlationKey(VK_RETURN, 200, 1000); });
	}

	return TRUE;
}

RealBnsScriptFunction(Script_TaskScheduleExist)
{
	BnsScript_CheckParmCount(3);
	DWORD dwTaskId = BnsScript_GetDWORDParm(0);
	DWORD dwScheduleId = BnsScript_GetDWORDParm(1);

	TaskInfo Ti;
	if (!CTask::GetInstance().ExistTask(dwTaskId, &Ti) || Ti.GetScheduleId() != dwScheduleId)
		return TRUE;

	BnsSleep(1000);
	ExuteExtendLoop(BnsScript_GetWCHARParm(2), *_pCurrentScriptCode);
	return TRUE;
}

RealBnsScriptFunction(Script_Task_TaskBall)
{
	BnsScript_CheckParmCount(4);

	_VecPoint.push_back(CPerson::GetInstance().GetPoint());
	CFarm::GetInstance().TakeBall_By_PointList(BnsScript_GetDWORDParm(2), TaskInfoParm(BnsScript_GetDWORDParm(0), BnsScript_GetDWORDParm(1)), CBaseObject::CreateFixPointInfo(BnsScript_GetFLOATParm(3)), _VecPoint);
	_VecPoint.clear();
	return TRUE;
}

RealBnsScriptFunction(Script_Pickup)
{
	BnsScript_CheckParmCount(2);

	CONST std::wstring wsType = BnsScript_GetWCHARParm(0);
	if (wsType == L"Name")
		CFarm::GetInstance().Pickup_By_Name(GetUnEmptyObjName(BnsScript_GetWCHARParm(1)));
	else if (wsType == L"龙脉")
		CFarm::GetInstance().Pickup_By_LongMai(BnsScript_GetWCHARParm(1));
	else if (wsType == L"F")
		CFarm::GetInstance().Pickup_By_Flag(CBaseObject::CreateFixPointInfo(5));
	else if (wsType == L"RepeatName")
		CFarm::GetInstance().Pickup_By_RepeatName(BnsScript_GetWCHARParm(1));

	return TRUE;
}

RealBnsScriptFunction(Script_Telport)
{
	BnsScript_CheckParmCount(1);

	CGameFunction::GetInstance().Telport(BnsScript_GetWCHARParm(0));
	return TRUE;
}

RealBnsScriptFunction(Script_LineTelport)
{
	BnsScript_CheckParmCount(2);

	Point TarPoint = CBnsScript::GetPoint_By_ScriptParm(BnsScript_GetWCHARParm(0));
	DWORD dwMaxCount = BnsScript_GetDexParm(1);
	CGameFunction::GetInstance().LineTelport(CBaseObject::CreateFixPointInfo(10), TarPoint, dwMaxCount == 0 ? 0xFFFFFFFF : dwMaxCount);
	return TRUE;
}

RealBnsScriptFunction(Script_HeightTelport)
{
	BnsScript_CheckParmCount(2);

	Point TarPoint = CBnsScript::GetPoint_By_ScriptParm(BnsScript_GetWCHARParm(0));
	DWORD dwMaxCount = BnsScript_GetDexParm(1);

	if (TarPoint.Z > CPerson::GetInstance().GetPoint().Z)
	{
		LOG_CF_D( L"飞天瞬移");
		CGameFunction::GetInstance().HeightTelport(CBaseObject::CreateFixPointInfo(10), TarPoint, dwMaxCount == 0 ? 0xFFFFFFFF : dwMaxCount);
	}
	else
	{
		LOG_CF_D( L"下降瞬移");
		CGameFunction::GetInstance().FloatTelport(CBaseObject::CreateFixPointInfo(10), TarPoint, dwMaxCount == 0 ? 0xFFFFFFFF : dwMaxCount);
	}
	
	return TRUE;
}

FixPointInfo CBnsScript::GetFixPointInfo_By_ScriptParm(CONST std::wstring& cwstr)
{
	std::vector<std::wstring> vlst;
	FixPointInfo FixPtInfo;

	if (MyTools::CCharacter::Split(cwstr, L",", vlst, Split_Option_RemoveEmptyEntries | Split_Option_KeepOnly) == 4)
	{
		FixPtInfo.FixPoint.X = static_cast<float>(_wtof(vlst.at(0).c_str()));
		FixPtInfo.FixPoint.Y = static_cast<float>(_wtof(vlst.at(1).c_str()));
		FixPtInfo.FixPoint.Z = static_cast<float>(_wtof(vlst.at(2).c_str()));
		FixPtInfo.fDis = static_cast<float>(_wtof(vlst.at(3).c_str()));
	}
	else
		LOG_MSG_CF(L"脚本错误! 当前参数是:%s, 无法转换成FixPointInfo", cwstr.c_str());

	return FixPtInfo;
}

Point CBnsScript::GetPoint_By_ScriptParm(CONST std::wstring& wsParm)
{
	std::vector<std::wstring> vlst;
	Point Point_;

	if (MyTools::CCharacter::Split(wsParm, L",", vlst, Split_Option_RemoveEmptyEntries | Split_Option_KeepOnly) == 3)
	{
		Point_.X = static_cast<float>(_wtof(vlst.at(0).c_str()));
		Point_.Y = static_cast<float>(_wtof(vlst.at(1).c_str()));
		Point_.Z = static_cast<float>(_wtof(vlst.at(2).c_str()));
	}
	else
		LOG_MSG_CF(L"脚本错误! 当前参数是:%s, 无法转换成Point", wsParm.c_str());

	return Point_;
}

TaskInfoParm CBnsScript::GetTaskInfoParm_By_ScriptParm(CONST std::wstring& wstr)
{
	TaskInfoParm TaskInfoParm_;
	std::vector<std::wstring> vlst;

	if(MyTools::CCharacter::Split(wstr, L",", vlst, Split_Option_RemoveEmptyEntries | Split_Option_KeepOnly) == 2)
	{
		TaskInfoParm_.dwTaskId = static_cast<DWORD>(wcstol(vlst.at(0).c_str(), nullptr, 16));
		TaskInfoParm_.dwScheduleId = static_cast<DWORD>(wcstol(vlst.at(1).c_str(), nullptr, 16));
	}
	else
		LOG_MSG_CF(L"无法分解任务ID! %s", wstr.c_str());

	return TaskInfoParm_;
}

TaskContentMsg CBnsScript::GetTaskContentMsg_By_ScriptParm(CONST std::wstring& wstr)
{
	TaskContentMsg TaskContentMsg_;

	std::vector<std::wstring> vlst;

	if (MyTools::CCharacter::Split(wstr, L",", vlst, Split_Option_RemoveEmptyEntries | Split_Option_KeepOnly) == 3)
	{
		TaskContentMsg_.Ti.dwTaskId = static_cast<DWORD>(wcstol(vlst.at(0).c_str(), nullptr, 16));
		TaskContentMsg_.Ti.dwScheduleId = static_cast<DWORD>(wcstol(vlst.at(1).c_str(), nullptr, 16));
		TaskContentMsg_.dwKillCount = static_cast<DWORD>(_wtoi(vlst.at(2).c_str()));
	}
	else
		LOG_MSG_CF(L"无法分解任务ID! %s", wstr.c_str());

	return TaskContentMsg_;
}

#ifdef BnsProject_Dlg
VOID CBnsScript::InitTeamGatherShareMemory()
{
	if (_pTeamGatherContent != nullptr)
		return;

	HANDLE hFileMap = ::OpenFileMappingW(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, SZFILE_NAME_TEAM_SHAREINFO);
	if (hFileMap == NULL)
	{
		hFileMap = ::CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, NULL, sizeof(Login_Share_Info), SZFILE_NAME_TEAM_SHAREINFO);
		_pTeamGatherContent = (TeamGatherContent *)MapViewOfFile(hFileMap, FILE_MAP_READ | FILE_SHARE_WRITE, NULL, NULL, sizeof(TeamGatherContent));
		ZeroMemory(_pTeamGatherContent, sizeof(TeamGatherContent));
	}
	else
	{
		_pTeamGatherContent = (TeamGatherContent *)MapViewOfFile(hFileMap, FILE_MAP_READ | FILE_SHARE_WRITE, NULL, NULL, sizeof(TeamGatherContent));
	}
	
	// 判断是否存在该角色了
	for (UINT i = 0; i < _pTeamGatherContent->uCount; ++i)
	{
		if (std::wstring(_pTeamGatherContent->ArrMember[i].wszPlayerName) == CPerson::GetInstance().GetName())
		{
			_pTeamMemberGatherContent = &_pTeamGatherContent->ArrMember[i];
			_pTeamMemberGatherContent->dwPlayerId = CPerson::GetInstance().GetId();
			return; 
		}
	}

	_pTeamMemberGatherContent = &_pTeamGatherContent->ArrMember[_pTeamGatherContent->uCount];
	ZeroMemory(_pTeamMemberGatherContent, sizeof(TeamMemberGatherContent));
	_pTeamMemberGatherContent->uIndex = _pTeamGatherContent->uCount;
	_pTeamGatherContent->uCount += 1;

	_pTeamMemberGatherContent->dwPlayerId = CPerson::GetInstance().GetId();
	MyTools::CCharacter::wstrcpy_my(_pTeamMemberGatherContent->wszPlayerName, CPerson::GetInstance().GetName().c_str());
	MyTools::CCharacter::wstrcpy_my(_pTeamMemberGatherContent->wszAccountName, CGameVariable::GetInstance().GetAccShareInfo()->GetAccName());
}
#endif

RealBnsScriptFunction(Script_ClearAroundMonster)
{
	BnsScript_CheckParmCount(3);
	CONST std::wstring wsType = BnsScript_GetWCHARParm(0);
	FixPointInfo FixPtInfo = CBaseObject::CreateFixPointInfo(BnsScript_GetFLOATParm(2));

	if (wsType == L"Task")
	{
		TaskInfoParm TaskInfoParm_ = GetTaskInfoParm_By_ScriptParm(BnsScript_GetWCHARParm(1));
		LOG_CF_D( L"TaskId=%X, ScheduleId=%X, 完成状态=%d", TaskInfoParm_.dwTaskId, TaskInfoParm_.dwScheduleId, CTask::GetInstance().IsCompletionTask(TaskInfoParm_));

		CFarm::GetInstance().ClearAroundMonster_By_Condiction(FixPtInfo, [&TaskInfoParm_]{return CTask::GetInstance().IsCompletionTask(TaskInfoParm_); });
	}
	else if (wsType == L"Around")
	{
		CFarm::GetInstance().ClearAroundMonster_By_Condiction(FixPtInfo, [&FixPtInfo]
		{
			LOG_CF_D( L"MonsterCount=%d, NpcCount=%d", CMonsterExtend::GetInstance().GetAroundMonsterCount(FixPtInfo), CNpcExtend::GetInstance().GetAroundNpcCount(FixPtInfo));
			return CMonsterExtend::GetInstance().GetAroundMonsterCount(FixPtInfo) == NULL && CNpcExtend::GetInstance().GetAroundNpcCount(FixPtInfo) == NULL;
		});
	}
	else
		LOG_MSG_CF(L"错误的参数:%s", wsType.c_str());

	return TRUE;
}

RealBnsScriptFunction(Script_AttractMonster)
{
	BnsScript_CheckParmCount(0);

	CSkill::UseSkill_By_Id(Lux_SkillId_Z_Attract);
	BnsSleep(500);
	CSkill::UseSkill_By_Id(Lux_SkillId_3);
	BnsSleep(500);
	CSkill::UseSkill_By_Id(Lux_SkillId_Tab);
	BnsSleep(1000);
	CSkill::UseSkill_By_Id(Lux_SkillId_1);
	BnsSleep(500);
	CSkill::UseSkill_By_Id(Lux_SkillId_3);
	BnsSleep(500);

	return TRUE;
}

RealBnsScriptFunction(Script_SetAutoTakItemMode)
{
	BnsScript_CheckParmCount(1);

	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_AUTOTAKEITEM, BnsScript_GetDexParm(0));
	return TRUE;
}

RealBnsScriptFunction(Script_SetKaDaoMode)
{
	BnsScript_CheckParmCount(1);

	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_KADAOMODE, BnsScript_GetDexParm(0));
	return TRUE;
}

RealBnsScriptFunction(Script_ChooseColorTask)
{
	BnsScript_CheckParmCount(1);

	TaskInfoParm TaskInfoParm_ = GetTaskInfoParm_By_ScriptParm(BnsScript_GetWCHARParm(0));

	return CFarmSpeialTask::GetInstance().ChooseColorTask(FixPointInfo(Point(20052, -7522, 251), 30.0f), [&TaskInfoParm_]{
		return CTask::GetInstance().IsCompletionTask(TaskInfoParm_);
	});
}

RealBnsScriptFunction(Script_IceFireShieldTask)
{
	BnsScript_CheckParmCount(1);

	TaskInfoParm TaskInfoParm_ = GetTaskInfoParm_By_ScriptParm(BnsScript_GetWCHARParm(0));
	CFarmSpeialTask::GetInstance().SpecialTask_Ice_Fire_Shield(CBaseObject::CreateFixPointInfo(30), [&TaskInfoParm_]{
		return CTask::GetInstance().IsCompletionTask(TaskInfoParm_);
	});
	return TRUE;
}

RealBnsScriptFunction(Script_KillBoss)
{
	//BnsScript_CheckParmCount(1);
	//_CLStackTrace(L"CBnsScript::Script_KillBoss");
	FixPointInfo RangeDis;
	if (_Script_Current_ExcuteMethod.pScript_Code_Method->VecParm.size() == 1)
		RangeDis = CBaseObject::CreateFixPointInfo(30.0f);
	else if (_Script_Current_ExcuteMethod.pScript_Code_Method->VecParm.size() == 2)
		RangeDis = CBaseObject::CreateFixPointInfo(BnsScript_GetFLOATParm(1));

	CFarmBoss::GetInstance().KillBoss_By_Name(BnsScript_GetWCHARParm(0), RangeDis);
	return TRUE;
}

RealBnsScriptFunction(Script_FarmAround)
{
	// 杀怪(任务杀怪数量,[13E,2,10],10)
	BnsScript_CheckParmCount(3);

	_VecPoint.push_back(CPerson::GetInstance().GetPoint());

	CONST std::wstring wsType = BnsScript_GetWCHARParm(0);
	if (wsType == L"任务杀怪数量")
	{
		TaskInfoParm TaskParm;
		std::vector<std::wstring> vParm;
		if(MyTools::CCharacter::Split(BnsScript_GetWCHARParm(1), L",", vParm, Split_Option_RemoveEmptyEntries | Split_Option_KeepOnly) != 3)
		{
			LOG_MSG_CF(L"第二个参数数量应该是3个!");
			return TRUE;
		}

		TaskParm.dwTaskId = static_cast<DWORD>(wcstol(vParm.at(0).c_str(), nullptr, 16));
		TaskParm.dwScheduleId = static_cast<DWORD>(wcstol(vParm.at(1).c_str(), nullptr, 16));
		DWORD dwCount = static_cast<DWORD>(_wtoi(vParm.at(2).c_str()));

		CFarm::GetInstance().FarmAround_By_PointList(CBaseObject::CreateFixPointInfo(BnsScript_GetFLOATParm(2)), _VecPoint, [&TaskParm, &dwCount]{
			return CTask::GetInstance().IsCompletionTask(TaskParm) ? TRUE : (CTask::GetInstance().QueryTaskScheduleCount(TaskParm) >= dwCount ? TRUE : FALSE);
		});
	}
	else
	{
		LOG_MSG_CF(L"杀怪里面有这个类别嘛? 我怎么不知道?");
	}
	_VecPoint.clear();
	return TRUE;
}

RealBnsScriptFunction(Script_SetVariable)
{
	BnsScript_CheckParmCount(2);
	CONST std::wstring& wsVarName = BnsScript_GetWCHARParm(0);
	DWORD dwValue = BnsScript_GetDexParm(1);
	if (wsVarName == L"任务超时ID")
		dwValue = BnsScript_GetDWORDParm(1);

	LOG_CF_D( L"设置变量:%s -> Dec:%d, Hex:%X", wsVarName.c_str(), dwValue, dwValue);
	CGameVariable::GetInstance().SetVariable_By_Name(wsVarName, dwValue);
	return TRUE;
}

RealBnsScriptFunction(Script_UpateSkill)
{
	BnsScript_CheckParmCount(2);
	// 技能ID, Index就是对应面板上每个技能的第0,1,2个技能
	_VecLearnSkillId.push_back({ BnsScript_GetDWORDParm(0), BnsScript_GetDWORDParm(1) });
	return TRUE;
}

RealBnsScriptFunction(Script_DoAction)
{
	BnsScript_CheckParmCount(1);

	return CPersonFunction::GetInstance().DoAction(BnsScript_GetDWORDParm(0));
}

RealBnsScriptFunction(Script_StartLearnSkill)
{
	BnsScript_CheckParmCount(1);
	if (_VecLearnSkillId.size() == 0)
	{
		LOG_MSG_CF(L"你都没加点, 怎么学?");
		return FALSE;
	}

	struct SkillTypeText
	{
		DWORD dwSkillType;
		std::wstring wsText;
	};

	CONST static std::vector<SkillTypeText> VecSkillTypeText = 
	{
		{ 0xB, L"召唤清风系" }, { 0x15, L"召唤大地系" },
		{ 0xB, L"咒术寒冰系" }, { 0x15, L"咒术暗黑系" },
		{ 0xB, L"剑士火焰系" }, { 0x15, L"剑士雷电系" },
		{ 0xB, L"气功火系" }, { 0x15, L"气功冰系" },
		{ 0xB, L"刺客暗系" }, { 0x15, L"刺客雷系" },
		{ 0xB, L"灵剑士风系" }, { 0x15, L"灵剑士雷系" },
		{ 0xB, L"拳师风系" }, { 0x15, L"拳师火系" },
		{ 0xB, L"枪手火系" }, { 0x15, L"枪手暗系" },
	};

	auto wsSkillTypeText = BnsScript_GetWCHARParm(0);
	auto Ntype = 0;
	auto p = MyTools::CLPublic::Vec_find_if_Const(VecSkillTypeText, [wsSkillTypeText](CONST SkillTypeText& SkillTypeText_){ return SkillTypeText_.wsText == wsSkillTypeText; });
	if (p == nullptr)
	{
		LOG_MSG_CF(L"哪来这个系:%s", wsSkillTypeText.c_str());
		return FALSE;
	}

	CGameFunction::GetInstance().SwitchSkillPage(p->wsText);
	for (auto& itm : _VecLearnSkillId)
		itm.dwIndex += p->dwSkillType;

	if (p->wsText == L"召唤清风系" ||
		p->wsText == L"咒术寒冰系" ||
		p->wsText == L"剑士火焰系" ||
		p->wsText == L"气功火系" ||
		p->wsText == L"刺客暗系" ||
		p->wsText == L"灵剑士风系" ||
		p->wsText == L"拳师风系" ||
		p->wsText == L"枪手火系")
	{
		Ntype = 0;
	}
	else
	{
		Ntype = 1;
	}

	CGameFunction::GetInstance().StudySkill(_VecLearnSkillId,Ntype);
	CConsoleLog(L"学习技能完毕!");
	_VecLearnSkillId.clear();
	return TRUE;
}

RealBnsScriptFunction(Script_ForgotSkill)
{
	BnsScript_CheckParmCount(0);
	LOG_MSG_CF(L"这个函数不能用了……");
	//CSkill::ForgotSkill();
	return TRUE;
}

RealBnsScriptFunction(Script_SaveWareHouse)
{
	BnsScript_CheckParmCount(1);

	CBagAction::GetInstance().SaveItem(CObjectFunction::em_Bag_Type_Warehouse, BnsScript_GetWCHARParm(0));
	return TRUE;
}

RealBnsScriptFunction(Script_Farm4Boss)
{
	BnsScript_CheckParmCount(1);

	TaskInfoParm TaskInfoParm_ = GetTaskInfoParm_By_ScriptParm(BnsScript_GetWCHARParm(0));
	LOG_CF_D( L"TaskId=%X, ScheduleId=%X, 完成状态=%d", TaskInfoParm_.dwTaskId, \
		TaskInfoParm_.dwScheduleId, CTask::GetInstance().IsCompletionTask(TaskInfoParm_));

	CFarmSpeialTask::GetInstance().SpecialTask_3A9(CBaseObject::CreateFixPointInfo(14.0f), [&TaskInfoParm_]{
		return CTask::GetInstance().IsCompletionTask(TaskInfoParm_);
	});

	return TRUE;
}

RealBnsScriptFunction(Script_SetWareHouseItem)
{
	BnsScript_CheckParmCount(2);

	CPlayerTransaction::MainTask_Transaction_Item MainTask_Transaction_Item_(BnsScript_GetWCHARParm(0), BnsScript_GetDexParm(1));

	if (!MyTools::CLPublic::Vec_find_if(_VecTransactionItem, static_cast<CPlayerTransaction::MainTask_Transaction_Item*>(nullptr), [&MainTask_Transaction_Item_](CONST CPlayerTransaction::MainTask_Transaction_Item& MainTask_Transaction_Item__)
	{
		return MainTask_Transaction_Item__.wsItemName == MainTask_Transaction_Item_.wsItemName;
	}))
	{
		_VecTransactionItem.push_back(MainTask_Transaction_Item_);
	}
	return TRUE;
}

RealBnsScriptFunction(Script_WareHouseDeal)
{
	BnsScript_CheckParmCount(3);

	 std::wstring wsDealType = BnsScript_GetWCHARParm(0);

#if BnsProject_DLL

	auto fnPtr = [](CPlayerTransaction::em_Warehouse_Action emAction)
	{
		std::thread ExcuteScriptThread([emAction]
		{
			auto pWarehouseConfig = CGameVariable::GetInstance().GetWarehouseConfig();
			if (pWarehouseConfig == nullptr)
				return;

			auto pBnsScript = std::make_shared<CBnsScript>();

			CTransferScript TransferScript;
			std::wstring wsScriptContent = TransferScript.DownLoadScript(std::wstring(pWarehouseConfig->wszCityName));
			if (!pBnsScript->Read(em_ReadScript_Type_Text, wsScriptContent))
			{
				LOG_MSG_CF(L"脚本解析失败:%s.inf", pWarehouseConfig->wszCityName);
				return;
			}

			std::wstring wsFunName = L"fn";
			switch (emAction)
			{
			case CPlayerTransaction::em_Warehouse_Action_Medicine:
				wsFunName += L"买红";
				break;
			case CPlayerTransaction::em_Warehouse_Action_Eat:
				wsFunName += L"买包子";
				break;
			case CPlayerTransaction::em_Warehouse_Action_Hammer:
				wsFunName += L"买修理锤";
				break;
			case CPlayerTransaction::em_Warehouse_Action_Save:
				wsFunName += L"仓库存仓";
				break;
			case CPlayerTransaction::em_Warehouse_Action_City:
				wsFunName += L"城市传送";
				break;
			default:
				break;
			}
			pBnsScript->Excute(wsFunName);
		});
		ExcuteScriptThread.join();
	};

	DWORD dwLastMoney = BnsScript_GetDexParm(2);
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CHECKPAUSEPOINT, NULL);
	CPlayerTransaction::GetInstance().SetVecWarehouseTradeItemName(_VecWarehouseTradeItem);
	CPlayerTransaction::GetInstance().Transaction_By_Task_To_WarehouseAccount(wsDealType == L"交易" ? CPlayerTransaction::em_WarehouseType_Passive : CPlayerTransaction::em_WarehouseType_Active, _VecTransactionItem, dwLastMoney, fnPtr);
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CHECKPAUSEPOINT, 3 * 60);
	_VecTransactionItem.clear();
	_VecWarehouseTradeItem.clear();
#endif
	return TRUE;
}

RealBnsScriptFunction(Script_TakeWareHouse)
{
	BnsScript_CheckParmCount(0);

#if BnsProject_DLL
	auto fnPtr = [](CONST std::wstring& wsCityName)
	{
		std::thread ExcuteScriptThread([wsCityName]
		{
			auto pBnsScript = std::make_shared<CBnsScript>();

			CTransferScript TransferScript;
			std::wstring wsScriptContent = TransferScript.DownLoadScript(wsCityName);
			if (!pBnsScript->Read(em_ReadScript_Type_Text, wsScriptContent))
			{
				LOG_MSG_CF(L"脚本解析失败:%s.inf", wsCityName.c_str());
				return;
			}

			pBnsScript->Excute(L"fn城市传送");
		});
		ExcuteScriptThread.join();
	};

	if (CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_FORCESTOPPOINT) == TRUE)
	{
		CPlayerTransaction::GetInstance().Transaction_By_Task_ToMainTaskAccount(fnPtr);
	}
	else
	{
		DWORD dwOldTime = CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CHECKPAUSEPOINT, NULL);
		CPlayerTransaction::GetInstance().Transaction_By_Task_ToMainTaskAccount(fnPtr);
		CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CHECKPAUSEPOINT, dwOldTime);
	}

#endif
	return TRUE;
}

RealBnsScriptFunction(Script_Trance)
{
	BnsScript_CheckParmCount(0);

	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CHECKPAUSEPOINT, NULL);
	return BnsSleep(60 * 60 * 1000);
}

RealBnsScriptFunction(Script_GiveWareHouse)
{
	BnsScript_CheckParmCount(2);
	
	 std::wstring wsNpcName = BnsScript_GetWCHARParm(0);
	
	std::vector<std::wstring> vItem;
	for (CONST auto& itm : _VecTransactionItem)
		vItem.push_back(itm.wsItemName);

#if BnsProject_DLL
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CHECKPAUSEPOINT, NULL);
	// 先去仓库一趟!
	CBagAction::GetInstance().PullItem_By_WareHouse(wsNpcName, vItem);


	auto fnPtr = [](CONST std::wstring& wsCityName)
	{
		std::thread ExcuteScriptThread([wsCityName]
		{
			auto pBnsScript = std::make_shared<CBnsScript>();

			CTransferScript TransferScript;
			std::wstring wsScriptContent = TransferScript.DownLoadScript(wsCityName);
			if (!pBnsScript->Read(em_ReadScript_Type_Text, wsScriptContent))
			{
				LOG_MSG_CF(L"脚本解析失败:%s.inf", wsCityName.c_str());
				return;
			}

			pBnsScript->Excute(L"fn城市传送");
		});
		ExcuteScriptThread.join();
	};

	// 再交易!
	DWORD dwLastMoney = BnsScript_GetDexParm(1);
	CPlayerTransaction::GetInstance().Transaction_By_Finish(vItem, dwLastMoney, fnPtr);

	_VecTransactionItem.clear();
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CHECKPAUSEPOINT, 3 * 60);
#endif
	return TRUE;
}

RealBnsScriptFunction(Script_KillDog_By_Kffm)
{
	BnsScript_CheckParmCount(0);
	
	CONST static std::vector<Point> DogPointVec = 
	{
		Point(-44400, 31132, -1084),
		Point(-44424, 31564, -1084), Point(-44392, 31844, -1084), Point(-44504, 33108, -1084), 
		Point(-44476, 33820, -1084), Point(-44464, 35048, -1084), Point(-43924, 35100, -1084),
	};

	// 先跑到第一个坐标
	LOG_CF_D( L"跑到第一个坐标!");
	BnsMove(DogPointVec.at(1), 1);
	// 等待周围36m之内只有1只狗的时候
	LOG_CF_D( L"等待36m之内,只有一只狗!");
	CSomeFun::GetInstance().TimeOut_Condiction_GameRun(60 * 1000, []{
		return CMonsterExtend::GetInstance().GetAroundMonsterCount(CBaseObject::CreateFixPointInfo(36.0f)) != 1;
	});

	// 第一只狗
	LOG_CF_D( L"冲到第二个坐标");
	BnsMove(DogPointVec.at(2), 2);
	LOG_CF_D( L"开始动手!");
	CMonster Dog;
	if (CMonsterExtend::GetInstance().GetTopMonster_By_Dis(CBaseObject::CreateFixPointInfo(20), &Dog))
	{
		LOG_CF_D( L"开始杀第一个狗!");
		CFarm::GetInstance().KillMonster_By_Dog(Dog, CBaseObject::CreateFixPointInfo(20.0f));
	}
	LOG_CF_D( L"退回到第一个坐标,等待18s技能恢复!");
	BnsMove(DogPointVec.at(0), 1);
	CSomeFun::GetInstance().TimeOut_Condiction_GameRun(18 * 1000, []{ return !EMPTY_PERSONHP; });
	BnsMove(DogPointVec.at(1), 1);

	// 第二只狗
	LOG_CF_D( L"等待13m范围内有狗就干!");
	CSomeFun::GetInstance().TimeOut_Condiction_GameRun(60 * 1000, []{
		return CMonsterExtend::GetInstance().GetAroundMonsterCount(CBaseObject::CreateFixPointInfo(15.0f)) != 1;
	});

	LOG_CF_D( L"开始动手!");
	if (CMonsterExtend::GetInstance().GetTopMonster_By_Dis(CBaseObject::CreateFixPointInfo(20), &Dog))
	{
		LOG_CF_D( L"开始杀第二个狗!");
		CFarm::GetInstance().KillMonster_By_Dog(Dog, CBaseObject::CreateFixPointInfo(20.0f));
	}

	// 第三只狗
	LOG_CF_D( L"退回到第一个坐标,等待18s技能恢复!");
	BnsMove(DogPointVec.at(0), 1);
	CSomeFun::GetInstance().TimeOut_Condiction_GameRun(18 * 1000, []{ return !EMPTY_PERSONHP; });
	BnsMove(DogPointVec.at(1), 1);

	LOG_CF_D( L"去第二个坐标");
	BnsMove(DogPointVec.at(2), 1);

	LOG_CF_D( L"等待13m范围内有狗就干!");
	CSomeFun::GetInstance().TimeOut_Condiction_GameRun(60 * 1000, []{
		return CMonsterExtend::GetInstance().GetAroundMonsterCount(CBaseObject::CreateFixPointInfo(14.0f)) != 1;
	});

	LOG_CF_D( L"开始动手!");
	if (CMonsterExtend::GetInstance().GetTopMonster_By_Dis(CBaseObject::CreateFixPointInfo(20), &Dog))
	{
		LOG_CF_D( L"开始杀第三个狗!");
		CFarm::GetInstance().KillMonster_By_Dog(Dog, CBaseObject::CreateFixPointInfo(20.0f));
	}

	// 第四只狗
	LOG_CF_D( L"等待18s技能恢复!");
	CSomeFun::GetInstance().TimeOut_Condiction_GameRun(18 * 1000, []{ return !EMPTY_PERSONHP; });
	LOG_CF_D( L"出发去第四只狗!");
	BnsMove(DogPointVec.at(3), 1);
	LOG_CF_D( L"开始动手!");
	if (CMonsterExtend::GetInstance().GetTopMonster_By_Dis(CBaseObject::CreateFixPointInfo(20), &Dog))
	{
		LOG_CF_D( L"开始杀第四个狗!");
		CFarm::GetInstance().KillMonster_By_Dog(Dog, CBaseObject::CreateFixPointInfo(20.0f));
	}

	BnsMove(DogPointVec.at(4), 1);
	BnsMove(DogPointVec.at(5), 1);
	BnsMove(DogPointVec.at(6), 1);
	LOG_CF_D( L"杀狗完毕!");
	return TRUE;
}

RealBnsScriptFunction(Script_SetKffm_Fm_Round)
{
	BnsScript_CheckParmCount(3);

	auto& MonsterName = BnsScript_GetWCHARParm(0);
	FixPointInfo RoundInfo(CSomeFun::GetInstance().GetPoint_By_ScriptParm(BnsScript_GetWCHARParm(1)), BnsScript_GetFLOATParm(2));
	CFarm::GetInstance().SetPlayerClassesRoundMode();
	return CGameVariable::GetInstance().Action_Map_Kffm_Fm_SpecialMonster([&MonsterName, &RoundInfo](std::map<std::wstring, FixPointInfo>& map_)
	{
		auto itr = map_.find(MonsterName);
		if (itr == map_.end())
			map_.insert(std::make_pair(MonsterName, RoundInfo));
		else
			itr->second = RoundInfo;
		LOG_CF_D( L"SpecialMonsterMap.size=%d", map_.size());
	});
}

RealBnsScriptFunction(Script_ExcuteSpecialTask)
{
	BnsScript_CheckParmCount(2);

	CONST std::wstring cwstr = BnsScript_GetWCHARParm(0);
	TaskInfoParm TaskParm = CBnsScript::GetTaskInfoParm_By_ScriptParm(BnsScript_GetWCHARParm(1));

	if (cwstr == L"10级副本")
		CFarmSpeialTask::GetInstance().SpecialTask_135([&TaskParm]{return CTask::GetInstance().IsCompletionTask(TaskParm); });
	else if (cwstr == L"分裂迷宫扔炸弹")
		CFarmSpeialTask::GetInstance().ThrowBomb_Maze();
	else if (cwstr == L"分裂迷宫转盘")
		CFarmSpeialTask::GetInstance().TurnTable_Maze();
	else if (cwstr == L"分裂迷宫分裂之柱")
		CFarmSpeialTask::GetInstance().Maze_Column();
	else if (cwstr == L"分裂迷宫法器强化队长")
		CFarmSpeialTask::GetInstance().MagicCaptain_Maze();
	else
		LOG_MSG_CF(L"无效参数:%s", cwstr.c_str());

	return TRUE;
}

RealBnsScriptFunction(Script_KillBoss_By_Dis)
{
	BnsScript_CheckParmCount(3);
	CONST std::wstring& wsBossName  = BnsScript_GetWCHARParm(0);
	CONST std::wstring& wsType		= BnsScript_GetWCHARParm(2);
	float fKillDis                  = BnsScript_GetFLOATParm(1);
	float fMinDis                   = MAX_MOVE_DIS;
	INT uIndex                      = -1;

	for (UINT i = 0; i < _VecPoint.size(); ++i)
	{
		float fDis = fabs(CBaseObject::GetPersonDis(_VecPoint.at(i)));
		if (fDis < fMinDis)
		{
			uIndex = i;
			fMinDis = fDis;
		}
	}

	if (uIndex == -1)
	{
		//LOG_CF_D( L"没有离设置坐标最近点至少是%.2fm的坐标", MAX_MOVE_DIS);
		_VecPoint.clear();
		return TRUE;
	}

	for (UINT i = static_cast<UINT>(uIndex); i < _VecPoint.size() && GameRun && !EMPTY_PERSONHP; ++i)
	{
		CPersonFunction::GetInstance().WaitGameLoad();

		if (!BnsMove(_VecPoint.at(i), 1.0f))
		{
			LOG_CF_E( L"走最近点检测到无法到达的坐标,需要重新执行任务!");
			_VecPoint.clear();
			return FALSE;
		}

		if (CMonsterExtend::GetInstance().FindMonster_By_Name(wsBossName, CBaseObject::CreateFixPointInfo(fKillDis), nullptr))
		{
			if (wsType == L"Boss优先")
			{
				CFarmBoss::GetInstance().KillBoss_By_Name(wsBossName, CBaseObject::CreateFixPointInfo(30.0f));
			}
			else if (wsType == L"小怪优先")
			{
				// UnExist -> ClearAroundMonster
				CFilter::GetInstance().AddMonsterFilter(wsBossName, CFilter::MonsterFilterType::Filter_Monster_Low);
				CFarm::GetInstance().ClearAroundMonster(CBaseObject::CreateFixPointInfo(fKillDis), []{return FALSE; });
				CFilter::GetInstance().RemoveMonsterFilter(wsBossName);
				CFarmBoss::GetInstance().KillBoss_By_Name(wsBossName, CBaseObject::CreateFixPointInfo(30.0f));
			}
			else
			{
				LOG_MSG_CF(L"参数错误:%s", wsType.c_str());
			}
		}

		// UnExist -> ClearAroundMonster
		CFarm::GetInstance().ClearAroundMonster(CBaseObject::CreateFixPointInfo(fKillDis), []{return FALSE; });
	}

	_VecPoint.clear();

	return TRUE;
}

RealBnsScriptFunction(Script_AttractBoss)
{
	BnsScript_CheckParmCount(3);

	if (_VecPoint.size() == 0)
	{
		LOG_MSG_CF(L"引BOSS的坐标呢?");
		return FALSE;
	}

	em_Player_Classes emPlayerClasses = CPerson::GetInstance().GetClasses();
	if (emPlayerClasses == em_Player_Classes_Destroyer)
	{
		LOG_CF_D( L"等待技能1,2冷却完毕!");
		while (GameRun && !EMPTY_PERSONHP && (!CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_1, TRUE) || !CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Lux_SkillId_2, TRUE)) && BnsSleep(500));
		LOG_CF_D( L"1,2技能冷却完毕了!");
	}
	else if (emPlayerClasses == em_Player_Classes_KongFuForceMaster)
	{
		LOG_CF_D( L"等待技能C冷却完毕!");
		while (GameRun && !EMPTY_PERSONHP && !CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Kffm_Kfm_SkillId_C, TRUE) && BnsSleep(500));
		LOG_CF_D( L"C技能冷却完毕了!");
	}
	else if (emPlayerClasses == em_Player_Classes_Summoner)
	{
		LOG_CF_D( L"等待技能X冷却完毕!");
		while (GameRun && !EMPTY_PERSONHP && !CFarmSkill::GetInstance().IsCool_By_SkillId_In_Ui(Sum_SkillId_X, TRUE) && BnsSleep(500));
		LOG_CF_D( L"X技能冷却完毕了!");
	}


	CONST auto& wsBossName = BnsScript_GetWCHARParm(0);
	auto fAttractDis = BnsScript_GetFLOATParm(1);
	auto dwTimeOut = BnsScript_GetDexParm(2);

	CONST auto& AttractBossPoint = _VecPoint.back();

	CMonster Monster;
	BOOL bRetCode = CSomeFun::GetInstance().TimeOut_Condiction_GameRun(dwTimeOut, [&wsBossName, &fAttractDis, &Monster, &AttractBossPoint]{
		return !CMonsterExtend::GetInstance().FindMonster_By_Name(wsBossName, CBaseObject::CreateFixPointInfo(fAttractDis), &Monster) &&
			!CMonsterExtend::GetInstance().FindMonster_By_Name(wsBossName, FixPointInfo(AttractBossPoint, 16), &Monster) && !EMPTY_PERSONHP;
	});

	if (!bRetCode)
	{
		// 这个是引来的!
		if (CMonsterExtend::GetInstance().FindMonster_By_Name(wsBossName, CBaseObject::CreateFixPointInfo(fAttractDis), nullptr))
		{
			if (emPlayerClasses == em_Player_Classes_Destroyer)
			{
				LOG_CF_D( L"找到BOSS了, 先用1,2击晕");
				Monster.SetAngle();
				BnsSleep(200);
				Monster.SetAngle();
				BnsSleep(200);
				CSomeFun::GetInstance().SimlationKey(VK_1);
				CSomeFun::GetInstance().SimlationKey(VK_1);
				Monster.SetAngle();
				BnsSleep(100);
				Monster.SetAngle();
				BnsSleep(100);
				CSomeFun::GetInstance().SimlationKey(VK_2);
				CSomeFun::GetInstance().SimlationKey(VK_2);
				BnsSleep(100);
				CSomeFun::GetInstance().SimlationKey('F', 200);
				CSomeFun::GetInstance().SimlationKey('F', 200);

				LOG_CF_D( L"沿着路径跑!");
				for (CONST auto& itm : _VecPoint)
					BnsMove(itm, 3);

				CSomeFun::GetInstance().SimlationKey('Z', 200);
			}
			else if (emPlayerClasses == em_Player_Classes_KongFuForceMaster)
			{
				Monster.SetAngle();
				BnsSleep(200);
				Monster.SetAngle();
				BnsSleep(200);
				CSomeFun::GetInstance().SimlationKey(VK_2);
				CSomeFun::GetInstance().SimlationKey(VK_2);

				LOG_CF_D( L"沿着路径跑!");
				for (CONST auto& itm : _VecPoint)
					BnsMove(itm, 3);

				BnsSleep(1500);
				CSomeFun::GetInstance().SimlationKey('C', 200);
			}
			else if (emPlayerClasses == em_Player_Classes_Summoner)
			{
				Monster.SetAngle();
				BnsSleep(200);
				Monster.SetAngle();
				BnsSleep(200);
				CSomeFun::GetInstance().SimlationKey('R',10);
				CSomeFun::GetInstance().SimlationKey('R',10);
				

				LOG_CF_D( L"沿着路径跑!");
				for (CONST auto& itm : _VecPoint)
					BnsMove(itm, 3);

				CSomeFun::GetInstance().SimlationKey('E', 10);
				BnsSleep(1500);
				CSomeFun::GetInstance().SimlationKey('X', 200);
			}

			LOG_CF_D( L"等待怪物进攻击范围!");
			auto ulTick = ::GetTickCount64();
			while (GameRun && !EMPTY_PERSONHP && Monster.GetDis() > 5.0f && static_cast<DWORD>(::GetTickCount64() - ulTick) <= 10 * 1000 && BnsSleep(500));
		}
		else if (CMonsterExtend::GetInstance().FindMonster_By_Name(wsBossName, FixPointInfo(AttractBossPoint, 16), nullptr))
		{
			// 已经有人在打了!
			BnsMove(Monster.GetPoint(), 5);
		}

		CFarmBoss::GetInstance().KillBoss_By_Name(wsBossName, FixPointInfo(AttractBossPoint, 30));
	}
	else
		LOG_CF_D(L"引怪已经超时了!");

	_VecPoint.clear();
	return TRUE;
}

RealBnsScriptFunction(Script_BreakDoor)
{
	BnsScript_CheckParmCount(2);
	return CFarm::GetInstance().FarmCorpse(BnsScript_GetWCHARParm(0), CBaseObject::CreateFixPointInfo(BnsScript_GetFLOATParm(1)));
}

RealBnsScriptFunction(Script_SetKffm_Fm_Round_Around)
{
	BnsScript_CheckParmCount(3);
	// 圆形走位清怪([Point],半径,Dis)
	FixPointInfo RoundPoint(CSomeFun::GetInstance().GetPoint_By_ScriptParm(BnsScript_GetWCHARParm(0)), BnsScript_GetFLOATParm(1));
	FixPointInfo FixPtInfo = CBaseObject::CreateFixPointInfo(BnsScript_GetFLOATParm(2));

	CFarm::GetInstance().SetPlayerClassesRoundMode();
	return CFarm::GetInstance().FarmAround_By_Round(RoundPoint, FixPtInfo, [&FixPtInfo]{return CMonsterExtend::GetInstance().GetAroundMonsterCount(FixPtInfo) == 0; });
}

RealBnsScriptFunction(Script_AddSkill_By_Level)
{
	if (!CPersonFunction::GetInstance().WaitForWarStatus())
		return TRUE;

	CInstanceZoneScript InstanceZoneScript;
	CInstanceZoneScript::_em_InstanceZoneScript_Exit_Type emExitType = CInstanceZoneScript::Script_Exit_Type_Count;;

	// 关闭任务超时判断
	DWORD dwValue = CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_TASKTIMEOUTID, NULL);

	std::wstring wsScriptName;
	em_Player_Classes emPlayerClasses = CPerson::GetInstance().GetClasses();
	DWORD dwLevel = CPerson::GetInstance().GetLevel();
	switch (emPlayerClasses)
	{
	case em_Player_Classes_KongFuForceMaster:
		wsScriptName = L"气宗加技能45级.inf";
		break;
	case em_Player_Classes_Destroyer:
		if (dwLevel < 26 && dwLevel >= 20)
			wsScriptName = L"加技能20级.inf";
		else if (dwLevel < 28 && dwLevel >= 26)
			wsScriptName = L"加技能26级-群.inf";
		else if (dwLevel < 31 && dwLevel >= 28)
			wsScriptName = L"加技能28级.inf";
		else if (dwLevel < 35 && dwLevel >= 31)
			wsScriptName = L"加技能31级-单.inf";
		else if (dwLevel < 39 && dwLevel >= 35)
			wsScriptName = L"加技能35级-群.inf";
		else if (dwLevel < 42 && dwLevel >= 39)
			wsScriptName = L"加技能39级-单.inf";
		else if (dwLevel < 45 && dwLevel >= 42)
			wsScriptName = L"加技能42级-群.inf";
		else if (dwLevel < 50 && dwLevel >= 45)
			wsScriptName = L"加技能45级.inf";
		else if (dwLevel >= 50)
			wsScriptName = L"加技能50级.inf";
		break;
	default:
		break;
	}

	InstanceZoneScript.Run(wsScriptName.c_str(), emExitType, L"1");

	// 还原任务超时判断
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_TASKTIMEOUTID, dwValue);
	return TRUE;
}

RealBnsScriptFunction(Script_Buy_By_Bag)
{
	BnsScript_CheckParmCount(1);
	CONST std::wstring wsItemName = BnsScript_GetWCHARParm(0);
	if (wsItemName == Item_衣料)
		CBagAction::GetInstance().Buy(Item_特殊硬币, 0x1, 0x2C, 0x1);
	else
		LOG_MSG_CF(L"不能兑换该物品!:%s", wsItemName.c_str());
	return TRUE;
}

RealBnsScriptFunction(Script_PullWareHouse)
{
	BnsScript_CheckParmCount(2);
	//
	std::vector<std::wstring> vItem = { BnsScript_GetWCHARParm(1) };
	return CBagAction::GetInstance().PullItem_By_WareHouse(BnsScript_GetWCHARParm(0), vItem);
}

RealBnsScriptFunction(Script_UpdateDailyTask)
{
	// TaskID,DailyTaskName,Type
	BnsScript_CheckParmCount(2);

	DWORD dwDailyTaskId = BnsScript_GetDWORDParm(0);

	// 判断任务是否都存在
	if (CTask::GetInstance().ExistTask(dwDailyTaskId, nullptr))
	{
		LOG_CF_D( L"任务ID:%X 交任务失败", dwDailyTaskId);
		return TRUE;
	}

	LOG_CF_D( L"保存任务:[%X] 到服务器为'完成'!", dwDailyTaskId);
	CDailyExtend::GetInstance().SetDailyTaskFinish(BnsScript_GetWCHARParm(1));
	return TRUE;
}

RealBnsScriptFunction(Script_JudgePickDailyTask)
{
	BnsScript_CheckParmCount(2);

	DWORD dwTaskId = BnsScript_GetDWORDParm(0);
	DWORD nCount = 0;
	while (GameRun && !EMPTY_PERSONHP)
	{
		if (CTask::GetInstance().ExistTask(dwTaskId, nullptr))
		{
			LOG_CF_D( L"接日常任务成功! ID:%X 已经存在了!", dwTaskId);
			CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CURRENT_DAILYTASK, dwTaskId);
			break;
		}
		LOG_CF_D( L"接日常任务失败……循环接任务!");

		// Record Sumit BL Task Content!

		if (_LastPickBLTaskContent.wsType == L"Npc")
			CTaskExtend::GetInstance().PickBLTask_By_Npc(_LastPickBLTaskContent.SumitBLTask_Src_, _LastPickBLTaskContent.wsName);
		else if (_LastPickBLTaskContent.wsType == L"Item")
			CTaskExtend::GetInstance().PickBLTask_By_Item(_LastPickBLTaskContent.SumitBLTask_Src_, _LastPickBLTaskContent.wsName);
		else if (_LastPickBLTaskContent.wsType == L"Parm")
			CTaskExtend::GetInstance().PickBLTask_By_Parm(_LastPickBLTaskContent.SumitBLTask_Src_, [](SumitBLTask_Src&){ return TRUE; });

		if (++nCount >= 10)
		{
			LOG_CF_D( L"接任务:[%X] 都失败!, 修改该任务进度=完成!", dwTaskId);
			CDailyExtend::GetInstance().SetDailyTaskFinish(BnsScript_GetWCHARParm(1));
			break;
		}
		BnsSleep(1000);
	}
	return TRUE;
}

RealBnsScriptFunction(Script_TaskChannel)
{
	BnsScript_CheckParmCount(1);

	SYSTEMTIME  SysTime;
	::GetLocalTime(&SysTime);
	auto fnCondiction = [&SysTime](_In_ WORD nHour, _In_ WORD nMinute)
	{
		return SysTime.wHour == nHour && SysTime.wMinute <= nMinute;
	};

	CONST std::wstring wsTaskName = BnsScript_GetWCHARParm(0);
	if (wsTaskName == L"丰收村")
	{
		// Refush Channel 17:00 - 17:30  20:00 - 20:30   23:00 - 23:30
		if (fnCondiction(17, 30) || fnCondiction(20, 30) || fnCondiction(23, 30))
		{
			LOG_CF_D( L"当前小时是:%d, 分钟是:%d, 进入丰收村换线!", SysTime.wHour, SysTime.wMinute);
			CFarm::GetInstance().SetFixChannel(0);
		}
		else
			LOG_CF_D( L"当前小时是:%d, 分钟是:%d, 不进入丰收村换线!", SysTime.wHour, SysTime.wMinute);
	}
	else if (wsTaskName == L"大漠")
	{
		if (CPerson::GetInstance().GetChannel() != 1)
		{
			CFarm::GetInstance().SetFixChannel(1);
		}
		else
		{
			LOG_CF_D( L"本身就在1线,return");
		}
	}
	else
	{
		LOG_MSG_CF(L"参数错误");
		return FALSE;
	}
	return TRUE;
}


RealBnsScriptFunction(Script_ExistMonster)
{
	BnsScript_CheckParmCount(6);

	CONST auto& MonsterName = BnsScript_GetWCHARParm(0);

	enum em_ExistMonster_Type{ em_ExistMonster_Type_Monster, em_ExistMonster_Type_Npc };
	enum em_ExistMonster_Condiction{ emExistMonster_if, emExistMonster_while };

	em_ExistMonster_Condiction emExistMonsterCondiction;
	if (BnsScript_GetWCHARParm(2) == L"if")
		emExistMonsterCondiction = emExistMonster_if;
	else if (BnsScript_GetWCHARParm(2) == L"while")
		emExistMonsterCondiction = emExistMonster_while;
	else
	{
		LOG_MSG_CF(L"第二个参数不正确! 只能是if || while");
		return FALSE;
	}

	em_ExistMonster_Type emExistMonsterType;
	if (BnsScript_GetWCHARParm(1) == L"Monster")
		emExistMonsterType = em_ExistMonster_Type_Monster;
	else if (BnsScript_GetWCHARParm(1) == L"Npc")
		emExistMonsterType = em_ExistMonster_Type_Npc;
	else
	{
		LOG_MSG_CF(L"第二个参数不正确! 只能是Monster || Npc");
		return FALSE;
	}

	FixPointInfo FixPtInfo(CSomeFun::GetInstance().GetPoint_By_ScriptParm(BnsScript_GetWCHARParm(3)), BnsScript_GetFLOATParm(4));

	if (emExistMonsterType == em_ExistMonster_Type_Monster)
	{
		if (CMonsterExtend::GetInstance().FindMonster_By_Name(MonsterName, FixPtInfo, nullptr))
		{
			LOG_CF_D( L"存在怪物:%s, 执行回调!", MonsterName.c_str());
			ExuteExtendLoop(BnsScript_GetWCHARParm(5), *_pCurrentScriptCode);
			return TRUE;
		}
	}
	else if (emExistMonsterType == em_ExistMonster_Type_Npc)
	{
		if (CNpcExtend::GetInstance().FindNpc_By_Name(MonsterName, FixPtInfo, nullptr))
		{
			LOG_CF_D( L"存在Npc:%s, 执行回调!", MonsterName.c_str());
			ExuteExtendLoop(BnsScript_GetWCHARParm(5), *_pCurrentScriptCode);
			return TRUE;
		}
	}

	if (emExistMonsterCondiction == emExistMonster_if)
	{
		LOG_CF_D( L"if模式,只执行一次!");
		return TRUE;
	}

	LOG_CF_D( L"范围内不存在怪物!");
	return BnsSleep(500);
}

RealBnsScriptFunction(Script_UnExistMonster)
{
	BnsScript_CheckParmCount(6);
	CONST auto& MonsterName = BnsScript_GetWCHARParm(0);

	enum em_ExistMonster_Type{ em_ExistMonster_Type_Monster, em_ExistMonster_Type_Npc };
	enum em_ExistMonster_Condiction{ emExistMonster_if, emExistMonster_while };

	em_ExistMonster_Condiction emExistMonsterCondiction;
	if (BnsScript_GetWCHARParm(2) == L"if")
		emExistMonsterCondiction = emExistMonster_if;
	else if (BnsScript_GetWCHARParm(2) == L"while")
		emExistMonsterCondiction = emExistMonster_while;
	else
	{
		LOG_MSG_CF(L"第二个参数不正确! 只能是if || while");
		return FALSE;
	}

	em_ExistMonster_Type emExistMonsterType;
	if (BnsScript_GetWCHARParm(1) == L"Monster")
		emExistMonsterType = em_ExistMonster_Type_Monster;
	else if (BnsScript_GetWCHARParm(1) == L"Npc")
		emExistMonsterType = em_ExistMonster_Type_Npc;
	else
	{
		LOG_MSG_CF(L"第二个参数不正确! 只能是Monster || Npc");
		return FALSE;
	}

	FixPointInfo FixPtInfo(CSomeFun::GetInstance().GetPoint_By_ScriptParm(BnsScript_GetWCHARParm(3)), BnsScript_GetFLOATParm(4));

	if (emExistMonsterType == em_ExistMonster_Type_Monster)
	{
		if (!CMonsterExtend::GetInstance().FindMonster_By_Name(MonsterName, FixPtInfo, nullptr))
		{
			LOG_CF_D( L"不存在怪物:%s, 执行回调!", MonsterName.c_str());
			ExuteExtendLoop(BnsScript_GetWCHARParm(5), *_pCurrentScriptCode);
			return TRUE;
		}
	}
	else if (emExistMonsterType == em_ExistMonster_Type_Npc)
	{
		if (!CNpcExtend::GetInstance().FindNpc_By_Name(MonsterName, FixPtInfo, nullptr))
		{
			LOG_CF_D( L"不存在Npc:%s, 执行回调!", MonsterName.c_str());
			ExuteExtendLoop(BnsScript_GetWCHARParm(5), *_pCurrentScriptCode);
			return TRUE;
		}
	}


	if (emExistMonsterCondiction == emExistMonster_if)
	{
		LOG_CF_D( L"if模式,只执行一次!");
		return TRUE;
	}

	LOG_CF_D( L"范围内存在怪物!");
	return BnsSleep(500);
}


RealBnsScriptFunction(Script_SetDisableSkill)
{
	BnsScript_CheckParmCount(2);

	DWORD dwSkillID = BnsScript_GetDWORDParm(1);
	auto& str = BnsScript_GetWCHARParm(0);
	if (str == L"禁用")
	{
		CFarmSkill::GetInstance().AddDisableSkillId(dwSkillID);
	}
	else if (str == L"恢复")
	{
		CFarmSkill::GetInstance().RemoveDisableSkillId(dwSkillID);
	}
	else
	{
		LOG_MSG_CF(L"参数错误:%s, 只能是禁用||恢复", str.c_str());
		return FALSE;
	}

	return TRUE;
}

RealBnsScriptFunction(Script_Escape)
{
	BnsScript_CheckParmCount(0);
	return CPersonFunction::GetInstance().DoEscape();
}

RealBnsScriptFunction(Script_SetRecvGiftName)
{
	BnsScript_CheckParmCount(1);
	CONST auto& ItemName = BnsScript_GetWCHARParm(0);
	return CGameVariable::GetInstance().Action_Set_GiftFilterName([&ItemName](std::set<std::wstring>& set_){
		if (set_.find(ItemName) == set_.end())
			set_.insert(ItemName);
	});
}

RealBnsScriptFunction(Script_StartRecvGift)
{
	BnsScript_CheckParmCount(0);

	std::vector<std::wstring> GiftFilterNameVec;
	CGameVariable::GetInstance().Action_Set_GiftFilterName([&GiftFilterNameVec](std::set<std::wstring>& set_)
	{
		GiftFilterNameVec.assign(set_.begin(), set_.end());
	});

	// 打开礼箱
	LOG_CF_D(L"打开还没拆的礼箱");
	CMailAction::GetInstance().RecvUnOpenGift(GiftFilterNameVec);

	// 接收礼箱
	LOG_CF_D(L"打开已经拆的礼箱");
	return CMailAction::GetInstance().RecvOpenGift(GiftFilterNameVec);

}

RealBnsScriptFunction(Script_ExistMonster_HP)
{
	BnsScript_CheckParmCount(3);
	CONST std::wstring& wsName = BnsScript_GetWCHARParm(0);
	CONST std::wstring& wsType = BnsScript_GetWCHARParm(1);
	FixPointInfo FixPtInfo = CBaseObject::CreateFixPointInfo(BnsScript_GetFLOATParm(2));

	LOG_CF_D( L"执行等待怪物死亡功能");
	if (wsType == L"Monster")
	{
		CMonsterExtend::GetInstance().DoAction_By_MonsterName(wsName, FixPtInfo, [&FixPtInfo](CONST CMonster& Monster){
			while (GameRun && Monster.GetDis() < FixPtInfo.fDis && Monster.GetHp() != NULL && BnsSleep(500));
			LOG_CF_D( L"等待的怪物已经死亡,准备返回");
		});
	}
	else if (wsType == L"Npc")
	{
		CNpcExtend::GetInstance().DoAction_By_ExistNpc_For_Name(wsName, FixPtInfo, [&FixPtInfo](CONST CNpc& Npc){
			while (GameRun && Npc.GetDis() < FixPtInfo.fDis && Npc.GetHp() != NULL && BnsSleep(500));
			LOG_CF_D( L"等待的Npc已经死亡,准备返回");
		});
	}
	else
	{
		LOG_MSG_CF(L"错误的参数:%s", wsType.c_str());
	}

	return TRUE;
}

RealBnsScriptFunction(Script_PickCorpse_By_SetPoint)
{
	BnsScript_CheckParmCount(3);

	CFarm::GetInstance().PickCorpse_By_PointList(BnsScript_GetWCHARParm(1), BnsScript_GetDWORDParm(0), CBaseObject::CreateFixPointInfo(BnsScript_GetFLOATParm(2)), _VecPoint);
	_VecPoint.clear();
	return TRUE;
}

RealBnsScriptFunction(Script_SetDungoenTime)
{
	BnsScript_CheckParmCount(1);

	CGameVariable::GetInstance().GetTickVariable_By_Name(BnsScript_GetWCHARParm(0)) = ::GetTickCount64();
	return TRUE;
}

RealBnsScriptFunction(Script_IsDungoenTimeOut)
{
	BnsScript_CheckParmCount(2);

	auto ulTick = CGameVariable::GetInstance().GetTickVariable_By_Name(BnsScript_GetWCHARParm(0));
	if (static_cast<DWORD>(::GetTickCount64() - ulTick) >= BnsScript_GetDexParm(1) * 1000)
	{
		LOG_CF_E( L"副本超时,用时:%ds,重启游戏!", static_cast<DWORD>((::GetTickCount64() - ulTick) / 1000));
		//CBnsGameFun::GetInstance().SetCloseGame(em_Close_Result_ReStart);
		CGameVariable::GetInstance().SetForceBackToSwitchPlayer();
	}
	return TRUE;
}

RealBnsScriptFunction(Script_TeamGather)
{
	BnsScript_CheckParmCount(4);
	// 队伍集合(ScheduleName, ScheduleIndex, AtleaseTeamMember,[Point])

	if (EMPTY_PERSONHP)
		return TRUE;

	static CONST float fMaxGatherDis = 15.0f;
	Point GatherPoint = CBnsScript::GetPoint_By_ScriptParm(BnsScript_GetWCHARParm(3));
	if (CPerson::GetInstance().GetDis(GatherPoint) > fMaxGatherDis)
	{
		LOG_CF_D(L"与集合点的坐标距离=%.2f, 超过了%.2f", CPerson::GetInstance().GetDis(GatherPoint), fMaxGatherDis);
		return TRUE;
	}

	// Remove Timeout Check!
	auto dwCheckPausePointTimeOut = CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CHECKPAUSEPOINT, 0);
	auto dwCheckTaskTimeOut = CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_TASKTIMEOUTID, 0);

	CONST std::wstring& wsScheduleName = BnsScript_GetWCHARParm(0);
	DWORD	dwScheduleIndex = BnsScript_GetDexParm(1);
	DWORD dwLeastTeamMemberCount = BnsScript_GetDexParm(2);

	CFarm::GetInstance().CallBackCat();
#if BnsProject_DLL
	if (CTeamMemberExtend::GetInstance().GetTeamMemberCount() == 0)
	{
		LOG_CF_D( L"队伍没有人……集合毛线!");
		CClientTeam::GetInstance().BnsScript_LeaveGungoenTeam();
		return FALSE;
	}

	// 
	CSomeFun::GetInstance().SendKeepALive(nullptr,
		MyTools::CCharacter::MakeFormatText(L"日常任务[%s]的进度[%s]中", _wsCurrentDailyTaskName.c_str(), wsScheduleName.c_str()));

	ULONGLONG ulTick = ::GetTickCount64();
	CClientTeam::GetInstance().RefreshGunoneTeamSchedule();
	auto& TeamDungoneSchedule_ = CClientTeam::GetInstance().GetTeamDungoenSchedule();
	// 获取队伍其他人的进度, 判断最高人的进度
	TeamMemberSchedule MaxSceduleTeamMember;
	Point FixPoint = CPerson::GetInstance().GetPoint();
	if (TeamDungoneSchedule_.FindByMaxScheduleIndex(MaxSceduleTeamMember))
	{
		// 当最大进度 > 现在的进度的时候, 直接break;
		if (MaxSceduleTeamMember.uScheduleIndex > dwScheduleIndex)
		{
			LOG_CF_D( L"最大进度的已经到了%d去了,现在进度只是:%d, break;", MaxSceduleTeamMember.uScheduleIndex, dwScheduleIndex);
			goto lb_Exit;
		}

		BnsMove(FixPoint, 3);
		LOG_CF_D( L"等待其他人的进度……");
		while (GameRun && !EMPTY_PERSONHP)
		{
			CFarm::GetInstance().ClearAround_TarIdSelf(CBaseObject::CreateFixPointInfo(16));

			// 否则就是当前的进度是最新的, 需要等待其他队友!
			CClientTeam::GetInstance().SetDungeonSchedule(wsScheduleName, dwScheduleIndex);
			CClientTeam::GetInstance().RefreshGunoneTeamSchedule();
			if (TeamDungoneSchedule_.VecMember.size() == 0)
			{
				LOG_CF_D( L"队伍进度表.size=0");
				BnsSleep(1000);
				continue;
			}


			if (static_cast<DWORD>(::GetTickCount64() - ulTick) >= 5 * 60 * 1000)
			{
				LOG_CF_E( L"5分钟队伍集合超时! 离开队伍!");
				CClientTeam::GetInstance().BnsScript_LeaveGungoenTeam();
				return FALSE;
			}

			if (CTeamMemberExtend::GetInstance().GetTeamMemberCount() < dwLeastTeamMemberCount)
			{
				LOG_CF_D( L"当前队伍人数=%d人, 至少需要%d个人集合. 没法玩了. 退队包平安!", CTeamMemberExtend::GetInstance().GetTeamMemberCount(), dwLeastTeamMemberCount);
				CClientTeam::GetInstance().BnsScript_LeaveGungoenTeam();
				// return 0 让下面的代码不继续执行了!
				return FALSE;
			}

			TeamDungoneSchedule_.FindByMaxScheduleIndex(MaxSceduleTeamMember);

			BOOL bSame = TRUE;
			DWORD dwLoseTeamMemberCount = 0;
			for (CONST auto& itm : TeamDungoneSchedule_.VecMember)
			{
				BOOL bLose = FALSE;
				CTeamMemberExtend::GetInstance().Action_By_TeamMemberName(itm.wsPlayerName, [&bLose](CTeamMember& Member){ bLose = !Member.IsConnect(); });
				if (bLose)
				{
					LOG_CF_D( L"队友:[%s,%s] 已经掉线了", itm.wsAccountName.c_str(), itm.wsPlayerName.c_str());
					dwLoseTeamMemberCount += 1;
					continue;
				}

				if (itm.uScheduleIndex == NULL)
				{
					LOG_CF_D( L"队友:[%s,%s] 还没初始化副本进度……等!", itm.wsAccountName.c_str(), itm.wsPlayerName.c_str());
					bSame = FALSE;
					break;
				}

				if (itm.uScheduleIndex < MaxSceduleTeamMember.uScheduleIndex)
				{
					LOG_CF_D( L"当前最新的进度是:[%s,%X], 队友:[%s,%s] 的进度是:[%s,%X], 等!", MaxSceduleTeamMember.wsScheduleName.c_str(), \
						MaxSceduleTeamMember.uScheduleIndex, itm.wsAccountName.c_str(), itm.wsPlayerName.c_str(), itm.wsScheduleName.c_str(), itm.uScheduleIndex);

					bSame = FALSE;
					break;
				}
			}

			if (dwLoseTeamMemberCount != 0)
			{
				LOG_CF_D( L"有人掉线了! 等上线", CTeamMemberExtend::GetInstance().GetTeamMemberCount() - dwLoseTeamMemberCount, dwLeastTeamMemberCount);
				continue;
			}

			
			if (bSame)
			{
				LOG_CF_D( L"所有人的进度都一致了,继续往下跑!");
				break;
			}

			// 所有人进度不一样之前, 先等着!
			BnsSleep(2000);
		}
	}
#endif
#ifdef BnsProject_Dlg
	ULONGLONG ulTick = ::GetTickCount64();
	// 获取队伍其他人的进度, 判断最高人的进度
	TeamMemberGatherContent* pMaxSceduleTeamMember = nullptr;
	Point FixPoint = CPerson::GetInstance().GetPoint();

	InitTeamGatherShareMemory();
	auto fnFindByMaxScheduleIndex = [this, &pMaxSceduleTeamMember]
	{
		UINT uMinScheduleIndex = 0;
		for (UINT i = 0; i < _pTeamGatherContent->uCount; ++i)
		{
			if (_pTeamGatherContent->ArrMember[i].uScheduleIndex >= uMinScheduleIndex)
			{
				uMinScheduleIndex = _pTeamGatherContent->ArrMember[i].uScheduleIndex;
				pMaxSceduleTeamMember = &_pTeamGatherContent->ArrMember[i];
			}
		}
	};
	LOG_C_D(L"_pTeamGatherContent->uCount=%d", _pTeamGatherContent->uCount);
	while (GameRun && !EMPTY_PERSONHP)
	{
		if (_pTeamGatherContent->uCount >= dwLeastTeamMemberCount)
			break;
		BnsSleep(500);
	}
	
	fnFindByMaxScheduleIndex();
	if (pMaxSceduleTeamMember != nullptr)
	{
		// 当最大进度 > 现在的进度的时候, 直接break;
		if (pMaxSceduleTeamMember->uScheduleIndex > dwScheduleIndex)
		{
			LOG_CF_D( L"最大进度的已经到了[%s,%s,%d]去了,现在进度只是:%d, break;", \
				pMaxSceduleTeamMember->wszAccountName, pMaxSceduleTeamMember->wszScheduleName, pMaxSceduleTeamMember->uScheduleIndex, dwScheduleIndex);
			goto lb_Exit;
		}

		BnsMove(FixPoint, 3);
		LOG_CF_D( L"等待其他人的进度……");
		while (GameRun && !EMPTY_PERSONHP)
		{
			CFarm::GetInstance().ClearAround_TarIdSelf(CBaseObject::CreateFixPointInfo(16));

			// 否则就是当前的进度是最新的, 需要等待其他队友!
			MyTools::CCharacter::wstrcpy_my(_pTeamMemberGatherContent->wszScheduleName, wsScheduleName.c_str());
			_pTeamMemberGatherContent->uScheduleIndex = dwScheduleIndex;

			if (static_cast<DWORD>(::GetTickCount64() - ulTick) >= 5 * 60 * 1000)
			{
				LOG_MSG_CF(L"5分钟队伍集合超时!");
				continue;
			}


			fnFindByMaxScheduleIndex();

			BOOL bSame = TRUE;
			DWORD dwLoseTeamMemberCount = 0;
			for (UINT i = 0; i < _pTeamGatherContent->uCount; ++i)
			{
				CONST auto& itm = _pTeamGatherContent->ArrMember[i];
				BOOL bLose = FALSE;
				CTeamMemberExtend::GetInstance().Action_By_TeamMemberName(itm.wszPlayerName, [&bLose](CTeamMember& Member){ bLose = !Member.IsConnect(); });
				if (bLose)
				{
					LOG_CF_D( L"队友:[%s,%s] 已经掉线了, 忽略他!", itm.wszAccountName, itm.wszPlayerName);
					dwLoseTeamMemberCount += 1;
					continue;
				}

				if (itm.uScheduleIndex == NULL)
				{
					LOG_CF_D( L"队友:[%s,%s] 还没初始化副本进度……等!", itm.wszAccountName, itm.wszPlayerName);
					bSame = FALSE;
					break;
				}

				if (itm.uScheduleIndex < pMaxSceduleTeamMember->uScheduleIndex)
				{
					LOG_CF_D( L"当前最新的进度是:[%s,%X], 队友:[%s,%s] 的进度是:[%s,%X], 等!", pMaxSceduleTeamMember->wszScheduleName, \
						pMaxSceduleTeamMember->uScheduleIndex, itm.wszAccountName, itm.wszPlayerName, itm.wszScheduleName, itm.uScheduleIndex);

					bSame = FALSE;
					break;
				}
			}

			if (CTeamMemberExtend::GetInstance().GetTeamMemberCount() - dwLoseTeamMemberCount < dwLeastTeamMemberCount)
			{
				LOG_CF_D( L"当前队伍在线人数=%d人, 至少需要%d个人集合. 没法玩了. 退队包平安!", CTeamMemberExtend::GetInstance().GetTeamMemberCount() - dwLoseTeamMemberCount, dwLeastTeamMemberCount);
				// return 0 让下面的代码不继续执行了!
				return TRUE;
			}


			if (bSame)
			{
				LOG_CF_D( L"所有人的进度都一致了,继续往下跑!");
				break;
			}

			// 所有人进度不一样之前, 先等着!
			BnsSleep(2000);
		}
	}
	else
		LOG_MSG_CF(L"pMaxSceduleTeamMember = nullptr!");
#endif
lb_Exit:;
	// restore Timeout Check!

	CTeamMemberExtend::GetInstance().SetCurrentTeamIndexByClasses();
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CHECKPAUSEPOINT, dwCheckPausePointTimeOut);
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_TASKTIMEOUTID, dwCheckTaskTimeOut);
	return TRUE;
}

RealBnsScriptFunction(Script_TakeCatForSummoner)
{
	BnsScript_CheckParmCount(1);

	struct SummonerCatContent
	{
		 std::wstring wsCatName;
		DWORD dwParamter;
	};

	std::vector<SummonerCatContent> SummonerCatContentVec = {
		{ Npc_嘟嘟, 0x060800F4 }, { Npc_皮皮, 0x050800F4 }, { Npc_淘淘, 0x040800F4 },
		{ Npc_乖乖, 0x030800F4 }, { Npc_咪咪, 0x020800F4 },
	};

	CONST std::wstring wsCatName = BnsScript_GetWCHARParm(0);
	CONST SummonerCatContent* p = MyTools::CLPublic::Vec_find_if(SummonerCatContentVec, [&wsCatName](CONST SummonerCatContent& itm){
		return itm.wsCatName == wsCatName;
	});
	if (p == nullptr)
	{
		LOG_MSG_CF(L"不存在这个猫名:[%s]", wsCatName.c_str());
		return FALSE;
	}

	CNpc Npc;
	if (!CNpcExtend::GetInstance().FindNpc_By_Name(wsCatName,CBaseObject::CreateFixPointInfo(10.0f), &Npc))
	{
		LOG_CF_E( L"无法搜索到这个猫名:[%s]的Npc", wsCatName.c_str());
		return FALSE;
	}

	BnsMove(Npc.GetPoint(), 1);
	BnsSleep(1000);

	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&Npc, &p]{
		CGameSend::InitializeCatForSummoner(Npc.GetId(), Npc.GetIdParm(), L"Cat", p->dwParamter);
	}) && BnsSleep(2 * 1000) && CPersonFunction::GetInstance().WaitGameLoad();
}

RealBnsScriptFunction(Script_SetTimeoutByInstanceCount)
{
	BnsScript_CheckParmCount(2);
	DWORD dwCount = BnsScript_GetDexParm(0);
	DWORD dwTime = BnsScript_GetDexParm(1);

	if (_ulInstanceTimeOutTick == NULL)
		_ulInstanceTimeOutTick = ::GetTickCount64();

	DWORD dwSpentSecond = static_cast<DWORD>(::GetTickCount64() - _ulInstanceTimeOutTick) / 1000;
	if (dwSpentSecond >= dwTime)
	{
		_ulInstanceTimeOutTick = ::GetTickCount64();
		_dwInstanceTimeOutCount += 1;
		LOG_CF_D( L"打副本超时, 次数+1=%d, 耗时[%d]s, 超过了设定的时间[%d]s", _dwInstanceTimeOutCount, dwSpentSecond, dwTime);
	}
	else
	{
		LOG_CF_D( L"副本花费了%ds, 并没有超过%ds,次数重置=0", dwSpentSecond, dwTime);
		_ulInstanceTimeOutTick = ::GetTickCount64();
		_dwInstanceTimeOutCount = 0;
	}


	LOG_CF_D( L"设置的最大超时次数:%d <= 当前超时次数:%d", dwCount, _dwInstanceTimeOutCount);
	if (dwCount <= _dwInstanceTimeOutCount)
	{
		LOG_CF_E(L"副本超时了! 准备小退!");
		CBnsConfig::GetInstance().WriteAccountLog(MyTools::CCharacter::MakeFormatText(L"打副本超时, 次数:%d >= %d, 耗时%ds, 超过了%ds",
			dwCount, _dwInstanceTimeOutCount, dwSpentSecond, dwTime));


		CGameVariable::GetInstance().SetForceBackToSwitchPlayer();
	}

	return TRUE;
}

RealBnsScriptFunction(Script_SetTeamChannel)
{
	BnsScript_CheckParmCount(1);

	DWORD dwChannel = 0;
#if BnsProject_DLL
	dwChannel = CClientTeam::GetInstance(). GetChannelForServer(CBnsScript::GetTaskInfoParm_By_ScriptParm(BnsScript_GetWCHARParm(0)));
	LOG_CF_D(L"Server返回的频道是%d", dwChannel);
#else
	dwChannel = CPerson::GetInstance().GetChannel();
	LOG_CF_D( L"dlg下切换到自身频道%d", dwChannel);
#endif
	return CFarm::GetInstance().SetFixChannel(dwChannel, TRUE);
}

RealBnsScriptFunction(Script_SupplementCat)
{
	BnsScript_CheckParmCount(1);
	DWORD dwPercentHp = BnsScript_GetDexParm(0);

	CFarm::GetInstance().BeckonCat();

	CCat Cat;
	if (!CCatExpend::GetInstance().FindSelfCat(&Cat) || Cat.IsDead() || Cat.GetPercentHp() >= dwPercentHp)
		return TRUE;

	while (GameRun && !EMPTY_PERSONHP && Cat.GetPercentHp() != 100)
	{
		CFarm::GetInstance().ClearAround_TarIdSelf(CBaseObject::CreateFixPointInfo(17.0f));
		if (CPerson::GetInstance().GetPercentHp() < 30)
		{
			LOG_CF_D( L"人物血量低于30,等待吃药回血……");
			BnsSleep(5000);
			continue;
		}

		LOG_CF_D( L"鼓励回血中……");
		CFarmSkill::GetInstance().UseSkill_By_SkillId(Sum_SkillId_Z);
		BnsSleep(1 * 1000);
		CPersonFunction::GetInstance().WaitForProgressBarBySkill(4 * 1000);
	}

	return TRUE;
}

RealBnsScriptFunction(Script_WaitToOpenTheDoor)
{
	BnsScript_CheckParmCount(3);

	DWORD dwOldValue = CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CHECKPAUSEPOINT, NULL);

	CCorpse Door;
	FixPointInfo FixPtInfo = CBaseObject::CreateFixPointInfo(BnsScript_GetFLOATParm(1));

	CONST std::wstring wsDoorName = GetUnEmptyObjName(BnsScript_GetWCHARParm(0));
	if (wsDoorName == L"")
	{
		// 最近的门
		if (!CResExtend::GetInstance().FindCorpse_By_Condition(FixPtInfo, &Door, [](CONST CCorpse&){ return TRUE; }))
		{
			LOG_CF_E( L"找不到周围%.2f的门:%s", BnsScript_GetFLOATParm(1), wsDoorName.c_str());
			CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CHECKPAUSEPOINT, dwOldValue);
			return TRUE;
		}
	}
	else if (wsDoorName[0] == '!')
	{
		// 排除
		if (!CResExtend::GetInstance().FindCorpse_By_Condition(FixPtInfo, &Door, [&wsDoorName](CONST CCorpse& Corpse){ return Corpse.GetName() != wsDoorName; }))
		{
			LOG_CF_E( L"找不到周围%.2f的门:%s", BnsScript_GetFLOATParm(1), wsDoorName.c_str());
			CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CHECKPAUSEPOINT, dwOldValue);
			return TRUE;
		}
	}
	else
	{
		if (!CResExtend::GetInstance().FindCorpse_By_Name(wsDoorName, FixPtInfo, &Door))
		{
			LOG_CF_E( L"找不到周围%.2f的门:%s", BnsScript_GetFLOATParm(1), wsDoorName.c_str());
			CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CHECKPAUSEPOINT, dwOldValue);
			return TRUE;
		}
	}


	LOG_CF_D( L"等待开门……");
	DWORD dwTimeOut = BnsScript_GetDexParm(2);
	for (auto ulTick = ::GetTickCount64(); static_cast<DWORD>(::GetTickCount64() - ulTick) < dwTimeOut * 1000 && GameRun && !EMPTY_PERSONHP && Door.IsCollect();)
	{
		
		CFarm::GetInstance().ClearAround_TarIdSelf(CBaseObject::CreateFixPointInfo(16));
		BnsSleep(1000);
	}

	
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_CHECKPAUSEPOINT, dwOldValue);
	LOG_CF_D( L"等待完毕!");
	return TRUE;
}

RealBnsScriptFunction(Script_ClearMonsterForTaskByRound)
{
	BnsScript_CheckParmCount(3);

	// 圆形走位任务清怪([TaskId,ScheduleId,KillCount],[RoundPoint,RoundDis],dis)
	CONST TaskContentMsg Tcm = GetTaskContentMsg_By_ScriptParm(BnsScript_GetWCHARParm(0));
	FixPointInfo RoundPoint = GetFixPointInfo_By_ScriptParm(BnsScript_GetWCHARParm(1));
	FixPointInfo FixPtInfo = CBaseObject::CreateFixPointInfo(BnsScript_GetFLOATParm(2));

	CFarm::GetInstance().SetPlayerClassesRoundMode();
	return CFarm::GetInstance().FarmAround_By_Round(RoundPoint, FixPtInfo, [&Tcm]{return CTask::GetInstance().IsCompletionTask(Tcm.Ti) || (Tcm.dwKillCount != NULL && CTask::GetInstance().QueryTaskScheduleCount(Tcm.Ti) >= Tcm.dwKillCount); });
}

RealBnsScriptFunction(Script_WaitToExistMonster)
{
	BnsScript_CheckParmCount(2);

	CONST std::wstring wsMonsterName = BnsScript_GetWCHARParm(0);
	CONST FixPointInfo FixPtInfo = GetFixPointInfo_By_ScriptParm(BnsScript_GetWCHARParm(1));

	Point FixPoint = CPerson::GetInstance().GetPoint();
	while (GameRun && !EMPTY_PERSONHP && !CMonsterExtend::GetInstance().FindMonster_By_Name(wsMonsterName, FixPtInfo, nullptr))
	{
		CMonster Monster;
		if (CMonsterExtend::GetInstance().FindMonster_By_Name(wsMonsterName, CBaseObject::CreateFixPointInfo(16.0f), &Monster) && Monster.GetTargetId() != NULL)
		{
			LOG_CF_D( L"怪物都已经打起来了! 不等了!");
			break;
		}
		if (!CMonsterExtend::GetInstance().FindMonster_By_Name(wsMonsterName, CBaseObject::CreateFixPointInfo(-1), nullptr))
		{
			LOG_CF_D( L"怪物都挂了! 不等了!");
			break;
		}
		CFarm::GetInstance().ClearAround_TarIdSelf(CBaseObject::CreateFixPointInfo(16));
		BnsMove(FixPoint, 3);
		BnsSleep(500);
	}

	return TRUE;
}

RealBnsScriptFunction(Script_BaiXianPratice)
{
	BnsScript_CheckParmCount(2);
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_BAIXIAN_STAGE_STEP, BnsScript_GetDexParm(0));
	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_BAIXIAN_TIMEOUT, BnsScript_GetDexParm(1) * 1000);
	return CBaiXianPracticeFieldServices::GetInstance().Begin();
}

RealBnsScriptFunction(Script_SetDeadPoint)
{
	BnsScript_CheckParmCount(1);

	if (GameRun && !EMPTY_PERSONHP)
		CFindPathScript::GetInstance().SetDeadPoint(CSomeFun::GetInstance().GetPoint_By_ScriptParm(BnsScript_GetWCHARParm(0)));
	return TRUE;
}

RealBnsScriptFunction(Script_InitDungonePoint)
{
	BnsScript_CheckParmCount(0);
	if (GameRun && !EMPTY_PERSONHP)
	{
		//CFindPathScript::GetInstance().InitGameScript(*reinterpret_cast<CBnsScript*>(lpCustomeFunParm), [](CONST Point& TarPoint){
		//	BnsMove(TarPoint, 3);
		//});
	}
	return TRUE;
}

RealBnsScriptFunction(Script_ClearDungonePoint)
{
	BnsScript_CheckParmCount(0);
	if (GameRun && !EMPTY_PERSONHP)
		CFindPathScript::GetInstance().ClearGameScript();
	return TRUE;
}

RealBnsScriptFunction(Script_ClearDeadPoint)
{
	BnsScript_CheckParmCount(0);
	if (GameRun && !EMPTY_PERSONHP)
		CFindPathScript::GetInstance().ClearDeadPoint();
	return TRUE;
}

RealBnsScriptFunction(Script_SetDeadCallBack)
{
	BnsScript_CheckParmCount(2);
	if (GameRun && !EMPTY_PERSONHP)
	{
		CFindPathScript::GetInstance().SetDeadPoint(CSomeFun::GetInstance().GetPoint_By_ScriptParm(BnsScript_GetWCHARParm(0)));

		/*while (GameRun)
		{
			LOG_CF_D( L"执行回调:%s", BnsScript_GetWCHARParm(1).c_str());
			((CBnsScript*)lpCustomeFunParm)->Excute(BnsScript_GetWCHARParm(1).c_str(), CBnsGameFun::GetInstance().GetScriptExceptionPtr());

			// 人物没挂的时候就break
			if (!EMPTY_PERSONHP) 
				break;

			CPersonFunction::GetInstance().Resurrect();
			CSomeFun::GetInstance().SetSpeed(3.0f);
			CFindPathScript::GetInstance().MoveToDeadPoint(CPerson::GetInstance().GetPoint());
		}
		*/
	}
	CFindPathScript::GetInstance().ClearDeadPoint();
	return TRUE;
}

RealBnsScriptFunction(Script_GuiMenDefence)
{
	BnsScript_CheckParmCount(2);

	return CFarmSpeialTask::GetInstance().Defence_GuiMen(BnsScript_GetWCHARParm(0), CBnsScript::GetFixPointInfo_By_ScriptParm(BnsScript_GetWCHARParm(1)), []{
#if BnsProject_DLL
		CClientTeam::GetInstance().BnsScript_LeaveGungoenTeam();
#endif
	});
}

RealBnsScriptFunction(Script_SetLeaseTeamMemberCount)
{
	BnsScript_CheckParmCount(1);

#if BnsProject_DLL
	DWORD dwLeaseCount = BnsScript_GetDexParm(0);
	if (CTeamMemberExtend::GetInstance().GetTeamMemberCount() < dwLeaseCount)
	{
		LOG_CF_D( L"当前队伍人数=%d < %d, 只能怒退队伍了!", CTeamMemberExtend::GetInstance().GetTeamMemberCount(), dwLeaseCount);
		CClientTeam::GetInstance().BnsScript_LeaveGungoenTeam();
		return FALSE;
	}
#endif
	LOG_CF_D( L"注入测试下无视人数!");
	return TRUE;
}

RealBnsScriptFunction(Script_SetCloudConfig)
{
	BnsScript_CheckParmCount(2);
	// 设置云配置(ConfigName,ConfigValue)

	if (EMPTY_PERSONHP)
		return TRUE;

#if BnsProject_DLL
	CBnsConfig::BnsConfigContent ConfigContent_;
	ConfigContent_.wsConfigName = BnsScript_GetWCHARParm(0);
	ConfigContent_.wsConfigValue = BnsScript_GetWCHARParm(1);
	CBnsConfig::GetInstance().SetSingleConfigValueByServer(ConfigContent_);
#endif

	return TRUE;
}

RealBnsScriptFunction(Script_BlackForestDefence)
{
	BnsScript_CheckParmCount(3);
	return CFarmSpeialTask::GetInstance().Defence_BlackForest(BnsScript_GetWCHARParm(0), BnsScript_GetDexParm(1), CBnsScript::GetFixPointInfo_By_ScriptParm(BnsScript_GetWCHARParm(2)), []
	{
#if BnsProject_DLL
		CClientTeam::GetInstance().BnsScript_LeaveGungoenTeam();
#endif
	});
}

RealBnsScriptFunction(Script_SetTeamSchedule)
{
	BnsScript_CheckParmCount(2);
	// 设置队伍云进度(ScheduleName, ScheduleIndex)

	if (EMPTY_PERSONHP)
		return TRUE;

#if BnsProject_DLL
	if (CTeamMemberExtend::GetInstance().GetTeamMemberCount() == 0)
	{
		LOG_CF_D( L"队伍没有人……!");
		return TRUE;
	}

	CClientTeam::GetInstance().SetDungeonSchedule(BnsScript_GetWCHARParm(0), BnsScript_GetDexParm(1));
#else
	InitTeamGatherShareMemory();
	_pTeamMemberGatherContent->dwScheduleId = BnsScript_GetDexParm(1);
	MyTools::CCharacter::wstrcpy_my(_pTeamMemberGatherContent->wszScheduleName, BnsScript_GetWCHARParm(0).c_str());
	//CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->dwTeamSchedule = BnsScript_GetDexParm(1);
#endif
	return TRUE;
}

RealBnsScriptFunction(Script_WaitToClose)
{
	// 等待怪物接近(MonsterName,dis,maxdis)
	BnsScript_CheckParmCount(3);

	CONST std::wstring wsMonsterName = BnsScript_GetWCHARParm(0);
	FixPointInfo FixPtInfo = CBaseObject::CreateFixPointInfo(BnsScript_GetFLOATParm(1));

	Point FixPoint = CPerson::GetInstance().GetPoint();
	while (GameRun && !EMPTY_PERSONHP && CMonsterExtend::GetInstance().FindMonster_By_Name(wsMonsterName, CBaseObject::CreateFixPointInfo(BnsScript_GetFLOATParm(2)), nullptr))
	{
		CMonster Monster;
		if (CMonsterExtend::GetInstance().FindMonster_By_Name(wsMonsterName, FixPtInfo, &Monster))
		{
			LOG_CF_D( L"怪物成功靠近!");
			break;
		}
		LOG_CF_D( L"怪物现在的距离是:%.2f, 靠近%.2f才可以", Monster.GetDis(), FixPtInfo.fDis);
		CFarm::GetInstance().ClearAround_TarIdSelf(FixPtInfo);
		BnsMove(FixPoint, 3);
		BnsSleep(500);
	}

	return TRUE;
}

RealBnsScriptFunction(Script_CallBackCat)
{
	BnsScript_CheckParmCount(0);

	CFarm::GetInstance().CallBackCat();
	return TRUE;
}

RealBnsScriptFunction(Script_DungoneMove_By_SetPoint)
{
	BnsScript_CheckParmCount(0);
	if (_VecPoint.size() == 0)
		return TRUE;

	while (GameRun && !EMPTY_PERSONHP)
	{
		if (CPerson::GetInstance().GetPoint().X == 0 || CPerson::GetInstance().GetPoint().Y == 0)
		{
			CPersonFunction::GetInstance().WaitGameLoad();
			continue;
		}
		int nIndex = MyTools::CLPublic::GetRecentlyIndexByVec(_VecPoint, CPerson::GetInstance().GetPoint(), 30.0f * 50);
		if (nIndex == -1)
		{
			LOG_CF_D(L"和所有坐标距离都>30");
			break;
		}

		CPersonFunction::GetInstance().MoveToPoint_For_List(_VecPoint);
		if (CPerson::GetInstance().GetDis(_VecPoint.at(_VecPoint.size() - 1)) <= 5)
		{
			LOG_CF_D(L"副本走最近点 成功!");
			break;
		}
		BnsSleep(1000);
	}

	_VecPoint.clear();
	return TRUE;
}

RealBnsScriptFunction(Script_LeaveGungone)
{
	BnsScript_CheckParmCount(2);

	CONST std::wstring wsDoorName = CBnsScript::GetUnEmptyObjName(BnsScript_GetWCHARParm(0));
	FixPointInfo FixPtInfo = CBaseObject::CreateFixPointInfo(BnsScript_GetFLOATParm(1));

	if (wsDoorName != L"")
	{
		CResExtend::GetInstance().DoAction_By_ExistCorpse_For_Name(BnsScript_GetWCHARParm(0), FixPtInfo, [&FixPtInfo](CONST CCorpse& Corpse)
		{
			BnsMove(Corpse.GetPoint(), FixPtInfo.fDis);
			Corpse.Take();
		});
	}
	else
	{
		CResExtend::GetInstance().Action_Corpse_By_MinDis(FixPtInfo, [&FixPtInfo](CONST CCorpse& Corpse)
		{
			BnsMove(Corpse.GetPoint(), FixPtInfo.fDis);
			Corpse.Take();
		});
	}
	
	BnsSleep(5 * 1000);
	CPersonFunction::GetInstance().WaitGameLoad();
	return TRUE;
}

RealBnsScriptFunction(Script_TakeShield_To_Rebound)
{
	BnsScript_CheckParmCount(2);

	CONST std::wstring wsBossName = BnsScript_GetWCHARParm(0);
	auto FixPtInfo = CBaseObject::CreateFixPointInfo(BnsScript_GetFLOATParm(1));

	CMonster Monster;
	if (!CMonsterExtend::GetInstance().FindMonster_By_Name(wsBossName, FixPtInfo, &Monster))
		return TRUE;

	CFarmBoss::GetInstance().TakeShield(FixPtInfo, [&Monster]
	{
		Monster.SetAngle();
		return Monster.IsDead();
	});

	return CPersonFunction::GetInstance().CancelPutDownStatus();
}

RealBnsScriptFunction(Script_TarClear_SilentBoat)
{
	BnsScript_CheckParmCount(1);

	auto FixPtInfo = CBaseObject::CreateFixPointInfo(BnsScript_GetFLOATParm(0));
	while (GameRun && !EMPTY_PERSONHP && CMonsterExtend::GetInstance().GetAroundMonsterCount(FixPtInfo) > 0)
	{
		CMonster Monster;
		if (!CMonsterExtend::GetInstance().FindMonster_By_Condiction(FixPtInfo, &Monster, [](CONST CMonster& tmpMonster){ return tmpMonster.GetTargetId() != NULL; }))
		{
			BnsMove(FixPtInfo.FixPoint, 3);
			BnsSleep(1000);
			continue;
		}

		CFarm::GetInstance().KillMonster_By_Single(Monster, FixPtInfo, TRUE, nullptr);
	}
	BnsMove(FixPtInfo.FixPoint, 3);
	return TRUE;
}

RealBnsScriptFunction(Script_CheckItemCount)
{
	BnsScript_CheckParmCount(2);

	CONST std::wstring	wsItemName = BnsScript_GetWCHARParm(0);
	DWORD		dwCount = BnsScript_GetDexParm(1);

	CBag Item;
	if (!CBagFunction::GetInstance().ExistItem_By_Name(wsItemName, &Item))
	{
		LOG_MSG_CF(L"背包没物品:%s, 最低要求有%d个", wsItemName.c_str(), dwCount);
		return FALSE;
	}

	if (Item.GetCount() < dwCount)
	{
		LOG_MSG_CF(L"背包物品:%s, 最低要求有%d个, 现在才%d个", wsItemName.c_str(), dwCount, Item.GetCount());
		return FALSE;
	}

	return TRUE;
}

RealBnsScriptFunction(Script_AddPlayerAttribute)
{
	BnsScript_CheckParmCount(2);

	LOG_CF_D( L"准备加点……等待战斗状态结束!");
	CPersonFunction::GetInstance().WaitForWarStatus();

	while (GameRun && !EMPTY_PERSONHP && !CObjectFunction::GetInstance().IsShow_In_GameUi(L"CharEquipWindowPanel"))
	{
		LOG_CF_D( L"按P");
		CSomeFun::GetInstance().SimlationKey('P');
		BnsSleep(1000);
	}

	DWORD dwAttributePoint = BnsScript_GetDexParm(1);
	if (BnsScript_GetWCHARParm(0) == L"攻击")
	{
		CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"CharEquipWindow_AbilltyPoint", [dwAttributePoint](CONST CGameUi& CharEquipWindow_AbilltyPoint)
		{
			/*DWORD dwMaxAttribute = ReadDWORD(CharEquipWindow_AbilltyPoint.GetBase() + 人物属性点偏移 + 0x0);
			dwMaxAttribute += ReadDWORD(CharEquipWindow_AbilltyPoint.GetBase() + 人物属性点偏移 + 0x4);
			dwMaxAttribute += ReadDWORD(CharEquipWindow_AbilltyPoint.GetBase() + 人物属性点偏移 + 0x8);
			LOG_CF_D( L"最大属性点=%d", dwMaxAttribute);*/
			CGameVariable::GetInstance().PushMainThreadFunPtr_synch([dwAttributePoint, CharEquipWindow_AbilltyPoint]{ CGameSend::AddPlayerAttribute(dwAttributePoint, ReadDWORD(CharEquipWindow_AbilltyPoint.GetBase() + 人物属性点偏移 + 0x8)); });
			BnsSleep(5000);
			CSomeFun::GetInstance().SimlationKey('P');
		});
	}
	else if (BnsScript_GetWCHARParm(0) == L"防御")
	{
		CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"CharEquipWindow_AbilltyPoint", [dwAttributePoint](CONST CGameUi& CharEquipWindow_AbilltyPoint)
		{
			CGameVariable::GetInstance().PushMainThreadFunPtr_synch([dwAttributePoint, CharEquipWindow_AbilltyPoint]{ CGameSend::AddPlayerAttribute(ReadDWORD(CharEquipWindow_AbilltyPoint.GetBase() + 人物属性点偏移 + 0x4), dwAttributePoint); });
			BnsSleep(5000);
			CSomeFun::GetInstance().SimlationKey('P');
		});
	}
	else
	{
		LOG_MSG_CF(L"Invalid Parmacter:%s", BnsScript_GetWCHARParm(0).c_str());
	}
	return TRUE;
}

RealBnsScriptFunction(Script_QuickSummonerLevel)
{
	BnsScript_CheckParmCount(1);

	DWORD dwIndex = BnsScript_GetDexParm(0);

	CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"JumpingLevelPanel", [dwIndex](CONST CGameUi& JumpingLevelPanel)
	{
		SumitTask_Src SumitTask_Src_;
		SumitTask_Src_.dwScheduleId = 1;
		SumitTask_Src_.dwTaskId     = 0x685;
		SumitTask_Src_.dwParm3      = 0;
		SumitTask_Src_.dwParm4      = 0;
		SumitTask_Src_.dwParm5      = 0;
		SumitTask_Src_.dwObjId      = 0;
		SumitTask_Src_.dwObjIdParm  = 0;
		SumitTask_Src_.dwItemIndex  = 0;

		switch (dwIndex)
		{
		case 1:
			break;
		case 2:
			SumitTask_Src_.dwParm4 = 0x1;
			CTaskExtend::GetInstance().SumitTask_By_Parm(SumitTask_Src_, L"", L"");
			BnsSleep(3 * 1000);

			SumitTask_Src_.dwParm4 = 0x0;
			CTaskExtend::GetInstance().SumitTask_By_Parm(SumitTask_Src_, L"", L"");
			BnsSleep(3 * 1000);
			break;
		case 3:
			CGameVariable::GetInstance().PushMainThreadFunPtr_synch([JumpingLevelPanel]{ CGameSend::InitializeCatForSummoner(JumpingLevelPanel.GetBase(), 0x2, L"Cat"); });
			break;
		case 4:
			CGameVariable::GetInstance().PushMainThreadFunPtr_synch([JumpingLevelPanel]{ CGameSend::SummonerTakeGift(JumpingLevelPanel.GetBase()); });
			break;
		case 5:
			
			CGameVariable::GetInstance().PushMainThreadFunPtr_synch([JumpingLevelPanel]{ CGameSend::CloseGameUi_Task_50Level(JumpingLevelPanel.GetBase());  });
			break;
		case 6:
			// Buy Item For Star Store
			break;
		case 7:
			CGameVariable::GetInstance().PushMainThreadFunPtr_synch([]{ CGameSend::UseTicket_By_Level50();  });
			
			break;
		case  8: // Leave 
			CGameVariable::GetInstance().PushMainThreadFunPtr_synch([]{ CGameSend::LeaveSkillTrainRoom_Confirm(); });
			Sleep(5000);
			CGameVariable::GetInstance().PushMainThreadFunPtr_synch([]{ CGameSend::LeaveSkillTrainRoom(); });
			break;
		
		default:
			break;
		}
	});

	return TRUE;
}

RealBnsScriptFunction(Script_SetFloating)
{
	BnsScript_CheckParmCount(3);
	FixPointInfo FixPtInfo = CBnsScript::GetFixPointInfo_By_ScriptParm(BnsScript_GetWCHARParm(2));
	if (BnsScript_GetDexParm(0) == 1 && CPerson::GetInstance().GetDis(FixPtInfo.FixPoint) > FixPtInfo.fDis)
	{
		LOG_CF_D( L"限定坐标:[%d,%d,%d], 超出限定距离:%.2f, 距离:%.2f", FixPtInfo.FixPoint.GetX(), FixPtInfo.FixPoint.GetY(), FixPtInfo.FixPoint.GetZ(), FixPtInfo.fDis, CPerson::GetInstance().GetDis(FixPtInfo.FixPoint));
		return TRUE;
	}
	
	CBnsHook::GetInstance().HookHeight(BnsScript_GetDexParm(0) == 1, BnsScript_GetFLOATParm(1));
	return TRUE;
}

RealBnsScriptFunction(Script_MoveOnSky)
{
	BnsScript_CheckParmCount(4);
	if (CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_FLOATING) != 0x1)
	{
		LOG_CF_D( L"不在浮空状态, 不能调用浮空走路!");
		return TRUE;
	}
	FixPointInfo FixPtInfo;
	FixPtInfo.FixPoint.X = BnsScript_GetFLOATParm(0);
	FixPtInfo.FixPoint.Y = BnsScript_GetFLOATParm(1);
	FixPtInfo.FixPoint.Z = BnsScript_GetFLOATParm(2);
	FixPtInfo.fDis = BnsScript_GetFLOATParm(3);
	CPersonFunction::GetInstance().Move_2D(FixPtInfo);
	return TRUE;
}

RealBnsScriptFunction(Script_Talk)
{
	BnsScript_CheckParmCount(2);

	CPersonFunction::GetInstance().WaitGameLoad();
	while (GameRun && !CObjectFunction::GetInstance().IsShow_In_GameUi(L"ChatInputPanel"))
		CSomeFun::GetInstance().SimlationKey(VK_RETURN, 200, 1000);

	CONST std::wstring& wsTalkText = BnsScript_GetWCHARParm(1);
	CONST std::wstring& wsType = BnsScript_GetWCHARParm(0);
	CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(L"ChatInputPanel", [wsTalkText, wsType](CONST CGameUi& ChatInputPanel)
	{
		if (wsType == L"普通")
			CGameVariable::GetInstance().PushMainThreadFunPtr_synch([wsTalkText, ChatInputPanel]{ CGameSend::Talk(wsTalkText.c_str(), ChatInputPanel.GetBase()); });

	});

	while (GameRun && CObjectFunction::GetInstance().IsShow_In_GameUi(L"ChatInputPanel"))
		CSomeFun::GetInstance().SimlationKey(VK_RETURN, 200, 1000);
	
	return TRUE;
}

RealBnsScriptFunction(Script_Mail_StopGame)
{
	BnsScript_CheckParmCount(0);

	CGameVariable::GetInstance().CurrentScriptName() = CGameVariable::GetInstance().GetShareInfo()->ConsoleContent.GlobalAccount.wszScriptName;
	CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->GameCmd = em_GameCmd::em_GameCmd_Stop;

	return TRUE;
}

RealBnsScriptFunction(Script_NpcTalk)
{
	BnsScript_CheckParmCount(1);

	CTaskExtend::GetInstance().TalkNpc_And_VirtualButton(BnsScript_GetWCHARParm(0));
	return TRUE;
}

RealBnsScriptFunction(Script_Run)
{
	BnsScript_CheckParmCount(1);
	CPersonFunction::GetInstance().Run(static_cast<CPersonFunction::em_Run_Type>(BnsScript_GetDexParm(0)));
	return TRUE;
}

RealBnsScriptFunction(Script_Resurrect)
{
	if (EMPTY_PERSONHP)
		CPersonFunction::GetInstance().Resurrect();
	return TRUE;
}

RealBnsScriptFunction(Script_JumpRun)
{
	BnsScript_CheckParmCount(3);

	Point RunPoint = CBnsScript::GetPoint_By_ScriptParm(BnsScript_GetWCHARParm(0));
	Point JumpPoint = CBnsScript::GetPoint_By_ScriptParm(BnsScript_GetWCHARParm(1));
	Point EndPoint = CBnsScript::GetPoint_By_ScriptParm(BnsScript_GetWCHARParm(2));

	if (CPerson::GetInstance().GetDis(RunPoint) >= 30)
	{
		LOG_CF_D( L"距离起跑点太远!");
		return TRUE;
	}

	BnsMove(RunPoint, 1);
	BnsSleep(2 * 1000);
	CBaseObject::SetAngle(EndPoint);
	BnsSleep(1000);
	CPersonFunction::GetInstance().Run(CPersonFunction::em_Run_Type::em_Run_Type_OnFloor);

	BOOL bTimeout = FALSE;
	// 跑到起跑点
	bTimeout = CSomeFun::GetInstance().TimeOut_Condiction_GameRun(5 * 1000, [RunPoint]{ return CPerson::GetInstance().GetDis(RunPoint) > 2; });
	if (bTimeout)
	{
		LOG_CF_D( L"起跑5秒竟然也不能到达起跑点!");
		return TRUE;
	}
	// 开始信仰之跃
	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([]{CGameSend::SimulationKey(VK_SPACE, TRUE); });

	bTimeout = CSomeFun::GetInstance().TimeOut_Condiction_GameRun(10 * 1000, [EndPoint]{  return CPerson::GetInstance().Get2DDis(EndPoint) > 2; });
	if (bTimeout)
	{
		LOG_CF_D( L"在空中10秒都没到达, 看来是没戏了!");
		return TRUE;
	}

	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([]{CGameSend::SimulationKey(VK_SPACE, FALSE); });
	CSomeFun::GetInstance().SimlationKey('W');
	CSomeFun::GetInstance().SimlationKey(VK_SPACE);
	return TRUE;
}

RealBnsScriptFunction(Script_TakeStarCoin)
{
	BnsScript_CheckParmCount(2);

	BOOL bSucc = FALSE;
	for (int i = 0; i < 5 && GameRun && !bSucc; ++i)
	{
		CGameSend::StarCoinContent StarCoinContent_;
		StarCoinContent_.wParm1 = static_cast<WORD>(BnsScript_GetDWORDParm(0));
		StarCoinContent_.wParm2 = static_cast<WORD>(BnsScript_GetDWORDParm(1));
		CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&bSucc, StarCoinContent_]{ bSucc = CGameSend::TakeStarCoin(StarCoinContent_); });
		BnsSleep(5 * 1000);
		LOG_CF_D( L"第%d次领取星币,结果是:%s", i + 1, bSucc ? L"领取成功!" : L"领取失败!");
	}

	return TRUE;
}

RealBnsScriptFunction(Script_ShopStore)
{
	BnsScript_CheckParmCount(2);

	DWORD dwItemId = BnsScript_GetDWORDParm(0);
	DWORD dwStarCoinCount = BnsScript_GetDexParm(1);
	LOG_CF_D(L"dwItemId=%X, dwStarCoinCount=%d", dwItemId, dwStarCoinCount);

	DWORD dwCurrentCoinCount = CPerson::GetInstance().GetStarCoin();
	if (dwCurrentCoinCount < dwStarCoinCount)
	{
		CBnsConfig::GetInstance().WriteAccountLog(MyTools::CCharacter::MakeFormatText(L"当前星币数量%d < 物品所需的星币数量%d 购买物品失败!", dwCurrentCoinCount, dwStarCoinCount));
		return TRUE;
	}
	
	while (GameRun && !CObjectFunction::GetInstance().IsShow_In_GameUi(UI_TEXT_F9))
	{
		LOG_CF_D(L"按F9!");
		CSomeFun::GetInstance().SimlationKey(VK_F9, 200, 5000);
	}

	BnsSleep(3 * 1000);
	CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(UI_TEXT_F9, [dwItemId](CONST CGameUi& GoodsStorePanel)
	{
		CGameVariable::GetInstance().PushMainThreadFunPtr_synch([dwItemId, GoodsStorePanel]{ CGameSend::ShopStarCoinStore(dwItemId, GoodsStorePanel.GetObj()); });
	});
	BnsSleep(3 * 1000);
	CSomeFun::GetInstance().SimlationKey('Y', 200, 5 * 1000);
	CSomeFun::GetInstance().SimlationKey(VK_F9, 200, 5 * 1000);


	CBnsConfig::GetInstance().WriteAccountLog(MyTools::CCharacter::MakeFormatText(L"星币商店购买物品%s,剩余星币%d!", 
		dwStarCoinCount != 0 && CPerson::GetInstance().GetStarCoin() == dwCurrentCoinCount ? L"失败" : L"成功", 
		CPerson::GetInstance().GetStarCoin()));
	return TRUE;
}

RealBnsScriptFunction(Script_XueYuGong_IceFireBird)
{
	BnsScript_CheckParmCount(2);

	FixPointInfo RoundInfo = CBnsScript::GetFixPointInfo_By_ScriptParm(BnsScript_GetWCHARParm(0));
	FixPointInfo FixPtInfo = CBaseObject::CreateFixPointInfo(50);
	
	std::vector<std::wstring> VecMonster;
	MyTools::CCharacter::Split(BnsScript_GetWCHARParm(1), L",", VecMonster, Split_Option_RemoveEmptyEntries | Split_Option_KeepOnly);

	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_KILLMONSTER_ROUND_MODE, static_cast<DWORD>(CFarm::em_KillMonster_Round_Mode_XYG_IceFireBird));
	while (GameRun && !EMPTY_PERSONHP)
	{
		BOOL bExist = FALSE;
		for (CONST auto& itm : VecMonster)
		{
			CMonster Monster;
			if (CMonsterExtend::GetInstance().FindMonster_By_Name(itm, CBaseObject::CreateFixPointInfo(30), &Monster))
			{
				bExist = TRUE;
				CFarm::GetInstance().KillMonster_By_Round(Monster, FixPtInfo, RoundInfo);
			}
		}
		

		if (!bExist)
		{
			LOG_CF_D( L"冰火鸟30m之内都遍历不到! 应该是杀完了!");
			break;
		}


		BnsSleep(500);
	}

	return TRUE;
}

RealBnsScriptFunction(Script_ExcuteDailyTask)
{
	BnsScript_CheckParmCount(0);

#if BnsProject_DLL
	std::vector<std::wstring> VecDailyTask;

	CTransferScript().QueryDailyTask(VecDailyTask);
	for(CONST auto& itm : VecDailyTask)
		LOG_CF_D(L"今天还可以执行的日常任务:%s", itm.c_str());

	CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_DISABLE_UPDATEDAILYTASK) = 0x0;
	for (CONST auto& itm : VecDailyTask)
	{
		LOG_CF_D(L"准备执行日常任务:%s", itm.c_str());
		CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_TASKTIMEOUTID, NULL);
		
		_wsCurrentDailyTaskName = itm;

		CDailyScript DailyScript;
		DailyScript.Run(itm);
	}

	return TRUE;
#else
	LOG_MSG_CF(L"注入模式下没办法执行日常!");
	return TRUE;
#endif
}

RealBnsScriptFunction(Script_ExcuteOneTimeTask)
{
	BnsScript_CheckParmCount(1);

	DWORD dwTaskId = BnsScript_GetDWORDParm(0);
	std::wstring wsConfigName;
	wsConfigName = CLOUD_CONFIG_NAME_ONE_TIME_TASK + MyTools::CCharacter::MakeFormatText(L"%X", dwTaskId);

#if BnsProject_DLL
	if (CBnsConfig::GetInstance().GetSingleConfigValueByServer(CGameVariable::GetInstance().GetAccShareInfo()->MyAcount_Info.szPlayerName, wsConfigName) == L"1")
	{
		LOG_CF_D(L"任务:[%X]已经完成了!", dwTaskId);
		return TRUE;
	}
#endif

	CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_DISABLE_UPDATEDAILYTASK) = 0x1;


	do 
	{
		CDailyScript DailyScript;

		DailyScript.Run(MyTools::CCharacter::MakeFormatText(L"%X.inf", dwTaskId));
		BnsSleep(1000);
	} while (CTask::GetInstance().ExistTask(dwTaskId, nullptr) && GameRun);
	if (GameRun)
	{
		LOG_CF_D(L"任务:[%X]已经完成了! 保存到服务器!", dwTaskId);

		CBnsConfig::BnsConfigContent Content;
		Content.wsConfigName = wsConfigName;
		Content.wsConfigValue = L"1";
		CBnsConfig::GetInstance().SetSingleConfigValueByServer(Content);;
	}

	CGameVariable::GetInstance().GetVariable_By_Name(VARIABLE_NAME_DISABLE_UPDATEDAILYTASK) = 0x0;
	return TRUE;
}

RealBnsScriptFunction(Script_SetDungoenSchedule)
{
	BnsScript_CheckParmCount(1);

	if (!EMPTY_PERSONHP)
	{
		CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_DUNGEON_SCHEDULE, BnsScript_GetDexParm(0));
	}
	else
	{
		LOG_CF_D(L"副本里应该是团灭了……复活重新再来~");
		CPersonFunction::GetInstance().Resurrect();
	}

	return TRUE;
}

RealBnsScriptFunction(Script_SetDifficultyForTeam)
{
	BnsScript_CheckParmCount(1);

	CONST std::wstring& wsType = BnsScript_GetWCHARParm(0);
	if (wsType == L"简单")
		CGameVariable::GetInstance().PushMainThreadFunPtr_synch([]{ CGameSend::SetDifficultyForTeam(CGameSend::emTeamDifficulty::emTeamDifficulty_Easy); });
	else if (wsType == L"困难")
		CGameVariable::GetInstance().PushMainThreadFunPtr_synch([]{ CGameSend::SetDifficultyForTeam(CGameSend::emTeamDifficulty::emTeamDifficulty_Normal); });
	else
		LOG_MSG_CF(L"Parm=[简单|困难]");
	return TRUE;
}

RealBnsScriptFunction(Script_SetPartyAuction)
{
	BnsScript_CheckParmCount(1);

	CONST std::wstring wsParm = BnsScript_GetWCHARParm(0);
	if (wsParm == L"黑龙教地下监狱")
		CTeamAuction::GetInstance().SetAuction_By_Gungoen_HeiLongJiao();
	else if (wsParm == L"nullptr")
		CTeamAuction::GetInstance().SetAuction_Null();

	return TRUE;
}

RealBnsScriptFunction(Script_UntieBaGua)
{
	// 解印八卦(封印的Name, 解印的Name)
	BnsScript_CheckParmCount(2);

	// 判断身上的八卦是否都是解印的八卦
	CBag Equi;
	if (!CBagFunction::GetInstance().GetEqui_By_Type(CBag::em_Equi_Type::em_Equi_Type_Diagrams1, &Equi))
		return TRUE;

	if (Equi.GetName() == BnsScript_GetWCHARParm(1))
	{
		LOG_CF_D(L"八卦已经装备上了, 就不再解印八卦了! 顺便把所有其他八卦全丢了!");
		CONST static std::vector<CBag::em_Equi_Type> VecType = 
		{ 
			CBag::em_Equi_Type::em_Equi_Type_Diagrams1, CBag::em_Equi_Type::em_Equi_Type_Diagrams2, CBag::em_Equi_Type::em_Equi_Type_Diagrams3,CBag::em_Equi_Type::em_Equi_Type_Diagrams4, 
			CBag::em_Equi_Type::em_Equi_Type_Diagrams5, CBag::em_Equi_Type::em_Equi_Type_Diagrams6, CBag::em_Equi_Type::em_Equi_Type_Diagrams7, CBag::em_Equi_Type::em_Equi_Type_Diagrams8 
		};

		// 已经一套了, 可以移除直接丢了!
		CFilter::GetInstance().AddItemFilter(Item_黑龙教八卦牌, CFilter::ItemFilterType::Filter_Item_Drop);
		CFilter::GetInstance().AddItemFilter(Item_黑龙教封印的八卦牌, CFilter::ItemFilterType::Filter_Item_Drop);

		CBagAction::GetInstance().DiscardItem_By_Type(VecType);
		return TRUE;
	}

	auto fnExistBaGuaType = [](_In_ DWORD dwEquiType, _In_ CONST std::vector<CBag>& VecItem)
	{
		return MyTools::CLPublic::Vec_find_if_Const(VecItem, [dwEquiType](_In_ CONST CBag& itm){ return itm.GetItemType() == dwEquiType; }) != nullptr;
	};

	// 判断背包是否有8件八卦了
	auto fnExistBaGuaSuit = [fnExistBaGuaType](_In_ CONST std::wstring& wsBaGuaName)
	{
		std::vector<CBag> VecItem;
		// 不足8件八卦
		if (CBagFunction::GetInstance().GetItemList_By_Name(wsBaGuaName, VecItem) < 8)
			return false;

		return	fnExistBaGuaType(static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams1), VecItem) && \
				fnExistBaGuaType(static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams2), VecItem) && \
				fnExistBaGuaType(static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams3), VecItem) && \
				fnExistBaGuaType(static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams4), VecItem) && \
				fnExistBaGuaType(static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams5), VecItem) && \
				fnExistBaGuaType(static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams6), VecItem) && \
				fnExistBaGuaType(static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams7), VecItem) && \
				fnExistBaGuaType(static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams8), VecItem);
	};

	auto fnGetUntieId = []
	{
		CBag Item;
		return CBagFunction::GetInstance().ExistItem_By_Name(L"", &Item) && Item.GetCount() >= 3 ? Item.GetGId() : NULL;
	};

	auto fnSealBaGua = [fnGetUntieId](_In_ CONST std::wstring& wsName, _In_ DWORD dwType)
	{
		CBag Item;
		if (CBagFunction::GetInstance().ExistItem_By_Condiction([wsName, dwType](_In_ CONST CBag& itm){ return itm.GetName() == wsName && itm.GetItemType() == dwType; }, &Item))
		{
			DWORD dwUntieId = fnGetUntieId();
			LOG_CF_D(L"存在封印的八卦牌[%s], 类型是[%X], 解印符的ID=%X", Item.GetName().c_str(), Item.GetItemType(), dwUntieId);
			if (dwUntieId == NULL)
				return;

			Item.EquiUntie(dwUntieId, 3,0);
			BnsSleep(2 * 1000);
		}
	};

	auto fnWearBaGua = [](_In_ CONST std::wstring& wsName, _In_ DWORD dwType)
	{
		CBag Item;
		if (CBagFunction::GetInstance().ExistItem_By_Condiction([wsName, dwType](_In_ CONST CBag& itm){ return itm.GetName() == wsName && itm.GetItemType() == dwType; }, &Item))
		{
			LOG_CF_D(L"穿八卦[%s], Type=[%d]", Item.GetName().c_str(), Item.GetItemType());
			Item.WearEqui();
			BnsSleep(1000);
		}
	};

	std::vector<CBag> VecItem;
	CBagFunction::GetInstance().GetItemList_By_Name(BnsScript_GetWCHARParm(1), VecItem); // 获取已经解印解开了的八卦牌

	// 判断第一张解开的八卦牌是否存在, 不存在的话， 就去寻找封印的一号八卦牌并且解印
	if (!fnExistBaGuaType(static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams1), VecItem))
		fnSealBaGua(BnsScript_GetWCHARParm(0), static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams1));
	if (!fnExistBaGuaType(static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams2), VecItem))
		fnSealBaGua(BnsScript_GetWCHARParm(0), static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams2));
	if (!fnExistBaGuaType(static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams3), VecItem))
		fnSealBaGua(BnsScript_GetWCHARParm(0), static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams3));
	if (!fnExistBaGuaType(static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams4), VecItem))
		fnSealBaGua(BnsScript_GetWCHARParm(0), static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams4));
	if (!fnExistBaGuaType(static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams5), VecItem))
		fnSealBaGua(BnsScript_GetWCHARParm(0), static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams5));
	if (!fnExistBaGuaType(static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams6), VecItem))
		fnSealBaGua(BnsScript_GetWCHARParm(0), static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams6));
	if (!fnExistBaGuaType(static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams7), VecItem))
		fnSealBaGua(BnsScript_GetWCHARParm(0), static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams7));
	if (!fnExistBaGuaType(static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams8), VecItem))
		fnSealBaGua(BnsScript_GetWCHARParm(0), static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams8));

	if (fnExistBaGuaSuit(BnsScript_GetWCHARParm(1)))
	{
		// 穿装备
		fnWearBaGua(BnsScript_GetWCHARParm(1), static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams1));
		fnWearBaGua(BnsScript_GetWCHARParm(1), static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams2));
		fnWearBaGua(BnsScript_GetWCHARParm(1), static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams3));
		fnWearBaGua(BnsScript_GetWCHARParm(1), static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams4));
		fnWearBaGua(BnsScript_GetWCHARParm(1), static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams5));
		fnWearBaGua(BnsScript_GetWCHARParm(1), static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams6));
		fnWearBaGua(BnsScript_GetWCHARParm(1), static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams7));
		fnWearBaGua(BnsScript_GetWCHARParm(1), static_cast<DWORD>(CBag::em_Equi_Type::em_Equi_Type_Diagrams8));
	}


	return TRUE;
}

RealBnsScriptFunction(Script_PointGather)
{
	BnsScript_CheckParmCount(1);

	UINT uTeamMemberCount = BnsScript_GetDexParm(0);

	while (GameRun && !EMPTY_PERSONHP)
	{
		std::vector<CPlayer> VecPlayer;
		if (CObjectFunction::GetInstance().GetSortGameObjectList(VecPlayer, CObjectFunction::em_GameObject_Type_Player, CBaseObject::CreateFixPointInfo(3.0f)) != uTeamMemberCount)
		{
			BnsSleep(5 * 1000);
			break;
		}
		BnsSleep(500);
	}

	return TRUE;
}

RealBnsScriptFunction(Script_OpenDoor)
{
	BnsScript_CheckParmCount(2);
	CFarm::GetInstance().TakeCorpse(CBaseObject::CreateFixPointInfo(BnsScript_GetFLOATParm(0)), CBnsScript::GetUnEmptyObjName(BnsScript_GetWCHARParm(1)));
	return TRUE;
}

RealBnsScriptFunction(Script_CheckSetPointDis)
{
	BnsScript_CheckParmCount(2);

	static std::map<ULONGLONG, std::vector<Point>> MapVecPoint;
	if (MapVecPoint.find(_pCurrentScriptCode->ulCodeHash) == MapVecPoint.end())
		MapVecPoint.insert(std::make_pair(_pCurrentScriptCode->ulCodeHash, _VecPoint));

	if (_VecPoint.size() == 0)
	{
		auto itr = MapVecPoint.find(_pCurrentScriptCode->ulCodeHash);
		if (itr == MapVecPoint.end())
		{
			LOG_MSG_CF(L"设置坐标呢?");
			return FALSE;
		}
		
		_VecPoint = itr->second;
		MapVecPoint.erase(itr);
	}

	float fDis = BnsScript_GetFLOATParm(0);
	for (CONST auto& itm : _VecPoint)
	{
		if (CPerson::GetInstance().GetDis(itm) < fDis)
		{
			LOG_CF_D(L"检查到坐标[%d,%d,%d] 距离设置的坐标< %.2f, 不回调!", itm.GetX(), itm.GetY(), itm.GetZ(), fDis);
			return TRUE;
		}
	}

	_VecPoint.clear();
	LOG_CF_D(L"所有坐标距离都>%.2fm, 执行回调:%s", fDis, BnsScript_GetWCHARParm(1).c_str());
	ExuteExtendLoop(BnsScript_GetWCHARParm(1), *_pCurrentScriptCode);
	return TRUE;
}

RealBnsScriptFunction(Script_Stop)
{
	StopGame;
	return TRUE;
}

BOOL CBnsScript::Script_AddWarehouseItem()
{
	BnsScript_CheckParmCount(1);

	_VecWarehouseTradeItem.push_back(BnsScript_GetWCHARParm(0));
	return TRUE;
}

BOOL CBnsScript::Script_ReceiveLvPrize()
{
	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([]{CGameSend::ReceiveLvPrize(); });
	return TRUE;
}
