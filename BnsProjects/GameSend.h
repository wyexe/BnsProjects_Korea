#ifndef __BNS_GAMESEND_H__
#define __BNS_GAMESEND_H__

#include "GameVariable.h"


class CGameSend
{
public:
	explicit CGameSend();
	~CGameSend();

	// 使用技能
	static BOOL UseSkill(_In_ DWORD dwSkillId);

	// 使用物品
	static BOOL UseItem(_In_ DWORD dwItemParm);

	// 穿装备
	static BOOL WearEqui(_In_ DWORD dwEquiType, _In_ DWORD dwParm);

	// 脱装备
	static BOOL RelieveEqui(_In_ DWORD dwEquiType);

	// 隐藏玩家
	static BOOL HidePlayer();

	// 退出门派
	static BOOL ExitSchool();

	// 任务捡起
	static BOOL TakeItem_By_Task(_In_ DWORD dwId, _In_ DWORD dwIdParm);

	// 捡起Npc
	static BOOL TakeItem_By_Npc(_In_ DWORD dwId, _In_ DWORD dwIdParm);

	// 丢弃物品
	static BOOL DiscardItem(_In_ DWORD dwItemSId, _In_ DWORD dwCount);

	// 分解物品
	static BOOL ExplainItem(_In_ DWORD dwItemSId);

	// 选择频道
	static BOOL SwitchChannel(_In_ DWORD dwChannel);
	
	// 小退
	static BOOL BackToSwitchPlayer();

	// 镶嵌宝石
	static BOOL SetGem(_In_ DWORD dwItemSId, _In_ DWORD dwIndex, _In_ DWORD dwInventory2PanelObj);

	// 取下宝石
	static BOOL RelieveGem(_In_ DWORD dwIndex);

	// 模拟按键
	static BOOL SimulationKey(_In_ DWORD dwAscii, _In_ BOOL bPush);

	// 捡物
	static BOOL TakeItem_By_DropItem(_In_ DWORD dwId, _In_ DWORD dwIdParm);

	// 整理背包
	static BOOL CleanBag();

	// 卖
	static BOOL SellItem_By_Npc(_In_ DWORD dwItemSId, _In_ DWORD dwCount, _In_ DWORD dwNpcId, _In_ DWORD dwNpcIdParm);

	// 买
	static BOOL BuyItem_By_Npc(_In_ DWORD dwNpcId, _In_ DWORD dwNpcIdParm, _In_ DWORD dwCount, _In_ DWORD dwIndex);

	// 买
	static BOOL BuyItem_By_Bag(_In_ DWORD dwPageIndex, _In_ DWORD dwIndex, _In_ DWORD dwCount, _In_ DWORD dwItemIdParm);

	// 开背包
	static BOOL CreateMoreBag();

	// 仓库开背包(仓库 = 0x3, 衣柜 = 0x6)
	static BOOL ExpandInvenSlot(_In_ DWORD dwNpcId, _In_ DWORD dwNpcIdParm, _In_ DWORD dwType);

	// 遁术
	static BOOL TransportCity(_In_ DWORD dwTarCityId);

	// 装备强化
	static BOOL ItemGrowth(_In_ DWORD dwGrowthItemSId, _In_ DWORD dwSacrificeSId, _In_ DWORD dwLuckyCharmSId);

	// 装备突破
	static BOOL ItemEvolution(_In_ DWORD dwGrowthItemSId, _In_ DWORD dwType, _In_ DWORD dwSacrificeGId, _In_ DWORD dwEvolutionId);

	// 装备进化
	static BOOL ItemEndEvolution(_In_ DWORD dwEqumentSId, _In_ DWORD dwSacrificeSId, _In_ DWORD dwEvoltionId);

	// 解印
	static BOOL UnsealItem(_In_ DWORD dwUnsealItemSId, _In_ DWORD dwUntieId, _In_ DWORD dwUntieCount, _In_ DWORD nType);

	// 开箱子
	static BOOL OpenBox(_In_ DWORD dwBoxSID, _In_ BOOL bSpecial);

	// 钥匙开箱子
	static BOOL OpenBox_By_Key(_In_ BOOL bIsLightKey, _In_ DWORD dwKeyId, _In_ DWORD dwKeyCount, _In_ DWORD dwBoxSId);

	// 走路
	static BOOL Move(_In_ float fX, _In_ float fY, _In_ float fZ);

	// 邮寄
	static BOOL Mail(_In_ DWORD dwNpcId, _In_ DWORD dwNpcIdParm, _In_ MailParm_Src* pMailParm);

	// 存仓
	static BOOL SaveItem(_In_ DWORD dwNpcId, _In_ DWORD dwNpcIdParm, _In_ DWORD dwItemSId, _In_ DWORD dwCount, _In_ DWORD dwWareHourseSId);

	// 交主线任务
	static BOOL SumitMainTask(_In_ CONST SumitTask_Src* pTaskSrc);
	static BOOL SumitSpecialMainTask(_In_ CONST SumitTask_Src* pTaskSrc);

	// 交支线任务
	static BOOL PickBLTask(_In_ CONST SumitBLTask_Src* pTaskSrc);

	// 刷新频道
	static BOOL RefushChannel();

	// 删除角色
	static BOOL DeletePlayer(_In_ DWORD dwIndexParm);

	// 创建角色CALL
	static BOOL CreatePlayer(_In_ LPCWSTR pwszPlayerName, _In_ DWORD dwAreaId, _In_ DWORD dwIndex, _In_ DWORD dwClasses, _In_ BOOL bIsQuickLevel = FALSE);

	// 开始PK
	static BOOL StartPK();

	// 离开PK场
	static BOOL ExitPK();

	// PK结束CALL
	static BOOL EndPK();

	// PK准备CALL
	static BOOL ReadyPk();

	// 抱尸体
	static BOOL PickCorpse(_In_ DWORD dwId, _In_ DWORD dwIdParm);

	// 获取点卡时间
	static BOOL RefushDianKa(_In_ DWORD dwPanelObj);

	// 修改视觉
	static BOOL SwitchAngel(_In_ DWORD dwPanelObj);

	// 登录
	static BOOL Login(_In_ DWORD dwAccountAddr, _In_ DWORD dwPassAddr);

	// 收邮件
	static BOOL RecvMail(_In_ DWORD dwNpcId, _In_ DWORD dwNpcIdParm, _In_ DWORD dwUiObj, _In_ DWORD dwMailParm);

	// 收无物品邮件
	static BOOL RecvEmptyMail(_In_ DWORD dwUiObj);

	// 收邮件
	static BOOL RecvAllMail(_In_ DWORD dwUiObj);

	// 设置竞拍品质
	static BOOL SetAuctionQuality(_In_ em_Auction_Quality emAuctionQuality);

	// 邀请队友CALL
	static BOOL InviteTeamMember(_In_ DWORD dwId, _In_ DWORD dwIdParm);

	// 离开队伍
	static BOOL LeaveTeam();

	// 捡球
	static BOOL TakeBall(_In_ DWORD dwId, _In_ DWORD dwIdParm);

	// 一次捡物
	static BOOL Pickup(_In_ DWORD dwId, _In_ DWORD dwIdParm);

	// 瞬移
	static BOOL Telport(_In_ DWORD* pArray, _In_ DWORD dwCALL, _In_ DWORD dwPersonBase);
	static BOOL Telport(_In_ DWORD dwCALL, _In_ DWORD dwPersonBase);

	// 切换新任务
	static BOOL SwitchNewMainTask(_In_ DWORD dwUiObj);

	// 技能加点
	static BOOL AddSkillPoint(_In_ DWORD dwIndex, _In_ DWORD dwLearnId);

	// 技能加点发包
	static BOOL AddSkillPoint_Send(_In_ DWORD dwSkillBook3Panel);

	// 切换技能系
	static BOOL SwitchSkillPage(_In_ DWORD dwSkillBook3Panel);

	// 动作
	static BOOL ActionCALL(_In_ DWORD dwActionId);

	// 密语
	static BOOL TalkTo(_In_ LPCWSTR pwszPlayerName, _In_ LPCWSTR pwszContent);

	// 发起交易
	static BOOL InviteTansaction(_In_ DWORD dwPlayerId, _In_ DWORD dwPlayerIdParm);

	// 放置交易物品
	static BOOL PushTransactionItem(_In_ DWORD dwTradeShelfPanelObj, _In_ DWORD dwSId, _In_ DWORD dwCount);

	// 锁定交易框
	static BOOL LockTransaction();

	// 交易
	static BOOL SumitTransaction(_In_ DWORD Addres);

	// 放置交易金钱
	static BOOL PushTransactionMoney(_In_ DWORD dwMoney);

	// 取消交易
	static BOOL CancelDeal();

	// 接受交易
	static BOOL AcceptDeal();

	// 逃脱
	static BOOL Escape();

	// 根据ID返回OBJ
	static DWORD GetItemObj_By_ID(_In_ DWORD dwItemId);

	// 获取城市ID
	static DWORD GetCurrentCityId();

	// 获取物品类型ID
	static DWORD GetItemType(_In_ DWORD dwItemId);

	// 获取物品类型名称Addr
	static DWORD GetItemTypeNameAddr(_In_ DWORD dwParm1, _In_ DWORD dwParm2);

	// 遗忘技能
	//static VOID SetForgotSkill(_In_ DWORD dwUiObj);
	//static VOID ConfirmForgotSkill(_In_ DWORD dwUiObj);

	// 接受队伍邀请
	static BOOL AcceptTeamInvite(_In_ DWORD dwObj);

	// 关闭游戏UI
	static BOOL CloseGameWindow(_In_ DWORD dwObj);

	// 
	static BOOL CloseGameUi_Task_50Level(_In_ DWORD JumpingLevelPanel);

	// 白仙训练场关闭UI
	static BOOL SetShowUi(_In_ DWORD dwObj, _In_ BOOL bShow);

	// 游戏UI点YES
	static BOOL ClickYes(_In_ DWORD dwObj);

	// 技能
	static DWORD GetSkillParm(_In_ DWORD dwSkillId);
	static DWORD GetSkillObj(_In_ DWORD dwSkillId, _In_ DWORD dwSkillParm);

	// 查询当前杀怪数量
	static DWORD QueryTaskScheduleCount(_In_ DWORD dwTaskId);

	// 开启礼箱
	static BOOL OpenGiftBox(_In_ DWORD dwGiftId);

	// 收礼箱
	static BOOL RecvGift(_In_ DWORD dwItemId, _In_ DWORD dwGiftParm, _In_ DWORD dwGiftID, _In_ DWORD dwBoxId);

	// 二次捡物CALL
	static BOOL SecondPickup(_In_ DWORD dwId, _In_ DWORD dwParm);

	// 使用气宗卷
	static DWORD GetLevelTickObj();
	static BOOL UseKfmVolume1();
	static BOOL UseKfmVolume2();

	// 提出队员
	static BOOL RemoveTeamMember(_In_ DWORD dwIndex);

	// 召唤拿猫
	static BOOL InitializeCatForSummoner(_In_ DWORD dwNpcId, _In_ DWORD dwNpcIdParm, _In_ LPCWSTR wsCatName, _In_ DWORD dwCatParamter);
	static BOOL InitializeCatForSummoner(_In_ DWORD JumpingLevelPanelBase, _In_ DWORD dwIndex, _In_ LPCWSTR wsCatName);

	// 猫使用技能
	static BOOL UseSkillByCat(_In_ DWORD dwSkillId);

	// 白仙训练场领取奖励(职业,第几阶段的奖励)
	static BOOL AcceptPrizesForBaiXianPracticeField(_In_ DWORD dwOpponentClasses, _In_ DWORD dwPrizesIndex);

	// 召唤白仙训练场的英雄
	static BOOL BeckonHeroForBaiXianPracticeField(_In_ DWORD dwDuelBotTrainingSelectJobPanel, _In_ DWORD dwHeroIndex, _In_ DWORD dwStageIndex);

	// 退出白仙训练场
	static BOOL ExitBaiXianPracticeField();

	// 时空之塔转盘(索引从1开始)
	static BOOL TurntableBySpaceTimeTurret(_In_ DWORD dwIndex);

	// 退出时空之塔
	static BOOL ExitSpaceTimeTurret();

	// 时空之塔领取奖励
	static BOOL AcceptPrizesForSpaceTimeTurret();

	// 设置角度
	static BOOL SetAngle(_In_ DWORD dwAngle);

	// 加属性点
	static BOOL AddPlayerAttribute(_In_ DWORD dwPowerAttribute, _In_ DWORD dwHpAttributeCount);

	// 进入PK|副本 房间
	enum em_Room_Type : DWORD
	{
		em_Room_Type_PK = 0x1,
		em_Room_Type_Dungeon = 0x2
	};
	static BOOL IntoRoom(_In_ DWORD TotalGateConfirmPanelBase, _In_ em_Room_Type emRoom);

	// 召唤拿礼物(快速50级)
	static BOOL SummonerTakeGift(_In_ DWORD JumpingLevelPanelBase);

	//  离开技能训练场
	static BOOL LeaveSkillTrainRoom();

	static BOOL LeaveSkillTrainRoom_Confirm();

	static BOOL UseTicket_By_Level50();

	static BOOL Talk(_In_ LPCWSTR pwszText, _In_ DWORD ChatInputPanelBase);

	static BOOL InputOTP(_In_ LPCWSTR pwszOtpText);

	static LPVOID AllocGameMem(_In_ DWORD dwSize);

	// 复活
	static BOOL Resurrect();

	//台服领级别奖励
	static VOID ReceiveLvPrize();
	// 领取星币
	struct StarCoinContent
	{
		WORD wParm1;
		WORD wParm2;
	};
	static BOOL TakeStarCoin(_In_ CONST StarCoinContent& StarCoinContent_);

	static BOOL ShopStarCoinStore(_In_ DWORD dwItemId, _In_ DWORD dwGoodsStorePanel);

	// 修改组队难度
	enum emTeamDifficulty{ emTeamDifficulty_Easy = 0x1, emTeamDifficulty_Normal = 0x2 };
	static BOOL SetDifficultyForTeam(_In_ emTeamDifficulty emDifficulty);

	// 八卦强化
	static BOOL StrengthenBaGua(_In_ DWORD dwEquiSid, _In_ DWORD dwSacrificeSid);

	// 获取八卦属性 (ReturnValue + 0x16 == Attribute)
	static DWORD GetBaGuaAttribute(_In_ DWORD dwId);

	// 创建副本房间密码
	static BOOL CreatePassWorkRoom(_In_ DWORD dwPass);

	// 查找房间
	static BOOL FindDungoenRoom_By_Id(_In_ DWORD dwId, _In_ DWORD dwPass);

	// 队员是否准备了
	static BOOL IsRoomReady(_In_ DWORD dwRoomBase);

	//副本房间 准备
	static BOOL ReadyForGungoenRoom(_In_ DWORD dwRoomBase);

	// 
	static BOOL IsRoomReadyForCaptain(_In_ DWORD dwRoomBase);

	//  选择副本地图
	static BOOL ChoiceGungeonMap(_In_ DWORD dwMapId, _In_ DWORD dwRoomBase);

	//
	static BOOL ChoiceGungeonDifficulty(_In_ DWORD dwValue, _In_ DWORD dwRoomBase);

	//
	static BOOL BackToCrossAreaTeamMainForm();

	//
	static BOOL ExitCrossAreaTeamRoom();

	//
	static BOOL ExitCrossAreaTeamMainForm();

};



#endif