#ifndef __GITBNSPJRECTS_CONSOLEDLL_FUNCTION_CONSOLECARD_H__
#define __GITBNSPJRECTS_CONSOLEDLL_FUNCTION_CONSOLECARD_H__

#include "ConsoleVariable.h"
#include <MyTools/ClassInstance.h>
#include "../BnsKorServer_IOCP/ServerCommon.h"
class CConsoleCard : public MyTools::CClassInstance<CConsoleCard>
{
public:
	CConsoleCard();
	~CConsoleCard() = default;

	// 返回一张没有被使用的卡号, 并且将这张卡号设置为已经使用的状态
	BOOL GetNotUseCard(_In_ CardInfo& CardInfo_);

	// 是否存在wsCardNo, 存在的话就会调用fnPtr
	BOOL ExistCard(_In_ CONST std::wstring& wsCardNo, _In_ std::function<void(CardInfo&)> fnPtr);

	// 是否存在CardCRC, 存在的话就会调用fnPtr
	BOOL ExistCard(_In_ DWORD dwCardCrc, _In_ std::function<void(CardInfo&)> fnPtr);

	// 获取卡号数量
	UINT GetCardCount() CONST;

	// 添加卡号
	VOID AddCard(_In_ CONST CardInfo& CardInfo_);

	// 遍历所有卡号
	VOID ForEach(_In_ std::function<VOID(CardInfo&)> fnPtr);

	// 删除卡号
	VOID RemoveCard(_In_ std::function<BOOL(CONST CardInfo&)>);

	// 获取
	std::vector<CardInfo>& GetData();
private:
	// 需要在使用前加锁!
	CardInfo* ExistCard(_In_ std::function<BOOL(CONST CardInfo&)> fnPred);
private:
	std::vector<CardInfo> VecCard;
	MyTools::CLLock LockVecCard;
};



#endif