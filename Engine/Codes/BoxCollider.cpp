#include "BoxCollider.h"

using namespace Engine;

Engine::BoxCollider::BoxCollider(const wchar_t* name, const Vector2& scale, const Vector2& offset)
	: Collider(name),
	_scale(scale),
	_offset(offset),
	scale(_scale)
{
}

bool Engine::BoxCollider::IsCollide(Collider* other)
{
	scale = Vector2(1.f, 1.f);	
	BoxCollider* otherBoxCollider = static_cast<BoxCollider*>(other);
	
	Vector2 radiusSum = (_scale + otherBoxCollider->_scale) * 0.5f;
	Vector2 distance = XMVectorAbs(otherBoxCollider->transform->position - transform->position);

	if (radiusSum.x >= distance.x && radiusSum.y >= distance.y)
		return true;

	return false;
}
