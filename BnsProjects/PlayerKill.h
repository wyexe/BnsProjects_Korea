#ifndef __BNS_PLAYERKILL_H__
#define __BNS_PLAYERKILL_H__

#include "GameVariable.h"

typedef enum _em_StartPK_Status
{
	em_StartPK_Status_None,		// 还没点PK按钮
	em_StartPK_Status_Wait,		// 已经点了PK了, 等待匹配
	em_StartPK_Status_Done		// 匹配到玩家了!
}em_StartPK_Status;

typedef enum _em_PKing_Status
{
	em_PKing_Status_Load,		
	em_PKing_Status_Lose,		// 战斗结束
	em_PKing_Status_Ready,		// 准备
	em_PKing_Status_PK,			// PKing
	em_PKing_Status_Wait,		// 过图完毕
}em_PKing_Status;

class CPlayer;
class CPlayerKill : public MyTools::CClassInstance<CPlayerKill>
{
public:
	explicit CPlayerKill();
	~CPlayerKill() = default;

	BOOL StartPK();

	// 任务是否完成了
	static BOOL IsFinishPK();
public:
	// 获取PK房间的状态
	em_StartPK_Status	GetStartPKStatus();

	// 获取PK中的状态
	em_PKing_Status		GetPKingStatus();

	// 开始匹配PK玩家
	BOOL ClickPK();

	// 离开PK房间
	BOOL ClickExit();

	// 战斗结束PK
	BOOL ClickEndPK();

	// 准备Pk
	BOOL ClickReadyPk();

	// 是否在PK房间中!
	BOOL IsExistPKRoom();

	// 是否PK读图中……
	BOOL IsLoadPkRoom();

	BOOL Zy_IsLoadPkRoom();

	BOOL Zy_IsExistPKRoom();

	// 进入PK房间
	BOOL InToPkRoom();

	// 是否完成PK条件
	BOOL IsFinishPK_Condition();

	// 开始匹配PK玩家
	BOOL StartMatchingPlayer();

	// PK的准备工作
	BOOL BeforePK();

	// 开始PK战斗
	BOOL FightPK();

	// PK结束
	BOOL EndPK();

	BOOL KillPlayer(_In_ DWORD dwBean, _In_ CPlayer* pPlayer);

	// 开灵石袋子
	BOOL UseItem_By_Pk();
private:
	DSIABLE_COPY_AND_ASSIGN(CPlayerKill)
private:
	BOOL bFirst;
	BOOL bMatchResult;
};


#endif