#pragma once
#include "Component.h"
#include "ICollisionNotify.h"

namespace Engine
{
	class MonoBehavior abstract : public Component, public ICollisionNotify
	{
	protected:
		explicit MonoBehavior(const wchar_t* name);
		virtual ~MonoBehavior() = default;

	public:
		virtual void Awake() = 0;
		virtual void Start() = 0;
		virtual void FixedUpdate() {}
		virtual void Update(const float& deltaTime) = 0;
		virtual void LateUpdate(const float& deltaTime) = 0;

	public:
		// ICollisionNotify��(��) ���� ��ӵ�
		virtual void OnCollisionEnter(CollisionInfo const& info) {};
		virtual void OnCollision(CollisionInfo const& info) {};
		virtual void OnCollisionExit(CollisionInfo const& info) {};

	public:
		// Component��(��) ���� ��ӵ�
		void Free() override;
		
	};
}