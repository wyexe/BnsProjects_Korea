#include "stdafx.h"
#include "Person.h"
#include <MyTools/CLSearchBase.h>
#include <MyTools/Log.h>

#define _SELF L"Person.cpp"
CPerson::CPerson() : CPlayer()
{

}

CPerson::CPerson(DWORD dwBase_) : CPlayer(dwBase_)
{

}

CPerson& CPerson::GetInstance()
{
	static CPerson Person;
	Person.SetBase(ReadDWORD(ReadDWORD(ReadDWORD(人物基址) + 人物基址偏移1) + 人物基址偏移2));
	return Person;
}

DWORD CPerson::GetLevel() CONST
{
	return ReadDWORD(GetBase() + 等级偏移) & 0xFF;
}

DWORD CPerson::GetMoney() CONST
{
	return ReadDWORD(GetBase() + Money偏移);
}

DWORD CPerson::GetVigour() CONST
{
	return 10000;
}

BOOL CPerson::IsWarOfStatus() CONST
{
	return GetPersonStatus() == em_WarStatus_None ? FALSE : TRUE;
}

em_WarStatus CPerson::GetPersonStatus() CONST
{
	return (em_WarStatus)ReadBYTE(ReadDWORD(GetBase() + 0x14) + 状态偏移);
}

DWORD CPerson::GetTargetId() CONST
{
	return ReadDWORD(GetBase() + 怪物目标ID);
}

BOOL CPerson::IsLoadProgressBar() CONST
{
	//if (ReadBYTE(ReadDWORD(GetBase() + 0x14) + 人物读条偏移 + 0x10) != NULL)
	//	return FALSE;

	return (ReadBYTE(ReadDWORD(GetBase() + 0x14) + 人物读条偏移) == 1 && ReadBYTE(ReadDWORD(GetBase() + 0x14) + 人物读条偏移 + 0x10) == 0x0) ? TRUE : FALSE;
}

DWORD CPerson::GetPkBean() CONST
{
	return ReadDWORD(GetBase() + PK豆偏移);
}

DWORD CPerson::GetChannel() CONST
{
	return static_cast<DWORD>(ReadBYTE(GetBase() + 当前频道偏移));
}

BOOL CPerson::IsPickStatus() CONST
{
	return ReadDWORD(GetBase() + 捡物状态偏移) != NULL;
}

CPerson::~CPerson()
{

}

DWORD CPerson::GetInviateId_By_Transaction() CONST
{
	return ReadDWORD(GetBase() + 人物邀请交易ID偏移);
}

BOOL CPerson::IsMoving() CONST
{
	//return ReadDWORD(GetPersonPointAddr - 0x54 + g_dwMoveStatusOffset) == 0x1;
	return ReadBYTE(ReadDWORD(GetBase() + 0x14) + 正在走路偏移) == 0x1;
}

DWORD CPerson::GetMapId() CONST
{
	return ReadDWORD(GetBase() + MAPID偏移);
}

BOOL CPerson::IsLoadingByUseSkill() CONST
{
	return ReadDWORD(ReadDWORD(GetBase() + 0x14) + 技能读条偏移) != NULL;
}

BOOL CPerson::IsSustainabilityByUseSkill() CONST
{
	return ReadDWORD(ReadDWORD(GetBase() + 0x14) + 技能读条偏移 + 0x4) != NULL;
}

CONST Point CPerson::GetPoint() CONST
{
	return GetPersonPoint();
}

BOOL CPerson::IsPassDoor() CONST
{
	return ReadBYTE(ReadDWORD(GetBase() + 0x14) + 过门偏移) == 0x1;
}

CPerson::em_LoseConnect_Reason CPerson::IsLoseConnect() CONST
{
	// == 0x8 Lose Connect    == 0xC = 挤号
	switch (ReadDWORD(ReadDWORD(人物基址) + 判断是否掉线偏移))
	{
	case 0x8:
		return em_LoseConnect_Reason::em_LoseConnect_Reason_LoseConnect;
	case 0xC:
		return em_LoseConnect_Reason::em_LoseConnect_Reason_SqueezeAccount;
	default:
		break;
	}
	return ReadDWORD(ReadDWORD(人物基址) + 判断是否掉线偏移) != NULL ? em_LoseConnect_Reason::em_LoseConnect_Reason_Other : em_LoseConnect_Reason_Normal;
}

CONST std::wstring CPerson::GetText_By_LoseConnectReason(_In_ em_LoseConnect_Reason emReason)
{
	struct ReasonText
	{
		std::wstring			wsText;
		em_LoseConnect_Reason	emLoseReason;
	};

	CONST static std::vector<ReasonText> vlst = {
		{ L"与游戏服务器断开连接, 估计VPN挂了", em_LoseConnect_Reason::em_LoseConnect_Reason_LoseConnect },
		{ L"正常状态呐……不该啊!", em_LoseConnect_Reason::em_LoseConnect_Reason_Normal },
		{ L"其他原因断开连接的! 暂时不知道是为嘛!", em_LoseConnect_Reason::em_LoseConnect_Reason_Other },
		{ L"被挤号啦!!!", em_LoseConnect_Reason::em_LoseConnect_Reason_SqueezeAccount },
	};

	auto p = MyTools::CLPublic::Vec_find_if_Const(vlst, [&emReason](CONST ReasonText& Rt){ return Rt.emLoseReason == emReason; });
	return p != nullptr ? p->wsText : std::wstring(L"Empty");
}

BOOL CPerson::IsLoadingMapRes() CONST
{
	static DWORD dwMapResBase = NULL;
	if (dwMapResBase == NULL)
	{
		dwMapResBase = MyTools::CLSearchBase::FindBase("EB038D4900833D????????0074", 0x8DEBC9B - 0x8DEBCA0, 0x2, 0x0, L"bsengine_Shipping");
		if (dwMapResBase == NULL)
		{
			LOG_MSG_CF( L"是否加载地图资源完毕偏移的自动更新地址失效拉!");
			return 0;
		}
	}
	return ReadBYTE(dwMapResBase) != NULL;
}

float CPerson::Get2DDis(_In_ CONST Point& TarPoint) CONST
{
	return MyTools::CLPublic::GetDisBy2D(CPerson::GetInstance().GetPoint(), TarPoint) / 50;
}

DWORD CPerson::GetStarCoin() CONST
{
	return ReadDWORD(ReadDWORD(ReadDWORD(人物基址) + 星币偏移1) + 星币数量偏移);
}
