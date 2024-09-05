#include "Button.h"
#include "InputManager.h"

using namespace Engine;

Button::Button(const wchar_t* name)
	: Component(name),
	_pInputManager(InputManager::GetInstance())
{
}

void Button::Update(const float& deltaTime)
{
	const Vector3& mousePosition = _pInputManager->GetMousePosition();

	if (mousePosition.x >= _buttonRange.left &&
		mousePosition.x <= _buttonRange.right &&
		mousePosition.y >= _buttonRange.top &&
		mousePosition.y <= _buttonRange.bottom)
	{
		if (_pInputManager->IsKeyDown(Input::DIM_LB))
		{
			_onPressed();
		}
		else
		{
			if (!_isHover)
			{
				_onHover();
				_isHover = true;
			}
		}
	}
	else 
	{
		_cancel();
		_isHover = false;
	}
	
}

void Engine::Button::SetRange(const Vector3& position, float width, float height)
{
	_buttonRange = {
		position.x - width * 0.5f,
		position.y - height * 0.5f,
		position.x + width * 0.5f,
		position.y + height * 0.5f };
}
