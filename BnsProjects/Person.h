#ifndef __GITBNSPROJECTS_OBJECT_GAMEOBJECT_PERSON_H__
#define __GITBNSPROJECTS_OBJECT_GAMEOBJECT_PERSON_H__

#include "Player.h"
#define EMPTY_PERSONHP					CPerson::GetInstance().IsDead()



class CPerson : public CPlayer
{
public:
	explicit CPerson();
	CPerson(DWORD dwBase_);
	~CPerson();
public:
	enum em_Person_Buff
	{
		em_Person_Buff_Skill_3 = 0x18E6C6A
	};
public:
	static CPerson&	GetInstance();
public:
	// 等级
	DWORD		GetLevel() CONST;

	// 身上的银两
	DWORD		GetMoney() CONST;

	// 疲劳
	DWORD		GetVigour() CONST;

	// 是否战斗状态!
	BOOL		IsWarOfStatus() CONST;

	// 人物状态
	em_WarStatus GetPersonStatus() CONST;

	// 人物目标ID
	virtual DWORD		GetTargetId() CONST;

	// 是否出现了进度条
	BOOL IsLoadProgressBar() CONST;

	// PK豆
	DWORD GetPkBean() CONST;

	// 获取人物当前频道
	DWORD GetChannel() CONST;

	// 是否捡物状态
	BOOL IsPickStatus() CONST;

	// 获取交易邀请的ID
	DWORD GetInviateId_By_Transaction() CONST;

	// 是否在移动中
	BOOL IsMoving() CONST;

	// 当前地图ID
	DWORD GetMapId() CONST;

	//
	BOOL IsLoadingByUseSkill() CONST;

	//
	BOOL IsSustainabilityByUseSkill() CONST;

	virtual CONST Point GetPoint() CONST;

	// 是否过门中
	BOOL IsPassDoor() CONST;

	// 是否掉线
	enum em_LoseConnect_Reason
	{  
		em_LoseConnect_Reason_Normal,
		em_LoseConnect_Reason_LoseConnect,
		em_LoseConnect_Reason_SqueezeAccount,
		em_LoseConnect_Reason_Other
	};
	em_LoseConnect_Reason IsLoseConnect() CONST;

	// 是否正在加载资源中
	BOOL IsLoadingMapRes() CONST;

	static CONST std::wstring GetText_By_LoseConnectReason(_In_ em_LoseConnect_Reason emReason);

	float Get2DDis(_In_ CONST Point& TarPoint) CONST;

	// 星币
	DWORD GetStarCoin() CONST;
private:

};

#endif