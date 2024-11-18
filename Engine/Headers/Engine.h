#pragma once
#include "Export_Headers.h"

namespace Engine
{
	inline void ClearLayer(const int layerGroup);
	inline void ClearObjectList(const int layerGroup);
	inline std::list<GameObject*>* FindObjectList(const int layerGroup);
	inline GameObject* FindObject(const int layerGroup, const wchar_t* objectTag);
	inline void AddObjectInLayer(const int layerGroup, GameObject* pObject);
	inline HWND GetWindow();
	inline void ChangeScene(Engine::Scene* pScene);
	#include "Export.inl"
}

namespace GameObject
{
	template<typename T, typename... Args>
	inline T* Instantiate(const Vector3& position, const Vector3& rotation, Args&&... args)
	{
		T* pInstance = new T(std::forward<Args>(args)...);
		pInstance->transform->position = position;
		pInstance->transform->rotation = rotation;

		return pInstance;
	}
	#include "Export_Object.inl"
}

namespace Camera
{
	inline Engine::Camera* GetCurrentCamera();
	inline void SetCurrentCamera(const wchar_t* name);
	inline void AddCamera(const wchar_t* name, Engine::Camera* pCamera);
	#include "Export_Camera.inl"
}

namespace Resource
{
	#include "Export_Resource.inl"
}

namespace Time
{
	inline float GetGlobalDeltaTime();
	inline void SetSlowTime(float rate);
	inline void SetSlowTime(float rate, float duration);
	inline float GetDeltaTime();
	#include "Export_Time.inl"
}

namespace Input
{
	inline bool IsKeyDown(_byte keycord);
	inline bool IsKeyDown(Input::MouseState mouseState);
	inline bool IsKeyDown(Input::PadState padState);
	inline bool IsKeyUp(_byte keycord);
	inline bool IsKeyUp(Input::MouseState mouseState);
	inline bool IsKeyUp(Input::PadState padState);
	inline bool IsKeyPress(_byte keycord);
	inline bool IsKeyPress(Input::MouseState mouseState);
	inline bool IsKeyPress(Input::PadState padState);
	inline float GetAxis(Input::Axis type);
	inline float GetMouseMove(Input::MouseMove mouseMove);
	inline const Vector3& GetMousePosition();
	inline bool IsMouseWheel(Input::MouseState mouseState);
	inline void SetThumbDeadZone(short left, short right);
	inline void SetTriggerThreshold(byte value);
	inline void SetVibration(float power);
	#include "Export_Input.inl"
}

namespace Sound
{
	inline FMOD::Channel* PlaySound(const char* soundTag, int channelID, float volume, bool isLoop);
	inline void DistancePlaySound(const Vector3& position, const char* soundTag, int groupID, bool isLoop);
	inline void SetFadeVolume(int groupID, float volume);
	inline void SetVolume(int groupID, float volume);
	inline void StopSound(int groupID);
	inline void SetSoundTarget(Engine::GameObject* pTarget);
	inline void PauseSound(int groupID, bool isPause);
	#include "Export_Sound.inl"
}