#ifndef __GITBNSPROJECTS_BNSPROJECTS_SOMEFUN_FARM_FARMSPECIALTASK_H__
#define __GITBNSPROJECTS_BNSPROJECTS_SOMEFUN_FARM_FARMSPECIALTASK_H__

#include "Farm.h"

class CFarmSpeialTask : public MyTools::CClassInstance<CFarmSpeialTask>
{
public:
	explicit CFarmSpeialTask() = default;
	~CFarmSpeialTask() = default;

public:
	// ɱ��ʯ�����ɫ������
	BOOL ChooseColorTask(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<BOOL(VOID)> fnIsComplete) CONST;

	// ��������������
	BOOL SpecialTask_Ice_Fire_Shield(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<BOOL(VOID)> fnIsComplete) CONST;

	// �����µ�10�ڵ�ɱ4��СBOSS������
	BOOL SpecialTask_3A9(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<BOOL(VOID)> fnIsComplete) CONST;

	// �ڶ��µĵ�3��10������
	BOOL SpecialTask_135(_In_ std::function<BOOL(VOID)> fnIsComplete) CONST;

	// ���Źط���
	BOOL Defence_GuiMen(_In_ CONST std::wstring& wsDoorName, _In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<VOID(VOID)> LeaveTeamPtr) CONST;

	// ��ɭ�ַ���
	BOOL Defence_BlackForest(_In_ CONST std::wstring& wsCoreName, _In_ DWORD dwCoreIndex, _In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<VOID(VOID)> LeaveTeamPtr) CONST;

	// �����Թ���ը��
	BOOL ThrowBomb_Maze() CONST;

	// �����Թ�ת��
	BOOL TurnTable_Maze() CONST;

	// �����Թ�����ǿ���ӳ�
	BOOL MagicCaptain_Maze() CONST;

	// �����Թ�����֮��
	BOOL Maze_Column() CONST;
private:
	// ��ɭ�ַ��ذ�������
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