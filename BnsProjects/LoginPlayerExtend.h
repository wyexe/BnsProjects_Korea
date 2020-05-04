#ifndef __GITBNSPROJECTS_BNSPROJECTS_OJECTS_GAMEOBJECTS_LOGINPLAYER_LOGINPLAYEREXTEND_H__
#define __GITBNSPROJECTS_BNSPROJECTS_OJECTS_GAMEOBJECTS_LOGINPLAYER_LOGINPLAYEREXTEND_H__

#include "GameVariable.h"

class CLoginPlayer;
class CLoginPlayerExtend : public MyTools::CClassInstance<CLoginPlayerExtend>
{
private:
	enum
	{
		Min_NameLength = 0x4
	};
public:
	CLoginPlayerExtend() = default;
	~CLoginPlayerExtend() = default;

	static BOOL					IsExistStanadrdLoginPlayerName(_In_ CONST std::wstring& wsLoginPlayerName);
public:
	// 创建角色
	BOOL CreatePlayer(_In_ DWORD dwAreaId, _In_ DWORD dwIndex, _In_ em_Player_Classes emPlayerClasses, _In_ BOOL IsQuickLevel = FALSE) CONST;

	VOID GetRandPlayerName(_Out_ std::wstring& wsPlayerName, _In_ int nNameLength) CONST;

	// 获取角色等级
	DWORD GetLevel_By_Index(_In_ DWORD dwIndex) CONST;

	// 获取当前人物角色索引
	DWORD GetLoginPlayerIndex() CONST;
	BOOL  SetLoginPlayerIndex(_In_ UINT uIndex) CONST;

	// 删除角色
	BOOL DeletePlayer(_In_ DWORD dwIndex, _In_ DWORD dwSleepTime = NULL) CONST;
	BOOL DeletePlayer_By_Condition(_In_ std::function<BOOL(CONST CLoginPlayer&)> fnCondition) CONST;

	// 返回角色状态
	em_LoginPlayerStatus GetLoginPlayerStatus_By_Index(_In_ DWORD dwIndex) CONST;

	// 返回角色的服务器ID
	DWORD	GetLoginPlayerServerId_By_Index(_In_ DWORD dwIndex) CONST;

	// 是否存在符合命名规范的角色
	BOOL ExistStandardLoginPlayer(_Out_opt_ CLoginPlayer* pLoginPlayer) CONST;

	// 根据角色名返回对象
	BOOL ExistLoginPlayer_By_Name(_In_ CONST std::wstring& wsLoginPlayerName, _Out_opt_ CLoginPlayer* pLoginPlayer) CONST;

	// 使用气宗劵
	BOOL UseKfmVolume() CONST;

	// 剩余直接50级的劵
	BOOL ExistLevelTick() CONST;

	BOOL IsUseLevelTick() CONST;
private:
	DSIABLE_COPY_AND_ASSIGN(CLoginPlayerExtend)
};



#endif