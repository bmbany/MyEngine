#pragma once
#include "Component.h"

namespace Engine
{
	class Model;
	class AnimationController;
	class CameraManager;
	class Texture;
	class SkinnedMeshRenderer : public Component
	{
	public:
		explicit SkinnedMeshRenderer(const wchar_t* name, const wchar_t* modelFilePath);

	private:
		virtual ~SkinnedMeshRenderer() = default;

	public:
		void Awake() override;
		void Render() override;

	public:
		void SetMaterial(TextureType type, const std::shared_ptr<Texture>& texture);

	private:
		// Component을(를) 통해 상속됨
		void Free() override;

	private:
		CameraManager*			_pCameraMgr{ nullptr };
		std::shared_ptr<Model>	_model;
		AnimationController*	_pAnimationController{ nullptr };
		ID3D11DeviceContext*	_pDeviceContext{ nullptr };
		const wchar_t*			_modelFilePath = nullptr;
	};
}