#ifndef __GITBNSPROJECTS_BNSPROJECTS_SOCKET_SOCKETAPI_H__
#define __GITBNSPROJECTS_BNSPROJECTS_SOCKET_SOCKETAPI_H__

#include "../BnsProjects/GameStruct.h"
#include "../BnsKorServer_IOCP/ServerCommon.h"


class CSocketAPI
{
public:
	enum class em_JoinTeam_Type
	{
		// ����
		em_JoinTeam_Type_MainLine,
		// ����
		em_JoinTeam_Type_Gungoen
	};

	enum class em_CrossAreaTeam_ResultType
	{
		Succ, Undefine, Timeout
	};

	struct CiphertextScriptInfo
	{
		std::wstring wsScriptName;				// �ű�����
		std::wstring wsAccountName;
		std::shared_ptr<CHAR> pScriptContent;	// ���Ľű�
		DWORD uExpressSize;						// ���Ľű�����
		DWORD uCipherTextSize;					// ���Ľű�����
		DWORD dwExpressCRC;						// ����CRC
		DWORD dwCiphertextCRC;					// ����CRC

		CiphertextScriptInfo() = default;
		void Clear()
		{
			wsScriptName.clear();
			uExpressSize = uCipherTextSize = dwExpressCRC = dwCiphertextCRC = NULL;
			pScriptContent.reset();
		}
	};

	struct SocketTeamResult
	{
		DWORD dwTeamId;
		ClientTeamMember Captain;
		std::vector<ClientTeamMember> VecMember;
	};

public:
	CSocketAPI() = default;
	~CSocketAPI() = default;

	///////Console///////////////////////////////////////////////////////////////////

	// ����
	VOID ConsoleKeepALive() CONST;

	// ��ѯ����
	BOOL QueryCard(_In_ _Out_ std::vector<CardInfo>& VecCard) CONST;

	// ����
	BOOL GetServerAnnouncement(_Out_ std::wstring& wsAnnouncement) CONST;

	////Client//////////////////////////////////////////////////////////////////////

	// Run
	BOOL Run(_In_ CONST std::wstring& wsIp, _In_ USHORT uPort) CONST;

	// Stop
	VOID Stop() CONST;

	// ����
	VOID KeepALive(_In_ CONST KeepALiveContent* pContent) CONST;

	// ���ؽű�
	BOOL DownLoadScript(_In_ _Out_ CiphertextScriptInfo& Content, _Out_opt_ std::wstring& wsErrText) CONST;

	// ��¼
	VOID AlreadyLogin(_In_ float fVersion, _In_ CONST std::wstring& wsAccountName, _In_ CONST std::wstring& wsAccountPass, _In_ CONST std::wstring& wsCardNo) CONST;

	// ���
	VOID JoinTeam(_In_ em_JoinTeam_Type emType, _In_ CONST ClientTeamInfo* pClientTeamInfo, _Out_ SocketTeamResult& Result) CONST;

	// ���
	VOID LeaveTeam(_In_ em_JoinTeam_Type emType) CONST;

	// С�Ž���
	VOID ClientTrade(_In_ em_Trade_Warehouse_Type emType, _In_ CONST VerItemSupport& Content, _Out_ ItemSupportResult& Result) CONST;

	// �ֿ�Ž���
	VOID WarehouseTrade(_In_ em_Trade_Warehouse_Type emType, _In_ CONST WareHouseInfo& Content, _Out_ WarehouseActionResult& Result) CONST;

	// ���ø�������
	VOID SetGungoenSchedule(_In_ DWORD dwTeamId, _In_ CONST std::wstring& wsScheduleName, _In_ UINT uScheduleIndex) CONST;

	// ��ȡ��������
	VOID GetGungoenSchedule(_In_ DWORD dwTeamId, _Out_ std::vector<TeamMemberSchedule>& VecResult) CONST;

	// ���ö���ֻ��
	VOID SetTeamReadonly(_In_ DWORD dwTeamId) CONST;

	// ������־
	VOID AddTradeContentLog(_In_ CONST TradeContent& Content) CONST;

	// ��¼��־
	VOID AddLoginContentLog(_In_ CONST std::wstring& wsPlayerName) CONST;

	// �����־
	VOID AddBannedContentLog() CONST;

	// �ʺ���־
	VOID AddAccountLog(_In_ CONST ClientAccountLog& Content) CONST;

	// ��ȡ�����˵�Ƶ��
	DWORD GetLessPlayerChannel(_In_ CONST TeamChannelMsg& Content) CONST;

	// ��ȡ������
	VOID ReadCloudConfig(_In_ CONST std::wstring& wsPlayerName, _In_ _Out_ ConfigContent& Content) CONST;

	// �޸�������
	VOID WriteCloudConfig(_In_ CONST ConfigContent& Content) CONST;

	// OTP
	DWORD CalcualteOTP() CONST;

	// ��ȡδ��ɵ��ճ�����
	BOOL GetDailyTaskSchedule(_Out_ std::vector<std::wstring>& VecResult) CONST;

	// �޸��ճ�����=���
	VOID SetDailyTaskSchedule(_In_ CONST std::wstring& wsDailyTaskName) CONST;

	// ������
	DWORD JoinCrossAreaTeam() CONST;

	// ���ÿ����ӵ�Detail
	BOOL SetCrossAreaTeamContent(_In_ CONST CrossAreaTeamMember& Member) CONST;

	// ���ݶ���ID��ȡ�����ӵĶ�������(�ӳ��������ж�Ա, ��Աֻ���ضӳ�)
	em_CrossAreaTeam_ResultType GetTeamGroupContent(_In_ DWORD dwTeamId, _Out_ CrossAreaTeamGroup& Teamgroup) CONST;

	// ������
	VOID LeaveCrossAreaTeam(_In_ DWORD dwTeamId) CONST;

	// ��ѯ�˺ŵ�����Ӳ��
	VOID QueryAccountVirtualSeriaNumber(_In_ CONST std::vector<std::wstring>& VecAccount, _Out_ std::vector<SocketAccountVirtualSeriaNumber>& VecVirtualSeriaNumber) CONST;
private:

};



#endif // !__GITBNSPROJECTS_BNSPROJECTS_SOCKET_SOCKETAPI_H__
