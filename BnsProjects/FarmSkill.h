#ifndef __GITBNSPROJECTS_BNSPROJECTS_OBJECTS_FARMSKILL_FARSKILL_H__
#define __GITBNSPROJECTS_BNSPROJECTS_OBJECTS_FARMSKILL_FARSKILL_H__

#include "GameVariable.h"
#include <MyTools/CLLock.h>

class CSkill;
class CUiSkill;
class CPlayer;
class CFarmSkill : public MyTools::CClassInstance<CFarmSkill>
{
public:
	CFarmSkill() ;
	~CFarmSkill() = default;

public:

	// ʹ����ʿ��E����,��û��ȴ��Ϸ���FALSE
	BOOL UseSkill_By_Destoer_E();

	// ʹ����ʿ��Q����, ��û��ȴ��Ϸ���FALSE
	BOOL UseSkill_By_Destoer_Q();

	// �жϼ����Ƿ� ����ʹ��
	BOOL IsActive_By_SkillId_In_Ui(_In_ DWORD dwSkillId) CONST;

	// �жϼ���ID�Ƿ���ȴ���(������ȴCALL+UI)
	BOOL IsCool_By_SkillId_In_Ui(_In_ DWORD dwSkillId);

	// �жϼ���ID�Ƿ���� ��UI��
	BOOL IsExist_By_SkillId_In_Ui(_In_ DWORD dwSkillId) CONST;

	// �жϼ���ID�Ƿ���ȴ���(������ȴCALL+UI) , ����2��ʾ �Ƿ���Լ����Ƿ�����
	BOOL IsCool_By_SkillId_In_Ui(_In_ DWORD dwSkillId, _In_ BOOL bPassSkillActive);

	// ʹ�ü���_ById(�ȴ�ʩ�����)
	BOOL UseSkill_By_SkillId(_In_ DWORD dwSkillId, _In_ em_Skill_Type emSkillType = em_Skill_Type_Person);
	BOOL UseSkill_By_SkillId_WhichCool(_In_ DWORD dwSkillId, _In_ em_Skill_Type emSkillType = em_Skill_Type_Person);

	// ʹ�ü���_ById(���������Ƿ�ȴ�ʩ�����)
	BOOL UseSkill_BySkillId_In_Sleep(_In_ DWORD dwSkillId, _In_ BOOL bNeedResult = TRUE, _In_ em_Skill_Type emSkillType = em_Skill_Type_Person);

	// ��ѯ���� By Name
	BOOL	GetSkill_By_Name(_In_ CONST std::wstring& wsSkillName, _Out_opt_ CSkill* pSkill = nullptr) CONST;
	BOOL	GetSkill_By_Id(_In_ DWORD dwSkillId, _Out_opt_ CSkill* pSkill = nullptr) CONST;
	BOOL	GetSkill_By_Condiction(_In_ std::function<BOOL(CONST CSkill&)> fn, _Out_opt_ CSkill* pSkill = nullptr) CONST;

	// ��ѯ���� By ID In Ui
	BOOL	GetUiSkill_By_Id(_In_ DWORD dwSkillId, _Out_opt_ CUiSkill* pUiSkill) CONST;

	// ���ؼ��ܣ�����=ְҵ��
	BOOL UseFloorSkill(_In_ em_Player_Classes emPlayerClasses);

	// ʹ��SS(���÷�����ְҵ)
	BOOL UseSkill_By_SS();

	// �ж����ؼ���ϵ���Ƿ���ȴ���
	BOOL IsCool_By_SkillId_In_HideUi(_In_ DWORD dwSkillId);

	// �ж��Ƿ����QTE��
	BOOL ExistQTE() CONST;

	// �ж��Ƿ񻨷ۼ��� ����
	BOOL IsUnEnableSkill3() CONST;

	// �ȴ�������ȴ
	VOID WaitForCool_By_SkillId(DWORD dwSkillId);

	// ʹ�ô���
	VOID UseULT();

	// �����õļ���
	VOID AddDisableSkillId(_In_ DWORD dwSkillId);
	VOID RemoveDisableSkillId(_In_ DWORD dwSkillId);
	BOOL IsDisableSkill(_In_ DWORD dwSkillId);

private:
	std::mutex _MtxSetDisbaleSkillId;
	std::set<DWORD> _SetDisableSkillId;
private:
	DSIABLE_COPY_AND_ASSIGN(CFarmSkill)
};


#endif