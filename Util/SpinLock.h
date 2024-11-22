#pragma once

void AcquireSpinLock(volatile long* plCount);
void AcquireSpinLockNoYield(volatile long* plCount);
void AcquireSpinLockWithSleep(volatile long* plCount, unsigned long dwSleepTime);
void ReleaseSpinLock(volatile long* plCount);