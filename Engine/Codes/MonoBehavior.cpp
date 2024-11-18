#include "MonoBehavior.h"
#include "GameObject.h"
#include "EventInvoker.h"
#include "GameManager.h"

using namespace Engine;

Engine::MonoBehavior::MonoBehavior(const wchar_t* name)
	: Component(name)
	, _pEventInvoker(GameManager::GetInstance()->GetEventInvoker())
{
}

void Engine::MonoBehavior::Invoke(const wchar_t* name, const std::function<void()>& function, float delay)
{
	_pEventInvoker->Invoke(name, function, delay);
}

void Engine::MonoBehavior::InvokeRepeating(const wchar_t* name, const std::function<void()>& function, float delay, float repeatDelay)
{
	_pEventInvoker->InvokeRepeating(name, function, delay, repeatDelay);
}

void Engine::MonoBehavior::CancelInvoke(const wchar_t* name)
{
	_pEventInvoker->CancelInvoke(name);
}