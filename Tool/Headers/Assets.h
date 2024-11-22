#pragma once
#include "../../Engine/Headers/Base.h"

class Assets : public Engine::Base
{
public:
	explicit Assets() = default;
	virtual ~Assets() = default;

public:
	void DrawGUI();
	void Update();

private:
	std::wstring OpenFileDialog();
	void AddItem(const std::string& fileName);

private:
	// Base을(를) 통해 상속됨
	void Free() override;

private:
	std::vector<std::string> _items;
};