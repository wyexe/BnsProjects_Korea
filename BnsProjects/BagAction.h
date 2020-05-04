#ifndef __GITBNSPROJECTS_BNSPROJECTS_OBJECT_OTHEROBJECT_BAG_BAGACTION_H__
#define __GITBNSPROJECTS_BNSPROJECTS_OBJECT_OTHEROBJECT_BAG_BAGACTION_H__

#include "BagBase.h"
class CBagAction : public MyTools::CClassInstance<CBagAction>
{
public:
	CBagAction() = default;
	~CBagAction() = default;
public:
	// �ֽⶫ��
	BOOL ExplainItem() CONST;

	// ������
	BOOL DiscardItem() CONST;

	// ������(������Ʒ������)
	BOOL DiscardItem_By_Type(_In_ CONST std::vector<CBag::em_Equi_Type>& VecType) CONST;

	// �Ƿ���Ҫ��ҩ -> ����Ѫ��
	BOOL SupplementHp() CONST;

	// ǿ������
	BOOL EquiGrowth(_In_ CBag::em_Equi_Type emType, _In_ CONST std::wstring& wsSacrificeName) CONST;

	// ǿ��ûǿ�����İ���
	BOOL BaGuaGrowth(_In_ CONST std::wstring& wsSacrificeName) CONST;

	// ��װ��
	BOOL WearEqui_By_Name(_In_ CONST std::wstring& wsEquiName) CONST;

	// ��ת��
	BOOL RollWheel_By_ItemName(_In_ CONST std::wstring& wsItemName, _In_ DWORD dwWheelId) CONST;

	// ���
	BOOL SaveItem(_In_ int nBagType, _In_ CONST std::wstring& wsNpcName) CONST;

	// ȡ��
	BOOL PullItem_By_WareHouse(_In_ CONST std::wstring& wsNpcName, _In_ CONST std::vector<std::wstring>& vItem) CONST;

	struct tagPullWardrobeItm{ std::wstring wsItmName; UINT uCount; };
	BOOL PullItemByWardrobeWithList(_In_ CONST std::wstring& wsNpcName, _In_ CONST CBagAction::tagPullWardrobeItm& PullItm) CONST;

	// ������������!
	BOOL QuickRepairWeaspon() CONST;

	// �ӱ���������Ʒ
	BOOL Buy(_In_ CONST std::wstring& wsItemName, _In_ DWORD dwPageIndex, _In_ DWORD dwIndex, _In_ DWORD dwCount) CONST;

	// ȡ��ʯ
	BOOL RemoveALLGem() CONST;

	// �Ƿ������ƷName
	BOOL ExistItemByNameWithType(_In_ CONST std::wstring& wsItemName, _In_ int nBagType, _Out_opt_ CBag* pBag) CONST;

	// ����Ʒ
	VOID DiscardItem_By_Condition(_In_ std::function<BOOL(CONST CBag&)> fnItemFilter) CONST;

	// ʹ����Ʒ
	VOID UseItem_By_Condition(_In_ std::function<BOOL(CONST CBag&)> fnItemFilter) CONST;

	// ������
	BOOL OpenBox_By_Condition(_In_ std::function<BOOL(CONST CBag&)> fnItemFilter) CONST;

	// �ֽ�װ��
	BOOL Explain_By_Condition(_In_ std::function<BOOL(CONST CBag&)> fnItemFilter) CONST;

	// ����Ʒ
	BOOL SellItem_By_Condition(_In_ CONST std::wstring& wsNpcName, _In_ std::function<BOOL(CONST CBag&)> fnItemFilter) CONST;
	
private:
	DSIABLE_COPY_AND_ASSIGN(CBagAction)
};



#endif