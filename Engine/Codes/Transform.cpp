#include "Transform.h"

using namespace Engine;

Engine::Transform::Transform(const wchar_t* name)
    : Component(name)
    , scale(_scale)
    , rotation(_rotation)
    , position(_position)
    , direction(_direction)
    , worldMatrix(_worldMatrix)
{
    _scale = { 1.f, 1.f, 1.f };
}

void Engine::Transform::UpdateTransform()
{
    XMMATRIX scale = XMMatrixScaling(_scale.x, _scale.y, _scale.z);
    XMMATRIX rotationX = XMMatrixRotationX(XMConvertToRadians(_rotation.x));
    XMMATRIX rotationY = XMMatrixRotationY(XMConvertToRadians(_rotation.y));
    XMMATRIX rotationZ = XMMatrixRotationZ(XMConvertToRadians(_rotation.z));
    XMMATRIX translation = XMMatrixTranslation(_position.x, _position.y, _position.z);

    _worldMatrix = scale * rotationX * rotationY * rotationZ * translation;

    if (nullptr != _pParent)
        _worldMatrix *= _pParent->_worldMatrix;
}

void Engine::Transform::Free()
{
}