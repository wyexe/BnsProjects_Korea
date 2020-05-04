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
	// ��ȡPK�߼��������ļ�
	BOOL ReadPkConfig();
	// ����PK���ȵ�Text
	CONST std::wstring& GetPkScheduleText();
	// ����PK�߼��Ľ���
	BOOL SavePkConfig();
	// �����׶�
	BOOL TaskUpgrade_PK();
	// PK�׶�
	BOOL AccountPK_PK();
	// PK5���ŵĽ׶�
	BOOL AfterPK_PK();
	// �л��ʺ��������߼�
	BOOL SwitchAccProc(std::function<BOOL(void)> fnExit);
	// �����
	int ActiveCard();
private:
	DSIABLE_COPY_AND_ASSIGN(CSwAccPkMgr)
private:
	PSHARED_INFO pShare;
	PACCOUNT_INFO_GAME pAccGame;
};



#endif // !__GITBNSPROJECTS_BNSPRJECTS_PK_SWACCPKMGR_H__