#pragma once

#include "Base.h"

namespace Engine
{
    class TimeManager : public Base, public Singleton<TimeManager>
    {
        friend class Singleton;
    private:
        explicit TimeManager();
        virtual ~TimeManager() = default;

    public:
        float GetDeltaTime() { return 0.1f <= _deltaTime ? 0.1f * _slowRate : _deltaTime * _slowRate; }
        float GetGlobalDeltaTime() { return _globalDeltaTime; }
        void SetSlowTime(float rate);
        void SetSlowTime(float rate, float duration);
        void Update();
    private:
        // Base을(를) 통해 상속됨
        void Free() override;

    private:
        LARGE_INTEGER _currTime;
        LARGE_INTEGER _oldTime;
        float _deltaTime = 0.f;
        float _globalDeltaTime = 0.f;
        float _elapsed = 0.f;
        float _slowRate = 1.f;
        float _duration = 0.f;
        float _frequency = 0.f;
    };
}
