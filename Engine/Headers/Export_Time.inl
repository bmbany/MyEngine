#pragma once

void SetSlowTime(float rate)
{
	Engine::TimeManager::GetInstance()->SetSlowTime(rate);
}

void SetSlowTime(float rate, float duration)
{
	Engine::TimeManager::GetInstance()->SetSlowTime(rate, duration);
}

float GetGlobalDeltaTime()
{
	return Engine::TimeManager::GetInstance()->GetGlobalDeltaTime();
}

float GetDeltaTime() 
{ 
	return Engine::TimeManager::GetInstance()->GetDeltaTime();
}

