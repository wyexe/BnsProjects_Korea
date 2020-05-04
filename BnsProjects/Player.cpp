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
	{ em_Player_Classes_Destroyer,			L"力士"},
	{ em_Player_Classes_Assassin,			L"刺客" },
	{ em_Player_Classes_BladeMaster,		L"剑士" },
	{ em_Player_Classes_Summoner,			L"召唤师" },
	{ em_Player_Classes_ForceMaster,		L"气功" },
	{ em_Player_Classes_KongFuMaster,		L"拳师" },
	{ em_Player_Classes_Danncer,			L"灵剑士" },
	{ em_Player_Classes_Warlock,			L"咒术师" },
	{ em_Player_Classes_KongFuForceMaster,	L"气宗" },
	{ em_Player_Classes_Gunner,				L"枪手" },
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
	return ReadDWORD(GetBase() + HP偏移);
}

DWORD CPlayer::GetMaxHp() CONST
{
	// 自身血量 + 装备血量
	return ReadDWORD(GetBase() + MAXHP偏移1) + ReadDWORD(GetBase() + MAXHP偏移2);
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
	return ReadDWORD(GetBase() + MP偏移) & 0xFFFF;
}

CONST std::wstring& CPlayer::GetName() CONST
{
	SetName();
	return Name;
}

VOID CPlayer::SetName() CONST
{
	WCHAR wszText[MAX_OBJECT_NAME_LEN] = { 0 };
	DWORD dwAddr = ReadDWORD(GetBase() + Name偏移);
	if (dwAddr != NULL && ReadDWORD(dwAddr) != NULL)
		MyTools::CCharacter::wstrcpy_my(wszText, (LPCWSTR)dwAddr, _countof(wszText) - 1);
	Name = wszText;
}

CONST Point CPlayer::GetPoint() CONST
{
	DWORD dwAddr = ReadDWORD(GetBase() + 0x14) + Point偏移;
	return Point(ReadFloat(dwAddr + 0x0), ReadFloat(dwAddr + 0x4), ReadFloat(dwAddr + 0x8));
}

em_Camp CPlayer::GetCamp() CONST
{
	return ReadBYTE(GetBase() + 阵营偏移) == 0x1 ? em_Camp_武林盟 : em_Camp_浑天教;
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
		//LOG_CF_D( L"最大血HP=NULL");
		return TRUE;
	}
	else if (m_dwOldMaxHp != GetMaxHp())
	{
		LOG_CF_D( L"怪物最初的MAXHP=%d, 现在的MAXHP=%d, 最大血变了, 当死亡处理!", m_dwOldMaxHp, GetMaxHp());
		return TRUE;
	}
	else if (m_OldObjectId != GetObjectId())
	{
		LOG_CF_D( L"怪物最初的ID=[%X,%X]变了,现在=[%X,%X], 当死亡处理", m_OldObjectId.dwId, m_OldObjectId.dwIdParm, GetObjectId().dwId, GetObjectId().dwIdParm);
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
	return ReadDWORD(GetBase() + 怪物目标ID) & 0xFFFF;
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
			LOG_CF_D(L"按F二次捡尸体!");
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

	// 进度条完毕后再休眠1秒, 因为进度条刚完的时候, F窗口还没显示!
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
	//07=拔剑姿态
	return static_cast<em_Player_Pose>(ReadBYTE(ReadDWORD(GetBase() + 0x14) + 战斗姿势偏移));
}

em_FightStatus CPlayer::GetFightStatus() CONST
{
	DWORD dwValue = ReadDWORD(ReadDWORD(GetBase() + 0x14) + 怪物状态偏移);
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
	return ReadDWORD(ReadDWORD(GetBase() + 0x14) + 是否使用技能中偏移) != NULL;
}

CONST std::wstring CPlayer::GetTextByPlayerClasses(_In_ em_Player_Classes emPlayerClasses)
{
	PlayerclassesText PlayerclassesText_;
	if (!FindPlayerclassesText(&PlayerclassesText_, [&emPlayerClasses](CONST PlayerclassesText& PlayerclassesText__){
		return PlayerclassesText__.emPlayerClasses == emPlayerClasses;
	}))
	{
		LOG_CF_E( L"这是什么奇葩职业?emPlayerClasses=%X", emPlayerClasses);
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
	switch (ReadDWORD(ReadDWORD(GetBase() + 0x14) + 倒地偏移))
	{
	case 0x4:
	case 0xD:	// 倒地
	case 0x400:
	case 0xB:	// 眩晕
	case 0x30:	// 擒拿
	case 0xF:	// 浮空
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
