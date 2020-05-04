#ifndef __GITBNSPROJECTS_BNSPROJECTS_SOMEFUN_TASKUPGRADE_TASKUPGRADEMGR_H__
#define __GITBNSPROJECTS_BNSPROJECTS_SOMEFUN_TASKUPGRADE_TASKUPGRADEMGR_H__

#include "GameVariable.h"

class CLoginPlayer;
class CBnsGameFun;
class CTaskUpgradeMgr : public MyTools::CClassInstance<CTaskUpgradeMgr>
{
public:
	explicit CTaskUpgradeMgr();
	~CTaskUpgradeMgr() = default;

	BOOL Run(_In_ CONST std::wstring& wsFunName);

	struct ModeTextPtr
	{
		std::wstring wsFunName;
		std::function<BOOL(VOID)> fnPtr;
	};
	std::vector<ModeTextPtr>& GetVecModePtr();
public:
	// 黑号模式
	BOOL Run_By_BlackAccount();

	// 白号模式
	BOOL Run_By_WhiteAccount();

	// 免费模式
	BOOL Run_By_FreeAccount();

	// 删号模式
	BOOL Run_By_DeleteAccount();

	//删号模式2
	BOOL Run_By_DeleteAccount2();

	// 力士模式
	BOOL Run_By_DesocoryAccount();

	// 白号力士免费模式
	BOOL Run_By_FreeWhiteAccount();

	// 加点模式
	BOOL Run_By_AddSkillAccount();

	// 手动模式
	BOOL Run_By_DeletePlayer();

	// 召唤模式
	BOOL Run_By_SummonerAccount();

	// 召唤模式2
	BOOL Run_By_SummonerAccount2();

	// 召唤50级模式
	BOOL Run_By_SummonerAccount_Level50();

	// 召唤50级模式加速
	BOOL Run_By_SummonerAccount_Speed_Level50();

	// 召唤50级模式1
	BOOL Run_By_SummonerAccount1_Level50();

	// 召唤模式7章
	BOOL RUn_By_SummonerAccount_Capter7();

	// 召唤换号模式
	BOOL Run_By_SummonerSwitchAccount();

	BOOL Run_By_ExcuteScript();

	// 创建角色模式
	BOOL Run_By_CreatePlayer();

	// 整理模式
	BOOL Run_By_Clean();

	// 保号召唤模式
	BOOL Run_By_SummonerAccount_Safe();

	// 星币兑换模式
	BOOL Run_By_StarConinMode();

	// 职业模式
	BOOL Run_By_Classes(_In_ em_Player_Classes emPlayerClasses, _In_ CONST std::wstring& wsClassesText, std::function<em_Player_Classes(DWORD)> GetCreatePlayerClassesPtr);

	// 职业模式 角色池2用
	BOOL Run_By_Classes2(_In_ em_Player_Classes emPlayerClasses, _In_ CONST std::wstring& wsClassesText, std::function<em_Player_Classes(DWORD)> GetCreatePlayerClassesPtr);

	// 咒术模式
	BOOL Run_By_Warlock();

	// 剑士模式
	BOOL Run_By_BladeMaster();

	// 灵剑模式
	BOOL Run_By_Danncer();

	// 气功模式
	BOOL Run_By_ForceMaster();

	// 气拳模式
	BOOL Run_By_KongFuForceMaster();

	// 拳师模式
	BOOL Run_By_KongFuMaster();

	// 枪手模式em_Player_Classes_Gunner
	BOOL Run_By_Gunner();

	// 删除所有角色
	BOOL DeletePlayer_From_OtherAllClasses(_In_ DWORD dwIndex) CONST;

	// 刺客模式
	BOOL Run_By_Assassin();

	// 练级模式
	BOOL Run_By_UpgradeLevelMode();

	// 练级模式2
	BOOL Run_By_UpgradeLevelMode2();
private:

	// 是否拥有超过45级角色的帐号
	BOOL ExistMoreThanLevel45_In_Account(_In_ CLoginPlayer* pLoginPlayer, _In_ std::function<BOOL(CONST CLoginPlayer&)> fnOtherCondition) CONST;

	// 所有角色都超过了16级
	BOOL IsMoreThanLevel16_In_ALL_Account() CONST;

	// 获取剩余空位位置
	int GetEmptyLocationCount() CONST;

	// 判断有多少个角色是删除状态
	UINT GetCount_By_DeleteStatus() CONST;

	// 是否有仓库号
	BOOL IsExistWarehouse_By_LoginPlayer(_Out_opt_ CLoginPlayer* pLoginPlayer) CONST;

	// 进入仓库号模式
	BOOL ToWarehouseMode() CONST;

	//是否有邮寄号
	BOOL IsExistMail_By_LoginPlayer(_Out_opt_ CLoginPlayer* pLoginPlayer) CONST;

	// 进入邮寄模式
	BOOL ToMailMode() CONST;

	// 进入特殊的模式
	BOOL ToSpecialMode() CONST;

	// 删除角色
	BOOL DeletePlayer_From_MainTask(_In_ CONST std::wstring& wsLoginPlayerName) CONST;

	// 删除无用的角色
	BOOL DeletePlayer_From_OtherClasses() CONST;

	// 写入删除角色日志
	BOOL WriteDeletePlayerLog(_In_ CONST std::wstring& wsPlayerName) CONST;

	// 50级劵用光了
	VOID ActionForUnExistLevelTick(_In_ CONST std::vector<CLoginPlayer>& VecLoginPlayer) CONST;

	// 获取整理模式可用的角色的大区
	DWORD GetPlayerAreaByCleanMode() CONST;

	// 50级召唤模式下, 是否所有角色都 < 50 && > 16
	BOOL IsExistPlayerInLevel50Mode() CONST;

	VOID RemoveClasses_By_Mode(_In_ em_Player_Classes emPlayerClasses, _In_ _Out_opt_ std::vector<CLoginPlayer>& VecLoginPlayer, std::function<em_Player_Classes(DWORD)> GetCreatePlayerClassesPtr) CONST;

	//角色池2用
	VOID RemoveClasses_By_Mode2(_In_ em_Player_Classes emPlayerClasses, _In_ _Out_opt_ std::vector<CLoginPlayer>& VecLoginPlayer, std::function<em_Player_Classes(DWORD)> GetCreatePlayerClassesPtr) CONST;

	// 角色池
	CONST std::vector<em_Player_Classes>& GetPlayerPool() CONST;

	// 角色池2
	CONST std::vector<em_Player_Classes>& GetPlayerPool2() CONST;
private:
	std::function<BOOL(VOID)> _IsDeletePlayerPtr;
	CBnsGameFun& RefBnsGameFun;
	DSIABLE_COPY_AND_ASSIGN(CTaskUpgradeMgr)
};


#endif