#ifndef __GITBNSPROJECTS_BNSPROJECTS_SOCKET_SOCKETAPI_H__
#define __GITBNSPROJECTS_BNSPROJECTS_SOCKET_SOCKETAPI_H__

#include "../BnsProjects/GameStruct.h"
#include "../BnsKorServer_IOCP/ServerCommon.h"


class CSocketAPI
{
public:
	enum class em_JoinTeam_Type
	{
		// 主线
		em_JoinTeam_Type_MainLine,
		// 副本
		em_JoinTeam_Type_Gungoen
	};

	enum class em_CrossAreaTeam_ResultType
	{
		Succ, Undefine, Timeout
	};

	struct CiphertextScriptInfo
	{
		std::wstring wsScriptName;				// 脚本名字
		std::wstring wsAccountName;
		std::shared_ptr<CHAR> pScriptContent;	// 密文脚本
		DWORD uExpressSize;						// 明文脚本长度
		DWORD uCipherTextSize;					// 密文脚本长度
		DWORD dwExpressCRC;						// 明文CRC
		DWORD dwCiphertextCRC;					// 密文CRC

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

	// 心跳
	VOID ConsoleKeepALive() CONST;

	// 查询卡号
	BOOL QueryCard(_In_ _Out_ std::vector<CardInfo>& VecCard) CONST;

	// 公告
	BOOL GetServerAnnouncement(_Out_ std::wstring& wsAnnouncement) CONST;

	////Client//////////////////////////////////////////////////////////////////////

	// Run
	BOOL Run(_In_ CONST std::wstring& wsIp, _In_ USHORT uPort) CONST;

	// Stop
	VOID Stop() CONST;

	// 心跳
	VOID KeepALive(_In_ CONST KeepALiveContent* pContent) CONST;

	// 下载脚本
	BOOL DownLoadScript(_In_ _Out_ CiphertextScriptInfo& Content, _Out_opt_ std::wstring& wsErrText) CONST;

	// 登录
	VOID AlreadyLogin(_In_ float fVersion, _In_ CONST std::wstring& wsAccountName, _In_ CONST std::wstring& wsAccountPass, _In_ CONST std::wstring& wsCardNo) CONST;

	// 组队
	VOID JoinTeam(_In_ em_JoinTeam_Type emType, _In_ CONST ClientTeamInfo* pClientTeamInfo, _Out_ SocketTeamResult& Result) CONST;

	// 离队
	VOID LeaveTeam(_In_ em_JoinTeam_Type emType) CONST;

	// 小号交易
	VOID ClientTrade(_In_ em_Trade_Warehouse_Type emType, _In_ CONST VerItemSupport& Content, _Out_ ItemSupportResult& Result) CONST;

	// 仓库号交易
	VOID WarehouseTrade(_In_ em_Trade_Warehouse_Type emType, _In_ CONST WareHouseInfo& Content, _Out_ WarehouseActionResult& Result) CONST;

	// 设置副本进度
	VOID SetGungoenSchedule(_In_ DWORD dwTeamId, _In_ CONST std::wstring& wsScheduleName, _In_ UINT uScheduleIndex) CONST;

	// 获取副本进度
	VOID GetGungoenSchedule(_In_ DWORD dwTeamId, _Out_ std::vector<TeamMemberSchedule>& VecResult) CONST;

	// 设置队伍只读
	VOID SetTeamReadonly(_In_ DWORD dwTeamId) CONST;

	// 交易日志
	VOID AddTradeContentLog(_In_ CONST TradeContent& Content) CONST;

	// 登录日志
	VOID AddLoginContentLog(_In_ CONST std::wstring& wsPlayerName) CONST;

	// 封号日志
	VOID AddBannedContentLog() CONST;

	// 帐号日志
	VOID AddAccountLog(_In_ CONST ClientAccountLog& Content) CONST;

	// 获取最少人的频道
	DWORD GetLessPlayerChannel(_In_ CONST TeamChannelMsg& Content) CONST;

	// 读取云配置
	VOID ReadCloudConfig(_In_ CONST std::wstring& wsPlayerName, _In_ _Out_ ConfigContent& Content) CONST;

	// 修改云配置
	VOID WriteCloudConfig(_In_ CONST ConfigContent& Content) CONST;

	// OTP
	DWORD CalcualteOTP() CONST;

	// 获取未完成的日常任务
	BOOL GetDailyTaskSchedule(_Out_ std::vector<std::wstring>& VecResult) CONST;

	// 修改日常任务=完成
	VOID SetDailyTaskSchedule(_In_ CONST std::wstring& wsDailyTaskName) CONST;

	// 跨服组队
	DWORD JoinCrossAreaTeam() CONST;

	// 设置跨服组队的Detail
	BOOL SetCrossAreaTeamContent(_In_ CONST CrossAreaTeamMember& Member) CONST;

	// 根据队伍ID获取跨服组队的队友内容(队长返回所有队员, 队员只返回队长)
	em_CrossAreaTeam_ResultType GetTeamGroupContent(_In_ DWORD dwTeamId, _Out_ CrossAreaTeamGroup& Teamgroup) CONST;

	// 跨服离队
	VOID LeaveCrossAreaTeam(_In_ DWORD dwTeamId) CONST;

	// 查询账号的虚拟硬件
	VOID QueryAccountVirtualSeriaNumber(_In_ CONST std::vector<std::wstring>& VecAccount, _Out_ std::vector<SocketAccountVirtualSeriaNumber>& VecVirtualSeriaNumber) CONST;
private:

};



#endif // !__GITBNSPROJECTS_BNSPROJECTS_SOCKET_SOCKETAPI_H__
