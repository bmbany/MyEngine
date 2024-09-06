#include "EventInvoker.h"

using namespace Engine;

void Engine::EventInvoker::Update(const float& deltaTime)
{
	auto iter_end = _events.end();

	for (auto iter = _events.begin(); iter != iter_end;)
	{
		iter->elpased += deltaTime;

		if (iter->elpased >= iter->firstDelay)
		{
			iter->function();

			if (!iter->isRepeat)
			{
				iter = _events.erase(iter);
				iter_end = _events.end();
				continue;
			}

			iter->firstDelay = iter->repeatDelay;
			iter->elpased = 0.f;
		}

		++iter;
	}
}

void Engine::EventInvoker::Invoke(const wchar_t* name, const std::function<void()>& function, float delay)
{
	_events.emplace_back(function, name, delay);
}

void Engine::EventInvoker::InvokeRepeating(const wchar_t* name, const std::function<void()>& function, float delay, float repeatDelay)
{
	_events.emplace_back(function, name, delay, repeatDelay, true);
}

void Engine::EventInvoker::CancelInvoke(const wchar_t* name)
{
	_events.remove_if([name](const Event& event) { return event.name == name; });
}

void Engine::EventInvoker::Free()
{
}

EventInvoker* Engine::EventInvoker::Create()
{
	return new EventInvoker;
}
