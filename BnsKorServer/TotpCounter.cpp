#include "TotpCounter.h"

CTotpCounter::CTotpCounter(_In_ LONG TimeStep /*= 0*/, _In_ LONG StartTime /*= 0*/) : _TimeStep(TimeStep), _StartTime(StartTime)
{

}

LONG CTotpCounter::GetTimeStep() CONST _NOEXCEPT
{
	return _TimeStep;
}

LONG CTotpCounter::GetStartTime() CONST _NOEXCEPT
{
	return _StartTime;
}

LONGLONG CTotpCounter::GetValueAtTime(LONGLONG Time_) CONST _NOEXCEPT
{
	LONGLONG timeSinceStartTime = Time_ - static_cast<LONGLONG>(_StartTime);
	return timeSinceStartTime >= 0 ? timeSinceStartTime / _TimeStep : (timeSinceStartTime - _TimeStep - 1) / _TimeStep;
}

LONG CTotpCounter::GetValueStartTime(LONG Value_) CONST _NOEXCEPT
{
	return _StartTime + Value_ * _TimeStep;
}
