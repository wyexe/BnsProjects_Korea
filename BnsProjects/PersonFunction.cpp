#include "stdafx.h"
#include "PersonFunction.h"
#include <MyTools/CLSearchBase.h>
#include <MyTools/Log.h>
#include <MyTools/TimeTick.h>
#include <MyTools/CLEchoException.h>
#include "SomeFun.h"
#include "FarmSkill.h"
#include "Person.h"
#include "ObjectFunction.h"
#include "MiniDump.h"
#include "GameFunction.h"
#include "BnsGameFun.h"
#include "BnsConfig.h"
#include "Task.h"
#include "GameSend.h"
#include "Farm.h"
#include "CatExpend.h"
#include "Cat.h"
#include "ConsoleLog.h"
#define _SELF L"PersonFunction.cpp"

BOOL CPersonFunction::SwitchChannel(_In_ DWORD dwChannel) CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&dwChannel]{
		CGameSend::SwitchChannel(dwChannel);
	});
}

BOOL CPersonFunction::HidePlayer() CONST
{
	if ((ReadDWORD(ReadDWORD(ReadDWORD(�����ַ) + �����ַƫ��1 + 0xC) + 0x10) & 0xFF) == 1)
		return TRUE;

	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([]{CGameSend::HidePlayer(); });
}

BOOL CPersonFunction::BackToSwitchPlayer() CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([]{CGameSend::BackToSwitchPlayer(); });
}

BOOL CPersonFunction::IsGameLoad() CONST
{
	static DWORD dwBase = NULL;
	static DWORD dwOffset = NULL;
	if (dwBase == NULL)
	{
		dwBase = MyTools::CLSearchBase::FindBase_ByCALL("8B44240450E8????????8BC8E8????????B001", 0x092B6EF0 - 0x092B6EF5, (DWORD)::GetModuleHandleW(L"bsengine_Shipping"), 1, 0, L"bsengine_Shipping", 0x21 + 0x1);
		if (dwBase == NULL)
		{
			LOG_MSG_CF( L"��ͼ���Զ����µ�ַʧЧ��!");
			return 0;
		}

		dwOffset = MyTools::CLSearchBase::FindBase_ByCALL("8B44240450E8????????8BC8E8????????B001", 0x092B6EF0 - 0x092B6EFC, (DWORD)::GetModuleHandleW(L"bsengine_Shipping"), 1, 0, L"bsengine_Shipping", 0xF + 0x2, 0xFF);
		if (dwOffset == NULL)
		{
			LOG_MSG_CF( L"��ͼ���Զ����µ�ַƫ��ʧЧ��!");
			return 0;
		}
		//LOG_CF_D( L"GameLoad:[[%X]+%X]", dwBase, dwOffset);
	}
	// ������ֵ��Ƕ���, �Ǿ͵���ͼ������!
	if (IsGameChart())
		return TRUE;

	if (ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(�����ַ) + �����ַƫ��3) + 0xC) + 0x4) == 0x5)
		return TRUE;

	DWORD dwValue1 = ReadDWORD(ReadDWORD(dwBase) + 0x28);
	if (ReadDWORD(ReadDWORD(dwBase) + dwOffset) == 0x1)
		return dwValue1 == 0x13 ? FALSE : TRUE;
	return FALSE;
}

BOOL CPersonFunction::IsGameChart() CONST
{
	return (CGameVariable::GetInstance().GetGameChartValue() & 0xFF) == 0x1;
}

BOOL CPersonFunction::WaitGameLoad(_In_ BOOL bForceWait /* = FALSE */, _In_ BOOL bCheckSS /* = TRUE */) CONST
{
	UINT uCount = 0;
	while (GameRun && IsGameLoad())
	{
		if (uCount++ >= 60 * 5)
		{
			CBnsGameFun::GetInstance().SetCloseGame(L"��ͼ��Ȼ������5min, ׼���ر���Ϸ����!", em_Close_Result::em_Close_Result_ReStart);
		}
		else if (uCount % 60 == 0)
		{
			LOG_CF_D(L"��ͼ������%d������", uCount / 60);
		}
		BnsSleep(1 * 1000);
	}

	BnsSleep(uCount != NULL ? 2 * 1000 : 0);

	if (bCheckSS && (CPerson::GetInstance().GetPoint().X == 0 || CPerson::GetInstance().GetPoint().Y == 0))
	{
		LOG_CF_D( L"��·��ͼ��ʱ�����X,Y=0, ʹ��SS");
		CSomeFun::GetInstance().TimeOut_Condiction_GameRun(15 * 1000, []{return !CFarmSkill::GetInstance().UseSkill_By_SS(); });
		BnsSleep(5 * 1000);
	}
	return TRUE;
}

BOOL CPersonFunction::IsWaitForGameLoad(_In_ DWORD dwMaxTime) CONST
{
	return CSomeFun::GetInstance().TimeOut_Condiction_GameRun(dwMaxTime, [&]{return IsGameLoad() && !EMPTY_PERSONHP; });
}


VOID CPersonFunction::SetMoveFlag(_In_ DWORD dwFlag)
{
	_dwMoveFlagBackup = _dwMoveFlag;
	_dwMoveFlag = dwFlag;
}

VOID CPersonFunction::RestoreMoveFlag()
{
	_dwMoveFlag = _dwMoveFlagBackup;
}

BOOL CPersonFunction::Move(_In_ CONST Point& TargetPoint) CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&TargetPoint]
	{
		CGameSend::Move(TargetPoint.X, TargetPoint.Y, TargetPoint.Z);
	});
}

BOOL CPersonFunction::MoveToPoint(_In_ CONST Point& TargetPoint, _In_ float fDis, _In_ float fMaxDis /* = 100.0f */) CONST
{
	if (CPerson::GetInstance().GetLevel() > 2)
	{
		CFarm::GetInstance().BeckonCat();
	}
	if (CBaseObject::GetPersonDis(TargetPoint) > fMaxDis)
	{
		LOG_CF_D(L"�յ��뵱ǰ�������=%.2f, ����%.2f, ����!:%d,%d,%d", CBaseObject::GetPersonDis(TargetPoint), fMaxDis, TargetPoint.GetX(), TargetPoint.GetY(), TargetPoint.GetZ());
		return TRUE;
	}
	else if (!GameRun || EMPTY_PERSONHP)
		return FALSE;


	MyTools::CLEchoException::GetInstance().InvokeAction(__FUNCTIONW__, [=] 
	{
		int isMoveCount = 1;
		MyTools::CTimeTick TimeTick;
		static BOOL bIngore = FALSE;

		//����MoveToPoint����ִ��һ����·��
		Move(TargetPoint);
		BnsSleep(50);

		while (GameRun && !EMPTY_PERSONHP)
		{
			if (CBaseObject::GetPersonDis(TargetPoint) <= fDis)
				break;

			if (IsGameLoad())
			{
				LOG_CF_D(L"��·�����г��ֹ�ͼ����");
				WaitGameLoad();
				LOG_CF_D(L"��·�����й�ͼ���, ����������!");
				break;
			}

			if (!CPerson::GetInstance().IsMoving())
			{
				//LOG_CF_D( L"û������·״̬��ִ����·");
				if ((_dwMoveFlag & MOVE_FLAG_DISABLE_JUMP) == 0 && isMoveCount % 2 == 0)
				{
					CSomeFun::GetInstance().SimlationKey(VK_SPACE);
				}
				Move(TargetPoint);
				isMoveCount++;
			}

			if ((_dwMoveFlag & MOVE_FLAG_DISABLE_SPEED) == 0 && TimeTick.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Second) >= 7)
			{
				LOG_CF_D(L"7s��û��������1��׼������");
				Move(TargetPoint);
				CSomeFun::GetInstance().SimlationKey(VK_SPACE);
			}

			if ((_dwMoveFlag & MOVE_FLAG_DISABLE_SS) == 0 && TimeTick.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Second) >= 10)
			{
				LOG_CF_D(L"10s��û����SS");
				CFarmSkill::GetInstance().UseSkill_By_SS();
			}

			if (TimeTick.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Second) >= 14)//����12��û��ֱ���˳�
			{
				LOG_CF_D(L"TarPoint:%d,%d,%d, ����:%.2f �޷�����Ŀ�ĵ�, ���Ը�����!", TargetPoint.GetX(), TargetPoint.GetY(), TargetPoint.GetZ(), CBaseObject::GetPersonDis(TargetPoint));
				if (bIngore)
				{
					CBnsConfig::GetInstance().WriteAccountLog(L"���ܿ�������!");
					CGameVariable::GetInstance().SetForceBackToSwitchPlayer();
				}

				// �����ڹ�ͼ��ʱ�����Ч�ж�����2����·ʧ��!
				/*if (CPerson::GetInstance().IsPassDoor())
				{
					LOG_CF_D(L"PassDoor=%X", ReadDWORD(ReadDWORD(CPerson::GetInstance().GetBase() + 0x14) + ����ƫ��));
					bIngore = TRUE;
				}*/

				goto lb_Exit;
			}

			if (CBaseObject::GetPersonDis(TargetPoint) >= fMaxDis)
			{
				LOG_CF_D(L"�յ��뵱ǰ�������=%.2f, ����30, ����!:%d,%d,%d", CBaseObject::GetPersonDis(TargetPoint), TargetPoint.GetX(), TargetPoint.GetY(), TargetPoint.GetZ());
				break;
			}

			BnsSleep(100);
		}

		bIngore = FALSE;
	lb_Exit:;
		HidePlayer();
	});
	
	return TRUE;
}

BOOL CPersonFunction::MoveToPoint_For_List(_In_ std::vector<Point>& PointVec) CONST
{
	int nIndex = MyTools::CLPublic::GetRecentlyIndexByVec(PointVec, CPerson::GetInstance().GetPoint(), MAX_MOVE_DIS * 50);
	if (nIndex == -1)
		return FALSE;

	auto& tmpPoint = PointVec.at(nIndex);
	LOG_CF_D( L"�����������, �ӵ�%d������[%d,%d,%d]��ʼ��! ����:%.2f", nIndex, tmpPoint.GetX(), tmpPoint.GetY(), tmpPoint.GetZ(), CPerson::GetInstance().GetDis(tmpPoint));
	for (UINT i = static_cast<UINT>(nIndex); i < PointVec.size(); ++i)
		MoveToPoint(PointVec.at(i), CSomeFun::GetInstance().GetSpeed() >= 3.0f ? 3.0f : 2.0f);

	return TRUE;
}

BOOL CPersonFunction::Move_2D(_In_ CONST FixPointInfo& TarFixPoint) CONST
{
	int isMoveCount = 1;

	//����MoveToPoint����ִ��һ����·��
	Move(TarFixPoint.FixPoint);
	BnsSleep(50);
	while (GameRun && !EMPTY_PERSONHP)
	{
		if (MyTools::CLPublic::GetDisBy2D(TarFixPoint.FixPoint, CPerson::GetInstance().GetPoint()) <= TarFixPoint.fDis)
			break;

		if (IsGameLoad())
		{
			LOG_CF_D( L"��·�����г��ֹ�ͼ����");
			WaitGameLoad();
			LOG_CF_D( L"��·�����й�ͼ���, ����������!");
			break;
		}

		if (!CPerson::GetInstance().IsMoving())
		{
			LOG_CF_D( L"û������·״̬��ִ����·��");
			if (isMoveCount % 2 == 0)
			{
				CSomeFun::GetInstance().SimlationKey(VK_SPACE);
			}
			Move(TarFixPoint.FixPoint);
			isMoveCount++;
		}
		BnsSleep(100);
	}

	HidePlayer();
	return TRUE;
}

BOOL CPersonFunction::WaitForProgressBar(_In_ DWORD dwMaxTime /*= 10 * 1000*/) CONST
{
	return CSomeFun::GetInstance().TimeOut_Condiction_GameRun(dwMaxTime, []{return CPerson::GetInstance().IsLoadProgressBar() && !EMPTY_PERSONHP; });
}

DWORD CPersonFunction::GetCityId() CONST
{
	DWORD dwValue = 0;
	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&dwValue]{dwValue = CGameSend::GetCurrentCityId(); });
	return dwValue;
}

BOOL CPersonFunction::TransportCiry(_In_ DWORD dwTargetCityId) CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&dwTargetCityId]{ CGameSend::TransportCity(dwTargetCityId); });
}


BOOL CPersonFunction::ExistBuff_Person(_In_ DWORD dwBuffId, _Out_opt_ GAMEOBJ_BUFF* pGameObjBuff /*= nullptr OPTIONAL*/) CONST
{
	std::vector<GAMEOBJ_BUFF> vlst;
	CObjectFunction::GetInstance().GetPersonBuffList(vlst);

	return MyTools::CLPublic::Vec_find_if(vlst, pGameObjBuff, [&dwBuffId](CONST GAMEOBJ_BUFF& GB){return GB.dwBuffId == dwBuffId; });
}

BOOL CPersonFunction::ExistBuff_Person(_In_ CONST std::vector<DWORD>& BuffVec) CONST
{
	std::vector<GAMEOBJ_BUFF> vlst;
	CObjectFunction::GetInstance().GetPersonBuffList(vlst);

	return MyTools::CLPublic::Vec_find_if(vlst, static_cast<GAMEOBJ_BUFF*>(nullptr), [&BuffVec](CONST GAMEOBJ_BUFF& GB)
	{
		return MyTools::CLPublic::Vec_find_if(BuffVec, static_cast<DWORD*>(nullptr), [&GB](CONST DWORD dwBuffId_)
		{
			return dwBuffId_ == GB.dwBuffId;
		});
	});
}


BOOL CPersonFunction::Resurrect() CONST
{
	ULONGLONG ulTick = ::GetTickCount64();
	while (GameRun && EMPTY_PERSONHP)
	{
		if (static_cast<DWORD>(::GetTickCount64() - ulTick) >= 3 * 60 * 1000)
		{
			CConsoleLog(L"����3���ӳ�ʱ! ǿ��С��!");
			CGameVariable::GetInstance().SetForceBackToSwitchPlayer();
			break;
		}

		CConsoleLog(L"�����С���");
		BnsSleep(4 * 1000);
		CGameVariable::GetInstance().PushMainThreadFunPtr_synch([]{CGameSend::Resurrect(); });
		BnsSleep(8 * 1000);
		WaitGameLoad();
	}
	return TRUE;
}

BOOL CPersonFunction::TryRelive() CONST
{
	while (GameRun)
	{
		// �����С���
		if (CPerson::GetInstance().GetAbNormalStatus() == em_WarStatus::em_WarStatus_Healing)
		{
			BnsSleep(1000);
			continue;
		}

		// ����״̬
		if (CPerson::GetInstance().GetAbNormalStatus() == em_WarStatus::em_WarStatus_Dying)
		{
			LOG_CF_D( L"����״̬������1���Ը���");
			CSomeFun::GetInstance().SimlationKey(VK_1);
			BnsSleep(500);
			CSomeFun::GetInstance().SimlationKey('F');
			continue;
		}

		LOG_CF_D( L"���ﳹ�׹���~");
		CCat Cat;
		if (!CCatExpend::GetInstance().FindSelfCat(&Cat))
			break;

		if (Cat.IsDead())
			return FALSE;


		LOG_CF_D(L"è��û��, ��R����!");
		while (GameRun && EMPTY_PERSONHP && !Cat.IsDead())
		{
			CSomeFun::GetInstance().SimlationKey('R');
			BnsSleep(1000);
		}
		LOG_CF_D(L"�������!");
		break;
	}
	return TRUE;
}

BOOL CPersonFunction::WaitToLoadProgressBar(DWORD dwSleepTime, DWORD dwMaxSleepTime, std::function<void(void)> f) CONST
{
	f();
	BnsSleep(dwSleepTime);

	CSomeFun::GetInstance().TimeOut_Condiction_GameRun(dwMaxSleepTime, []{
		return CPerson::GetInstance().IsLoadProgressBar() && !EMPTY_PERSONHP;
	});

	return BnsSleep(1000);
}

BOOL CPersonFunction::Pickup(CONST GameObjectID& GameObjectID_) CONST
{
	CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&GameObjectID_]{CGameSend::Pickup(GameObjectID_.dwId, GameObjectID_.dwIdParm); });
	return BnsSleep(1000);
}

//BOOL CPersonFunction::Turntable(_In_ DWORD dwAttendancePanel) CONST
//{
//	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&dwAttendancePanel]{CGameSend::Turntable(dwAttendancePanel); });
//}

BOOL CPersonFunction::CancelPutDownStatus() CONST
{
	// ���ֵ�ǰ״̬�Ǽ���״̬(������Ʒ|ʬ��), ��һֱ��4����
	while (GameRun && !EMPTY_PERSONHP && CPerson::GetInstance().IsPickStatus())
		CSomeFun::GetInstance().SimlationKey(VK_4, 200, 2 * 1000);

	return TRUE;
}

BOOL CPersonFunction::DoAction(DWORD dwActionId) CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([&dwActionId]{CGameSend::ActionCALL(dwActionId); });
}

BOOL CPersonFunction::DoEscape() CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([]{CGameSend::Escape(); });
}

DWORD CPersonFunction::GetMapId() CONST
{
	return ReadDWORD(CPerson::GetInstance().GetBase() + 0x90);
}

std::tuple<CONST std::wstring, CONST std::wstring, CONST std::wstring, CONST std::wstring, DWORD, DWORD, Point, Point> CPersonFunction::MakeBnsConfigParm(_In_ CONST std::wstring& wsMapName, _In_ CONST Point& TarPoint) CONST
{
	CONST CGameVariable::ProvinceInfo * pProvinceInfo = CGameVariable::GetInstance().FindProvinceInfoList(CPersonFunction::GetInstance().GetCityId());
	TaskInfoParm TaskParm = CTask::GetInstance().GetMianTaskInfo();

	CONST std::wstring wsAccountName = CGameVariable::GetInstance().GetAccShareInfo()->GetAccountInfo()->szUserName;
	CONST std::wstring wsPlayerName = CPerson::GetInstance().GetName();
	CONST std::wstring wsTarMapName = wsMapName;
	CONST std::wstring wsTarCityName = pProvinceInfo == nullptr ? L"" : pProvinceInfo->wsMapName;
	Point CurPoint = CPerson::GetInstance().GetPoint();
	Point TarPoint_ = CurPoint;
	
	return make_tuple(wsAccountName, wsPlayerName, wsTarMapName, wsTarCityName, TaskParm.dwTaskId, TaskParm.dwScheduleId, CurPoint, TarPoint_);
}

DWORD CPersonFunction::GetMaxChannel() CONST
{
	DWORD dwMaxChannel = NULL;
	for (auto i = 0; i < 3 && dwMaxChannel == NULL; ++i)
	{
		CGameVariable::GetInstance().PushMainThreadFunPtr_synch([]{CGameSend::RefushChannel(); });
		BnsSleep(5 * 1000);
		dwMaxChannel = ReadDWORD(CPerson::GetInstance().GetBase() + ���Ƶ��ƫ��);
	}

	if (dwMaxChannel != NULL)
	{
		for (DWORD i = 0; i < dwMaxChannel; ++i)
		{
			if (!((0x1 << i % 0x8) & ReadBYTE(ReadDWORD(CPerson::GetInstance().GetBase() + ���Ƶ��ƫ�� + 0xC))))
			{
				// set Max Channel = Last Channel = i (i + 1 = UnVisited Channel)
				dwMaxChannel = i;
				break;
			}
		}
	}

	return dwMaxChannel == NULL ? CPerson::GetInstance().GetChannel() : dwMaxChannel;
}

BOOL CPersonFunction::Exist_Lux_Tab_Buff() CONST
{
	CONST static std::vector<DWORD> Vec = {
		Lux_BuffId_Tab_1, Lux_BuffId_Tab_2
	};
	return ExistBuff_Person(Vec);
}

BOOL CPersonFunction::Exist_Lux_E_Buff() CONST
{
	return ExistBuff_Person(Lux_BuffId_E, nullptr);
}

BOOL CPersonFunction::Exist_Lux_V_Buff() CONST
{
	CONST static std::vector<DWORD> Vec = {
		Lux_BuffId_V_1, Lux_BuffId_V_2, Lux_BuffId_V_3, Lux_BuffId_V_4
	};
	return ExistBuff_Person(Vec);
}

BOOL CPersonFunction::WaitForProgressBarBySkill(_In_ DWORD dwMaxTimeOut) CONST
{
	return CSomeFun::GetInstance().TimeOut_Condiction_GameRun(dwMaxTimeOut, []{ return CPerson::GetInstance().IsLoadingByUseSkill(); });
}

BOOL CPersonFunction::Exist_Sumoner_X_Buff() CONST
{
	return ExistBuff_Person(Sumoner_BuffId_X, nullptr);
}

BOOL CPersonFunction::Exist_Sumoner_3_Buff() CONST
{
	return ExistBuff_Person(Sumoner_BuffId_3, nullptr);
}

BOOL CPersonFunction::WaitForWarStatus() CONST
{
	Point FixPoint = CPerson::GetInstance().GetPoint();
	while (GameRun && !EMPTY_PERSONHP)
	{
		if (!CPerson::GetInstance().IsWarOfStatus())
			return TRUE;

		CFarm::GetInstance().ClearAround_TarIdSelf(CBaseObject::CreateFixPointInfo(16));
		BnsMove(FixPoint, 3);
		BnsSleep(1000);
	}
	return FALSE;
}

BOOL CPersonFunction::ExitSchool() CONST
{
	return CGameVariable::GetInstance().PushMainThreadFunPtr_synch([]{CGameSend::ExitSchool(); });
}

CPersonFunction::CPersonFunction() : _dwMoveFlag(MOVE_FLAG_NORMAL), _dwMoveFlagBackup(NULL)
{
	
}

BOOL CPersonFunction::Run(_In_ em_Run_Type emRunType) CONST
{
	static DWORD dwRunBase = NULL;
	if (dwRunBase == NULL)
	{
		// ���аٱ��ַ
		dwRunBase = MyTools::CLSearchBase::FindBase_ByCALL("E8????????83C4??8D??????E8????????39??????0F84", 0x96E05BB - 0x96E05C7, reinterpret_cast<DWORD>(::GetModuleHandleW(L"bsengine_Shipping.dll")), 0x1, 0x0, L"bsengine_Shipping.dll", 0x9, 0xFFFFFFFF);
		if (dwRunBase == NULL)
		{
			CBnsGameFun::GetInstance().SetCloseGame(L"���аٱ��ַʧЧ!", em_Close_Result::em_Close_Result_ReStart);
		}
	}
	LOG_CF_D( L"dwAddr=%X", dwRunBase);
	BOOL bExist = FALSE;
	DWORD dwArrayCount1 = ReadDWORD(ReadDWORD(dwRunBase) + 0x48);
	for (DWORD i = 0; i < dwArrayCount1 && !bExist; ++i)
	{
		DWORD dwArrayAddr = ReadDWORD(ReadDWORD(ReadDWORD(dwRunBase) + 0x44) + i * 4);

		DWORD dwArrayCount2 = ReadDWORD(dwArrayAddr + 0x40);
		for (DWORD j = 0; j < dwArrayCount2; ++j)
		{
			DWORD dwAddr = ReadDWORD(ReadDWORD(dwArrayAddr + 0x3C) + j * 4);
			//LOG_CF_D( L"dwArrayAddr=%X", dwArrayAddr);
			//LOG_CF_D( L"dwArrayAddr=%X", dwAddr);
			LOG_CF_D( L"dwAddr+0x2c=%X", ReadDWORD(dwAddr + 0x2C));
#ifdef BnsProject_TaiWan
			if (ReadDWORD(dwAddr + 0x2C) == 0x1795)
#endif // BnsProject_TaiWan
#ifdef BnsProject_Korea
			if (ReadDWORD(dwAddr + 0x2C) == 0x17A1)
#endif // BnsProject_Korea

			
			{
				LOG_CF_D( L"dwAddr-for=%X", dwAddr + ���аٱ�ƫ��);
				if (emRunType == em_Run_Type::em_Run_Type_OnFloor)
				{
					//CCharacter::ReadDWORD(dwAddr + ���аٱ�ƫ��) & 0xFFFF0000;
#ifdef BnsProject_TaiWan
					MyTools::CCharacter::WriteDWORD(dwAddr + ���аٱ�ƫ��, 0xE420);
#endif // BnsProject_TaiWan
#ifdef BnsProject_Korea
					MyTools::CCharacter::WriteDWORD(dwAddr + ���аٱ�ƫ��, 0xD020);
#endif // BnsProject_Korea
				}
				else if (emRunType == em_Run_Type::em_Run_Type_FastFly)
				{
#ifdef BnsProject_TaiWan
					MyTools::CCharacter::WriteDWORD(dwAddr + ���аٱ�ƫ��, 0xF020);
#endif // BnsProject_TaiWan
#ifdef BnsProject_Korea
					MyTools::CCharacter::WriteDWORD(dwAddr + ���аٱ�ƫ��, 0xC020);
#endif // BnsProject_Korea
					
				}

				bExist = TRUE;
				break;
			}
		}
	}
	return TRUE;
}
