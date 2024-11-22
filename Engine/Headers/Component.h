#pragma once
#include "Base.h"

namespace Engine
{
	class GameObject;
	class Transform;
	class Component abstract : public Base
	{
		friend class GameObject;
	protected:
		explicit Component(const wchar_t* name);
		virtual ~Component() = default;

	public:
		Property<GameObject*, READ_ONLY> gameObject	{ _pOwner };
		Property<Transform*, READ_ONLY> transform	{ _pTransform };

	public:
		virtual void Awake() {}
		virtual void Start() {}
		virtual void FixedUpdate() {}
		virtual void Update(const float& deltaTime) {};
		virtual void LateUpdate(const float& deltaTime) {};
		virtual void Render() {}

	public:
		template <typename T>
		T* AddComponent(const wchar_t* name);
	
		template<typename T>
		T* GetComponent();

		template<typename T>
		T* GetComponent(const wchar_t* name);

		// Base을(를) 통해 상속됨
		virtual void Free() = 0;

	private:
		GameObject* _pOwner		{ nullptr };
		Transform* _pTransform	{ nullptr };
	};
}

#include "GameObject.h"
template <typename T>
inline T* Engine::Component::AddComponent(const wchar_t* name)
{
	return _pOwner->AddComponent<T>(name);
}

template<typename T>
inline T* Engine::Component::GetComponent()
{
	return _pOwner->GetComponent<T>();
}

template<typename T>
inline T* Engine::Component::GetComponent(const wchar_t* name)
{	
	return _pOwner->GetComponent<T>(name);
}