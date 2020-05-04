#include "stdafx.h"
#include "FarmSpecialTask.h"
#include <thread>
#include <MyTools/Log.h>
#include "PersonFunction.h"
#include "SomeFun.h"
#include "ResExtend.h"
#include "MonsterExtend.h"
#include "ObjectFunction.h"
#include "NpcExtend.h"
#include "FarmSkill.h"
#include "Person.h"
#include "Corpse.h"
#include "Monster.h"
#include "Npc.h"
#include "BnsFilter.h"
#include "TeamMember.h"
#include "TeamMemberExtend.h"
#include "Skill.h"
#include "ResAction.h"
#include "Cat.h"
#include "CatExpend.h"
#include "FarmBoss.h"
#if BnsProject_DLL
#include "ClientTeam.h"
#endif

#define _SELF L"FarmSpecialTask.cpp"


BOOL CFarmSpeialTask::ChooseColorTask(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<BOOL(VOID)> fnIsComplete) CONST
{
	std::vector<DWORD> vCorpseId = { 0x6, 0x5, 0x3, 0x2, 0x1, 0x0 };

	auto GetCorpseId_By_Text = [](_In_ WCHAR cText)
	{
		if (cText == L'황')
			return 2;
		if (cText == L'청')
			return 1;
		if (cText == L'백')
			return 5;
		if (cText == L'녹')
			return 3;
		if (cText == L'적')
			return 6;

		LOG_MSG_CF(L"握草, 这是什么梗!");
		return 0;
	};

	while (GameRun && !EMPTY_PERSONHP && !fnIsComplete() && vCorpseId.size() > 0)
	{
		BnsMove(Point(20052, -7522, 251), 3);

		CCorpse Corpse;
		if (!CResExtend::GetInstance().FindCorpse_By_Id(*vCorpseId.begin(), FixPtInfo, &Corpse))
		{
			LOG_CF_D( L"找不到石像任务的尸体ID:%X", *vCorpseId.begin());
			break;
		}

		Corpse.Take();
		BnsSleep(10 * 1000);

		CMonster Monster(NULL);
		if (!CMonsterExtend::GetInstance().FindMonster_By_Name(Monster_土像将军, CBaseObject::CreateFixPointInfo(5.0f), &Monster))
		{
			LOG_CF_D( L"附近找不到怪物, 应该是答对了!");
			vCorpseId.erase(vCorpseId.begin());
			continue;
		}

		// 杀死石像将军
		CFarm::GetInstance().KillMonster_By_Single(Monster, FixPtInfo, TRUE, []{return 1; });

		// 搜索关键字
		BnsSleep(5000);
		BOOL bFindTalkMsg = CObjectFunction::GetInstance().FindTalkMsg_By_Condiction([&vCorpseId, &GetCorpseId_By_Text](TalkMsg_Src& TalkMsg_){
			static CONST WCHAR wszText[9] = { L"...황청백녹적" };
			BOOL bExist = TRUE;
			for (UINT i = 0; i < 7; ++i)
			{
				if (TalkMsg_.wsText.find(wszText[i]) == -1)
				{
					bExist = FALSE;
					break;
				}
			}
			if (bExist)
			{
				LOG_CF_D( L"TalkMsg_.wsText=%s", TalkMsg_.wsText.c_str());
				vCorpseId.clear();
				vCorpseId.push_back(GetCorpseId_By_Text(TalkMsg_.wsText[0]));
				vCorpseId.push_back(GetCorpseId_By_Text(TalkMsg_.wsText[4]));
				vCorpseId.push_back(GetCorpseId_By_Text(TalkMsg_.wsText[8]));
				vCorpseId.push_back(GetCorpseId_By_Text(TalkMsg_.wsText[12]));
				vCorpseId.push_back(GetCorpseId_By_Text(TalkMsg_.wsText[16]));
			}
			return bExist;
		});
		LOG_CF_D( L"%s", bFindTalkMsg ? L"找到字幕了" : L"找不到怪物在吹什么!");

		CCorpse Pillare;
		if (!CResExtend::GetInstance().FindCorpse_By_Id(0x7, FixPtInfo, &Pillare))
		{
			LOG_CF_D( L"找不到还原柱子的尸体ID:%X", 0x7);
			break;
		}

		Pillare.Take();
		BnsSleep(5 * 1000);
	}

	return TRUE;
}
BOOL CFarmSpeialTask::SpecialTask_Ice_Fire_Shield(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<BOOL(VOID)> fnIsComplete) CONST
{
	CMonster Monster;
	CCorpse Corpse_Ice;
	CCorpse Corpse_Fire;
	if (!CMonsterExtend::GetInstance().FindMonster_By_Name(Monster_姜铁忠, FixPtInfo, &Monster))
	{
		LOG_CF_D( L"找不到冰火罩子的怪物:강철중");
		return FALSE;
	}
	if (!CResExtend::GetInstance().FindCorpse_By_Id(0x13, FixPtInfo, &Corpse_Ice))
	{
		LOG_CF_D( L"找不到冰罩子");
		return FALSE;
	}
	if (!CResExtend::GetInstance().FindCorpse_By_Id(0x14, FixPtInfo, &Corpse_Fire))
	{
		LOG_CF_D( L"找不到火罩子");
		return FALSE;
	}

	while (GameRun && !EMPTY_PERSONHP && !fnIsComplete() && !Monster.IsDead())
	{
		BnsSleep(500);
		if (Corpse_Ice.IsTaskCollect())
		{
			LOG_CF_D( L"可以去采集冰罩子了!");
			Corpse_Ice.Take();
		}
		else if (Corpse_Fire.IsTaskCollect())
		{
			LOG_CF_D( L"可以去采集火罩子了!");
			Corpse_Fire.Take();
		}
		else if (Monster.GetDis(FixPtInfo.FixPoint) < 8.0f)
		{
			BnsMove(Point(7544, -14952, -1680), 3);
			Monster.SetAngle();
		}
		else
		{
			BnsMove(FixPtInfo.FixPoint, 3);
			Monster.SetAngle();
		}

		while (GameRun && !EMPTY_PERSONHP && CPerson::GetInstance().IsPickStatus())
		{
			if (Monster.GetDis() > 10.0f)
			{
				BnsMove(Monster.GetPoint(), 10);
				CPersonFunction::GetInstance().Move(CBaseObject::GetPersonPoint());
			}

			LOG_CF_D( L"用枪喷它!");
			Monster.SetAngle();
			CSomeFun::GetInstance().SimlationKey('R');
			BnsSleep(500);
		}

	}
	return TRUE;
}

BOOL CFarmSpeialTask::SpecialTask_3A9(_In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<BOOL(VOID)> fnIsComplete) CONST
{
	// 法师 力士 拳师 刺客 
	static CONST std::vector<std::wstring> wsArray = { Monster_黑蛇门尹鹗, Monster_南道派泰广, Monster_义贤门南宫载, Monster_寒坤派胡越灵 };

	BOOL bThread = TRUE;
	std::thread t([&bThread]{
		while (GameRun && bThread)
		{
			BnsSleep(300);
			if (!CPersonFunction::GetInstance().IsGameLoad() && CFarmSkill::GetInstance().IsExist_By_SkillId_In_Ui(Lux_SkillId_Floor_F))
				CFarmSkill::GetInstance().UseSkill_By_SkillId(Lux_SkillId_Floor_F);
		}
	});

	auto fnIsLoseHp = [](_In_ CMonster& Monster, ULONGLONG& ulTick, _In_ DWORD& dwLastHp)
	{
		BOOL bLoseHp = TRUE;
		if (Monster.GetHp() == dwLastHp)
		{
			if (static_cast<DWORD>(::GetTickCount64() - ulTick) >= 4 * 1000)
			{
				bLoseHp = FALSE;
				LOG_CF_D( L"4秒不掉血了!");
			}

		}
		else if (Monster.GetHp() > dwLastHp)
		{
			LOG_CF_D( L"突然满血了, 不打了!");
			return FALSE;
		}
		else
		{
			dwLastHp = Monster.GetHp();
			ulTick = ::GetTickCount64();
		}
		return bLoseHp;
	};

	while (GameRun && !EMPTY_PERSONHP && !fnIsComplete())
	{
		BnsSleep(100);

		CNpc Npc;
		for (CONST auto& itm : wsArray)
		{
			BnsMove(FixPtInfo.FixPoint, 3);
			if (CPersonFunction::GetInstance().IsGameLoad())
			{
				CPersonFunction::GetInstance().WaitGameLoad();
				break;
			}

			if (!CNpcExtend::GetInstance().FindNpc_By_Name(itm, FixPtInfo, &Npc))
				continue;

			LOG_CF_D( L"找到Npc:%s", Npc.GetName().c_str());
			CMonster Monster(Npc);
			auto ulTick = ::GetTickCount64();
			auto dwHp = Monster.GetHp();
			auto& FarmSkill = CFarmSkill::GetInstance();
			//BOOL bKaDao = Monster.GetName() == L"흑사문 은악" ? TRUE : FALSE;
			LOG_CF_D( L"开始干%s!", Monster.GetName().c_str());

			while (GameRun && !EMPTY_PERSONHP && !Monster.IsDead() && fnIsLoseHp(Monster, ulTick, dwHp))
			{
				if (CPersonFunction::GetInstance().IsGameLoad())
				{
					CPersonFunction::GetInstance().WaitGameLoad();
					break;
				}

				if (Monster.GetDis() > 3.0f)
				{
					if (!CFarm::GetInstance().Move_To_Monster(Monster, FixPtInfo))
					{
						LOG_CF_D( L"轮4个大汉时,无法移动到怪物旁边! ID=%X, Name=%s, HP=%d,Dis=%.2f", Monster.GetId(), Monster.GetName().c_str(), Monster.GetHp(), Monster.GetDis());
						break;
					}
				}

				Monster.SetAngle(); // 对准

				// 用V加血(内置有判断条件)
				if (CFarm::GetInstance().UseLux_V(Monster))
					continue;

				// 使用技能
				if (CPerson::GetInstance().GetPercentHp() < 90 && CPerson::GetInstance().GetMp() >= 20 && FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_X))
				{
					LOG_CF_D( L"使用X!");
					FarmSkill.UseSkill_By_SkillId(Lux_SkillId_X);
				}
				if (CNpcExtend::GetInstance().GetAroundNpcCount(CBaseObject::CreateFixPointInfo(5.0f)) > 2 && FarmSkill.UseSkill_By_Destoer_Q())
				{
					BnsSleep(1500);
				}


				if (FarmSkill.UseSkill_By_Destoer_E())
				{
					BnsSleep(100);
				}
				if (FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_3))
				{
					CSkill::UseSkill_By_Id(Lux_SkillId_3);
				}
				if (FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_1))
				{
					FarmSkill.UseSkill_By_SkillId(Lux_SkillId_1);
				}
				if (FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_2))
				{
					FarmSkill.UseSkill_By_SkillId(Lux_SkillId_2);
				}
				if (FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_4))
				{
					FarmSkill.UseSkill_By_SkillId(Lux_SkillId_4);
				}
				if (FarmSkill.IsCool_By_SkillId_In_Ui(Lux_SkillId_Z_Attract))
				{
					FarmSkill.UseSkill_By_SkillId(Lux_SkillId_Z_Attract);
				}
				// HP > 50 使用Tab
				if (CPerson::GetInstance().GetPercentHp() >= 70 && CNpcExtend::GetInstance().GetAroundNpcCount(CBaseObject::CreateFixPointInfo(3.0f)) < 2)
				{
					if (CPerson::GetInstance().GetMp() >= 45)
					{
						CSkill::UseSkill_By_Id(Lux_SkillId_T, TRUE);
						BnsSleep(150);
						if (CPersonFunction::GetInstance().Exist_Lux_E_Buff())
						{
							CSkill::UseSkill_By_Id(Lux_SkillId_E_R, TRUE);
						}
						else
						{
							CSkill::UseSkill_By_Id(Lux_SkillId_R, TRUE);
						}
						BnsSleep(100);
					}
					else
					{
						CSkill::UseSkill_By_Id(Lux_SkillId_R, TRUE);
						BnsSleep(50);
					}
					continue;
				}
				else if (CPerson::GetInstance().GetMp() > 20)
				{
					CPersonFunction::GetInstance().Move(Monster.GetBehindPoint());
					Monster.SetAngle(); // 对准
					FarmSkill.UseSkill_BySkillId_In_Sleep(Lux_SkillId_Tab);
				}
				else if (CPersonFunction::GetInstance().Exist_Lux_Tab_Buff())
				{
					CPersonFunction::GetInstance().Move(Monster.GetBehindPoint());
					Monster.SetAngle(); // 对准
				}
				CSkill::UseSkill_By_Id(Lux_SkillId_V_R);
				BnsSleep(100);
			}
		}
	}
	bThread = FALSE;
	t.join();
	return TRUE;
}

BOOL CFarmSpeialTask::SpecialTask_135(_In_ std::function<BOOL(VOID)> fnIsComplete) CONST
{
#ifdef BnsProject_TaiWan
	CONST std::wstring wsNpcName = L"夏焌光";
	static CONST std::vector<Point> vPoint = {
		Point(-47248, -18836, -2856), Point(-46624, -17816, -2856), Point(-47532, -16328, -2868), 
		Point(-46024, -16368, -2864), Point(-46288, -13792, -2884), Point(-46628, -15080, -2884), Point(-46512, -15080, -2836)
	};

	static CONST std::vector< std::wstring> vCorpseName = {
		L"炸彈配件箱", L"炸藥桶", L"引爆裝置"
	};
#endif // BnsProject_TaiWan
#ifdef BnsProject_Korea
	CONST std::wstring wsNpcName = Npc_南郭;
	static CONST std::vector<Point> vPoint = {
		Point(-47248, -18836, -2856), Point(-46624, -17816, -2856), Point(-47532, -16328, -2868),
		Point(-46024, -16368, -2864), Point(-46288, -13792, -2884), Point(-46628, -15080, -2884), Point(-46512, -15080, -2836)
	};

	static CONST std::vector< std::wstring> vCorpseName = {
		Corpse_炸弹部件箱, Corpse_炸药桶, Corpse_引爆装置
	};
#endif // BnsProject_TaiWan

	// 隐形怪不杀!
	CFilter::GetInstance().AddMonsterFilter(Monster_南郭的炸弹, CFilter::MonsterFilterType::Filter_Monster_Low);
	CFilter::GetInstance().AddMonsterFilter(L"夏焌光的炸彈", CFilter::MonsterFilterType::Filter_Monster_Low);
	CNpc Npc;
	if (!CNpcExtend::GetInstance().FindNpc_By_Name(wsNpcName,CBaseObject::CreateFixPointInfo(50.0f), &Npc))
	{
#if BnsProject_Dlg
		LOG_MSG_CF(L"找不到跟随的Npc:%s!!!", wsNpcName.c_str());
#endif
#if BnsProject_DLL
		CGameVariable::GetInstance().SetForceBackToSwitchPlayer();
#endif
		return FALSE;
	}

	auto fnIsClosePoint = [&Npc]
	{
		return std::find_if(vPoint.begin(), vPoint.end(), [&Npc](CONST Point& Tar){
			return Npc.GetDis(Tar) < 8.0f;
		}) != vPoint.end();
	};

	auto fnFindNearCorpse = [](_Out_opt_ CCorpse& Corpse)
	{
		return std::find_if(vCorpseName.begin(), vCorpseName.end(), [&Corpse](_In_ CONST  std::wstring& wsCorpseName){
			return CResExtend::GetInstance().FindCorpse_By_Name(wsCorpseName, CBaseObject::CreateFixPointInfo(10.0f), &Corpse);
		}) != vCorpseName.end();
	};

	DWORD dwFoodPercent = CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_FOODPERCENT, 1);
	while (GameRun && !EMPTY_PERSONHP && !CPerson::GetInstance().IsDead() && Npc.GetDis() < 50.0f)
	{
		CPersonFunction::GetInstance().Move(Npc.GetPoint());
		BnsSleep(300);

		if (fnIsClosePoint())
		{
			Point NpcPoint = Npc.GetPoint();
			LOG_CF_D( L"靠近了一个坐标, 清怪后搜索尸体!Point:[%d,%d,%d]", NpcPoint.GetX(), NpcPoint.GetY(), NpcPoint.GetZ());
			CFarm::GetInstance().ClearAroundMonster(FixPointInfo(NpcPoint, 10), []{return FALSE; });
			CCorpse Corpse;
			if (!fnFindNearCorpse(Corpse))
			{
				LOG_CF_D( L"附近并没有适合的尸体!");
				continue;
			}
#ifdef BnsProject_TaiWan
			if (!Corpse.IsCollect() && Corpse.GetName() == L"引爆裝置")
			{
				LOG_CF_D( L"采集到最后一个爆炸杆了!");
				break;
			}
#endif //BnsProject_TaiWan
#ifdef BnsProject_Korea
			if (!Corpse.IsCollect() && Corpse.GetName() == Corpse_引爆装置)
			{
				LOG_CF_D( L"采集到最后一个爆炸杆了!");
				break;
			}
#endif //BnsProject_Korea

			if (!Corpse.IsCollect())
			{
				LOG_CF_D( L"该尸体已经被采集过了!");
				continue;
			}
			if (!Corpse.IsTaskCollect())
			{
				LOG_CF_D( L"该尸体暂时还不能采集!");
				continue;
			}
			LOG_CF_D( L"去采集尸体:%s", Corpse.GetName().c_str());
			Corpse.Take();
			continue;
		}

		CFarm::GetInstance().ClearAroundMonster(CBaseObject::CreateFixPointInfo(5.0f), []{return FALSE; });
	}

	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_FOODPERCENT, dwFoodPercent);
	return TRUE;
}

BOOL CFarmSpeialTask::Defence_GuiMen(_In_ CONST std::wstring& wsDoorName, _In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<VOID(VOID)> LeaveTeamPtr) CONST
{
#if BnsProject_DLL
	auto& DungoneSchedule_ = CClientTeam::GetInstance().GetTeamDungoenSchedule();
#endif
	while (GameRun && !EMPTY_PERSONHP)
	{
#if BnsProject_DLL
		// 判断是否有一方的2个号都掉线了. 是的话就退队!
		for (UINT i = 0; i < DungoneSchedule_.VecMember.size(); i += 2)
		{
			BOOL bLose = FALSE;
			CTeamMemberExtend::GetInstance().Action_By_TeamMemberName(DungoneSchedule_.VecMember.at(i).wsPlayerName, [&bLose](CONST CTeamMember& TeamMember_){ bLose = !TeamMember_.IsConnect(); });
			if (bLose)
			{
				// 已经有一个没了。 
				if (i + 1 < DungoneSchedule_.VecMember.size())
				{
					LOG_CF_D( L"队友:[%s,%x]掉线了,他就一个人守关的都掉了。没法玩了!", DungoneSchedule_.VecMember.at(i).wsAccountName.c_str(), DungoneSchedule_.VecMember.at(i).wsPlayerName.c_str());
					LeaveTeamPtr();
					return FALSE;
				}
				CTeamMemberExtend::GetInstance().Action_By_TeamMemberName(DungoneSchedule_.VecMember.at(i + 1).wsPlayerName, [&bLose](CONST CTeamMember& TeamMember_){ bLose = !TeamMember_.IsConnect(); });
				if (bLose)
				{
					LOG_CF_D( L"2个一起守关的队友都掉线了!  没法玩了!");
					LeaveTeamPtr();
					return FALSE;
				}
			}
		}
#endif

		CMonster Monster;
		if (CMonsterExtend::GetInstance().GetTopMonster_By_Dis(FixPtInfo, &Monster))
		{
			CFarm::GetInstance().KillMonster_By_Single(Monster, FixPtInfo, TRUE, nullptr);
			continue;
		}
		BnsMove(FixPtInfo.FixPoint, 3);

		CCorpse Door;
		if (CResExtend::GetInstance().FindCorpse_By_Name(wsDoorName, CBaseObject::CreateFixPointInfo(-1), &Door) && !Door.IsCollect())
		{
			LOG_CF_D( L"大门已开! 可以跑路了!");
			break;
		}

		if (CObjectFunction::GetInstance().FindTalkMsg_By_Condiction([](TalkMsg_Src& TS){ return TS.wsText.find(Text_鬼门关入口已打开) != -1;  }))
		{
			//  将聊天消息内容清理掉!
			CObjectFunction::GetInstance().SetTalkMsg_By_Condition(L"Empty", [](CONST TalkMsg_Src& TalkMsg){ return TalkMsg.wsText.find(Text_鬼门关入口已打开) != -1; });
			LOG_CF_D( L"出现鬼门关开启的消息了!");
			break;
		}

		BnsSleep(500);
	}
	return TRUE;
}

BOOL CFarmSpeialTask::Defence_BlackForest(_In_ CONST std::wstring& wsCoreName, _In_ DWORD dwCoreIndex, _In_ CONST FixPointInfo& FixPtInfo, _In_ std::function<VOID(VOID)> LeaveTeamPtr) CONST
{
	int nPutForceMasterNpcCount = 0;
	int nPutPikeNpcCount		= 0;
	CCat Cat;
	if (!CCatExpend::GetInstance().FindSelfCat(&Cat))
		return TRUE;

	DWORD dwTeamIndex = CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex();

	float fKillDis = 0;
	switch (dwTeamIndex)
	{
	case 0: case 1:
		// 专杀幻魔, 血魔犬
		CFilter::GetInstance().AddMonsterFilter(Monster_幻魔, CFilter::MonsterFilterType::Filter_Monster_High);
		CFilter::GetInstance().AddMonsterFilter(Monster_血魔犬, CFilter::MonsterFilterType::Filter_Monster_High);
		fKillDis = 11;
		break;
	case 2: case 3:
		// 力魔, 剑魔, 奇魔,强魔, 拳魔
		CFilter::GetInstance().AddMonsterFilter(Monster_力魔, CFilter::MonsterFilterType::Filter_Monster_High);
		CFilter::GetInstance().AddMonsterFilter(Monster_剑魔, CFilter::MonsterFilterType::Filter_Monster_High);
		CFilter::GetInstance().AddMonsterFilter(Monster_奇魔, CFilter::MonsterFilterType::Filter_Monster_High);
		CFilter::GetInstance().AddMonsterFilter(Monster_强魔, CFilter::MonsterFilterType::Filter_Monster_High);
		CFilter::GetInstance().AddMonsterFilter(Monster_拳魔, CFilter::MonsterFilterType::Filter_Monster_High);
		fKillDis = 14;
		break;
	case 4: case 5:
		// 怨魔 骨魔 轴魔
		CFilter::GetInstance().AddMonsterFilter(Monster_怨魔, CFilter::MonsterFilterType::Filter_Monster_High);
		CFilter::GetInstance().AddMonsterFilter(Monster_骨魔, CFilter::MonsterFilterType::Filter_Monster_High);
		CFilter::GetInstance().AddMonsterFilter(Monster_轴魔, CFilter::MonsterFilterType::Filter_Monster_High);
		fKillDis = 13;
		break;
	default:
		break;
	}
	DWORD dwOldSet = CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_DISABLE_BECKONCAT, TRUE);
	CPersonFunction::GetInstance().SetMoveFlag(MOVE_FLAG_DISABLE_JUMP | MOVE_FLAG_DISABLE_SPEED | MOVE_FLAG_DISABLE_SS);
	while (GameRun && !EMPTY_PERSONHP)
	{
#if BnsProject_DLL
		if (CTeamMemberExtend::GetInstance().GetTeamMemberCount() < 5)
		{
			LOG_CF_D( L"队伍人数=%d, 有人掉线了! 不打了!, 跑路保平安!", CTeamMemberExtend::GetInstance().GetTeamMemberCount());
			LeaveTeamPtr();
			break;
		}
#endif

		// 第一波防守
		if (dwCoreIndex == 0x0 && (dwTeamIndex == 0x2 || dwTeamIndex == 0x3) && Defence_BlackForest_Help(nPutForceMasterNpcCount, nPutPikeNpcCount))
			continue;
		
		// 第二波防守
		if (dwCoreIndex == 0x1)
		{
			// 小型狂魔人, 中型狂魔人，大型狂魔人
			CONST static std::vector<std::wstring> FirstKillVec = { Monster_小型狂魔人, Monster_中型狂魔人, Monster_大型狂魔人 };
			CMonster SpecialMonster;

			CONST static float fSpecialKillDis = 45.0f;
			auto Ptr = MyTools::CLPublic::Vec_find_if_Const(FirstKillVec, [&SpecialMonster](CONST std::wstring wsMonsterName){ return CMonsterExtend::GetInstance().FindMonster_By_Name(wsMonsterName, CBaseObject::CreateFixPointInfo(fSpecialKillDis), &SpecialMonster); });
			if (Ptr != nullptr)
			{
				LOG_CF_D( L"发现狂魔人!!!");
				CSomeFun::GetInstance().SetSpeed(3);
				CPersonFunction::GetInstance().MoveToPoint(SpecialMonster.GetFacePoint(20), 3, 100);

				// 设置一直按S后退
				CFarm::GetInstance().BackwardWhenClosing(SpecialMonster, TRUE, 10, 14);

				// 打怪过程中禁止走路
				CFarm::GetInstance().SetMoveToMonster(FALSE);
				CFarm::GetInstance().KillMonster_By_Single(SpecialMonster, CBaseObject::CreateFixPointInfo(fSpecialKillDis), TRUE, nullptr);

				// 恢复走路
				CFarm::GetInstance().SetMoveToMonster(TRUE);
				CFarm::GetInstance().BackwardWhenClosing(SpecialMonster, FALSE, 10, 14);
				LOG_CF_D( L"杀完狂魔人回家!");
				CPersonFunction::GetInstance().MoveToPoint(FixPtInfo.FixPoint, 3, 100);
				continue;
			}
		}

		CMonster DemonShadowCBig;
		if (dwCoreIndex == 0x0 && CMonsterExtend::GetInstance().FindMonster_By_Name(Monster_风魔人, CBaseObject::CreateFixPointInfo(25), &DemonShadowCBig))
		{
			LOG_CF_D( L"去杀风魔人!");
			CFarm::GetInstance().KillMonster_By_Single(DemonShadowCBig, CBaseObject::CreateFixPointInfo(25), TRUE, nullptr);
			continue;
		}

		CMonster Monster;
		if (CMonsterExtend::GetInstance().GetTopMonster_By_Dis(FixPtInfo, &Monster))
		{
			CFarm::GetInstance().KillMonster_By_Single(Monster, FixPtInfo, TRUE, nullptr);
			continue;
		}

		BnsMove(FixPtInfo.FixPoint, 3);
		if (Cat.IsDead())
		{
			LOG_CF_D(L"猫挂了!");
			CFarmBoss::GetInstance().ForceBeckonCat(Cat);
			CCatExpend::GetInstance().FindSelfCat(&Cat);
		}

		CMonster Core;
		if (CMonsterExtend::GetInstance().FindMonster_By_Name(wsCoreName, FixPtInfo, &Core))
		{
			if (Core.GetHp() == Core.GetMaxHp() && BnsSleep(3 * 1000) && Core.GetHp() == Core.GetMaxHp())
			{
				LOG_CF_D( L"守点完成!");
				break;
			}
		}
		//else
			//LOG_CF_D( L"遍历不到封魔核是什么鬼?");

		BnsSleep(500);
	}

	CGameVariable::GetInstance().SetVariable_By_Name(VARIABLE_NAME_DISABLE_BECKONCAT, dwOldSet);
	CPersonFunction::GetInstance().RestoreMoveFlag();
	return CPersonFunction::GetInstance().CancelPutDownStatus();
}

BOOL CFarmSpeialTask::Defence_BlackForest_Help(_In_ _Out_ int& nPutForceMasterNpcCount, _In_ _Out_ int& nPutPikeNpcCount) CONST
{
	struct ForceMasterPoint
	{
		UINT  uIndex;
		Point Pt;
	};

	CONST static std::vector<ForceMasterPoint> ForceMasterPointVec = 
	{
		{ 0, Point(-4300, -15556, -206) }, { 1, Point(-4140, -12332, 585) }, { 2, Point(-2068, -14944, -144) },
		{ 3, Point(-9464, -15944, -510) }, { 4, Point(-2916, -7932, 556) }, { 5, Point(492, -8244, 288) }
	};

	CONST static std::vector<ForceMasterPoint> PikePointVec = {
		{ 0, Point(-5552, -16016, -200) }, { 1, Point(-6480, -18244, -161) }, { 2, Point(-4924, -12600, 651) },
		{ 3, Point(-3648, -11980, 553) }, { 4, Point(-2016, -12724, -124) }, { 5, Point(-1200, -16096, -135) }
	};

	CONST auto& HelpVec = GetBlackForestForceMasterHelpVec();

	std::vector<CMonster> ForceMasterList;
	if (CMonsterExtend::GetInstance().GetAroundMonsterListByName(Monster_作战队气功师, CBaseObject::CreateFixPointInfo(30), ForceMasterList) < 3)
	{
		CMonster ForceMaster;
		if (!CMonsterExtend::GetInstance().FindMonster_By_Name(Monster_负伤的冲角团气功师, CBaseObject::CreateFixPointInfo(-1), &ForceMaster))
		{
			BnsMove(Point(-5017, -11158, 639), 3);
			BnsSleep(1000);
			CMonsterExtend::GetInstance().FindMonster_By_Name(Monster_负伤的冲角团气功师, CBaseObject::CreateFixPointInfo(-1), &ForceMaster);
		}

		if (ForceMaster.GetBase() != NULL)
		{
			auto Ptr = MyTools::CLPublic::Vec_find_if_Const(ForceMasterPointVec, [&ForceMaster](CONST ForceMasterPoint& Fmp){ return ForceMaster.GetDis(Fmp.Pt) < 3; });
			if (Ptr != nullptr)
			{
				LOG_CF_D( L"去气功%d的区域", Ptr->uIndex + 1);
				if (Ptr->uIndex + 1 >= HelpVec.size())
				{
					LOG_MSG_CF(L"Ptr->uIndex + 1=%d, HelpVec.size()=%d", Ptr->uIndex + 1, HelpVec.size());
					return FALSE;
				}
				Defence_BlackForest_Help_Move(HelpVec.at(Ptr->uIndex));
			}

			// 判断身上没有捡物的BUFF的话, 那就不要去放东西!
			for (int i = 0; i < 5 && GameRun && !EMPTY_PERSONHP && CPersonFunction::GetInstance().ExistBuff_Person(0x4C4C62A, nullptr); ++i)
			{
				LOG_CF_D( L"nPutForceMasterNpcCount=%d", nPutForceMasterNpcCount);
				switch (nPutForceMasterNpcCount)
				{
				case 0:
					BnsMove(Point(-4814, -9862, 602), 3);
					break;
				case 1:
					BnsMove(Point(-4813, -10051, 602), 3);
					break;
				case 2:
					BnsMove(Point(-4814, -9663, 602), 3);
					break;
				case 3:
					BnsMove(Point(-4617, -9663, 602), 3);
					break;
				case 4:
					BnsMove(Point(-4620, -9865, 602), 3);
					break;
				case 5:
					BnsMove(Point(-4618, -10056, 602), 3);
					break;
				default:
					break;
				}

				CFarm::GetInstance().TakeCorpse(NULL, CBaseObject::CreateFixPointInfo(5), Monster_义马郑);
				nPutForceMasterNpcCount = nPutForceMasterNpcCount + 1 > 5 ? 0 : nPutForceMasterNpcCount + 1;
			}
			return TRUE;
		}

		return FALSE;
	}
	
	//LOG_CF_D( L"气功士已满3个, 判断-突擊組長 徐召風");
	if (CMonsterExtend::GetInstance().GetAroundMonsterListByName(Monster_突击队西洋枪手, CBaseObject::CreateFixPointInfo(30), ForceMasterList) < 3)
	{
		// 作業組長 高粱玖还没有出现,继续找气功士
		CMonster Pike;
		if (!CMonsterExtend::GetInstance().FindMonster_By_Name(Monster_负伤的冲角团西洋枪手, CBaseObject::CreateFixPointInfo(-1), &Pike))
		{
			BnsMove(Point(-5017, -11158, 639), 3);
			BnsSleep(1000);
			CMonsterExtend::GetInstance().FindMonster_By_Name(Monster_负伤的冲角团西洋枪手, CBaseObject::CreateFixPointInfo(-1), &Pike);
		}

		if (Pike.GetBase() != NULL)
		{
			auto Ptr = MyTools::CLPublic::Vec_find_if_Const(PikePointVec, [&Pike](CONST ForceMasterPoint& Fmp){ return Pike.GetDis(Fmp.Pt) < 3; });
			if (Ptr != nullptr)
			{
				LOG_CF_D( L"去枪手%d的区域", Ptr->uIndex + 1);
				if (Ptr->uIndex + 6 >= HelpVec.size())
				{
					LOG_MSG_CF(L"Ptr->uIndex + 1=%d, HelpVec.size()=%d", Ptr->uIndex + 6, HelpVec.size());
					return FALSE;
				}
				// +6 是因为前面的6个都是气功
				Defence_BlackForest_Help_Move(HelpVec.at(Ptr->uIndex + 6));
			}

			// 判断身上没有捡物的BUFF的话, 那就不要去放东西!
			for (int i = 0; i < 5 && GameRun && !EMPTY_PERSONHP && CPersonFunction::GetInstance().ExistBuff_Person(0x4C4C62A, nullptr); ++i)
			{
				LOG_CF_D( L"nPutPikeNpcCount=%d", nPutPikeNpcCount);
				switch (nPutPikeNpcCount)
				{
				case 0:
					BnsMove(Point(-5222, -9414, 602), 3);
					break;
				case 1:
					BnsMove(Point(-5398, -9498, 602), 3);
					break;
				case 2:
					BnsMove(Point(-5576, -9602, 602), 3);
					break;
				case 3:
					BnsMove(Point(-5663, -9422, 602), 3);
					break;
				case 4:
					BnsMove(Point(-5492, -9328, 602), 3);
					break;
				case 5:
					BnsMove(Point(-5322, -9236, 602), 3);
					break;
				default:
					break;
				}

				CFarm::GetInstance().TakeCorpse(NULL, CBaseObject::CreateFixPointInfo(5), Monster_义马郑);
				nPutPikeNpcCount = nPutPikeNpcCount + 1 > 5 ? 0 : nPutPikeNpcCount + 1;
			}
			return TRUE;
		}

		return FALSE;
	}
	return FALSE;
}

CONST std::vector<CFarmSpeialTask::BlackForestForceMasterHelp>& CFarmSpeialTask::GetBlackForestForceMasterHelpVec() CONST
{
	// 气功地点1
	CONST static std::vector<Point> ForceMasterHelpPointVec1_Front =
	{
		Point(-5190, -10229, 599), Point(-5195, -10601, 599), Point(-5289, -10862, 600), Point(-5341, -11121, 626), Point(-5393, -11423, 656),
		Point(-5488, -11708, 652), Point(-5640, -11969, 652), Point(-5797, -12203, 638), Point(-5978, -12436, 622), Point(-6210, -12677, 598),
		Point(-6369, -12854, 610), Point(-6369, -12854, 610), Point(-6513, -13026, 624), Point(-6513, -13026, 624), Point(-6600, -13211, 612),
		Point(-6611, -13462, 603), Point(-6548, -13701, 593), Point(-6554, -13959, 539), Point(-6692, -14216, 444), Point(-6766, -14530, 221),
		Point(-6434, -14765, -22), Point(-6276, -14879, -70), Point(-5993, -15031, -133), Point(-5770, -15122, -152), Point(-5509, -15237, -188),
		Point(-5316, -15306, -202), Point(-5316, -15306, -202), Point(-5108, -15368, -208), Point(-4954, -15404, -208), Point(-4767, -15458, -208),
		Point(-4561, -15534, -208), Point(-4337, -15605, -208),
	};

	CONST static std::vector<Point> ForceMasterHelpPointVec1_Back =
	{
		Point(-4497, -15634, -209), Point(-4835, -15601, -209), Point(-5208, -15526, -207), Point(-5582, -15446, -189), Point(-5583, -15446, -189),
		Point(-5854, -15343, -162), Point(-6170, -15159, -120), Point(-6328, -14854, -59), Point(-6433, -14675, 22), Point(-6557, -14565, 123),
		Point(-6691, -14477, 234), Point(-6711, -14361, 338), Point(-6599, -14220, 417), Point(-6502, -14042, 494), Point(-6503, -13862, 557),
		Point(-6545, -13642, 609), Point(-6567, -13324, 596), Point(-6578, -13138, 619), Point(-6415, -12909, 615), Point(-6111, -12611, 606),
		Point(-5902, -12475, 621), Point(-5631, -12290, 631), Point(-5564, -12025, 652), Point(-5482, -11640, 657), Point(-5406, -11275, 641),
		Point(-5406, -11275, 641), Point(-5315, -10840, 599), Point(-5290, -10636, 598), Point(-5224, -10471, 598), Point(-5150, -10161, 598),
		Point(-5110, -9972, 598), Point(-4780, -9837, 602),
	};

	// 气功地点2
	CONST static std::vector<Point> ForceMasterHelpPointVec2_Front =
	{
		Point(-5037, -10252, 599), Point(-4862, -10645, 601), Point(-4711, -11161, 614), Point(-4559, -11561, 618),
		Point(-4476, -11852, 622), Point(-4338, -12281, 602), Point(-4144, -12332, 602),
	};

	CONST static std::vector<Point> ForceMasterHelpPointVec2_Back =
	{
		Point(-4452, -11796, 612), Point(-4501, -11632, 613), Point(-4621, -11291, 603), Point(-4712, -10945, 608),
		Point(-4847, -10638, 600), Point(-4945, -10305, 598), Point(-5008, -10107, 599), Point(-4756, -9858, 599),
	};

	// 气功地点3
	CONST static std::vector<Point> ForceMasterHelpPointVec3_Front =
	{
		Point(-4709, -9989, 598), Point(-4383, -10100, 598), Point(-4081, -10193, 598), Point(-3699, -10246, 598),
		Point(-3419, -10264, 633), Point(-3073, -10263, 589), Point(-2764, -10227, 534), Point(-2409, -10177, 378),
		Point(-2412, -10382, 335), Point(-2427, -10927, 145), Point(-2391, -11390, 26), Point(-2188, -11643, 6),
		Point(-1778, -12189, -71), Point(-1489, -12681, -169), Point(-1306, -13008, -175), Point(-1303, -13266, -177),
		Point(-1307, -13591, -173), Point(-1379, -13835, -151), Point(-1407, -14108, -134), Point(-1438, -14390, -124),
		Point(-1568, -14611, -118), Point(-1795, -14764, -126), Point(-2067, -14861, -139),
	};

	CONST static std::vector<Point> ForceMasterHelpPointVec3_Back =
	{
		Point(-2353, -14612, -151), Point(-2631, -14444, -152), Point(-2946, -14248, -183), Point(-3240, -13994, -163),
		Point(-3481, -13768, -86), Point(-3714, -13503, 27), Point(-3213, -13375, -77), Point(-3094, -13119, -73),
		Point(-2899, -12798, -92), Point(-2657, -12468, -105), Point(-2476, -12186, -66), Point(-2564, -11786, 3),
		Point(-2617, -11469, 71), Point(-2698, -11220, 187), Point(-2734, -10933, 320), Point(-2532, -10537, 299),
		Point(-2419, -10338, 350), Point(-2580, -10193, 427), Point(-2696, -10082, 505), Point(-2944, -9740, 541),
		Point(-3184, -9354, 566), Point(-3384, -9093, 598), Point(-3682, -8735, 579), Point(-3989, -8513, 581),
		Point(-4319, -8672, 597), Point(-4547, -9054, 598), Point(-4721, -9351, 598), Point(-4885, -9624, 598),
		Point(-4934, -9910, 598), Point(-4780, -9837, 602),
	};

	CONST static std::vector<Point> ForceMasterHelpPointVec4_Front = {
		Point(-5479, -10011, 599), Point(-5722, -10183, 599), Point(-6019, -10372, 598), Point(-6315, -10653, 598), 
		Point(-6590, -10986, 596), Point(-6821, -11356, 596), Point(-7056, -11675, 596), Point(-7208, -11981, 608), 
		Point(-7279, -12152, 702), Point(-7530, -12371, 673), Point(-7900, -12696, 569), Point(-8203, -12976, 398), 
		Point(-8456, -13103, 299), Point(-8805, -13175, 158), Point(-9239, -13221, -57), Point(-9591, -13241, -259), 
		Point(-9958, -13277, -450), Point(-10283, -13341, -590), Point(-10604, -13498, -704), Point(-10858, -13718, -798), 
		Point(-10913, -13995, -800), Point(-10855, -14283, -780), Point(-10693, -14620, -747), Point(-10465, -14896, -690), 
		Point(-10115, -15238, -622), Point(-9954, -15401, -586), Point(-9703, -15667, -542), Point(-9465, -15909, -514), Point(-9464, -15944, -510),
	};

	CONST static std::vector<Point> ForceMasterHelpPointVec4_Back = {
		Point(-9646, -15660, -537), Point(-9943, -15356, -587), Point(-10268, -15025, -656), Point(-10512, -14724, -719), 
		Point(-10699, -14408, -756), Point(-10817, -14110, -783), Point(-10875, -13682, -797), Point(-10561, -13575, -713), 
		Point(-10279, -13388, -596), Point(-9897, -13316, -422), Point(-9538, -13269, -238), Point(-9274, -13235, -77), 
		Point(-8945, -13188, 100), Point(-8617, -12971, 273), Point(-8401, -12922, 377), Point(-8308, -12756, 520), 
		Point(-8138, -12610, 634), Point(-7887, -12408, 685), Point(-7645, -12127, 692), Point(-7248, -11938, 606), 
		Point(-6943, -11774, 599), Point(-6709, -11558, 599), Point(-6640, -11339, 628), Point(-6456, -10989, 598), 
		Point(-6274, -10714, 598), Point(-6067, -10449, 598), Point(-5843, -10211, 598), Point(-5502, -10030, 598), Point(-5200, -9876, 598), Point(-4780, -9837, 602),
	};

	CONST static std::vector<Point> ForceMasterHelpPointVec5_Front = {
		Point(-4889, -9632, 598), Point(-4622, -9346, 598), Point(-4328, -9103, 598), Point(-3968, -8900, 589), 
		Point(-3620, -8645, 573), Point(-3341, -8446, 572), Point(-3070, -8237, 563), Point(-2842, -7920, 548), Point(-3002, -7960, 558), Point(-2916, -7932, 556),
	};

	CONST static std::vector<Point> ForceMasterHelpPointVec5_Back = {
		Point(-3196, -8149, 563), Point(-3529, -8340, 565), Point(-3826, -8533, 571), Point(-4080, -8757, 587), 
		Point(-4387, -9005, 598), Point(-4690, -9316, 598), Point(-4905, -9614, 598), Point(-5215, -9884, 598),
	};

	CONST static std::vector<Point> ForceMasterHelpPointVec6_Front = {
		Point(-5157, -9607, 599), Point(-4855, -9358, 599), Point(-4546, -9079, 598), Point(-4214, -8796, 593), Point(-3625, -8365, 565), 
		Point(-3130, -8015, 562), Point(-2656, -7784, 534), Point(-2221, -7654, 504), Point(-1751, -7542, 460), Point(-1187, -7734, 333), 
		Point(-865, -7900, 273), Point(-489, -7977, 261), Point(-29, -8066, 291), Point(496, -8124, 276), Point(492, -8244, 288),
	};

	CONST static std::vector<Point> ForceMasterHelpPointVec6_Back = {
		Point(87, -8089, 288), Point(-425, -7952, 263), Point(-799, -7878, 267), Point(-1141, -7833, 312), Point(-1444, -7855, 414), 
		Point(-1806, -7945, 519), Point(-2568, -8073, 535), Point(-2818, -8172, 554), Point(-3052, -8289, 566), Point(-3559, -8507, 568), 
		Point(-3900, -8653, 578), Point(-4149, -8851, 592), Point(-4355, -9092, 598), Point(-4595, -9405, 598), Point(-4753, -9702, 598), Point(-4811, -9873, 599),
	};

	CONST static std::vector<Point> PikeHelpPointVec1_Front = {
		Point(-5309, -10217, 599), Point(-5354, -10455, 599), Point(-5217, -10690, 599), Point(-5126, -10853, 604), Point(-5029, -11038, 626), 
		Point(-5061, -11344, 658), Point(-5242, -11572, 665), Point(-5507, -11847, 654), Point(-5725, -12107, 644), Point(-5788, -12273, 634), 
		Point(-6025, -12596, 611), Point(-6328, -12864, 605), Point(-6503, -13064, 615), Point(-6596, -13267, 599), Point(-6605, -13478, 616), 
		Point(-6601, -13741, 582), Point(-6580, -13989, 533), Point(-6745, -14232, 447), Point(-6918, -14498, 353), Point(-7024, -14726, 281), 
		Point(-7024, -15078, 72), Point(-6784, -15366, -71), Point(-6605, -15439, -83), Point(-6341, -15568, -120), Point(-6059, -15734, -169), 
		Point(-5896, -15829, -186), Point(-5731, -15945, -197), Point(-5578, -16029, -203),
	};

	CONST static std::vector<Point> PikeHelpPointVec1_Back = {
		Point(-5838, -15792, -187), Point(-6036, -15545, -159), Point(-6226, -15336, -117), Point(-6415, -15045, -77), Point(-6502, -14779, -13), 
		Point(-6572, -14614, 96), Point(-6662, -14525, 186), Point(-6624, -14385, 281), Point(-6548, -14247, 393), Point(-6477, -14049, 489), 
		Point(-6452, -13847, 559), Point(-6504, -13661, 606), Point(-6577, -13317, 598), Point(-6445, -13041, 608), Point(-6342, -12778, 608), 
		Point(-6136, -12517, 616), Point(-5931, -12262, 634), Point(-5737, -11986, 650), Point(-5580, -11691, 650), Point(-5396, -11254, 639), 
		Point(-5221, -10812, 598), Point(-5197, -10563, 598), Point(-5228, -10295, 598), Point(-5280, -10060, 598), Point(-5329, -9789, 598), Point(-5355, -9491, 598),
	};

	CONST static std::vector<Point> PikeHelpPointVec2_Front = {
		Point(-5551, -10108, 598), Point(-5963, -10404, 598), Point(-6252, -10676, 598), Point(-6479, -11031, 597), Point(-6599, -11346, 620), 
		Point(-6755, -11774, 599), Point(-6953, -12115, 622), Point(-7082, -12301, 679), Point(-7477, -12824, 579), Point(-7918, -12953, 483), 
		Point(-8474, -13109, 289), Point(-8873, -13178, 136), Point(-9224, -13204, -45), Point(-9558, -13241, -244), Point(-10025, -13298, -482), 
		Point(-10331, -13362, -605), Point(-10694, -13493, -724), Point(-10967, -13710, -820), Point(-11084, -13897, -838), Point(-10924, -14112, -797), 
		Point(-10747, -14552, -755), Point(-10590, -14802, -717), Point(-10398, -15092, -658), Point(-10185, -15389, -603), Point(-9851, -15628, -554), 
		Point(-9409, -16032, -501), Point(-9086, -16330, -429), Point(-8757, -16622, -360), Point(-8517, -16852, -295), Point(-8062, -17255, -256), 
		Point(-7693, -17381, -234), Point(-7330, -17529, -200), Point(-6998, -17731, -165), Point(-6508, -18019, -164), Point(-6380, -18130, -163),
	};

	CONST static std::vector<Point> PikeHelpPointVec2_Back = {
		Point(-6616, -17965, -164), Point(-6807, -17744, -163), Point(-7022, -17447, -186), Point(-7240, -17130, -210), Point(-7341, -16952, -167), 
		Point(-7384, -16614, -80), Point(-7229, -16434, -74), Point(-6984, -16140, -106), Point(-6771, -15837, -112), Point(-6680, -15551, -87), 
		Point(-6665, -15193, -58), Point(-6582, -14773, 10), Point(-6587, -14608, 104), Point(-6549, -14433, 215), Point(-6661, -14368, 312),
		Point(-6574, -14214, 414), Point(-6532, -14025, 510), Point(-6532, -13816, 569), Point(-6594, -13648, 606), Point(-6601, -13259, 601), 
		Point(-6575, -13000, 646), Point(-6555, -12585, 609), Point(-6300, -12254, 606), Point(-5978, -12007, 637), Point(-5619, -11639, 650), 
		Point(-5380, -11169, 630), Point(-5246, -10733, 598), Point(-5178, -10393, 598), Point(-5187, -10095, 598), Point(-5282, -9829, 598), Point(-5423, -9383, 600),
	};

	CONST static std::vector<Point> PikeHelpPointVec3_Front = {
		Point(-5303, -10415, 598), Point(-5253, -10746, 598), Point(-5216, -10995, 614), Point(-5178, -11166, 638), Point(-5126, -11406, 661), 
		Point(-5089, -11635, 670), Point(-5054, -11846, 688), Point(-5036, -11974, 714), Point(-5024, -12582, 646),
	};

	CONST static std::vector<Point> PikeHelpPointVec3_Back = {
		Point(-4995, -12350, 663), Point(-4985, -12111, 691), Point(-5059, -11862, 688), Point(-5105, -11509, 664), Point(-5137, -11294, 652), Point(-5161, -10991, 615), 
		Point(-5202, -10720, 598), Point(-5242, -10533, 598), Point(-5226, -10333, 598), Point(-5185, -10068, 598), Point(-5258, -9829, 599), Point(-5426, -9477, 599),
	};

	CONST static std::vector<Point> PikeHelpPointVec4_Front = {
		Point(-5208, -10223, 598), Point(-4934, -10577, 599), Point(-4717, -10696, 601), Point(-4497, -10834, 599), Point(-4287, -10991, 598),
		Point(-4041, -11190, 566), Point(-3824, -11404, 570), Point(-3647, -11807, 557),
	};

	CONST static std::vector<Point> PikeHelpPointVec4_Back = {
		Point(-3717, -11635, 560), Point(-3849, -11346, 575), Point(-3955, -11087, 588), Point(-4108, -10824, 598), Point(-4296, -10662, 598), 
		Point(-4474, -10551, 598), Point(-4672, -10392, 598), Point(-4864, -10240, 598), Point(-5132, -9980, 598), Point(-5317, -9797, 598), Point(-5334, -9503, 599),
	};

	CONST static std::vector<Point> PikeHelpPointVec5_Front = {
		Point(-4800, -9983, 598), Point(-4536, -10103, 598), Point(-4291, -10211, 598), Point(-4061, -10282, 598), Point(-3854, -10335, 598),
		Point(-3606, -10350, 598), Point(-3404, -10253, 599), Point(-3127, -10258, 594), Point(-2858, -10289, 562), Point(-2447, -10145, 383), 
		Point(-2424, -10300, 366), Point(-2406, -10624, 244), Point(-2348, -11064, 73), Point(-2272, -11447, 16), Point(-2252, -11708, -7), 
		Point(-2221, -12034, -63), Point(-2187, -12293, -82), Point(-2175, -12555, -107), Point(-2189, -12816, -128),
	};

	CONST static std::vector<Point> PikeHelpPointVec5_Back = {
		Point(-2187, -12552, -106), Point(-2301, -12247, -74), Point(-2384, -12013, -35), Point(-2470, -11727, 4), Point(-2549, -11458, 51), 
		Point(-2623, -11289, 106), Point(-2725, -11198, 217), Point(-2653, -10922, 261), Point(-2467, -10647, 243), Point(-2441, -10419, 322), 
		Point(-2455, -10218, 375), Point(-2561, -10061, 445), Point(-2690, -9920, 500), Point(-2826, -9716, 520), Point(-2988, -9520, 539), 
		Point(-3176, -9348, 565), Point(-3446, -9165, 598), Point(-3765, -9039, 598), Point(-4107, -8885, 592), Point(-4391, -8945, 598), 
		Point(-4633, -9045, 598), Point(-4866, -9234, 598), Point(-5090, -9378, 598), Point(-5282, -9433, 598),
	};

	CONST static std::vector<Point> PikeHelpPointVec6_Front = {
		Point(-5017, -10074, 599), Point(-4855, -10341, 599), Point(-4765, -10666, 600), Point(-4428, -10762, 598), Point(-4415, -10729, 598), 
		Point(-4161, -10634, 598), Point(-3950, -10553, 598), Point(-3747, -10470, 598), Point(-3560, -10382, 598), Point(-3447, -10318, 633), 
		Point(-3337, -10275, 666), Point(-3160, -10248, 596), Point(-2938, -10295, 576), Point(-2748, -10205, 530), Point(-2444, -10191, 378), 
		Point(-2433, -10444, 314), Point(-2362, -10993, 100), Point(-2232, -11298, 17), Point(-2207, -11491, 14), Point(-2159, -11713, -9), 
		Point(-2035, -11977, -62), Point(-1915, -12113, -70), Point(-1828, -12284, -85), Point(-1702, -12567, -125), Point(-1576, -12816, -169), 
		Point(-1465, -13004, -172), Point(-1404, -13225, -175), Point(-1342, -13417, -178), Point(-1274, -13629, -173), Point(-1212, -13846, -157), 
		Point(-1274, -14035, -140), Point(-1398, -14191, -130),
	};

	CONST static std::vector<Point> PikeHelpPointVec6_Back = {
		Point(-1555, -14030, -136), Point(-1808, -14002, -133), Point(-2055, -13984, -133), Point(-2250, -14000, -135), Point(-2576, -13882, -155), 
		Point(-2837, -13751, -166), Point(-3009, -13678, -142), Point(-3124, -13603, -113), Point(-3387, -13458, -46), Point(-3557, -13355, 30), 
		Point(-3721, -13281, 111), Point(-3882, -13267, 211), Point(-3988, -13090, 348), Point(-4071, -12926, 434), Point(-4162, -12686, 510), 
		Point(-4366, -12547, 587), Point(-4499, -12274, 623), Point(-4589, -12004, 653), Point(-4702, -11642, 650), Point(-4776, -11281, 631), 
		Point(-4837, -11019, 618), Point(-4889, -10796, 610), Point(-4994, -10466, 598), Point(-5081, -10242, 598), Point(-5181, -10025, 598), Point(-5364, -9672, 598),
	};


	static std::vector<BlackForestForceMasterHelp> HelpVec;
	if (HelpVec.size() == NULL)
	{
		HelpVec.push_back(BlackForestForceMasterHelp(ForceMasterHelpPointVec1_Front, ForceMasterHelpPointVec1_Back, Point(-4296, -15560, -206), TRUE));
		HelpVec.push_back(BlackForestForceMasterHelp(ForceMasterHelpPointVec2_Front, ForceMasterHelpPointVec2_Back, Point(-4144, -12332, 602), TRUE));
		HelpVec.push_back(BlackForestForceMasterHelp(ForceMasterHelpPointVec3_Front, ForceMasterHelpPointVec3_Back, Point(-2068, -14944, -144), TRUE));
		HelpVec.push_back(BlackForestForceMasterHelp(ForceMasterHelpPointVec4_Front, ForceMasterHelpPointVec4_Back, Point(-9496, -15944, -510), TRUE));
		HelpVec.push_back(BlackForestForceMasterHelp(ForceMasterHelpPointVec5_Front, ForceMasterHelpPointVec5_Back, Point(-2916, -7932, 556), TRUE));
		HelpVec.push_back(BlackForestForceMasterHelp(ForceMasterHelpPointVec6_Front, ForceMasterHelpPointVec6_Back, Point(492, -8244, 288), TRUE));

		HelpVec.push_back(BlackForestForceMasterHelp(PikeHelpPointVec1_Front, PikeHelpPointVec1_Back, Point(-5556, -16020, -200), FALSE));
		HelpVec.push_back(BlackForestForceMasterHelp(PikeHelpPointVec2_Front, PikeHelpPointVec2_Back, Point(-6480, -18244, -161), FALSE));
		HelpVec.push_back(BlackForestForceMasterHelp(PikeHelpPointVec3_Front, PikeHelpPointVec3_Back, Point(-4924, -12600, 651), FALSE));
		HelpVec.push_back(BlackForestForceMasterHelp(PikeHelpPointVec4_Front, PikeHelpPointVec4_Back, Point(-3648, -11980, 553), FALSE));
		HelpVec.push_back(BlackForestForceMasterHelp(PikeHelpPointVec5_Front, PikeHelpPointVec5_Back, Point(-2012, -12724, 553), FALSE));
		HelpVec.push_back(BlackForestForceMasterHelp(PikeHelpPointVec6_Front, PikeHelpPointVec6_Back, Point(-1204, -14096, -134), FALSE));
	}

	return HelpVec;
}

BOOL CFarmSpeialTask::Defence_BlackForest_Help_Move(_In_ CONST BlackForestForceMasterHelp& BlackForestHelp) CONST
{
	std::for_each(BlackForestHelp.Vec1.begin(), BlackForestHelp.Vec1.end(), [](CONST Point& Pt){ CPersonFunction::GetInstance().MoveToPoint(Pt, 3, 50); });

	BnsMove(BlackForestHelp.Pt, 3);
	BnsSleep(500);
	CFarm::GetInstance().ClearAroundMonster(CBaseObject::CreateFixPointInfo(17), []{ return FALSE; });
	BnsMove(BlackForestHelp.Pt, 3);
	CFarm::GetInstance().ClearAroundMonster(CBaseObject::CreateFixPointInfo(17), []{ return FALSE; });
	BnsSleep(500);

	BnsMove(BlackForestHelp.Pt, 3);
	LOG_CF_D( L"开始捡受伤的尸体!");
	CFarm::GetInstance().TakeCorpse(NULL, CBaseObject::CreateFixPointInfo(16), L"");

	std::for_each(BlackForestHelp.Vec2.begin(), BlackForestHelp.Vec2.end(), [](CONST Point& Pt){ CPersonFunction::GetInstance().MoveToPoint(Pt, 3, 50); });
	LOG_CF_D( L"成功回去!!");
	return TRUE;
}

BOOL CFarmSpeialTask::ThrowBomb_Maze() CONST
{
	FixPointInfo FixPtInfo = CBaseObject::CreateFixPointInfo(50);
	
	// 调查队员
	CONST static std::vector<std::wstring> FishNameVec = 
	{
		L"조사대원 일호", L"조사대원 이호", L"조사대원 삼호", L"조사대원 사호", L"조사대원 오호", L"조사대원 육호",
		L"조사대원 칠호", L"조사대원 팔호", L"조사대원 구호", L"조사대원 십호", L"조사대원 십일호", L"조사대원 이호",
	};
	CONST static Point CorpsePoint(6929, 2762, 11423);

	auto fnCaptain = [&FixPtInfo](_In_ CONST CMonster& Monster)
	{
		while (GameRun && !EMPTY_PERSONHP && !Monster.IsDead())
		{
			std::vector<DWORD> FilterBallVec;
			CCorpse Ball;
			if (CResExtend::GetInstance().FindTopBall(FixPtInfo, FilterBallVec, &Ball))
			{
				LOG_CF_D( L"怪物死后有球……");
				Ball.Take();
			}
			else
			{
				LOG_CF_D( L"去尸体旁边捡球!");
				BnsMove(CorpsePoint, 1);
				CResExtend::GetInstance().DoAction_By_ExistCorpse_For_Name(L"의문의 시체", FixPtInfo, [](CONST CCorpse& Corpse){ Corpse.Take(); });
			}
		}
		CFarm::GetInstance().Move_To_Monster(Monster, FixPtInfo);
		BnsSleep(1000);
		Monster.SetAngle();
		CSomeFun::GetInstance().SimlationKey('R', 500);
		CSomeFun::GetInstance().SimlationKey('R', 500);
		BnsSleep(2000);
		if (Monster.GetHp() > 1)
			CFarm::GetInstance().KillMonster_By_Single(Monster, FixPtInfo, TRUE, nullptr);

	};

	auto fnMember = [&FixPtInfo](_In_ CONST CMonster& Monster)
	{
		CFarm::GetInstance().Move_To_Monster(Monster, FixPtInfo);
		while (GameRun && !EMPTY_PERSONHP && !Monster.IsDead())
		{
			if (Monster.GetHp() == 1)
			{
				BnsSleep(1000);
				continue;
			}

			CFarm::GetInstance().KillMonster_By_Single(Monster, FixPtInfo, TRUE, nullptr);
		}
	};

	for (CONST auto& itm : FishNameVec)
	{
		LOG_CF_D( L"正在干:%s", itm.c_str());

		CMonster Monster;
		if (!CMonsterExtend::GetInstance().FindMonster_By_Name(itm, FixPtInfo, &Monster))
		{
			LOG_CF_D( L"找不到怪物:%s 就此作罢!", itm.c_str());
			continue;
		}

		if (CTeamMemberExtend::GetInstance().IsTeamCaptain(CPerson::GetInstance().GetName()))
			fnCaptain(Monster);
		else
			fnMember(Monster);
	}

	return BnsMove(FixPtInfo.FixPoint, 3);
}

BOOL CFarmSpeialTask::TurnTable_Maze() CONST
{
	// 转盘台
	CONST static Point TurnTablePoint(184, -11716, 2280);

	// 非队长的就跑去门口等着开门就行了
	if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() != 0)
		return TRUE;

	BOOL bDoorOpened = FALSE;
	while (GameRun && !EMPTY_PERSONHP && !bDoorOpened)
	{
		BnsMove(TurnTablePoint, 3);

		// 捡枪
		LOG_CF_D( L"捡枪!");
		CResExtend::GetInstance().DoAction_By_ExistCorpse_For_Name(L"독기총 거치대", CBaseObject::CreateFixPointInfo(30), [](CONST CCorpse& Corpse){Corpse.Take(); });

		CMonster Monster;
		while (GameRun && !EMPTY_PERSONHP && !CMonsterExtend::GetInstance().FindMonster_By_Name(L"맹독아귀", CBaseObject::CreateFixPointInfo(30), &Monster))
		{
			// 开始转盘
			BnsMove(TurnTablePoint, 3);
			BnsSleep(2000);

			LOG_CF_D( L"开始转盘!");
			CResExtend::GetInstance().DoAction_By_ExistCorpse_For_Name(L"복불복 돌림판 작동 장치", CBaseObject::CreateFixPointInfo(30), [](CONST CCorpse& Corpse){ Corpse.Take(); });
			BnsSleep(20 * 1000);
		}

		while (GameRun && !EMPTY_PERSONHP && !Monster.IsDead())
		{
			Monster.SetAngle();
			CSomeFun::GetInstance().SimlationKey('R', 500);
			BnsSleep(2000);
			CBaseObject::SetAngle(TurnTablePoint);
			BnsSleep(1000);
		}

		LOG_CF_D( L"放下枪!");
		CPersonFunction::GetInstance().CancelPutDownStatus();

		CResExtend::GetInstance().DoAction_By_ExistCorpse_For_Name(L"미궁의 석문", CBaseObject::CreateFixPointInfo(20), [&bDoorOpened](CONST CCorpse& Corpse){ bDoorOpened = !Corpse.IsCollect(); });
		LOG_CF_D( L"最近门的开启状态=%s", bDoorOpened ? L"YES" : L"NO");
	}
	return TRUE;
}

BOOL CFarmSpeialTask::MagicCaptain_Maze() CONST
{
	// 火炎掌
	CObjectFunction::GetInstance().SetTalkMsg_By_Condition(L"Empty", [](CONST TalkMsg_Src& Tms){ return Tms.wsText.find(L"법기강화대장들이 열파를 사용하기 위해 준비합니다") != -1; });
	// 玄冰掌
	CObjectFunction::GetInstance().SetTalkMsg_By_Condition(L"Empty", [](CONST TalkMsg_Src& Tms){ return Tms.wsText.find(L"법기강화대장들이 한파를 사용하기 위해 준비합니다") != -1; });

	while (GameRun && !EMPTY_PERSONHP && CMonsterExtend::GetInstance().FindMonster_By_Name(L"법기강화대장",CBaseObject::CreateFixPointInfo(30), nullptr))
	{

		CMonsterExtend::GetInstance().FindMonster_By_Condiction(CBaseObject::CreateFixPointInfo(-1), nullptr, [](CONST CMonster& Monster){ return Monster.GetDis() < 30 && Monster.Maze_GetShieldColor() != CMonster::em_Maze_Shield_Color::em_Maze_Shield_Color_None; });
		if (CObjectFunction::GetInstance().FindTalkMsg_By_Condiction([](CONST TalkMsg_Src& Tms){ return Tms.wsText.find(L"법기강화대장들이 열파를 사용하기 위해 준비합니다") != -1; }))
		{
			
		}
		else if (CObjectFunction::GetInstance().FindTalkMsg_By_Condiction([](CONST TalkMsg_Src& Tms){ return Tms.wsText.find(L"법기강화대장들이 한파를 사용하기 위해 준비합니다") != -1; }))
		{

		}

		// 输出

	}
	return TRUE;
}

BOOL CFarmSpeialTask::Maze_Column() CONST
{
	CONST static Point FixPoint(-2769, -5679, 9423);

	CCorpse Door;
	if (!CResExtend::GetInstance().FindCorpse_By_Name(L"미궁의 석문", FixPointInfo(Point(-1916, -6548, 9456), 5), &Door))
	{
		LOG_MSG_CF(L"迷宫之门不知道跑哪里去了!");
		return TRUE;
	}

	while (GameRun && !EMPTY_PERSONHP && Door.IsCollect())
	{
		// 队长操作
		if (CTeamMemberExtend::GetInstance().GetCurrentTeamMemberIndex() == 0)
		{
			CCorpse MazeColumn;
			if (!CResExtend::GetInstance().FindCorpse_By_Condition(CBaseObject::CreateFixPointInfo(16), &MazeColumn, [](CONST CCorpse& Corpse){ return Corpse.GetName() == L"분열의 기둥" && Corpse.IsCollect(); }))
			{
				LOG_MSG_CF(L"附近竟然找不到分裂之柱???");
				break;
			}

			// 开启柱子
			MazeColumn.Take();
		}
		// 不是队长的话， 站在这里看着就行了

		CFarm::GetInstance().ClearAroundMonster(CBaseObject::CreateFixPointInfo(16), []{return FALSE; });
		BnsMove(FixPoint, 3);
		BnsSleep(1000);
	}

	return BnsMove(FixPoint, 3);
}
