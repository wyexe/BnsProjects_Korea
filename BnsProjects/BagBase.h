#ifndef __GITBNSPROJECTS_BNSPROJECTS_OBJECT_OTHEROBJECT_BAG_BAGBASE_H__
#define __GITBNSPROJECTS_BNSPROJECTS_OBJECT_OTHEROBJECT_BAG_BAGBASE_H__

#include "GameVariable.h"


class CGameUi;
class CBag
{
public:
	enum em_ItemColor
	{
		em_ItemColor_Gray = 0x1,		// 灰色
		em_ItemColor_White = 0x2,		// 白色
		em_ItemColor_Green = 0x3,		// 绿色
		em_ItemColor_Blue = 0x4,		// 蓝色
		em_ItemColor_Pink = 0x5,		// 粉红色
		em_ItemColor_Gold = 0x6,		// 金色
		em_ItemColor_Legend = 0x7		// 传送
	};

	///////////装备结构///////////////////////////////////////////////////////////////
	enum em_Equi_Type
	{
		em_Equi_Type_Weapon = 0x1,	// 武器
		em_Equi_Type_Necklace = 0x7,	// 项链
		em_Equi_Type_Earring = 0x3,	// 耳环
		em_Equi_Type_Ring = 0x6,	// 戒指
		em_Equi_Type_Bracelet = 0x10,	// 手镯
		em_Equi_Type_Belt = 0x11, // 腰带
		em_Equi_Type_Diagrams1 = 0x8,	// 1号八卦
		em_Equi_Type_Diagrams2 = 0x9,	// 2号八卦
		em_Equi_Type_Diagrams3 = 0xA,	// 3号八卦
		em_Equi_Type_Diagrams4 = 0xB,	// 4号八卦
		em_Equi_Type_Diagrams5 = 0xC,	// 5号八卦
		em_Equi_Type_Diagrams6 = 0xD,	// 6号八卦
		em_Equi_Type_Diagrams7 = 0xE,	// 7号八卦
		em_Equi_Type_Diagrams8 = 0xF,	// 8号八卦
		em_Equi_Type_Soul	   = 0x16,  // 魂
		em_Equi_Type_Cloth		= 0x2,	// 衣服
		em_Equi_Type_Hat		= 0x5,  // 帽子
		em_Equi_Type_Glasses	= 0x4,   // 眼镜
		em_Equi_Type_LatestFashion = 0x12, // 时装
	};
public:
	// 获取物品名称!
	static std::wstring GetItemName(_In_ DWORD dwItemObj);

	// 根据物品ID返回物品OBJ
	static DWORD    GetItemObj_By_ID(_In_ DWORD dwItemId);

	// 卸载宝石
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

	// Count 数量
	DWORD		GetCount() CONST;

	// Name  物品名称
	CONST std::wstring&	GetName() CONST;
	VOID		SetName() CONST;
	CONST std::wstring	GetColorName() CONST;	// 颜色名字

	// Color
	em_ItemColor GetColor() CONST;

	// Index
	UINT		GetIndex() CONST; // 背包位置

	// TypeName 右上角的名字
	CONST std::wstring	GetTypeName() CONST;
	CONST std::wstring	GetTypeName_Dlg() CONST;

	// Explain 分解
	BOOL		Explain() CONST;

	// ItemType 分类
	static DWORD GetItemType_By_GId(_In_ DWORD dwGId);
	DWORD		GetItemType() CONST;
	DWORD		GetItemType_Dlg() CONST;
	BOOL		IsEquiment() CONST;

	// UseItem	使用物品
	BOOL		UseItem() CONST;

	// OpenBox	打开箱子
	BOOL		OpenBox() CONST;

	// Enhanced	强化 Parm1=要成长装备的Sid , Parm2=成长符之类的东西
	BOOL		EquiEnhanced(_In_ DWORD dwGrowthItemSId, _In_ DWORD dwLuckyCharmSId = NULL) CONST;

	// Evolution	装备5进化到6
	BOOL		EquiEvolution(_In_ CONST CBag& Weaspon, _In_ DWORD dwEvolutionId) CONST;

	//  装备10进化到1
	BOOL		EquiEndEvolution(_In_ DWORD dwGrowthItemSId, _In_ DWORD dwEvoltionId) CONST;

	// UntieEqui 解印
	BOOL		EquiUntie(_In_ DWORD dwUntieId, _In_ DWORD dwUntieCount, _In_ DWORD nType) CONST;

	// OpenBox By Key 用钥匙开箱子
	BOOL		OpenBox_By_Key(_In_ DWORD dwKeyId, _In_ DWORD dwKeyCount) CONST;

	// 穿装备
	BOOL		WearEqui() CONST;

	// Discard 丢
	BOOL		DiscardItem() CONST;

	// 镶嵌宝石 (要判断一下现在是第几颗宝石)
	BOOL		SetGem(_In_ UINT uIndex, _In_ CONST CGameUi& Inventory2Panel) CONST;

	// 放置交易物品
	BOOL		PushTransactionItem(_In_ DWORD TradeShelfPanelObj,  _In_ DWORD dwCount) CONST;

	// 脱装备
	BOOL		RelieveEqui() CONST;

	// 获取/设置邮寄数量
	UINT		GetMailCount() CONST;
	VOID		SetMailCount(_In_ UINT uMailCount_);

	// 是否强化八卦
	BOOL		IsGrowthed() CONST;

	// 强化八卦
	BOOL		GrowthBaGua(_In_ DWORD dwSacrificeSid) CONST;

	// 强化过八卦的属性ID(把这个丢给GetBaGuaAttribute 可以返回强化过属性的值和类型)
	DWORD		GetExtendBaGuaAttributeId() CONST;
private:
	DWORD			m_dwBagBase;
	UINT			m_uIndex;
	UINT			uMailCount;
	mutable	std::wstring Name;
};


#endif