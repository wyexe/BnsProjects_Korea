#ifndef __GITBNSPROJECTS_BNSPROJECTS_SOMEFUN_FARM_FARMSPECIALTASK_H__
#define __GITBNSPROJECTS_BNSPROJECTS_SOMEFUN_FARM_FARMSPECIALTASK_H__

#include "Farm.h"

class CFarmSpeialTask : public MyTools::CClassInstance<CFarmSpeialTask>
{
public:
	explicit CFarmSpeialTask() = default;
	~CFarmSpeialTask() = default;

public:
	// 杀死石像后报颜色的任务
	BOOL ChooseColorTask(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<BOOL(VOID)> fnIsComplete) CONST;

	// 雷狱冰火罩任务
	BOOL SpecialTask_Ice_Fire_Shield(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<BOOL(VOID)> fnIsComplete) CONST;

	// 第五章第10节的杀4个小BOSS的任务
	BOOL SpecialTask_3A9(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<BOOL(VOID)> fnIsComplete) CONST;

	// 第二章的第3节10级副本
	BOOL SpecialTask_135(_In_ std::function<BOOL(VOID)> fnIsComplete) CONST;

	// 鬼门关防守
	BOOL Defence_GuiMen(_In_ CONST std::wstring& wsDoorName, _In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<VOID(VOID)> LeaveTeamPtr) CONST;

	// 黑森林防守
	BOOL Defence_BlackForest(_In_ CONST std::wstring& wsCoreName, _In_ DWORD dwCoreIndex, _In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<VOID(VOID)> LeaveTeamPtr) CONST;

	// 分裂迷宫扔炸弹
	BOOL ThrowBomb_Maze() CONST;

	// 分裂迷宫转盘
	BOOL TurnTable_Maze() CONST;

	// 分裂迷宫法器强化队长
	BOOL MagicCaptain_Maze() CONST;

	// 分裂迷宫分裂之柱
	BOOL Maze_Column() CONST;
private:
	// 黑森林防守帮助气功
	struct BlackForestForceMasterHelp
	{
		BOOL					IsForceMaster;
		CONST std::vector<Point>&	Vec1;
		CONST std::vector<Point>&	Vec2;
		CONST Point				Pt;

		BlackForestForceMasterHelp(CONST std::vector<Point>& V1, CONST std::vector<Point>& V2, CONST Point& p1, _In_ BOOL IsForceMaster_) : Vec1(V1), Vec2(V2), Pt(p1), IsForceMaster(IsForceMaster_){}

		//BlackForestForceMasterHelp(CONST BlackForestForceMasterHelp&) = delete;

		BlackForestForceMasterHelp& operator =(CONST BlackForestForceMasterHelp&) = delete;
	};

	BOOL Defence_BlackForest_Help(_In_ _Out_ int& nPutForceMasterNpcCount, _In_ _Out_ int& nPutPikeNpcCount) CONST;

	BOOL Defence_BlackForest_Help_Move(_In_ CONST BlackForestForceMasterHelp& BlackForestHelp) CONST;

	CONST std::vector<BlackForestForceMasterHelp>& GetBlackForestForceMasterHelpVec() CONST;

private:
	DSIABLE_COPY_AND_ASSIGN(CFarmSpeialTask)
};



#endif