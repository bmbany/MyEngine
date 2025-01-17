#pragma once
#include "../../Engine/Headers/Camera.h"

class MainCamera : public Engine::Camera
{
public:
	explicit MainCamera() = default;
private:
	virtual ~MainCamera() = default;

public:
	void SetCanMove(bool isActive) { _CanCameraMove = isActive; }

public:
	void Update(const float deltaTime) override;

private:
	bool _CanCameraMove{ false };
};