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

	// 整理背包
	VOID		CleanBag() CONST;

	// 开背包
	BOOL		CreateMoreBag() CONST;

	// 是否包满了  Parm=最低拥有多少个格子
	BOOL		IsBagFull(_In_ int nBagType, DWORD dwRemainCount) CONST;

	// 获取空的背包位置
	BOOL		GetEmptyItemObj(_In_ int nBagType, _Out_opt_ DWORD& dwItemObj) CONST;

	//  返回背包的锤子
	BOOL		ExistHammer(_Out_opt_ CBag* Hammer) CONST;

	// 返回背包的锤子数量
	DWORD		GetHammerCount() CONST;

	// 是否存在物品
	BOOL		ExistItem_By_Name(_In_ CONST std::wstring& wsItemName, _Out_opt_ CBag* pBag = nullptr) CONST;
	BOOL		ExistItem_By_Id(_In_ DWORD dwItemId, _Out_opt_ CBag* pBag = nullptr) CONST;
	BOOL		ExistItem_By_Condiction(_In_ std::function<BOOL(CONST CBag&)> fn, _Out_opt_ CBag* pBag = nullptr) CONST;
	BOOL		DoItem_By_EixstItem(_In_ std::function<BOOL(CONST CBag&)> fnFind, _In_ std::function<VOID(CONST CBag&)> fnDo) CONST;
	BOOL		DoItem_By_ExistItem_To_Name(_In_ CONST std::wstring& wsName, _In_ std::function<VOID(CONST CBag&)> fnDo) CONST;
	UINT		GetItemCount_By_Name(_In_ CONST std::wstring& wsItemName) CONST;

	// 根据Id返回身上装备位置
	BOOL		GetEqui_By_Type(_In_ CBag::em_Equi_Type emType, _Out_opt_ CBag* pEqui = nullptr) CONST;
	BOOL		ExistEqui_By_Condiction(_In_ std::function<BOOL(CONST CBag&)> fn, _Out_opt_ CBag* pBag = nullptr) CONST;
	BOOL		ExistEqui_By_Name(_In_ CONST std::wstring& wsName, _Out_opt_ CBag* pBag = nullptr) CONST;
	
	// 武器上是否存在宝石
	BOOL		ExistGem_By_Weaspon(DWORD dwItemId) CONST;

	// 背包物品占用多少个格子
	UINT		TotalOccupyItem(_In_ int emBagType) CONST;

	// 返回背包名字相同的Vector
	UINT		GetItemList_By_Name(_In_ CONST std::wstring& wsItemName, _Out_ std::vector<CBag>& VecItem) CONST;
private:
	DSIABLE_COPY_AND_ASSIGN(CBagFunction)
};



#endif