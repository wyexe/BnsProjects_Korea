#ifndef __BNS_NPC_H__
#define __BNS_NPC_H__

#include "Player.h"

class CBag;
class CNpc : public CPlayer
{
public:
	explicit CNpc(__in DWORD dwNpcBase);
	explicit CNpc();
	~CNpc();

	// NpcType -> 0xE
	DWORD	GetNpcType() CONST;

	// NpcName
	virtual CONST std::wstring& GetName() CONST;
	virtual VOID SetName() CONST;

	// Exist
	virtual BOOL	IsExist() CONST;

	// sort
	bool	operator < (_In_ const CNpc& GameObj) CONST;

public:
	// À©³ä²Ö¿â±³°ü
	BOOL	ExpandInvenSlot() CONST;
	BOOL	ExpandClothSlot() CONST;

	// ´æ²Ö¿â
	BOOL	SaveItem(_In_ DWORD dwItemSID, _In_ DWORD dwCount, _In_ DWORD dwWareHourseSID) CONST;

	// ¼ñNpc
	BOOL	TakeNpcItem() CONST;

	// ÊÕÓÊ¼þ
	BOOL	RecvMail(_In_ DWORD dwDeliveryPanelObj, _In_ DWORD dwMailParm) CONST;

	// Mail
	BOOL	Mail(_In_ LPCWSTR pwszPlayerName, _In_ DWORD dwMoney, _In_ std::vector<CBag>& vlst) CONST;

	// Buy
	BOOL	BuyItem(_In_ DWORD dwItemIndex/*´Ó0¿ªÊ¼*/, _In_ DWORD dwCount) CONST;

	// Sell
	BOOL	Sell(_In_ CONST CBag& Item) CONST;
};



#endif