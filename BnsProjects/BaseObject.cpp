#include "stdafx.h"
#include "BaseObject.h"
#include <MyTools/Log.h>
#include <MyTools/CLEchoException.h>
#include "GameSend.h"

#define _SELF L"BaseObject.cpp"

CBaseObject::CBaseObject()
{
	m_fDis = 0;
	SetBase(NULL);
}

CBaseObject::CBaseObject(_In_ DWORD dwBase_)
{
	m_fDis = 0;
	SetBase(dwBase_);
}

CBaseObject::~CBaseObject()
{
}

DWORD CBaseObject::GetBase() CONST
{
	return dwBase;
}

VOID CBaseObject::SetBase(_In_ DWORD dwBase_)
{
	dwBase = dwBase_;
}

DWORD CBaseObject::GetId() CONST
{
	return ReadDWORD(GetBase() + 0x8);
}

DWORD CBaseObject::GetIdParm() CONST
{
	return ReadDWORD(GetBase() + 0xC);
}

GameObjectID CBaseObject::GetObjectId() CONST
{
	return GameObjectID(GetId(), GetIdParm());
}

float CBaseObject::Get3DDis(_In_ CONST Point& p1, _In_ CONST Point& p2)
{
	return MyTools::CLPublic::GetDisBy3D(p1, p2) / 50.0f;
}

CONST Point CBaseObject::GetPersonPoint()
{
	//LOG_CF_D( L"%x,%x", GetMoveBaseAddr, GetPersonPointAddr);
	DWORD dwAddr = GetPersonPointAddr;
	return Point(ReadFloat(dwAddr + 0x0), ReadFloat(dwAddr + 0x4), ReadFloat(dwAddr + 0x8));
}

float CBaseObject::GetAngle(_In_ CONST Point& TarPoint)
{
	Point NowPoint = GetPersonPoint();
	float fOppositeSide = NowPoint.X - TarPoint.X;		// 对边
	float fAdjacentSide = NowPoint.Y - TarPoint.Y;		// 邻边
	//float fHypotenuseSide = TarPoint.Z - NowPoint.Z;	// 斜边

	//DWORD dwPointAddr = GetPersonPointAddr;
	//DWORD dwMoveAddr = GetMoveBaseAddr;

	// 根据射线计算角度 弧度=180/π
	float fAngle = (float)fabs((float)atan2(abs(fAdjacentSide), abs(fOppositeSide))) * (float)(180.0f / 3.1415926f);
	//LOG_CF_D( L"fAngle=%.2f,fX=%.2f,fY=%.2f", fAngle, fOppositeSide, fAdjacentSide);
	if (fOppositeSide > 0 && fAdjacentSide > 0) // X > 0 && Y > 0
	{
		//LOG_CF_D( L"第3象限,fAngle=%.2f", fAngle);
		fAngle += 180.0f;
	}
	else if (fOppositeSide >= 0 && fAdjacentSide <= 0) // X > 0 && Y < 0 
	{
		//LOG_CF_D( L"第4象限,fAngle=%.2f", fAngle);
		fAngle = 180.0f - fabs(fAngle);
	}
	else if (fOppositeSide <= 0 && fAdjacentSide <= 0) // X < 0 && Y < 0 
	{
		//LOG_CF_D( L"第1象限,fAngle=%.2f", fAngle);
	}
	else if (fOppositeSide <= 0 && fAdjacentSide >= 0) // X < 0 && Y > 0 
	{
		//LOG_CF_D( L"第2象限,fAngle=%.2f", fAngle);
		fAngle = 360.0f - fabs(fAngle);
	}
	return fAngle;
}

VOID CBaseObject::SetAngle(_In_ CONST Point& TarPoint, _In_ DWORD dwCameraHeight /* = 0xF623 */, _In_ float fMoveLeftAngle /* = 0.0f */)
{
	MyTools::CLEchoException::GetInstance().InvokeAction(__FUNCTIONW__, [=] 
	{
		float fAngle = GetAngle(TarPoint);
		if (fMoveLeftAngle != 0.0f && fAngle > fMoveLeftAngle)
			fAngle -= fMoveLeftAngle;

		DWORD dwData = (DWORD)((fAngle - 0.5f) / 0.005493247f);

		//if (ReadDWORD(GetPersonPointAddr + 0x10) != dwData)
		//{
		//	// send angle to server
		//	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&fAngle]{CGameSend::SetAngle(static_cast<DWORD>(fAngle)); });
		//}

		// set angle to client

		// 头部转向
		MyTools::CCharacter::WriteDWORD(GetPersonPointAddr + 0x10, dwData);
		MyTools::CCharacter::WriteDWORD(GetMoveBaseAddr + 0x60 + 0x4, dwData);
		MyTools::CCharacter::WriteDWORD(GetMoveBaseAddr + 0x60, dwCameraHeight);
	});
}

DWORD CBaseObject::GetObjectNameAddr(_In_ DWORD dwParm1, _In_ DWORD dwParm2)
{
	__try
	{
		DWORD dwValue = 0;
		_asm
		{
			PUSHAD
				PUSH dwParm2
				PUSH dwParm1

				MOV EAX, 名字返回基址
				MOV EAX, DWORD PTR DS : [EAX]
				MOV ECX, DWORD PTR DS : [EAX + 名字返回偏移1]
				MOV EDX, DWORD PTR DS : [ECX]
				MOV EDX, DWORD PTR DS : [EDX + 名字返回偏移2]
				CALL EDX
				MOV dwValue, EAX
				POPAD
		}
		return dwValue;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E( L"GetObjectNameAddr发生了异常");
	}
	return 0;
}

float CBaseObject::GetPersonDis(_In_ CONST Point& TarPoint)
{
	return Get3DDis(TarPoint, GetPersonPoint());
}

FixPointInfo CBaseObject::CreateFixPointInfo(float fDis)
{
	return FixPointInfo(GetPersonPoint(), fDis);
}


bool CBaseObject::SameSide(CONST Point& A, CONST Point& B, CONST Point& C, CONST Point& P)
{
	Point AB_ = Point(B.X - A.X, B.Y - A.Y, 0.0f);
	Point AC_ = Point(C.X - A.X, B.Y - A.Y, 0.0f);
	Point AP_ = Point(P.X - A.X, P.Y - A.Y, 0.0f);

	Point v1_ = Point(0.0f, 0.0f, AB_.X * AC_.Y - AB_.Y * AC_.X);
	Point v2_ = Point(0.0f, 0.0f, AB_.X * AP_.Y - AB_.Y * AP_.X);

	return (v1_.X * v2_.X + v1_.Y * v2_.Y + v1_.Z * v2_.Z) >= 0;
}

bool CBaseObject::PointinTriangle(CONST Point& A, CONST Point& B, CONST Point& C, CONST Point& P)
{
	return SameSide(A, B, C, P) && SameSide(B, C, A, P) && SameSide(C, A, B, P);
}

std::pair<Point, Point> CBaseObject::GetLeftRightVec(CONST Point& v, float fAngle, float r)
{
	fAngle /= 2;
	auto W = static_cast<float>(r * (sin(fAngle * 3.1415926 / 180)));
	auto H = static_cast<float>(r * (cos(fAngle * 3.1415926 / 180)));
	Point L(v.X - W, v.Y + H, 0.0f);
	Point R(v.X + W, v.Y + H, 0.0f);
	return std::make_pair(L, R);
}

float CBaseObject::GetDis(_In_ CONST Point& TarPoint) CONST
{
	return GetPersonDis(TarPoint);
}

VOID CBaseObject::SetDis(_In_ CONST float fDis)
{
	m_fDis = fDis;
}

float CBaseObject::SortDis() CONST
{
	return m_fDis;
}
