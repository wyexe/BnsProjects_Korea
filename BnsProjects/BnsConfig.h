#ifndef __GITBNSPROJECTS_BNSPROJECTS_CONFIG_BNSCONFIG_H__
#define __GITBNSPROJECTS_BNSPROJECTS_CONFIG_BNSCONFIG_H__

#include "GameStruct.h"
#include <MyTools/ClassInstance.h>

class CBnsConfig : public MyTools::CClassInstance<CBnsConfig>
{
public:

	typedef struct _AccountConfig
	{
		std::wstring wsAccountName;
		std::wstring wsAccountPass;
		_AccountConfig()
		{
			wsAccountName = L"";
			wsAccountPass = L"";
		}
	}AccountConfig;
	 
	struct BnsConfigContent
	{
		std::wstring wsConfigName;
		std::wstring wsConfigValue;
	};

public:
	CBnsConfig();
	~CBnsConfig() = default;

	// 设置配置文件的路径
	VOID SetConfigPath(_In_ PSHARED_INFO pShare, _In_ PACCOUNT_INFO_GAME pAccGame, _In_ CONST std::wstring& wsPath);

	// 清空这个帐号的配置
	BOOL ClearAccountConfig(_In_ CONST std::wstring& wsAccountName) CONST;

	// 写入帐号日志
	VOID WriteAccountLog(_In_ CONST std::wstring& wsContent);
	VOID WriteAccountLog(_In_ PACCOUNT_INFO_GAME pAccGame, _In_ CONST std::wstring& wsContent);

	// 获取瞬移文件
	std::shared_ptr<CHAR> GetTelprotFile(_In_ CONST std::wstring& wsPath, _In_ CONST ULONG& ulFileLen) CONST;

	BOOL PrintDumpInfo(_In_ CONST std::wstring& wsPath) CONST;

	// 保存当前时间到Server
	BOOL SaveLocalTimeConfigToServer(_In_ CONST std::wstring& wsPlayerName, _In_ CONST std::wstring& wsConfigName) CONST;

	// 获取Server的单个配置文件内容
	CONST std::wstring GetSingleConfigValueByServer(_In_ CONST std::wstring& wsPlayerName, _In_ CONST std::wstring& wsConfigName) CONST;

	// 设置Server的单个配置文件内容
	VOID SetSingleConfigValueByServer(_In_ CONST BnsConfigContent& ConfigContent_) CONST;

	// 保存截图
	BOOL SaveScreenshot(_In_ PACCOUNT_INFO_GAME pAccGame, _Out_ std::wstring& wsPicPath);

	//
	VOID SetConsoleLogPtr(_In_ std::function<VOID(em_ConsoleLog, CONST std::wstring&, CONST std::wstring&)> ConsoleLogPtr);
private:
	// 分割内容
	BOOL DivisionConfig(_Out_opt_ std::vector<std::wstring>& vlst, _In_ CONST std::wstring& wsPath)  CONST;

	// 读取配置文件
	BOOL ReadConfig(_In_ CONST std::wstring& wsPath, _Out_opt_ std::wstring& wsContent) CONST;

	// 保存配置文件
	BOOL WriteConfig(_In_ CONST std::wstring& wsPath, _In_ CONST std::wstring& wsContent) CONST;

	// 获取所有的帐号文件
	BOOL GetAccountConfigList(_Out_opt_ std::vector<AccountConfig>& vlst)  CONST;

	// 保存帐号文件
	BOOL SaveAccountConfigList(_In_ CONST std::vector<AccountConfig>& vlst) CONST;

	// 枚举所有的窗口+进程
	BOOL PrintProcInfo_By_Dump(_Out_opt_ std::wstring& wsText) CONST;

	// 输出ShareInfo
	BOOL PrintShareInfo(_Out_opt_ std::wstring& wsText) CONST;

	//
	VOID PrintToConsoleLog(_In_ em_ConsoleLog emType, CONST std::wstring& wsAccountName, CONST std::wstring& wsContent);

	DSIABLE_COPY_AND_ASSIGN(CBnsConfig)
private:
	std::wstring wsConfigPath;
	PSHARED_INFO m_pShare;
	PACCOUNT_INFO_GAME m_pAccGame;
	std::function<VOID(em_ConsoleLog, CONST std::wstring&, CONST std::wstring&)> _ConsoleLogPtr;
};



#endif // __GITBNSPROJECTS_BNSPROJECTS_CONFIG_BNSCONFIG_H__