#ifndef __GITBNSPROJECTS_BNSKORSERVER_OTP_TOTPCOUNTER_H__
#define __GITBNSPROJECTS_BNSKORSERVER_OTP_TOTPCOUNTER_H__

#include <MyTools/CLPublic.h>

class CTotpCounter
{
public:
	CTotpCounter(_In_ LONG TimeStep = 0, _In_ LONG StartTime = 0);
	~CTotpCounter() = default;

	//
	LONG GetTimeStep() CONST _NOEXCEPT;

	//
	LONG GetStartTime() CONST _NOEXCEPT;

	// 
	LONGLONG GetValueAtTime(LONGLONG Time_) CONST _NOEXCEPT;

	//
	LONG GetValueStartTime(LONG Value_) CONST _NOEXCEPT;
private:
	LONG _TimeStep;
	LONG _StartTime;
};

#endif // !__GITBNSPROJECTS_BNSKORSERVER_OTP_TOTPCOUNTER_H__
