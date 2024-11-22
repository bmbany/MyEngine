#pragma once
#include "../../Engine/Headers/Scene.h"

class Inspector;
class Assets;
class Hierarchy;
class MainCamera;
class MainFrame : public Engine::Scene
{
public:
	explicit MainFrame() = default;
private:
	virtual ~MainFrame() = default;

public:
	// Scene을(를) 통해 상속됨
	int Update(const float& deltaTime) override;
	int LateUpdate(const float& deltaTime) override;

private:
	void Game();

private:
	bool Initialize() override;
	void Free() override;

private:
	std::unordered_map<std::wstring, bool> _isShowMenus;
	Inspector*	_pInpector{ nullptr };
	Assets*		_pAssets{ nullptr };
	Hierarchy*	_pHierarchy{ nullptr };
	MainCamera* _pMainCamera{ nullptr };
};