#include "SpinLock.h"
#include <Windows.h>

void AcquireSpinLock(volatile LONG* plCount)
{
	LONG	lOldCount;

lb_try:
	lOldCount = _InterlockedCompareExchange(plCount, 1, 0);

	if (lOldCount)
	{
		for (DWORD i = 0; i < 1024; i++)
		{
			YieldProcessor();
		}
		goto lb_try;
	}
}

void AcquireSpinLockNoYield(volatile LONG* plCount)
{
	LONG	lOldCount;

lb_try:
	lOldCount = _InterlockedCompareExchange(plCount, 1, 0);

	if (lOldCount)
	{
		goto lb_try;
	}
}

void AcquireSpinLockWithSleep(volatile LONG* plCount, DWORD dwSleepTime)
{
	LONG	lOldCount;

lb_try:
	lOldCount = _InterlockedCompareExchange(plCount, 1, 0);

	if (lOldCount)
	{
		Sleep(dwSleepTime);
		goto lb_try;
	}

}
void ReleaseSpinLock(volatile LONG* plCount)
{
	_InterlockedDecrement(plCount);
}