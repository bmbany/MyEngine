#include "Transform.h"

using namespace Engine;

Engine::Transform::Transform(const wchar_t* name)
    : Component(name)
{
    _scale = { 1.f, 1.f, 1.f };
}

void Engine::Transform::UpdateTransform()
{
    XMMATRIX scale = XMMatrixScaling(_scale.x, _scale.y, _scale.z);
    XMMATRIX rotation = XMMatrixRotationQuaternion(Quaternion::CreateFromYawPitchRoll(_rotation));
    XMMATRIX translation = XMMatrixTranslation(_position.x, _position.y, _position.z);

    _worldMatrix = scale * rotation * translation;

    if (nullptr != _pParent)
        _worldMatrix *= _pParent->_worldMatrix;
}

void Engine::Transform::Free()
{
}