#ifndef __BNS_BNSCRIPT_H__
#define __BNS_BNSCRIPT_H__

#include <MyTools/Script.h>
#include "GameVariable.h"
#include "PlayerTransaction.h"

#define RealBnsScriptFunction(MethodName) BOOL CBnsScript::MethodName()

#define BnsScript_CheckParmCount(x)															\
	if (!GameRun || _Script_Current_ExcuteMethod.pScript_Code_Method == nullptr)										\
	{																						\
		return TRUE;																		\
	}																						\
	else if (_Script_Current_ExcuteMethod.pScript_Code_Method->VecParm.size() != x)									\
	{																						\
		LOG_MSG_CF( L"函数:%s 参数应该是%d个, 你传了%d个", __FUNCTIONW__, x, _Script_Current_ExcuteMethod.pScript_Code_Method->VecParm.size());	\
		return FALSE;																		\
	}

#define BnsScript_GetDWORDParm(Index)	_Script_Current_ExcuteMethod.pScript_Code_Method->VecParm.at(Index).ConvertDWORD()
#define BnsScript_GetDexParm(Index)		_Script_Current_ExcuteMethod.pScript_Code_Method->VecParm.at(Index).ConvertDWORD_By_Dec()
#define BnsScript_GetFLOATParm(Index)	_Script_Current_ExcuteMethod.pScript_Code_Method->VecParm.at(Index).ConvertFLOAT()
#define BnsScript_GetWCHARParm(Index)	_Script_Current_ExcuteMethod.pScript_Code_Method->VecParm.at(Index).GetString()


class CBnsScript : public MyTools::CScript
{
public:
	CBnsScript();
	~CBnsScript();

	VOID BindCustomeMethod();
	static BOOL ReleaseBnsScript();
public:
	BOOL IsExitScript();
private:
	BOOL Script_Move();							// 走到(X,Y,Z)
	BOOL Script_UseSkill();						// 使用技能(SkillId)
	BOOL Script_Sleep();						// Sleep(1000)
	BOOL Script_Farm_By_Dis();					// 范围清怪(Dis)
	BOOL Script_Sell();							// 卖(NpcName)
	BOOL Script_Simulationkey();				// 按键(V)
	BOOL Script_UseItem();						// 使用物品(ItemName, SleepTime)
	BOOL Script_SetAngle();						// 对准(Type,Parm)
	BOOL Script_Farm_By_Task();					// 任务清怪(TaskId,ScheduleId, Dis)
	BOOL Script_TakeItem_By_DropItem();			// 捡物(Dis)
	BOOL Script_TakeItem_By_Corpse();			// 捡尸体(TaskId,ScheduleId, Dis)
	BOOL Script_TakeItem_By_Npc();				// 捡Npc(TaskId,ScheduleId, Dis)
	BOOL Script_TakeItem_By_Npc_By_SetPoint();	// 范围捡Npc(TaskId,ScheduleId, Dis)
	BOOL Script_MessageBox();					// MessageBox(Text)
	BOOL Script_WearEqui();						// 穿装备(EquiName)
	BOOL Script_DeliverCity();					// 遁术(CityName)
	BOOL Script_GameChat();						// 过图()
	BOOL Script_SetSpeed();						// 加速(nSpeed)
	BOOL Script_SetOptimization();				// 优化(100)
	BOOL Script_CleanBag();						// 整理背包
	BOOL Script_SumitMainTask();				// 交主线任务(Type, NpcName, parm)
	BOOL Script_PickBLTask();					// 接支线任务(Type, NpcName, parm)
	BOOL Script_SetPoint();					    // 设置坐标(X,Y,Z)
	BOOL Script_MovePoint_By_SetPoint();		// 走最近点()
	BOOL Script_Back_MovePoint_By_SetPoint();	// 倒着走()
	BOOL Script_GrowthEqui();					// 强化装备(Type, SacrificeName)
	BOOL Script_OpenBox();					    // 开箱子(BoxName)
	BOOL Script_OpenBox_By_Key();				// 钥匙开箱子(BoxName, KeyName, keyCount)
	BOOL Script_ExplainEqui();				    // 分解()
	BOOL Script_SetFixChannel();				// 切换固定频道(nChannel)
	BOOL Script_SetRandChannel();				// 切换随机频道()
	BOOL Script_FilterMonster();				// 设置怪物过滤(MonsterName, FilterName{优先,必杀,不杀})
	BOOL Script_FollowNpc();					// 跟随Npc(NpcName, TargetX,TargetY,TargetZ,Dis)
	BOOL Script_FilterItem();					// 设置物品过滤(ItemName, FilterName{分解,出售,存仓……})
	BOOL Script_Buy();						    // 买(NpcName, Index, Count)
	BOOL Script_ExpandBag();					// 扩展背包()
	BOOL Script_ExcuteScript();				    // 执行脚本(ScriptName, FunName)
	BOOL Script_MoveToTarget();				    // 移动到目标(Type,Name,fDis)
	BOOL Script_SetGem();						// 镶嵌宝石(GemName, Index)
	BOOL Script_ClearGem();					    // 取宝石()
	BOOL Script_Untie();						// 解印(EquiName, UntieName,Count)
	BOOL Script_EquiEvolution();				// 装备突破(EquiName, SacrificeName,Id)
	BOOL Script_EquiEndEvolution();			    // 装备进化(EquiName, SacrificeName, Id)
	BOOL Script_RemoveEqui();					// 脱装备(Index)
	BOOL Script_RepairWeapon();				    // 修理武器(持久度)
	BOOL Script_SaveCloth();					// 存入衣柜(ClothName)
	BOOL Script_PickCorpse();					// 抱起尸体(尸体名,BUFFID,范围)
	BOOL Script_ClearMonsterFilter();			// 清除怪物过滤()
	BOOL Script_PickDropItem();				    // 抱起僵尸尸体(BUFFID,范围)
	BOOL Script_PickDropItem_By_SetPoint();	    // 范围抱起僵尸尸体(BUFFID,范围)
	BOOL Script_MoveTo();						// 走路(x,y,z,dis)
	BOOL Script_ExcuteTask();					// 执行主线任务()
	BOOL Script_ExcuteFileFun();				// 执行脚本函数(xx.inf,main)
	BOOL Script_SetExitCondition();			    // 设置退出条件
	BOOL Script_SetExitFlag();				    // 设置标志
	BOOL Script_TakeCorpse_By_Dis();			// 范围捡尸体(任务ID,进度ID,范围,尸体名)
	BOOL Script_IsTaskDone();					// 任务是否完成(任务ID,进度ID,回调函数)
	BOOL Script_EatFood();					    // 吃坐红(HP)
	BOOL Script_EatMedicine();				    // 吃药(HP)
	BOOL Script_SetMailItem();				    // 设置邮寄物品(ItemName)
	BOOL Script_Mail();						    // 邮寄(NpcName,dwMoney)
	BOOL Script_StartPk();					    // 开始PK()
	BOOL Script_PickCorpse_No_Move();			// 站着捡(Name,fDis)
	BOOL Script_TestMessageBox();				// 测试弹窗
	BOOL Script_TestMail();					    // 测试邮寄
	BOOL Script_JoinTeam();					    // 组队(人数上限,是否一直等待)
	BOOL Script_LeaveTeam();					// 离队()
	BOOL Script_JoinDungoenTeam();			    // 副本组队(人物上限,是否一直等待)
	BOOL Script_LeaveDungoenTeam();			    // 副本离队()
	BOOL Script_CloseNpcTalkDlg();			    // 关闭对话界面
	BOOL Script_SetMailFialdText();			    // 设置邮寄失败(PK)
	BOOL Script_TakeCorpse_By_Status();		    // 必捡尸体(Name,Dis)
	BOOL Script_RecvMail();					    // 收邮件(NpcName)
	BOOL Script_TaskScheduleExist();			// 判断任务进度(TaskId, ScheduleId, 回调)
	BOOL Script_Task_TaskBall();				// 任务捡球(进度Id, BuffId, Dis)
	BOOL Script_Pickup();						// 捡任务(Type,Name)
	BOOL Script_Telport();					    // 瞬移(FileName)
	BOOL Script_LineTelport();				    // 直线瞬移([Point],Count)
	BOOL Script_HeightTelport();				// 高度瞬移([Point],Count)
	BOOL Script_ClearAroundMonster();			// 清怪(任务ID,进度ID,范围)
	BOOL Script_AttractMonster();				// 群怪()
	BOOL Script_SetAutoTakItemMode();			// 设置自动捡物(0||1)
	BOOL Script_SetKaDaoMode();				    // 设置卡刀模式(0||1)
	BOOL Script_ChooseColorTask();			    // 石像颜色任务()
	BOOL Script_IceFireShieldTask();			// 冰火罩子任务()
	BOOL Script_KillBoss();					    // 杀BOSS
	BOOL Script_FarmAround();					// 杀怪(Type,Content,Dis)
	BOOL Script_SetVariable();				    // 设置变量(Name,Value)
	BOOL Script_UpateSkill();					// 技能加点
	BOOL Script_StartLearnSkill();			    // 学习技能
	BOOL Script_ForgotSkill();				    // 遗忘所有技能()
	BOOL Script_DoAction();					    // 动作()
	BOOL Script_SaveWareHouse();				// 存仓(NpcName)
	BOOL Script_PullWareHouse();				// 取仓(NpcName,ItemName)
	BOOL Script_Farm4Boss();					// 轮殴4个BOSS([TaskId,ScheduleId])
	BOOL Script_SetWareHouseItem();			    // 设置仓库号交易物品(ItemName,Count)
	BOOL Script_WareHouseDeal();				// 仓库号交易(交易|物资,NpcName, Money)
	BOOL Script_TakeWareHouse();				// 取仓库号物品()
	BOOL Script_Trance();						// 发呆
	BOOL Script_GiveWareHouse();				// 给仓库号物品(NpcName, Money)
	BOOL Script_KillDog_By_Kffm();			    // 气宗杀狗(Dis)
	BOOL Script_SetKffm_Fm_Round();			    // 设置圆形走位(BossName,[Point],Dis)
	BOOL Script_ExcuteSpecialTask();			// 执行特殊任务(Name,[TaskId,ScheduleId])
	BOOL Script_KillBoss_By_Dis();			    // 范围杀BOSS(BossName,Dis,Boss优先|小怪优先)
	BOOL Script_AttractBoss();				    // 引BOSS(BossName,[Point],Dis, TimeOut)
	BOOL Script_BreakDoor();					// 破门而入(CorpseName, Dis)
	BOOL Script_SetKffm_Fm_Round_Around();	    // 圆形走位清怪([Point],半径,Dis)
	BOOL Script_AddSkill_By_Level();			// 等级技能加点()
	BOOL Script_Buy_By_Bag();					// 背包购物(Name)
	BOOL Script_UpdateDailyTask();			    // 日常任务进度([TaskId],接任务||交任务)
	BOOL Script_TaskChannel();				    // 任务换线(丰收村/大漠)
	BOOL Script_ExistMonster();				    // 怪物存在([Name],Monster||Npc,if||while,[Point],Dis,CALLBACK)
	BOOL Script_UnExistMonster();				// 怪物不存在([Name],Monster||Npc,if||while,[Point],Dis,CALLBACK)
	BOOL Script_SetDisableSkill();			    // 设置技能释放(禁用||恢复,SkillID)
	BOOL Script_Escape();						// 逃脱
	BOOL Script_SetRecvGiftName();			    // 设置接收礼箱名字(XX)
	BOOL Script_StartRecvGift();				// 开始接收礼箱()
	BOOL Script_ExistMonster_HP();			    // 等待怪物死亡
	BOOL Script_PickCorpse_By_SetPoint();		// 范围抱起尸体
	BOOL Script_SetDungoenTime();				// 设置副本超时(Name)
	BOOL Script_IsDungoenTimeOut();			    // 判断副本超时(Name,Time)
	BOOL Script_JudgePickDailyTask();			// 判断接日常任务(TaskId,[TaskId1,TaskId2])
	BOOL Script_TeamGather();					// 队伍集合(ScheduleName, ScheduleIndex, AtleaseTeamMember)
	BOOL Script_TakeCatForSummoner();			// 召唤拿猫(猫名)
	BOOL Script_SetTimeoutByInstanceCount();	// 设置副本次数超时(Count,Time)
	BOOL Script_SetTeamChannel();				// 切换队伍频道()
	BOOL Script_SupplementCat();				// 给猫回血(dwPercentHp)
	BOOL Script_WaitToOpenTheDoor();			// 等待开门(Name,Dis)
	BOOL Script_ClearMonsterForTaskByRound();	// 圆形走位任务清怪([TaskId,ScheduleId,KillCount],[RoundPoint,RoundDis],dis)
	BOOL Script_WaitToExistMonster();			// 等待怪物接近(MonsterName,[Point,Dis]();
	BOOL Script_BaiXianPratice();				// 白仙修练场(Stage,TimeOut)
	BOOL Script_SetDeadPoint();				    // 设置死亡坐标([Point])
	BOOL Script_InitDungonePoint();			    // 初始化脚本坐标()
	BOOL Script_ClearDungonePoint();			// 清空脚本坐标()
	BOOL Script_ClearDeadPoint();				// 清除死亡坐标()
	BOOL Script_SetDeadCallBack();			    // 设置死亡回调([Point],CALLBACK)
	BOOL Script_ExpandWarehouse();			    // 扩展背包空间(仓库||衣柜||背包)
	BOOL Script_GuiMenDefence();				// 鬼门关防守()
	BOOL Script_SetLeaseTeamMemberCount();	    // 最低队伍人数(Count)
	BOOL Script_SetCloudConfig();				// 设置云配置(ConfigName,ConfigValue)
	BOOL Script_BlackForestDefence();			// 黑森林防守([Point,Dis])
	BOOL Script_SetTeamSchedule();			    // 设置队伍云进度(ScheduleName, ScheduleIndex)
	BOOL Script_WaitToClose();				    // 等待怪物靠近(MonsterName,dis)
	BOOL Script_CallBackCat();				    // 召回猫()
	BOOL Script_DungoneMove_By_SetPoint();	    // 副本走最近点()
	BOOL Script_LeaveGungone();				    // 离开副本传送门(Name)
	BOOL Script_TakeShield_To_Rebound();		// 捡盾反弹(BossName,dis)
	BOOL Script_TarClear_SilentBoat();		    // 海盗船目标清怪(dis)
	BOOL Script_CheckItemCount();				// 检查物品数量(ItemName,Count)
	BOOL Script_AddPlayerAttribute();			// 加属性点()
	BOOL Script_QuickSummonerLevel();			// 召唤50级任务(Index)
	BOOL Script_SetFloating();				    // 设置浮空(bOpen,Height)
	BOOL Script_MoveOnSky();					// 浮空走路(X,Y,Z,dis)
	BOOL Script_Talk();						    // 喊话(Text)
	BOOL Script_Mail_StopGame();				// 邮寄停止挂机()
	BOOL Script_NpcTalk();					    // 按键对话(NpcName)
	BOOL Script_Run();						    // 跑()
	BOOL Script_Resurrect();					// 复活()
	BOOL Script_JumpRun();					    // 神行跳([SourcePoint],[JumpPoint],[EndPoint])
	BOOL Script_TakeStarCoin();				    // 领取星币(Parm1,Parm2)
	BOOL Script_ShopStore();					// 星币商店(ID)
	BOOL Script_XueYuGong_IceFireBird();		// 雪玉宫冰火鸟([RoundPoint,RoundRange])
	BOOL Script_ExcuteDailyTask();				// 执行日常任务()
	BOOL Script_ExcuteOneTimeTask();			// 执行一次性任务(TaskID)
	BOOL Script_SetDungoenSchedule();			// 设置副本进度(Schedule)
	BOOL Script_SetDifficultyForTeam();			// 设置副本难度(简单|困难)
	BOOL Script_SetPartyAuction();				// 设置捡物竞拍(DungoenName)
	BOOL Script_UntieBaGua();					// 解印八卦(封印的Name, 解印的Name)
	BOOL Script_PointGather();					// 坐标集合(Count)
	BOOL Script_OpenDoor();						// 副本开门(Name,dis)
	BOOL Script_CheckSetPointDis();				// 判断设置坐标距离(dis,CALLBACK)
	BOOL Script_Stop();							// Stop()
	BOOL Script_AddWarehouseItem();  // 添加仓库号交易物品(ItemName)
	BOOL Script_ReceiveLvPrize();  // 领取级别奖励()
protected:
	virtual BOOL			IsExcuteNext();
private:
	static unsigned WINAPI	_HelpThread(LPVOID lpParm);
	static BOOL				SupplementHp(DWORD dwPercentHp);
	static CONST std::wstring			GetUnEmptyObjName(CONST std::wstring& cwstr);
	static FixPointInfo		GetFixPointInfo_By_ScriptParm(CONST std::wstring& cwstr);
	static Point			GetPoint_By_ScriptParm(CONST std::wstring& wsParm);
	static TaskInfoParm GetTaskInfoParm_By_ScriptParm(CONST std::wstring& wstr);
	static TaskContentMsg	GetTaskContentMsg_By_ScriptParm(CONST std::wstring& wstr);
	
#ifdef BnsProject_Dlg
	// 初始化队伍集合(只在dlg下用)
	VOID InitTeamGatherShareMemory();
#endif
private:
	struct PickBLTaskContent
	{
		std::wstring wsType;
		std::wstring wsName;
		SumitBLTask_Src SumitBLTask_Src_;
	};
#ifdef BnsProject_Dlg
	struct TeamMemberGatherContent
	{
		WCHAR wszAccountName[64];
		WCHAR wszPlayerName[64];
		DWORD dwPlayerId;
		WCHAR wszScheduleName[64];
		UINT uScheduleIndex;
		DWORD dwTaskId;
		DWORD dwScheduleId;
		UINT uIndex;

		TeamMemberGatherContent()
		{
			dwPlayerId = dwTaskId = dwScheduleId = NULL;
			uScheduleIndex = uIndex = NULL;
		}
	};

	struct TeamGatherContent
	{
		TeamMemberGatherContent ArrMember[10];
		UINT uCount;
	};
#endif
private:
	std::vector<Point>                      _VecPoint;
	std::vector<std::wstring>              _VecMailItem;
	std::vector<LearnSkillContent>	       _VecLearnSkillId;
	std::vector<CPlayerTransaction::MainTask_Transaction_Item> _VecTransactionItem;
	std::vector<std::wstring>			_VecWarehouseTradeItem;
	PickBLTaskContent                  _LastPickBLTaskContent;
	DWORD                              _dwInstanceTimeOutCount;
	ULONGLONG                          _ulInstanceTimeOutTick;

	// 当前日常任务名 --- 主要是发送心跳用的
	std::wstring						_wsCurrentDailyTaskName;
#ifdef BnsProject_Dlg
	TeamGatherContent*					_pTeamGatherContent;
	TeamMemberGatherContent*		   _pTeamMemberGatherContent;
#endif
public:
	static HANDLE						hHelpThread;
	static BOOL							bExistHelpThread;
	BOOL								_bExitScript;
};



#endif