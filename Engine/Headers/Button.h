#pragma once
#include "MonoBehavior.h"
#include "Client_Define.h"

class Button : public Engine::MonoBehavior
{
public:
	explicit Button();
private:
	virtual ~Button() = default;
public:
	void SetIsReat(bool isRepeat) { _isRepeat = isRepeat; }
	void SetRange(Vector3 position, D2D1_SIZE_F size) {_buttonRange = { position.x-(size.width*0.5f), position.y - (size.height * 0.5f),position.x + (size.width * 0.5f),position.y +( size.height * 0.5f)}; }
	void SetOnPressed(std::function<void()>function) { _onPressed = function; }; //��ư Ŭ��
	void SetOnHover(std::function<void()> function) { _onHover = function; }; //��ư ȣ��
	void SetCancel(std::function<void()> function) { _cancel = function; }; //ȣ�����
private:
	void MouseUpdate();
public:
	// MonoBehavior��(��) ���� ��ӵ�
	void Awake() override;
	void Start() override;
	void Update(const float& deltaTime) override;
	void LateUpdate(const float& deltaTime) override;
private:
	bool _isRepeat = true, _isExecute=false; //ȣ�� ����ݺ�
	std::function<void()> _onPressed = [](){};
	std::function<void()> _onHover = []() {};
	std::function<void()> _cancel = []() {};
	D2D1_RECT_F _buttonRange;
	POINT _mousePosition;
};
