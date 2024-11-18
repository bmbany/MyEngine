#pragma once
#include "Collider.h"

namespace Engine
{
	class SphereCollider : public Collider
	{
	public:
		explicit SphereCollider(const wchar_t* name, const float radius);

	public:
		// Collider을(를) 통해 상속됨
		bool IsCollide(Collider* other) override;

	public:
		Property<float> radius;
	private:
		float _radius = 0.f;
	};
}