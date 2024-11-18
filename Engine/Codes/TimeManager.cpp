#include "TimeManager.h"

using namespace Engine;

Engine::TimeManager::TimeManager()
{
    LARGE_INTEGER frequency{};

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&_oldTime);
    QueryPerformanceCounter(&_currTime);

    _frequency = 1.f / float(frequency.QuadPart);
}

void Engine::TimeManager::SetSlowTime(float rate)
{
    _slowRate = rate;
    _duration = 0.f;
}

void Engine::TimeManager::SetSlowTime(float rate, float duration)
{
    _slowRate = rate;
    _duration = duration;
    _elapsed = 0.f;
}

void TimeManager::Update()
{
    _oldTime = _currTime;
    QueryPerformanceCounter(&_currTime);

    _globalDeltaTime = (_currTime.QuadPart - _oldTime.QuadPart) * _frequency ;
    _deltaTime = _globalDeltaTime;

    if (0.f < _duration)
    {
        _elapsed += _globalDeltaTime;

        if (_elapsed >= _duration)
        {
            _slowRate = 1.f;
            _elapsed = 0.f;
            _duration = 0.f;
        }
    }
}

void TimeManager::Free()
{
}
