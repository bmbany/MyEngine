#pragma once
#include "Component.h"

namespace Engine
{
	class Model;
	class CameraManager;
	class MeshRenderer : public Component
	{
		REGISTER_COMPONENT(MeshRenderer);

	public:
		explicit MeshRenderer(const wchar_t* name);

	private:
		virtual ~MeshRenderer() = default;

	public:
		void Initialize(const wchar_t* modelFilePath);
		void Render() override;

	private:
		// Component을(를) 통해 상속됨
		void Free() override;

	private:
		CameraManager*			_pCameraMgr{ nullptr };
		std::shared_ptr<Model>	_model;
		ID3D11DeviceContext*	_pDeviceContext{ nullptr };
	};
}