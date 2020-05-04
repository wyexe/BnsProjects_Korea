#ifndef __GITBNSPROJECTS_CONSOLEDLL_FUNCTION_OPTIMIZEGAME_H__
#define __GITBNSPROJECTS_CONSOLEDLL_FUNCTION_OPTIMIZEGAME_H__

#include <MyTools/ToolsPublic.h>
class COptimizeGame
{
public:
	COptimizeGame() = default;
	~COptimizeGame() = default;

	//
	BOOL Open(_In_ CONST std::wstring& wsGamePath) CONST;

	//
	BOOL Close(_In_ CONST std::wstring& wsGamePath) CONST;
private:
	//
	BOOL ActionForFile(_In_ CONST std::wstring& wsGamePath, _In_ BOOL bOptimize) CONST;

	// 
	CONST std::vector<std::wstring>& GetVecFile() CONST;
};



#endif // !__GITBNSPROJECTS_CONSOLEDLL_FUNCTION_OPTIMIZEGAME_H__
