#ifndef __GITBNSPROJECTS_BNSPRJECTS_PK_SWACCPKMGR_H__
#define __GITBNSPROJECTS_BNSPRJECTS_PK_SWACCPKMGR_H__

#include "GameVariable.h"
#include "BnsGameFun.h"

#define MINTIME_DIANKA		150

class CSwAccPkMgr : public CBnsGameFun
{
public:
	explicit CSwAccPkMgr(_In_ PSHARED_INFO PSHARED_INFO_, _In_ PACCOUNT_INFO_GAME PACCOUNT_INFO_GAME_);
	~CSwAccPkMgr();

	BOOL Run(std::function<BOOL(void)> fnExit);
private:
	// 读取PK逻辑的配置文件
	BOOL ReadPkConfig();
	// 返回PK进度的Text
	CONST std::wstring& GetPkScheduleText();
	// 保存PK逻辑的进度
	BOOL SavePkConfig();
	// 练级阶段
	BOOL TaskUpgrade_PK();
	// PK阶段
	BOOL AccountPK_PK();
	// PK5个号的阶段
	BOOL AfterPK_PK();
	// 切换帐号练级主逻辑
	BOOL SwitchAccProc(std::function<BOOL(void)> fnExit);
	// 激活卡号
	int ActiveCard();
private:
	DSIABLE_COPY_AND_ASSIGN(CSwAccPkMgr)
private:
	PSHARED_INFO pShare;
	PACCOUNT_INFO_GAME pAccGame;
};



#endif // !__GITBNSPROJECTS_BNSPRJECTS_PK_SWACCPKMGR_H__