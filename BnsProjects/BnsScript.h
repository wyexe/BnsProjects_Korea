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
		LOG_MSG_CF( L"����:%s ����Ӧ����%d��, �㴫��%d��", __FUNCTIONW__, x, _Script_Current_ExcuteMethod.pScript_Code_Method->VecParm.size());	\
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
	BOOL Script_Move();							// �ߵ�(X,Y,Z)
	BOOL Script_UseSkill();						// ʹ�ü���(SkillId)
	BOOL Script_Sleep();						// Sleep(1000)
	BOOL Script_Farm_By_Dis();					// ��Χ���(Dis)
	BOOL Script_Sell();							// ��(NpcName)
	BOOL Script_Simulationkey();				// ����(V)
	BOOL Script_UseItem();						// ʹ����Ʒ(ItemName, SleepTime)
	BOOL Script_SetAngle();						// ��׼(Type,Parm)
	BOOL Script_Farm_By_Task();					// �������(TaskId,ScheduleId, Dis)
	BOOL Script_TakeItem_By_DropItem();			// ����(Dis)
	BOOL Script_TakeItem_By_Corpse();			// ��ʬ��(TaskId,ScheduleId, Dis)
	BOOL Script_TakeItem_By_Npc();				// ��Npc(TaskId,ScheduleId, Dis)
	BOOL Script_TakeItem_By_Npc_By_SetPoint();	// ��Χ��Npc(TaskId,ScheduleId, Dis)
	BOOL Script_MessageBox();					// MessageBox(Text)
	BOOL Script_WearEqui();						// ��װ��(EquiName)
	BOOL Script_DeliverCity();					// ����(CityName)
	BOOL Script_GameChat();						// ��ͼ()
	BOOL Script_SetSpeed();						// ����(nSpeed)
	BOOL Script_SetOptimization();				// �Ż�(100)
	BOOL Script_CleanBag();						// ������
	BOOL Script_SumitMainTask();				// ����������(Type, NpcName, parm)
	BOOL Script_PickBLTask();					// ��֧������(Type, NpcName, parm)
	BOOL Script_SetPoint();					    // ��������(X,Y,Z)
	BOOL Script_MovePoint_By_SetPoint();		// �������()
	BOOL Script_Back_MovePoint_By_SetPoint();	// ������()
	BOOL Script_GrowthEqui();					// ǿ��װ��(Type, SacrificeName)
	BOOL Script_OpenBox();					    // ������(BoxName)
	BOOL Script_OpenBox_By_Key();				// Կ�׿�����(BoxName, KeyName, keyCount)
	BOOL Script_ExplainEqui();				    // �ֽ�()
	BOOL Script_SetFixChannel();				// �л��̶�Ƶ��(nChannel)
	BOOL Script_SetRandChannel();				// �л����Ƶ��()
	BOOL Script_FilterMonster();				// ���ù������(MonsterName, FilterName{����,��ɱ,��ɱ})
	BOOL Script_FollowNpc();					// ����Npc(NpcName, TargetX,TargetY,TargetZ,Dis)
	BOOL Script_FilterItem();					// ������Ʒ����(ItemName, FilterName{�ֽ�,����,��֡���})
	BOOL Script_Buy();						    // ��(NpcName, Index, Count)
	BOOL Script_ExpandBag();					// ��չ����()
	BOOL Script_ExcuteScript();				    // ִ�нű�(ScriptName, FunName)
	BOOL Script_MoveToTarget();				    // �ƶ���Ŀ��(Type,Name,fDis)
	BOOL Script_SetGem();						// ��Ƕ��ʯ(GemName, Index)
	BOOL Script_ClearGem();					    // ȡ��ʯ()
	BOOL Script_Untie();						// ��ӡ(EquiName, UntieName,Count)
	BOOL Script_EquiEvolution();				// װ��ͻ��(EquiName, SacrificeName,Id)
	BOOL Script_EquiEndEvolution();			    // װ������(EquiName, SacrificeName, Id)
	BOOL Script_RemoveEqui();					// ��װ��(Index)
	BOOL Script_RepairWeapon();				    // ��������(�־ö�)
	BOOL Script_SaveCloth();					// �����¹�(ClothName)
	BOOL Script_PickCorpse();					// ����ʬ��(ʬ����,BUFFID,��Χ)
	BOOL Script_ClearMonsterFilter();			// ����������()
	BOOL Script_PickDropItem();				    // ����ʬʬ��(BUFFID,��Χ)
	BOOL Script_PickDropItem_By_SetPoint();	    // ��Χ����ʬʬ��(BUFFID,��Χ)
	BOOL Script_MoveTo();						// ��·(x,y,z,dis)
	BOOL Script_ExcuteTask();					// ִ����������()
	BOOL Script_ExcuteFileFun();				// ִ�нű�����(xx.inf,main)
	BOOL Script_SetExitCondition();			    // �����˳�����
	BOOL Script_SetExitFlag();				    // ���ñ�־
	BOOL Script_TakeCorpse_By_Dis();			// ��Χ��ʬ��(����ID,����ID,��Χ,ʬ����)
	BOOL Script_IsTaskDone();					// �����Ƿ����(����ID,����ID,�ص�����)
	BOOL Script_EatFood();					    // ������(HP)
	BOOL Script_EatMedicine();				    // ��ҩ(HP)
	BOOL Script_SetMailItem();				    // �����ʼ���Ʒ(ItemName)
	BOOL Script_Mail();						    // �ʼ�(NpcName,dwMoney)
	BOOL Script_StartPk();					    // ��ʼPK()
	BOOL Script_PickCorpse_No_Move();			// վ�ż�(Name,fDis)
	BOOL Script_TestMessageBox();				// ���Ե���
	BOOL Script_TestMail();					    // �����ʼ�
	BOOL Script_JoinTeam();					    // ���(��������,�Ƿ�һֱ�ȴ�)
	BOOL Script_LeaveTeam();					// ���()
	BOOL Script_JoinDungoenTeam();			    // �������(��������,�Ƿ�һֱ�ȴ�)
	BOOL Script_LeaveDungoenTeam();			    // �������()
	BOOL Script_CloseNpcTalkDlg();			    // �رնԻ�����
	BOOL Script_SetMailFialdText();			    // �����ʼ�ʧ��(PK)
	BOOL Script_TakeCorpse_By_Status();		    // �ؼ�ʬ��(Name,Dis)
	BOOL Script_RecvMail();					    // ���ʼ�(NpcName)
	BOOL Script_TaskScheduleExist();			// �ж��������(TaskId, ScheduleId, �ص�)
	BOOL Script_Task_TaskBall();				// �������(����Id, BuffId, Dis)
	BOOL Script_Pickup();						// ������(Type,Name)
	BOOL Script_Telport();					    // ˲��(FileName)
	BOOL Script_LineTelport();				    // ֱ��˲��([Point],Count)
	BOOL Script_HeightTelport();				// �߶�˲��([Point],Count)
	BOOL Script_ClearAroundMonster();			// ���(����ID,����ID,��Χ)
	BOOL Script_AttractMonster();				// Ⱥ��()
	BOOL Script_SetAutoTakItemMode();			// �����Զ�����(0||1)
	BOOL Script_SetKaDaoMode();				    // ���ÿ���ģʽ(0||1)
	BOOL Script_ChooseColorTask();			    // ʯ����ɫ����()
	BOOL Script_IceFireShieldTask();			// ������������()
	BOOL Script_KillBoss();					    // ɱBOSS
	BOOL Script_FarmAround();					// ɱ��(Type,Content,Dis)
	BOOL Script_SetVariable();				    // ���ñ���(Name,Value)
	BOOL Script_UpateSkill();					// ���ܼӵ�
	BOOL Script_StartLearnSkill();			    // ѧϰ����
	BOOL Script_ForgotSkill();				    // �������м���()
	BOOL Script_DoAction();					    // ����()
	BOOL Script_SaveWareHouse();				// ���(NpcName)
	BOOL Script_PullWareHouse();				// ȡ��(NpcName,ItemName)
	BOOL Script_Farm4Boss();					// ��Ź4��BOSS([TaskId,ScheduleId])
	BOOL Script_SetWareHouseItem();			    // ���òֿ�Ž�����Ʒ(ItemName,Count)
	BOOL Script_WareHouseDeal();				// �ֿ�Ž���(����|����,NpcName, Money)
	BOOL Script_TakeWareHouse();				// ȡ�ֿ����Ʒ()
	BOOL Script_Trance();						// ����
	BOOL Script_GiveWareHouse();				// ���ֿ����Ʒ(NpcName, Money)
	BOOL Script_KillDog_By_Kffm();			    // ����ɱ��(Dis)
	BOOL Script_SetKffm_Fm_Round();			    // ����Բ����λ(BossName,[Point],Dis)
	BOOL Script_ExcuteSpecialTask();			// ִ����������(Name,[TaskId,ScheduleId])
	BOOL Script_KillBoss_By_Dis();			    // ��ΧɱBOSS(BossName,Dis,Boss����|С������)
	BOOL Script_AttractBoss();				    // ��BOSS(BossName,[Point],Dis, TimeOut)
	BOOL Script_BreakDoor();					// ���Ŷ���(CorpseName, Dis)
	BOOL Script_SetKffm_Fm_Round_Around();	    // Բ����λ���([Point],�뾶,Dis)
	BOOL Script_AddSkill_By_Level();			// �ȼ����ܼӵ�()
	BOOL Script_Buy_By_Bag();					// ��������(Name)
	BOOL Script_UpdateDailyTask();			    // �ճ��������([TaskId],������||������)
	BOOL Script_TaskChannel();				    // ������(���մ�/��Į)
	BOOL Script_ExistMonster();				    // �������([Name],Monster||Npc,if||while,[Point],Dis,CALLBACK)
	BOOL Script_UnExistMonster();				// ���ﲻ����([Name],Monster||Npc,if||while,[Point],Dis,CALLBACK)
	BOOL Script_SetDisableSkill();			    // ���ü����ͷ�(����||�ָ�,SkillID)
	BOOL Script_Escape();						// ����
	BOOL Script_SetRecvGiftName();			    // ���ý�����������(XX)
	BOOL Script_StartRecvGift();				// ��ʼ��������()
	BOOL Script_ExistMonster_HP();			    // �ȴ���������
	BOOL Script_PickCorpse_By_SetPoint();		// ��Χ����ʬ��
	BOOL Script_SetDungoenTime();				// ���ø�����ʱ(Name)
	BOOL Script_IsDungoenTimeOut();			    // �жϸ�����ʱ(Name,Time)
	BOOL Script_JudgePickDailyTask();			// �жϽ��ճ�����(TaskId,[TaskId1,TaskId2])
	BOOL Script_TeamGather();					// ���鼯��(ScheduleName, ScheduleIndex, AtleaseTeamMember)
	BOOL Script_TakeCatForSummoner();			// �ٻ���è(è��)
	BOOL Script_SetTimeoutByInstanceCount();	// ���ø���������ʱ(Count,Time)
	BOOL Script_SetTeamChannel();				// �л�����Ƶ��()
	BOOL Script_SupplementCat();				// ��è��Ѫ(dwPercentHp)
	BOOL Script_WaitToOpenTheDoor();			// �ȴ�����(Name,Dis)
	BOOL Script_ClearMonsterForTaskByRound();	// Բ����λ�������([TaskId,ScheduleId,KillCount],[RoundPoint,RoundDis],dis)
	BOOL Script_WaitToExistMonster();			// �ȴ�����ӽ�(MonsterName,[Point,Dis]();
	BOOL Script_BaiXianPratice();				// ����������(Stage,TimeOut)
	BOOL Script_SetDeadPoint();				    // ������������([Point])
	BOOL Script_InitDungonePoint();			    // ��ʼ���ű�����()
	BOOL Script_ClearDungonePoint();			// ��սű�����()
	BOOL Script_ClearDeadPoint();				// �����������()
	BOOL Script_SetDeadCallBack();			    // ���������ص�([Point],CALLBACK)
	BOOL Script_ExpandWarehouse();			    // ��չ�����ռ�(�ֿ�||�¹�||����)
	BOOL Script_GuiMenDefence();				// ���Źط���()
	BOOL Script_SetLeaseTeamMemberCount();	    // ��Ͷ�������(Count)
	BOOL Script_SetCloudConfig();				// ����������(ConfigName,ConfigValue)
	BOOL Script_BlackForestDefence();			// ��ɭ�ַ���([Point,Dis])
	BOOL Script_SetTeamSchedule();			    // ���ö����ƽ���(ScheduleName, ScheduleIndex)
	BOOL Script_WaitToClose();				    // �ȴ����￿��(MonsterName,dis)
	BOOL Script_CallBackCat();				    // �ٻ�è()
	BOOL Script_DungoneMove_By_SetPoint();	    // �����������()
	BOOL Script_LeaveGungone();				    // �뿪����������(Name)
	BOOL Script_TakeShield_To_Rebound();		// ��ܷ���(BossName,dis)
	BOOL Script_TarClear_SilentBoat();		    // ������Ŀ�����(dis)
	BOOL Script_CheckItemCount();				// �����Ʒ����(ItemName,Count)
	BOOL Script_AddPlayerAttribute();			// �����Ե�()
	BOOL Script_QuickSummonerLevel();			// �ٻ�50������(Index)
	BOOL Script_SetFloating();				    // ���ø���(bOpen,Height)
	BOOL Script_MoveOnSky();					// ������·(X,Y,Z,dis)
	BOOL Script_Talk();						    // ����(Text)
	BOOL Script_Mail_StopGame();				// �ʼ�ֹͣ�һ�()
	BOOL Script_NpcTalk();					    // �����Ի�(NpcName)
	BOOL Script_Run();						    // ��()
	BOOL Script_Resurrect();					// ����()
	BOOL Script_JumpRun();					    // ������([SourcePoint],[JumpPoint],[EndPoint])
	BOOL Script_TakeStarCoin();				    // ��ȡ�Ǳ�(Parm1,Parm2)
	BOOL Script_ShopStore();					// �Ǳ��̵�(ID)
	BOOL Script_XueYuGong_IceFireBird();		// ѩ�񹬱�����([RoundPoint,RoundRange])
	BOOL Script_ExcuteDailyTask();				// ִ���ճ�����()
	BOOL Script_ExcuteOneTimeTask();			// ִ��һ��������(TaskID)
	BOOL Script_SetDungoenSchedule();			// ���ø�������(Schedule)
	BOOL Script_SetDifficultyForTeam();			// ���ø����Ѷ�(��|����)
	BOOL Script_SetPartyAuction();				// ���ü��ﾺ��(DungoenName)
	BOOL Script_UntieBaGua();					// ��ӡ����(��ӡ��Name, ��ӡ��Name)
	BOOL Script_PointGather();					// ���꼯��(Count)
	BOOL Script_OpenDoor();						// ��������(Name,dis)
	BOOL Script_CheckSetPointDis();				// �ж������������(dis,CALLBACK)
	BOOL Script_Stop();							// Stop()
	BOOL Script_AddWarehouseItem();  // ��Ӳֿ�Ž�����Ʒ(ItemName)
	BOOL Script_ReceiveLvPrize();  // ��ȡ������()
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
	// ��ʼ�����鼯��(ֻ��dlg����)
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

	// ��ǰ�ճ������� --- ��Ҫ�Ƿ��������õ�
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