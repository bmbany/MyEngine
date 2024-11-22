#pragma once
#include "Base.h"

namespace Engine
{
	class EventDispatcher : public Base, public Singleton<EventDispatcher>
	{
	private:
		explicit EventDispatcher() = default;
		virtual ~EventDispatcher() = default;

	public:

	private:
		void Free() override;

	private:

	};
}