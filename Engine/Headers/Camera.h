#pragma once
#include "Base.h"

namespace Engine
{
	class Transform;
	class Camera : public Base
	{
	public:
		explicit Camera();
	protected:
		virtual ~Camera() = default;

	public:
		Property<Transform*, READ_ONLY> transform{ _pTransform };

	public:
		const Matrix& GetViewMatrix() const { return _view; }
		const Matrix& GetViewTransposMatrix() const { return _viewTranspos; }
		const Matrix& GetProjectionMatrix() const { return _projection; }
		const Matrix& GetProjectionTransposMatrix() const { return _projectionTranspos; }

	public:
		virtual void FixedUpdate();
		virtual void Update(const float deltaTime);
		virtual void LateUpdate(const float deltaTime);

	private:
		// Base을(를) 통해 상속됨
		void Free() override;

	protected:
		Matrix _view;
		Matrix _viewTranspos;
		Matrix _projection;
		Matrix _projectionTranspos;

		Transform* _pTransform{ nullptr };
		float _fovY{ 0.f };
		float _near{ 0.f };
		float _far{ 0.f };
	};
}