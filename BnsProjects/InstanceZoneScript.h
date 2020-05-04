#ifndef __GITBNSPROJECTS_BNSPROJECTS_SCRIPT_INSTANCEZONESCRIPT_H__
#define __GITBNSPROJECTS_BNSPROJECTS_SCRIPT_INSTANCEZONESCRIPT_H__

#include "GameVariable.h"
#include "BnsScript.h"
class CInstanceZoneScript : public CBnsScript
{
public:
	explicit CInstanceZoneScript() = default;
	~CInstanceZoneScript() = default;

	enum _em_InstanceZoneScript_Exit_Type{ Script_Exit_Type_Level, Script_Exit_Type_Count, Script_Exit_Type_Task };
	BOOL Run(_In_ CONST std::wstring& wsScriptFileName, _In_ CONST _em_InstanceZoneScript_Exit_Type emExitType, _In_ CONST std::wstring& wsExitParm);
private:
	// 读取脚本文件
	BOOL ReadInstanceZoneScript(_In_ CONST std::wstring& wsScriptFileName);

	// 初始化
	BOOL InitInstanceZoneScipt();

	// 
	BOOL Start(_In_ CONST _em_InstanceZoneScript_Exit_Type emExitType, _In_ CONST std::wstring& wsExitParm);
};



#endif
