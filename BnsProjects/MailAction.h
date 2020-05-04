#ifndef __GITBNSPROJECTS_BNSPROJECTS_SOMEFUN_MAIL_MAILACTION_H__
#define __GITBNSPROJECTS_BNSPROJECTS_SOMEFUN_MAIL_MAILACTION_H__

#include "GameVariable.h"

class CGameUi;
class CBag;
class CMailAction : public MyTools::CClassInstance<CMailAction>
{
public:
	explicit CMailAction() = default;
	~CMailAction() = default;

public:
	// 邮寄
	struct MailItemContent
	{
		std::wstring wsItemName;
		DWORD		 dwCount;
		MailItemContent(_In_ CONST std::wstring& wsItemName_) : wsItemName(wsItemName_), dwCount(0){}
		MailItemContent(_In_ CONST std::wstring& wsItemName_, _In_ UINT dwCount_) : wsItemName(wsItemName_), dwCount(dwCount_){}
	}; 

	// 收货邮寄
	BOOL Mail(_In_ CONST std::wstring& wsNpcName, _In_ CONST std::vector<MailItemContent>& ItemStrList, _In_ DWORD dwSurplusMoney) CONST;

	// 物资邮寄
	BOOL MailItemSupport(_In_ CONST std::wstring& wsNpcName, _In_ CONST std::wstring& wsPlayerName, _In_ CONST std::vector<MailItemContent>& ItemStrList, _In_ DWORD dwMailMoney) CONST;

	BOOL ExistMailItem(_In_ const std::vector<std::wstring>& ItemStrList) CONST;

	// 收邮件
	BOOL		RecvMail() CONST;
	BOOL		RecvEmptyMail(_In_ DWORD DeliveryPanelObj) CONST;
	DWORD		GetMailCount() CONST;
	DWORD		GetMailParm(_In_ DWORD dwCompValue) CONST;
	BOOL		SetRecvMailItem(_In_ DWORD DeliveryPanelObj) CONST;

	// 获取邮件标题
	BOOL		GetMailTitle(_In_ DWORD dwIndex, _Out_opt_ std::wstring& wsTitle) CONST;

	// 礼箱
	BOOL		RecvUnOpenGift(_In_ CONST std::vector<std::wstring>& FilterNameVec) CONST;
	BOOL		RecvOpenGift(_In_ CONST std::vector<std::wstring>& FilterNameVec) CONST;
private:
	UINT		SetMailSupportItemToList(_In_ std::vector<MailItemContent>& ItemStrList, _Out_opt_ std::vector<CBag>& VecBag) CONST;
	UINT		SetMailItemToList(_In_ CONST std::vector<MailItemContent>& ItemStrList, _Out_opt_ std::vector<CBag>& VecBag) CONST;
private:
	DSIABLE_COPY_AND_ASSIGN(CMailAction)
};


#endif