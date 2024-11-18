#include "CameraManager.h"
#include "Camera.h"

using namespace Engine;

void Engine::CameraManager::FixedUpdate()
{
	_pCurrentCamera->FixedUpdate();
}

void Engine::CameraManager::Update(const float& deltaTime)
{
	_pCurrentCamera->Update(deltaTime);
}

void Engine::CameraManager::LateUpdate(const float& deltaTime)
{
	_pCurrentCamera->LateUpdate(deltaTime);
}

void Engine::CameraManager::SetCurrentCamera(const wchar_t* name)
{
	auto iter = _cameras.find(name);
	assert(iter != _cameras.end());

	if (iter != _cameras.end())
	{
		_pCurrentCamera = _cameras[name];
	}
}

void Engine::CameraManager::AddCamera(const wchar_t* name, Camera* pCamera)
{
	auto iter = _cameras.find(name);
	assert(iter == _cameras.end());

	if (iter == _cameras.end())
	{
		_cameras[name] = pCamera;
	}
}

void Engine::CameraManager::Free()
{
	for (auto& camera : _cameras)
		SafeRelease(camera.second);
}
