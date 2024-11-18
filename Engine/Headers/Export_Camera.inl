#pragma once

Engine::Camera* GetCurrentCamera()
{
	return Engine::CameraManager::GetInstance()->GetCurrentCamera();
}

void SetCurrentCamera(const wchar_t* name)
{
	Engine::CameraManager::GetInstance()->SetCurrentCamera(name);
}

void AddCamera(const wchar_t* name, Engine::Camera* pCamera)
{
	Engine::CameraManager::GetInstance()->AddCamera(name, pCamera);
}