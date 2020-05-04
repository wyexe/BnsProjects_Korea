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
	// 每分钟输出一次
	VOID PrintPlayerMsgToConsole();

	// 检查坐标超时
	BOOL CheckUnMove();

	// 检查任务超时
	BOOL CheckTaskTimeout();

private:
	ULONGLONG		_ulMoveTick;
	ULONGLONG		_ulTaskTick;
	UINT			_uTimeTick;
	Point			_LastPersonPoint;
	TaskInfoParm	_LastTask;
};



#endif // !__GITBNSPROJECTS_BNSPROJECTS_GAME_SOMEFUN_TIMEOUTCHECK_TIMEOUTCHECK_H__
