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
	// 搜索Npc(不包含过滤)
	BOOL FindNpc_By_Name(_In_ CONST std::wstring& wsNpcName, _In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ std::vector<CNpc>& vlst) CONST;
	BOOL FindNpc_By_Name(_In_ CONST std::wstring& wsNpcName, _In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CNpc* pNpc) CONST;

	//
	BOOL FindNpc_By_Id(_In_ DWORD dwNpcId, _In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CNpc* pNpc) CONST;

	//
	BOOL FindNpc_By_Condiction(_In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CNpc* pNpc, std::function<BOOL(CONST CNpc&)> fnFindNpc) CONST;

	// 对符合条件的Npc调用函数
	BOOL DoAction_By_ExistNpc(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<BOOL(CONST CNpc&)> fnFind, _In_ std::function<VOID(CONST CNpc&)> fnAction) CONST;

	// 对符合名称的Npc调用函数
	BOOL DoAction_By_ExistNpc_For_Name(_In_ CONST std::wstring wsName, _In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<VOID(CONST CNpc&)> fnAction) CONST;

	// 获取周围的Npc(过滤后)
	UINT GetAroundNpcList(_In_ CONST FixPointInfo& FixPtInfo, _Out_ std::vector<CNpc>& vlst) CONST;

	// 获取周围的目标ID=Self的Npc(过滤后)
	UINT GetAroundTargetNpcList(_In_ CONST FixPointInfo& FixPtInfo, _Out_ std::vector<CNpc>& vlst) CONST;

	// 获取最近的Npc(过滤后)
	BOOL GetTopNpc_By_Dis(_In_ CONST FixPointInfo& FixPtInfo, _Out_opt_  CNpc* pNpc) CONST;

	// 获取周围Npc数量(过滤后)
	UINT GetAroundNpcCount(_In_ CONST FixPointInfo& FixPtInfo) CONST;

	// 卖东西
	BOOL SellItem_By_Npc(_In_ CONST std::wstring& wsNpcName) CONST;
private:
	DSIABLE_COPY_AND_ASSIGN(CNpcExtend)
};



#endif