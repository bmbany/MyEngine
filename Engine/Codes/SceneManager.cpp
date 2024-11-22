#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"

#include "ThreadPool.h"

#include "InputManager.h"
#include "GraphicManager.h"
#include <Psapi.h>

using namespace Engine;

bool show = true;
size_t g_size = 0;

void Engine::SceneManager::Start()
{
	for (auto& layer : _layers)
	{
		for (auto& object : layer)
			object->Start();
	}
}

void Engine::SceneManager::FixedUpdate()
{
	for (auto& layer : _layers)
	{
		for (auto& object : layer)
		{
			if (object->IsActive())
				object->FixedUpdate();
		}
	}
}

int Engine::SceneManager::Update(const float& deltaTime)
{
	if (nullptr == _pScene || !_isSetUp)
		return GameState::Error;

	int maxSize = -1;

	ThreadPool* pThreadPool = ThreadPool::GetInstance();	
	
	for (auto& layer : _layers)
	{
		// ThreadPool Test
		std::vector<GameObject*> pool;
		size_t size = layer.size();
		pool.reserve(size);

		maxSize = max(maxSize, (int)size);

		for (auto& object : layer)
			pool.push_back(object);		

		size_t maxThreadCount = pThreadPool->GetMaxThreadCount();
		size_t offset = size / maxThreadCount;

		if (1 < offset)
		{
			for (size_t i = 0; i < maxThreadCount; i++)
			{
				size_t begin = i * offset;
				size_t end = begin + offset;

				if (i == (maxThreadCount - 1))
					end += size % pThreadPool->GetMaxThreadCount();

				pThreadPool->AddTask([&pool, begin, end, deltaTime]()
					{
						for (size_t j = begin; j < end; j++)
						{
							pool[j]->Update(deltaTime);
						}
					});
			}

			pThreadPool->Execute();
		}
		else
		{
			for (auto& object : pool)
			{
				if (object->IsActive())
					object->Update(deltaTime);
			}
		}
	}
	
	/*for (auto& layer : _layers)
	{
		maxSize = max(maxSize, (int)layer.size());

		for (auto& object : layer)
		{
			if (object->IsActive())
				object->Update(deltaTime);
		}
	}*/

	g_size = maxSize;

	_pScene->Update(deltaTime);

	return 0;
}

int Engine::SceneManager::LateUpdate(const float& deltaTime)
{
	if (nullptr == _pScene || !_isSetUp)
		return GameState::Error;

	for (auto& layer : _layers)
	{	
		std::erase_if(layer, [](GameObject*& object)
			{
				if (object->IsDead())
				{
					SafeRelease(object);
					return true;
				}

				return false;
			});

		for (auto& object : layer)
		{
			if (object->IsActive())
				object->LateUpdate(deltaTime);
		}
	}

	_pScene->LateUpdate(deltaTime);

	return 0;
}

void Engine::SceneManager::AddRenderGroup()
{
	for (auto& layer : _layers)
	{
		for (auto& object : layer)
		{
			if (object->IsActive())
				object->AddRenderer();
		}
	}
}

void Engine::SceneManager::SetUpLayer(const int layerSize)
{
	_layers.resize(layerSize);	
}

void Engine::SceneManager::ChangeScene(Scene* pScene)
{
	assert(nullptr != pScene);
	
	SafeRelease(_pScene);

	for (auto& layer : _layers)
	{
		for (auto& object : layer)
		{
			if (object->_dontDestroy)
				continue;

			object->SetDead();
		}
	}

	_pScene = pScene;
	_pScene->Initialize();
	_isSetUp = true;
}

void Engine::SceneManager::ClearObjectList(const int layerGroup)
{
	for (auto& layer : _layers)
	{
		for (auto& object : layer)
		{
			object->SetDead();
		}
	}
}

void Engine::SceneManager::ClearLayer(const int layerGroup)
{
	for (auto& object : _layers[layerGroup])
		object->SetDead();
}

std::list<GameObject*>* Engine::SceneManager::FindObjectList(const int layerGroup)
{
	return &_layers[layerGroup];
}

GameObject* Engine::SceneManager::FindObject(const int layerGroup, const wchar_t* objectTag)
{
	if (0 > layerGroup || (int)_layers.size() <= layerGroup)
		return nullptr;

	for (auto& object : _layers[layerGroup])
	{
		if (object->_isDead)
			continue;

		if (*object == objectTag)
			return object;
	}

	return nullptr;
}

void Engine::SceneManager::AddObjectInLayer(const int layerGroup, GameObject* pObject)
{
	assert(!_layers.empty());
	_layers[layerGroup].push_back(pObject);
}

void Engine::SceneManager::Free()
{
	for (auto& layer : _layers)
	{
		for (auto& object : layer)
			SafeRelease(object);

		layer.clear();
	}

	_layers.clear();
	_layers.shrink_to_fit();

	SafeRelease(_pScene);
	
	ThreadPool* pThreadPool = ThreadPool::GetInstance();
	SafeRelease(pThreadPool);
}

SceneManager* Engine::SceneManager::Create()
{
	return new SceneManager;
}