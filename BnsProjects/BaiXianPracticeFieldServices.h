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
	// 走到白仙修练场
	BOOL MoveToBaiXianPracticeField() CONST;

	// 开始循环修炼
	BOOL BeginPracticeLoop(_In_ CONST ULONGLONG& ulTick) CONST;

	// 是否超时
	BOOL IsTimeOut(_In_ CONST ULONGLONG& ulTick) CONST;

	// 获取白仙修练场的单独英雄修炼infomation
	DWORD GetPracticeInfoAddrByClasses(_In_ em_Player_Classes emPlayerClasses) CONST;

	// 获取某个英雄的修炼进度, 修炼完毕=0
	DWORD GetPracticeStageByClasses(_In_ em_Player_Classes emPlayerClasses) CONST;

	// 获取某个英雄的奖品进度, 获取完毕=0
	DWORD GetPracticePrizesByClasses(_In_ em_Player_Classes emPlayerClasses) CONST;
	// 领取奖品
	BOOL AcceptPracticePrizesByClassesAndIndex(_In_ em_Player_Classes emPlayerClasses, _In_ DWORD dwIndex) CONST;

	// 是否在选择英雄
	BOOL  ExistShowChoiceHero() CONST;

	// 是否出现结算奖励
	BOOL  ExistShowTrainingResult() CONST;

	// 选择对战英雄
	BOOL ChoiceHero() CONST;

	// 开始杀
	BOOL Kill(_In_ CONST CPlayer& Player) CONST;
private:
	DSIABLE_COPY_AND_ASSIGN(CBaiXianPracticeFieldServices);
};



#endif