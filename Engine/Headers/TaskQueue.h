#pragma once
#include "Base.h"

namespace Engine
{
	class TaskQueue : public Base, public Singleton<TaskQueue>
	{
		friend class Singleton;
	private:
		explicit TaskQueue() = default;
		virtual ~TaskQueue() = default;

	public:
		void AddTask();

	private:
		// Base을(를) 통해 상속됨
		void Free() override;

	private:
		std::queue<int> _tasks;
	};
}