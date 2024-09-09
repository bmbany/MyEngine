#pragma once
#include "Collider.h"

namespace Engine
{
	class SphereCollider : public Collider
	{
	public:
		explicit SphereCollider(const wchar_t* name, const float radius);

		// Collider��(��) ���� ��ӵ�
		bool IsCollide(Collider* other) override;
	};
}