#ifndef __GITBNSPROJECTS_BNSPROJECTS_OBJECT_OTHEROBJECT_BAG_BAGFUNCTION_H__
#define __GITBNSPROJECTS_BNSPROJECTS_OBJECT_OTHEROBJECT_BAG_BAGFUNCTION_H__

#include "BagBase.h"
class CBagFunction : public MyTools::CClassInstance<CBagFunction>
{
public:
	typedef struct _Equi_Type
	{
		std::wstring wsEquiName;
		CBag::em_Equi_Type emEquiType;
	}Equi_Type;
public:
	CBagFunction() = default;
	~CBagFunction() = default;

	static BOOL GetEquiType_By_Text(_In_ CONST std::wstring& wsText, _Out_opt_ Equi_Type& EquiType);
public:

	DWORD		GetEndurance() CONST;

	// ������
	VOID		CleanBag() CONST;

	// ������
	BOOL		CreateMoreBag() CONST;

	// �Ƿ������  Parm=���ӵ�ж��ٸ�����
	BOOL		IsBagFull(_In_ int nBagType, DWORD dwRemainCount) CONST;

	// ��ȡ�յı���λ��
	BOOL		GetEmptyItemObj(_In_ int nBagType, _Out_opt_ DWORD& dwItemObj) CONST;

	//  ���ر����Ĵ���
	BOOL		ExistHammer(_Out_opt_ CBag* Hammer) CONST;

	// ���ر����Ĵ�������
	DWORD		GetHammerCount() CONST;

	// �Ƿ������Ʒ
	BOOL		ExistItem_By_Name(_In_ CONST std::wstring& wsItemName, _Out_opt_ CBag* pBag = nullptr) CONST;
	BOOL		ExistItem_By_Id(_In_ DWORD dwItemId, _Out_opt_ CBag* pBag = nullptr) CONST;
	BOOL		ExistItem_By_Condiction(_In_ std::function<BOOL(CONST CBag&)> fn, _Out_opt_ CBag* pBag = nullptr) CONST;
	BOOL		DoItem_By_EixstItem(_In_ std::function<BOOL(CONST CBag&)> fnFind, _In_ std::function<VOID(CONST CBag&)> fnDo) CONST;
	BOOL		DoItem_By_ExistItem_To_Name(_In_ CONST std::wstring& wsName, _In_ std::function<VOID(CONST CBag&)> fnDo) CONST;
	UINT		GetItemCount_By_Name(_In_ CONST std::wstring& wsItemName) CONST;

	// ����Id��������װ��λ��
	BOOL		GetEqui_By_Type(_In_ CBag::em_Equi_Type emType, _Out_opt_ CBag* pEqui = nullptr) CONST;
	BOOL		ExistEqui_By_Condiction(_In_ std::function<BOOL(CONST CBag&)> fn, _Out_opt_ CBag* pBag = nullptr) CONST;
	BOOL		ExistEqui_By_Name(_In_ CONST std::wstring& wsName, _Out_opt_ CBag* pBag = nullptr) CONST;
	
	// �������Ƿ���ڱ�ʯ
	BOOL		ExistGem_By_Weaspon(DWORD dwItemId) CONST;

	// ������Ʒռ�ö��ٸ�����
	UINT		TotalOccupyItem(_In_ int emBagType) CONST;

	// ���ر���������ͬ��Vector
	UINT		GetItemList_By_Name(_In_ CONST std::wstring& wsItemName, _Out_ std::vector<CBag>& VecItem) CONST;
private:
	DSIABLE_COPY_AND_ASSIGN(CBagFunction)
};



#endif