#ifndef __GITBNSPROJECTS_BNSPROJECTS_SOMEFUN_BAIXIANPRCTICEFIELD_BAIXIANPRCTICEFIELDSERVICES_H__
#define __GITBNSPROJECTS_BNSPROJECTS_SOMEFUN_BAIXIANPRCTICEFIELD_BAIXIANPRCTICEFIELDSERVICES_H__

#include "GameVariable.h"

class CPlayer;
class CBaiXianPracticeFieldServices : public MyTools::CClassInstance<CBaiXianPracticeFieldServices>
{
public:
	CBaiXianPracticeFieldServices() = default;
	~CBaiXianPracticeFieldServices() = default;

public:
	BOOL Begin() CONST;

private:
	// �ߵ�����������
	BOOL MoveToBaiXianPracticeField() CONST;

	// ��ʼѭ������
	BOOL BeginPracticeLoop(_In_ CONST ULONGLONG& ulTick) CONST;

	// �Ƿ�ʱ
	BOOL IsTimeOut(_In_ CONST ULONGLONG& ulTick) CONST;

	// ��ȡ�����������ĵ���Ӣ������infomation
	DWORD GetPracticeInfoAddrByClasses(_In_ em_Player_Classes emPlayerClasses) CONST;

	// ��ȡĳ��Ӣ�۵���������, �������=0
	DWORD GetPracticeStageByClasses(_In_ em_Player_Classes emPlayerClasses) CONST;

	// ��ȡĳ��Ӣ�۵Ľ�Ʒ����, ��ȡ���=0
	DWORD GetPracticePrizesByClasses(_In_ em_Player_Classes emPlayerClasses) CONST;
	// ��ȡ��Ʒ
	BOOL AcceptPracticePrizesByClassesAndIndex(_In_ em_Player_Classes emPlayerClasses, _In_ DWORD dwIndex) CONST;

	// �Ƿ���ѡ��Ӣ��
	BOOL  ExistShowChoiceHero() CONST;

	// �Ƿ���ֽ��㽱��
	BOOL  ExistShowTrainingResult() CONST;

	// ѡ���սӢ��
	BOOL ChoiceHero() CONST;

	// ��ʼɱ
	BOOL Kill(_In_ CONST CPlayer& Player) CONST;
private:
	DSIABLE_COPY_AND_ASSIGN(CBaiXianPracticeFieldServices);
};



#endif