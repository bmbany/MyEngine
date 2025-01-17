#pragma once
#include "Base.h"

namespace Engine
{
	class Camera;
	class CameraManager : public Base, public Singleton<CameraManager>
	{
		friend class Singleton;
	private:
		explicit CameraManager() = default;
		virtual ~CameraManager() = default;

	public:
		void FixedUpdate();
		void Update(const float deltaTime);
		void LateUpdate(const float deltaTime);

	public:
		Camera* GetCurrentCamera() const { return _pCurrentCamera; }
		void SetCurrentCamera(const wchar_t* name);

	public:
		void AddCamera(const wchar_t* name, Camera* pCamera);

	private:
		// Base을(를) 통해 상속됨
		void Free() override;

	private:
		std::unordered_map<std::wstring, Camera*> _cameras;
		Camera* _pCurrentCamera{ nullptr };
	};
}