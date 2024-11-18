#include "SkinnedMeshRenderer.h"
#include "ResourceManager.h"
#include "GraphicManager.h"
#include "CameraManager.h"
#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#include "VIBuffer.h"
#include "AnimationController.h"
#include "ConstantBuffer.h"
#include "Camera.h"

using namespace Engine;

Engine::SkinnedMeshRenderer::SkinnedMeshRenderer(const wchar_t* name, const wchar_t* modelFilePath)
	: Component(name)
	, _pCameraMgr(CameraManager::GetInstance())
{
	_pDeviceContext = g_pGraphicMgr->GetDeviceContext();
	_model = g_pResourceMgr->LoadResource<Model>(modelFilePath);
	assert(_model.get());

	for (auto& mesh : _model->_meshs)
	{
		mesh->_pMaterial->BindVertexShader(L"../Resources/Shader/MeshVS_Skinned.cso");
		mesh->_pMaterial->BindPixelShader(L"../Resources/Shader/MeshPS.cso");
	}

	_modelFilePath = modelFilePath;
}

void Engine::SkinnedMeshRenderer::Awake()
{
	_pAnimationController = AddComponent<AnimationController>(L"AnimationController", _modelFilePath);
	_pAnimationController->_pSkeleton = _model->_pSkeleton;
}

void Engine::SkinnedMeshRenderer::Render()
{
	const auto& animationTransform = _pAnimationController->GetAnimationTransform();

	Camera* pCurrentCamera = _pCameraMgr->GetCurrentCamera();

	WVP wvp
	{
		.view = pCurrentCamera->GetViewTransposMatrix(),
		.projection = pCurrentCamera->GetProjectionTransposMatrix(),
	};

	g_pConstantBuffer->UpdateConstantBuffer(L"MatrixPallete", animationTransform.data());

	for (auto& mesh : _model->_meshs)
	{
		wvp.world = transform->worldMatrix->Transpose();
		wvp.WVP = wvp.projection * wvp.view * wvp.world;
		g_pConstantBuffer->UpdateConstantBuffer(L"WVP", &wvp);

		mesh->_pVIBuffer->SetParameters(_pDeviceContext);
		mesh->_pMaterial->SetParameters();

		_pDeviceContext->DrawIndexed(mesh->_pVIBuffer->_indexCount, 0, 0);
	}
}

void Engine::SkinnedMeshRenderer::SetMaterial(TextureType type, const std::shared_ptr<Texture>& texture)
{
	for (auto& mesh : _model->_meshs)
	{
		mesh->_pMaterial->BindTextureMap(type, texture);
	}
}

void Engine::SkinnedMeshRenderer::Free()
{
}
