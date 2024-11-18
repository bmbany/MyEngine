#include "ThreadPool.h"

using namespace Engine;

ThreadPool::ThreadPool()
{
	SYSTEM_INFO systemInfo{};
	GetSystemInfo(&systemInfo);
	_maxThreadCount = systemInfo.dwNumberOfProcessors;
	
	_maxThreadCount = 5;
	_capacity = _maxThreadCount;
	_taskQueue = new std::function<void()>[_capacity];

	_threads = new Thread[_maxThreadCount];
	_completeEventList = new HANDLE[_maxThreadCount];

	auto ThreadFunc = [](void* pArg) -> UINT
		{  
			Thread thread = *(Thread*)pArg;
			thread.pOwner->WorkerThread((Thread*)pArg);
			
			return 0;
		};

	for (DWORD i = 0; i < _maxThreadCount; i++)
	{
		_completeEventList[i] = CreateEvent(nullptr, FALSE, FALSE, nullptr);

		for (DWORD j = 0; j < ThreadEvent::End; j++)
		{
			_threads[i].eventList[j] = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		}

		UINT threadID = 0;
		_threads[i].thread = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunc, _threads + i, 0, &threadID);
		_threads[i].index = i;
		_threads[i].pOwner = this;
	}
}

void ThreadPool::AddTask(const std::function<void()>& task)
{
	if (_front >= _capacity)
	{
		int newSize = _capacity * 2;
		std::function<void()>* temp = new std::function<void()>[newSize];

		memcpy(temp, _taskQueue, sizeof(std::function<void()>) * _capacity);

		delete[] _taskQueue;
		_taskQueue = temp;
		_capacity = newSize;
	}

	_taskQueue[_front++] = task;
	_size++;

	//AcquireSpinLock(&_spinLock);
	//_tasks.push(task);
	//ReleaseSpinLock(&_spinLock);
}

void ThreadPool::Execute()
{
	_front = 0;

	for (DWORD i = 0; i < _maxThreadCount; i++)
	{
		SetEvent(_threads[i].eventList[ThreadEvent::Process]);
	}

	WaitForMultipleObjects(_maxThreadCount, _completeEventList, TRUE, INFINITE);

	_front = 0;
}

void ThreadPool::WorkerThread(const Thread* thread)
{
	bool isLoop = true;
	while (isLoop)
	{
		DWORD eventIndex = WaitForMultipleObjects(ThreadEvent::End, thread->eventList, FALSE, INFINITE);

		switch (eventIndex)
		{
		case ThreadEvent::Process:
			while (true)
			{
				std::function<void()>* task = nullptr;

				AcquireSpinLock(&_spinLock);
				
				if (0 >= _size)
				{
					ReleaseSpinLock(&_spinLock);
					break;
				}

				task = &_taskQueue[_front];
				PopTask();

				ReleaseSpinLock(&_spinLock);
				
				(*task)();

				/*std::function<void()> task = nullptr;
				AcquireSpinLock(&_spinLock);
				if (!_tasks.empty())
				{
					task = _tasks.front();
					_tasks.pop();
				}
				else
				{
					ReleaseSpinLock(&_spinLock);
					break;
				}
				ReleaseSpinLock(&_spinLock);
				task();*/
			}

			SetEvent(_completeEventList[thread->index]);
			break;

		case ThreadEvent::End:
			isLoop = false;
			break;
		}
	}
}

void ThreadPool::PopTask()
{
	if (0 >= _size)
		return;

	_front++;
	_size--;
}

void Engine::ThreadPool::Free()
{
	for (DWORD i = 0; i < _maxThreadCount; i++)
	{
		SetEvent(_threads[i].eventList[ThreadEvent::End]);

		for (DWORD j = 0; j < ThreadEvent::End; j++)
		{
			CloseHandle(_threads[i].eventList[j]);
		}

		CloseHandle(_threads[i].thread);
	}

	delete[] _completeEventList;
	delete[] _threads;
	delete[] _taskQueue;
}
