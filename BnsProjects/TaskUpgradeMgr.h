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
	// �ں�ģʽ
	BOOL Run_By_BlackAccount();

	// �׺�ģʽ
	BOOL Run_By_WhiteAccount();

	// ���ģʽ
	BOOL Run_By_FreeAccount();

	// ɾ��ģʽ
	BOOL Run_By_DeleteAccount();

	//ɾ��ģʽ2
	BOOL Run_By_DeleteAccount2();

	// ��ʿģʽ
	BOOL Run_By_DesocoryAccount();

	// �׺���ʿ���ģʽ
	BOOL Run_By_FreeWhiteAccount();

	// �ӵ�ģʽ
	BOOL Run_By_AddSkillAccount();

	// �ֶ�ģʽ
	BOOL Run_By_DeletePlayer();

	// �ٻ�ģʽ
	BOOL Run_By_SummonerAccount();

	// �ٻ�ģʽ2
	BOOL Run_By_SummonerAccount2();

	// �ٻ�50��ģʽ
	BOOL Run_By_SummonerAccount_Level50();

	// �ٻ�50��ģʽ����
	BOOL Run_By_SummonerAccount_Speed_Level50();

	// �ٻ�50��ģʽ1
	BOOL Run_By_SummonerAccount1_Level50();

	// �ٻ�ģʽ7��
	BOOL RUn_By_SummonerAccount_Capter7();

	// �ٻ�����ģʽ
	BOOL Run_By_SummonerSwitchAccount();

	BOOL Run_By_ExcuteScript();

	// ������ɫģʽ
	BOOL Run_By_CreatePlayer();

	// ����ģʽ
	BOOL Run_By_Clean();

	// �����ٻ�ģʽ
	BOOL Run_By_SummonerAccount_Safe();

	// �ǱҶһ�ģʽ
	BOOL Run_By_StarConinMode();

	// ְҵģʽ
	BOOL Run_By_Classes(_In_ em_Player_Classes emPlayerClasses, _In_ CONST std::wstring& wsClassesText, std::function<em_Player_Classes(DWORD)> GetCreatePlayerClassesPtr);

	// ְҵģʽ ��ɫ��2��
	BOOL Run_By_Classes2(_In_ em_Player_Classes emPlayerClasses, _In_ CONST std::wstring& wsClassesText, std::function<em_Player_Classes(DWORD)> GetCreatePlayerClassesPtr);

	// ����ģʽ
	BOOL Run_By_Warlock();

	// ��ʿģʽ
	BOOL Run_By_BladeMaster();

	// �齣ģʽ
	BOOL Run_By_Danncer();

	// ����ģʽ
	BOOL Run_By_ForceMaster();

	// ��ȭģʽ
	BOOL Run_By_KongFuForceMaster();

	// ȭʦģʽ
	BOOL Run_By_KongFuMaster();

	// ǹ��ģʽem_Player_Classes_Gunner
	BOOL Run_By_Gunner();

	// ɾ�����н�ɫ
	BOOL DeletePlayer_From_OtherAllClasses(_In_ DWORD dwIndex) CONST;

	// �̿�ģʽ
	BOOL Run_By_Assassin();

	// ����ģʽ
	BOOL Run_By_UpgradeLevelMode();

	// ����ģʽ2
	BOOL Run_By_UpgradeLevelMode2();
private:

	// �Ƿ�ӵ�г���45����ɫ���ʺ�
	BOOL ExistMoreThanLevel45_In_Account(_In_ CLoginPlayer* pLoginPlayer, _In_ std::function<BOOL(CONST CLoginPlayer&)> fnOtherCondition) CONST;

	// ���н�ɫ��������16��
	BOOL IsMoreThanLevel16_In_ALL_Account() CONST;

	// ��ȡʣ���λλ��
	int GetEmptyLocationCount() CONST;

	// �ж��ж��ٸ���ɫ��ɾ��״̬
	UINT GetCount_By_DeleteStatus() CONST;

	// �Ƿ��вֿ��
	BOOL IsExistWarehouse_By_LoginPlayer(_Out_opt_ CLoginPlayer* pLoginPlayer) CONST;

	// ����ֿ��ģʽ
	BOOL ToWarehouseMode() CONST;

	//�Ƿ����ʼĺ�
	BOOL IsExistMail_By_LoginPlayer(_Out_opt_ CLoginPlayer* pLoginPlayer) CONST;

	// �����ʼ�ģʽ
	BOOL ToMailMode() CONST;

	// ���������ģʽ
	BOOL ToSpecialMode() CONST;

	// ɾ����ɫ
	BOOL DeletePlayer_From_MainTask(_In_ CONST std::wstring& wsLoginPlayerName) CONST;

	// ɾ�����õĽ�ɫ
	BOOL DeletePlayer_From_OtherClasses() CONST;

	// д��ɾ����ɫ��־
	BOOL WriteDeletePlayerLog(_In_ CONST std::wstring& wsPlayerName) CONST;

	// 50�����ù���
	VOID ActionForUnExistLevelTick(_In_ CONST std::vector<CLoginPlayer>& VecLoginPlayer) CONST;

	// ��ȡ����ģʽ���õĽ�ɫ�Ĵ���
	DWORD GetPlayerAreaByCleanMode() CONST;

	// 50���ٻ�ģʽ��, �Ƿ����н�ɫ�� < 50 && > 16
	BOOL IsExistPlayerInLevel50Mode() CONST;

	VOID RemoveClasses_By_Mode(_In_ em_Player_Classes emPlayerClasses, _In_ _Out_opt_ std::vector<CLoginPlayer>& VecLoginPlayer, std::function<em_Player_Classes(DWORD)> GetCreatePlayerClassesPtr) CONST;

	//��ɫ��2��
	VOID RemoveClasses_By_Mode2(_In_ em_Player_Classes emPlayerClasses, _In_ _Out_opt_ std::vector<CLoginPlayer>& VecLoginPlayer, std::function<em_Player_Classes(DWORD)> GetCreatePlayerClassesPtr) CONST;

	// ��ɫ��
	CONST std::vector<em_Player_Classes>& GetPlayerPool() CONST;

	// ��ɫ��2
	CONST std::vector<em_Player_Classes>& GetPlayerPool2() CONST;
private:
	std::function<BOOL(VOID)> _IsDeletePlayerPtr;
	CBnsGameFun& RefBnsGameFun;
	DSIABLE_COPY_AND_ASSIGN(CTaskUpgradeMgr)
};


#endif