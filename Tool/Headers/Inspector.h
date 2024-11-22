#pragma once
#include "../../Engine/Headers/Base.h"

class Inspector : public Engine::Base
{
public:
	explicit Inspector() = default;
	virtual ~Inspector() = default;

public:
	void DrawGUI();
	void Update();

private:
	// Base��(��) ���� ��ӵ�
	void Free() override;
};