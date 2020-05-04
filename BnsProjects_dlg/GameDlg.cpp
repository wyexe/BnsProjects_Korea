// GameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BnsProjects_dlg.h"
#include "GameDlg.h"
#include "afxdialogex.h"
#include "../BnsProjects/GameFunction.h"
#include "../BnsProjects/ObjectFunction.h"
#include <algorithm>
#include "../BnsProjects/Task.h"
#include "RecordPointDlg.h"
#include "GameCmdDlg.h"
#include "../BnsProjects/BnsScript.h"
#include "../BnsProjects/PlayerKill.h"
#include "../BnsProjects/GrammarFunction.h"
#include <afxinet.h>
#include "../BnsProjects/MiniDump.h"
#include <MyTools/CLSearchBase.h>
#include <MyTools/CLFile.h>
#include <MyTools/CLProcess.h>
#include <MyTools/CmdLog.h>
#include <MyTools/TimeTick.h>
#include <PersonFunction.h>
#include <ResExtend.h>
#include <Task.h>
#include <BnsGameFun.h>
#include <MonsterExtend.h>
#include <FarmSkill.h>
#include <BagAction.h>
#include <PersonFunction.h>
#include <SomeFun.h>
#include <Person.h>
#include <Monster.h>
#include <Npc.h>
#include <DropItem.h>
#include <Corpse.h>
#include <GameUI.h>
#include <LoginPlayer.h>
#include <TeamMember.h>
#include <Player.h>
#include <UiSkill.h>
#include <FarmTask.h>
#include <LoginPlayerExtend.h>
#include <PlayerExtend.h>
#include <BnsConfig.h>
#include "BnsFilter.h"
#include <MailAction.h>
#include <set>
#include <map>
#include <thread>
#include <BnsHook.h>
#include <GameSend.h>
#include <NpcExtend.h>
#include <TeamMemberExtend.h>
#include <DxOptimize.h>
#include <Cat.h>
#include <CatExpend.h>
#include <Skill.h>
#include <Farm.h>
#include <BaiXianPracticeFieldServices.h>
#include <FarmSpecialTask.h>
#include <Language.h>
#include <MyTools/Log.h>
#include <MyTools/LdrHeader.h>
#include <MyTools/CLEchoException.h>
#include <GameExpression.h>
#include <BagFunction.h>
#include <FarmBoss.h>
#include "FunctionTest.h"
#include "DailyTask_Skill.h"
#include "TaskUpgradeMgr.h"
#include "DailyTask_SnowHole.h"
#include "DailyTask_HukryongPrison.h"

// CGameDlg dialog
#pragma comment(linker,"/STACK:10240000,1024000")
#define _SELF L"GameDlg.cpp"

std::vector<CWnd*> vDlg;



IMPLEMENT_DYNAMIC(CGameDlg, CDialogEx)

CGameDlg::CGameDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGameDlg::IDD, pParent)
{

}

CGameDlg::~CGameDlg()
{
}

void CGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGameDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SUMIT, &CGameDlg::OnBnClickedBtnSumit)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CGameDlg::OnNMClickList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CGameDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CGameDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CGameDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CGameDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CGameDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CGameDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CGameDlg::OnBnClickedButton7)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CGameDlg::OnNMDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON8, &CGameDlg::OnBnClickedButton8)
END_MESSAGE_MAP()

VOID TryReadLocalAccountFile(_Out_ std::wstring& wsAccountName)
{
	struct LoginAccountContent
	{
		std::wstring wsPlayerName;
		std::wstring wsAccountName;
		std::wstring wsAccountPass;
		std::wstring wsKey;
	};

	if (!MyTools::CLPublic::FileExist(L"C:\\BnsKor\\Account.txt"))
		return;

	std::wstring wsFileContent;
	MyTools::CLFile::ReadUnicodeFile(L"C:\\BnsKor\\Account.txt", wsFileContent);
	
	std::vector<std::wstring> VecFileContent;
	MyTools::CCharacter::Split(wsFileContent, L"\r\n", VecFileContent, Split_Option_RemoveEmptyEntries);

	std::vector<LoginAccountContent> VecAccount;
	for (CONST auto& itm : VecFileContent)
	{
		std::vector<std::wstring> VecAccountText;
		if (MyTools::CCharacter::Split(itm, L",", VecAccountText, Split_Option_RemoveEmptyEntries) != 4)
		{
			LOG_MSG_CF(L"Invalid Account Format Text:%s", itm.c_str());
			return;
		}

		LoginAccountContent AccountContent_;
		AccountContent_.wsPlayerName = VecAccountText.at(0);
		AccountContent_.wsAccountName = VecAccountText.at(1);
		AccountContent_.wsAccountPass = VecAccountText.at(2);
		AccountContent_.wsKey = VecAccountText.at(3);
		VecAccount.push_back(std::move(AccountContent_));
	}

	auto wsPlayerName = CPerson::GetInstance().GetName();
	auto Element = MyTools::CLPublic::Vec_find_if_Const(VecAccount, [wsPlayerName](CONST LoginAccountContent& itm) { return itm.wsPlayerName == wsPlayerName; });
	if (Element == nullptr)
	{
		LOG_MSG_CF(L"UnExist PlayerName[%s] in Account.txt", wsPlayerName.c_str());
		StopGame;
		return;
	}

	wsAccountName = Element->wsAccountName;
}

// CGameDlg message handlers
BOOL CGameDlg::OnInitDialog()
{
	auto fnAddCombox = [this]
	{
		CComboBox * CCB = (CComboBox *)GetDlgItem(IDC_CCB_CMD);
		CCB->InsertString(0, L"遍历怪物");
		CCB->InsertString(1, L"遍历NPC");
		CCB->InsertString(2, L"遍历技能");
		CCB->InsertString(3, L"遍历背包");
		CCB->InsertString(4, L"遍历玩家");
		CCB->InsertString(5, L"地面物品");
		CCB->InsertString(6, L"支线任务");
		CCB->InsertString(7, L"人物状态");
		CCB->InsertString(8, L"UI遍历");
		CCB->InsertString(9, L"遍历尸体");
		CCB->InsertString(10, L"遍历所有对象");
		CCB->InsertString(11, L"遍历装备");
		CCB->InsertString(12, L"遍历仓库");
		CCB->InsertString(13, L"城市Id");
		CCB->InsertString(14, L"人物BUFF");
		CCB->InsertString(15, L"僵尸尸体");
		CCB->InsertString(16, L"聊天框");
		CCB->InsertString(17, L"队伍遍历");
		CCB->InsertString(18, L"登录角色遍历");
		CCB->InsertString(19, L"UI技能遍历");
		CCB->InsertString(20, L"武器宝石遍历");
		CCB->InsertString(21, L"冷却中的隐藏UI技能");
		CCB->InsertString(22, L"衣柜遍历");
		CCB->InsertString(23, L"未开启礼包遍历");
		CCB->InsertString(24, L"已开启礼包遍历");
		CCB->InsertString(25, L"猫遍历");
		CCB->InsertString(26, L"怪物BUFF");
		CCB->InsertString(27, L"Mail");
		CCB->InsertString(28, L"队伍竞拍");
	};

	MyTools::CLGrammar::AddTranList(L"SetSpeed", CGrammarFunction::SetSpeed);
	MyTools::CLGrammar::AddTranList(L"HookMainTask", CGrammarFunction::HookMainTask);
	MyTools::CLGrammar::AddTranList(L"HookBLTask", CGrammarFunction::HookBLTask);
	MyTools::CLGrammar::AddTranList(L"GetCityId", CGrammarFunction::GetCityId);
	MyTools::CLGrammar::AddTranList(L"TakeItem", CGrammarFunction::TakeItem);
	MyTools::CLGrammar::AddTranList(L"SumitMainTask", CGrammarFunction::SumitMainTask);
	MyTools::CLGrammar::AddTranList(L"PickBLTask", CGrammarFunction::PickBLTask);
	MyTools::CLGrammar::AddTranList(L"SetAccountName", CGrammarFunction::SetAccountName);
	MyTools::CLGrammar::AddTranList(L"SearchMemBase", CGrammarFunction::SearchMemBase);
	MyTools::CLGrammar::AddTranList(L"HookTelport", CGrammarFunction::HookTelport);
	MyTools::CLGrammar::AddTranList(L"Telport", CGrammarFunction::Telport);
	MyTools::CLGrammar::AddTranList(L"HookMonsterSkill", CGrammarFunction::HookMonsterSkill);
	MyTools::CLGrammar::AddTranList(L"DeletePlayer", CGrammarFunction::DeletePlayer);
	MyTools::CLGrammar::AddTranList(L"HookAddSkill", CGrammarFunction::HookAddSkill);
	MyTools::CLGrammar::AddTranList(L"CreatePlayer", CGrammarFunction::CreatePlayer);
	MyTools::CLGrammar::AddTranList(L"HookShopStore", CGrammarFunction::HookShopStore);
	MyTools::CLGrammar::AddTranList(L"HookEquiEvolution", CGrammarFunction::HookEquiEvolution);
	
	CListCtrl * m_List = ((CListCtrl *)GetDlgItem(IDC_LIST1));
	if (m_List != NULL)
		m_List->SetExtendedStyle(m_List->GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	fnAddCombox();

	auto wsPlayerName = CPerson::GetInstance().GetName();
	std::wstring wsAccountName;
	TryReadLocalAccountFile(wsAccountName);
	if (wsPlayerName.empty())
	{
		MyTools::CLog::GetInstance().SetClientName(wsPlayerName == L"" ? L"TestClient" : wsPlayerName, L"C:\\Bns\\Log\\");
		MyTools::CCmdLog::GetInstance().Run(wsPlayerName, CGameExprFun::GetInstance().GetVec());
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"PlayerName=%s", wsPlayerName.c_str());

		std::wstring wsDlgText = wsPlayerName + L"-Injectordlg";
		this->SetWindowTextW(wsDlgText.c_str());
	}
	else
	{
		std::wstring wsDlgText = wsAccountName + L"-Injectordlg";
		this->SetWindowTextW(wsDlgText.c_str());
		
		MyTools::CLog::GetInstance().SetClientName(wsAccountName, L"C:\\Bns\\Log\\");
		MyTools::CCmdLog::GetInstance().Run(L"Bns", CGameExprFun::GetInstance().GetVec());
		LOG_C(MyTools::CLog::em_Log_Type_Custome, L"AccountName=%s", wsAccountName.c_str());
	}
	
	CSomeFun::GetInstance().InitDLL();
	CGameFunction::GetInstance().InitTimer();

	if (!wsAccountName.empty())
	{
		::SetWindowTextW(CGameVariable::GetInstance().GetAccShareInfo()->hGameWnd, wsAccountName.c_str());
		MyTools::CCharacter::wstrcpy_my(CGameVariable::GetInstance().GetAccShareInfo()->MyAcount_Info.szUserName, wsAccountName.c_str());
	}

	RegDumpFunction();
	MyTools::CCharacter::wstrcpy_my(g_wszDumpPath, L"C:\\", _countof(g_wszDumpPath) - 1);
	vDlg.push_back(this);
	return TRUE;
}

void CGameDlg::OnBnClickedBtnSumit()
{
	StartGame;
	CComboBox * CCB = (CComboBox*)GetDlgItem(IDC_CCB_CMD);
	CEdit * pEdit = (CEdit *)GetDlgItem(IDC_EDIT_COPY);
	CListCtrl * m_List = (CListCtrl *)GetDlgItem(IDC_LIST1);
	if (CCB == NULL || m_List == NULL || pEdit == NULL)
		return;
	
	auto fnDeleteListData = [&m_List]
	{
		m_List->DeleteAllItems();
		int nColumnCount = m_List->GetHeaderCtrl()->GetItemCount();
		for (int i = 0; i < nColumnCount; i++)
		{
			m_List->DeleteColumn(0);
		}
	};

	// Delete All Item
	fnDeleteListData();

	int nIndex = CCB->GetCurSel();
	if (nIndex == 0)
	{
		m_List->InsertColumn(0, L"Base", LVCFMT_LEFT, 80);
		m_List->InsertColumn(1, L"Type", LVCFMT_LEFT, 50);
		m_List->InsertColumn(2, L"ID", LVCFMT_LEFT, 50);
		m_List->InsertColumn(3, L"Name", LVCFMT_LEFT, 100);
		m_List->InsertColumn(4, L"Point", LVCFMT_LEFT, 150);
		m_List->InsertColumn(5, L"HP/MAXHP", LVCFMT_LEFT, 100);
		m_List->InsertColumn(6, L"Dis", LVCFMT_LEFT, 50);
		m_List->InsertColumn(7, L"Tar", LVCFMT_LEFT, 80);
		m_List->InsertColumn(8, L"Name", LVCFMT_LEFT, 100);

		std::vector<CMonster> vlst;
		CObjectFunction::GetInstance().GetSortGameObjectList(vlst, CObjectFunction::em_GameObject_Type_Monster, CBaseObject::CreateFixPointInfo(-1));
		
		std::for_each(vlst.begin(), vlst.end(), [&m_List](CMonster& Monster){
			int nRow = m_List->InsertItem(m_List->GetItemCount(), MyTools::CCharacter::MakeFormatText(L"%X", Monster.GetBase()).c_str());

			m_List->SetItemText(nRow, 1, MyTools::CCharacter::MakeFormatText(L"%X", Monster.GetIdParm()).c_str());

			m_List->SetItemText(nRow, 2, MyTools::CCharacter::MakeFormatText(L"%X", Monster.GetId()).c_str());

			m_List->SetItemText(nRow, 3, Monster.GetName().c_str());

			Point Pt = Monster.GetPoint();
			m_List->SetItemText(nRow, 4, MyTools::CCharacter::MakeFormatText(L"%d,%d,%d", Pt.GetX(), Pt.GetY(), (int)Pt.Z).c_str());

			m_List->SetItemText(nRow, 5, MyTools::CCharacter::MakeFormatText(L"%d/%d", Monster.GetHp(), Monster.GetMaxHp()).c_str());

			m_List->SetItemText(nRow, 6, MyTools::CCharacter::MakeFormatText(L"%.1f", Monster.GetDis()).c_str());

			m_List->SetItemText(nRow, 7, MyTools::CCharacter::MakeFormatText(L"%X", Monster.GetTargetId()).c_str());

			m_List->SetItemText(nRow, 8, CLanguage::GetInstance().GetOtherText_By_LocalText(Monster.GetName()).c_str());
		});

	}
	else if (nIndex == 1)
	{
		m_List->InsertColumn(0, L"Base", LVCFMT_LEFT, 80);
		m_List->InsertColumn(1, L"Type", LVCFMT_LEFT, 50);
		m_List->InsertColumn(2, L"ID", LVCFMT_LEFT, 50);
		m_List->InsertColumn(3, L"Name", LVCFMT_LEFT, 100);
		m_List->InsertColumn(4, L"Point", LVCFMT_LEFT, 150);
		m_List->InsertColumn(5, L"HP/MAXHP", LVCFMT_LEFT, 100);
		m_List->InsertColumn(6, L"Dis", LVCFMT_LEFT, 50);
		m_List->InsertColumn(7, L"Name", LVCFMT_LEFT, 100);

		std::vector<CNpc> vlst;
		CObjectFunction::GetInstance().GetSortGameObjectList<CNpc>(vlst, CObjectFunction::em_GameObject_Type_Npc , CBaseObject::CreateFixPointInfo(100));

		std::for_each(vlst.begin(), vlst.end(), [&m_List](CNpc& Npc){
			int nRow = m_List->InsertItem(m_List->GetItemCount(), MyTools::CCharacter::MakeFormatText(L"%X", Npc.GetBase()).c_str());

			m_List->SetItemText(nRow, 1, MyTools::CCharacter::MakeFormatText(L"%X", Npc.GetIdParm()).c_str());

			m_List->SetItemText(nRow, 2, MyTools::CCharacter::MakeFormatText(L"%X", Npc.GetId()).c_str());

			m_List->SetItemText(nRow, 3, Npc.GetName().c_str());

			Point Pt = Npc.GetPoint();
			m_List->SetItemText(nRow, 4, MyTools::CCharacter::MakeFormatText(L"%d,%d,%d", Pt.GetX(), Pt.GetY(), (int)Pt.Z).c_str());

			m_List->SetItemText(nRow, 5, MyTools::CCharacter::MakeFormatText(L"%d/%d", Npc.GetHp(), Npc.GetMaxHp()).c_str());

			m_List->SetItemText(nRow, 6, MyTools::CCharacter::MakeFormatText(L"%.1f", Npc.GetDis()).c_str());

			m_List->SetItemText(nRow, 7, CLanguage::GetInstance().GetOtherText_By_LocalText(Npc.GetName()).c_str());
		});
	}
	else if (nIndex == 2)
	{
		m_List->InsertColumn(0, L"Base", LVCFMT_LEFT, 80);
		m_List->InsertColumn(1, L"ID", LVCFMT_LEFT, 50);
		m_List->InsertColumn(2, L"SkillParm", LVCFMT_LEFT, 50);
		m_List->InsertColumn(3, L"SkillObj", LVCFMT_LEFT, 100);
		m_List->InsertColumn(4, L"Name", LVCFMT_LEFT, 150);
		m_List->InsertColumn(5, L"Cooldown", LVCFMT_LEFT, 100);
		m_List->InsertColumn(6, L"Name", LVCFMT_LEFT, 100);
		
		std::vector<CSkill> vlst;
		CObjectFunction::GetInstance().GetSkillList(vlst);

		CString str;
		pEdit->GetWindowTextW(str);
		DWORD dwFilterId = wcstol(str.GetBuffer(), nullptr, 16);


		std::for_each(vlst.begin(), vlst.end(), [&m_List, &dwFilterId](CSkill& Skill){
			if (dwFilterId != NULL && Skill.GetId() != dwFilterId)
				return;

			int nRow = m_List->InsertItem(m_List->GetItemCount(), MyTools::CCharacter::MakeFormatText(L"%X", Skill.GetBase()).c_str());

			m_List->SetItemText(nRow, 1, MyTools::CCharacter::MakeFormatText(L"%X", Skill.GetId()).c_str());

			m_List->SetItemText(nRow, 2, L"UnKnow"/*MyTools::CCharacter::MakeFormatText(L"%X", Skill.GetSkillParm())*/);

			m_List->SetItemText(nRow, 3, L"UnKnow"/*MyTools::CCharacter::MakeFormatText(L"%X", Skill.GetSkillObj())*/);

			m_List->SetItemText(nRow, 4, Skill.GetName().c_str());

			m_List->SetItemText(nRow, 5, L"UnKnow"/*MyTools::CCharacter::MakeFormatText(L"%X", Skill.GetCooldown())*/);

			m_List->SetItemText(nRow, 6, CLanguage::GetInstance().GetOtherText_By_LocalText(Skill.GetName()).c_str());
		});
	}
	else if (nIndex == 3 || nIndex == 11 || nIndex == 12 || nIndex == 22)
	{
		m_List->InsertColumn(0, L"Base", LVCFMT_LEFT, 80);
		m_List->InsertColumn(1, L"Index", LVCFMT_LEFT, 50);
		m_List->InsertColumn(2, L"GID", LVCFMT_LEFT, 50);
		m_List->InsertColumn(3, L"Name", LVCFMT_LEFT, 100);
		m_List->InsertColumn(4, L"Color", LVCFMT_LEFT, 50);
		m_List->InsertColumn(5, L"Count", LVCFMT_LEFT, 50);
		m_List->InsertColumn(6, L"Type", LVCFMT_LEFT, 50);
		m_List->InsertColumn(7, L"TypeName", LVCFMT_LEFT, 100);
		m_List->InsertColumn(8, L"SID", LVCFMT_LEFT, 100);
		m_List->InsertColumn(9, L"Name", LVCFMT_LEFT, 100);

		std::vector<CBag> vlst;
		if (nIndex == 3)
			CObjectFunction::GetInstance().GetBagList(vlst, CObjectFunction::em_Bag_Type_Bag);
		else if (nIndex == 11)
			CObjectFunction::GetInstance().CObjectFunction::GetBagList(vlst, CObjectFunction::em_Bag_Type_Equi);
		else if (nIndex == 12)
			CObjectFunction::GetInstance().CObjectFunction::GetBagList(vlst, CObjectFunction::em_Bag_Type_Warehouse);
		else if (nIndex == 22)
			CObjectFunction::GetInstance().CObjectFunction::GetBagList(vlst, CObjectFunction::em_Bag_Type_Wardrobe);
		else
			return;
		
		std::for_each(vlst.begin(), vlst.end(), [&m_List](CBag& Bag){

			int nRow = m_List->InsertItem(m_List->GetItemCount(), MyTools::CCharacter::MakeFormatText(L"%X", Bag.GetBase()).c_str());

			m_List->SetItemText(nRow, 1, MyTools::CCharacter::MakeFormatText(L"%d", Bag.GetIndex()).c_str());

			m_List->SetItemText(nRow, 2, MyTools::CCharacter::MakeFormatText(L"%X", Bag.GetGId()).c_str());

			m_List->SetItemText(nRow, 3, Bag.GetName().c_str());

			m_List->SetItemText(nRow, 4, Bag.GetColorName().c_str());

			m_List->SetItemText(nRow, 5, MyTools::CCharacter::MakeFormatText(L"%d", Bag.GetCount()).c_str());

			m_List->SetItemText(nRow, 6, MyTools::CCharacter::MakeFormatText(L"%X", Bag.GetItemType_Dlg()).c_str());

			m_List->SetItemText(nRow, 7, Bag.GetTypeName_Dlg().c_str());

			m_List->SetItemText(nRow, 8, MyTools::CCharacter::MakeFormatText(L"%X", Bag.GetSId()).c_str());

			m_List->SetItemText(nRow, 9, CLanguage::GetInstance().GetOtherText_By_LocalText(Bag.GetName()).c_str());
		});
	}
	else if (nIndex == 4)
	{
		m_List->InsertColumn(0, L"Base", LVCFMT_LEFT, 80);
		m_List->InsertColumn(1, L"Type", LVCFMT_LEFT, 50);
		m_List->InsertColumn(2, L"ID", LVCFMT_LEFT, 50);
		m_List->InsertColumn(3, L"Name", LVCFMT_LEFT, 100);
		m_List->InsertColumn(4, L"Point", LVCFMT_LEFT, 150);
		m_List->InsertColumn(5, L"HP/MAXHP", LVCFMT_LEFT, 100);
		m_List->InsertColumn(6, L"Dis", LVCFMT_LEFT, 50);
		m_List->InsertColumn(7, L"Classes", LVCFMT_LEFT, 80);

		std::vector<CPlayer> vlst;
		CObjectFunction::GetInstance().GetSortGameObjectList(vlst, CObjectFunction::em_GameObject_Type_Player, CBaseObject::CreateFixPointInfo(100));

		std::for_each(vlst.begin(), vlst.end(), [&m_List](CPlayer& Player){
			int nRow = m_List->InsertItem(m_List->GetItemCount(), MyTools::CCharacter::MakeFormatText(L"%X", Player.GetBase()).c_str());

			m_List->SetItemText(nRow, 1, MyTools::CCharacter::MakeFormatText(L"%X", Player.GetIdParm()).c_str());

			m_List->SetItemText(nRow, 2, MyTools::CCharacter::MakeFormatText(L"%X", Player.GetId()).c_str());

			m_List->SetItemText(nRow, 3, Player.GetName().c_str());

			Point Pt = Player.GetPoint();
			m_List->SetItemText(nRow, 4, MyTools::CCharacter::MakeFormatText(L"%d,%d,%d", Pt.GetX(), Pt.GetY(), (int)Pt.Z).c_str());

			m_List->SetItemText(nRow, 5, MyTools::CCharacter::MakeFormatText(L"%d/%d", Player.GetHp(), Player.GetMaxHp()).c_str());

			m_List->SetItemText(nRow, 6, MyTools::CCharacter::MakeFormatText(L"%.1f", Player.GetDis()).c_str());

			m_List->SetItemText(nRow, 7, Player.GetClassesText().c_str());
		});
		LOG_CF_D( L"Person.MapId=%X", CPerson::GetInstance().GetMapId());
		
	}
	else if (nIndex == 5)
	{
		m_List->InsertColumn(0, L"Base", LVCFMT_LEFT, 80);
		m_List->InsertColumn(1, L"Type", LVCFMT_LEFT, 50);
		m_List->InsertColumn(2, L"ID", LVCFMT_LEFT, 50);
		m_List->InsertColumn(3, L"MonsterID", LVCFMT_LEFT, 50);
		m_List->InsertColumn(4, L"Point", LVCFMT_LEFT, 150);
		m_List->InsertColumn(5, L"Dis", LVCFMT_LEFT, 50);
		m_List->InsertColumn(6, L"+10", LVCFMT_LEFT, 50);
		m_List->InsertColumn(7, L"+14", LVCFMT_LEFT, 50);
		m_List->InsertColumn(8, L"+18", LVCFMT_LEFT, 50);

		std::vector<CDropItem> vlst;
		CResExtend::GetInstance().GetAroundDropItem(CBaseObject::CreateFixPointInfo(100), vlst);
		std::for_each(vlst.begin(), vlst.end(), [&m_List](CDropItem& DropItem){
			int nRow = m_List->InsertItem(m_List->GetItemCount(), MyTools::CCharacter::MakeFormatText(L"%X", DropItem.GetBase()).c_str());

			m_List->SetItemText(nRow, 1, MyTools::CCharacter::MakeFormatText(L"%X", DropItem.GetIdParm()).c_str());

			m_List->SetItemText(nRow, 2, MyTools::CCharacter::MakeFormatText(L"%X", DropItem.GetId()).c_str());

			m_List->SetItemText(nRow, 3, MyTools::CCharacter::MakeFormatText(L"%X", DropItem.GetMonsterId()).c_str());

			Point Pt = DropItem.GetPoint();
			m_List->SetItemText(nRow, 4, MyTools::CCharacter::MakeFormatText(L"%d,%d,%d", Pt.GetX(), Pt.GetY(), (int)Pt.Z).c_str());

			m_List->SetItemText(nRow, 5, MyTools::CCharacter::MakeFormatText(L"%.1f", DropItem.GetDis()).c_str());

			m_List->SetItemText(nRow, 6, MyTools::CCharacter::MakeFormatText(L"%X", ReadDWORD(ReadDWORD(DropItem.GetBase() + 0x14) + 0x10)).c_str());
			m_List->SetItemText(nRow, 7, MyTools::CCharacter::MakeFormatText(L"%X", ReadDWORD(ReadDWORD(DropItem.GetBase() + 0x14) + 0x14)).c_str());
			m_List->SetItemText(nRow, 8, MyTools::CCharacter::MakeFormatText(L"%X", ReadDWORD(ReadDWORD(DropItem.GetBase() + 0x14) + 0x18)).c_str());
		});
	}
	else if (nIndex == 6)
	{
		m_List->InsertColumn(0, L"TaskId", LVCFMT_LEFT, 50);
		m_List->InsertColumn(1, L"ScheduleId", LVCFMT_LEFT, 50);
		m_List->InsertColumn(2, L"TaskName", LVCFMT_LEFT, 250);
		m_List->InsertColumn(3, L"KillCount", LVCFMT_LEFT, 80);
		m_List->InsertColumn(4, L"TaskIdBase", LVCFMT_LEFT, 80);
		m_List->InsertColumn(5, L"ScheduleIdBase", LVCFMT_LEFT, 80);
		m_List->InsertColumn(6, L"主线", LVCFMT_LEFT, 80);

		std::vector<TaskInfo> vlst;
		CTask::GetInstance().GetTaskInfo(vlst);

		auto TaskParm = CTask::GetInstance().GetMianTaskInfo();

		std::for_each(vlst.begin(), vlst.end(), [&m_List, &TaskParm](TaskInfo& Ti){
			int nRow = m_List->InsertItem(m_List->GetItemCount(), MyTools::CCharacter::MakeFormatText(L"%X", Ti.GetTaskId()).c_str());

			m_List->SetItemText(nRow, 1, MyTools::CCharacter::MakeFormatText(L"%X", Ti.GetScheduleId()).c_str());

			m_List->SetItemText(nRow, 2, Ti.GetName().c_str());

			m_List->SetItemText(nRow, 3, MyTools::CCharacter::MakeFormatText(L"%X", CTask::GetInstance().QueryTaskScheduleCount(TaskInfoParm(Ti.GetTaskId(), Ti.GetScheduleId()))).c_str());

			m_List->SetItemText(nRow, 4, MyTools::CCharacter::MakeFormatText(L"%X", Ti.dwObj_TaskId).c_str());

			m_List->SetItemText(nRow, 5, MyTools::CCharacter::MakeFormatText(L"%X", Ti.dwObj_ScheduleId).c_str());

			m_List->SetItemText(nRow, 6, Ti.GetTaskId() == TaskParm.dwTaskId ? L"YES" : L"No");
		});
	}
	else if (nIndex == 7)
	{
		LOG_CF_D( L"dwPersonStatus=%X,GetFightStatus=%X", (DWORD)CPerson::GetInstance().GetPersonStatus(), (DWORD)CPerson::GetInstance().GetFightStatus());
	}
	else if (nIndex == 8)
	{
		m_List->InsertColumn(0, L"Base", LVCFMT_LEFT, 80);
		m_List->InsertColumn(1, L"Id", LVCFMT_LEFT, 50);
		m_List->InsertColumn(2, L"Obj", LVCFMT_LEFT, 80);
		m_List->InsertColumn(3, L"IsShow", LVCFMT_LEFT, 50);
		m_List->InsertColumn(4, L"Name", LVCFMT_LEFT, 150);

		CString str;
		pEdit->GetWindowTextW(str);
		
		std::map<std::wstring, CGameUi> vlst;
		CObjectFunction::GetInstance().GetGameUIList(vlst);

		std::for_each(vlst.begin(), vlst.end(), [&m_List, &str](std::map<std::wstring, CGameUi>::value_type& GUI){
			if (str == L"" || (str.Trim() == L"1" && GUI.second.IsShow()) || (str.Trim() == L"2" && !GUI.second.IsShow()) || (str != L"" && str != L"1" && str != L"2" && MyTools::CCharacter::wstrstr_my(GUI.second.GetName().c_str(), str.GetBuffer())))
			{
				int nRow = m_List->InsertItem(m_List->GetItemCount(), MyTools::CCharacter::MakeFormatText(L"%X", GUI.second.GetBase()).c_str());

				m_List->SetItemText(nRow, 1, MyTools::CCharacter::MakeFormatText(L"%X", GUI.second.GetId()).c_str());

				m_List->SetItemText(nRow, 2, MyTools::CCharacter::MakeFormatText(L"%X", GUI.second.GetObj()).c_str());

				m_List->SetItemText(nRow, 3, MyTools::CCharacter::MakeFormatText(L"%X", ReadDWORD(GUI.second.GetObj() + 0x38)).c_str());

				m_List->SetItemText(nRow, 4, GUI.second.GetName().c_str());
			}
		});
	}
	else if (nIndex == 9 || nIndex == 15)
	{
		m_List->InsertColumn(0, L"Base", LVCFMT_LEFT, 80);
		m_List->InsertColumn(1, L"Type", LVCFMT_LEFT, 50);
		m_List->InsertColumn(2, L"ID", LVCFMT_LEFT, 50);
		m_List->InsertColumn(3, L"Point", LVCFMT_LEFT, 150);
		m_List->InsertColumn(4, L"Dis", LVCFMT_LEFT, 50);
		m_List->InsertColumn(5, L"Name", LVCFMT_LEFT, 100);
		m_List->InsertColumn(6, L"+1C", LVCFMT_LEFT, 100);
		m_List->InsertColumn(7, L"Name", LVCFMT_LEFT, 100);

		std::vector<CCorpse> vlst;
		if (nIndex == 9)
			CObjectFunction::GetInstance().GetSortGameObjectList(vlst, CObjectFunction::em_GameObject_Type_Corpse, CBaseObject::CreateFixPointInfo(-1));
		else if (nIndex == 15)
			CObjectFunction::GetInstance().GetSortGameObjectList(vlst, CObjectFunction::em_GameObject_Type_MonsterCorpse, CBaseObject::CreateFixPointInfo(-1));

		std::for_each(vlst.begin(), vlst.end(), [&m_List](CCorpse& Corpse){
			int nRow = m_List->InsertItem(m_List->GetItemCount(), MyTools::CCharacter::MakeFormatText(L"%X", Corpse.GetBase()).c_str());

			m_List->SetItemText(nRow, 1, MyTools::CCharacter::MakeFormatText(L"%X", Corpse.GetIdParm()).c_str());

			m_List->SetItemText(nRow, 2, MyTools::CCharacter::MakeFormatText(L"%X", Corpse.GetId()).c_str());

			Point Pt = Corpse.GetPoint();
			m_List->SetItemText(nRow, 3, MyTools::CCharacter::MakeFormatText(L"%d,%d,%d", Pt.GetX(), Pt.GetY(), (int)Pt.Z).c_str());

			m_List->SetItemText(nRow, 4, MyTools::CCharacter::MakeFormatText(L"%.1f", Corpse.GetDis()).c_str());

			m_List->SetItemText(nRow, 5, Corpse.GetName().c_str());

			m_List->SetItemText(nRow, 6, MyTools::CCharacter::MakeFormatText(L"%X", ReadDWORD(Corpse.GetBase() + 任务尸体是否可捡偏移) & 0xFF).c_str());

			m_List->SetItemText(nRow, 7, CLanguage::GetInstance().GetOtherText_By_LocalText(Corpse.GetName()).c_str());
		});
	}
	else if (nIndex == 10)
	{
		m_List->InsertColumn(0, L"Base", LVCFMT_LEFT, 80);
		m_List->InsertColumn(1, L"Type", LVCFMT_LEFT, 50);
		m_List->InsertColumn(2, L"ID", LVCFMT_LEFT, 50);

		std::vector<CPlayer> vlst;
		CObjectFunction::GetInstance().GetSortGameObjectList(vlst, CObjectFunction::em_GameObject_Type_ALL, CBaseObject::CreateFixPointInfo(-1));
		sort(vlst.begin(), vlst.end(), [](CONST CPlayer& p1, CONST CPlayer& p2){ return p1.GetId() < p2.GetId(); });
		std::for_each(vlst.begin(), vlst.end(), [&m_List](CPlayer& TempObj){
			int nRow = m_List->InsertItem(m_List->GetItemCount(), MyTools::CCharacter::MakeFormatText(L"%X", TempObj.GetBase()).c_str());

			m_List->SetItemText(nRow, 1, MyTools::CCharacter::MakeFormatText(L"%X", TempObj.GetIdParm()).c_str());

			m_List->SetItemText(nRow, 2, MyTools::CCharacter::MakeFormatText(L"%X", TempObj.GetId()).c_str());
		});
		LOG_C_D(L"Object Count=%d", vlst.size());
	}
	else if (nIndex == 13)
	{
		pEdit->SetWindowText(MyTools::CCharacter::MakeFormatText(L"0x%X", CPersonFunction::GetInstance().GetCityId()).c_str());
	}
	else if (nIndex == 14 || nIndex == 26)
	{
		m_List->InsertColumn(0, L"Base", LVCFMT_LEFT, 80);
		m_List->InsertColumn(1, L"ID", LVCFMT_LEFT, 80);
		m_List->InsertColumn(2, L"TarId", LVCFMT_LEFT, 80);
		m_List->InsertColumn(3, L"Count", LVCFMT_LEFT, 80);

		std::vector<GAMEOBJ_BUFF> vlst;
		if (nIndex == 14)
			CObjectFunction::GetInstance().GetPersonBuffList(vlst);
		else if (nIndex == 26)
			CObjectFunction::GetInstance().GetMonsterBuffList(vlst);

		std::for_each(vlst.begin(), vlst.end(), [&m_List](GAMEOBJ_BUFF& GOB){
			int nRow = m_List->InsertItem(m_List->GetItemCount(), MyTools::CCharacter::MakeFormatText(L"%X", GOB.dwBuffBase).c_str());

			m_List->SetItemText(nRow, 1, MyTools::CCharacter::MakeFormatText(L"%X", GOB.dwBuffId).c_str());

			m_List->SetItemText(nRow, 2, MyTools::CCharacter::MakeFormatText(L"%X", GOB.dwTargetId).c_str());

			m_List->SetItemText(nRow, 3, MyTools::CCharacter::MakeFormatText(L"%X", GOB.dwBuffCount).c_str());
		});
	}
	else if (nIndex == 16)
	{
		m_List->InsertColumn(0, L"Base", LVCFMT_LEFT, 80);
		m_List->InsertColumn(1, L"Text", LVCFMT_LEFT, 200);

		std::vector<TalkMsg_Src> vlst;
		CObjectFunction::GetInstance().GetTalkMessage(vlst);

		std::for_each(vlst.begin(), vlst.end(), [&m_List](TalkMsg_Src& TalkMsg){
			int nRow = m_List->InsertItem(m_List->GetItemCount(), MyTools::CCharacter::MakeFormatText(L"%X", TalkMsg.dwBase).c_str());

			m_List->SetItemText(nRow, 1, TalkMsg.wsText.c_str());
		});
	}
	else if (nIndex == 17)
	{
		m_List->InsertColumn(0, L"Base", LVCFMT_LEFT, 80);
		m_List->InsertColumn(1, L"Name", LVCFMT_LEFT, 80);
		m_List->InsertColumn(2, L"Captain", LVCFMT_LEFT, 80);
		m_List->InsertColumn(3, L"IsCon", LVCFMT_LEFT, 80);
		m_List->InsertColumn(4, L"Channel", LVCFMT_LEFT, 80);
		m_List->InsertColumn(5, L"PerHp", LVCFMT_LEFT, 80);
		m_List->InsertColumn(6, L"Point", LVCFMT_LEFT, 80);
		m_List->InsertColumn(7, L"Dis", LVCFMT_LEFT, 80);
		m_List->InsertColumn(8, L"MapId", LVCFMT_LEFT, 80);

		std::vector<CTeamMember> vlst;
		CObjectFunction::GetInstance().GetTeamList(vlst);

		std::for_each(vlst.begin(), vlst.end(), [&m_List](CTeamMember& TeamMember){
			int nRow = m_List->InsertItem(m_List->GetItemCount(), MyTools::CCharacter::MakeFormatText(L"%X", TeamMember.GetBase()).c_str());

			m_List->SetItemText(nRow, 1, TeamMember.GetName().c_str());
			m_List->SetItemText(nRow, 2, TeamMember.IsCaptain() ? L"YES" : L"NO");
			m_List->SetItemText(nRow, 3, TeamMember.IsConnect() ? L"YES" : L"NO");
			m_List->SetItemText(nRow, 4, MyTools::CCharacter::MakeFormatText(L"%d", TeamMember.GetChannel()).c_str());

			DWORD dwHp = static_cast<DWORD>(static_cast<float>(ReadDWORD(TeamMember.GetBase() + 0x18)));
			DWORD dwMaxHp = static_cast<DWORD>(static_cast<float>(ReadDWORD(TeamMember.GetBase() + 0x30) + ReadDWORD(TeamMember.GetBase() + 0x38)));
			m_List->SetItemText(nRow, 5, MyTools::CCharacter::MakeFormatText(L"%d/%d", dwHp, dwMaxHp).c_str());

			Point Pt = TeamMember.GetPoint();
			m_List->SetItemText(nRow, 6, MyTools::CCharacter::MakeFormatText(L"%d,%d,%d", Pt.GetX(), Pt.GetY(), Pt.GetZ()).c_str());
			m_List->SetItemText(nRow, 7, MyTools::CCharacter::MakeFormatText(L"%.2f", TeamMember.GetDis()).c_str());
			m_List->SetItemText(nRow, 8, MyTools::CCharacter::MakeFormatText(L"%X", TeamMember.GetMapId()).c_str());
		});
	}
	else if (nIndex == 18)
	{
		m_List->InsertColumn(0, L"uIndex", LVCFMT_LEFT, 80);
		m_List->InsertColumn(1, L"Name", LVCFMT_LEFT, 80);
		m_List->InsertColumn(2, L"Level", LVCFMT_LEFT, 80);
		m_List->InsertColumn(3, L"IsDelete", LVCFMT_LEFT, 80);
		m_List->InsertColumn(4, L"Classes", LVCFMT_LEFT, 80);
		m_List->InsertColumn(5, L"ServerId", LVCFMT_LEFT, 80);

		std::vector<CLoginPlayer> vlst;
		CObjectFunction::GetInstance().GetLoginPlayerList(vlst);
		std::for_each(vlst.begin(), vlst.end(), [&m_List](CONST CLoginPlayer& LoginPlayer_){
			int nRow = m_List->InsertItem(m_List->GetItemCount(), MyTools::CCharacter::MakeFormatText(L"%d", LoginPlayer_.GetIndex()).c_str());

			m_List->SetItemText(nRow, 1, LoginPlayer_.GetName().c_str());
			m_List->SetItemText(nRow, 2, MyTools::CCharacter::MakeFormatText(L"%d", LoginPlayer_.GetLevel()).c_str());
			m_List->SetItemText(nRow, 3, LoginPlayer_.IsDeleteStatus() ? L"Yes" : L"No");

			PlayerclassesText PlayerclassesText_;
			BOOL bRetCode = CPlayer::FindPlayerclassesText(&PlayerclassesText_, [&LoginPlayer_](CONST PlayerclassesText& PlayerclassesText_){
				return PlayerclassesText_.emPlayerClasses == LoginPlayer_.GetLoginPlayerClasses();
			});

			m_List->SetItemText(nRow, 4, bRetCode ? PlayerclassesText_.wsText.c_str() : MyTools::CCharacter::MakeFormatText(L"未知职业:%X", LoginPlayer_.GetLoginPlayerClasses()).c_str());

			m_List->SetItemText(nRow, 5, MyTools::CCharacter::MakeFormatText(L"%X", LoginPlayer_.GetServerId()).c_str());
		});
	}
	else if (nIndex == 19)
	{
		m_List->InsertColumn(0, L"Base", LVCFMT_LEFT, 80);
		m_List->InsertColumn(1, L"Id", LVCFMT_LEFT, 80);
		m_List->InsertColumn(2, L"IsCool", LVCFMT_LEFT, 80);
		m_List->InsertColumn(3, L"IsActive", LVCFMT_LEFT, 80);
		m_List->InsertColumn(4, L"Index", LVCFMT_LEFT, 50);
		m_List->InsertColumn(5, L"KeyText", LVCFMT_LEFT, 150);
		m_List->InsertColumn(6, L"Name1", LVCFMT_LEFT, 150);
		m_List->InsertColumn(7, L"Name2", LVCFMT_LEFT, 150);
		
		std::vector<CUiSkill> vlst;
		CObjectFunction::GetInstance().GetUiSkillList(vlst);
		for (CONST auto& itm : vlst)
		{
			int nRow = m_List->InsertItem(m_List->GetItemCount(), MyTools::CCharacter::MakeFormatText(L"%X", itm.GetBase()).c_str());

			m_List->SetItemText(nRow, 1, MyTools::CCharacter::MakeFormatText(L"%X", itm.GetId()).c_str());
			m_List->SetItemText(nRow, 2, itm.IsCool() ? L"Yes" : L"No");
			m_List->SetItemText(nRow, 3, itm.IsActive() ? L"Yes" : L"No");
			m_List->SetItemText(nRow, 4, MyTools::CCharacter::MakeFormatText(L"%d", itm.GetIndex()).c_str());
			m_List->SetItemText(nRow, 5, itm.GetTextKeyByIndex().c_str());

			m_List->SetItemText(nRow, 6, itm.GetName().c_str());
			m_List->SetItemText(nRow, 7, CLanguage::GetInstance().GetOtherText_By_LocalText(itm.GetName()).c_str());
		}
	}
	else if (nIndex == 20)
	{
		m_List->InsertColumn(0, L"Obj", LVCFMT_LEFT, 80);
		m_List->InsertColumn(1, L"ID", LVCFMT_LEFT, 80);
		m_List->InsertColumn(2, L"Name", LVCFMT_LEFT, 150);
		m_List->InsertColumn(3, L"Name", LVCFMT_LEFT, 150);

		std::vector<CObjectFunction::WeasponGemInfo> vlst;
		CObjectFunction::GetInstance().GetWeasponGemInfoList(vlst);
		for (CONST auto& itm : vlst)
		{
			int nRow = m_List->InsertItem(m_List->GetItemCount(), MyTools::CCharacter::MakeFormatText(L"%X", itm.dwItemObj).c_str());
			m_List->SetItemText(nRow, 1, MyTools::CCharacter::MakeFormatText(L"%X", itm.dwItemId).c_str());
			m_List->SetItemText(nRow, 2, itm.wsGemName.c_str());
			m_List->SetItemText(nRow, 3, CLanguage::GetInstance().GetOtherText_By_LocalText(itm.wsGemName).c_str());
		}
	}
	else if (nIndex == 21)
	{
		m_List->InsertColumn(0, L"ID", LVCFMT_LEFT, 80);
		
		std::vector<DWORD> vlst;
		CObjectFunction::GetInstance().GetHideUiSkillList(vlst);

		for (auto& itm : vlst)
		{
			m_List->InsertItem(m_List->GetItemCount(), MyTools::CCharacter::MakeFormatText(L"%X", itm).c_str());
		}
		
	}
	else if (nIndex == 23)
	{
		m_List->InsertColumn(0, L"Base", LVCFMT_LEFT, 80);
		m_List->InsertColumn(1, L"Parm", LVCFMT_LEFT, 80);
		m_List->InsertColumn(2, L"Title", LVCFMT_LEFT, 250);
		m_List->InsertColumn(3, L"Item", LVCFMT_LEFT, 300);

		std::vector<UnOpenGiftInfo> vlst;
		CObjectFunction::GetInstance().GetUnopenGiftList(vlst);
		for (CONST auto& itm : vlst)
		{
			int nRow = m_List->InsertItem(m_List->GetItemCount(), MyTools::CCharacter::MakeFormatText(L"%X", itm.dwBase).c_str());
			m_List->SetItemText(nRow, 1, MyTools::CCharacter::MakeFormatText(L"%X", itm.dwParm).c_str());
			m_List->SetItemText(nRow, 2, itm.wsTitle.c_str());

			std::wstring wsText;
			for (CONST auto& txt : itm.vItem)
				wsText += txt.ItemName + L"+";

			m_List->SetItemText(nRow, 3, wsText.c_str());
		}
	}
	else if (nIndex == 24)
	{
		m_List->InsertColumn(0, L"Base", LVCFMT_LEFT, 80);
		m_List->InsertColumn(1, L"Parm", LVCFMT_LEFT, 80);
		m_List->InsertColumn(2, L"Title", LVCFMT_LEFT, 250);
		m_List->InsertColumn(3, L"Item", LVCFMT_LEFT, 300);

		std::vector<OpenGiftInfo> vlst;
		CObjectFunction::GetInstance().GetGiftList(vlst);
		for (CONST auto& itm : vlst)
		{
			int nRow = m_List->InsertItem(m_List->GetItemCount(), MyTools::CCharacter::MakeFormatText(L"%X", itm.dwBase).c_str());
			m_List->SetItemText(nRow, 1, MyTools::CCharacter::MakeFormatText(L"%X", itm.dwParm).c_str());
			m_List->SetItemText(nRow, 2, itm.wsTitle.c_str());

			std::wstring wsText;
			for (CONST auto& txt : itm.vItem)
				wsText += txt.ItemName + L"+";

			m_List->SetItemText(nRow, 3, wsText.c_str());
		}
	}
	else if (nIndex == 25)
	{
		m_List->InsertColumn(0, L"Base", LVCFMT_LEFT, 80);
		m_List->InsertColumn(1, L"Type", LVCFMT_LEFT, 50);
		m_List->InsertColumn(2, L"ID", LVCFMT_LEFT, 50);
		m_List->InsertColumn(3, L"Name", LVCFMT_LEFT, 100);
		m_List->InsertColumn(4, L"Point", LVCFMT_LEFT, 150);
		m_List->InsertColumn(5, L"HP/MAXHP", LVCFMT_LEFT, 100);
		m_List->InsertColumn(6, L"Dis", LVCFMT_LEFT, 50);
		m_List->InsertColumn(7, L"MasterId", LVCFMT_LEFT, 80);
		m_List->InsertColumn(8, L"TarId", LVCFMT_LEFT, 80);

		std::vector<CCat> vlst;
		CObjectFunction::GetInstance().GetSortGameObjectList(vlst, CObjectFunction::em_GameObject_Type_Pet, CBaseObject::CreateFixPointInfo(100));

		std::for_each(vlst.begin(), vlst.end(), [&m_List](CCat& Monster){
			int nRow = m_List->InsertItem(m_List->GetItemCount(), MyTools::CCharacter::MakeFormatText(L"%X", Monster.GetBase()).c_str());

			m_List->SetItemText(nRow, 1, MyTools::CCharacter::MakeFormatText(L"%X", Monster.GetIdParm()).c_str());

			m_List->SetItemText(nRow, 2, MyTools::CCharacter::MakeFormatText(L"%X", Monster.GetId()).c_str());

			m_List->SetItemText(nRow, 3, Monster.GetName().c_str());

			Point Pt = Monster.GetPoint();
			m_List->SetItemText(nRow, 4, MyTools::CCharacter::MakeFormatText(L"%d,%d,%d", Pt.GetX(), Pt.GetY(), (int)Pt.Z).c_str());

			m_List->SetItemText(nRow, 5, MyTools::CCharacter::MakeFormatText(L"%d/%d", Monster.GetHp(), Monster.GetMaxHp()).c_str());

			m_List->SetItemText(nRow, 6, MyTools::CCharacter::MakeFormatText(L"%.1f", Monster.GetDis()).c_str());

			m_List->SetItemText(nRow, 7, MyTools::CCharacter::MakeFormatText(L"%X", Monster.GetMasterId()).c_str());

			m_List->SetItemText(nRow, 8, MyTools::CCharacter::MakeFormatText(L"%X", Monster.GetTargetId()).c_str());
		});
	}
	else if (nIndex == 27)
	{
		m_List->InsertColumn(0, L"Index", LVCFMT_LEFT, 80);
		m_List->InsertColumn(1, L"Title", LVCFMT_LEFT, 50);

		DWORD dwMailCount = CMailAction::GetInstance().GetMailCount();
		for (DWORD i = 0; i < dwMailCount; ++i)
		{
			std::wstring wsTitle;
			if (!CMailAction::GetInstance().GetMailTitle(i, wsTitle))
				continue;

			int nRow = m_List->InsertItem(m_List->GetItemCount(), MyTools::CCharacter::MakeFormatText(L"%X", i).c_str());

			m_List->SetItemText(nRow, 1, wsTitle.c_str());
		}
	}
	else if (nIndex == 28)
	{
		CObjectFunction::GetInstance().Action_By_ShowGameUi_Name(UI_TEXT_PARTYAUCTION, [&m_List](CONST CGameUi& PartyAuctionPanel)
		{
			m_List->InsertColumn(0, L"Base", LVCFMT_LEFT, 80);
			m_List->InsertColumn(1, L"Index", LVCFMT_LEFT, 50);
			m_List->InsertColumn(2, L"ID", LVCFMT_LEFT, 50);
			m_List->InsertColumn(3, L"Name", LVCFMT_LEFT, 50);

			AuctionContent AuctionContent_;
			if (!CObjectFunction::GetInstance().GetAuctionContent(PartyAuctionPanel, AuctionContent_))
				return;

			for (CONST auto& itm : AuctionContent_.VecAuctionItem)
			{
				int nRow = m_List->InsertItem(m_List->GetItemCount(), MyTools::CCharacter::MakeFormatText(L"%X", itm.dwBase).c_str());

				m_List->SetItemText(nRow, 1, MyTools::CCharacter::MakeFormatText(L"%d", AuctionContent_.Index).c_str());

				m_List->SetItemText(nRow, 2, MyTools::CCharacter::MakeFormatText(L"%X", itm.dwItemId).c_str());

				m_List->SetItemText(nRow, 3, itm.wsItemName.c_str());
			}
		});
	}
}


void CGameDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1 && pNMListView->iSubItem != -1)
	{
		CListCtrl * m_list = (CListCtrl *)GetDlgItem(IDC_LIST1);
		CString str = m_list->GetItemText(pNMListView->iItem, pNMListView->iSubItem);
		((CEdit*)GetDlgItem(IDC_EDIT_COPY))->SetWindowTextW(str);
	}
	*pResult = 0;
}


void CGameDlg::OnBnClickedButton1()
{
	CRecordPointDlg dlg;
	dlg.DoModal();
}


void CGameDlg::OnBnClickedButton2()
{
	CGameCmdDlg dlg;
	dlg.DoModal();
}

HANDLE hExcuteScriptThread = NULL;
unsigned WINAPI _ExcuteScriptThread(LPVOID lpParm)
{
	while (GameRun)
	{
		CBnsScript BnsScript;
		WCHAR wszScriptPath[MAX_PATH] = { L"C:\\Bns\\" };

		TaskInfoParm MainTaskInfo = CTask::GetInstance().GetMianTaskInfo();

		lstrcatW(wszScriptPath, MyTools::CCharacter::MakeFormatText(L"%X", MainTaskInfo.dwTaskId).c_str());
		lstrcatW(wszScriptPath, L".inf");

		if (!BnsScript.Read(CBnsScript::em_ReadScript_Type_FilePath, wszScriptPath))
		{
			LOG_MSG_CF(L"解析脚本:%s失败!", wszScriptPath);
			hExcuteScriptThread = NULL;
			return 0;
		}

		while (GameRun)
		{
			WCHAR wszScriptFunName[32] = { L"Task_" };
			TaskInfoParm tmpTaskInfo = CTask::GetInstance().GetMianTaskInfo();

			if (tmpTaskInfo.dwTaskId != MainTaskInfo.dwTaskId)
			{
				LOG_CF_D( L"主线Id已经变了, break;");
				break;
			}

			lstrcatW(wszScriptFunName, MyTools::CCharacter::MakeFormatText(L"%X", tmpTaskInfo.dwScheduleId).c_str());
			LOG_CF_D( L"执行函数:%s", wszScriptFunName);
			BOOL bRetCode = BnsScript.Excute(wszScriptFunName);
			if (EMPTY_PERSONHP)
			{
				LOG_CF_D( L"人物挂了,复活!");
				CPersonFunction::GetInstance().Resurrect();
				LOG_CF_D( L"复活完毕!");
				continue;
			}

			if (!bRetCode)
			{
				LOG_CF_D( L"执行函数:%s,返回0,break;", wszScriptFunName);
				break;
			}
			BnsSleep(500);
		}
		BnsSleep(100);
	}
	CBnsScript::ReleaseBnsScript();
	LOG_CF_D(L"_ExcuteScriptThread!");
	hExcuteScriptThread = NULL;
	return 0;
}
HANDLE hTestScriptThread = NULL;
void CGameDlg::OnBnClickedButton3()
{
	if (hTestScriptThread != NULL)
	{
		AfxMessageBox(L"我就知道, 你又想坑我!");
		return;
	}
	if (hExcuteScriptThread == NULL)
	{
		StartGame;
		hExcuteScriptThread = cbBEGINTHREADEX(NULL, NULL, _ExcuteScriptThread, this, NULL, NULL);
	}
	else
	{
		StopGame;
		WaitForSingleObject(hExcuteScriptThread, INFINITE);
		LOG_CF_D( L"停止挂机");
	}
}

void CGameDlg::OnOK()
{
	
}

void WriteNameToFile()
{
	DWORD dwMaxAddrCount = ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(名字返回基址) + 名字返回偏移1) + 0xC));
	// dd [[[[0x12B1ED8]+30F0]+0C]+10]+38*9A1
	DWORD dwMaxAddr = ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(名字返回基址) + 名字返回偏移1) + 0xC) + 0x10) + 0x38 * (dwMaxAddrCount - 1);
	DWORD dwMaxID = ReadDWORD(dwMaxAddr + 0x8);

	WCHAR* p = new WCHAR[2048];
	ZeroMemory(p, 2048 * sizeof(WCHAR));

	auto fnGetName = [](LPCWSTR pwszText)
	{
		std::vector<std::wstring> vParm;
		MyTools::CCharacter::Split(pwszText, L".", vParm, Split_Option_RemoveEmptyEntries | Split_Option_KeepOnly);
		if (vParm.size() == 0)
			return std::wstring(pwszText);
		return std::wstring(vParm.at(vParm.size() - 1));
	};

	LOG_CF_D( L"dwMaxID=%d", dwMaxID);
	int nNameLength = 1;
	for (DWORD i = 0; i < dwMaxID; ++i)
	{
		DWORD dwAddr = CBaseObject::GetObjectNameAddr(i, 0);
		if (dwAddr == NULL)
		{
			//Log(LOG_LEVEL_EXCEPTION, L"ID=%X, dwAddr=NULL", i); 
			continue;
		}

		DWORD dwNameAddr = ReadDWORD(dwAddr + 0x18);
		LPCWSTR pwszSourceName = reinterpret_cast<LPCWSTR>(ReadDWORD(dwAddr + 0x10));
		if (dwNameAddr != NULL && ReadDWORD(dwNameAddr) != NULL && pwszSourceName != NULL && ReadDWORD((DWORD)pwszSourceName) != NULL)
		{
			LPCWSTR pwszKoreaName = reinterpret_cast<LPCWSTR>(dwNameAddr);
			//LOG_CF_D( L"dwNameAddr=%X, Name=%s, pwszSourceName=%s", dwAddr, pwszKoreaName, pwszSourceName);

			if (wcslen(pwszSourceName) > 1024 || wcslen(pwszKoreaName) > 1024)
				continue;

			//if (CCharacter::wstrcmp_my(pwszKoreaName,L"독초거사"))
			//{
			//	LOG_CF_D( L"dwAddr=%X, pwszKoreaName=%s, pwszSourceName=%s, i=%X", dwAddr, pwszKoreaName, pwszSourceName, i);
			//}
			// KoreaName, Name
			swprintf_s(p, nNameLength * 2048 - 1, L"%s--%s", pwszKoreaName, pwszSourceName);
			MyTools::CLFile::AppendUnicodeFile(L"C:\\1.txt", std::wstring(p));
			
		}
		//else
			//Log(LOG_LEVEL_EXCEPTION, L"ID=%X,dwNameAddr=%X, Name=NULL", i, dwNameAddr);
	}

	delete[] p;
}

BOOL ContainTrubidGroud(_In_ CONST std::vector<Point>& VecPoint, _In_ CONST Point& ComprPoint, _In_ float fMaxDis, _Out_opt_ Point* pTurbidGroundPoint = nullptr)
{
	return MyTools::CLPublic::Vec_find_if_Const(VecPoint, pTurbidGroundPoint, [ComprPoint, fMaxDis](CONST Point& Pt) { return CBaseObject::Get3DDis(ComprPoint, Pt) < fMaxDis; });
}

BOOL FindSafePath(_In_ CONST std::vector<Point>& VecTurbiGround_, _In_ CONST Point& TarPoint, _Out_ std::vector<Point>& VecRetPoint)
{
	auto VecTurbiGround = VecTurbiGround_;
	auto fnRemovePointPtr = [&VecTurbiGround](_In_ CONST Point& FixPoint)
	{
		MyTools::CLPublic::Vec_erase_if(VecTurbiGround, [FixPoint](CONST Point& TurbiGroudPoint) { return CBaseObject::Get3DDis(FixPoint, TurbiGroudPoint) <= 4; });
	};


	// 去掉自身坐标的浊气阵(不然就无法寻路)
	fnRemovePointPtr(CPerson::GetInstance().GetPoint());


	// 去掉BOSS坐标的浊气阵
	fnRemovePointPtr(TarPoint);

	enum { em_Max_FindSafePath_Range = 30, em_TurbiGroud_Dis = 5 };
	// 建立圆, 圆心=BOSS, 半径=30。 寻路不可超出这个范围 , 防止触碰到浊气阵, 将浊气阵的范围从4 -> 5
	Point CurPoint = CPerson::GetInstance().GetPoint();

	while (GameRun && !EMPTY_PERSONHP && CBaseObject::Get3DDis(TarPoint, CurPoint) > 2)
	{
		// 不用A* 也不用dijkstra, 直接用一个坐标搜索即可!
		std::vector<Point> VecDir;
		VecDir.push_back(Point(CurPoint.GetX() + 50, CurPoint.GetY(), CurPoint.GetZ()));
		VecDir.push_back(Point(CurPoint.GetX() - 50, CurPoint.GetY(), CurPoint.GetZ()));
		VecDir.push_back(Point(CurPoint.GetX(), CurPoint.GetY() + 50, CurPoint.GetZ()));
		VecDir.push_back(Point(CurPoint.GetX(), CurPoint.GetY() - 50, CurPoint.GetZ()));

		// Remove 
		MyTools::CLPublic::Vec_erase_if(VecDir, [TarPoint](CONST Point& Pt) { return CBaseObject::Get3DDis(Pt, TarPoint) >= em_Max_FindSafePath_Range; });
		MyTools::CLPublic::Vec_erase_if(VecDir, [VecRetPoint](CONST Point& Pt)
		{
			for (auto& itm : VecRetPoint)
			{
				if (itm.GetX() == Pt.GetX() && itm.GetY() == Pt.GetY())
					return TRUE;
			}

			return FALSE;
		});
		MyTools::CLPublic::Vec_erase_if(VecDir, [=](CONST Point& Pt)
		{
			return ContainTrubidGroud(VecTurbiGround, Pt, em_TurbiGroud_Dis);
		});
		if (VecDir.size() == 0)
		{
			LOG_MSG_CF(L"VecDir.size = 0, 寻路失败!");
			break;
		}

		std::sort(VecDir.begin(), VecDir.end(), [TarPoint](CONST Point& p1, CONST Point& p2) { return CBaseObject::Get3DDis(p1, TarPoint) < CBaseObject::Get3DDis(p2, TarPoint); });
		if (VecDir.size() == 0)
		{
			LOG_MSG_CF(L"VecDir.size = 0, 寻路失败!");
			break;
		}

		CurPoint = VecDir.at(0);
		VecRetPoint.push_back(CurPoint);
		LOG_CF_D(L"CurPoint.dis=%.2f", CBaseObject::Get3DDis(CurPoint, TarPoint));
	}
	return TRUE;
}



void CGameDlg::OnBnClickedButton4()
{
	//CLoginPlayerExtend::GetInstance().DeletePlayer(0);
	CSomeFun::GetInstance().SimlationKey(VK_ESCAPE, 10);
}

HANDLE hPkThread = NULL;
unsigned WINAPI _PkThread(LPVOID lpParm)
{
	//CPlayerKill Pk;
	//Pk.StartPK();
	
	//BnIDsNDuIS
	CPlayer Player;
	if (CPlayerExtend::GetInstance().FindPlayer_By_Name(L"BnIDsNDuIS", CBaseObject::CreateFixPointInfo(50), &Player))
	{
		while (GameRun)
		{
			BnsMove(Player.GetFacePoint(6), 1);
			BnsSleep(500);
		}
	}
	
	
	/*auto fnCome = [](_In_ CONST Point& TarPoint)
	{
		while (CPerson::GetInstance().GetDis(TarPoint) > 2)
			::Sleep(50);
	};

	BnsMove(Point(-20120, 16464, -92), 1);
	BnsSleep(2000);
	CBaseObject::SetAngle(Point(-19224, 15568, 236));
	BnsSleep(1000);
	CPersonFunction::GetInstance().Run(CPersonFunction::em_Run_Type::em_Run_Type_OnFloor);
	while (GameRun && CPerson::GetInstance().GetDis(Point(-19832, 16140, 60)) > 2)
		::Sleep(50);

	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([]{CGameSend::SimulationKey(VK_SPACE, TRUE); });
	while (GameRun && CPerson::GetInstance().Get2DDis(Point(-19420, 15764, 244)) > 2)
		::Sleep(50);

	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([]{CGameSend::SimulationKey(VK_SPACE, FALSE); });
	CSomeFun::GetInstance().SimlationKey('W');
	CSomeFun::GetInstance().SimlationKey(VK_SPACE);*/

	/*while (GameRun)
	{
		Point Pt = CPerson::GetInstance().GetPoint();
		LOG_CF_D( L"Point[%d,%d,%d, Value1=%X,Value2=%X]", Pt.GetX(), Pt.GetY(), Pt.GetZ(), ReadDWORD(GetPersonPointAddr + 0x10), ReadDWORD(GetMoveBaseAddr + 0x60));
		::Sleep(500);
	}*/

	//CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_BAIXIAN_STAGE_STEP, 0x3);
	//CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_BAIXIAN_TIMEOUT, 60 * 1000);
	//CBaiXianPracticeFieldServices::GetInstance().Begin();
	// 等待敌军现身!
	/*CPlayer Player;
	if (!CSomeFun::GetInstance().TimeOut_Condiction_GameRun(10 * 1000, [&Player]{
		return !CPlayerExtend::GetInstance().GetTopPraticeNpc_By_Dis(CBaseObject::CreateFixPointInfo(50.0f), &Player);
	}))
	{
		CSomeFun::GetInstance().SetSpeed(2.0f);
		LOG_CF_D( L"Start");
		if (CPerson::GetInstance().GetClasses() == em_Player_Classes::em_Player_Classes_Destroyer)
			CFarm::GetInstance().PK_Lux(Player, FALSE, []{return FALSE; });
		else if (CPerson::GetInstance().GetClasses() == em_Player_Classes::em_Player_Classes_Summoner)
			CFarm::GetInstance().Pk_Summoner(Player, FixPointInfo(Point(-54176, -48592, 6480), 8), []{return FALSE; });
	}*/

	// 等待战斗开始!
	
	
	LOG_CF_D(L"_PkThread");
	hPkThread = NULL;
	return 0;
}

void CGameDlg::OnBnClickedButton5()
{
	if (hPkThread == NULL)
	{
		StartGame;
		hPkThread = cbBEGINTHREADEX(NULL, NULL, _PkThread, this, NULL, NULL);
	}
	else
	{
		StopGame;
		if (WaitForSingleObject(hPkThread, 8000) == WAIT_TIMEOUT)
		{
			LOG_CF_D( L"强制结束!");
			::TerminateThread(hPkThread, 0);
			::CloseHandle(hPkThread);
		}
		hPkThread = NULL;
		LOG_CF_D( L"停止挂机");
	}
}


unsigned WINAPI _TestScriptThread(LPVOID lpParm)
{
	CBnsScript BnsScript;
	WCHAR wszScriptPath[64] = { L"C:\\Bns\\" };

	lstrcatW(wszScriptPath, L"Bns.inf");
	LOG_CF_D( L"读取文件:%s", wszScriptPath);
	if (!BnsScript.Read(CBnsScript::em_ReadScript_Type_FilePath, wszScriptPath))
	{
		LOG_MSG_CF(L"解析脚本:%s失败!", wszScriptPath);
		hExcuteScriptThread = NULL;
		return 0;
	}

	WCHAR wszScriptFunName[32] = { L"main" };
	LOG_CF_D( L"执行函数:%s", wszScriptFunName);
	BnsScript.Excute(wszScriptFunName);
	CPersonFunction::GetInstance().Resurrect();
	BnsScript.ReleaseBnsScript();
	hTestScriptThread = NULL;
	return 0;
}

void CGameDlg::OnBnClickedButton6()
{
	if (hExcuteScriptThread != NULL)
	{
		AfxMessageBox(L"我就知道, 你又想坑我!");
		return;
	}
	if (hTestScriptThread == NULL)
	{
		StartGame;
		hTestScriptThread = cbBEGINTHREADEX(NULL, NULL, _TestScriptThread, this, NULL, NULL);
	}
	else
	{
		StopGame;
		if (WaitForSingleObject(hTestScriptThread, 30 * 1000) == WAIT_TIMEOUT)
		{
			LOG_CF_D( L"强制结束!");
			::TerminateThread(hTestScriptThread, 0);
			::CloseHandle(hTestScriptThread);
		}
		hTestScriptThread = NULL;
		LOG_CF_D( L"停止挂机");
	}
}

HANDLE hFollowCaptainThread = NULL;
DWORD WINAPI _FollowCaptainThread(LPVOID lpParm)
{
	StartGame;
	CBnsScript BnsScript;
	CFarm::GetInstance().FollowCaptain();
	BnsScript.ReleaseBnsScript();
	hFollowCaptainThread = NULL;
	return 0;
}

void CGameDlg::OnBnClickedButton7()
{
	if (hFollowCaptainThread == NULL)
	{
		hFollowCaptainThread = cbBEGINTHREADEX(NULL, NULL, _FollowCaptainThread, this, NULL, NULL);
	}
	else
		StopGame;
}


void CGameDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	StopGame;
	if (hTestScriptThread != NULL)
	{
		if (WaitForSingleObject(hTestScriptThread, 3000) == WAIT_TIMEOUT)
		{
			LOG_CF_D( L"强制结束!");
			::TerminateThread(hTestScriptThread, 0);
			::CloseHandle(hTestScriptThread);
		}
		hTestScriptThread = NULL;
	}
	if (hExcuteScriptThread != NULL)
	{
		StopGame;
		if (WaitForSingleObject(hExcuteScriptThread, 3000) == WAIT_TIMEOUT)
		{
			LOG_CF_D( L"强制结束!");
			::TerminateThread(hExcuteScriptThread, 0);
			::CloseHandle(hExcuteScriptThread);
		}
		hExcuteScriptThread = NULL;
	}
	if (hFollowCaptainThread != NULL)
	{
		StopGame;
		if (WaitForSingleObject(hFollowCaptainThread, 3000) == WAIT_TIMEOUT)
		{
			LOG_CF_D( L"强制结束!");
			::TerminateThread(hFollowCaptainThread, 0);
			::CloseHandle(hFollowCaptainThread);
		}
		hFollowCaptainThread = NULL;
	}
	MyTools::CCmdLog::GetInstance().Stop();
	DeleteDlgList(this);
	CDialogEx::OnClose();
}

VOID DeleteDlgList(_In_ CWnd* dlg)
{
	auto itr = find_if(vDlg.begin(), vDlg.end(), [&dlg](CWnd* dlg_){
		return dlg->GetSafeHwnd() == dlg_->GetSafeHwnd();
	});

	if (itr != vDlg.end())
	{
		vDlg.erase(itr);
	}
}

void CGameDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	CComboBox * CCB = (CComboBox *)GetDlgItem(IDC_CCB_CMD);
	if (pNMListView->iItem != -1 && pNMListView->iSubItem != -1 && CCB->GetCurSel() == 0x6)
	{
		if (pNMListView->iSubItem == 0x0) // Column
		{
			CListCtrl * m_list = (CListCtrl *)GetDlgItem(IDC_LIST1);
			CString str = m_list->GetItemText(pNMListView->iItem, pNMListView->iSubItem);
			((CEdit*)GetDlgItem(IDC_EDIT_COPY))->SetWindowTextW(str);

			str += L".inf";

			WCHAR wszPath[MAX_PATH] = { L"C:\\Bns\\" };
			lstrcatW(wszPath, str.GetBuffer());
			if (MyTools::CLPublic::FileExit(wszPath))
			{
				STARTUPINFO si = { sizeof(STARTUPINFOW) };
				PROCESS_INFORMATION pi = { 0 };
				WCHAR wszCommonLine[MAX_PATH] = { L"notepad.exe " };
				lstrcatW(wszCommonLine, wszPath);
				::CreateProcessW(NULL, wszCommonLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
			}
		}
		
	}
	*pResult = 0;
}


void CGameDlg::OnBnClickedButton8()
{
	if (!GameRun)
	{
		//std::thread t([]
		//{
		//	StartGame;
		//	//CDailyTask_SnowHole SnowHole;
		//	//SnowHole.KillBoss();
		//	CDailyTask_HukryongPrison bukahole;
		//	bukahole.Run();
		//	//CFarm::GetInstance().ClearAroundMonster(FixPointInfo(CBaseObject::GetPersonPoint(), 50), []{return FALSE; });
		//});
		//t.detach();

		//std::thread t1([] 
		//{
		//	while (GameRun && !EMPTY_PERSONHP)
		//	{
		//		BnsSleep(100);
		//		// 是否要用倒地技能
		//		CFarmSkill::GetInstance().UseFloorSkill(CPerson::GetInstance().GetClasses());
		//	}
		//});
		//t1.detach();

			std::thread t2([]
			{
				StartGame;
				while (GameRun && !EMPTY_PERSONHP)
				{
					std::vector<CUiSkill> VecSkill;
					CObjectFunction::GetInstance().GetUiSkillList(VecSkill);
					for (auto& itm : VecSkill)
					{
						LOG_CF_D(L"SkillName[%s,%s], ID=%X, KeyText=%s,姿态=%d, IsCool=%d, IsActive=%d",
							itm.GetName().c_str(),
							CLanguage::GetInstance().GetOtherText_By_LocalText(itm.GetName()).c_str(),
							itm.GetId(),
							itm.GetTextKeyByIndex().c_str(),
							CPerson::GetInstance().GetFightPose(),
							itm.IsCool(),
							itm.IsActive());
					}
					BnsSleep(100);
				}
			});
			t2.detach();
		//}
		
	}
	else
	{
		StopGame;
	}
}
