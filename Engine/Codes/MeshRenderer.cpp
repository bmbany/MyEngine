#include "MeshRenderer.h"
#include "ResourceManager.h"
#include "GraphicManager.h"
#include "CameraManager.h"
#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#include "VIBuffer.h"
#include "ConstantBuffer.h"
#include "Camera.h"

using namespace Engine;

Engine::MeshRenderer::MeshRenderer(const wchar_t* name)
	: Component(name)
	, _pCameraMgr(CameraManager::GetInstance())
{
	_pDeviceContext = g_pGraphicMgr->GetDeviceContext();	
}

void Engine::MeshRenderer::Initialize(const wchar_t* modelFilePath)
{
	_model = g_pResourceMgr->LoadResource<Model>(modelFilePath);
	assert(_model.get());

	for (auto& mesh : _model->_meshs)
	{
		mesh->_pMaterial->BindVertexShader(L"../Resources/Shader/MeshVS.cso");
		mesh->_pMaterial->BindPixelShader(L"../Resources/Shader/MeshPS.cso");
	}
}

void Engine::MeshRenderer::Render()
{
	Camera* pCurrentCamera = _pCameraMgr->GetCurrentCamera();

	WVP wvp
	{
		.view = pCurrentCamera->GetViewTransposMatrix(),
		.projection = pCurrentCamera->GetProjectionTransposMatrix(),
	};

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

void Engine::MeshRenderer::Free()
{
}
