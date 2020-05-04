#ifndef __GITBNSPROJECTS_OBJECT_GAMEOBJECT_PLAYER_H__
#define __GITBNSPROJECTS_OBJECT_GAMEOBJECT_PLAYER_H__

#include "BaseObject.h"


class CPlayer : public CBaseObject
{
public:
	explicit CPlayer();
	explicit CPlayer(DWORD dwBase_);
	~CPlayer();

public:
	// 获取职业
	em_Player_Classes	GetClasses() CONST;

	// 获取职业并转换成Text
	CONST std::wstring			GetClassesText() CONST;

	// 邀请交易
	BOOL				InviteTansaction() CONST;

	// 调用函数当职业是XX的时候
	static BOOL			FindPlayerclassesText(_Out_opt_ PlayerclassesText* pPlayerclassesText, _In_ std::function<BOOL(CONST PlayerclassesText&)> fnFind);
	static CONST std::wstring		GetTextByPlayerClasses(_In_ em_Player_Classes emPlayerClasses);
public:
	//////自己类的函数////////////////////////////////////////////////////////////////////

	// HP
	virtual DWORD	GetHp() CONST;
	virtual DWORD	GetMaxHp() CONST;
	virtual DWORD	GetPercentHp() CONST;

	// Mp
	virtual DWORD	GetMp() CONST;

	// Dead
	virtual BOOL	IsDead() CONST;

	// Camp
	virtual em_Camp	GetCamp() CONST;
	
	// Point
	virtual Point	GetBehindPoint(_In_ float fDis) CONST;
	virtual Point	GetFrontPoint(_In_ float fDis) CONST;
	virtual Point	GetLeftPoint(_In_ float fDis) CONST;
	virtual Point	GetRightPoint(_In_ float fDis) CONST;
	virtual Point	GetFacePoint(float fDis) CONST;

	// 目标
	virtual DWORD	GetTargetId() CONST;

	// 状态
	virtual em_FightStatus GetFightStatus() CONST;

	bool operator < (__in const CPlayer& GameObj);

	// 参数= 捡物后Sleep x秒后才去判断进度条
	virtual BOOL	Take(DWORD dwSleepTime = 2 * 1000) CONST;

	// 战斗姿势(气宗还是拳宗的姿势)
	virtual em_Player_Pose	GetFightPose() CONST;

	virtual BOOL	IsUseSkilling() CONST;

	virtual em_WarStatus GetAbNormalStatus() CONST;
public:
	/////基类里继承的虚函数/////////////////////////////////////////////////////////////////////
	virtual CONST std::wstring& GetName() CONST;

	virtual VOID	 SetName() CONST;

	virtual CONST Point	GetPoint() CONST;

	virtual float   GetDis() CONST;

	virtual float	GetDis(_In_ CONST Point& TarPoint) CONST;

	virtual BOOL	IsExist() CONST;

	virtual void	SetAngle() CONST;

	VOID			SetRoundAngle() CONST;

	float  GetAngle() CONST;

	virtual void SetBase(_In_ DWORD dwBase_);
public:
	/// 邀请入队
	static BOOL			InviteTeamMember(DWORD dwPlayerId);
private:
	CONST static std::vector<PlayerclassesText> vPlayerclassesText;
protected:
	mutable std::wstring Name;
	DWORD	m_dwOldMaxHp;
	GameObjectID m_OldObjectId;
};


#endif