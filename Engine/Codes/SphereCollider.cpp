#include "SphereCollider.h"

using namespace Engine;

Engine::SphereCollider::SphereCollider(const wchar_t* name)
	: Collider(name)
{
}

void Engine::SphereCollider::Initialize(const float radius)
{
	_radius = radius;
}

bool Engine::SphereCollider::IsCollide(Collider* other)
{
	return false;
}
