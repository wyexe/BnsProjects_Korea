#ifndef __GITBNSPROJECTS_BNSPROJECTS_OBJECTS_OBJECTFUNCTION_H__
#define __GITBNSPROJECTS_BNSPROJECTS_OBJECTS_OBJECTFUNCTION_H__

#include "ObjectTraverse.h"
#include <MyTools/CLLock.h>

class CPlayer;
class CObjectFunction : public CObjectTraverse, public MyTools::CClassInstance<CObjectFunction>
{
public:
	typedef struct _WeasponGemInfo
	{
		std::wstring wsGemName;
		DWORD dwItemObj;
		DWORD dwItemId;
	}WeasponGemInfo;

public:
	CObjectFunction() = default;
	~CObjectFunction() = default;

public:
	BOOL IsShow_In_GameUi(_In_ CONST std::wstring& wsUiName) CONST;

	BOOL Action_By_ShowGameUi_Name(_In_ CONST std::wstring& wsUiName, _In_ std::function<VOID(CONST CGameUi&)> Worker) CONST;

	BOOL Action_By_GameUiName(_In_ CONST std::wstring& wsUiName, _In_ std::function<VOID(CONST CGameUi&)> Worker) CONST;

	// ����Ԫ��
	template<typename T>
	BOOL FindElement_By_ObjectTraverse(_In_ em_GameObject_Type emType, _In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ T* pElement, _In_ std::function<BOOL(CONST T&)> fnFind) CONST
	{
		std::vector<T> vlst;
		CObjectFunction::GetInstance().GetSortGameObjectList<T>(vlst, emType, FixPtInfo);

		return MyTools::CLPublic::Vec_find_if(vlst, pElement, fnFind);
	}

	// ��������OBJ
	BOOL FindALLObject_By_Id(_In_ DWORD dwObjId, _In_ CPlayer* pTempObj) CONST;

	// ��������
	BOOL FindTalkMsg_By_Condiction(_In_ std::function<BOOL(TalkMsg_Src& TalkMsg_)> fnCondiction) CONST;
	BOOL SetTalkMsg_By_Condition(_In_ CONST std::wstring& wsText, _In_ std::function<BOOL(TalkMsg_Src& TalkMsg_)> fnCondiction) CONST;

	// Ui����
	UINT GetUiSkillList(_Out_ std::vector<CUiSkill>& vlst) CONST;

	// ������ʯ
	UINT GetWeasponGemInfoList(_Out_ std::vector<WeasponGemInfo>& vlst) CONST;

	// �Ƿ���� Ŀ������ BUFF
	BOOL ExistMonsterBuff(_In_ DWORD dwTarId, _In_ DWORD dwBuffID) CONST;

	// ��ȡ��ǰ���ĵ�Index
	int GetIndex_By_Auction() CONST;

	// ��ȡ���龺��
	BOOL GetTeamAuctionContent(_Out_ AuctionContent& AuctionContent_) CONST;
private:
	BOOL ExistGameUI(_In_ CONST std::wstring& wsUiName, _Out_opt_ CGameUi* pGameUi) CONST;
private:
	DSIABLE_COPY_AND_ASSIGN(CObjectFunction)
};



#endif