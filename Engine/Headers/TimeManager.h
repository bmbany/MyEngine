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
        NOCOPY(TimeManager)
    public:
        float GetSumTime() { return _elapsed; }
        void SetSumTime(float time) { _elapsed = time; }

        float GetDeltaTime() { return 0.1f <= _deltaTime ? 0.1f * _slowRate : _deltaTime * _slowRate; }

        void SetSlowTime(float rate);
        void SetSlowTime(float rate, float duration);
        float GetGlobalDeltaTime() { return 0.1f <= _globalDeltaTime ? 0.1f : _globalDeltaTime; }
        void Update();

    private:
        // Base을(를) 통해 상속됨
        void Free() override;

    private:
        LARGE_INTEGER _currTime;
        LARGE_INTEGER _oldTime;
        float _deltaTime;
        float _globalDeltaTime;
        float _elapsed;
        float _slowRate = 1.f;
        float _duration = 0.f;
        float _frequency;
    };
}
