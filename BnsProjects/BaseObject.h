#ifndef __GITBNSPROJECTS_BNSPROJECTS_OBJECT_GAMEOBJECT_BASEOBJECT_H__
#define __GITBNSPROJECTS_BNSPROJECTS_OBJECT_GAMEOBJECT_BASEOBJECT_H__

#include "GameVariable.h"

typedef struct _GameObjectID
{
	DWORD dwId;
	DWORD dwIdParm;
	_GameObjectID()
	{
		dwId = dwIdParm = NULL;
	}
	_GameObjectID(DWORD dwId_, DWORD dwIdParm_)
	{
		dwId = dwId_;
		dwIdParm = dwIdParm_;
	}

	bool operator != (CONST _GameObjectID& GameObjId) CONST
	{
		return GameObjId.dwId != this->dwId || GameObjId.dwIdParm != this->dwIdParm;
	}
}GameObjectID;

//dd [[[[[[[0AA729C4]+3BC]+0]+40]+218]+224]+218]+54
#ifdef BnsProject_TaiWan
#define GetMoveBaseAddr					ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(g_dwMoveBase) + 人物坐标偏移1) + 0x0) + 人物坐标偏移2)
#define GetPersonPointAddr				ReadDWORD(ReadDWORD(ReadDWORD(GetMoveBaseAddr + 人物坐标偏移3) + 人物坐标偏移4) + 人物坐标偏移5) + 0x54

#endif // BnsProJect_TaiWan

#ifdef BnsProject_Korea
#define GetMoveBaseAddr					ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(g_dwMoveBase) + 人物坐标偏移1) + 0x0) + 人物坐标偏移2)
#define GetPersonPointAddr				ReadDWORD(ReadDWORD(ReadDWORD(GetMoveBaseAddr + 人物坐标偏移3) + 人物坐标偏移4) + 人物坐标偏移5) + 0x54

#endif // BnsProJect_TaiWan

class CBaseObject
{
public:
	explicit CBaseObject();
	explicit CBaseObject(_In_ DWORD dwBase_);
	~CBaseObject();

	DWORD	GetBase() CONST;

	virtual VOID	SetBase(_In_ DWORD dwBase_);

	VOID SetDis(_In_ CONST float fDis);

	float SortDis() CONST;

	inline DWORD GetId() CONST;
	inline DWORD GetIdParm() CONST;

	GameObjectID GetObjectId() CONST;

	// Name
	virtual CONST std::wstring&	GetName() CONST = NULL;
	virtual VOID		SetName() CONST = NULL;
	// Point
	virtual CONST Point	GetPoint() CONST = NULL;

	virtual float	GetDis() CONST = NULL;

	virtual float	GetDis(_In_ CONST Point& TarPoint) CONST;

	virtual BOOL	IsExist() CONST = NULL;

	virtual void	SetAngle() CONST = NULL;

public:

	// 计算距离
	static float Get3DDis(_In_ CONST Point& p1, _In_ CONST Point& p2);

	// 返回名字Object
	static DWORD		GetObjectNameAddr(_In_ DWORD dwParm1, _In_ DWORD dwParm2);

	// 获取人物当前坐标
	static CONST Point		GetPersonPoint();

	// 获取角度
	static float			GetAngle(_In_ CONST Point& TarPoint);

	// 对准角度
	static VOID				SetAngle(_In_ CONST Point& TarPoint, _In_ DWORD dwCameraHeight = 0xF623, _In_ float fMoveLeftAngle = 0.0f);

	static float			GetPersonDis(_In_ CONST Point& TarPoint);
	static FixPointInfo		CreateFixPointInfo(float fDis);

	// 获取前面的坐标(人物与怪物之间)
	template<typename T>
	static CONST Point		_GetFrontPoint(_In_ CONST T* Tar, float fDis = 2.0f)
	{
		Point TargetPoint = Tar->GetPoint();
		float k = Tar->GetDis() == 0.0f ? fDis : fDis / Tar->GetDis();
		float X = (TargetPoint.X - GetPersonPoint().X) * k;
		float Y = (TargetPoint.Y - GetPersonPoint().Y) * k;
		return Point(TargetPoint.X - X, TargetPoint.Y - Y, TargetPoint.Z);
	}

	// 获取左右2边的坐标(人物与怪物之间)
	template<typename T>
	static CONST Point		_GetSidePoint(_In_ CONST T* Tar, _In_ BOOL bLeft, float fdis = 3.0f)
	{
		Point RetPoint;
		Point TarPoint = Tar->GetPoint();
		Point CurPoint = GetPersonPoint();

		/*
			互相垂直两直线斜率的乘积=-1
			(y2-y1)(x2-x1) * (y-y1)(x-x1) = -1
			(x2-x1)(x-x1)+ (y2-y1) (y-y1) = 0
			y-y1= - (x2-x1)(x-x1)/(y2-y1) ......(1)
			又：设BC=a 即BC^2 = a^2
			(x-x1)^2+(y-y1)^2 = a^2 ......(2)
			将（1）代入（2）：
			(x-x1)^2 + {-(x2-x1)(x-x1)/(y2-y1)}^2 = a^2
			(x-x1)^2{1+(x2-x1)^2/(y2-y1)^2} = a^2
			(x-x1)^2 = a^2(y2-y1)^2 / [(x2-x1)^2+(y2-y1)^2]

			x = x1 ± a(y2-y1) /√[(x2-x1)^2+(y2-y1)^2]
			y = y1 ± a(x2-x1) /√[(x2-x1)^2+(y2-y1)^2]
			
		*/

		float h = Tar->GetDis();
		RetPoint.X = bLeft ? (TarPoint.X - fdis * (CurPoint.Y - TarPoint.Y) / h) : (TarPoint.X + fdis * (CurPoint.Y - TarPoint.Y) / h);
		RetPoint.Y = bLeft ? (TarPoint.Y - fdis * (CurPoint.X - TarPoint.X) / h) : (TarPoint.Y + fdis * (CurPoint.X - TarPoint.X) / h);
		RetPoint.Z = TarPoint.Z;
		return RetPoint;
	}

	// 获取怪物脸前的坐标
	template<typename T>
	static CONST Point		_GetFacePoint(_In_ CONST T* Tar, float fDis = 2.0f)
	{
#define PI (float)3.1415926535
		DWORD wAngle = ReadDWORD(Tar->GetBase() + 0x9A) & 0xFFFF;
		Point TargetPoint = Tar->GetPoint();
		wAngle = wAngle == 0 ? 1 : wAngle;
		int nQuadrant = (wAngle / 90) + (wAngle % 90 != 0 ? 1 : 0);
		wAngle = wAngle % 90 == 0 ? 90 : wAngle % 90;
		float X = fDis * cos((float)wAngle * PI / 180.0f) * 50.0f;
		float Y = fDis * sin((float)wAngle * PI / 180.0f) * 50.0f;
		// 面朝上, 从左开始转
		switch (nQuadrant)
		{
		case 1: // 第一象限
			TargetPoint.X += X;
			TargetPoint.Y += Y;
			break;
		case 2: // 第四象限
			TargetPoint.X -= Y; // 如果不是这样做的话, 那么就90°过后应该是用 cos(90 - Angle)
			TargetPoint.Y += X;
			break;
		case 3: // 第三象限
			TargetPoint.X -= X;
			TargetPoint.Y -= Y;
			break;
		case 4: // 第二象限
			TargetPoint.X += Y;
			TargetPoint.Y -= X;
			break;
		default:
			break;
		}

		return TargetPoint;
	}

	// 获取怪物屁股后面的坐标
	template<typename T>
	static CONST Point	_GetBehindPoint(_In_ CONST T* Tar, float fDis = 2.0f)
	{
#define PI (float)3.1415926535
		DWORD wAngle = ReadDWORD(Tar->GetBase() + 0x9A) & 0xFFFF;
		Point TargetPoint = Tar->GetPoint();
		wAngle = wAngle == 0 ? 1 : wAngle;
		int nQuadrant = (wAngle / 90) + (wAngle % 90 != 0 ? 1 : 0);
		wAngle = wAngle % 90 == 0 ? 90 : wAngle % 90;
		float X = fDis * cos((float)wAngle * PI / 180.0f) * 50.0f;
		float Y = fDis * sin((float)wAngle * PI / 180.0f) * 50.0f;
		// 面朝上, 从左开始转
		switch (nQuadrant)
		{
		case 1: // 第一象限
			TargetPoint.X -= X;
			TargetPoint.Y -= Y;
			break;
		case 2: // 第四象限
			TargetPoint.X += Y; // 如果不是这样做的话, 那么就90°过后应该是用 cos(90 - Angle)
			TargetPoint.Y -= X;
			break;
		case 3: // 第三象限
			TargetPoint.X += X;
			TargetPoint.Y += Y;
			break;
		case 4: // 第二象限
			TargetPoint.X -= Y;
			TargetPoint.Y += X;
			break;
		default:
			break;
		}

		return TargetPoint;
	}

	static bool SameSide(CONST Point& A, CONST Point& B, CONST Point& C, CONST Point& P);

	static bool PointinTriangle(CONST Point& A, CONST Point& B, CONST Point& C, CONST Point& P);

	static std::pair<Point, Point> GetLeftRightVec(CONST Point& v, float fAngle, float r);
protected:
	DWORD dwBase;
	float m_fDis;
};


#endif