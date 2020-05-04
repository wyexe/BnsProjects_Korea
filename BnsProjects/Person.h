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
	// �ȼ�
	DWORD		GetLevel() CONST;

	// ���ϵ�����
	DWORD		GetMoney() CONST;

	// ƣ��
	DWORD		GetVigour() CONST;

	// �Ƿ�ս��״̬!
	BOOL		IsWarOfStatus() CONST;

	// ����״̬
	em_WarStatus GetPersonStatus() CONST;

	// ����Ŀ��ID
	virtual DWORD		GetTargetId() CONST;

	// �Ƿ�����˽�����
	BOOL IsLoadProgressBar() CONST;

	// PK��
	DWORD GetPkBean() CONST;

	// ��ȡ���ﵱǰƵ��
	DWORD GetChannel() CONST;

	// �Ƿ����״̬
	BOOL IsPickStatus() CONST;

	// ��ȡ���������ID
	DWORD GetInviateId_By_Transaction() CONST;

	// �Ƿ����ƶ���
	BOOL IsMoving() CONST;

	// ��ǰ��ͼID
	DWORD GetMapId() CONST;

	//
	BOOL IsLoadingByUseSkill() CONST;

	//
	BOOL IsSustainabilityByUseSkill() CONST;

	virtual CONST Point GetPoint() CONST;

	// �Ƿ������
	BOOL IsPassDoor() CONST;

	// �Ƿ����
	enum em_LoseConnect_Reason
	{  
		em_LoseConnect_Reason_Normal,
		em_LoseConnect_Reason_LoseConnect,
		em_LoseConnect_Reason_SqueezeAccount,
		em_LoseConnect_Reason_Other
	};
	em_LoseConnect_Reason IsLoseConnect() CONST;

	// �Ƿ����ڼ�����Դ��
	BOOL IsLoadingMapRes() CONST;

	static CONST std::wstring GetText_By_LoseConnectReason(_In_ em_LoseConnect_Reason emReason);

	float Get2DDis(_In_ CONST Point& TarPoint) CONST;

	// �Ǳ�
	DWORD GetStarCoin() CONST;
private:

};

#endif