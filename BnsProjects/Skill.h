#ifndef __BNS_SKILL_H__
#define __BNS_SKILL_H__

#include "GameVariable.h"
class CGameUi;
class CSkill
{
public:
	explicit	CSkill(DWORD dwSkillBase);
	~CSkill();

	// Base
	inline	DWORD GetBase() CONST;
	inline	VOID  SetBase(DWORD dwBase_){ m_dwSkillBase = dwBase_; };

	// Id
	DWORD	GetId() CONST;

	// Name
	CONST std::wstring& GetName() CONST;
	VOID	  SetName();

	// UseSkill
	BOOL	UseSkill() CONST;

	// ��ȴ
	BOOL	IsCooldown();

public:
	// ʹ�ü���
	static BOOL UseSkill_By_Id(_In_ DWORD dwSkillId, _In_ BOOL bNeedResult = FALSE, em_Skill_Type emSkillType = em_Skill_Type_Person);
	// ѧϰ����
	static BOOL StudySkill(_In_ CONST std::vector<LearnSkillContent>& vlst, _In_ CONST DWORD Ntype);
	// ��������
	static BOOL ForgotSkill() = delete;
	// ��ȡ���ܵ�
	static DWORD GetSkillPoint() = delete;
public:

	// Cooldown
	DWORD	GetCooldownParm();
	DWORD	GetCooldown();

private:
	DWORD m_dwSkillBase;
	DWORD m_dwSkillParm;
	DWORD m_dwSkillObj;
	DWORD m_dwCooldownParm;
	mutable std::wstring Name;
};




#endif