#ifndef __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_CARD_CARDEXTEND_H__
#define __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_CARD_CARDEXTEND_H__

#include <map>
#include <MyTools/CLLock.h>
#include <MyTools/ClassInstance.h>
#include "ServerCommon.h"

class CCardExtend : public MyTools::CClassInstance<CCardExtend>
{
public:
	CCardExtend() : _Lock(L"CCardExtend.Lock") {};
	~CCardExtend() = default;

	// ��ʼ��, ��ȡ���ݿ���������Ч(��Чʱ��>0)�Ŀ��ŵ��ڴ�
	BOOL Initialize();

	// ��������
	BOOL FindCard_By_CRC(_In_ DWORD dwCardCrc, _Out_ CardInfo& CardInfo_) CONST;
	BOOL FindCard_By_Condition(_Out_ CardInfo& CardInfo_, _In_ std::function<BOOL(CONST CardInfo&)> Finder) CONST;

	// ��������
	BOOL FindCard_By_CardNo(_In_ CONST std::wstring& wsCardNo, _Out_ CardInfo& Card) CONST;

	// �޸Ŀ�����ص���Ϣ
	BOOL SetCardContent(_In_ DWORD dwCardCrc, _In_ std::function<VOID(CardInfo&)> ActionPtr);

	// �жϸÿ��Ƿ���Ч
	BOOL IsCardEffective(_In_ CONST ClientAccountContent& AccountContent, _Out_ std::wstring& wsErrText) CONST;

	// ���Լ���� -> �ɹ������TRUE, ���Ұѿ�����Ϣ��ӵ��ڴ�
	BOOL TryActiveCard(_In_ CONST ClientAccountContent& AccountContent, _In_ DWORD dwCrc, _Out_ CardInfo& Card);

	// �Ա�Token�Ϳ��ŵ�Token
	BOOL ComparedToken(_In_ DWORD dwCardCrc, _In_ DWORD dwToken) CONST;

	//
	VOID Print();
private:
	MyTools::CLLock _Lock;
	std::map<DWORD, CardInfo> _MapCard;
};



#endif // !__GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_CARD_CARDEXTEND_H__
