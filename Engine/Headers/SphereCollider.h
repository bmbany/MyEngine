#pragma once
#include "Collider.h"

namespace Engine
{
	class SphereCollider : public Collider
	{
		REGISTER_COMPONENT(SphereCollider);

	public:
		explicit SphereCollider(const wchar_t* name);

	private:
		virtual ~SphereCollider() = default;

	public:
		void Initialize(const float radius);

	public:
		// Collider��(��) ���� ��ӵ�
		bool IsCollide(Collider* other) override;

	public:
		Property<float> radius{ _radius };

	private:
		float _radius = 0.f;
	};
}