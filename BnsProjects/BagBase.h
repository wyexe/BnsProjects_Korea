#ifndef __GITBNSPROJECTS_BNSPROJECTS_OBJECT_OTHEROBJECT_BAG_BAGBASE_H__
#define __GITBNSPROJECTS_BNSPROJECTS_OBJECT_OTHEROBJECT_BAG_BAGBASE_H__

#include "GameVariable.h"


class CGameUi;
class CBag
{
public:
	enum em_ItemColor
	{
		em_ItemColor_Gray = 0x1,		// ��ɫ
		em_ItemColor_White = 0x2,		// ��ɫ
		em_ItemColor_Green = 0x3,		// ��ɫ
		em_ItemColor_Blue = 0x4,		// ��ɫ
		em_ItemColor_Pink = 0x5,		// �ۺ�ɫ
		em_ItemColor_Gold = 0x6,		// ��ɫ
		em_ItemColor_Legend = 0x7		// ����
	};

	///////////װ���ṹ///////////////////////////////////////////////////////////////
	enum em_Equi_Type
	{
		em_Equi_Type_Weapon = 0x1,	// ����
		em_Equi_Type_Necklace = 0x7,	// ����
		em_Equi_Type_Earring = 0x3,	// ����
		em_Equi_Type_Ring = 0x6,	// ��ָ
		em_Equi_Type_Bracelet = 0x10,	// ����
		em_Equi_Type_Belt = 0x11, // ����
		em_Equi_Type_Diagrams1 = 0x8,	// 1�Ű���
		em_Equi_Type_Diagrams2 = 0x9,	// 2�Ű���
		em_Equi_Type_Diagrams3 = 0xA,	// 3�Ű���
		em_Equi_Type_Diagrams4 = 0xB,	// 4�Ű���
		em_Equi_Type_Diagrams5 = 0xC,	// 5�Ű���
		em_Equi_Type_Diagrams6 = 0xD,	// 6�Ű���
		em_Equi_Type_Diagrams7 = 0xE,	// 7�Ű���
		em_Equi_Type_Diagrams8 = 0xF,	// 8�Ű���
		em_Equi_Type_Soul	   = 0x16,  // ��
		em_Equi_Type_Cloth		= 0x2,	// �·�
		em_Equi_Type_Hat		= 0x5,  // ñ��
		em_Equi_Type_Glasses	= 0x4,   // �۾�
		em_Equi_Type_LatestFashion = 0x12, // ʱװ
	};
public:
	// ��ȡ��Ʒ����!
	static std::wstring GetItemName(_In_ DWORD dwItemObj);

	// ������ƷID������ƷOBJ
	static DWORD    GetItemObj_By_ID(_In_ DWORD dwItemId);

	// ж�ر�ʯ
	static BOOL		RemoveGem_By_Index(int nIndex);
public:
	explicit CBag();
	explicit CBag(DWORD dwBagBase, UINT uIndex);
	~CBag();

	// Base 
	inline DWORD GetBase() CONST;

	// Id
	DWORD		GetGId() CONST;

	// Parm
	DWORD		GetSId() CONST;

	// Count ����
	DWORD		GetCount() CONST;

	// Name  ��Ʒ����
	CONST std::wstring&	GetName() CONST;
	VOID		SetName() CONST;
	CONST std::wstring	GetColorName() CONST;	// ��ɫ����

	// Color
	em_ItemColor GetColor() CONST;

	// Index
	UINT		GetIndex() CONST; // ����λ��

	// TypeName ���Ͻǵ�����
	CONST std::wstring	GetTypeName() CONST;
	CONST std::wstring	GetTypeName_Dlg() CONST;

	// Explain �ֽ�
	BOOL		Explain() CONST;

	// ItemType ����
	static DWORD GetItemType_By_GId(_In_ DWORD dwGId);
	DWORD		GetItemType() CONST;
	DWORD		GetItemType_Dlg() CONST;
	BOOL		IsEquiment() CONST;

	// UseItem	ʹ����Ʒ
	BOOL		UseItem() CONST;

	// OpenBox	������
	BOOL		OpenBox() CONST;

	// Enhanced	ǿ�� Parm1=Ҫ�ɳ�װ����Sid , Parm2=�ɳ���֮��Ķ���
	BOOL		EquiEnhanced(_In_ DWORD dwGrowthItemSId, _In_ DWORD dwLuckyCharmSId = NULL) CONST;

	// Evolution	װ��5������6
	BOOL		EquiEvolution(_In_ CONST CBag& Weaspon, _In_ DWORD dwEvolutionId) CONST;

	//  װ��10������1
	BOOL		EquiEndEvolution(_In_ DWORD dwGrowthItemSId, _In_ DWORD dwEvoltionId) CONST;

	// UntieEqui ��ӡ
	BOOL		EquiUntie(_In_ DWORD dwUntieId, _In_ DWORD dwUntieCount, _In_ DWORD nType) CONST;

	// OpenBox By Key ��Կ�׿�����
	BOOL		OpenBox_By_Key(_In_ DWORD dwKeyId, _In_ DWORD dwKeyCount) CONST;

	// ��װ��
	BOOL		WearEqui() CONST;

	// Discard ��
	BOOL		DiscardItem() CONST;

	// ��Ƕ��ʯ (Ҫ�ж�һ�������ǵڼ��ű�ʯ)
	BOOL		SetGem(_In_ UINT uIndex, _In_ CONST CGameUi& Inventory2Panel) CONST;

	// ���ý�����Ʒ
	BOOL		PushTransactionItem(_In_ DWORD TradeShelfPanelObj,  _In_ DWORD dwCount) CONST;

	// ��װ��
	BOOL		RelieveEqui() CONST;

	// ��ȡ/�����ʼ�����
	UINT		GetMailCount() CONST;
	VOID		SetMailCount(_In_ UINT uMailCount_);

	// �Ƿ�ǿ������
	BOOL		IsGrowthed() CONST;

	// ǿ������
	BOOL		GrowthBaGua(_In_ DWORD dwSacrificeSid) CONST;

	// ǿ�������Ե�����ID(���������GetBaGuaAttribute ���Է���ǿ�������Ե�ֵ������)
	DWORD		GetExtendBaGuaAttributeId() CONST;
private:
	DWORD			m_dwBagBase;
	UINT			m_uIndex;
	UINT			uMailCount;
	mutable	std::wstring Name;
};


#endif