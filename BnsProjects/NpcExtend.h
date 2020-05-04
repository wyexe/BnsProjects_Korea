#ifndef __GITBNSPROJECTS_BNSPROJECTS_OBJECT_GAMEOBJECT_NPC_NPCEXTEND_H__
#define __GITBNSPROJECTS_BNSPROJECTS_OBJECT_GAMEOBJECT_NPC_NPCEXTEND_H__

#include "GameVariable.h"

class CNpc;
class CNpcExtend : public MyTools::CClassInstance<CNpcExtend>
{
public:
	CNpcExtend() = default;
	~CNpcExtend() = default;

public:
	// ����Npc(����������)
	BOOL FindNpc_By_Name(_In_ CONST std::wstring& wsNpcName, _In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ std::vector<CNpc>& vlst) CONST;
	BOOL FindNpc_By_Name(_In_ CONST std::wstring& wsNpcName, _In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CNpc* pNpc) CONST;

	//
	BOOL FindNpc_By_Id(_In_ DWORD dwNpcId, _In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CNpc* pNpc) CONST;

	//
	BOOL FindNpc_By_Condiction(_In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CNpc* pNpc, std::function<BOOL(CONST CNpc&)> fnFindNpc) CONST;

	// �Է���������Npc���ú���
	BOOL DoAction_By_ExistNpc(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<BOOL(CONST CNpc&)> fnFind, _In_ std::function<VOID(CONST CNpc&)> fnAction) CONST;

	// �Է������Ƶ�Npc���ú���
	BOOL DoAction_By_ExistNpc_For_Name(_In_ CONST std::wstring wsName, _In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<VOID(CONST CNpc&)> fnAction) CONST;

	// ��ȡ��Χ��Npc(���˺�)
	UINT GetAroundNpcList(_In_ CONST FixPointInfo& FixPtInfo, _Out_ std::vector<CNpc>& vlst) CONST;

	// ��ȡ��Χ��Ŀ��ID=Self��Npc(���˺�)
	UINT GetAroundTargetNpcList(_In_ CONST FixPointInfo& FixPtInfo, _Out_ std::vector<CNpc>& vlst) CONST;

	// ��ȡ�����Npc(���˺�)
	BOOL GetTopNpc_By_Dis(_In_ CONST FixPointInfo& FixPtInfo, _Out_opt_  CNpc* pNpc) CONST;

	// ��ȡ��ΧNpc����(���˺�)
	UINT GetAroundNpcCount(_In_ CONST FixPointInfo& FixPtInfo) CONST;

	// ������
	BOOL SellItem_By_Npc(_In_ CONST std::wstring& wsNpcName) CONST;
private:
	DSIABLE_COPY_AND_ASSIGN(CNpcExtend)
};



#endif