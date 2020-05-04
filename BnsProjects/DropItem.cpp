#include "stdafx.h"
#include "DropItem.h"
#include "SomeFun.h"
#include "PersonFunction.h"
#include "GameSend.h"

#define _SELF L"DropItem.cpp"
CDropItem::CDropItem(DWORD dwDropItemBase) : CPlayer(dwDropItemBase)
{
	
}

CDropItem::CDropItem()
{

}

CDropItem::~CDropItem()
{
}

CONST Point CDropItem::GetPoint() CONST
{
	auto fn_GetDropItemPoint = [this](int n)
	{
		return (float)(4 * (int)(short)ReadDWORD(GetBase() + 0x28 + n));
	};

	return Point(fn_GetDropItemPoint(0), fn_GetDropItemPoint(2), fn_GetDropItemPoint(4));
}

float CDropItem::GetDis() CONST
{
	return CBaseObject::GetPersonDis(GetPoint());
}

float CDropItem::GetDis(_In_ CONST Point& Tar) CONST
{
	return Get3DDis(Tar, GetPoint());
}

bool CDropItem::operator <(const CDropItem& DropItem) CONST
{
	return this->SortDis() < DropItem.SortDis();
}

BOOL CDropItem::TakeItem() CONST
{
	if (GetDis() > 2.0f)
	{
		BnsMove(GetPoint(), 2.0f);
		BnsSleep(300);
	}
	if (GetId() == NULL || !IsDropItem())
		return FALSE;

	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([this]{
		CGameSend::TakeItem_By_DropItem(GetId(), GetIdParm());
	});
}

BOOL CDropItem::PickCorpse() CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([this]{
		CGameSend::PickCorpse(GetId(), GetIdParm());
	});
}

BOOL CDropItem::IsExist() CONST
{
	return TRUE;
}

DWORD CDropItem::GetMonsterId() CONST
{
	return ReadDWORD(ReadDWORD(GetBase() + 0x14) + 0x38);
}

BOOL CDropItem::IsDropItem() CONST
{
	if (ReadDWORD(ReadDWORD(GetBase() + 0x14) + 0x14) != -1 && ReadDWORD(ReadDWORD(GetBase() + 0x14) + 0x18) != -1)
		return TRUE;
	return FALSE;
}

CONST std::wstring& CDropItem::GetName() CONST
{
	return Name;
}

void CDropItem::SetAngle() CONST
{
	CBaseObject::SetAngle(GetPoint());
}

BOOL CDropItem::IsCorpse() CONST
{
	if (ReadDWORD(ReadDWORD(GetBase() + 0x14) + 0x10) == -1 && (ReadDWORD(ReadDWORD(GetBase() + 0x14) + 0x14) == -1 || ReadDWORD(ReadDWORD(GetBase() + 0x14) + 0x18) == -1))
		return FALSE;
	return TRUE;
}
