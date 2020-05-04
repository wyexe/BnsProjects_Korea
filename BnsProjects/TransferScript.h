#ifndef __GITBNSPROJECTS_BNSPROJECTS_GAME_SOCKET_SCRIPT_TRANSFERSCRIPT_H__
#define __GITBNSPROJECTS_BNSPROJECTS_GAME_SOCKET_SCRIPT_TRANSFERSCRIPT_H__

#include "../ConsoleDLL/SocketAPI.h"

class CTransferScript
{
public:
	CTransferScript() = default;
	~CTransferScript() = default;

	// ���ؽű�
	std::wstring DownLoadScript(_In_ CONST std::wstring& wsScriptName) CONST;

	// �ӷ������ϲ�ѯ���컹�ж��ٸ��ճ�������
	VOID QueryDailyTask(_Out_ std::vector<std::wstring>& VecDailyTask) CONST;
private:
	// ���ܽű�
	BOOL DecodeScript(_In_ _Out_ CSocketAPI::CiphertextScriptInfo& CiphertextContent, _Out_ std::wstring& wsScriptContent) CONST;
private:
	CSocketAPI _SocketAPI;
};



#endif // !__GITBNSPROJECTS_BNSPROJECTS_GAME_SOCKET_SCRIPT_EXTRACTSCRIPT_H__
