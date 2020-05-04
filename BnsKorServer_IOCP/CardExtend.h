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

	// 初始化, 读取数据库内所有有效(有效时间>0)的卡号到内存
	BOOL Initialize();

	// 搜索卡号
	BOOL FindCard_By_CRC(_In_ DWORD dwCardCrc, _Out_ CardInfo& CardInfo_) CONST;
	BOOL FindCard_By_Condition(_Out_ CardInfo& CardInfo_, _In_ std::function<BOOL(CONST CardInfo&)> Finder) CONST;

	// 搜索卡号
	BOOL FindCard_By_CardNo(_In_ CONST std::wstring& wsCardNo, _Out_ CardInfo& Card) CONST;

	// 修改卡号相关的信息
	BOOL SetCardContent(_In_ DWORD dwCardCrc, _In_ std::function<VOID(CardInfo&)> ActionPtr);

	// 判断该卡是否还有效
	BOOL IsCardEffective(_In_ CONST ClientAccountContent& AccountContent, _Out_ std::wstring& wsErrText) CONST;

	// 尝试激活卡号 -> 成功激活返回TRUE, 并且把卡号信息添加到内存
	BOOL TryActiveCard(_In_ CONST ClientAccountContent& AccountContent, _In_ DWORD dwCrc, _Out_ CardInfo& Card);

	// 对比Token和卡号的Token
	BOOL ComparedToken(_In_ DWORD dwCardCrc, _In_ DWORD dwToken) CONST;

	//
	VOID Print();
private:
	MyTools::CLLock _Lock;
	std::map<DWORD, CardInfo> _MapCard;
};



#endif // !__GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_CARD_CARDEXTEND_H__
