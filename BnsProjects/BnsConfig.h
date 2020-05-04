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

	// ���������ļ���·��
	VOID SetConfigPath(_In_ PSHARED_INFO pShare, _In_ PACCOUNT_INFO_GAME pAccGame, _In_ CONST std::wstring& wsPath);

	// �������ʺŵ�����
	BOOL ClearAccountConfig(_In_ CONST std::wstring& wsAccountName) CONST;

	// д���ʺ���־
	VOID WriteAccountLog(_In_ CONST std::wstring& wsContent);
	VOID WriteAccountLog(_In_ PACCOUNT_INFO_GAME pAccGame, _In_ CONST std::wstring& wsContent);

	// ��ȡ˲���ļ�
	std::shared_ptr<CHAR> GetTelprotFile(_In_ CONST std::wstring& wsPath, _In_ CONST ULONG& ulFileLen) CONST;

	BOOL PrintDumpInfo(_In_ CONST std::wstring& wsPath) CONST;

	// ���浱ǰʱ�䵽Server
	BOOL SaveLocalTimeConfigToServer(_In_ CONST std::wstring& wsPlayerName, _In_ CONST std::wstring& wsConfigName) CONST;

	// ��ȡServer�ĵ��������ļ�����
	CONST std::wstring GetSingleConfigValueByServer(_In_ CONST std::wstring& wsPlayerName, _In_ CONST std::wstring& wsConfigName) CONST;

	// ����Server�ĵ��������ļ�����
	VOID SetSingleConfigValueByServer(_In_ CONST BnsConfigContent& ConfigContent_) CONST;

	// �����ͼ
	BOOL SaveScreenshot(_In_ PACCOUNT_INFO_GAME pAccGame, _Out_ std::wstring& wsPicPath);

	//
	VOID SetConsoleLogPtr(_In_ std::function<VOID(em_ConsoleLog, CONST std::wstring&, CONST std::wstring&)> ConsoleLogPtr);
private:
	// �ָ�����
	BOOL DivisionConfig(_Out_opt_ std::vector<std::wstring>& vlst, _In_ CONST std::wstring& wsPath)  CONST;

	// ��ȡ�����ļ�
	BOOL ReadConfig(_In_ CONST std::wstring& wsPath, _Out_opt_ std::wstring& wsContent) CONST;

	// ���������ļ�
	BOOL WriteConfig(_In_ CONST std::wstring& wsPath, _In_ CONST std::wstring& wsContent) CONST;

	// ��ȡ���е��ʺ��ļ�
	BOOL GetAccountConfigList(_Out_opt_ std::vector<AccountConfig>& vlst)  CONST;

	// �����ʺ��ļ�
	BOOL SaveAccountConfigList(_In_ CONST std::vector<AccountConfig>& vlst) CONST;

	// ö�����еĴ���+����
	BOOL PrintProcInfo_By_Dump(_Out_opt_ std::wstring& wsText) CONST;

	// ���ShareInfo
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