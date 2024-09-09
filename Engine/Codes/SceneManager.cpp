#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"

using namespace Engine;

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

	for (auto& layer : _layers)
	{
		for (auto& object : layer)
		{
			if (object->IsActive())
				object->Update(deltaTime);
		}
	}

	_pScene->Update(deltaTime);

	return 0;
}

int Engine::SceneManager::LateUpdate(const float& deltaTime)
{
	if (nullptr == _pScene || !_isSetUp)
		return GameState::Error;

	for (auto& layer : _layers)
	{
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
}

SceneManager* Engine::SceneManager::Create()
{
	return new SceneManager;
}