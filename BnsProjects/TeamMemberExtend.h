#ifndef __GITBNSPROJECTS_BNSPROJECTS_OBJECT_GAMEOBJECT_TEAM_TEAMMEMBEREXTEND_H__
#define __GITBNSPROJECTS_BNSPROJECTS_OBJECT_GAMEOBJECT_TEAM_TEAMMEMBEREXTEND_H__

#include "GameVariable.h"

class CTeamMember;
class CTeamMemberExtend : public MyTools::CClassInstance<CTeamMemberExtend>
{
public:
	CTeamMemberExtend() = default;
	~CTeamMemberExtend() = default;
public:
	struct TeamIndexClasses
	{
		std::wstring wsPlayerName;
		int			 nIndex;

		TeamIndexClasses(_In_ CONST std::wstring& wsPlayerName_, _In_ int nIndex_) : wsPlayerName(wsPlayerName_), nIndex(nIndex_) {}
	};
public:
	// 判断队长是否队长
	BOOL IsTeamCaptain(_In_ CONST std::wstring& wsPlayerName) CONST;

	// 返回掉线的成员
	BOOL IsTeamMember_LoseConnect(_In_ CTeamMember* pTeamMember) CONST;

	// 返回要离队的成员
	BOOL IsTeamMember_LeaveTeam(_In_ CTeamMember* pTeamMember) CONST;

	// 是否在队伍里
	BOOL ExistTeamMember(_In_ CONST std::wstring& wsMemberName, _Out_opt_ CTeamMember* pTeamMember = nullptr) CONST;

	// 查找队员
	BOOL FindTeamMember_By_Condiction(_In_ CTeamMember* pTeamMember, std::function<BOOL(CONST CTeamMember&)> fnFind) CONST;

	// 设置队伍拍卖品质
	BOOL SetAuctionQuality(em_Auction_Quality emAuctionQuality = em_Auction_Quality_Extreme) CONST;

	// 离开队伍
	BOOL LeaveTeam() CONST;

	// 获取队伍成员数量
	DWORD GetTeamMemberCount() CONST;

	// 获取队长
	BOOL GetCaptain(_Out_opt_ CTeamMember* pCaptain) CONST;

	// 判断队伍是否在一个频道
	BOOL IsSameChannel_By_Team() CONST;

	// 返回当前人物所在的位置
	DWORD GetCurrentTeamMemberIndex() CONST;

	// 根据索引返回TeamMember
	BOOL GetTeamMemberByIndex(_In_ DWORD dwIndex, _Out_opt_ CTeamMember* pTeamMeber) CONST;

	// 判断是否有队员HP<%?
	BOOL ExistTeamMemberWhenHpLessValue(_In_ DWORD dwPercentHp) CONST;

	// 获取在坐标范围内有多少个队员
	UINT GetTeamMemberCountByPoint(_In_ CONST FixPointInfo& FixPoint) CONST;
	UINT GetTeamMemberCountByCondition(_In_ std::function<BOOL(CONST CTeamMember&)> Finder) CONST;

	// 对某个队员进行操作
	BOOL Action_By_TeamMemberName(_In_ CONST std::wstring& wsPlayerName, _In_ std::function<VOID(CTeamMember&)> Worker) CONST;

	// 判断队员是否低于%? 的血量
	BOOL GetTeamMember_By_LessPercentHp(_In_ DWORD dwPercentHp, _Out_opt_ CTeamMember* pTeamMeber) CONST;

	// 循环
	VOID ForEach(_In_ std::function<VOID(CONST CTeamMember& TeamMember)> Worker);

	// 组队状态下是否复活(死的队友多了就不复活了)
	BOOL IsTryRelive() CONST;

	int GetCurrentTeamIndexByClasses(_In_ em_Player_Classes emPlayerClasses) CONST;

	VOID SetCurrentTeamIndexByClasses();
private:
	std::vector<TeamIndexClasses> VecTeamIndexClasses;
private:
	DSIABLE_COPY_AND_ASSIGN(CTeamMemberExtend)
};



#endif