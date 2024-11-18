#pragma once

#include "Base.h"
#include "Texture.h"
#include "ICollisionNotify.h"

struct CollisionInfo;

namespace Engine
{	
	class Transform;
	class Collider;
	class Component;
	class MonoBehavior;
	class GameManager;	
	class GameObject : public Base
	{
		friend class SceneManager;
		friend class Renderer;
	public:
		explicit GameObject(const wchar_t* name);
	private:
		virtual ~GameObject();

	public:
		Property<Transform*, READ_ONLY> transform{ _pTransform };

	public:
		inline std::vector<Collider*>& GetColliders() { return _colliders; }
		inline bool IsDead() const { return _isDead; }

		inline void SetDead() { _isDead = true; }
		inline void SetDontDestroyObject(bool isActive) { _dontDestroy = isActive; }
		inline void SetRenderGroup(int renderGroup) { _renderGroup = renderGroup; }
		inline void SetNotAffectCamera(bool isActive) { _isNotAffectCamera = isActive; }

		template<typename T>
		T* GetComponent(const wchar_t* name)
		{
			return static_cast<T*>(_componentData[name]);
		}

		template<typename T>
		T* GetComponent()
		{
			for (auto& component : _components)
			{
				if (typeid(*component) == typeid(T))
					return static_cast<T*>(component);
			}

			return nullptr;
		}

		template<typename T, typename... Args>
			requires (std::is_base_of<Component, T>::value) &&
			requires (Args&&... args) { new T(std::forward<Args>(args)...); }
		T* AddComponent(Args&&... args)
		{
			T* pComponent = new T(std::forward<Args>(args)...);
			pComponent->_pOwner = this;
			pComponent->_pTransform = _pTransform;
			pComponent->Awake();
			
			_componentData[pComponent->GetName()] = pComponent;
			_components.push_back(pComponent);

			if constexpr (std::is_base_of_v<Collider, T>)
				_colliders.push_back(pComponent);
			
			if constexpr (std::is_base_of_v<ICollisionNotify, T>)
				_collisionEventComponents.push_back(pComponent);

			return pComponent;
		}

		void OnCollisionEnter(CollisionInfo const& info);
		void OnCollision(CollisionInfo const& info);
		void OnCollisionExit(CollisionInfo const& info);

	private:
		void Start();
		void FixedUpdate();
		int Update(const float& deltaTime);
		int LateUpdate(const float& deltaTime);
		void AddRenderer();
		void Render();

	private:
		void Free() override;
	
	private:
		using ComponentData = std::unordered_map<std::wstring_view, Component*>;
		ComponentData					_componentData;
		std::vector<Component*>			_components;
		std::vector<Collider*>			_colliders;
		std::vector<ICollisionNotify*>	_collisionEventComponents;
		GameManager*					_pGameManager{ nullptr };
		Transform*						_pTransform{ nullptr };

		int								_renderGroup{ 0 };
		bool							_isDead{ false };
		bool							_dontDestroy{ false };
		bool							_isFirstInit{ false };
		bool							_isNotAffectCamera{ false };
	};	
}

#include "Transform.h"