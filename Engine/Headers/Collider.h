#pragma once
#include "Component.h"

namespace Engine
{
	class Collider abstract : public Component
	{
	protected:
		explicit Collider(const wchar_t* name);
		virtual ~Collider() = default;

	public:
		void SetActive(bool isActive) override;
		void InsertOther(Collider* pCollider);
		void EraseOther(Collider* pCollider);
		bool IsPrevColided(Collider* pCollider);
		virtual bool IsCollide(Collider* other) = 0;

	private:
		// Component을(를) 통해 상속됨
		void Free() override;

	protected:
		std::unordered_set<Collider*> _collidedOthers;
	};
}
