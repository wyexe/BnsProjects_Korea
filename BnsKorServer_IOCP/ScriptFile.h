#ifndef __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_SCRIPTFILE_SCRIPTFILE_H__
#define __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_SCRIPTFILE_SCRIPTFILE_H__

#include <MyTools/ClassInstance.h>

class CScriptFile : public MyTools::CClassInstance<CScriptFile>
{
public:
	struct CipherScriptText
	{
		std::wstring wsScriptName;				// 脚本名
		std::wstring wsCapter;					// 
		std::shared_ptr<CHAR> pScriptBuffer;	// 加密后的脚本内容
		std::shared_ptr<WCHAR> pExpressScript;  // 
		UINT uExpressSize      = 0;				// 明文脚本大小
		UINT uCipherTextSize   = 0;				// 密文脚本大小
		DWORD dwExpressTextCrc = NULL;			// 明文脚本CRC(Client校验解密)
		DWORD dwCipherTextCrc  = NULL;			// 密文脚本CRC(socket传输校验)
	};
public:
	CScriptFile() : _Lock(L"CScriptFile.Lock") {};
	~CScriptFile() = default;

	// 
	VOID Initialize();

	// 读取当前目录'TaskScript'的脚本内容并且加密到内存
	BOOL ReadScriptFileToMemory();

	// 查找脚本
	BOOL FindScriptText_By_ScriptName(_In_ CONST std::wstring& wsScriptName, _Out_opt_ CipherScriptText& Content) CONST;

	// 测试脚本是否有效
	BOOL TestEffectiveScriptContent() CONST;

	BOOL FindCapterByTaskId(_In_ DWORD dwTaskId, _Out_ std::wstring& wsCapter) CONST;

	VOID Print() CONST;
private:
	// 遍历目录
	BOOL TraverseDirectory(_In_ CONST std::vector<std::wstring> VecDirectory);

	// 读取加密后的脚本
	BOOL ReadScriptFileContent(_In_ CONST std::wstring& wsPath, _In_ _Out_ CipherScriptText& Content);

	// 读取'日常'文件夹下的任务ID
	BOOL ReLoadDailyTask() CONST;
private:

	// 测试是否有重复的脚本
	BOOL TestRepeatScriptFile();

private:
	MyTools::CLLock _Lock;
	std::map<std::wstring, CipherScriptText> _MapScript;
	BOOL _bRepeat;
};



#endif // !__GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_SCRIPTFILE_SCRIPTFILE_H__
