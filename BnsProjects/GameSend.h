#ifndef __BNS_GAMESEND_H__
#define __BNS_GAMESEND_H__

#include "GameVariable.h"


class CGameSend
{
public:
	explicit CGameSend();
	~CGameSend();

	// ʹ�ü���
	static BOOL UseSkill(_In_ DWORD dwSkillId);

	// ʹ����Ʒ
	static BOOL UseItem(_In_ DWORD dwItemParm);

	// ��װ��
	static BOOL WearEqui(_In_ DWORD dwEquiType, _In_ DWORD dwParm);

	// ��װ��
	static BOOL RelieveEqui(_In_ DWORD dwEquiType);

	// �������
	static BOOL HidePlayer();

	// �˳�����
	static BOOL ExitSchool();

	// �������
	static BOOL TakeItem_By_Task(_In_ DWORD dwId, _In_ DWORD dwIdParm);

	// ����Npc
	static BOOL TakeItem_By_Npc(_In_ DWORD dwId, _In_ DWORD dwIdParm);

	// ������Ʒ
	static BOOL DiscardItem(_In_ DWORD dwItemSId, _In_ DWORD dwCount);

	// �ֽ���Ʒ
	static BOOL ExplainItem(_In_ DWORD dwItemSId);

	// ѡ��Ƶ��
	static BOOL SwitchChannel(_In_ DWORD dwChannel);
	
	// С��
	static BOOL BackToSwitchPlayer();

	// ��Ƕ��ʯ
	static BOOL SetGem(_In_ DWORD dwItemSId, _In_ DWORD dwIndex, _In_ DWORD dwInventory2PanelObj);

	// ȡ�±�ʯ
	static BOOL RelieveGem(_In_ DWORD dwIndex);

	// ģ�ⰴ��
	static BOOL SimulationKey(_In_ DWORD dwAscii, _In_ BOOL bPush);

	// ����
	static BOOL TakeItem_By_DropItem(_In_ DWORD dwId, _In_ DWORD dwIdParm);

	// ������
	static BOOL CleanBag();

	// ��
	static BOOL SellItem_By_Npc(_In_ DWORD dwItemSId, _In_ DWORD dwCount, _In_ DWORD dwNpcId, _In_ DWORD dwNpcIdParm);

	// ��
	static BOOL BuyItem_By_Npc(_In_ DWORD dwNpcId, _In_ DWORD dwNpcIdParm, _In_ DWORD dwCount, _In_ DWORD dwIndex);

	// ��
	static BOOL BuyItem_By_Bag(_In_ DWORD dwPageIndex, _In_ DWORD dwIndex, _In_ DWORD dwCount, _In_ DWORD dwItemIdParm);

	// ������
	static BOOL CreateMoreBag();

	// �ֿ⿪����(�ֿ� = 0x3, �¹� = 0x6)
	static BOOL ExpandInvenSlot(_In_ DWORD dwNpcId, _In_ DWORD dwNpcIdParm, _In_ DWORD dwType);

	// ����
	static BOOL TransportCity(_In_ DWORD dwTarCityId);

	// װ��ǿ��
	static BOOL ItemGrowth(_In_ DWORD dwGrowthItemSId, _In_ DWORD dwSacrificeSId, _In_ DWORD dwLuckyCharmSId);

	// װ��ͻ��
	static BOOL ItemEvolution(_In_ DWORD dwGrowthItemSId, _In_ DWORD dwType, _In_ DWORD dwSacrificeGId, _In_ DWORD dwEvolutionId);

	// װ������
	static BOOL ItemEndEvolution(_In_ DWORD dwEqumentSId, _In_ DWORD dwSacrificeSId, _In_ DWORD dwEvoltionId);

	// ��ӡ
	static BOOL UnsealItem(_In_ DWORD dwUnsealItemSId, _In_ DWORD dwUntieId, _In_ DWORD dwUntieCount, _In_ DWORD nType);

	// ������
	static BOOL OpenBox(_In_ DWORD dwBoxSID, _In_ BOOL bSpecial);

	// Կ�׿�����
	static BOOL OpenBox_By_Key(_In_ BOOL bIsLightKey, _In_ DWORD dwKeyId, _In_ DWORD dwKeyCount, _In_ DWORD dwBoxSId);

	// ��·
	static BOOL Move(_In_ float fX, _In_ float fY, _In_ float fZ);

	// �ʼ�
	static BOOL Mail(_In_ DWORD dwNpcId, _In_ DWORD dwNpcIdParm, _In_ MailParm_Src* pMailParm);

	// ���
	static BOOL SaveItem(_In_ DWORD dwNpcId, _In_ DWORD dwNpcIdParm, _In_ DWORD dwItemSId, _In_ DWORD dwCount, _In_ DWORD dwWareHourseSId);

	// ����������
	static BOOL SumitMainTask(_In_ CONST SumitTask_Src* pTaskSrc);
	static BOOL SumitSpecialMainTask(_In_ CONST SumitTask_Src* pTaskSrc);

	// ��֧������
	static BOOL PickBLTask(_In_ CONST SumitBLTask_Src* pTaskSrc);

	// ˢ��Ƶ��
	static BOOL RefushChannel();

	// ɾ����ɫ
	static BOOL DeletePlayer(_In_ DWORD dwIndexParm);

	// ������ɫCALL
	static BOOL CreatePlayer(_In_ LPCWSTR pwszPlayerName, _In_ DWORD dwAreaId, _In_ DWORD dwIndex, _In_ DWORD dwClasses, _In_ BOOL bIsQuickLevel = FALSE);

	// ��ʼPK
	static BOOL StartPK();

	// �뿪PK��
	static BOOL ExitPK();

	// PK����CALL
	static BOOL EndPK();

	// PK׼��CALL
	static BOOL ReadyPk();

	// ��ʬ��
	static BOOL PickCorpse(_In_ DWORD dwId, _In_ DWORD dwIdParm);

	// ��ȡ�㿨ʱ��
	static BOOL RefushDianKa(_In_ DWORD dwPanelObj);

	// �޸��Ӿ�
	static BOOL SwitchAngel(_In_ DWORD dwPanelObj);

	// ��¼
	static BOOL Login(_In_ DWORD dwAccountAddr, _In_ DWORD dwPassAddr);

	// ���ʼ�
	static BOOL RecvMail(_In_ DWORD dwNpcId, _In_ DWORD dwNpcIdParm, _In_ DWORD dwUiObj, _In_ DWORD dwMailParm);

	// ������Ʒ�ʼ�
	static BOOL RecvEmptyMail(_In_ DWORD dwUiObj);

	// ���ʼ�
	static BOOL RecvAllMail(_In_ DWORD dwUiObj);

	// ���þ���Ʒ��
	static BOOL SetAuctionQuality(_In_ em_Auction_Quality emAuctionQuality);

	// �������CALL
	static BOOL InviteTeamMember(_In_ DWORD dwId, _In_ DWORD dwIdParm);

	// �뿪����
	static BOOL LeaveTeam();

	// ����
	static BOOL TakeBall(_In_ DWORD dwId, _In_ DWORD dwIdParm);

	// һ�μ���
	static BOOL Pickup(_In_ DWORD dwId, _In_ DWORD dwIdParm);

	// ˲��
	static BOOL Telport(_In_ DWORD* pArray, _In_ DWORD dwCALL, _In_ DWORD dwPersonBase);
	static BOOL Telport(_In_ DWORD dwCALL, _In_ DWORD dwPersonBase);

	// �л�������
	static BOOL SwitchNewMainTask(_In_ DWORD dwUiObj);

	// ���ܼӵ�
	static BOOL AddSkillPoint(_In_ DWORD dwIndex, _In_ DWORD dwLearnId);

	// ���ܼӵ㷢��
	static BOOL AddSkillPoint_Send(_In_ DWORD dwSkillBook3Panel);

	// �л�����ϵ
	static BOOL SwitchSkillPage(_In_ DWORD dwSkillBook3Panel);

	// ����
	static BOOL ActionCALL(_In_ DWORD dwActionId);

	// ����
	static BOOL TalkTo(_In_ LPCWSTR pwszPlayerName, _In_ LPCWSTR pwszContent);

	// ������
	static BOOL InviteTansaction(_In_ DWORD dwPlayerId, _In_ DWORD dwPlayerIdParm);

	// ���ý�����Ʒ
	static BOOL PushTransactionItem(_In_ DWORD dwTradeShelfPanelObj, _In_ DWORD dwSId, _In_ DWORD dwCount);

	// �������׿�
	static BOOL LockTransaction();

	// ����
	static BOOL SumitTransaction(_In_ DWORD Addres);

	// ���ý��׽�Ǯ
	static BOOL PushTransactionMoney(_In_ DWORD dwMoney);

	// ȡ������
	static BOOL CancelDeal();

	// ���ܽ���
	static BOOL AcceptDeal();

	// ����
	static BOOL Escape();

	// ����ID����OBJ
	static DWORD GetItemObj_By_ID(_In_ DWORD dwItemId);

	// ��ȡ����ID
	static DWORD GetCurrentCityId();

	// ��ȡ��Ʒ����ID
	static DWORD GetItemType(_In_ DWORD dwItemId);

	// ��ȡ��Ʒ��������Addr
	static DWORD GetItemTypeNameAddr(_In_ DWORD dwParm1, _In_ DWORD dwParm2);

	// ��������
	//static VOID SetForgotSkill(_In_ DWORD dwUiObj);
	//static VOID ConfirmForgotSkill(_In_ DWORD dwUiObj);

	// ���ܶ�������
	static BOOL AcceptTeamInvite(_In_ DWORD dwObj);

	// �ر���ϷUI
	static BOOL CloseGameWindow(_In_ DWORD dwObj);

	// 
	static BOOL CloseGameUi_Task_50Level(_In_ DWORD JumpingLevelPanel);

	// ����ѵ�����ر�UI
	static BOOL SetShowUi(_In_ DWORD dwObj, _In_ BOOL bShow);

	// ��ϷUI��YES
	static BOOL ClickYes(_In_ DWORD dwObj);

	// ����
	static DWORD GetSkillParm(_In_ DWORD dwSkillId);
	static DWORD GetSkillObj(_In_ DWORD dwSkillId, _In_ DWORD dwSkillParm);

	// ��ѯ��ǰɱ������
	static DWORD QueryTaskScheduleCount(_In_ DWORD dwTaskId);

	// ��������
	static BOOL OpenGiftBox(_In_ DWORD dwGiftId);

	// ������
	static BOOL RecvGift(_In_ DWORD dwItemId, _In_ DWORD dwGiftParm, _In_ DWORD dwGiftID, _In_ DWORD dwBoxId);

	// ���μ���CALL
	static BOOL SecondPickup(_In_ DWORD dwId, _In_ DWORD dwParm);

	// ʹ�����ھ�
	static DWORD GetLevelTickObj();
	static BOOL UseKfmVolume1();
	static BOOL UseKfmVolume2();

	// �����Ա
	static BOOL RemoveTeamMember(_In_ DWORD dwIndex);

	// �ٻ���è
	static BOOL InitializeCatForSummoner(_In_ DWORD dwNpcId, _In_ DWORD dwNpcIdParm, _In_ LPCWSTR wsCatName, _In_ DWORD dwCatParamter);
	static BOOL InitializeCatForSummoner(_In_ DWORD JumpingLevelPanelBase, _In_ DWORD dwIndex, _In_ LPCWSTR wsCatName);

	// èʹ�ü���
	static BOOL UseSkillByCat(_In_ DWORD dwSkillId);

	// ����ѵ������ȡ����(ְҵ,�ڼ��׶εĽ���)
	static BOOL AcceptPrizesForBaiXianPracticeField(_In_ DWORD dwOpponentClasses, _In_ DWORD dwPrizesIndex);

	// �ٻ�����ѵ������Ӣ��
	static BOOL BeckonHeroForBaiXianPracticeField(_In_ DWORD dwDuelBotTrainingSelectJobPanel, _In_ DWORD dwHeroIndex, _In_ DWORD dwStageIndex);

	// �˳�����ѵ����
	static BOOL ExitBaiXianPracticeField();

	// ʱ��֮��ת��(������1��ʼ)
	static BOOL TurntableBySpaceTimeTurret(_In_ DWORD dwIndex);

	// �˳�ʱ��֮��
	static BOOL ExitSpaceTimeTurret();

	// ʱ��֮����ȡ����
	static BOOL AcceptPrizesForSpaceTimeTurret();

	// ���ýǶ�
	static BOOL SetAngle(_In_ DWORD dwAngle);

	// �����Ե�
	static BOOL AddPlayerAttribute(_In_ DWORD dwPowerAttribute, _In_ DWORD dwHpAttributeCount);

	// ����PK|���� ����
	enum em_Room_Type : DWORD
	{
		em_Room_Type_PK = 0x1,
		em_Room_Type_Dungeon = 0x2
	};
	static BOOL IntoRoom(_In_ DWORD TotalGateConfirmPanelBase, _In_ em_Room_Type emRoom);

	// �ٻ�������(����50��)
	static BOOL SummonerTakeGift(_In_ DWORD JumpingLevelPanelBase);

	//  �뿪����ѵ����
	static BOOL LeaveSkillTrainRoom();

	static BOOL LeaveSkillTrainRoom_Confirm();

	static BOOL UseTicket_By_Level50();

	static BOOL Talk(_In_ LPCWSTR pwszText, _In_ DWORD ChatInputPanelBase);

	static BOOL InputOTP(_In_ LPCWSTR pwszOtpText);

	static LPVOID AllocGameMem(_In_ DWORD dwSize);

	// ����
	static BOOL Resurrect();

	//̨���켶����
	static VOID ReceiveLvPrize();
	// ��ȡ�Ǳ�
	struct StarCoinContent
	{
		WORD wParm1;
		WORD wParm2;
	};
	static BOOL TakeStarCoin(_In_ CONST StarCoinContent& StarCoinContent_);

	static BOOL ShopStarCoinStore(_In_ DWORD dwItemId, _In_ DWORD dwGoodsStorePanel);

	// �޸�����Ѷ�
	enum emTeamDifficulty{ emTeamDifficulty_Easy = 0x1, emTeamDifficulty_Normal = 0x2 };
	static BOOL SetDifficultyForTeam(_In_ emTeamDifficulty emDifficulty);

	// ����ǿ��
	static BOOL StrengthenBaGua(_In_ DWORD dwEquiSid, _In_ DWORD dwSacrificeSid);

	// ��ȡ�������� (ReturnValue + 0x16 == Attribute)
	static DWORD GetBaGuaAttribute(_In_ DWORD dwId);

	// ����������������
	static BOOL CreatePassWorkRoom(_In_ DWORD dwPass);

	// ���ҷ���
	static BOOL FindDungoenRoom_By_Id(_In_ DWORD dwId, _In_ DWORD dwPass);

	// ��Ա�Ƿ�׼����
	static BOOL IsRoomReady(_In_ DWORD dwRoomBase);

	//�������� ׼��
	static BOOL ReadyForGungoenRoom(_In_ DWORD dwRoomBase);

	// 
	static BOOL IsRoomReadyForCaptain(_In_ DWORD dwRoomBase);

	//  ѡ�񸱱���ͼ
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