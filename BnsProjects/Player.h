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
	// ��ȡְҵ
	em_Player_Classes	GetClasses() CONST;

	// ��ȡְҵ��ת����Text
	CONST std::wstring			GetClassesText() CONST;

	// ���뽻��
	BOOL				InviteTansaction() CONST;

	// ���ú�����ְҵ��XX��ʱ��
	static BOOL			FindPlayerclassesText(_Out_opt_ PlayerclassesText* pPlayerclassesText, _In_ std::function<BOOL(CONST PlayerclassesText&)> fnFind);
	static CONST std::wstring		GetTextByPlayerClasses(_In_ em_Player_Classes emPlayerClasses);
public:
	//////�Լ���ĺ���////////////////////////////////////////////////////////////////////

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

	// Ŀ��
	virtual DWORD	GetTargetId() CONST;

	// ״̬
	virtual em_FightStatus GetFightStatus() CONST;

	bool operator < (__in const CPlayer& GameObj);

	// ����= �����Sleep x����ȥ�жϽ�����
	virtual BOOL	Take(DWORD dwSleepTime = 2 * 1000) CONST;

	// ս������(���ڻ���ȭ�ڵ�����)
	virtual em_Player_Pose	GetFightPose() CONST;

	virtual BOOL	IsUseSkilling() CONST;

	virtual em_WarStatus GetAbNormalStatus() CONST;
public:
	/////������̳е��麯��/////////////////////////////////////////////////////////////////////
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
	/// �������
	static BOOL			InviteTeamMember(DWORD dwPlayerId);
private:
	CONST static std::vector<PlayerclassesText> vPlayerclassesText;
protected:
	mutable std::wstring Name;
	DWORD	m_dwOldMaxHp;
	GameObjectID m_OldObjectId;
};


#endif