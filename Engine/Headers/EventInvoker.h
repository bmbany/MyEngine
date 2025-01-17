#pragma once
#include "Base.h"

namespace Engine
{
	class EventInvoker final : public Base
	{
	private:
		struct Event
		{
			std::function<void()> function;
			std::wstring name;
			float firstDelay = 0.f;
			float repeatDelay = 0.f;
			float elpased = 0.f;
			bool isRepeat = false;
		};
	private:
		explicit EventInvoker() = default;
		virtual ~EventInvoker() = default;

	public:
		void Update(const float deltaTime);
		void Invoke(const wchar_t* name, const std::function<void()>& function, float delay);
		void InvokeRepeating(const wchar_t* name, const std::function<void()>& function, float delay, float repeatDelay);
		void CancelInvoke(const wchar_t* name);

	private:
		// Base을(를) 통해 상속됨
		void Free() override;

	public:
		static EventInvoker* Create();

	private:
		std::list<Event> _events;
	};
}
