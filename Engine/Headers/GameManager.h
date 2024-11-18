#pragma once

#include "Base.h"

namespace Engine
{
	class Layer;
	class Scene;
	class WinApp;

	// Util
	class GameObject;
	class Renderer;
	class EventInvoker;

	// Manager
	class CameraManager;
	class SceneManager;
	class InputManager;
	class TimeManager;
	class SoundManager;

	class GameManager : public Base, public Singleton<GameManager>
	{
		friend class Singleton;
	public:
		struct GameDefaultSetting
		{
			HINSTANCE hInstance{};
			const wchar_t* appName{};
			int renderGroupSize{};
			int maxSoundGroup{};
			int layerSize{};
			int width{};
			int height{};
			int fiexedCount = 50;
			bool isFullScreen = false;
		};
	private:
		explicit GameManager();
		virtual ~GameManager() = default;

	public:
		void Run();
		bool Initialize(const GameDefaultSetting& info);

		// WinApp
		HWND GetWindow() const;

		// TimeMgr
		void SetSlowTime(float rate);

		// SceneMgr
		void ChangeScene(Scene* pScene);
		void ClearObjectList(const int layerGroup);
		void ClearLayer(const int layerGroup);
		std::list<GameObject*>* FindObjectList(const int layerGroup);
		GameObject* FindObject(const int layerGroup, const wchar_t* objectTag);
		void AddObjectInLayer(const int layerGroup, GameObject* pObject);

		// SoundMgr
		void LoadSound(const char* filePath);

		// Renderer
		bool AddRenderGroup(const int renderGroup, GameObject* pObject);
		void SetSortGroup(const int sortGroup, bool(*sortFunc)(GameObject*, GameObject*));

		// EventInvoker
		EventInvoker* GetEventInvoker() const;

	private:
		void StartGame();
		void FixedUpdateGame();
		int UpdateGame();
		int LateUpdateGame();
		void RenderGame();

		// Base을(를) 통해 상속됨
		void Free() override;

	private:
		// Manager
		InputManager*		_pInputMgr{ nullptr };
		TimeManager*		_pTimeMgr{ nullptr };
		SoundManager*		_pSoundMgr{ nullptr };
		SceneManager*		_pSceneMgr{ nullptr };
		CameraManager*		_pCameraMgr{ nullptr };

		WinApp*				_pWinApp{ nullptr };
		Renderer*			_pRenderer{ nullptr };
		HWND				_hWnd{ nullptr };		
		EventInvoker*		_pEventInvoker{ nullptr };
		float				_elapsed{ 0.f };
		float				_fixed{ 0.f };
		bool				_isSceneChange{ false };
	};
}