#pragma once
#include "Component.h"

namespace Engine
{
    class Transform : public Component
    {        
    public:
        explicit Transform(const wchar_t* name);
    private:
        virtual ~Transform() = default;

    public:
        Property<Vector3>   position    { _position };
        Property<Vector3>   direction   { _direction };
        Property<Vector3>   rotation    { _rotation };
        Property<Vector3>   scale       { _scale };
        Property<Matrix>    worldMatrix { _worldMatrix };

    public:
        void SetParent(Transform* pParent) { _pParent = pParent; }
        void UpdateTransform();

    private:
        // Component을(를) 통해 상속됨
        void Free() override;

    private:
        Matrix      _worldMatrix;
        Vector3     _scale;
        Vector3     _rotation;
        Vector3     _position;
        Vector3     _direction;
        Transform*  _pParent = nullptr;
    };
}