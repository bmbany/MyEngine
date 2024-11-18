#include "SphereCollider.h"

Engine::SphereCollider::SphereCollider(const wchar_t* name, const float radius)
	: Collider(name)
	, _radius(radius)
	, radius(_radius)
{
}

bool Engine::SphereCollider::IsCollide(Collider* other)
{
	return false;
}
