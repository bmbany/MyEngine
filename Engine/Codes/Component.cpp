#include "Component.h"

Engine::Component::Component(const wchar_t* name)
	: gameObject(_pOwner),
	transform(_pTransform)
{
	SetName(name);
}
