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

	// ����һ��û�б�ʹ�õĿ���, ���ҽ����ſ�������Ϊ�Ѿ�ʹ�õ�״̬
	BOOL GetNotUseCard(_In_ CardInfo& CardInfo_);

	// �Ƿ����wsCardNo, ���ڵĻ��ͻ����fnPtr
	BOOL ExistCard(_In_ CONST std::wstring& wsCardNo, _In_ std::function<void(CardInfo&)> fnPtr);

	// �Ƿ����CardCRC, ���ڵĻ��ͻ����fnPtr
	BOOL ExistCard(_In_ DWORD dwCardCrc, _In_ std::function<void(CardInfo&)> fnPtr);

	// ��ȡ��������
	UINT GetCardCount() CONST;

	// ��ӿ���
	VOID AddCard(_In_ CONST CardInfo& CardInfo_);

	// �������п���
	VOID ForEach(_In_ std::function<VOID(CardInfo&)> fnPtr);

	// ɾ������
	VOID RemoveCard(_In_ std::function<BOOL(CONST CardInfo&)>);

	// ��ȡ
	std::vector<CardInfo>& GetData();
private:
	// ��Ҫ��ʹ��ǰ����!
	CardInfo* ExistCard(_In_ std::function<BOOL(CONST CardInfo&)> fnPred);
private:
	std::vector<CardInfo> VecCard;
	MyTools::CLLock LockVecCard;
};



#endif