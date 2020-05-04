#include "stdafx.h"
#include "Player.h"
#include <MyTools/Log.h>
#include "SomeFun.h"
#include "PersonFunction.h"
#include "Person.h"
#include "GameUI.h"
#include "ObjectFunction.h"
#include "GameSend.h"

#define _SELF L"Player.cpp"

CONST std::vector<PlayerclassesText> CPlayer::vPlayerclassesText = 
{
	{ em_Player_Classes_Destroyer,			L"��ʿ"},
	{ em_Player_Classes_Assassin,			L"�̿�" },
	{ em_Player_Classes_BladeMaster,		L"��ʿ" },
	{ em_Player_Classes_Summoner,			L"�ٻ�ʦ" },
	{ em_Player_Classes_ForceMaster,		L"����" },
	{ em_Player_Classes_KongFuMaster,		L"ȭʦ" },
	{ em_Player_Classes_Danncer,			L"�齣ʿ" },
	{ em_Player_Classes_Warlock,			L"����ʦ" },
	{ em_Player_Classes_KongFuForceMaster,	L"����" },
	{ em_Player_Classes_Gunner,				L"ǹ��" },
};

CPlayer::CPlayer(DWORD dwBase_) : CBaseObject(dwBase_)
{
	m_dwOldMaxHp = GetMaxHp();
	m_OldObjectId = GetObjectId();
}

CPlayer::CPlayer() : m_dwOldMaxHp(NULL), m_OldObjectId(NULL, NULL)
{
	
}


CPlayer::~CPlayer()
{

}

DWORD CPlayer::GetHp() CONST
{
	return ReadDWORD(GetBase() + HPƫ��);
}

DWORD CPlayer::GetMaxHp() CONST
{
	// ����Ѫ�� + װ��Ѫ��
	return ReadDWORD(GetBase() + MAXHPƫ��1) + ReadDWORD(GetBase() + MAXHPƫ��2);
}

DWORD CPlayer::GetPercentHp() CONST
{
	float fMaxHp = static_cast<float>(GetMaxHp());
	if (fMaxHp == 0)
		return 0;

	return static_cast<DWORD>(static_cast<float>(GetHp()) / fMaxHp * 100);
}

DWORD CPlayer::GetMp() CONST
{
	return ReadDWORD(GetBase() + MPƫ��) & 0xFFFF;
}

CONST std::wstring& CPlayer::GetName() CONST
{
	SetName();
	return Name;
}

VOID CPlayer::SetName() CONST
{
	WCHAR wszText[MAX_OBJECT_NAME_LEN] = { 0 };
	DWORD dwAddr = ReadDWORD(GetBase() + Nameƫ��);
	if (dwAddr != NULL && ReadDWORD(dwAddr) != NULL)
		MyTools::CCharacter::wstrcpy_my(wszText, (LPCWSTR)dwAddr, _countof(wszText) - 1);
	Name = wszText;
}

CONST Point CPlayer::GetPoint() CONST
{
	DWORD dwAddr = ReadDWORD(GetBase() + 0x14) + Pointƫ��;
	return Point(ReadFloat(dwAddr + 0x0), ReadFloat(dwAddr + 0x4), ReadFloat(dwAddr + 0x8));
}

em_Camp CPlayer::GetCamp() CONST
{
	return ReadBYTE(GetBase() + ��Ӫƫ��) == 0x1 ? em_Camp_������ : em_Camp_�����;
}

float CPlayer::GetDis() CONST
{
	return GetPersonDis(GetPoint());
}

float CPlayer::GetDis(_In_ CONST Point& TarPoint) CONST
{
	return Get3DDis(GetPoint(), TarPoint);
}

VOID CPlayer::SetAngle() CONST
{
	CBaseObject::SetAngle(GetPoint(), 0xFCE3);
	/*CSomeFun::GetInstance().TimeOut_Condiction_GameRun(1000, [this]
	{
		CBaseObject::SetAngle(GetPoint(), 0xFCE3);
		return !IsDead() && CPerson::GetInstance().GetTargetId() == NULL && CPerson::GetInstance().GetDis(GetPoint()) <= 15.0f;
	});*/
}

VOID CPlayer::SetRoundAngle() CONST
{
	CBaseObject::SetAngle(GetPoint(), 0xFCE3, 5.0f);
}

BOOL CPlayer::IsDead() CONST
{
	if (GetHp() == NULL)
	{
		//LOG_CF_D( L"���ѪHP=NULL");
		return TRUE;
	}
	else if (m_dwOldMaxHp != GetMaxHp())
	{
		LOG_CF_D( L"���������MAXHP=%d, ���ڵ�MAXHP=%d, ���Ѫ����, ����������!", m_dwOldMaxHp, GetMaxHp());
		return TRUE;
	}
	else if (m_OldObjectId != GetObjectId())
	{
		LOG_CF_D( L"���������ID=[%X,%X]����,����=[%X,%X], ����������", m_OldObjectId.dwId, m_OldObjectId.dwIdParm, GetObjectId().dwId, GetObjectId().dwIdParm);
		return TRUE;
	}
	return FALSE;
}

BOOL CPlayer::IsExist() CONST
{
	return TRUE;
}

DWORD CPlayer::GetTargetId() CONST
{
	return ReadDWORD(GetBase() + ����Ŀ��ID) & 0xFFFF;
}

bool CPlayer::operator<(__in const CPlayer& GameObj)
{
	return this->SortDis() < GameObj.SortDis();
}

BOOL CPlayer::InviteTeamMember(DWORD dwPlayerId)
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&dwPlayerId]{CGameSend::InviteTeamMember(dwPlayerId, 0x10000); });
}

Point CPlayer::GetBehindPoint(_In_ float fDis) CONST
{
	return _GetBehindPoint(this, fDis);
}

Point CPlayer::GetFrontPoint(_In_ float fDis) CONST
{
	return _GetFrontPoint(this, fDis);
}

BOOL CPlayer::Take(DWORD dwSleepTime) CONST
{
	if (GetPersonDis(GetPoint()) > 3.0f)
	{
		SetAngle();
		BnsSleep(dwSleepTime);
		BnsMove(GetPoint(), 3.0f);
		//CPersonFunction::GetInstance().Move(CBaseObject::GetPersonPoint());
		this->SetAngle();
		CSomeFun::GetInstance().SimlationKey('W', 50, 500);
		BnsSleep(dwSleepTime);
	}

	CPersonFunction::GetInstance().Pickup(this->GetObjectId());

	auto fnPickupItem = []
	{
		while (!EMPTY_PERSONHP && CObjectFunction::GetInstance().IsShow_In_GameUi(L"PickupItemPanel") && GameRun)
		{
			LOG_CF_D(L"��F���μ�ʬ��!");
			CSomeFun::GetInstance().SimlationKey('F');
			BnsSleep(1000);
			CSomeFun::GetInstance().SimlationKey('Y');
			BnsSleep(1000);
		}
		return TRUE;
	};

	CSomeFun::GetInstance().TimeOut_Condiction_GameRun(8 * 1000, []{
		return CPerson::GetInstance().IsLoadProgressBar();
	});

	// ��������Ϻ�������1��, ��Ϊ�����������ʱ��, F���ڻ�û��ʾ!
	BnsSleep(dwSleepTime == NULL ? 0 : 1000);
	return fnPickupItem();
}

em_Player_Classes CPlayer::GetClasses() CONST
{
	return (em_Player_Classes)ReadBYTE(GetBase() + 0x28);
}

CONST std::wstring CPlayer::GetClassesText() CONST
{
	return GetTextByPlayerClasses(GetClasses());
}

BOOL CPlayer::InviteTansaction() CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([this]{CGameSend::InviteTansaction(GetId(), GetIdParm()); });;
}

BOOL CPlayer::FindPlayerclassesText(_Out_opt_ PlayerclassesText* pPlayerclassesText, _In_ std::function<BOOL(CONST PlayerclassesText&)> fnFind)
{
	return MyTools::CLPublic::Vec_find_if(vPlayerclassesText, pPlayerclassesText, fnFind);
}

em_Player_Pose CPlayer::GetFightPose() CONST
{
	//07=�ν���̬
	return static_cast<em_Player_Pose>(ReadBYTE(ReadDWORD(GetBase() + 0x14) + ս������ƫ��));
}

em_FightStatus CPlayer::GetFightStatus() CONST
{
	DWORD dwValue = ReadDWORD(ReadDWORD(GetBase() + 0x14) + ����״̬ƫ��);
	switch (dwValue)
	{
	case 0xD:
		return em_FightStatus::em_FightStatus_Fall;
	case 0xB:
		return em_FightStatus::em_FightStatus_Dizzy;
	case 0x12:
		return em_FightStatus::em_FightStatus_Weak;
	case 0x30:
		return em_FightStatus::em_FightStatus_Snatch;
	default:
		break;
	}
	return em_FightStatus::em_FightStatus_Other;
}

BOOL CPlayer::IsUseSkilling() CONST
{
	return ReadDWORD(ReadDWORD(GetBase() + 0x14) + �Ƿ�ʹ�ü�����ƫ��) != NULL;
}

CONST std::wstring CPlayer::GetTextByPlayerClasses(_In_ em_Player_Classes emPlayerClasses)
{
	PlayerclassesText PlayerclassesText_;
	if (!FindPlayerclassesText(&PlayerclassesText_, [&emPlayerClasses](CONST PlayerclassesText& PlayerclassesText__){
		return PlayerclassesText__.emPlayerClasses == emPlayerClasses;
	}))
	{
		LOG_CF_E( L"����ʲô����ְҵ?emPlayerClasses=%X", emPlayerClasses);
		return std::wstring(L"");
	}

	return PlayerclassesText_.wsText;
}

float CPlayer::GetAngle() CONST
{
	return CBaseObject::GetAngle(GetPoint());
}

em_WarStatus CPlayer::GetAbNormalStatus() CONST
{
	switch (ReadDWORD(ReadDWORD(GetBase() + 0x14) + ����ƫ��))
	{
	case 0x4:
	case 0xD:	// ����
	case 0x400:
	case 0xB:	// ѣ��
	case 0x30:	// ����
	case 0xF:	// ����
		return em_WarStatus::em_WarStatus_Floor;
	case 0x1F:
		return em_WarStatus::em_WarStatus_Hide;
	case 0x6:
		return em_WarStatus::em_WarStatus_Dying;
	case 0x21:
		return em_WarStatus::em_WarStatus_Healing;
	default:
		break;
	}
	return em_WarStatus::em_WarStatus_None;
}

void CPlayer::SetBase(_In_ DWORD dwBase_)
{
	dwBase = dwBase_;
	m_dwOldMaxHp = GetMaxHp();
	m_OldObjectId = GetObjectId();
}

Point CPlayer::GetLeftPoint(_In_ float fDis) CONST
{
	return CBaseObject::_GetSidePoint(this, TRUE, fDis);
}

Point CPlayer::GetRightPoint(_In_ float fDis) CONST
{
	return CBaseObject::_GetSidePoint(this, FALSE, fDis);
}

Point CPlayer::GetFacePoint(float fDis) CONST
{
	return _GetFacePoint(this, fDis);
}
