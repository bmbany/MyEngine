#include "MainCamera.h"
#include "../../Engine/Headers/Engine.h"

void MainCamera::Update(const float& deltaTime)
{
	constexpr float dpi = 0.1f;
	if (_CanCameraMove)
	{
		if (Input::IsKeyPress(Input::DIM_RB))
		{
			SetCursorPos(960, 540);
			float moveX = Input::GetMouseMove(Input::DIM_X);
			float moveY = Input::GetMouseMove(Input::DIM_Y);

			if (moveX || moveY)
			{
				_pTransform->rotation += Vector3(moveY * dpi * deltaTime, moveX * dpi * deltaTime, 0.f);
			}
		}

		float speed = 100.f * deltaTime;
		float vertical = Input::GetAxis(Input::Axis::Vertical);
		float horizon = Input::GetAxis(Input::Axis::Horizontal);
		_pTransform->position += { horizon * speed, 0.f, -vertical * speed };
		
		if (Input::IsKeyPress(DIK_Q))
		{
			_pTransform->position += Vector3(0.f, 1.f, 0.f) * speed;
		}
		if (Input::IsKeyPress(DIK_E))
		{
			_pTransform->position += Vector3(0.f, -1.f, 0.f) * speed;
		}
	}
}
