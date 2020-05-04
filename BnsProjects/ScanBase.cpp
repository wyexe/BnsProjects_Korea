#include "stdafx.h"
#include "ScanBase.h"
#include <MyTools/CLSearchBase.h>
#include <MyTools/Log.h>
#define Bns_Korea_Clinet				L"Client.exe"
#define _SELF L"ScanBase.cpp"

CScanBase::CScanBase()
{
}

CScanBase::~CScanBase()
{
}

BOOL CScanBase::Search()
{
	DWORD dwBase = 0x0;
	DWORD dwCALL = 0x0;
	DWORD dwAddr = 0x0;
	DWORD dwClient = (DWORD)::GetModuleHandleW(L"Client.exe");
	DWORD dwBsengineDLL = reinterpret_cast<DWORD>(::GetModuleHandleW(L"bsengine_Shipping.dll"));

	//人物基址
	DWORD dwStartTick = ::GetTickCount();
	dwBase = MyTools::CLSearchBase::FindBase("807E1000740E", 0x59 - 0x43, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	人物基址	0x%X", dwBase);
	//人物邀请偏移
	dwBase = MyTools::CLSearchBase::FindBase("0fbec983e90074", 0xF2 - 0xE8, 2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	人物邀请偏移	0x%X", dwBase);
#ifdef BnsProject_TaiWan
	//人物瞬移钩挂地址
	dwAddr = MyTools::CLSearchBase::FindAddr("7d0983c0018985????????6a00", 0x005F9EDC - 0x005F9EED, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	人物瞬移钩挂地址	0x%X", dwAddr);
#endif // BnsProject_TaiWan
#ifdef BnsProject_Korea
	//人物瞬移钩挂地址
	dwAddr = MyTools::CLSearchBase::FindAddr("7d0983c0018985", 0x64A10F - 0x64A1D9, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	人物瞬移钩挂地址	0x%X", dwAddr);
#endif // BnsProject_Korea

	
	//人物瞬移偏移
	dwBase = MyTools::CLSearchBase::FindBase("668B97????????668956??eb??8b??8b??6a018b??ff", 0x005F944D - 0x005F944D, 3, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	人物瞬移偏移	0x%X", dwBase);
	//隐藏玩家CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("75078ad3e8????????83??0183??04", 0xB25 - 0xB29, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	隐藏玩家CALL	0x%X", dwCALL);
	//任务捡起CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("E8????????83F80275????????E8????????EB", 0x0045595E - 0x00455975, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	任务捡起CALL	0x%X", dwCALL);
	//捡NPC任务CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("33C0384424??8985????????8985", 0x0042539C - 0x004253C1, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	捡NPC任务CALL	0x%X", dwCALL);
	//穿装备CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("32DBFFD085C0743E", 0x004C5CAF - 0x004C5CD0, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	穿装备CALL	0x%X", dwCALL);
	//脱装备CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("8038??75??8B??8B50??8D8C24", 0x00478D92 - 0x00478DBC, dwClient,1,0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	脱装备CALL	0x%X", dwCALL);
	//大任务偏移
	dwBase = MyTools::CLSearchBase::FindBase("6683bf??0000000275??E8", 0xA5 - 0xC1, 2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	大任务偏移	0x%X", dwBase);
	//任务基址
	//dwBase = CLSearchBase::FindBase("33db3bcf895c2424", 0x00405B38 - 0x00405B29, 2, 0, Bns_Korea_Clinet);
	//CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, CLog::em_Log_Type_Debug, FALSE, L"#define	任务基址	0x%X", dwBase);
	//当前任务杀怪数量CALL
	dwCALL = MyTools::CLSearchBase::FindAddr("74??8B????EB??33??39??????????75??33C0????????83", 0x8EF8A0 - 0x8EF890, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	当前任务杀怪数量CALL	0x%X", dwCALL);
	//背包返回名字参数偏移1
	dwBase = MyTools::CLSearchBase::FindBase("8ad884db751e8b7c2420", 0x0098B530 - 0x0098B581, 2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	背包返回名字参数偏移1	0x%X", dwBase);
	//背包返回名字参数偏移2
	dwBase = MyTools::CLSearchBase::FindBase("8ad884db751e8b7c2420", 0x0098B530 - 0x0098B57B, 2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	背包返回名字参数偏移2	0x%X", dwBase);
	//判断物品能否使用CALL
	dwCALL = MyTools::CLSearchBase::FindAddr("750532C0C20C008B4424", 0xAE - 0xA0, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	判断物品能否使用CALL	0x%X", dwCALL);
	//获取物品类型CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("c7442420000000008b00", 0x004D3D3A - 0x004D3D4C, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	获取物品类型CALL	0x%X", dwCALL);
	//存入衣柜CALL
	//dwCALL = CLSearchBase::FindCALL("8ac3eb??8d4424", 0x433FF0 - 0x433FD5, dwClient, 1, 0, Bns_Korea_Clinet);
	//CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, CLog::em_Log_Type_Debug, FALSE, L"#define	存入衣柜CALL	0x%X", dwCALL);
	//游戏申请内存CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("7510c746??00000000c746", 0x008440E0 - 0x0084411A, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	游戏申请内存CALL	0x%X", dwCALL);
	//发包CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("7510c746??00000000c746", 0x008440E0 - 0x0084414F, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	发包CALL	0x%X", dwCALL);
	//蹲下捡物CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("77043bcb76??6a02", 0x004A8573 - 0x004A8582, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	蹲下捡物CALL	0x%X", dwCALL);
	//蹲下捡物偏移
	//dwBase = CLSearchBase::FindBase("74??C6??????????01E9", 0x006F83AE - 0x006F83B0, 2, 0, Bns_Korea_Clinet, 0xFFFF);
	//CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, CLog::em_Log_Type_Debug, FALSE, L"#define	蹲下捡物偏移	0x%X", dwBase);
	//遁术发包ID
	dwBase = MyTools::CLSearchBase::FindBase("85C07404C640??015E", 0x00555030 - 0x554FF3, 4, 0, Bns_Korea_Clinet, 0xFFFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	遁术发包ID	0x%X", dwBase);
	//遁术发包内容
	dwBase = MyTools::CLSearchBase::FindBase("85C07404C640??015E", 0x00555030 - 0x00555004, 2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	遁术发包内容	0x%X", dwBase);
	//遁术CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("85C07404C640??015E", 0x00555030 - 0x00555010, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	遁术CALL	0x%X", dwCALL);
	//NPC购买CALL
	//dwCALL = CLSearchBase::FindCALL("8b7424??85f60f84????????6a28", 0x008435B6 - 0x008435D4, dwClient, 1, 0, Bns_Korea_Clinet);
	//CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, CLog::em_Log_Type_Debug, FALSE, L"#define	NPC购买CALL	0x%X", dwCALL);
	//开背包发包ID
	dwBase = MyTools::CLSearchBase::FindBase("33C08B90????????8BB2????????E8????????6A30E8", 0x00952EC5 - 0x00952EE6, 4, 0, Bns_Korea_Clinet, 0xFFFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	开背包发包ID	0x%X", dwBase);
	//开背包发包内容
	dwBase = MyTools::CLSearchBase::FindBase("33C08B90????????8BB2????????E8????????6A30E8", 0x00952EC5 - 0x00952EF2, 2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	开背包发包内容	0x%X", dwBase);
	//取任务IDCALL
	dwCALL = MyTools::CLSearchBase::FindCALL("6A48E8????????33F683C4043B??74??E8", 0x0053A84C - 0x0053A85C, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	取任务IDCALL	0x%X", dwCALL);
	//HookTask_Addr
	dwAddr = MyTools::CLSearchBase::FindAddr("eb0b8b44244c", 0x4DD38D - 0x4DD393, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	HookTask_Addr	0x%X", dwAddr);
	//取支线任务IDCALL
	dwCALL = MyTools::CLSearchBase::FindCALL("6A40E8????????83C40485C074??E8", 0x5398A0 - 0x5398AE, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	取支线任务IDCALL	0x%X", dwCALL);
	//HookBLTask_Addr
	dwAddr = MyTools::CLSearchBase::FindAddr("c6461300????12e8", 0x52CBAF - 0x52CB95, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	HookBLTask_Addr	0x%X", dwAddr);
	//HookMonsterSkillAddr2
	dwAddr = MyTools::CLSearchBase::FindAddr("83e4f88b451483ec0c", 0x005CA8D3 - 0x005CA8D0, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	HookMonsterSkillAddr2	0x%X", dwAddr);
	//HookTakeItemJudge
	dwAddr = MyTools::CLSearchBase::FindAddr("74??83781C0074??807C24??00", 0x006F7F00 - 0x006F7F41, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	HookTakeItemJudge	0x%X", dwAddr);
	//HookTakeItem_ForBlueAddr1
	dwAddr = MyTools::CLSearchBase::FindAddr("0fb680????????ff2485????????8b????3987", 0x7D996C - 0x7D996C, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	HookTakeItem_ForBlueAddr1	0x%X", dwAddr);
	//HookTakeItem_ForBlueAddr2
	dwAddr = MyTools::CLSearchBase::FindAddr("0fb680????????ff2485????????8b????3987", 0x7D996C - 0x007D9983, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	HookTakeItem_ForBlueAddr2	0x%X", dwAddr);
	//技能遍历偏移
	dwBase = MyTools::CLSearchBase::FindBase("8B7424??85F60F84????????837E5C00", 0x004E4281 - 0x004E42A4, 2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	技能遍历偏移	0x%X", dwBase);
	//获取技能参数CALL
	dwCALL = MyTools::CLSearchBase::FindAddr("3c017d08", 0x0051711D - 0x0517110, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	获取技能参数CALL	0x%X", dwCALL);
	//技能名字返回偏移1
	dwBase = MyTools::CLSearchBase::FindBase("894424??895424??75083b??0f84", 0x0045456C - 0x00454584, 2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	技能名字返回偏移1	0x%X", dwBase);
	//技能冷却参数基址
	dwBase = MyTools::CLSearchBase::FindBase_ByCALL("85C00F84????????0FB740026685C075", 0x00447CBB - 0x00447CAA, (DWORD)::GetModuleHandleW(Bns_Korea_Clinet), 1, 0, Bns_Korea_Clinet, 0x2);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	技能冷却参数基址	0x%X", dwBase);
	//技能冷却偏移 
	dwBase = MyTools::CLSearchBase::FindBase("83b9????????00740d8b89????????8b018b50", 0x004E0000 - 0x004E0009, 2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	技能冷却偏移	0x%X", dwBase);
	//技能冷却CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("83f8050f8f????????6a01", 0x004A0F31 - 0x004A0F2C, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	技能冷却CALL	0x%X", dwCALL);
	//释放技能CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("837F040074198B4F04", 0x004516A3 - 0x004516BB, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	释放技能CALL	0x%X", dwCALL);
	//技能加点CALL
#ifdef BnsProject_Korea
	dwCALL = MyTools::CLSearchBase::FindCALL("3958??74??8a??????8b??????E8????????3a", 0x00A3FB1A - 0x00A3FB27, dwClient, 1, 1, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	技能加点CALL	0x%X", dwCALL);
#endif // BnsProject_Korea
#ifdef BnsProject_TaiWan
	dwCALL = MyTools::CLSearchBase::FindCALL("E8????????3A??74??8B??8B????8B", 0x9A1B44 - 0x9A1B44, dwClient, 1, 1, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	技能加点CALL	0x%X", dwCALL);
#endif // BnsProject_TaiWan
#ifdef BnsProject_TaiWan
	//技能加点发包CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("A1????????85??74??8B40??85??74??8B80????????85??74??8B80????????85??74??0FBE8F", 0xA05 - 0xA32, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	技能加点发包CALL	0x%X", dwCALL);
#endif // BnsProject_TaiWan
#ifdef BnsProject_Korea
		//技能加点发包CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("75??0FBE8F????????????E8??????????E8????????3C", 0x00A3D81E - 0x00A3D829, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	技能加点发包CALL	0x%X", dwCALL);
#endif // BnsProject_Korea

	
	//名字返回基址
	dwBase = MyTools::CLSearchBase::FindBase("894424??895424??75083b??0f84", 0x4F - 0x61, 2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	名字返回基址	0x%X", dwBase);
	//名字返回偏移1
	dwBase = MyTools::CLSearchBase::FindBase("750885ff0f84????????8b4308", 0x00606A72 - 0x00606A8E, 2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	名字返回偏移1	0x%X", dwBase);
	//名字返回偏移2
	dwBase = MyTools::CLSearchBase::FindBase("750885ff0f84????????8b4308", 0x62C586 - 0x62C5AA, 2, 0, Bns_Korea_Clinet, 0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	名字返回偏移2	0x%X", dwBase);
	//物品使用CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("807c24??000f85????????8b4c2420", 0x006B542A - 0x006B5440, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	物品使用CALL	0x%X", dwCALL);
	//物品分解CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("B001??83C408C383F802", 0xFa9 - 0xF88, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	物品分解CALL	0x%X", dwCALL);

#ifdef BnsProject_TaiWan
	//丢弃物品CAL
	dwCALL = MyTools::CLSearchBase::FindCALL("74??8B8E????????8B??8B92????????68??????????8B??FFD28B", 0x0093A35D - 0x0093A38E, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	丢弃物品CALL	0x%X", dwCALL);
#endif // BnsProject_TaiWan

#ifdef BnsProject_Korea
	//丢弃物品CAL
	dwCALL = MyTools::CLSearchBase::FindCALL("74??8B8E????????8B96??????????66", 0x0096669F - 0x009666B7, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	丢弃物品CALL	0x%X", dwCALL);
#endif // BnsProject_Korea
	
	//整理背包CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("e8????????3c0175168b4c240c", 0x007B3B02 - 0x007B3B02, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	整理背包CALL	0x%X", dwCALL);
	//武器强化CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("83c410c2040083??02", 0x914 - 0x90B, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	武器强化CALL	0x%X", dwCALL);
	//武器进化CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("83c410c2040083??03", 0x7DADFF - 0x7DAE6D, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	武器进化CALL	0x%X", dwCALL);
	////武器二级进化CALL   与武器进化call一样
	//dwCALL = MyTools::CLSearchBase::FindCALL("566A018D742420", 0x0070ED7C - 0x0070ED89, dwClient, 1, 0, Bns_Korea_Clinet);
	//MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	武器二级进化CALL	0x%X", dwCALL);
	//解印CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("1C8B46785152508B??E8", 0xDE - 0xE7, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	解印CALL	0x%X", dwCALL);
	//开箱子CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("3C0175??8D8E????????51E8????????90C6", 0x00971C7F - 0x00971C7A, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	开箱子CALL	0x%X", dwCALL);
	//选择频道CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("74??FF15????????85C074??E8????????83F801", 0x1A - 0x37, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	选择频道CALL	0x%X", dwCALL);
	//小退CALL
	dwCALL = MyTools::CLSearchBase::FindAddr("750b????7f04????7e??395804", 0xAA9C33 - 0xAA9C00, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	小退CALL	0x%X", dwCALL);
	//领取礼包偏移
	dwBase = MyTools::CLSearchBase::FindBase("8b4f048b068b90????????6a00518bce????5e", 0x008BE24C - 0x008BE236, 2, 1, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	领取礼包偏移	0x%X", dwBase);
	//领取礼包CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("8b4f048b068b90????????6a00518bce????5e", 0x008BE24C - 0x008BE247, dwClient, 1, 1, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	领取礼包CALL	0x%X", dwCALL);
	//获取人物状态偏移
	dwBase = MyTools::CLSearchBase::FindBase("8bcbffd083f80f75", 0x004BC304 - 0x004BC2FE, 2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	获取人物状态偏移	0x%X", dwBase);
#ifdef BnsProject_TaiWan
	// 镶嵌宝石偏移
	dwBase = MyTools::CLSearchBase::FindBase("05??????????E8????????8D7424??8A??C7", 0x007A05C4 - 0x007A05C4, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	镶嵌宝石偏移	0x%X", dwBase);
#endif // BnsProject_TaiWan
#ifdef BnsProject_Korea
	// 镶嵌宝石偏移
	dwBase = MyTools::CLSearchBase::FindBase("c2????3c??75??8B178B52308D4424??508BCFFFD2", 0x007AEB71 - 0x007AEB91, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	镶嵌宝石偏移	0x%X", dwBase);
#endif // BnsProject_Korea

#ifdef BnsProject_TaiWan
	//镶嵌宝石CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("83C40884??75??83??0183??06", 0x75FAB2 - 0x75FAD8, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	镶嵌宝石CALL	0x%X", dwCALL);
#endif // BnsProject_TaiWan
#ifdef BnsProject_Korea
	//镶嵌宝石CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("83????84??75??83????83????83????7C", 0x007E67AD - 0x007E67D4, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	镶嵌宝石CALL	0x%X", dwCALL);
#endif // BnsProject_Korea
	/*
#ifdef BnsProject_TaiWan
	//取宝石CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("83E4F880??????????00????75??E8", 0x0047AB43 - 0x0047AB40, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	取宝石CALL	0x%X", dwCALL);
#endif // BnsProject_TaiWan*/
	// 取宝石CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("8B4D??8B50??8B006A", 0x6DA - 0x6E9, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	取宝石CALL	0x%X", dwCALL);

	//捡JB发包ID
	dwBase = MyTools::CLSearchBase::FindBase("8b178b521c8d4424", 0x0044EC1C - 0x0044EC38, 4, 0, Bns_Korea_Clinet, 0xFFFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	捡JB发包ID	0x%X", dwBase);
	//捡JB法宝内容
	dwBase = MyTools::CLSearchBase::FindBase("8b178b521c8d4424", 0x0044EC1C - 0x0044EC49, 2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	捡JB法宝内容	0x%X", dwBase);
	//存仓call
	dwAddr = MyTools::CLSearchBase::FindCALL("3C0175??88??????????B001EB", 0x417 - 0x412, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	存仓CALL	0x%X", dwAddr);
	//刷新频道CALL
	dwAddr = MyTools::CLSearchBase::FindCALL("B001E9????????8B????3B????75??A1", 0x94A477 - 0x94A449, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	刷新频道CALL	0x%X", dwAddr);

#ifdef BnsProject_TaiWan
	//买物品发包填充CALL
	dwAddr = MyTools::CLSearchBase::FindCALL("50E8????????3C0175??85??75??39", 0xB69 - 0xB4B, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	买物品发包填充CALL	0x%X", dwAddr);
#endif
#ifdef BnsProject_Korea
	//买物品发包填充CALL
	dwAddr = MyTools::CLSearchBase::FindCALL("E8????????3C0175??????75??39", 0x3D9 - 0x3BC, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	买物品发包填充CALL	0x%X", dwAddr);
#endif
	
	//卖物品CALL
	dwAddr = MyTools::CLSearchBase::FindCALL("E8????????EB138B??8B??44", 0xB5F - 0xB74, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	卖物品CALL	0x%X", dwAddr);
	//根据ID返回OBJ基址
	dwBase = MyTools::CLSearchBase::FindBase("E9????????A1????????3B??895C24", 0x00CF491A - 0x00CF4953, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	根据ID返回OBJ基址	0x%X", dwBase);
	//根据ID返回OBJ偏移1
	dwBase = MyTools::CLSearchBase::FindBase("E9????????A1????????3B??895C24", 0x00CF491A - 0x00CF4958, 2, 0, Bns_Korea_Clinet, 0xFFFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	根据ID返回OBJ偏移1	0x%X", dwBase);
	//根据ID返回OBJ偏移2
	dwBase = MyTools::CLSearchBase::FindBase("E9????????A1????????3B??895C24", 0x00CF491A - 0x00CF4967, 2, 0, Bns_Korea_Clinet, 0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	根据ID返回OBJ偏移2	0x%X", dwBase);
	// 删除角色CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("8B4424????E8????????C6??????????FF", 0x00C492C6 - 0x00C492CB, dwClient, 0x1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	删除角色CALL 	0x%X", dwCALL);
	// 创建角色索引CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("C2??008B442408E8", 0x00BB8A63 - 0x00BB8A6A, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	创建角色索引CALL		0x%X", dwCALL);
	// 创建角色CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("84C00F95C0EB??BB", 0x00C402F1 - 0x00C402EC, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	创建角色CALL		0x%X", dwCALL);
	//PK偏移
	dwBase = MyTools::CLSearchBase::FindBase("E8????????3C0175??E8????????EB", 0x00A222C0 - 0x00A22224, 2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	PK偏移	0x%X", dwBase);
	//PK状态偏移
	dwBase = MyTools::CLSearchBase::FindBase("85FF0F84????????E8????????83B8", 0x00A78CE5 - 0x00A78D01, 2, 0, Bns_Korea_Clinet, 0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	PK状态偏移	0x%X", dwBase);
	// PKCALL
	dwCALL = MyTools::CLSearchBase::FindCALL("E8????????3C0175??E8????????EB", 0x00A222C0 - 0x00A222B3, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	PKCALL		0x%X", dwCALL);
	// 离开PK房间CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("C2??0085C074??E8????????85??0F84", 0x00BA5F52 - 0x00BA5F68, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	离开PK房间CALL		0x%X", dwCALL);
	// PK战斗结束CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("8B??74??E8????????8B??E8????????E9", 0xC74CE1 - 0x00C74CEC, dwClient, 1, 1, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	PK战斗结束CALL		0x%X", dwCALL);
	// PK过图偏移
	dwBase = MyTools::CLSearchBase::FindBase("C7442418FFFFFFFFE8????????6A", 0x004FB967 - 0x004FB9BD, 2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	PK过图偏移	0x%X", dwBase);
	// PK准备CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("FF??E9????????3B????75??8B????8B", 0x00AA3EAB - 0x00AA3EC3, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	PK准备CALL		0x%X", dwCALL);
#ifdef BnsProject_TaiWan
	// 尸体名字返回偏移
	dwBase = MyTools::CLSearchBase::FindBase_ByCALL("83C4??83F80175??8B??24??E9", 0x00525755 - 0x0052577D, (DWORD)::GetModuleHandleW(Bns_Korea_Clinet), 0x1, 0x0, Bns_Korea_Clinet, 0x1 + 0x2);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	尸体名字返回偏移	0x%X", dwBase);
#endif // BnsProject_TaiWan
#ifdef BnsProject_Korea
	// 尸体名字返回偏移
	dwBase = MyTools::CLSearchBase::FindBase_ByCALL("E8????????83????0F????????E8????????80????????74??8B??????8D??????E8", 0x2373 - 0x2373, (DWORD)::GetModuleHandleW(Bns_Korea_Clinet), 0x1, 0x0, Bns_Korea_Clinet, 0x1 + 0x2);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	尸体名字返回偏移	0x%X", dwBase);
#endif // BnsProject_Korea
	// 人物BUFF偏移1
	dwBase = MyTools::CLSearchBase::FindBase("FF??C786????????04000000E8", 0x00B71D36 - 0x00B71D62, 2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	人物BUFF偏移1	0x%X", dwBase);
	// 人物BUFF偏移2
#ifdef BnsProject_TaiWan
	dwBase = MyTools::CLSearchBase::FindBase("E8????????8B87????????85C074??????E8????????EB", 0x639 - 0x632, 2, 1, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	人物BUFF偏移2	0x%X", dwBase);
#endif // BnsProject_TaiWan
#ifdef BnsProject_Korea
	dwBase = MyTools::CLSearchBase::FindBase("E8????????8B80????????85C074??8B4B??3B48??75", 0xDDC96F - 0xDDC942, 2, 1, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	人物BUFF偏移2	0x%X", dwBase);
#endif // BnsProject_Korea

	
	// 人物BUFF偏移3
	dwBase = MyTools::CLSearchBase::FindBase("83??FF83??????0f87????????FF2485????????8B??????????85", 0x70190D - 0x701921, 2, 1, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	人物BUFF偏移3	0x%X", dwBase);
	// 人物BUFF偏移_SELF
	dwBase = MyTools::CLSearchBase::FindBase("C1F80C83E00F663D050074", 0x006A4580 - 0x006A450C, 2, 0, Bns_Korea_Clinet, 0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	人物BUFF偏移_SELF	0x%X", dwBase);
	// 人物BUFF偏移_OTHER
	dwBase = MyTools::CLSearchBase::FindBase("C1F80C83E00F663D050074", 0x006A4580 - 0x006A4535, 2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	人物BUFF偏移_OTHER	0x%X", dwBase);
	// 人物BUFF_ARRAY_SIZE
	dwBase = MyTools::CLSearchBase::FindBase("8D45??8B4C24??66", 0x006BF1BD - 0x006BF1CC, 1, 0, Bns_Korea_Clinet, 0xFFFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	人物BUFF_ARRAY_SIZE	0x%X", dwBase);
	// 抱尸体发包参数1
	dwBase = MyTools::CLSearchBase::FindBase("8B??60FF??8B??85??74??6A", 0x00442E6A - 0x00442E83, 4, 0, Bns_Korea_Clinet, 0xFFFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	抱尸体发包参数1	0x%X", dwBase);
	// 抱尸体发包参数2
	dwBase = MyTools::CLSearchBase::FindBase("8B??60FF??8B??85??74??6A", 0x00442E6A - 0x00442E94, 2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	抱尸体发包参数2	0x%X", dwBase);
	// 角色遍历偏移
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	角色遍历偏移	0x19C", dwBase);
	// 聊天消息框偏移1
	dwBase = MyTools::CLSearchBase::FindBase("81??????0000??8B??8B??0485", 0x00B4C303 - 0x00B4C303, 2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	聊天消息框偏移1	0x%X", dwBase);
	// 聊天消息框偏移2
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	聊天消息框偏移2	0x88", dwBase);
	// 视觉基址
	dwBase = MyTools::CLSearchBase::FindBase("33C03D????????74??A1????????83E800", 0x00721A80 - 0x00721A89, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	视角基址	0x%X", dwBase);
	// 获取点卡时间Base
	dwBase = MyTools::CLSearchBase::FindBase("83E803740F83E802", 0x94D8F3 - 0x94D8EE, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	获取点卡时间Base	0x%X", dwBase);
	// 获取点卡时间CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("83E803740F83E802", 0x0094D8F3 - 0x0094D8FD, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	获取点卡时间CALL	0x%X", dwCALL);
	// HookDianKaAddr
	dwAddr = MyTools::CLSearchBase::FindAddr("8944242489542428E8????????85", 0x00B94926 - 0x00B94933, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	HookDianKaAddr	0x%X", dwAddr);
	// 登录CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("6A148B??FFD08B4C24", 0x009591F9 - 0x00959222, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	登录CALL	0x%X", dwCALL);
	// 邮件偏移3
	dwBase = MyTools::CLSearchBase::FindBase("8B4C24??8B??8B92????????8D4424??5055C64424????FF", 0x00AB805E - 0x00AB8086, 2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	邮件偏移3	0x%X", dwBase);
	// 邮件偏移1
	dwBase = MyTools::CLSearchBase::FindBase("8B4C24??8B??8B92????????8D4424??5055C64424????FF", 0x00AB805E - 0x00AB808E, 3, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	邮件偏移1	0x%X", dwBase);
	// 集体收邮件CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("3BAF????????75????8B??E8????????EB????8B", 0x00AB8007 - 0x00AB801C, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	集体收邮件CALL	0x%X", dwCALL);
	// 收邮件偏移
	dwBase = MyTools::CLSearchBase::FindBase("8B??E8????????8D????????????E8????????E9????????3b", 0xAA0C1F - 0xAA0C43, 2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	收邮件偏移	0x%X", dwBase);
	// 设置收邮件CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("8B4C24??8B??8B92????????8D4424??5055C64424????FF", 0x00AB805E - 0x00AB8096, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	设置收邮件CALL	0x%X", dwCALL);
	// 设置竞拍品质CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("75??8B5424403B5038", 0x008EC0A7 - 0x008EC0D9, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	设置竞拍品质CALL	0x%X", dwCALL);
	// 队伍遍历偏移
	dwBase = MyTools::CLSearchBase::FindBase("85??75????0B0000006A02", 0x0099A0A5 - 0x0099A09B, 2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	队伍遍历偏移	0x%X", dwBase);
	// 离开队伍CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("E9????????81??????0000BB08000000", 0x008F991E - 0x008F9919, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	离开队伍CALL	0x%X", dwCALL);
	// 邀请队员CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("E8????????EB??83F80275??8B??68", 0x8FF107 - 0x8FF107, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	邀请队员CALL	0x%X", dwCALL);
#ifdef BnsProject_TaiWan
	// 一次捡物CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("8B86????00008B8E????00008BB6????00006A00505651", 0x47E - 0x496, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	一次捡物CALL	0x%X", dwCALL);
#endif
#ifdef BnsProject_Korea
	// 一次捡物CALL
	//dwCALL = MyTools::CLSearchBase::FindCALL("8B??????????8B??????????8B??????????6A0050555157E8????????????8B????8B??????????8B", 0x007865C6 - 0x007865DE, dwClient, 1, 0, Bns_Korea_Clinet);
	dwCALL = MyTools::CLSearchBase::FindAddr("8BAC24????????8B9C24????????8BC30B8424", 0x0042763D - 0x00427610, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	一次捡物CALL	0x%X", dwCALL);
#endif

	// 切换新任务CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("8B46??83E8??75??8B46??3B47??75??57E8", 0x0086459C - 0x008645AD, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	切换新任务CALL	0x%X", dwCALL);

	// HookMonsterAddr
	dwAddr = MyTools::CLSearchBase::FindAddr("8B75??85??0F84????????8B8B????????85??74", 0x42BE11 - 0x42BE1C, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	HookMonsterAddr	0x%X", dwAddr);
	// 动作CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("E8????????8B????8B??8B4424??6A006A00??????E8", 0xC8A1A0 - 0xC8A1B5, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	动作CALL 0x%X", dwCALL);
	// 怪物BUFF偏移_OTHER
	dwBase = MyTools::CLSearchBase::FindBase("C1F80C83E00F663D050074", 0x006A4580 - 0x006A4573, 2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	怪物BUFF偏移_OTHER	0x%X", dwBase);
	// 系统BUFF偏移_OTHER
	dwBase = MyTools::CLSearchBase::FindBase("C1F80C83E00F663D050074", 0x006AC270 - 0x006AC23D, 2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	系统BUFF偏移_OTHER	0x%X", dwBase);
	// 遗忘技能确认CALL
	//dwCALL = MyTools::CLSearchBase::FindCALL("C204003B????7506??E8????????B001", 0x0092D01A - 0x0092D00D, dwClient, 1, 0, Bns_Korea_Clinet);
	//MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	遗忘技能确认CALL 0x%X", dwCALL);
	// 技能重置对话框CALL
	//dwCALL = MyTools::CLSearchBase::FindCALL("B001E9????????3B????750C", 0x0092E0C2 - 0x0092E0CE, dwClient, 1, 0, Bns_Korea_Clinet);
	//MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	技能重置对话框CALL 0x%X", dwCALL);
	// 人物邀请交易ID偏移
	dwBase = MyTools::CLSearchBase::FindBase("8B88????00000B88????000075??8B??????0000", 0x00522120 - 0x00522120, 2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	人物邀请交易ID偏移	0x%X", dwBase);
	// 发起

	dwCALL = MyTools::CLSearchBase::FindCALL("C20800BA02000000E8", 0x00C64A2A - 0x00C64A02, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	发起交易CALL 0x%X", dwCALL);
	// 放置交易物品CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("B001E9????????83F80F", 0x009A4E2F - 0x009A4E2A, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	放置交易物品CALL 0x%X", dwCALL);
	// 放置交易物品偏移
	dwBase = MyTools::CLSearchBase::FindBase("B001E9????????83F80F", 0x009A4E2F - 0x009A4E21, 2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	放置交易物品偏移	0x%X", dwBase);
	// 放置交易金钱CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("83F8018944241075", 0x009A4FCA - 0x009A4FE3, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	放置交易金钱CALL 0x%X", dwCALL);
	// 锁定交易框CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("80B8????????0074??8BC6E8", 0x009A5146 - 0x009A515C, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	锁定交易框CALL 0x%X", dwCALL);
	// 放置交易物品偏移
	dwBase = MyTools::CLSearchBase::FindBase("80B8????????0074??8BC6E8", 0x009A5146 - 0x009A5146, 2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	是否锁定自己的交易框偏移	0x%X", dwBase);
	// 对方是否锁定交易偏移
	dwBase = MyTools::CLSearchBase::FindBase("B80B000000C380B8", 0x0059B3B2 - 0x0059B3C1, 2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	对方是否锁定交易偏移	0x%X", dwBase);
	// 交易CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("C2????3BBE????????75??8BB6????????85??0F84", 0x4F4 - 0x51B, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	交易CALL 0x%X", dwCALL);
	// 战斗姿势偏移
	dwBase = MyTools::CLSearchBase::FindBase("6A01508B462451", 0x00685C9E - 0x00685C7D, 2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	战斗姿势偏移	0x%X", dwBase);
	// 选择角色偏移
	dwBase = MyTools::CLSearchBase::FindBase("83C00183C12C83F80A7C??E9", 0x00985395 - 0x009853A5, 3, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	选择角色偏移	0x%X", dwBase);
	// 取消交易CALL
	//dwCALL = MyTools::CLSearchBase::FindCALL("740E83B880000000007405E8??????????C3", 0x00C1244F - 0x00C1245A, dwClient, 1, 0, Bns_Korea_Clinet);
	dwCALL = MyTools::CLSearchBase::FindCALL("51A1????????85??74??8B????85??74??83B8??????????74??E8", 0x00E182B0 - 0x00E182CA, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	取消交易CALL 0x%X", dwCALL);
	// 接受交易CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("83F801750C8BC6E8", 0x00C12419 - 0x00C12420, dwClient, 1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	接受交易CALL 0x%X", dwCALL);
#ifdef BnsProject_TaiWan
	// UI技能遍历偏移1
	//dwBase = MyTools::CLSearchBase::FindBase("85C074??837808028D8B", 0xCB3686 - 0xCB3696, 2, 0, Bns_Korea_Clinet);
	dwBase = MyTools::CLSearchBase::FindBase("80??????????0174??8B??????????85C074??83", 0xD782CB - 0xD782FC, 2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	UI技能遍历偏移1	0x%X", dwBase);
#endif
#ifdef BnsProject_Korea
	// UI技能遍历偏移1
	//dwBase = MyTools::CLSearchBase::FindBase("85C074??837808028D8B", 0xCB3686 - 0xCB3696, 2, 0, Bns_Korea_Clinet);
	dwBase = MyTools::CLSearchBase::FindBase("80??????????0174??8B??????????85C074??83", 0xFB0 - 0xFF4, 2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	UI技能遍历偏移1	0x%X", dwBase);
#endif


	// UI技能遍历偏移2
	dwBase = MyTools::CLSearchBase::FindBase("B932000000E8????????3C01", 0x22D - 0x23B, 2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	UI技能遍历偏移2	0x%X", dwBase);
	// 物品右上角分类名字基址
	dwBase = MyTools::CLSearchBase::FindBase("C1E60681C6??????????B9", 0xB8875A - 0xB8875D, 0x2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	物品右上角分类名字基址	0x%X", dwBase);
	// 交易物品数量偏移
	dwBase = MyTools::CLSearchBase::FindBase("8BB8????????85FF746780BF", 0x0056B0AC - 0x0056B0BF, 0x2, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	交易物品数量偏移	0x%X", dwBase);
	// 逃脱CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("E8????????E9????????E8????????8B??85??0F84????????83??1C00", 0x87AC36 - 0x87AC36, dwClient, 1, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	逃脱CALL	0x%X", dwCALL);
	// 背包购买物品发包参数
	dwBase = MyTools::CLSearchBase::FindBase("C1E810C7??0400000000C7??????????6689??088B4424", 0x0094B6B0 - 0x0094B6BA, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 背包购买物品发包参数 0x%X", dwBase);
	// 第一颗宝石偏移
	dwBase = MyTools::CLSearchBase::FindBase("FFD085C00F84????????83??05", 0x00721978 - 0x00721997, 0x2, 0x0, Bns_Korea_Clinet, 0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 第一颗宝石偏移 0x%X", dwBase);
	// 隐藏UI技能偏移
	dwBase = MyTools::CLSearchBase::FindBase("FFD08BF0A1????????85C00F85", 0x00417F2C - 0x00417F3D, 0x2, 0x0, Bns_Korea_Clinet, 0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 隐藏UI技能偏移 0x%X", dwBase);
	// 遁术CALL2
	dwCALL = MyTools::CLSearchBase::FindCALL("33??837F????8987????????8987????????0F85", 0x008F1EE5 - 0x008F1EE0, dwClient, 1, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	遁术CALL2 	0x%X", dwCALL);
	// 遁术偏移
	dwBase = MyTools::CLSearchBase::FindBase("8B89????00008B018B50??", 0x4E6369 - 0x4E6369, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 遁术偏移 0x%X", dwBase);
	// 打开礼箱CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("85C00F84????????8078100075", 0x009E8292 - 0x009E82AE, dwClient, 1, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	打开礼箱CALL 	0x%X", dwCALL);
	// 已打开礼箱物品偏移
	dwBase = MyTools::CLSearchBase::FindBase("32C0EB??8B??????81", 0xA788D4 - 0xA788DC, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 已打开礼箱物品偏移 0x%X", dwBase);
	// 收礼包CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("32C0EB??8B??????81", 0xA788D4 - 0xA788FF, dwClient, 1, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	收礼包CALL 	0x%X", dwCALL);
	// 未打开礼箱物品偏移
	dwBase = MyTools::CLSearchBase::FindBase("C7442414020000008B86????????33", 0x00453D16 - 0x00453D1E, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 未打开礼箱物品偏移 0x%X", dwBase);
	// 二次捡物CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("EB??8B??8B??6A??FFD0Eb??8B??????8B", 0x00434B4E - 0x00434B65, dwClient, 0x1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	二次捡物CALL 	0x%X", dwCALL);
	// 使用气宗劵CALL2
	dwCALL = MyTools::CLSearchBase::FindCALL("8378??007405E8????????8B??8B??048b", 0x0977E09 - 0x977E0F, dwClient, 0x1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	使用气宗劵CALL2 	0x%X", dwCALL);
	// 使用气宗劵CALL1
	dwCALL = MyTools::CLSearchBase::FindCALL("85C07E0432C0", 0x004563B9 - 0x004563D3, dwClient, 0x1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	使用气宗劵CALL1 	0x%X", dwCALL);
	// 获取劵对象CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("85C07E0432C0", 0x004563B9 - 0x04563C3, dwClient, 0x1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	获取劵对象CALL 	0x%X", dwCALL);
	// 队伍邀请队长ID偏移
	dwBase = MyTools::CLSearchBase::FindBase("8B??????89??????????68????????8D??????????C7??????????01", 0x00919C6D - 0x00919C71, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 队伍邀请队长ID偏移 0x%X", dwBase);
#ifdef BnsProject_Korea
	// 踢出队员CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("E8????????C786????????????????E9????????8B??????????80", 0x009366C7 - 0x009366C7, dwClient, 0x1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	踢出队员CALL 	0x%X", dwCALL);
#endif // BnsProject_Korea
#ifdef BnsProject_TaiWan
	// 踢出队员CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("E8????????C786????????????????E9????????8B15", 0x009366C7 - 0x009366C7, dwClient, 0x1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	踢出队员CALL 	0x%X", dwCALL);
#endif // BnsProject_TaiWan
	
	// 召唤拿猫发包参数1
	dwBase = MyTools::CLSearchBase::FindBase("508D????83C8FFE8????????8B4424", 0x00501507 - 0x005014B9, 0x4, 0x0, Bns_Korea_Clinet, 0xFFFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 召唤拿猫发包参数1 0x%X", dwBase);
	// 召唤拿猫发包参数2
	dwBase = MyTools::CLSearchBase::FindBase("508D????83C8FFE8????????8B4424", 0x00501507 - 0x005014C5, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 召唤拿猫发包参数2 0x%X", dwBase);
	// 召唤拿猫CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("508D????83C8FFE8????????8B4424", 0x00501507 - 0x0050150E, dwClient, 0x1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	召唤拿猫CALL 	0x%X", dwCALL);
	// 召唤猫释放技能CALL
	dwCALL = MyTools::CLSearchBase::FindAddr("83??04000F84????????8B??E8", 0x4307AE - 0x430740, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	召唤猫释放技能CALL 	0x%X", dwCALL);
	// 白仙修练场召唤英雄CALL
	dwCALL = MyTools::CLSearchBase::FindAddr("7D??83C00183F80977??E8????????EB0232C0", 0xBA0AA4 - 0xBA0A70, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	白仙修练场召唤英雄CALL 	0x%X", dwCALL);
	// 白仙修练场偏移1
	dwBase = MyTools::CLSearchBase::FindBase("7D??83C00183F80977??E8????????EB0232C0", 0xBA0AA4 - 0xBA0A96, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 白仙修练场偏移1 0x%X", dwBase);
	// 白仙修练场偏移3
	dwBase = MyTools::CLSearchBase::FindBase("7D??83C00183F80977??E8????????EB0232C0", 0xBA0AA4 - 0xBA0A8C, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 白仙修练场偏移3 0x%X", dwBase);
	// 白仙修练场获取奖励CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("E8????????3C0175??8B4C24??8B018B", 0xBA26D9 - 0xBA26D9, dwClient, 0x1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	白仙修练场获取奖励CALL 	0x%X", dwCALL);
	// 白仙修练场退出CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("6A016A00FF??E8??????????C3", 0xBA0FDF - 0xBA0FE5, dwClient, 0x1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	白仙修练场退出CALL 	0x%X", dwCALL);
	// 时空之塔转盘CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("6A006A00FF??33????8D??01", 0xB9694C - 0x00B96979, dwClient, 0x1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	时空之塔转盘CALL 	0x%X", dwCALL);
	// 时空之塔领取奖励CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("C20400A1????????80B8????????0074", 0xB9AB5D - 0xB9AB6E, dwClient, 0x1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	时空之塔领取奖励CALL 	0x%X", dwCALL);
	// 退出时空之塔CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("C2????83??????????0074??66", 0xB9C012 - 0xB9C027, dwClient, 0x1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	退出时空之塔CALL 	0x%X", dwCALL);
	// Hook副本不过图Addr
	dwAddr = MyTools::CLSearchBase::FindAddr("B101D95C24??894424??D94424??D91C24E8????????5F", 0x6553C3 - 0x6553D4, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	Hook副本不过图Addr	0x%X", dwAddr);
	// 设置角度发包参数1
	dwBase = MyTools::CLSearchBase::FindBase("DFE0F6C4??75??6683F902", 0x005FA4B1 - 0x005FA4CC, 0x4, 0x0, Bns_Korea_Clinet, 0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 设置角度发包参数1 0x%X", dwBase);
	// 设置角度发包参数2
	dwBase = MyTools::CLSearchBase::FindBase("DFE0F6C4??75??6683F902", 0x005FA4B1 - 0x5FA4DD, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 设置角度发包参数2 0x%X", dwBase);
	// UI技能资源偏移
	dwBase = MyTools::CLSearchBase::FindBase("DDD88D??????E8????????C7", 0x68D5D7 - 0x68D5D1, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define UI技能资源偏移 0x%X", dwBase);
	// 怪物BUFF偏移1
	dwBase = MyTools::CLSearchBase::FindBase("FFD083B8????????0074??8B??8B????8B", 0x6A0B1F - 0x6A0B4C, 0x2, 0x0, Bns_Korea_Clinet, 0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 怪物BUFF偏移1 0x%X", dwBase);
	// 怪物BUFF偏移2
	dwBase = MyTools::CLSearchBase::FindBase("89??????????FFD08D????E8", 0xC27727 - 0xC27716, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 怪物BUFF偏移2 0x%X", dwBase);
	// 怪物BUFF偏移3
	dwBase = MyTools::CLSearchBase::FindBase("0F84????????8b????????8D??????????E8????????8B", 0xD5F16E - 0xD5F179, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 怪物BUFF偏移3 0x%X", dwBase);
	// 怪物BUFF偏移4
	dwBase = MyTools::CLSearchBase::FindBase("0B????75??8D??????E8??????????E8", 0x6CA83D - 0x6CA842, 0x2, 0x0, Bns_Korea_Clinet, 0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 怪物BUFF偏移4 0x%X", dwBase);
	// 怪物BUFF偏移5
	dwBase = MyTools::CLSearchBase::FindBase("83??????0175??80??????????01", 0x006BF3EA - 0x006BF39E, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 怪物BUFF偏移5 0x%X", dwBase);
	// 怪物BUFF偏移6
	dwBase = MyTools::CLSearchBase::FindBase("83??????0175??80??????????01", 0x006BF3EA - 0x006BF3B0, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 怪物BUFF偏移6 0x%X", abs(static_cast<int>(dwBase)));
	// 背包遍历偏移
	dwBase = MyTools::CLSearchBase::FindBase("3c0175168b4c240c", 0x0075B467 - 0x0075B448, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 背包遍历偏移 0x%X", dwBase);
	// 当前频道偏移
	dwBase = MyTools::CLSearchBase::FindBase("E8????????8B??????????83??087E??8B", 0x008EDD5D - 0x008EDD56, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 当前频道偏移 0x%X", dwBase);
	// 走路偏移
	dwBase = MyTools::CLSearchBase::FindBase("FF??84C00F84????????D9EE68", 0x6557F5 - 0x6557E4, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 走路偏移 0x%X", dwBase);
	// 背包格子数量偏移
	dwBase = MyTools::CLSearchBase::FindBase("8A??????????EB??8A??????????EB??8A??????????3C", 0x0051EC4A - 0x0051EC68, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 背包格子数量偏移 0x%X", dwBase);
	// 人物基址偏移1
	dwBase = MyTools::CLSearchBase::FindBase("8A??????????EB??8A??????????EB??8A??????????3C", 0x0051EC4A - 0x0051EC11, 0x2, 0x0, Bns_Korea_Clinet, 0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 人物基址偏移1 0x%X", dwBase);
	// 人物基址偏移2
	dwBase = MyTools::CLSearchBase::FindBase("8A??????????EB??8A??????????EB??8A??????????3C", 0x0051EC4A - 0x0051EC18, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 人物基址偏移2 0x%X", dwBase);
	// 人物基址偏移3
	dwBase = MyTools::CLSearchBase::FindBase("8378??0074??8B40??8B??048D", 0x005929EE - 0x005929F4, 0x2, 0x0, Bns_Korea_Clinet, 0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 人物基址偏移3 0x%X", dwBase);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define Name偏移 0x8C", dwBase);
	// MAPID偏移
	dwBase = MyTools::CLSearchBase::FindBase("33??E9????????8B??????8B????????????E8????????83C4??84", 0x0065D484 - 0x0065D48F, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define MAPID偏移 0x%X", dwBase);
	// HP偏移
	dwBase = MyTools::CLSearchBase::FindBase("F6??0174??DF??????????8B4424??DEF1", 0x0042A606 - 0x0042A60B, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define HP偏移 0x%X", dwBase);
	// Point偏移
	dwBase = MyTools::CLSearchBase::FindBase("2B??8D??????8B??????89??89", 0x0062B3AD - 0x0062B3C0, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define Point偏移 0x%X", dwBase);
	// 卖NPC偏移
	dwBase = MyTools::CLSearchBase::FindBase("85??0F84????????83????0175??C7??10", 0x0096D8EA - 0x0096D908, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 卖NPC偏移 0x%X", dwBase);
	// 怪物遍历_偏移
	dwBase = MyTools::CLSearchBase::FindBase("EB0233??8B??????????8B??05", 0x682D95 - 0x682DA1, 0x1, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 怪物遍历_偏移 0x%X", dwBase);
	// 怪物属性偏移
	dwBase = MyTools::CLSearchBase::FindBase("83????33??C74424??????????8A????0F????????8D????????????8A????C1????0B??0F", 0x004FD406 - 0x004FD41B, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 怪物属性偏移 0x%X", dwBase);
	// 交任务CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("E8????????8B??????81??????????74??66", 0x82242B - 0x82242B, dwClient, 0x1, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	交任务CALL 	0x%X", dwCALL);
	// 判断是否掉线偏移
	dwBase = MyTools::CLSearchBase::FindBase("8B????85??74??85??74??C6", 0x5E3CC0 - 0x5E3CB1, 0x2, 0x0, Bns_Korea_Clinet, 0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 判断是否掉线偏移 0x%X", dwBase);
#ifdef BnsProject_TaiWan
	// 加属性点发包参数1
	dwBase = MyTools::CLSearchBase::FindBase("EB??8B????0F??????03????3B", 0x6EAACE - 0x6EAAEC, 0x4, 0x0, Bns_Korea_Clinet, 0xFFFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 加属性点发包参数1 0x%X", dwBase);
	// 加属性点发包参数2
	dwBase = MyTools::CLSearchBase::FindBase("EB??8B????0F??????03????3B", 0x6EAACE - 0x6EAAF9, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 加属性点发包参数2 0x%X", dwBase);
#endif // BnsProject_TaiWan
#ifdef BnsProject_Korea
	
	// 加属性点发包参数1
	dwBase = MyTools::CLSearchBase::FindBase("8b????????ffd08d??????e8????????c7????????????????????39", 0x007114E7 - 0x0071151A, 0x4, 0x0, Bns_Korea_Clinet, 0xFFFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 加属性点发包参数1 0x%X", dwBase);
	// 加属性点发包参数2
	dwBase = MyTools::CLSearchBase::FindBase("8b????????ffd08d??????e8????????c7????????????????????39", 0x007114E7 - 0x00711527, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 加属性点发包参数2 0x%X", dwBase);
#endif // BnsProject_Korea
	
	// 切换技能系CALL
	dwCALL = MyTools::CLSearchBase::FindAddr("8B??85??8B??89??24??0F84????????FF15????????85", 0x9A18B0 - 0x9A1870, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 切换技能系CALL 0x%X", dwCALL);
	// BOSS被控制BUFF数量偏移
	dwBase = MyTools::CLSearchBase::FindBase("FFD289??????????89??24??89??????????89", 0x71CD85 - 0x71CD87, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define BOSS被控制BUFF数量偏移 0x%X", dwBase);
	// BOSS是否可控偏移
	dwBase = MyTools::CLSearchBase::FindBase("6A01E8????????8A??????????88", 0x7185D6 - 0x7185E3, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define BOSS是否可控偏移 0x%X", dwBase);
	// 装备突破CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("8B85????????8B8D????????8B9D????????8B95????????8BBD????????53", 0x007E2CC6 - 0x007E2D1D, dwClient, 0x1, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 装备突破CALL 0x%X", dwCALL);
	// 进入副本空间CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("FF15????????85C074??8B46??83E8??75??6A01", 0x633 - 0x64F, dwClient, 0x1, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 进入副本空间CALL 0x%X", dwCALL);
	// 进入副本空间偏移
	dwBase = MyTools::CLSearchBase::FindBase("FF15????????85C074??8B46??83E8??75??6A01", 0x00B30263 - 0x00B30278, 0x2, 0x0, Bns_Korea_Clinet, 0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 进入副本空间偏移 0x%X", dwBase);
	// 召唤拿猫偏移1
	dwBase = MyTools::CLSearchBase::FindBase("0F????????????8B??????0F????????????6A006A00", 0x42966D - 0x42966D, 0x3, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 召唤拿猫偏移1 0x%X", dwBase);
	// 召唤拿猫偏移2
	dwBase = MyTools::CLSearchBase::FindBase("0F????????????8B??????0F????????????6A006A00", 0x42966D - 0x429678, 0x3, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 召唤拿猫偏移2 0x%X", dwBase);
	// 召唤拿礼物CALL
	dwAddr = MyTools::CLSearchBase::FindAddr("0F????????????0F????????????6A016A006A00", 0x429711 - 0x4296E0, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 召唤拿礼物CALL 0x%X", dwAddr);
	// 离开技能训练场CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("C2????8B??8B??????????8B??8B????6A01", 0xC40AC0 - 0xC40AB5, dwClient, 0x1, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 离开技能训练场CALL 0x%X", dwCALL);
	// 技能训练场偏移
	dwBase = MyTools::CLSearchBase::FindBase("C2????3B??74??A1????????3B??74??8B", 0xC409CF - 0xC409E6, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 技能训练场偏移 0x%X", dwBase);
	// 喊话CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("85??74??83????0074????E9????????????E8", 0x72FDC4 - 0x72FDD6, dwClient, 0x1, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 喊话CALL 0x%X", dwCALL);
	// 输入OTPCALL
	dwCALL = MyTools::CLSearchBase::FindCALL("8D??????85??74??83????0074??E8", 0x9FB717 - 0x9FB725, dwClient, 0x1, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 输入OTPCALL 0x%X", dwCALL);
	// 邮寄CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("E8????????3C0175??8D????????8D??????????E8", 0xA65C37 - 0xA65C37, dwClient, 0x1, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 邮寄CALL 0x%X", dwCALL);
	// 邮寄参数偏移
	dwBase = MyTools::CLSearchBase::FindBase("3B??89??????0F84????????8D????????E8????????8D??????????8D", 0xA659A3 - 0xA6599D, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 邮寄参数偏移 0x%X", dwBase);
	// 收无物品邮件CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("E8????????3C0175??8D????????8D??????????E8", 0xA65C37 - 0xA65C37, dwClient, 0x1, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 收无物品邮件CALL 0x%X", dwCALL);
	// 设置瞬移浮空偏移
	dwBase = MyTools::CLSearchBase::FindBase("8D????8D??????????????0100000089", 0x636E43 - 0x636E8E, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 设置瞬移浮空偏移 0x%X", dwBase);
	// 瞬移次数偏移 in 瞬移Buffer填充CALL.rtf
	dwBase = MyTools::CLSearchBase::FindBase("8B????85??74??83????8B??B9", 0x637C70 - 0x637C8A, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 瞬移次数偏移 0x%X", dwBase);
	// 瞬移距离偏移  in 瞬移Buffer填充CALL.rtf
	dwBase = MyTools::CLSearchBase::FindBase("D9????D9??????D9????D9??????E8????????8B??????????83??0C", 0x637CC3 - 0x637CB6, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 瞬移距离偏移 0x%X", dwBase);
	// 瞬移Buffer填充CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("D9????D9??????D9????D9??????E8????????8B??????????83??0C", 0x637CC3 - 0x637CD1, dwClient, 0x1, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 瞬移Buffer填充CALL 0x%X", dwCALL);
	// 全局加速偏移1
	dwBase = MyTools::CLSearchBase::FindBase_ByCALL("83a6??????????8d4c24??c74424??????????e8", 0x964A811 - 0x0964A831, dwBsengineDLL, 0x1, 0x0, L"bsengine_Shipping.dll", 0x2, 0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 全局加速偏移1 0x%X", dwBase);
	// 全局加速偏移2
	dwBase = MyTools::CLSearchBase::FindBase_ByCALL("83a6??????????8d4c24??c74424??????????e8", 0x964A811 - 0x0964A831, dwBsengineDLL, 0x1, 0x0, L"bsengine_Shipping.dll", 0x5, 0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 全局加速偏移2 0x%X", dwBase);
	// 全局加速偏移3
	dwBase = MyTools::CLSearchBase::FindBase("83a6??????????8d4c24??c74424??????????e8", 0x02C3A8B1 - 0x02C3A8E2, 0x4, 0x0, L"bsengine_Shipping.dll");
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 全局加速偏移3 0x%X", dwBase);
	// 怪物隐形偏移
	dwBase = MyTools::CLSearchBase::FindBase("FF??3C010F??????????8B??8B????8B??FF??83", 0x0071FC3C - 0x0071FC4F, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 怪物隐形偏移 0x%X", dwBase);
	// 神行百变偏移
	dwBase = MyTools::CLSearchBase::FindBase("FF??83??FF85??74??81", 0x0A20BA14 - 0x0A20BA1D, 0x2, 0x0, L"bsengine_Shipping.dll");
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 神行百变偏移 0x%X", dwBase);
	// 复活发包参数1
	dwBase = MyTools::CLSearchBase::FindBase("6A006A006A158B??8B??????????6A01", 0x445B1B - 0x445B3E, 0x4, 0x0, Bns_Korea_Clinet, 0xFFFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 复活发包参数1 0x%X", dwBase);
	// 复活发包参数2
	dwBase = MyTools::CLSearchBase::FindBase("6A006A006A158B??8B??????????6A01", 0x445B1B - 0x445B4F, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 复活发包参数2 0x%X", dwBase);
	// 强制开箱子偏移
	dwBase = MyTools::CLSearchBase::FindBase("80??????????00????????89??????0F??????????8A", 0x4A8C4B - 0x4A8C4B, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 强制开箱子偏移 0x%X", dwBase);
	// 领取星币发包参数1
	dwBase = MyTools::CLSearchBase::FindBase("80??????74??80????0074??6A14E8", 0x4310DE - 0x4310F8, 0x4, 0x0, Bns_Korea_Clinet, 0xFFFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 领取星币发包参数1 0x%X", dwBase);
	// 领取星币发包参数2
	dwBase = MyTools::CLSearchBase::FindBase("80??????74??80????0074??6A14E8", 0x4310DE - 0x431109, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 领取星币发包参数2 0x%X", dwBase);
	// 星币偏移1
	dwBase = MyTools::CLSearchBase::FindBase("E8????????85C0????8B??24??8b??24??????8b??E8????????B0", 0xC5990C - 0xC598D4, 0x2, 0x0, Bns_Korea_Clinet, 0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 星币偏移1 0x%X", dwBase);
	// 星币偏移2
	dwBase = MyTools::CLSearchBase::FindBase("E8????????85C0????8B??24??8b??24??????8b??E8????????B0", 0xC5990C - 0xC598DB, 0x2, 0x0, Bns_Korea_Clinet, 0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 星币偏移2 0x%X", dwBase);
	// 正在走路偏移
	dwBase = MyTools::CLSearchBase::FindBase("8B??38??????000074??88??????00008B", 0x652325 - 0x652327, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 正在走路偏移 0x%X", dwBase);
	// 星币商店购物CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("C38B??1039??????????75??????E8??????????????8B", 0xC54F99 - 0x00C54FA7, dwClient, 0x1, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 星币商店购物CALL 0x%X", dwCALL);
	// 捡物状态偏移
	dwBase = MyTools::CLSearchBase::FindBase("8b????FFD03B??0F??????????8b??????????3B??8b", 0x6939DA - 0x6939EF, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 捡物状态偏移 0x%X", dwBase);
	// 星币数量偏移
	dwBase = MyTools::CLSearchBase::FindBase("8B????85??74??8B??????????8B??????????EB??33??33", 0x70EAAF - 0x70EAB6, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 星币数量偏移 0x%X", dwBase);
	// 星币UI偏移
	//dwBase = MyTools::CLSearchBase::FindBase("8D??????????B80100000089", 0xC56016 - 0xC56016, 0x2, 0x0, Bns_Korea_Clinet);
	//MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 星币UI偏移 0x%X", dwBase);
	// 物品颜色偏移
	dwBase = MyTools::CLSearchBase::FindBase("C68424????????04E8????????80BC24????????01", 0xC92ADA - 0xC92B00, 0x3, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 物品颜色偏移 0x%X", dwBase);
	// 组队难度CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("83??0074??????EB????02000000", 0x990EA6 - 0x990ECB, dwClient, 0x1, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 组队难度CALL 0x%X", dwCALL);
	// 八卦强化CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("83F8??75??8D85????????E8", 0x007E2DAB - 0x007E2D9C, dwClient, 0x1, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 八卦强化CALL 0x%X", dwCALL);
	// 八卦属性偏移
	dwBase = MyTools::CLSearchBase::FindBase("A1????????8B??????????0F??????8B??????8B??????FF??8B??????????83", 0x49F692 - 0x49F697, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 八卦属性偏移 0x%X", dwBase);
	// 八卦属性CALL偏移
	dwBase = MyTools::CLSearchBase::FindBase("A1????????8B??????????0F??????8B??????8B??????FF??8B??????????83", 0x49F692 - 0x49F6A5, 0x2, 0x0, Bns_Korea_Clinet, 0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 八卦属性CALL偏移 0x%X", dwBase);
	// 物品八卦ID偏移
	dwBase = MyTools::CLSearchBase::FindBase("33??80????0189??24??0F94C033????89", 0x4A1B5F - 0x4A1B73, 0x3, 0x0, Bns_Korea_Clinet, 0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 物品八卦ID偏移 0x%X", dwBase);
	// 竞拍偏移1
	dwBase = MyTools::CLSearchBase::FindBase("E8????????85C06A0174", 0xA40002 - 0xA3FFF9, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 竞拍偏移1 0x%X", dwBase);
	// 竞拍偏移2
	dwBase = MyTools::CLSearchBase::FindBase_ByCALL("E8????????85C06A0174", 0xA40002 - 0xA40002, (DWORD)::GetModuleHandleW(Bns_Korea_Clinet), 0x1, 0x0, Bns_Korea_Clinet, 0x4 + 0x2, 0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 竞拍偏移2 0x%X", dwBase);
	// 创建副本房间发包参数1
	dwBase = MyTools::CLSearchBase::FindBase("E8??????????C074??663B", 0x005BA776 - 0x005BA7A4, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 创建副本房间发包参数1 0x%X", dwBase);
	// 创建副本房间发包参数2
	dwBase = MyTools::CLSearchBase::FindBase("E8??????????C074??663B", 0x005BA776 - 0x005BA793, 0x4, 0x0, Bns_Korea_Clinet,0xFFFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 创建副本房间发包参数2 0x%X", dwBase);
	// 寻找房间发包参数1
	dwBase = MyTools::CLSearchBase::FindBase("8B??E8????????3C??74??6A28", 0x005BA9D0 - 0x005BAA01, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 寻找房间发包参数1 0x%X", dwBase);
	// 寻找房间发包参数2
	dwBase = MyTools::CLSearchBase::FindBase("8B??E8????????3C??74??6A28", 0x005BA9D0 - 0x005BA9F1, 0x4, 0x0, Bns_Korea_Clinet, 0xFFFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 寻找房间发包参数2 0x%X", dwBase);
	// 副本房间偏移1
	dwBase = MyTools::CLSearchBase::FindBase("3B??0F??????????8B????3B??0F??????????B9", 0x00B47893 - 0x00B4788D, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 副本房间偏移1 0x%X", dwBase);
	// 副本房间偏移2
	dwBase = MyTools::CLSearchBase::FindBase("3B??0F??????????8B????3B??0F??????????B93B", 0x00B47893 - 0x00B4789B, 0x2, 0x0, Bns_Korea_Clinet,0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 副本房间偏移2 0x%X", dwBase);
	// 房间准备CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("8B??E8????????3C0175??8B??E8????????EB??6A", 0x00B45F85 - 0x00B45F9D, dwClient, 0x1, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 房间准备CALL 0x%X", dwCALL);
	// 房间是否准备CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("8B??E8????????3C0175??8B??E8????????EB", 0x00B45F85 - 0x00B45F87, dwClient, 0x1, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 房间是否准备CALL 0x%X", dwCALL);
	// 队长是否准备CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("C20400E8????????3A", 0x00B46096 - 0x00B46099, dwClient, 0x1, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 队长是否准备CALL 0x%X", dwCALL);
	// 队长房间准备偏移
	dwBase = MyTools::CLSearchBase::FindBase("C20400E8????????3A", 0x00B46096 - 0x00B460A2, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 队长房间准备偏移 0x%X", dwBase);
	// 选择副本CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("85??74??84??74??E8", 0x00B51856 - 0x00B5184A, dwClient, 0x1, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 选择副本CALL 0x%X", dwCALL);
	// 选择副本难度CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("8B??E8????????80????000F94C0", 0x005CE1EA - 0x005CE1EC, dwClient, 0x1, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 选择副本难度CALL 0x%X", dwCALL);


	// 隐藏UI偏移
	dwBase = MyTools::CLSearchBase::FindBase("8378????74??8B41??83E8??75??8B41??39", 0x00DEAC65 - 0x00DEAC8D, 0x2, 0x0, Bns_Korea_Clinet, 0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 隐藏UI偏移 0x%X", dwBase);
	/*//退出50级UI偏移
	dwBase = MyTools::CLSearchBase::FindBase("3B87????????0F85????????8B??8B42??8B??FFD0E8????????8B40", 0x00DEB84E - 0x00DEB85C, 0x2, 0x0, Bns_Korea_Clinet, 0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 退出50级UI偏移 0x%X", dwBase);
	//使用50级卷偏移1
	dwBase = MyTools::CLSearchBase::FindBase("74??8B40??85C074??8B78??85FF74??6A", 0x00DEAF4F - 0x00DEAF51, 0x2, 0x0, Bns_Korea_Clinet, 0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 使用50级卷偏移1 0x%X", dwBase);
	//使用50级卷偏移2
	dwBase = MyTools::CLSearchBase::FindBase("74??8B40??85C074??8B78??85FF74??6A", 0x00DEAF4F - 0x00DEAF58, 0x2, 0x0, Bns_Korea_Clinet, 0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 使用50级卷偏移2 0x%X", dwBase);
	// 使用50级卷CALL1
	dwCALL = MyTools::CLSearchBase::FindCALL("74??8B40??85C074??8B78??85FF74??6A", 0x00DEAF4F - 0x00DEAF6D, dwClient, 0x1, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 使用50级卷CALL1 0x%X", dwCALL);
	// 使用50级卷CALL2
	dwCALL = MyTools::CLSearchBase::FindCALL("74??8B40??85C074??8B78??85FF74??6A", 0x00DEAF4F - 0x00DEAF7F, dwClient, 0x1, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 使用50级卷CALL2 0x%X", dwCALL);
	*/
	// 离开洪门修理场CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("7E??E8????????E9????????3B??7E??3B??0F", 0x00DEAD41 - 0x00DEAD62, dwClient, 0x1, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 离开洪门修理场CALL 0x%X", dwCALL);

	// 退出门派CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("8B91????????8B89????????525150E8????????B0??5E????5DC2????E8", 0x00B51798 - 0x00B517B5, dwClient, 0x1, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 退出门派CALL 0x%X", dwCALL);

	//仓库号交易物品数量偏移
	dwBase = MyTools::CLSearchBase::FindBase("85C074??0FBE40??83E80183F8??77??C1E0", 0x005B9C80 - 0x005B9CB4, 0x2, 0x0, Bns_Korea_Clinet, 0xFFFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 仓库号交易物品数量偏移 0x%X", dwBase);

	// 退回跨服组队界面CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("85??????8B????EB??33C06A????E8", 0xDB7E03 - 0xDB7E11, dwClient, 0x1, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 退回跨服组队界面CALL 0x%X", dwCALL);
	// 副本房间偏移3
	dwBase = MyTools::CLSearchBase::FindBase("85??????8B????EB??33C06A????E8", 0xDB7E03 - 0xDB7DF1, 0x2, 0x0, Bns_Korea_Clinet, 0xFFFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 副本房间偏移3 0x%X", dwBase);
	// 退出跨服组队房间CALL
	dwAddr = MyTools::CLSearchBase::FindAddr("8B??83????0075??83????01????8B", 0x5CE931 - 0x5CE930, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	退出跨服组队房间CALL	0x%X", dwAddr);
	// 退出跨服组队界面CALL
	dwAddr = MyTools::CLSearchBase::FindAddr("C380??????????01????6A10", 0x5CE8EC - 0x5CE8C0, 0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	退出跨服组队界面CALL	0x%X", dwAddr);
	// 创建角色偏移 
	dwBase = MyTools::CLSearchBase::FindBase_ByCALL("84C00F95C0EB??BB", 0xD5572D - 0xD55722, dwClient, 1, 0, Bns_Korea_Clinet, 0x9 + 0x2, 0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	创建角色偏移		0x%X", dwBase);
	// 遍历礼箱偏移
	dwBase = MyTools::CLSearchBase::FindBase("0F??????74??8B??????????8B??????????8B", 0xAB8E01 - 0xAB8DDE, 0x2, 0x0, Bns_Korea_Clinet, 0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 遍历礼箱偏移 0x%X", dwBase);
	// UI技能名字偏移
	dwBase = MyTools::CLSearchBase::FindBase("8B????85??8B????89??????0F??????????83", 0x6D7422 - 0x6D7434, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define UI技能名字偏移 0x%X", dwBase);
	// 技能名字偏移1
	dwBase = MyTools::CLSearchBase::FindBase("C6????????????018B??????????8B????8B????????8D", 0x06D749C - 0x6D74AA, 0x2, 0x0, Bns_Korea_Clinet, 0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 技能名字偏移1 0x%X", dwBase);
	// 技能名字偏移2
	dwBase = MyTools::CLSearchBase::FindBase("C6????????????018B??????????8B????8B????????8D", 0x06D749C - 0x6D74AD, 0x2, 0x0, Bns_Korea_Clinet, 0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 技能名字偏移2 0x%X", dwBase);
#ifdef BnsProject_TaiWan
	// 人物属性点偏移
	dwBase = MyTools::CLSearchBase::FindBase("89????89????7E??89??????88", 0x6FC446 - 0x6FC449, 0x2, 0x0, Bns_Korea_Clinet, 0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 人物属性点偏移 0x%X", dwBase);
#endif // BnsProject_TaiWan
#ifdef BnsProject_Korea
	// 人物属性点偏移
	dwBase = MyTools::CLSearchBase::FindBase("7E??8B??89????89????E8????????B0??E9", 0x00711AEF - 0x00711B21, 0x2, 0x0, Bns_Korea_Clinet, 0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 人物属性点偏移 0x%X", dwBase);
#endif // BnsProject_Korea
	
	// Money偏移
	dwBase = MyTools::CLSearchBase::FindBase("3b??7F??7C??8B??????????3B??73??8B????8B", 0x7C3938 - 0x7C393E, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define Money偏移 0x%X", dwBase);
	// 阵营偏移
	dwBase = MyTools::CLSearchBase::FindBase("FF??8b??85??0f??????????8B????8B????85??72", 0x53F26D - 0x53F289, 0x3, 0x0, Bns_Korea_Clinet, 0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 阵营偏移 0x%X", dwBase);
	// MP偏移
	dwBase = MyTools::CLSearchBase::FindBase("8B????????????8b????????????8D??????????????81??????0000", 0x42553C - 0x425529, 0x3, 0x0, Bns_Korea_Clinet, 0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define MP偏移 0x%X", dwBase);
	// 怪物目标ID
	dwBase = MyTools::CLSearchBase::FindBase("74??8B????8B??????????3b??8B????75??8b", 0x6B5E63 - 0x6B5E68, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 怪物目标ID 0x%X", dwBase);
	// 人物读条偏移
	dwBase = MyTools::CLSearchBase::FindBase("FF??FF15????????8B??8B??8B??????????C7??????01000000FF??85??75", 0x69DA8C - 0x69DA74, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 人物读条偏移 0x%X", dwBase);
	// 状态偏移
	dwBase = MyTools::CLSearchBase::FindBase("6689??????????8A??????????88??????????8B??E8", 0x669CCB - 0x669CC5, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 状态偏移 0x%X", dwBase);
	// 倒地偏移
	dwBase = MyTools::CLSearchBase::FindBase("83??????????000F85????????FF15????????8B??8B", 0x66D554 - 0x66D554, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 倒地偏移 0x%X", dwBase);
	// MAXHP偏移1
	dwBase = MyTools::CLSearchBase::FindBase("E9????????8b????????????8b??03??????????8b", 0x832E85 - 0x832E93, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define MAXHP偏移1 0x%X", dwBase);
	// MAXHP偏移2
	dwBase = MyTools::CLSearchBase::FindBase("E9????????8b????????????8b??03??????????8b", 0x832E85 - 0x832E8A, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define MAXHP偏移2 0x%X", dwBase);
	// 申请入队CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("75??399D????????72??8B85????????EB??8D85", 0xDBE - 0xDE3, dwClient, 0x1, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 申请入队CALL 0x%X", dwCALL);
	// 同意申请入队CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("EB??807D????75??837E????75", 0x8C8 - 0x8C3, dwClient, 0x1, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 同意申请入队CALL 0x%X", dwCALL);
	// 跨服掉线进入CALL
	dwCALL = MyTools::CLSearchBase::FindCALL("74??????C7??????????????????E8????????8B??????????85", 0x00AA8730 - 0x00AA873E, dwClient, 0x1, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 跨服掉线进入CALL 0x%X", dwCALL);
	/*
	//解印参数4基址
	dwBase = MyTools::CLSearchBase::FindBase("FFD08B15????????85F674??66837E????75??85D2C74424??????????74??8B0A85C974??83", 0x00CC8EC1 - 0x00CC8E8A, 0x1, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 解印参数4基址 0x%X", dwBase);
	//解印参数4偏移1
	dwBase = MyTools::CLSearchBase::FindBase("FFD08B15????????85F674??66837E????75??85D2C74424??????????74??8B0A85C974??83", 0x00CC8EC1 - 0x00CC8E8F, 0x2, 0x0, Bns_Korea_Clinet);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 解印参数4偏移1 0x%X", dwBase);
	//解印参数4偏移2
	dwBase = MyTools::CLSearchBase::FindBase("8B????8B41??????E8????????8BE5??C2", 0x00E16A80 - 0x00E16A83, 0x2, 0x0, Bns_Korea_Clinet,0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define 解印参数4偏移2 0x%X", dwBase);
	*/
	DWORD dwEndTick = ::GetTickCount();
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"花费%dms", dwEndTick - dwStartTick);

	return TRUE;
}
