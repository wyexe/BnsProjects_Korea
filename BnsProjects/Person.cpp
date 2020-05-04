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
	Person.SetBase(ReadDWORD(ReadDWORD(ReadDWORD(�����ַ) + �����ַƫ��1) + �����ַƫ��2));
	return Person;
}

DWORD CPerson::GetLevel() CONST
{
	return ReadDWORD(GetBase() + �ȼ�ƫ��) & 0xFF;
}

DWORD CPerson::GetMoney() CONST
{
	return ReadDWORD(GetBase() + Moneyƫ��);
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
	return (em_WarStatus)ReadBYTE(ReadDWORD(GetBase() + 0x14) + ״̬ƫ��);
}

DWORD CPerson::GetTargetId() CONST
{
	return ReadDWORD(GetBase() + ����Ŀ��ID);
}

BOOL CPerson::IsLoadProgressBar() CONST
{
	//if (ReadBYTE(ReadDWORD(GetBase() + 0x14) + �������ƫ�� + 0x10) != NULL)
	//	return FALSE;

	return (ReadBYTE(ReadDWORD(GetBase() + 0x14) + �������ƫ��) == 1 && ReadBYTE(ReadDWORD(GetBase() + 0x14) + �������ƫ�� + 0x10) == 0x0) ? TRUE : FALSE;
}

DWORD CPerson::GetPkBean() CONST
{
	return ReadDWORD(GetBase() + PK��ƫ��);
}

DWORD CPerson::GetChannel() CONST
{
	return static_cast<DWORD>(ReadBYTE(GetBase() + ��ǰƵ��ƫ��));
}

BOOL CPerson::IsPickStatus() CONST
{
	return ReadDWORD(GetBase() + ����״̬ƫ��) != NULL;
}

CPerson::~CPerson()
{

}

DWORD CPerson::GetInviateId_By_Transaction() CONST
{
	return ReadDWORD(GetBase() + �������뽻��IDƫ��);
}

BOOL CPerson::IsMoving() CONST
{
	//return ReadDWORD(GetPersonPointAddr - 0x54 + g_dwMoveStatusOffset) == 0x1;
	return ReadBYTE(ReadDWORD(GetBase() + 0x14) + ������·ƫ��) == 0x1;
}

DWORD CPerson::GetMapId() CONST
{
	return ReadDWORD(GetBase() + MAPIDƫ��);
}

BOOL CPerson::IsLoadingByUseSkill() CONST
{
	return ReadDWORD(ReadDWORD(GetBase() + 0x14) + ���ܶ���ƫ��) != NULL;
}

BOOL CPerson::IsSustainabilityByUseSkill() CONST
{
	return ReadDWORD(ReadDWORD(GetBase() + 0x14) + ���ܶ���ƫ�� + 0x4) != NULL;
}

CONST Point CPerson::GetPoint() CONST
{
	return GetPersonPoint();
}

BOOL CPerson::IsPassDoor() CONST
{
	return ReadBYTE(ReadDWORD(GetBase() + 0x14) + ����ƫ��) == 0x1;
}

CPerson::em_LoseConnect_Reason CPerson::IsLoseConnect() CONST
{
	// == 0x8 Lose Connect    == 0xC = ����
	switch (ReadDWORD(ReadDWORD(�����ַ) + �ж��Ƿ����ƫ��))
	{
	case 0x8:
		return em_LoseConnect_Reason::em_LoseConnect_Reason_LoseConnect;
	case 0xC:
		return em_LoseConnect_Reason::em_LoseConnect_Reason_SqueezeAccount;
	default:
		break;
	}
	return ReadDWORD(ReadDWORD(�����ַ) + �ж��Ƿ����ƫ��) != NULL ? em_LoseConnect_Reason::em_LoseConnect_Reason_Other : em_LoseConnect_Reason_Normal;
}

CONST std::wstring CPerson::GetText_By_LoseConnectReason(_In_ em_LoseConnect_Reason emReason)
{
	struct ReasonText
	{
		std::wstring			wsText;
		em_LoseConnect_Reason	emLoseReason;
	};

	CONST static std::vector<ReasonText> vlst = {
		{ L"����Ϸ�������Ͽ�����, ����VPN����", em_LoseConnect_Reason::em_LoseConnect_Reason_LoseConnect },
		{ L"����״̬�š������ð�!", em_LoseConnect_Reason::em_LoseConnect_Reason_Normal },
		{ L"����ԭ��Ͽ����ӵ�! ��ʱ��֪����Ϊ��!", em_LoseConnect_Reason::em_LoseConnect_Reason_Other },
		{ L"��������!!!", em_LoseConnect_Reason::em_LoseConnect_Reason_SqueezeAccount },
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
			LOG_MSG_CF( L"�Ƿ���ص�ͼ��Դ���ƫ�Ƶ��Զ����µ�ַʧЧ��!");
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
	return ReadDWORD(ReadDWORD(ReadDWORD(�����ַ) + �Ǳ�ƫ��1) + �Ǳ�����ƫ��);
}
