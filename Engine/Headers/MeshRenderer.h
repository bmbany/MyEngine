#pragma once
#include "Component.h"

namespace Engine
{
	class Model;
	class CameraManager;
	class MeshRenderer : public Component
	{
	public:
		explicit MeshRenderer(const wchar_t* name, const wchar_t* modelFilePath);

	private:
		virtual ~MeshRenderer() = default;

	public:
		void Render() override;

	private:
		// Component을(를) 통해 상속됨
		void Free() override;

	private:
		CameraManager*			_pCameraMgr{ nullptr };
		std::shared_ptr<Model>	_model;
		ID3D11DeviceContext*	_pDeviceContext{ nullptr };
		const wchar_t*			_modelFilePath = nullptr;
	};
}