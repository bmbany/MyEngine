#pragma once
#include "../../Engine/Headers/Base.h"

constexpr int MAXSIZE = 128;

namespace Engine
{
	class GameObject;
}

class Hierarchy : public Engine::Base
{	
public:
	explicit Hierarchy();
	virtual ~Hierarchy() = default;

public:
	void DrawGUI();
	void Update();

private:
	void AddObject();
	void Rename();

private:
	// Base을(를) 통해 상속됨
	void Free() override;

private:
	using ObjectData = std::pair<bool, Engine::GameObject*>;
	std::unordered_map<std::string, ObjectData>	_objects;
	std::unordered_map<std::string, ObjectData>::iterator _selectedObject;
	
	bool _isRename{ false };
	char _renameBuffer[MAXSIZE]{};
};