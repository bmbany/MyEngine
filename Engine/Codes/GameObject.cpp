#include "GameObject.h"
#include "TextRenderer.h"
#include "Collider.h"
#include "ICollisionNotify.h"
#include "GameManager.h"

using namespace Engine;

Engine::GameObject::GameObject(const wchar_t* name)
	: _pGameManager(GameManager::GetInstance()),
	transform(_pTransform)
{
	SetName(name);
	_pTransform = AddComponent<Transform>(L"Transform");
	_pSpriteRenderer = AddComponent<SpriteRenderer>(L"SpriteRenderer");
}

Engine::GameObject::~GameObject()
{
	Free();
}

void Engine::GameObject::Start()
{
	if (_isFirstInit) return;

	for (auto& component : _components)
		component->Start();

	_isFirstInit = true;	
}

void Engine::GameObject::FixedUpdate()
{
	if (!_isFirstInit) return;

	for (auto& component : _components)
	{
		if (component->IsActive())
			component->FixedUpdate();
	}
}

int Engine::GameObject::Update(const float& deltaTime)
{
	if (!_isFirstInit) return 0;

	for (auto& component : _components)
	{
		if (component->IsActive())
			component->Update(deltaTime);
	}

	return 0;
}

int Engine::GameObject::LateUpdate(const float& deltaTime)
{
	if (!_isFirstInit) return 0;

	for (auto& component : _components)
	{
		if (component->IsActive())
			component->LateUpdate(deltaTime);
	}

	_pTransform->UpdateTransform();

	return 0;
}

void Engine::GameObject::AddRenderer()
{
	if (!_isFirstInit) return;

	_pGameManager->AddRenderGroup(_renderGroup, this);
}

void Engine::GameObject::Render()
{
	if (!_isFirstInit) return;

	/*if (_isNotAffectCamera)
		_cameraMatrix = D2D1::Matrix3x2F::Identity();*/

	for (auto& component : _components)
	{
		if (component->IsActive())
			component->Render();
	}
}

void Engine::GameObject::OnCollisionEnter(CollisionInfo const& info)
{
	for (auto& component : _registeredCollisionEventComponents)
		component->OnCollisionEnter(info);
}

void Engine::GameObject::OnCollision(CollisionInfo const& info)
{
	for (auto& component : _registeredCollisionEventComponents)
		component->OnCollision(info);
}

void Engine::GameObject::OnCollisionExit(CollisionInfo const& info)
{
	for (auto& component : _registeredCollisionEventComponents)
		component->OnCollisionExit(info);
}

void Engine::GameObject::Free()
{
	for (auto& component : _components)
		SafeRelease(component);
}