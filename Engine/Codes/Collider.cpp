#include "Collider.h"
#include "GameObject.h"
#include "Transform.h"
#include "SpriteRenderer.h"

using namespace Engine;

Engine::Collider::Collider(const wchar_t* name)
	: Component(name)
{
}

void Engine::Collider::SetActive(bool isActive)
{
	__super::SetActive(isActive);

	if (false == isActive)
	{
		for (auto& collider : _collidedOthers)
		{
			CollisionInfo info;
			info.itSelf = collider;
			info.other = this;
			collider->EraseOther(this);

			gameObject->OnCollisionExit(info);
			collider->gameObject->OnCollisionExit(info);
		}

		Engine::GameObject* const& pp = nullptr;
		_collidedOthers.clear();
	}
}

void Engine::Collider::InsertOther(Collider* pCollider)
{
	_collidedOthers.insert(pCollider);
}

void Engine::Collider::EraseOther(Collider* pCollider)
{
	_collidedOthers.erase(pCollider);
}

bool Engine::Collider::IsPrevColided(Collider* pCollider)
{
	auto iter = _collidedOthers.find(pCollider);
	
	return iter != _collidedOthers.end();
}

void Engine::Collider::Free()
{
	for (auto& collider : _collidedOthers)
		collider->EraseOther(this);
}