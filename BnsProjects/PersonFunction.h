#ifndef __GITBNSPROJECTS_BNSPROJECTS_OBJECTS_GAMEOBJECT_PERSON_PERSONFUNCTION_H__
#define __GITBNSPROJECTS_BNSPROJECTS_OBJECTS_GAMEOBJECT_PERSON_PERSONFUNCTION_H__

#include "GameVariable.h"

#define MOVE_FLAG_NORMAL		0x2
#define MOVE_FLAG_DISABLE_SPEED 0x4
#define MOVE_FLAG_DISABLE_JUMP  0x8
#define MOVE_FLAG_DISABLE_SS	0x10

class CPersonFunction : public MyTools::CClassInstance<CPersonFunction>
{
public:
	CPersonFunction();
	~CPersonFunction() = default;

public:
	// �ȴ��������
	BOOL WaitForProgressBar(_In_ DWORD dwMaxTime = 10 * 1000) CONST;

	// �л�Ƶ��
	BOOL SwitchChannel(_In_ DWORD dwChannel) CONST;

	// �������
	BOOL HidePlayer() CONST;

	// ����ʽ����
	BOOL DoAction(DWORD dwActionId) CONST;

	// С��
	BOOL BackToSwitchPlayer() CONST;

	// �Ƿ��ͼ��
	BOOL IsGameLoad() CONST;

	// �Ƿ���ֶ���
	BOOL IsGameChart() CONST;

	// �ȴ���ͼ
	BOOL WaitGameLoad(_In_ BOOL bForceWait = FALSE, _In_ BOOL bCheckSS = TRUE) CONST;

	// �ȴ����ֹ�ͼ(����=��ʱʱ��)
	BOOL IsWaitForGameLoad(_In_ DWORD dwMaxTime) CONST;

	// ������·Flag
	VOID SetMoveFlag(_In_ DWORD dwFlag);
	// �ָ���·Flag
	VOID RestoreMoveFlag();

	// ��·
	BOOL Move(_In_ CONST Point& TargetPoint) CONST;
	BOOL MoveToPoint(_In_ CONST Point& TargetPoint, _In_ float fDis, _In_ float fMaxDis = MAX_MOVE_DIS) CONST;
	BOOL MoveToPoint_For_List(_In_ std::vector<Point>& PointVec) CONST;
	BOOL Move_2D(_In_ CONST FixPointInfo& TarFixPoint) CONST;

	// ��ȡ��ǰ����Id
	DWORD GetCityId() CONST;

	// ��ȡ��ǰ��ͼId
	DWORD GetMapId() CONST;

	// ��ȡ���Ƶ��
	DWORD GetMaxChannel() CONST;

	// ����
	BOOL TransportCiry(_In_ DWORD dwTargetCityId) CONST;

	// �Ƿ����BUFFID
	BOOL ExistBuff_Person(_In_ DWORD dwBuffId, _Out_opt_ GAMEOBJ_BUFF* pGameObjBuff = nullptr) CONST;
	BOOL ExistBuff_Person(_In_ CONST std::vector<DWORD>& BuffVec) CONST;

	// ����
	BOOL Resurrect() CONST;

	// ���Ը���
	BOOL TryRelive() CONST;

	// �ȴ�����
	BOOL WaitToLoadProgressBar(DWORD dwSleepTime, DWORD dwMaxSleepTime, std::function<void(void)> f) CONST;

	// һ�μ���
	BOOL Pickup(CONST struct _GameObjectID& GameObjectID_) CONST;

	// ת��
	//BOOL Turntable(_In_ DWORD dwAttendancePanel) CONST;

	// �������ϵĶ���, ȡ������״̬
	BOOL CancelPutDownStatus() CONST;

	// ����
	BOOL DoEscape() CONST;

	// �Ƿ�Tab״̬
	BOOL Exist_Lux_Tab_Buff() CONST;

	// �Ƿ�ʹ����ʿE��״̬
	BOOL Exist_Lux_E_Buff() CONST;

	// Exist Lux V Buff
	BOOL Exist_Lux_V_Buff() CONST;

	// �Ƿ�����ٻ�������״̬
	BOOL Exist_Sumoner_X_Buff() CONST;

	// �Ƿ�����ٻ��Ļ���״̬
	BOOL Exist_Sumoner_3_Buff() CONST;

	// �ȴ����ܶ������
	BOOL WaitForProgressBarBySkill(_In_ DWORD dwMaxTimeOut) CONST;

	// �ȴ�ս��״̬ȡ��!
	BOOL WaitForWarStatus() CONST;

	// �˳�����!
	BOOL ExitSchool() CONST;

	// ��
	enum em_Run_Type{ em_Run_Type_OnFloor, em_Run_Type_FastFly };
	BOOL Run(_In_ em_Run_Type emRunType) CONST;
private:
	std::tuple<CONST std::wstring, CONST std::wstring, CONST std::wstring, CONST std::wstring, DWORD, DWORD, Point, Point> MakeBnsConfigParm(_In_ CONST std::wstring& wsMapName, _In_ CONST Point& TarPoint) CONST;
private:
	DSIABLE_COPY_AND_ASSIGN(CPersonFunction)
private:
	DWORD _dwMoveFlag;
	DWORD _dwMoveFlagBackup;
};


#endif