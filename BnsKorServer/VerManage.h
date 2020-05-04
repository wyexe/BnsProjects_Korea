#ifndef __GITBNSPROJECTS_BNSKORSERVER_VERMANAGE_H__
#define __GITBNSPROJECTS_BNSKORSERVER_VERMANAGE_H__

#include "DataBaseModel.h"




class ByteBuffer;
class CVerManage
{
public:
	CVerManage();
	~CVerManage();

	// 解包
	static BOOL UnPack(_In_ DWORD dwCmd, _In_ ByteBuffer& ClientByteBuffer,_Out_ LPVOID lpParm);

	// 查询单张卡(使用前记得加锁)
	static BOOL IsExistCard(_In_ DWORD dwCardCrc, _Out_ CardInfo& CardInfo_);
	// 查询多张卡(内部已加锁)
	static BOOL IsExistCardList(_In_ CONST std::vector<DWORD>& VecCardCrc, _Out_ std::vector<VerCardInfo>& vCardList);

	// 
	static BOOL QueryClientCard(_In_ CONST std::vector<DWORD>& VecCardCrc, _In_ CONST ClientAccountContent& ClientAccount, _Out_ ByteBuffer& ServerByteBuffer);

	// 返回卡号
	static BOOL QueryServerCard(_Out_ std::deque<CardInfo>& vCard);

	//
	static BOOL TryActiveCard(_In_ DWORD dwCardCrc, _In_ CONST std::wstring& wstrIp, _Out_ CardInfo& CardInfo_);

	//
	static BOOL KeepALive(_In_ CONST KeepALiveContent& KeepALiveContent_, _In_ ClientAccountContent& ClientAccount, _Out_ ByteBuffer& ServerByteBuffer);

	//
	static BOOL GetVerTitle(_In_ std::wstring& wstrIp, _Out_ ByteBuffer& ServerByteBuffer);

	// 游戏登录的时候发来的验证! 将Token,Ip,Tick初始化
	static BOOL SetToken(_In_ ClientAccountContent& AccountContent, _Out_ ByteBuffer& ServerByteBuffer);

	static BOOL IsALive(_In_ CONST ClientAccountContent& ClientAccount, _Out_ ByteBuffer& ServerByteBuffer);

	// 卡号是否正确
	static BOOL CheckCard(_In_ CONST ClientAccountContent& ClientAccount, _Out_ ByteBuffer& ServerByteBuffer);

	static BOOL IsCompServer(_In_ DWORD dwServerId1, _In_ DWORD dwServerId2);

	// 添加交易信息到数据库
	static BOOL AddDealInfo(_In_ CONST ClientAccountContent& ClientAccount, _In_ CONST VerDealInfo& VerDealInfo_);

	// 添加断开连接信息到数据库
	static BOOL AddBreakConnectInfo(_In_ CONST std::wstring& wsIp, _In_ CONST std::wstring wsAccountName);

	static std::wstring& GetBnsVersion(_In_ BOOL bReLoad);

	static BOOL DoAction_By_CardList(_In_ std::function<VOID(std::deque<CardInfo>&)> fn);

	// 添加登录完毕的连接信息到数据库
	static BOOL AddLoginedMsgToDb(_In_ CONST ClientAccountContent& ClientAccount);

	// 封号了, 判断是否老号!
	static BOOL IsNewBanned(_In_ CONST std::wstring& wsAccountName);

	static BOOL& IsBanned();

	// 是否无效的帐号, 返回TRUE表示该帐号已经禁用了!
	static BOOL IsInvalidAccount(_In_ CONST std::wstring& wsAccountName, _In_ CONST std::wstring& wsIp, _In_ CONST std::wstring& wsCardNo);

	static BOOL SaveAccountLog(_In_ CONST ClientAccountLog& ClientLog, _Out_ ByteBuffer& ByteBuffer_);

	// 添加封号记录到数据库
	static BOOL AddBannedAccountToDb(_In_ CONST ClientAccountContent& ClientAccount);
};



#endif