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
	// ������ɫ
	BOOL CreatePlayer(_In_ DWORD dwAreaId, _In_ DWORD dwIndex, _In_ em_Player_Classes emPlayerClasses, _In_ BOOL IsQuickLevel = FALSE) CONST;

	VOID GetRandPlayerName(_Out_ std::wstring& wsPlayerName, _In_ int nNameLength) CONST;

	// ��ȡ��ɫ�ȼ�
	DWORD GetLevel_By_Index(_In_ DWORD dwIndex) CONST;

	// ��ȡ��ǰ�����ɫ����
	DWORD GetLoginPlayerIndex() CONST;
	BOOL  SetLoginPlayerIndex(_In_ UINT uIndex) CONST;

	// ɾ����ɫ
	BOOL DeletePlayer(_In_ DWORD dwIndex, _In_ DWORD dwSleepTime = NULL) CONST;
	BOOL DeletePlayer_By_Condition(_In_ std::function<BOOL(CONST CLoginPlayer&)> fnCondition) CONST;

	// ���ؽ�ɫ״̬
	em_LoginPlayerStatus GetLoginPlayerStatus_By_Index(_In_ DWORD dwIndex) CONST;

	// ���ؽ�ɫ�ķ�����ID
	DWORD	GetLoginPlayerServerId_By_Index(_In_ DWORD dwIndex) CONST;

	// �Ƿ���ڷ��������淶�Ľ�ɫ
	BOOL ExistStandardLoginPlayer(_Out_opt_ CLoginPlayer* pLoginPlayer) CONST;

	// ���ݽ�ɫ�����ض���
	BOOL ExistLoginPlayer_By_Name(_In_ CONST std::wstring& wsLoginPlayerName, _Out_opt_ CLoginPlayer* pLoginPlayer) CONST;

	// ʹ�����ڄ�
	BOOL UseKfmVolume() CONST;

	// ʣ��ֱ��50���Ą�
	BOOL ExistLevelTick() CONST;

	BOOL IsUseLevelTick() CONST;
private:
	DSIABLE_COPY_AND_ASSIGN(CLoginPlayerExtend)
};



#endif