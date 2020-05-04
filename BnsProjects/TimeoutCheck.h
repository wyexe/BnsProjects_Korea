#ifndef __GITBNSPROJECTS_BNSPROJECTS_GAME_SOMEFUN_TIMEOUTCHECK_TIMEOUTCHECK_H__
#define __GITBNSPROJECTS_BNSPROJECTS_GAME_SOMEFUN_TIMEOUTCHECK_TIMEOUTCHECK_H__

#include "GameVariable.h"

class CTimeoutCheck
{
public:
	CTimeoutCheck();
	~CTimeoutCheck() = default;

	BOOL Check();
private:
	// ÿ�������һ��
	VOID PrintPlayerMsgToConsole();

	// ������곬ʱ
	BOOL CheckUnMove();

	// �������ʱ
	BOOL CheckTaskTimeout();

private:
	ULONGLONG		_ulMoveTick;
	ULONGLONG		_ulTaskTick;
	UINT			_uTimeTick;
	Point			_LastPersonPoint;
	TaskInfoParm	_LastTask;
};



#endif // !__GITBNSPROJECTS_BNSPROJECTS_GAME_SOMEFUN_TIMEOUTCHECK_TIMEOUTCHECK_H__
