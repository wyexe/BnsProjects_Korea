#ifndef __GITBNSPROJECTS_BNSPROJECTS_OBJECT_GAMEOBJECT_PLAYER_PLAYEREXTEND_H__
#define __GITBNSPROJECTS_BNSPROJECTS_OBJECT_GAMEOBJECT_PLAYER_PLAYEREXTEND_H__

#include "GameVariable.h"

class CPlayer;
class CPlayerExtend : public MyTools::CClassInstance<CPlayerExtend>
{
public:
	CPlayerExtend() = default;
	~CPlayerExtend() = default;

public:
	// �������
	BOOL FindPlayer_By_Id(_In_ DWORD dwPlayerId, _In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CPlayer* pPlayer) CONST;

	//
	BOOL FindPlayer_By_Name(_In_ CONST std::wstring& wsPlayerName, _In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CPlayer* pPlayer) CONST;

	//
	BOOL FindPlayer_By_Condition(_In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CPlayer* pPlayer, std::function<BOOL(CONST CPlayer&)> f) CONST;

	// ���������һ�����
	BOOL GetTopPlayer_By_Dis(_In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CPlayer* pPlayer) CONST;

	// ���������һ��ѵ��Npc
	BOOL GetTopPraticeNpc_By_Dis(_In_ CONST FixPointInfo& FixPtInfo, _Out_opt_ CPlayer* pPlayer) CONST;
private:
	DSIABLE_COPY_AND_ASSIGN(CPlayerExtend)
};



#endif