#pragma once
#include "Base.h"

namespace Engine
{
	class Scene;
	class GameObject;
	class SceneManager : public Base
	{
	private:
		explicit SceneManager() = default;
		virtual ~SceneManager() = default;

	public:
		void Start();
		void FixedUpdate();
		int Update(const float deltaTime);
		int LateUpdate(const float deltaTime);
		void AddRenderGroup();

		void SetUpLayer(const int layerSize);
		void ChangeScene(Scene* pScene);
		void ClearObjectList(const int layerGroup);
		void ClearLayer(const int layerGroup);
		std::list<GameObject*>* FindObjectList(int layerGroup);
		GameObject* FindObject(const int layerGroup, const wchar_t* objectTag);
		void AddObjectInLayer(const int layerGroup, GameObject* pObject);

	private:
		// Base을(를) 통해 상속됨
		void Free() override;

	public:
		static SceneManager* Create();

	private:
		std::vector<std::list<GameObject*>> _layers;
		Scene*								_pScene = nullptr;
		bool								_isSetUp = false;
	};
}

