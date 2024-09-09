#pragma once

namespace Engine
{	
	class Collider;

	struct CollisionInfo
	{
		Collider* other = nullptr;
		Collider* itSelf = nullptr;
	};

	class ICollisionNotify
	{
	public:
		virtual void OnCollisionEnter(CollisionInfo const& info) = 0;
		virtual void OnCollision(CollisionInfo const& info) = 0;
		virtual void OnCollisionExit(CollisionInfo const& info) = 0;
	};
}