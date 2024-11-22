#include "Camera.h"
#include "Transform.h"
#include "GraphicManager.h"
#include "InputManager.h"

using namespace Engine;

Engine::Camera::Camera()
{
	_pTransform = new Transform(L"Camera");
	_pTransform->position = Vector3(0.f, 10.f, -50.f);

	_fovY = XMConvertToRadians(45.f);
	_near = 0.01f;
	_far = 1000.f;
}

void Engine::Camera::FixedUpdate()
{
}

void Engine::Camera::Update(const float& deltaTime)
{
	const float speed = 100.f;
	const float dpi = 0.1f;

	InputManager* pInputManager = InputManager::GetInstance();	

	if (pInputManager->IsKeyPress(Input::DIM_RB))
	{
		SetCursorPos(960, 540);
		float moveX = pInputManager->GetMouseMove(Input::DIM_X);
		float moveY = pInputManager->GetMouseMove(Input::DIM_Y);

		if (moveX || moveY)
		{
			_pTransform->rotation += Vector3(moveY * dpi * deltaTime, moveX * dpi * deltaTime, 0.f);
		}
	}

	if (pInputManager->IsKeyPress(DIK_W))
	{
		_pTransform->position += -_pTransform->worldMatrix->Forward() * speed * deltaTime;
	}
	if (pInputManager->IsKeyPress(DIK_S))
	{
		_pTransform->position += -_pTransform->worldMatrix->Backward() * speed * deltaTime;
	}
	if (pInputManager->IsKeyPress(DIK_A))
	{
		_pTransform->position += _pTransform->worldMatrix->Left() * speed * deltaTime;
	}
	if (pInputManager->IsKeyPress(DIK_D))
	{
		_pTransform->position += _pTransform->worldMatrix->Right() * speed * deltaTime;
	}
	if (pInputManager->IsKeyPress(DIK_Q))
	{
		_pTransform->position += Vector3(0.f, 1.f, 0.f) * speed * deltaTime;
	}
	if (pInputManager->IsKeyPress(DIK_E))
	{
		_pTransform->position += Vector3(0.f, -1.f, 0.f) * speed * deltaTime;
	}
}

void Engine::Camera::LateUpdate(const float& deltaTime)
{
	const Matrix& world = _pTransform->worldMatrix;
	_view = XMMatrixInverse(nullptr, world);
	_viewTranspos = XMMatrixTranspose(_view);
	_projection = XMMatrixPerspectiveFovLH(_fovY, g_pGraphicMgr->GetAspectRatio(), _near, _far);
	_projectionTranspos = XMMatrixTranspose(_projection);
	_pTransform->UpdateTransform();
}

void Engine::Camera::Free()
{
	SafeRelease(_pTransform);
}