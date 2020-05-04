#ifndef __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_SCRIPTFILE_SCRIPTFILE_H__
#define __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_SCRIPTFILE_SCRIPTFILE_H__

#include <MyTools/ClassInstance.h>

class CScriptFile : public MyTools::CClassInstance<CScriptFile>
{
public:
	struct CipherScriptText
	{
		std::wstring wsScriptName;				// �ű���
		std::wstring wsCapter;					// 
		std::shared_ptr<CHAR> pScriptBuffer;	// ���ܺ�Ľű�����
		std::shared_ptr<WCHAR> pExpressScript;  // 
		UINT uExpressSize      = 0;				// ���Ľű���С
		UINT uCipherTextSize   = 0;				// ���Ľű���С
		DWORD dwExpressTextCrc = NULL;			// ���Ľű�CRC(ClientУ�����)
		DWORD dwCipherTextCrc  = NULL;			// ���Ľű�CRC(socket����У��)
	};
public:
	CScriptFile() : _Lock(L"CScriptFile.Lock") {};
	~CScriptFile() = default;

	// 
	VOID Initialize();

	// ��ȡ��ǰĿ¼'TaskScript'�Ľű����ݲ��Ҽ��ܵ��ڴ�
	BOOL ReadScriptFileToMemory();

	// ���ҽű�
	BOOL FindScriptText_By_ScriptName(_In_ CONST std::wstring& wsScriptName, _Out_opt_ CipherScriptText& Content) CONST;

	// ���Խű��Ƿ���Ч
	BOOL TestEffectiveScriptContent() CONST;

	BOOL FindCapterByTaskId(_In_ DWORD dwTaskId, _Out_ std::wstring& wsCapter) CONST;

	VOID Print() CONST;
private:
	// ����Ŀ¼
	BOOL TraverseDirectory(_In_ CONST std::vector<std::wstring> VecDirectory);

	// ��ȡ���ܺ�Ľű�
	BOOL ReadScriptFileContent(_In_ CONST std::wstring& wsPath, _In_ _Out_ CipherScriptText& Content);

	// ��ȡ'�ճ�'�ļ����µ�����ID
	BOOL ReLoadDailyTask() CONST;
private:

	// �����Ƿ����ظ��Ľű�
	BOOL TestRepeatScriptFile();

private:
	MyTools::CLLock _Lock;
	std::map<std::wstring, CipherScriptText> _MapScript;
	BOOL _bRepeat;
};



#endif // !__GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_SCRIPTFILE_SCRIPTFILE_H__
