#ifndef __BNS_PLAYERKILL_H__
#define __BNS_PLAYERKILL_H__

#include "GameVariable.h"

typedef enum _em_StartPK_Status
{
	em_StartPK_Status_None,		// ��û��PK��ť
	em_StartPK_Status_Wait,		// �Ѿ�����PK��, �ȴ�ƥ��
	em_StartPK_Status_Done		// ƥ�䵽�����!
}em_StartPK_Status;

typedef enum _em_PKing_Status
{
	em_PKing_Status_Load,		
	em_PKing_Status_Lose,		// ս������
	em_PKing_Status_Ready,		// ׼��
	em_PKing_Status_PK,			// PKing
	em_PKing_Status_Wait,		// ��ͼ���
}em_PKing_Status;

class CPlayer;
class CPlayerKill : public MyTools::CClassInstance<CPlayerKill>
{
public:
	explicit CPlayerKill();
	~CPlayerKill() = default;

	BOOL StartPK();

	// �����Ƿ������
	static BOOL IsFinishPK();
public:
	// ��ȡPK�����״̬
	em_StartPK_Status	GetStartPKStatus();

	// ��ȡPK�е�״̬
	em_PKing_Status		GetPKingStatus();

	// ��ʼƥ��PK���
	BOOL ClickPK();

	// �뿪PK����
	BOOL ClickExit();

	// ս������PK
	BOOL ClickEndPK();

	// ׼��Pk
	BOOL ClickReadyPk();

	// �Ƿ���PK������!
	BOOL IsExistPKRoom();

	// �Ƿ�PK��ͼ�С���
	BOOL IsLoadPkRoom();

	BOOL Zy_IsLoadPkRoom();

	BOOL Zy_IsExistPKRoom();

	// ����PK����
	BOOL InToPkRoom();

	// �Ƿ����PK����
	BOOL IsFinishPK_Condition();

	// ��ʼƥ��PK���
	BOOL StartMatchingPlayer();

	// PK��׼������
	BOOL BeforePK();

	// ��ʼPKս��
	BOOL FightPK();

	// PK����
	BOOL EndPK();

	BOOL KillPlayer(_In_ DWORD dwBean, _In_ CPlayer* pPlayer);

	// ����ʯ����
	BOOL UseItem_By_Pk();
private:
	DSIABLE_COPY_AND_ASSIGN(CPlayerKill)
private:
	BOOL bFirst;
	BOOL bMatchResult;
};


#endif