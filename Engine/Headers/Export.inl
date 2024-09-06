#pragma once

void ClearLayer(const int layerGroup)
{
	Engine::GameManager::GetInstance()->ClearLayer(layerGroup);
}
void ClearObjectList(const int layerGroup)
{
	Engine::GameManager::GetInstance()->ClearObjectList(layerGroup);
}

std::list<Engine::GameObject*>* FindObjectList(const int layerGroup)
{
	return Engine::GameManager::GetInstance()->FindObjectList(layerGroup);
}
Engine::GameObject* FindObject(const int layerGroup, const wchar_t* objectTag)
{
	return Engine::GameManager::GetInstance()->FindObject(layerGroup, objectTag);
}
void AddObjectInLayer(const int layerGroup, Engine::GameObject* pObject)
{
	Engine::GameManager::GetInstance()->AddObjectInLayer(layerGroup, pObject);
}

HWND GetWindow()
{
	return Engine::GameManager::GetInstance()->GetWindow();
}

void ChangeScene(Engine::Scene* pScene)
{
	Engine::GameManager::GetInstance()->ChangeScene(pScene);
}