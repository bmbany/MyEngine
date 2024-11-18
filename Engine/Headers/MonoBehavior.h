#pragma once
#include "Component.h"
#include "ICollisionNotify.h"

namespace Engine
{
	class Transform;
	class EventInvoker;
	class MonoBehavior abstract : public Component, public ICollisionNotify
	{
		friend class GameObject;
	protected:
		explicit MonoBehavior(const wchar_t* name);
		virtual ~MonoBehavior() = default;

	public:
		virtual void Awake() = 0;
		virtual void Start() = 0;
		virtual void FixedUpdate() {}
		virtual void Update(const float& deltaTime) = 0;
		virtual void LateUpdate(const float& deltaTime) = 0;
		virtual void Render() {}

	public:
		// ICollisionNotify을(를) 통해 상속됨
		virtual void OnCollisionEnter(CollisionInfo const& info) {};
		virtual void OnCollision(CollisionInfo const& info) {};
		virtual void OnCollisionExit(CollisionInfo const& info) {};

		// EventInvoker
		void Invoke(const wchar_t* name, const std::function<void()>& function, float delay);
		void InvokeRepeating(const wchar_t* name, const std::function<void()>& function, float delay, float repeatDelay);
		void CancelInvoke(const wchar_t* name);

	public:
		// Component을(를) 통해 상속됨
		virtual void Free() = 0;

	private:
		EventInvoker* _pEventInvoker{ nullptr };
	};
}