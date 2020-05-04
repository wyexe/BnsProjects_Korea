#ifndef __GITBNSPRJECTS_BNSKORSERVER_UPDATEFILE_H__
#define __GITBNSPRJECTS_BNSKORSERVER_UPDATEFILE_H__

#include <memory>
#include <vector>
#include <deque>
#include <MyTools/ToolsPublic.h>
#include <MyTools/CLLock.h>

typedef struct _ExpressScriptInfo
{
	std::wstring wsScriptName;					// 脚本名字
	std::wstring wsCapter;						// 章节
	std::shared_ptr<CHAR> pScriptContent;		// 明文脚本
	DWORD dwScriptSize;							// 脚本大小
	DWORD dwContentLength;						// 加密内容长度
	DWORD dwScriptNameHash;						// 名字Hash
	DWORD dwExpressCRC;							// 明文CRC
	DWORD dwCiphertextCRC;						// 密文CRC
	_ExpressScriptInfo()
	{
		dwScriptNameHash = NULL;
		dwExpressCRC = dwCiphertextCRC = NULL;
	}
}ExpressScriptInfo;

typedef struct _ExpressScriptList
{
	std::vector<ExpressScriptInfo> vScript;
	float fVersion;
	CLLock ScriptLock;
	_ExpressScriptList() : ScriptLock(L"ExpressScriptList::ScriptLock")
	{
		fVersion = 1.0f;
	}
}ExpressScriptList;

extern ExpressScriptList vExpressScriptList;
class CUpdateFile
{
public:
	CUpdateFile();
	~CUpdateFile();

	// ReLoad Script File
	static BOOL UpdateScript();
	// 查询明文表
	static BOOL GetExpressScriptInfo(_In_ LPCSTR pszFileName, _Out_ ExpressScriptInfo& Esi);

	// GetCapter
	static std::wstring QueryCapter_By_ExpressScript(_In_ DWORD dwTaskId);

private:
	// Find *.inf in Dirctory
	static BOOL GetScriptFile(_In_ std::vector<ExpressScriptInfo>& vScript);

	// Read *.inf
	static BOOL ReadScriptFile(_In_ _Out_ ExpressScriptInfo& Esi);

	// Check Repeat File
	static BOOL ExistRepeatFile(_In_ CONST std::vector<ExpressScriptInfo>& VecScriptInfo);
};

#endif // __GITBNSPRJECTS_BNSKORSERVER_UPDATEFILE_H__