#pragma once
#include "Component.h"

namespace Engine
{
	class InputManager;
	class Button final : public Component
	{
		REGISTER_COMPONENT(Button);

	private:
		struct RectF
		{
			float left = 0.f;
			float top = 0.f;
			float right = 0.f;
			float bottom = 0.f;
		};
	public:
		explicit Button(const wchar_t* name);
	private:
		virtual ~Button() = default;
	
	public:
		void Update(const float& deltaTime) override;

	public:
		void SetRange(const Vector3& position, float width, float height); 
		void SetOnPressed(std::function<void()>function) { _onPressed = function; };
		void SetOnHover(std::function<void()> function) { _onHover = function; };
		void SetCancel(std::function<void()> function) { _cancel = function; };

	private:
		// Component을(를) 통해 상속됨
		void Free() override;

	private:
		std::function<void()>	_onPressed;
		std::function<void()>	_onHover;
		std::function<void()>	_cancel;
		RectF					_buttonRange;
		InputManager*			_pInputManager = nullptr;
		bool					_isHover = false;
	};
}