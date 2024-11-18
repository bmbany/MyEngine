#pragma once
#include "Base.h"

namespace Engine
{
	enum ThreadEvent { Process, Destroy, End };

	class ThreadPool;
	struct Thread
	{
		HANDLE eventList[ThreadEvent::End];
		HANDLE thread;
		ThreadPool* pOwner = nullptr;
		DWORD index = 0;
	};

	class ThreadPool : public Base, public Singleton<ThreadPool>
	{
		friend class Singleton;
	private:
		explicit ThreadPool();
		virtual ~ThreadPool() = default;

	public:
		DWORD GetMaxThreadCount() const { return _maxThreadCount; }
		void AddTask(const std::function<void()>& task);
		void Execute();

	private:
		void WorkerThread(const Thread* thread);
		void PopTask();

	private:
		// Base을(를) 통해 상속됨
		void Free() override;

	private:
		using TaskQueue = std::queue<std::function<void()>>;
		std::function<void()>*	_taskQueue;
		LONG					_front{ 0 };
		LONG 					_size{ 0 };
		LONG					_capacity{ 0 };
		TaskQueue				_tasks;
		Thread*					_threads{ nullptr };
		HANDLE*					_completeEventList{ nullptr };
		DWORD					_maxThreadCount{};
		LONG volatile			_spinLock{};		
	};
}